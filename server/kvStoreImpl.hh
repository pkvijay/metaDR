// -*- C++ -*-
// Scaffolding originally generated from include/kvStore.x.
// Edit to add functionality.

#ifndef __XDR_SERVER_KVSTOREIMPL_HH_INCLUDED__
#define __XDR_SERVER_KVSTOREIMPL_HH_INCLUDED__

#include "include/kvStore.hh"
#include "kvMemStore.h"

namespace kvs {

class KvStoreApi_v1_server {
public:
   using rpc_interface_type = KvStoreApi_v1;

   KvStoreApi_v1_server();
   ~KvStoreApi_v1_server() {}
   std::unique_ptr<Result> create(std::unique_ptr<KvPair> arg);
   std::unique_ptr<Result> remove(std::unique_ptr<LongString> arg);
   std::unique_ptr<Result> set(std::unique_ptr<KvPair> arg);
   std::unique_ptr<Result> get(std::unique_ptr<LongString> arg);
   std::unique_ptr<Result> list(std::unique_ptr<LongString> arg);

private:
   KvMemStore* kvStore;
};

}

#endif // !__XDR_SERVER_KVSTOREIMPL_HH_INCLUDED__
