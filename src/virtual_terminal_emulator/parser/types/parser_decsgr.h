


#if !defined(_VTE_SGR) || !defined(_VTE_NGR)
#error "Must define _VTE_SGR and _VTE_NGR before including this file"
#endif

#define SGR(set, name, value) _VTE_SGR(set##_##name, value)
#define NGR(set, name, value) _VTE_NGR(set##_##name, value)

NGR(RESET, ALL, 0)
NGR(SET, SUPERSCRIPT, 4)
NGR(SET, SUBSCRIPT, 5)
NGR(SET, OVERLINE, 6)
NGR(SET, TRANSPARENCY, 8)
NGR(RESET, SUPERSUBSCRIPT, 24)
NGR(RESET, OVERLINE, 26)
NGR(RESET, TRANSPARENCY, 28)

#undef SGR
#undef NGR
