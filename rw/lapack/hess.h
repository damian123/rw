#ifndef __RWHESS_H__
#define __RWHESS_H__
/*
 * HessenbergDecomp - Hessenberg decomposition of a matrix
 *
 * $Id: //lapack/13/rw/lapack/hess.h#1 $
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
 * This decomposition is usually formed as a precursor to computing
 * the eigenvalues and/or Schur decomposition of a non-symmetric matrix.
 *
 * This class includes an optional balancing transformation in addition
 * to the standard Hessenberg decomposition.
 */

#include <rw/lapkdefs.h>
#include <rw/math/mathvec.h>
#include <rw/math/genmat.h>
#include <rw/lapack/bal.h>

/**
 * @ingroup nonsymmetric_eigenvalue_decomposition_classes
 * @brief Encapsulates a Hessenberg decomposition as well as an optional
 * balance transformation.
 *
 * A Hessenberg decomposition uses orthogonal transformations to
 * reduce a matrix <i>A</i> to a matrix <i>H</i> that is 0 below
 * the first subdiagonal. Computation of the Hessenberg decomposition
 * is the first step in the QR method for computing eigenvalues
 * of a nonsymmetric matrix.
 *
 * The class \link RWHessenbergDecomp RWHessenbergDecomp<T> \endlink
 * encapsulates a Hessenberg decomposition as well as an optional
 * balance transformation. The decomposition of a matrix <i>A</i>
 * is:
 *
 * \f$A = BQHQ'B^{-1}\f$
 *
 * where <i>Q</i> is orthogonal, <i>H</i> is 0 below the first subdiagonal,
 * and <i>B</i> is a balance transformation. See the entry \link
 * RWBalanceTransform RWBalanceTransform<T> \endlink.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/hess.h>
 *
 * RWHessenbergDecomp<double> hess(A);   // A is a
 *                                          // RWGenMat<double>
 * @endcode
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/lapack/hess.h>
 *
 * int main()
 * {
 *     RWGenMat<double> A;
 *     std::cin >> A;
 *     RWHessenbergDecomp<double> hess(A);
 *     std::cout << "Input matrix: " << A << std::endl;
 *     std::cout << "B: " << hess.B() << std::endl;
 *     std::cout << "Q: " << hess.Q() << std::endl;
 *     std::cout << "H: " << hess.H() << std::endl;
 *     return 0;
 * }
 * @endcode
 *
 */
template <class TypeT>
class RWHessenbergDecomp
{
private:
    RWBalanceTransform<TypeT> B_;  // Balancing transformation
    RWGenMat<TypeT>           H_;  // Hessenberg matrix and Q info
    RWMathVec<TypeT>        tau_;// scalar reflectors for generating Q

public:
    typedef typename rw_numeric_traits<TypeT>::norm_type norm_type;

    /**
     * Default constructor. Builds a decomposition of size 0 x 0.
     */
    RWHessenbergDecomp();

    /**
     * Constructs the Hessenberg decomposition of the matrix \a A. The
     * boolean parameters determine whether or not the permutation or
     * scaling parts of the balance transformation are applied prior
     * to forming the Hessenberg decomposition.
     */
    RWHessenbergDecomp(const RWGenMat<TypeT>& A, bool permute = true, bool scale = true);

    /**
     * Builds a Hessenberg decomposition of the matrix represented by
     * the balance decomposition.
     */
    RWHessenbergDecomp(const RWBalanceDecomp<TypeT>&);

    /**
     * Replaces the current decomposition with the Hessenberg decomposition
     * of the matrix \a A. The boolean parameters determine whether
     * or not the permutation or scaling parts of the balance transformation
     * are applied prior to forming the Hessenberg decomposition.
     */
    void factor(const RWGenMat<TypeT>& A, bool permute = true, bool scale = true);

    /**
     * Replaces the current decomposition with the Hessenberg decomposition
     * of the matrix represented by the balance decomposition.
     */
    void factor(const RWBalanceDecomp<TypeT>&);

    /**
     * Returns the number of columns in the matrix that the decomposition
     * represents.
     */
    unsigned            cols() const {
        return unsigned(H_.cols());
    }
    unsigned            rows() const {
        return unsigned(H_.rows());
    }

    /**
     * Computes an explicit representation of the balance transformation.
     */
    RWGenMat<TypeT>           B() const;

    /**
     * Returns an object which represents the balance transformation.
     */
    RWBalanceTransform<TypeT> balanceTransform() const {
        return B_;
    }

    /**
     * Computes an explicit representation of the orthogonal matrix
     * \c Q.
     */
    RWGenMat<TypeT>           Q() const;
    RWGenMat<TypeT>           H() const;

    /**
     * Computes the inner product of the balance transformation \c B
     * and the vector \a x.
     */
    RWMathVec<TypeT>        Bx(const RWMathVec<TypeT>& x) const;

    /**
     * Computes the (conjugate) transpose inverse of the balance transformation
     * \c B and the vector \a x.
     */
    RWMathVec<TypeT>        BInvTx(const RWMathVec<TypeT>& x) const;

    /**
     * Computes the inner product of the orthogonal matrix \c Q
     * and the vector \a x.
     */
    RWMathVec<TypeT>        Qx(const RWMathVec<TypeT>& x) const;

    /**
     * Computes the (conjugate) transpose of the orthogonal matrix \c Q
     * and the vector \a x.
     */
    RWMathVec<TypeT>        QTx(const RWMathVec<TypeT>&) const;

    /**
     * Computes the inner product of the balance transformation \c B
     * and the matrix \a X.
     */
    RWGenMat<TypeT>         BX(const RWGenMat<TypeT>& X) const;

    /**
     * Computes the (conjugate) transpose inverse of the balance transformation
     * \c B and the matrix \a X.
     */
    RWGenMat<TypeT>         BInvTX(const RWGenMat<TypeT>& X)const;

    /**
     * Computes the inner product of the orthogonal matrix \c Q
     * and the matrix \a X.
     */
    RWGenMat<TypeT>         QX(const RWGenMat<TypeT>& X) const;

    /**
     * Computes the (conjugate) transpose of the orthogonal matrix \c Q
     * and the matrix \a X.
     */
    RWGenMat<TypeT>         QTX(const RWGenMat<TypeT>& X) const;
};

#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/hess.cc>
#endif

#endif
