

/**********************************************************************
 *
 * Template definitions for RWTBitVec<N>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tbitvec.cc#1 $
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

#include <rw/tools/cstrutil.h>

template <size_t N> bool
RWTBitVec<N>::operator==(bool b) const
{
    const RWByte val = RWByte(b ? ~0u : 0);
    if (RW_NPOS != rwCStrFindFirstNotOf(reinterpret_cast<const char*>(vec_),
                                        sizeof vec_ - 1,
                                        static_cast<char>(val),
                                        0)) {
        return false;
    }

    const RWByte mask = RWByte((N & 7) ? (1 << (N & 7)) - 1 : 0xff);
    return (val & mask) == (vec_[sizeof vec_ - 1] & mask);
}

template <size_t N> bool
RWTBitVec<N>::operator==(const RWTBitVec<N>& u) const
{
    if (0 != rwCStrCompareExact(reinterpret_cast<const char*>(vec_),
                                sizeof vec_ - 1,
                                reinterpret_cast<const char*>(u.vec_),
                                sizeof u.vec_ - 1)) {
        return false;
    }

    const RWByte mask = RWByte((N & 7) ? (1 << (N & 7)) - 1 : 0xff);
    return (vec_[sizeof vec_ - 1] & mask) == (u.vec_[sizeof u.vec_ - 1] & mask);
}

template <size_t N> bool
RWTBitVec<N>::operator<(const RWTBitVec<N>& u) const
{
    // Check the most significant (partially full) byte, if any:
    const RWByte mask = RWByte((N & 7) ? (1 << (N & 7)) - 1 : 0xff);
    if ((vec_[sizeof vec_ - 1] & mask) < (u.vec_[sizeof u.vec_ - 1] & mask)) {
        return true;
    }

    // Check the full bytes:
    for (size_t i = sizeof vec_ - 1; 0 < i; --i) {
        if (vec_[i - 1] < u.vec_[i - 1]) {
            return true;
        }
    }

    return false;
}
