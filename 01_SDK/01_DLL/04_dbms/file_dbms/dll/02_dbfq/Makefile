include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -I./util -I./hdr -I./extension -fPIC

define make_sub_dir
set -e;										\
	for subdir in $(SUBDIR); do						\
		if [ -d "$${subdir}" ]; then					\
			$(MAKE) $(@) --directory="$${subdir}"; \
		fi								\
	done
endef


SUBDIR := util extension

.PHONY: all

CPPFILES = ASIDBFQ.cpp

OBJ = $(CPPFILES:.cpp=.o)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

all:
	@$(make_sub_dir)
	make dll_all

install:
	cp -f asi_dbfq.so /usr/local/ashin/nanny/dll
	cp -f asi_dbfq.so $(AS_PKG_HOME)/nanny/dll

clean:
	@$(make_sub_dir)
	make dll_clean

dll_all: $(OBJ)
	$(CC) -shared $(CFLAGS) -o asi_dbfq.so $(OBJ) ./util/ASIDBFQMember.o ./util/ASIDBFQUtil.o ./util/QueryUtil.o ./extension/TokenQuery.o ../01_dbf_lib/*.o ../../../../../../01_SDK/01_BASE/lv_00/src/MutexExt.o ../../../../../../01_SDK/01_BASE/lv_01_global/src/LogUtil.o ../../../../../../01_SDK/01_BASE/lv_02_util/src/TokenCString.o ../../../../../../01_SDK/05_LIB/as_lib/src/libascomm.a -luuid -ldl

dll_clean:
	rm -f $(OBJ)
	rm -f asi_dbfq.so
	

