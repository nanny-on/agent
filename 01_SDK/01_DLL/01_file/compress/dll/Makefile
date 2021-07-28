include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -fPIC -fomit-frame-pointer

CPPFILES = Compress.cpp CompSecuUtil.cpp CompUtil.cpp CompMemToken.cpp FileToken.cpp MiniLzo.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)
	$(CC) -shared $(CFLAGS) -o asi_comp.so $(OBJ) ../../../../../01_SDK/01_BASE/lv_00/src/ComGlobalMethod.o ../../../../../01_SDK/03_UTIL/01_system/src/FileUtil.o ../../../../../01_SDK/01_BASE/lv_02_util/src/TokenString.o ../../../../../01_SDK/05_LIB/as_lib/src/libascomm.a

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

install:
	cp -f asi_comp.so /usr/local/ashin/nanny/dll
	cp -f asi_comp.so $(AS_PKG_HOME)/nanny/dll

clean:
	rm -f $(OBJ)
	rm -f asi_comp.so