// -*- C++ -*-
// Scaffolding originally generated from server/viewChange.x.
// Edit to add functionality.

#ifndef __XDR_SERVER_VIEWCHANGEIMPL_HH_INCLUDED__
#define __XDR_SERVER_VIEWCHANGEIMPL_HH_INCLUDED__ 1

#include "server/viewChange.hh"

namespace vr {

class ViewChangeApi_v1_server {
public:
  using rpc_interface_type = ViewChangeApi_v1;

  void startViewChange(std::unique_ptr<ViewChangeRequest> arg);
  void doViewChange(std::unique_ptr<ViewChangeInfo> arg);
  void startView(std::unique_ptr<ViewStartInfo> arg);
};

}

#endif // !__XDR_SERVER_VIEWCHANGEIMPL_HH_INCLUDED__