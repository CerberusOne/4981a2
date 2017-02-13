STRCHR(3)                                Linux Programmer's Manual                               STRCHR(3)

[1mNAME[0m
       strchr, strrchr, strchrnul - locate character in string

[1mSYNOPSIS[0m
       [1m#include <string.h>[0m

       [1mchar *strchr(const char *[4m[22ms[24m[1m, int [4m[22mc[24m[1m);[0m

       [1mchar *strrchr(const char *[4m[22ms[24m[1m, int [4m[22mc[24m[1m);[0m

       [1m#define _GNU_SOURCE         [22m/* See feature_test_macros(7) */
       [1m#include <string.h>[0m

       [1mchar *strchrnul(const char *[4m[22ms[24m[1m, int [4m[22mc[24m[1m);[0m

[1mDESCRIPTION[0m
       The [1mstrchr[22m() function returns a pointer to the first occurrence of the character [4mc[24m in the string [4ms[24m.

       The [1mstrrchr[22m() function returns a pointer to the last occurrence of the character [4mc[24m in the string [4ms[24m.

       The  [1mstrchrnul[22m()  function  is  like [1mstrchr[22m() except that if [4mc[24m is not found in [4ms[24m, then it returns a
       pointer to the null byte at the end of [4ms[24m, rather than NULL.

       Here "character" means "byte"; these functions do not work with wide or multibyte characters.

[1mRETURN VALUE[0m
       The [1mstrchr[22m() and [1mstrrchr[22m() functions return a pointer to the matched character or NULL if the char‐
       acter  is  not  found.  The terminating null byte is considered part of the string, so that if [4mc[24m is
       specified as '\0', these functions return a pointer to the terminator.

       The [1mstrchrnul[22m() function returns a pointer to the matched character, or a pointer to the null  byte
       at the end of [4ms[24m (i.e., [4ms+strlen(s)[24m) if the character is not found.

[1mVERSIONS[0m
       [1mstrchrnul[22m() first appeared in glibc in version 2.1.1.

[1mATTRIBUTES[0m
       For an explanation of the terms used in this section, see [1mattributes[22m(7).

       ┌─────────────────────────────────┬───────────────┬─────────┐
       │[1mInterface                        [22m│ [1mAttribute     [22m│ [1mValue   [22m│
       ├─────────────────────────────────┼───────────────┼─────────┤
       │[1mstrchr[22m(), [1mstrrchr[22m(), [1mstrchrnul[22m() │ Thread safety │ MT-Safe │
       └─────────────────────────────────┴───────────────┴─────────┘
[1mCONFORMING TO[0m
       [1mstrchr[22m(), [1mstrrchr[22m(): POSIX.1-2001, POSIX.1-2008, C89, C99, SVr4, 4.3BSD.

       [1mstrchrnul[22m() is a GNU extension.

[1mSEE ALSO[0m
       [1mindex[22m(3),  [1mmemchr[22m(3), [1mrindex[22m(3), [1mstring[22m(3), [1mstrlen[22m(3), [1mstrpbrk[22m(3), [1mstrsep[22m(3), [1mstrspn[22m(3), [1mstrstr[22m(3),
       [1mstrtok[22m(3), [1mwcschr[22m(3), [1mwcsrchr[22m(3)

[1mCOLOPHON[0m
       This page is part of release 4.06 of the Linux [4mman-pages[24m project.  A description  of  the  project,
       information  about  reporting  bugs,  and  the  latest  version  of  this  page,  can  be  found at
       https://www.kernel.org/doc/man-pages/.

GNU                                             2015-08-08                                       STRCHR(3)
