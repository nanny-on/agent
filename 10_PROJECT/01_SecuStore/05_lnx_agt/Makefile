include ../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

define make_sub_dir
set -e;										\
	for subdir in $(SUBDIR); do						\
		if [ -d "$${subdir}" ]; then					\
			$(MAKE) $(@) --directory="$${subdir}";	\
		fi								\
	done
endef

SUBDIR := extension util manage thread dbms_manage manage logic

.PHONY: all

all:
	@$(make_sub_dir)
	make sys_all

install:
	cp -f nannysvc /usr/local/ashin/nanny/sbin
	cp -f nannysvc $(AS_PKG_HOME)/nanny/sbin
	
clean:
	@$(make_sub_dir)
	make sys_clean

sys_clean:
	rm -f nannysvc nannysvc.debug
	rm -f *.o

sys_all:
	$(CC) $(CFLAGS) -c Environment.cpp EnvironmentOp.cpp ComExternObject.cpp MainDlg.cpp MainDlg_Class.cpp MainDlg_DBSync.cpp MainDlg_SockEvt.cpp MainDlg_Util.cpp MainDlg_WMEvt.cpp NannyAgent.cpp
	$(CC) $(CFLAGS) -o nannysvc *.o extension/*.o dbms_manage/env/*.o dbms_manage/etc/*.o dbms_manage/info/*.o dbms_manage/log/*.o dbms_manage/org/*.o dbms_manage/po_etc/*.o dbms_manage/po_fa/*.o dbms_manage/po_fe/*.o dbms_manage/po_in/devo/*.o dbms_manage/po_in/ptn/*.o dbms_manage/po_in/rs/*.o dbms_manage/po_in/vuln/*.o dbms_manage/po_nc/*.o dbms_manage/po_pms/*.o dbms_manage/ptn/*.o dbms_manage/site/*.o dbms_manage/*.o manage/*.o manage/db/env/*.o manage/db/etc/*.o manage/db/info/*.o manage/db/log/*.o manage/db/org/*.o manage/db/po_etc/*.o manage/db/po_fa/*.o manage/db/po_fe/*.o manage/db/po_in/devo/*.o manage/db/po_in/ptn/*.o manage/db/po_in/rs/*.o manage/db/po_in/vuln/*.o manage/db/po_nc/*.o manage/db/po_pms/*.o manage/db/ptn/*.o manage/db/site/*.o manage/mem/*.o logic/base/*.o logic/impl/*.o logic/lgn/*.o logic/link/*.o logic/mgr/ctrl/*.o logic/mgr/env/*.o logic/mgr/info/*.o logic/mgr/log/*.o logic/mgr/org/*.o logic/mgr/po_etc/*.o logic/mgr/po_fa/*.o logic/mgr/po_fe/*.o logic/mgr/po_in/devo/*.o logic/mgr/po_in/ptn/*.o logic/mgr/po_in/rs/*.o logic/mgr/po_in/vuln/*.o logic/mgr/po_nc/*.o logic/mgr/po_pms/*.o logic/mgr/ptn_in/*.o logic/mgr/ptn_nc/*.o logic/mgr/site/*.o logic/mgr/*.o logic/udt/*.o logic/util/*.o thread/*.o util/*.o ../../../01_SDK/01_BASE/lv_00/src/*.o ../../../01_SDK/01_BASE/lv_01_global/src/*.o ../../../01_SDK/01_GLB_W32/src/*.o ../../../01_SDK/01_BASE/lv_02_util/src/*.o ../../../01_SDK/01_DLL/01_file/compress/dll_lib/*.o ../../../01_SDK/01_DLL/01_file/find_file/dll_lib/*.o ../../../01_SDK/01_DLL/01_file/gather_fileinfo/src_lib/*.o ../../../01_SDK/01_DLL/01_file/remove_file/src_lib/*.o ../../../01_SDK/01_DLL/02_network/network/dll_lib/*.o ../../../01_SDK/01_DLL/02_network/socket/asi_cltsock_lib/*.o ../../../01_SDK/01_DLL/02_network/socket/asi_svrsock_lib/*.o ../../../01_SDK/01_DLL/03_system/device_info/src_lib/*.o ../../../01_SDK/01_DLL/03_system/res_info/dll_lib/*.o ../../../01_SDK/01_DLL/03_system/res_usage_info/src_lib/*.o ../../../01_SDK/01_DLL/03_system/software_info/src_lib/*.o ../../../01_SDK/01_DLL/03_system/white_pattern/dll_lib/*.o ../../../01_SDK/01_DLL/04_dbms/file_dbms/dll/01_dbf_lib/*.o ../../../01_SDK/01_DLL/04_dbms/file_dbms/dll/02_dbfq_lib/*.o ../../../01_SDK/01_DLL/05_encrypt/seed/src_lib/*.o ../../../01_SDK/03_UTIL/01_system/src/*.o ../../../01_SDK/05_LIB/as_lib/src/libascomm.a -luuid -lodbc -ldl -lpthread -lrt -static-libgcc -static-libstdc++
#	objcopy --only-keep-debug nannysvc nannysvc.debug
#	objcopy --strip-debug nannysvc
