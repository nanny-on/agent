include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = LogicMgrLogDeployFile.cpp LogicMgrLogDevice.cpp LogicMgrLogDoc.cpp LogicMgrLogSecu.cpp LogicMgrLogEvent.cpp LogicMgrLogPatch.cpp LogicMgrLogRs.cpp LogicMgrLogRsBk.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
