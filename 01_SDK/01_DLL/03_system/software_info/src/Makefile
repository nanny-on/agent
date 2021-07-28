include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -fPIC

CPPFILES = SoftwareInfo.cpp SwInfoUtil.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)
	$(CC) -shared $(CFLAGS) -o asi_swinfo.so $(OBJ) ../../../../../01_SDK/05_LIB/as_lib/src/libascomm.a

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

install:
	cp -f asi_swinfo.so /usr/local/ashin/nanny/dll
	cp -f asi_swinfo.so $(AS_PKG_HOME)/nanny/dll
	
clean:
	rm -f $(OBJ)
	rm -f asi_swinfo.so
