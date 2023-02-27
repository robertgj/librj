# Programs in this directory
bsTree_PROGRAMS:=bsTree_interp
PROGRAMS+=$(bsTree_PROGRAMS)

VPATH += src/bsTree

bsTree_interp_C_SOURCES := bsTree.c bsTree_wrapper.c

bsTree_interp_STATIC_LIBRARIES := interp.a

$(call add_extra_CFLAGS_macro,$(bsTree_interp_C_SOURCES),-Isrc/interp)
