#include "vsReplicationClient.h"

using namespace std;
using namespace kvs;

namespace vr {

void
VsReplicationClient::prepare(const PrepInfo& prepInfo)
{
   _srpcClient->prepare(prepInfo);
}

void
VsReplicationClient::prepareOk(const PrepResponse& prepResponse)
{
   _srpcClient->prepareOk(prepResponse);
}

void
VsReplicationClient::commit(const CommitInfo& commitInfo)
{
   _srpcClient->commit(commitInfo);
}

} //namespace vr
