include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -fPIC

CFILES = rm_log.cpp

OBJ = $(CFILES:.cpp=.o)

all: $(OBJ)
	$(CC) -shared $(CFLAGS) -o asi_rmlog.so $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CFILES)

install:
	cp -f asi_rmlog.so /usr/local/ashin/nanny/dll
	cp -f asi_rmlog.so $(AS_PKG_HOME)/nanny/dll

clean:
	rm -f $(OBJ)
	rm -f asi_rmlog.so
