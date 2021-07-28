include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = TimerUtil.cpp TokenString.cpp TokenCString.cpp ValidTimeUtil.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
