#ifndef __RWSKEWMAT_H__
#define __RWSKEWMAT_H__
/*
 * RWSkewMat<TypeT> - An skew symmetric matrix of Ts
 *
 * Stores a skew symmetric matrix.  Note that only half the matrix is actually
 * stored.  This means if you change, say, entry (2,3) you are also
 * implicitly changing entry (3,2).
 *
 * The lower left triangle of the matrix is stored in column major
 * order.
 *
 * Bounds checking is enabled by setting the preprocessor
 * symbol RWBOUNDS_CHECK.  This is strongly encouraged.
 *
 ***************************************************************************
 * $Id: //lapack/13/rw/lapack/skewmat.h#1 $
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

#include <rw/dcomplex.h>
#include <rw/lapkdefs.h>
#include <rw/math/mathvec.h>
#include <rw/math/genmat.h>
#include <rw/lapack/mat.h>

#include <rw/lapack/symmat.h>
#include <rw/lapack/latraits.h>

template <class TypeT>
class RWGenMat;
template <class TypeT>
class RWSymMat;

/**
 * @ingroup sparse_matrix_classes
 * @brief Encapsulates skew symmetric matrices.
 *
 * The class \link RWSkewMat RWSkewMat<T> \endlink encapsulates
 * skew symmetric matrices. A skew symmetric matrix is defined by
 * the requirement that <i>A<SUB>ij</SUB> = -A<SUB>ji</SUB></i>.
 * This strict definition implies that the diagonal entries must
 * be 0. This requirement is relaxed by the Rogue Wave skew symmetric
 * matrix classes, which require only that <i>A<SUB>ij</SUB>
 * = -A<SUB>ji</SUB></i> for the off-diagonal elements; in
 * other words, the diagonal need \e not be 0. Skew symmetric
 * matrices with nonzero diagonals are sometimes useful, for example,
 * as rotation matrices in computer graphics applications.
 *
 * @section RWSkewMat_synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/skewmat.h>
 *
 * RWSkewMat<double> A;
 * @endcode

 * @section RWSkewMat_example Example
 *
 * @code
 * #include <rw/lapack/skewmat.h>
 *
 * int main()
 * {
 *     RWSkewMat<float> T(3,3);
 *     RWMathVec<float> x(3);
 *     RWMathVec<float> Tx = product(T,x);
 *     return 0;
 * }
 * @endcode
 *
 * @section RWSkewMat_storage_scheme Storage Scheme
 *
 * The upper triangle of the matrix is stored in column major order.
 * The lower triangle is then calculated implicitly.
 *
 * \f[
 * \begin{bmatrix}
 * A_{11}  & A_{12}  & A_{12}   & ...    & A_{1n} \\
 * -A_{12} & A_{22}  & A_{23}   & ...    & A_{2n} \\
 * -A_{13} & -A_{23} & A_{33}   & ...    & A_{3n} \\
 * .       &         &          &        &        \\
 * .       &         &          &        &        \\
 * .       &         &          &        &        \\
 * A_{1n}  & A_{2n}  & A_{3n}   & ...    & A_{nn} \\
 * \end{bmatrix}
 * \f]
 *
 * The data is stored in the following order:
 *
 * <tt>[  A<sub>11</sub> A<sub>12</sub> A<sub>22</sub>
 *        A<sub>13</sub> A<sub>23</sub> A<sub>33</sub>
 *        ...
 *        A<sub>1n</sub> A<sub>2n</sub> A<sub>3n</sub>
 *        ...
 *        A<sub>nn</sub> ]
 * </tt>
 *
 * The mapping between the array and storage vector is as follows:
 *
 * \f[
 * A(i + 1, j + 1) \to
 *   \left\{
 *     \begin{array}{l l}
 *        \text{vec}[j(j+1)/ 2+i] & \quad
 *          \text{if  } i \leq j \\
 *        \text{-vec}[i(i+1)/ 2+j]
 *          & \quad \text{if  } j \leq i \\
 *     \end{array}
 *   \right.
 * \f]
 */
template <class TypeT>
class RWSkewMat
{
private:

    RWMathVec<TypeT> vec;
    unsigned n;
    // The data which define the matrix


public:

    /**
     * Default constructor. Builds a matrix of size 0 x 0. This constructor
     * is necessary to declare a matrix with no explicit constructor
     * or to declare an array of matrices.
     */
    RWSkewMat();

    /**
     * Build a copy of its argument, \a A. Note that the new matrix
     * references \a A's data. To construct a matrix with its own copy
     * of the data, use either the copy() or deepenShallowCopy() member
     * functions.
     */
    RWSkewMat(const RWSkewMat<TypeT>& A);

    /**
     * Defines an uninitialized matrix of size \a n x \a nAgain. Both arguments
     * must be equal or a runtime error occurs. This constructor is
     * used, rather than a constructor that takes only a single argument,
     * to avoid type conversion problems.
     */
    RWSkewMat(unsigned n, unsigned nAgain);

    /**
     * Constructs a size \a n x \a nAgain matrix using the data in the passed
     * vector. Arguments \a n and \a nAgain must be equal or a runtime error
     * occurs. This data must be stored in the format described in the
     * \ref RWSkewMat_storage_scheme section. The resultant matrix references
     * the data in vector \a vd.
     */
    RWSkewMat(const RWMathVec<TypeT>& vd, unsigned n, unsigned nAgain);

    RW_LAPK_MEMBER_EXPORT RWSkewMat(const typename rw_linear_algebra_traits<TypeT>::generic_skew_mat& re);

    /**
     * Constructs a complex matrix from the real and imaginary parts
     * supplied.
     * @note the imaginary part of a complex skew-symmetric
     * matrix is symmetric, \e not skew-symmetric.
     */
    RW_LAPK_MEMBER_EXPORT RWSkewMat(const RWSkewMat<double>& re, const RWSkewMat<double>& im);

    ~RWSkewMat();
    // Constructors, conversion functions, and a destructor.

    static const char* className;
    // For use in error messages

    /**
     * Returns the number of rows in the matrix.
     */
    inline unsigned rows() const {
        return n;
    }

    /**
     * Returns the number of columns in the matrix.
     */
    inline unsigned cols() const {
        return n;
    }
    // Member functions to access the shape of the matrix


    /**
     * Returns the value of the \a ij <sup>th</SUP> element of the matrix.
     * Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined when the header file is read. The member function
     * bcval() does the same thing with guaranteed bounds checking.
     */
    inline TypeT  val(int i, int j) const;

    /**
     * Returns the value of the \a ij <sup>th</SUP> element of the matrix,
     * after doing bounds checking.
     */
    TypeT  bcval(int i, int j) const;

    /**
     * Sets the \a ij <sup>th</SUP> element of the matrix equal to \a
     * x. Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined when the header file is read. The member function
     * bcset() does the same thing with guaranteed bounds checking.
     */
    inline TypeT  set(int i, int j, TypeT x);

    /**
     * Sets the \a ij <sup>th</SUP> element of the matrix equal to \a
     * x, after doing bounds checking.
     */
    TypeT  bcset(int i, int j, TypeT x);

    /**
     * References the \a ij <sup>th</SUP> element.
     * This operator is equivalent to calling the ref() member function.
     * Bounds checking is done if the preprocessor symbol \c
     * RWBOUNDS_CHECK is defined before including the header file.
     */
    inline RWNGRef<TypeT> operator()(int i, int j);

    /**
     * Returns a reference to the \a ij <sup>th</SUP> element of the
     * matrix. Bounds checking is done if the preprocessor symbol \c
     * RWBOUNDS_CHECK is defined when the header file is read. The member
     * function bcref() does the same thing with guaranteed bounds
     * checking.
     */
    inline RWNGRef<TypeT> ref(int i, int j);

    /**
     * Returns a reference to the \a ij <sup>th</SUP> element of the
     * matrix, after doing bounds checking.
     */
    RWNGRef<TypeT> bcref(int i, int j);

    /**
     * Accesses the \a ij <sup>th</SUP> element.
     * Using this operator is equivalent to calling the val()
     * member function. Bounds checking is done if the preprocessor
     * symbol \c RWBOUNDS_CHECK is defined before including the
     * header file.
     */
    inline TypeT  operator()(int i, int j) const; // { return val(i,j); }
    // Access and/or change elements of the matrix.
    // The versions that begin with the letters bc do bounds
    // checking.  The others do bounds checking only if the
    // preprocessor symbol RWBOUNDS_CHECK is defined.

    /**
     * Returns the \a k x \a k upper left corner of the matrix. The
     * submatrix and the matrix share the same data.
     */
    RWSkewMat<TypeT> leadingSubmatrix(int k);
    // Access to submatrices.
    // The leading submatrix of order k is the kxk matrix in the upper
    // left.
    // Access to diagonals.  The same conventions as for
    // element access apply (with respect to bcxxx and xxx names).


    /**
     * Sets the matrix elements equal to the elements of \a A. The two
     * matrices must be the same size. To make the matrix reference
     * the same data as \a A, use the reference member function.
     */
    RWSkewMat<TypeT>& operator=(const RWSkewMat<TypeT>& A);

    /**
     * Makes this matrix a reference to the argument matrix. The two
     * matrices share the same data. The matrices do \e not have
     * to be the same size before calling reference(). To copy a matrix
     * into another of the same size, use the operator=() member operator.
     */
    RWSkewMat<TypeT>& reference(RWSkewMat<TypeT>& m);
    // reference() makes an alias, operator= makes a copy.


    operator RWGenMat<TypeT>() const;

    /**
     * Sets every element of the matrix to 0.
     */
    void zero() {
        vec = TypeT(0);
    }
    // Sets all elements of the matrix equal to zero.

    /**
     * Creates a copy of this matrix with distinct data. The stride
     * of the data vector in the new matrix is guaranteed to be 1.
     */
    RWSkewMat<TypeT> copy() const;

    /**
     * Creates a copy of this matrix with distinct data. The stride
     * of the data vector in the new matrix is guaranteed to be 1.
     */
    RWSkewMat<TypeT> deepCopy() const {
        return copy();
    }

    /**
     * Ensures that the data in the matrix is \e not shared by any
     * other matrix or vector. Also ensures that the stride in the data
     * vector is equal to 1. If necessary, a new copy of the data vector
     * is made.
     */
    void deepenShallowCopy() {
        vec.deepenShallowCopy();
    }
    // copy() returns a RWSkewMat<TypeT> with its own copy of the data.
    // deepenShallowCopy makes sure the data is not shared by another
    // vector or matrix.

    RWMathVec<TypeT> dataVec() {
        return vec;
    }

    /**
     * Returns the matrix's data vector. This is where the explicitly
     * stored entries in the matrix are kept.
     */
    const RWMathVec<TypeT>& dataVec() const {
        return vec;
    }

    /**
     * Returns a pointer to the first item of data in the vector storing
     * the matrix's data. You can use this (with caution!) to pass the
     * matrix's data to C or FORTRAN subroutines. Be aware that the
     * stride of the data vector may \e not be 1.
     */
    TypeT* data() {
        return vec.data();
    }

    const TypeT* data() const {
        return vec.data();
    }
    // Access to the raw data.
    /**
     * Resizes the matrix. Any new entries in the matrix are set to
     * 0. Both arguments must be the same.
     */
    void resize(unsigned m, unsigned n);
    // Resize the matrix.  New elements are set to zero.

    /**
     * Reads a matrix from an input stream. The format of the matrix
     * is the same as the format output by the printOn() member function.
     * Below is a sample matrix that could be input. Note that extra
     * white space and any text preceding the dimension specification
     * are ignored. Only the skew symmetric part of the matrix is used.
     *
     * @code
     * 3x3  [  4    5    7
     *        -5    9    5
     *        -7  -5     3  ]
     *
     * @endcode
     */
    void scanFrom(std::istream&);                // Human readable

    /**
     * Prints the matrix to an output stream in human readable format.
     */
    void printOn(std::ostream&) const;

    /**
     * Reads in a matrix from an RWvistream, the Rogue Wave virtual
     * input stream class. The matrix must have been stored to the stream
     * using the saveOn() member function.
     */
    void restoreFrom(RWvistream&);        // Internal US-ASCII or binary code

    /**
     * Stores a matrix to an RWvostream, the Rogue Wave virtual output
     * stream class. The matrix can be read using the restoreFrom()
     * member function.
     */
    void saveOn(RWvostream&) const;

    /**
     * Reads in a matrix from an RWFile. The matrix must have been stored
     * to the file using the saveOn() member function.
     */
    void restoreFrom(RWFile&);

    /**
     * Stores a matrix to an RWFile. The matrix can be read using the
     * restoreFrom() member function.
     */
    void saveOn(RWFile&) const;

    /**
     * Returns the number of bytes that it would take to write the matrix
     * to a file using saveOn().
     */
    unsigned binaryStoreSize() const;
    // Input/output member functions
    // Whether US-ASCII or binary formatting is used with readFrom and
    // storeOn is determined by the type of rwstream passed.

    /*
     * The operator()() member functions are declared with ref(),
     * since the declarations look the same.
     *
     * The operator=() assignment operators are declared with reference(),
     * since the functions are so closely related.
     */

    /**
     * Two matrices are considered equal if they
     * have the same size and their elements are all exactly the same.
     * Be aware that floating point arithmetic is \e not exact;
     * matrices that are theoretically equal are not always numerically
     * equal.
     */
    bool operator==(const RWSkewMat<TypeT>& X);

    /**
     * @copydoc operator==(const RWSkewMat<TypeT>&)
     */
    bool operator!=(const RWSkewMat<TypeT>& X) {
        return !(operator==(X));
    }
    // Two matrices are considered equal if their shapes are identical and
    // all their components are identical.

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     */
    RWSkewMat<TypeT>& operator+=(const RWSkewMat<TypeT>& m);

    /**
     * @copydoc operator+=(const RWSkewMat<TypeT>&)
     */
    RWSkewMat<TypeT>& operator-=(const RWSkewMat<TypeT>& m);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     * @note operator*=() does element-by-element
     * multiplication, \e not inner-product style matrix multiplication.
     * You can use the product() global function to do matrix-matrix inner
     * product multiplication.
     */
    RWSkewMat<TypeT>& operator*=(const RWSkewMat<TypeT>& m);

    /**
     * Performs the indicated operation on each element of the matrix.
     */
    RWSkewMat<TypeT>& operator*=(TypeT);

    /**
     * @copydoc operator+=(const RWSkewMat<TypeT>&)
     */
    RWSkewMat<TypeT>& operator/=(const RWSkewMat<TypeT>& m);

    /**
     * @copydoc operator*=(TypeT)
     */
    RWSkewMat<TypeT>& operator/=(TypeT);
    // assignment operators.  self must be same size as m.


};

/*
 * This function returns a new matrix the same shape as the one
 * passed in but with the data passed in.  This way we restrict
 * all the >if statements to a couple lines
 */
template <class TypeT>
inline RWSkewMat<TypeT> sameShapeMatrix(RWMathVec<TypeT>& vec, const RWSkewMat<TypeT>& A)
{
    return RWSkewMat<TypeT>(vec, A.rows(), A.cols());
}

inline RWSkewMat<double> sameShapeRealMatrix(RWMathVec<double>& vec, const RWSkewMat<DComplex>& A)
{
    return RWSkewMat<double>(vec, A.rows(), A.cols());
}

/**
 * @relates RWSkewMat
 * Unary minus operator. Returns a copy of the matrix's negation.
 */
template <class TypeT>
RWSkewMat<TypeT>  operator-(const RWSkewMat<TypeT>&);        // Unary minus

/**
 * @relates RWSkewMat
 * Unary plus operator. Returns a copy of the matrix.
 */
template <class TypeT>
RWSkewMat<TypeT>  operator+(const RWSkewMat<TypeT>&);        // Unary plus

/**
 * @relates RWSkewMat
 * Performs element-by-element operations on the arguments. To do
 * inner product matrix multiplication, you can use the product()
 * global function.
 */
template <class TypeT>
RWSkewMat<TypeT>  operator*(const RWSkewMat<TypeT>&, const RWSkewMat<TypeT>&);

/**
 * @relates RWSkewMat
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
RWSkewMat<TypeT>  operator/(const RWSkewMat<TypeT>&, const RWSkewMat<TypeT>&);

/**
 * @relates RWSkewMat
 * @copydoc operator/(const RWSkewMat<TypeT>&, const RWSkewMat<TypeT>&)
 */
template <class TypeT>
RWSkewMat<TypeT>  operator+(const RWSkewMat<TypeT>&, const RWSkewMat<TypeT>&);

/**
 * @relates RWSkewMat
 * @copydoc operator/(const RWSkewMat<TypeT>&, const RWSkewMat<TypeT>&)
 */
template <class TypeT>
RWSkewMat<TypeT>  operator-(const RWSkewMat<TypeT>&, const RWSkewMat<TypeT>&);

/**
 * @relates RWSkewMat
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
inline RWSkewMat<TypeT>  operator*(const RWSkewMat<TypeT>& A, TypeT x)
{
    RWMathVec<TypeT> temp(A.dataVec()*x);
    return sameShapeMatrix(temp, A);
}

/**
 * @relates RWSkewMat
 * @copydoc operator*(const RWSkewMat<TypeT>&, TypeT)
 */
template <class TypeT>
#ifndef RW_NO_INLINED_TEMP_DESTRUCTORS
inline RWSkewMat<TypeT>  operator*(TypeT x, const RWSkewMat<TypeT>& A)
{
    return A * x;
}
#else
RWSkewMat<TypeT>  operator*(TypeT x, const RWSkewMat<TypeT>& A);
#endif

/**
 * @relates RWSkewMat
 * @copydoc operator*(const RWSkewMat<TypeT>&, TypeT)
 */
template <class TypeT>
RWSkewMat<TypeT>  operator/(const RWSkewMat<TypeT>& A, TypeT x);

/**
 * @relates RWSkewMat
 * @copydoc operator*(const RWSkewMat<TypeT>&, TypeT)
 */
template <class TypeT>
RWSkewMat<TypeT>  operator/(TypeT x, const RWSkewMat<TypeT>& A);
// Arithmetic operators; Notice that operator* is an
// element by element multiply, NOT a matrix multiply.

/**
 * @relates RWSkewMat
 * Writes the matrix to the stream. This is equivalent to calling
 * the printOn() member function.
 */
template <class TypeT>
inline std::ostream&  operator<<(std::ostream& s, const RWSkewMat<TypeT>& m)
{
    m.printOn(s);
    return s;
}

/**
 * @relates RWSkewMat
 * Reads the matrix from the stream. This is equivalent to calling
 * the scanFrom() member function.
 */
template <class TypeT>
inline std::istream&  operator>>(std::istream& s, RWSkewMat<TypeT>& m)
{
    m.scanFrom(s);
    return s;
}

// I/O for humans

/**
 * @relates RWSkewMat
 * Returns the transpose of the argument matrix.
 */
template <class TypeT>
RWSkewMat<TypeT>  transpose(const RWSkewMat<TypeT>&);
// The transpose of the matrix.

/**
 * @relates RWSkewMat
 * Returns the inner product (matrix-vector product) of \a A and
 * \a x.
 */
template <class TypeT>
RWMathVec<TypeT>  product(const RWSkewMat<TypeT>& A, const RWMathVec<TypeT>& x);

/**
 * @relates RWSkewMat
 * Returns the inner product (matrix-vector product) of \a x and
 * \a A. This is equal to the product of \a A transpose and \a x.
 */
template <class TypeT>
RWMathVec<TypeT>  product(const RWMathVec<TypeT>& x, const RWSkewMat<TypeT>& A);
// inner products

// Below are functions that assume automatic conversion to GenMat's
// Automatic conversions do not take place for templatized functions,
// So we provide them here...
template <class TypeT>
RWGenMat<TypeT>  product(const RWSkewMat<TypeT>& A, const RWGenMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  product(const RWGenMat<TypeT>& A, const RWSkewMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  product(const RWSkewMat<TypeT>& A, const RWSkewMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWSkewMat<TypeT>& A, const RWGenMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWGenMat<TypeT>& A, const RWSkewMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWSkewMat<TypeT>& A, const RWSkewMat<TypeT>& B);

inline RWGenMat<DComplex>  conjTransposeProduct(const RWSkewMat<DComplex>& A, const RWGenMat<DComplex>& B)
{
    return conjTransposeProduct(RWGenMat<DComplex>(A), B);
}

inline RWGenMat<DComplex>  conjTransposeProduct(const RWGenMat<DComplex>& A, const RWSkewMat<DComplex>& B)
{
    return conjTransposeProduct(A, RWGenMat<DComplex>(B));
}

inline RWGenMat<DComplex>  conjTransposeProduct(const RWSkewMat<DComplex>& A, const RWSkewMat<DComplex>& B)
{
    return conjTransposeProduct(RWGenMat<DComplex>(A), RWGenMat<DComplex>(B));
}

template <class TypeT>
inline RWGenMat<TypeT>  adjoint(const RWSkewMat<TypeT>& A)
{
    RWGenMat<TypeT> a(A);
    return adjoint(a);
}

/**
 * @relates RWSkewMat
 * Extracts the skew symmetric part of a square matrix. The skew
 * symmetric part of a matrix \a A is <tt>(A<SUP>-AT</SUP>)/2</tt>.
 * If the argument \a keepMainDiag is set to \c true (as is the
 * default) then the main diagonal of \a A is kept, even though
 * it should be 0 according to the strict mathematical definition
 * of skew-symmetry.
 */
template <class TypeT>
RWSkewMat<TypeT>  toSkewMat(const RWGenMat<TypeT>& A, bool keepMainDiag = true);


/**
 * @relates RWSkewMat
 * Returns a matrix whose entries are the absolute value of the
 * argument. The absolute value of a complex number is considered
 * to be the sum of the absolute values of its real and imaginary
 * parts. To get the norm of a complex matrix, you can use the
 * norm() function.
 */
template <class TypeT>
inline RWSymMat<typename rw_numeric_traits<TypeT>::norm_type>  abs(const RWSkewMat<TypeT>& A)
{
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;
    //RWMathVec<typename rw_numeric_traits<TypeT>::norm_type> temp(abs(A.dataVec()));
    RWMathVec<rt> temp(abs(A.dataVec()));
    return RWSymMat<rt>(temp, A.rows(), A.cols());
}

/**
 * @relates RWSkewMat
 * Returns a matrix where each element is the real part of the corresponding
 * element in the matrix \a A.
 */
inline RWSkewMat<double>  real(const RWSkewMat<DComplex>& A)
{
    RWMathVec<double> temp(real(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWSkewMat
 * Returns a matrix where each element is the imaginary part of
 * the corresponding element in the matrix \a A.
 */
inline RWSkewMat<double>  imag(const RWSkewMat<DComplex>& A)
{
    RWMathVec<double> temp(imag(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}
/**
 * @relates RWSkewMat
 * Returns a matrix where each element is the norm (magnitude) of
 * the corresponding element in the matrix \a A.
 */
inline RWSymMat<double>  norm(const RWSkewMat<DComplex>& A)
{
    RWMathVec<double> temp(norm(A.dataVec()));
    return RWSymMat<double>(temp, A.rows(), A.cols());
}/* Sorry, no arg function for SkewMat */



/*
 * Inline functions
 */
template <class TypeT>
inline TypeT RWSkewMat<TypeT>::val(int i, int j) const
{
#ifdef RWBOUNDS_CHECK
    return bcval(i, j);
#else
    int index = (i <= j) ? (j * (j + 1) / 2 + i) : (i * (i + 1) / 2 + j);
    TypeT val = vec(index);
    return ((i <= j) || (val == TypeT(0))) ?  val : -val;
#endif
}

template <class TypeT>
inline TypeT RWSkewMat<TypeT>::set(int i, int j, TypeT x)
{
    return bcset(i, j, x);
}

template <class TypeT>
inline RWNGRef<TypeT> RWSkewMat<TypeT>::ref(int i, int j)
{
    return bcref(i, j);
}

template <class TypeT>
inline RWNGRef<TypeT> RWSkewMat<TypeT>::operator()(int i, int j)
{
    return ref(i, j);
}
template <class TypeT>
inline TypeT RWSkewMat<TypeT>::operator()(int i, int j) const
{
    return val(i, j);
}


#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/skewmat.cc>
#endif

#endif
