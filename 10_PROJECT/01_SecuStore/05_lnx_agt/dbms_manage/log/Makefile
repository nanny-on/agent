include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = DBMgrLogDeployFile.cpp DBMgrLogDevice.cpp DBMgrLogDoc.cpp DBMgrLogDocDScan.cpp DBMgrLogDocHost.cpp DBMgrLogEvent.cpp DBMgrLogSecu.cpp DBMgrLogPatch.cpp DBMgrLogRs.cpp DBMgrLogRsBk.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
