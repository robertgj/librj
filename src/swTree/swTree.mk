# Programs in this directory
swTree_PROGRAMS:=swTree_lg_test swTree_interp
PROGRAMS+=$(swTree_PROGRAMS)

VPATH += src/swTree

swTree_lg_test_C_SOURCES := swTree_lg_test.c swTree_lg.c 

swTree_interp_C_SOURCES := swTree.c swTree_wrapper.c swTree_lg.c

swTree_interp_STATIC_LIBRARIES := interp.a

$(call add_extra_CFLAGS_macro,$(swTree_interp_C_SOURCES),-Isrc/interp)
