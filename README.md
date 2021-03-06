metaDR - (Metadata Replicated)
==============================
This project provides a replicated in-memory metadata filesystem using
Viewstamped Replication [1]. Most of the distributed systems have to manage
metadata corresponding to the service that they provide.  For example, GFS [3]
master node maintains the metadata of the entire filesystem. Zookeeper [2]
provides a replicated metadata filesystem which can be used to implement other
distributed services like locking, leader election etc. It is inevitable that
this metadata need to be highly available as this can be a single point of
failure. Viewstamped replication [1] provides the algorithm to achieve consensus
in distributed systems. We are implementing a simple in-memory key-value store
that is replicated and made highly available using viewstamped replication. We
have modeled the key-value store interface to be similar to the one in
zookeeper, where the key is in the format of UNIX filesystem path and value is a
string.  At any point of time, there is a single primary and multiple backups.
All the client requests have to be issued to the primary in order to guarantee
strong consistency. When the primary fails, one of the backups changes role to
primary. The system can tolerate ‘f’ failures where the total number of nodes in
the system configuration is 2f + 1.

The system is being implemented using C++11 on Linux based platform.  There are
two main components in the system: the client library and the backend server
executable. The client library provides the APIs for the KV store. The users of
the system are expected to use the client library APIs to store their metadata.
The client library uses RPCs to transfer the client requests to the backend
server. The backend takes care of processing the client request, modifying the
actual key-value store and replicating the update/deletion across all the
backups.  The main KV store client APIs are create, set, get, list and remove. A
shell kind of utility is provided that can be used by the clients to invoke
different commands to manage the KV store.  The shell can be used if the users
do not want to write code using the client library. If the size of the system
configuration is N, then N instances of backend server need to be started. One
of them would be elected as primary and the others will be backup. Each instance
of the server needs to be started with a configuration file. The configuration
file contains the details of the client and the replication ports, where the
server has to start the listener threads and in addition, it contains the
details of all the nodes in the system. This is how each server instance would
know the details of the nodes in system with which it has to communicate to
implement the consensus protocol.  The client needs to be aware of the system
configuration and all client requests should be sent only to the primary. If a
request were sent to a backup, then the client would get an error explaining
that the node is not primary. In case of primary failure, the client can
broadcast the request to all the nodes and only one of them would accept the
request, which is the new primary.  From then on, the client can just send the
request to the new primary. Server management client APIs can be provided for
reconfiguration and shutdown support. This should allow the clients to
dynamically add more server instances to the system and to cleanly shutdown any
server instance for maintenance reasons.

XDRPP [6] module provides an XDR based RPC [4] implementation. XDR [5] stands
for External Data Representation which describes the interface definition
language (IDL), which can be used to express data structures, transmitted
between machines using RPC. XDRPP comprises of a compiler that facilitates stub
generation from XDR based IDLs. It consists of a runtime library that hides the
network communication details and facilitates RPC invocations across different
machines. 

Build Dependencies :

. gmake 
. gcc or clang with C++11 
. bison
. flex 
. pkg-config

One useful feature of XDRPP is that is can trace and pretty-print RPC calls if
the following environment variables are set.
$ export XDR_TRACE_CLIENT=XDR_TRACE_SERVER=1

Testing :

The server executable should be started with the config files which provide the
configuration and endpoint details. For sample config file, refer to the tests
folder. For eg: To run a instance of server run 'server/server tests/config-0'.
Similarly run the other instances of the replica server using the corresponding
config files. NOTE that in view-stamped replication, the primary is chosen in a
round-robin fashion. So make sure that the replica-0 in the configuration is
started first.
Once all replicas are started, use metaDR shell to run issue some client
requests. There are some sample test scripts, suffixed with .tst extension in
the tests folder. For eg:- to run the create.tst, run
'shell/shell localhost:4004 tests/create.tst'

For more details on the design of this project, refer to 
https://github.com/pkvijay/metaDR/blob/master/Design.pdf 

References :

[1] Viewstamped Replication Revisited
(http://www.scs.stanford.edu/14au-cs244b/sched/readings/vr-revisited.pdf)

[2] Zookeeper
(http://www.scs.stanford.edu/14au-cs244b/sched/readings/zookeeper.pdf)

[3] The Google File System
(http://www.scs.stanford.edu/14au-cs244b/sched/readings/gfs.pdf)

[4] RPC Specification version 2 (https://tools.ietf.org/html/rfc5531)

[5] XDR: External Data Representation Standard
(http://tools.ietf.org/html/rfc4506)

[6] XDRPP (http://www.scs.stanford.edu/14au-cs244b/labs/xdrpp-doc/)

