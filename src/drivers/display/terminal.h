#ifndef H_TERMINALDRIVER
#define H_TERMINALDRIVER 1

#define PRINTF_ALIAS_STANDARD_FUNCTION_NAMES 1
#include <limine.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

static inline int printf(struct limine_terminal *term, const char *format, ...);

extern bool initialised;
extern char *resetcolour;

extern struct limine_terminal **terminals;
extern struct limine_terminal *default_trm;
extern uint64_t term_count;

extern void _terminal_print(const char *str, struct limine_terminal *term);
extern void terminal_print(const char *str);
extern void _terminal_printi(int num, struct limine_terminal *term);
extern void terminal_printi(int num);
extern void _terminal_printc(char c, struct limine_terminal *term);
extern void terminal_printc(char c);

extern void terminal_println(const char *str);
extern void terminal_print_info(const char *str);


extern void _terminal_reset(struct limine_terminal *term);
extern void terminal_reset();
extern void _terminal_clear(const char *ansii_colour, struct limine_terminal *term);
extern void terminal_clear(const char *ansii_colour);


extern void terminal_callback(struct limine_terminal *term, uint64_t type, uint64_t first, uint64_t second, uint64_t third);

extern void terminal_init();

#endif