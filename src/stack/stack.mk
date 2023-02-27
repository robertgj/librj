# Programs in this directory
stack_PROGRAMS:=stack_interp
PROGRAMS+=$(stack_PROGRAMS)

VPATH += src/stack

stack_interp_C_SOURCES := stack_wrapper.c stack.c

stack_interp_STATIC_LIBRARIES := interp.a

$(call add_extra_CFLAGS_macro,$(stack_interp_C_SOURCES),-Isrc/interp)
