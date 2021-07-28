include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = DBMgrPoDvDefault.cpp DBMgrPoInDevOBL.cpp DBMgrPoInDevOBLPkg.cpp DBMgrPoInDevOEx.cpp DBMgrPoInDevOExPkg.cpp DBMgrPoInDevOExUnit.cpp DBMgrPoInDevOInfo.cpp DBMgrPoInDevONotify.cpp DBMgrPoInDevONotifyPkg.cpp DBMgrPoInDevOOp.cpp DBMgrPoInDevOWL.cpp DBMgrPoInDevOWLPkg.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
