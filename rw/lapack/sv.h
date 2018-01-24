#ifndef __RWSV_H__
#define __RWSV_H__
/*
 * RWSVDecomp:   Singular value decomposition
 * RWSVServer:   SVD server using bidiagonal reduction
 * RWSVQRServer: SVD server using QR and then bidiagonal reduction
 *
 * $Id: //lapack/13/rw/lapack/sv.h#1 $
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
 * This class represents the singular value decomposition of an
 * mxn matrix A:
 *
 *                          [ s1                 ]
 *                          [    .               ]
 *            [           ] [      .             ] [   v1'   ]
 *            [           ] [        .           ] [    .    ]
 *        A = [ u1 ... um ] [          sr        ] [    .    ]
 *            [           ] [             0      ] [    .    ]
 *            [           ] [               .    ] [   vn'   ]
 *                          [                 .  ]
 *                          [                   .]
 *
 * where s1,...,sr are real positive numbers called the singular values
 * of A; u1,...,um are the left singular vectors of A; and v1,...,vn are
 * the right singular vectors of A.  The left singular vectors for an
 * orthonormal basis for Rm, the right singular vectors are an orthonormal
 * basis for Rn.  The singular values are ordered so s1>=s2>=...>=sr.
 * Often, only some of the singular vectors are needed; the default
 * server computes only the first r.  The number of singular values, r,
 * is the rank of A.  Often we truncate the singular value decomposition
 * and ignore any singular values less than some threshold, then r becomes
 * the numerical rank of A.  Doing this can yield more meaningful
 * computational results, since otherwise the insignificant singular
 * values can dominate the computation.
 */

#include <rw/dcomplex.h>
#include <rw/lapkdefs.h>
#include <rw/math/genmat.h>
#include <rw/math/mathvec.h>
#include <rw/math/numtrait.h>
//#include <rw/lapack/latraits.h>

template <class TypeT, class SVDCalc>
class RWSVServer;

/**
 * @ingroup decomposition_classes
 * @brief Used to construct and work with singular value decompositions.
 *
 * A singular value decomposition is a representation of a matrix
 * <i>A</i> of the form:
 *
 * \f$A = UEV'\f$
 *
 * where <i>U</i> and <i>V</i> are orthogonal, and <i>E</i> is diagonal.
 * The entries along the diagonal of <i>E</i> are the singular values;
 * the columns of <i>U</i> are the left singular vectors, and the
 * columns of <i>V</i> are the right singular vectors.
 *
 * The class \link RWSVDecomp RWSVDecomp<T> \endlink is used to
 * construct and work with singular value decompositions. The singular
 * values are always ordered smallest to largest with this class.
 * You may need more control over the computation of the decomposition
 * than is provided by this class. For example, if you don't need
 * all the singular vectors, you can use the SV decomposition server
 * class, \link RWSVServer RWSVServer<T> \endlink, to
 * do the construction.
 *
 * The template parameter \c \<SVDCalc\> determines the algorithm used
 * by the \link RWSVDecomp RWSVDecomp<T> \endlink class to compute
 * the singular value decomposition and must implement the following
 * method:
 *
 * @note
 * For greater flexibility, the user can implement this method,
 * or the Linear Algebra Module provides two classes to perform
 * this function - \link RWSVDCalc RWSVDCalc<T> \endlink and
 * \link RWSVDDivConqCalc RWSVDDivConqCalc<T>\endlink. Please
 * see their descriptions for more information.
 *
 * @code
 * bool computeSVD(const RWGenMat<T>&   A,
 *                       RWGenMat<T>&   U,
 *                       RWGenMat<T>&  VT,
 *                       RWMathVec<norm_type>&  sigma,
 *                       norm_type  tolerance,
 *                   int numLeftVectors = -1,
 *                   int numRightVectors = -1);
 * @endcode
 *
 * where #norm_type is a typedef for rw_numeric_traits<T>::norm_type.
 *
 * Parameters:
 *
 * <b>A</b> - The input matrix for which the singular value decomposition
 * is being computed.
 *
 * <b>U</b> - The output matrix of left singular victors (the columns
 * of U are the left singular vectors).
 *
 * <b>VT</b> - The output matrix of right singular victors (the
 * rows of VT are the right singular victors).
 *
 * <b>sigma</b> - The output vector of singular values in descending
 * order.
 *
 * <b>tolerance</b> - The input singular values with magnitude less
 * than tolerance will be set to zero.
 *
 * <b>numLeftVectors</b> - The input number of left vectors to compute.
 * If the number is less than zero, the default number of vectors
 * will be computed (it is up to the developer to determine what
 * the default is).
 *
 * <b>numRightVectors</b> - The input number of right vectors to
 * compute. If the number is less than zero, the default number
 * of vectors will be computed (it is up to the developer to determine
 * what the default is).
 *
 * The return value is \c true if the decomposition was successfully
 * computed.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/sv.h>
 * #include <rw/lapack/svddccalc.h>
 *
 * RWGenMat<double> A;
 * .
 * .
 * .
 * RWSVDecomp<double,RWSVDDivConqCalc<double> >sv(A);
 * @endcode
 *
 * @section example Example
 *
 * @code
 * #include <rw/iostream>
 * #include <rw/lapack/sv.h>
 * #include <rw/lapack/svddccalc.h>
 *
 * int main()
 * {
 *     RWGenMat<double> A;
 *     std::cin >> A;
 *     RWSVDecomp<double, RWSVDDivConqCalc<double> >
 *                        sv(A);
 *     std::cout << "Input matrix: " << A << std::endl;
 *     std::cout << "singular values: " <<
 *                  sv.singularValues() << std::endl;
 *     std::cout << "left vectors: " << sv.leftVectors()
 *               << std::endl;
 *     std::cout << "right vectors: " << sv.rightVectors()
 *               << std::endl;
 *     return 0;
 * }
 * @endcode
 */
template <class TypeT, class SVDCalc>
class RWSVDecomp : public SVDCalc
{
public:
    /**
     * Typedef for the usual return type of numerical <tt>norm</tt>-like
     * functions. For more information, see rw_numeric_traits<T>::norm_type.
     */
    typedef typename rw_numeric_traits<TypeT>::norm_type norm_type;
private:
    RWMathVec<norm_type> sigma_;  // the singular values
    RWGenMat<TypeT>     U_;           // columns are the computed left singular values
    RWGenMat<TypeT>     VT_;          // rows are the computed right singular values
    bool       computedAll_; // Did I compute everything the server tried to compute?

public:
    /**
     * Default constructor. Builds a decomposition of size 0 x 0.
     */
    RWSVDecomp();

    /**
     * Copy constructor. References the data in the original decomposition
     * for efficiency.
     */
    RWSVDecomp(const RWSVDecomp<TypeT, SVDCalc>& A);

    /**
     * Builds a singular value decomposition of \a A. The parameter
     * \a tol specifies the accuracy to which the singular values are
     * required. By default, they are computed to within machine precision.
     * To construct a singular value decomposition with non-default
     * options, you can use the singular value decomposition server
     * classes \link RWSVServer RWSVServer<T> \endlink.
     */
    RWSVDecomp(const RWGenMat<TypeT>& A, norm_type tol = 0);

    /**
     * Builds a singular value decomposition of \a A. The parameter
     * \a tol specifies the accuracy to which the singular values are
     * required. By default, they are computed to within machine precision.
     * To construct a singular value decomposition with non-default
     * options, you can use the singular value decomposition server
     * class \link RWSVServer RWSVServer<T> \endlink.
     */
    void factor(const RWGenMat<TypeT>& A, norm_type tol = 0);

    /**
     * Assigns the passed value to this decomposition. The current
     * contents of the decomposition are lost.
     */
    RWSVDecomp<TypeT, SVDCalc>& operator=(const RWSVDecomp<TypeT, SVDCalc>& x);

    /**
     * Returns the number of columns in the matrix that the decomposition
     * represents.
     */
    unsigned cols() const {
        return (unsigned)VT_.cols();
    }

    /**
     * Returns the \a i <SUP>th</SUP> singular value.
     */
    norm_type                  singularValue(int i) const;

    /**
     * Returns the vector of singular values.
     */
    const RWMathVec<norm_type> singularValues() const {
        return sigma_;
    }

    /**
     * Returns the \a i <SUP>th</SUP> left singular vector. Throws an
     * exception if the \a i <SUP>th</SUP> vector is <i>not</i> computed.
     */
    const RWMathVec<TypeT>    leftVector(int i) const;

    /**
     * Returns a matrix whose columns are the left singular vectors.
     */
    const RWGenMat<TypeT>     leftVectors() const {
        return U_;
    }

    /**
     * Returns an indication of whether all singular values are successfully
     * computed.
     */
    bool             good() const {
        return computedAll_;
    }

    /**
     * @copydoc good()
     */
    bool             fail() const {
        return !computedAll_;
    }

    /**
     * Returns the number of left singular vectors computed.
     */
    unsigned numLeftVectors() const {
        return (unsigned)U_.cols();
    }

    /**
     * Returns the number of right singular vectors computed.
     */
    unsigned numRightVectors() const {
        return (unsigned)VT_.rows();
    }

    /**
     * Returns the rank of the matrix. The rank is indicated by the
     * number of nonzero singular values.
     */
    unsigned rank() const {
        return (unsigned)sigma_.length();
    }

    /**
     * Returns the \a i <SUP>th </SUP>right singular vector. Throws an
     * exception if the \a i <SUP>th</SUP> vector is <i>not</i> computed.
     */
    const RWMathVec<TypeT>    rightVector(int i) const;

    /**
     * Returns a matrix whose columns are the right singular vectors.
     */
    const RWGenMat<TypeT>     rightVectors() const {
        return adjoint(VT_);
    }

    /**
     * Returns the number of rows in the matrix that the decomposition
     * represents.
     */
    unsigned rows() const {
        return (unsigned)U_.rows();
    }

    /**
     * Truncates all singular values with magnitude less than \a tol
     * by setting them to 0. If \a tol is a measure of the expected
     * error in entries of the matrix, this truncation provides a more
     * meaningful decomposition. The rank of the decomposition is a
     * measure of the numerical rank of the matrix.
     */
    void                  truncate(norm_type tol);

    friend class RWSVServer<TypeT, SVDCalc>;
};


/**
 * @ingroup decomposition_classes
 * @brief Used to construct instances of the singular value decomposition
 * class, \link RWSVDecomp RWSVDecomp<T> \endlink
 *
 * The singular value decomposition server class, \link RWSVServer
 * RWSVServer<T> \endlink, is used to construct instances of the
 * singular value decomposition class, \link RWSVDecomp RWSVDecomp<T>
 * \endlink. Using a server rather than the standard singular value
 * decomposition constructor gives you control over what is computed.
 *
 * The template parameter \c \<SVDCalc\> determines the algorithm used
 * by the \link RWSVDecomp RWSVDecomp<T> \endlink class to compute
 * the singular value decomposition and must implement the following
 * method:
 *
 * @note
 * For greater flexibility, the user can implement this method,
 * or the Linear Algebra Module provides two classes to perform
 * this function - \link RWSVDCalc RWSVDCalc<T> \endlink and
 * \link RWSVDDivConqCalc RWSVDDivConqCalc<T> \endlink. Please
 * see their descriptions for more information.
 *
 * @code
 * bool computeSVD(const RWGenMat<T>&   A,
 *                       RWGenMat<T>&   U,
 *                       RWGenMat<T>&  VT,
 *                       RWMathVec<norm_type>&  sigma,
 *                       norm_type  tolerance,
 *                   int numLeftVectors = -1,
 *                   int numRightVectors = -1);
 * @endcode
 *
 * where norm_type is a typedef for rw_numeric_traits<T>::norm_type.
 *
 * Parameters:
 *
 * <b>A</b> - The input matrix for which the singular value decomposition
 * is being computed.
 *
 * <b>U</b> - The output matrix of left singular victors (the columns
 * of U are the left singular vectors).
 *
 * <b>VT</b> - The output matrix of right singular victors (the
 * rows of VT are the right singular victors).
 *
 * <b>sigma</b> - The output vector of singular values in descending
 * order.
 *
 * <b>tolerance</b> - The input singular values with magnitude less
 * than tolerance will be set to zero.
 *
 * <b>numLeftVectors</b> - The input number of left vectors to compute.
 * If the number is less than zero, the default number of vectors
 * will be computed (it is up to the developer to determine what
 * the default is).
 *
 * <b>numRightVectors</b> - The input number of right vectors to
 * compute. If the number is less than zero, the default number
 * of vectors will be computed (it is up to the developer to determine
 * what the default is).
 *
 * The return value is \c true if the decomposition was successfully
 * computed.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/sv.h>
 * #include <rw/lapack/svddccalc.h>
 *
 * RWSVServer<double, RWSVDDivConqCalc<double> > server;
 * @endcode

 * @section example Example
 *
 * @code
 * // Read a matrix from standard input and print its singular
 * // values. Use a SV decomposition server to prevent computing
 * // the singular vectors.
 *
 * #include <iostream>
 * #include <rw/math/genmat.h>
 * #include <rw/lapack/sv.h>
 * #include <rw/lapack/svddccalc.h>
 *
 * int main()
 * {
 *     RWGenMat<double> A;
 *     std::cin >> A;
 *     RWSVServer<double, RWSVDDivConqCalc<double> > server;
 *     server.computeVectors(0);          // compute zero vectors
 *     RWSVDecomp<double, RWSVDDivConqCalc<double> > SV = server(A);
 *     std::cout << "Singular values are " << SV.SingularValues()
 *          << std::endl;
 *     return 0;
 * }
 * @endcode
 *
 */
template <class TypeT, class SVDCalc>
class RWSVServer : public SVDCalc
{
public:
    /**
     * Typedef for the usual return type of numerical <tt>norm</tt>-like
     * functions. For more information, see rw_numeric_traits<T>::norm_type.
     */
    typedef typename rw_numeric_traits<TypeT>::norm_type norm_type;
protected:
    bool computeAllLeftVecs_;      // compute all m left vecs if true
    bool computeAllRightVecs_;     // compute all n right vecs if true
    bool computeDefaultLeftVecs_;  // compute first min(m,n) left vecs if true
    bool computeDefaultRightVecs_; // compute first min(m,n) right vecs if true
    unsigned  numLeftVecs_;             // number of left vectors to compute (if switches all off)
    unsigned  numRightVecs_;            // number of right vectors to compute (if switches all off)
    norm_type tol_;                     // when to disregard singular values

public:
    /**
     * Default constructor. Builds a new server, which computes all
     * singular vectors by default.
     */
    RWSVServer();

    /**
     * Computes a singular value decomposition.
     */
    virtual RWSVDecomp<TypeT, SVDCalc> operator()(const RWGenMat<TypeT>&) const;

    /**
     * Configures the server to compute the first \c min(m,n) singular
     * vectors, where \c m x \c n is the size of the input matrix.
     */
    void  computeAllVectors();

    /**
     * Configures the server to compute all left singular vectors.
     */
    void  computeAllLeftVectors();

    /**
     * Configures the server to compute all right singular vectors.
     */
    void  computeAllRightVectors();

    /**
     * Configures the server to compute all singular vectors.
     */
    void  computeDefaultVectors();

    /**
     * Configures the server to compute the first \c min(m,n) left singular
     * vectors, where \c m x \c n is the size of the input matrix.
     */
    void  computeDefaultLeftVectors();

    /**
     * Configures the server to compute the first \c min(m,n) right singular
     * vectors, where \c m x \c n is the size of the input matrix.
     */
    void  computeDefaultRightVectors();

    /**
     * Configures the server to compute the first \a n singular vectors.
     */
    void  computeVectors(unsigned n);

    /**
     * Configures the server to compute the first \a n left singular
     * vectors.
     */
    void  computeLeftVectors(unsigned n);

    /**
     * Configures the server to compute the first \a n right singular
     * vectors.
     */
    void  computeRightVectors(unsigned n);

    /**
     * Specifies an accuracy to which the singular values should be
     * computed.
     */
    void  setTolerance(norm_type);

    virtual ~RWSVServer() {};
};

#if 0
// This class not implemented at this time.
template <class TypeT>
class RWSVQRServer : public RWSVServer
{
    RWSVQRServer();
    virtual RWSVDecomp operator()(const RWGenMat<TypeT>&) const;
};
#endif


#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/sv.cc>
#endif


#endif
