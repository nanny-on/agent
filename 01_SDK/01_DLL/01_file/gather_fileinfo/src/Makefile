include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -fPIC

CPPFILES = FileInfo.cpp ElfInfo.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)
	$(CC) -shared $(CFLAGS) -o asi_fi.so $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

install:
	cp -f asi_fi.so /usr/local/ashin/nanny/dll
	cp -f asi_fi.so $(AS_PKG_HOME)/nanny/dll

clean:
	rm -f $(OBJ)
	rm -f asi_fi.so
