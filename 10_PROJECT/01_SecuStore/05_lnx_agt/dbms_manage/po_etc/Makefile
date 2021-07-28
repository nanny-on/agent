include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = DBMgrPoCtlPanel.cpp DBMgrPoCtlPanelPkg.cpp DBMgrPoCtlPanelUnit.cpp DBMgrPoCtlProc.cpp DBMgrPoCtlProcPkg.cpp DBMgrPoCtlProcUnit.cpp DBMgrPoDeployFile.cpp DBMgrPoDeployFileChkUnit.cpp DBMgrPoDeployFileInfoUnit.cpp DBMgrPoDeployFilePkg.cpp DBMgrPoDeployFileSchUnit.cpp DBMgrPoDeployFileTarUnit.cpp DBMgrPoDeployFileUnit.cpp DBMgrPoDeployFileUnitChkPkg.cpp DBMgrPoDeployFileUnitInfoPkg.cpp DBMgrPoDeployFileUnitSchPkg.cpp DBMgrPoDeployFileUnitTarPkg.cpp DBMgrPoHostNotify.cpp DBMgrPoHostNotifyPkg.cpp DBMgrPoHostNotifyUnit.cpp DBMgrPoHostRmInfo.cpp DBMgrPoHostRun.cpp DBMgrPoHostWebSock.cpp DBMgrPoPower.cpp DBMgrPoPowerPkg.cpp DBMgrPoPowerUnit.cpp DBMgrPoSch.cpp DBMgrPoSchPkg.cpp DBMgrPoSchUnit.cpp DBMgrPoSelfProtect.cpp DBMgrPoSelfProtectPkg.cpp DBMgrPoSelfProtectUnit.cpp DBMgrPoSvrInfoLgn.cpp DBMgrPoSvrInfoUdt.cpp DBMgrPoWinRun.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
