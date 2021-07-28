include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = LogicMgrPoCtlPanel.cpp LogicMgrPoCtlProc.cpp LogicMgrPoDeployFile.cpp LogicMgrPoHostRmInfo.cpp LogicMgrPoHostRun.cpp LogicMgrPoPower.cpp LogicMgrPoHostWebSock.cpp LogicMgrPoSch.cpp LogicMgrPoSelfProtect.cpp LogicMgrPoSvrInfoLgn.cpp LogicMgrPoSvrInfoUdt.cpp LogicMgrPoWinRun.cpp LogicMgrPoHostNotify.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
