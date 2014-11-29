#ifndef _VSREPLICATIONCLIENT_H
#define _VSREPLICATIONCLIENT_H

#include <string>

#include "include/clientBase.h"
#include "vsReplication.hh"

namespace vr {

class VsReplicationClient : public ClientBase<ReplicationApi_v1> {
public:
   void prepare(const PrepInfo& prepInfo);
   void prepareOk(const PrepResponse& prepResponse);
   void commit(const CommitInfo& commitInfo);
};

} //namespace vr

#endif
