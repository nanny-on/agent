include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -I..

CPPFILES = ThreadBase.cpp ThreadFinder.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
