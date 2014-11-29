
SHELL_SRC := shell/shell.cc

SHELL_OBJ := $(SHELL_SRC:.cc=.o)

shell/%.o: shell/%.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

shell/shell: $(SHELL_OBJ) libClient/libclient.a
	$(LD) -o $@ $(LDFLAGS) $(SHELL_OBJ) $(LIBS)

