#ifndef _RECOVERYHANDLER_H
#define _RECOVERYHANDLER_H

#include "recovery.hh"
#include "vsReplicaState.h"
#include "include/endPoint.h"
#include "opLogHandler.h"
#include "vrCommon.h"
#include "include/clientWrapper.h"
#include "include/kvsCallback.h"
#include "recoveryClient.h"
#include "concurrentQueue.h"

#include <unordered_map>
#include <atomic>

namespace vr {

class RecoveryHandler {
public:
   RecoveryHandler(ReplicaId id,
                   std::unordered_map<ReplicaId, EndPoint>& replicas,
                   IOpLogHandler* logHandler, ReplicaState& repState,
                   std::recursive_mutex& repMutex);
   ~RecoveryHandler();

   void recover();

   void processRecoveryRequest(std::unique_ptr<RecoveryRequest> recoveryReq);
   void processRecoveryResponse(std::unique_ptr<RecoveryResponse> recoveryResp);

   void registerUpdateCb(kvs::UpdateCbFunc cb) { _updateCb = cb; }
   void registerDeleteCb(kvs::DeleteCbFunc cb) { _deleteCb = cb; }

private:
   void invokeRecoveryRequest(const EndPoint& ep,
                              const RecoveryRequest& recoveryReq);
   void invokeRecoveryResponse(const EndPoint& ep,
                               const RecoveryResponse& recoveryResp);

   using RecoveryClientWrapper = ClientWrapper<RecoveryClient>;
   std::shared_ptr<RecoveryClientWrapper> getRecoveryClient(const EndPoint& ep);

   bool responseDone(Nonce nonce);
   void recoverStateFromLog(const OpLog& opLog);
   void cleanup();
   void processRecoveryReqInt();

   ReplicaId _replicaId;
   std::unordered_map<ReplicaId, EndPoint>& _replicas;
   IOpLogHandler* _logHandler;
   ReplicaState& _replicaState;
   std::recursive_mutex& _repMutex;
   kvs::UpdateCbFunc _updateCb;
   kvs::DeleteCbFunc _deleteCb;
   std::thread _prrt; // process recovery req thread
   CQueue<RecoveryRequest> _recoveryReqQ;
   WaitMap<Nonce> _waitMap;
   std::recursive_mutex _recoveryClientMutex;
   std::unordered_map<std::string,
                      std::shared_ptr<RecoveryClientWrapper>> _clWrappers;
   std::recursive_mutex _recoveryRespMutex;
   std::unordered_map<Nonce, std::vector<std::unique_ptr<RecoveryResponse>>>
      _recoveryRespMap;
   std::unordered_map<ViewInt, unsigned> _respViewCnt;
   std::atomic<bool> _receivedRespFromPri;
};

} //namespace vr


#endif
