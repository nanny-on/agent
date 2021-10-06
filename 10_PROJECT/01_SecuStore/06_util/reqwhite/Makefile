include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = reqwhite.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)
	$(CC) $(CFLAGS) -g -o reqwhite $(OBJ) ../../../../01_SDK/05_LIB/as_lib/src/libascomm.a -ldl -static-libgcc -static-libstdc++

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
	rm -f reqwhite

install:
	cp -f reqwhite /bin
	cp -f reqwhite $(AS_PKG_HOME)/tool