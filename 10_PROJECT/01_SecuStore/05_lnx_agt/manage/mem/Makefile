include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ManageChkExistInterval.cpp ManageDelLastNotify.cpp ManageDevOInfo.cpp ManageDvDisable.cpp ManageDeviceMedia.cpp ManageEpsRegInfo.cpp ManageEvtMon.cpp ManageFileDown.cpp ManageLinkEnv.cpp ManageLogicTimer.cpp ManagePMSWork.cpp ManageProcessInfo.cpp ManagePatchFile.cpp ManageSysDrive.cpp ManageVulnFileProtect.cpp ManageVulnPreScan.cpp ManageVulnRegProtect.cpp ManageWinSession.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
