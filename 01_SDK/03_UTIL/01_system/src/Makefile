include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = EncodeUtil.cpp FileUtil.cpp HostToAddrUtil.cpp MemoryFile.cpp PowerUtil.cpp ProcInfo.cpp ProcUtil.cpp RegUtil.cpp ResUtil.cpp SystemInfo.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
