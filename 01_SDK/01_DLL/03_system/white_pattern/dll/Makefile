include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -fPIC

CPPFILES = ElfFile.cpp FileAnalyze.cpp AriaEx.cpp AriaReal.cpp CRC32.cpp HashUtil.cpp WhiteEngineUtil.cpp WhitePatternUtil.cpp WhiteEngine.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)
	$(CC) -shared $(CFLAGS) -o asi_weng.so $(OBJ) ../../../../../01_SDK/05_LIB/as_lib/src/libascomm.a

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

install:
	cp -f asi_weng.so /usr/local/ashin/nanny/dll
	cp -f asi_weng.so $(AS_PKG_HOME)/nanny/dll

clean:
	rm -f $(OBJ)
	rm -f asi_weng.so
