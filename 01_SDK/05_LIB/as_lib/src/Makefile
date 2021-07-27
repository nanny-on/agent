include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -Wall -fPIC -shared -g

TARGET = libascomm.a

CPPFILES = base32.cpp etc.cpp odbc_util.cpp seedx.cpp str_token.cpp conf.cpp file.cpp parse.cpp sha1.cpp string.cpp daemon.cpp hash.cpp proc.cpp sha256.cpp sha512.cpp sysinfo.cpp db_class.cpp math.cpp registry.cpp signal.cpp time.cpp db_query.cpp net.cpp seed.cpp sqlite_util.cpp util.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	ar rscv $@ $(OBJ)
.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(TARGET) $(OBJ)
