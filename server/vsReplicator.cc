#include <iostream>

#include "vsReplicator.h"

using namespace kvs;
using namespace std;

namespace vr {

#define HEART_BEAT_SLEEP 200 // in ms
#define PREP_RESPONSE_TIMEOUT 2000 // in ms
#define HEART_BEAT_TIMEOUT 3 // in sec

VsReplicator::VsReplicator(ReplicaId id,
                           unordered_map<ReplicaId, EndPoint>& replicas,
                           IOpLogHandler* logHandler)
                              : _id(id), _replicas(replicas),
                                _logHandler(logHandler), _replicaState(),
                                _viewChangeHandler(_id, _replicas, _logHandler,
                                                   _replicaState)
{
    thread hbt(&VsReplicator::heartBeat, this);
   _hbt = move(hbt);
   thread ppit(&VsReplicator::processPrepInfo, this);
   _ppit = move(ppit);
}

VsReplicator::~VsReplicator()
{
   _hbt.join();
   _ppit.join();
   delete _logHandler;
}

void
VsReplicator::replicateUpdate(const string& key, const string& val) {
   lock_guard<recursive_mutex> lg(_repMutex);
   cout << "replicateUpdate called " << endl;
   assert(isPrimary());
   KvPair kvPair;
   kvPair.key = key;
   kvPair.val = val;

   PrepInfo prepInfo;
   prepInfo.viewNum = _replicaState.viewNum();
   prepInfo.kvPair = move(kvPair);
   prepInfo.opNum = _replicaState.incrementOpNum();
   prepInfo.opType = UPDATE;
   prepInfo.commitNum = _replicaState.commitNum();
   replicate(prepInfo);
}

void
VsReplicator::replicateDelete(const string& key)
{
   lock_guard<recursive_mutex> lg(_repMutex);
   cout << "replicateDelete called" << endl;
   assert(isPrimary());
   KvPair kvPair;
   kvPair.key = key;

   PrepInfo prepInfo;
   prepInfo.viewNum = _replicaState.viewNum();
   prepInfo.kvPair = move(kvPair);
   prepInfo.opNum = _replicaState.incrementOpNum();
   prepInfo.opType = DELETE;
   prepInfo.commitNum = _replicaState.commitNum();
   replicate(prepInfo);
}

void
VsReplicator::replicate(const PrepInfo& prepInfo)
{
   //TODO assert _repMutex is locked
   if (_replicaState.status() != NORMAL) {
      //TODO: propogate error to client
      throw runtime_error("status is NOT normal to replicate");
   }
   if (prepInfo.opType == UPDATE) {
      _logHandler->appendUpdate(prepInfo.kvPair, prepInfo.opNum);
   }
   else {
      assert(prepInfo.opType == DELETE);
      _logHandler->appendDelete(prepInfo.kvPair.key, prepInfo.opNum);
   }

   const shared_ptr<Response>& resp = _waitMap.get(prepInfo.opNum);
   invokeForAllReplicas(_id, this, &VsReplicator::invokePrepare, prepInfo);
   {
      unique_lock<recursive_mutex> lg(resp->rMutex);
      ++resp->cnt;
      if (!quorumReached(resp)) {
         cv_status status =
            resp->cond.wait_for(lg,
                                chrono::milliseconds(PREP_RESPONSE_TIMEOUT));
         if (status == cv_status::timeout) {
            cerr << "TimedOut to reach quorum for opNum, " << prepInfo.opNum
                 << endl;
            // TODO: Propogate the error to the client
            throw runtime_error("Failed to reach quorum");
         }
      }
   }
   _replicaState.commitNum(prepInfo.opNum);
   cout << "Committed opNum : " << prepInfo.opNum << endl;
   CommitInfo commitInfo;
   commitInfo.viewNum = prepInfo.viewNum;
   commitInfo.commitNum = prepInfo.opNum;
   invokeForAllReplicas(_id, this, &VsReplicator::invokeCommit,
                        commitInfo);
   /*
    *TODO: Handle the rollover logic, in case quorum not reached.
    *      Clean up the _waitMap once the response is processed.
    */
}

void
VsReplicator::processPrepare(unique_ptr<PrepInfo> prepInfo)
{
   cout << "ProcessPrepare called for PrepInfo ("
        << prepInfo->kvPair.key << ", " << prepInfo->kvPair.val << ")" << endl;
   if (_replicaState.status() != NORMAL) {
      cerr << "Replica status not NORMAL, ignoring prepare req" << endl;
      return;
   }
   if (prepInfo->viewNum != _replicaState.viewNum()) {
      //TODO: if prepInfo->viewNum > _replicaState.viewNum do state transfer
      cerr << "prepInfo->viewNum : " << prepInfo->viewNum
           << ", is not same as _replicaState.viewNum : "
           << _replicaState.viewNum()  << ". Ignoring prepInfo." << endl;
      return;
   }
   _prepReqQ.push(*(prepInfo.get()));
   cout << "Pushed prepInfo for opNum : " << prepInfo->opNum << endl;
}

void
VsReplicator::processPrepareOk(unique_ptr<PrepResponse> prepResponse)
{
   cout << "ProcessPrepareOk called for PrepResponse->opNum, "
        << prepResponse->opNum << endl;
   if (_replicaState.status() != NORMAL) {
      cerr << "Replica state is not NORMAL. Ignoring prepResponse" << endl;
      return;
   }
   if (prepResponse->viewNum != _replicaState.viewNum()) {
      //TODO: if prepResponse->viewNum > _replicaState.viewNum do state transfer
      cerr << "prepResponse->viewNum : " << prepResponse->viewNum
           << ", is not same as _replicaState.viewNum : "
           << _replicaState.viewNum()  << ". Ignoring prepResponse." << endl;
      return;
   }
   shared_ptr<Response> resp = _waitMap.get(prepResponse->opNum);
   if (quorumReached(resp)) {
      return;
   }
   bool notify = false;
   {
      lock_guard<recursive_mutex> lg(resp->rMutex);
      ++resp->cnt;
      notify = quorumReached(resp);
   }
   if (notify) {
      resp->cond.notify_one();
   }
}

void
VsReplicator::processCommit(unique_ptr<CommitInfo> commitInfo)
{
   lock_guard<recursive_mutex> lg(_repMutex);
   cout << "ProcessCommit called for commitInfo->commitNum : "
        << commitInfo->commitNum << endl;
   _replicaState.lastCommitTime(chrono::system_clock::now());
   if (_replicaState.status() != NORMAL) {
      cerr << "Replica state is not NORMAL. Ignoring commitInfo" << endl;
      return;
   }
   if (commitInfo->viewNum != _replicaState.viewNum()) {
      //TODO: if commitInfo->viewNum > _replicaState.viewNum do state transfer
      cerr << "commitInfo->viewNum : " << commitInfo->viewNum
           << ", is not same as _replicaState.viewNum : "
           << _replicaState.viewNum()  << ". Ignoring commit." << endl;
      return;
   }
   if (commitInfo->commitNum > _replicaState.commitNum()) {
      _replicaState.commitNum(commitInfo->commitNum);
      const LogEntry& entry = _logHandler->getLogEntry(commitInfo->commitNum);
      if (entry.opType == UPDATE) {
         assert(_updateCb != nullptr);
         _updateCb(entry.kvPair.key, entry.kvPair.val);
      }
      else {
         assert(entry.opType == DELETE);
         assert(_deleteCb != nullptr);
         _deleteCb(entry.kvPair.key);
      }
   }
}

bool
VsReplicator::isPrimary()
{
   return (_id == _replicaState.priId());
}

void
VsReplicator::invokePrepare(const EndPoint& ep, const PrepInfo& prepInfo)
{
   lock_guard<recursive_mutex> lg(_repClientMutex);
   shared_ptr<RepClientWrapper> cw;
   try {
      cw = getRepClient(ep);
      cw->client.prepare(prepInfo);
   }
   catch (const exception& ex) {
      if (cw.get() != nullptr) {
         cw->client.close();
      }
      cerr << "Prepare to endpoint, " << ep
           << " failed with exception, " << ex.what() << endl;
   }
}

void
VsReplicator::invokePrepareOk(const EndPoint& ep,
                              const PrepResponse& prepResponse)
{
   lock_guard<recursive_mutex> lg(_repClientMutex);
   shared_ptr<RepClientWrapper> cw;
   try {
      cw = getRepClient(ep);
      cw->client.prepareOk(prepResponse);
   }
   catch (const exception& ex) {
      if(cw.get() != nullptr) {
         cw->client.close();
      }
      cerr << "PrepareOk to endpoint, " << ep
           << " failed with exception, " << ex.what() << endl;
   }
}

void
VsReplicator::invokeCommit(const EndPoint& ep, const CommitInfo& commitInfo)
{
   lock_guard<recursive_mutex> lg(_repClientMutex);
   shared_ptr<RepClientWrapper> cw;
   try {
      cw = getRepClient(ep);
      cw->client.commit(commitInfo);
   }
   catch (const exception& ex) {
      if (cw.get() != nullptr) {
         cw->client.close();
      }
      cerr << "Commit to endpoint, " << ep
           << " failed with exception, " << ex.what() << endl;
   }
}

void
VsReplicator::heartBeat()
{
   cout << "heartBeat thread called for replica Id : " << _id
        << ", thread Id : " << this_thread::get_id() << endl;
   while (true) {
      if (isPrimary() && _replicaState.status() == NORMAL) {
         CommitInfo commitInfo;
         commitInfo.viewNum = _replicaState.viewNum();
         commitInfo.commitNum = _replicaState.commitNum();
         invokeForAllReplicas(_id, this, &VsReplicator::invokeCommit,
                              commitInfo);
      }
      else {
         chrono::duration<double> elapsedSeconds =
            chrono::system_clock::now() - _replicaState.lastCommitTime();
         if (elapsedSeconds.count() > HEART_BEAT_TIMEOUT) {
            cerr << "Heartbeat timed out : " << elapsedSeconds.count()
                 << " seconds" << endl;
            lock_guard<recursive_mutex> lg(_repMutex);
            _viewChangeHandler.changeView();
         }
      }
      chrono::milliseconds dura(HEART_BEAT_SLEEP);
      this_thread::sleep_for(dura);
   }
}

void
VsReplicator::processPrepInfo()
{
   if (isPrimary()) {
      return;
   }
   cout << "processPrepInfo thread called for replica Id : " << _id
        << ", thread id : " << this_thread::get_id() << endl;
   while (true) {
      const PrepInfo& prepInfo = _prepReqQ.pop();
      if (_replicaState.status() != NORMAL) {
         cerr << "Replica state is not NORAML. Igoring preInfo processing"
              << endl;
         continue;
      }
      if (prepInfo.viewNum != _replicaState.viewNum()) {
         //TODO: if prepInfo.viewNum > _replicaState.viewNum do state transfer
         cerr << "In processPrepInfo, prepInfo.viewNum : " << prepInfo.viewNum
              << ", is not same as _replicaState.viewNum : "
              << _replicaState.viewNum()  << ". Ignoring prepInfo." << endl;
         continue;
      }

      lock_guard<recursive_mutex> lg(_repMutex);
      if (prepInfo.opType == UPDATE) {
         _logHandler->appendUpdate(prepInfo.kvPair, prepInfo.opNum);
      }
      else {
         assert(prepInfo.opType == DELETE);
         _logHandler->appendDelete(prepInfo.kvPair.key, prepInfo.opNum);
      }
      PrepResponse prepResponse;
      prepResponse.viewNum = _replicaState.viewNum();
      prepResponse.opNum = prepInfo.opNum;
      prepResponse.replicaId = _id;
      const EndPoint& priEp = _replicas[_replicaState.priId()];
      invokePrepareOk(priEp, prepResponse);
      // TODO: commit the commitNum from prepInfo
   }
}

shared_ptr<VsReplicator::RepClientWrapper>
VsReplicator::getRepClient(const EndPoint& ep)
{
   lock_guard<recursive_mutex> lg(_repClientMutex);
   shared_ptr<RepClientWrapper>& cw = _clWrappers[ep];
   if (cw.get() == nullptr || !cw->client.isConnected()) {
      cw = shared_ptr<RepClientWrapper>(new RepClientWrapper(ep));
   }
   return cw;
}

} //namespace vr
