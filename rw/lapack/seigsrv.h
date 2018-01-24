#ifndef __RWSEIGSRV_H__
#define __RWSEIGSRV_H__

/*
 * RWSymEigServer     - Abstract base for symmetric/Hermitian eigenservers
 * RWSymQREigServer   - QR method, this is the default server
 * RWSymPDQREigServer - QR method for positive definite matrices
 * RWSymRFQREigServer - Root free variant of QR (can't compute eigenvectors)
 * RWSymBisEigServer  - Bisection method
 *
 * $Id: //lapack/13/rw/lapack/seigsrv.h#1 $
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
 * Symmetric/Hermitian eigen-decomposition servers.  This classes
 * represent various algorithms for computing spectral
 * factorizations.  They are useful for people who demand more
 * control over the computation of eigen values/vectors than
 * is provided by the constructors in RWSymEigDecomp<TypeT>.
 *
 */

#include <rw/lapkdefs.h>
#include <rw/math/genmat.h>
#include <rw/math/mathvec.h>
#include <rw/lapack/symeig.h>
#include <rw/lapack/lapkfunc.h>

template <class TypeT>
class RWTriDiagDecomp;

/**
 * @ingroup symmetric_eigenvalue_decomposition_classes
 * @brief Abstract base class for the symmetric eigenvalue server.
 *
 * The class \link RWSymEigServer RWSymEigServer<T> \endlink is an abstract
 * base class for the symmetric eigenvalue server. For each precision,
 * the Linear Algebra Module provides several derived types:
 *
 * -  The class \link RWSymQREigServer RWSymQREigServer<T> \endlink uses
 *    the implicitly shifted QR method, switching between QR and QL
 *    variants in order to handle graded matrices effectively. This
 *    is the default server.
 * -  The class \link RWSymPDQREigServer RWSymPDQREigServer<T> \endlink
 *    uses a combination of Cholesky factorization and bidiagonal QR
 *    iteration. It applies to positive definite matrices only. The
 *    resulting eigenvalue decomposition may be significantly more
 *    accurate than those of the other servers.
 * -  The class \link RWSymRFQREigServer RWSymRFQREigServer<T> \endlink
 *    uses a version of the QR algorithm without square roots. It
 *    can only compute all the eigenvalues, and no eigenvectors.
 * -  The class \link RWSymSomeEigServer RWSymSomeEigServer<T> \endlink
 *    uses the bisection method to compute some of the eigenvalues and
 *    (optionally) eigenvectors. You can use this server to compute
 *    the first \e k largest eigenvalues, the first \e k smallest
 *    eigenvalues, or the <i>i <SUP>th</SUP></i> through
 *    <i>j <SUP>th</SUP></i> largest eigenvalues.
 * -  The class \link RWSymRangeEigServer RWSymRangeEigServer<T> \endlink
 *    uses bisection to compute all the eigenvalues and (optionally)
 *    eigenvectors in a real range.
 *
 * This base class serves two purposes:
 *
 * -  to provide a specification of the commonality between servers, and
 * -  to house implementation code common to several different servers.
 *
 * The common implementation includes a set of protected routines
 * to modify a given \link RWSymEigDecomp RWSymEigDecomp<T> \endlink
 * object.
 * This way, all server classes, including any that you write, can
 * access the internals of the decomposition object without being
 * explicitly listed as friend classes.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/seigsrv.h>
 *
 * RWSymEigServer<double> server;
 * RWSymEigDecomp<double> eig = server(A);  // A is a RWSymMat<double>
 * @endcode
 *
 * @section example Example
 *
 * See the specific server subclasses for examples.
 *
 */
template <class TypeT>
class RWSymEigServer
{
protected:
    static unsigned&  n(RWSymEigDecomp<TypeT>& x) {
        return x.n;
    }
    static RWMathVec<TypeT>&    lambda(RWSymEigDecomp<TypeT>& x) {
        return x.lambda;
    }
    static RWGenMat<TypeT>& P(RWSymEigDecomp<TypeT>& x) {
        return x.P;
    }
    static bool& computedAll(RWSymEigDecomp<TypeT>& x) {
        return x.computedAll;
    }
    static bool& accurate(RWSymEigDecomp<TypeT>& x) {
        return x.accurate;
    }
    // These functions provide access to the guts of the EigDecomp
    // object for the server.  This way we can declare more servers
    // without having to make them all friends of EigDecomp.

public:
    /**
     * Computes a symmetric eigenvalue decomposition. The default method
     * implemented in this class first constructs a tridiagonal decomposition
     * from the given matrix, next computes the eigenvalue decomposition
     * for that decomposition, and finally transforms the eigenvectors
     * as required.
     *
     * If self is an instance of \link RWSymPDQREigServer
     * RWSymPDQREigServer<T>\endlink, the matrix you pass in must be
     * positive definite.
     */
    virtual RWSymEigDecomp<TypeT> operator()(const RWSymMat<TypeT>& A);

    /**
     * Computes a symmetric eigenvalue decomposition. The default method
     * implemented in this class first constructs a tridiagonal decomposition
     * from the given matrix, next computes the eigenvalue decomposition
     * for that decomposition, and finally transforms the eigenvectors
     * as required.
     *
     * If self is an instance of \link RWSymPDQREigServer
     * RWSymPDQREigServer<T>\endlink, the matrix you pass in must be
     * positive definite.
     */
    virtual RWSymEigDecomp<TypeT> operator()(const RWSymBandMat<TypeT>& A);

    // The function to decompose a tri diagonal matrix is called decompose()
    // rather than op() to allow its redefinition in subclasses without
    // hiding the op() functions.
    /**
     * Computes the eigenvalue decomposition of the tridiagonal matrix
     * inside the tridiagonal decomposition. This pure virtual function
     * is implemented in the specific server classes. It is used to
     * implement the default operator()() functions.
     */
    virtual RWSymEigDecomp<TypeT>   decompose(const RWTriDiagDecomp<TypeT>& A) = 0;

    // This is nice to know when forming the tri-diagonal decomposition of
    // a matrix, so we can decide whether or not to keep the orthogonal matrix
    // part.
    /**
     * Returns \c true if this server is configured to compute eigenvectors
     * as well as eigenvalues.
     */
    virtual bool computeEigenVectors() const = 0;

    virtual ~RWSymEigServer() {};
};


/**
 * @ingroup symmetric_eigenvalue_decomposition_classes
 * @brief Default server for the QR method of computing eigenvalues.
 *
 * The class \link RWSymQREigServer RWSymQREigServer<T> \endlink is
 * the server for the QR method of computing eigenvalues. This is
 * the default server.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/seigsrv.h>
 *
 * RWSymQREigServer<double> server;
 * RWSymEigDecomp<double> eig = server(A); // A is a RWSymBandMat<double>
 * @endcode
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/lapack/seigsrv.h>
 *
 * int main()
 * {
 *     RWSymMat<double> A;                     // input a matrix
 *     std::cin >> A;
 *
 *     RWSymQREigServer<double> server;        // configure a server
 *     server.computeEigenVectors(false);      // turn off eigenvectors
 *     RWSymEigDecomp<double> eig = server(A);
 *
 *     return 0;
 * }
 * @endcode
 *
 */
template <class TypeT>
class RWSymQREigServer : public RWSymEigServer<TypeT>
{
private:
    bool computeVecs_;
public:

    /**
     * Constructs a server. The parameter indicates whether this server
     * should be configured to compute eigenvectors as well as eigenvalues.
     */
    RWSymQREigServer(bool computeVecs = true);

    /**
     * Computes the eigenvalue decomposition of the tridiagonal matrix
     * inside the tridiagonal decomposition.
     */
    virtual RWSymEigDecomp<TypeT>   decompose(const RWTriDiagDecomp<TypeT>& A);

    // Documented in base class.
    virtual bool computeEigenVectors() const;

    /**
     * Sets whether or not the server should compute eigenvectors as
     * well as eigenvalues.
     */
    void              computeEigenVectors(bool computeVecs);

    virtual ~RWSymQREigServer() {};
};


/**
 * @ingroup symmetric_eigenvalue_decomposition_classes
 * @brief Server for the positive definite QR method of computing
 * eigenvalues.
 *
 * The class \link RWSymPDQREigServer RWSymPDQREigServer<T> \endlink
 * is the server for the positive definite QR method of computing
 * eigenvalues. This server applies only to matrices that you know
 * are positive definite. Trying to use it on nonpositive definite
 * matrices results in a decomposition with the incomplete flag
 * set; you can test for this in the class \link RWSymEigDecomp
 * RWSymEigDecomp<T> \endlink. Use of this server with positive
 * definite matrices can result in much more accurate eigenvalues
 * and eigenvectors.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/seigsrv.h>
 *
 * RWSymPDQREigServer<double> server;
 * RWSymEigDecomp<double> eig = server(A);  // A is a RWSymBandMat<double>
 * @endcode
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/lapack/seigsrv.h>
 *
 * int main()
 * {
 *     RWSymMat<double> A;                     // input a matrix
 *     std::cin >> A;
 *
 *     RWSymPDQREigServer<double> server;      // configure a server
 *     server.computeEigenVectors(false);      // turn off eigenvectors
 *     RWSymEigDecomp<double> eig = server(A);
 *
 *     return 0;
 * }
 * @endcode
 *
 */
template <class TypeT>
class RWSymPDQREigServer : public RWSymEigServer<TypeT>
{
private:
    bool computeVecs_;
public:

    /**
     * Constructs a server. The parameter indicates whether this server
     * should be configured to compute eigenvectors as well as eigenvalues.
     */
    RWSymPDQREigServer(bool computeVecs = true);

    /**
     * Computes the eigenvalue decomposition of the tridiagonal matrix
     * inside the tridiagonal decomposition.
     */
    virtual RWSymEigDecomp<TypeT>   decompose(const RWTriDiagDecomp<TypeT>& A);

    // Documented in base class.
    virtual bool computeEigenVectors() const;

    /**
     * Sets whether or not the server should compute eigenvectors as
     * well as eigenvalues.
     */
    void              computeEigenVectors(bool computeVecs);

    virtual ~RWSymPDQREigServer() {};
};


/**
 * @ingroup symmetric_eigenvalue_decomposition_classes
 * @brief Server for the root-free QR method of computing eigenvalues.
 *
 * The class \link RWSymRFQREigServer RWSymRFQREigServer<T> \endlink
 * is the server for the root-free QR method of computing eigenvalues.
 * This method computes all the eigenvalues and no eigenvectors.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/seigsrv.h>
 *
 * RWSymRFQREigServer<double> server;
 * RWSymEigDecomp<double> eig = server(A);  // A is a RWSymBandMat<double>
 * @endcode
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/lapack/seigsrv.h>
 *
 * int main()
 * {
 *     RWSymMat<double> A;                     // input a matrix
 *     std::cin >> A;
 *
 *     RWSymRFQREigServer<double> server;      // configure a server
 *     RWSymEigDecomp<double> eig = server(A);
 *
 *     return 0;
 * }
 * @endcode
 *
 */
template <class TypeT>
class RWSymRFQREigServer : public RWSymEigServer<TypeT>
{
public:

    /**
     * Constructs a server.
     */
    RWSymRFQREigServer();

    /**
     * Computes the eigenvalue decomposition of the tridiagonal matrix
     * inside the tridiagonal decomposition.
     */
    virtual RWSymEigDecomp<TypeT>   decompose(const RWTriDiagDecomp<TypeT>& A);

    /**
     * Always returns \c false, since this server can \e not compute
     * eigenvectors.
     */
    virtual bool computeEigenVectors() const;

    virtual ~RWSymRFQREigServer() {};
};


/**
 * @ingroup symmetric_eigenvalue_decomposition_classes
 * @brief Server for symmetric eigenvalues that allows the computation
 * of a subset of the eigenvalues and (optionally) their corresponding
 * eigenvectors.
 *
 * The symmetric eigenvalue server class, \link RWSymSomeEigServer
 * RWSymSomeEigServer<T> \endlink, allows the computation of a subset of
 * the eigenvalues and (optionally) their corresponding eigenvectors. The
 * computation uses the bisection method to find the eigenvalues, followed
 * by inverse iteration to find the eigenvectors. The subset of
 * eigenvalues to be computed is specified using the RWSlice class, or one
 * of its subclasses, RWRange or RWToEnd. This provides the flexibility
 * to specify either the smallest eigenvalues, the largest, or a selection
 * in between. The eigenvalue ordering is smallest to largest.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/seigsrv.h>
 *
 * RWSymSomeEigServer<double> server;
 * RWSymEigDecomp<double> eig = server(A);  // A is a RWSymBandMat<double>
 * @endcode
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/lapack/seigsrv.h>
 *
 * int main()
 * {
 *     RWSymMat<double> A;                     // input a matrix
 *     std::cin >> A;
 *
 *     RWSymSomeEigServer<double> server;      // configure a server
 *     server.setRange(RWSlice(0,5));          // the 5 smallest eigenvalues
 *     RWSymEigDecomp<double> eig = server(A);
 *
 *     return 0;
 * }
 * @endcode
 *
 */
template <class TypeT>
class RWSymSomeEigServer  : public RWSymEigServer<TypeT>
{
private:
    bool computeVecs_;
    float       tolerance_;
    RWSlice   range;
public:

    /**
     * Constructs a server. The parameter indicates whether this server
     * should be configured to compute eigenvectors as well as eigenvalues.
     */
    RWSymSomeEigServer(bool computeVecs = true);

    /**
     * Computes the eigenvalue decomposition of the tridiagonal matrix
     * inside the tridiagonal decomposition.
     */
    virtual RWSymEigDecomp<TypeT>   decompose(const RWTriDiagDecomp<TypeT>& A);

    /**
     * Sets the tolerance to which we have to compute the eigenvalues.
     * Returns the previous tolerance.
     */
    TypeT               setTolerance(TypeT tolerance);

    /**
     * Sets the range of eigenvalues to be computed. Returns the previous
     * range.
     */
    RWSlice           setRange(const RWSlice& range);

    // Documented in base class.
    virtual bool computeEigenVectors() const;

    /**
     * Sets whether or not the server should compute eigenvectors as
     * well as eigenvalues.
     */
    void              computeEigenVectors(bool computeVecs);

    virtual ~RWSymSomeEigServer() {};
};


/**
 * @ingroup symmetric_eigenvalue_decomposition_classes
 * @brief Server for symmetric eigenvalues that allows the computation
 * of only the eigenvalues in a given range and (optionally) their
 * corresponding eigenvectors.
 *
 * The symmetric eigenvalue server class, \link RWSymRangeEigServer
 * RWSymRangeEigServer<T> \endlink, allows the computation of only the
 * eigenvalues in a given range and (optionally) their corresponding
 * eigenvectors. The computation uses the bisection method to find the
 * eigenvalues, followed by inverse iteration to find the eigenvectors.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/seigsrv.h>
 *
 * RWSymRangeEigServer<double> server;
 * RWSymEigDecomp<double> eig = server(A);  // A is a RWSymBandMat<double>
 * @endcode
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/lapack/seigsrv.h>
 *
 * int main()
 * {
 *     RWSymMat<double> A;                    // input a matrix
 *     std::cin >> A;
 *
 *     RWSymRangeEigServer<double> server;    // configure a server
 *     server.setRange(0.0, 0.5);             // Set the range from 0.0 to 0.5
 *     RWSymEigDecomp<double> eig = server(A);
 *
 *     return 0;
 * }
 * @endcode
 *
 */
template <class TypeT>
class RWSymRangeEigServer  : public RWSymEigServer<TypeT>
{
private:
    bool computeVecs_;
    TypeT       tolerance_;
    TypeT       small_;
    TypeT       large_;
public:

    /**
     * Constructs a server. The parameter indicates whether this server
     * should be configured to compute eigenvectors as well as eigenvalues.
     * The new server is configured to compute all eigenvalues.
     */
    RWSymRangeEigServer(bool computeVecs = true);

    /**
     * Computes the eigenvalue decomposition of the tridiagonal matrix
     * inside the tridiagonal decomposition.
     */
    virtual RWSymEigDecomp<TypeT>   decompose(const RWTriDiagDecomp<TypeT>& A);

    /**
     * Sets the tolerance to which you must compute the eigenvalues.
     * Returns the previous tolerance.
     */
    TypeT               setTolerance(TypeT tolerance);

    /**
     * Sets the range of eigenvalues to be computed. To set up a range
     * which is open on one side (extends to infinity), you can use
     * the setSmall() or setLarge() functions.
     */
    void              setRange(TypeT from, TypeT to);

    /**
     * Sets the lower boundary on the range of eigenvalues to be computed.
     * Returns the previous lower boundary.
     */
    TypeT               setSmall(TypeT sm);

    /**
     * Sets the upper boundary on the range of eigenvalues to be computed.
     * Returns the previous upper boundary.
     */
    TypeT               setLarge(TypeT large);

    // Documented in base class.
    virtual bool computeEigenVectors() const;

    /**
     * Sets whether or not the server should compute eigenvectors as
     * well as eigenvalues.
     */
    void              computeEigenVectors(bool computeVecs);

    virtual ~RWSymRangeEigServer() {};
};





#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/seigsrv.cc>
#endif

#endif
