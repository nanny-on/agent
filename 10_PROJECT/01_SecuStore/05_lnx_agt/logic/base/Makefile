include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = LogicBase.cpp LogicBase_CltLgn.cpp LogicBase_CltMgr.cpp LogicBase_CltUdt.cpp LogicBase_Log.cpp LogicBase_SvrLink.cpp LogicBaseImpl.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
