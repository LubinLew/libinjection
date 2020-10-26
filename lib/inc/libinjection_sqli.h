/**
 * Copyright 2012-2016 Nick Galbreath
 */

#ifndef __LIBINJECTION_SQLI_H__
#define __LIBINJECTION_SQLI_H__


#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/*-----------------------------------------------------------------------------------*/


#define MAX_FINGERPRINT_LEN (8)

typedef enum sqli_flags {
    FLAG_NONE          = 0x00,
    FLAG_QUOTE_NONE    = 0x01,   /* 1 << 0 */
    FLAG_QUOTE_SINGLE  = 0x02,   /* 1 << 1 */
    FLAG_QUOTE_DOUBLE  = 0x04,   /* 1 << 2 */
    FLAG_SQL_ANSI      = 0x08,   /* 1 << 3 */
    FLAG_SQL_MYSQL     = 0x10    /* 1 << 4 */
} sqli_flags_e;


typedef enum lookup_type {
    LOOKUP_WORD        = 1,
    LOOKUP_TYPE        = 2,
    LOOKUP_OPERATOR    = 3,
    LOOKUP_FINGERPRINT = 4
} lookup_type_e;


typedef struct libinjection_sqli_token {
    /*
     * position and length of token
     * in original string
     */
    size_t pos;
    size_t len;

    /*  count:
     *  in type 'v', used for number of opening '@'
     *  but maybe used in other contexts
     */
    int  count;

    char type;
    char str_open;
    char str_close;
    char val[32];
} sqli_token_t;


/**
 * Pointer to function, takes c-string input,
 *  returns '\0' for no match, else a char
 */
typedef struct libinjection_sqli_state sqli_state_t;
typedef char (*ptr_lookup_fn)(sqli_state_t* state, int lookuptype, const char* word, size_t len);

struct libinjection_sqli_state {
    /* input data */
    const char *s;      /* input data pointer, does not need to be null terminated. it is also not modified. */
    size_t      slen;   /* input length */

    /* How to lookup a word or fingerprint */
    ptr_lookup_fn  lookup;
    void          *userdata;

    int flags;

    /* pos is the index in the string during tokenization */
    size_t pos;

    /* MAX TOKENS + 1 since we use one extra token  to determine the type of the previous token */
    sqli_token_t tokenvec[8];

    /* Pointer to token position in tokenvec, above */
    sqli_token_t* current;

    /* fingerprint pattern c-string +1 for ending null
     * Minimum of 8 bytes to add gcc's -fstack-protector to work */
    char   fingerprint[MAX_FINGERPRINT_LEN];
    size_t fingerprint_length;

    /*
     * Line number of code that said decided if the input was SQLi or
     * not.  Most of the time it's line that said "it's not a matching
     * fingerprint" but there is other logic that sometimes approves
     * an input. This is only useful for debugging.
     *
     */
    int reason;

    /* Number of ddw (dash-dash-white) comments
     * These comments are in the form of  '--[whitespace]' or '--[EOF]'
     * All databases treat this as a comment.
     */
     int stats_comment_ddw;

    /* Number of ddx (dash-dash-[notwhite]) comments
     * ANSI SQL treats these are comments, MySQL treats this as
     * two unary operators '-' '-'
     * If you are parsing result returns FALSE and
     * stats_comment_dd > 0, you should reparse with COMMENT_MYSQL
     */
    int stats_comment_ddx;

    /* c-style comments found /x .. x/ */
    int stats_comment_c;

    /* '#' operators or MySQL EOL comments found */
    int stats_comment_hash;

    /* number of tokens folded away */
    int stats_folds;

    /* total tokens processed */
    int stats_tokens;
};

sqli_token_t* libinjection_sqli_get_token(sqli_state_t* sqlistate, int i);

/*
 * Version info.
 *
 * This is moved into a function to allow SWIG and other auto-generated
 * binding to not be modified during minor release changes.  We change
 * change the version number in the c source file, and not regenerated
 * the binding
 *
 * See python's normalized version
 * http://www.python.org/dev/peps/pep-0386/#normalizedversion
 */
const char* libinjection_version(void);

/**
 *
 */
void libinjection_sqli_init(sqli_state_t* sql_state,
                            const char* s, size_t slen,
                            int flags);

/**
 * Main API: tests for SQLi in three possible contexts, no quotes,
 * single quote and double quote
 *
 * \param sql_state core data structure
 *
 * \return 1 (true) if SQLi, 0 (false) if benign
 */
int libinjection_is_sqli(sqli_state_t* sql_state);

/*  FOR HACKERS ONLY
 *   provides deep hooks into the decision making process
 */
void libinjection_sqli_callback(sqli_state_t*  sql_state,
                                ptr_lookup_fn fn, void* userdata);


/*
 * Resets state, but keeps initial string and callbacks
 */
void libinjection_sqli_reset(sqli_state_t* sql_state, int flags);

/**
 *
 */

/**
 * This detects SQLi in a single context, mostly useful for custom
 * logic and debugging.
 *
 * \param sql_state  Main data structure
 * \param flags flags to adjust parsing
 *
 * \returns a pointer to sfilter.fingerprint as convenience
 *          do not free!
 *
 */
const char* libinjection_sqli_fingerprint(sqli_state_t* sql_state, int flags);

/**
 * The default "word" to token-type or fingerprint function.  This
 * uses a ASCII case-insensitive binary tree.
 */
char libinjection_sqli_lookup_word(sqli_state_t* sql_state,
                                   int lookup_type, const char* s, size_t slen);

/* Streaming tokenization interface.
 *
 * sql_state->current is updated with the current token.
 *
 * \returns 1, has a token, keep going, or 0 no tokens
 *
 */
int  libinjection_sqli_tokenize(sqli_state_t * sql_state);

/**
 * parses and folds input, up to 5 tokens
 *
 */
int libinjection_sqli_fold(sqli_state_t * sql_state);

/** The built-in default function to match fingerprints
 *  and do false negative/positive analysis.  This calls the following
 *  two functions.  With this, you over-ride one part or the other.
 *
 *     return libinjection_sqli_blacklist(sql_state) &&
 *        libinjection_sqli_not_whitelist(sql_state);
 *
 * \param sql_state should be filled out after libinjection_sqli_fingerprint is called
 */
int libinjection_sqli_check_fingerprint(sqli_state_t * sql_state);

/* Given a pattern determine if it's a SQLi pattern.
 *
 * \return TRUE if sqli, false otherwise
 */
int libinjection_sqli_blacklist(sqli_state_t* sql_state);

/* Given a positive match for a pattern (i.e. pattern is SQLi), this function
 * does additional analysis to reduce false positives.
 *
 * \return TRUE if SQLi, false otherwise
 */
int libinjection_sqli_not_whitelist(sqli_state_t * sql_state);

/*-----------------------------------------------------------------------------------*/
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */


#endif /* __LIBINJECTION_SQLI_H__ */

