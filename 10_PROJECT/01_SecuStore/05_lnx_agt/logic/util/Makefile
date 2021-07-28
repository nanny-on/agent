include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = LogicDocDeleteInfo.cpp LogicLocalEnvAuth.cpp LogicLocalEnvLog.cpp LogicPWEvtMon.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
