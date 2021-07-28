include ../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

define make_sub_dir
set -e;										\
	for subdir in $(SUBDIR); do						\
		if [ -d "$${subdir}" ]; then					\
			$(MAKE) $(@) --directory="$${subdir}"; \
		fi								\
	done
endef


SUBDIR := env etc info log org po_etc po_fa po_fe po_in po_nc po_pms ptn site

.PHONY: all

all:
	@$(make_sub_dir)
	make dm_all
	
clean:
	@$(make_sub_dir)
	make dm_clean
	
dm_clean:
	rm -f *.o

dm_all:
	$(CC) $(CFLAGS) -c DBMgrBase.cpp FDBMgrBase.cpp
