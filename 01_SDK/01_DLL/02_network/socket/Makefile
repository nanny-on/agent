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

SUBDIR := common_util asi_cltsock asi_cltsock_lib asi_svrsock asi_svrsock_lib

INSTDIR := asi_cltsock asi_svrsock

.PHONY: all

all:
	@$(make_sub_dir)

clean:
	@$(make_sub_dir)

install:
	@$(inst_sub_dir)