include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -I/usr/include/mysql

CPPFILES = editconf.cpp 

OBJ = $(CPPFILES:.cpp=.o)

all:
	$(CC) $(CFLAGS) -c $(CPPFILES)
	make build

clean:
	rm -f $(OBJ)
	rm -f editconf

install:
	cp -f editconf /usr/local/ashin/.tools
	cp -f editconf $(AS_PKG_HOME)/tool	

build:
	$(CC) $(CFLAGS) -g -o editconf editconf.o ../../../../01_SDK/05_LIB/as_lib/src/libascomm.a -lodbc -luuid -ldl -lsqlite3 -static-libgcc -static-libstdc++