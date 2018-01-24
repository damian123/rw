#ifndef __RWMTHVECPK_CC__
#define __RWMTHVECPK_CC__

/***************************************************************************
 *
 * mthvecpk.cc
 *
 * $Id: //math/13/rw/math/mthvecpk.cc#1 $
 *
 * Copyright (c) 1991-2015 Rogue Wave Software, Inc.  All Rights Reserved.
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
 **************************************************************************/

#include <rw/rwerr.h>
#include <rw/matherr.h>
#include <rw/math/mthvecpk.h>

#ifdef RW_MATH_NO_NEW_HEADER
#  include <stdio.h>
#else
#  include <cstdio>
#endif


template< class T >
RWMathVecPick<T>& RWMathVecPick<T>::operator=(const RWMathVec<T>& v)
{
    register size_t n = v.length();
    lengthCheck(n);
    for (register unsigned i = 0; i < n; i++) {
        (*this)(i) = v(i);
    }
    return *this;
}


template< class T >
RWMathVecPick<T>& RWMathVecPick<T>::operator=(const RWMathVecPick<T>& p)
{
    if (this != &p) {
        register size_t n = p.length();
        lengthCheck(n);
        for (register unsigned i = 0; i < n; i++) {
            (*this)(i) = p(i);
        }
    }
    return *this;
}


template< class T >
RWMathVecPick<T>& RWMathVecPick<T>::operator=(const T& s)
{
    register size_t n = length();
    for (register unsigned i = 0; i < n; i++) {
        (*this)(i) = s;
    }
    return *this;
}


/********************** U T I L I T I E S ***********************/

template< class T >
void RWMathVecPick<T>::assertElements() const
{
    size_t N  = pick.length();
    size_t VN = V.length();
    for (unsigned i = 0; i < N; i++) {
        if (pick(i) < 0 || pick(i) >= VN) {
            RWTHROW(RWBoundsErr(RWMessage(RWMATH_INDEX, (int)pick(i), (size_t)(V.length() - 1))));
        }
    }
}


template< class T >
void RWMathVecPick<T>::lengthCheck(size_t n) const
{
    if (length() != n) {
        RWTHROW(RWBoundsErr(RWMessage(RWMATH_VNMATCH, (size_t)length(), n)));
    }
}

#endif
