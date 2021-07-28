include ../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -I./thread -fPIC

define make_sub_dir
set -e;										\
	for subdir in $(SUBDIR); do						\
		if [ -d "$${subdir}" ]; then					\
			$(MAKE) $(@) --directory="$${subdir}"; \
		fi								\
	done
endef


SUBDIR := thread

.PHONY: all

CPPFILES = FindFile.cpp FindFileUtil.cpp

OBJ = $(CPPFILES:.cpp=.o)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

all:
	@$(make_sub_dir)
	make dll_all

install:
	cp -f asi_ff.so /usr/local/ashin/nanny/dll
	cp -f asi_ff.so $(AS_PKG_HOME)/nanny/dll

clean:
	@$(make_sub_dir)
	make dll_clean

dll_all: $(OBJ)
	$(CC) -shared $(CFLAGS) -o asi_ff.so $(OBJ) ../../../../../01_SDK/01_BASE/lv_00/src/MutexExt.o ../../../../../01_SDK/01_BASE/lv_00/src/SemExt.o ../../../../../01_SDK/01_BASE/lv_02_util/src/TokenString.o ../../../../../01_SDK/01_BASE/lv_01_global/src/LogUtil.o ./thread/ThreadBase.o ../../doc_file_fmt/src_lib/DocFileFmtDLLUtil.o ../../gather_fileinfo/src_lib/FileInfoDLLUtil.o ./thread/ThreadFinder.o ../../../../../01_SDK/05_LIB/as_lib/src/libascomm.a -lpthread

dll_clean:
	rm -f $(OBJ)
	rm -f asi_ff.so
	

