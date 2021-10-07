include ../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = asi_glb_w32_file.cpp asi_glb_w32_handle.cpp LnxString.cpp LnxStringW.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
