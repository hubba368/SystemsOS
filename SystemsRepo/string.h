#ifndef _STRING_H_
#define _STRING_H_

#include <size_t.h>
#include <_null.h>
#include <errno.h>

// Compares string pointers str1 and str2
int strcmp(const char* str1, const char* str2); 

// Copies string pointed by source to destination
char * strcpy(char * destination, const char * source);

// variant of strcpy that detects various errors such as buffer overflows 
errno_t strcpy_s(char *destination, size_t numberOfElements, const char *source);

// Computes the length of the string source until a null terminator is detected
size_t strlen(const char* source);

// Copies n characters from src to dest
void * memcpy(void * destination, const void * source, size_t count);

// variant of memcpy that detects various errors such as buffer overflows 
errno_t memcpy_s(void *destination, size_t destinationSize, const void *source, size_t count);

// Copies the character c to the first n character of the string pointed to, by the argument str.
void * memset(void *destination, char val, size_t count);

// Copies the character c to the first n character of the string pointed to, by the argument str.
unsigned short * memsetw(unsigned short * destination, unsigned short val, size_t count);

#endif