#ifndef _CLIENTWRAPPER_H
#define _CLIENTWRAPPER_H

#include "endPoint.h"

template<typename T>
struct ClientWrapper {
   ClientWrapper(const EndPoint& ep) { client.open(ep); }
   ~ClientWrapper() { client.close(); }

   T client;
};

#endif
