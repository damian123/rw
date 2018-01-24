#ifndef __RWSYMFCT_H__
#define __RWSYMFCT_H__

/*
 * SymFact - A symmetric indefinite U^T D U factorization
 *
 * $Id: //lapack/13/rw/lapack/symfct.h#1 $
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
 * This header file will work both with lapack.h++ and linpack.h++.
 */

#include <rw/lapack/latraits.h>
#include <rw/lapack/symmat.h>
#include <rw/lapkdefs.h>
#include <rw/math/mathvec.h>
#include <rw/math/rwalloc.h>

/**
 * @ingroup factorization_classes
 * @brief Encapsulates factorizations of symmetric matrices.
 *
 * A \e factorization is a representation of a matrix that can
 * be used to efficiently solve systems of equations, and to compute
 * the inverse, determinant, and condition number of a matrix. The
 * class \link RWSymFact RWSymFact<T>\endlink encapsulates factorizations
 * of symmetric matrices. Provided the matrix being factored is
 * nonsingular, the resulting factorization can always be used to
 * solve a system of equations.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/symfct.h>
 *
 * RWSymFact<double> LU2(B);       // B is a RWSymMat<double>
 * @endcode
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/lapack/symfct.h>
 *
 * int main()
 * {
 *    // Read in a matrix and a right-hand side and
 *    // print the solution
 *    RWSymMat<double> A;
 *    RWMathVec<double> b;
 *    std::cin >> A >> b;
 *
 *    RWSymFact<double> LU(A);
 *    if (LU.good()) {
 *       std::cout << "solution is " << solve(LU, b) << std::endl;
 *    } else {
 *       std::cout << "Could not factor A, perhaps it is singular"
 *                 << std::endl;
 *    }
 *
 *    return 0;
 * }
 * @endcode
 *
 */
template <class TypeT>
class RWSymFact
{
public:
    typedef typename rw_numeric_traits<TypeT>::norm_type norm_type;
private:
    RWSymMat<TypeT> factorization;
    RWTWorkBuffer<rw_lapack_int_t> pvts;       // used by Lapack.h++
    RWMathVec<rw_lapack_int_t> pivots;         // used by Linpack.h++
    // The factorization and the pivot vector.
    // This cannot be a union because IntVec has a constructor

    rw_lapack_int_t   info;           // used by Lapack.h++
    // If info is non-zero, something went wrong in the factorization.

    union {
        norm_type    Anorm; // used by Lapack.h++
        norm_type    cond;  // used by Linpack.h++
    };

    // If Anorm is non-negative, then it is the infinity norm of the
    // original matrix.  This is used when computing the condition
    // number.
    // cond is the reciprocal condition number if calculated.  If
    // condition number not calculated contains 0 for singularity
    // else -1.

protected:

    static const char* className;
    // For use in error messages

    void dofactor(bool estimateCondition);
    // Do the factorization.  Call this after the matrix
    // variable factorization is set up.

    void calldi(const RWSymMat<TypeT>& A, int job, TypeT* det, int* inert) const;
    // call the linpack xxxdi routine with the job number indicated
    // and with the 2 byte array det.

public:

    /**
     * Default constructor. Builds a factorization of a <tt>0 x 0</tt>
     * matrix. You use the member function factor() to fill in the
     * factorization.
     */
    RWSymFact();

    RWSymFact(const RWSymFact<TypeT>&);

    /**
     * Constructs a factorization of the matrix \a A. This factorization
     * can be used to solve systems of equations, and to calculate inverses
     * and determinants. If the parameter \a estimateCondition is \c true,
     * you can use the function condition() to obtain an estimate of the
     * condition number of the matrix. Setting \a estimateCondition to
     * \c false can save some computation if the condition number
     * is not needed.
     */
    RWSymFact(const RWSymMat<TypeT>& A, bool estimateCondition = true);

    /**
     * Factors a matrix. Calling factor() replaces the current factorization
     * with the factorization of the matrix \a A. This is commonly used
     * to initialize a factorization constructed with the default (no
     * parameters) constructor.
     */
    void factor(const RWSymMat<TypeT>& A, bool estimateCondition = true);

    ~RWSymFact();
    // The constructors.
    // You can save a little bit of time by not estimating the
    // condition of the factorization, and thus giving up any
    // knowledge of how accurate solutions obtained using this
    // factorization will be.

    /**
     * Checks whether the factorization is successfully constructed.
     * If good() returns \c false, attempting to use the factorization to solve
     * a system of equations results in an exception being thrown.
     */
    bool good() const {
        return !fail();
    }

    /**
     * Checks whether the factorization is successfully constructed.
     * If fail() returns \c true, attempting
     * to use the factorization to solve a system of equations results
     * in an exception being thrown.
     */
    bool fail() const;

    /**
     * Tests if the matrix is singular to within machine precision.
     * If the factorization is a positive definite type and the matrix
     * that was factored is \e not actually positive definite, then
     * isSingular() may return \c true regardless of whether or not
     * the matrix is actually singular.
     */
    bool isSingular() const;

    /**
     * Returns the number of rows in the matrix represented by this
     * factorization.
     */
    int       rows() const {
        return factorization.rows();
    }

    /**
     * Returns the number of columns in the matrix represented by this
     * factorization.
     */
    int       cols() const {
        return factorization.cols();
    }

    /**
     * Calculates the reciprocal condition number of the matrix represented
     * by this factorization. If this number is near \c 0,
     * the matrix is ill-conditioned and solutions to systems of equations
     * computed using this factorization may \e not be accurate. If the number
     * is near \c 1, the matrix is well-conditioned. For the condition number
     * to be computed, the norm of the matrix must be computed at the time the
     * factorization is constructed. If you set the optional boolean parameter
     * in the \link RWSymFact(const RWSymMat<TypeT>&, bool) constructor\endlink
     * or factor() to \c false, calling condition() generates an exception.
     */
    typename rw_numeric_traits<TypeT>::norm_type condition() const;
    // If fail() returns true calling solve() or inverse() or ... may
    // fail.  condition() returns an approximation to the reciprocal
    // condition number of the factorization.


    /**
     * Solves a system of equations. Returns the vector \c x, which
     * satisfies \f$ Ax = b \f$, where \c A is the matrix represented by this
     * factorization. It is wise to call good() or fail() to make
     * sure that the factorization was successfully constructed.
     */
    RWMathVec<TypeT> solve(const RWMathVec<TypeT>& b) const;

    /**
     * Solves a system of equations. Returns the matrix \c x, which
     * satisfies \f$ Ax = b \f$, where \c A is the matrix represented by this
     * factorization. It is wise to call good() or fail() to make
     * sure that the factorization was successfully constructed.
     */
    RWGenMat<TypeT> solve(const RWGenMat<TypeT>& b) const;

    /**
     * Calculates the determinant of the matrix represented by this
     * factorization.
     */
    TypeT determinant() const;

    /**
     * Computes the inverse of the matrix represented by this factorization.
     * Although matrix inverses are very useful in theoretical analysis,
     * they are rarely necessary in implementation. A factorization
     * is nearly always as useful as the actual inverse, and can be
     * constructed at far less cost.
     */
    RWSymMat<TypeT> inverse() const;
    // solve and inverse functions
};


/**
 * @relates RWSymFact
 *
 * Solves a system of equations. Returns the vector \c x, which
 * satisfies \f$ Ax = b \f$, where \a A is the matrix represented
 * by the factorization \a A. It is wise to call one of the member functions
 * good() or fail() of \a A to make sure that the factorization was
 * successfully constructed.
 */
template <class TypeT>
RWMathVec<TypeT>  solve(const RWSymFact<TypeT>& A, const RWMathVec<TypeT>& b);

/**
 * @relates RWSymFact
 *
 * Solves a system of equations. Returns the matrix \c X, which
 * satisfies \f$ AX = B \f$, where \a A is the matrix represented
 * by the factorization \a A. It is wise to call one of the member functions
 * good() or fail() of \a A to make sure that the factorization was
 * successfully constructed.
 */
template <class TypeT>
RWGenMat<TypeT>  solve(const RWSymFact<TypeT>& A, const RWGenMat<TypeT>& B);

/**
 * @relates RWSymFact
 *
 * Calculates the determinant of the matrix represented by factorization
 * \a A.
 */
template <class TypeT>
TypeT  determinant(const RWSymFact<TypeT>& A);

template <class TypeT>
inline TypeT    determinant(const RWSymMat<TypeT>& A)
{
    return RWSymFact<TypeT>(A).determinant();
}

/**
 * @relates RWSymFact
 *
 * Computes the inverse of the matrix represented by factorization \a A.
 * Although matrix inverses are very useful in theoretical analysis,
 * they are rarely necessary in implementation. A factorization
 * is nearly always as useful as the actual inverse, and can be
 * constructed at far less cost.
 */
template <class TypeT>
RWSymMat<TypeT>  inverse(const RWSymFact<TypeT>& A);

/**
 * @relates RWSymFact
 *
 * Calculates the reciprocal condition number of the matrix represented
 * by the factorization \a A. If this number is near \c 0, the matrix is
 * ill-conditioned and solutions to systems of equations computed using this
 * factorization may \e not be accurate. If the number is near \c 1, the matrix
 * is well-conditioned. For the condition number to be computed, the norm
 * of the matrix must be computed at the time the factorization is
 * constructed. If you set the optional boolean parameter in the
 * \link RWSymFact(const RWSymMat<TypeT>&, bool) constructor\endlink
 * or the factor() member function of \a A to \c false,
 * calling \link condition(const RWSymFact<TypeT>&) condition()\endlink
 * generates an exception.
 */
template <class TypeT>
inline typename rw_numeric_traits<TypeT>::norm_type  condition(const RWSymFact<TypeT>& A)
{
    return A.condition();
}

#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/symfct.cc>
#endif


#endif
