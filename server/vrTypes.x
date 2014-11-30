% #include "include/kvStore.hh"

namespace vr {
   typedef unsigned hyper ViewInt;
   typedef unsigned hyper OpInt;
   typedef unsigned hyper ReplicaId;
   typedef unsigned hyper CommitInt;

   enum Status {
      NORMAL,
      VIEW_CHANGE,
      RECOVERING
   };

   enum OpType {
      UPDATE,
      DELETE
   };

   struct LogEntry {
      kvs::KvPair kvPair;
      OpType opType;
      OpInt opNum;
      bool committed;
   };

   typedef LogEntry OpLog<>;
}
