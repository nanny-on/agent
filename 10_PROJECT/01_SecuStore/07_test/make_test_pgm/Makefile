include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = make_test_pgm.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)
	$(CC) $(CFLAGS) -g -o make_test_pgm $(OBJ) -static-libgcc -static-libstdc++

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
	rm -f make_test_pgm