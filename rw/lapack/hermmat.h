
#ifndef __RWHERMMAT_H__
#define __RWHERMMAT_H__
/*
 * HermMat - A hermetian matrix of Complexes
 *
 * Stores an Hermitian matrix.  Note that only the lower left
 * half of the matrix is actually stored.
 *
 * The lower left triangle of the matrix is stored in column major
 * order.
 *
 * Bounds checking is enabled by setting the preprocessor
 * symbol RWBOUNDS_CHECK.  This is strongly encouraged.
 *
 ***************************************************************************
 *
 * $Id: //lapack/13/rw/lapack/hermmat.h#1 $
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
#include <rw/math/mathvec.h>
#include <rw/math/genmat.h>
#include <rw/lapack/mat.h>
#include <rw/lapack/symmat.h>
#include <rw/lapack/skewmat.h>


template <class TypeT>
class RWSymMat;
template <class TypeT>
class RWSkewMat;
template <class TypeT>
class RWGenMat;


// this matrix should exist for Complex'es only
// define as a template so it can be used with different 'Complex' classes

/**
 * @ingroup sparse_matrix_classes
 * @brief Encapsulates a Hermitian matrix.
 *
 * The class \link RWHermMat RWHermMat<T> \endlink encapsulates
 * a Hermitian matrix. A Hermitian matrix is defined by the requirement
 * that <i>A<SUB>ij</SUB> = conj(A<SUB>ji</SUB>)</i>.
 * This strict definition implies that the diagonal elements have
 * 0 imaginary parts. The Rogue Wave classes relax this definition
 * along the diagonal so that diagonal elements can have nonzero
 * imaginary parts.
 *
 * @section RWHermMat_synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/hermmat.h>
 *
 * RWHermMat<T> H;
 * @endcode
 *
 * @section RWHermMat_example Example
 *
 * @code
 * #include <rw/lapack/hermmat.h>
 * #include <rw/lapack/symmat.h>
 * #include <rw/lapack/skewmat.h>
 *
 * int main()
 * {
 *   RWHermMat<T> H(4,4);
 *   RWSymMat<double> Hreal = real(H);
 *   RWSkewMat<double> Himag = imag(H);
 *   return 0;
 * }
 * @endcode
 *
 * @section RWHermMat_storage_scheme Storage Scheme
 *
 * The upper triangle of the matrix is stored in column major order. The
 * lower triangle is then calculated implicitly. The upper triangle is
 * stored in column major order. This storage scheme is used so that the
 * leading part of the matrix is always located in contiguous memory.
 *
 * For example, given the following matrix:
 *
 * \f[
 *   \begin{bmatrix}
 *    A_{11} & A_{12} & A_{13} & \dots & A_{1n} \\
 *    \text{conj}(A_{12}) & A_{22} & A_{23} & \dots & A_{2n} \\
 *    \text{conj}(A_{13}) & \text{conj}(A_{23}) & A_{33} & \dots & A_{3n} \\
 *    \vdots \\
 *    \text{conj}(A_{1n}) & \text{conj}(A_{2n}) & \text{conj}(A_{3n}) &
 *    \dots & A_{nn}
 *   \end{bmatrix}
 * \f]
 *
 *
 * the data is stored in the following order:
 *
 * [ A<SUB>11</SUB> A<SUB>12</SUB> A<SUB>22</SUB> A<SUB>13</SUB> A<SUB>23</SUB>
 * A<SUB>33</SUB> ... A<SUB>1n</SUB> A<SUB>2n</SUB> A<SUB>3n</SUB> ...
 * A<SUB>nn</SUB> ]
 *
 * The mapping between the array and the storage vector is as follows:
 *
 * \f[
 * A(i + 1, j + 1) =
 *   \left\{
 *     \begin{array}{l l}
 *        \text{vec}[j(j+1) / 2+i] & \quad i \leq j \\
 *        \text{conj}(\text{vec}[i(i+1) / 2+j]) & \quad j < i \\
 *     \end{array}
 *   \right.
 * \f]
 */
template <class TypeT>
class RWHermMat
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
    RWHermMat();

    /**
     * Builds a copy of its argument, \a A. Note that the new matrix
     * references the data in \a A. To construct a matrix with its own copy
     * of the data, you can use either the copy() or deepenShallowCopy()
     * member functions.
     */
    RWHermMat(const RWHermMat<TypeT>& A);

    /**
     * Defines an uninitialized \a x by \a y matrix. Both arguments
     * must be equal or a runtime error occurs.
     */
    RWHermMat(unsigned x, unsigned y);

    /**
     * Constructs a \a x by \a y matrix using the data in the passed
     * vector. This data must be stored in the format described in the
     * \ref RWHermMat_storage_scheme section. The
     * resultant matrix references the data in vector \a vd.
     */
    RWHermMat(const RWMathVec<TypeT>& data, unsigned x, unsigned y);

    /**
     * Constructs a complex matrix from the real parts
     * supplied. The imaginary part is assumed to be 0.
     */
    RWHermMat(const RWSymMat<double>& re);

    /**
     * Constructs a complex matrix from the real and imaginary parts
     * supplied.
     */
    RWHermMat(const RWSymMat<double>& re, const RWSkewMat<double>& im);
    ~RWHermMat();
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

    /**
     * Returns the value of the \a ij <SUP>th</SUP> element of the matrix.
     * Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined when the header file is read. The member function
     * bcval() does the same thing with guaranteed bounds checking.
     */
    inline TypeT  val(int i, int j) const;

    /**
     * Returns the value of the \a ij <SUP>th</SUP> element of the matrix,
     * after doing bounds checking.
     */
    TypeT  bcval(int i, int j) const;

    /**
     * Sets the \a ij <SUP>th</SUP> element of the matrix equal to \a
     * x. Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined when the header file is read. The member function
     * bcset() does the same thing with guaranteed bounds checking.
     */
    inline TypeT  set(int i, int j, TypeT x);

    /**
     * Sets the \a ij <SUP>th</SUP> element of the matrix equal to \a
     * x, after doing bounds checking.
     */
    TypeT  bcset(int i, int j, TypeT x);

    /**
     * Accesses the \a ij <SUP>th</SUP> element. This operator returns
     * a reference type that can be used for assigning or accessing an element.
     * Using this operator is equivalent to calling the
     * ref() member function. Bounds checking is done if
     * the preprocessor symbol \c RWBOUNDS_CHECK is defined before including
     * the header file.
     */
    inline RWCJRef<TypeT> operator()(int i, int j);

    /**
     * Returns a reference to the \a ij <SUP>th</SUP> element of the
     * matrix. Bounds checking is done if the preprocessor symbol \c
     * RWBOUNDS_CHECK is defined when the header file is read. The member
     * function bcref() does the same thing with guaranteed bounds
     * checking.
     */
    inline RWCJRef<TypeT> ref(int i, int j);

    /**
     * Returns a reference to the \a ij <SUP>th</SUP> element of the
     * matrix, after doing bounds checking.
     */
    RWCJRef<TypeT> bcref(int i, int j);

    /**
     * Accesses the \a ij <SUP>th</SUP> element.
     * This operator returns a value type that can only be used for accessing
     * an element. Using this operator is equivalent to
     * calling the val() member function. Bounds checking is done if
     * the preprocessor symbol \c RWBOUNDS_CHECK is defined before including
     * the header file.
     */
    inline TypeT  operator()(int i, int j) const; // { return val(i,j); }

    /**
     * Returns the \a k x \a k upper left corner of the matrix. The
     * submatrix and the matrix share the same data.
     */
    RWHermMat<TypeT> leadingSubmatrix(int k);

    /**
     * Sets the matrix elements equal to the elements of \a A. The two
     * matrices must be the same size. To make the matrix reference
     * the same data as \a A, you can use the reference member function.
     */
    RWHermMat<TypeT>& operator=(const RWHermMat<TypeT>& A);

    /**
     * Makes this matrix a reference to the argument matrix. The two
     * matrices share the same data. The matrices do <i>not</i> have
     * to be the same size before calling reference(). To copy a matrix
     * into another of the same size, use the operator=() member operator.
     */
    RWHermMat<TypeT>& reference(RWHermMat<TypeT>& m);

    operator RWGenMat<TypeT>() const;

    /**
     * Sets every element of the matrix to 0.
     */
    void zero() {
        vec = TypeT(0, 0);
    }

    /**
     * Creates a copy of this matrix with distinct data. The stride
     * of the data vector in the new matrix is guaranteed to be 1.
     */
    RWHermMat<TypeT> copy() const;

    /**
     * Creates a copy of this matrix with distinct data. The stride
     * of the data vector in the new matrix is guaranteed to be 1.
     */
    RWHermMat<TypeT> deepCopy() const {
        return copy();
    }

    /**
     * Ensures that the data in the matrix is <i>not</i> shared by any
     * other matrix or vector. Also ensures that the stride in the data
     * vector is equal to 1. If necessary, a new copy of the data vector
     * is made.
     */
    void deepenShallowCopy() {
        vec.deepenShallowCopy();
    }

    /**
     * Returns the matrix's data vector. This is where the explicitly
     * stored entries in the matrix are kept.
     */
    RWMathVec<TypeT> dataVec() {
        return vec;
    }
    const RWMathVec<TypeT>& dataVec() const {
        return vec;
    }

    /**
     * Returns a pointer to the first item of data in the vector storing
     * the matrix's data. You can use this to pass the matrix's data
     * to C or FORTRAN subroutines, but be aware that the stride of
     * the data vector may <i>not</i> be 1.
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

    /**
     * Sets the imaginary part of the main diagonal to 0, thus ensuring
     * that the matrix satisfies the strict mathematical definition
     * of Hermitian.
     */
    void makeDiagonalReal();

    /**
     * Reads a matrix from an input stream. The format of the matrix
     * is the same as the format output by the printOn() member function.
     * Below is a sample matrix which could be input. Note that extra
     * white space and any text preceding the dimension specification
     * are ignored. Only the Hermitian part of the matrix is used.
     *
     * @code
     * 3x3
     * [ (4,2)   (5,2)   (7,8)
     *   (5,-2)  (9,2)   (5,3)
     *   (7,-8)  (5,-3)  (3,0)
     * ]
     * @endcode
     */
    void scanFrom(std::istream&);

    /**
     * Prints the matrix to an output stream in human readable format.
     */
    void printOn(std::ostream&) const;

    /**
     * Reads in a matrix from an RWvistream, the Rogue Wave virtual
     * input stream class. The matrix must have been stored to the stream
     * using the \link saveOn(RWvostream&)const saveOn() \endlink
     * member function.
     */
    void restoreFrom(RWvistream&);

    /**
     * Stores a matrix to an RWvostream, the Rogue Wave virtual output
     * stream class. The matrix can be read using the
     * \link restoreFrom(RWvistream&) restoreFrom() \endlink
     * member function.
     */
    void saveOn(RWvostream&) const;

    /**
     * Reads in a matrix from an RWFile. The matrix must have been stored
     * to the file using the \link saveOn(RWFile&)const saveOn() \endlink
     * member function.
     */
    void restoreFrom(RWFile&);

    /**
     * Stores a matrix to an RWFile. The matrix can be read using the
     * \link restoreFrom(RWFile&) restoreFrom() \endlink
     * member function.
     */
    void saveOn(RWFile&) const;

    /**
     * Returns the number of bytes that it would take to write the matrix
     * to a file using saveOn().
     */
    unsigned binaryStoreSize() const;

    /*
     * The operator()() member functions are declared with ref(),
     * since the declarations look the same.
     *
     * The operator=() assignment operators are declared with reference(),
     * since the functions are so closely related.
     */

    /**
     * Boolean operators. Two matrices are considered equal if they
     * have the same size and their elements are all exactly the same.
     * Be aware that floating point arithmetic is <i>not</i> exact,
     * so matrices that are theoretically equal are <i>not</i> always
     * numerically equal.
     */
    bool operator==(const RWHermMat<TypeT>& X);

    /**
     * Boolean operators. Two matrices are considered equal if they
     * have the same size and their elements are all exactly the same.
     * Be aware that floating point arithmetic is <i>not</i> exact,
     * so matrices that are theoretically equal are <i>not</i> always
     * numerically equal.
     */
    bool operator!=(const RWHermMat<TypeT>& X) {
        return !(operator==(X));
    }
    // Two matrices are considered equal if their shapes are identical and
    // all their components are identical.

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     */
    RWHermMat<TypeT>& operator+=(const RWHermMat<TypeT>& m);
    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     */
    RWHermMat<TypeT>& operator-=(const RWHermMat<TypeT>& m);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     * This operator does element-by-element
     * multiplication, <i>not</i> inner product style matrix multiplication.
     * You can use the product() global function to do matrix-matrix inner
     * product multiplication.
     */
    RWHermMat<TypeT>& operator*=(const RWHermMat<TypeT>& m);

    /**
     * Performs the indicated operation on each element of the matrix.
     */
    RWHermMat<TypeT>& operator*=(TypeT);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     */
    RWHermMat<TypeT>& operator/=(const RWHermMat<TypeT>& m);

    /**
     * Performs the indicated operation on each element of the matrix.
     */
    RWHermMat<TypeT>& operator/=(TypeT);


};

/*
 * This function returns a new matrix the same shape as the one
 * passed in but with the data passed in.  This way we restrict
 * all the >if statements to a couple lines
 */
template <class TypeT>
inline RWHermMat<TypeT> sameShapeMatrix(RWMathVec<TypeT>& vec, const RWHermMat<TypeT>& A)
{
    return RWHermMat<TypeT>(vec, A.rows(), A.cols());
}


/**
 * @relates RWHermMat
 *
 * Unary minus operator. Returns a copy
 * of the matrix or its negation.
 */
template <class TypeT>
RWHermMat<TypeT>  operator-(const RWHermMat<TypeT>&);        // Unary minus

/**
 * @relates RWHermMat
 *
 * Unary plus operator. Returns a copy
 * of the matrix.
 */
template <class TypeT>
RWHermMat<TypeT>  operator+(const RWHermMat<TypeT>&);

/**
 * @relates RWHermMat
 *
 * Performs element-by-element operations on the arguments. To do
 * inner product matrix multiplication, use the product() global function.
 */
template <class TypeT>
RWHermMat<TypeT>  operator*(const RWHermMat<TypeT>&, const RWHermMat<TypeT>&);

/**
 * @relates RWHermMat
 *
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
RWHermMat<TypeT>  operator/(const RWHermMat<TypeT>&, const RWHermMat<TypeT>&);

/**
 * @relates RWHermMat
 *
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
RWHermMat<TypeT>  operator+(const RWHermMat<TypeT>&, const RWHermMat<TypeT>&);

/**
 * @relates RWHermMat
 *
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
RWHermMat<TypeT>  operator-(const RWHermMat<TypeT>&, const RWHermMat<TypeT>&);

/**
 * @relates RWHermMat
 *
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
inline RWHermMat<TypeT>  operator*(const RWHermMat<TypeT>& A, TypeT x)
{
    RWMathVec<TypeT> temp(A.dataVec()*x);
    return sameShapeMatrix(temp, A);
}

#ifndef RW_NO_INLINED_TEMP_DESTRUCTORS
/**
 * @relates RWHermMat
 *
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
inline RWHermMat<TypeT>  operator*(TypeT x, const RWHermMat<TypeT>& A)
{
    return A * x;
}
#else
template <class TypeT>
RWHermMat<TypeT>  operator*(TypeT x, const RWHermMat<TypeT>& A);
#endif

/**
 * @relates RWHermMat
 *
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
RWHermMat<TypeT>  operator/(const RWHermMat<TypeT>& A, TypeT x);

/**
 * @relates RWHermMat
 *
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
RWHermMat<TypeT>  operator/(TypeT x, const RWHermMat<TypeT>& A);

#if defined(_MSC_VER)
template <class TypeT>
inline  std::ostream& operator<<(std::ostream& s, const RWHermMat<TypeT>& m)
{
    m.printOn(s);
    return s;
}
template <class TypeT>
inline  std::istream& operator>>(std::istream& s, RWHermMat<TypeT>& m)
{
    m.scanFrom(s);
    return s;
}
#else
/**
 * @relates RWHermMat
 *
 * Writes the matrix to the stream. This is equivalent to calling
 * the printOn() member function.
 */
template <class TypeT>
inline std::ostream&  operator<<(std::ostream& s, const RWHermMat<TypeT>& m)
{
    m.printOn(s);
    return s;
}

/**
 * @relates RWHermMat
 *
 * Reads the matrix from the stream. This is equivalent to calling
 * the scanFrom() member function.
 */
template <class TypeT>
inline std::istream&  operator>>(std::istream& s, RWHermMat<TypeT>& m)
{
    m.scanFrom(s);
    return s;
}
#endif


/**
 * @relates RWHermMat
 *
 * Returns the transpose of the argument matrix.
 */
template <class TypeT>
RWHermMat<TypeT>  transpose(const RWHermMat<TypeT>&);

/**
 * @relates RWHermMat
 *
 * Returns the inner product (matrix-vector product) of \a A and
 * \a x.
 */
template <class TypeT>
RWMathVec<TypeT>  product(const RWHermMat<TypeT>& A, const RWMathVec<TypeT>& x);

/**
 * @relates RWHermMat
 *
 * Returns the inner product (matrix-vector product) of \a x and
 * \a A. This is equal to the product of \a A transpose and \a x.
 */
template <class TypeT>
RWMathVec<TypeT>  product(const RWMathVec<TypeT>& x, const RWHermMat<TypeT>& A);
// inner products

// Below are functions that assume automatic conversion to GenMat's
// Automatic conversions do not take place for templatized functions,
// So we provide them here...
template <class TypeT>
RWGenMat<TypeT>  product(const RWHermMat<TypeT>& A, const RWGenMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  product(const RWGenMat<TypeT>& A, const RWHermMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  product(const RWHermMat<TypeT>& A, const RWHermMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWHermMat<TypeT>& A, const RWGenMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWGenMat<TypeT>& A, const RWHermMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWHermMat<TypeT>& A, const RWHermMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  conjTransposeProduct(const RWHermMat<TypeT>& A, const RWGenMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  conjTransposeProduct(const RWGenMat<TypeT>& A, const RWHermMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  conjTransposeProduct(const RWHermMat<TypeT>& A, const RWHermMat<TypeT>& B);

template <class TypeT>
inline RWGenMat<TypeT>  adjoint(const RWHermMat<TypeT>& A)
{
    RWGenMat<TypeT> a(A);
    return adjoint(a);
}

template <class TypeT>
RWHermMat<TypeT>  toHermMat(const RWGenMat<TypeT>& A);

/**
 * @relates RWHermMat
 *
 * Builds a Hermitian matrix that matches the upper triangular part
 * of \a A. The lower triangle of \a A is <i>not</i> referenced.
 */
template <class TypeT>
RWHermMat<TypeT>  upperToHermMat(const RWGenMat<TypeT>& A);

/**
 * @relates RWHermMat
 *
 * Builds a Hermitian matrix that matches the lower triangular part
 * of \a A. The upper triangle of \a A is <i>not</i> referenced.
 */
template <class TypeT>
RWHermMat<TypeT>  lowerToHermMat(const RWGenMat<TypeT>& A);

/**
 * @relates RWHermMat
 *
 * Returns a matrix whose entries are the absolute value of the
 * argument. The absolute value of a complex number is considered
 * to be the sum of the absolute values of its real and imaginary
 * parts. To get the norm of a complex matrix, you can use the
 * norm() function.
 */
template <class TypeT>
RWSymMat<double>  abs(const RWHermMat<TypeT>& A);

/**
 * @relates RWHermMat
 *
 * Returns a matrix where each element is the complex conjugate
 * of the corresponding element in the matrix \a A.
 */
template <class TypeT>
RWHermMat<TypeT>  conj(const RWHermMat<TypeT>& A);

/**
 * @relates RWHermMat
 *
 * Returns a matrix where each element is the real part of the corresponding
 * element in the matrix \a A.
 */
template <class TypeT>
RWSymMat<double>  real(const RWHermMat<TypeT>& A);

/**
 * @relates RWHermMat
 *
 * Returns a matrix where each element is the imaginary part of
 * the corresponding element in the matrix \a A.
 */
template <class TypeT>
RWSkewMat<double>  imag(const RWHermMat<TypeT>& A);

/**
 * @relates RWHermMat
 *
 * Returns a matrix where each element is the norm (magnitude) of
 * the corresponding element in the matrix \a A.
 */
template <class TypeT>
RWSymMat<double>  norm(const RWHermMat<TypeT>& A);

/**
 * @relates RWHermMat
 *
 * Returns a matrix where each element is the argument of the corresponding
 * element in the matrix \a A.
 */
template <class TypeT>
RWSkewMat<double>  arg(const RWHermMat<TypeT>& A);



/*
 * Inline functions
 */

template <class TypeT>
inline TypeT RWHermMat<TypeT>::val(int i, int j) const
{
#ifdef RWBOUNDS_CHECK
    return bcval(i, j);
#else
    int index = (i <= j) ? (j * (j + 1) / 2 + i) : (i * (i + 1) / 2 + j);
    TypeT val = vec(index);
    return (i <= j) ?  val : RW_MATH_SL_STD_GLOBAL(conj)(val);
#endif
}

template <class TypeT>
inline TypeT RWHermMat<TypeT>::set(int i, int j, TypeT x)
{
    return bcset(i, j, x);
}

template <class TypeT>
inline RWCJRef<TypeT> RWHermMat<TypeT>::ref(int i, int j)
{
    return bcref(i, j);
}

template <class TypeT>
inline RWCJRef<TypeT> RWHermMat<TypeT>::operator()(int i, int j)
{
    return ref(i, j);
}
template <class TypeT>
inline TypeT RWHermMat<TypeT>::operator()(int i, int j) const
{
    return val(i, j);
}

#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/hermmat.cc>
#endif

#endif
