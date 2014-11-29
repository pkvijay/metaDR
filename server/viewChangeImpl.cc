// Scaffolding originally generated from server/viewChange.x.
// Edit to add functionality.

#include "server/viewChangeImpl.hh"
#include "configurator.h"
#include "vsReplicator.h"

using namespace std;

namespace vr {

void
ViewChangeApi_v1_server::startViewChange(std::unique_ptr<ViewChangeRequest> arg)
{
   Configurator* configurator = Configurator::getInstance();
   VsReplicator* replicator = configurator->getReplicator();
   replicator->processStartViewChange(move(arg));

}

void
ViewChangeApi_v1_server::doViewChange(std::unique_ptr<ViewChangeInfo> arg)
{
   Configurator* configurator = Configurator::getInstance();
   VsReplicator* replicator = configurator->getReplicator();
   replicator->processDoViewChange(move(arg));
}

void
ViewChangeApi_v1_server::startView(std::unique_ptr<ViewStartInfo> arg)
{
   Configurator* configurator = Configurator::getInstance();
   VsReplicator* replicator = configurator->getReplicator();
   replicator->processStartView(move(arg));
}

}
