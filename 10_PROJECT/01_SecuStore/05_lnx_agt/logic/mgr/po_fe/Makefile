include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = LogicMgrPoFeExclude.cpp LogicMgrPoFeNotify.cpp LogicMgrPoFePtnBL.cpp LogicMgrPoFePtnLo.cpp LogicMgrPoFePtnOp.cpp LogicMgrPoFePtnWL.cpp LogicMgrPoFeSinglePtn.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
