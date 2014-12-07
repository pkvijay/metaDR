
SERVER_SRC := server/server.cc \
			  server/kvStoreImpl.cc \
           server/kvMemStore.cc \
			  server/serverdb.cc \
			  server/sqlstmt.cc \
			  server/sqlite3.c \
           server/vsReplicationClient.cc \
           server/vsReplicationImpl.cc \
           server/vsReplicator.cc \
           server/configurator.cc \
           server/inMemOpLogHandler.cc \
           server/vsReplicaState.cc \
           server/vrCommon.cc \
           server/viewChangeClient.cc \
           server/viewChangeImpl.cc \
           server/viewChangeHandler.cc \
           server/recoveryClient.cc \
           server/recoveryImpl.cc \
           server/recoveryHandler.cc

SERVER_OBJ := $(SERVER_SRC:.c=.o)
SERVER_OBJ := $(SERVER_OBJ:.cc=.o)
$(SERVER_OBJ): include/kvStore.hh

server/%.o: server/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

server/%.o: server/%.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

# Generate initial files to be edited
scaffold: include/kvStore.x xdrpp
	$(XDRC) -serverhh -o server/kvStoreImpl.hh $<
	$(XDRC) -servercc -o server/kvStoreImpl.cc $<

server/server: $(SERVER_OBJ)
	$(LD) -o $@ $(LDFLAGS) $(SERVER_OBJ) $(LIBS)
