include ../../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ManagePoInPtnBL.cpp ManagePoInPtnBLPkg.cpp ManagePoInPtnBLUnit.cpp ManagePoInPtnEx.cpp ManagePoInPtnExPkg.cpp ManagePoInPtnExTrust.cpp ManagePoInPtnExUnit.cpp ManagePoInPtnNo.cpp ManagePoInPtnNoPkg.cpp ManagePoInPtnOp.cpp ManagePoInPtnOpPkg.cpp ManagePoInPtnOpUnit.cpp ManagePoInPtnSP.cpp ManagePoInPtnSPPkg.cpp ManagePoInPtnSPRule.cpp ManagePoInPtnSPUnit.cpp ManagePoInPtnWL.cpp ManagePoInPtnWLPkg.cpp ManagePoInPtnWLUnit.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)
	
clean:
	rm -f $(OBJ)
