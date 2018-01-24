#ifndef __RWMathArr_cc__
#define __RWMathArr_cc__

/***************************************************************************
 *
 * mtharray.cc
 *
 * $Id: //math/13/rw/math/mtharray.cc#1 $
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

#include <sstream>  // use std::istringstream to parse char str ctor
#include <rw/math/mtharray.h>
#include <rw/math/rwbla.h>
#include <rw/rwerr.h>
#include <rw/matherr.h>
#include <rw/rand.h>
#include <string>

template <class T> RWMathArray<T>::RWMathArray(const char* str)
    : RWArrayView(RWIntVec(), sizeof(T))
{
    // The scanFrom function uses the stream method putback().
    // This method will dump core if you initialize
    // the std::istringstream object with a static string.
    // e.g. if you do something like:  RWMathVec<double> x("[1 2 3 4]");
    // this routine will dump core UNLESS you make a copy of the string and
    // initialize the std::istringstream object with the copy.
    // If the scanFrom function can be changed so that it does not use the
    // putback() function, you do away with making the copy here.
    std::istringstream s((std::string)(str));

    scanFrom(s);
    if (!s.good()) {
        RWTHROW(RWExternalErr(RWMessage(RWMATH_STRCTOR)));
    }
}

template <class T>
RWMathArray<T>::RWMathArray()
    : RWArrayView(new RWAllocBlock< T, RWDefaultAllocator<T> >(1), RWIntVec())
{
}

template <class T>
RWMathArray<T>::RWMathArray(size_t nx, size_t ny, size_t nz, RWUninitialized)
    : RWArrayView(new RWAllocBlock< T, RWDefaultAllocator<T> >(nx* ny* nz), nx, ny, nz)
{
}

template <class T>
RWMathArray<T>::RWMathArray(size_t nx, size_t ny, size_t nz, size_t nw, RWUninitialized)
    : RWArrayView(new RWAllocBlock< T, RWDefaultAllocator<T> >(nx* ny* nz* nw), nx, ny, nz, nw)
{
}


template <class T>
RWMathArray<T>::RWMathArray(const RWIntVec& n, RWTRand<RWRandGenerator>& r, Storage storage)
    : RWArrayView(new RWAllocBlock< T, RWDefaultAllocator<T> >(static_cast<size_t>(prod(n))), n, storage)
{
    iterator i = begin(), stop = end();
    size_t size = static_cast<size_t>(prod(n));
    std::vector<double> vec(size);
    r(&vec[0], size);
    std::vector<double>::iterator it = vec.begin();
    while (i != stop) {
        *i = (T) * it++;
        ++i;
    }
}

template <class T>
RWMathArray<T>::RWMathArray(size_t nx, size_t ny, size_t nz,
                            RWTRand<RWRandGenerator>& r)
    : RWArrayView(new RWAllocBlock< T, RWDefaultAllocator<T> >(nx* ny* nz), nx, ny, nz)
{
    iterator i = begin(), stop = end();
    size_t size = nx * ny * nz;
    std::vector<double> vec(size);
    r(&vec[0], size);
    std::vector<double>::iterator it = vec.begin();
    while (i != stop) {
        *i = (T) * it++;
        ++i;
    }
}

template <class T>
RWMathArray<T>::RWMathArray(size_t nx, size_t ny, size_t nz,
                            size_t nw, RWTRand<RWRandGenerator>& r)
    : RWArrayView(new RWAllocBlock< T, RWDefaultAllocator<T> >(nx* ny* nz* nw), nx, ny, nz, nw)
{
    iterator i = begin(), stop = end();
    size_t size = nx * ny * nz * nw;
    std::vector<double> vec(size);
    r(&vec[0], size);
    std::vector<double>::iterator it = vec.begin();
    while (i != stop) {
        *i = (T) * it++;
        ++i;
    }
}

template <class T>
RWMathArray<T>::RWMathArray(const RWIntVec& n, T scalar)
    : RWArrayView(new RWAllocBlock< T, RWDefaultAllocator<T> >(static_cast<size_t>(prod(n))), n)
{
    T scalar2 = scalar;  // Temporary required to finesse K&R compilers
    RWBlas_util<T>::set((size_t)prod(n), data(), 1 , &scalar2);
}

template <class T>
RWMathArray<T>::RWMathArray(size_t nx, size_t ny, size_t nz, T scalar)
    : RWArrayView(new RWAllocBlock< T, RWDefaultAllocator<T> >(nx* ny* nz), nx, ny, nz)
{
    T scalar2 = scalar;  // Temporary required to finesse K&R compilers
    RWBlas_util<T>::set(nx * ny * nz, data(), 1, &scalar2);
}

template <class T>
RWMathArray<T>::RWMathArray(size_t nx, size_t ny, size_t nz, size_t nw, T scalar)
    : RWArrayView(new RWAllocBlock< T, RWDefaultAllocator<T> >(nx* ny* nz* nw), nx, ny, nz, nw)
{
    T scalar2 = scalar;  // Temporary required to finesse K&R compilers
    RWBlas_util<T>::set(nx * ny * nz * nw, data(), 1, &scalar2);
}

template <class T> RWMathArray<T>::RWMathArray(const T* dat, const RWIntVec& n)
    : RWArrayView(new RWAllocBlock< T, RWDefaultAllocator<T> >(static_cast<size_t>(prod(n))), n)
{
    size_t temp = static_cast<size_t>(prod(n));
    RW_ASSERT(temp <= RW_LONG_MAX);
    RWBlas<T>::copy(static_cast<long>(temp), dat, 1, data(), 1);
}

template <class T> RWMathArray<T>::RWMathArray(const T* dat, size_t nx, size_t ny, size_t nz)
    : RWArrayView(new RWAllocBlock< T, RWDefaultAllocator<T> >(nx* ny* nz), nx, ny, nz)
{
    size_t temp = nx * ny * nz;
    RW_ASSERT(temp <= RW_LONG_MAX);
    RWBlas<T>::copy(static_cast<long>(temp), dat, 1, data(), 1);
}

template <class T> RWMathArray<T>::RWMathArray(const T* dat, size_t nx, size_t ny, size_t nz, size_t nw)
    : RWArrayView(new RWAllocBlock< T, RWDefaultAllocator<T> >(nx* ny* nz* nw), nx, ny, nz, nw)
{
    size_t temp = nx * ny * nz * nw;
    RW_ASSERT(temp <= RW_LONG_MAX);
    RWBlas<T>::copy(static_cast<long>(temp), dat, 1, data(), 1);
}

template <class T>
RWMathArray<T>::RWMathArray(const RWMathVec<T>& vec, const RWIntVec& n)
    : RWArrayView(vec, (void*)vec.data(), n, rwCompactArrayStep(RWDataView::COLUMN_MAJOR, n)*vec.stride())
{
}

template <class T>
RWMathArray<T>::RWMathArray(const RWMathVec<T>& vec, size_t nx, size_t ny, size_t nz)
    : RWArrayView(vec, (void*)vec.data(), static_cast<unsigned>(nx), static_cast<unsigned>(ny), static_cast<unsigned>(nz), rwCompactArrayStep(RWDataView::COLUMN_MAJOR, static_cast<unsigned>(nx), static_cast<unsigned>(ny), static_cast<unsigned>(nz))*vec.stride())
{
}

template <class T>
RWMathArray<T>::RWMathArray(const RWMathVec<T>& vec, size_t nx, size_t ny, size_t nz, size_t nw)
    : RWArrayView(vec, (void*)vec.data(), static_cast<unsigned>(nx), static_cast<unsigned>(ny), static_cast<unsigned>(nz), static_cast<unsigned>(nw), rwCompactArrayStep(RWDataView::COLUMN_MAJOR, static_cast<unsigned>(nx), static_cast<unsigned>(ny), static_cast<unsigned>(nz), static_cast<unsigned>(nw))*vec.stride())
{
}


template <class T>
RWMathArray<T>::RWMathArray(RWBlock* blk, const RWIntVec& n)
    : RWArrayView(blk, n)
{
    size_t minlen = prod(length()) * sizeof(T);
    if (blk->length() < minlen) {
        RWTHROW(RWBoundsErr(RWMessage(RWMATH_SHORTBLOCK, blk->length(), minlen)));
    }
}

template <class T>
RWMathArray<T>::RWMathArray(RWBlock* blk, size_t nx, size_t ny, size_t nz)
    : RWArrayView(blk, nx, ny, nz)
{
    size_t minlen = prod(length()) * sizeof(T);
    if (blk->length() < minlen) {
        RWTHROW(RWBoundsErr(RWMessage(RWMATH_SHORTBLOCK, blk->length(), minlen)));
    }
}

template <class T>
RWMathArray<T>::RWMathArray(RWBlock* blk, size_t nx, size_t ny, size_t nz, size_t nw)
    : RWArrayView(blk, nx, ny, nz, nw)
{
    size_t minlen = prod(length()) * sizeof(T);
    if (blk->length() < minlen) {
        RWTHROW(RWBoundsErr(RWMessage(RWMATH_SHORTBLOCK, blk->length(), minlen)));
    }
}

template <class T>
RWMathArray<T>::RWMathArray(const RWMathVec<T>& v)
    : RWArrayView(v, (void*)v.data(), RWIntVec(1, (int)v.length()), RWIntVec(1, (int)v.stride()))
    // cast in above line needed for some cfront 2.1 compilers (like SUN's)
{
}

template <class T>
RWMathArray<T>::RWMathArray(const RWGenMat<T>& m)
    : RWArrayView(m, (void*)m.data(),
                  RWIntVec(2, static_cast<int>(m.rows()), static_cast<int>(m.cols() - m.rows())), // [ m.rows() m.cols() ]
                  RWIntVec(2, m.rowStride(), m.colStride() - m.rowStride())) // [ m.rowStride() m.colStride() ]
{
}

// Assignment ------------------------------------------------------------
template <class T>
RWMathArray<T>& RWMathArray<T>::operator=(const RWMathArray<T>& rhs)
{
    if (this != &rhs) {
        lengthCheck(rhs.length());
        if (sameDataBlock(rhs)) {
            (*this) = rhs.deepCopy();   // Avoid aliasing problems
        }
        else {
            const T* x;
            T* y;
            for (DoubleArrayLooper l(npts, step, rhs.step); l; ++l) {
                x = rhs.data() + l.start2;
                y = data() + l.start1;
                if (l.stride2 < 0) {
                    (x += (l.length - 1) * l.stride2);
                }
                if (l.stride1 < 0) {
                    (y += (l.length - 1) * l.stride1);
                }
                RW_ASSERT(l.length <= RW_LONG_MAX);
                RWBlas<T>::copy(static_cast<long>(l.length), x, l.stride2, y, l.stride1);
            }
        }
    }
    return *this;
}

template <class T>
RWMathArray<T>& RWMathArray<T>::operator=(const T& scalar)
{
    T scalar2 = scalar;  // Temporary required to finesse K&R compilers
    size_t length = 1;

    if (prod(npts) == 0) {
        return *this;
    }

    if (npts.length() == 0) {
        RWBlas_util<T>::set(1, data(), 1, &scalar2);
        return *this;
    }

    length = static_cast<size_t>(prod(npts));

    ArrayLooper looper(npts, step);

    if (length == looper.length * looper.stride) {
        RWBlas_util<T>::set(length, data(), 1, &scalar2);
    }
    else {
        for (/**/; looper; ++looper) {
            RWBlas_util<T>::set(looper.length, data() + looper.start, looper.stride, &scalar2);
        }
    }
    return *this;
}

template <class T>
RWMathArray<T>& RWMathArray<T>::reference(const RWMathArray<T>& v)
{
    RWArrayView::reference(v);
    return *this;
}

// Return copy o self with distinct instance variables
template <class T>
RWMathArray<T> RWMathArray<T>::deepCopy() const
{
    return copy();
}

// Synonym for deepCopy().  Not made inline because some compilers
// have trouble with inlined temporaries with destructors
template <class T>
RWMathArray<T> RWMathArray<T>::copy() const
{
    RWMathArray<T> temp(npts, rwUninitialized, COLUMN_MAJOR);
    temp = *this;
    return temp;
}

// Guarantee that references==1 and stride==1
template <class T>
void RWMathArray<T>::deepenShallowCopy()
{
    bool isStrideCompact = (dimension() == 0 || step((int)0) == 1);
    for (int r = static_cast<unsigned>(npts.length()) - 1; r > 0 && isStrideCompact; --r) {
        if (step(r) != npts(r - 1)*step(r - 1)) {
            isStrideCompact = false;
        }
    }
    if (!isStrideCompact || !isSimpleView()) {
        RWArrayView::reference(copy());
    }
}

template <class T>
void RWMathArray<T>::resize(const RWIntVec& N, Storage storage)
{
    if (N != npts) {
        RWMathArray<T> temp(N, rwUninitialized, storage);
        temp = (T) 0;

        size_t p = npts.length() < N.length() ? npts.length() : N.length(); // # dimensions to copy
        if (p > 0) {
            RWIntVec n(p, rwUninitialized);  // The size of the common parts of the arrays
            for (unsigned i = 0; i < p; i++) {
                n(i) = (N(i) < npts(i)) ? N(i) : npts(i);
            }

            // Build a view of the source common part
            RWGenMat<int> sstr(dimension(), p, 0);
            sstr.diagonal() = 1;
            RWMathArray<T> sp = slice(RWIntVec(dimension(), 0), n, sstr);

            // Build a view of the destination common part
            RWGenMat<int> dstr(N.length(), p, 0);
            dstr.diagonal() = 1;
            RWMathArray<T> dp = temp.slice(RWIntVec(N.length(), 0), n, dstr);

            dp = sp;
        }
        reference(temp);
    }
}

template <class T>
void RWMathArray<T>::resize(size_t m, size_t n, size_t o, Storage storage)
{
    RWIntVec i(3, rwUninitialized);
    i(0) = static_cast<unsigned>(m);
    i(1) = static_cast<unsigned>(n);
    i(2) = static_cast<unsigned>(o);
    resize(i, storage);
}

template <class T>
void RWMathArray<T>::resize(size_t m, size_t n, size_t o, size_t p, Storage storage)
{
    RWIntVec i(4, rwUninitialized);
    i(0) = static_cast<unsigned>(m);
    i(1) = static_cast<unsigned>(n);
    i(2) = static_cast<unsigned>(o);
    i(3) = static_cast<unsigned>(p);
    resize(i, storage);
}

template <class T>
void RWMathArray<T>::reshape(const RWIntVec& N, Storage storage)
{
    if (N != npts) {
        RWMathArray<T> temp(N, rwUninitialized, storage);
        reference(temp);
    }
}

template <class T>
void RWMathArray<T>::reshape(size_t m, size_t n, size_t o, Storage storage)
{
    RWIntVec i(3, rwUninitialized);
    i(0) = static_cast<unsigned>(m);
    i(1) = static_cast<unsigned>(n);
    i(2) = static_cast<unsigned>(o);
    reshape(i, storage);
}

template <class T>
void RWMathArray<T>::reshape(size_t m, size_t n, size_t o, size_t p, Storage storage)
{
    RWIntVec i(4, rwUninitialized);
    i(0) = static_cast<unsigned>(m);
    i(1) = static_cast<unsigned>(n);
    i(2) = static_cast<unsigned>(o);
    i(3) = static_cast<unsigned>(p);
    reshape(i, storage);
}

template <class T>
RWMathArray<T> RWMathArray<T>::slice(const RWIntVec& start, const RWIntVec& n, const RWGenMat<int>& str) const
{
    sliceCheck(start, n, str);
    return RWMathArray<T>(*this, (T*)data() + dot(start, stride()), n, product(stride(), str));
}


template <class T>
T toScalar(const RWMathArray<T>& A)
{
    A.dimensionCheck(0u);
    return *(A.data());
}

template <class T>
RWMathVec<T> toVec(const RWMathArray<T>& A)
{
    A.dimensionCheck(1u);
    return RWMathVec<T>(A, (T*)A.data(), (size_t)A.length((int)0), A.stride((int)0));
}

template <class T>
RWGenMat<T> toGenMat(const RWMathArray<T>& A)
{
    A.dimensionCheck(2u);
    return RWGenMat<T>(A, (T*)A.data(), (size_t)A.length((int)0), (size_t)A.length(1), A.stride((int)0), A.stride(1));
}

#include <rw/math/mtharrio.cc>
#include <rw/math/mtharrmt.cc>
#include <rw/math/mtharrop.cc>
#include <rw/math/mtharrsu.cc>

#endif
