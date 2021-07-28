include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = SHA2Util.cpp Seedx.cpp SeedUtil.cpp StrEncrypt.cpp SecurityUtil.cpp SSLSockUtil.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)

