#ifndef __RWEIGSRV_H__
#define __RWEIGSRV_H__

/*
 * RWEigServer     - Abstract base for eigenservers
 * RWSchurEigServer- Schur decomposition method, this is the default server
 * RWHessEigServer - Don't generate Schur decomp - get eigenvectors by inverse iteration
 *
 *
 * $Id: //lapack/13/rw/lapack/eigsrv.h#1 $
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

#include <rw/lapkdefs.h>
#include <rw/math/genmat.h>
#include <rw/math/mathvec.h>
#include <rw/lapack/eig.h>
#include <rw/dcomplex.h>

template <class TypeT>
class RWBalanceDecomp;
template <class TypeT>
class RWHessenbergDecomp;
template <class TypeT>
class RWSchurDecomp;

/**
 * @ingroup nonsymmetric_eigenvalue_decomposition_classes
 * @brief Abstract base class for the nonsymmetric eigenvalue servers.
 *
 * The class \link RWEigServer RWEigServer<T> \endlink is an abstract
 * base class for the nonsymmetric eigenvalue servers. For each
 * precision, the Linear Algebra Module supplies two derived types:
 *
 * -  The class \link RWSchurEigServer RWSchurEigServer<T> \endlink
 *    uses the Schur decomposition method to compute the eigenvalue
 *    decomposition.
 * -  The class \link RWHessEigServer RWHessEigServer<T> \endlink computes
 *    the eigenvalues directly from the Hessenberg decomposition, then
 *    uses inverse iteration to find the desired eigenvectors. This
 *    server is most appropriate when only a few of the eigenvectors
 *    are required, about 20% or less.
 *
 * The reason for having a base class for an eigenvalue server is
 * to allow access to the eigenvalue decomposition objects, \link
 * RWEigDecomp RWEigDecomp<T>\endlink, by the different servers.
 * By making the base class a friend to the decomposition object,
 * then providing protected member functions to access the internals
 * of the \link RWEigDecomp RWEigDecomp<T> \endlink object, we effectively
 * make all subclasses of \link RWEigServer RWEigServer<T> \endlink
 * friends to the decomposition. This way, servers can be added
 * in the future without changing the decomposition object code.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/eigsrv.h>
 *
 * RWEigDecomp<double> eig = server(A);  // server is a
 *                                       // RWEigServer<double>
 * @endcode
 */
template <class TypeT>
class RWEigServer
{
protected:
#if !(defined(_MSC_VER))
    static unsigned&  n(RWEigDecomp<TypeT>& x) {
        return x.n;
    }
#endif
    template <class OtherType>
    static unsigned&  n(RWEigDecomp<OtherType>& x) {
        return x.n;
    }

#if !(defined(_MSC_VER))
    static RWMathVec<DComplex>&    lambda(RWEigDecomp<TypeT>& x) {
        return x.lambda;
    }
#endif
    template <class OtherType>
    static RWMathVec<DComplex>&    lambda(RWEigDecomp<OtherType>& x) {
        return x.lambda;
    }

#if !(defined(_MSC_VER))
    static RWGenMat<TypeT>& P(RWEigDecomp<TypeT>& x) {
        return x.P;
    }
#endif
    template <class OtherType>
    static RWGenMat<TypeT>& P(RWEigDecomp<OtherType>& x) {
        return x.P;
    }

#if !(defined(_MSC_VER))
    static RWGenMat<TypeT>& Q(RWEigDecomp<TypeT>& x) {
        return x.Q;
    }
#endif
    template <class OtherType>
    static RWGenMat<TypeT>& Q(RWEigDecomp<OtherType>& x) {
        return x.Q;
    }

#if !(defined(_MSC_VER))
    static bool& computedAll(RWEigDecomp<TypeT>& x) {
        return x.computedAll;
    }
#endif
    template <class OtherType>
    static bool& computedAll(RWEigDecomp<OtherType>& x) {
        return x.computedAll;
    }

#if !(defined(_MSC_VER))
    static bool& accurate(RWEigDecomp<TypeT>& x) {
        return x.accurate;
    }
#endif
    template <class OtherType>
    static bool& accurate(RWEigDecomp<OtherType>& x) {
        return x.accurate;
    }
    /*
    static unsigned&  n(RWEigDecomp<TypeT>& x) {return x.n;}
    static RWMathVec<TypeT>&    lambda(RWEigDecomp<TypeT>& x) {return x.lambda;}
    static RWGenMat<TypeT>& P(RWEigDecomp<TypeT>& x) {return x.P;}
    static RWGenMat<TypeT>& Q(RWEigDecomp<TypeT>& x) {return x.Q;}
    static bool& computedAll(RWEigDecomp<TypeT>& x) {return x.computedAll;}
    static bool& accurate(RWEigDecomp<TypeT>& x) {return x.accurate;}
    */
    // These functions provide access to the guts of the EigDecomp
    // object for the server.  This way we can declare more servers
    // without having to make them all friends of EigDecomp.

public:
    // Compute a decomposition.  The subclasses have operator()
    // functions for intermediate forms, like Hessenberg and Schur
    // decompositions.
    /**
     * Creates a decomposition using the server.
     */
    virtual RWEigDecomp<TypeT> operator()(const RWGenMat<TypeT>&) = 0;

    virtual ~RWEigServer() {};
};

/**
 * @ingroup nonsymmetric_eigenvalue_decomposition_classes
 * @brief Encapsulates Schur decomposition eigenvalue servers
 * used to construct eigenvalue decomposition objects of type
 * \link RWEigDecomp RWEigDecomp<T> \endlink from Schur decompositions.
 *
 * The class \link RWSchurEigServer RWSchurEigServer<T> \endlink
 * encapsulates Schur decomposition eigenvalue servers. These servers
 * are used to construct eigenvalue decomposition objects of type
 * \link RWEigDecomp RWEigDecomp<T> \endlink from Schur decompositions.
 * A server can be configured to control which eigenvectors are
 * computed, and other details of the computation.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/eigsrv.h>
 *
 * RWSchurEigServer<double> server;
 * RWSchurEigDecomp<double> deig = server(A);   // A is an
 *                                       // RWGenMat<double>
 * @endcode

 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/lapack/eigsrv.h>
 *
 * int main()
 * {
 *     RWGenMat<double> A;            // input a matrix
 *     std::cin >> A;
 *
 *     RWSchurEigServer<double> server;   // configure a server
 *     server.balance(false);         // turn off balancing option
 *     server.selectEigenVectors(RWMathVec<int>("[1 2 3 8 9]"));
 *
 *     RWSchurEigDecomp<double> deig = server(A);
 * }
 * @endcode
 *
 */
template <class TypeT>
class RWSchurEigServer : public RWEigServer<TypeT>
{
private:
    bool computeLeftVecs_;
    bool computeRightVecs_;
    bool scale_;
    bool permute_;
    RWMathVec<int>    selectVec_;       // Which eigenvectors to compute
    RWSlice   selectRange_;     // (either Vec==nil, or Range==RWAll, or both)

    //this will exists for DComplex only:
    RWEigDecomp<TypeT> op_specialized(const RWSchurDecomp<TypeT>& A);

public:

    /**
     * Constructs a server. You can adjust some basic options at construction
     * time, or adjust them later using member functions.
     */
    RWSchurEigServer(bool computeLeftVecs = true, bool computeRightVecs = true, bool scale = true, bool permute = true);

    /**
     * Computes an eigenvalue decomposition.
     */
    virtual RWEigDecomp<TypeT> operator()(const RWGenMat<TypeT>& A);

    /**
     * Computes an eigenvalue decomposition.
     */
    virtual RWEigDecomp<TypeT> operator()(const RWBalanceDecomp<TypeT>& A);

    /**
     * Computes an eigenvalue decomposition.
     */
    virtual RWEigDecomp<TypeT> operator()(const RWHessenbergDecomp<TypeT>& A);

    /**
     * Computes an eigenvalue decomposition.
     */
    virtual RWEigDecomp<TypeT> operator()(const RWSchurDecomp<TypeT>& A);

    /**
     * Returns whether or not this server is configured to compute the
     * left eigenvectors.
     */
    bool computeLeftEigenVectors() const;

    /**
     * Configures whether or not left eigenvectors are to be computed.
     */
    void      computeLeftEigenVectors(bool);

    /**
     * Returns whether or not this server is configured to compute the
     * right eigenvectors.
     */
    bool computeRightEigenVectors() const;

    /**
     * Configures whether or not right eigenvectors are to be computed.
     */
    void      computeRightEigenVectors(bool);

    /**
     * Returns \c true if this server is configured to compute all the
     * eigenvectors.
     */
    bool computeAllEigenVectors() const;

    /**
     * Returns \c true if the server is configured to do a scaling transformation
     * before computing the eigenvalues. Sometimes this transformation
     * can result in more accurate eigenvalues.
     */
    bool scale() const;

    /**
     * Configures whether or not to do a scaling transformation before
     * computing the eigenvalues. Sometimes this transformation can
     * result in more accurate eigenvalues.
     */
    void      scale(bool);

    /**
     * Selects which eigenvectors are to be computed. This call causes
     * the server to forget which eigenvectors were previously selected.
     * With a real (as opposed to complex) eigenserver object, selecting
     * either of a complex conjugate pair of eigenvalues causes both
     * to be selected.
     */
    void      selectEigenVectors(const RWMathVec<int>&);     // Compute only selected eigenvectors

    /**
     * @copydoc selectEigenVectors(const RWMathVec<int>&)
     */
    void      selectEigenVectors(const RWSlice&);

    /**
     * Returns \c true if the server is configured to attempt a permutation
     * before computing the eigenvalues. Sometimes this can result in
     * less computation.
     */
    bool permute() const;

    /**
     * Configures whether or not to do a permutation transformation
     * before computing the eigenvalues. Sometimes this can result in
     * less computation.
     */
    void      permute(bool);

    /**
     * Controls whether to do permutation and scaling transformations
     * (balancing) before computing the eigenvalue decomposition.
     */
    void      balance(bool);   // sets both scale and permute

    virtual ~RWSchurEigServer() {};
};

/**
 * @ingroup nonsymmetric_eigenvalue_decomposition_classes
 * @brief Encapsulates Hessenberg decomposition eigenvalue servers
 * used to construct eigenvalue decomposition objects of type \link
 * RWEigDecomp RWEigDecomp<T> \endlink from Hessenberg decompositions.
 *
 * The class \link RWHessEigServer RWHessEigServer<T> \endlink encapsulates
 * Hessenberg decomposition eigenvalue servers. These servers are
 * used to construct eigenvalue decomposition objects of type \link
 * RWEigDecomp RWEigDecomp<T> \endlink from Hessenberg decompositions.
 * The server can be configured to control which eigenvectors are
 * computed, and other details of the computation. The eigenvectors
 * are computed using inverse iteration. This is a good method to
 * use when you need only a small number of the eigenvectors, perhaps
 * less than 20%.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/eigsrv.h>
 *
 * RWHessEigServer<double> server;
 * RWHessEigDecomp<double> deig = server(A); // A is an
 *                                   // RWGenMat<double>
 * @endcode
 *
 * @section example Example
 *
 * @code
 * #include <rw/lapack/eigsrv.h>
 * #include <iostream>
 *
 * int main()
 * {
 *     RWGenMat<double> A; // input a matrix
 *     std::cin >> A;
 *
 *     RWHessEigServer<double> server; // configure a server
 *     server.balance(false); // turn off balancing option
 *     server.selectEigenVectors(RWMathVec<int>("[1 2 3 8 9]"));
 *
 *     RWHessEigDecomp<double> deig = server(A);
 *     return 0;
 * }
 * @endcode
 *
 */
template <class TypeT>
class RWHessEigServer : public RWEigServer<TypeT>
{
private:
    bool computeLeftVecs_;
    bool computeRightVecs_;
    bool scale_;
    bool permute_;
    RWMathVec<int>    selectVec_;       // Which eigenvectors to compute
    RWSlice   selectRange_;     // (either Vec==nil, or Range==RWAll, or both)]
    RWEigDecomp<TypeT> setEigenVals(const RWHessenbergDecomp<TypeT>& H, RWMathVec<double>* wr = 0, RWMathVec<double>* wi = 0);   // Compute eigenvalues

    //these will exists for DComplex only:
    RWEigDecomp<TypeT> setEigenVals_specialized(const RWHessenbergDecomp<TypeT>& H, RWMathVec<double>* = 0, RWMathVec<double>* = 0);   // Compute eigenvalues
    RWEigDecomp<TypeT> op_specialized(const RWHessenbergDecomp<TypeT>& A);
    //RWGenMat<DComplex> blahblah();

public:
    /**
     * Constructs a server. You can adjust some basic options at construction
     * time, or adjust them later using member functions.
     */
    RWHessEigServer(bool computeLeftVecs = true, bool computeRightVecs = true, bool scale = true, bool permute = true);

    /**
     * Computes an eigenvalue decomposition.
     */
    virtual RWEigDecomp<TypeT> operator()(const RWGenMat<TypeT>& A);

    /**
     * Computes an eigenvalue decomposition.
     */
    virtual RWEigDecomp<TypeT> operator()(const RWBalanceDecomp<TypeT>& A);

    /**
     * Computes an eigenvalue decomposition.
     */
    virtual RWEigDecomp<TypeT> operator()(const RWHessenbergDecomp<TypeT>& A);

    /**
     * Returns whether or not this server is configured to compute the
     * left eigenvectors.
     */
    bool computeLeftEigenVectors() const;

    /**
     * Configures whether or not left eigenvectors are to be computed.
     */
    void      computeLeftEigenVectors(bool);

    /**
     * Returns whether or not this server is configured to compute the
     * right eigenvectors.
     */
    bool computeRightEigenVectors() const;

    /**
     * Configures whether or not right eigenvectors are to be computed.
     */
    void      computeRightEigenVectors(bool);

    /**
     * Returns \c true if this server is configured to compute all the
     * eigenvectors.
     */
    bool computeAllEigenVectors() const;

    /**
     * Returns \c true if the server is configured to do a scaling transformation
     * before computing the eigenvalues. Sometimes this transformation
     * can result in more accurate eigenvalues.
     */
    bool scale() const;

    /**
     * Configures whether or not to do a scaling transformation before
     * computing the eigenvalues. Sometimes this transformation can
     * result in more accurate eigenvalues.
     */
    void      scale(bool);

    /**
     * Selects which eigenvectors are to be computed. This call causes
     * the server to forget which eigenvectors were previously selected.
     * With a real as opposed to complex eigenserver object, selecting
     * either of a complex conjugate pair of eigenvalues causes both
     * to be selected.
     */
    void      selectEigenVectors(const RWMathVec<int>&);

    /**
     * @copydoc selectEigenVectors()
     */
    void      selectEigenVectors(const RWSlice&);

    /**
     * Returns \c true if the server is configured to attempt a permutation
     * before computing the eigenvalues. Sometimes this can result in
     * less computation.
     */
    bool permute() const;

    /**
     * Configures whether or not to do a permutation transformation
     * before computing the eigenvalues. Sometimes this can result in
     * less computation.
     */
    void      permute(bool);

    /**
     * Controls whether to do permutation and scaling transformations
     * (balancing) before computing the eigenvalue decomposition.
     */
    void      balance(bool);   // sets both scale and permute

    virtual ~RWHessEigServer() {};
};

#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/eigsrv.cc>
#endif

#endif
