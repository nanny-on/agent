include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -I/usr/include

CPPFILES = white_test.cpp

OBJ = $(CPPFILES:.cpp=.o)

all:
	$(CC) $(CFLAGS) -c $(CPPFILES)
	make build

clean:
	rm -f $(OBJ)
	rm -f white_test

install:
	cp -f white_test ..
	
build:
	$(CC) $(CFLAGS) -g -o white_test white_test.o ../../dll_lib/ASIWEngDLLUtil.o ../../../../../../01_SDK/05_LIB/as_lib/src/libascomm.a -ldl -static-libgcc -static-libstdc++
