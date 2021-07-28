include ../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

define make_sub_dir
set -e;										\
	for subdir in $(SUBDIR); do						\
		if [ -d "$${subdir}" ]; then					\
			$(MAKE) $(@) --directory="$${subdir}"; \
		fi								\
	done
endef

SUBDIR := lv_00 lv_01_global lv_02_util

.PHONY: all

all:
	@$(make_sub_dir)

clean:
	@$(make_sub_dir)