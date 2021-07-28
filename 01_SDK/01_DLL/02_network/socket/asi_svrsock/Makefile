include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -I../common_util -fPIC

CPPFILES = TCP_EventServer.cpp Server_DLL.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)
	$(CC) -shared $(CFLAGS) -o asi_svrsock.so ../common_util/*.o $(OBJ) ../../../../../01_SDK/05_LIB/as_lib/src/libascomm.a -L$(OPENSSL_PATH)/lib -lssl -lcrypto

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)
	
install:
	cp -f asi_svrsock.so /usr/local/ashin/nanny/dll
	cp -f asi_svrsock.so $(AS_PKG_HOME)/nanny/dll	

clean:
	rm -f $(OBJ)
	rm -f asi_svrsock.so

