#ifndef RW_TOOLS_CSTRUTIL_H
#define RW_TOOLS_CSTRUTIL_H

/**********************************************************************
 *
 * Header file describing common functionality for char-based string
 * classes.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/cstrutil.h#1 $
 *
 **********************************************************************
 *
 * Copyright (c) 1989-2015 Rogue Wave Software, Inc.  All Rights Reserved.
 * 
 * This computer software is owned by Rogue Wave Software, Inc. and is
 * protected by U.S. copyright laws and other laws and by international
 * treaties.  This computer software is furnished by Rogue Wave Software, Inc.
 * pursuant to a written license agreement and may be used, copied, transmitted,
 * and stored only in accordance with the terms of such license agreement and 
 * with the inclusion of the above copyright notice.  This computer software or
 * any other copies thereof may not be provided or otherwise made available to
 * any other person.
 * 
 * U.S. Government Restricted Rights.  This computer software: (a) was
 * developed at private expense and is in all respects the proprietary
 * information of Rogue Wave Software, Inc.; (b) was not developed with 
 * government funds; (c) is a trade secret of Rogue Wave Software, Inc. for all
 * purposes of the Freedom of Information Act; and (d) is a commercial item and
 * thus, pursuant to Section 12.212 of the Federal Acquisition Regulations (FAR)
 * and DFAR Supplement Section 227.7202, Government's use, duplication or
 * disclosure of the computer software is subject to the restrictions set forth
 * by Rogue Wave Software, Inc.
 *
 **********************************************************************/

#include <rw/defs.h>

#include <ctype.h>
#include <string.h>

inline int
rwCStrCompareExact(const char* first, size_t firstLen,
                   const char* second, size_t secondLen)
{
    RW_PRECONDITION2(first != rwnil && second != rwnil,
                     "rwCStrCompareExact(const char* first, size_t firstLen, "
                     "const char* second, size_t secondLen): null pointer");
    int diff = memcmp(first, second, rwmin(firstLen, secondLen));
    if (diff != 0) {
        return diff;
    }
    else {
        if (firstLen == secondLen) {
            return 0;
        }
        else if (firstLen < secondLen) {
            return -1;
        }
        else {
            return 1;
        }
    }
}

inline int
rwCStrCompareIgnoreCase(const char* first, size_t firstLen,
                        const char* second, size_t secondLen)
{
    RW_PRECONDITION2(first != rwnil && second != rwnil,
                     "rwCStrCompareIgnoreCase(const char* first, size_t firstLen, "
                     "const char* second, size_t secondLen): null pointer");
    size_t len = rwmin(firstLen, secondLen);

    for (size_t i = 0; i < len; ++i) {
        int c1 = tolower(RW_STATIC_CAST(unsigned char, first[i]));
        int c2 = tolower(RW_STATIC_CAST(unsigned char, second[i]));

        // Check for equality
        if (c1 != c2) {
            return c1 - c2;
        }
    }

    // strings are equal up to the length of the shorter one.
    if (firstLen == secondLen) {
        return 0;
    }
    else if (firstLen < secondLen) {
        return -1;
    }
    else {
        return 1;
    }
}

inline int
rwCStrCompareIgnoreCaseStrict(const char* first, size_t firstLen,
                              const char* second, size_t secondLen)
{
    RW_PRECONDITION2(first != rwnil && second != rwnil,
                     "rwCStrCompareIgnoreCaseStrict(const char* first, size_t firstLen, "
                     "const char* second, size_t secondLen): null pointer");
    size_t len = rwmin(firstLen, secondLen);

    for (size_t i = 0; i < len; ++i) {
        int c1 = first[i];
        int c2 = second[i];
        if (isalpha(c1) && isalpha(c2)) {
            c1 = tolower(RW_STATIC_CAST(unsigned char, c1));
            c2 = tolower(RW_STATIC_CAST(unsigned char, c2));
        }

        // Check for equality
        if (c1 != c2) {
            return c1 - c2;
        }
    }

    // strings are equal up to the length of the shorter one.
    if (firstLen == secondLen) {
        return 0;
    }
    else if (firstLen < secondLen) {
        return -1;
    }
    else {
        return 1;
    }
}

inline size_t
rwCStrFindFirstNotOf(const char* haystack, size_t haystackLen, char needle, size_t start)
{
    RW_PRECONDITION2(haystack != rwnil,
                     "rwCStrFindFirstNotOf(const char* haystack, size_t haystackLen, "
                     "char needle): null pointer");
    if (haystackLen > 0) {
        for (size_t i = start; i < haystackLen; ++i) {
            if (haystack[i] != needle) {
                return i;
            }
        }
    }
    return RW_NPOS;
}

inline size_t
rwCStrFindFirstNotOf(const char* haystack, size_t haystackLen,
                     const char* needle, size_t needleLen, size_t start)
{
    RW_PRECONDITION2(haystack != rwnil && needle != rwnil,
                     "rwCStrFindFirstNotOf(const char* haystack, size_t haystackLen, "
                     "const char* needle, size_t needleLen): null pointer");
    if (needleLen > 0 && haystackLen > 0) {
        char lookup[256] = {};
        for (size_t i = 0; i < needleLen; ++i) {
            lookup[(unsigned char)needle[i]] = 1;
        }

        for (size_t i = start; i < haystackLen; ++i) {
            if (lookup[(unsigned char)haystack[i]] == 0) {
                return i;
            }
        }
    }

    return RW_NPOS;
}

inline size_t
rwCStrFindFirstOf(const char* haystack, size_t haystackLen, char needle, size_t start)
{
    RW_PRECONDITION2(haystack != rwnil,
                     "rwCStrFindFirstOf(const char* haystack, size_t haystackLen, "
                     "char needle): null pointer");
    if (haystackLen > 0 && start < haystackLen) {
        const char* loc = (const char*)memchr(haystack + start, needle, haystackLen - start);
        if (loc != 0) {
            return (size_t)(loc - haystack);
        }
    }
    return RW_NPOS;
}

inline size_t
rwCStrFindFirstOf(const char* haystack, size_t haystackLen,
                  const char* needle, size_t needleLen, size_t start)
{
    RW_PRECONDITION2(haystack != rwnil && needle != rwnil,
                     "rwCStrFindFirstOf(const char* haystack, size_t haystackLen, "
                     "const char* needle, size_t needleLen): null pointer");
    if (needleLen > 0 && haystackLen > 0) {
        char lookup[256] = {};
        for (size_t i = 0; i < needleLen; ++i) {
            lookup[(unsigned char)needle[i]] = 1;
        }

        for (size_t i = start; i < haystackLen; ++i) {
            if (lookup[(unsigned char)haystack[i]] == 1) {
                return i;
            }
        }
    }

    return RW_NPOS;
}

inline size_t
rwCStrFindLastNotOf(const char* haystack, size_t haystackLen, char needle, size_t end)
{
    RW_PRECONDITION2(haystack != rwnil,
                     "rwCStrFindLastNotOf(const char* haystack, size_t haystackLen, "
                     "char needle): null pointer");
    if (haystackLen > 0) {
        for (size_t i = rwmin(haystackLen - 1, end) + 1; i--;) {
            if (haystack[i] != needle) {
                return i;
            }
        }
    }
    return RW_NPOS;
}

inline size_t
rwCStrFindLastNotOf(const char* haystack, size_t haystackLen,
                    const char* needle, size_t needleLen, size_t end)
{
    RW_PRECONDITION2(haystack != rwnil && needle != rwnil,
                     "rwCStrFindLastNotOf(const char* haystack, size_t haystackLen, "
                     "const char* needle, size_t needleLen): null pointer");
    if (needleLen > 0 && haystackLen > 0) {
        char lookup[256] = {};
        for (size_t i = 0; i < needleLen; ++i) {
            lookup[(unsigned char)needle[i]] = 1;
        }

        for (size_t i = rwmin(haystackLen - 1, end) + 1; i--;) {
            if (lookup[(unsigned char)haystack[i]] == 0) {
                return i;
            }
        }
    }

    return RW_NPOS;
}

inline size_t
rwCStrFindLastOf(const char* haystack, size_t haystackLen, char needle, size_t end)
{
    RW_PRECONDITION2(haystack != rwnil,
                     "rwCStrFindLastOf(const char* haystack, size_t haystackLen, "
                     "char needle): null pointer");
    if (haystackLen > 0) {
        for (size_t i = rwmin(haystackLen - 1, end) + 1; i--;) {
            if (haystack[i] == needle) {
                return i;
            }
        }
    }
    return RW_NPOS;
}

inline size_t
rwCStrFindLastOf(const char* haystack, size_t haystackLen,
                 const char* needle, size_t needleLen, size_t end)
{
    RW_PRECONDITION2(haystack != rwnil && needle != rwnil,
                     "rwCStrFindLastOf(const char* haystack, size_t haystackLen, "
                     "const char* needle, size_t needleLen): null pointer");
    if (needleLen > 0 && haystackLen > 0) {
        char lookup[256] = {};
        for (size_t i = 0; i < needleLen; ++i) {
            lookup[(unsigned char)needle[i]] = 1;
        }

        for (size_t i = rwmin(haystackLen - 1, end) + 1; i--;) {
            if (lookup[(unsigned char)haystack[i]] == 1) {
                return i;
            }
        }
    }

    return RW_NPOS;
}

inline size_t
rwCStrFindExactImp(const char* haystack, size_t haystackLen,
                   const char* needle, size_t needleLen, size_t start)
{
    RW_PRECONDITION2(haystack != rwnil && needle != rwnil,
                     "rwCStrFindExactImp(const char* haystack, size_t haystackLen, "
                     "const char* needle, size_t needleLen, size_t start): null pointer");
    const char* pos = haystack + start;
    const char* end = haystack + haystackLen - needleLen;

    if (needleLen == 1) {
        pos = (const char*) memchr(pos, (unsigned char) * needle, haystackLen - start);
        return pos == NULL ? RW_NPOS : (size_t)(pos - haystack);
    }
    else {
        while (pos <= end) {
            pos = (const char*) memchr(pos, (unsigned char) * needle, (size_t)(end - pos + 1));
            if (pos == NULL) {
                return RW_NPOS;
            }
            if (memcmp(pos, needle, needleLen) == 0) {
                return (size_t)(pos - haystack);
            }
            ++pos;
        }
    }
    return RW_NPOS;
}

inline size_t
rwCStrFindExact(const char* haystack, size_t haystackLen,
                const char* needle, size_t needleLen, size_t start)
{
    if (start + needleLen > haystackLen) {
        return RW_NPOS;
    }
    if (needleLen == 0) {
        return start;
    }
    return rwCStrFindExactImp(haystack, haystackLen, needle, needleLen, start);
}

inline size_t
rwCStrFindIgnoreCaseImp(const char* haystack, size_t haystackLen,
                        const char* needle, size_t needleLen, size_t start)
{
    RW_PRECONDITION2(haystack != rwnil && needle != rwnil,
                     "rwCStrFindIgnoreCaseImp(const char* haystack, size_t haystackLen, "
                     "const char* needle, size_t needleLen, size_t start): null pointer");
    // case insensitive search for 'needle'
    char u = (char)tolower(RW_STATIC_CAST(unsigned char, *needle));
    char l = (char)toupper(RW_STATIC_CAST(unsigned char, *needle));
    const char* pos = haystack + start;
    const char* end = haystack + haystackLen - needleLen;
    while (pos <= end) {
        if ((*pos == u || *pos == l) &&
                (0 == rwCStrCompareIgnoreCase(pos + 1, needleLen - 1, needle + 1, needleLen - 1))) {
            return (size_t)(pos - haystack);
        }
        ++pos;
    }
    return RW_NPOS;
}

inline size_t
rwCStrFindIgnoreCase(const char* haystack, size_t haystackLen,
                     const char* needle, size_t needleLen, size_t start)
{
    if (start + needleLen > haystackLen) {
        return RW_NPOS;
    }
    if (needleLen == 0) {
        return start;
    }
    return rwCStrFindIgnoreCaseImp(haystack, haystackLen, needle, needleLen, start);
}

inline size_t
rwCStrRFindExactImp(const char* haystack, const char* needle,
                    size_t needleLen, size_t start)
{
    RW_PRECONDITION2(haystack != rwnil && needle != rwnil,
                     "rwCStrRFindExactImp(const char* haystack, const char* needle, "
                     "size_t needleLen, size_t start): null pointer");
    const char* pos = haystack + start;
    while (pos >= haystack) {
        if (memcmp(pos--, needle, needleLen) == 0) {
            return (size_t)(pos - haystack + 1);
        }
    }
    return RW_NPOS;
}

inline size_t
rwCStrRFindExact(const char* haystack, size_t haystackLen,
                 const char* needle, size_t needleLen, size_t start)
{
    if (needleLen > haystackLen) {
        return RW_NPOS;
    }

    start = rwmin(start, haystackLen - needleLen);
    if (needleLen == 0) {
        return start;
    }
    return rwCStrRFindExactImp(haystack, needle, needleLen, start);
}

inline size_t
rwCStrRFindIgnoreCaseImp(const char* haystack, const char* needle,
                         size_t needleLen, size_t start)
{
    RW_PRECONDITION2(haystack != rwnil && needle != rwnil,
                     "rwCStrRFindIgnoreCaseImp(const char* haystack, const char* needle, "
                     "size_t needleLen, size_t start): null pointer");
    const char* pos = haystack + start;
    char u = (char)tolower(RW_STATIC_CAST(unsigned char, *needle));
    char l = (char)toupper(RW_STATIC_CAST(unsigned char, *needle));
    while (pos >= haystack) {
        if ((*pos == u || *pos == l) &&
                (0 == rwCStrCompareIgnoreCase(pos + 1, needleLen - 1, needle + 1, needleLen - 1))) {
            return (size_t)(pos - haystack);
        }
        --pos;
    }

    return RW_NPOS;
}

inline size_t
rwCStrRFindIgnoreCase(const char* haystack, size_t haystackLen,
                      const char* needle, size_t needleLen, size_t start)
{
    if (needleLen > haystackLen) {
        return RW_NPOS;
    }

    start = rwmin(start, haystackLen - needleLen);
    if (needleLen == 0) {
        return start;
    }
    return rwCStrRFindIgnoreCaseImp(haystack, needle, needleLen, start);
}

#endif
