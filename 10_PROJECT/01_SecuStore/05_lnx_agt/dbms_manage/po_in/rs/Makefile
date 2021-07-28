include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = DBMgrPoInRsBk.cpp DBMgrPoInRsNo.cpp DBMgrPoInRsNoPkg.cpp DBMgrPoInRsOp.cpp DBMgrPoInRsOpObjUnit.cpp DBMgrPoInRsOpPkg.cpp DBMgrPoInRsOpSubUnit.cpp DBMgrPoInRsOpUnit.cpp DBMgrPoInRsOpUnitObjPkg.cpp DBMgrPoInRsOpUnitSubPkg.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
