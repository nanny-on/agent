include ../../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -I.. -I../extension -I../hdr

CPPFILES = ASIDBFQMember.cpp ASIDBFQUtil.cpp QueryUtil.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
