include ../../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ManagePoInRsBk.cpp ManagePoInRsNo.cpp ManagePoInRsNoPkg.cpp ManagePoInRsOp.cpp ManagePoInRsOpObjUnit.cpp ManagePoInRsOpPkg.cpp ManagePoInRsOpSubUnit.cpp ManagePoInRsOpUnit.cpp ManagePoInRsOpUnitObjPkg.cpp ManagePoInRsOpUnitSubPkg.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
