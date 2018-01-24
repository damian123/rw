#ifndef __RWTD_H__
#define __RWTD_H__

/*
 * RWHermTriDiagDecomp      - Abstract base class for tri-diagonal decompositions
 * RWHermDenseTriDiagDecomp - Decomposition of a symmetric/Hermitian matrix
 * RWHermBandTriDiagDecomp  - Decomposition of a symmetric/Hermitian banded matrix
 *
 * $Id: //lapack/13/rw/lapack/td.h#1 $
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
 * This class represents a decomposition of the form A=QTQ', where A is
 * symmetric/Hermitian, Q is orthogonal/unitary, and TypeT is real tridiagonal.
 * This is usually done as a precursor to computing eigenvalues, since
 * the eigenvalues of A and TypeT are the same, and those of TypeT are easier to
 * compute.
 *
 * This class is used internally in the eigen-decomposition and
 * eigen-server classes.  The only reason a user should be interested
 * in this class is to implement his own routines to compute
 * eigenvalues/vectors of a tri-diagonal matrix as part of a new
 * eigen-server class.
 *
 * Each derived class must supply an implementation of the routine to
 * compute QC for a matrix C.  The routine to compute Qx for a vector x
 * by default calls the QC routine.
 *
 * The banded class can optionally not bother storing Q.  This can save
 * a lot of memory if the eigenvectors are not needed, or if you are
 * content to obtain the eigenvectors by inverse iteration later.
 */

#include <rw/lapkdefs.h>
#include <rw/math/mathvec.h>
#include <rw/math/genmat.h>
#include <rw/lapack/latraits.h>
#include <rw/dcomplex.h>

template <class TypeT>
class RWHermMat;
template <class TypeT>
class RWHermBandMat;
template <class TypeT>
class RWSymMat;
template <class TypeT>
class RWSymBandMat;

/**
 * @ingroup symmetric_eigenvalue_decomposition_classes
 * @brief Encapsulates the tridiagonal decomposition of a symmetric matrix.
 *
 * The class \link RWTriDiagDecomp RWTriDiagDecomp<T> \endlink encapsulates
 * the tridiagonal decomposition of a symmetric matrix <i>A</i>:
 *
 * \f$
 * A = Q^{\prime}TQ
 * \f$
 *
 * where <i>Q</i> is orthogonal and <i>T</i> is tridiagonal and
 * real, even in the complex case. This decomposition is often done
 * as a precursor to computing eigenvalues, since the eigenvalues
 * of <i>T</i> and <i>A</i> are the same, and those of <i>T</i>
 * are easier to compute. This class is used internally by the symmetric
 * eigenvalue classes.
 *
 * The class \link RWTriDiagDecomp RWTriDiagDecomp<T> \endlink is
 * an abstract base class. Subclasses must implement the pure virtual
 * function \c decompose. This separates the implementation of the
 * storage of <i>Q</i> from the interface, so that band and dense
 * decompositions can use different storage techniques. The Linear
 * Algebra Module provides the derived classes \link RWDenseTriDiagDecomp
 * RWDenseTriDiagDecomp<T> \endlink and \link RWBandTriDiagDecomp
 * RWBandTriDiagDecomp<T> \endlink.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/td.h>
 * @endcode
 *
 * @section example Example
 *
 * See specific subclasses for examples.
 *
 */
template <class TypeT>
class RWTriDiagDecomp
{
public:

    virtual ~RWTriDiagDecomp() {};

    /**
     * Typedef for the usual return type of numerical <tt>norm</tt>-like
     * functions. For more information, see rw_numeric_traits<T>::norm_type.
     */
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;
private:
    RWMathVec<rt> diag;    // diagonal of TypeT
    RWMathVec<rt> offdiag; // sub/super diagonal of TypeT
public:
    /**
     * Sets the tridiagonal matrix directly.
     */
    void set(const RWMathVec<rt>& diag_, const RWMathVec<rt>& offdiag_);

    /**
     * Transforms the vector from tridiagonal decomposition space back
     * to the original matrix space. This amounts to multiplying it
     * by the matrix <i>Q</i>.
     */
    virtual RWMathVec<TypeT> transform(const RWMathVec<rt>&) const;

    /**
     * @copydoc transform(const RWMathVec<rw_numeric_traits<TypeT>::norm_type>&) const
     */
    virtual RWGenMat<TypeT> transform(const RWGenMat<rt>&) const = 0;

    /**
     * Returns the diagonal of the symmetric tridiagonal matrix <i>T</i>.
     */
    const RWMathVec<rt> diagonal() const {
        return diag;
    }

    /**
     * Returns the subdiagonal or superdiagonal of the symmetric tridiagonal
     * matrix <i>T</i>.
     */
    const RWMathVec<rt> offDiagonal() const {
        return offdiag;
    }

    /**
     * Returns the number of rows in the decomposition.
     */
    unsigned     rows() const {
        return (unsigned)diag.length();
    }

    /**
     * Returns the number of columns in the decomposition.
     */
    unsigned     cols() const {
        return (unsigned)diag.length();
    }
};

/**
 * @ingroup decomposition_classes
 * @brief Encapsulates the symmetric tridiagonal decomposition
 * of a dense symmetric matrix.
 *
 * The class \link RWDenseTriDiagDecomp RWDenseTriDiagDecomp<T>
 * \endlink encapsulates the symmetric tridiagonal decomposition
 * of a dense symmetric matrix <i>A</i>, where:
 *
 * \f$
 * A = QTQ^\prime
 * \f$
 *
 * and <i>Q</i> is orthogonal and <i>T</i> is real tridiagonal symmetric.
 * These classes are used internally by the symmetric eigenvalue
 * classes.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/td.h>
 *
 * RWDenseTriDiagDecomp<double> QTQ(A);    // A is a
 *                                   // RWSymMat<double>
 * @endcode

 * @section example Example
 *
 * @code
 * #include <rw/lapack/symmat.h>
 * #include <rw/lapack/td.h>
 * #include <iostream>
 *
 * int main()
 * {
 *     RWSymMat<double> A;
 *     std::cin >> A;
 *     RWDenseTriDiagDecomp<double> decomp(A);
 *     std::cout << "T's diagonal is" << decomp.diagonal() <<
 *                  std::endl;
 *     std::cout << "T's off diagonal is " << decomp.offDiagonal()
 *               << std::endl;
 *     return 0;
 * }
 * @endcode
 *
 */
template <class TypeT>
class RWDenseTriDiagDecomp : public RWTriDiagDecomp<TypeT>
{
public:
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;


private:
    RWMathVec<TypeT> Qdata;   // Data necessary to generate Q (if keepQ==true during construction)
    RWMathVec<TypeT> tau;     // Scalar factors of the elementary reflectors

    RWGenMat<DComplex> transformSpecialized(const RWGenMat<double>&) const;

public:
    /**
     * Constructs a tridiagonal decomposition of \a A.
     */
    RWDenseTriDiagDecomp(const typename rw_linear_algebra_traits<TypeT>::hermitian_type& A);

    // Documented in base class.
    virtual RWMathVec<TypeT> transform(const RWMathVec<rt>&) const;

    // Documented in base class.
    virtual RWGenMat<TypeT> transform(const RWGenMat<rt>&) const;

    virtual ~RWDenseTriDiagDecomp() {}
};

/**
 * @ingroup decomposition_classes
 * @brief Encapsulates a symmetric tridiagonal decomposition of a symmetric
 * banded matrix.
 *
 * The class \link RWBandTriDiagDecomp RWBandTriDiagDecomp<T>\endlink
 * encapsulates a symmetric tridiagonal decomposition of a symmetric
 * banded matrix <i>A</i>:
 *
 * \f$
 * A = QTQ^\prime
 * \f$
 *
 * where <i>Q</i> is orthogonal and <i>T</i> is real tridiagonal
 * symmetric. This class is used internally by the symmetric eigenvalue
 * classes.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/td.h>
 *
 * RWBandTriDiagDecomp<double>(A);  // A is a DoubleSymMat
 * @endcode

 * @section example Example
 *
 * @code
 * #include <rw/lapack/sbndmat.h>
 * #include <rw/lapack/td.h>
 * #include <iostream>
 *
 * int main()
 * {
 *     RWSymBandMat<double> A;
 *     std::cin >> A;
 *     RWSymBandTriDiagDecomp<double> decomp(A);
 *     std::cout << "T's diagonal is" << decomp.diagonal() << std::endl;
 *     std::cout << "T's off diagonal is " << decomp.offDiagonal()
 *                << std::endl;
 *     return 0;
 * }
 * @endcode
 *
 */
template <class TypeT>
class RWBandTriDiagDecomp : public RWTriDiagDecomp<TypeT>
{
public:
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;
private:
    RWGenMat<TypeT> Q;       // The matrix Q (if keepQ was true during construction)

    //this will exists for DComplex only:
    //RWGenMat<TypeT> transformSpecialized(const RWGenMat<typename rw_numeric_traits<TypeT>::norm_type>&) const;
    RWGenMat<DComplex> transformSpecialized(const RWGenMat<double>& C) const;

public:
    /**
     * Constructs a tridiagonal decomposition of \a A. By setting the
     * optional boolean parameter \a keepQ to \c false, the data necessary to
     * transform via \c Q is <i>not</i> stored. This can save substantial
     * storage costs, but an exception is thrown if you attempt to use
     * the member function transform.
     */
    RWBandTriDiagDecomp(const typename rw_linear_algebra_traits<TypeT>::banded_hermitian_type& A, bool keepQ = true);

    // Documented in base class.
    virtual RWGenMat<TypeT> transform(const RWGenMat<rt>& x) const;

    // Documented in base class.
    virtual RWMathVec<TypeT> transform(const RWMathVec<rt>& x) const;

    virtual ~RWBandTriDiagDecomp() {};
};



#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/td.cc>
#endif


#endif
