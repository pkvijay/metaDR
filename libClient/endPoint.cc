#include "include/endPoint.h"
#include <stdexcept>

using namespace std;

EndPoint::EndPoint(const string& endpoint)
{
   size_t pos = endpoint.find_first_of(':');
   if (pos == string::npos) {
      cerr << "Endpoint, " << endpoint
           << " should be provided in hostIp:portNum format" << endl;
      throw invalid_argument(endpoint);
   }
   ip = endpoint.substr(0, pos);
   port = endpoint.substr(pos + 1, endpoint.size() - pos - 1); 
}


std::ostream& 
operator<<(ostream& os, const EndPoint& ep)
{
   os << ep.ip << ":" << ep.port;
   return os;
}
