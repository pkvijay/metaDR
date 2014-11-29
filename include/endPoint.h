#ifndef _ENDPOINT_H
#define _ENDPOINT_H

#include <string>
#include <iostream>

struct EndPoint {
   EndPoint() {}
   EndPoint(const std::string& _ip,
            const std::string& _port) : ip(_ip), port(_port) {}

   // Constructor to handle string rep of hostIp:Port
   EndPoint(const std::string& endpoint);

   operator std::string() const { return ip + ":" + port; }

   std::string ip;
   std::string port;
};

std::ostream&
operator<<(std::ostream& os, const EndPoint& ep);

#endif
