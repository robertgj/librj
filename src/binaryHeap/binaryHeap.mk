# Programs in this directory
binaryHeap_PROGRAMS:=binaryHeap_interp
PROGRAMS+=$(binaryHeap_PROGRAMS)

VPATH += src/binaryHeap

binaryHeap_interp_C_SOURCES := binaryHeap.c binaryHeap_wrapper.c

binaryHeap_interp_STATIC_LIBRARIES := interp.a

$(call add_extra_CFLAGS_macro,$(binaryHeap_interp_C_SOURCES),-Isrc/interp)
