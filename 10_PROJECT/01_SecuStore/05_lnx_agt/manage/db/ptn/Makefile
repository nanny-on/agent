include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ManagePtnDeploy.cpp ManagePtnPatch.cpp ManagePtnPatchFile.cpp ManagePtnPatchScan.cpp ManagePtnPatchScanEnv.cpp ManagePtnPatchScanEnvUnit.cpp ManagePtnPatchWsus.cpp ManagePtnProcFile.cpp ManagePtnVersion.cpp ManagePtnVuln.cpp ManagePtnVulnMP.cpp ManagePtnVulnScan.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
