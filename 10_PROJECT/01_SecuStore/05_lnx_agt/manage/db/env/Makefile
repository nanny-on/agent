include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ManageEnvLicense.cpp ManageEnvLocal.cpp ManageEnvLog.cpp ManageEnvLogUnit.cpp ManageEnvNotifyInfo.cpp ManageEnvPolicy.cpp ManageEnvSelfProtectAgt.cpp ManageEnvSOrgLink.cpp ManageEnvSocket.cpp ManageEnvTrustSubject.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
