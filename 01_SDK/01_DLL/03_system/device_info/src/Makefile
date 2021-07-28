include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -fPIC

CPPFILES = DeviceInfo.cpp DeviceInfoUtil.cpp DevInfo.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)
	$(CC) -shared $(CFLAGS) -o asi_devinfo.so $(OBJ) ../../../../../01_SDK/05_LIB/as_lib/src/libascomm.a

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

install:
	cp -f asi_devinfo.so /usr/local/ashin/nanny/dll
	cp -f asi_devinfo.so $(AS_PKG_HOME)/nanny/dll

clean:
	rm -f $(OBJ)
	rm -f asi_devinfo.so
