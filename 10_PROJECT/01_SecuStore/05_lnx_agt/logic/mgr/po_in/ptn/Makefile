include ../../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = LogicMgrPoInPtnBL.cpp LogicMgrPoInPtnEx.cpp LogicMgrPoInPtnExTrust.cpp LogicMgrPoInPtnNo.cpp LogicMgrPoInPtnOp.cpp LogicMgrPoInPtnSP.cpp LogicMgrPoInPtnSPRule.cpp LogicMgrPoInPtnWL.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
