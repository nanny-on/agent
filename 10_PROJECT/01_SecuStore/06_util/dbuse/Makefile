include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -I/usr/include -I$(SQLITE3_PATH)/include

CPPFILES = dbuse.cpp

OBJ = $(CPPFILES:.cpp=.o)

all:
	$(CC) $(CFLAGS) -c $(CPPFILES)
	make build

clean:
	rm -f $(OBJ)
	rm -f dbuse

install:
	cp -f dbuse /usr/local/ashin/.tools
	cp -f dbuse $(AS_PKG_HOME)/tool
	
build:
	$(CC) $(CFLAGS) -g -o dbuse dbuse.o ../../../../01_SDK/05_LIB/as_lib/src/libascomm.a -lodbc -ldl -static-libgcc -static-libstdc++