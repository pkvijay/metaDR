#ifndef _KVMEMSTORE_H
#define _KVMEMSTORE_H

#include <string>
#include <set>
#include <unordered_map>
#include <assert.h>

#include "replicator.h"

namespace kvs {

class KvMemStore {
public:
   static KvMemStore* getInstance();
   static void deleteInstance();
   bool hasKey(const std::string& key);
   std::string get(const std::string& key);
   bool set(const std::string& key, const std::string& val);
   void remove(const std::string& key);
   std::set<std::string> list(const std::string& prefix,
                              const char ignoreInSuffix);
   void registerReplicator(IReplicator* replicator);
   bool isPrimary() { return _replicator->isPrimary(); }

private:
   KvMemStore() : _replicator(nullptr) {}
   ~KvMemStore();
   bool setInt(const std::string& key, const std::string& val);
   void removeInt(const std::string& val);
   static void updateCb(const std::string& key, const std::string& val);
   static void deleteCb(const std::string& key);

   static KvMemStore* instance;
   std::unordered_map<std::string, std::string> kvMap;
   IReplicator* _replicator;
};

} //namespace kvs

#endif
