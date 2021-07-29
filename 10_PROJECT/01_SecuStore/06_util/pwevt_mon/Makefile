include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = pwevt_timer.cpp pwevt_thread_base.cpp pwevt_thread.cpp pwevt_dlg.cpp pwevt_mon.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)
	$(CC) $(CFLAGS) -g -o pwevt_mon $(OBJ) ../../../../01_SDK/01_BASE/lv_00/src/SemExt.o ../../../../01_SDK/05_LIB/as_lib/src/libascomm.a -lpthread -ldl -luuid -static-libgcc -static-libstdc++

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
	rm -f pwevt_mon

install:
	cp -f pwevt_mon /usr/local/ashin/nanny/sbin
	cp -f pwevt_mon $(AS_PKG_HOME)/nanny/sbin