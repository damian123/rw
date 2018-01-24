#ifndef __GENFACT_H__
#define __GENFACT_H__

/***************************************************************************
 *
 * genfact.h
 *
 * $Id: //math/13/rw/math/genfact.h#1 $
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

#include <rw/mathdefs.h>
#include <rw/math/genmat.h>
#include <rw/math/mathvec.h>  // Need to store an integer vector of pivots
#include <rw/math/numtrait.h>

/**
 * @ingroup lu_factorization_classes
 * @brief A templatized LU factorization class that holds the LU factorization
 * of a general square matrix of type T.
 *
 * Class \link RWGenFact RWGenFact<T> \endlink is a templatized
 * LU factorization class. This class holds the LU factorization
 * of a general square matrix of type T. Once such a factorization
 * is built, it can be used to invert a matrix, calculate its determinant,
 * or solve a set of simultaneous linear equations.
 *
 * Optionally, a condition number can be calculated and then recovered
 * via member function condition(), which returns the reciprocal
 * of the standard definition for the condition number of a matrix.
 *
 * The result is in the range [0,1]. The closer the result is to
 * 0, the closer the matrix is to being singular. See Dongarra et
 * al. (1979) for additional information.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/math/genfact.h>
 * #include <rw/math/genmat.h>
 * RWGenMat<T> matrix;
 * template <class T>
 * RWGenFact<T> LUFactor(matrix);
 * @endcode
 */
template <class T>
class RWGenFact
{
private:

    RWGenMat<T> factorization;
    RWMathVec<RWPivotType> pivots;
    // The factorization and the pivot vector

    typename rw_numeric_traits<T>::norm_type sing_eps;
    // epsilon used by isSingular() and fail()

    typename rw_numeric_traits<T>::norm_type cond;
    // cond is the reciprocal condition number if
    // calculated.  If condition number not calculated
    // contains 0 for singularity else -1.

protected:


    void dofactor(bool estimateCondition);
    // Do the factorization.  Call this after the matrix
    // variable factorization is set up.

    void calldi(const RWGenMat<T>& A, int job, T* det) const;
    // Call the linpack xxxdi routine with the job number indicated

public:

    /**
     * This is a convenience typedef.  The condition numbers have type dependent
     * on the traits class.
     */
    typedef typename rw_numeric_traits<T>::norm_type  norm_type;

    /**
     * Sets up an empty LU factorization. Member function factor() can
     * then be used to actually calculate the factorization.
     */
    RWGenFact();

    /**
     * Copy constructor. Reference semantics are used; that is, a <i>shallow
     * copy</i> of the argument is made.
     */
    RWGenFact(const RWGenFact<T>& A);

    /**
     * Constructs an LU factorization from the matrix \a m. If the argument
     * \a estimateCondition is \c true, a condition number is also calculated.
     * Setting \a estimateCondition to \c false saves some time (perhaps 20%),
     * but gives you less information about how ill-conditioned the matrix is. If
     * the matrix \a m is not square, an exception with value \c RWLAPK_NOTSQUARE
     * occurs. This constructor also serves as a type conversion from
     * \link RWGenMat RWGenMat<T> \endlink to \link RWGenFact RWGenFact<T>
     * \endlink
     */
    RWGenFact(const RWGenMat<T>& A, bool estimateCondition = true);

    // You can only call factor() for a matrix that has been constructed
    // with the default constructor.
    /**
     * Recalculates the factorization, this time using the matrix \a
     * m. If the argument \a estimateCondition is \c true, a condition number is
     * also calculated.
     */
    void factor(const RWGenMat<T>& A, bool estimateCondition = true);

    ~RWGenFact();

    /**
     * Returns \c true if the factorization succeeds.
     */
    bool good() const {
        return !fail();
    }

    /**
     * Returns \c true if the factorization fails, perhaps due to singularity,
     * otherwise returns \c false.
     */
    bool fail() const;

    /**
     * Checks to see if the matrix is singular. If isSingular() returns
     * a non-zero value, the matrix is singular. Note that the LU factorization
     * is still defined; however, an attempt to use it to compute the
     * matrix inverse fails.
     */
    bool isSingular() const;
    int       rows() const {
        return (int)factorization.rows();
    }
    int       cols() const {
        return (int)factorization.cols();
    }

    /**
     * An exception with value \c RWMATH_NOCONDITION occurs if the condition
     * number is not calculated.
     */
    norm_type condition() const;

    norm_type singEps() const {
        return sing_eps;
    }

    void      setSingEps(norm_type eps);
    // If fail() returns true calling solve() or inverse() or ... may
    // fail.  condition() returns an approximation to the reciprocal
    // condition number of the factorization.

    RWMathVec<T> solve(const RWMathVec<T>& b) const;

    RWGenMat<T> solve(const RWGenMat<T>& b) const;

    T determinant() const;

    RWGenMat<T> inverse() const;
};

/**
 * @relates RWGenFact
 *
 * Calculates the solution to \f$ A*x = b \f$, where \a m is an
 * LU factorization constructed from the \link RWGenMat RWGenMat<T>
 * \endlink \c A.
 *
 * If the factorization <i>m</i> is invalid, perhaps because the
 * original matrix <b>A</b> was singular, an exception with value
 * \c LPAK_CANTSOLVE occurs. If the number of elements in <b>x</b> does
 * not match the order of <b>A</b>, an exception with value \c LPAK_VECLENGTH
 * occurs.
 */
template <class T>
inline RWMathVec<T> solve(const RWGenFact<T>& m, const RWMathVec<T>& b)
{
    return m.solve(b);
}

/**
 * @relates RWGenFact
 *
 * Calculates the solution to \f$ A*x = b \f$.
 */
template <class T>
inline RWMathVec<T> solve(const RWGenMat<T>& A, const RWMathVec<T>& b)
{
    return RWGenFact<T>(A).solve(b);
}

/**
 * @relates RWGenFact
 *
 * Calculates the solution to \f$ A*x = b \f$, where \a b is an
 * LU factorization constructed from the \link RWGenMat RWGenMat<T>
 * \endlink \a A.
 *
 * If the factorization <i>m</i> is invalid, perhaps because the
 * original matrix <b>A</b> was singular, an exception with value
 * \c LPAK_CANTSOLVE occurs. If the number of elements in <b>x</b> does
 * not match the order of <b>A</b>, an exception with value \c LPAK_VECLENGTH
 * occurs.
 */
template <class T>
inline RWGenMat<T> solve(const RWGenFact<T>& m, const RWGenMat<T>& b)
{
    return m.solve(b);
}

/**
 * @relates RWGenFact
 *
 * Calculates the solution to \f$ A*x = b \f$.
 */
template <class T>
inline RWGenMat<T> solve(const RWGenMat<T>& A, const RWGenMat<T>& b)
{
    return RWGenFact<T>(A).solve(b);
}

/**
 * @relates RWGenFact
 *
 * Returns the determinant of the matrix from which an LU factorization
 * \a A is constructed.
 */
template <class T>
inline T    determinant(const RWGenFact<T>& A)
{
    return A.determinant();
}

/**
 * @relates RWGenMat
 *
 * Returns the determinant of the matrix from which an LU factorization
 * \a A is constructed.
 */
template <class T>
inline T    determinant(const RWGenMat<T>& A)
{
    return RWGenFact<T>(A).determinant();
}

template <class T>
T determinant(const RWGenMat<T>& A, typename RWGenFact<T>::norm_type sing_eps);

/**
 * @relates RWGenFact
 *
 * Returns the inverse of the matrix from which an existing LU factorization
 * was constructed. If the factorization <i>A</i> is invalid, perhaps
 * because the original matrix was singular, an exception with value
 * \c LPAK_CANTSOLVE occurs.
 */
template <class T>
inline RWGenMat<T> inverse(const RWGenFact<T>& A)
{
    return A.inverse();
}

/**
 * @relates RWGenFact
 *
 * Returns the inverse of the matrix from which an existing LU factorization
 * was constructed. If the factorization of <i>A</i> is invalid, perhaps
 * because the original matrix was singular, an exception with value
 * \c LPAK_CANTSOLVE occurs.
 */
template <class T>
inline RWGenMat<T> inverse(const RWGenMat<T>& A)
{
    RWGenFact<T> temp(A);
    return temp.inverse();
}

/**
 * @relates RWGenFact
 *
 * Returns the inverse condition number of the
 * matrix whose LU factorization is \a A.
 */
template <class T>
inline double condition(const RWGenFact<T>& A)
{
    return A.condition();
}

/**
 * @relates RWGenFact
 * Returns the inverse condition number of matrix \a A
 */
template <class T>
inline double condition(const RWGenMat<T>& A)
{
    return RWGenFact<T>(A).condition();
}

#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/math/genfact.cc>
#endif

#endif
