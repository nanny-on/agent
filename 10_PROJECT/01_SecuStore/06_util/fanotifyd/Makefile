include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = fanotify_timer.cpp fanotify_thread_base.cpp fanotify_thread.cpp fanotify_thread_event.cpp fanotify_dlg.cpp fanotify_mon.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)
	$(CC) $(CFLAGS) -g -o fanotifyd $(OBJ) ../../../../01_SDK/01_BASE/lv_00/src/SemExt.o ../../../../01_SDK/05_LIB/as_lib/src/libascomm.a -lpthread -ldl -luuid -static-libgcc -static-libstdc++

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
	rm -f fanotifyd

install:
	cp -f fanotifyd /usr/local/ashin/nanny/sbin
	cp -f fanotifyd $(AS_PKG_HOME)/nanny/sbin