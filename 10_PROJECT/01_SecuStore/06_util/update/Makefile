include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = mz_ioapi.cpp mz_unzip.cpp mz_zip.cpp zip.cpp update.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)
	$(CC) $(CFLAGS) -g -o update $(OBJ) ../../../../01_SDK/05_LIB/as_lib/src/libascomm.a -lz -ldl -static-libgcc -static-libstdc++

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
	rm -f update

install:
	cp -f update /usr/local/ashin/.tools
	cp -f update $(AS_PKG_HOME)/tool