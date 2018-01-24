#ifndef __RWQR_H__
#define __RWQR_H__
/*
 * RWQRDecomp:       representation of a QR decomposition
 * RWQRDecompServer: produce QR decompositions, using possibly non-default options
 *
 * $Id: //lapack/13/rw/lapack/qr.h#1 $
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
 * A QR decomposition decomposes a matrix A like this:
 *
 *                    [ R ]
 *            A P = Q [   ]
 *                    [ 0 ]
 *
 * where P is a permutation matrix, Q is orthogonal, and R is upper
 * triangular if A is taller than wide, and upper trapezoidal is  A
 * is wider than tall.
 */

#include <rw/lapkdefs.h>
#include <rw/math/genmat.h>
#include <rw/math/mathvec.h>
#include <rw/dcomplex.h>

template <class TypeT, class QRCalc>
class  RWQRDecompServer;
template <class TypeT, class QRCalc>
class  RWCODecomp;

/**
 * @ingroup decomposition_classes
 * @brief Used to construct and work with QR decompositions.
 *
 * A QR decomposition is a representation of a matrix <i>A</i> of
 * the form:
 *
 * \f[
 * AP = QR
 * \f]
 *
 * where \c P is a permutation matrix, \c Q is orthogonal,
 * and \c R is upper trapezoidal, or upper triangular if \c A
 * is full rank, with no more columns than rows.
 *
 * The class \link RWQRDecomp RWQRDecomp<T,QRCalc> \endlink is used
 * to construct and work with QR decompositions. Sometimes it is
 * desirable to zero out the rectangular part of the trapezoidal
 * matrix \c R; this can be done using the orthogonal decomposition
 * class,\link RWQRDecomp RWQRDecomp<T,QRCalc>\endlink. A common
 * use of QR decompositions is to solve linear least squares problems;
 * this is most conveniently done using the \link RWLeastSqQR
 * RWLeastSqQR<T,QRCalc> \endlink class.
 *
 * You may need more control over the computation of the decomposition
 * than is provided by this class. For example, if you don't want
 * to use pivoting, you can use the QR decomposition server class,
 * \link RWQRDecompServer RWQRDecompServer<T,QRCalc>\endlink, to
 * do the construction.
 *
 * @note
 * For greater flexibility, the user can implement this method,
 * or the Linear Algebra Module provides two classes to perform
 * this function - \link RWQRCalc RWQRCalc<T> \endlink and
 * \link RWQRCalcP3 RWQRCalcP3<T> \endlink. Please see their
 * descriptions in this reference guide for more information.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/qr.h>
 * #include <rw/lapack/qrcalcp3.h>
 *
 * RWQRDecomp<double, RWQRCalcP3<double> > qr(A);  // A is a RWGenMat<double>
 * @endcode

 * @section example Example
 *
 * @code
 * #include <rw/iostream>
 * #include <rw/math/genmat.h>
 * #include <rw/lapack/qr.h>
 * #include <rw/lapack/qrcalcp3.h>
 *
 * int main()
 * {
 *     RWGenMat<double> A;
 *     std::cin >> A;
 *     RWQRDecomp<double, RWQRCalc3P<double> > qr(A);
 *     std::cout << "Input matrix: " << A << std::endl;
 *     std::cout << "Permutation: " << qr.P() <<
 *                  std::endl;
 *     std::cout << "Q: " << qr.Q() << std::endl;
 *     std::cout << "R: " << qr.R() << std::endl;
 *     return 0;
 * }
 * @endcode
 *
 */
template <class TypeT, class QRCalc>
class RWQRDecomp : public QRCalc
{
protected:
    // Private Instance Variables -----------------------------------------------
    RWMathVec<RWPivotType>  pivots_;  // length() = 0 if no pivoting was done
    RWMathVec<TypeT>  tau_;     // scalars needed to recover Q
    RWGenMat<TypeT>   QR_;      // upper triangular part is R, rest is used to compute Q

    // Private Methods ----------------------------------------------------------
    //these will exists for DComplex only:
    //RWMathVec<TypeT>     RTx_specialized( const RWMathVec<TypeT>& x ) const;

public:
    // Public typedefs ----------------------------------------------------------
    typedef typename rw_numeric_traits<TypeT>::norm_type norm_type;

    // Public Constructors ------------------------------------------------------
    // Default ctor

    /**
     * Default constructor. Builds a decomposition of size 0 x 0.
     */
    RWQRDecomp();

    // Copy ctor

    /**
     * Copy constructor. References the data in the original decomposition
     * for efficiency.
     */
    RWQRDecomp(const RWQRDecomp<TypeT, QRCalc>&);

    // Construct a QR decomp for the given matrix

    /**
     * Builds a QR decomposition of \a A. By default, pivoting is done
     * so that the entries along the diagonal of \c R are non-increasing.
     * To construct a QR decomposition with non-default options, use
     * the QR decomposition server class \link RWQRDecompServer
     * RWQRDecompServer<T,QRCalc> \endlink.
     */
    RWQRDecomp(const RWGenMat<TypeT>& A);

    // Destructor ---------------------------------------------------------------
    ~RWQRDecomp();

    // Public Methods -----------------------------------------------------------

    /**
     * Builds a QR decomposition of \a A.
     */
    void factor(const RWGenMat<TypeT>& A);


    /**
     * Returns the number of rows in the matrix that the decomposition
     * represents.
     */
    unsigned rows() const {
        return (unsigned)QR_.rows();
    }

    /**
     * Returns the number of columns in the matrix that the decomposition
     * represents.
     */
    unsigned cols() const {
        return (unsigned)QR_.cols();
    }

    /**
     * Computes an explicit representation of the permutation matrix.
     */
    RWGenMat<TypeT> P() const;           // Extract components of the decomposition

    /**
     * Returns an explicit representation of the upper trapezoidal matrix
     * \c R.
     */
    RWGenMat<TypeT> R() const;

    /**
    * Returns the main diagonal of the upper trapezoidal matrix \c
     * R.
     */
    RWMathVec<TypeT> Rdiagonal() const;

    /**
     * Computes an explicit representation of the orthogonal matrix
     * \c Q.
     */
    RWGenMat<TypeT> Q() const;

    /**
     * Computes the inner product of the permutation matrix \c P and
     * the vector \a x.
     */
    RWMathVec<TypeT> Px(const RWMathVec<TypeT>& x) const;    // Compute using the decomposition

    /**
     * Computes the inner product of the transpose of the permutation
     * matrix \c P and the vector \a x.
     */
    RWMathVec<TypeT> PTx(const RWMathVec<TypeT>& x) const;

    /**
     * Computes the inner product of the matrix \c R and the vector
     * \a x.
     */
    RWMathVec<TypeT> Rx(const RWMathVec<TypeT>& x) const;

    /**
     * Computes the inner product of the (conjugate) transpose of the
     * matrix \c R and the vector \a x.
     */
    RWMathVec<TypeT> RTx(const RWMathVec<TypeT>& x) const;

    /**
     * Computes the inner product of the inverse of matrix \c R and the
     * vector \a x. An exception is raised if you call this for a
     * singular decomposition.
     */
    RWMathVec<TypeT> Rinvx(const RWMathVec<TypeT>& x) const;

    /**
     * Computes the inner product of the (conjugate) transpose inverse of
     * matrix \c R and the vector \a x. An exception is raised if you call
     * this for a singular decomposition.
     */
    RWMathVec<TypeT> RTinvx(const RWMathVec<TypeT>& x) const;

    /**
     * Computes the inner product of the orthogonal matrix \c Q and the
     * vector \a x.
     */
    RWMathVec<TypeT> Qx(const RWMathVec<TypeT>& x) const;

    /**
     * Computes the inner product of the (conjugate) transpose of orthogonal
     * matrix \c Q and the vector \a x.
     */
    RWMathVec<TypeT> QTx(const RWMathVec<TypeT>& x) const;

    // Public Operators ---------------------------------------------------------
    // assignment operator

    /**
     * Assigns the passed value to this decomposition. The current contents
     * of the decomposition are lost.
     */
    RWQRDecomp<TypeT, QRCalc>& operator=(const RWQRDecomp<TypeT, QRCalc>&);

    // Friend Classes -----------------------------------------------------------
    friend class RWQRDecompServer<TypeT, QRCalc>;
    friend class RWCODecomp<TypeT, QRCalc>;
};

/**
 * @ingroup decomposition_classes
 * @brief Used to construct instances of the QR decomposition class,
 * \link RWQRDecomp RWQRDecomp<T,QRCalc> \endlink.
 *
 * The QR decomposition server class, \link RWQRDecompServer
 * RWQRDecompServer<T,QRCalc>\endlink, is used to construct instances
 * of the QR decomposition class, \link RWQRDecomp
 * RWQRDecomp<T,QRCalc>\endlink. Using a server rather than the
 * standard QR decomposition constructor gives you control over
 * how pivoting is done.
 *
 * Normally, columns in the input matrix may be moved in order to
 * increase the robustness of the computation of the QR decomposition.
 * This is called <i>pivoting</i>. In some cases, you may want to
 * ensure that certain columns of the input matrix are kept at the
 * front of the QR decomposition, or you may want to disable pivoting
 * altogether. Using a server object gives you this control.
 *
 * @note
 * For greater flexibility, the user can implement this method,
 * or the Linear Algebra Module provides two classes to perform
 * this function - \link RWQRCalc RWQRCalc<T> \endlink
 * and \link RWQRCalcP3 RWQRCalcP3<T> \endlink. Please see their
 * descriptions in this reference guide for more information.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/qr.h>
 * #include <rw/lapack/qrcalcp3.h>
 *
 * RWQRDecompServer<double, RWQRCalcP3<double> > server;
 * @endcode

 * @section example Example
 *
 * @code
 * // Read a matrix from standard input and print its
 * //  QR decomposition.
 * // Use a QR decomposition server to prevent pivoting.
 *
 * #include <iostream>
 * #include <rw/math/genmat.h>
 * #include <rw/lapack/qr.h>
 * #include <rw/lapack/qrcalcp3.h>
 *
 * int main()
 * {
 *     RWGenMat<double> A;
 *     std::cin >> A;
 *     RWQRDecompServer<double, RWQRCalcP3<double> >
 *                              server;
 *     server.setPivoting(false);
 *     RWQRDecomp<double, RWQRCalcP3<double> > QR =
 *                        server(A);
 *     std::cout << "Q=" << QR.Q() << std::endl;
 *     std::cout << "R=" << QR.R() << std::endl;
 *     return 0;
 * }
 * @endcode
 *
 */
template <class TypeT, class QRCalc>
class RWQRDecompServer : public QRCalc
{
private:
    bool pivot_;       // Do we pivot?
    RWMathVec<int>    initial_;     // Indices of columns to be moved to initial position

public:
    typedef typename rw_numeric_traits<TypeT>::norm_type norm_type;

    /**
     * Default constructor. Builds a new server, which does pivoting
     * by default.
     */
    RWQRDecompServer();

    /**
     *  Controls whether pivoting is done.
     */
    void setPivoting(bool);   // turn pivoting option on or off

    /**
     * Moves \a i to an initial position in the decomposition.
     */
    void setInitialIndex(int i);   // i moves to the begin of the decomposition

    /**
     * Makes \a i a free index, so that it may be pivoted to any column
     * of the decomposition. Initially, all indices are free.
     */
    void setFreeIndex(int i);      // i is removed from initial and final lists

    /**
     * Computes a QR decomposition.
     */
    RWQRDecomp<TypeT, QRCalc> operator()(const RWGenMat<TypeT>&) const;  // build a QR decomposition
};


#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/qr.cc>
#endif

#endif
