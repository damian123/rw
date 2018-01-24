#ifndef __RWHBNDMAT_H__
#define __RWHBNDMAT_H__
/*
 * RWHermBandMat - An Hermitian banded matrix of Complexes
 *
 * Stores an Hermitian banded matrix.  All entries farther above or below
 * the diagonal than the upper bandwidth
 * are defined to be zero.  The total bandwidth is twice the
 * upperBandwidth, plus one for the diagonal.  Only
 * the elements which may be non-zero are stored.
 *
 * The top half of the matrix is stored column by column.  Each
 * column takes up
 * <upperBandwidth+1> entries, some of these entries near the top
 * left corner are not used.
 *
 * Bounds checking is enabled by setting the preprocessor
 * symbol RWBOUNDS_CHECK.  This is strongly encouraged.
 *
 ***************************************************************************
 *
 * $Id: //lapack/13/rw/lapack/hbndmat.h#1 $
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
#include <rw/ref.h>
#include <rw/lapack/sbndmat.h>



template <class TypeT>
class RWSymBandMat;

template <class TypeT>
class RWGenMat;

template <class TypeT>
class RWBandMat;

/**
 * @ingroup sparse_matrix_classes
 * @brief Encapsulates a Hermitian banded matrix. A Hermitian
 * banded matrix is Hermitian, and nonzero only near the diagonal.
 *
 * The class \link RWHermBandMat RWHermBandMat<T> \endlink encapsulates
 * a Hermitian banded matrix. A Hermitian banded matrix is Hermitian,
 * and nonzero only near the diagonal. Specifically, if <i>h</i>
 * is the half bandwidth, then any entries for which <i>i-j>h</i>
 * or <i>j-i>h</i> are defined to be 0; in addition, an entry
 * <i>A<SUB>ij</SUB></i> = <i>conj(A<SUB>ji</SUB>)</i>.
 * Although this strict definition implies a matrix with 0 imaginary
 * parts along the diagonal, the Rogue Wave class implementation
 * allows arbitrary diagonal entries.
 *
 * @section RWHermBandMat_synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/hbndmat.h>
 *
 * RWHermBandMat<DComplex> H;
 * @endcode
 *
 * @section RWHermBandMat_example Example
 *
 * @code
 * #include <rw/lapack/hbndmat.h>
 *
 * int main()
 * {
 *     RWHermBandMat<DComplex> HB(5,5,1);
 *     HB.zero();
 *     HB.set(2, 2, DComplex(2,1));
 * }
 * @endcode
 *
 * @section RWHermBandMat_storage_scheme Storage Scheme
 *
 * As an example of the storage scheme for banded Hermitian matrices, consider
 * the following 9x9 matrix with a half bandwidth of 2, where entries
 * <i>a<SUB>ij</SUB></i> are notation for <i>conj(A<SUB>ij</SUB>)</i>:
 *
 * \f[
 *   \begin{bmatrix}
 *    A_{11} & A_{12} & A_{13} & 0 & 0 & 0 & 0 & 0 & 0 \\
 *    a_{12} & A_{22} & A_{23} & A_{24} & 0 & 0 & 0 & 0 & 0 \\
 *    a_{13} & a_{23} & A_{33} & A_{34} & A_{35} & 0 & 0 & 0 & 0 \\
 *    0 & a_{24} & a_{34} & A_{44} & A_{45} & A_{46} & 0 & 0 & 0 \\
 *    0 & 0 & a_{35} & a_{45} & A_{55} & A_{56} & A_{57} & 0 & 0 \\
 *    0 & 0 & 0 & a_{46} & a_{56} & A_{66} & A_{67} & A_{68} & 0 \\
 *    0 & 0 & 0 & 0 & a_{57} & a_{67} & A_{77} & A_{78} & A_{79} \\
 *    0 & 0 & 0 & 0 & 0 & a_{68} & a_{78} & A_{88} & A_{89} \\
 *    0 & 0 & 0 & 0 & 0 & 0 & a_{79} & a_{89} & A_{99}
 *   \end{bmatrix}
 * \f]
 *
 * The upper triangle is stored column by column. For convenience,
 * there are some unused locations in the vector of data. These are indicated
 * as \c XXX in the following illustration of the storage vector:
 *
 * <tt>[ XXX XXX A<SUB>11</SUB> XXX A<SUB>12</SUB> A<SUB>22</SUB> A<SUB>13</SUB>
 * A<SUB>23</SUB> A<SUB>33</SUB> A<SUB>24</SUB> A<SUB>34</SUB> A<SUB>44</SUB>
 * A<SUB>35</SUB>  A<SUB>45</SUB> A<SUB>55</SUB> A<SUB>46</SUB> A<SUB>56</SUB>
 * A<SUB>66</SUB> A<SUB>57</SUB> A<SUB>67</SUB> A<SUB>77</SUB> A<SUB>68</SUB>
 * A<SUB>78</SUB> A<SUB>88</SUB> A<SUB>79</SUB> A<SUB>89</SUB> A<SUB>99</SUB>
 * ]</tt>
 *
 * The mapping between the array and the storage vector is as follows:
 *
 * \f[
 * A(i + 1, j + 1) =
 *   \left\{
 *     \begin{array}{l l}
 *        \text{vec}[i + \text{halfwidth} + j*\text{halfwidth}] & \quad
 *          \text{if  } j \geq i \\
 *        \text{conj}(\text{vec}[j + \text{halfwidth} + i*\text{halfwidth}])
 *          & \quad \text{if  } j < i \\
 *     \end{array}
 *   \right.
 * \f]
 */
template <class TypeT>
class RWHermBandMat
{
private:
    // Private Instance Variables ----------------------------------------------
    // The data which define the matrix
    RWMathVec<TypeT> vec;
    unsigned n;
    unsigned bandu; // The upper bandwidth

    // Private Class Variables -------------------------------------------------
    static TypeT zero_; // The zero value for all the non-stored zero values.

    // Private Instance Methods ------------------------------------------------
    // Initialize entries in the data vector which are not used to zero
    void zeroUnusedEntries();

public:
    // Public Class Variables --------------------------------------------------
    static const char* className; // For use in error messages

    /**
     * Default constructor. Builds a matrix of size 0 x 0. This constructor
     * is necessary to declare a matrix with no explicit constructor
     * or an array of matrices.
     */
    RWHermBandMat();

    /**
     * Builds a copy of its argument, \a A. Note that the new matrix
     * references \a A's data. To construct a matrix with its own copy
     * of the data, use either the copy() or deepenShallowCopy() member
     * functions.
     */
    RWHermBandMat(const RWHermBandMat<TypeT>& A);

    /**
     * Defines an uninitialized \a x by \a y matrix with half
     * bandwidth \a hb.
     */
    RWHermBandMat(unsigned x, unsigned y, unsigned hb);

    /**
     * Constructs a \a x by \a y matrix with half bandwidth \a hb
     * using the data in the passed vector. This data must be stored
     * in the format described in the \ref RWHermBandMat_storage_scheme
     * section. The resultant matrix references the data in vector \a vd.
     */
    RWHermBandMat(const RWMathVec<TypeT>& vd, unsigned x, unsigned y, unsigned hb);

    /**
     * Converts a real matrix to a Hermitian banded matrix.
     */
    RWHermBandMat(const RWSymBandMat<double>& re);

    ~RWHermBandMat();

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
     * Returns the bandwidth of the matrix.
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

    /**
     * Returns the value of the \a ij <SUP>th</SUP> element of the matrix.
     * Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined when the header file is read. The member function
     * bcval() does the same thing with guaranteed bounds checking.
     */
    inline TypeT            val(int i, int j) const;

    /**
     * Sets the \a ij <SUP>th</SUP> element of the matrix equal to \a
     * x. Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK
     * is defined when the header file is read. The member function
     * bcset() does the same thing with guaranteed bounds checking.
     */
    inline TypeT            set(int i, int j, TypeT x);

    /**
     * Returns a reference to the \a ij <SUP>th</SUP> element of the matrix.
     * Bounds checking is done if the preprocessor symbol \c RWBOUNDS_CHECK is
     * defined when the header file is read. The member function \c
     * bcref does the same thing with guaranteed bounds checking.
     */
    inline RWROCJRef<TypeT> ref(int i, int j);

    /**
     * Returns the value of the \a ij <SUP>th</SUP> element of the matrix,
     * after doing bounds checking.
     */
    TypeT                   bcval(int i, int j) const;

    /**
     * Sets the \a ij <SUP>th</SUP> element of the matrix equal to \a
     * x, after doing bounds checking.
     */
    TypeT                   bcset(int i, int j, TypeT x);

    /**
     * Returns a reference to the \a ij <SUP>th</SUP> element of the
     * matrix, after doing bounds checking.
     */
    RWROCJRef<TypeT>        bcref(int i, int j);

    /**
     * Returns the \a k x \a k upper left corner of the matrix. The
     * submatrix and the matrix share the same data.
     */
    RWHermBandMat<TypeT> leadingSubmatrix(int k);

    /**
     * Makes this matrix a reference to the argument matrix. The two
     * matrices share the same data. The matrices do <i>not</i> have
     * to be the same size before calling reference(). To copy a matrix
     * into another of the same size, use the operator=() member operator.
     */
    RWHermBandMat<TypeT>& reference(RWHermBandMat<TypeT>& m);

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
    RWHermBandMat copy() const;

    /**
     * Creates a copy of this matrix with distinct data. The stride
     * of the data vector in the new matrix is guaranteed to be 1.
     */
    RWHermBandMat deepCopy() const {
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
     * the matrix's data. You can use this to pass the matrix's data
     * to C or FORTRAN subroutines, but be aware that the stride of
     * the data vector may <i>not</i> be 1.
     */
    TypeT*                  data() {
        return vec.data();
    }
    const TypeT*            data() const {
        return vec.data();
    }

    /**
     * Resizes the matrix or changes both the size of the matrix and
     * the bandwidth. Any new entries in the matrix are set to 0.
     */
    void resize(unsigned m, unsigned n);

    /**
     * @copydoc resize(unsigned, unsigned)
     */
    void resize(unsigned n, unsigned nAgain, unsigned halfWidth);

    /**
     * Sets the imaginary part of the main diagonal to 0, thus ensuring
     * that the matrix satisfies the strict mathematical definition
     * of Hermitian.
     */
    void makeDiagonalReal();

    /**
     * Reads a matrix from an input stream. The format of the matrix
     * is the same as the format output by the printOn() member function:
     * first the half bandwidth, followed by the matrix itself. Below
     * is a sample matrix showing the format. Note that extra white
     * space and text preceding the bandwidth specification are ignored.
     * Only the Hermitian part of the relevant band of the matrix is
     * used.
     *
     * @code
     * 1  5x5
     * [ (3,0)  (7,1) (0,0)  (0,0)  (0,0)
     *   (7,-1) (2,0) (2,-3) (0,0)  (0,0)
     *   (0,0)  (2,3) (8,0)  (9,-5) (0,0)
     *   (0,0)  (0,0) (9,5)  (8,0)  (7,1)
     *   (0,0)  (0,0) (0,0)  (7,-1) (8,0) ]
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
     * using the \link saveOn(RWvostream&)const saveOn() \endlink
     * member function.
     */
    void restoreFrom(RWvistream&);          // Internal US-ASCII or binary code

    /**
     * Stores a matrix to an RWvostream, the Rogue Wave virtual output
     * stream class. The matrix can be read using the \link
     * restoreFrom(RWvistream&) restoreFrom() \endlink
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
     * \link restoreFrom(RWFile&) restoreFrom() \endlink member function.
     */
    void saveOn(RWFile&) const;

    /**
     * Returns the number of bytes that it would take to write the matrix
     * to a file using saveOn().
     */
    unsigned binaryStoreSize() const;

    // Conversion to a general matrix
    operator RWGenMat<TypeT>() const;

    /**
     * Accesses the \a ij <SUP>th</SUP> element.
     * A reference type is returned, so
     * this operator can be used for assigning or accessing an element.
     * In this case, using this operator is equivalent to calling the
     * ref() member function. Bounds checking is done if
     * the preprocessor symbol \c RWBOUNDS_CHECK is defined before including
     * the header file.
     */
    inline RWROCJRef<TypeT> operator()(int i, int j);

    /**
     * Accesses the \a ij <SUP>th</SUP> element.
     * A value is returned, so this operator can be used only for accessing
     * an element. Using this operator is equivalent to
     * calling the val() member function. Bounds checking is done if
     * the preprocessor symbol \c RWBOUNDS_CHECK is defined before including
     * the header file.
     */
    inline TypeT  operator()(int i, int j) const;

    /**
     * Boolean operators. Two matrices are considered equal if they
     * have the same size and their elements are all exactly the same.
     * Be aware that floating point arithmetic is <i>not</i> exact;
     * matrices which are theoretically equal are <i>not</i> always
     * numerically equal.
     */
    bool operator==(const RWHermBandMat<TypeT>& X);

    /**
     * Boolean operators. Two matrices are considered equal if they
     * have the same size and their elements are all exactly the same.
     * Be aware that floating point arithmetic is <i>not</i> exact;
     * matrices which are theoretically equal are <i>not</i> always
     * numerically equal.
     */
    bool operator!=(const RWHermBandMat<TypeT>& X) {
        return !(operator==(X));
    }

    /**
     * Sets the matrix elements equal to the elements of \a A. The two
     * matrices must be the same size. To make the matrix reference
     * the same data as \a A, you can use the reference member function.
     */
    RWHermBandMat<TypeT>& operator=(const RWHermBandMat<TypeT>& A);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     */
    RWHermBandMat<TypeT>& operator+=(const RWHermBandMat<TypeT>& m);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     */
    RWHermBandMat<TypeT>& operator-=(const RWHermBandMat<TypeT>& m);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     * This function does element-by-element
     * multiplication, <i>not</i> inner product style matrix multiplication.
     * You can use the product global function to do matrix-matrix inner
     * product multiplication.
     */
    RWHermBandMat<TypeT>& operator*=(const RWHermBandMat<TypeT>& m);

    /**
     * Performs the indicated operation on each element of the matrix.
     */
    RWHermBandMat<TypeT>& operator*=(TypeT);

    /**
     * Performs element-by-element arithmetic on the data in the matrices.
     */
    RWHermBandMat<TypeT>& operator/=(const RWHermBandMat<TypeT>& m);

    /**
     * Performs the indicated operation on each element of the matrix.
     */
    RWHermBandMat<TypeT>& operator/=(TypeT);
};

/***************************************************************************
 *
 * RWHermBandMat<TypeT> Inline Function Definitions
 *
 **************************************************************************/

template <class TypeT>
inline TypeT RWHermBandMat<TypeT>::val(int i, int j) const
{
    return bcval(i, j);
}

template <class TypeT>
inline TypeT RWHermBandMat<TypeT>::set(int i, int j, TypeT x)
{
    return bcset(i, j, x);
}

template <class TypeT>
inline RWROCJRef<TypeT> RWHermBandMat<TypeT>::ref(int i, int j)
{
    return bcref(i, j);
}

template <class TypeT>
inline RWROCJRef<TypeT> RWHermBandMat<TypeT>::operator()(int i, int j)
{
    return ref(i, j);
}

template <class TypeT>
inline TypeT RWHermBandMat<TypeT>::operator()(int i, int j) const
{
    return val(i, j);
}


/***************************************************************************
 *
 * Global Functions
 *
 **************************************************************************/

// This function returns a new matrix the same shape as the one
// passed in but with the data passed in.  This way we restrict
// all the >if statements to a couple lines
template <class TypeT>
inline RWHermBandMat<TypeT> sameShapeMatrix(RWMathVec<TypeT>& vec, const RWHermBandMat<TypeT>& A)
{
    return RWHermBandMat<TypeT>(vec, A.rows(), A.cols(), A.lowerBandwidth());
}

/**
 * @relates RWHermBandMat
 *
 * Unary minus operator. Returns a copy
 * of the matrix.
 */
template <class TypeT>
RWHermBandMat<TypeT>  operator-(const RWHermBandMat<TypeT>&); // Unary minus

/**
 * @relates RWHermBandMat
 *
 * Unary plus operator. Returns a copy
 * of the matrix.
 */
template <class TypeT>
RWHermBandMat<TypeT>  operator+(const RWHermBandMat<TypeT>&); // Unary plus

/**
 * @relates RWHermBandMat
 *
 * Performs element-by-element operations on the arguments. To do
 * inner product matrix multiplication, you can use the product
 * global function.
 */
template <class TypeT>
RWHermBandMat<TypeT>  operator*(const RWHermBandMat<TypeT>&, const RWHermBandMat<TypeT>&);

/**
 * @relates RWHermBandMat
 *
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
RWHermBandMat<TypeT>  operator/(const RWHermBandMat<TypeT>&, const RWHermBandMat<TypeT>&);

/**
 * @relates RWHermBandMat
 *
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
RWHermBandMat<TypeT>  operator+(const RWHermBandMat<TypeT>&, const RWHermBandMat<TypeT>&);

/**
 * @relates RWHermBandMat
 *
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
RWHermBandMat<TypeT>  operator-(const RWHermBandMat<TypeT>&, const RWHermBandMat<TypeT>&);

/**
 * @relates RWHermBandMat
 *
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
inline RWHermBandMat<TypeT>  operator*(const RWHermBandMat<TypeT>& A, TypeT x)
{
    RWMathVec<TypeT> temp(A.dataVec()*x);
    return sameShapeMatrix(temp, A);
}

#ifndef RW_NO_INLINED_TEMP_DESTRUCTORS

/**
 * @relates RWHermBandMat
 *
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
inline RWHermBandMat<TypeT>  operator*(TypeT x, const RWHermBandMat<TypeT>& A)
{
    return A * x;
}

#else

template <class TypeT>
RWHermBandMat<TypeT>  operator*(TypeT x, const RWHermBandMat<TypeT>& A);

#endif

/**
 * @relates RWHermBandMat
 *
 * Performs element-by-element operations on the arguments.
 */
template <class TypeT>
RWHermBandMat<TypeT>  operator/(const RWHermBandMat<TypeT>& A, TypeT x);

/**
 * @relates RWHermBandMat
 *
 * Writes the matrix to the stream. This is equivalent to calling
 * the printOn() member function.
 */
template <class TypeT>
inline std::ostream&  operator<<(std::ostream& s, const RWHermBandMat<TypeT>& m)
{
    m.printOn(s);
    return s;
}

/**
 * @relates RWHermBandMat
 *
 * Reads the matrix from the stream. This is equivalent to calling
 * the scanFrom() member function.
 */
template <class TypeT>
inline std::istream&  operator>>(std::istream& s, RWHermBandMat<TypeT>& m)
{
    m.scanFrom(s);
    return s;
}

/**
 * @relates RWHermBandMat
 *
 * Returns the transpose of the argument matrix.
 */
template <class TypeT>
RWHermBandMat<TypeT>  transpose(const RWHermBandMat<TypeT>&);

/**
 * @relates RWHermBandMat
 *
 * Returns the inner product (matrix-vector product) of \a A and
 * \a B.
 */
template <class TypeT>
RWMathVec<TypeT>  product(const RWHermBandMat<TypeT>& A, const RWMathVec<TypeT>& x);

/**
 * @relates RWHermBandMat
 *
 * Returns the inner product (matrix-vector product) of \a x and
 * \a A. This is equal to the product of \a A transpose and \a x.
 */
template <class TypeT>
RWMathVec<TypeT>  product(const RWMathVec<TypeT>& x, const RWHermBandMat<TypeT>& A);

// Below are functions that assume automatic conversion to GenMat's
// Automatic conversions do not take place for templatized functions,
// So we provide them here...
template <class TypeT>
RWGenMat<TypeT>  product(const RWHermBandMat<TypeT>& A, const RWGenMat<TypeT>& B);

template <class TypeT>
RWGenMat<TypeT>  product(const RWGenMat<TypeT>& A, const RWHermBandMat<TypeT>& B);

template <class TypeT>
RWGenMat<TypeT>  product(const RWHermBandMat<TypeT>& A, const RWHermBandMat<TypeT>& B);

template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWHermBandMat<TypeT>& A, const RWGenMat<TypeT>& B);

template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWGenMat<TypeT>& A, const RWHermBandMat<TypeT>& B);

template <class TypeT>
RWGenMat<TypeT>  transposeProduct(const RWHermBandMat<TypeT>& A, const RWHermBandMat<TypeT>& B);

template <class TypeT>
RWGenMat<TypeT>  conjTransposeProduct(const RWHermBandMat<TypeT>& A, const RWGenMat<TypeT>& B);

template <class TypeT>
RWGenMat<TypeT>  conjTransposeProduct(const RWGenMat<TypeT>& A, const RWHermBandMat<TypeT>& B);

template <class TypeT>
RWGenMat<TypeT>  conjTransposeProduct(const RWHermBandMat<TypeT>& A, const RWHermBandMat<TypeT>& B);

template <class TypeT>
inline RWGenMat<TypeT>  adjoint(const RWHermBandMat<TypeT>& A)
{
    RWGenMat<TypeT> a(A);
    return adjoint(a);
}

/**
 * @relates RWHermBandMat
 *
 * Extracts the Hermitian part of a band of entries from a square
 * matrix. The main diagonal is extracted, along with \a h subdiagonals
 * and superdiagonals. If <i>B</i> is this band, the Hermitian part
 * of the band is \f$ (B+conj(B_{T}))/2 \f$.
 */
template <class TypeT>
RWHermBandMat<TypeT>  toHermBandMat(const RWGenMat<TypeT>& A, unsigned h);

/**
 * @relates RWBandMat
 *
 * Extracts the Hermitian part of a banded matrix. The Hermitian
 * part of the banded matrix \a A is \f$ (A + conj( A^{T}))/2 \f$.
 */
template <class TypeT>
RWHermBandMat<TypeT>  toHermBandMat(const RWBandMat<TypeT>& A);

/**
 * @relates RWHermBandMat
 *
 * Returns a matrix whose entries are the absolute value of the
 * argument. The absolute value of a complex number is considered
 * to be the sum of the absolute values of its real and imaginary
 * parts. To get the norm of a complex matrix, you can use the
 * norm() function.
 */
template <class TypeT>
RWSymBandMat<double>  abs(const RWHermBandMat<TypeT>& A);

/**
 * @relates RWHermBandMat
 *
 * Returns a matrix where each element is the complex conjugate
 * of the corresponding element in the matrix \a A.
 */
template <class TypeT>
RWHermBandMat<TypeT>  conj(const RWHermBandMat<TypeT>& A);

/**
 * @relates RWHermBandMat
 *
 * Returns a matrix where each element is the real part of the corresponding
 * element in the matrix \a A.
 */
template <class TypeT>
RWSymBandMat<double>  real(const RWHermBandMat<TypeT>& A);

/**
 * @relates RWHermBandMat
 *
 * Returns a matrix where each element is the norm (magnitude) of
 * the corresponding element in the matrix \a A.
 */
template <class TypeT>
RWSymBandMat<double>  norm(const RWHermBandMat<TypeT>& A);


#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/hbndmat.cc>
#endif

#endif
