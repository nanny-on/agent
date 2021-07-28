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


SUBDIR := db mem

.PHONY: all

all:
	@$(make_sub_dir)
	make mgr_all
	
clean:
	@$(make_sub_dir)
	make mgr_clean
	
mgr_clean:
	rm -f *.o

mgr_all:
	$(CC) $(CFLAGS) -c ManageBaseUtil.cpp Manage_Struct.cpp