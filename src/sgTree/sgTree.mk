# Programs in this directory
sgTree_PROGRAMS:=sgTree_interp
PROGRAMS+=$(sgTree_PROGRAMS)

VPATH += src/sgTree

sgTree_interp_C_SOURCES := sgTree.c sgTree_wrapper.c stack.c

sgTree_interp_STATIC_LIBRARIES := interp.a

$(call add_extra_CFLAGS_macro,$(sgTree_interp_C_SOURCES),\
-Isrc/stack -Isrc/interp)
