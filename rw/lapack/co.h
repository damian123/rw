#ifndef __RWCO_H__
#define __RWCO_H__
/*
 * CODecomp: representation of a complete orthogonal decomposition
 *
 *
 * A complete orthogonal decomposition decomposes a matrix A like this:
 *
 *                    [ T 0 ]
 *            A P = Q [     ] Z'
 *                    [ 0 0 ]
 *
 * where P is a permutation matrix, Q and Z are orthogonal, and T is upper
 * triangular.
 *
 * Implementation details:
 * The Q and P parts of the decomposition are represented using a QRDecomp
 * object.  In order to save copying the R part, CODecomp is made a friend
 * or QRDecomp and we invalidate the R portion of the decomposition,
 * writing T and information needed to compute Z over top of it.  Note
 * that the TZ_ object refers to the same data as QRdecomp_.QR_.
 *
 * $Id: //lapack/13/rw/lapack/co.h#1 $
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

#include <rw/lapack/qr.h>
#include <rw/lapack/utrimat.h>
#include <rw/dcomplex.h>
#include <rw/math/numtrait.h>

/**
 * @ingroup decomposition_classes
 * @brief Encapsulates a complete orthogonal decomposition.
 *
 * The class \link RWCODecomp RWCODecomp<T,QRCalc> \endlink encapsulates
 * a complete orthogonal decomposition. A <i>complete orthogonal
 * decomposition</i> decomposes a general rectangular matrix <i>A</i>
 * into the form:
 *
 * \f$
 * AP = Q \begin{bmatrix}
 * T & 0\\
 * 0 & 0
 * \end{bmatrix} Z^{\prime}
 * \f$
 *
 * where <i>P</i> is a permutation, <i>Q</i> and <i>Z</i> are orthogonal
 * matrices, and <i>T</i> is an upper triangular matrix. This transformation
 * is closely related to the QR transformation. The difference is
 * that an extra orthogonal transformation, <i>Z</i>, is applied
 * to zero out the columns to the right of <i>T</i>.
 *
 * @note
 * For greater flexibility, the user can implement this method,
 * or the Linear Algebra Module provides two classes to perform
 * this function - \link RWQRCalc RWQRCalc<T>\endlink and
 * \link RWQRCalcP3 RWQRCalcP3<T>\endlink. Please see their
 * descriptions in this reference guide for more information.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/qrcalc3.h>
 * #include <rw/lapack/co.h>
 *
 * RWCODecomp<double, RWQRCalcP3<double> > QTZ(A); // A is an RWGenMat<double>
 * @endcode

 * @section example Example
 *
 * This decomposition is commonly used in the solution of least
 * squares problems. The implementation of the Linear Algebra Module
 * class \link RWLeastSqQR RWLeastSqQR<T>\endlink uses the complete orthogonal
 * transformation.
 *
 * @code
 *
 * #include <rw/lapack/qrcalc3.h>
 * #include <iostream>
 * #include <rw/lapack/co.h>
 *
 * int main()
 * {
 *     RWGenMat<double> A;
 *     std::cin >> A;
 *     RWCODecomp<double, RWQRCalcP3<double> > co(A);
 *     std::cout << "Input matrix: " << A << endl;
 *     std::cout << "Permutation: " << co.P() <<
 *                  std::endl;
 *     std::cout << "Q: " << co.Q() << endl;
 *     std::cout << "Z: " << co.Z() << endl;
 *     std::cout << "T: " << co.T() << endl;
 *     return 0;
 * }
 * @endcode
 *
 */
template <class TypeT, class QRCalc>
class RWCODecomp
{
private:
    RWQRDecomp<TypeT, QRCalc> QRDecomp_;
    RWMathVec<TypeT> Ztau_;                // scalars needed to recover Z
    RWGenMat<TypeT>  TZ_;                  // upper triangular part is T, rest is used to compute Z
    void            dofactor(double tol); // build Ztau_ and TZ_ from QRdecomp_

public:

    typedef typename rw_numeric_traits<TypeT>::norm_type norm_type;

    /**
     * Default constructor. Builds a decomposition of size 0 x 0.
     */
    RWCODecomp();

    /**
     * Copy constructor. References the data in the original decomposition
     * for efficiency.
     */
    RWCODecomp(const RWCODecomp<TypeT, QRCalc>& A);

    /**
     * Builds a complete orthogonal representation of the matrix represented
     * by the QR decomposition. Entries along the diagonal of the <i>R</i>
     * factor of the QR decomposition that are smaller in magnitude
     * than \a tol are treated as 0.
     */
    RWCODecomp(const RWQRDecomp<TypeT, QRCalc>& A, double tol = 0);

    /**
     * Builds a complete orthogonal decomposition of \a A. Entries along
     * the diagonal of <i>T</i> that would be smaller in magnitude than
     * \a tol are treated as 0.
     */
    RWCODecomp(const RWGenMat<TypeT>& A, double tol = 0);

    /**
     * Builds a complete orthogonal representation of the matrix represented
     * by the QR decomposition. Entries along the diagonal of the <i>R</i>
     * factor of the QR decomposition that are smaller in magnitude
     * than \a tol are treated  as 0. The current contents of the decomposition
     * are lost.
     */
    void factor(const RWQRDecomp<TypeT, QRCalc>& A, double tol = 0);

    /**
     * Builds a complete orthogonal decomposition of \a A. Entries along
     * the diagonal of <i>T</i> that would be smaller in magnitude than
     * \a tol are treated as 0. The current contents of the decomposition
     * are lost.
     */
    void factor(const RWGenMat<TypeT>& A, double tol = 0);

    ~RWCODecomp();
    RWCODecomp<TypeT, QRCalc>& operator=(const RWCODecomp<TypeT, QRCalc>&);

    /**
     * Returns the number of rows in the matrix that the decomposition
     * represents.
     */
    unsigned        rows() const {
        return QRDecomp_.rows();
    }

    /**
     * Returns the number of columns in the matrix that the decomposition
     * represents.
     */
    unsigned        cols() const {
        return QRDecomp_.cols();
    }

    /**
     * Computes an explicit representation of the permutation matrix.
     */
    RWGenMat<TypeT>  P() const {
        return QRDecomp_.P();
    }

    /**
     * Computes the inner product of the permutation, or its transpose,
     * and the vector \a x.
     */
    RWMathVec<TypeT> Px(const RWMathVec<TypeT>& x) const {
        return QRDecomp_.Px(x);
    }

    /**
     * @copydoc Px(const RWMathVec<TypeT>&) const
     */
    RWMathVec<TypeT> PTx(const RWMathVec<TypeT>& x) const {
        return QRDecomp_.PTx(x);
    }

    /**
     * Computes the inner product of the orthogonal matrix \c Q, or
     * its (conjugate) transpose, and the vector \a x.
     */
    RWMathVec<TypeT> Qx(const RWMathVec<TypeT>& x) const {
        return QRDecomp_.Qx(x);
    }

    /**
     * @copydoc Qx(const RWMathVec<TypeT>&) const
     */
    RWMathVec<TypeT> QTx(const RWMathVec<TypeT>& x) const {
        return QRDecomp_.QTx(x);
    }

    /**
     * Computes an explicit representation of the orthogonal matrix
     * \c Q.
     */
    RWGenMat<TypeT>  Q() const {
        return QRDecomp_.Q();
    }

    /**
     * Returns the rank of the matrix that the decomposition represents.
     * The rank is also the number of rows and columns in the factor
     * <i>T</i>.
     */
    unsigned       rank() const {
        return unsigned(TZ_.rows());
    }

    /**
     * Returns an explicit representation of the triangular matrix \c
     * T.
     */
    RWUpperTriMat<TypeT> T() const;

    /**
     * Computes an explicit representation of the orthogonal matrix
     * \c Z.
     */
    RWGenMat<TypeT>      Z() const;

    /**
     * Computes the inner product of the matrix \c T, its (conjugate)
     * transpose, its inverse, or its (conjugate) transpose inverse,
     * and the vector \a x.
     */
    RWMathVec<TypeT>         Tx(const RWMathVec<TypeT>& x) const;

    /**
     * @copydoc Tx(const RWMathVec<TypeT>&) const
     */
    RWMathVec<TypeT>         TTx(const RWMathVec<TypeT>& x) const;

    /**
     * @copydoc Tx(const RWMathVec<TypeT>&) const
     */
    RWMathVec<TypeT>         Tinvx(const RWMathVec<TypeT>& x) const;

    /**
     * @copydoc Tx(const RWMathVec<TypeT>&) const
     */
    RWMathVec<TypeT>         TTinvx(const RWMathVec<TypeT>& x) const;

    /**
     * Computes the inner product of the orthogonal matrix \c Z, or
     * its (conjugate) transpose, and the vector \a x.
     */
    RWMathVec<TypeT>         Zx(const RWMathVec<TypeT>& x) const;

    /**
     * @copydoc Zx(const RWMathVec<TypeT>&) const
     */
    RWMathVec<TypeT>         ZTx(const RWMathVec<TypeT>& x) const;
};


#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/co.cc>
#endif

#endif
