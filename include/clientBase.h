#ifndef _CLIENTBASE_H
#define _CLIENTBASE_H

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>

#include "xdrpp/rpcbind.hh"
#include "xdrpp/socket.h"
#include "xdrpp/srpc.h"
#include "endPoint.h"

template<typename T>
class ClientBase {
public:
   ClientBase();
   virtual ~ClientBase();
   void open(const EndPoint& ep);
   void close();
   bool isConnected();

protected:
   xdr::srpc_client<T>* _srpcClient;
};


template<typename T>
inline ClientBase<T>::ClientBase() : _srpcClient(nullptr) {}

template<typename T>
inline ClientBase<T>::~ClientBase()
{
   this->close();
}

template<typename T>
inline bool
ClientBase<T>::isConnected()
{
   return _srpcClient != nullptr;
}

template<typename T>
inline void
ClientBase<T>::open(const EndPoint& ep)
{
   if (this->isConnected()) {
      std::cout << "Already connected!" << std::endl;
      exit(1);
   }

   auto fd = xdr::tcp_connect(ep.ip.c_str(), ep.port.c_str());
   _srpcClient = new xdr::srpc_client<T>{fd.release()};
}

template<typename T>
inline void
ClientBase<T>::close()
{
   if (this->isConnected()) {
      delete _srpcClient;
      _srpcClient = nullptr;
   }
}

#endif
