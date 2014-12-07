#include "recoveryClient.h"

using namespace std;

namespace vr {

void
RecoveryClient::recoveryRequest(const RecoveryRequest& recoveryReq)
{
   _srpcClient->recoveryRequest(recoveryReq);
}

void
RecoveryClient::recoveryResponse(const RecoveryResponse& recoveryResp)
{
   _srpcClient->recoveryResponse(recoveryResp);
}

} //namespace vr
