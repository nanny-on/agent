include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = LogicMgrCtrlHostRes.cpp LogicMgrCtrlInstantMsg.cpp LogicMgrCtrlRemoteCtrl.cpp LogicMgrCtrlRemoteOrder.cpp LogicMgrCtrlRemotePower.cpp LogicMgrCtrlRestore.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
