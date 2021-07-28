include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = DBMgrHost.cpp DBMgrHostHw.cpp DBMgrHostKey.cpp DBMgrHostPolicy.cpp DBMgrHostStatusPo.cpp DBMgrHostStatusPoNc.cpp DBMgrHostSw.cpp DBMgrHostSys.cpp DBMgrUser.cpp DBMgrUserPolicy.cpp DBMgrHostPatch.cpp DBMgrHostVulnRst.cpp DBMgrHostVulnRstUnit.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
