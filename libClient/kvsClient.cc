#include <iostream>
#include <set>
#include <string>

#include "include/kvsClient.h"

using namespace std;

namespace kvs {

static ClientError
getClientError(kvs::Error err)
{
   assert(err != NONE);
   assert(err != KEY_ALREADY_PRESENT);
   switch (err)
   {
      case KEY_NOT_PRESENT :
         return KEY_NOT_FOUND;
      case KEY_HAS_NO_PARENT :
         return NO_PARENT;
      case KEY_HAS_CHILDREN :
         return HAS_CHILDREN;
      case KEY_MALFORMED :
         return MALFORMED_KEY;
      case REPLICA_NOT_PRIMARY :
         return NOT_PRIMARY_REPLICA;
      case QUORUM_NOT_REACHED :
         return FAILED_TO_REACH_QUORUM;
      case STATUS_NOT_NORMAL :
         return REPLICA_STATUS_NOT_NORMAL;
      default :
         assert(false);
   }
}

bool
Client::create(const std::string &path, const std::string &val)
{
   KvPair args;

   args.key = path;
   args.val = val;

   auto r = _srpcClient->create(args);
   if (r->err() == KEY_ALREADY_PRESENT) {
      return false;
   }
   if (r->err() != NONE) {
      throw ClientException(getClientError(r->err()));
   }
   return true;
}

bool
Client::remove(const std::string &path)
{
   auto r = _srpcClient->remove(path);
   if (r->err() == KEY_NOT_PRESENT) {
      return false;
   }
   if (r->err() != NONE) {
      throw ClientException(getClientError(r->err()));
   }
   return true;
}

std::string
Client::get(const std::string &path)
{
   auto r = _srpcClient->get(path);
   // if discriminant is not 1, then there is some error
   if (r->discriminant() != 1) {
      throw ClientException(getClientError(r->err()));
   }
   return r->val();
}

void
Client::set(const std::string &path, const std::string &val)
{
   KvPair args;
   args.key = path;
   args.val = val;

   auto r = _srpcClient->set(args);
   if (r->err() != NONE) {
      throw ClientException(getClientError(r->err()));
   }
}

std::set<string>
Client::list(const string &path)
{
   auto r = _srpcClient->list(path);
   // if discriminant is not 2, then some error
   if (r->discriminant() != 2) {
      throw ClientException(getClientError(r->err()));
   }
   std::set<string> res;
   for (xdr::xvector<xdr::xstring<> >::const_iterator it = r->keys().begin();
        it != r->keys().end(); ++it) {
      res.insert(*it);
   }
   return res;
}

} //namespace kvs
