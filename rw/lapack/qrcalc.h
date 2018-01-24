#ifndef __RWQRCALC_H__
#define __RWQRCALC_H__
/*
 * RWQRCalc:     Calculates a QR decomposition
 *
 * $Id: //lapack/13/rw/lapack/qrcalc.h#1 $
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
 * A QR decomposition decomposes a matrix A like this:
 *
 *                    [ R ]
 *            A P = Q [   ]
 *                    [ 0 ]
 *
 * where P is a permutation matrix, Q is orthogonal, and R is upper
 * triangular if A is taller than wide, and upper trapezoidal is  A
 * is wider than tall.
 */

template< class TypeT >
class RWGenMat;

template< class TypeT >
class RWMathVec;

/**
 * @ingroup decomposition_classes
 * @brief Computes the QR decomposition of a matrix using the LAPACK
 * functions \c xgeqpf (with pivoting) or \c xgeqrf (no pivoting).
 *
 * Computes the QR decomposition of a matrix A using the LAPACK
 * functions \c xgeqpf (with pivoting) or \c xgeqrf (no pivoting).
 * This class may be used as the QRCalc template parameter for the
 * classes \link RWQRDecomp RWQRDecomp<T,QRCalc>\endlink,
 * \link RWQRDecompServer RWQRDecompServer<T,QRCalc>\endlink,
 * \link RWLeastSqQR RWLeastSqQR<T,QRCalc> \endlink
 * and \link RWCODecomp RWCODecomp<T,QRCalc>\endlink.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/qrcalc.h
 * @endcode
 */
template < class TypeT >
class RWQRCalc
{
public:
    typedef typename rw_numeric_traits<TypeT>::norm_type norm_type;

    /**
     * Parameters: \par
     *
     * On entry, \a A is the M-by-N matrix for which the QR decomposition
     * is being computed. On exit, the upper triangle of the array contains
     * the min(M,N)-by-N upper triangular matrix R; the elements below
     * the diagonal, together with the vector \a tau, represent the orthogonal
     * matrix Q as a product of min(m,n) elementary reflectors. \par
     *
     * If \a pivots != null, then on entry, if pivots[i] != 0, the i-th
     * column of \a A is permuted to the front of A*P (a leading column);
     * if pivots(i) == 0, the i-th column of \a A is a free column. On
     * exit, if pivots[i] = k, then the  i-th column of A*P was the
     * k-th column of \a A. If \a pivots == null, pivoting is not done.
     */
    static bool computeQR(RWGenMat<TypeT>& A, RWMathVec<TypeT>& tau, int* const pivots);

    /**
     * @copydoc computeQR( RWGenMat<TypeT>&, RWMathVec<TypeT>&, int *const)
     */
    static bool computeQR(RWGenMat<TypeT>& A, RWMathVec<TypeT>& tau, long* const pivots);

private:
    static bool _computeQR(RWGenMat<TypeT>& A, RWMathVec<TypeT>& tau, rw_lapack_int_t* const pivots);
};

#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/qrcalc.cc>
#endif

#endif // __RWQRCALC_H__
