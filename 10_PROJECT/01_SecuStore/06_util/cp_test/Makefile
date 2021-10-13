include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = cp_test.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)
	$(CC) $(CFLAGS) -g -o cp_test $(OBJ) -static-libgcc -static-libstdc++

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
	rm -f cp_test

install:
	cp -f cp_test /bin
	cp -f cp_test $(AS_PKG_HOME)/tool