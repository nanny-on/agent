include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ManageDocDeleteInfo.cpp ManageErrorCode.cpp ManageLinkLicense.cpp ManageLocalEnvAuth.cpp ManageLocalEnvLog.cpp ManageOsType.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
