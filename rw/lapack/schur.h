#ifndef __RWSCHUR_H__
#define __RWSCHUR_H__
/*
 * SchurDecomp - Schur decomposition of a matrix
 *
 * $Id: //lapack/13/rw/lapack/schur.h#1 $
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
 * The Schur decomposition is often more useful than a full
 * eigendecomposition.  It provides the eigenvalues, and orthogonal
 * bases for invariant subspaces --- what more could you want?
 *
 * This class includes an optional balancing transformation in addition
 * to the standard Schur decomposition.
 */

#include <rw/dcomplex.h>
#include <rw/lapkdefs.h>
#include <rw/math/mathvec.h>
#include <rw/math/genmat.h>
#include <rw/lapack/bal.h>

template <class TypeT>
class RWHessenbergDecomp;

/**
 * @ingroup nonsymmetric_eigenvalue_decomposition_classes
 * @brief Encapsulates a Schur decomposition as well as an
 * optional balance transformation.
 *
 * A Schur decomposition uses orthogonal transformations to reduce
 * a matrix \e A to a matrix \e T that is upper quasitriangular.
 * For a complex matrix, \e T is actually triangular. For a
 * real matrix, \e T is triangular, except the diagonal can
 * contain 2 x 2 blocks, allowing nonzero entries on the subdiagonal.
 * The columns of the orthogonal transformation matrix, \e Z,
 * are called Schur vectors.
 *
 * The class \link RWSchurDecomp RWSchurDecomp<T> \endlink encapsulates
 * a Schur decomposition as well as an optional balance transformation.
 * The decomposition of a matrix \e A is:
 *
 * \f$A = BZTZ'B^{-1}\f$
 *
 * where \e Z is orthogonal, \e T is quasi-upper triangular,
 * and \e B is a balance transformation (see \link RWBalanceTransform
 * RWBalanceTransform<T>\endlink). The Schur decomposition yields
 * eigenvalues immediately. Since the eigenvalues of \e A and
 * \e T are the same, and since \e T is quasi-upper triangular,
 * the eigenvalues are simply either the diagonal elements of \e T,
 * or the complex conjugate pair corresponding to the eigenvalues
 * of a 2 x 2 block on the diagonal. The invariant subspace corresponding
 * to the first \e k eigenvalues of \e A is spanned by the
 * first \e k Schur vectors; since the matrix of Schur vectors,
 * \e Z, is orthogonal, this basis is orthonormal. Often, the
 * Schur vectors are preferable to the eigenvectors themselves because
 * of this orthogonality.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/schur.h>
 *
 * RWSchurDecomp<double> schur(A);     // A is an
 *                                   // RWGenMat<double>
 * @endcode

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
 *     RWSchurDecomp<double> schur(A,false,false); //false for
 *                                                 // no balancing
 *     std::cout << "Input matrix: "  << A << std::endl;
 *     std::cout << "eigenvalues: "   <<
 *                  schur.eigenvalues() << std::endl;
 *     std::cout << "Schur vectors: " << schur.Z()
 *               << std::endl;
 *     return 0;
 * }
 * @endcode
 *
 */
template <class TypeT>
class RWSchurDecomp
{
private:
    RWBalanceTransform<TypeT> B_;  // Balancing transformation
    RWGenMat<TypeT>           T_;  // Schur matrix
    RWGenMat<TypeT>           Z_;  // Schur vectors
    RWMathVec<DComplex>              w_;  // Eigenvalues

    //these will exists for DComplex only:
    void factor_specialized(const RWHessenbergDecomp<TypeT>&);
    bool moveToFront_specialized(const RWMathVec<int>&); // Move the listed eigenvalues to the front (returns true if all went well)

public:
    /**
     * Default constructor. Builds a decomposition of size 0 x 0.
     */
    RWSchurDecomp();

    /**
     * Constructs the Schur decomposition of the matrix \a A. The boolean
     * parameters determine whether or not the permutation or scaling
     * parts of the balance transformation are applied prior to forming
     * the Schur decomposition.
     */
    RWSchurDecomp(const RWGenMat<TypeT>& A, bool permute = true, bool scale = true);

    /**
     * Builds a Schur decomposition of the matrix represented by the
     * balance decomposition.
     */
    RWSchurDecomp(const RWBalanceDecomp<TypeT>&);

    /**
     * Builds a Schur decomposition of the matrix represented by the
     * Hessenberg decomposition.
     */
    RWSchurDecomp(const RWHessenbergDecomp<TypeT>&);

    /**
     * Replaces the current decomposition with the Schur decomposition
     * of the matrix \a A. The boolean parameters determine whether
     * or not the permutation or scaling parts of the balance transformation
     * are applied prior to forming the Schur decomposition.
    */
    void factor(const RWGenMat<TypeT>& A, bool permute = true, bool scale = true);

    /**
     * Replaces the current decomposition with the Schur decomposition
     * of the matrix represented by the balance decomposition.
     */
    void factor(const RWBalanceDecomp<TypeT>&);

    /**
     * Replaces the current decomposition with the Schur decomposition
     * of the matrix represented by the Hessenberg decomposition.
     */
    void factor(const RWHessenbergDecomp<TypeT>&);

    /**
     * Returns the number of columns in the matrix that the decomposition
     * represents.
     */
    unsigned            cols() const {
        return unsigned(T_.cols());
    }

    /**
     * Returns the number of rows in the matrix that the decomposition
     * represents.
     */
    unsigned            rows() const {
        return unsigned(T_.rows());
    }

    /**
     * Returns \c true if the decomposition was successfully computed,
     * \c false if not.
     */
    bool           good() const;// did this decomposition get computed properly

    /**
     * Computes an explicit representation of the balance transformation.
     */
    RWGenMat<TypeT>           B() const;

    /**
     * Computes an explicit representation of the orthogonal matrix
     * \c Z.
     */
    RWGenMat<TypeT>           Z() const;

    /**
     * Computes an explicit representation of the quasi-upper triangular matrix
     * \c T.
     */
    RWGenMat<TypeT>           T() const;

    /**
     * Returns the \a i <SUP>th</SUP> eigenvalue.
     */
    DComplex                 eigenvalue(int) const;

    /**
     * Returns a vector of all computed eigenvalues.
     */
    RWMathVec<DComplex>              eigenvalues() const {
        return w_;
    };

    /**
     * Computes the inner product of the balance transformation \c B,
     * or its (conjugate) transpose inverse, and the vector \a x.
     */
    RWMathVec<TypeT>              Bx(const RWMathVec<TypeT>& x) const;

    /**
     * @copydoc Bx(const RWMathVec<TypeT>&) const
     */
    RWMathVec<TypeT>              BInvTx(const RWMathVec<TypeT>& x) const;

    /**
     * Computes the inner product of the orthogonal matrix \c Z, or
     * its (conjugate) transpose, and the vector \a x.
     */
    RWMathVec<TypeT>              Zx(const RWMathVec<TypeT>& x) const;

    /**
     * @copydoc Zx(const RWMathVec<TypeT>&) const
     */
    RWMathVec<TypeT>              ZTx(const RWMathVec<TypeT>& x) const;

    /**
     * Computes the inner product of the balance transformation \c B,
     * or its (conjugate) transpose inverse, and the matrix \a X.
     */
    RWGenMat<TypeT>           BX(const RWGenMat<TypeT>& X) const;

    /**
     * @copydoc BX(const RWGenMat<TypeT>&) const
     */
    RWGenMat<TypeT>           BInvTX(const RWGenMat<TypeT>& X)const;

    /**
     * Computes the inner product of the orthogonal matrix \c Z, or
     * its (conjugate) transpose, and the matrix \a X.
     */
    RWGenMat<TypeT>           ZX(const RWGenMat<TypeT>& X) const;

    /**
     * @copydoc ZX(const RWGenMat<TypeT>&) const
     */
    RWGenMat<TypeT>           ZTX(const RWGenMat<TypeT>& X) const;

    /**
     * Reorders the decomposition so that the \a i <SUP>th</SUP>eigenvalue
     * is moved to the \a j <SUP>th</SUP> position. In the real case,
     * if \a i corresponds to a 2 x 2 block (a complex eigenvalue),
     * the entire block is moved. The function returns where the eigenvalue
     * was actually moved to; this may differ from \a j for two reasons:
     * it may have been impossible to move the eigenvalue to position
     * \a j due to the presence of 2 x 2 blocks, or the matrix may be
     * extremely ill-conditioned.
     */
    int                 move(int i, int j);             // Move an eigenvalue, returns where it actually ended up

    /**
     * Reorders the decomposition so that the eigenvalues whose indices
     * are passed in the vector are moved to the front. In the real
     * case, if an index corresponds to a 2 x 2 block (a complex eigenvalue),
     * the entire block is moved. The function returns \c true if the
     * reordering is successful.
     */
    bool           moveToFront(const RWMathVec<int>&); // Move the listed eigenvalues to the front (returns true if all went well)
};



#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/schur.cc>
#endif

#endif
