#include "vrCommon.h"
#include "configurator.h"

using namespace std;

namespace vr {

const unordered_map<ReplicaId, EndPoint>&
getAllReplicas()
{
   Configurator* configurator = Configurator::getInstance();
   return configurator->getReplicas();
}

size_t
getMajorityCnt() {
   return (getAllReplicas().size() + 1) / 2 + 1;
}

bool
quorumReached(const shared_ptr<Response>& resp)
{
   size_t majority = getMajorityCnt();
   lock_guard<recursive_mutex> lg(resp->rMutex);
   return (resp->cnt >= majority);
}

} //namespace vr
