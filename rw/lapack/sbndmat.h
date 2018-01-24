
#ifndef __RWSBNDMAT_H__
#define __RWSBNDMAT_H__
/*
 * RWSymBandMat<TypeT> - A symmetric banded matrix of Ts
 *
 * Stores a symmetric banded matrix.  All entries farther above or below
 * the diagonal that the upper bandwidth
 * are defined to be zero.  The total bandwidth is twice the
 * upperBandwidth, plus one for the diagonal.  Only
 * the elements which may be non-zero are stored.
 *
 * The top half of the matrix is stored column by column.  Each
 * column takes up
 * <upperBandwidth+1> entries, some of these entries near the top left
 * corner are not used.
 *
 * Bounds checking is enabled by setting the preprocessor
 * symbol RWBOUNDS_CHECK.  This is strongly encouraged.
 *
 ***************************************************************************
 * $Id: //lapack/13/rw/lapack/sbndmat.h#1 $
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
class RWBandMat;

/**
 * @ingroup sparse_matrix_classes
 * @brief Encapsulates a symmetric band matrix.
 *
 * The class \link RWSymBandMat RWSymBandMat<T> \endlink encapsulates
 * symmetric band matrices. A symmetric banded matrix is symmetric,
 * and nonzero only near the diagonal. Specifically, if \e h
 * is the half bandwidth, then any entries for which \f$ i-j > h \f$
 * or  \f$ j-i > h \f$ are defined to be 0; in addition, an entry
 * \f$ A_{ij} = A_{ji} \f$.
 *
 * @section RWSymBandMat_synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/sbndmat.h>
 *
 * RWSymBandMat<double> d;
 * @endcode

 * @section RWSymBandMat_example Example
 *
 * @code
 * #include <rw/lapack/sbndmat.h>
 *
 * int main()
 * {
 *     RWSymBandMat<float> SB(4,4,1);
 *     SB.diagonal(-1) = -1;
 *     SB.diagonal( ) = 0;
 *     SB.diagonal( 1) = 1;
 * }
 * @endcode
 *
 * @section RWSymBandMat_storage_scheme Storage Scheme
 *
 * As an example of the storage scheme, consider the following
 * 9 x 9 matrix with a half bandwidth of two:
 *
 * \f[
 * \begin{bmatrix}
 * & A_{11} & A_{12}   & A_{13}  & 0       & 0       & 0       & 0       & 0       & 0      \\
 * & A_{12} & A_{22}   & A_{23}  & A_{24}  & 0       & 0       & 0       & 0       & 0      \\
 * & A_{13} & A_{23}   & A_{33}  & A_{34}  & A_{35}  & 0       & 0       & 0       & 0      \\
 * & 0      & A_{24}   & A_{34}  & A_{44}  & A_{45}  & A_{46}  & 0       & 0       & 0      \\
 * & 0      & 0        & A_{35}  & A_{45}  & A_{55}  & A_{56}  & A_{57}  & 0       & 0      \\
 * & 0      & 0        & 0       & A_{46}  & A_{56}  & A_{66}  & A_{67}  & A_{68}  & 0      \\
 * & 0      & 0        & 0       & 0       & A_{57}  & A_{67}  & A_{77}  & A_{78}  & A_{79} \\
 * & 0      & 0        & 0       & 0       & 0       & A_{68}  & A_{78}  & A_{88}  & A_{89} \\
 * & 0      & 0        & 0       & 0       & 0       & 0       & A_{79}  & A_{89}  & A_{99} \\
 * \end{bmatrix}
 * \f]
 *
 * The upper triangle is stored column by column. For convenience, there are some
 * unused locations left in the vector of data. These are indicated as \c XXX
 * in the following illustration of the storage vector:
 *
 * <tt>[     XXX            XXX            A<sub>11</sub>
 *           XXX            A<sub>12</sub> A<sub>22</sub>
 *           A<sub>13</sub> A<sub>23</sub> A<sub>33</sub>
 *           A<sub>24</sub> A<sub>34</sub> A<sub>44</sub>
 *           A<sub>35</sub> A<sub>45</sub> A<sub>55</sub>
 *           A<sub>46</sub> A<sub>56</sub> A<sub>66</sub>
 *           A<sub>57</sub> A<sub>67</sub> A<sub>77</sub>
 *           A<sub>68</sub> A<sub>78</sub> A<sub>88</sub>
 *           A<sub>79</sub> A<sub>89</sub> A<sub>99</sub>
 *     ]
 * </tt>
 *
 * The mapping between the array and storage vector is as follows:
 *
 * \f[
 * A(i + 1, j + 1) \to
 *   \left\{
 *     \begin{array}{l l}
 *        \text{vec}[i + \text{halfwidth} + j*\text{halfwidth}] & \quad
 *          \text{if  } j \geq i \\
 *        \text{vec}[j + \text{halfwidth} + i*\text{halfwidth}] & \quad
 *          \text{if  } j < i \\
 *     \end{array}
 *   \right.
 * \f]
 */
template <class TypeT>
class RWSymBandMat
{
private:
    // Private Instance Varialbes -----------------------------------------------
    // The data which define the matrix
    RWMathVec<TypeT> vec;
    unsigned n;
    unsigned bandu; // The upper bandwidth

    // Private Class Variables --------------------------------------------------
    static TypeT zero_; // The value returned when referencing zero elements.

    // Private Methods ----------------------------------------------------------
    // Initialize entries in the data vector which are not used to zero
    void zeroUnusedEntries();

public:
    // Public Constructors ------------------------------------------------------

    /**
     * Default constructor. Builds a matrix of size 0 x 0. This constructor
     * is necessary to declare a matrix with no explicit constructor
     * or to declare an array of matrices.
     */
    RWSymBandMat();

    /**
     * Build a copy of its argument, \a A. Note that the new matrix
     * references \a A's data. To construct a matrix with its own copy
     * of the data, you can use either the copy() or deepenShallowCopy()
     * member functions.
     */
    RWSymBandMat(const RWSymBandMat<TypeT>& A);

    /**
     * Defines an uninitialized matrix of size \a n x \a nAgain with half
     * bandwidth \a hb.
     */
    RWSymBandMat(unsigned n, unsigned nAgain, unsigned hb);

    /**
     * Constructs an \a n x \a nAgain matrix with half bandwidth \a hb using
     * the data in the passed vector. This data must be stored in the
     * format described in the \ref RWSymBandMat_storage_scheme section. The
     * resultant matrix references the data in  vector \a vd.
     */
    RWSymBandMat(const RWMathVec<TypeT>& vd, unsigned n, unsigned nAgain, unsigned hb);

    RW_LAPK_MEMBER_EXPORT RWSymBandMat(const typename rw_linear_algebra_traits<TypeT>::narrow_sym_band_mat& n);

    /**
     * Constructs a complex matrix from the real and imaginary parts
     * supplied. If no imaginary part is supplied, it is assumed to
     * be 0.
     */
    RW_LAPK_MEMBER_EXPORT RWSymBandMat(const RWSymBandMat<double>& re, const RWSymBandMat<double>& im);

    // Destructor ---------------------------------------------------------------
    ~RWSymBandMat();

    // Public Class Variables ---------------------------------------------------
    static const char* className; // For use in error messages


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

    /**
     * Returns the bandwidth of the matrix. The bandwidth is twice the
     * half bandwidth plus 1 for the main diagonal.
     */
    inline unsigned bandwidth() const {
        return 2 * bandu + 1;
    }

    /**
     * Returns the half bandwidth of the matrix.
     */
    inline unsigned halfBandwidth() const {
        return bandu;
    }

    /**
     * Returns the lower bandwidth of the matrix.
     */
    inline unsigned lowerBandwidth() const {
        return bandu;
    }

    /**
     * Returns the upper bandwidth of the matrix.
     */
    inline unsigned upperBandwidth() const {
        return bandu;
    }


    // Access and/or change elements of the matrix.
    // The versions that begin with the letters bc do bounds
    // checking.  The others do bounds checking only if the
    // preprocessor symbol RWBOUNDS_CHECK is defined.

    /**
     * Returns the value of the \a ij <sup>th</SUP> element of the matrix.
     * Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined when the header file is read. The member function
     * bcval() does the same thing with guaranteed bounds checking.
     */
    inline TypeT          val(int i, int j) const;

    /**
     * Sets the \a ij <sup>th</SUP> element of the matrix equal to \a
     * x. Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined when the header file is read. The member function
    *  bcset() does the same thing with guaranteed bounds checking.
     */
    inline TypeT          set(int i, int j, TypeT x);

    /**
     * Returns a reference to the \a ij <sup>th</SUP> element of the
     * matrix. Bounds checking is done if the preprocessor symbol
     * RWBOUNDS_CHECK is defined when the header file is read. The member
     * function bcref() does the same thing with guaranteed bounds
     * checking.
     */
    inline RWRORef<TypeT> ref(int i, int j);

    /**
     * Returns the value of the \a ij <sup>th</SUP> element of the matrix,
     * after doing bounds checking.
     */
    TypeT                 bcval(int i, int j) const;

    /**
     * Sets the \a ij <sup>th</SUP> element of the matrix equal to \a
     * x, after doing bounds checking.
     */
    TypeT                 bcset(int i, int j, TypeT x);

    /**
     * Returns a reference to the \a ij <sup>th</SUP> element of the
     * matrix, after doing bounds checking.
     */
    RWRORef<TypeT>        bcref(int i, int j);

    /**
     * Returns the \a k x \a k upper left corner of the matrix. The
     * submatrix and the matrix share the same data.
     */
    RWSymBandMat<TypeT> leadingSubmatrix(int k);

    // Access to diagonals.  The same conventions as for
    // element access apply (with respect to bcxxx and xxx names).

    /**
     * Returns a reference to the \a j <SUP>th</SUP> diagonal of the
     * matrix. The main diagonal is indexed by 0, diagonals in the upper
     * triangle are indexed with positive integers, and diagonals in
     * the lower triangle are indexed with negative integers. Bounds
     * checking is done if the preprocessor symbol \c RWBOUNDS_CHECK is
     * defined when the header file is read. The member function
     * bcdiagonal() does the same thing with guaranteed bounds checking.
     */
    inline RWMathVec<TypeT> diagonal(int j = 0) const;

    /**
     * Returns a reference to the \a j <SUP>th</SUP> diagonal of the
     * matrix, after doing bounds checking. The main diagonal is indexed
     * by 0, diagonals in the upper triangle are indexed with positive
     * integers, diagonals in the lower triangle are indexed with negative
     * integers.
     */
    RWMathVec<TypeT>        bcdiagonal(int j = 0) const;

    /**
     * Makes this matrix a reference to the argument matrix. The two
     * matrices  share the same data. The matrices do \e not have
     * to be the same size before calling reference(). To copy a matrix
     * into another of the same size, use the operator=() member operator.
     */
    RWSymBandMat<TypeT>& reference(RWSymBandMat<TypeT>& m);

    /**
     * Sets every element of the matrix to 0.
     */
    void zero() {
        vec = TypeT(0);
    }

    // copy() returns a RWSymBandMat<TypeT> with its own copy of the data.
    // deepenShallowCopy makes sure the data is not shared by another
    // vector or matrix.

    /**
     * Creates a copy of this matrix with distinct data. The stride
     * of the data vector in the new matrix is guaranteed to be 1.
     */
    RWSymBandMat<TypeT> copy() const;

    /**
     * Creates a copy of this matrix with distinct data. The stride
     * of the data vector in the new matrix is guaranteed to be 1.
     */
    RWSymBandMat<TypeT> deepCopy() const {
        return copy();
    }

    /**
     * Ensures that the data in the matrix is \e not shared by any
     * other matrix or vector. Also ensure that the stride in the data
     * vector is equal to 1. If necessary, a new copy of the data vector
     * is made.
     */
    void            deepenShallowCopy() {
        vec.deepenShallowCopy();
    }

    // Access to the raw data.

    /**
     * Returns the matrix's data vector. This is where the explicitly
     * stored entries in the matrix are kept.
     */
    RWMathVec<TypeT>        dataVec() {
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
    TypeT*                  data() {
        return vec.data();
    }

    const TypeT*            data() const {
        return vec.data();
    }


    /**
     * Resizes the matrix or changes both its size and half bandwidth.
     * Any new entries in the matrix are set to 0. Both arguments must
     * be the same.
     */
    void resize(unsigned m, unsigned n);

    /**
     * @copydoc resize(unsigned , unsigned )
     */
    void resize(unsigned n, unsigned nAgain, unsigned halfWidth);



    // Input/output member functions
    // Whether US-ASCII or binary formatting is used with readFrom and
    // storeOn is determined by the type of rwstream passed.

    /**
     * Reads a matrix from an input stream. The format of the matrix
     * is the same as the format output by the printOn() member function:
     * first the half bandwidth, followed by the matrix itself. Below
     * is a sample matrix showing the format. Note that extra white
     * space and text preceding the bandwidth specification are ignored.
     * Only the symmetric part of the relevant band of the matrix is
     * used.
     *
     * @code
     * 1  5x5
     * [
     *   3  7  0  0  0
     *   7  2  2  0  0
     *   0  2  8  9  0
     *   0  0  9  8  7
     *   0  0  0  7  8
     * ]
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

    // Public Member Operators --------------------------------------------------
    // Conversion to a general matrix
    operator RWGenMat<TypeT>() const;

    // Indexing Operators

    /**
     * Returns a reference to the \a ij <SUP>th</SUP> element of the
     * matrix.
     * Using this operator is equivalent to calling the ref() member function.
     * Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined before including the header file.
     */
    inline RWRORef<TypeT> operator()(int i, int j);

    /**
     * Accesses the \a ij <SUP>th</SUP> element.
     * Using this operator is equivalent to calling the val() member
     * function. Bounds checking is done if the preprocessor symbol \c
     * RWBOUNDS_CHECK is defined before including the header file.
     */
    inline TypeT          operator()(int i, int j) const;

    // Two matrices are considered equal if their shapes are identical and
    // all their components are identical.

    /**
     * Two matrices are considered equal if they
     * have the same size and their elements are all exactly the same.
     * Be aware that floating point arithmetic is \e not exact;
     * matrices that are theoretically equal are not always numerically
     * equal.
     */
    bool operator==(const RWSymBandMat<TypeT>& X);

    /**
     * @copydoc operator==(const RWSymBandMat<TypeT>&)
     */
    bool operator!=(const RWSymBandMat<TypeT>& X) {
        return !(operator==(X));
    }

    // assignment operators.  self must be same size as m.


    /**
     * Sets the matrix elements equal to the elements of \a A. The two
     * matrices must be the same size. To make the matrix reference
     * the same data as \a A, use the reference() member function.
     */
    RWSymBandMat<TypeT>& operator=(const RWSymBandMat<TypeT>& A);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     */
    RWSymBandMat<TypeT>& operator+=(const RWSymBandMat<TypeT>& m);

    /**
     * @copydoc operator+=(const RWSymBandMat<TypeT>&)
     */
    RWSymBandMat<TypeT>& operator-=(const RWSymBandMat<TypeT>& m);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     * @note \link operator*=(const RWSymBandMat<TypeT>&) operator*=() \endlink
     * does element-by-element multiplication, \e not inner product
     * style matrix multiplication.
     * You can use the product() global function to do matrix-matrix inner
     * product multiplication.
     */
    RWSymBandMat<TypeT>& operator*=(const RWSymBandMat<TypeT>& m);

    /**
     * Performs the indicated operation on each element of the matrix.
     */
    RWSymBandMat<TypeT>& operator*=(TypeT);

    /**
     * @copydoc operator+=(const RWSymBandMat<TypeT>&)
     */
    RWSymBandMat<TypeT>& operator/=(const RWSymBandMat<TypeT>& m);

    /**
     * @copydoc operator*=(TypeT)
     */
    RWSymBandMat<TypeT>& operator/=(TypeT);
};

/****************************************************************************
 *
 * RWSymBandMat<TypeT> inline functions defs
 *
 ***************************************************************************/
template <class TypeT>
inline TypeT RWSymBandMat<TypeT>::val(int i, int j) const
{
    return bcval(i, j);
}

template <class TypeT>
inline TypeT RWSymBandMat<TypeT>::set(int i, int j, TypeT x)
{
    return bcset(i, j, x);
}

template <class TypeT>
inline RWRORef<TypeT> RWSymBandMat<TypeT>::ref(int i, int j)
{
    return bcref(i, j);
}

template <class TypeT>
inline RWRORef<TypeT> RWSymBandMat<TypeT>::operator()(int i, int j)
{
    return ref(i, j);
}

template <class TypeT>
inline TypeT RWSymBandMat<TypeT>::operator()(int i, int j) const
{
    return val(i, j);
}

template <class TypeT>
inline RWMathVec<TypeT> RWSymBandMat<TypeT>::diagonal(int i) const
{
#if defined(RWBOUNDS_CHECK)
    return bcdiagonal(i);
#else
    int iabs = (i >= 0) ? i : -i;
    return vec.slice((iabs + 1) * (bandu), (size_t)n - iabs, bandu + 1);
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
inline RWSymBandMat<TypeT> sameShapeMatrix(RWMathVec<TypeT>& vec, const RWSymBandMat<TypeT>& A)
{
    return RWSymBandMat<TypeT>(vec, A.rows(), A.cols(), A.lowerBandwidth());
}

inline RWSymBandMat<double> sameShapeRealMatrix(RWMathVec<double>& vec, const RWSymBandMat<DComplex>& A)
{
    return RWSymBandMat<double>(vec, A.rows(), A.cols(), A.lowerBandwidth());
}

// Arithmetic operators; Notice that operator* is an
// element by element multiply, NOT a matrix multiply.

/**
 * @relates RWSymBandMat
 * Unary minus operator. Returns a copy of matrix's negation.
 */
template <class TypeT>
RWSymBandMat<TypeT>  operator-(const RWSymBandMat<TypeT>&);        // Unary minus

/**
 * @relates RWSymBandMat
 * Unary plus operator. Returns a copy of the matrix.
 */
template <class TypeT>
RWSymBandMat<TypeT>  operator+(const RWSymBandMat<TypeT>&);        // Unary plus

/**
 * @relates RWSymBandMat
 * Performs element-by-element operation on the arguments. To do
 * inner product matrix multiplication, you can use the product()
 * global function.
 */
template <class TypeT>
RWSymBandMat<TypeT>  operator*(const RWSymBandMat<TypeT>&, const RWSymBandMat<TypeT>&);

/**
 * @relates RWSymBandMat
 * @copydoc operator+(const RWSymBandMat<TypeT>&, const RWSymBandMat<TypeT>&);
 */
template <class TypeT>
RWSymBandMat<TypeT>  operator/(const RWSymBandMat<TypeT>&, const RWSymBandMat<TypeT>&);

/**
 * @relates RWSymBandMat
 * Performs element-by-element operation on the arguments.
 */
template <class TypeT>
RWSymBandMat<TypeT>  operator+(const RWSymBandMat<TypeT>&, const RWSymBandMat<TypeT>&);

/**
 * @relates RWSymBandMat
 * @copydoc operator+(const RWSymBandMat<TypeT>&, const RWSymBandMat<TypeT>&);
 */
template <class TypeT>
RWSymBandMat<TypeT>  operator-(const RWSymBandMat<TypeT>&, const RWSymBandMat<TypeT>&);


/**
 * @relates RWSymBandMat
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
inline RWSymBandMat<TypeT>  operator*(const RWSymBandMat<TypeT>& A, TypeT x)
{
    RWMathVec<TypeT> temp(A.dataVec()*x);
    return sameShapeMatrix(temp, A);
}
/**
 * @relates RWSymBandMat
 * @copydoc operator*(const RWSymBandMat<TypeT>&, TypeT)
 */
template <class TypeT>
#ifndef RW_NO_INLINED_TEMP_DESTRUCTORS
inline RWSymBandMat<TypeT>  operator*(TypeT x, const RWSymBandMat<TypeT>& A)
{
    return A * x;
}
#else
RWSymBandMat<TypeT>  operator*(TypeT x, const RWSymBandMat<TypeT>& A);
#endif

/**
 * @relates RWSymBandMat
 * @copydoc operator*(const RWSymBandMat<TypeT>&, TypeT)
 */
template <class TypeT>
RWSymBandMat<TypeT>  operator/(const RWSymBandMat<TypeT>& A, TypeT x);

// Stream I/O ---------------------------------------------------------------

/**
 * @relates RWSymBandMat
 * Writes the matrix to the stream. This is equivalent to calling
 * the printOn() member function.
 */
template <class TypeT>
inline std::ostream&  operator<<(std::ostream& s, const RWSymBandMat<TypeT>& m)
{
    m.printOn(s);
    return s;

}

/**
 * @relates RWSymBandMat
 * Reads the matrix from the stream. This is equivalent to calling
 * the scanFrom() member function.
 */
template <class TypeT>
inline std::istream&  operator>>(std::istream& s, RWSymBandMat<TypeT>& m)
{
    m.scanFrom(s);
    return s;
}

/**
 * @relates RWSymBandMat
 * Returns the transpose of the argument matrix.
 */
template <class TypeT>
RWSymBandMat<TypeT>  transpose(const RWSymBandMat<TypeT>&);

// inner products

/**
 * @relates RWSymBandMat
 * Returns the inner product (matrix-vector product) of \a A and
 * \a x.
 */
template <class TypeT>
RWMathVec<TypeT>  product(const RWSymBandMat<TypeT>& A, const RWMathVec<TypeT>& x);

/**
 * @relates RWSymBandMat
 * Returns the inner product (matrix-vector product) of \a x and
 * \a A. This is equal to the product of \a A transpose and \a x.
 */
template <class TypeT>
RWMathVec<TypeT>  product(const RWMathVec<TypeT>& x, const RWSymBandMat<TypeT>& A);

#if !(defined(_MSC_VER))

/**
 * @relates RWSymBandMat
 * Returns a matrix whose entries are the absolute value of the
 * argument. The absolute value of a complex number is considered
 * to be the sum of the absolute values of its real and imaginary
 * parts. To get the norm of a complex matrix, you can use the
 * norm() function.
 */
template <class TypeT>
inline RWSymBandMat<typename rw_numeric_traits<TypeT>::norm_type>  abs(const RWSymBandMat<TypeT>& A)
{
    RWMathVec<typename rw_numeric_traits<TypeT>::norm_type> temp(abs(A.dataVec()));
    return sameShapeMatrix(temp, A);
}
//specialize for DComplex:
#  if !(defined(__hppa))
template<>
#  endif
inline RWSymBandMat<rw_numeric_traits<DComplex>::norm_type>  abs(const RWSymBandMat<DComplex>& A)
{
    RWMathVec<rw_numeric_traits<DComplex>::norm_type> temp(abs(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

#else //using MSVC compiler:

template <class TypeT>
inline RWSymBandMat<typename rw_numeric_traits<TypeT>::norm_type>  abs(const RWSymBandMat<TypeT>& M)
{
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;
    RWMathVec<rt> temp(abs(M.dataVec()));
    //return sameShapeMatrix(temp,M);
    //this function should call sameShape{Real}Matrix, and return the result of that function.
    // I'm putting code directly from those functions here -- MSVC5 cannot handle complicated template specializations -- till a better solution comes up
    return RWSymBandMat<rt>(temp, M.rows(), M.cols(), M.lowerBandwidth());
};

#endif // !defined(_MSC_VER)

/**
 * @relates RWSymBandMat
 * Returns a matrix where each element is the complex conjugate
 * of the corresponding element in the matrix \a A.
 */
inline RWSymBandMat<DComplex>  conj(const RWSymBandMat<DComplex>& A)
{
    RWMathVec<DComplex> temp(conj(A.dataVec()));
    return sameShapeMatrix(temp, A);
}

/**
 * @relates RWSymBandMat
 * Returns a matrix where each element is the real part of the corresponding
 * element in the matrix \a A.
 */
inline RWSymBandMat<double>  real(const RWSymBandMat<DComplex>& A)
{
    RWMathVec<double> temp(real(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWSymBandMat
 * Returns a matrix where each element is the imaginary part of
 * the corresponding element in the matrix \a A.
 */
inline RWSymBandMat<double>  imag(const RWSymBandMat<DComplex>& A)
{
    RWMathVec<double> temp(imag(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWSymBandMat
 * Returns a matrix where each element is the norm (magnitude) of
 * the corresponding element in the matrix \a A.
 */
inline RWSymBandMat<double>  norm(const RWSymBandMat<DComplex>& A)
{
    RWMathVec<double> temp(norm(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWSymBandMat
 * Returns a matrix where each element is the argument of the corresponding
 * element in the matrix \a A.
 */
inline RWSymBandMat<double>  arg(const RWSymBandMat<DComplex>& A)
{
    RWMathVec<double> temp(arg(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}



inline RWGenMat<DComplex>  conjTransposeProduct(const RWSymBandMat<DComplex>& A, const RWGenMat<DComplex>& B)
{
    return conjTransposeProduct(RWGenMat<DComplex>(A), B);
}

inline RWGenMat<DComplex>  conjTransposeProduct(const RWGenMat<DComplex>& A, const RWSymBandMat<DComplex>& B)
{
    return conjTransposeProduct(A, RWGenMat<DComplex>(B));
}

inline RWGenMat<DComplex>  conjTransposeProduct(const RWSymBandMat<DComplex>& A, const RWSymBandMat<DComplex>& B)
{
    return conjTransposeProduct(RWGenMat<DComplex>(A), RWGenMat<DComplex>(B));
}

/**
 * @relates RWSymBandMat
 * @copydoc minValue(const RWSymBandMat<float>&)
 */
inline double  minValue(const RWSymBandMat<double>& A)
{
    return minValue(A.dataVec());
}

/**
 * @relates RWSymBandMat
 * Returns the maximum entry in the matrix.
 */
inline double  maxValue(const RWSymBandMat<double>& A)
{
    return maxValue(A.dataVec());
}

/**
 * @relates RWSymBandMat
 * Returns the minimum entry in the matrix.
 */
inline float  minValue(const RWSymBandMat<float>& A)
{
    return minValue(A.dataVec());
}

/**
 * @relates RWSymBandMat
 * @copydoc maxValue(const RWSymBandMat<double>& )
 */
inline float  maxValue(const RWSymBandMat<float>& A)
{
    return maxValue(A.dataVec());
}

// Below are functions that assume automatic conversion to GenMat's
// Automatic conversions do not take place for templatized functions,
// So we provide them here...


template <class TypeT>
RWGenMat<TypeT>  product(const RWSymBandMat<TypeT>& A, const RWGenMat<TypeT>& B);

template <class TypeT>
RWGenMat<TypeT>  product(const RWGenMat<TypeT>& A, const RWSymBandMat<TypeT>& B);

template <class TypeT>
RWGenMat<TypeT>  product(const RWSymBandMat<TypeT>& A, const RWSymBandMat<TypeT>& B);

template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWSymBandMat<TypeT>& A, const RWGenMat<TypeT>& B);

template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWGenMat<TypeT>& A, const RWSymBandMat<TypeT>& B);

template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWSymBandMat<TypeT>& A, const RWSymBandMat<TypeT>& B);

template <class TypeT>
inline RWGenMat<TypeT>  adjoint(const RWSymBandMat<TypeT>& A)
{
    RWGenMat<TypeT> a(A);
    return adjoint(a);
}

template <class TypeT>
RWSymBandMat<TypeT>  toSymBandMat(const RWGenMat<TypeT>& A, unsigned halfband);

/**
 * @relates RWSymBandMat
 *
 * Extracts the symmetric part of a banded matrix. The symmetric
 * part of the banded matrix \a A is \f$ (A + A^{T})/2 \f$.
 */
template <class TypeT>
RWSymBandMat<TypeT>  toSymBandMat(const RWBandMat<TypeT>& A);


#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/sbndmat.cc>
#endif

#endif
