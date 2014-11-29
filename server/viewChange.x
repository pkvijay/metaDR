% #include "vrTypes.hh"

namespace vr {

   struct ViewChangeRequest {
      ViewInt viewNum;
      ReplicaId replicaId;
   };

   struct ViewChangeInfo {
      ViewInt viewNum;
      OpLog opLog;
      ViewInt oldViewNum;
      OpInt opNum;
      CommitInt commitNum;
      ReplicaId replicaId;
   };

   struct ViewStartInfo {
      ViewInt viewNum;
      OpLog opLog;
      OpInt opNum;
      CommitInt commitNum;
      ReplicaId replicaId;
   };

   program ViewChangeApi {
      version ViewChangeApi_v1 {
         void startViewChange(ViewChangeRequest) = 1;
         void doViewChange(ViewChangeInfo) = 2;
         void startView(ViewStartInfo) = 3;
      } = 1;
   } = 0x40048100;
}
