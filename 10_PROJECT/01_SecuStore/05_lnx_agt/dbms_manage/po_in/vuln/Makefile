include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = DBMgrPoInVulnAx.cpp DBMgrPoInVulnAxPkg.cpp DBMgrPoInVulnAxUnit.cpp DBMgrPoInVulnEditApp.cpp DBMgrPoInVulnEditAppPkg.cpp DBMgrPoInVulnEditAppUnit.cpp DBMgrPoInVulnOp.cpp DBMgrPoInVulnOpPkg.cpp DBMgrPoInVulnPatchException.cpp DBMgrPoInVulnPatchExceptionPkg.cpp DBMgrPoInVulnPatchExceptionUnit.cpp DBMgrPoInVulnQna.cpp DBMgrPoInVulnQnaPkg.cpp DBMgrPoInVulnQnaUnit.cpp DBMgrPoInVulnScan.cpp DBMgrPoInVulnScanPkg.cpp DBMgrPoInVulnScanUnit.cpp DBMgrPoInVulnScanUnitPkg.cpp DBMgrPoInVulnSecuUsb.cpp DBMgrPoInVulnSecuUsbPkg.cpp DBMgrPoInVulnSecuUsbUnit.cpp DBMgrPoInVulnSw.cpp DBMgrPoInVulnSwPkg.cpp DBMgrPoInVulnSwUnit.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
