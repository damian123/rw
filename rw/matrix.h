#ifndef __RWMATRIX_H
#define __RWMATRIX_H

/***************************************************************************
 *
 * matrix.h - Base class for general matrix views.
 *
 * $Id: //math/13/rw/matrix.h#1 $
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

#include <rw/dataview.h>

class RW_MATH_SYMBOLIC RWMatView : public RWDataView
{
protected:
    size_t   nrows, ncols;                    // Size of the matrix
    int        rowstep, colstep;                // Stride length of the matrix

    RWMatView();
    RWMatView(size_t m, size_t n, size_t s, Storage = COLUMN_MAJOR);
    RWMatView(RWBlock* block, size_t m, size_t n, Storage = COLUMN_MAJOR);

protected:
    RWMatView(const RWDataView& v, void* b, size_t m, size_t n, int r, int s)
        : RWDataView(v, b) {
        nrows = m;
        ncols = n;
        rowstep = r;
        colstep = s;
    }

    void      reference(const RWMatView&);

public:  // These functions need to be public so that they can be called by global functions

    /**
     * @internal
     * Ensure 0<=i,j<nrows,ncols
     */
    void boundsCheck(int, int) const;

    /**
     * @internal
     * Ensure 0<=i<nrows
     */
    void rowBoundsCheck(int i) const;

    /**
     * @internal
     * Ensure 0<=j<ncols
     */
    void colBoundsCheck(int j) const;

    /**
     * @internal
     * Check that size of self is m,n
     */
    void lengthCheck(size_t, size_t) const;

    /**
     * @internal
     * Ensure n=ncols
     */
    void colCheck(size_t) const;

    /**
     * @internal
     * Ensure m=nrows
     */
    void rowCheck(size_t) const;

    /**
     * @internal
     * Check that slice is valid
     */
    void sliceCheck(int, int, size_t, int, int) const;

    /**
     * @internal
     * Check that slice is valid
     */
    void sliceCheck(int, int, size_t, size_t, int, int, int, int) const;

    /**
     * @internal
     * Check that there is at least n points
     */
    void numPointsCheck(const char*, int) const;

public:  // These functions really are for public consumption
    size_t   cols() const {
        return ncols;
    }
    int        colStride() const {
        return colstep;
    }
    size_t   rows() const {
        return nrows;
    }
    int        rowStride() const {
        return rowstep;
    }
};

/*
 * The MatrixLooper class is used internally by the matrix code.
 * This class is not documented in the manual, and may disappear or
 * change in a future release, so I wouldn't use it if I were you!
 *
 * The MatrixLooper and DoubleMatrixLooper are exactly analogous to the
 * ArrayLooper classes, except for the special case of general matrices.
 *
 * The matrix looper loops through a column/row at a time.  If the columns
 * are contiguous, then it strings them together into one big mega-column.
 *
 * Whether we loop through columns or rows is determined by the GenMatStorage
 * enum.  The default is to loop through columns for matrices stored in
 * column major order and to loop through rows for matrices stored in row
 * major order.
 */

class RW_MATH_SYMBOLIC MatrixLooper
{
    int    ncolsLeft;
    int    colstep;   // How far to step from one column to the next

public:
    int      start;    // begin of the current column
    size_t length;   // length of the column
    int      stride;   // the inter-column stride (the rowstride of the matrix)

    MatrixLooper(size_t, size_t, int, int, RWDataView::Storage = RWDataView::RWEITHER);

    void operator++() {
        start += colstep;
        --ncolsLeft;
    }
    operator void* () {
        return (ncolsLeft <= 0) ? 0 : this;
    }
};

class RW_MATH_SYMBOLIC DoubleMatrixLooper
{
    int    ncolsLeft;
    int    colstep1;
    int    colstep2;

public:
    size_t length;
    int      start1;
    int      start2;
    int      stride1;
    int      stride2;

    DoubleMatrixLooper(size_t, size_t, int, int, int, int, RWDataView::Storage = RWDataView::RWEITHER);

    void operator++() {
        start1 += colstep1;
        start2 += colstep2;
        --ncolsLeft;
    }
    operator void* () {
        return (ncolsLeft <= 0) ? 0 : this;
    }
};

#endif /*__RWMATRIX_H__*/
