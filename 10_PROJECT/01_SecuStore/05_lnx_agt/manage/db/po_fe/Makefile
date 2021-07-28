include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ManagePoFeExclude.cpp ManagePoFeExcludePkg.cpp ManagePoFeExcludeUnit.cpp ManagePoFeNotify.cpp ManagePoFeNotifyPkg.cpp ManagePoFePtnBL.cpp ManagePoFePtnLo.cpp ManagePoFePtnOp.cpp ManagePoFePtnWL.cpp ManagePoFeSinglePtn.cpp ManagePoFeSinglePtnUnit.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
