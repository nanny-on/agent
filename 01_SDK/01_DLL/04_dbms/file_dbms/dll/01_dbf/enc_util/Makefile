include ../../../../../../../build.config

CFLAGS = $(DEFINES) $(INCLUDES) -I.. -I./seed -I./sha2 -I./string

define make_sub_dir
set -e;										\
	for subdir in $(SUBDIR); do						\
		if [ -d "$${subdir}" ]; then					\
			$(MAKE) $(@) --directory="$${subdir}"; \
		fi								\
	done
endef

SUBDIR := seed sha2 string

.PHONY: all

CPPFILES = SecurityUtil.cpp

OBJ = $(CPPFILES:.cpp=.o)

.cpp.o:
	$(CC) $(CFLAGS) -c $(CPPFILES)

all:
	@$(make_sub_dir)
	make obj_all

clean:
	@$(make_sub_dir)
	make obj_clean

obj_all: $(OBJ)

obj_clean:
	rm -f $(OBJ)