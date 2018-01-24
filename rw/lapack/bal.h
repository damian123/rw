#ifndef __RWBAL_H__
#define __RWBAL_H__

/*
 * BalanceTransform - combined permutation and diagonal balancing transformation
 * BalanceDecomp    - balance decomposition of a matrix
 *
 * This is the representation of a balancing transformation.  Sometimes you
 * can improve the quality of eigenvectors and eigenvalues by first scaling
 * the matrix so the rows and columns have approximately the same norm.  This
 * is done by the transformation encapsulated in these classes.
 *
 * The Transform class encapsulates just the transform itself.  The Decomp
 * class encapsulates a decomposition of a particular matrix.  The only way
 * to set up a Transform is via the Decomp class.  This is true because the
 * transform's only constructor (excepting the copy ctor) is private, and
 * Decomp is a friend class.
 *
 *
 * $Id: //lapack/13/rw/lapack/bal.h#1 $
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
 */

#include <rw/math/mathvec.h>
#include <rw/math/genmat.h>
#include <rw/lapack/latraits.h>

template <class TypeT>
class RWBalanceDecomp;


/**
 * @ingroup decomposition_classes
 *
 * @brief Encapsulates the permutation and balance transformations on a
 * nonsymmetric matrix before computing its eigenvalues.
 *
 * A permutation and balance transformation is often performed on
 * a nonsymmetric matrix before computing its eigenvalues. The permutation
 * attempts to break the matrix into three independent blocks such
 * that the first and last block are triangular, and the balance
 * transformation is a diagonal scaling transformation that attempts
 * to make the row and column norms close to each other. The class \link
 * RWBalanceTransform RWBalanceTransform<T>\endlink encapsulates
 * these two transformations.
 *
 * The class \link RWBalanceTransform RWBalanceTransform<T>\endlink
 * is \e not often required explicitly; most often it is used
 * implicitly by the nonsymmetric eigenvalue decompositions and
 * servers. If you do need to use this class explicitly, it is usually
 * best to construct an object and extract the transform from the
 * object.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/bal.h>
 *
 * RWBalanceTransform<double> dtrans;
 * @endcode
 */
template <class TypeT>
class RWBalanceTransform
{

    friend class RWBalanceDecomp<TypeT>;

private:
    char    job_;      // Encodes whether this does scaling, permuting, or both
    rw_lapack_int_t    ilo_, ihi_; // breakup of the matrix into disconnected pieces, if possible
    RWMathVec<typename rw_numeric_traits<TypeT>::norm_type>  scale_; // details of the permutations and scaling factors
    void init(RWGenMat<TypeT>* A, bool permute, bool scale);
    // A is overwritten with the balanced matrix.
public:

    /**
     * Constructs an empty transformation.
     */
    RWBalanceTransform();

    /**
     * Initializes the transformation.
     */
    RWBalanceTransform<TypeT>& operator=(const RWBalanceTransform&);

    /**
     * Returns the number of columns in the transformation matrix.
     */
    unsigned     cols() const {
        return (unsigned)scale_.length();
    }

    /**
     * Returns the number of rows in the transformation matrix.
     */
    unsigned     rows() const {
        return (unsigned)scale_.length();
    }

    /**
     * If a permutation breaks the matrix into three independent blocks,
     * this function returns the index where the middle block begins.
     * The part of the matrix above this block is triangular, so its
     * eigenvalues need \e not be computed.
     */
    unsigned     lowIndex() const {
        return (unsigned)ilo_;
    }

    /**
     * If a permutation breaks the matrix into three independent blocks,
     * this function returns the index where the middle block ends.
     * The part of the matrix below this block is triangular, so its
     * eigenvalues need \e not be computed.
     */
    unsigned     highIndex() const {
        return (unsigned)ihi_;
    }

    /**
     * Computes the inner product of the transform and \a x. This is
     * typically used to transform right eigenvectors.
     */
    RWGenMat<TypeT>    transform(const RWGenMat<TypeT>& x) const;// Compute BX

    /**
     * Computes the inner product of the inverse transpose of the transform
     * and \a x. This is typically used to transform left eigenvectors.
     */
    RWGenMat<TypeT>    invTransform(const RWGenMat<TypeT>& x) const;// Compute inv(B)'X

    /**
     * @copydoc transform(const RWGenMat<TypeT>&) const
     */
    RWMathVec<TypeT>   transform(const RWMathVec<TypeT>& x) const;// Compute BX

    /**
     * @copydoc invTransform(const RWGenMat<TypeT>&) const
     */
    RWMathVec<TypeT>   invTransform(const RWMathVec<TypeT>& x) const;// Compute inv(B)'X
};


/**
 * @ingroup decomposition_classes
 *
 * @brief Encapsulates the decomposition implicit in the permutation and balance
 * transformations on a nonsymmetric matrix before computing its eigenvalues.
 *
 * A permutation and balance transformation is often performed on
 * a nonsymmetric matrix before computing its eigenvalues. The permutation
 * attempts to break the matrix into three independent blocks such
 * that the first and last block are triangular, and the balance
 * transformation is a diagonal scaling transformation that attempts
 * to make the row and column norms close to each other. The class
 * \link RWBalanceDecomp RWBalanceDecomp<T>\endlink encapsulates
 * the decomposition:
 *
 * \f[
 * A = BCB^{-1}
 * \f]
 *
 * where <i>B=PD</i>, an amalgamation of the permutation and balance
 * transformations. This decomposition is used internally by the
 * nonsymmetric eigenvalue decomposition classes, including the
 * Schur and Hessenberg decomposition classes.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/bal.h>
 *
 * RWBalanceDecomp<double> bal(A);     // A is a RWGenMat<double>
 * @endcode
 */
template <class TypeT>
class RWBalanceDecomp
{
private:
    RWGenMat<TypeT>           C_;
    RWBalanceTransform<TypeT> B_;
    // C_ must be declared before B_ because B_ uses C_ in its constructor
public:

    /**
     * Constructs an empty decomposition.
     */
    RWBalanceDecomp();

    /**
     * Constructs a decomposition of the matrix \a A. The boolean parameters
     * control whether permutation, scaling (balancing), both, or neither
     * are done. The matrix must be square.
     */
    RWBalanceDecomp(const RWGenMat<TypeT>& A, bool permute = true, bool scale = true);

    void factor(const RWGenMat<TypeT>& A, bool permute = true, bool scale = true);

    /**
     * Returns the number of columns in the decomposition.
     */
    unsigned                   cols() const {
        return (unsigned)C_.cols();
    }

    /**
     * Returns the number of rows in the decomposition.
     */
    unsigned                   rows() const {
        return (unsigned)C_.rows();
    }

    /**
     * Returns the balanced matrix.
     */
    const RWGenMat<TypeT>            C() const {
        return C_;
    }

    /**
     * Returns a representation of the balance transformation computed.
     */
    const RWBalanceTransform<TypeT>& B() const {
        return B_;
    }
};

#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/bal.cc>
#endif

#endif
