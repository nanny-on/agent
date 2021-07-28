include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = DBMgrEnvLicense.cpp DBMgrEnvLog.cpp DBMgrEnvLogUnit.cpp DBMgrEnvNotifyInfo.cpp DBMgrEnvPolicy.cpp DBMgrEnvSelfProtectAgt.cpp DBMgrEnvSOrgLink.cpp DBMgrEnvTrustSubject.cpp DBMgrEnvLocal.cpp DBMgrEnvSocket.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
