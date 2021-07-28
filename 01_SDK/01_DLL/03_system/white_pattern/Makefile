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

define inst_sub_dir
set -e;                                     \
    for instdir in $(INSTDIR); do                     \
        if [ -d "$${instdir}" ]; then                    \
            $(MAKE) $(@) --directory="$${instdir}"; \
        fi                              \
    done
endef

SUBDIR := dll_lib dll

INSTDIR := dll

.PHONY: all

all:
	@$(make_sub_dir)

clean:
	@$(make_sub_dir)

install:
	@$(inst_sub_dir)