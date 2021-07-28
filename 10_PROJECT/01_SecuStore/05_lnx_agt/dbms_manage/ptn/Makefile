include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = DBMgrPtnDeploy.cpp DBMgrPtnPatch.cpp DBMgrPtnPatchFile.cpp DBMgrPtnPatchScan.cpp DBMgrPtnPatchScanEnv.cpp DBMgrPtnPatchScanEnvUnit.cpp DBMgrPtnPatchWsus.cpp DBMgrPtnProcFile.cpp DBMgrPtnVersion.cpp DBMgrPtnVuln.cpp DBMgrPtnVulnMP.cpp DBMgrPtnVulnScan.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
