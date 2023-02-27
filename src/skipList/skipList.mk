# Programs in this directory
skipList_PROGRAMS:=skipList_interp
PROGRAMS+=$(skipList_PROGRAMS)

VPATH += src/skipList

skipList_interp_C_SOURCES := jsw_slib.c jsw_rand.c skipList.c skipList_wrapper.c

skipList_interp_STATIC_LIBRARIES := interp.a

$(call add_extra_CFLAGS_macro,$(skipList_interp_C_SOURCES),-Isrc/interp)
