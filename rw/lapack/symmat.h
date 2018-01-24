
#ifndef __RWSYMMAT_H__
#define __RWSYMMAT_H__
/*
 * RWSymMat<TypeT> - A symmetric matrix of Ts
 *
 * Stores a symmetric matrix.  Only half the matrix is actually
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
 * $Id: //lapack/13/rw/lapack/symmat.h#1 $
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
#include <rw/lapack/latraits.h>
#include <rw/math/numtrait.h>

template <class TypeT>
class RWGenMat;

/**
 * @ingroup sparse_matrix_classes
 * @brief Represents a symmetric matrix.
 *
 * The class \link RWSymMat RWSymMat<T>\endlink represents symmetric
 * matrices. A symmetric matrix is defined by the requirement that
 * \f$ A_{ij} = A_{ji} \f$, and so a symmetric matrix
 * is equal to its transpose.
 *
 * @section RWSymMat_synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/symmat.h>
 *
 * RWSymMat<double> A;
 * @endcode
 *
 * @section RWSymMat_example Example
 *
 * @code
 * #include <rw/lapack/symmat.h>
 *
 * int main()
 * {
 *     RWSymMat<double> S(4,4,3.1); // initialize to 3.1
 *     RWSymMat<double> A = 4.0*S;
 *     return 0;
 * }
 * @endcode
 *
 * @section RWSymMat_storage_scheme Storage Scheme
 *
 * The upper triangle of the matrix is stored in column major order.
 * The lower triangle is then calculated implicitly. This storage scheme
 * was chosen so that the leading part of the matrix was always located
 * in contiguous memory.
 *
 * For example, given the following symmetric matrix:
 *
 * \f[
 *   \begin{bmatrix}
 *    A_{11} & A_{12} & A_{13} & \dots & A_{1n} \\
 *    A_{12} & A_{22} & A_{23} & \dots & A_{2n} \\
 *    A_{13} & A_{23} & A_{33} & \dots & A_{3n} \\
 *    \vdots \\
 *    A_{1n} & A_{2n} & A_{3n} & \dots & A_{nn}
 *   \end{bmatrix}
 * \f]
 *
 * The data is stored in the following order:
 *
 * \f[
 *   \left[
 *    \begin{array}{cccccccccccc}
 *     A_{11} & A_{12} & A_{22} & A_{13} & A_{23} & A_{33} &
 *     \dots & A_{1n} & A_{2n} & A_{3n} & \dots & A_{nn}
 *    \end{array}
 *   \right]
 * \f]
 *
 * The mapping between the array and storage vector is as follows:
 *
 * \f[
 *  A(i+1, j+1) \to
 *  \begin{cases}
 *   \text{vec}[j(j+1)/2+i], & i \leq j \\
 *   \text{vec}[i(i+1)/2+j], & j \leq i
 *  \end{cases}
 * \f]
 */
template <class TypeT>
class RWSymMat
{
private:

    RWMathVec<TypeT> vec;
    unsigned n;
    // The data which define the matrix


public:

    /**
     * Default constructor. Builds a matrix of size \c 0 x \c 0. This
     * constructor is necessary to declare a matrix with no explicit
     * constructor or to declare an array of matrices.
     */
    RWSymMat();

    /**
     * Builds a copy of its parameter, \a A. Note that the new matrix
     * references \a A's data. To construct a matrix with its own copy
     * of the data, you can use either the copy() or deepenShallowCopy()
     * member functions.
     */
    RWSymMat(const RWSymMat<TypeT>& A);

    /**
     * Defines an uninitialized matrix of size \a n x \a nAgain. Both
     * parameters must be equal or a runtime error occurs. This constructor
     * is used, rather than a constructor that takes only a single
     * parameter, to avoid type conversion problems.
     */
    RWSymMat(unsigned n, unsigned nAgain);

    /**
     * Defines a matrix of size \a n x \a nAgain where each element is
     * initialized to \a initval. Both parameters must be equal or a runtime
     * error occurs.
     */
    RWSymMat(unsigned n, unsigned nAgain, TypeT initval);

    /**
     * Constructs a size \a n x \a nAgain matrix using the data in the
     * passed vector. This data must be stored in the format described
     * in the \ref RWSymMat_storage_scheme section. The resultant matrix
     * references the data in vector \a data.
     */
    RWSymMat(const RWMathVec<TypeT>& data, unsigned n, unsigned nAgain);

    /**
     * Constructs a complex matrix from the real part supplied. Imaginary
     * part is assumed to be \c 0.
     */
    RW_LAPK_MEMBER_EXPORT RWSymMat(const typename rw_linear_algebra_traits<TypeT>::generic_sym_mat& re);

    /**
     * Constructs a complex matrix from the real and imaginary parts
     * supplied.
     */
    RW_LAPK_MEMBER_EXPORT RWSymMat(const RWSymMat<double>& re, const RWSymMat<double>& im);

    ~RWSymMat();
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
     * Returns the value of the \a i\a j <SUP>th</SUP> element of the
     * matrix. Bounds checking is done if the preprocessor symbol
     * \c RWBOUNDS_CHECK is defined when the header file is read.
     * The member function bcval() does the same thing with guaranteed
     * bounds checking.
     */
    inline TypeT  val(int i, int j) const;

    /**
     * Returns the value of the \a i\a j <SUP>th</SUP> element of the
     * matrix, after doing bounds checking.
     */
    TypeT  bcval(int i, int j) const;

    /**
     * Sets the \a i\a j <SUP>th</SUP> element of the matrix equal to \a x.
     * Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined when the header file is read. The member function
     * bcset() does the same thing with guaranteed bounds checking.
     */
    inline TypeT  set(int i, int j, TypeT x);

    /**
     * Sets the \a i\a j <SUP>th</SUP> element of the matrix equal to
     * \a x, after doing bounds checking.
     */
    TypeT  bcset(int i, int j, TypeT x);

    /**
     * Accesses the \a i\a j <SUP>th</SUP> element when self <i>is not</i>
     * a \c const matrix. A reference type is returned, so this operator
     * can be used for assigning or accessing an element. Using this
     * operator is equivalent to calling the ref() member function.
     * Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined before including the header file.
     */
    inline TypeT& operator()(int i, int j);

    /**
     * Returns a reference to the \a i\a j <SUP>th</SUP> element of the
     * matrix. Bounds checking is done if the preprocessor symbol
     * \c RWBOUNDS_CHECK is defined when the header file is read. The
     * member function bcref() does the same thing with guaranteed bounds
     * checking.
     */
    inline TypeT& ref(int i, int j);

    /**
     * Returns a reference to the \a i\a j <SUP>th</SUP> element of the
     * matrix, after doing bounds checking.
     */
    TypeT& bcref(int i, int j);

    /**
     * Accesses the \a i\a j <SUP>th</SUP> element when self \e is a
     * \c const matrix. A value is returned, so this operator can be used
     * only for accessing an element. Using this operator is equivalent to
     * calling the val() member function. Bounds checking is done if the
     * preprocessor symbol \c RWBOUNDS_CHECK is defined before including
     * the header file.
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
    RWSymMat<TypeT> leadingSubmatrix(int k);
    // Access to submatrices.
    // The leading submatrix of order k is the kxk matrix in the upper
    // left.
    // Access to diagonals.  The same conventions as for
    // element access apply (with respect to bcxxx and xxx names).

    /**
     * Sets the matrix elements equal to the elements of \a A. The two
     * matrices must be the same size. To make the matrix reference
     * the same data as \a A, you can use the reference() member function.
     */
    RWSymMat<TypeT>& operator=(const RWSymMat<TypeT>& A);

    /**
     * Makes this matrix a reference to the parameter matrix. The two
     * matrices share the same data. The matrices do \e not have
     * to be the same size before calling reference(). To copy a matrix
     * into another of the same size, use the operator=() member operator.
     */
    RWSymMat<TypeT>& reference(RWSymMat<TypeT>& m);
    // reference() makes an alias, operator= makes a copy.

    /**
     * Sets each element in the matrix equal to \a x.
     */
    RWSymMat<TypeT>& operator=(TypeT x) {
        vec = TypeT(x);
        return *this;
    }

    operator RWGenMat<TypeT>() const;

    /**
     * Sets every element of the matrix to \c 0.
     */
    void zero() {
        vec = TypeT(0);
    }


    /**
     * Creates a copy of this matrix with distinct data. The stride
     * of the data vector in the new matrix is guaranteed to be \c 1.
     */
    RWSymMat<TypeT> copy() const;

    /**
     * @copydoc copy() const
     */
    RWSymMat<TypeT> deepCopy() const {
        return copy();
    }

    /**
     * Ensures that the data in the matrix is \e not shared by any
     * other matrix or vector. Also ensures that the stride in the data
     * vector is equal to \c 1. If necessary, a new copy of the data vector
     * is made.
     */
    void deepenShallowCopy() {
        vec.deepenShallowCopy();
    }
    // copy() returns a RWSymMat<TypeT> with its own copy of the data.
    // deepenShallowCopy makes sure the data is not shared by another
    // vector or matrix.

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
     * \c 0. Both parameters must be the same.
     */
    void resize(unsigned n, unsigned nAgain);
    // Resize the matrix.  New elements are set to zero.

    //RWSymMat<TypeT> apply(CmathFunTy) const;

    /**
     * Returns the result of applying the passed function \a func to every
     * element in the matrix. If \c TypeT is DComplex, \a func takes and
     * returns a DComplex. For all other \c TypeT, \a func takes and
     * returns a \c double.
     */
    RWSymMat<TypeT>
    apply(typename rw_linear_algebra_traits<TypeT>::lapkFunType func) const;

    // Not externally documented yet
    /*
     * Returns the result of applying the passed function \a func to every
     * element in the matrix. Function \a func takes a DComplex and returns
     * a \c double.
     */
    RWSymMat<double> apply2(CmathFunTy2 func) const;
    // Each element in the matrix returned by apply is
    // the result of calling f() on the corresponding element
    // in this matrix.

    /**
     * Reads a matrix from an input stream. The format of the matrix
     * is the same as the format output by the printOn() member function.
     * Below is a sample matrix that could be input. Note that extra
     * white space and any text preceding the dimension specification
     * are ignored. Only the symmetric part of the matrix is used.
     *
     * @code
     * 3x3 [ 4 5 7
     * 5 9 5
     * 7 5 3 ]
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
     * Boolean operator. Two matrices are considered equal if they
     * have the same size and their elements are all exactly the same.
     * Be aware that floating point arithmetic is \e not exact;
     * matrices that are theoretically equal are not always numerically
     * equal.
     */
    bool operator==(const RWSymMat<TypeT>& X);

    /**
     * @copydoc operator==(const RWSymMat<TypeT>&)
     */
    bool operator!=(const RWSymMat<TypeT>& X) {
        return !(operator==(X));
    }
    // Two matrices are considered equal if their shapes are identical and
    // all their components are identical.

    /**
     * Performs the indicated operation on each element of the matrix.
     */
    RWSymMat<TypeT>& operator+=(TypeT x);

    /**
     * @copydoc operator+=(TypeT)
     */
    RWSymMat<TypeT>& operator-=(TypeT x);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     */
    RWSymMat<TypeT>& operator+=(const RWSymMat<TypeT>& A);

    /**
     * @copydoc operator+=(const RWSymMat<TypeT>&)
     */
    RWSymMat<TypeT>& operator-=(const RWSymMat<TypeT>& A);

    /**
     * @copydoc operator+=(const RWSymMat<TypeT>&)
     *
     * @note \link operator*=(const RWSymMat<TypeT>&) operator*=() \endlink
     * does element-by-element multiplication, \e not inner product style
     * matrix multiplication. Use the product() global function to do
     * matrix-matrix inner product multiplication.
     */
    RWSymMat<TypeT>& operator*=(const RWSymMat<TypeT>& A);

    /**
     * @copydoc operator+=(TypeT)
     */
    RWSymMat<TypeT>& operator*=(TypeT x);

    /**
     * @copydoc operator+=(const RWSymMat<TypeT>&)
     */
    RWSymMat<TypeT>& operator/=(const RWSymMat<TypeT>& A);

    /**
     * @copydoc operator+=(TypeT)
     */
    RWSymMat<TypeT>& operator/=(TypeT x);
    // assignment operators.  self must be same size as m.

    /**
     * Increments each element in the matrix.
     */
    RWSymMat<TypeT>& operator++();   // Prefix operator
    void operator++(int);   // Postfix operator

    /**
     * Decrements each element in the matrix.
     */
    RWSymMat<TypeT>& operator--();   // Prefix operator
    void operator--(int);   // Postfix operator
    // Increment/Decrement operators

};

/*
 * This function returns a new matrix the same shape as the one
 * passed in but with the data passed in.  This way we restrict
 * all the >if statements to a couple lines
 */
template <class TypeT>
inline RWSymMat<TypeT> sameShapeMatrix(RWMathVec<TypeT>& vec, const RWSymMat<TypeT>& A)
{
    return RWSymMat<TypeT>(vec, A.rows(), A.cols());
}

//template <class TypeT>
inline RWSymMat<double> sameShapeRealMatrix(RWMathVec<double>& vec, const RWSymMat<DComplex>& A)
{
    return RWSymMat<double>(vec, A.rows(), A.cols());
}


/**
 * @relates RWSymMat
 *
 * Unary minus operator. Returns negation of the matrix.
 */
template <class TypeT>
RWSymMat<TypeT>  operator-(const RWSymMat<TypeT>&);        // Unary minus

/**
 * @relates RWSymMat
 *
 * Unary plus operator. Returns a copy of the matrix.
 */
template <class TypeT>
RWSymMat<TypeT>  operator+(const RWSymMat<TypeT>&);        // Unary plus

/**
 * @relates RWSymMat
 *
 * @copydoc operator+(const RWSymMat<TypeT>&,const RWSymMat<TypeT>&)
 * To do inner product matrix multiplication, you can use the product()
 * global function.
 */
template <class TypeT>
RWSymMat<TypeT>  operator*(const RWSymMat<TypeT>&, const RWSymMat<TypeT>&);

/**
 * @relates RWSymMat
 *
 * @copydoc operator+(const RWSymMat<TypeT>&,const RWSymMat<TypeT>&)
 */
template <class TypeT>
RWSymMat<TypeT>  operator/(const RWSymMat<TypeT>&, const RWSymMat<TypeT>&);

/**
 * @relates RWSymMat
 *
 * Performs element-by-element operations on the parameters.
 */
template <class TypeT>
RWSymMat<TypeT>  operator+(const RWSymMat<TypeT>&, const RWSymMat<TypeT>&);

/**
 * @relates RWSymMat
 *
 * @copydoc operator+(const RWSymMat<TypeT>&,const RWSymMat<TypeT>&)
 */
template <class TypeT>
RWSymMat<TypeT>  operator-(const RWSymMat<TypeT>&, const RWSymMat<TypeT>&);

/**
 * @relates RWSymMat
 *
 * @copydoc operator+(const RWSymMat<TypeT>&,const RWSymMat<TypeT>&)
 */
template <class TypeT>
inline RWSymMat<TypeT>  operator*(const RWSymMat<TypeT>& A, TypeT x)
{
    RWMathVec<TypeT> temp(A.dataVec()*x);
    return sameShapeMatrix(temp, A);
}

#ifndef RW_NO_INLINED_TEMP_DESTRUCTORS

/**
 * @relates RWSymMat
 *
 * @copydoc operator+(const RWSymMat<TypeT>&,const RWSymMat<TypeT>&)
 */
template <class TypeT>
inline RWSymMat<TypeT>  operator*(TypeT x, const RWSymMat<TypeT>& A)
{
    return A * x;
}
#else
template <class TypeT>
RWSymMat<TypeT>  operator*(TypeT x, const RWSymMat<TypeT>& A);
#endif

/**
 * @relates RWSymMat
 *
 * @copydoc operator+(const RWSymMat<TypeT>&,const RWSymMat<TypeT>&)
 */
template <class TypeT>
RWSymMat<TypeT>  operator/(const RWSymMat<TypeT>& A, TypeT x);

/**
 * @relates RWSymMat
 *
 * @copydoc operator+(const RWSymMat<TypeT>&,const RWSymMat<TypeT>&)
 */
template <class TypeT>
RWSymMat<TypeT>  operator/(TypeT x, const RWSymMat<TypeT>& A);

/**
 * @relates RWSymMat
 *
 * @copydoc operator+(const RWSymMat<TypeT>&,const RWSymMat<TypeT>&)
 */
template <class TypeT>
inline RWSymMat<TypeT> operator+(const RWSymMat<TypeT>& A, TypeT x)
{
    RWMathVec<TypeT> temp(A.dataVec() + x);
    return sameShapeMatrix(temp, A);
}

#ifndef RW_NO_INLINED_TEMP_DESTRUCTORS

/**
 * @relates RWSymMat
 *
 * @copydoc operator+(const RWSymMat<TypeT>&,const RWSymMat<TypeT>&)
 */
template <class TypeT>
inline RWSymMat<TypeT>  operator+(TypeT x, const RWSymMat<TypeT>& A)
{
    return A + x;
}

/**
 * @relates RWSymMat
 *
 * @copydoc operator+(const RWSymMat<TypeT>&,const RWSymMat<TypeT>&)
 */
template <class TypeT>
inline RWSymMat<TypeT>  operator-(const RWSymMat<TypeT>& A, TypeT x)
{
    return A + (-x);
}
#else
template <class TypeT>
RWSymMat<TypeT>  operator+(TypeT x, const RWSymMat<TypeT>& A);
template <class TypeT>
RWSymMat<TypeT>  operator-(const RWSymMat<TypeT>& A, TypeT x);
#endif

/**
 * @relates RWSymMat
 *
 * @copydoc operator+(const RWSymMat<TypeT>&,const RWSymMat<TypeT>&)
 */
template <class TypeT>
RWSymMat<TypeT>  operator-(TypeT x, const RWSymMat<TypeT>& A);
// Arithmetic operators; Notice that operator* is an
// element by element multiply, NOT a matrix multiply.

/**
 * @relates RWSymMat
 *
 * Writes the matrix to the stream. This is equivalent to calling
 * the printOn() member function.
 */
template <class TypeT>
inline std::ostream&  operator<<(std::ostream& s, const RWSymMat<TypeT>& m)
{
    m.printOn(s);
    return s;
}

/**
 * @relates RWSymMat
 *
 * Reads the matrix from the stream. This is equivalent to calling
 * the scanFrom() member function.
 */
template <class TypeT>
inline std::istream&  operator>>(std::istream& s, RWSymMat<TypeT>& m)
{
    m.scanFrom(s);
    return s;
}

// I/O for humans

/**
 * @relates RWSymMat
 *
 * Returns the transpose of the parameter matrix. Since a symmetric
 * matrix is its own transpose, this function just returns itself.
 */
template <class TypeT>
RWSymMat<TypeT>  transpose(const RWSymMat<TypeT>&);
// The transpose of the matrix.

/**
 * @relates RWSymMat
 *
 * Returns the inner product (matrix-vector product) of \a A and
 * \a x.
 */
template <class TypeT>
RWMathVec<TypeT>  product(const RWSymMat<TypeT>& A, const RWMathVec<TypeT>& x);

/**
 * @relates RWSymMat
 *
 * Returns the inner product (matrix-vector product) of \a x and
 * \a A. This is equal to the product of \a A transpose and \a x.
 */
template <class TypeT>
RWMathVec<TypeT>  product(const RWMathVec<TypeT>& x, const RWSymMat<TypeT>& A);
// inner products

// Below are functions that assume automatic conversion to GenMat's
// Automatic conversions do not take place for templatized functions,
// So we provide them here...
template <class TypeT>
RWGenMat<TypeT>  product(const RWSymMat<TypeT>& A, const RWGenMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  product(const RWGenMat<TypeT>& A, const RWSymMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  product(const RWSymMat<TypeT>& A, const RWSymMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWSymMat<TypeT>& A, const RWGenMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWGenMat<TypeT>& A, const RWSymMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWSymMat<TypeT>& A, const RWSymMat<TypeT>& B);
/*
RWGenMat<DComplex>  conjTransposeProduct(const RWSymMat<DComplex>& A, const RWGenMat<DComplex>& B);
RWGenMat<DComplex>  conjTransposeProduct(const RWGenMat<DComplex>& A, const RWSymMat<DComplex>& B);
RWGenMat<DComplex>  conjTransposeProduct(const RWSymMat<DComplex>& A, const RWSymMat<DComplex>& B);
*/
inline RWGenMat<DComplex> conjTransposeProduct(const RWSymMat<DComplex>& A, const RWGenMat<DComplex>& B)
{
    return conjTransposeProduct(RWGenMat<DComplex>(A), B);
}

inline RWGenMat<DComplex> conjTransposeProduct(const RWGenMat<DComplex>& A, const RWSymMat<DComplex>& B)
{
    return conjTransposeProduct(A, RWGenMat<DComplex>(B));
}

inline RWGenMat<DComplex> conjTransposeProduct(const RWSymMat<DComplex>& A, const RWSymMat<DComplex>& B)
{
    return conjTransposeProduct(RWGenMat<DComplex>(A), RWGenMat<DComplex>(B));
}

template <class TypeT>
inline RWGenMat<TypeT>  adjoint(const RWSymMat<TypeT>& A)
{
    RWGenMat<TypeT> a(A);
    return adjoint(a);
}

/**
 * @relates RWSymMat
 *
 * Extracts the symmetric part of a square matrix. The symmetric
 * part of matrix \a A is \f$ (A+A^{T})/2 \f$.
 */
template <class TypeT>
RWSymMat<TypeT>  toSymMat(const RWGenMat<TypeT>& A);

/**
 * @relates RWSymMat
 *
 * Builds a symmetric matrix that matches the upper triangular part
 * of \a A. The lower triangle of \a A is \e not referenced.
 */
template <class TypeT>
RWSymMat<TypeT>  upperToSymMat(const RWGenMat<TypeT>& A);

/**
 * @relates RWSymMat
 *
 * Builds a symmetric matrix that matches the lower triangular part
 * of \a A. The upper triangle of \a A is \e not referenced.
 */
template <class TypeT>
RWSymMat<TypeT>  lowerToSymMat(const RWGenMat<TypeT>& A);

#if !(defined(_MSC_VER))

/**
 * @relates RWSymMat
 *
 * Returns a matrix whose entries are the absolute value of the
 * parameter. The absolute value of a complex number is considered
 * to be the sum of the absolute values of its real and imaginary
 * parts. To get the norm of a complex matrix, you can use the
 * norm() function.
 */
template <class TypeT>
inline RWSymMat<typename rw_numeric_traits<TypeT>::norm_type>  abs(const RWSymMat<TypeT>& M)
{
    RWMathVec<typename rw_numeric_traits<TypeT>::norm_type> temp(abs(M.dataVec()));
    return sameShapeMatrix(temp, M);
}
//specialize for DComplex:
#  if !(defined(__hppa))
template <>
#  endif
inline RWSymMat<rw_numeric_traits<DComplex>::norm_type>  abs(const RWSymMat<DComplex>& M)
{
    RWMathVec<rw_numeric_traits<DComplex>::norm_type> temp(abs(M.dataVec()));
    return sameShapeRealMatrix(temp, M);
}

#else //using MSVC compiler:

template <class TypeT>
inline RWSymMat<typename rw_numeric_traits<TypeT>::norm_type>  abs(const RWSymMat<TypeT>& M)
{
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;
    RWMathVec<rt> temp(abs(M.dataVec()));
    //return sameShapeMatrix(temp,M);
    //this function should call sameShape{Real}Matrix, and return the result of that function.
    // I'm putting code directly from those functions here -- MSVC5 cannot handle complicated template specializations -- till a better solution comes up
    return RWSymMat<rt>(temp, M.rows(), M.cols());
}


#endif // !defined(_MSC_VER)


/**
 * @relates RWSymMat
 *
 * Returns a matrix where each element is the complex conjugate
 * of the corresponding element in the matrix \a A.
 */
inline RWSymMat<DComplex>  conj(const RWSymMat<DComplex>& A)
{
    RWMathVec<DComplex> temp(conj(A.dataVec()));
    return sameShapeMatrix(temp, A);
}

/**
 * @relates RWSymMat
 *
 * Returns a matrix where each element is the real part of the corresponding
 * element in the matrix \a A.
 */
inline RWSymMat<double>  real(const RWSymMat<DComplex>& A)
{
    RWMathVec<double> temp(real(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWSymMat
 *
 * Returns a matrix where each element is the imaginary part of
 * the corresponding element in the matrix \a A.
 */
inline RWSymMat<double>  imag(const RWSymMat<DComplex>& A)
{
    RWMathVec<double> temp(imag(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWSymMat
 *
 * Returns a matrix where each element is the norm (magnitude) of
 * the corresponding element in the matrix \a A.
 */
inline RWSymMat<double>  norm(const RWSymMat<DComplex>& A)
{
    RWMathVec<double> temp(norm(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWSymMat
 *
 * Returns a matrix where each element is the argument of
 * the corresponding element in the matrix \a A.
 */
inline RWSymMat<double>  arg(const RWSymMat<DComplex>& A)
{
    RWMathVec<double> temp(arg(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWSymMat
 *
 * Returns the minimum entry in the matrix.
 */
inline double  minValue(const RWSymMat<double>& A)
{
    return minValue(A.dataVec());
}

/**
 * @relates RWSymMat
 *
 * Returns the maximum entry in the matrix.
 */
inline double  maxValue(const RWSymMat<double>& A)
{
    return maxValue(A.dataVec());
}

/**
 * @relates RWSymMat
 *
 * @copydoc minValue(const RWSymMat<double>&)
 */
inline float  minValue(const RWSymMat<float>& A)
{
    return minValue(A.dataVec());
}

/**
 * @relates RWSymMat
 *
 * @copydoc maxValue(const RWSymMat<double>&)
 */
inline float  maxValue(const RWSymMat<float>& A)
{
    return maxValue(A.dataVec());
}


#if defined(RWSTDLIBTYPE) && !defined(_MSC_VER) && !defined(RW_RWSTDLIB_USES_MSVC_CMATH) && !defined(__linux__)
template <class TypeT>
inline RWSymMat<TypeT>  cos(const RWSymMat<TypeT>& A)
{
    return A.apply(std::cos);
}
template <class TypeT>
inline RWSymMat<TypeT>  cosh(const RWSymMat<TypeT>& A)
{
    return A.apply(std::cosh);
}
template <class TypeT>
inline RWSymMat<TypeT>  exp(const RWSymMat<TypeT>& A)
{
    return A.apply(std::exp);
}
template <class TypeT>
inline RWSymMat<TypeT>  log(const RWSymMat<TypeT>& A)
{
    return A.apply(std::log);
}
template <class TypeT>
inline RWSymMat<TypeT>  sin(const RWSymMat<TypeT>& A)
{
    return A.apply(std::sin);
}
template <class TypeT>
inline RWSymMat<TypeT>  sinh(const RWSymMat<TypeT>& A)
{
    return A.apply(std::sinh);
}
template <class TypeT>
inline RWSymMat<TypeT>  sqrt(const RWSymMat<TypeT>& A)
{
    return A.apply(std::sqrt);
}
#else

/**
 * @relates RWSymMat
 *
 * @copydoc acos(const RWSymMat<float>&)
 */
template <class TypeT>
inline RWSymMat<TypeT>  cos(const RWSymMat<TypeT>& A)
{
    return RWSymMat<TypeT>(::cos(A.dataVec()), A.rows(), A.cols());
}

/**
 * @relates RWSymMat
 *
 * @copydoc acos(const RWSymMat<float>&)
 */
template <class TypeT>
inline RWSymMat<TypeT>  cosh(const RWSymMat<TypeT>& A)
{
    return RWSymMat<TypeT>(::cosh(A.dataVec()), A.rows(), A.cols());
}

/**
 * @relates RWSymMat
 *
 * @copydoc acos(const RWSymMat<float>&)
 */
template <class TypeT>
inline RWSymMat<TypeT>  exp(const RWSymMat<TypeT>& A)
{
    return RWSymMat<TypeT>(::exp(A.dataVec()), A.rows(), A.cols());
}

/**
 * @relates RWSymMat
 *
 * @copydoc acos(const RWSymMat<float>&)
 */
template <class TypeT>
inline RWSymMat<TypeT>  log(const RWSymMat<TypeT>& A)
{
    return RWSymMat<TypeT>(::log(A.dataVec()), A.rows(), A.cols());
}

/**
 * @relates RWSymMat
 *
 * @copydoc acos(const RWSymMat<float>&)
 */
template <class TypeT>
inline RWSymMat<TypeT>  sin(const RWSymMat<TypeT>& A)
{
    return RWSymMat<TypeT>(::sin(A.dataVec()), A.rows(), A.cols());
}

/**
 * @relates RWSymMat
 *
 * @copydoc acos(const RWSymMat<float>&)
 */
template <class TypeT>
inline RWSymMat<TypeT>  sinh(const RWSymMat<TypeT>& A)
{
    return RWSymMat<TypeT>(::sinh(A.dataVec()), A.rows(), A.cols());
}

/**
 * @relates RWSymMat
 *
 * @copydoc acos(const RWSymMat<float>&)
 */
template <class TypeT>
inline RWSymMat<TypeT>  sqrt(const RWSymMat<TypeT>& A)
{
    return RWSymMat<TypeT>(::sqrt(A.dataVec()), A.rows(), A.cols());
}
#endif
// Math functions applicable to both real and complex types.

#if defined(RWSTDLIBTYPE) && !defined(_MSC_VER) && !defined(RW_RWSTDLIB_USES_MSVC_CMATH) && !defined(__linux__)
inline RWSymMat<float>  log10(const RWSymMat<float>& A)
{
    return A.apply(std::log10);
}
#else

/**
 * @relates RWSymMat
 *
 * @copydoc acos(const RWSymMat<float>&)
 */
inline RWSymMat<float>  log10(const RWSymMat<float>& A)
{
    return A.apply(::log10);
}
#endif
#if defined(RWSTDLIBTYPE) && !defined(_MSC_VER) && !defined(RW_RWSTDLIB_USES_MSVC_CMATH) && !defined(__linux__)
inline RWSymMat<float>  acos(const RWSymMat<float>& A)
{
    return A.apply(std::acos);
}
inline RWSymMat<float>  asin(const RWSymMat<float>& A)
{
    return A.apply(std::asin);
}
inline RWSymMat<float>  atan(const RWSymMat<float>& A)
{
    return A.apply(std::atan);
}
inline RWSymMat<float>  tan(const RWSymMat<float>& A)
{
    return A.apply(std::tan);
}
inline RWSymMat<float>  tanh(const RWSymMat<float>& A)
{
    return A.apply(std::tanh);
}
#else

/**
 * @relates RWSymMat
 *
 * Returns a matrix where each element is formed by applying the
 * appropriate function to each element of the parameter matrix.
 */
inline RWSymMat<float>  acos(const RWSymMat<float>& A)
{
    return A.apply(::acos);
}

/**
 * @relates RWSymMat
 *
 * @copydoc acos(const RWSymMat<float>&)
 */
inline RWSymMat<float>  asin(const RWSymMat<float>& A)
{
    return A.apply(::asin);
}

/**
 * @relates RWSymMat
 *
 * @copydoc acos(const RWSymMat<float>&)
 */
inline RWSymMat<float>  atan(const RWSymMat<float>& A)
{
    return A.apply(::atan);
}

/**
 * @relates RWSymMat
 *
 * @copydoc acos(const RWSymMat<float>&)
 */
inline RWSymMat<float>  tan(const RWSymMat<float>& A)
{
    return A.apply(::tan);
}

/**
 * @relates RWSymMat
 *
 * @copydoc acos(const RWSymMat<float>&)
 */
inline RWSymMat<float>  tanh(const RWSymMat<float>& A)
{
    return A.apply(::tanh);
}
#endif
#if defined(RWSTDLIBTYPE) && !defined(_MSC_VER) && !defined(RW_RWSTDLIB_USES_MSVC_CMATH) && !defined(__linux__)
inline RWSymMat<float>  ceil(const RWSymMat<float>& A)
{
    return A.apply(std::ceil);
}
inline RWSymMat<float>  floor(const RWSymMat<float>& A)
{
    return A.apply(std::floor);
}
#else

/**
 * @relates RWSymMat
 *
 * Returns a matrix where each element in the matrix is the smallest
 * integer greater than or equal to the corresponding entry in the
 * parameter matrix.
 */
inline RWSymMat<float>  ceil(const RWSymMat<float>& A)
{
    return A.apply(::ceil);
}

/**
 * @relates RWSymMat
 *
 * Returns a matrix where each element in the matrix is the largest
 * integer smaller than or equal to the corresponding entry in the
 * parameter matrix.
 */
inline RWSymMat<float>  floor(const RWSymMat<float>& A)
{
    return A.apply(::floor);
}
#endif

/**
 * @relates RWSymMat
 *
 * Returns a matrix where each element is formed by applying the
 * appropriate function to corresponding elements of the parameter matrices.
 */
RWSymMat<float>  atan2(const RWSymMat<float>&, const RWSymMat<float>&);


#if defined(RWSTDLIBTYPE) && !defined(_MSC_VER) && !defined(RW_RWSTDLIB_USES_MSVC_CMATH) && !defined(__linux__)
inline RWSymMat<double>  log10(const RWSymMat<double>& A)
{
    return A.apply(std::log10);
}
#else

/**
 * @relates RWSymMat
 *
 * @copydoc acos(const RWSymMat<float>&)
 */
inline RWSymMat<double>  log10(const RWSymMat<double>& A)
{
    return A.apply(::log10);
}
#endif
#if defined(RWSTDLIBTYPE) && !defined(_MSC_VER) && !defined(RW_RWSTDLIB_USES_MSVC_CMATH) && !defined(__linux__)
inline RWSymMat<double>  acos(const RWSymMat<double>& A)
{
    return A.apply(std::acos);
}
inline RWSymMat<double>  asin(const RWSymMat<double>& A)
{
    return A.apply(std::asin);
}
inline RWSymMat<double>  atan(const RWSymMat<double>& A)
{
    return A.apply(std::atan);
}
inline RWSymMat<double>  tan(const RWSymMat<double>& A)
{
    return A.apply(std::tan);
}
inline RWSymMat<double>  tanh(const RWSymMat<double>& A)
{
    return A.apply(std::tanh);
}
#else

/**
 * @relates RWSymMat
 *
 * @copydoc acos(const RWSymMat<float>&)
 */
inline RWSymMat<double>  acos(const RWSymMat<double>& A)
{
    return A.apply(::acos);
}

/**
 * @relates RWSymMat
 *
 * @copydoc acos(const RWSymMat<float>&)
 */
inline RWSymMat<double>  asin(const RWSymMat<double>& A)
{
    return A.apply(::asin);
}

/**
 * @relates RWSymMat
 *
 * @copydoc acos(const RWSymMat<float>&)
 */
inline RWSymMat<double>  atan(const RWSymMat<double>& A)
{
    return A.apply(::atan);
}

/**
 * @relates RWSymMat
 *
 * @copydoc acos(const RWSymMat<float>&)
 */
inline RWSymMat<double>  tan(const RWSymMat<double>& A)
{
    return A.apply(::tan);
}

/**
 * @relates RWSymMat
 *
 * @copydoc acos(const RWSymMat<float>&)
 */
inline RWSymMat<double>  tanh(const RWSymMat<double>& A)
{
    return A.apply(::tanh);
}
#endif
#if defined(RWSTDLIBTYPE) && !defined(_MSC_VER) && !defined(RW_RWSTDLIB_USES_MSVC_CMATH) && !defined(__linux__)
inline RWSymMat<double>  ceil(const RWSymMat<double>& A)
{
    return A.apply(std::ceil);
}
inline RWSymMat<double>  floor(const RWSymMat<double>& A)
{
    return A.apply(std::floor);
}
#else

/**
 * @relates RWSymMat
 *
 * @copydoc ceil(const RWSymMat<float>&)
 */
inline RWSymMat<double>  ceil(const RWSymMat<double>& A)
{
    return A.apply(::ceil);
}

/**
 * @relates RWSymMat
 *
 * @copydoc floor(const RWSymMat<float>&)
 */
inline RWSymMat<double>  floor(const RWSymMat<double>& A)
{
    return A.apply(::floor);
}
#endif

/**
 * @relates RWSymMat
 *
 * @copydoc atan2(const RWSymMat<float>&,const RWSymMat<float>&)
 */
RWSymMat<double>  atan2(const RWSymMat<double>&, const RWSymMat<double>&);

/*
 * Inline functions
 */
template <class TypeT>
inline TypeT RWSymMat<TypeT>::val(int i, int j) const
{
#ifdef RWBOUNDS_CHECK
    return bcval(i, j);
#else
    if (i > j) {
        int temp = i;
        i = j;
        j = temp;
    }
    return vec(j * (j + 1) / 2 + i);
#endif
}
template <class TypeT>
inline TypeT RWSymMat<TypeT>::set(int i, int j, TypeT x)
{
    return bcset(i, j, x);
}
template <class TypeT>
inline TypeT& RWSymMat<TypeT>::ref(int i, int j)
{
#ifdef RWBOUNDS_CHECK
    return bcref(i, j);
#else
    if (i > j) {
        int temp = i;
        i = j;
        j = temp;
    }
    return vec(j * (j + 1) / 2 + i);
#endif
}
template <class TypeT>
inline TypeT& RWSymMat<TypeT>::operator()(int i, int j)
{
    return ref(i, j);
}
template <class TypeT>
inline TypeT RWSymMat<TypeT>::operator()(int i, int j) const
{
    return val(i, j);
}

#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/symmat.cc>
#endif

#endif
