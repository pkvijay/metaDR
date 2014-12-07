#include "recoveryHandler.h"

#include <iostream>

using namespace std;

namespace vr {

#define RECOVERY_TIMEOUT 8000 //ms

RecoveryHandler::RecoveryHandler(ReplicaId id,
                                 unordered_map<ReplicaId, EndPoint>& replicas,
                                 IOpLogHandler* logHandler,
                                 ReplicaState& repState,
                                 recursive_mutex& repMutex)
                                    : _replicaId(id), _replicas(replicas),
                                       _logHandler(logHandler),
                                       _replicaState(repState),
                                       _repMutex(repMutex), _updateCb(nullptr),
                                       _deleteCb(nullptr)
{
   thread prrt(&RecoveryHandler::processRecoveryReqInt, this);
   _prrt = move(prrt);
}

RecoveryHandler::~RecoveryHandler()
{
   _prrt.join();
}


void
RecoveryHandler::recover()
{
   lock_guard<recursive_mutex> lg(_repMutex);
   cout << "Recover called by replicaId : " << _replicaId << endl;
   _replicaState.status(RECOVERING);
   //TODO: Use checkpoint module to recover to the state before crashing.

   RecoveryRequest recoveryReq;
   recoveryReq.replicaId = _replicaId;
   auto now = chrono::system_clock::now();
   recoveryReq.nonce =
      chrono::duration_cast<chrono::seconds>(now.time_since_epoch()).count();
   cout << "nonce : " << recoveryReq.nonce << endl;
   recoveryReq.knownCommitNum = 0;
   invokeForAllReplicas(_replicaId, this,
                        &RecoveryHandler::invokeRecoveryRequest,
                        recoveryReq);
   const shared_ptr<Response>& resp = _waitMap.get(recoveryReq.nonce);
   {
      unique_lock<recursive_mutex> lg(resp->rMutex);
      if (!responseDone(recoveryReq.nonce)) {
         cv_status status =
            resp->cond.wait_for(lg,
                                chrono::milliseconds(RECOVERY_TIMEOUT));
         if (status == cv_status::timeout) {
            throw runtime_error("Recovery timed out");
         }
      }
   }
   {
      lock_guard<recursive_mutex> lg(_recoveryRespMutex);
      ViewInt highestViewNum = 0;
      for (auto it = _respViewCnt.begin(); it != _respViewCnt.end(); ++it) {
         if (it->second >= getMajorityCnt()) {
            highestViewNum = it->first;
            cout << "Highest view num : " << highestViewNum << endl;
            break;
         }
      }
      const auto& recoveryRespVec = _recoveryRespMap[recoveryReq.nonce];
      for (const auto& recoveryResp : recoveryRespVec) {
         cout << "Replica : " << recoveryResp->replicaId << ", view num : "
              << recoveryResp->viewNum << endl;
         if (recoveryResp->viewNum == highestViewNum &&
             recoveryResp->isPrimary) {
            recoverStateFromLog(recoveryResp->opLog);
            _replicaState.priId(recoveryResp->replicaId);
            _replicaState.viewNum(recoveryResp->viewNum);
            _replicaState.opNum(recoveryResp->opNum);
            _replicaState.commitNum(recoveryResp->commitNum);
            _logHandler->setLog(recoveryResp->opLog);
            _replicaState.lastCommitTime(chrono::system_clock::now());
            _replicaState.status(NORMAL);
            break;
         }
         else {
            assert(highestViewNum >= recoveryResp->viewNum);
         }
      }
   }
   assert(_replicaState.status() == NORMAL);
   cleanup();
}

void
RecoveryHandler::processRecoveryRequest(unique_ptr<RecoveryRequest> recoveryReq)
{
   cout << "processRecoveryRequest called from replica "
        << recoveryReq->replicaId << endl;
   if (_replicaState.status() != NORMAL) {
      cout << "Ignoring the recoveryRequest since the replica state is not "
               "NORMAL" << endl;
      return;
   }
   _recoveryReqQ.push(*(recoveryReq.get()));
}

void
RecoveryHandler::processRecoveryResponse(
      unique_ptr<RecoveryResponse> recoveryResp)
{
   cout << "processRecoveryResponse called from replica "
        << recoveryResp->replicaId << ", for nonce : "
        << recoveryResp->nonce << ", for view : "
        << recoveryResp->viewNum << endl;
   if (_replicaState.status() != RECOVERING) {
      cout << "Ignoring recoveryResponse from replicaId, "
           << recoveryResp->replicaId << ", since the replica state "
           "is not recovering" << endl;
      return;
   }
   Nonce nonce = recoveryResp->nonce;
   {
      lock_guard<recursive_mutex> lg(_recoveryRespMutex);
      if (recoveryResp->isPrimary) {
         cout << "Received response from primary" << endl;
         _receivedRespFromPri = true;
      }
      ++_respViewCnt[recoveryResp->viewNum];
      _recoveryRespMap[nonce].push_back(move(recoveryResp));
      if (_recoveryRespMap[nonce].size() >= getMajorityCnt()) {
         cout << "Reached quorum for recovery response" << endl;
      }
   }
   shared_ptr<Response> resp = _waitMap.get(nonce);
   bool notify = false;
   {
      lock_guard<recursive_mutex> lg(resp->rMutex);
      ++resp->cnt;
      notify = responseDone(nonce);
      cout << "recoveryResponse Resp cnt : " << resp->cnt << endl;
   }
   if (notify) {
      resp->cond.notify_one();
   }
}

void
RecoveryHandler::invokeRecoveryRequest(const EndPoint& ep,
                                       const RecoveryRequest& recoveryReq)
{
   lock_guard<recursive_mutex> lg(_recoveryClientMutex);
   shared_ptr<RecoveryClientWrapper> cw;
   unsigned tryCnt = 0;
   while (++tryCnt <= 2 ) {
      try {
         cw = getRecoveryClient(ep);
         cw->client.recoveryRequest(recoveryReq);
         break;
      }
      catch (const exception& ex) {
         if(cw.get() != nullptr) {
            cw->client.close();
         }
         cerr << "recoveryRequest to endpoint, " << ep
              << " failed with exception, " << ex.what() << endl;
      }
   }
}

void
RecoveryHandler::invokeRecoveryResponse(const EndPoint& ep,
                                        const RecoveryResponse& recoveryResp)
{
   lock_guard<recursive_mutex> lg(_recoveryClientMutex);
   shared_ptr<RecoveryClientWrapper> cw;
   unsigned tryCnt = 0;
   while (++tryCnt <= 2) {
      try {
         cw = getRecoveryClient(ep);
         cw->client.recoveryResponse(recoveryResp);
         break;
      }
      catch (const exception& ex) {
         if(cw.get() != nullptr) {
            cw->client.close();
         }
         cerr << "recoveryResponse to endpoint, " << ep
              << " failed with exception, " << ex.what() << endl;
      }
   }
}

shared_ptr<RecoveryHandler::RecoveryClientWrapper>
RecoveryHandler::getRecoveryClient(const EndPoint& ep)
{
   lock_guard<recursive_mutex> lg(_recoveryClientMutex);
   shared_ptr<RecoveryClientWrapper>& cw = _clWrappers[ep];
   if (cw.get() == nullptr || !cw->client.isConnected()) {
      cw = shared_ptr<RecoveryClientWrapper>(new RecoveryClientWrapper(ep));
   }
   return cw;
}

bool
RecoveryHandler::responseDone(Nonce nonce)
{
   shared_ptr<Response> resp = _waitMap.get(nonce);
   bool done = _receivedRespFromPri && quorumReached(resp);
   if (!done) {
      return false;
   }
   lock_guard<recursive_mutex> lg(_recoveryRespMutex);
   for (auto it = _respViewCnt.begin(); it != _respViewCnt.end(); ++it) {
      if (it->second >= getMajorityCnt()) {
         return true;
      }
   }
   return false;
}

void
RecoveryHandler::recoverStateFromLog(const OpLog& opLog)
{
   // TODO assert replicator is locked
   for (const LogEntry& entry : opLog) {
      if (entry.committed) {
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
}

void
RecoveryHandler::cleanup()
{
   _waitMap.clear();
   _clWrappers.clear();
   _recoveryRespMap.clear();
   _respViewCnt.clear();
   _receivedRespFromPri = false;
}

void
RecoveryHandler::processRecoveryReqInt()
{
   cout << "processRecoveryReq thread called for replica Id : " << _replicaId
        << ", thread id : " << this_thread::get_id() << endl;
   while (true) {
      const RecoveryRequest& recoveryReq = _recoveryReqQ.pop();
      if (_replicaState.status() != NORMAL) {
         cout << "Ignoring the recoveryRequest since the replica state is not "
               "NORMAL" << endl;
         return;
      }
      RecoveryResponse recoveryResp;
      {
         lock_guard<recursive_mutex> lg(_repMutex);
         recoveryResp.viewNum = _replicaState.viewNum();
         recoveryResp.nonce = recoveryReq.nonce;
         recoveryResp.replicaId = _replicaId;
         if (_replicaState.priId() == _replicaId) {
            recoveryResp.isPrimary = true;
            // TODO use recoveryReq.knownCommitNum to send only the required log
            recoveryResp.opLog = _logHandler->getLog();
            recoveryResp.opNum = _replicaState.opNum();
            recoveryResp.commitNum = _replicaState.commitNum();
         }
      }
      invokeRecoveryResponse(_replicas[recoveryReq.replicaId], recoveryResp);
   }
}


} //namespace vr
