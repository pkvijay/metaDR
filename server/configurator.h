#ifndef _CONFIGURATOR_H
#define _CONFIGURATOR_H

#include <string>
#include <vector>
#include <unordered_map>

#include "include/endPoint.h"
#include "vrTypes.hh"
#include "vsReplicator.h"

namespace vr {

class Configurator {
public:
   static Configurator* getInstance();
   static void deleteInstance();

   void bootStrap(const std::string& cfgFile);

   ReplicaId getReplicaId() const;
   const std::string& getClientPort() const;
   const std::string& getReplicationPort() const;
   const std::unordered_map<ReplicaId, EndPoint>& getReplicas() const;
   const EndPoint& getReplicaEndPoint(ReplicaId id) const;
   VsReplicator* getReplicator();

private:
   Configurator();
   ~Configurator() {}

   static Configurator* _instance;

   ReplicaId _replicaId;
   VsReplicator* _replicator;
   std::string _clientPort;

   // port number where this replica instance should run
   std::string _replicationPort;

   std::unordered_map<ReplicaId, EndPoint> _replicas;
};


} //namespace vr

#endif
