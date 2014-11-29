/*
 * Below includes are to include the types under kvs in kvStore.x and types
 * under vr in vrTypes.x
 */

% #include "include/kvStore.hh"
% #include "vrTypes.hh"

namespace vr { // vr => viewstamped replication

   struct PrepInfo {
      ViewInt viewNum;
      kvs::KvPair kvPair;
      OpInt opNum;
      OpType opType;
      CommitInt commitNum;
   };

   struct CommitInfo {
      ViewInt viewNum;
      CommitInt commitNum;
   };

   struct PrepResponse {
      ViewInt viewNum;
      OpInt opNum;
      ReplicaId replicaId;
   };

   program ReplicationApi {
      version ReplicationApi_v1 {
         void prepare(PrepInfo) = 1;
         void prepareOk(PrepResponse) = 2;
         void commit(CommitInfo) = 3;
      } = 1;
   } = 0x40048088;
}
