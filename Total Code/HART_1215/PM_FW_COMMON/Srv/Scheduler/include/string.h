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

#ifdef __cplusplus
extern "C" {
#endif

#define EOS '\0'

void    *memccpy(void *restrict, const void *restrict, int, unsigned int);
void    *memchr(const void *, int, unsigned int);
int      memcmp(const void *, const void *, unsigned int);
void    *memcpy(void *restrict, const void *restrict, unsigned int);
void    *memmove(void *, const void *, unsigned int);
void    *memset(void *, int, unsigned int);
char    *stpcpy(char *restrict, const char *restrict);
char    *stpncpy(char *restrict, const char *restrict, unsigned int);
char    *strcat(char *restrict, const char *restrict);
char    *strchr(const char *, int);
int      strcmp(const char *, const char *);
int      strcoll(const char *, const char *);
char    *strcpy(char *restrict, const char *restrict);
unsigned int   strcspn(const char *, const char *);
char    *strdup(const char *);
char    *strerror(int);
int      strerror_r(int, char *, unsigned int);
unsigned int   strlen(const char *);
char    *strncat(char *restrict, const char *restrict, unsigned int);
int      strncmp(const char *, const char *, unsigned int);
char    *strncpy(char *restrict, const char *restrict, unsigned int);
char    *strndup(const char *, unsigned int);
unsigned int   strnlen(const char *, unsigned int);
char    *strpbrk(const char *, const char *);
char    *strrchr(const char *, int);
char    *strsignal(int);
unsigned int   strspn(const char *, const char *);
char    *strstr(const char *, const char *);
char    *strtok(char *restrict, const char *restrict);
char    *strtok_r(char *restrict, const char *restrict, char **restrict);
unsigned int   strxfrm(char *restrict, const char *restrict, unsigned int);

int      bcmp(const void *, const void * , unsigned int);
void     binvert(void *, unsigned int);
void     bswap(void *, void * , unsigned int);
void     swap(void *, void * , unsigned int);
void     uswap(void *, void * , unsigned int);
void     bzero(void*, unsigned int);
void     bcopy(const void *, void * , unsigned int);
void     bcopyBytes(const void *, void * , unsigned int);
void     bcopyWords(const void *, void * , unsigned int);
void     bcopyLongs(const void *, void * , unsigned int);
void     bfill(void *, unsigned int , int);
void     bfillBytes(void *, unsigned int , int);
char    *index(const char *, int);
char    *rindex(const char *, int);

#ifdef __cplusplus
}
#endif

#endif
