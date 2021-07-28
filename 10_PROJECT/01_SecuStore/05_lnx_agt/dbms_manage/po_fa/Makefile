include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = DBMgrPoFaBk.cpp DBMgrPoFaClear.cpp DBMgrPoFaClearPkg.cpp DBMgrPoFaClearUnit.cpp DBMgrPoFaDelFileAfterBoot.cpp DBMgrPoFaEnv.cpp DBMgrPoFaEnvPkg.cpp DBMgrPoFaEnvUnit.cpp DBMgrPoFaNotify.cpp DBMgrPoFaNotifyPkg.cpp DBMgrPoFaOp.cpp DBMgrPoFaOpObjUnit.cpp DBMgrPoFaOpPkg.cpp DBMgrPoFaOpSchUnit.cpp DBMgrPoFaOpSubUnit.cpp DBMgrPoFaOpUnit.cpp DBMgrPoFaOpUnitObjPkg.cpp DBMgrPoFaOpUnitRulPkg.cpp DBMgrPoFaOpUnitSchPkg.cpp DBMgrPoFaOpUnitSubPkg.cpp DBMgrPoFaProc.cpp DBMgrPoFaProcPkg.cpp DBMgrPoFaProcUnit.cpp DBMgrPoFaInotifyFile.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
