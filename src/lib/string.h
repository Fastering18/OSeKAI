#ifndef H_STRINGLIB
#define H_STRINGLIB 1

#include <terminal.h>
#include <stddef.h>

extern int isspace(const char str);

extern size_t strlen(const char *str);
extern char *strcpy(char *destination, const char *source);
extern char *strncpy(char *destination, const char *source, size_t n);
extern char *strcat(char *destination, const char *source);
extern char *strchr(const char *str, char ch);
extern int strcmp(const char *a, const char *b);
extern int strncmp(const char *a, const char *b, size_t n);

extern char* itoa(int num, char* str, int base);
char* ltoa(int64_t num, char* str, int base);

extern void strrev(unsigned char *str);

#endif