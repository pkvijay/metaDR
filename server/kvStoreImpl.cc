// Scaffolding originally generated from include/kvStore.x.
// Edit to add functionality.

#include <iostream>
#include <assert.h>
#include <string>
#include <set>

#include "server/kvStoreImpl.hh"
#include "replicaException.h"

using namespace std;

namespace kvs {

static size_t
pruneKey(string& key) //IN/OUT
{
   assert(key.at(0) == '/');
   size_t lastSlashIndex = 0;
   for (size_t i = 1; i < key.size(); ++i) {
      if (key.at(i) == '/') {
            if (lastSlashIndex == i - 1) {
            key.erase(i, 1);
            --i;
            continue;
         }
         lastSlashIndex = i;
         continue;
      }
   }
   return lastSlashIndex;
}

static Error
checkKeyFormat(const string& key)
{
   if (key.empty() || key.at(0) != '/') {
      cerr << "Key, " << key << ", does not begin with /" << endl;
      return KEY_MALFORMED;
   }
   if (key.size() == 1) {
      cerr << "Key, " << key << ", is not valid" << endl;
      return KEY_MALFORMED;
   }
   for (size_t i = 1; i < key.size(); ++i) {
      if (i == key.size() - 1 && key.at(i) == '/') {
         cerr << "Key, " << key << ", cannot end with /" << endl;
         return KEY_MALFORMED;
      }
      unsigned keyInt = static_cast<unsigned>(key.at(i));
      if (!((keyInt >= 47 && keyInt <= 57) ||  // slash, 0 - 9
            (keyInt >= 65 && keyInt <= 90) ||  // A - Z
            (keyInt >= 97 && keyInt <= 122) || // a - z
            keyInt == 95)) {                   // _
         cerr << "Key, " << key << ", can only have numbers, chars, "
                      "underscore and /" << endl;
         return KEY_MALFORMED;
      }
   }
   return NONE;
}

KvStoreApi_v1_server::KvStoreApi_v1_server() :
   kvStore(KvMemStore::getInstance())
{}

std::unique_ptr<Result>
KvStoreApi_v1_server::create(std::unique_ptr<KvPair> arg)
{
   unique_ptr<Result> res(new Result);
   if (!kvStore->isPrimary()) {
      res->err() = REPLICA_NOT_PRIMARY;
      return res;
   }
   res->err() = checkKeyFormat(arg->key);
   if (res->err() == NONE) {
      size_t lastSlashIndex = pruneKey(arg->key);
      if (kvStore->hasKey(arg->key)) {
         cerr << "Key, " << arg->key << ", already present" << endl;
         res->err() = KEY_ALREADY_PRESENT;
      }
      if (lastSlashIndex > 0) {
         const string& parent = arg->key.substr(0, lastSlashIndex);
         if (!kvStore->hasKey(parent)) {
            cerr << "Parent not present for key, " << arg->key  << endl;
            res->err() = KEY_HAS_NO_PARENT;
         }
      }
      if (res->err() == NONE) {
         try {
            kvStore->set(arg->key, arg->val);
            cout << "Key, " << arg->key << ", added with value, "
                 << arg->val << endl;
         }
         catch (const QuorumNotReached& ex) {
            res->err() = QUORUM_NOT_REACHED;
         }
         catch (const ReplicaStateNotNormal& ex) {
            res->err() = STATUS_NOT_NORMAL;
         }
      }
   }
   return res;
}

std::unique_ptr<Result>
KvStoreApi_v1_server::remove(std::unique_ptr<LongString> arg)
{
   unique_ptr<Result> res(new Result);
   if (!kvStore->isPrimary()) {
      res->err() = REPLICA_NOT_PRIMARY;
      return res;
   }
   string& key = *arg;
   res->err() = checkKeyFormat(key);
   if (res->err() == NONE) {
      pruneKey(key);
      if (!kvStore->hasKey(key)) {
         res->err() = KEY_NOT_PRESENT;
         cerr << "Key, " << key << ", to remove is not present" << endl;
      }
      else if (!kvStore->list(key + '/', '/').empty()) {
         res->err() = KEY_HAS_CHILDREN;
         cerr << "Key, " << key << ", to remove has children" << endl;
      }
      else {
         try {
            kvStore->remove(key);
            cout << "Key, " << key << ", removed" << endl;
         }
         catch (const QuorumNotReached& ex) {
            res->err() = QUORUM_NOT_REACHED;
         }
         catch (const ReplicaStateNotNormal& ex) {
            res->err() = STATUS_NOT_NORMAL;
         }
      }
   }
   return res;
}

std::unique_ptr<Result>
KvStoreApi_v1_server::set(std::unique_ptr<KvPair> arg)
{
   unique_ptr<Result> res(new Result);
   if (!kvStore->isPrimary()) {
      res->err() = REPLICA_NOT_PRIMARY;
      return res;
   }
   res->err() = checkKeyFormat(arg->key);
   if (res->err() == NONE) {
      pruneKey(arg->key);
      if (!kvStore->hasKey(arg->key)) {
         res->err() = KEY_NOT_PRESENT;
         cerr << "Key, " << arg->key << ", is not present" << endl;
      }
      else {
         try {
            kvStore->set(arg->key, arg->val);
            cout << "Key, " << arg->key << ", set with value, " << arg->val
                 << endl;
         }
         catch (const QuorumNotReached& ex) {
            res->err() = QUORUM_NOT_REACHED;
         }
         catch (const ReplicaStateNotNormal& ex) {
            res->err() = STATUS_NOT_NORMAL;
         }
      }
   }
   return res;
}

std::unique_ptr<Result>
KvStoreApi_v1_server::get(std::unique_ptr<LongString> arg)
{
   unique_ptr<Result> res(new Result);
   if (!kvStore->isPrimary()) {
      res->err() = REPLICA_NOT_PRIMARY;
      return res;
   }
   string& key = *arg;
   res->err() = checkKeyFormat(key);
   if (res->err() == NONE) {
      pruneKey(key);
      if (!kvStore->hasKey(key)) {
         res->err() = KEY_NOT_PRESENT;
         cerr << "Key, " << key << ", not present" << endl;
      }
      else {
         res->discriminant(1);
         res->val() = kvStore->get(key);
      }
   }
   return res;
}

std::unique_ptr<Result>
KvStoreApi_v1_server::list(std::unique_ptr<LongString> arg)
{
   unique_ptr<Result> res(new Result);
   if (!kvStore->isPrimary()) {
      res->err() = REPLICA_NOT_PRIMARY;
      return res;
   }
   string& key = *arg;
   if (key.size() == 1 && key.at(0) == '/') {
      key.clear();
   }
   else {
      res->err() = checkKeyFormat(key);
      if (res->err() == NONE) {
         pruneKey(key);
         if (!kvStore->hasKey(key)) {
            res->err() = KEY_NOT_PRESENT;
            cerr << "Key, " << key << ", not present" << endl;
            return res;
         }
      }
      else {
         return res;
      }
   }
   res->discriminant(2);
   const std::set<string>& keySet = kvStore->list(key + '/', '/');
   for (auto it = keySet.begin(); it != keySet.end(); ++it) {
      size_t lastSlashIndex = it->find_last_of("/");
      assert(lastSlashIndex < it->size() - 1);
      const string& leaf = it->substr(lastSlashIndex + 1,
                                      it->size() - lastSlashIndex);
      res->keys().push_back(leaf);
   }
  return res;
}

} //namespace kvs
