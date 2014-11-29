
LIBCLIENT_SRC := libClient/kvsClient.cc \
               libClient/endPoint.cc

LIBCLIENT_OBJ := $(LIBCLIENT_SRC:.cc=.o)
$(LIBCLIENT_OBJ): include/kvsClient.h

libClient/%.o: libClient/%.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

libClient/libclient.a: $(LIBCLIENT_OBJ)
	$(AR) -r $@ $(LIBCLIENT_OBJ)

