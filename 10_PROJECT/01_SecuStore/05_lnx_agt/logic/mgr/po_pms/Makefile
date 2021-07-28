include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = LogicMgrPoPmDm.cpp LogicMgrPoPmEx.cpp LogicMgrPoPmNo.cpp LogicMgrPoPmOp.cpp LogicMgrPoPmScan.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
