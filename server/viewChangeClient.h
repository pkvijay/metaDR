#ifndef _VIEWCHANGECLIENT_H
#define _VIEWCHANGECLIENT_H

#include "include/clientBase.h"
#include "viewChange.hh"

namespace vr {

class ViewChangeClient : public ClientBase<ViewChangeApi_v1> {
public:
   void startViewChange(const ViewChangeRequest& viewChangeReq);
   void doViewChange(const ViewChangeInfo& viewChangeInfo);
   void startView(const ViewStartInfo& viewStartInfo);
};

} //namespace vr

#endif
