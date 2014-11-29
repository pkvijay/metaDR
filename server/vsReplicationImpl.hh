// -*- C++ -*-
// Scaffolding originally generated from server/vsReplication.x.
// Edit to add functionality.

#ifndef __XDR_SERVER_VSREPLICATIONIMPL_HH_INCLUDED__
#define __XDR_SERVER_VSREPLICATIONIMPL_HH_INCLUDED__ 1

#include "server/vsReplication.hh"

namespace vr {

class ReplicationApi_v1_server {
public:
   using rpc_interface_type = ReplicationApi_v1;

   ReplicationApi_v1_server() {}
   void prepare(std::unique_ptr<PrepInfo> arg);
   void prepareOk(std::unique_ptr<PrepResponse> arg);
   void commit(std::unique_ptr<CommitInfo> arg);
};

}

#endif // !__XDR_SERVER_VSREPLICATIONIMPL_HH_INCLUDED__
