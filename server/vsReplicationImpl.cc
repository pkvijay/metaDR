// Scaffolding originally generated from server/vsReplication.x.
// Edit to add functionality.

#include <iostream>

#include "vsReplicationImpl.hh"
#include "configurator.h"
#include "vsReplicator.h"

using namespace std;
using namespace kvs;

namespace vr {

void
ReplicationApi_v1_server::prepare(std::unique_ptr<PrepInfo> arg)
{
   Configurator* configurator = Configurator::getInstance();
   VsReplicator* replicator = configurator->getReplicator();
   replicator->processPrepare(move(arg));
}

void
ReplicationApi_v1_server::prepareOk(std::unique_ptr<PrepResponse> arg)
{
   Configurator* configurator = Configurator::getInstance();
   VsReplicator* replicator = configurator->getReplicator();
   replicator->processPrepareOk(move(arg));
}

void
ReplicationApi_v1_server::commit(std::unique_ptr<CommitInfo> arg)
{
   Configurator* configurator = Configurator::getInstance();
   VsReplicator* replicator = configurator->getReplicator();
   replicator->processCommit(move(arg));
}

}
