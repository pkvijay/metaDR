// Scaffolding originally generated from server/recovery.x.
// Edit to add functionality.

#include "server/recoveryImpl.hh"
#include "configurator.h"
#include "vsReplicator.h"

using namespace std;

namespace vr {

void
RecoveryApi_v1_server::recoveryRequest(std::unique_ptr<RecoveryRequest> arg)
{
   Configurator* configurator = Configurator::getInstance();
   VsReplicator* replicator = configurator->getReplicator();
   replicator->processRecoveryRequest(move(arg));
}

void
RecoveryApi_v1_server::recoveryResponse(std::unique_ptr<RecoveryResponse> arg)
{
   Configurator* configurator = Configurator::getInstance();
   VsReplicator* replicator = configurator->getReplicator();
   replicator->processRecoveryResponse(move(arg));
}

}
