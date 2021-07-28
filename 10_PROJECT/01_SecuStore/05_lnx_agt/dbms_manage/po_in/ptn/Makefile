include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = DBMgrPoInPtnBL.cpp DBMgrPoInPtnBLPkg.cpp DBMgrPoInPtnBLUnit.cpp DBMgrPoInPtnEx.cpp DBMgrPoInPtnExPkg.cpp DBMgrPoInPtnExTrust.cpp DBMgrPoInPtnExUnit.cpp DBMgrPoInPtnNo.cpp DBMgrPoInPtnNoPkg.cpp DBMgrPoInPtnOp.cpp DBMgrPoInPtnOpPkg.cpp DBMgrPoInPtnOpUnit.cpp DBMgrPoInPtnSP.cpp DBMgrPoInPtnSPPkg.cpp DBMgrPoInPtnSPRule.cpp DBMgrPoInPtnSPUnit.cpp DBMgrPoInPtnWL.cpp DBMgrPoInPtnWLPkg.cpp DBMgrPoInPtnWLUnit.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
