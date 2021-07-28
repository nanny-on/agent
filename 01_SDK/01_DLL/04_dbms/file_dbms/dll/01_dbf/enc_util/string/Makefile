include ../../../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -I../../../common_header

CPPFILES = StrEncrypt.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
