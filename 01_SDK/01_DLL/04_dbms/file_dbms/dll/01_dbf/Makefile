include ../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -I./util -I./enc_util -I./enc_util/seed -I./enc_util/sha2  -I./enc_util/string -fPIC

define make_sub_dir
set -e;										\
	for subdir in $(SUBDIR); do						\
		if [ -d "$${subdir}" ]; then					\
			$(MAKE) $(@) --directory="$${subdir}"; \
		fi								\
	done
endef


SUBDIR := util enc_util

.PHONY: all

CPPFILES = ASIDBF.cpp

OBJ = $(CPPFILES:.cpp=.o)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

all:
	@$(make_sub_dir)
	make dll_all

install:
	cp -f asi_dbf.so /usr/local/ashin/nanny/dll
	cp -f asi_dbf.so $(AS_PKG_HOME)/nanny/dll

clean:
	@$(make_sub_dir)
	make dll_clean

dll_all: $(OBJ)
	$(CC) -shared $(CFLAGS) -o asi_dbf.so $(OBJ) ./enc_util/SecurityUtil.o ./enc_util/seed/Seedx.o ./enc_util/seed/SeedUtil.o ./enc_util/sha2/SHA2Util.o ./enc_util/string/StrEncrypt.o ./util/ASIDBFMember.o ./util/ASIDBFUtil.o ./util/ASIDBFUtil_Auth.o ./util/ASIDBFUtil_ComFn.o ./util/ASIDBFUtil_Database.o ./util/ASIDBFUtil_Init.o ./util/ASIDBFUtil_Table.o  ./util/ASIDBFUtil_Table_Fast.o ../../../../../01_GLB_W32/src/asi_glb_w32_file.o ../../../../../01_GLB_W32/src/asi_glb_w32_handle.o ../../../../../../01_SDK/01_BASE/lv_00/src/MutexExt.o ../../../../../../01_SDK/01_BASE/lv_01_global/src/LogUtil.o ../../../../../../01_SDK/03_UTIL/01_system/src/FileUtil.o ../../../../../../01_SDK/05_LIB/as_lib/src/libascomm.a -luuid -ldl

dll_clean:
	rm -f $(OBJ)
	rm -f asi_dbf.so
	

