include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = LogicMgrHost.cpp LogicMgrHostHw.cpp LogicMgrHostKey.cpp LogicMgrHostStatusPo.cpp LogicMgrHostSw.cpp LogicMgrHostStatusPoNc.cpp LogicMgrHostSys.cpp LogicMgrUser.cpp LogicMgrHostVulnRst.cpp LogicMgrHostPatch.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
