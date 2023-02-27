# Programs in this directory
redblackTree_PROGRAMS:=redblackTree_interp
PROGRAMS+=$(redblackTree_PROGRAMS)

VPATH += src/redblackTree

redblackTree_interp_C_SOURCES := redblackTree.c redblackTree_wrapper.c

redblackTree_interp_STATIC_LIBRARIES := interp.a

$(call add_extra_CFLAGS_macro,$(redblackTree_interp_C_SOURCES),-Isrc/interp)
