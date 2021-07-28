include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = LogicMgrPoNcPtnBL.cpp LogicMgrPoNcPtnEx.cpp LogicMgrPoNcPtnExTrust.cpp LogicMgrPoNcPtnLo.cpp LogicMgrPoNcPtnNo.cpp LogicMgrPoNcPtnOp.cpp LogicMgrPoNcPtnSP.cpp LogicMgrPoNcPtnSPRule.cpp LogicMgrPoNcPtnWL.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
