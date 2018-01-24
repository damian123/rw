#ifndef __RWEIG_H__
#define __RWEIG_H__

/*
 * EigDecomp - Eigenvalues of a general matrix
 *
 * $Id: //lapack/13/rw/lapack/eig.h#1 $
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
 * This class represents the eigenvalues/vectors of a non-symmetric
 * matrix.
 *
 */

#include <rw/lapkdefs.h>
#include <rw/math/genmat.h>
#include <rw/math/mathvec.h>

template <class T1>
class RWEigServer;

/**
 * @ingroup nonsymmetric_eigenvalue_decomposition_classes
 * @brief Encapsulates the eigenvalues and eigenvectors
 * of a nonsymmetric matrix.
 *
 * The class \link RWEigDecomp RWEigDecomp<T> \endlink encapsulates
 * the eigenvalues and eigenvectors of a nonsymmetric matrix. You
 * can construct an eigenvalue decomposition object in two ways:
 *
 * -  Use the decomposition class constructor. This has the advantage
 *    of simplicity, but allows only limited control over the computation.
 *    Your only choice is whether or not to compute eigenvectors.
 * -  Use an eigenvalue server object. This gives you more precise
 *    control over the computation. See \link RWEigServer RWEigServer<T>
 *    \endlink in this manual for details.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/eig.h>
 *
 * RWEigDecomp<double> eig(A);      // A is an
 *                                   // RWGenMat<double>
 * @endcode

 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/lapack/eig.h>
 *
 * int main()
 * {
 *    RWGenMat<double> A;
 *    std::cin >> A;
 *    RWEigDecomp<double> eig(A);
 *    std::cout << "eigenvalues: " << eig.eigenValues() <<
 *                 std::endl;
 *    std::cout << "left eigenvectors: " << eig.leftEigenVectors()
 *               << std::endl;
 *    std::cout << "right vectors: " << eig.rightEigenVectors() << endl;
 *    return 0;
 * }
 * @endcode
 *
 */
template <class TypeT>
class RWEigDecomp
{
private:
    unsigned  n;           // dimension of the matrix
    RWMathVec<DComplex>    lambda;      // computed eigenvalues (length may be < n)
    RWGenMat<TypeT> P;           // columns contain left eigenvectors
    RWGenMat<TypeT> Q;           // columns contain right eigenvectors
    bool computedAll; // Did I compute everything the server tried to compute?
    bool accurate;    // Were all results as accurate as possible?

public:
    /**
     * Default constructor. Builds a decomposition of a 0 x 0 matrix.
     */
    RWEigDecomp();

    /**
     * Copy and precision conversion constructors. Where possible, data
     * is referenced for efficiency.
     */
    RWEigDecomp(const RWEigDecomp<double>& A);

    /**
     * Constructs a representation of the eigenvalues and eigenvectors
     * of the matrix \a A. The boolean parameter \a computeVecs controls whether
     * eigenvectors are computed.
     */
    RWEigDecomp(const RWGenMat<TypeT>& A, bool computeVecs = true);

    /**
     * Constructs a representation of the eigenvalues and eigenvectors
     * of the matrix \a A. The boolean parameter \a computeVecs controls whether
     * eigenvectors are computed. The current contents of the decomposition are
     * lost.
     */
    void factor(const RWGenMat<TypeT>& A, bool computeVecs = true);

    /**
     * Assigns the passed value to this decomposition. The current contents
     * of the decomposition are lost.
     */
    RWEigDecomp<TypeT>& operator=(const RWEigDecomp<TypeT>&);

    /**
     * Returns the number of columns in the decomposed matrix.
     */
    unsigned        cols() const {
        return n;
    }

    /**
     * Returns the \a i<SUP>th</SUP> eigenvalue.
     */
    DComplex             eigenValue(int i) const;

    /**
     * Returns a vector of all computed eigenvalues.
     */
    const RWMathVec<DComplex>     eigenValues() const {
        return lambda;
    }

    /**
     * Returns the \a i<SUP>th</SUP> left eigenvector.
     */
    const RWMathVec<DComplex>     leftEigenVector(int i) const;

    /**
     * Returns a vector of all computed left eigenvectors.
     */
    const RWGenMat<DComplex>      leftEigenVectors() const;

    /**
     * Returns the \a i<SUP>th</SUP> right eigenvector.
     */
    const RWMathVec<DComplex>     rightEigenVector(int i) const;

    /**
     * Returns a matrix of all computed right eigenvectors.
     */
    const RWGenMat<DComplex>      rightEigenVectors() const;

    /**
     * Returns \c true if all desired eigenvalues and eigenvectors are
     * successfully computed to full desired accuracy.
     */
    bool       good() const;

    /**
     * Returns \c true if either an eigenvalue or eigenvector that is
     * supposed to be computed fails to be computed, or some of the
     * computed quantities are <i>not</i> computed to full desired accuracy.
     */
    bool       inaccurate() const;

    /**
     * Returns \c true if an eigenvalue or eigenvector that is supposed
     * to be computed fails to be computed.
     */
    bool       fail() const;

    /**
     * Returns the number of eigenvalues or eigenvectors represented
     * in this object.
     */
    unsigned        numEigenValues() const {
        return (unsigned)lambda.length();
    }

    /**
     * @copydoc numEigenValues()
     */
    unsigned        numLeftEigenVectors() const {
        return (unsigned)P.cols();
    }

    /**
     * @copydoc numEigenValues()
     */
    unsigned        numRightEigenVectors() const {
        return (unsigned)Q.cols();
    }

    /**
     * Returns the number of rows in the decomposed matrix.
     */
    unsigned        rows() const {
        return n;
    }

    template <class T1>
    friend class RWEigServer;
    template <class T1>
    friend class RWEigDecomp;

};


#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/eig.cc>
#endif

#endif
