/**
 * Copyright 2012-2016 Nick Galbreath
 */

#ifndef __LIBINJECTION_H__
#define __LIBINJECTION_H__


#include <string.h> /* for size_t */


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/*-----------------------------------------------------------------------------------*/

/**
 * Version info.
 */
const char* libinjection_version(void);


/**
 * Simple API for SQLi detection - returns a SQLi fingerprint or NULL
 *
 * \param[in]  s      input string, may contain nulls, does not need to be null-terminated
 * \param[in]  slen input  string length
 * \param[out] fingerprint buffer of 8+ characters.  c-string,
 * \return 1 if SQLi, 0 if benign.  fingerprint will be set or set to empty string.
 */
int libinjection_sqli(const char* data, size_t len, char fingerprint[]);


/** 
 * ALPHA version of xss detector.
 *
 * NOT DONE.
 *
 * \param[in] s    input string, may contain nulls, does not need to be null-terminated
 * \param[in] slen input string length
 * \return 1 if XSS found, 0 if benign
 *
 */
int libinjection_xss(const char* s, size_t slen);


/*-----------------------------------------------------------------------------------*/
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */


#endif /* __LIBINJECTION_H__ */

