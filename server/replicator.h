#ifndef _REPLICATOR_H
#define _REPLICATOR_H

#include <string>

namespace kvs {

class IReplicator {
public:
   virtual void replicateUpdate(const std::string& key,
                                const std::string& val) = 0;
   virtual void replicateDelete(const std::string& key) = 0;
   virtual bool isPrimary() = 0;


   IReplicator() : _updateCb(nullptr), _deleteCb(nullptr) {}

   virtual ~IReplicator() {}

   using UpdateCbFunc = void(*)(const std::string& key, const std::string& val);
   using DeleteCbFunc = void(*)(const std::string& key);

   void registerUpdateCb(UpdateCbFunc cb) { _updateCb = cb; }
   void registerDeleteCb(DeleteCbFunc cb) { _deleteCb = cb; }

protected:
   UpdateCbFunc _updateCb;
   DeleteCbFunc _deleteCb;
};

} //namespace kvs

#endif
