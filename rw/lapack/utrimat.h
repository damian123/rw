#ifndef __RWUTRIMAT_H__
#define __RWUTRIMAT_H__

/*
 * $Id: //lapack/13/rw/lapack/utrimat.h#1 $
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
#include <rw/lapack/latraits.h>

template <class TypeT>
class RWGenMat;
template <class TypeT>
class RWLowerTriMat;

/**
 * @ingroup sparse_matrix_classes
 *
 * @brief Encapsulates upper triangular matrices, which are \c 0 above
 * the diagonal.
 *
 * The class \link RWUpperTriMat RWUpperTriMat<T> \endlink encapsulates
 * upper triangular matrices. An upper triangular matrix is 0 above
 * the diagonal.
 *
 * @section RWUpperTriMat_synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/utrimat.h>
 *
 * RWUpperTriMat<double>;
 * @endcode
 *
 * @section RWUpperTriMat_example Example
 *
 * @code
 * #include <rw/lapack/utrimat.h>
 *
 * int main()
 * {
 *     RWUpperTriMat<double> U(5,5);
 *     RWLowerTriMat<double> L = transpose(U);
 *
 *     return 0;
 * }
 * @endcode
 *
 * @section RWUpperTriMat_storage_scheme Storage Scheme
 *
 * The matrix is stored column-by-column. For example, the matrix:
 *
 * \f[
 * \begin{bmatrix}
 * A_{11} & A_{12} & A_{13} & ...A_{1n} \\
 * 0      & A_{22} & A_{23} & ...A_{2n} \\
 * 0      & 0      & A_{33} & ...A_{3n} \\
 *        &        & .      &           \\
 *        &        & .      &           \\
 *        &        & .      &           \\
 * 0      & 0      & 0      & ...A_{nn}
 * \end{bmatrix}
 * \f]
 *
 * is stored as follows:
 *
 * <tt>[ A<sub>11</sub>
 *       A<sub>12</sub> A<sub>22</sub>
 *       A<sub>13</sub> A<sub>23</sub> A<sub>33</sub>
 *       ...
 *       A<sub>1n</sub> A<sub>2n</sub> A<sub>3n</sub>
 *                                 ... A<sub>nn</sub> ]
 * </tt>
 *
 * The mapping between the array and storage vector is as follows:
 *
 * \f[
 * A(i+1,j+1) \rightarrow vec \left [\frac{j(j+1)}{2+i} \right ]
 * \f]
 */
template <class TypeT>
class RWUpperTriMat
{
private:

    // The data which define the matrix
    RWMathVec<TypeT> vec;
    unsigned n;

    // The zero that is returned for the value of non-stored zero entries
    static TypeT zero_;

public:

    /**
     * Default constructor. Builds a matrix of size 0 x 0. This constructor
     * is necessary to declare a matrix with no explicit constructor
     * or to declare an array of matrices.
     */
    RWUpperTriMat();

    /**
     * Builds a copy of its argument, \a rhs. Note that the new matrix
     * references the data of \a rhs. To construct a matrix with its own copy
     * of the data, you can use copy() or deepenShallowCopy().
     */
    RWUpperTriMat(const RWUpperTriMat<TypeT>& rhs);

    /**
     * Defines an uninitialized matrix of size \a n x \a n.
     *
     * @note This constructor is used, rather than a constructor that takes
     * only a single argument, to avoid type conversion problems. Both
     * arguments must be equal or a runtime error occurs.
     */
    RWUpperTriMat(unsigned n, unsigned nAgain);

    /**
     * Constructs a size \a n x \a n matrix using the data in the passed
     * vector. This data must be stored in the format described in the
     * \ref RWUpperTriMat_storage_scheme section. The resultant
     * matrix references the data in vector \a data.
     *
     * @note This constructor is used, rather than a constructor that takes
     * only a single argument, to avoid type conversion problems. The last
     * two arguments must be equal or a runtime error occurs.
     */
    RWUpperTriMat(const RWMathVec<TypeT>& data, unsigned n, unsigned nAgain);

    /**
     * Constructs a complex matrix from the real part
     * supplied.  The imaginary part is assumed to be 0.
     */
    RW_LAPK_MEMBER_EXPORT RWUpperTriMat(const typename rw_linear_algebra_traits<TypeT>::narrow_upper_tri_mat& re);

    /**
     * Constructs a complex matrix from the real and imaginary parts
     * supplied.
     */
    RW_LAPK_MEMBER_EXPORT RWUpperTriMat(const RWUpperTriMat<double>& re, const RWUpperTriMat<double>& im);

    /**
     * Destructor.
     */
    ~RWUpperTriMat<TypeT>();

    static const char* className; // For use in error messages

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
     * Returns the value of the \a ij <sup>th</sup> element of the matrix.
     * Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined when the header file is read. The member function
     * bcval() does the same thing with guaranteed bounds checking.
     */
    inline TypeT  val(int i, int j) const;

    /**
     * Sets the \a ij <sup>th</sup>element of the matrix equal to \a x.
     * Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined when the header file is read. The member function
     * bcset() does the same thing with guaranteed bounds checking.
     */
    inline TypeT  set(int i, int j, TypeT x);

    /**
     * Accesses the \a ij <SUP>th</SUP> element. A reference type is returned,
     * so this operator can be used for assigning or accessing an element. In
     * this case, using this operator is equivalent to calling the ref() member
     * function. Bounds checking is done if the preprocessor symbol
     * \c RWBOUNDS_CHECK is defined before including the header file.
     */
    inline RWRORef<TypeT> operator()(int i, int j);

    /**
     * Accesses the \a ij <SUP>th</SUP> element. A value is returned, so this
     * operator can be used only for accessing an element. Using this operator
     * is equivalent to calling the val() member function. Bounds checking is
     * done if the preprocessor symbol \c RWBOUNDS_CHECK is defined before
     * including the header file.
     */
    inline TypeT  operator()(int i, int j) const; // { return val(i,j); }

    /**
     * Returns a reference to the \a ij <sup>th</sup> element of the matrix.
     * Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined when the header file is read. The member function bcref()
     * does the same thing with guaranteed bounds checking.
     */
    inline RWRORef<TypeT> ref(int i, int j);

    /**
     * Returns the value of the \a ij <sup>th</sup> element of the matrix,
     * after doing bounds checking.
     */
    TypeT  bcval(int i, int j) const;

    /**
     * Sets the \a ij <sup>th</sup> element of the matrix equal to \a x,
     * after doing bounds checking.
     */
    TypeT  bcset(int i, int j, TypeT x);

    /**
     * Returns a reference to the \a ij <sup>th</sup> element of the
     * matrix, after doing bounds checking.
     */
    RWRORef<TypeT> bcref(int i, int j);

    /**
     * Sets every element of the matrix to 0.
     */
    void zero() {
        vec = TypeT(0);
    }

    /**
     * Returns the \a k x \a k upper left corner of the matrix. The
     * submatrix and the matrix share the same data.
     */
    RWUpperTriMat<TypeT> leadingSubmatrix(int k);

    /**
     * Makes this matrix a reference to the matrix \a m. The two
     * matrices share the same data. The matrices do <i>not</i> have
     * to be the same size before calling reference(). To copy a matrix
     * into another of the same size, use operator=().
     */
    RWUpperTriMat<TypeT>& reference(RWUpperTriMat<TypeT>& m);

    /**
     * Creates a copy of this matrix with distinct data. The stride
     * of the data vector in the new matrix is guaranteed to be 1.
     */
    RWUpperTriMat<TypeT> copy() const;

    /**
     * Creates a copy of this matrix with distinct data. The stride
     * of the data vector in the new matrix is guaranteed to be 1.
     */
    RWUpperTriMat<TypeT> deepCopy() const {
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

    /**
     * @copydoc dataVec()
     */
    const RWMathVec<TypeT>& dataVec() const {
        return vec;
    }

    /**
     * Returns a pointer to the first item of data in the vector storing
     * the matrix's data. You can use this (with caution!) to pass the
     * matrix's data to C or FORTRAN subroutines. Be aware that the stride
     * of the data vector may <i>not</i> be 1.
     */
    TypeT* data() {
        return vec.data();
    }

    /**
     * @copydoc data()
     */
    const TypeT* data() const {
        return vec.data();
    }

    /**
     * Resizes the matrix. Any new entries in the matrix are set to
     * 0. Both arguments must be the same.
     */
    void resize(unsigned m, unsigned n);


    /**
     * Reads a matrix from an input stream. The format of the matrix
     * is the same as the format output by printOn(). Below is a sample
     * matrix that could be input. Note that extra white space and any
     * text preceding the dimension specification are ignored. Only the
     * upper triangle of the matrix is used.
     *
     * @code
     * 4x4
     * [
     *  4    3    3    9
     *  0    9   -1    5
     *  0    0    3    1
     *  0    0    0   -2
     * ]
     * @endcode
     */
    void scanFrom(std::istream& is);                // Human readable

    /**
     * Prints the matrix to an output stream in human readable format.
     */
    void printOn(std::ostream& os) const;

    /**
     * Reads in a matrix from an RWvistream, the Rogue Wave virtual
     * input stream class. The matrix must have been stored to the stream
     * using the \link saveOn(RWvostream&)const saveOn() \endlink
     * member function.
     */
    void restoreFrom(RWvistream& is);     // Internal US-ASCII or binary code

    /**
     * Stores a matrix to an RWvostream, the Rogue Wave virtual output
     * stream class. The matrix can be read using
     * \link restoreFrom(RWvistream&) restoreFrom() \endlink.
     */
    void saveOn(RWvostream& os) const;

    /**
     * Reads in a matrix from an RWFile. The matrix must have been stored
     * to the file using the \link saveOn(RWFile&)const saveOn() \endlink
     * member function.
     */
    void restoreFrom(RWFile& f);

    /**
     * Stores a matrix to an RWFile. The matrix can be read using the
     * \link restoreFrom(RWFile&) restoreFrom() \endlink member function.
     */
    void saveOn(RWFile& f) const;

    /**
     * Returns the number of bytes that it would take to write the matrix
     * to a file using saveOn().
     */
    unsigned binaryStoreSize() const;

    /**
     * @internal
     * Converts RWUpperTriMat into an RWGenMat.
     */
    operator RWGenMat<TypeT>() const;

    // Two matrices are considered equal if their shapes are identical and
    // all their components are identical.
    /**
     * Equality operator. Returns \c true if two matrices have the same
     * size and their elements are all exactly the same. Otherwise this
     * function returns \c false. Be aware that floating point arithmetic
     * is <i>not</i> exact; matrices that are theoretically equal are
     * <i>not</i> always numerically equal.
     */
    bool operator==(const RWUpperTriMat<TypeT>& X);

    /**
     * Inequality operator. Returns \c false if two matrices have the same
     * size and their elements are all exactly the same. Otherwise this
     * function returns \c true. Be aware that floating point arithmetic
     * is <i>not</i> exact; matrices that are theoretically equal are
     * <i>not</i> always numerically equal.
     */
    bool operator!=(const RWUpperTriMat<TypeT>& X) {
        return !(operator==(X));
    }

    /**
     * Sets the matrix elements equal to the elements of \a A. The two
     * matrices must be the same size. To make the matrix reference
     * the same data as \a A, use reference().
     */
    RWUpperTriMat<TypeT>& operator=(const RWUpperTriMat<TypeT>& A);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     */
    RWUpperTriMat<TypeT>& operator+=(const RWUpperTriMat<TypeT>& m);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     */
    RWUpperTriMat<TypeT>& operator-=(const RWUpperTriMat<TypeT>& m);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     * This function does element-by-element multiplication, <i>not</i>
     * inner product style matrix multiplication. You can use the product()
     * global function to do matrix-matrix inner product multiplication.
     */
    RWUpperTriMat<TypeT>& operator*=(const RWUpperTriMat<TypeT>& m);

    /**
     * Performs the indicated operation on each element of the matrix.
     */
    RWUpperTriMat<TypeT>& operator*=(TypeT v);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     */
    RWUpperTriMat<TypeT>& operator/=(const RWUpperTriMat<TypeT>& m);

    /**
     * Performs the indicated operation on each element of the matrix.
     */
    RWUpperTriMat<TypeT>& operator/=(TypeT v);
};

/****************************************************************************
 *
 * RWUpperTriMat<TypeT> Inline functions
 *
 ****************************************************************************/
template <class TypeT>
inline TypeT RWUpperTriMat<TypeT>::val(int i, int j) const
{
#ifdef RWBOUNDS_CHECK
    return bcval(i, j);
#else
    return (j < i) ? TypeT(0) : vec(j * (j + 1) / 2 + i);
#endif
}

template <class TypeT>
inline TypeT RWUpperTriMat<TypeT>::set(int i, int j, TypeT x)
{
#ifdef RWBOUNDS_CHECK
    return bcset(i, j, x);
#else
    return vec(j * (j + 1) / 2 + i) = x;
#endif
}

template <class TypeT>
inline RWRORef<TypeT> RWUpperTriMat<TypeT>::ref(int i, int j)
{
    return bcref(i, j);
}

template <class TypeT>
inline RWRORef<TypeT> RWUpperTriMat<TypeT>::operator()(int i, int j)
{
    return ref(i, j);
}

template <class TypeT>
inline TypeT RWUpperTriMat<TypeT>::operator()(int i, int j) const
{
    return val(i, j);
}


/****************************************************************************
 *
 * Global Functions
 *
 ***************************************************************************/

/**
 * @internal
 * This function returns a new matrix the same shape as the one
 * passed in but with the data passed in.  This way we restrict
 * all the >if statements to a couple lines
 */
template <class TypeT>
inline RWUpperTriMat<TypeT> sameShapeMatrix(RWMathVec<TypeT>& vec, const RWUpperTriMat<TypeT>& A)
{
    return RWUpperTriMat<TypeT>(vec, A.rows(), A.cols());
}

/**
 * @internal
 * This function returns a new matrix the same shape as the one
 * passed in but with the data passed in.  This way we restrict
 * all the >if statements to a couple lines
 */
inline RWUpperTriMat<double> sameShapeRealMatrix(RWMathVec<double>& vec, const RWUpperTriMat<DComplex>& A)
{
    return RWUpperTriMat<double>(vec, A.rows(), A.cols());
}

/**
 * @relates RWUpperTriMat
 *
 * Unary minus operator. Returns the negation of the matrix \a m.
 */
template <class TypeT>
RWUpperTriMat<TypeT>  operator-(const RWUpperTriMat<TypeT>& m);        // Unary minus

/**
 * @relates RWUpperTriMat
 *
 * Unary plus operator. Returns a copy of the matrix \a m.
 */
template <class TypeT>
RWUpperTriMat<TypeT>  operator+(const RWUpperTriMat<TypeT>& m);        // Unary plus

/**
 * @relates RWUpperTriMat
 *
 * Performs element-by-element operations on the arguments. To do
 * inner product matrix multiplication, you can use the product()
 * global function.
 */
template <class TypeT>
RWUpperTriMat<TypeT>  operator*(const RWUpperTriMat<TypeT>&, const RWUpperTriMat<TypeT>&);

/**
 * @relates RWUpperTriMat
 *
 * Performs element-by-element division on the arguments.
 */
template <class TypeT>
RWUpperTriMat<TypeT>  operator/(const RWUpperTriMat<TypeT>&, const RWUpperTriMat<TypeT>&);

/**
 * @relates RWUpperTriMat
 *
 * Performs element-by-element addition on the arguments.
 */
template <class TypeT>
RWUpperTriMat<TypeT>  operator+(const RWUpperTriMat<TypeT>&, const RWUpperTriMat<TypeT>&);

/**
 * @relates RWUpperTriMat
 *
 * Performs element-by-element subtraction on the arguments.
 */
template <class TypeT>
RWUpperTriMat<TypeT>  operator-(const RWUpperTriMat<TypeT>&, const RWUpperTriMat<TypeT>&);

/**
 * @relates RWUpperTriMat
 *
 * Performs element-by-element multiplication on the arguments.
 */
template <class TypeT>
inline RWUpperTriMat<TypeT>  operator*(const RWUpperTriMat<TypeT>& A, TypeT x)
{
    RWMathVec<TypeT> temp(A.dataVec()*x);
    return sameShapeMatrix(temp, A);
}

#ifndef RW_NO_INLINED_TEMP_DESTRUCTORS

/**
 * @relates RWUpperTriMat
 *
 * Performs element-by-element multiplication on the arguments.
 */
template <class TypeT>
inline RWUpperTriMat<TypeT>  operator*(TypeT x, const RWUpperTriMat<TypeT>& A)
{
    return A * x;
}

#else

template <class TypeT>
RWUpperTriMat<TypeT>  operator*(TypeT x, const RWUpperTriMat<TypeT>& A);

#endif

/**
 * @relates RWUpperTriMat
 *
 * Performs element-by-element division on the arguments.
 */
template <class TypeT>
RWUpperTriMat<TypeT>  operator/(const RWUpperTriMat<TypeT>& A, TypeT x);


/**
 * @relates RWUpperTriMat
 *
 * Writes the matrix to the stream. This is equivalent to calling the
 * printOn() member function.
 */
template <class TypeT>
inline std::ostream&  operator<<(std::ostream& s, const RWUpperTriMat<TypeT>& m)
{
    m.printOn(s);
    return s;
}

/**
 * @relates RWUpperTriMat
 *
 * Reads the matrix from the stream. This is equivalent to calling the
 * scanFrom() member function.
 */
template <class TypeT>
inline std::istream&  operator>>(std::istream& s, RWUpperTriMat<TypeT>& m)
{
    m.scanFrom(s);
    return s;
}


/**
 * @relates RWUpperTriMat
 *
 * Returns the transpose of the argument matrix. The transpose is
 * made to reference the same data as the argument matrix.
 */
template <class TypeT>
RWLowerTriMat<TypeT>  transpose(const RWUpperTriMat<TypeT>&);


/**
 * @relates RWUpperTriMat
 *
 * Returns the inner product (matrix-vector product) of \a A and
 * \a x.
 */
template <class TypeT>
RWMathVec<TypeT>  product(const RWUpperTriMat<TypeT>& A, const RWMathVec<TypeT>& x);


/**
 * @relates RWUpperTriMat
 *
 * Returns the inner product (matrix-vector product) of \a x and
 * \a A. This is equal to the product of \a A transpose and \a x.
 */
template <class TypeT>
RWMathVec<TypeT>  product(const RWMathVec<TypeT>& x, const RWUpperTriMat<TypeT>& A);

/**
 * @internal
 * Converts RWUpperTriMat into GenMat and then forward the call. This assumes
 * automatic conversion to GenMat do not take place for templatized functions.
 */
template <class TypeT>
RWGenMat<TypeT>  product(const RWUpperTriMat<TypeT>& A, const RWGenMat<TypeT>& B);

/**
 * @internal
 * Converts RWUpperTriMat into GenMat and then forward the call. This assumes
 * automatic conversion to GenMat do not take place for templatized functions.
 */
template <class TypeT>
RWGenMat<TypeT>  product(const RWGenMat<TypeT>& A, const RWUpperTriMat<TypeT>& B);

/**
 * @internal
 * Converts RWUpperTriMat into GenMat and then forward the call. This assumes
 * automatic conversion to GenMat do not take place for templatized functions.
 */
template <class TypeT>
RWGenMat<TypeT>  product(const RWUpperTriMat<TypeT>& A, const RWUpperTriMat<TypeT>& B);

/**
 * @internal
 * Converts RWUpperTriMat into GenMat and then forward the call. This assumes
 * automatic conversion to GenMat do not take place for templatized functions.
 */
template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWUpperTriMat<TypeT>& A, const RWGenMat<TypeT>& B);

/**
 * @internal
 * Converts RWUpperTriMat into GenMat and then forward the call. This assumes
 * automatic conversion to GenMat do not take place for templatized functions.
 */
template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWGenMat<TypeT>& A, const RWUpperTriMat<TypeT>& B);

/**
 * @internal
 * Converts RWUpperTriMat into GenMat and then forward the call. This assumes
 * automatic conversion to GenMat do not take place for templatized functions.
 */
template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWUpperTriMat<TypeT>& A, const RWUpperTriMat<TypeT>& B);

/**
 * @internal
 * Converts RWUpperTriMat into GenMat and then forward the call. This assumes
 * automatic conversion to GenMat do not take place for templatized functions.
 */
template <class TypeT>
inline RWGenMat<TypeT>  adjoint(const RWUpperTriMat<TypeT>& A)
{
    RWGenMat<TypeT> a(A);
    return adjoint(a);
}

/**
 * @relates RWUpperTriMat
 *
 * Extracts the upper triangular part of a square matrix.
 */
template <class TypeT>
RWUpperTriMat<TypeT>  toUpperTriMat(const RWGenMat<TypeT>& A);

#if !defined(_MSC_VER) && !defined(DOXYGEN)

template <class TypeT>
inline RWUpperTriMat<typename rw_numeric_traits<TypeT>::norm_type>  abs(const RWUpperTriMat<TypeT>& A)
{
    RWMathVec<typename rw_numeric_traits<TypeT>::norm_type> temp(abs(A.dataVec()));
    return sameShapeMatrix(temp, A);
}
//specialize FOR DComplex:
#  if !defined(__hppa)
template <>
#  endif
inline RWUpperTriMat<rw_numeric_traits<DComplex>::norm_type>  abs(const RWUpperTriMat<DComplex>& A)
{
    RWMathVec<rw_numeric_traits<DComplex>::norm_type> temp(abs(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

#else //using MSVC compiler or doxygen

/**
 * @relates RWUpperTriMat
 *
 * Returns a matrix whose entries are the absolute value of the
 * argument. The absolute value of a complex number is considered
 * to be the sum of the absolute values of its real and imaginary
 * parts. To get the norm of a complex matrix, you can use the
 * norm() function.
 */
template <class TypeT>
inline RWUpperTriMat<typename rw_numeric_traits<TypeT>::norm_type>  abs(const RWUpperTriMat<TypeT>& A)
{
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;
    RWMathVec<rt> temp(abs(A.dataVec()));
    //return sameShapeMatrix(temp,M);
    //this function should call sameShape{Real}Matrix, and return the result of that function.
    // I'm putting code directly from those functions here -- MSVC5 cannot handle complicated template specializations -- till a better solution comes up
    return RWUpperTriMat<rt>(temp, A.rows(), A.cols());
};

#endif // !defined(_MSC_VER)

/**
 * @relates RWUpperTriMat
 *
 * Returns a matrix where each element is the complex conjugate
 * of the corresponding element in the matrix \a A.
 */
template< class TypeT >
inline RWUpperTriMat<TypeT>  conj(const RWUpperTriMat<TypeT>& A)
{
    RWMathVec<TypeT> temp(conj(A.dataVec()));
    return sameShapeMatrix(temp, A);
}

/**
 * @relates RWUpperTriMat
 *
 * Returns a matrix where each element is the real part of the corresponding
 * element in the matrix \a A.
 */
inline RWUpperTriMat<double>  real(const RWUpperTriMat<DComplex>& A)
{
    RWMathVec<double> temp(real(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWUpperTriMat
 *
 * Returns a matrix where each element is the imaginary part of
 * the corresponding element in the matrix \a A.
 */
inline RWUpperTriMat<double>  imag(const RWUpperTriMat<DComplex>& A)
{
    RWMathVec<double> temp(imag(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWUpperTriMat
 *
 * Returns a matrix where each element is the norm (magnitude) of
 * the corresponding element in the matrix \a A.
 */
inline RWUpperTriMat<double>  norm(const RWUpperTriMat<DComplex>& A)
{
    RWMathVec<double> temp(norm(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWUpperTriMat
 *
 * Returns a matrix where each element is the argument of the corresponding
 * element in the matrix \a A.
 */
inline RWUpperTriMat<double>  arg(const RWUpperTriMat<DComplex>& A)
{
    RWMathVec<double> temp(arg(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @internal
 * Converts RWUpperTriMat into GenMat and then forward the call. This assumes
 * automatic conversion to GenMat do not take place for templatized functions.
 */
inline RWGenMat<DComplex>  conjTransposeProduct(const RWUpperTriMat<DComplex>& A, const RWGenMat<DComplex>& B)
{
    return conjTransposeProduct(RWGenMat<DComplex>(A), B);
}

/**
 * @internal
 * Converts RWUpperTriMat into GenMat and then forward the call. This assumes
 * automatic conversion to GenMat do not take place for templatized functions.
 */
inline RWGenMat<DComplex>  conjTransposeProduct(const RWGenMat<DComplex>& A, const RWUpperTriMat<DComplex>& B)
{
    return conjTransposeProduct(A, RWGenMat<DComplex>(B));
}

/**
 * @internal
 * Converts RWUpperTriMat into GenMat and then forward the call. This assumes
 * automatic conversion to GenMat do not take place for templatized functions.
 */
inline RWGenMat<DComplex>  conjTransposeProduct(const RWUpperTriMat<DComplex>& A, const RWUpperTriMat<DComplex>& B)
{
    return conjTransposeProduct(RWGenMat<DComplex>(A), RWGenMat<DComplex>(B));
}

/**
 * @relates RWUpperTriMat
 *
 * Returns the maximum entry in the matrix.
 */
inline double  minValue(const RWUpperTriMat<double>& A)
{
    return minValue(A.dataVec());
}

/**
 * @relates RWUpperTriMat
 *
 * Returns the maximum entry in the matrix.
 */
inline double  maxValue(const RWUpperTriMat<double>& A)
{
    return maxValue(A.dataVec());
}

/**
 * @relates RWUpperTriMat
 *
 * @copydoc minValue(const RWUpperTriMat<double>&)
 */
inline float  minValue(const RWUpperTriMat<float>& A)
{
    return minValue(A.dataVec());
}

/**
 * @relates RWUpperTriMat
 *
 * @copydoc maxValue(const RWUpperTriMat<double>&)
 */
inline float  maxValue(const RWUpperTriMat<float>& A)
{
    return maxValue(A.dataVec());
}




#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/utrimat.cc>
#endif

#endif
