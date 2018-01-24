#ifndef __MATPICK_CC__
#define __MATPICK_CC__

/***************************************************************************
 *
 * matpick.cc
 *
 * $Id: //math/13/rw/math/matpick.cc#1 $
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

#include <rw/math/genmat.h>
#include <rw/rwerr.h>
#include <rw/matherr.h>
#include <rw/math/matpick.h>
#ifdef RW_MATH_NO_NEW_HEADER
#  include <stdio.h>
#else
#  include <cstdio>
#endif

template <class T>
RWGenMatPick<T>& RWGenMatPick<T>::operator=(const RWGenMat<T>& v)
{
    lengthCheck(v.rows(), v.cols());
    for (unsigned j = (unsigned)cols(); j--;) {
        T* dest = V.data() + colpick(j) * V.colStride();
        const T* src = v.data() + j * v.colStride();
        for (unsigned i = (unsigned)rows(); i--;) {
            dest[rowpick(i)*V.rowStride()] = src[i * v.rowStride()];
        }
    }
    return *this;
}

template <class T>
RWGenMatPick<T>& RWGenMatPick<T>::operator=(const RWGenMatPick<T>& p)
{
    if (this != &p) {
        lengthCheck(p.rows(), p.cols());
        for (unsigned j = (unsigned)cols(); j--;) {
            T* dest = V.data() + colpick(j) * V.colStride();
            const T* src = p.V.data() + p.colpick(j) * p.V.colStride();
            for (unsigned i = (unsigned)rows(); i--;) {
                dest[rowpick(i)*V.rowStride()] = src[p.rowpick(i) * p.V.rowStride()];
            }
        }
    }
    return *this;
}

template <class T>
RWGenMatPick<T>& RWGenMatPick<T>::operator=(const T& s)
{
    for (unsigned j = (unsigned)cols(); j--;) {
        T* dest = V.data() + colpick(j) * V.colStride();
        for (unsigned i = (unsigned)rows(); i--;) {
            dest[rowpick(i)*V.rowStride()] = s;
        }
    }
    return *this;
}

/********************** U T I L I T I E S ***********************/

template <class T>
void RWGenMatPick<T>::assertElements() const
{
    int i = rowpick.length();
    while (--i >= 0) {
        if (rowpick(i) < 0 || rowpick(i) >= V.rows()) {
            RWTHROW(RWBoundsErr(RWMessage(RWMATH_INDEX, (int)rowpick(i), (size_t)(V.rows() - 1))));
        }
    }
    i = colpick.length();
    while (--i >= 0) {
        if (colpick(i) < 0 || colpick(i) >= V.cols()) {
            RWTHROW(RWBoundsErr(RWMessage(RWMATH_INDEX, (int)colpick(i), (size_t)(V.cols() - 1))));
        }
    }
}

template <class T>
void RWGenMatPick<T>::lengthCheck(size_t m, size_t n) const
{
    if (rows() != m || cols() != n) {
        RWTHROW(RWBoundsErr(RWMessage(RWMATH_MNMATCH, (size_t)rows(), (size_t)cols(), m, n)));
    }
}

#endif // __MATPICK_CC__
