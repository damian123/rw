#ifndef __RWSYMEIG_H__
#define __RWSYMEIG_H__

/*
 * SymEigDecomp - Spectral factorization of a symmetric/Hermitian matrix
 *
 * $Id: //lapack/13/rw/lapack/symeig.h#1 $
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
 * This class represents a spectral factorization of a symmetric/Hermitian
 * matrix.  In addition, it has constructors for constructing a
 * factorization from a symmetric/Hermitian or banded symmetric/Hermitian,
 * with an option of whether or not to compute eigenvectors.  The
 * actual computation is done by a subclass of DoubleSymEigServer.
 * If you want to control which server gets used, or need more
 * control over the computation, then you need to use an eigenserver
 * class directly.
 *
 */

#include <rw/lapkdefs.h>
#include <rw/lapack/symmat.h>
#include <rw/lapack/sbndmat.h>
#include <rw/math/genmat.h>
#include <rw/math/mathvec.h>

template <class TypeT>
class RWSymBandMat;

template <class TypeT>
class RWSymEigServer;

template <class T1>
class RWHermEigServer;
template <class T1>
class RWHermEigDecomp;
//template <class TypeT>
//class DoubleSymEigDecomp;   // declaration here for complex types

template <class TypeT>
class RWSymMat;

/**
 * @ingroup symmetric_eigenvalue_decomposition_classes
 * @brief Encapsulates the eigenvalues and eigenvectors of a symmetric matrix.
 *
 * The class \link RWSymEigDecomp RWSymEigDecomp<T> \endlink encapsulates
 * the eigenvalues and eigenvectors of a symmetric matrix. You can
 * construct an eigenvalue decomposition object in two ways:
 *
 * -  Use the decomposition class constructor. This has the advantage
 *    of simplicity, but allows only limited control over the computation.
 *    Your only choice is whether or not to compute eigenvectors.
 * -  Use an eigenvalue server object. This gives you more precise
 *    control over the computation. See \link RWSymEigServer
 *    RWSymEigServer<T> \endlink for details.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/symeig.h>    // RWSymEigDecomp<T>
 *
 * RWSymEigDecomp<double> eig(A);   // A is an RWSymMat<double>
 * @endcode
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/lapack/symeig.h>
 *
 * int main()
 * {
 *     RWSymMat<double> A;
 *     std::cin >> A;
 *     RWSymEigDecomp<double> eig(A);
 *     std::cout << "eigenvalues: " << eig.eigenValues() << std::endl;
 *     std::cout << "eigenvectors: " << eig.eigenVectors() << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 *
 */
template <class TypeT>
class RWSymEigDecomp
{
private:
    unsigned  n;           // dimension of the matrix
    RWMathVec<TypeT>    lambda;      // computed eigenvalues (length may be < n)
    RWGenMat<TypeT> P;           // columns contain eigenvectors
    bool computedAll; // Did I compute everything the server tried to compute?
    bool accurate;    // Were all results as accurate as possible?

public:
    /**
     * Default constructor. Builds a decomposition of a \c 0 x \c 0
     * matrix.
     */
    RWSymEigDecomp();

    /**
     * Copy and precision conversion constructor. Where possible, data
     * is referenced for efficiency.
     */
    RWSymEigDecomp(const RWSymEigDecomp<TypeT>& A);

    /**
     * Constructs a representation of the eigenvalues and eigenvectors
     * of the matrix \a A. The boolean parameter controls whether eigenvectors
     * are computed.
     */
    RWSymEigDecomp(const RWSymMat<TypeT>& A, bool computeVecs = true);

    /**
     * @copydoc RWSymEigDecomp(const RWSymMat<TypeT>&,bool)
     */
    RWSymEigDecomp(const RWSymBandMat<TypeT>& A, bool computeVecs = true);

    /**
     * Constructs a representation of the eigenvalues and eigenvectors
     * of the matrix \a A. The boolean parameter controls whether eigenvectors
     * are computed. The current contents of the decomposition are lost.
     */
    void factor(const RWSymMat<TypeT>& A, bool computeVecs = true);

    /**
     * @copydoc factor(const RWSymMat<TypeT>&,bool)
     */
    void factor(const RWSymBandMat<TypeT>& A, bool computeVecs = true);

    /**
     * Assigns this decomposition the passed value. The current contents
     * of the decomposition are lost.
     */
    RWSymEigDecomp<TypeT>& operator=(const RWSymEigDecomp<TypeT>& A);

    /**
     * Returns the number of columns in the decomposed matrix.
     */
    unsigned        cols() const {
        return n;
    }

    /**
     * Returns the \a i <SUP>th</SUP> eigenvalue.
     */
    TypeT             eigenValue(int i) const;

    /**
     * Returns a vector of all computed eigenvalues.
     */
    const RWMathVec<TypeT>    eigenValues() const {
        return lambda;
    }

    /**
     * Returns the \a i <SUP>th</SUP> eigenvector.
     */
    const RWMathVec<TypeT>    eigenVector(int i) const;

    /**
     * Returns a matrix whose columns are the eigenvectors.
     */
    const RWGenMat<TypeT> eigenVectors() const {
        return P;
    }

    /**
     * Returns \c true if all desired eigenvalues and eigenvectors are
     * successfully computed to full desired accuracy.
     */
    bool       good() const; // True if everything went perfectly

    /**
     * Returns \c true if either an eigenvalue or eigenvector that is
     * supposed to be computed fails to be computed, or some of the
     * computed quantities are not computed to full desired accuracy.
     */
    bool       inaccurate() const;

    /**
     * Returns \c true if an eigenvalue or eigenvector that is supposed
     * to be computed fails to be computed.
     */
    bool       fail() const;

    /**
     * Returns the number of eigenvalues in this object.
     */
    unsigned        numEigenValues() const {
        return (unsigned)lambda.length();
    }

    /**
     * Returns the number of eigenvectors in this object.
     */
    unsigned        numEigenVectors()const {
        return (unsigned)P.cols();
    }

    /**
     * Returns the number of rows in the decomposed matrix.
     */
    unsigned        rows() const {
        return n;
    }

    friend class RWSymEigServer<TypeT>;
    template <class T1>
    friend class RWHermEigServer;
    template <class T1>
    friend class RWHermEigDecomp;

};

#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/symeig.cc>
#endif


#endif
