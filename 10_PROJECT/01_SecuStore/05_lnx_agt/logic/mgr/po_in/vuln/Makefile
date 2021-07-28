include ../../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = LogicMgrPoInVulnAx.cpp LogicMgrPoInVulnEditApp.cpp LogicMgrPoInVulnOp.cpp LogicMgrPoInVulnPatchException.cpp LogicMgrPoInVulnQna.cpp LogicMgrPoInVulnScan.cpp LogicMgrPoInVulnSecuUsb.cpp LogicMgrPoInVulnSw.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
