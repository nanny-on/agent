include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ManagePoNcPtnBL.cpp ManagePoNcPtnBLPkg.cpp ManagePoNcPtnBLUnit.cpp ManagePoNcPtnEx.cpp ManagePoNcPtnExPkg.cpp ManagePoNcPtnExTrust.cpp ManagePoNcPtnExUnit.cpp ManagePoNcPtnLo.cpp ManagePoNcPtnNo.cpp ManagePoNcPtnNoPkg.cpp ManagePoNcPtnOp.cpp ManagePoNcPtnOpPkg.cpp ManagePoNcPtnOpUnit.cpp ManagePoNcPtnSP.cpp ManagePoNcPtnSPPkg.cpp ManagePoNcPtnSPRule.cpp ManagePoNcPtnSPUnit.cpp ManagePoNcPtnWL.cpp ManagePoNcPtnWLPkg.cpp ManagePoNcPtnWLUnit.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
