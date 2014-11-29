
#include <unistd.h>
#include <sys/socket.h>

#include <iostream>
#include <thread>

#include "xdrpp/srpc.h"
#include "xdrpp/rpcbind.hh"
#include "xdrpp/socket.h"

#include "configurator.h"
#include "kvStoreImpl.hh"
#include "vsReplicationImpl.hh"
#include "viewChangeImpl.hh"

using namespace std;
using namespace xdr;
using namespace kvs;
using namespace vr;


static void
runListener(rpc_tcp_listener& rl)
{
   try {
      cout << "Thread id, " << this_thread::get_id() << " running" << endl;
      rl.run();
   }
   catch (exception& ex) {
      cerr << "Listener thread(id : " << this_thread::get_id()
           << "), failed with exception : " << ex.what() << endl;
      throw;
   }
}

int main(int argc, const char *argv[])
{
   if (argc < 2) {
      cerr << "Please provide config file" << endl;
      return 1;
   }
   Configurator* configurator = Configurator::getInstance();
   configurator->bootStrap(argv[1]);
   const string& clientPort = configurator->getClientPort();
   const string& replicationPort = configurator->getReplicationPort();

   KvStoreApi_v1_server kvServer;
   ReplicationApi_v1_server repServer;
   ViewChangeApi_v1_server viewChangeServer;

   rpc_tcp_listener rl1(tcp_listen(clientPort.c_str(), AF_INET));
   rl1.register_service(kvServer);

   rpc_tcp_listener rl2(tcp_listen(replicationPort.c_str(), AF_INET));
   rl2.register_service(repServer);
   rl2.register_service(viewChangeServer);

   thread t1(runListener, ref(rl1));
   cout << "Started kvServer thread, id : " << t1.get_id() << endl;
   thread t2(runListener, ref(rl2));
   cout << "Started replication threadm id : " << t2.get_id() << endl;

   t1.join();
   t2.join();

   Configurator::deleteInstance();
}

