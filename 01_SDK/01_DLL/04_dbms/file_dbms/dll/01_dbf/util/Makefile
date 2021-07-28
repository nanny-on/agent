include ../../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -I.. -I../enc_util -I../enc_util/seed -I../enc_util/seed_base -I../enc_util/sha2 -I../enc_util/string -I../../../common_header

CPPFILES = ASIDBFMember.cpp ASIDBFUtil.cpp ASIDBFUtil_Auth.cpp ASIDBFUtil_ComFn.cpp ASIDBFUtil_Database.cpp ASIDBFUtil_Init.cpp ASIDBFUtil_Table.cpp ASIDBFUtil_Table_Fast.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
