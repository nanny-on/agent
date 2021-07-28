include ../../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ManagePoDvDefault.cpp ManagePoInDevOBL.cpp ManagePoInDevOBLPkg.cpp ManagePoInDevOEx.cpp ManagePoInDevOExPkg.cpp ManagePoInDevOExUnit.cpp ManagePoInDevOInfo.cpp ManagePoInDevONotify.cpp ManagePoInDevONotifyPkg.cpp ManagePoInDevOOp.cpp ManagePoInDevOWL.cpp ManagePoInDevOWLPkg.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
