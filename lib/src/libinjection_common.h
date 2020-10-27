/**
 * Copyright 2012-2016 Nick Galbreath
 */

#ifndef __LIBINJECTION_COMMON_H__
#define __LIBINJECTION_COMMON_H__


#include <stdio.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/*-----------------------------------------------------------------------------------*/
#define LI_TRUE  (1)
#define LI_FALSE (0)


#if defined (__GNUC__)
/* provide the compiler with branch prediction information */
#define likely(x)     __builtin_expect(!!(x), 1)
#define unlikely(x)   __builtin_expect(!!(x), 0)
#else   /* __GNUC__ */
#define likely(x)     (x)
#define unlikely(x)   (x)
#endif   /* __GNUC__ */


#if defined(_WIN32)
#define EXTERN    __cdecl
#else
#define EXTERN
#endif


/* clear a struct data */
#define struct_clear(_st) memset(_st, 0x00, sizeof(*_st))


/*################ DEBUG #####################*/
#ifdef __DEBUG
#define Trace(fmt, ...) printf(stderr, "[%s:%d]"fmt"\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else  /* __DEBUG */
#define Trace(fmt, ...)
#endif /* __DEBUG */

/*-----------------------------------------------------------------------------------*/
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */


#endif /* __LIBINJECTION_COMMON_H__ */

