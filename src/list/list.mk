# Programs in this directory
list_PROGRAMS:=list_interp
PROGRAMS+=$(list_PROGRAMS)

VPATH += src/list

list_interp_C_SOURCES := list.c list_wrapper.c

list_interp_STATIC_LIBRARIES := interp.a

$(call add_extra_CFLAGS_macro,$(list_interp_C_SOURCES),-Isrc/interp)
