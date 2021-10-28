include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = exe_test_pgm.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)
	$(CC) $(CFLAGS) -g -o exe_test_pgm $(OBJ) -static-libgcc -static-libstdc++

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
	rm -f exe_test_pgm

install:
	cp -f exe_test_pgm /usr/local/ashin/.tools
	cp -f exe_test_pgm $(AS_PKG_HOME)/tool