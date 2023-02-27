# Programs in this directory
splayCache_PROGRAMS:=splayCache_interp
PROGRAMS+=$(splayCache_PROGRAMS)

VPATH += src/splayCache

splayCache_interp_C_SOURCES := splayCache_wrapper.c splayCache.c

splayCache_interp_STATIC_LIBRARIES := interp.a

$(call add_extra_CFLAGS_macro,$(splayCache_interp_C_SOURCES),-Isrc/interp)
