include ../../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ManagePoInVulnAx.cpp ManagePoInVulnAxPkg.cpp ManagePoInVulnAxUnit.cpp ManagePoInVulnEditApp.cpp ManagePoInVulnEditAppPkg.cpp ManagePoInVulnEditAppUnit.cpp ManagePoInVulnOp.cpp ManagePoInVulnOpPkg.cpp ManagePoInVulnPatchException.cpp ManagePoInVulnPatchExceptionPkg.cpp ManagePoInVulnPatchExceptionUnit.cpp ManagePoInVulnQna.cpp ManagePoInVulnQnaPkg.cpp ManagePoInVulnQnaUnit.cpp ManagePoInVulnScan.cpp ManagePoInVulnScanPkg.cpp ManagePoInVulnScanUnit.cpp ManagePoInVulnScanUnitPkg.cpp ManagePoInVulnSecuUsb.cpp ManagePoInVulnSecuUsbPkg.cpp ManagePoInVulnSecuUsbUnit.cpp ManagePoInVulnSw.cpp ManagePoInVulnSwPkg.cpp ManagePoInVulnSwUnit.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
