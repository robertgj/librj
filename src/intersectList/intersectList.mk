# Programs in this directory
intersectList_PROGRAMS=intersectList_test intersectList_random_test \
segmentIntersection_test statusTree_test

PROGRAMS+=$(intersectList_PROGRAMS)

VPATH += src/intersectList

#
# intersectList_test
#

STATIC_LIBRARIES += libintersectList

libintersectList_C_SOURCES:=\
point.c segment.c segmentList.c eventQueue.c statusTree.c intersectList.c

$(call add_extra_CFLAGS_macro,$(libintersectList_C_SOURCES),-Isrc/interp)
intersectList_test_C_SOURCES := intersectList_test.c 
$(call add_extra_CFLAGS_macro,$(intersectList_test_C_SOURCES),-Isrc/interp)
intersectList_test_STATIC_LIBRARIES := libintersectList.a


intersectList_random_test_C_SOURCES := intersectList_random_test.c 
$(call add_extra_CFLAGS_macro,$(intersectList_random_test_C_SOURCES),\
-Isrc/interp)
intersectList_random_test_STATIC_LIBRARIES := libintersectList.a

segmentIntersection_test_C_SOURCES := segmentIntersection_test.c 
$(call add_extra_CFLAGS_macro,$(segmentIntersection_test_C_SOURCES),-Isrc/interp)
segmentIntersection_test_STATIC_LIBRARIES := libintersectList.a

statusTree_test_C_SOURCES := statusTree_test.c 
$(call add_extra_CFLAGS_macro,$(statusTree_test_C_SOURCES),-Isrc/interp)
statusTree_test_STATIC_LIBRARIES := libintersectList.a

