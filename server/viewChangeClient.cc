#include "viewChangeClient.h"

using namespace std;

namespace vr {

void
ViewChangeClient::startViewChange(const ViewChangeRequest& viewChangeReq)
{
   _srpcClient->startViewChange(viewChangeReq);
}

void
ViewChangeClient::doViewChange(const ViewChangeInfo& viewChangeInfo)
{
   _srpcClient->doViewChange(viewChangeInfo);
}

void
ViewChangeClient::startView(const ViewStartInfo& viewStartInfo)
{
   _srpcClient->startView(viewStartInfo);
}

} //namespace vr
