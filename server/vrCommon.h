#ifndef _VRCOMMON_H
#define _VRCOMMON_H

#include <thread>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <condition_variable>

#include "vrTypes.hh"
#include "include/endPoint.h"

namespace vr {

const std::unordered_map<ReplicaId, EndPoint>& getAllReplicas();

size_t getMajorityCnt();

/*
 * Below invoker function, calls obj->func(endpoint, args..) for each replica
 * endpoint in a separate thread.
 * id corresponds to the replica, that calls this function. Other than that
 * replica id, func is invoked for all other replicas.
 */
template<typename T, typename Function, typename... Args>
void
invokeForAllReplicas(ReplicaId id, T* obj, Function&& func, Args&&... args)
{
   //Configurator* configurator = Configurator::getInstance();
   const std::unordered_map<ReplicaId, EndPoint>& replicas = getAllReplicas();
      //configurator->getReplicas();
   std::vector<std::thread> thrds;
   for (auto it = replicas.begin(); it != replicas.end(); ++it) {
      ReplicaId rid = it->first;
      const EndPoint& ep = it->second;
      if (rid == id) {
         continue;
      }
      std::thread t(func, obj, std::cref(ep), args...);
      thrds.push_back(move(t));
   }
   for (size_t i = 0; i < thrds.size(); ++i) {
      thrds.at(i).join();
   }
}

struct Response {
   size_t cnt = 0;
   std::recursive_mutex rMutex;
   std::condition_variable_any cond;
};

template<typename T>
class WaitMap {
public:
   void add(const T& t, const std::shared_ptr<Response>& response)
   {
      std::lock_guard<std::mutex> lg(_mutex);
      _map[t] = response;
   }

   std::shared_ptr<Response> get(const T& t)
   {
      std::lock_guard<std::mutex> lg(_mutex);
      std::shared_ptr<Response>& resp = _map[t];
      if (resp.get() == nullptr) {
         resp = std::move(std::unique_ptr<Response>(new Response()));
      }
      return resp;
   }

   void clear()
   {
      std::lock_guard<std::mutex> lg(_mutex);
      _map.clear();
   }

   void erase(const T& t)
   {
      std::lock_guard<std::mutex> lg(_mutex);
      _map.erase(t);
   }

private:
   std::unordered_map<T, std::shared_ptr<Response>> _map;
   std::mutex _mutex;
};

bool quorumReached(const std::shared_ptr<Response>& resp);

} //namespace vr


#endif
