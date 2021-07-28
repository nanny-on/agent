include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ManagePoCtlPanel.cpp ManagePoCtlPanelPkg.cpp ManagePoCtlPanelUnit.cpp ManagePoCtlProc.cpp ManagePoCtlProcPkg.cpp ManagePoCtlProcUnit.cpp ManagePoDeployFile.cpp ManagePoDeployFileChkUnit.cpp ManagePoDeployFileInfoUnit.cpp ManagePoDeployFilePkg.cpp ManagePoDeployFileSchUnit.cpp ManagePoDeployFileTarUnit.cpp ManagePoDeployFileUnit.cpp ManagePoDeployFileUnitChkPkg.cpp ManagePoDeployFileUnitInfoPkg.cpp ManagePoDeployFileUnitSchPkg.cpp ManagePoDeployFileUnitTarPkg.cpp ManagePoHostRmInfo.cpp ManagePoHostRun.cpp ManagePoHostWebSock.cpp ManagePoPower.cpp ManagePoPowerPkg.cpp ManagePoPowerUnit.cpp ManagePoSch.cpp ManagePoSchPkg.cpp ManagePoSchUnit.cpp ManagePoSelfProtect.cpp ManagePoSelfProtectPkg.cpp ManagePoSelfProtectUnit.cpp ManagePoSvrInfoLgn.cpp ManagePoSvrInfoUdt.cpp ManagePoWinRun.cpp ManagePoHostNotify.cpp ManagePoHostNotifyPkg.cpp ManagePoHostNotifyUnit.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
