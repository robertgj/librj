# Programs in this directory
splayTree_PROGRAMS:=splayTree_interp
PROGRAMS+=$(splayTree_PROGRAMS)

VPATH += src/splayTree

splayTree_interp_C_SOURCES := splayTree_wrapper.c splayTree.c

splayTree_interp_STATIC_LIBRARIES := interp.a

$(call add_extra_CFLAGS_macro,$(splayTree_interp_C_SOURCES),-Isrc/interp)
