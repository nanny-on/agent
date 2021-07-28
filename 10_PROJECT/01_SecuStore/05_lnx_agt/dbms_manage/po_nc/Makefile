include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = DBMgrPoNcPtnBL.cpp DBMgrPoNcPtnBLPkg.cpp DBMgrPoNcPtnBLUnit.cpp DBMgrPoNcPtnEx.cpp DBMgrPoNcPtnExPkg.cpp DBMgrPoNcPtnExTrust.cpp DBMgrPoNcPtnExUnit.cpp DBMgrPoNcPtnLo.cpp DBMgrPoNcPtnNo.cpp DBMgrPoNcPtnNoPkg.cpp DBMgrPoNcPtnOp.cpp DBMgrPoNcPtnOpPkg.cpp DBMgrPoNcPtnOpUnit.cpp DBMgrPoNcPtnSP.cpp DBMgrPoNcPtnSPPkg.cpp DBMgrPoNcPtnSPRule.cpp DBMgrPoNcPtnSPUnit.cpp DBMgrPoNcPtnWL.cpp DBMgrPoNcPtnWLPkg.cpp DBMgrPoNcPtnWLUnit.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
