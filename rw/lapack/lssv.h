#ifndef __RWLSSV_H__
#define __RWLSSV_H__
/*
 * RWLeastSqSV:  solve least square SV problem with orthogonal decomposition
 *
 * $Id: //lapack/13/rw/lapack/lssv.h#1 $
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
 * Uses a singular value decomposition to solve the linear
 * least squares problem.  The solution is the minimum norm solution
 * which minimizes the residual.
 *
 * When building a factorization using either a constructor or one
 * of the factor functions, you can supply a tolerance parameter.
 * Entries along the diagonal of the R factor of the SV decomposition
 * less than this tolerance are treated as zero.  Using this can
 * prevent insignificant entries of R from corrupting your solution.
 *
 * The factor functions and constructors taking a SV decomposition
 * are useful if you'd like to examine the singular values
 * for yourself and decide what tolerance to use.
 */

#include <rw/math/numtrait.h>
#include <rw/lapack/sv.h>
#include <rw/dcomplex.h>

/**
 * @ingroup least_squares_factorization_classes
 *
 * @brief Represents a factorization of a system of equations
 * with no exact solution such that the singular value method of
 * least squares can be used.
 *
 * A linear system of equations has the form \f$ Ax = b \f$,
 * where \e A contains more rows than columns and generally has no
 * exact solution; it is overdetermined. The best that we can do
 * is to find a solution vector \e x such that the norm of the
 * residual vector, \f$ Ax-b \f$, is made as small as possible. The
 * vector \e x is then a solution in the <i>least squares</i>
 * sense, and this sort of problem is called a <i>least squares
 * problem</i>.
 *
 * There are three classes in the Linear Algebra Module for solving
 * least squares problems. The approach is to construct a factorization
 * of the matrix \e A that can be used to solve the problem.
 * The classes \link RWLeastSqCh RWLeastSqCh<T>\endlink \link RWLeastSqQR
 * RWLeastSqQR<T,QRCalc>\endlink and \link RWLeastSqSV RWLeastSqSV<T,SVDCalc>
 * \endlink encapsulate this factorization object. There are three
 * methods provided:
 *
 * -  <i>Cholesky method</i> or method of normal equations, encoded by \c Ch
 * -  <i>Complete orthogonal factorization method</i>, encoded by \c QR
 * -  <i>Singular value method</i>, encoded by \c SV.
 *
 * Which method you choose depends on the trade-off between speed
 * and robustness that you want to make. As you move down the list,
 * the methods decrease in speed, but increase in accuracy and robustness.
 *
 * A system of equations \f$ Ax = b \f$
 * is underdetermined if the columns of \e A are linearly dependent.
 * In this case, there are infinitely many solutions. Often, the one
 * that is desired is the one with minimum norm. The classes \link
 * RWLeastSqQR RWLeastSqQR<T,QRCalc> \endlink and \link RWLeastSqSV
 * RWLeastSqSV<T>\endlink allow you to compute this solution.
 *
 * Note that the classes \link RWLeastSqQR RWLeastSqQR<T,QRCalc>\endlink and
 * \link RWLeastSqSV RWLeastSqSV<T>\endlink are publicly
 * derived from their underlying decomposition types, so that advanced
 * users can manipulate the underlying decomposition directly.
 *
 * @note
 * For greater flexibility with \link RWLeastSqQR RWLeastSqQR<T,QRCalc>\endlink,
 * the user can implement this method, or the Linear Algebra Module provides
 * two classes to perform this function - \link RWQRCalc RWQRCalc<T>\endlink
 * and \link RWQRCalcP3 RWQRCalcP3<T>\endlink.
 * Please see their descriptions in this reference guide for more
 * information.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/lsch.h>
 * #include <rw/lapack/lsqr.h>
 * #include <rw/lapack/lssv.h>
 * #include <rw/lapack/dvddccalc.h>
 *
 * RWLeastSqSV<double, RWSVDDivConqCalc<double> > SVD(A);   // A is
 *                                            // an RWGenMat<double>
 * RWMathVec<double> x = SVD.solve(b);  // b is an RWMathVec<double>
 * @endcode

 * @section example Example
 *
 * @code
 * // Find the least squares (if overdetermined) or
 * // minimum norm (if underdetermined) solution to
 * // the linear system of equations Ax=b. Also input
 * // a tolerance called epsilon which represents the
 * // accuracy of the entries in A. This makes the
 * // procedure robust in the event that A has nearly
 * // linearly dependent columns.
 *
 * #include <iostream>
 * #include <rw/lapack/lsch.h>
 * #include <rw/lapack/lsqr.h>
 * #include <rw/lapack/lssv.h>
 * #include <rw/lapack/svddccalc.h>
 *
 * int main()
 * {
 *     RWGenMat<double> A;
 *     double tol;
 *     RWMathVec<double> b;
 *     std::cin >> A >> tol >> b;
 *     RWLeastSqSV<double, RWSVDDivConqCalc<double>
 *                >decomp (A,tol);
 *     RWMathVec<double> x = decomp.solve(b);
 *     std::cout << "Solution is " << x << std::endl;
 *     return 0;
 * }
 * @endcode
 *
 */
template <class TypeT, class SVDCalc>
class RWLeastSqSV : public RWSVDecomp<TypeT, SVDCalc>
{
public:
    /**
     * Default constructor. Builds an empty decomposition. The decomposition
     * is filled in using the factor function.
     */
    RWLeastSqSV();

    /**
     * Constructs a factorization representing the given matrix. The supplied
     * tolerance tells when to consider entries along the diagonal of the
     * underlying decomposition as 0. The tolerance should indicate the accuracy
     * to which you know entries in \a A.
     */
    RWLeastSqSV(const RWGenMat<TypeT>& A, double tol = 0);

    /**
     * Constructs a factorization representing the linear system represented
     * by the given decomposition. The parameter \a tol tells when to
     * consider entries along the diagonal of the given SV decomposition
     * as 0. The tolerance should indicate the accuracy to which you
     * know the entries in the matrix.
     */
    RWLeastSqSV(const RWSVDecomp<TypeT, SVDCalc>& A, double tol = 0);

    // Inherited from RWSVDecomp<TypeT, SVDCalc>
    //void      factor(const RWGenMat<TypeT>& A, double tol=0);
    //bool good() const;
    //bool fail() const;
    //unsigned  rows() const {return decomp_.rows();}
    //unsigned  cols() const {return decomp_.cols();}
    //unsigned  rank() const {return decomp_.rank();}
    //void      truncate(double tol);

    /**
     * Returns the residual vector, \f$ b-Ax \f$, associated with the
     * right-hand side vector \a b and the corresponding least squares
     * solution vector \c x
     */
    RWMathVec<TypeT>    residual(const RWMathVec<TypeT>& b) const;

    /**
     * Returns the norm of the residual vector,  \f$ (b-Ax)'(b-Ax) \f$,
     * associated with the right-hand side vector \a b and the corresponding
     * least squares solution vector \c x.
     */
    typename rw_numeric_traits<TypeT>::norm_type       residualNorm(const RWMathVec<TypeT>& b) const;

    /**
     * Returns the solution to the least squares problem for the given
     * right-hand-side vector \a b. If the system matrix is overdetermined,
     * this is the vector that minimizes the norm of the residual vector.
     * If the system matrix is <i>not</i> overdetermined, this is the
     * minimum norm vector that satisfies the system equations.
     */
    RWMathVec<TypeT>    solve(const RWMathVec<TypeT>& b) const;
};

template <class TypeT, class SVDCalc>
inline  RWMathVec<TypeT> solve(const RWLeastSqSV<TypeT, SVDCalc>& A, const RWMathVec<TypeT>& b)
{
    return A.solve(b);
}

template <class TypeT, class SVDCalc>
inline  RWMathVec<TypeT> residual(const RWLeastSqSV<TypeT, SVDCalc>& A, const RWMathVec<TypeT>& b)
{
    return A.residual(b);
}

template <class TypeT, class SVDCalc>
inline  TypeT    residualNorm(const RWLeastSqSV<TypeT, SVDCalc>& A, const RWMathVec<TypeT>& b)
{
    return A.residualNorm(b);
}


#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/lssv.cc>
#endif

#endif
