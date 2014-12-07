#ifndef _RECOVERYCLIENT_H
#define _RECOVERYCLIENT_H

#include "include/clientBase.h"
#include "recovery.hh"

namespace vr {

class RecoveryClient : public ClientBase<RecoveryApi_v1> {
public:
   void recoveryRequest(const RecoveryRequest& recoveryReq);
   void recoveryResponse(const RecoveryResponse& recoveryResp);
};

} //namespace vr


#endif
