// -*- C++ -*-
// Scaffolding originally generated from server/recovery.x.
// Edit to add functionality.

#ifndef __XDR_SERVER_RECOVERYIMPL_HH_INCLUDED__
#define __XDR_SERVER_RECOVERYIMPL_HH_INCLUDED__ 1

#include "server/recovery.hh"

namespace vr {

class RecoveryApi_v1_server {
public:
  using rpc_interface_type = RecoveryApi_v1;

  void recoveryRequest(std::unique_ptr<RecoveryRequest> arg);
  void recoveryResponse(std::unique_ptr<RecoveryResponse> arg);
};

}

#endif // !__XDR_SERVER_RECOVERYIMPL_HH_INCLUDED__