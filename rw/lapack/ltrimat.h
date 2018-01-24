
#ifndef __RWLTRIMAT_H__
#define __RWLTRIMAT_H__
/*
 * RWLowerTriMat - A lower triangular matrix of doubles
 *
 * Stores a lower triangular matrix.
 *
 * The matrix is stored in row major order.  This way the vector
 * of data is exactly the same as the matrix's transpose (an UpperTriMat)
 *
 * Bounds checking is enabled by setting the preprocessor
 * symbol RWBOUNDS_CHECK.  This is strongly encouraged.
 *
 ***************************************************************************
 * $Id: //lapack/13/rw/lapack/ltrimat.h#1 $
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
#include <rw/lapack/latraits.h>

template <class TypeT>
class RWGenMat;

template <class TypeT>
class RWUpperTriMat;


/**
 * @ingroup sparse_matrix_classes
 *
 * @brief Encapsulates lower triangular matrices, which are zero
 * above the diagonal.
 *
 * A lower triangular matrix is 0 above the diagonal. The class
 *  \link RWLowerTriMat RWLowerTriMat<T> \endlink encapsulates lower
 * triangular matrices.
 *
 * @section RWLowerTriMat_synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/ltrimat.h>
 *
 * RWLowerTriMat<double> d;
 * @endcode

 * @section RWLowerTriMat_example Example
 *
 * @code
 * #include <rw/lapack/ltrimat.h>
 *
 * int main()
 * {
 *     RWLowerTriMat<DComplex> L(5,5);
 *     L.leadingSubmatrix(2).zero();
 *     return 0;
 * }
 * @endcode
 *
 * @section RWLowerTriMat_storage_scheme Storage Scheme
 *
 * The matrix is stored row by row. For example, the matrix:
 *
 * \f[
 * \begin{bmatrix}
 * A_{11} & 0      & 0      & ... & 0      \\
 * A_{12} & A_{22} & 0      & ... & 0      \\
 * A_{13} & A_{23} & A_{33} & ... & 0      \\
 * .      &        &        &     &        \\
 * .      &        &        &     &        \\
 * .      &        &        &     &        \\
 * A_{1n} & A_{2n} & A_{3n} & ... & A_{nn} \\
 * \end{bmatrix}
 * \f]
 *
 * is stored as follows:
 *
 * <tt>[ A<sub>11</sub> A<sub>12</sub> A<sub>22</sub> A<sub>13</sub>
 * A<sub>23</sub> A<sub>33</sub> ... A<sub>1n</sub> A<sub>2n</sub> A<sub>3n</sub>
 * ... A<sub>nn</sub> ]</tt>
 *
 * The mapping between the array and storage vector is as follows:
 *
 * \f[
 * A(i+1,j+1) \to vec[i(i+1)/2+j]
 * \f]
 *
 */
template <class TypeT>
class RWLowerTriMat
{
private:
    // Private Data ------------------------------------------------------------
    // The data which define the matrix
    RWMathVec<TypeT> vec;
    unsigned n;

    static TypeT zero_; // represents the zero matrix elements

public:

    // Public Constructors ----------------------------------------------------

    /**
     * Default constructor. Builds a matrix of size 0 x 0. This constructor
     * is necessary to declare a matrix with no explicit constructor
     * or to declare an array of matrices.
     */
    RWLowerTriMat();

    /**
     * Builds a copy of its argument, \a A. Note that the new matrix
     * references \a A's data. To construct a matrix with its own copy
     * of the data, use either the copy() or deepenShallowCopy() member
     * functions.
     */
    RWLowerTriMat(const RWLowerTriMat<TypeT>& A);

    /**
     * Defines an uninitialized matrix of size \a n x \a nAgain. Both arguments
     * must be equal or a runtime error occurs. This constructor is
     * used, rather than a constructor that takes only a single argument,
     * to avoid type conversion problems.
     */
    RWLowerTriMat(unsigned n, unsigned nAgain);

    /**
     * Constructs a size \a n x \a nAgain matrix using the data in the passed
     * vector. This data must be stored in the format described in the
     * \ref RWLowerTriMat_storage_scheme section. The resultant
     * matrix references the data in vector \a vd.
     */
    RWLowerTriMat(const RWMathVec<TypeT>& vd, unsigned n, unsigned nAgain);

    /**
     * Constructs a complex matrix from the real part
     * supplied.  The imaginary part is assumed to be 0.
     */
    RW_LAPK_MEMBER_EXPORT RWLowerTriMat(const typename rw_linear_algebra_traits<TypeT>::narrow_lower_tri_mat& re);

    /**
     * Constructs a complex matrix from the real and imaginary parts
     * supplied. If no imaginary part is supplied, it is assumed to
     * be 0.
     */
    RW_LAPK_MEMBER_EXPORT RWLowerTriMat(const RWLowerTriMat<double>& re, const RWLowerTriMat<double>& im);


    // Destructor ---------------------------------------------------------------
    ~RWLowerTriMat();

    // Public Data Members ------------------------------------------------------
    // For use in error messages
    static const char* className;


    // Public Methods -----------------------------------------------------------

    // Member functions to access the shape of the matrix
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


    // Access and/or change elements of the matrix.
    // The versions that begin with the letters bc do bounds
    // checking.  The others do bounds checking only if the
    // preprocessor symbol RWBOUNDS_CHECK is defined.

    /**
     * Returns a reference to the \a ij<SUP>th</SUP> element of the
     * matrix. Bounds checking is done if the preprocessor symbol \c
     * RWBOUNDS_CHECK is defined when the header file is read. The member
     * function bcref() does the same thing with guaranteed bounds
     * checking.
     */
    inline RWRORef<TypeT> ref(int i, int j);

    /**
     * Returns the value of the \a ij<SUP>th</SUP> element of the matrix.
     * Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined when the header file is read. The member function
     * bcval() does the same thing with guaranteed bounds checking.
     */
    inline TypeT  val(int i, int j) const;

    /**
     * Sets the \a ij<SUP>th</SUP> element of the matrix equal to \a
     * x. Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined when the header file is read. The member function
     * bcset() does the same thing with guaranteed bounds checking.
     */
    inline TypeT  set(int i, int j, TypeT x);

    /**
     * Returns the value of the \a ij<SUP>th</SUP> element of the matrix,
     * after doing bounds checking.
     */
    TypeT  bcval(int i, int j) const;

    /**
     * Sets the \a ij<SUP>th</SUP> element of the matrix equal to \a
     * x, after doing bounds checking.
     */
    TypeT  bcset(int i, int j, TypeT x);

    /**
     * Returns a reference to the \a ij<SUP>th</SUP> element of the
     * matrix, after doing bounds checking.
     */
    RWRORef<TypeT> bcref(int i, int j);


    // Access to submatrices.
    // The leading submatrix of order k is the kxk matrix in the upper
    // left.
    /**
     * Returns the \a k x \a k upper left corner of the matrix. The
     * submatrix and the matrix share the same data.
     */
    RWLowerTriMat<TypeT> leadingSubmatrix(int k);

    // reference() makes an alias, operator= makes a copy.
    /**
     * Sets the matrix elements equal to the elements of \a A. The two
     * matrices must be the same size. To make the matrix reference
     * the same data as \a A, use the reference() member function.
     */
    RWLowerTriMat<TypeT>& operator=(const RWLowerTriMat<TypeT>& A);

    /**
     * Makes this matrix a reference to the argument matrix. The two
     * matrices share the same data. The matrices do \e not have
     * to be the same size before calling reference(). To copy a matrix
     * into another of the same size, you can use the operator=() member
     * operator.
     */
    RWLowerTriMat<TypeT>& reference(RWLowerTriMat<TypeT>& m);

    /**
     * Sets every element of the matrix to 0.
     */
    void zero() {
        vec = TypeT(0);
    }

    // copy() returns a DoubleLowerTriMat with its own copy of the data.
    // deepenShallowCopy makes sure the data is not shared by another
    // vector or matrix.
    /**
     * Creates a copy of this matrix with distinct data. The stride
     * of the data vector in the new matrix is guaranteed to be 1.
     */
    RWLowerTriMat<TypeT> copy() const;

    /**
     * Creates a copy of this matrix with distinct data. The stride
     * of the data vector in the new matrix is guaranteed to be 1.
     */
    RWLowerTriMat<TypeT> deepCopy() const {
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

    // Access to the raw data.
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

    /**
     * Resizes the matrix. Any new entries in the matrix are set to
     * 0. Both arguments must be the same.
     */
    void resize(unsigned m, unsigned n);


    // Input/output member functions
    // Whether US-ASCII or binary formatting is used with readFrom and
    // storeOn is determined by the type of rwstream passed.
    /**
     * Reads a matrix from an input stream. The format of the matrix
     * is the same as the format output by the printOn() member function.
     * Below is a sample matrix that could be input. Note that extra
     * white space and any text preceding the dimension specification
     * are ignored. Only the lower triangle of the matrix is used.
     *
     * @code
     * 3x3
     * [   4   0   0
     *    -5   9   0
     *    -7  -5   3
     * ]
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
    void restoreFrom(RWvistream&);          // Internal US-ASCII or binary code

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

    // Public Operators ---------------------------------------------------------

    /**
     * @internal
     * Converts RWLowerTriMat into an RWGenMat.
     */
    operator RWGenMat<TypeT>() const;

    /**
     * Accesses the \a ij<SUP>th</SUP> element. If the matrix <i>is
     * not</i> a \c const matrix, a reference type is returned, so
     * this operator can be used for assigning or accessing an element.
     * In this case, using this operator is equivalent to calling the
     * ref() member function. If the matrix <i>is</i> a \c const matrix,
     * a value is returned, so this operator can be used only for accessing
     * an element. In this case, using this operator is equivalent to
     * calling the val() member function. Bounds checking is done if
     * the preprocessor symbol \c RWBOUNDS_CHECK is defined before including
     * the header file.
     */
    inline RWRORef<TypeT> operator()(int i, int j);

    /**
     * Accesses the \a ij<SUP>th</SUP> element. If the matrix <i>is
     * not</i> a \c const matrix, a reference type is returned, so
     * this operator can be used for assigning or accessing an element.
     * In this case, using this operator is equivalent to calling the
     * ref() member function. If the matrix \e is a \c const matrix,
     * a value is returned, so this operator can be used only for accessing
     * an element. In this case, using this operator is equivalent to
     * calling the val() member function. Bounds checking is done if
     * the preprocessor symbol \c RWBOUNDS_CHECK is defined before including
     * the header file.
     */
    inline TypeT  operator()(int i, int j) const;


    // The operator=() assignment operators are declared with reference(),
    // since the functions are so closely related.

    /**
     * Boolean operators. Two matrices are considered equal if they
     * have the same size and their elements are all exactly the same.
     * Be aware that floating point arithmetic is \e not exact;
     * matrices that are theoretically equal are not always numerically
     * equal.
     */
    bool operator==(const RWLowerTriMat<TypeT>& X);

    /**
     * @copydoc operator==(const RWLowerTriMat<TypeT>&)
     */
    bool operator!=(const RWLowerTriMat<TypeT>& X) {
        return !(operator==(X));
    }

    // assignment operators.  self must be same size as m.
    /**
     * Performs element-by-element arithmetic on the data in the matrix.
     */
    RWLowerTriMat<TypeT>& operator+=(const RWLowerTriMat<TypeT>& m);
    /**
     * @copydoc operator+=(const RWLowerTriMat<TypeT>&)
     */
    RWLowerTriMat<TypeT>& operator-=(const RWLowerTriMat<TypeT>& m);

    /**
     * Performs element-by-element arithmetic on the data in the matrix.
     * In particular, note that operator*=() does element-by-element
     * multiplication, \e not inner-product style matrix multiplication.
     * You can use the product() global function to do matrix-matrix
     * inner product multiplication.
     */
    RWLowerTriMat<TypeT>& operator*=(const RWLowerTriMat<TypeT>& m);

    /**
     * Performs the indicated operation on each element of the matrix.
     */
    RWLowerTriMat<TypeT>& operator*=(TypeT);
    RWLowerTriMat<TypeT>& operator/=(const RWLowerTriMat<TypeT>& m);

    /**
     * @copydoc operator*=(TypeT)
     */
    RWLowerTriMat<TypeT>& operator/=(TypeT);
};

/*****************************************************************************
 *
 * RWLowerTriMat<TypeT> Inline Functions
 *
 *****************************************************************************/
template <class TypeT>
inline TypeT RWLowerTriMat<TypeT>::val(int i, int j) const
{
#ifdef RWBOUNDS_CHECK
    return bcval(i, j);
#else
    return (i < j) ? (TypeT)(0) : vec(i * (i + 1) / 2 + j);
#endif
}

template <class TypeT>
inline TypeT RWLowerTriMat<TypeT>::set(int i, int j, TypeT x)
{
#ifdef RWBOUNDS_CHECK
    return bcset(i, j, x);
#else
    return vec(i * (i + 1) / 2 + j) = x;
#endif
}

template <class TypeT>
inline RWRORef<TypeT> RWLowerTriMat<TypeT>::ref(int i, int j)
{
    return bcref(i, j);
}

template <class TypeT>
inline RWRORef<TypeT> RWLowerTriMat<TypeT>::operator()(int i, int j)
{
    return ref(i, j);
}

template <class TypeT>
inline TypeT RWLowerTriMat<TypeT>::operator()(int i, int j) const
{
    return val(i, j);
}


/*****************************************************************************
 *
 * Global Functions
 *
 ****************************************************************************/

//  This function returns a new matrix the same shape as the one
// passed in but with the data passed in.  This way we restrict
// all the >if statements to a couple lines
template <class TypeT>
inline RWLowerTriMat<TypeT> sameShapeMatrix(RWMathVec<TypeT>& vec, const RWLowerTriMat<TypeT>& A)
{
    return RWLowerTriMat<TypeT>(vec, A.rows(), A.cols());
}


// added this function --Omon
inline RWLowerTriMat<double> sameShapeRealMatrix(RWMathVec<double>& vec, const RWLowerTriMat<DComplex>& A)
{
    return RWLowerTriMat<double>(vec, A.rows(), A.cols());
}


// Global Binary and Unary Operators -----------------------------------------

/**
 * @relates RWLowerTriMat
 *
 * Unary minus operator. Returns a copy of the negation of the matrix.
 */
template <class TypeT>
RWLowerTriMat<TypeT>  operator-(const RWLowerTriMat<TypeT>&);  // Unary minus

/**
 * @relates RWLowerTriMat
 *
 * Unary plus operator. Returns a copy of the matrix.
 */
template <class TypeT>
RWLowerTriMat<TypeT>  operator+(const RWLowerTriMat<TypeT>&);  // Unary plus

/**
 * @relates RWLowerTriMat
 *
 * Performs element-by-element operations on the arguments. To do
 * inner product matrix multiplication, you can use the product()
 * global function.
 */
template <class TypeT>
RWLowerTriMat<TypeT>  operator*(const RWLowerTriMat<TypeT>&, const RWLowerTriMat<TypeT>&);

/**
 * @relates RWLowerTriMat
 *
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
RWLowerTriMat<TypeT>  operator/(const RWLowerTriMat<TypeT>&, const RWLowerTriMat<TypeT>&);

/**
 * @relates RWLowerTriMat
 * @copydoc operator/(const RWLowerTriMat<TypeT>&, const RWLowerTriMat<TypeT>&)
 */
template <class TypeT>
RWLowerTriMat<TypeT>  operator+(const RWLowerTriMat<TypeT>&, const RWLowerTriMat<TypeT>&);

/**
 * @relates RWLowerTriMat
 * @copydoc operator/(const RWLowerTriMat<TypeT>&, const RWLowerTriMat<TypeT>&)
 */
template <class TypeT>
RWLowerTriMat<TypeT>  operator-(const RWLowerTriMat<TypeT>&, const RWLowerTriMat<TypeT>&);

/**
 * @relates RWLowerTriMat
 *
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
inline RWLowerTriMat<TypeT>  operator*(const RWLowerTriMat<TypeT>& A, TypeT x)
{
    RWMathVec<TypeT> temp(A.dataVec()*x);
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
#ifndef RW_NO_INLINED_TEMP_DESTRUCTORS
/**
 * @relates RWLowerTriMat
 * @copydoc operator*(const RWLowerTriMat<TypeT>&, TypeT)
 */
inline RWLowerTriMat<TypeT>  operator*(TypeT x, const RWLowerTriMat<TypeT>& A)
{
    return A * x;
}
#else
RWLowerTriMat<TypeT>  operator*(TypeT x, const RWLowerTriMat<TypeT>& A);
#endif

/**
 * @relates RWLowerTriMat
 * @copydoc operator*(const RWLowerTriMat<TypeT>&, TypeT)
 */
template <class TypeT>
RWLowerTriMat<TypeT>  operator/(const RWLowerTriMat<TypeT>& A, TypeT x);


// I/O Stream operators -----------------------------------------------------------
#if defined(_MSC_VER)
template <class TypeT>
inline  std::ostream& operator<<(std::ostream& s, const RWLowerTriMat<TypeT>& m)
{
    m.printOn(s);
    return s;
}
template <class TypeT>
inline  std::istream& operator>>(std::istream& s, RWLowerTriMat<TypeT>& m)
{
    m.scanFrom(s);
    return s;
}
#else
/**
 * @relates RWLowerTriMat
 *
 * Writes the matrix to the stream. This is equivalent to calling
 * the RWLowerTriMat<TypeT>::printOn() member function.
 */
template <class TypeT>
inline std::ostream&  operator<<(std::ostream& s, const RWLowerTriMat<TypeT>& m)
{
    m.printOn(s);
    return s;
}

/**
 * @relates RWLowerTriMat
 *
 * Reads the matrix from the stream. This is equivalent to calling
 * the RWLowerTriMat<TypeT>::scanFrom member function.
 */
template <class TypeT>
inline std::istream&  operator>>(std::istream& s, RWLowerTriMat<TypeT>& m)
{
    m.scanFrom(s);
    return s;
}
#endif

/**
 * @relates RWLowerTriMat
 *
 * Returns the transpose of the argument matrix. The returned matrix
 * references the same data as the argument matrix.
 *
 */
template <class TypeT>
RWUpperTriMat<TypeT>  transpose(const RWLowerTriMat<TypeT>&);

/**
 * @relates RWLowerTriMat
 *
 * Returns the inner product (matrix-vector product) of \a A and
 * \a x.
 */
template <class TypeT>
RWMathVec<TypeT>  product(const RWLowerTriMat<TypeT>& A, const RWMathVec<TypeT>& x);

/**
 * @relates RWLowerTriMat
 *
 * Returns the inner product (matrix-vector product) of \a x and
 * \a A. This is equal to the product of \a A transpose and \a x.
 */
template <class TypeT>
RWMathVec<TypeT>  product(const RWMathVec<TypeT>& x, const RWLowerTriMat<TypeT>& A);


// Below are functions that assume automatic conversion to GenMat's
// Automatic conversions do not take place for templatized functions,
// So we provide them here...
template <class TypeT>
RWGenMat<TypeT>  product(const RWLowerTriMat<TypeT>& A, const RWGenMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  product(const RWGenMat<TypeT>& A, const RWLowerTriMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  product(const RWLowerTriMat<TypeT>& A, const RWLowerTriMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWLowerTriMat<TypeT>& A, const RWGenMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWGenMat<TypeT>& A, const RWLowerTriMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWLowerTriMat<TypeT>& A, const RWLowerTriMat<TypeT>& B);


template <class TypeT>
inline RWGenMat<TypeT>  adjoint(const RWLowerTriMat<TypeT>& A)
{
    RWGenMat<TypeT> a(A);
    return adjoint(a);
}

/**
 * @relates RWLowerTriMat
 *
 * Extracts the lower triangular part of a square matrix.
 */
template <class TypeT>
RWLowerTriMat<TypeT>  toLowerTriMat(const RWGenMat<TypeT>& A);


#if !(defined(_MSC_VER))
/**
 * @relates RWLowerTriMat
 *
 * Returns a matrix whose entries are the absolute value of the
 * argument. The absolute value of a complex number is considered
 * to be the sum of the absolute values of its real and imaginary
 * parts. To get the norm of a complex matrix, you can use the
 * norm() function.
 */
template <class TypeT>
inline RWLowerTriMat<typename rw_numeric_traits<TypeT>::norm_type>  abs(const RWLowerTriMat<TypeT>& A)
{
    RWMathVec<typename rw_numeric_traits<TypeT>::norm_type> temp(abs(A.dataVec()));
    return sameShapeMatrix(temp, A);
}
//specialize for DComplex
#  if !(defined(__hppa))
template <>
#  endif
inline RWLowerTriMat<rw_numeric_traits<DComplex>::norm_type>  abs(const RWLowerTriMat<DComplex>& A)
{
    RWMathVec<rw_numeric_traits<DComplex>::norm_type> temp(abs(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

#else //using MSVC compiler:

template <class TypeT>
inline RWLowerTriMat<typename rw_numeric_traits<TypeT>::norm_type>  abs(const RWLowerTriMat<TypeT>& M)
{
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;
    RWMathVec<rt> temp(abs(M.dataVec()));
    //return sameShapeMatrix(temp,M);
    //this function should call sameShape{Real}Matrix, and return the result of that function.
    // I'm putting code directly from those functions here -- MSVC5 cannot handle complicated template specializations -- till a better solution comes up
    return RWLowerTriMat<rt>(temp, M.rows(), M.cols());
};

#endif /* !defined(_MSC_VER) */

/**
 * @relates RWLowerTriMat
 *
 * Returns a matrix where each element is the complex conjugate
 * of the corresponding element in the matrix \a A.
 */
inline RWLowerTriMat<DComplex>  conj(const RWLowerTriMat<DComplex>& A)
{
    RWMathVec<DComplex> temp(conj(A.dataVec()));
    return sameShapeMatrix(temp, A);
}

/**
 * @relates RWLowerTriMat
 *
 * Returns a matrix where each element is the real part of the corresponding
 * element in the matrix \a A.
 */
inline RWLowerTriMat<double>  real(const RWLowerTriMat<DComplex>& A)
{
    RWMathVec<double> temp(real(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWLowerTriMat
 *
 * Returns a matrix where each element is the imaginary part of
 * the corresponding element in the matrix \a A.
 */
inline RWLowerTriMat<double>  imag(const RWLowerTriMat<DComplex>& A)
{
    RWMathVec<double> temp(imag(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWLowerTriMat
 *
 * Returns a matrix where each element is the norm (magnitude) of
 * the corresponding element in the matrix \a A.
 */
inline RWLowerTriMat<double>  norm(const RWLowerTriMat<DComplex>& A)
{
    RWMathVec<double> temp(norm(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWLowerTriMat
 *
 * Returns a matrix where each element is the argument of the corresponding
 * element in the matrix \a A.
 */
inline RWLowerTriMat<double>  arg(const RWLowerTriMat<DComplex>& A)
{
    RWMathVec<double> temp(arg(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

inline RWGenMat<DComplex>  conjTransposeProduct(const RWLowerTriMat<DComplex>& A, const RWGenMat<DComplex>& B)
{
    return conjTransposeProduct(RWGenMat<DComplex>(A), B);
}

inline RWGenMat<DComplex>  conjTransposeProduct(const RWGenMat<DComplex>& A, const RWLowerTriMat<DComplex>& B)
{
    return conjTransposeProduct(A, RWGenMat<DComplex>(B));
}

inline RWGenMat<DComplex>  conjTransposeProduct(const RWLowerTriMat<DComplex>& A, const RWLowerTriMat<DComplex>& B)
{
    return conjTransposeProduct(RWGenMat<DComplex>(A), RWGenMat<DComplex>(B));
}

/**
 * @relates RWLowerTriMat
 *
 * Returns the minimum entry in the matrix.
 */
template <class TypeT>
inline TypeT  minValue(const RWLowerTriMat<TypeT>& A)
{
    return minValue(A.dataVec());
}

/**
 * @relates RWLowerTriMat
 *
 * Returns the maximum entry in the matrix.
 */
template <class TypeT>
inline TypeT  maxValue(const RWLowerTriMat<TypeT>& A)
{
    return maxValue(A.dataVec());
}




#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/ltrimat.cc>
#endif

#endif
