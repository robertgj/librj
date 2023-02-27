# Programs in this directory
trbTree_PROGRAMS:=trbTree_interp
PROGRAMS+=$(trbTree_PROGRAMS)

VPATH += src/trbTree

trbTree_interp_C_SOURCES := trbTree.c trbTree_wrapper.c

trbTree_interp_STATIC_LIBRARIES := interp.a

$(call add_extra_CFLAGS_macro,$(trbTree_interp_C_SOURCES),-Isrc/interp)
