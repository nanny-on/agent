include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ManagePoFaBk.cpp ManagePoFaClear.cpp ManagePoFaClearPkg.cpp ManagePoFaClearUnit.cpp ManagePoFaEnv.cpp ManagePoFaEnvPkg.cpp ManagePoFaEnvUnit.cpp ManagePoFaNotify.cpp ManagePoFaNotifyPkg.cpp ManagePoFaOp.cpp ManagePoFaOpObjUnit.cpp ManagePoFaOpPkg.cpp ManagePoFaOpSchUnit.cpp ManagePoFaOpSubUnit.cpp ManagePoFaOpUnit.cpp ManagePoFaOpUnitObjPkg.cpp ManagePoFaOpUnitRulPkg.cpp ManagePoFaOpUnitSchPkg.cpp ManagePoFaOpUnitSubPkg.cpp ManagePoFaProc.cpp ManagePoFaProcPkg.cpp ManagePoFaProcUnit.cpp ManagePoFaDelFileAfterBoot.cpp ManagePoFaInotifyFile.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
