/**
 * HEY THIS ISN'T DONE
 */


#ifndef __LIBINJECTION_XSS_H__
#define __LIBINJECTION_XSS_H__


#include <string.h> /* for size_t */


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/*-----------------------------------------------------------------------------------*/

int libinjection_is_xss(const char* data, size_t len, int flags);

/*-----------------------------------------------------------------------------------*/
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */


#endif /* __LIBINJECTION_XSS_H__ */

