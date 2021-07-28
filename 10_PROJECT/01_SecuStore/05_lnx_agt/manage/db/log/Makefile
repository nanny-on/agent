include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ManageLogDeployFile.cpp ManageLogDevice.cpp ManageLogDoc.cpp ManageLogDocDScan.cpp ManageLogDocHost.cpp ManageLogEvent.cpp ManageLogSecu.cpp ManageLogPatch.cpp ManageLogRs.cpp ManageLogRsBk.cpp
OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
