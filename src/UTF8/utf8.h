#include <stdarg.h>
#include <inttypes.h>
// C99 check
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L */

/* is c the start of a utf8 sequence? */
#ifndef isutf
#define isutf(c) (((c)&0xC0)!=0x80)
#else
#error "The macro \"isutf\" is already defined !"
#endif /* isutf */

typedef uint_least32_t u_int32_t;

/* convert UTF-8 data to wide character */
int u8_toucs(u_int32_t *dest, const size_t sz, char *src, const int srcsz);

/* the opposite conversion */
int u8_toutf8(char *dest, const int sz, u_int32_t *src, const int srcsz);

/* single character to UTF-8 */
int u8_wc_toutf8(char *dest, const u_int32_t ch);

/* character number to byte offset */
int u8_offset(char *str, int charnum);

/* byte offset to character number */
int u8_charnum(char *s, int offset);

/* return next character, updating an index variable */
u_int32_t u8_nextchar(char *s, int *i);

/* move to next character */
void u8_inc(char *s, int *i);

/* move to previous character */
void u8_dec(char *s, int *i);

/* returns length of next utf-8 sequence */
int u8_seqlen(char *s);

/* assuming src points to the character after a backslash, read an
   escape sequence, storing the result in dest and returning the number of
   input characters processed */
int u8_read_escape_sequence(char *src, u_int32_t *dest);

/* given a wide character, convert it to an ASCII escape sequence stored in
   buf, where buf is "sz" bytes. returns the number of characters output. */
int u8_escape_wchar(char *buf, const size_t sz, const u_int32_t ch);

/* convert a string "src" containing escape sequences to UTF-8 */
int u8_unescape(char *buf, int sz, char *src);

/* convert UTF-8 "src" to ASCII with escape sequences.
   if escape_quotes is nonzero, quote characters will be preceded by
   backslashes as well. */
int u8_escape(char *buf, const size_t sz, char *src, const int escape_quotes);

/* utility predicates used by the above */
int octal_digit(char c);
int hex_digit(char c);

/* return a pointer to the first occurrence of ch in s, or NULL if not
   found. character index of found character returned in *charn. */
char *u8_strchr(char *s, const u_int32_t ch, int *charn);

/* same as the above, but searches a buffer of a given size instead of
   a NUL-terminated string. */
char *u8_memchr(char *s, const u_int32_t ch, const size_t sz, int *charn);

/* count the number of characters in a UTF-8 string */
int u8_strlen(char *s);

int u8_is_locale_utf8(char *locale);

/* printf where the format string and arguments may be in UTF-8.
   you can avoid this function and just use ordinary printf() if the current
   locale is UTF-8. */
int u8_vprintf(char *fmt, va_list ap);
int u8_printf(char *fmt, ...);

// C99 check, because in this version the type _Bool was introduced
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
/* Contains the string at least one UTF8 char? */
_Bool u8_contains_string_u8_char(char* str);
#else
/* Contains the string at least one UTF8 char? */
int u8_contains_string_u8_char(char* str);
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L */
