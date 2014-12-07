% #include "vrTypes.hh"

namespace vr {

   typedef unsigned hyper Nonce;

   struct RecoveryRequest {
      ReplicaId replicaId;
      Nonce nonce;
      CommitInt knownCommitNum;
   };

   struct RecoveryResponse {
      ViewInt viewNum;
      Nonce nonce;
      ReplicaId replicaId;
      bool isPrimary;
      OpLog opLog;
      OpInt opNum;
      CommitInt commitNum;
   };

   program RecoveryApi {
      version RecoveryApi_v1 {
         void recoveryRequest(RecoveryRequest) = 1;
         void recoveryResponse(RecoveryResponse) = 2;
      } = 1;
   } = 0x40048102;
}
