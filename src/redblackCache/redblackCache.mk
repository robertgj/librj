# Programs in this directory
redblackCache_PROGRAMS:=redblackCache_interp
PROGRAMS+=$(redblackCache_PROGRAMS)

VPATH += src/redblackCache src/redblackTree

redblackCache_interp_C_SOURCES := \
redblackCache.c redblackCache_wrapper.c redblackTree.c

redblackCache_interp_STATIC_LIBRARIES := interp.a

$(call add_extra_CFLAGS_macro,$(redblackCache_interp_C_SOURCES),\
-Isrc/interp -Isrc/redblackTree)
