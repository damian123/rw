#ifndef __RWARRAY_H__
#define __RWARRAY_H__

/***************************************************************************
 *
 * array.h
 *
 * $Id: //math/13/rw/array.h#5 $
 *
 * Base class for array views and other classes needed for arrays.
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

#include <rw/math/mathvec.h>
#include <rw/rand.h>

/*
 * Note that there can be no default constructor for RWArrayView.
 * A default array is zero-dimensional, a scalar, and we need to
 * know the size of the thing being stored.  To generate this array
 * use RWArrayView( RWIntVec(), sizeof(<thing>) ).
 */
class RW_MATH_SYMBOLIC RWArrayView : public RWDataView
{
protected:
    RWIntVec      npts;                           // Length of the array
    RWIntVec      step;                           // Stride length of the array

    RWArrayView(const RWIntVec& ivec, size_t s, Storage = COLUMN_MAJOR);
    RWArrayView(size_t, size_t, size_t, size_t s, Storage = COLUMN_MAJOR);
    RWArrayView(size_t, size_t, size_t, size_t, size_t s, Storage = COLUMN_MAJOR);
    RWArrayView(RWBlock* block, const RWIntVec& ivec, Storage = COLUMN_MAJOR);
    RWArrayView(RWBlock* block, size_t, size_t, size_t, Storage = COLUMN_MAJOR);
    RWArrayView(RWBlock* block, size_t, size_t, size_t, size_t, Storage = COLUMN_MAJOR);
#if defined(__SUNPRO_CC) && ((__SUNPRO_CC == 0x580) || (__SUNPRO_CC == 0x590))
    /* Fix:
    Warning (Anachronism), badcopyacc2w: Formal argument 1 of type const RWArrayView&
    in call to RWArrayView::reference(const RWArrayView&) has an inaccessible copy
    constructor.
    */
public:
#endif
    RWArrayView(const RWArrayView&);
    RWArrayView& operator=(const RWArrayView&);
protected:
    RWArrayView(const RWDataView& v, void* b, const RWIntVec& n, const RWIntVec& s);
    RWArrayView(const RWDataView& v, void* b, int x, int y, int z, const RWIntVec& step);
    RWArrayView(const RWDataView& v, void* b, int x, int y, int z, int w, const RWIntVec& step);

    void      reference(const RWArrayView&);

public:  // These functions need to be public so that they can be called by global functions
    /**
     * @internal
     * Ensure 0<=i<npts
     */
    void boundsCheck(const RWIntVec& i) const;

    /**
     * @internal
     * Ensure 0<=i<npts
     */
    void boundsCheck(const int i[], const size_t length) const;

    /**
     * @internal
     * Ensure all entries in n are non-negative
     */
    void nonnegCheck(const RWIntVec& n) const;

    /**
     * @internal
     * Check that length of self is i
     */
    void lengthCheck(const RWIntVec& i) const;

    /**
     * @internal
     * Check that length of self is i
     */
    void lengthCheck(const int n[], const size_t length) const;

    /**
     * @internal
     * Check that slice is valid
     */
    void sliceCheck(const RWIntVec&, const RWIntVec&, const RWGenMat<int>&) const;

    /**
     * @internal
     * check p==#dimensions
     */
    void dimensionCheck(size_t p) const;

    /**
     * @internal
     * check dimension i has length l
     */
    void dimensionLengthCheck(int i, int l)const;

    /**
     * @internal
     * Check that there is at least n points
     */
    void numPointsCheck(const char*, int) const;

public:  // These functions really are for public consumption
    size_t      dimension() const {
        return npts.length();
    }
    RWIntVec        length() const {
        return npts;
    }
    int           length(int i) const {
        return npts(i);
    }
    RWIntVec        stride() const {
        return step;
    }
    int           stride(int i) const {
        return step(i);
    }
};


/**
 * @ingroup math_collection_classes
 *
 * @brief An \c n-dimensional index class for traversing arrays
 * of arbitrary dimension.
 *
 * Class RWMultiIndex is an \c n-dimensional index class.
 * It is a tool for traversing arrays of arbitrary dimension. As
 * shown in the <b>Example</b> below, you can use RWMultiIndex to
 * help you write subroutines that operate on arrays of arbitrary
 * dimension. Using automatic type conversion, you can then call
 * these subroutines with vectors, matrices, or arrays. For instance,
 * you can use the <b>Example</b> subroutine as follows:
 *
 * @code
 *
 * RWMathVec<int>  v;
 * RWGenMat<int>  A;
 * RWMathArray<int>  X;
 * .
 * .
 * .
 * int  vZeros  =  numZeros(v);
 * int  AZeros  =  numZeros(A);
 * int XZeros = numZeros(X)
 * @endcode
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/math/mtharray.h>            // Any array class will do
 * for(RWMultiIndex i(n); i; ++i) { ... }   // n is an IntVec
 * @endcode
 * @section example Example
 *
 * @code
 * #include <rw/math/mtharray.h>
 *
 * int numZeros(const RWMathArray<int>& X)     // Count the number
 *                                             // of zeros in X
 * {
 *    int count=0;
 *    for(RWMultiIndex i(X.length()); i; ++i)
 *    {
 *       if (X(i)==0) ++count;
 *    }
 *    return count;
 * }
 *
 * @endcode
 *
 */
class RW_MATH_SYMBOLIC RWMultiIndex : public RWIntVec
{
private:
    int    isOK;
    RWIntVec npts;
public:
    /**
     * Constructs an index to count from 0 to \a n.
     */
    RWMultiIndex(const RWIntVec& n);

    /**
     * Increments the index.
     */
    void operator++();

    void operator++(int) {
        operator++();
    }

    /**
     * Returns 0 if the index is no longer valid. An index becomes invalid
     * when it is incremented beyond its limit.
     */
    operator void* () {
        return isOK ? this : 0;
    }
};

/*
 * The rest contains internal details of the implementation.  They
 * may change in future releases
 */

/*
 * rwCompactArrayStep is used to set up the stride vector for
 * a compact array.
 */
RWIntVec RW_MATH_SYMBOLIC rwCompactArrayStep(RWDataView::Storage storage, const RWIntVec& n);
RWIntVec RW_MATH_SYMBOLIC rwCompactArrayStep(RWDataView::Storage storage, int, int, int);
RWIntVec RW_MATH_SYMBOLIC rwCompactArrayStep(RWDataView::Storage storage, int, int, int, int);

/*
 * The ArrayLooper class is used internally by the array code.
 * This class is not documented in the manual, and may disappear or
 * change in a future release, so I wouldn't use it if I were you!
 *
 * It is used (internally) as follows:
 *
 * for( ArrayLooper l(npts,step); l; ++l ) {
 *   // Do something with the following elements
 *   // begin[l.start+0*l.stride],...,begin[l.start+l.length*l.stride]
 * }
 *
 * The above loop visits every point in the array exactly once, in
 * a fairly efficient way.  If an array is compact, then all the elements
 * will have been visited after only one iteration of the loop.
 *
 * This is used in all the arithmetic operators, and in some other
 * functions which want to visit all the elements of an array.
 *
 * The DoubleArrayLooper class is similiar, except it loops over two
 * arrays (with the same dimensions) at the same time.
 */

class RW_MATH_SYMBOLIC ArrayLooper
{
    RWIntVec npts;
    RWIntVec step;
    RWIntVec state;

    bool isOK;

public:
    int      start;
    size_t length;
    int      stride;

    ArrayLooper(const RWIntVec&, const RWIntVec&);

    void operator++();
    operator void* () {
        return isOK ? this : 0;
    }
};

class RW_MATH_SYMBOLIC DoubleArrayLooper
{
    RWIntVec npts;
    RWIntVec step1;
    RWIntVec step2;
    RWIntVec state;

    bool isOK;

public:
    size_t length;
    int      start1;
    int      start2;
    int      stride1;
    int      stride2;

    DoubleArrayLooper(const RWIntVec&, const RWIntVec&, const RWIntVec&);

    void operator++();
    operator void* () {
        return isOK ? this : 0;
    }
};

#endif /*__RWARRAY_H__*/
