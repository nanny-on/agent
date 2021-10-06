include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

CPPFILES = accnotify_timer.cpp accnotify_thread_base.cpp accnotify_thread.cpp accnotify_thread_event.cpp accnotify_thread_policy.cpp accnotify_dlg.cpp accnotify_mon.cpp

OBJ = $(CPPFILES:.cpp=.o)

all: $(OBJ)
	$(CC) $(CFLAGS) -g -o accnotifyd $(OBJ) ../../../../01_SDK/01_BASE/lv_00/src/SemExt.o ../../../../01_SDK/01_GLB_W32/src/LnxString.o ../../../../01_SDK/05_LIB/as_lib/src/libascomm.a -lpthread -ldl -luuid -static-libgcc -static-libstdc++

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

clean:
	rm -f $(OBJ)
	rm -f accnotifyd

install:
	cp -f accnotifyd /usr/local/ashin/nanny/sbin
	cp -f accnotifyd $(AS_PKG_HOME)/nanny/sbin