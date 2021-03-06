#ifndef _VIEWCHANGEHANDLER_H
#define _VIEWCHANGEHANDLER_H

#include "viewChange.hh"
#include "vsReplicaState.h"
#include "include/endPoint.h"
#include "opLogHandler.h"
#include "vrCommon.h"
#include "viewChangeClient.h"
#include "include/clientWrapper.h"
#include "include/kvsCallback.h"

#include <unordered_map>

namespace vr {

class ViewChangeHandler {
public:
   ViewChangeHandler(ReplicaId id,
                     std::unordered_map<ReplicaId, EndPoint>& replicas,
                     IOpLogHandler* logHandler, ReplicaState& repState,
                     std::recursive_mutex& repMutex)
                        : _replicaId(id), _replicas(replicas),
                          _logHandler(logHandler), _replicaState(repState),
                          _repMutex(repMutex), _updateCb(nullptr),
                          _deleteCb(nullptr) {}

   void changeView();

   void processStartViewChange(
      std::unique_ptr<ViewChangeRequest> viewChangeReq);

   void processDoViewChange(std::unique_ptr<ViewChangeInfo> viewChangeinfo);
   void processStartView(std::unique_ptr<ViewStartInfo> viewStartInfo);

   void registerUpdateCb(kvs::UpdateCbFunc cb) { _updateCb = cb; }
   void registerDeleteCb(kvs::DeleteCbFunc cb) { _deleteCb = cb; }

private:
   void updateNewView(ViewInt viewNum, bool isPrimary);

   void invokeStartViewChange(const EndPoint& ep,
                              const ViewChangeRequest& viewChangeReq);

   bool invokeDoViewChange(const EndPoint& ep,
                           const ViewChangeInfo& viewChangeInfo);

   void invokeStartView(const EndPoint& ep, const ViewStartInfo& viewStartInfo);

   ReplicaId getNextPrimaryCandidate(ReplicaId currCandidate);

   using ViewChangeClWrapper = ClientWrapper<ViewChangeClient>;
   std::shared_ptr<ViewChangeClWrapper> getViewChangeClient(const EndPoint& ep);

   void cleanUp();

   ReplicaId _replicaId;
   std::unordered_map<ReplicaId, EndPoint>& _replicas;
   IOpLogHandler* _logHandler;
   ReplicaState& _replicaState;
   std::recursive_mutex& _repMutex;
   kvs::UpdateCbFunc _updateCb;
   kvs::DeleteCbFunc _deleteCb;
   WaitMap<ViewInt> _viewStartReqMap;
   WaitMap<ViewInt> _doViewChangeMap;
   std::unordered_map<std::string,
                      std::shared_ptr<ViewChangeClWrapper>> _clWrappers;
   std::recursive_mutex _vcClientMutex;
   std::recursive_mutex _viewChangeMutex;

   //TODO use unique_ptr<ViewChangeInfo> in the vector
   std::unordered_map<ViewInt, std::vector<ViewChangeInfo>> _viewChangeInfoMap;

   WaitMap<ViewInt> _startViewMap;
   std::unordered_map<ViewInt,
                      std::unique_ptr<ViewStartInfo>> _viewStartInfoMap;

};

} //namespace vr

#endif
