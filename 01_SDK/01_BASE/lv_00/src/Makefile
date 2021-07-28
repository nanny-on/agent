include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ComGlobalMethod.cpp MutexExt.cpp SemExt.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
