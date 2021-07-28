include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = ComExternMethod.cpp DateTime.cpp DownLoadFile.cpp DownLoadUtil.cpp HttpFileGetUtil.cpp MemToken.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f *.o
