STATIC_LIBRARIES += interp

VPATH+=src/interp

src/interp/interp_yacc.c : interp_yacc.y

src/interp/interp_lex.c : interp_lex.l

interp_C_SOURCES:= interp_callbacks.c interp_utility.c interp_ex.c \
interp_yacc.c interp_lex.c interp_compiletime.c interp_show.c

# Suppress compiler warnings
$(call add_extra_CFLAGS_macro,interp_lex.c interp_ex.c,-D_POSIX_C_SOURCE=199309L)

# Suppress analyzer warnings like:
#   src/interp/interp_lex.c: In function ‘yy_init_buffer’:
#   src/interp/interp_lex.c:1617:19: \
#     error: dereference of NULL ‘b_14(D)’ [CWE-476]
$(call add_extra_CFLAGS_macro,interp_lex.c,-Wno-analyzer-null-dereference)

# Suppress analyzer warnings like:
#   src/interp/interp_lex.c:1292:18: error: leak of ‘*b.yy_ch_buf’ [CWE-401] 
$(call add_extra_CFLAGS_macro,interp_lex.c,-Wno-analyzer-malloc-leak)

# Suppress sanitizer warnings like:
#   src/interp/interp_yacc.y:276:16: \
#     runtime error: index 1 out of bounds for type 'nodeTypeTag *[1]'
$(call add_extra_CFLAGS_macro,interp_yacc.c,-fno-sanitize=bounds-strict)

# Suppress analyzer warnings like:
#   src/interp/interp_yacc.c:453:21: error: leak of ‘yyptr’ [CWE-401]
$(call add_extra_CFLAGS_macro,interp_yacc.c,-Wno-analyzer-malloc-leak)

# Suppress analyzer warnings like:
#   src/interp/interp_yacc.c:1454:9: \
#     error: use of uninitialized value ‘*yyvsp_359 + _64’ [CWE-457]
$(call add_extra_CFLAGS_macro,interp_yacc.c,\
-Wno-analyzer-use-of-uninitialized-value)

CLEAN_FILES+=$(addprefix src/interp/interp_, lex.c lex.h yacc.c yacc.h)
