#include "kvMemStore.h"

#include <mutex>
#include <iostream>

using namespace std;

namespace kvs {

KvMemStore* KvMemStore::instance = nullptr;

static recursive_mutex kvMemMutex;

KvMemStore*
KvMemStore::getInstance() {
   lock_guard<recursive_mutex> lg(kvMemMutex);
   if (instance == nullptr) {
      instance = new KvMemStore();
   }
   return instance;
}

void
KvMemStore::deleteInstance() {
   lock_guard<recursive_mutex> lg(kvMemMutex);
   if (instance != nullptr) {
      delete instance;
   }
}

KvMemStore::~KvMemStore()
{
   if (_replicator != nullptr) {
      delete _replicator;
   }
}

bool
KvMemStore::hasKey(const string& key) {
   lock_guard<recursive_mutex> lg(kvMemMutex);
   return (kvMap.find(key) != kvMap.end());
}

std::string
KvMemStore::get(const string& key) {
   lock_guard<recursive_mutex> lg(kvMemMutex);
   auto it = kvMap.find(key);
   if (it == kvMap.end()) {
      const string errMsg = "Key " + key + " not found";
      cerr << errMsg << endl;
      throw runtime_error(errMsg);
   }
   return it->second;
}

bool
KvMemStore::set(const string& key, const string& val) {
   lock_guard<recursive_mutex> lg(kvMemMutex);
   assert(_replicator->isPrimary());
   _replicator->replicateUpdate(key, val);
   return setInt(key, val);
}

void
KvMemStore::remove(const string& key) {
   lock_guard<recursive_mutex> lg(kvMemMutex);
   auto it = kvMap.find(key);
   if (it == kvMap.end()) {
      const string errMsg = "Key " + key + " not present";
      cerr << errMsg << endl;
      throw runtime_error(errMsg);
   }
   assert(_replicator->isPrimary()); 
   _replicator->replicateDelete(key);
   cout << "Removing " << key << endl;
   kvMap.erase(it);
}

set<string>
KvMemStore::list(const string& prefix, const char ignoreInSuffix) {
   lock_guard<recursive_mutex> lg(kvMemMutex);
   std::set<string> res;
   for (auto it = kvMap.cbegin(); it != kvMap.cend(); ++it) {
      const string& key = it->first;
      if (key == prefix) {
         continue;
      }
      size_t pos = key.find(prefix);
      if (pos != string::npos) {
         pos += prefix.size();
         if (key.find(ignoreInSuffix, pos + 1) == string::npos) {
            res.insert(key);
         }
      }
   }
   return res;
}

void
KvMemStore::registerReplicator(IReplicator* replicator)
{
   lock_guard<recursive_mutex> lg(kvMemMutex);
   // Can register replicator only once
   assert(_replicator == nullptr);
   _replicator = replicator;
   _replicator->registerUpdateCb(&KvMemStore::updateCb);
   _replicator->registerDeleteCb(&KvMemStore::deleteCb);
}

bool
KvMemStore::setInt(const string& key, const string& val)
{
   lock_guard<recursive_mutex> lg(kvMemMutex);
   bool update = hasKey(key);
   cout << "Setting, " << key << " = " << val << endl;
   kvMap[key] = val;
   return update;
}

void
KvMemStore::removeInt(const string& key)
{
   lock_guard<recursive_mutex> lg(kvMemMutex);
   auto it = kvMap.find(key);
   if (it != kvMap.end()) {
      cout << "Removing " << key << endl;
      kvMap.erase(it);
   }
}

void
KvMemStore::updateCb(const string& key, const string& val)
{
   instance->setInt(key, val);
}

void
KvMemStore::deleteCb(const string& key)
{
   instance->removeInt(key);
}


} //namespace kvs
