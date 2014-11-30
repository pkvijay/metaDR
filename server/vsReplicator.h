#ifndef _VSREPLICATOR_H
#define _VSREPLICATOR_H

#include <thread>
#include <mutex>

#include "vrCommon.h"
#include "replicator.h"
#include "vsReplication.hh"
#include "vsReplicaState.h"
#include "opLogHandler.h"
#include "concurrentQueue.h"
#include "viewChangeHandler.h"
#include "vsReplicationClient.h"
#include "include/clientWrapper.h"

namespace vr {

class VsReplicator : public kvs::IReplicator {
public:
   VsReplicator(ReplicaId id,
                std::unordered_map<ReplicaId, EndPoint>& replicas,
                IOpLogHandler* logHandler);
   ~VsReplicator();

   void replicateUpdate(const std::string& key, const std::string& val);
   void replicateDelete(const std::string& key);
   bool isPrimary() { return _id == _replicaState.priId(); }

   void registerUpdateCb(kvs::UpdateCbFunc cb);
   void registerDeleteCb(kvs::DeleteCbFunc cb);

   void processPrepare(std::unique_ptr<PrepInfo> prepInfo);
   void processPrepareOk(std::unique_ptr<PrepResponse> prepResponse);
   void processCommit(std::unique_ptr<CommitInfo> commitInfo);

   void processStartViewChange(
      std::unique_ptr<ViewChangeRequest> viewChangeReq) {
      _viewChangeHandler.processStartViewChange(std::move(viewChangeReq));
   }

   void processDoViewChange(std::unique_ptr<ViewChangeInfo> viewChangeInfo) {
      _viewChangeHandler.processDoViewChange(std::move(viewChangeInfo));
   }

   void processStartView(std::unique_ptr<ViewStartInfo> viewStartInfo) {
      _viewChangeHandler.processStartView(std::move(viewStartInfo));
   }

private:
   void replicate(const PrepInfo& prepInfo);
   void invokePrepare(const EndPoint& ep, const PrepInfo& prepInfo);
   void invokePrepareOk(const EndPoint& ep, const PrepResponse& prepResponse);
   void invokeCommit(const EndPoint& ep, const CommitInfo& commitInfo);
   void heartBeat();
   void processPrepInfo();

   using RepClientWrapper = ClientWrapper<VsReplicationClient>;
   std::shared_ptr<RepClientWrapper> getRepClient(const EndPoint& ep);

   const ReplicaId _id;
   std::unordered_map<ReplicaId, EndPoint>& _replicas;
   IOpLogHandler* _logHandler;
   ReplicaState _replicaState;
   std::recursive_mutex _repMutex;
   ViewChangeHandler _viewChangeHandler;
   std::thread _hbt; // heart-beat thread
   std::thread _ppit; // process prepInfo thread
   CQueue<PrepInfo> _prepReqQ;
   WaitMap<OpInt> _waitMap;
   std::unordered_map<std::string,
                      std::shared_ptr<RepClientWrapper>> _clWrappers;
   std::recursive_mutex _repClientMutex;
};

} //namespace vr

#endif
