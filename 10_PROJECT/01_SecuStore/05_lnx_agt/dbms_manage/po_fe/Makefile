include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = DBMgrPoFeExclude.cpp DBMgrPoFeExcludePkg.cpp DBMgrPoFeExcludeUnit.cpp DBMgrPoFeNotify.cpp DBMgrPoFeNotifyPkg.cpp DBMgrPoFePtnBL.cpp DBMgrPoFePtnLo.cpp DBMgrPoFePtnOp.cpp DBMgrPoFePtnWL.cpp DBMgrPoFeSinglePtn.cpp DBMgrPoFeSinglePtnUnit.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
