/* string.h - string library header file */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:
 * Copyright (c) 2013, 2014 CAG Systems, Inc.
 *------------------------------------------------------------
 *
 *    @@@@     @@@      @@@@@       ####      ######
 *  @@       @@  @@    @@         ##    ##   ##
 * @@       @@    @@  @@   @@@@  ##      ## ######
 * @@      @@@@@@@@@@ @@    @@    ##    ##      ##
 *  @@@@@ @@@      @@@  @@@@@@      ####  #######
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable CAG license agreement.
 *
 * 1 tab == 4 spaces!
 *------------------------------------------------------------
 * modification history
 * --------------------
 * 03jun15,Jeff created
 *
 * DESCRIPTION
 * This file contains.
 */

#ifndef __INCstringh
#define __INCstringh

#include <libc/stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EOS '\0'

void    *memccpy(void *restrict, const void *restrict, int, size_t);
void    *memchr(const void *, int, size_t);
int      memcmp(const void *, const void *, size_t);
void    *memcpy(void *restrict, const void *restrict, size_t);
void    *memmove(void *, const void *, size_t);
void    *memset(void *, int, size_t);

int      strcmp(const char *, const char *);
char    *strcpy(char *restrict, const char *restrict);
size_t   strlen(const char *);


char    *stpncpy(char *restrict, const char *restrict, size_t);
char    *strcat(char *restrict, const char *restrict);
char    *strchr(const char *, int);
char    *stpcpy(char *restrict, const char *restrict);
int      strcoll(const char *, const char *);
size_t   strcspn(const char *, const char *);
char    *strdup(const char *);
char    *strerror(int);
int      strerror_r(int, char *, size_t);

char    *strncat(char *restrict, const char *restrict, size_t);
int      strncmp(const char *, const char *, size_t);
char    *strncpy(char *restrict, const char *restrict, size_t);
char    *strndup(const char *, size_t);
size_t   strnlen(const char *, size_t);
char    *strpbrk(const char *, const char *);
char    *strrchr(const char *, int);
char    *strsignal(int);
size_t   strspn(const char *, const char *);
char    *strstr(const char *, const char *);
char    *strtok(char *restrict, const char *restrict);
char    *strtok_r(char *restrict, const char *restrict, char **restrict);
size_t   strxfrm(char *restrict, const char *restrict, size_t);

int      bcmp(const void *, const void * , size_t);
void     binvert(void *, size_t);
void     bswap(void *, void * , size_t);
void     swap(void *, void * , size_t);
void     uswap(void *, void * , size_t);
void     bzero(void*, size_t);
void     bcopy(const void *, void * , size_t);
void     bcopyBytes(const void *, void * , size_t);
void     bcopyWords(const void *, void * , size_t);
void     bcopyLongs(const void *, void * , size_t);
void     bfill(void *, size_t , int);
void     bfillBytes(void *, size_t , int);
char    *index(const char *, int);
char    *rindex(const char *, int);

#ifdef __cplusplus
}
#endif

#endif
