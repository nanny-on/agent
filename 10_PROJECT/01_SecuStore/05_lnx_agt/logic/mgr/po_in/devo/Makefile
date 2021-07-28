include ../../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = LogicMgrPoDvLo.cpp LogicMgrPoInDevOBL.cpp LogicMgrPoInDevOEx.cpp LogicMgrPoInDevOInfo.cpp LogicMgrPoInDevONotify.cpp LogicMgrPoInDevOOp.cpp LogicMgrPoInDevOWL.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
