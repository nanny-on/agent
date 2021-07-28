include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ManagePoPmDm.cpp ManagePoPmDmPkg.cpp ManagePoPmDmUnit.cpp ManagePoPmEx.cpp ManagePoPmExPkg.cpp ManagePoPmExUnit.cpp ManagePoPmNo.cpp ManagePoPmNoPkg.cpp ManagePoPmOp.cpp ManagePoPmScan.cpp ManagePoPmScanPkg.cpp ManagePoPmScanUnit.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
