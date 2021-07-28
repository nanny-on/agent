include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ManageHost.cpp ManageHostHw.cpp ManageHostKey.cpp ManageHostPatch.cpp ManageHostPolicy.cpp ManageHostStatusPo.cpp ManageHostStatusPoNc.cpp ManageHostSw.cpp ManageHostSys.cpp ManageUser.cpp ManageUserPolicy.cpp ManageHostVulnRst.cpp ManageHostVulnRstUnit.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
