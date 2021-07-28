include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -fPIC

CPPFILES = network.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)
	$(CC) -shared $(CFLAGS) -o asi_network.so $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

install:
	cp -f asi_network.so /usr/local/ashin/nanny/dll
	cp -f asi_network.so $(AS_PKG_HOME)/nanny/dll

clean:
	rm -f $(OBJ)
	rm -f asi_network.so
