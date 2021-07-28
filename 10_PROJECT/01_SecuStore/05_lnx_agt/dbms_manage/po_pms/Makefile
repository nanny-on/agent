include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = DBMgrPoPmDm.cpp DBMgrPoPmDmPkg.cpp DBMgrPoPmDmUnit.cpp DBMgrPoPmEx.cpp DBMgrPoPmExPkg.cpp DBMgrPoPmExUnit.cpp DBMgrPoPmNo.cpp DBMgrPoPmNoPkg.cpp DBMgrPoPmOp.cpp DBMgrPoPmScan.cpp DBMgrPoPmScanPkg.cpp DBMgrPoPmScanUnit.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
