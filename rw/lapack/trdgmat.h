#ifndef __RWTRDGMAT_H__
#define __RWTRDGMAT_H__

/*
 * $Id: //lapack/13/rw/lapack/trdgmat.h#1 $
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
class RWBandMat;

/**
 * @ingroup sparse_matrix_classes
 * @brief Encapsulates tridiagonal matrices.
 *
 * The class \link RWTriDiagMat RWTriDiagMat<T> \endlink encapsulates
 * tridiagonal matrices. A tridiagonal matrix is nonzero only on the
 * diagonal, the subdiagonal, and the superdiagonal. It is a banded
 * matrix with upper and lower bandwidth equal to 1.
 *
 * @section RWTriDiagMat_synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/trdgmat.h>
 *
 * RWTriDiagMat<double> A;
 * @endcode
 *
 * @section RWTriDiagMat_example Example
 *
 * @code
 * #include <rw/lapack/trdgmat.h>
 *
 * int main()
 * {
 *     RWTriDiagMat<float> m(5,5);
 *     m.diagonal() = 1;
 *     m.leadingSubmatrix(3).zero();
 *
 *     return 0;
 * }
 * @endcode
 *
 * @section RWTriDiagMat_storage_scheme Storage Scheme
 *
 * A tridiagonal matrix is nonzero only along the main diagonal, the
 * subdiagonal, and the superdiagonal:
 *
 * \f[
 * \begin{bmatrix}
 * A_{11} & A_{12} & 0      & 0      & ...    &        &        &          \\
 * A_{21} & A_{22} & A_{23} & 0      & 0      & ...    &        &          \\
 * 0      & A_{32} & A_{33} & A_{34} & 0      & 0      & ...    &          \\
 * 0      & 0      & A_{43} & A_{44} & A_{45} & 0      & 0      & ...      \\
 * 0      & 0      & 0      & A_{54} & A_{55} & A_{56} & 0      & 0        \\
 * .      &        &        &        &        &        &        &          \\
 * .      &        &        &        &        &        &        &          \\
 * .      &        &        &        &        &        &        &          \\
 * \end{bmatrix}
 * \f]
 *
 * The matrix is stored in an analogous way to the banded matrix. For
 * convenience, there are some unused locations left in the vector of
 * data. These are indicated as XXX in the following illustration of
 * the storage vector:
 *
 * <tt>[ XXX A<sub>11</sub> A<sub>21</sub>
 *           A<sub>12</sub> A<sub>22</sub> A<sub>32</sub>
 *           A<sub>23</sub> A<sub>33</sub> A<sub>43</sub>
 *           A<sub>34</sub> A<sub>44</sub> A<sub>54</sub>
 *           ...
 *           A<sub>nn</sub> XXX ]
 * </tt>
 *
 * The mapping between the array and storage vector is as follows:
 *
 * \f[
 * A(i+1,j+1) \rightarrow vec [ i + 1 + j*2 ]
 * \f]
 */
template <class TypeT>
class RWTriDiagMat
{
private:

    // The data which define the matrix
    RWMathVec<TypeT> vec;
    unsigned n;

    // The value that is returned when accessing the non-stored zero entries.
    static TypeT zero_;

    // Initialize entries in the data vector which are not used to zero
    void zeroUnusedEntries();

public:

    static const char* className; // For use in error messages


    /**
     * Default constructor. Builds a matrix of size 0 x 0. This constructor
     * is necessary to declare a matrix with no explicit constructor
     * or to declare an array of matrices.
     */
    RWTriDiagMat();

    /**
     * Defines an uninitialized matrix of size \a n x \a n.
     *
     * @note This constructor is used, rather than a constructor that takes
     * only a single argument, to avoid type conversion problems. Both
     * arguments must be equal or a runtime error occurs.
     */
    RWTriDiagMat(unsigned n, unsigned nAgain);

    /**
     * Constructs a size \a n x \a n matrix using the data in the passed
     * vector. This data must be stored in the format described in the
     * \ref RWTriDiagMat_storage_scheme section. The resultant
     * matrix references the data in vector \a data.
     *
     * @note This constructor is used, rather than a constructor that takes
     * only a single argument, to avoid type conversion problems. The last
     * two arguments must be equal or a runtime error occurs.
     */
    RWTriDiagMat(const RWMathVec<TypeT>& data, unsigned n, unsigned nAgain);

    /**
     * Builds a copy of its argument, \a rhs. Note that the new matrix
     * references the data of \a rhs. To construct a matrix with its own copy
     * of the data, you can use either the copy() or deepenShallowCopy()
     * member functions.
     */
    RWTriDiagMat(const RWTriDiagMat<TypeT>& rhs);

    /**
     * Constructs a complex matrix from the real part
     * supplied.  The imaginary part is assumed to be 0.
     */
    RW_LAPK_MEMBER_EXPORT RWTriDiagMat(const typename rw_linear_algebra_traits<TypeT>::generic_tri_diag_mat& re);

    /**
     * Constructs a complex matrix from the real and imaginary parts
     * supplied.
     */
    RW_LAPK_MEMBER_EXPORT RWTriDiagMat(const RWTriDiagMat<double>& re, const RWTriDiagMat<double>& im);

    /**
     * Destructor.
     */
    ~RWTriDiagMat();

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
     * Returns the bandwidth of the matrix. The bandwidth of a tridiagonal
     * matrix is always 3.
     */
    inline unsigned bandwidth() const {
        return 3;
    }

    /**
     * Returns the lower bandwidth of the matrix. The lower bandwidth
     * of a tridiagonal matrix is always 1.
     */
    inline unsigned lowerBandwidth() const {
        return 1;
    }

    /**
     * Returns the upper bandwidth of the matrix. The upper bandwidth
     * of a tridiagonal matrix is always 1.
     */
    inline unsigned upperBandwidth() const {
        return 1;
    }

    /**
     * Returns the half bandwidth of the matrix. The half bandwidth
     * of a tridiagonal matrix is always 1.
     */
    inline unsigned halfBandwidth() const {
        return 1;
    }

    /**
     * Returns the value of the \a ij <sup>th</sup> element of the matrix.
     * Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined when the header file is read. The member function
     * bcval() does the same thing with guaranteed bounds checking.
     */
    inline TypeT          val(int i, int j) const;

    /**
     * Sets the \a ij <sup>th</sup>element of the matrix equal to \a x.
     * Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined when the header file is read. The member function
     * bcset() does the same thing with guaranteed bounds checking.
     */
    inline TypeT          set(int i, int j, TypeT x);

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
    TypeT                 bcval(int i, int j) const;

    /**
     * Sets the \a ij <sup>th</sup> element of the matrix equal to \a x,
     * after doing bounds checking.
     */
    TypeT                 bcset(int i, int j, TypeT x);

    /**
     * Returns a reference to the \a ij <sup>th</sup> element of the
     * matrix, after doing bounds checking.
     */
    RWRORef<TypeT>        bcref(int i, int j);

    /**
     * Returns the \a k x \a k upper left corner of the matrix. The
     * submatrix and the matrix share the same data.
     */
    RWTriDiagMat<TypeT> leadingSubmatrix(int k);

    /**
     * Returns a reference to the \a j <sup>th</sup> diagonal of the
     * matrix. The main diagonal is indexed by 0, the super diagonal
     * by 1, and the subdiagonal by -1. Bounds checking is done if the
     * preprocessor symbol \c RWBOUNDS_CHECK is defined when the header
     * file is read. The member function bcdiagonal() does the same
     * thing with guaranteed bounds checking.
     */
    inline RWMathVec<TypeT> diagonal(int j = 0) const;

    /**
     * Returns a reference to the \a j <SUP>th</SUP> diagonal of the
     * matrix, after doing bounds checking. The main diagonal is indexed
     * by 0, the super diagonal by 1, and the subdiagonal by -1.
     */
    RWMathVec<TypeT> bcdiagonal(int j = 0) const;

    /**
     * Makes this matrix a reference to the matrix \a m. The two
     * matrices share the same data. The matrices do <i>not</i> have
     * to be the same size before calling reference(). To copy a matrix
     * into another of the same size, use operator=().
     */
    RWTriDiagMat<TypeT>& reference(RWTriDiagMat<TypeT>& m);

    /**
     * Sets every element of the matrix to 0.
     */
    void zero() {
        vec = TypeT(0);
    }

    /**
     * Creates a copy of this matrix with distinct data. The stride
     * of the data vector in the new matrix is guaranteed to be 1.
     */
    RWTriDiagMat<TypeT> copy() const;

    /**
     * Creates a copy of this matrix with distinct data. The stride
     * of the data vector in the new matrix is guaranteed to be 1.
     */
    RWTriDiagMat<TypeT> deepCopy() const {
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
     *  4    1    0    0
     * -5    9    2    0
     *  0   -5    3    9
     *  0    0    4    3
     * ]
     * @endcode
     */
    void scanFrom(std::istream& is); // Human readable

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
    void restoreFrom(RWvistream& is); // Internal US-ASCII or binary code

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
    inline TypeT  operator()(int i, int j) const;

    /**
     * @internal
     * Converts RWTriDiagMat into an RWGenMat.
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
    bool operator==(const RWTriDiagMat<TypeT>& X);

    /**
     * Inequality operator. Returns \c false if two matrices have the same
     * size and their elements are all exactly the same. Otherwise this
     * function returns \c true. Be aware that floating point arithmetic
     * is <i>not</i> exact; matrices that are theoretically equal are
     * <i>not</i> always numerically equal.
     */
    bool operator!=(const RWTriDiagMat<TypeT>& X) {
        return !(operator==(X));
    }

    /**
     * Sets the matrix elements equal to the elements of \a A. The two
     * matrices must be the same size. To make the matrix reference
     * the same data as \a A, use reference().
     */
    RWTriDiagMat<TypeT>& operator=(const RWTriDiagMat<TypeT>& A);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     */
    RWTriDiagMat<TypeT>& operator+=(const RWTriDiagMat<TypeT>& m);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     */
    RWTriDiagMat<TypeT>& operator-=(const RWTriDiagMat<TypeT>& m);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     * This function does element-by-element multiplication, <i>not</i>
     * inner product style matrix multiplication. You can use the product()
     * global function to do matrix-matrix inner product multiplication.
     */
    RWTriDiagMat<TypeT>& operator*=(const RWTriDiagMat<TypeT>& m);

    /**
     * Performs the indicated operation on each element of the matrix.
     */
    RWTriDiagMat<TypeT>& operator*=(TypeT);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     */
    RWTriDiagMat<TypeT>& operator/=(const RWTriDiagMat<TypeT>& m);

    /**
     * Performs the indicated operation on each element of the matrix.
     */
    RWTriDiagMat<TypeT>& operator/=(TypeT);

};

/****************************************************************************
 *
 * RWTriDiagMat<TypeT> Inline Functions
 *
 ***************************************************************************/
template <class TypeT>
inline TypeT RWTriDiagMat<TypeT>::val(int i, int j) const
{
#ifdef RWBOUNDS_CHECK
    return bcval(i, j);
#else
    return ((i - j > 1) || (i - j < (-1))) ? TypeT(0) : vec(i + 1 + j * 2);
#endif
}

template <class TypeT>
inline TypeT RWTriDiagMat<TypeT>::set(int i, int j, TypeT x)
{
#ifdef RWBOUNDS_CHECK
    return bcset(i, j, x);
#else
    return vec(i + 1 + j * 2) = x;
#endif
}

template <class TypeT>
inline RWRORef<TypeT> RWTriDiagMat<TypeT>::ref(int i, int j)
{
    return bcref(i, j);
}

template <class TypeT>
inline RWRORef<TypeT> RWTriDiagMat<TypeT>::operator()(int i, int j)
{
    return ref(i, j);
}

template <class TypeT>
inline TypeT RWTriDiagMat<TypeT>::operator()(int i, int j) const
{
    return val(i, j);
}

template <class TypeT>
inline RWMathVec<TypeT> RWTriDiagMat<TypeT>::diagonal(int i) const
{
#if defined(RWBOUNDS_CHECK)
    return bcdiagonal(i);
#else
    int iabs = (i >= 0) ? i : -i;
    return vec.slice(i > 0 ? 1 + i * 2 : 1 + iabs, static_cast<size_t>(n) - iabs, 3);
#endif
}




/****************************************************************************
 *
 * Global Functions
 *
 ***************************************************************************/

// This function returns a new matrix the same shape as the one
// passed in but with the data passed in.  This way we restrict
// all the >if statements to a couple lines
template <class TypeT>
inline RWTriDiagMat<TypeT> sameShapeMatrix(RWMathVec<TypeT>& vec, const RWTriDiagMat<TypeT>& A)
{
    return RWTriDiagMat<TypeT>(vec, A.rows(), A.cols());
}

inline RWTriDiagMat<double> sameShapeRealMatrix(RWMathVec<double>& vec, const RWTriDiagMat<DComplex>& A)
{
    return RWTriDiagMat<double>(vec, A.rows(), A.cols());
}


/**
 * @relates RWTriDiagMat
 *
 * Unary minus operator. Returns the negation of the matrix \a m.
 */
template <class TypeT>
RWTriDiagMat<TypeT>  operator-(const RWTriDiagMat<TypeT>& m);        // Unary minus

/**
 * @relates RWTriDiagMat
 *
 * Unary plus operator. Returns a copy of the matrix \a m.
 */
template <class TypeT>
RWTriDiagMat<TypeT>  operator+(const RWTriDiagMat<TypeT>& m);        // Unary plus

/**
 * @relates RWTriDiagMat
 *
 * Performs element-by-element operations on the arguments. To do
 * inner product matrix multiplication, you can use the product()
 * global function.
 */
template <class TypeT>
RWTriDiagMat<TypeT>  operator*(const RWTriDiagMat<TypeT>&, const RWTriDiagMat<TypeT>&);

/**
 * @relates RWTriDiagMat
 *
 * Performs element-by-element division on the arguments.
 */
template <class TypeT>
RWTriDiagMat<TypeT>  operator/(const RWTriDiagMat<TypeT>&, const RWTriDiagMat<TypeT>&);

/**
 * @relates RWTriDiagMat
 *
 * Performs element-by-element addition on the arguments.
 */
template <class TypeT>
RWTriDiagMat<TypeT>  operator+(const RWTriDiagMat<TypeT>&, const RWTriDiagMat<TypeT>&);

/**
 * @relates RWTriDiagMat
 *
 * Performs element-by-element subtraction on the arguments.
 */
template <class TypeT>
RWTriDiagMat<TypeT>  operator-(const RWTriDiagMat<TypeT>&, const RWTriDiagMat<TypeT>&);

/**
 * @relates RWTriDiagMat
 *
 * Performs element-by-element multiplication on the arguments.
 */
template <class TypeT>
inline RWTriDiagMat<TypeT>  operator*(const RWTriDiagMat<TypeT>& A, TypeT x)
{
    RWMathVec<TypeT> temp(A.dataVec()*x);
    return sameShapeMatrix(temp, A);
}

#ifndef RW_NO_INLINED_TEMP_DESTRUCTORS

/**
 * @relates RWTriDiagMat
 *
 * Performs element-by-element multiplication on the arguments.
 */
template <class TypeT>
inline RWTriDiagMat<TypeT>  operator*(TypeT x, const RWTriDiagMat<TypeT>& A)
{
    return A * x;
}

#else

template <class TypeT>
RWTriDiagMat<TypeT>  operator*(TypeT x, const RWTriDiagMat<TypeT>& A);

#endif

/**
 * @relates RWTriDiagMat
 *
 * Performs element-by-element division on the arguments.
 */
template <class TypeT>
RWTriDiagMat<TypeT>  operator/(const RWTriDiagMat<TypeT>& A, TypeT x);


/**
 * @relates RWTriDiagMat
 *
 * Writes the matrix to the stream. This is equivalent to calling the
 * \link RWTriDiagMat::printOn() printOn() \endlink member function.
 */
template <class TypeT>
inline std::ostream&  operator<<(std::ostream& s, const RWTriDiagMat<TypeT>& m)
{
    m.printOn(s);
    return s;
}

/**
 * @relates RWTriDiagMat
 *
 * Reads the matrix from the stream. This is equivalent to calling the
 * \link RWTriDiagMat::scanFrom() scanFrom() \endlink member function.
 */
template <class TypeT>
inline std::istream&  operator>>(std::istream& s, RWTriDiagMat<TypeT>& m)
{
    m.scanFrom(s);
    return s;
}


/**
 * @relates RWTriDiagMat
 *
 * Returns the transpose of the argument matrix. The transpose is
 * made to reference the same data as the argument matrix.
 */
template <class TypeT>
RWTriDiagMat<TypeT>  transpose(const RWTriDiagMat<TypeT>&);

/**
 * @relates RWTriDiagMat
 *
 * Returns the inner product (matrix-vector product) of \a A and
 * \a x.
 */
template <class TypeT>
RWMathVec<TypeT>  product(const RWTriDiagMat<TypeT>& A, const RWMathVec<TypeT>& x);

/**
 * @relates RWTriDiagMat
 *
 * Returns the inner product (matrix-vector product) of \a x and
 * \a A. This is equal to the product of \a A transpose and \a x.
 */
template <class TypeT>
RWMathVec<TypeT>  product(const RWMathVec<TypeT>& x, const RWTriDiagMat<TypeT>& A);

// Below are functions that assume automatic conversion to GenMat's
// Automatic conversions do not take place for templatized functions,
// So we provide them here...
template <class TypeT>
RWGenMat<TypeT>  product(const RWTriDiagMat<TypeT>& A, const RWGenMat<TypeT>& B);

template <class TypeT>
RWGenMat<TypeT>  product(const RWGenMat<TypeT>& A, const RWTriDiagMat<TypeT>& B);

template <class TypeT>
RWGenMat<TypeT>  product(const RWTriDiagMat<TypeT>& A, const RWTriDiagMat<TypeT>& B);

template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWTriDiagMat<TypeT>& A, const RWGenMat<TypeT>& B);

template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWGenMat<TypeT>& A, const RWTriDiagMat<TypeT>& B);

template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWTriDiagMat<TypeT>& A, const RWTriDiagMat<TypeT>& B);

template <class TypeT>
inline RWGenMat<TypeT>  adjoint(const RWTriDiagMat<TypeT>& A)
{
    RWGenMat<TypeT> a(A);
    return adjoint(a);
}

/**
 * @relates RWTriDiagMat
 *
 * Extracts the tridiagonal part of a square matrix. The tridiagonal
 * part of a matrix \c A consists of the main diagonal, the subdiagonal,
 * and the superdiagonal.
 */
template <class TypeT>
RWTriDiagMat<TypeT>  toTriDiagMat(const RWGenMat<TypeT>& A);


/**
 * @relates RWBandMat
 *
 * Extracts the tridiagonal part of a general banded matrix. The
 * tridiagonal part of matrix \a A consists of the main diagonal,
 * the subdiagonal, and the superdiagonal.
 */
template <class TypeT>
RWTriDiagMat<TypeT>  toTriDiagMat(const RWBandMat<TypeT>& A);


#if !defined(_MSC_VER) && !defined(DOXYGEN)

template <class TypeT>
/*inline*/ RWTriDiagMat<typename rw_numeric_traits<TypeT>::norm_type>  abs(const RWTriDiagMat<TypeT>& A)
{
    RWMathVec<typename rw_numeric_traits<TypeT>::norm_type> temp(abs(A.dataVec()));
    return sameShapeMatrix(temp, A);
}
//specialize for DComplex:
#  if !defined(__hppa)
template <>
#  endif
inline RWTriDiagMat<rw_numeric_traits<DComplex>::norm_type>  abs(const RWTriDiagMat<DComplex>& A)
{
    RWMathVec<rw_numeric_traits<DComplex>::norm_type> temp(abs(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

#else //using MSVC compiler or doxygen

/**
 * @relates RWTriDiagMat
 *
 * Returns a matrix whose entries are the absolute value of the
 * argument. The absolute value of a complex number is considered
 * to be the sum of the absolute values of its real and imaginary
 * parts. To get the norm of a complex matrix, you can use the
 * norm() function.
 */
template <class TypeT>
inline RWTriDiagMat<typename rw_numeric_traits<TypeT>::norm_type>  abs(const RWTriDiagMat<TypeT>& M)
{
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;
    RWMathVec<rt> temp(abs(M.dataVec()));
    //return sameShapeMatrix(temp,M);
    //this function should call sameShape{Real}Matrix, and return the result of that function.
    // I'm putting code directly from those functions here -- MSVC5 cannot handle complicated template specializations -- till a better solution comes up
    return RWTriDiagMat<rt>(temp, M.rows(), M.cols());
};

#endif // !defined(_MSC_VER)

/**
 * @relates RWTriDiagMat
 *
 * Returns a matrix where each element is the complex conjugate
 * of the corresponding element in the matrix \a A.
 */
inline RWTriDiagMat<DComplex>  conj(const RWTriDiagMat<DComplex>& A)
{
    RWMathVec<DComplex> temp(conj(A.dataVec()));
    return sameShapeMatrix(temp, A);
}

/**
 * @relates RWTriDiagMat
 *
 * Returns a matrix where each element is the real part of the corresponding
 * element in the matrix \a A.
 */
inline RWTriDiagMat<double>  real(const RWTriDiagMat<DComplex>& A)
{
    RWMathVec<double> temp(real(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWTriDiagMat
 *
 * Returns a matrix where each element is the imaginary part of
 * the corresponding element in the matrix \a A.
 */
inline RWTriDiagMat<double>  imag(const RWTriDiagMat<DComplex>& A)
{
    RWMathVec<double> temp(imag(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWTriDiagMat
 *
 * Returns a matrix where each element is the norm (magnitude) of
 * the corresponding element in the matrix \a A.
 */
inline RWTriDiagMat<double>  norm(const RWTriDiagMat<DComplex>& A)
{
    RWMathVec<double> temp(norm(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWTriDiagMat
 *
 * Returns a matrix where each element is the argument of the corresponding
 * element in the matrix \a A.
 */
inline RWTriDiagMat<double>  arg(const RWTriDiagMat<DComplex>& A)
{
    RWMathVec<double> temp(arg(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

inline RWGenMat<DComplex>  conjTransposeProduct(const RWTriDiagMat<DComplex>& A, const RWGenMat<DComplex>& B)
{
    return conjTransposeProduct(RWGenMat<DComplex>(A), B);
}

inline RWGenMat<DComplex>  conjTransposeProduct(const RWGenMat<DComplex>& A, const RWTriDiagMat<DComplex>& B)
{
    return conjTransposeProduct(A, RWGenMat<DComplex>(B));
}

inline RWGenMat<DComplex>  conjTransposeProduct(const RWTriDiagMat<DComplex>& A, const RWTriDiagMat<DComplex>& B)
{
    return conjTransposeProduct(RWGenMat<DComplex>(A), RWGenMat<DComplex>(B));
}

/**
 * @relates RWTriDiagMat
 *
 * Returns the minimum entry in the matrix.
 */
inline double  minValue(const RWTriDiagMat<double>& A)
{
    return minValue(A.dataVec());
}

/**
 * @relates RWTriDiagMat
 *
 * Returns the maximum entry in the matrix.
 */
inline double  maxValue(const RWTriDiagMat<double>& A)
{
    return maxValue(A.dataVec());
}

/**
 * @relates RWTriDiagMat
 *
 * @copydoc minValue(const RWTriDiagMat<double>&)
 */
inline float   minValue(const RWTriDiagMat<float>& A)
{
    return minValue(A.dataVec());
}

/**
 * @relates RWTriDiagMat
 *
 * @copydoc maxValue(const RWTriDiagMat<double>&)
 */
inline float   maxValue(const RWTriDiagMat<float>& A)
{
    return maxValue(A.dataVec());
}


#if defined(RW_MATH_COMPILE_INSTANTIATE)
#  include <rw/lapack/trdgmat.cc>
#endif

#endif
