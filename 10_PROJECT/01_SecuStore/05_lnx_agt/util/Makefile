include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ApplyPatchUtil.cpp BIniFile.cpp ComFuncUtil.cpp DeployPolicyUtil.cpp DocBackupUtil.cpp DocBackupUtil_RsBk.cpp ExecuteFileUtil.cpp ExecuteFileUtil_PPtn.cpp ExecuteFileUtil_VPtn.cpp ExPopupUtil.cpp FileDeleteUtil.cpp MatchFilterUtil.cpp SecureDeleteUtil.cpp SimpleSysCheckUtil.cpp SimpleVulnPreScanUtil.cpp UtilBase.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
