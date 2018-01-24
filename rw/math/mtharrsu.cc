#ifndef __RWMathArraysub_c__
#define __RWMathArraysub_c__

/***************************************************************************
 *
 * mtharrsu.cc -
 *   Definitions for RWMathArray<T> non-inline subscripting functions
 *
 * $Id: //math/13/rw/math/mtharrsu.cc#1 $
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

#include <rw/math/mtharray.h>

template <class T>
RWMathArray<T> RWMathArray<T>::operator()(const RWSlice& i, const RWSlice& j, const RWSlice& k)
{
    dimensionCheck(3u);
    i.boundsCheck(npts((int)0));
    j.boundsCheck(npts(1));
    k.boundsCheck(npts(2));
    unsigned numDims = 3;        // Calculate number of dimensions in the slice
    if (i.collapse()) {
        numDims--;
    }
    if (j.collapse()) {
        numDims--;
    }
    if (k.collapse()) {
        numDims--;
    }
    RWIntVec b(3, rwUninitialized);           // Build start position for the slice
    b((int)0) = i.begin();
    b(1) = j.begin();
    b(2) = k.begin();
    RWIntVec n(numDims, rwUninitialized);     // Build size of slice and stride matrix
    RWGenMat<int> str(3, numDims, 0);
    int index = 0;
    if (!i.collapse()) {
        n(index) = i.len(npts((int)0));
        str(0, index) = i.stride();
        index++;
    }
    if (!j.collapse()) {
        n(index) = j.len(npts(1));
        str(1, index) = j.stride();
        index++;
    }
    if (!k.collapse()) {
        n(index) = k.len(npts(2));
        str(2, index) = k.stride();
    }
    return slice(b, n, str);
}

template <class T>
const RWMathArray<T> RWMathArray<T>::operator()(const RWSlice& i, const RWSlice& j, const RWSlice& k) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k); // Call the non-const version
}

template <class T>
RWMathArray<T> RWMathArray<T>::operator()(const RWSlice& i, const RWSlice& j, const RWSlice& k, const RWSlice& l)
{
    dimensionCheck(4u);
    i.boundsCheck(npts((int)0));
    j.boundsCheck(npts(1));
    k.boundsCheck(npts(2));
    l.boundsCheck(npts(3));
    int numDims = 4;        // Calculate number of dimensions in the slice
    if (i.collapse()) {
        numDims--;
    }
    if (j.collapse()) {
        numDims--;
    }
    if (k.collapse()) {
        numDims--;
    }
    if (l.collapse()) {
        numDims--;
    }
    RWIntVec b(4, rwUninitialized);           // Build start position for the slice
    b((int)0) = i.begin();
    b(1) = j.begin();
    b(2) = k.begin();
    b(3) = l.begin();
    RWIntVec n(numDims, rwUninitialized);     // Build size of slice and stride matrix
    RWGenMat<int> str(4, numDims, 0);
    int index = 0;
    if (!i.collapse()) {
        n(index) = i.len(npts((int)0));
        str(0, index) = i.stride();
        index++;
    }
    if (!j.collapse()) {
        n(index) = j.len(npts(1));
        str(1, index) = j.stride();
        index++;
    }
    if (!k.collapse()) {
        n(index) = k.len(npts(2));
        str(2, index) = k.stride();
        index++;
    }
    if (!l.collapse()) {
        n(index) = l.len(npts(3));
        str(3, index) = l.stride();
    }
    return slice(b, n, str);
}

template <class T>
const RWMathArray<T> RWMathArray<T>::operator()(const RWSlice& i, const RWSlice& j, const RWSlice& k, const RWSlice& l) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k, l); // Call the non-const version
}

template <class T>
RWMathVec<T> RWMathArray<T>::operator()(const RWSlice& i, int j, int k)
{
    return toVec(operator()(i, RWSlice(j), RWSlice(k)));
}

template <class T>
const RWMathVec<T> RWMathArray<T>::operator()(const RWSlice& i, int j, int k) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k);
}

template <class T>
RWMathVec<T> RWMathArray<T>::operator()(int i, const RWSlice& j, int k)
{
    return toVec(operator()(RWSlice(i), j, RWSlice(k)));
}

template <class T>
const RWMathVec<T> RWMathArray<T>::operator()(int i, const RWSlice& j, int k) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k);
}

template <class T>
RWMathVec<T> RWMathArray<T>::operator()(int i, int j, const RWSlice& k)
{
    return toVec(operator()(RWSlice(i), RWSlice(j), k));
}

template <class T>
const RWMathVec<T> RWMathArray<T>::operator()(int i, int j, const RWSlice& k) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k);
}

template <class T>
RWMathVec<T> RWMathArray<T>::operator()(const RWSlice& i, int j, int k, int l)
{
    return toVec(operator()(i, RWSlice(j), RWSlice(k), RWSlice(l)));
}

template <class T>
const RWMathVec<T> RWMathArray<T>::operator()(const RWSlice& i, int j, int k, int l) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k, l);
}

template <class T>
RWMathVec<T> RWMathArray<T>::operator()(int i, const RWSlice& j, int k, int l)
{
    return toVec(operator()(RWSlice(i), j, RWSlice(k), RWSlice(l)));
}

template <class T>
const RWMathVec<T> RWMathArray<T>::operator()(int i, const RWSlice& j, int k, int l) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k, l);
}

template <class T>
RWMathVec<T> RWMathArray<T>::operator()(int i, int j, const RWSlice& k, int l)
{
    return toVec(operator()(RWSlice(i), RWSlice(j), k, RWSlice(l)));
}

template <class T>
const RWMathVec<T> RWMathArray<T>::operator()(int i, int j, const RWSlice& k, int l) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k, l);
}

template <class T>
RWMathVec<T> RWMathArray<T>::operator()(int i, int j, int k, const RWSlice& l)
{
    return toVec(operator()(RWSlice(i), RWSlice(j), RWSlice(k), l));
}

template <class T>
const RWMathVec<T> RWMathArray<T>::operator()(int i, int j, int k, const RWSlice& l) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k, l);
}

template <class T>
RWGenMat<T> RWMathArray<T>::operator()(int i, const RWSlice& j, const RWSlice& k)
{
    return toGenMat(operator()(RWSlice(i), j, k));
}

template <class T>
const RWGenMat<T> RWMathArray<T>::operator()(int i, const RWSlice& j, const RWSlice& k) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k);
}

template <class T>
RWGenMat<T> RWMathArray<T>::operator()(const RWSlice& i, int j, const RWSlice& k)
{
    return toGenMat(operator()(i, RWSlice(j), k));
}

template <class T>
const RWGenMat<T> RWMathArray<T>::operator()(const RWSlice& i, int j, const RWSlice& k) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k);
}

template <class T>
RWGenMat<T> RWMathArray<T>::operator()(const RWSlice& i, const RWSlice& j, int k)
{
    return toGenMat(operator()(i, j, RWSlice(k)));
}

template <class T>
const RWGenMat<T> RWMathArray<T>::operator()(const RWSlice& i, const RWSlice& j, int k) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k);
}

template <class T>
RWGenMat<T> RWMathArray<T>::operator()(int i, int j, const RWSlice& k, const RWSlice& l)
{
    return toGenMat(operator()(RWSlice(i), RWSlice(j), k, l));
}

template <class T>
const RWGenMat<T> RWMathArray<T>::operator()(int i, int j, const RWSlice& k, const RWSlice& l) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k, l);
}

template <class T>
RWGenMat<T> RWMathArray<T>::operator()(int i, const RWSlice& j, int k, const RWSlice& l)
{
    return toGenMat(operator()(RWSlice(i), j, RWSlice(k), l));
}

template <class T>
const RWGenMat<T> RWMathArray<T>::operator()(int i, const RWSlice& j, int k, const RWSlice& l) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k, l);
}

template <class T>
RWGenMat<T> RWMathArray<T>::operator()(int i, const RWSlice& j, const RWSlice& k, int l)
{
    return toGenMat(operator()(RWSlice(i), j, k, RWSlice(l)));
}

template <class T>
const RWGenMat<T> RWMathArray<T>::operator()(int i, const RWSlice& j, const RWSlice& k, int l) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k, l);
}

template <class T>
RWGenMat<T> RWMathArray<T>::operator()(const RWSlice& i, int j, int k, const RWSlice& l)
{
    return toGenMat(operator()(i, RWSlice(j), RWSlice(k), l));
}

template <class T>
const RWGenMat<T> RWMathArray<T>::operator()(const RWSlice& i, int j, int k, const RWSlice& l) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k, l);
}

template <class T>
RWGenMat<T> RWMathArray<T>::operator()(const RWSlice& i, int j, const RWSlice& k, int l)
{
    return toGenMat(operator()(i, RWSlice(j), k, RWSlice(l)));
}

template <class T>
const RWGenMat<T> RWMathArray<T>::operator()(const RWSlice& i, int j, const RWSlice& k, int l) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k, l);
}

template <class T>
RWGenMat<T> RWMathArray<T>::operator()(const RWSlice& i, const RWSlice& j, int k, int l)
{
    return toGenMat(operator()(i, j, RWSlice(k), RWSlice(l)));
}

template <class T>
const RWGenMat<T> RWMathArray<T>::operator()(const RWSlice& i, const RWSlice& j, int k, int l) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k, l);
}

template <class T>
RWMathArray<T> RWMathArray<T>::operator()(int i, const RWSlice& j, const RWSlice& k, const RWSlice& l)
{
    return operator()(RWSlice(i), j, k, l);
}

template <class T>
const RWMathArray<T> RWMathArray<T>::operator()(int i, const RWSlice& j, const RWSlice& k, const RWSlice& l) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k, l);
}

template <class T>
RWMathArray<T> RWMathArray<T>::operator()(const RWSlice& i, int j, const RWSlice& k, const RWSlice& l)
{
    return operator()(i, RWSlice(j), k, l);
}

template <class T>
const RWMathArray<T> RWMathArray<T>::operator()(const RWSlice& i, int j, const RWSlice& k, const RWSlice& l) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k, l);
}

template <class T>
RWMathArray<T> RWMathArray<T>::operator()(const RWSlice& i, const RWSlice& j, int k, const RWSlice& l)
{
    return operator()(i, j, RWSlice(k), l);
}

template <class T>
const RWMathArray<T> RWMathArray<T>::operator()(const RWSlice& i, const RWSlice& j, int k, const RWSlice& l) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k, l);
}

template <class T>
RWMathArray<T> RWMathArray<T>::operator()(const RWSlice& i, const RWSlice& j, const RWSlice& k, int l)
{
    return operator()(i, j, k, RWSlice(l));
}

template <class T>
const RWMathArray<T> RWMathArray<T>::operator()(const RWSlice& i, const RWSlice& j, const RWSlice& k, int l) const
{
    return ((RWMathArray<T>*)this)->operator()(i, j, k, l);
}

#endif
