#ifndef __GENMAT_CC__
#define __GENMAT_CC__

/***************************************************************************
 *
 * genmat.cc
 *
 * $Id: //math/13/rw/math/genmat.cc#1 $
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
#include <vector>
#include <rw/math/genmat.h>

//------------------------------------------------------------
// For some reason sunpro 410 does strange things if the header
// for RWMathVecPick class is included at this point.  So we
// just do a forward declaration.  Other compilers need the
// entire header (especially link time instantiators).
//-------------------------------------------------------------
#if defined(__SUNPRO_CC)
template <class T>
class RWGenMatPick;
#else
#  include <rw/math/matpick.h>
#endif
#include <rw/math/rwbla.h>
#include <rw/rwerr.h>
#include <rw/matherr.h>


template <class T>
RWGenMat<T>::RWGenMat(const char* str, Storage storage)
    : RWMatView()
{
    std::istringstream s((std::string)(str));
    scanFrom(s, storage);
    if (!s.good()) {
        RWTHROW(RWExternalErr(RWMessage(RWMATH_STRCTOR, str)));
    }
}

template <class T>
RWGenMat<T>::RWGenMat(size_t m, size_t n, RWTRand<RWRandGenerator>& r,
                      Storage storage)
    : RWMatView(new RWAllocBlock<T, RWDefaultAllocator<T> >(m* n), m, n, storage)
{
    size_t size = m * n;
    std::vector<double> vec(size);
    r(&vec[0], size);
    std::vector<double>::iterator it = vec.begin();
    for (unsigned j = (unsigned)n; j--;) {
        for (unsigned i = (unsigned)m; i--;) {
            (*this)(i, j) = (T) * it++;
        }
    }
}

template <class T>
RWGenMat<T>::RWGenMat(const T* dat, size_t m, size_t n, Storage storage)
    : RWMatView(new RWAllocBlock<T, RWDefaultAllocator<T> >(m* n), m, n, storage)
{
    size_t temp = nrows * ncols;
    RW_ASSERT(temp <= RW_LONG_MAX);
    RWBlas<T>::copy(static_cast<long>(temp), dat, 1, data(), 1);
}


template <class T>
RWGenMat<T>::RWGenMat(const RWMathVec<T>& vec, size_t m, size_t n, Storage storage)
    : RWMatView(vec, (void*)vec.data(), m, n, unsigned(storage == ROW_MAJOR ? n : 1)*vec.stride(),
                unsigned(storage == ROW_MAJOR ? 1 : m)*vec.stride())
{
    vec.lengthCheck(m * n);
}


template <class T>
RWGenMat<T>::RWGenMat(RWBlock* blk, size_t m, size_t n, Storage storage)
    : RWMatView(blk, m, n, storage)
{
    size_t minlen = m * n * sizeof(T);
    if (blk->length() < minlen) {
        RWTHROW(RWBoundsErr(RWMessage(RWMATH_SHORTBLOCK, blk->length(), minlen)));
    }
}


template <class T>
RWGenMat<T>& RWGenMat<T>::operator=(const RWGenMat<T>& rhs)
{
    if (this != &rhs) {
        lengthCheck(rhs.rows(), rhs.cols());
        if (sameDataBlock(rhs)) {
            (*this) = rhs.copy();   // Avoid aliasing problems
        }
        else {
            typename RWGenMat<T>::iterator itl;
            typename RWGenMat<T>::iterator last = this->end();
            typename RWGenMat<T>::const_iterator itr = rhs.begin();
            register T* rhs_ptr;
            register T* lhs_ptr;
            for (itl = this->begin(); itl < last; INC_ROW(itl), INC_ROW(itr)) {
                rhs_ptr = (T*) &*itr;
                lhs_ptr = (T*) &*itl;

                // Compensate for negative increments; the BLAS functions will
                // expect the pointer to be at the beginning of the vector, even
                // if we are reading from (or writing to) the vector backwards.
                //
                if (itr.stride() < 0) {
                    (rhs_ptr += itr.stride() * (itl.row_length() - 1));
                }
                if (itl.stride() < 0) {
                    (lhs_ptr += itl.stride() * (itl.row_length() - 1));
                }
                RW_ASSERT(itl.row_length() <= RW_LONG_MAX);
                RWBlas<T>::copy(static_cast<long>(itl.row_length()), rhs_ptr, itr.stride(), lhs_ptr, itl.stride());
            }
        }
    }
    return *this;
}

template <class T>
RWGenMat<T>& RWGenMat<T>::operator=(const T& scalar)
{
    typename RWGenMat<T>::iterator it;
    typename RWGenMat<T>::iterator last = this->end();
    for (it = this->begin(); it < last; INC_ROW(it)) {
        RWBlas_util<T>::set(it.row_length(), (T*)(&*it), it.stride(), &scalar);
    }
    return *this;
}


template <class T>
RWGenMat<T>& RWGenMat<T>::reference(const RWGenMat<T>& v)
{
    RWMatView::reference(v);
    return *this;
}

// Return copy of self with distinct instance variables
template <class T>
RWGenMat<T> RWGenMat<T>::copy(Storage s) const
{
    RWGenMat<T> temp(nrows, ncols, rwUninitialized, s);
    temp = *this;
    return temp;
}

// Synonym for copy().  Not made inline because some compilers
// have trouble with inlined temporaries with destructors
template <class T>
RWGenMat<T> RWGenMat<T>::deepCopy(Storage s) const
{
    return copy(s);
}

// Guarantee that references==1, rowstep=1 and colstep=nrows
template <class T>
void RWGenMat<T>::deepenShallowCopy(Storage s)
{
    bool isStrideCompact = (s == COLUMN_MAJOR) ? (rowstep == 1 && (unsigned int)colstep == nrows)
                           : (colstep == 1 && (unsigned int)rowstep == ncols);
    if (!isStrideCompact || !isSimpleView()) {
        RWMatView::reference(copy(s));
    }
}

template <class T>
void RWGenMat<T>::resize(size_t m, size_t n, Storage s)
{
    if (nrows != m || ncols != n) {
        RWGenMat<T> temp(m, n, T(0), s);
        size_t rowsToCopy = (nrows < m) ? nrows : m;
        size_t colsToCopy = (ncols < n) ? ncols : n;
        if (rowsToCopy > 0 && colsToCopy > 0) {
            temp.slice(0, 0, rowsToCopy, colsToCopy, 1, 0, 0, 1) = slice(0, 0, rowsToCopy, colsToCopy, 1, 0, 0, 1);
        }
        RWMatView::reference(temp);
    }
}

template <class T>
void RWGenMat<T>::reshape(size_t m, size_t n, Storage s)
{
    RWGenMat<T> temp(m, n, rwUninitialized, s);
    RWMatView::reference(temp);
}


template <class T>
const RWMathVec<T> RWGenMat<T>::diagonal(int k) const
{
    // Count on the slice function to do bounds checking
    if (k >= 0) {
        return slice(0, k, (nrows < ncols - k) ? nrows : (ncols - k), 1, 1);
    }
    else {
        return slice(-k, 0, (ncols < nrows + k) ? ncols : (nrows + k), 1, 1);
    }
}

template <class T>
RWMathVec<T> RWGenMat<T>::diagonal(int k)
{
    // Count on the slice function to do bounds checking
    if (k >= 0) {
        return slice(0, k, (nrows < ncols - k) ? nrows : (ncols - k), 1, 1);
    }
    else {
        return slice(-k, 0, (ncols < nrows + k) ? ncols : (nrows + k), 1, 1);
    }
}

// MatPick stuff

template <class T>
RWGenMat<T>& RWGenMat<T>::operator=(const RWGenMatPick<T>& p)
{
    p.lengthCheck(rows(), cols());
    RW_ASSERT(cols() <= RW_INT_MAX);
    RW_ASSERT(rows() <= RW_INT_MAX);
    for (int j = static_cast<int>(cols()); j--;) {
        T* dest = data() + j * colStride();
        const T* src = p.V.data() + p.colpick(j) * p.V.colStride();
        for (int i = static_cast<int>(rows()); i--;) {
            dest[i * rowStride()] = src[p.rowpick(i) * p.V.rowStride()];
        }
    }
    return *this;
}

template <class T>
RWGenMat<T>::RWGenMat(const RWGenMatPick<T>& p)
    : RWMatView(new RWAllocBlock<T, RWDefaultAllocator<T> >(p.rows()*p.cols()), p.rows(), p.cols())
{
    T* dest = data() + nrows * ncols;
    if (p.V.rowStride() == 1 && p.rowpick.stride() == 1) {
        for (size_t j = ncols; j--;) {
            const T* src = p.V.data() + p.colpick((unsigned)j) * p.V.colStride();
            const int* pdat = p.rowpick.data() + nrows;
            for (size_t i = nrows; i--;) {
                *(--dest) = src[*(--pdat)];
            }
        }
    }
    else {
        for (size_t j = ncols; j--;) {
            const T* src = p.V.data() + p.colpick((unsigned)j) * p.V.colStride();
            for (size_t i = nrows; i--;) {
                *(--dest) = src[p.rowpick((unsigned)i) * p.V.rowStride()];
            }
        }
    }
}

template <class T>
RWGenMatPick<T>  RWGenMat<T>::pick(const RWIntVec& ipick, const RWIntVec& jpick)
{
    return RWGenMatPick<T>(*this, ipick, jpick);
}

template <class T>
const RWGenMatPick<T> RWGenMat<T>::pick(const RWIntVec& ipick, const RWIntVec& jpick) const
{
    return RWGenMatPick<T>(*(RWGenMat<T>*)this, ipick, jpick);
}

#include <rw/math/matio.cc>
#include <rw/math/matmath.cc>
#include <rw/math/matop.cc>

#endif // __GENMAT_CC__
