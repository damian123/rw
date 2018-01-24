#ifndef __RWMATHVEC_CC__
#define __RWMATHVEC_CC__

/***************************************************************************
 *
 * mathvec.cc
 *
 * $Id: //math/13/rw/math/mathvec.cc#1 $
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

#include <sstream>  // use std::istringstream to parse char str ctor.
#include <rw/rwerr.h>
#include <rw/matherr.h>
#include <rw/math/rwbla.h>
#include <rw/rand.h>
#include <string>
#include <vector>

//------------------------------------------------------------
// For some reason sunpro 410 does strange things if the header
// for RWMathVecPick class is included at this point.  So we
// just do a forward declaration.  Other compilers need the
// entire header (especially link time instantiators).
//-------------------------------------------------------------
#if defined(__SUNPRO_CC)
template< class T >
class RWMathVecPick;
#else
#  include <rw/math/mthvecpk.h>
#endif

//***************************************************************
//
//  Constructors
//
//***************************************************************

template< class T >
RWMathVec<T>::RWMathVec(const char* str)
    : RWVecView()
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
    if (!s.good() && !s.eof()) {
        RWTHROW(RWExternalErr(RWMessage(RWMATH_STRCTOR, str)));
    }
}


template< class T >
RWMathVec<T>::RWMathVec(size_t n,  const T& scalar)
    : RWVecView(new RWAllocBlock< T, RWDefaultAllocator<T> >(n), n)
{
    RWBlas_util<T>::set(n, data(), 1, &scalar);
}


template< class T >
RWMathVec<T>::RWMathVec(size_t n, const T& scalar, const T& by)
    : RWVecView(new RWAllocBlock< T, RWDefaultAllocator<T> >(n), n)
{
    register size_t i = n;
    T* dp = data();
    T value = scalar;
    T byvalue = by;
    while (i--) {
        *dp++ = value;
        value += byvalue;
    }
}


template< class T >
RWMathVec<T>::RWMathVec(const T* dat, size_t n)
    : RWVecView(new RWAllocBlock< T, RWDefaultAllocator<T> >(n), n)
{
    RW_ASSERT(n <= RW_LONG_MAX);
    RWBlas<T>::copy(static_cast<long>(n), dat, 1, data(), 1);
}

template< class T >
RWMathVec<T>::RWMathVec(size_t n, RWRandInterface& randNum)
    : RWVecView(new RWAllocBlock< T, RWDefaultAllocator<T> >(n), n)
{
    iterator p = begin();
    RW_ASSERT(data() != 0 || n == 0);
    std::vector<double> vec(n);
    randNum(&vec[0], n);
    std::vector<double>::iterator out = vec.begin();
    while (p != end()) {
        *p = T(*out);
        ++p;
        ++out;
    }
}


template< class T >
RWMathVec<T>::RWMathVec(RWBlock* blk, size_t n)
    : RWVecView(blk, n)
{
    size_t minlen = n * sizeof(T);
    if (blk->length() < minlen) {
        RWTHROW(RWBoundsErr(RWMessage(RWMATH_SHORTBLOCK, blk->length(), minlen)));
    }
}

//**************************************************************************
//
//  Member Functions
//
//**************************************************************************

template< class T >
RWMathVec<T>& RWMathVec<T>::operator=(const RWMathVec<T>& rhs)
{
    if (this != &rhs) {
        lengthCheck(rhs.length());
        if (sameDataBlock(rhs)) {
            (*this) = rhs.copy();   // avoid aliasing problems
        }
        else {
            register T* rhs_ptr = (T*) rhs.data();
            register T* lhs_ptr = (T*) data();

            // Compensate for negative increments; the BLAS functions will
            // expect the pointer to be at the beginning of the vector, even
            // if we are reading from (or writing to) the vector backwards.
            //
            if (rhs.step < 0) {
                (rhs_ptr += rhs.step * (npts - 1));
            }
            if (step < 0) {
                (lhs_ptr += step * (npts - 1));
            }
            RW_ASSERT(npts <= RW_LONG_MAX);
            RWBlas<T>::copy(static_cast<long>(npts), rhs_ptr, rhs.step, lhs_ptr, step);
        }
    }

    return *this;
}


// Assign to a scalar:
template< class T >
RWMathVec<T>& RWMathVec<T>::operator=(const T& scalar)
{
    RWBlas_util<T>::set(npts, data(), step, &scalar);
    return *this;
}


template< class T >
RWMathVec<T>& RWMathVec<T>::reference(const RWMathVec<T>& v)
{
    RWVecView::reference(v);
    return *this;
}


// Return copy of self with distinct instance variables
template< class T >
RWMathVec<T> RWMathVec<T>::copy() const
{
    RWMathVec<T> temp(npts, rwUninitialized);
    register T* this_ptr = (T*) data();
    if (step < 0) {
        (this_ptr += step * (npts - 1));
    }
    RW_ASSERT(npts <= RW_LONG_MAX);
    RWBlas<T>::copy(static_cast<long>(npts), this_ptr, step, temp.data(), 1);
    return temp;
}


// Synonym for copy().  Not made inline because some compilers
// have trouble with inlined temporaries with destructors.
template< class T >
RWMathVec<T> RWMathVec<T>::deepCopy() const
{
    return copy();
}


// Guarantee that references==1 and stride==1
template< class T >
void RWMathVec<T>::deepenShallowCopy()
{
    if (!isSimpleView()) {
        RWVecView::reference(copy());
    }
}

//**************************************************************************
//
// Reset the length of the vector.  The contents of the old
// vector are saved.  If the vector has grown, the extra storage
// is zeroed out.  If the vector has shrunk, it is truncated.
//
//**************************************************************************
template< class T >
void RWMathVec<T>::resize(size_t N)
{
    if (N != npts) {
        RWMathVec<T> newvec(N, rwUninitialized);

        // Copy over the minimum of the two lengths:
        size_t nkeep = npts < N ? npts : N;
        T* x = data();
        if (stride() < 0) {
            (x += nkeep * stride());
        }
        RW_ASSERT(nkeep <= RW_LONG_MAX);
        RWBlas<T>::copy(static_cast<long>(nkeep), x, stride(), newvec.data(), 1);
        size_t oldLength = npts; // Remember the old length

        RWVecView::reference(newvec);

        // If vector has grown, zero out the extra storage:
        if (N > oldLength) {
            static const T zero = T(0);   /* "static" required for Glockenspiel */
            RWBlas_util<T>::set((size_t)(N - oldLength), (T*)data() + oldLength, 1, (T*)&zero);
        }
    }
}

//**************************************************************************
//
// Reset the length of the vector.  The results can be (and
// probably will be) garbage.
//
//**************************************************************************
template< class T >
void RWMathVec<T>::reshape(size_t N)
{
    if (N != npts) {
        RWVecView::reference(RWMathVec<T>(N, rwUninitialized));
    }
}


template< class T >
RWMathVec<T> RWMathVec<T>::slice(int start, size_t n, int str) const
{
    return (*this)[RWSlice(start, static_cast<unsigned>(n), str)];
}


template< class T >
RWMathVec<T>& RWMathVec<T>::operator=(const RWMathVecPick<T>& p)
{
    register size_t n = length();
    p.lengthCheck(n);
    RW_ASSERT(n <= RW_INT_MAX);
    for (register int i = 0; i < static_cast<int>(n); i++) {
        (*this)(i) = p(i);
    }
    return *this;
}


template< class T >
RWMathVec<T>::RWMathVec(const RWMathVecPick<T>& p)
    : RWVecView(new RWAllocBlock< T, RWDefaultAllocator<T> >(p.length()), p.length())
{
    register T* dp = data();
    RW_ASSERT(dp != 0 || npts == 0);
    RW_ASSERT(npts <= RW_INT_MAX);
    for (register int i = 0; i < static_cast<int>(npts); i++) {
        *dp = p(i);
        dp++;
    }
}


template< class T >
RWMathVecPick<T> RWMathVec<T>::pick(const RWIntVec& ipick)
{
    // Temporary necessary for cfront; will generate a warning:
    RWMathVecPick<T> temp(*this, ipick);
    return temp;
}


template< class T >
const RWMathVecPick<T> RWMathVec<T>::pick(const RWIntVec& ipick) const
{
    // Temporary necessary for cfront; will generate a warning.
    // without cast of this, a temporary RWMathVec is constructed
    RWMathVecPick<T> temp(*(RWMathVec<T>*)this, ipick);
    return temp;
}


#include <rw/math/mthvecio.cc>
#include <rw/math/mthvecop.cc>
#include <rw/math/mthvecma.cc>

#endif
