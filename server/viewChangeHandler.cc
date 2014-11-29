#include "viewChangeHandler.h"

#include <iostream>

using namespace std;

namespace vr {

#define VIEW_START_REQ_TIMEOUT 4000 // in ms
#define VIEW_CHANGE_COMPLETION_TIMEOUT 8000 // in ms

/*
 * Caller need to make sure that the Replicator is locked down
 * throughout the view change process.
 * Replicator should not process any requests until the status changes to
 * normal.
 */
void
ViewChangeHandler::changeView()
{
   cout << "changeView called by replicaId : " << _replicaId << endl;
   _replicaState.status(VIEW_CHANGE);
   ViewInt oldViewNum = _replicaState.viewNum();
   ViewInt newViewNum = _replicaState.incrementViewNum();
   ViewChangeRequest viewChangeReq;
   viewChangeReq.viewNum = newViewNum;
   viewChangeReq.replicaId = _replicaId;

   invokeForAllReplicas(_replicaId, this,
                        &ViewChangeHandler::invokeStartViewChange,
                        viewChangeReq);

   shared_ptr<Response> resp = _viewStartReqMap.get(newViewNum);
   {
      unique_lock<recursive_mutex> lg(resp->rMutex);
      ++resp->cnt;
      if (!quorumReached(resp)) {
         cv_status status =
            resp->cond.wait_for(lg,
                                chrono::milliseconds(VIEW_START_REQ_TIMEOUT));
         if (status == cv_status::timeout) {
            cerr << "Timed out to reach quorum for viewNum, " << newViewNum
                 << endl;
            return;
         }
      }
   }
   cout << "Reached quorum to start view change to viewNum " << newViewNum
        << endl;

   ViewChangeInfo viewChangeInfo;
   viewChangeInfo.viewNum = newViewNum;
   viewChangeInfo.opLog = _logHandler->getLog();
   viewChangeInfo.oldViewNum = oldViewNum;
   /*
    * Below opNum is being set to the current commitNum.
    * TODO: Try to re-do the uncommitted operations in the new view.
    */
   //viewChangeInfo.opNum = _replicaState.opNum();
   viewChangeInfo.opNum = _replicaState.commitNum();

   viewChangeInfo.commitNum = _replicaState.commitNum();
   viewChangeInfo.replicaId = _replicaId;
   {
      lock_guard<recursive_mutex> lg(_viewChangeMutex);
      _viewChangeInfoMap[newViewNum].push_back(viewChangeInfo);
   }
   shared_ptr<Response> viewChangeResp =
      _doViewChangeMap.get(newViewNum);
   {
      lock_guard<recursive_mutex> lg(viewChangeResp->rMutex);
      ++viewChangeResp->cnt;
   }
   unsigned cnt = 0;
   ReplicaId newPriId = getNextPrimaryCandidate(_replicaState.priId());
   while (newPriId != _replicaId && 
          !invokeDoViewChange(_replicas[newPriId], viewChangeInfo)) {
      if (++cnt == _replicas.size() - 1) {
         cerr << "Failed to invoke DoViewChange to any replica" << endl;
         break;
      }
      newPriId = getNextPrimaryCandidate(newPriId);
   }

   {
      unique_lock<recursive_mutex> lg(viewChangeResp->rMutex);
      if (!quorumReached(viewChangeResp)) {
         cout << "Resp cnt : " << viewChangeResp->cnt << endl;
         cv_status status =
            viewChangeResp->cond.wait_for(lg,
               chrono::milliseconds(VIEW_CHANGE_COMPLETION_TIMEOUT));
         if (status == cv_status::timeout) {
            cerr << "Timed out for view change to complete to new vieNum,  "
                 << newViewNum << endl;
            return;
         } 
      }
      if (newPriId == _replicaId && quorumReached(viewChangeResp)) {
         updateNewView(newViewNum);
      }
   }

   // Cleanup 
   {
      lock_guard<recursive_mutex> lg(_viewChangeMutex);
      if (_replicaState.status() == NORMAL) {
         _viewStartReqMap.clear();
         _doViewChangeMap.clear();
         _viewChangeInfoMap.clear();
      }
   }
}

void
ViewChangeHandler::processStartViewChange(
   unique_ptr<ViewChangeRequest> viewChangeReq)
{
   cout << "viewChangeRequest from " << viewChangeReq->replicaId
        << " for viewNum " << viewChangeReq->viewNum << endl;
   if (_replicaState.viewNum() > viewChangeReq->viewNum) {
      return;
   }
   shared_ptr<Response> resp =
      _viewStartReqMap.get(viewChangeReq->viewNum);
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
ViewChangeHandler::processDoViewChange(
   unique_ptr<ViewChangeInfo> viewChangeInfo)
{
   cout << "processDoViewChange called from replica, "
        << viewChangeInfo->replicaId << ", for new viewNum, "
        << viewChangeInfo->viewNum << endl;
   if (_replicaState.viewNum() > viewChangeInfo->viewNum) {
      return;
   }
   {
      lock_guard<recursive_mutex> lg(_viewChangeMutex);
      vector<ViewChangeInfo>& vcInfos =
         _viewChangeInfoMap[viewChangeInfo->viewNum];
      vcInfos.push_back(*viewChangeInfo);
      if (vcInfos.size() >= getMajorityCnt()) {
         cout << "Reached quorum doViewChange for new viewNum : "
              << viewChangeInfo->viewNum << endl;
      }
   }
   shared_ptr<Response> resp =
      _doViewChangeMap.get(viewChangeInfo->viewNum);
   bool notify = false;
   {
      lock_guard<recursive_mutex> lg(resp->rMutex);
      ++resp->cnt;
      notify = quorumReached(resp);
      cout << "DoViewChange Resp cnt : " << resp->cnt << endl;
   }
   if (notify) {
      resp->cond.notify_one();
   }
}

void
ViewChangeHandler::processStartView(unique_ptr<ViewStartInfo> viewStartInfo)
{
   cout << "processStartView called for viewNum : " << viewStartInfo->viewNum
        << endl;
   if (_replicaState.viewNum() > viewStartInfo->viewNum) {
      return; 
   }
   if (_replicaState.status() != VIEW_CHANGE) {
      cout << "Replica state is not VIEW_CHANGE. Ignoring the viewStartInfo "
           << "for viewNum : " << viewStartInfo->viewNum << endl;
   }
   //TODO: assert replicator is locked down
   _replicaState.priId(viewStartInfo->replicaId);
   _replicaState.viewNum(viewStartInfo->viewNum);
   _replicaState.opNum(viewStartInfo->opNum);
   _replicaState.commitNum(viewStartInfo->commitNum);
   _logHandler->setLog(viewStartInfo->opLog);
   _replicaState.lastCommitTime(chrono::system_clock::now());
   /*
    * status should be set to normal, after all the above is done, to avoid
    * the new primary's heart-beat thread to send wrong commitInfo details.
    */
   _replicaState.status(NORMAL);

   shared_ptr<Response> resp =
         _doViewChangeMap.get(viewStartInfo->viewNum);
   bool notify = false;
   {
      lock_guard<recursive_mutex> lg(resp->rMutex);
      ++resp->cnt;
      notify = quorumReached(resp);

   }
   if (notify) {
      resp->cond.notify_one();
   }
   cout << "Replica state changed to normal. New view num : "
        << _replicaState.viewNum() << ", new pri replicaId : "
        << _replicaState.priId() << endl;
}

void
ViewChangeHandler::updateNewView(ViewInt viewNum)
{
   lock_guard<recursive_mutex> lg(_viewChangeMutex);
   vector<ViewChangeInfo>& vcInfos =
      _viewChangeInfoMap[viewNum];
   assert(vcInfos.size() >= getMajorityCnt());
   size_t candidateViewChangeIdx = 0;
   ViewInt highestOldViewNum = vcInfos.at(0).oldViewNum;
   ViewInt highestOpNum = vcInfos.at(0).opNum;
   for (size_t i = 1; i < vcInfos.size(); ++i) {
      ViewInt oldViewNum = vcInfos.at(i).oldViewNum;
      OpInt opNum = vcInfos.at(i).opNum;
      if (oldViewNum == highestOldViewNum) {
         if (opNum > highestOpNum) {
            candidateViewChangeIdx = i;
            highestOpNum = opNum;
         }
      }
      else if (oldViewNum > highestOldViewNum) {
         candidateViewChangeIdx = i;
         highestOldViewNum = oldViewNum;
      }
   }
   const ViewChangeInfo& candidateVcInfo =
      vcInfos.at(candidateViewChangeIdx);
   _replicaState.priId(_replicaId);
   _replicaState.viewNum(candidateVcInfo.viewNum);
   _replicaState.opNum(candidateVcInfo.opNum);
   _replicaState.commitNum(candidateVcInfo.commitNum);
   _logHandler->setLog(candidateVcInfo.opLog);
   _replicaState.lastCommitTime(chrono::system_clock::now());
   /*
    * status should be set to normal, after all the above is done, to avoid
    * the new primary's heart-beat thread to send wrong commitInfo details.
    */
   _replicaState.status(NORMAL);

   cout << "Replica state changed to normal. New view num : "
        << _replicaState.viewNum() << ", new pri replicaId : "
        << _replicaState.priId() << endl;

   ViewStartInfo viewStartInfo;
   viewStartInfo.viewNum = candidateVcInfo.viewNum;
   viewStartInfo.opLog = _logHandler->getLog();
   viewStartInfo.opNum = candidateVcInfo.opNum;
   viewStartInfo.commitNum = candidateVcInfo.commitNum;
   viewStartInfo.replicaId = _replicaId;
   invokeForAllReplicas(_replicaId, this,
                        &ViewChangeHandler::invokeStartView,
                        viewStartInfo);
}

void
ViewChangeHandler::invokeStartViewChange(const EndPoint& ep,
                                         const ViewChangeRequest& viewChangeReq)
{
   lock_guard<recursive_mutex> lg(_vcClientMutex);
   shared_ptr<ViewChangeClWrapper> cw;
   try {
      cw = getViewChangeClient(ep);
      cw->client.startViewChange(viewChangeReq);
   }
   catch (const exception& ex) {
      if (cw.get() != nullptr) {
         cw->client.close();
      }
      cerr << "startViewChange to endpoint, " << ep
           << ", failed with exception, " << ex.what() << endl;
   }
}

bool
ViewChangeHandler::invokeDoViewChange(const EndPoint& ep,
                                      const ViewChangeInfo& viewChangeInfo)
{
   lock_guard<recursive_mutex> lg(_vcClientMutex);
   shared_ptr<ViewChangeClWrapper> cw;
   bool success = false;
   try {
      cw = getViewChangeClient(ep);
      cw->client.doViewChange(viewChangeInfo);
      success = true;
   }
   catch (const exception& ex) {
      if (cw.get() != nullptr) {
         cw->client.close();
      }
      cerr << "doViewChange to endpoint, " << ep
           << ", failed with exception, " << ex.what() << endl;
   }
   return success;
}

void
ViewChangeHandler::invokeStartView(const EndPoint& ep,
                                   const ViewStartInfo& viewStartInfo)
{
   lock_guard<recursive_mutex> lg(_vcClientMutex);
   shared_ptr<ViewChangeClWrapper> cw;
   try {
      cw = getViewChangeClient(ep);
      cw->client.startView(viewStartInfo);
   }
   catch (const exception& ex) {
      if (cw.get() != nullptr) {
         cw->client.close();
      }
      cerr << "startView to endpoint, " << ep
           << ", failed with exception, " << ex.what() << endl;
   }
}

ReplicaId
ViewChangeHandler::getNextPrimaryCandidate(ReplicaId currCandidate)
{
   ReplicaId newPriId = currCandidate + 1;
   if (newPriId == _replicas.size()) {
      newPriId = 0;
   }
   return newPriId;
}

shared_ptr<ViewChangeHandler::ViewChangeClWrapper>
ViewChangeHandler::getViewChangeClient(const EndPoint& ep)
{
   lock_guard<recursive_mutex> lg(_vcClientMutex);
   shared_ptr<ViewChangeClWrapper>& cw = _clWrappers[ep];
   if (cw.get() == nullptr || !cw->client.isConnected()) {
      cw = shared_ptr<ViewChangeClWrapper>(new ViewChangeClWrapper(ep));
   }
   return cw;
}

} //namespace vr
