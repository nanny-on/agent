include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = LogicMgrPtnGBO.cpp LogicMgrPtnGWO.cpp LogicMgrPtnPatch.cpp LogicMgrPtnPatch_Util.cpp LogicMgrPtnVuln.cpp LogicMgrPtnVuln_Util.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
