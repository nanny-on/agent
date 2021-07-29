include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = makehash.cpp

OBJ = $(CPPFILES:.cpp=.o)

all:
	$(CC) $(CFLAGS) -c $(CPPFILES)
	make build

clean:
	rm -f $(OBJ)
	rm -f makehash

install:
	cp -f makehash /usr/local/ashin/.tools
	cp -f makehash $(AS_PKG_HOME)/tool	

build:
	$(CC) $(CFLAGS) -o makehash makehash.o -L$(CURL_PATH)/lib ../../../../01_SDK/05_LIB/as_lib/src/libascomm.a -static-libgcc -static-libstdc++