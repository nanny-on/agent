include ../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES)

define make_sub_dir
set -e;										\
	for subdir in $(SUBDIR); do						\
		if [ -d "$${subdir}" ]; then					\
			$(MAKE) $(@) --directory="$${subdir}"; \
		fi								\
	done
endef

SUBDIR := exe_test_pgm make_test_pgm write_test_pgm

.PHONY: all

all:
	@$(make_sub_dir)

clean:
	@$(make_sub_dir)

install:
	@$(make_sub_dir)