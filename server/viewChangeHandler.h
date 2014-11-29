#ifndef _VIEWCHANGEHANDLER_H
#define _VIEWCHANGEHANDLER_H

#include "viewChange.hh"
#include "vsReplicaState.h"
#include "include/endPoint.h"
#include "opLogHandler.h"
#include "vrCommon.h"
#include "viewChangeClient.h"
#include "include/clientWrapper.h"

#include <unordered_map>

namespace vr {

class ViewChangeHandler {
public:
   ViewChangeHandler(ReplicaId id,
                     std::unordered_map<ReplicaId, EndPoint>& replicas,
                     IOpLogHandler* logHandler, ReplicaState& repState)
                        : _replicaId(id), _replicas(replicas),
                          _logHandler(logHandler), _replicaState(repState) {}

   void changeView();

   void processStartViewChange(
      std::unique_ptr<ViewChangeRequest> viewChangeReq);

   void processDoViewChange(std::unique_ptr<ViewChangeInfo> viewChangeinfo);
   void processStartView(std::unique_ptr<ViewStartInfo> viewStartInfo);

private:
   void updateNewView(ViewInt viewNum);

   void invokeStartViewChange(const EndPoint& ep,
                              const ViewChangeRequest& viewChangeReq);

   bool invokeDoViewChange(const EndPoint& ep,
                           const ViewChangeInfo& viewChangeInfo);

   void invokeStartView(const EndPoint& ep, const ViewStartInfo& viewStartInfo);

   ReplicaId getNextPrimaryCandidate(ReplicaId currCandidate);

   using ViewChangeClWrapper = ClientWrapper<ViewChangeClient>;
   std::shared_ptr<ViewChangeClWrapper> getViewChangeClient(const EndPoint& ep);

   ReplicaId _replicaId;
   std::unordered_map<ReplicaId, EndPoint>& _replicas;
   IOpLogHandler* _logHandler;
   ReplicaState& _replicaState;
   WaitMap<ViewInt> _viewStartReqMap;
   WaitMap<ViewInt> _doViewChangeMap;
   std::unordered_map<std::string,
                      std::shared_ptr<ViewChangeClWrapper>> _clWrappers;
   std::recursive_mutex _vcClientMutex;
   std::recursive_mutex _viewChangeMutex;

   //TODO use unique_ptr<ViewChangeInfo> in the vector
   std::unordered_map<ViewInt, std::vector<ViewChangeInfo>> _viewChangeInfoMap;
};

} //namespace vr

#endif
