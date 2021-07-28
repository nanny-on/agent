include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = LogicMgrEnvLicense.cpp LogicMgrEnvLog.cpp LogicMgrEnvLogUnit.cpp LogicMgrEnvNotifyInfo.cpp LogicMgrEnvSelfProtectAgt.cpp LogicMgrEnvSOrgLink.cpp LogicMgrEnvTrustSubject.cpp LogicMgrEnvSocket.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
