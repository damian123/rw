
/**********************************************************************
 *
 * Various utilities.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/utility.h#1 $
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

#ifndef RW_TOOLS_UTILITY_H
#define RW_TOOLS_UTILITY_H

#include <rw/defs.h>

// Wrapper classes to avoid unwanted type conversion.
/**
 * @ingroup tools_classes
 * @brief Encapsulates the simple type, \c size_t.
 *
 * Encapsulates the simple type, \c size_t.  This class is provided to avoid
 * unwanted type conversions.
 */
struct RW_TOOLS_SYMBOLIC RWSize_T {
    /**
     * Construct an RWSize_T with value \a n.
     */
    RWSize_T(size_t n)
        : N_(n) {
    }

    /**
     * Access the value.
     */
    size_t value() const {
        return N_;
    }

private:

    size_t N_;

};

enum RWSeverity {
    RWWARNING,
    RWDEFAULT,
    RWFATAL
};

// For backward compatibility (moved from compat.h, moved from defs.h):
void RWError(RWSeverity, const char*, const char*);

// Replace prior long list of rwmin rwmax with these two template functions
template <class T> inline T rwmin(T a, T b)
{
    return a < b ? a : b;
}
template <class T> inline T rwmax(T a, T b)
{
    return a > b ? a : b;
}

// Binary search
extern "C"
{
    RW_TOOLS_SYMBOLIC bool
    rwBSearch(const void*, const void*, size_t, size_t, RWcompare, size_t&);
}


// Function used to hash a pointer value.  Choose an appropriate
// algorithm based on whether a pointer will fit into a word:
inline unsigned
rwHashAddress(const void* a)
{
#if defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 1200
    // Suppress Intel warning:
    // warning #1684: conversion from pointer to same-sized integral type
    //    (potential portability problem)
#  pragma warning push
#  pragma warning (disable:1684)
#endif

#if RW_SIZEOF_PTR != RW_SIZEOF_UINT

    const int rwIdHashShift = ((RW_SIZEOF_PTR * CHAR_BIT) / 2);
    // ASSUMPTION 1: sizeof(void*) >= sizeof(unsigned) is true.
    // ASSUMPTION 2: if sizeof(void*) > sizeof(unsigned) then the bottom
    //    two unsigned's worth of bits is enough hash information.

    // Algorithm for where word size is not equal to pointer size.
    // This works for compact and large models under MS-DOS as well.
    return
        // get the least significant bits by casting to unsigned
        RW_STATIC_CAST(unsigned, (RW_REINTERPRET_CAST(ptrdiff_t, a)))
        // shift the next word's worth of high order bits to the bottom
        // and XOR them in, then stop looking.
        ^ RW_STATIC_CAST(unsigned, (RW_REINTERPRET_CAST(ptrdiff_t, a) >> rwIdHashShift));
#else

    // For other addresses, mix in some higher order bits
    // into the lower order bits:
    return
        RW_STATIC_CAST(unsigned, (RW_REINTERPRET_CAST(ptrdiff_t, a)))
        ^ (RW_STATIC_CAST(unsigned, (RW_REINTERPRET_CAST(ptrdiff_t, a) >> 3)));

#endif

#if defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 1200
#  pragma warning pop
#endif
}



// used by the x headers
#define RW_HASH_FUNC(TP,NAME) unsigned (*NAME)(const TP&)

#endif // RW_TOOLS_UTILITY_H
