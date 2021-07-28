include ../../../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -I../../../common_header

CPPFILES = Seedx.cpp SeedUtil.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
