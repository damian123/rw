
#ifndef __RWBANDMAT_H__
#define __RWBANDMAT_H__
/*
 * BandMat - A banded matrix
 *
 * Stores a banded matrix.  All entries farther above the diagonal
 * that the upper bandwidth, or farther below than the lower bandwidth
 * are defined to be zero.  The total bandwidth is the sum of the
 * upper and lower bandwidths, plus one for the diagonal.  Only
 * the elements which may be non-zero are stored.
 *
 * The matrix is stored column by column.  Each column takes up
 * <bandwidth> entries, some of these entries near the corners
 * are not used.
 *
 * Bounds checking is enabled by setting the preprocessor
 * symbol RWBOUNDS_CHECK.  This is strongly encouraged.
 *
 ***************************************************************************
 *
 * $Id: //lapack/13/rw/lapack/bandmat.h#1 $
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


// --------------------------------------------------------------------------
//
// Forward Declarations
//
//---------------------------------------------------------------------------
template <class TypeT>
class RWBandMat;

template <class TypeT>
class RWTriDiagMat;

template <class TypeT>
class RWSymBandMat;

template <class TypeT>
class RWHermBandMat;


/**
 * @ingroup sparse_matrix_classes
 *
 * @brief Encapsulates a banded matrix.
 *
 * The class \link RWBandMat RWBandMat<T>\endlink encapsulates a
 * banded matrix. A banded matrix is nonzero only near the diagonal.
 * Specifically, if \e u is the upper bandwidth and \e l
 * the lower, the matrix entry \e A<SUB>ij</SUB> is defined
 * as 0 if either <i>j-i>u</i> or <i>i-j>l</i>. The <i>total bandwidth</i>,
 * simply called \e bandwidth, is the sum of the upper and lower
 * bandwidths plus 1, for the diagonal. See the \ref RWBandMat_storage_scheme
 * section of this entry for an example of a banded matrix.
 *
 * @section RWBandMat_synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/bandmat.h>
 *
 * RWBandMat<double> d;
 * @endcode
 *
 * @section RWBandMat_example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/lapack/bandmat.h>
 *
 * int main(){
 *
 *     RWBandMat<double> d(9, 9, 2, 1);
 *
 *     d.diagonal(-2) = 1;
 *     d.diagonal(-1) = 2;
 *     d.diagonal( ) = 3;
 *     d.diagonal( 1) = 4;
 *
 *     std::cout << d;
 *     return 0;
 * }
 * @endcode
 *
 * @section RWBandMat_storage_scheme Storage Scheme
 *
 * The matrix is stored column-by-column. There are some unused locations
 * in the vector of data so that each column takes up the same number of
 * entries. For example, the following 9x9 matrix has an upper bandwidth of 1,
 * a lower bandwidth of 2, and thus a total bandwidth of 4:
 *
 * \f[
 * \begin{bmatrix}
 * A_{11} & A_{12} &  0  & 0   &  0  & 0   & 0   & 0 & 0 \\
 * A_{21} & A_{22} & A_{23} & 0   &  0  & 0   & 0   & 0 & 0 \\
 * A_{31} & A_{32} & A_{33} & A_{34} &  0  & 0   & 0   & 0 & 0 \\
 * 0   & A_{42} & A_{43} & A_{44} & A_{45} & 0   & 0   & 0 & 0 \\
 * 0   &  0  & A_{53} & A_{54} & A_{55} & A_{56} & 0   & 0 & 0 \\
 * 0   &  0  &  0  & A_{64} & A_{65} & A_{66} & A_{67} & 0 & 0 \\
 * 0   &  0  &  0  &  0  & A_{75} & A_{76} & A_{77} & A_{78} & 0 \\
 * 0   &  0  &  0  &  0  & 0   & A_{86} & A_{87} & A_{88} & A_{89} \\
 * 0   &  0  &  0  &  0  & 0   & 0   & A_{97} & A_{98} & A_{99}
 * \end{bmatrix}
 * \f]
 *
 * This matrix is stored as follows:
 *
 * <tt>[ XXX A<sub>11</sub> A<sub>21</sub> A<sub>31</sub> A<sub>12</sub>
 * A<sub>22</sub> A<sub>32</sub> A<sub>42</sub> A<sub>23</sub> A<sub>33</sub>
 * A<sub>43</sub> A<sub>53</sub>
 * <br>  A<sub>34</sub> A<sub>44</sub> A<sub>54</sub> A<sub>64</sub>
 * A<sub>45</sub> A<sub>55</sub> A<sub>65</sub> A<sub>75</sub> A<sub>56</sub>
 * A<sub>66</sub> A<sub>76</sub> A<sub>86</sub>
 * <br>  A<sub>67</sub> A<sub>77</sub> A<sub>87</sub> A<sub>97</sub>
 * A<sub>78</sub> A<sub>88</sub> A<sub>98</sub> XXX A<sub>89</sub>
 * A<sub>99</sub> XXX XXX ]</tt>
 *
 * where \c XXX indicates an unused storage location. The mapping between
 * array entry \a A<sub>ij</sub> and the storage vector is as follows:
 *
 * \f[
 *  A[i + 1, j + 1] \to vec[i - j + upperBandwidth + j * bandwidth]
 * \f]
 *
 */
template < class TypeT >
class RWBandMat
{
private:



    // Private Instance Variables -----------------------------------------------
    RWMathVec<TypeT> vec;
    unsigned n;
    unsigned band, bandu;   // bandwidth and upper bandwidth
    void zeroUnusedEntries();  // Initialize entries in the data vector which are not used to zero

    // Private Instance Variables -----------------------------------------------
    static TypeT     zero_;

public:
    // Public Constructors ------------------------------------------------------

    /**
     * Default constructor. Builds a matrix of size 0 by 0. Necessary
     * for declaring a matrix with no explicit constructor, or an array
     * of matrices.
     */
    RWBandMat();

    /**
     * Builds a copy of the argument, \a A. Note that the new matrix
     * references \a A's data. To construct a matrix with its own copy
     * of the data, use either the copy() or deepenShallowCopy() member
     * functions.
     */
    RWBandMat(const RWBandMat<TypeT>& A);

    /**
     * Defines an uninitialized matrix of size \a n x \a nAgain with lower
     * bandwidth \a lowerWidth and upper bandwidth \a upperWidth.
     */
    RWBandMat(unsigned n, unsigned nAgain, unsigned lowerWidth, unsigned upperWidth);

    /**
     * Constructs a size \a n x \a nAgain matrix, with lower bandwidth
     * \a lowerWidth and upper bandwidth \a upperWidth, using the data
     * in the passed vector. This data must be stored in the format
     * described in the \ref RWBandMat_storage_scheme section. The resultant
     * matrix references the data in vector \a data.
     */
    RWBandMat(const RWMathVec<TypeT>& data, unsigned n, unsigned nAgain, unsigned lowerWidth, unsigned upperWdth);

    /**
     * Constructs a banded matrix from a symmetric banded, Hermitian
     * banded, or tridiagonal matrix.
     */
    RWBandMat(const RWTriDiagMat<TypeT>& A);

    /**
     * @copydoc RWBandMat( const RWTriDiagMat<TypeT>& A )
     */
    RWBandMat(const RWSymBandMat<TypeT>& A);

    /**
     * @copydoc RWBandMat( const RWTriDiagMat<TypeT>& A )
     */
    RWBandMat(const RWHermBandMat<TypeT>& A);

    RW_LAPK_MEMBER_EXPORT RWBandMat(const typename rw_linear_algebra_traits<TypeT>::generic_band_mat& re);

    /**
     * Constructs a complex matrix from the real and imaginary parts
     * supplied. If no imaginary part is supplied, it is assumed to
     * be 0.
     */
    RW_LAPK_MEMBER_EXPORT RWBandMat(const RWBandMat<double>& re, const RWBandMat<double>& im);

    // Destructor ---------------------------------------------------------------
    ~RWBandMat();

    // Public Static Members ----------------------------------------------------
    static const char* className; // For use in error messages

    // Public Instance Members --------------------------------------------------

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
     * Returns the bandwidth of the matrix. The bandwidth is the sum
     * of the upper and lower bandwidths plus 1 for the main diagonal.
     */
    inline unsigned bandwidth() const {
        return band;
    }

    /**
     * Returns the lower bandwidth of the matrix.
     */
    inline unsigned lowerBandwidth() const {
        return band - bandu - 1;
    }

    /**
     * Returns the upper bandwidth of the matrix.
     */
    inline unsigned upperBandwidth() const {
        return bandu;
    }


    // Access and/or change elements of the matrix.
    /**
     * Returns the value of the <i>ij <SUP>th</SUP></i> element of the matrix,
     * after doing bounds checking.
     */
    TypeT          bcval(int i, int j) const;

    /**
     * Returns a reference to the <i>ij <SUP>th</SUP></i> element of the
     * matrix, after doing bounds checking.
     */
    RWRORef<TypeT> bcref(int i, int j);

    /**
     * Sets the <i>ij <SUP>th</SUP></i> element of the matrix equal to
     * \a x, after doing bounds checking.
     */
    TypeT          bcset(int i, int j, TypeT x);

    /**
     * Returns the value of the <i>ij <SUP>th</SUP></i> element of the matrix.
     * Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined when the header file is read. The member function
     * bcval() does the same thing with guaranteed bounds checking.
     */
    inline TypeT  val(int i, int j) const {
        return bcval(i, j);
    }

    /**
     * Sets the <i>ij <SUP>th</SUP></i> element of the matrix equal to
     * \a x. Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined when the header file is read. The member function
     * bcset() does the same thing with guaranteed bounds checking.
     */
    inline TypeT  set(int i, int j, TypeT x) {
#ifdef RWBOUNDS_CHECK
        return bcset(i, j, x);
#else
        return vec(i - j + bandu + j * band) = x;
#endif
    }

    /**
     * Accesses the <i>ij <SUP>th</SUP></i> element. If the matrix <i>is
     * not</i> a \c const matrix, a reference type is returned, so this
     * operator can be used for assigning or accessing an element. In
     * these cases, using this operator is equivalent to calling the
     * ref() member function. If the matrix \e is a \c const matrix,
     * a value is returned, so this operator can be used only for accessing
     * an element. In this case, using this operator is equivalent to
     * calling the val() member function. Bounds checking is done if
     * the preprocessor symbol \c RWBOUNDS_CHECK is defined before including
     * the header file.
     */
    inline RWRORef<TypeT> operator()(int i, int j) {
        return bcref(i, j);
    }

    /**
     * Returns a reference to the <i>ij <SUP>th</SUP></i> element of the
     * matrix. Bounds checking is done if the preprocessor symbol
     * \c RWBOUNDS_CHECK is defined when the header file is read. The member
     * function bcref() does the same thing with guaranteed bounds
     * checking.
     */
    inline RWRORef<TypeT> ref(int i, int j) {
        return bcref(i, j);
    }

    /**
     * Accesses the <i>ij <SUP>th</SUP></i> element. If the matrix <i>is
     * not</i> a \c const matrix, a reference type is returned, so this
     * operator can be used for assigning or accessing an element. In
     * these cases, using this operator is equivalent to calling the
     * ref() member function. If the matrix \e is a \c const matrix,
     * a value is returned, so this operator can be used only for accessing
     * an element. In this case, using this operator is equivalent to
     * calling the val() member function. Bounds checking is done if
     * the preprocessor symbol \c RWBOUNDS_CHECK is defined before including
     * the header file.
     */
    inline TypeT  operator()(int i, int j) const {
        return bcval(i, j);
    }


    // The leading submatrix of order k is the kxk matrix in the upper
    // left.
    /**
     * Returns the \a order x \a order upper left corner of the matrix. The
     * submatrix and the matrix share the same data.
     */
    RWBandMat<TypeT> leadingSubmatrix(int order);

    // Access to diagonals.
    /**
     * Returns a reference to the <i>j <SUP>th</SUP></i> diagonal of the
     * matrix, after doing bounds checking. The main diagonal is indexed
     * by 0, diagonals in the upper triangle are indexed with positive
     * integers, and diagonals in the lower triangle are indexed with
     * negative integers.
     */
    RWMathVec<TypeT> bcdiagonal(int j = 0) const;

    /**
     * Returns a reference to the <i>i <SUP>th</SUP></i> diagonal of the
     * matrix. The main diagonal is indexed by 0, diagonals in the upper
     * triangle are indexed with positive integers, and diagonals in
     * the lower triangle are indexed with negative integers. Bounds
     * checking is done if the preprocessor symbol \c RWBOUNDS_CHECK is
     * defined when the header file is read. The member function
     * bcdiagonal() does the same thing with guaranteed bounds checking.
     */
    inline RWMathVec<TypeT> diagonal(int i = 0) const {
#if defined(RWBOUNDS_CHECK)
        return bcdiagonal(i);
#else
        int iabs = (i >= 0) ? i : -i;
        return vec.slice(i > 0 ? upperBandwidth() + i * (bandwidth() - 1)
                         : upperBandwidth() + (unsigned)iabs
                         , (size_t)n - iabs, bandwidth());
#endif
    }

    // reference() makes an alias, operator= makes a copy.
    /**
     * Makes this matrix a reference to the argument matrix. The two
     * matrices share the same data. The matrices do \e not have
     * to be the same size before calling reference(). To copy a matrix
     * into another of the same size, use the operator=() member operator.
     */
    RWBandMat<TypeT>& reference(RWBandMat<TypeT>& m);

    // Convert to a general matrix.
    operator RWGenMat<TypeT>() const;

    /**
     * Sets every element of the matrix to 0.
     */
    void zero() {
        vec = TypeT(0);
    }


    // copy() returns a RWBandMat<TypeT> with its own copy of the data.
    // deepenShallowCopy makes sure the data is not shared by another
    // vector or matrix.
    /**
     * Creates a copy of this matrix with distinct data. The stride
     * of the data vector in the new matrix is guaranteed to be 1.
     */
    RWBandMat<TypeT> copy() const;

    /**
     * Creates a copy of this matrix with distinct data. The stride
     * of the data vector in the new matrix is guaranteed to be 1.
     */
    RWBandMat<TypeT> deepCopy() const {
        return copy();
    }

    /**
     * Ensures that the data in the matrix is \e not shared by any
     * other matrix or vector. Also ensure that the stride in the data
     * vector is equal to 1. If necessary, a new copy of the data vector
     * is made.
     */
    void deepenShallowCopy() {
        vec.deepenShallowCopy();
    }

    // Access to the raw data.
    /**
     * Returns the matrix's data vector, where the explicitly stored
     * entries in the matrix are kept.
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

    /**
     * Resizes the matrix or changes both its size and lower and upper
     * bandwidths. Any new entries in the matrix are set to 0.
     */
    void resize(unsigned m, unsigned n);
    /**
     * @copydoc resize(unsigned m, unsigned n)
     */
    void resize(unsigned n, unsigned nAgain, unsigned lowWidth, unsigned upWidth);

    /**
     * Reads a matrix from an input stream. The format of the matrix
     * is the same as the format output by the printOn() member function:
     * first the lower bandwidth, followed by the upper bandwidth, followed
     * by the matrix itself. The sample matrix below shows the format.
     * Note that extra white space and text preceding the bandwidth
     * specification are ignored. Only the relevant band of the matrix
     * is used.
     *
     * @code
     * 2  1  5x5
     * [  1  1  0  0  0
     *    7  4  3  0  0
     *    5  5  4  5  0
     *    0  1  3  1  1
     *    0  0  8  1  4  ]
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
     * using the saveOn(RWvostream&) const member function.
     */
    void restoreFrom(RWvistream&);        // Internal US-ASCII or binary code

    /**
     * Stores a matrix to an RWvostream, the Rogue Wave virtual output
     * stream class. The matrix can be read using the restoreFrom(RWvistream&)
     * member function.
     */
    void saveOn(RWvostream&) const;

    /**
     * Reads in a matrix from an RWFile. The matrix must have been stored
     * to the file using the saveOn(RWFile&) const member function.
     */
    void restoreFrom(RWFile&);

    /**
     * Stores a matrix to an RWFile. The matrix can be read using the
     * restoreFrom(RWFile&) member function.
     */
    void saveOn(RWFile&) const;

    /**
     * Returns the number of bytes that it would take to write the matrix
     * to a file using saveOn(RWFile&) const.
     */
    unsigned binaryStoreSize() const;

    /**
     * Sets the matrix elements equal to the elements of \a m. The two
     * matrices must be the same size. To make the matrix reference
     * the same data as \a m, use the reference() member function.
     */
    RWBandMat<TypeT>& operator=(const RWBandMat<TypeT>& m);

    /**
     * Boolean operators. Two matrices are considered equal if they
     * have the same size and their elements are all exactly the same.
     * Be aware that floating point arithmetic is \e not exact;
     * matrices which are theoretically equal are \e not always
     * numerically equal.
     */
    bool operator==(const RWBandMat<TypeT>& X);

    /**
     * @copydoc operator==(const RWBandMat<TypeT>&)
     */
    bool operator!=(const RWBandMat<TypeT>& X) {
        return !(operator==(X));
    }

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     */
    RWBandMat<TypeT>& operator+=(const RWBandMat<TypeT>& m);

    /**
     * @copydoc operator+=(const RWBandMat<TypeT>&)
     */
    RWBandMat<TypeT>& operator-=(const RWBandMat<TypeT>& m);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     * @note This operator does element-by-element multiplication, \e not
     * inner product style matrix multiplication. You can use product() to
     * do matrix-matrix inner product multiplication.
     */
    RWBandMat<TypeT>& operator*=(const RWBandMat<TypeT>& m);

    /**
     * @copydoc operator+=(const RWBandMat<TypeT>&)
     */
    RWBandMat<TypeT>& operator/=(const RWBandMat<TypeT>& m);

    /**
     * Performs the indicated operation on each element of the matrix.
     */
    RWBandMat<TypeT>& operator*=(TypeT);

    /**
     * @copydoc operator*=(TypeT)
     */
    RWBandMat<TypeT>& operator/=(TypeT);
};


template <class TypeT>
inline RWBandMat<TypeT> sameShapeMatrix(RWMathVec<TypeT>& vec, const RWBandMat<TypeT>& A)
{
    return RWBandMat<TypeT>(vec, A.rows(), A.cols(), A.lowerBandwidth(), A.upperBandwidth());
}

// added by Omon - 06/15
inline RWBandMat<double> sameShapeRealMatrix(RWMathVec<double>& vec, const RWBandMat<DComplex>& A)
{
    return RWBandMat<double>(vec, A.rows(), A.cols(), A.lowerBandwidth(), A.upperBandwidth());
}

/**
 * @relates RWBandMat
 *
 * Unary plus and minus operators. Each operator returns a copy
 * of the matrix or its negation.
 */
template <class TypeT>
RWBandMat<TypeT>  operator-(const RWBandMat<TypeT>&);        // Unary minus

/**
 * @relates RWBandMat
 *
 * @copydoc operator-(const RWBandMat<TypeT>&)
 */
template <class TypeT>
RWBandMat<TypeT>  operator+(const RWBandMat<TypeT>&);        // Unary plus


/**
 * @relates RWBandMat
 *
 * Performs element-by-element operations on the arguments. To do
 * inner product matrix multiplication, use the product global function.
 */
template <class TypeT>
RWBandMat<TypeT>  operator*(const RWBandMat<TypeT>&, const RWBandMat<TypeT>&);

/**
 * @relates RWBandMat
 *
 * @copydoc operator*(const RWBandMat<TypeT>&, const RWBandMat<TypeT>&)
 */
template <class TypeT>
RWBandMat<TypeT>  operator/(const RWBandMat<TypeT>&, const RWBandMat<TypeT>&);

/**
 * @relates RWBandMat
 *
 * @copydoc operator*(const RWBandMat<TypeT>&, const RWBandMat<TypeT>&)
 */
template <class TypeT>
RWBandMat<TypeT>  operator+(const RWBandMat<TypeT>&, const RWBandMat<TypeT>&);

/**
 * @relates RWBandMat
 *
 * @copydoc operator*(const RWBandMat<TypeT>&, const RWBandMat<TypeT>&)
 */
template <class TypeT>
RWBandMat<TypeT>  operator-(const RWBandMat<TypeT>&, const RWBandMat<TypeT>&);

/**
 * @relates RWBandMat
 *
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
inline RWBandMat<TypeT>  operator*(const RWBandMat<TypeT>& A, TypeT x)
{
    RWMathVec<TypeT> temp(A.dataVec()*x);
    return sameShapeMatrix(temp, A);
}

template <class TypeT>
#ifndef RW_NO_INLINED_TEMP_DESTRUCTORS
/**
 * @relates RWBandMat
 *
 * @copydoc operator*(const RWBandMat<TypeT>& A, TypeT x)
 */
inline RWBandMat<TypeT>  operator*(TypeT x, const RWBandMat<TypeT>& A)
{
    return A * x;
}
#else
RWBandMat<TypeT>  operator*(TypeT x, const RWBandMat<TypeT>& A);
#endif

/**
 * @relates RWBandMat
 *
 * @copydoc operator*(const RWBandMat<TypeT>& A, TypeT x)
 */
template <class TypeT>
RWBandMat<TypeT>  operator/(const RWBandMat<TypeT>& A, TypeT x);

#if defined(_MSC_VER)
template <class TypeT>
inline  std::ostream& operator<<(std::ostream& s, const RWBandMat<TypeT>& m)
{
    m.printOn(s);
    return s;
}

template <class TypeT>
inline  std::istream& operator>>(std::istream& s, RWBandMat<TypeT>& m)
{
    m.scanFrom(s);
    return s;
}

#else

/**
 * @relates RWBandMat
 *
 * Writes the matrix to the stream. This is equivalent to calling
 * the printOn() member function.
 */
template <class TypeT>
inline std::ostream&  operator<<(std::ostream& s, const RWBandMat<TypeT>& m)
{
    m.printOn(s);
    return s;
}

/**
 * @relates RWBandMat
 *
 * Reads the matrix from the stream. This is equivalent to calling
 * the scanFrom() member function.
 */
template <class TypeT>
inline std::istream&  operator>>(std::istream& s, RWBandMat<TypeT>& m)
{
    m.scanFrom(s);
    return s;
}

#endif

// I/O for humans

/**
 * @relates RWBandMat
 *
 * Returns the transpose of the argument matrix.
 */
template <class TypeT>
RWBandMat<TypeT>  transpose(const RWBandMat<TypeT>& A);

// The transpose of the matrix.
/**
 * @relates RWBandMat
 *
 * Returns the inner product (matrix-vector product) of \a A and
 * \a x.
 */
template <class TypeT>
RWMathVec<TypeT>  product(const RWBandMat<TypeT>& A, const RWMathVec<TypeT>& x);

/**
 * @relates RWBandMat
 *
 * Returns the inner product (matrix-vector product) of \a x and
 * \a A. This is equal to the product of \a A transpose and \a x.
 */
template <class TypeT>
RWMathVec<TypeT>  product(const RWMathVec<TypeT>& x, const RWBandMat<TypeT>& A);

/**
 * @relates RWBandMat
 *
 * Returns the inner product (matrix product) of \a A and \a B.
 * This product is a banded matrix whose lower bandwidth is the
 * sum of the lower bandwidths of \a A and \a B, and whose upper
 * bandwidth is the sum of the upper bandwidths of \a A and \a B.
 */
template <class TypeT>
RWBandMat<TypeT>  product(const RWBandMat<TypeT>& A, const RWBandMat<TypeT>& B);

/**
 * @relates RWBandMat
 *
 * Returns the inner product (matrix product) of the transpose of
 * \a A and the matrix \a B. This product is a banded matrix whose
 * lower bandwidth is the sum of the upper bandwidth of \a A and
 * the lower bandwidth of \a B, and whose upper bandwidth is the
 * sum of the lower bandwidth of \a A and the upper bandwidth of
 * \a B.
 */
template <class TypeT>
RWBandMat<TypeT>  transposeProduct(const RWBandMat<TypeT>& A, const RWBandMat<TypeT>& B);


// transposeProduct calculates A^TB
// inner products

// Below are functions that assume automatic conversion to GenMat's
// Automatic conversions do not take place for templatized functions,
// So we provide them here...
template <class TypeT>
RWGenMat<TypeT>  product(const RWBandMat<TypeT>& A, const RWGenMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  product(const RWGenMat<TypeT>& A, const RWBandMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWBandMat<TypeT>& A, const RWGenMat<TypeT>& B);
template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWGenMat<TypeT>& A, const RWBandMat<TypeT>& B);


template <class TypeT>
inline RWGenMat<TypeT>  adjoint(const RWBandMat<TypeT>& A)
{
    RWGenMat<TypeT> a(A);
    return adjoint(a);
}

/**
 * @relates RWBandMat
 *
 * Extracts a band of entries from a square matrix. The main diagonal
 * is extracted, along with \a bandl diagonals from the upper triangle
 * of the matrix, and \a bandu diagonals from the lower triangle of
 * the matrix.
 */
template <class TypeT>
RWBandMat<TypeT>  toBandMat(const RWGenMat<TypeT>& A, unsigned bandl, unsigned bandu);

#if !(defined(_MSC_VER))

/**
 * @relates RWBandMat
 *
 * Returns a matrix whose entries are the absolute value of the
 * argument. The absolute value of a complex number is considered
 * to be the sum of the absolute values of its real and imaginary
 * parts. To get the norm of a complex matrix, you can use the
 * norm() function.
 */
template <class TypeT>
inline RWBandMat<typename rw_numeric_traits<TypeT>::norm_type>  abs(const RWBandMat<TypeT>& M)
{
    RWMathVec<typename rw_numeric_traits<TypeT>::norm_type> temp(abs(M.dataVec()));
    return sameShapeMatrix(temp, M);
}
//specialize for DComplex:
#  if !(defined(__hppa))
template <>
#  endif
inline RWBandMat<rw_numeric_traits<DComplex>::norm_type>  abs(const RWBandMat<DComplex>& M)
{
    RWMathVec<rw_numeric_traits<DComplex>::norm_type> temp(abs(M.dataVec()));
    return sameShapeRealMatrix(temp, M);
}

#else //using MSVC compiler:

template <class TypeT>
inline RWBandMat<typename rw_numeric_traits<TypeT>::norm_type>  abs(const RWBandMat<TypeT>& M)
{
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;
    //RWMathVec<typename rw_numeric_traits<TypeT>::norm_type> temp(abs(M.dataVec()));
    RWMathVec<rt> temp(abs(M.dataVec()));
    //return sameShapeMatrix(temp,M);
    //this function should call sameShape{Real}Matrix, and return the result of that function.
    // I'm putting code directly from those functions here -- MSVC5 cannot handle complicated template specializations -- till a better solution comes up
    return RWBandMat<rt>(temp, M.rows(), M.cols(), M.lowerBandwidth(), M.upperBandwidth());
};


#endif // !defined(_MSC_VER)

/**
 * @relates RWBandMat
 *
 * Returns a matrix where each element is the complex conjugate
 * of the corresponding element in the matrix \a A.
 */
inline RWBandMat<DComplex>  conj(const RWBandMat<DComplex>& A)
{
    RWMathVec<DComplex> temp(conj(A.dataVec()));
    return sameShapeMatrix(temp, A);
}

/**
 * @relates RWBandMat
 *
 * Returns a matrix where each element is the real part of the corresponding
 * element in the matrix \a A.
 */
inline RWBandMat<double>  real(const RWBandMat<DComplex>& A)
{
    RWMathVec<double> temp(real(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWBandMat
 *
 * Returns a matrix where each element is the imaginary part of
 * the corresponding element in the matrix \a A.
 */
inline RWBandMat<double>  imag(const RWBandMat<DComplex>& A)
{
    RWMathVec<double> temp(imag(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}


/**
 * @relates RWBandMat
 *
 * Returns a matrix where each element is the norm (magnitude) of
 * the corresponding element in the matrix \a A.
 */
inline RWBandMat<double>  norm(const RWBandMat<DComplex>& A)
{
    RWMathVec<double> temp(norm(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}

/**
 * @relates RWBandMat
 *
 * Returns a matrix where each element is the argument of the corresponding
 * element in the matrix \a A.
 */
inline RWBandMat<double>  arg(const RWBandMat<DComplex>& A)
{
    RWMathVec<double> temp(arg(A.dataVec()));
    return sameShapeRealMatrix(temp, A);
}


/**
 * @relates RWBandMat
 *
 * Returns the inner product (matrix product) of the complex conjugate
 * transpose of \a A and the matrix \a B. This product is a banded
 * matrix whose lower bandwidth is the sum of the upper bandwidth
 * of \a A and the lower bandwidth of \a B, and whose upper bandwidth
 * is the sum of the lower bandwidth of \a A and the upper bandwidth
 * of \a B.
 */
inline RWBandMat<DComplex>  conjTransposeProduct(const RWBandMat<DComplex>& A, const RWBandMat<DComplex>& B)
{
    return product(conj(transpose(A)), B);
}
// conjTransposeProduct calculates conj(A^T)B


inline RWGenMat<DComplex>  conjTransposeProduct(const RWBandMat<DComplex>& A, const RWGenMat<DComplex>& B)
{
    return conjTransposeProduct(RWGenMat<DComplex>(A), B);
}

inline RWGenMat<DComplex>  conjTransposeProduct(const RWGenMat<DComplex>& A, const RWBandMat<DComplex>& B)
{
    return conjTransposeProduct(A, RWGenMat<DComplex>(B));
}

/**
 * @relates RWBandMat
 *
 * Returns the minimum entry in the matrix.
 */
template <class TypeT>
inline TypeT  minValue(const RWBandMat<TypeT>& A)
{
    return minValue(A.dataVec());
}

/**
 * @relates RWBandMat
 *
 * Returns the maximum entry in the matrix.
 */
template <class TypeT>
inline TypeT  maxValue(const RWBandMat<TypeT>& A)
{
    return maxValue(A.dataVec());
}


#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/bandmat.cc>
#endif

#endif
