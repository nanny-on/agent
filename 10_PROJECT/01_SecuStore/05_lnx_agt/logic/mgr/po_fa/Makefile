include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = LogicMgrPoFaBk.cpp LogicMgrPoFaClear.cpp LogicMgrPoFaEnv.cpp LogicMgrPoFaNotify.cpp LogicMgrPoFaOp.cpp LogicMgrPoFaProc.cpp LogicMgrPoFaDelFileAfterBoot.cpp LogicMgrPoFaInotifyFile.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
