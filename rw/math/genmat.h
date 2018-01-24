#ifndef __RWGENMAT_H__
#define __RWGENMAT_H__

/***************************************************************************
 *
 * genmat.h
 *
 * $Id: //math/13/rw/math/genmat.h#2 $
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
 **************************************************************************/

/*
 * Defining the preprocessor directive RWBOUNDS_CHECK will
 * cause bounds checking on the subscripting operators
 * and inlined math functions.
 */
#if defined(RW_MATH_MATRIX_PRODUCT)
#  error macro RW_MATH_MATRIX_PRODUCT is no longer supported. See current docs.
#endif


// Working around AIX VA 9.0 error evaluating default argument for inc_row()
// RW# 32303

#if (__IBMCPP__ == 900)
#  define INC_ROW(ARG1) ARG1.inc_row(1)
#else
#  define INC_ROW(ARG1) ARG1.inc_row()
#endif


#include <iterator>
#include <rw/dcomplex.h>
#include <rw/math/funcobj.h>
#include <rw/math/mathvec.h>
#include <rw/math/numtrait.h>
#include <rw/math/rwalloc.h>
#include <rw/matrix.h>
#include <rw/rand.h>
#include <rw/tools/iterator.h>

#if defined(RW_BROKEN_GCC_MATH_TEMPLATE)
// __promote and __promote_2 helper functions available with c++0x
// and gnu++0x flag is broken for some versions of gcc.  This causes
// overloads of math function like pow to not be selected during
// template instantiation.
// To work around this bug, specialization of those helper templates
// will have to be defined for each user class for which overloads of
// those math functions are defined.
namespace __gnu_cxx
{
template<class T> struct __promote_2 <RWGenMat<std::complex<T> >, T> { };

template<class T> struct __promote_2 <T, RWGenMat<std::complex<T> > > { };
}
#endif

template<class T>
class RWGenMatPick;

/**
 * @ingroup math_collection_classes
 * @brief The base class for RWGenMat iterators.
 *
 * A base class from which the iterators for RWGenMat are derived.
 */
template <class T>
class RWGenMatIteratorBase
{
public:
    /**
     * Typedef for the type of result from subtracting two iterators
     * obtained from this container.
     */
    typedef typename RWGenMat<T>::difference_type difference_type;

    /**
     * Typedef for the type of elements in this container.
     */
    typedef typename RWGenMat<T>::value_type      value_type;

    typedef typename RWGenMat<T>::pointer         pointer;

    /**
     * Typedef for a const pointer to a value in this container.
     */
    typedef typename RWGenMat<T>::const_pointer   const_pointer;

    friend class RWGenMat<T>;

protected:
    //Constructors -----------------------------------------------------
    RWGenMatIteratorBase() {
        ;
    }
    RWGenMatIteratorBase(T* d, size_t nrows,  size_t ncols,
                         size_t row, size_t col,
                         int rowstr, int colstr,
                         RWDataView::Storage storage
                        ) {
        // start the body here
        if (storage == RWDataView::ROW_MAJOR ||
                ((storage == RWDataView::RWEITHER) && colstr < rowstr)) {
            nrows_ = nrows;
            ncols_ = ncols;
            row_ = row;
            col_ = col;
            rowstep_ = rowstr;
            colstep_ = colstr;
        }
        else {
            nrows_ = ncols;
            ncols_ = nrows;
            row_ = col;
            col_ = row;
            rowstep_ = colstr;
            colstep_ = rowstr;
        }
        if (d != NULL) {
            data_ = d + row_ * rowstep_ + col_ * colstep_;
        }
        else {
            data_ = NULL;
            row_ = 0;
            col_ = 0;
        }
    }

#if defined(RW_MATH_REQUIRES_BASE_COPY_CONSTRUCTOR)
public:
#endif
    RWGenMatIteratorBase(const RWGenMatIteratorBase<T>& x)
        : data_(x.data_)
        , nrows_(x.nrows_)
        , ncols_(x.ncols_)
        , rowstep_(x.rowstep_)
        , colstep_(x.colstep_)
        , row_(x.row_)
        , col_(x.col_) {
        ;
    }

protected:
    //Operators --------------------------------------------------------

    // Prefix increment and decrement operators
    RWGenMatIteratorBase<T>& operator_plus_plus() {
        if (col_ == ncols_ - 1) {
            data_ += (1 - ncols_) * colstep_ + rowstep_;
            col_ = 0;
            ++row_;
        }
        else {
            data_ += colstep_;
            ++col_;
        }
        return *this;
    }

    RWGenMatIteratorBase<T>& operator_minus_minus() {
        if (col_ == 0) {
            data_ += (ncols_ - 1) * colstep_ - rowstep_;
            col_ = ncols_ - 1;
            --row_;
        }
        else {
            data_ -= colstep_;
            --col_;
        }
        return *this;
    }

    // Assignment operators
    RWGenMatIteratorBase<T>& operator_plus_equal(difference_type d) {
        if (d < 0) {
            return (operator_minus_equal(-d));
        }

        if (col_ + d >= ncols_) {
            data_ += ((col_ + d) % ncols_ - col_) * colstep_ + ((col_ + d) / ncols_) * rowstep_;
            row_ += (col_ + d) / ncols_;
            col_ = (col_ + d) % ncols_;
        }
        else {
            data_ += d * colstep_;
            col_ += d;
        }
        return *this;
    }

    RWGenMatIteratorBase<T>& operator_minus_equal(difference_type d) {
        if (d < 0) {
            return (operator_plus_equal(-d));
        }

        if (col_ < d % ncols_) {
            data_ += (ncols_ - d % ncols_) * colstep_ - (1 + d / ncols_) * rowstep_;
            col_ += ncols_ - d % ncols_;
            row_ -= (1 + d / ncols_);
        }
        else {
            data_ += -(d % int(ncols_)) * colstep_ - (d / int(ncols_)) * rowstep_;
            col_ += -d % ncols_;
            row_ -= d / ncols_;
        }
        return *this;
    }

    // Binary operators
    difference_type operator_minus(const RWGenMatIteratorBase<T>& x) const {
        return (ncols_ * (row_ - x.row_) + col_ - x.col_);
    }

    RWGenMatIteratorBase<T> operator_minus(difference_type n) const {
        RWGenMatIteratorBase<T> tmp = *this;
        return tmp.operator_minus_equal(n);
    }

    RWGenMatIteratorBase<T> operator_plus(difference_type n) const {
        RWGenMatIteratorBase<T> tmp = *this;
        return tmp.operator_plus_equal(n);
    }

    // Subscripting operator
    value_type& operator_square_brackets(difference_type n) const {
        return *(data_ + n);
    }

    // Indirection operator
    value_type& operator_star() const {
        return *data_;
    }

    pointer operator_arrow() const {
        return &(*data_);
    }

public:
    /**
     * Equality operator.  Returns \c true if the \link RWGenMatIteratorBase
     * RWGenMatIteratorBase<T>\endlink objects point to the same element.
     */
    bool operator==(const RWGenMatIteratorBase<T>& x) const {
        return (row_ == x.row_) && (col_ == x.col_);
    }

    /**
     * Inequality operator.  Returns \c false if the \link RWGenMatIteratorBase
     * RWGenMatIteratorBase<T>\endlink objects point to the same element.
     */
    bool operator !=(const RWGenMatIteratorBase<T>& x) const {
        return !(*this == x);
    }

    /**
     * Less than operator.  Determines the relative logical position
     * between two \link RWGenMatIteratorBase RWGenMatIteratorBase<T>\endlink
     * objects, not necessarily the relative memory location of the elements to
     * which they point.
     */
    bool operator <(const RWGenMatIteratorBase<T>& x) const {
        return (operator_minus(x) < 0);
    }

    /**
     * Greater than operator.  Determines the relative logical position
     * between two \link RWGenMatIteratorBase RWGenMatIteratorBase<T>\endlink
     * objects, not necessarily the relative memory location of the elements to
     * which they point.
     */
    bool operator >(const RWGenMatIteratorBase<T>& x) const {
        return !(*this < x) && !(*this == x);
    }

    /**
     * Less than or equal to operator.  Determines the relative logical position
     * between two \link RWGenMatIteratorBase RWGenMatIteratorBase<T>\endlink
     * objects, not necessarily the relative memory location of the elements to
     * which they point.
     */
    bool operator <=(const RWGenMatIteratorBase<T>& x) const {
        return (*this < x) || (*this == x);
    }

    /**
     * Greater than or equal to operator.  Determines the relative logical
     * position between two \link RWMathVecIteratorBase
     * RWMathVecIteratorBase<T>\endlink objects, not necessarily the relative
     * memory location of the elements to which they point.
     */
    bool operator >=(const RWGenMatIteratorBase<T>& x) const {
        return !(*this < x);
    }

    size_t getRow() const {
        return row_;
    }
    size_t getCol() const {
        return col_;
    }
    void inc_row(difference_type n = 1) {
        data_ += n * rowstep_;
        row_ += n;
    }
    int stride(void) const {
        return colstep_;
    }
    size_t row_length(void) const {
        return ncols_;
    }

private:

    value_type*  data_;
    size_t        nrows_;
    size_t        ncols_;
    int           rowstep_;
    int           colstep_;
    size_t        row_;
    size_t        col_;
}; // end of RWGenMatIteratorBase


/**
 * @ingroup math_collection_classes
 * @brief The random access iterator for the
 * \link RWGenMat RWGenMat<T> \endlink collection class.
 *
 * Class RWGenMatIterator is the random access
 * iterator for the \link RWGenMat RWGenMat<T> \endlink collection
 * class. The RWGenMatIterator class behaves like a pointer to type \c T, and
 * is used to increment through elements of an \link RWGenMat RWGenMat<T>
 * \endlink.
 *
 * @note This class should be accessed as the typedef \link RWGenMat::iterator RWGenMat<T>::iterator\endlink.

 * @section example Example
 *
 * @code
 * #include <rw/math/genmat.h>
 *
 * int main()
 * {
 *     RWGenMat<int> M(5,5,rwUninitialized);
 *     RWGenMat<int>::iterator i = M.begin();
 *     RWGenMat<int>::iterator stop = M.end();
 *
 *     // Assign a unique int value to each element of M
 *     for (int j=0; i != stop; ++i, ++j) *i = j;
 * }
 * @endcode
 *
 */
template <class T>
class RWGenMatIterator
    : public RWGenMatIteratorBase<T>,
  public std::iterator < std::random_access_iterator_tag,
      typename RWGenMat<T>::value_type,
      typename RWGenMat<T>::difference_type
      >
{
public:
    typedef typename RWGenMat<T>::difference_type difference_type;
    typedef typename RWGenMat<T>::value_type      value_type;
    typedef typename RWGenMat<T>::pointer         pointer;
    typedef typename RWGenMat<T>::const_pointer   const_pointer;

    friend class RWGenMat<T>;
    friend class const_iterator;

    /**
     * Constructs an iterator that is in an uninitialized state. The
     * iterator must be initialized before it is dereferenced.
     */
    RWGenMatIterator() {
        ;
    }
    RWGenMatIterator(T* d, size_t nrows,  size_t ncols,
                     size_t row, size_t col,
                     int rowstr, int colstr,
                     RWDataView::Storage storage = RWDataView::COLUMN_MAJOR
                    ) : RWGenMatIteratorBase<T>(d, nrows, ncols, row, col, rowstr, colstr, storage) {
        ;
    }

    /**
     * Prefix increment operator. The iterator
     * points to the next element in the \link RWGenMat
     * RWGenMat<T> \endlink. No error condition is set if the iterator
     * goes past the bounds of the matrix. Compare to RWGenMat<T>::begin()
     * and RWGenMat<T>::end() to check that the iterator position is
     * valid.
     */
    RWGenMatIterator<T>& operator ++() {
        this->operator_plus_plus();
        return *this;
    }

    /**
     * Prefix decrement operator. The iterator
     * points to the previous element in the \link RWGenMat
     * RWGenMat<T> \endlink. No error condition is set if the iterator
     * goes past the bounds of the matrix. Compare to RWGenMat<T>::begin()
     * and RWGenMat<T>::end() to check that the iterator position is
     * valid.
     */
    RWGenMatIterator<T>& operator --() {
        this->operator_minus_minus();
        return *this;
    }

    // Postfix increment and decrement operators if ya got'em
#if !defined(RW_NO_POSTFIX)
    /**
     * Postfix increment operator. The iterator
     * points to the next element in the \link RWGenMat
     * RWGenMat<T> \endlink. No error condition is set if the iterator
     * goes past the bounds of the matrix. Compare to RWGenMat<T>::begin()
     * and RWGenMat<T>::end() to check that the iterator position is
     * valid.
     */
    RWGenMatIterator<T> operator++(int) {
        RWGenMatIterator<T> tmp = *this;
        ++(*this);
        return tmp;
    }

    /**
     * Postfix decrement operator. The iterator
     * points to the previous element in the \link RWGenMat
     * RWGenMat<T> \endlink. No error condition is set if the iterator
     * goes past the bounds of the matrix. Compare to RWGenMat<T>::begin()
     * and RWGenMat<T>::end() to check that the iterator position is
     * valid.
     */

    RWGenMatIterator<T> operator--(int) {
        RWGenMatIterator<T> tmp = *this;
        --(*this);
        return tmp;
    }
#endif

    /**
     * The RWGenMatIterator is moved forward \a d elements in the
     * \link RWGenMat RWGenMat<T> \endlink. No error condition is set
     * if the iterator goes past the bounds of the matrix. Incrementing
     * by \a d and then decrementing by \a d returns the iterator to
     * its original position, even if doing so takes it past the bounds
     * of the matrix.
     */
    RWGenMatIterator<T>& operator +=(difference_type d) {
        this->operator_plus_equal(d);
        return *this;
    }

    /**
     * The RWGenMatIterator is moved backward \a d elements in the
     * \link RWGenMat RWGenMat<T> \endlink. No error condition is set
     * if the iterator goes past the bounds of the matrix. Incrementing
     * by \a d and then decrementing by \a d returns the iterator to
     * its original position, even if doing so takes it past the bounds
     * of the matrix.
     */
    RWGenMatIterator<T>& operator -=(difference_type d) {
        this->operator_minus_equal(d);
        return *this;
    }

    /**
     * If the return value is positive, the RWGenMatIterator is that many elements
     * past the RWGenMatIterator \a x. If the return value is negative, the
     * RWGenMatIterator is that many elements before the RWGenMatIterator \a x.
     */
    difference_type operator -(const RWGenMatIteratorBase<T>& x) const {
        return this->operator_minus(x);
    }

    /**
     * Returns an RWGenMatIterator that is \a n elements past or before self.
     */
    RWGenMatIterator<T> operator -(difference_type n) const {
        RWGenMatIterator<T> tmp = *this;
        return tmp -= n;
    }

    /**
     * Returns an RWGenMatIterator that is \a n elements past or before self.
     */
    RWGenMatIterator<T> operator +(difference_type n) const {
        RWGenMatIterator<T> tmp = *this;
        return tmp += n;
    }

    /**
     * Returns a reference to the element \a n elements after self if
     * \a n is positive, or \a n elements before \a n if \a n is negative.
     */
    value_type& operator [](difference_type n) const {
        return this->operator_square_brackets(n);
    }

    /**
     * Returns a reference to the element pointed to by self.
     */
    value_type& operator *() const {
        return this->operator_star();
    }

    /**
     *   For iterator a, and identifier m, a->m is equivalent to (*a).m.
     */
    pointer operator->() const {
        return this->operator_arrow();
    }

    // Comparison operators inherited from RWGenMatIteratorBase
}; // end of inner class iterator


/**
 * @ingroup math_collection_classes
 * @brief The random access iterator for the
 * \link RWGenMat RWGenMat<T> \endlink collection class.
 *
 * Class RWGenMatConstIterator is the random access
 * iterator for the \link RWGenMat RWGenMat<T> \endlink collection
 * class. The RWGenMatConstIterator class behaves like a pointer to type
 * T and is used to increment through elements of an \link RWGenMat
 * RWGenMat<T> \endlink. The RWGenMatConstIterator differs from a plain
 * RWGenMatIterator in that the value of a dereferenced RWGenMatConstIterator
 * may not be changed.
 *
 * @note This class should be accessed as the typedef \link RWGenMat::const_iterator RWGenMat<T>::const_iterator\endlink.
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/math/genmat.h>
 *
 * int main()
 * {
 *     RWGenMat<int> V(5,1); // Create a length 5 matrix
 *                           // with all 1's
 *     RWGenMat<int>::const_iterator i = V.begin();
 *     RWGenMat<int>::const_iterator stop = V.end();
 *
 *     // print each element of V
 *     while(i != stop) {
 *         std::cout << *i << std::endl;
 *         ++i;
 *     }
 * }
 * @endcode
 *
 */
template <class T>
class RWGenMatConstIterator
    : public RWGenMatIteratorBase<T>,
  public std::iterator < std::random_access_iterator_tag,
      typename RWGenMat<T>::value_type,
      typename RWGenMat<T>::difference_type
      >
{
public:
    typedef typename RWGenMat<T>::difference_type difference_type;
    typedef typename RWGenMat<T>::value_type      value_type;
    typedef typename RWGenMat<T>::pointer         pointer;
    typedef typename RWGenMat<T>::const_pointer   const_pointer;

    friend class RWGenMat<T>;

    /**
     * Constructs an iterator that is in an uninitialized state. The
     * iterator must be initialized before it is dereferenced.
     */
    RWGenMatConstIterator() {
        ;
    }
    RWGenMatConstIterator(T* d, size_t nrows,  size_t ncols,
                          size_t row, size_t col,
                          int rowstr, int colstr,
                          RWDataView::Storage storage = RWDataView::COLUMN_MAJOR
                         ) : RWGenMatIteratorBase<T>(d, nrows, ncols, row, col, rowstr, colstr, storage) {
        ;
    }

    /**
     * Constructs a RWGenMatConstIterator from another RWGenMatIterator or
     * RWGenMatConstIterator. Initialized RWGenMatIterator and
     * RWGenMatConstIterator instances can be obtained
     * via the RWGenMat<T>::begin() and RWGenMat<T>::end() member functions of
     * \link RWGenMat RWGenMat<T> \endlink.
     */
    RWGenMatConstIterator(const RWGenMatIteratorBase<T>& x)
        : RWGenMatIteratorBase<T>(x) {
        ;
    }

    /**
     * Prefix increment operator. The
     * RWGenMatConstIterator points to the next element in the
     * \link RWGenMat RWGenMat<T> \endlink. No error condition is set if the
     * RWGenMatConstIterator goes past the bounds of the matrix. Compare to
     * RWGenMat<T>::begin() and RWGenMat<T>::end() to check that the
     * RWGenMatConstIterator position is valid.
     */
    RWGenMatConstIterator<T>& operator ++() {
        this->operator_plus_plus();
        return *this;
    }

    /**
     * Prefix decrement operator. The
     * RWGenMatConstIterator points to the previous element in the
     * \link RWGenMat RWGenMat<T> \endlink. No error condition is set if the
     * RWGenMatConstIterator goes past the bounds of the matrix. Compare to
     * RWGenMat<T>::begin() and RWGenMat<T>::end() to check that the
     * RWGenMatConstIterator position is valid.
     */

    RWGenMatConstIterator<T>& operator --() {
        this->operator_minus_minus();
        return *this;
    }

    /**
     * Postfix increment operator. The
     * RWGenMatConstIterator points to the next element in the
     * \link RWGenMat RWGenMat<T> \endlink. No error condition is set if the
     * RWGenMatConstIterator goes past the bounds of the matrix. Compare to
     * RWGenMat<T>::begin() and RWGenMat<T>::end() to check that the
     * RWGenMatConstIterator position is valid.
     */

    RWGenMatConstIterator<T> operator++(int) {
        RWGenMatConstIterator<T> tmp = *this;
        ++(*this);
        return tmp;
    }

    /**
     * Postfix decrement operator. The
     * RWGenMatConstIterator points to the previous element in the
     * \link RWGenMat RWGenMat<T> \endlink. No error condition is set if the
     * RWGenMatConstIterator goes past the bounds of the matrix. Compare to
     * RWGenMat<T>::begin() and RWGenMat<T>::end() to check that the
     * RWGenMatConstIterator position is valid.
     */

    RWGenMatConstIterator<T> operator--(int) {
        RWGenMatConstIterator<T> tmp = *this;
        --(*this);
        return tmp;
    }

    /**
     * The RWGenMatConstIterator is moved forward \a d elements
     * in the \link RWGenMat RWGenMat<T> \endlink. No error condition
     * is set if the RWGenMatConstIterator goes past the bounds of the matrix.
     * Incrementing by \a d and then decrementing by \a d returns the
     * RWGenMatConstIterator to its original position, even if doing so
     * takes it past the bounds of the matrix.
     */
    RWGenMatConstIterator<T>& operator +=(difference_type d) {
        return this->operator_plus_equal(d), *this;
    }

    /**
     * The RWGenMatConstIterator is moved backward \a d elements
     * in the \link RWGenMat RWGenMat<T> \endlink. No error condition
     * is set if the RWGenMatConstIterator goes past the bounds of the matrix.
     * Incrementing by \a d and then decrementing by \a d returns the
     * RWGenMatConstIterator to its original position, even if doing so
     * takes it past the bounds of the matrix.
     */
    RWGenMatConstIterator<T>& operator -=(difference_type d) {
        return this->operator_minus_equal(d), *this;
    }

    /**
     * If the return value is positive, the RWGenMatConstIterator is that
     * many elements past the RWGenMatConstIterator \a x. If the return
     * value is negative, the RWGenMatConstIterator is that many elements
     * before the RWGenMatConstIterator \a x.
     */
    difference_type operator -(const RWGenMatIteratorBase<T>& x) const {
        return this->operator_minus(x);
    }

    /**
     * Returns a RWGenMatConstIterator that is \a n elements past (or before)
     * self.
     */
    RWGenMatConstIterator<T> operator -(difference_type n) const {
        RWGenMatConstIterator<T> tmp = *this;
        return tmp -= n;
    }

    /**
     * Returns a RWGenMatConstIterator that is \a n elements past (or before)
     * self.
     */
    RWGenMatConstIterator<T> operator +(difference_type n) const {
        RWGenMatConstIterator<T> tmp = *this;
        return tmp += n;
    }

    /**
     * Returns a reference to the element \a n elements after self if
     * \a n is positive, or before self if \a n is negative.
     */
    const value_type& operator [](difference_type n) const {
        return this->operator_square_brackets(n);
    }

    /**
     * Returns a reference to the element pointed to by self.
     */
    const value_type& operator *() const {
        return this->operator_star();
    }

    /**
     *   For iterator a, and identifier m, a->m is equivalent to (*a).m.
     */
    const_pointer operator ->() {
        return this->operator_arrow();
    }

    // Comparison operators inherited from RWGenMatIteratorBase
};


template <class T>
RWGenMatIterator<T> operator+(const typename RWGenMatIterator<T>::difference_type i, const RWGenMatIterator<T>& it)
{
    return it + i;
}

template <class T>
RWGenMatConstIterator<T> operator+(const typename RWGenMatConstIterator<T>::difference_type i, const RWGenMatConstIterator<T>& it)
{
    return it + i;
}

//**************************************************************************
//
//-----------------------  END OF ITERATOR CLASSES -------------------------
//
//**************************************************************************


typedef RWGenMat<int> RWIntMat;

// Predeclare template friend function
template <class T> RWGenMat<T> toGenMat(const RWMathArray<T>&);

/**
 * @ingroup math_collection_classes
 * @brief A templatized general matrix class.
 *
 * Class \link RWGenMat RWGenMat<T> \endlink is a templatized general
 * matrix class.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/math/genmat.h>
 * template<class T>
 * RWGenMat<T> matrix;
 * @endcode

 * @section example Example
 *
 * @code
 * #include <rw/math/genmat.h>
 *
 * int main()
 * {
 *    RWGenMat<int> A(3,5,rwUninitialized), B(5,2,rwUninitialized);
 *    A = 3;                    // Set all elements in A to 3
 *    B(RWAll,0) = 1;           // Set first column of B to 1
 *    B(RWAll,1) = 3;           // Set second column of B to 3
 *    RWGenMat<int> AB = product(A,B);
 * }
 * @endcode
 *
 *
 * @see \link RWConvertGenMat RWConvertGenMat<From,To> \endlink
 *
 */
template <class T>
class RWGenMat : public RWMatView
{
public:

    // typedefs

    // STL style typedefs

    typedef size_t    size_type;
    typedef ptrdiff_t difference_type;
    typedef T*        pointer;
    typedef const T*  const_pointer;
    typedef T         value_type;

    /**
     * A type that provides a random-access iterator over the elements
     * in the container.
     */
    typedef RWGenMatIterator<T> iterator;

    /**
     * A type that provides a \c const random-access iterator over the
     * elements in the container.
     */
    typedef RWGenMatConstIterator<T> const_iterator;

    /**
     * A type that provides a random-access, reverse-order iterator over
     * the elements in the container.
     */
#if !defined(RW_STDLIB_VER) || !defined(_RWSTD_NO_CLASS_PARTIAL_SPEC)
    typedef std::reverse_iterator<iterator> reverse_iterator;
#else
    typedef std::reverse_iterator < iterator,
            typename rw_iterator_traits<iterator>::iterator_category,
            typename rw_iterator_traits<iterator>::value_type,
            typename rw_iterator_traits<iterator>::reference,
            typename rw_iterator_traits<iterator>::pointer,
            typename rw_iterator_traits<iterator>::difference_type > reverse_iterator;
#endif

    /**
     * A type that provides a \c const random-access, reverse-order iterator
     * over the elements in the container.
     */
#if !defined(RW_STDLIB_VER) || !defined(_RWSTD_NO_CLASS_PARTIAL_SPEC)
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
#else
    typedef std::reverse_iterator < const_iterator,
            typename rw_iterator_traits<const_iterator>::iterator_category,
            typename rw_iterator_traits<const_iterator>::value_type,
            typename rw_iterator_traits<const_iterator>::reference,
            typename rw_iterator_traits<const_iterator>::pointer,
            typename rw_iterator_traits<const_iterator>::difference_type > const_reverse_iterator;
#endif

    // Numeric traits typedefs
    typedef typename rw_numeric_traits<T>::c_type          c_type;

    /**
     * Typedef for the promotion type. For more information,
     * see rw_numeric_traits<T>::promote_type.
     */
    typedef typename rw_numeric_traits<T>::promote_type    promote_type;

    /**
     * Typedef for the usual return type of numerical <tt>norm</tt>-like
     * functions. For more information, see rw_numeric_traits<T>::norm_type.
     */
    typedef typename rw_numeric_traits<T>::norm_type       norm_type;

    /**
     * Typedef for the function pointer used in the method apply().
     * For more information, see rw_numeric_traits<T>::mathFunType.
     */
    typedef typename rw_numeric_traits<T>::mathFunType     mathFunType;

    /**
     * Typedef for the function pointer used in the method apply2().
     * For more information, see rw_numeric_traits<T>::mathFunType2.
     */
    typedef typename rw_numeric_traits<T>::mathFunType2    mathFunType2;


private:
    // This constructor is used internally by real() and imag(), the
    // slice() function and the Array::op()() functions.
    RWGenMat(const RWDataView& b, T* start, size_t m, size_t n, int rowstr, int colstr) // For internal use
        : RWMatView(b, start, m, n, rowstr, colstr) {
        ;
    }

    friend RW_MATH_SYMBOLIC RWGenMat<double> real(const RWGenMat<DComplex>&);
    friend RW_MATH_SYMBOLIC RWGenMat<double> imag(const RWGenMat<DComplex>&);
    // Doxygen Enhancement Request 648302
    // Disable friend declaration so DOXYGEN will associate this method with
    // RWMathArray
    // If we could override the friend relationship to document this method
    // on RWMathArray this macro code would be unnecessary.
#if !defined(DOXYGEN)
    friend RWGenMat<T> toGenMat RWMATH_FRIEND_TEMPLATE_SPECIFIER(const RWMathArray<T>&);
#endif

    /*****************************
     * Routines for internal use *
     *****************************/

    /* These slice functions dispense with bounds checking for speed */
    RWMathVec<T> fastSlice(int i, int j, size_t n, int rowstr, int colstr) const {
        return RWMathVec<T>(*this, (T*)data() + i * rowstep + j * colstep, n, rowstr * rowstep + colstr * colstep);
    }

    RWGenMat<T> fastSlice(int i, int j, size_t m, size_t n, int rowstr1, int colstr1, int rowstr2, int colstr2) const {
        return RWGenMat<T>(*this, (T*)data() + i * rowstep + j * colstep, m, n,
                           rowstr1 * rowstep + colstr1 * colstep,
                           rowstr2 * rowstep + colstr2 * colstep);
    }

public:

    /**
     * Constructs a 0 x 0 (null) matrix, useful for declaring vectors
     * of matrices. This matrix, like any other matrix, can subsequently
     * be reshaped or resized. See member functions reshape() and resize().
     */
    RWGenMat() : RWMatView() {}

#ifdef RW_ALLOW_OLD_ARRAY_CONSTRUCTORS
    RWGenMat(size_t m, size_t n, Storage s = COLUMN_MAJOR)
        : RWMatView(new RWAllocBlock<T, RWDefaultAllocator<T> >(m* n), m, n, s) {}
#endif

    /**
     * Constructs a matrix with a specified number of rows and columns.
     * The optional storage indicator determines whether the matrix
     * is stored in \c ROW_MAJOR or \c COLUMN_MAJOR order. The #RWUninitialized
     * type is an enumeration type with only one value, #rwUninitialized.
     * The #rwUninitialized argument is used to distinguish the last
     * dimension size from an initial value.
     */
    RWGenMat(size_t m, size_t n, RWUninitialized, Storage s = COLUMN_MAJOR)
        : RWMatView(new RWAllocBlock<T, RWDefaultAllocator<T> >(m* n), m, n, s) {}

    /**
     * Constructs a matrix with \a m rows and \a n columns. Initialized
     * with random numbers generated by \a r. The optional storage indicator
     * determines whether the matrix is stored in \c ROW_MAJOR or \c
     * COLUMN_MAJOR order.
     */
    RWGenMat(size_t m, size_t n, RWTRand<RWRandGenerator>& r,
             Storage = COLUMN_MAJOR);

    /**
     * Constructs a matrix with a specified number of rows and columns.
     * Initializes each matrix element to \a initval. The optional storage
     * indicator determines whether the matrix is stored in \c ROW_MAJOR
     * or \c COLUMN_MAJOR order.
     */
    RWGenMat(size_t m, size_t n, const T& initval, Storage storage = COLUMN_MAJOR)
        : RWMatView(new RWAllocBlock<T, RWDefaultAllocator<T> >(m* n), m, n, storage) {
        RWBlas_util<T>::set(n * m, data(), 1, &initval);
    }

    /**
     * Constructs a matrix from the null terminated character string
     * \a s. The format of the character string is the same as that
     * expected by the global operator
     * \link operator>>(std::istream&, RWGenMat<T>&) operator>> \endlink
     * described in this
     * entry. The optional storage indicator determines whether the
     * matrix is stored in \c ROW_MAJOR or \c COLUMN_MAJOR order.
     */
    RWGenMat(const char* s, Storage = COLUMN_MAJOR);

    /**
     * Copy constructor. The new matrix and the old matrix both view
     * the same data.
     */
    RWGenMat(const RWGenMat<T>& a) : RWMatView((const RWMatView&)a) {}  // cast needed by Zortech 3.1

    RWGenMat(const RWGenMatPick<T>& p);

    /**
     * A matrix with \a m rows and \a n columns is constructed, using
     * the data in the vector \a dat as initial data. A copy of \a dat
     * is made. The vector \a dat must have at least \a n * \a m elements.
     * The optional storage indicator determines whether the matrix
     * is stored in \c ROW_MAJOR or \c COLUMN_MAJOR order.
     */
    RWGenMat(const T* dat, size_t m, size_t n, Storage = COLUMN_MAJOR);

    /**
     * A matrix with \a m rows and \a n columns is constructed, using
     * the data in the vector \a v. The matrix is a new view of the
     * same data as \a v, so no copy of the data is made. The optional
     * storage indicator determines whether the matrix is stored in
     * \c ROW_MAJOR or \c COLUMN_MAJOR order. If the vector does not
     * have length \a m times \a n, an exception of type \c MATX_NUMBERPOINTS
     * is thrown.
     */
    RWGenMat(const RWMathVec<T>& vec, size_t m, size_t s, Storage = COLUMN_MAJOR);

    RWGenMat(RWBlock* block,    size_t, size_t, Storage = COLUMN_MAJOR); // Use a custom block for the data

    /**
     * A complex matrix is constructed from the double precision matrices
     * \a re and \a im, with the real part of the matrix equal to \a
     * re and the imaginary part equal to \a im. A new copy of the data
     * is made. The optional storage indicator determines whether the
     * matrix is stored in \c ROW_MAJOR or \c COLUMN_MAJOR order.
     */
    RWGenMat(const RWGenMat<double>& re, const RWGenMat<double>& im, Storage = COLUMN_MAJOR);


    /********************
     * Member functions *
     ********************/
    //
    //  Added function typedef
    typedef T(*mathFuncType)(T);
    typedef typename rw_numeric_traits<T>::norm_type(*mathFuncType2)(T);
#ifdef RW_NATIVE_EXTENDED
    typedef T(*XmathFuncType)(T);
#endif

#ifndef RW_INLINE_FUNC_PTR_ARG
    /**
     * Returns the result of applying the passed function to every element
     * in the matrix. A function of type RWGenMat<T>::mathFunType
     * takes and returns a \c T. A function of type RWGenMat<T>::mathFunType2
     * takes a \c T and returns an
     * RWGenMat<T>::norm_type. For a description of this type, see
     * \link rw_numeric_traits rw_numeric_traits<T> \endlink.
     */
    RWGenMat<T> apply(mathFunType f) const;
#else
    RWGenMat<T> apply(mathFunType f) const {
        RWGenMat<T> temp(nrows, ncols, rwUninitialized);
        typename RWGenMat<T>::const_iterator it;
        typename RWGenMat<T>::iterator it_temp;
        typename RWGenMat<T>::const_iterator last = end();
        for (it = begin(), it_temp = temp.begin(); it < last; ++it, ++it_temp) {
            *it_temp = (*f)(*it);
        }
        return temp;
    }
#endif

    // Some compilers (Borland 4.5) only understand functions
    // templatized on a traits type if they are defined in line
    // at class scope.
#ifdef RW_INLINE_TRAITS_TEMPLATES
    RWGenMat<norm_type> apply2(mathFunType2 f) const {
        RWGenMat<norm_type> temp(rows(), cols(), rwUninitialized);
        register norm_type* dp = (norm_type*)temp.data();
        typename RWGenMat<T>::const_iterator it;
        typename RWGenMat<norm_type>::iterator it_temp;
        typename RWGenMat<T>::const_iterator last = end();
        for (it = begin(), it_temp = temp.begin(); it < last; ++it, ++it_temp) {
            *it_temp = (*f)(*it);
        }
        return temp;
    }
#else
    /**
     * Returns the result of applying the passed function to every element
     * in the matrix. A function of type RWGenMat<T>::mathFunType
     * takes and returns a \c T. A function of type RWGenMat<T>::mathFunType2
     * takes a \c T and returns an
     * RWGenMat<T>::norm_type. For a description of this type, see
     * \link rw_numeric_traits rw_numeric_traits<T> \endlink.
     */
    RWGenMat<norm_type> apply2(mathFunType2 f) const;
#endif


#ifdef RW_NATIVE_EXTENDED
    RWGenMat<T> apply(XmathFuncType f) const;
#endif

    /**
     * Returns an iterator that points to the element in the first row
     * and first column of self. The optional storage specifier determines
     * the order in which the iterator traverses the elements of the
     * matrix; the specifier is independent of the storage format of
     * the matrix. A \c COLUMN_MAJOR iterator proceeds down each column
     * while a \c ROW_MAJOR iterator proceeds along rows.
     *
     * @warning Binary difference and comparison operators between a \c ROW_MAJOR
     * iterator and a \c COLUMN_MAJOR iterator have unpredictable results.
     */
    iterator begin(RWDataView::Storage storage = RWDataView::COLUMN_MAJOR) {
        return iterator((T*)data(), rows(), cols(), 0, 0,
                        rowStride(), colStride(), storage);
    }

    /**
     * @copydoc begin(RWDataView::Storage)
     */
    const_iterator begin(RWDataView::Storage storage = RWDataView::COLUMN_MAJOR) const {
        return const_iterator((T*)data(), rows(), cols(), 0, 0,
                              rowStride(), colStride(), storage);
    }

    /**
     * @copydoc begin(RWDataView::Storage)
     */
    const_iterator cbegin(RWDataView::Storage storage = RWDataView::COLUMN_MAJOR) const {
        return begin(storage);
    }

    /**
     * Returns an iterator that points to one element past the last
     * element in the matrix. The optional storage specifier determines
     * the order in which the iterator traverses the elements of the
     * matrix; the specifier is independent of the storage format of
     * the matrix. A \c COLUMN_MAJOR iterator proceeds down each column,
     * while a \c ROW_MAJOR iterator proceeds along rows.
     *
     * @warning Binary difference and comparison operators between a \c ROW_MAJOR
     * iterator and a \c COLUMN_MAJOR iterator have unpredictable results.
     */
    iterator end(RWDataView::Storage storage = RWDataView::COLUMN_MAJOR) {
        if (storage == RWDataView::ROW_MAJOR ||
                (storage == RWDataView::RWEITHER && colStride() < rowStride())) {
            return iterator((T*)data(), rows(), cols(), rows(), 0,
                            rowStride(), colStride(), storage);
        }
        else {
            return iterator((T*)data(), rows(), cols(), 0, cols(),
                            rowStride(), colStride(), storage);
        }
    }

    /**
     * @copydoc end(RWDataView::Storage)
     */
    const_iterator end(RWDataView::Storage storage = RWDataView::COLUMN_MAJOR) const {
        if (storage == RWDataView::ROW_MAJOR ||
                (storage == RWDataView::RWEITHER && colStride() < rowStride())) {
            return const_iterator((T*)data(), rows(), cols(), rows(), 0,
                                  rowStride(), colStride(), storage);
        }
        else {
            return const_iterator((T*)data(), rows(), cols(), 0, cols(),
                                  rowStride(), colStride(), storage);
        }
    }

    /**
     * @copydoc end(RWDataView::Storage)
     */
    const_iterator cend(RWDataView::Storage storage = RWDataView::COLUMN_MAJOR) const {
        return end(storage);
    }

    /**
     * Returns an iterator that points to the element in the last row
     * and last column of self. The optional storage specifier determines
     * the order in which the iterator traverses the elements of the
     * matrix; the specifier is independent of the storage format of
     * the matrix. A \c COLUMN_MAJOR iterator proceeds down each column
     * while a \c ROW_MAJOR iterator proceeds along rows.
     *
     * @warning Binary difference and comparison operators between a \c ROW_MAJOR
     * iterator and a \c COLUMN_MAJOR iterator have unpredictable results.
     */
    reverse_iterator rbegin(RWDataView::Storage storage = RWDataView::COLUMN_MAJOR) {
        return reverse_iterator(end(storage));
    }

    /**
     * @copydoc begin(RWDataView::Storage)
     */
    const_reverse_iterator rbegin(RWDataView::Storage storage = RWDataView::COLUMN_MAJOR) const {
        return const_reverse_iterator(end(storage));
    }

    /**
     * @copydoc begin(RWDataView::Storage)
     */
    const_reverse_iterator crbegin(RWDataView::Storage storage = RWDataView::COLUMN_MAJOR) const {
        return rbegin(storage);
    }

    /**
     * Returns an iterator that points to one element before the first
     * element in the matrix. The optional storage specifier determines
     * the order in which the iterator traverses the elements of the
     * matrix; the specifier is independent of the storage format of
     * the matrix. A \c COLUMN_MAJOR iterator proceeds down each column,
     * while a \c ROW_MAJOR iterator proceeds along rows.
     *
     * @warning Binary difference and comparison operators between a \c ROW_MAJOR
     * iterator and a \c COLUMN_MAJOR iterator have unpredictable results.
     */
    reverse_iterator rend(RWDataView::Storage storage = RWDataView::COLUMN_MAJOR) {
        return reverse_iterator(begin(storage));
    }

    /**
     * @copydoc end(RWDataView::Storage)
     */
    const_reverse_iterator rend(RWDataView::Storage storage = RWDataView::COLUMN_MAJOR) const {
        return const_reverse_iterator(begin(storage));
    }

    /**
     * @copydoc rend(RWDataView::Storage)
     */
    const_reverse_iterator crend(RWDataView::Storage storage = RWDataView::COLUMN_MAJOR) const {
        return rend(storage);
    }

    T&    bcref(int i, int j);             // ref() with bounds checking: for matrix.h++ compatibility
    void       bcset(int i, int j, const T& x);   // set() with bounds checking: for matrix.h++ compatibility
    T     bcval(int i, int j) const;     // val() with bounds checking: for matrix.h++ compatibility

    /**
     * Returns the number of bytes required to store the matrix to an
     * RWFile using member function
     * \link RWGenMat::saveOn(RWFile&)const saveOn(RWFile&)\endlink.
     */
    size_t   binaryStoreSize() const;

    /**
     * Returns a vector that views a column of the matrix.
     */
    const RWMathVec<T> col(int) const;

    /**
     * @copydoc col(int) const
     */
    RWMathVec<T>  col(int);

    /**
     * Returns the number of columns of the matrix.
     */
    size_t   cols() const {
        return RWMatView::cols();
    }

    /**
     * Returns the stride to move through the data from one column to the next.
     * Could be computed as <tt>&A(i,j+1)-&A(i,j)</tt>.
     */
    int        colStride() const {
        return RWMatView::colStride();
    }

    /**
     * Returns a copy with distinct instance variables. The optional storage
     * indicator determines whether the matrix is stored in \c ROW_MAJOR or
     * \c COLUMN_MAJOR order.
     */
    RWGenMat<T> copy(Storage s = COLUMN_MAJOR) const;

    /**
     * Returns a pointer to the start of a matrix's data. Should be
     * used with care, as this function accesses the matrix's data directly.
     */
    T*    data() {
        return (T*)begin_;
    }

    /**
     * @copydoc data()
     */
    const T* data() const {
        return (T*)begin_;
    }


    // Return a vector of the data in the matrix.  This exists for backwards
    // compatibility with pre version 5.0 math.h++ which allowed you to get a
    // vector view of the data.  Note the changed semantics: this version of
    // dataVec gives you a copy, not a new view.

    // had to put inline so that Borland Builder 4 could find the function
    // dataVec

    RWMathVec<T>  dataVec(Storage storage = COLUMN_MAJOR) const { // Returns vector containing copy of matrix data
        RWMathVec<T> v(rows()*cols(), rwUninitialized);
        int vindex = 0;
        RW_ASSERT(rows() <= RW_INT_MAX);
        RW_ASSERT(cols() <= RW_INT_MAX);
        if (storage == RWGenMat<T>::ROW_MAJOR) {
            for (int i = 0; i < static_cast<int>(rows()); i++) {
                for (int j = 0; j < static_cast<int>(cols()); j++) {
                    v(vindex++) = val(i, j);
                }
            }
        }
        else {
            for (int j = 0; j < static_cast<int>(cols()); j++) {
                for (int i = 0; i < static_cast<int>(rows()); i++) {
                    v(vindex++) = val(i, j);
                }
            }
        }
        RWPOSTCONDITION(vindex == v.length());
        return v;
    }

    /**
     * Alias for copy().
     */
    RWGenMat<T> deepCopy(Storage s = COLUMN_MAJOR) const;

    /**
     * Invoking deepenShallowCopy() for a matrix guarantees that there
     * is only one reference to that object and that its data are in
     * contiguous storage. The optional storage indicator determines
     * whether the matrix is stored in \c ROW_MAJOR or \c COLUMN_MAJOR
     * order.
     */
    void       deepenShallowCopy(Storage s = COLUMN_MAJOR);

    /**
     * Returns the diagonal elements of the matrix as a vector, with
     * optional bounds checking. The default <tt>\a idiag = 0</tt> implies
     * the main diagonal; <tt>\a idiag = n</tt>  implies \c n diagonal
     * slices up from center; <tt>\a idiag = -n</tt>  implies \c n diagonal
     * slices down.
     */
    const RWMathVec<T> diagonal(int idiag = 0) const;   // Returns a view of a diagonal

    /**
     * @copydoc diagonal(int) const
     */
    RWMathVec<T>  diagonal(int idiag = 0);              // Returns a view of a diagonal

    /**
     * Returns a matrix pick. The results can be used as an lvalue.
     * You can think of the "picked" submatrix as specifying an intersection
     * of the rows listed in \a v1 and the columns listed in \a v2.
     * Before using this function, you must include the header file
     * \c rw\\math\\matpick.h.
     */
    RWGenMatPick<T> pick(const RWIntVec& v1, const RWIntVec& v2);

    /**
     * @copydoc pick(const RWIntVec&, const RWIntVec&)
     */
    const RWGenMatPick<T> pick(const RWIntVec& v1, const RWIntVec& v2) const;

    void       printOn(std::ostream& s) const;     // Pretty print

    T&    ref(int i, int j);               // for matrix.h++ compatibility

    /**
     * Makes self a view of the data contained in \a m. The view currently
     * associated with the matrix is lost.
     */
    RWGenMat<T>& reference(const RWGenMat<T>& m);

    /**
     * Changes the size of the matrix to \a m rows and \a n columns,
     * adding 0s or truncating as necessary. The optional storage indicator
     * determines whether the matrix is stored in \c ROW_MAJOR or \c
     * COLUMN_MAJOR order.
     */
    void       resize(size_t m, size_t n, Storage s = COLUMN_MAJOR);

    /**
     * Changes the size of the matrix to \a m rows and \a n columns.
     * After reshaping, the contents of the matrix are undefined; that
     * is, they can be garbage, and probably will be. The optional storage
     * indicator determines whether the matrix is stored in \c ROW_MAJOR
     * or \c COLUMN_MAJOR order.
     */
    void       reshape(size_t m, size_t n, Storage s = COLUMN_MAJOR);

    /**
     * Restores self from an RWFile. The optional
     * storage indicator determines whether the matrix is stored in
     * \c ROW_MAJOR or \c COLUMN_MAJOR order. To use these functions
     * with a user-defined type T, the corresponding operator \c >>
     * must be defined:
     *
     * @code
     * RWFile&      operator>>(RWFile&, T&);
     * @endcode
     */
    void       restoreFrom(RWFile&, Storage s = COLUMN_MAJOR);

    /**
     * Restores self from a virtual stream. The optional
     * storage indicator determines whether the matrix is stored in
     * \c ROW_MAJOR or \c COLUMN_MAJOR order. To use these functions
     * with a user-defined type T, the corresponding operator \c >>
     * must be defined:
     *
     * @code
     * RWvistream&  operator>>(RWvistream&, T&);
     * @endcode
     */
    void       restoreFrom(RWvistream&, Storage s = COLUMN_MAJOR);

    /**
     * Returns a vector that views a row of the matrix.
     */
    const RWMathVec<T> row(int) const;

    /**
     * @copydoc row(int) const
     */
    RWMathVec<T>  row(int);

    /**
     * Return the number of rows of the matrix.
     */
    size_t   rows() const {
        return RWMatView::rows();
    }

    /**
     * Returns the stride required to move through the data from one row to the
     * next. Could be computed as <tt>&A(i+1,j)-&A(i,j)</tt>.
     */
    int        rowStride() const {
        return RWMatView::rowStride();
    }

    /**
     * Stores self in a binary format to an
     * RWFile. If T is a user-defined type, the shift \c operator<<
     * must be defined:
     *
     * @code
     * RWFile&      operator<<(RWFile&, const T&);
     * @endcode
     */
    void       saveOn(RWFile&) const;

    /**
     * Stores self to a virtual stream.
     * If T is a user-defined type, the shift \c operator<<
     * must be defined:
     *
     * @code
     * RWvostream&  operator<<(RWvostream&, const T&);
     * @endcode
     */
    void       saveOn(RWvostream&) const;

    void       scanFrom(std::istream& s, Storage storage = COLUMN_MAJOR); // Read to eof or delimit with []

    void       set(int i, int j, const T& x);     // for matrix.h++ compatibility

    /**
     * Returns a vector that views a slice of the matrix. The slice
     * begins at element \a i,j and extends for \a n elements. The increment
     * between successive elements in the vector is \a rowstride rows
     * and \a colstride columns. For example:
     *
     * @code
     * A.slice(n-1,0, n, -1,1)
     * @endcode
     *
     * is a view of the diagonal from the bottom left to top right corners
     * of the \a n x \a n matrix \c A.
     */
    RWMathVec<T> slice(int i, int j, size_t n, int rowstride, int colstride) const {
        sliceCheck(i, j, n, rowstride, colstride);
        return fastSlice(i, j, n, rowstride, colstride);
    }

    /**
     * Returns a matrix that views a slice of the matrix. The slice
     * begins at element \a i, \a j and contains \a m rows and \a n columns.
     * The increment between successive elements in the slice's row
     * is \a rowstr1 rows and \a colstr1 columns. The increment between
     * successive elements in the slice's column is \a rowstr2 rows
     * and \a colstr2 columns. For example:
     *
     * @code
     * A.slice(n-1,0, n,n, -1,0, 0,1)
     * @endcode
     *
     * returns a view of the \a n x \a n matrix \c A upside down. A
     * more readable way to accomplish this is:
     *
     * @code
     * A(RWRange(n-1,0), RWRange(0,n-1))
     * @endcode
     */
    RWGenMat<T> slice(int i, int j, size_t m, size_t n, int rowstr1, int colstr1, int rowstr2, int colstr2) const {
        sliceCheck(i, j, m, n, rowstr1, colstr1, rowstr2, colstr2);
        return fastSlice(i, j, m, n, rowstr1, colstr1, rowstr2, colstr2);
    }

    T     val(int i, int j) const;     // for matrix.h++ compatibility
    void       zero() {
        (*this) = T(0);   // for matrix.h++ compatibility
    }

    /**
     * Subscripting operator for the matrix, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file. All subscripting
     * operators return a new view of the same data as the matrix being
     * subscripted. An object of type RWRange or RWToEnd, the global
     * object RWAll, or a character string may be substituted for an
     * RWSlice.
     */
    T&              operator()(int i, int j);

    /**
     * Subscripting operator for the matrix, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file. All subscripting
     * operators return a new view of the same data as the matrix being
     * subscripted. An object of type RWRange or RWToEnd, the global
     * object RWAll, or a character string may be substituted for an
     * RWSlice.
     */
    T               operator()(int i, int j) const;

    /**
     * Subscripting operator for the matrix, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file. All subscripting
     * operators return a new view of the same data as the matrix being
     * subscripted. An object of type RWRange or RWToEnd, the global
     * object RWAll, or a character string may be substituted for an
     * RWSlice.
     */
    RWMathVec<T>            operator()(int i, const RWSlice& j);

    /**
     * Subscripting operator for the matrix, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file. All subscripting
     * operators return a new view of the same data as the matrix being
     * subscripted. An object of type RWRange or RWToEnd, the global
     * object RWAll, or a character string may be substituted for an
     * RWSlice.
     */
    const RWMathVec<T>      operator()(int i, const RWSlice& j) const;

    /**
     * Subscripting operator for the matrix, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file. All subscripting
     * operators return a new view of the same data as the matrix being
     * subscripted. An object of type RWRange or RWToEnd, the global
     * object RWAll, or a character string may be substituted for an
     * RWSlice.
     */
    RWMathVec<T>            operator()(const RWSlice& i, int j);

    /**
     * Subscripting operator for the matrix, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file. All subscripting
     * operators return a new view of the same data as the matrix being
     * subscripted. An object of type RWRange or RWToEnd, the global
     * object RWAll, or a character string may be substituted for an
     * RWSlice.
     */
    const RWMathVec<T>      operator()(const RWSlice& i, int j) const;

    /**
     * Subscripting operator for the matrix, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file. All subscripting
     * operators return a new view of the same data as the matrix being
     * subscripted. An object of type RWRange or RWToEnd, the global
     * object RWAll, or a character string may be substituted for an
     * RWSlice.
     */
    RWGenMat<T>         operator()(const RWSlice& i, const RWSlice& j);

    /**
     * Subscripting operator for the matrix, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file. All subscripting
     * operators return a new view of the same data as the matrix being
     * subscripted. An object of type RWRange or RWToEnd, the global
     * object RWAll, or a character string may be substituted for an
     * RWSlice.
     */
    const RWGenMat<T>   operator()(const RWSlice& i, const RWSlice& j) const;


    /**
     * Assignment operator with conventional meaning. The expression
     * \f$ u = v; \f$ implies \f$ u_{ij} = v_{ij} \f$.
     * The RWGenMat objects must \e conform, that is, have the same dimensions.
     */
    RWGenMat<T>& operator=(const RWGenMat<T>& v); // Must be same size as v

    /**
     * Assignment operator with conventional meaning. The expression
     * \f$ u = v; \f$ implies \f$ u_{ij} = v_{ij} \f$.
     * The RWGenMat objects must \e conform, that is, have the same dimensions.
     */
    RWGenMat<T>& operator=(const RWGenMatPick<T>& v);

    /**
     * Assignment operator with conventional meaning.  The expression
     *  \f$ u = s; \f$ implies \f$ u_{ij} = s \f$.
     */
    RWGenMat<T>& operator=(const T& s);

    /**
     * Returns \c true if self and the argument are equivalent (or not
     * equivalent). That is, they must have the same number of rows
     * as well as columns, and each element in self must equal the corresponding
     * element in the argument.
     */
    bool   operator==(const RWGenMat<T>&) const;

    /**
     * Returns \c true if self and the argument are equivalent (or not
     * equivalent). That is, they must have the same number of rows
     * as well as columns, and each element in self must equal the corresponding
     * element in the argument.
     */
    bool   operator!=(const RWGenMat<T>& v) const;

    /**
     * Assignment by addition operator with conventional meaning. The expression
     * \f$ u += s; \f$ implies \f$ u_{ij} = u_{ij}+ s \f$.
     */
    RWGenMat<T>& operator+=(const T& s);

    /**
     * Assignment by subtraction operator with conventional meaning.  The expression
     * \f$ u -= s; \f$ implies \f$ u_{ij} = u_{ij} - s \f$.
     */
    RWGenMat<T>& operator-=(const T& s);

    /**
     * Assignment by multiplication operator with conventional meaning.  The
     * expression \f$ u *= s; \f$ implies \f$ u_{ij} = u_{ij} * s \f$.

     */
    RWGenMat<T>& operator*=(const T& s);

    /**
     * Assignment by division operator with conventional meaning.  The expression
     * \f$ u /= s; \f$ implies \f$ u_{ij} = u_{ij} / s \f$.
     */
    RWGenMat<T>& operator/=(const T& s);

    /**
     * Assignment by addition operator with conventional meaning. The expression
     * \f$ u += v; \f$ implies \f$ u_{ij} = u_{ij} + v_{ij} \f$.
     * The RWGenMat objects must \e conform, that is, have the same dimensions.
     */
    RWGenMat<T>& operator+=(const RWGenMat<T>& v);

    /**
     * Assignment by subtraction operator with conventional meaning. The expression
     * \f$ u -= v; \f$ implies \f$ u_{ij} = u_{ij} - v_{ij} \f$.
     * The RWGenMat objects must \e conform, that is, have the same dimensions.
     */
    RWGenMat<T>& operator-=(const RWGenMat<T>& v);

    /**
     * Assignment by multiplication operator with conventional meaning. The
     * expression \f$ u *= v; \f$ implies \f$ u_{ij} = u_{ij} * v_{ij} \f$.
     * The RWGenMat objects must \e conform, that is, have the same dimensions.
     */
    RWGenMat<T>& operator*=(const RWGenMat<T>& v);

    /**
     * Assignment by division operator with conventional meaning. The expression
     * \f$ u /= v; \f$ implies \f$ u_{ij} = u_{ij} / v_{ij} \f$.
     * The RWGenMat objects must \e conform, that is, have the same dimensions.
     */
    RWGenMat<T>& operator/=(const RWGenMat<T>& v);

    /**
     * Increments each element of self.
     */
    RWGenMat<T>& operator++();

    /**
     * Decrements each element of self.
     */
    RWGenMat<T>& operator--();
    /**
     * Increments each element of self. This overload is invoked if the operator
     * is used as a postfix operator.
     */
    void        operator++(int) {
        ++(*this);
    }

    /**
     * Decrements each element of self. This overload is invoked if the operator
     * is used as a postfix operator.
     */
    void        operator--(int) {
        --(*this);
    }

#if defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 1100
    // Suppress Intel warning:
    // warning #597 regarding operator() for T=DComplex will not be called
    // for implicit or explicit conversions
#  pragma warning push
#  pragma warning (disable:597)
#endif

#if !defined(RW_NO_CONVERSION_TO_SELF)
    RW_SUPPRESS_CONVERSION_OP_NEVER_CALLED_WARNING
    /**
     * Implicit conversion operator to
     * \link rw_numeric_traits::promote_type rw_numeric_traits<T>::promote_type\endlink.
     */
    operator RWGenMat<promote_type>() {
        RWGenMat<promote_type> tmp(rows(), cols(), rwUninitialized);
        const_iterator b = begin();
        const_iterator e = end();
        typename RWGenMat<promote_type>::iterator tmpiter = tmp.begin();

        while (b != e) {
            *tmpiter++ = (promote_type)(*b++);
        }
        return tmp;
    }
    RW_RESTORE_CONVERSION_OP_NEVER_CALLED_WARNING
#endif

#if defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 1100
#  pragma warning pop
#endif
};

/**
 * @relates RWGenMat
 *
 * Unary minus operator, which is applied element-by-element.
 * For instance, for matrices \a v, and \c w,
 * the expression \f$ w=-v \f$ implies \f$ w_{ij} =
 * - v_{ij} \f$.
 */
template <class T>
RWGenMat<T> operator-(const RWGenMat<T>& v);

/**
 * @relates RWGenMat
 *
 * Unary plus operator, which is applied element-by-element.
 * For instance, for matrices \a v, and \c w,
 * the expression \f$ w=+v \f$ implies \f$ w_{ij} =
 * + v_{ij} \f$.
 */
template <class T>
inline RWGenMat<T> operator+(const RWGenMat<T>& v)
{
    return v;
}

/**
 * @relates RWGenMat
 *
 * Addition operator, which is applied element-by-element.
 * For instance, for matrices \c u, \a v, and \c w,
 * the expression \f$ w=u+v \f$ implies \f$ w_{ij} = u_{ij}
 * + v_{ij} \f$. The two RWGenMat objects
 * must \e conform, that is, have the same
 * numbers of rows and columns, or an exception with value \c RWMATH_MNMATCH
 * occurs.
 */
template <class T>
RWGenMat<T> operator+(const RWGenMat<T>& u, const RWGenMat<T>& v);

/**
 * @relates RWGenMat
 *
 * Subtraction operator, which is applied element-by-element.
 * For instance, for matrices \c u, \a v, and \c w,
 * the expression \f$ w=u-v \f$ implies
 * \f$ w_{ij} = u_{ij} - v_{ij} \f$. The two
 * RWGenMat objects must \e conform, that is, have the same
 * numbers of rows and columns, or an exception with value \c RWMATH_MNMATCH
 * occurs.
 */
template <class T>
RWGenMat<T> operator-(const RWGenMat<T>& u, const RWGenMat<T>& v);

/**
 * @relates RWGenMat
 *
 * Multiplication operator, which is applied element-by-element.
 * For instance, for matrices \c u, \a v, and \c w,
 * the expression \f$ w=u*v \f$ implies
 * \f$ w_{ij} = u_{ij} * v_{ij} \f$. The two
 * RWGenMat objects must \e conform, that is, have the same
 * numbers of rows and columns, or an exception with value \c RWMATH_MNMATCH
 * occurs.
 */
template <class T>
RWGenMat<T> operator*(const RWGenMat<T>& u, const RWGenMat<T>& v);

/**
 * @relates RWGenMat
 *
 * Takes two matrices, \a A and \a B, and returns a matrix \c C such that:
 *
 * \f[
 * C_{ij} = A_{ij}B_{ij}
 * \f]
 */
template <class T>
RWGenMat<T> elementProduct(const RWGenMat<T>& A, const RWGenMat<T>& B)
{
    return operator*(A, B);
}

/**
 * @relates RWGenMat
 *
 * Division operator, which is applied element-by-element.
 * For instance, for matrices \c u, \a v, and \c w,
 * the expression \f$ w=u/v \f$ implies \f$ w_{ij} = u_{ij}
 * / v_{ij} \f$. The two RWGenMat objects
 * must \e conform, that is, have the same
 * numbers of rows and columns, or an exception with value \c RWMATH_MNMATCH
 * occurs.
 */
template <class T>
RWGenMat<T> operator/(const RWGenMat<T>& u, const RWGenMat<T>& v);

// The % operator used for inner products.  These definitions are
// postponed until after product() has been declared
//  inline RWGenMat<T> operator%(const RWGenMat<T>& A, const RWGenMat<T>& B) {return product(A,B);}
//  inline RWMathVec<T>   operator%(const RWGenMat<T>& A, const RWMathVec<T>& x) {return product(A,x);}
//  inline RWMathVec<T>   operator%(const RWMathVec<T>& x, const RWGenMat<T>& A) {return product(x,A);}

/**
 * @relates RWGenMat
 *
 * Addition operator, which is applied element-by-element.
 * For instance, for matrices \a u, \c w, and scaler \a s,
 * the expression \f$ w=u+s \f$ implies
 * \f$ w_{ij} = u_{ij} + s \f$.
 */
template <class T>
RWGenMat<T> operator+(const RWGenMat<T>& u, const T& s);

/**
 * @relates RWGenMat
 *
 * Subtraction operator, which is applied element-by-element.
 * For instance, for matrices \a v, \c w, and scaler \a s,
 * the expression \f$ w=s-v \f$ implies
 * \f$ w_{ij} =  s - v_{ij} \f$.
 */
template <class T>
RWGenMat<T> operator-(const T& s, const RWGenMat<T>& v);

/**
 * @relates RWGenMat
 *
 * Multiplication operator, which is applied element-by-element.
 * For instance, for matrices \a u, \c w, and scaler \a s,
 * the expression \f$ w=u*s \f$ implies
 * \f$ w_{ij} = u_{ij} * s \f$.
 */
template <class T>
RWGenMat<T> operator*(const RWGenMat<T>& u, const T& s);

/**
 * @relates RWGenMat
 *
 * Division operator, which is applied element-by-element.
 * For instance, for matrices \a v, \c w, and scaler \a s,
 * the expression \f$ w=s/v \f$ implies
 * \f$ w_{ij} = s / v_{ij} \f$.
 */
template <class T>
RWGenMat<T> operator/(const T& s, const RWGenMat<T>& v);

/**
 * @relates RWGenMat
 *
 * Outputs a matrix \a m to \b std::ostream \a s. First, the number of
 * rows and columns is output, then the values, separated by spaces,
 * are output row by row, beginning with a left bracket \c [ and
 * terminating with a right bracket \c ].
 */
template <class T>
std::ostream&    operator<<(std::ostream& s, const RWGenMat<T>& m);

/**
 * @relates RWGenMat
 *
 * Reads a matrix \a v from istream \a s. First, the number of rows
 * and columns is read, then the matrix values, separated by white
 * space, row-by-row. If the sequence of numbers begins with a left
 * bracket \c [ , the operator reads to a matching right bracket
 * \c ]. If no bracket is present, it reads to end of file. The
 * matrix \a v is stored in \c COLUMN_MAJOR order.
 */
template <class T>
std::istream&    operator>>(std::istream& s, RWGenMat<T>& v);

/**
 * @relates RWGenMat
 *
 * Multiplication operator, which is applied element-by-element.
 * For instance, for matrices \a v, \c w, and scaler \a s,
 * the expression \f$ w=s*v \f$ implies
 * \f$ w_{ij} = s * v_{ij} \f$.
 */
template <class T>
RWGenMat<T> operator*(const T& s, const RWGenMat<T>& v);

/**
 * @relates RWGenMat
 *
 * Addition operator, which is applied element-by-element.
 * For instance, for matrices \a v, \c w, and scaler \a s,
 * the expression \f$ w=s+v \f$ implies
 * \f$ w_{ij} = s + v_{ij} \f$.
 */
template <class T>
RWGenMat<T> operator+(const T& s, const RWGenMat<T>& v);

/**
 * @relates RWGenMat
 *
 * Subtraction operator, which is applied element-by-element.
 * For instance, for matrices \a u, \c w, and scaler \a s,
 * the expression \f$ w=u-s \f$ implies
 * \f$ w_{ij} = u_{ij} - s \f$.
 */
template <class T>
RWGenMat<T> operator-(const RWGenMat<T>& u, const T& s);

/**
 * @relates RWGenMat
 *
 * Division operator, which is applied element-by-element.
 * For instance, for matrices \a u, \c w, and scaler \a s,
 * the expression \f$ w=u/s \f$ implies
 * \f$ w_{ij} = u_{ij} / s \f$.
 */
template <class T>
RWGenMat<T> operator/(const RWGenMat<T>& u, const T& s);

/********************
 * Global Functions *
 ********************/

/**** Miscellaneous Functions ****/

/**
 * @relates RWGenMat
 *
 * @copydoc abs(const RWGenMat<double>&)
 *
 * See the example in abs(const RWGenMat<double>&)
 *
 * @note The absolute value of a complex number is of type \c double.
 * Therefore, if abs() is invoked for class \link RWGenMat RWGenMat<DComplex>
 * \endlink, a vector of class \link RWGenMat RWGenMat<double>\endlink is
 * returned.
 */
RWGenMat<double> RW_MATH_SYMBOLIC abs(const RWGenMat<DComplex>& M);

/**
 * @relates RWGenMat
 *
 * Returns the absolute values of each element.
 *
 * @section example Example
 *
 * @code
 * #include <rw/math/genmat.h>
 * #include <iostream>
 * const double adata[] = {1.2,2.4,-1.2,0.8,-4.5};
 * int main() {
 *         RWGenMat<double> a(adata,5, 1, 1);
 *         RWGenMat<double> b = abs(a);
 *         std::cout << b;
 *         return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 * 5x1 [
 * 1.2
 * 2.4
 * 1.2
 * 0.8
 * 4.5
 * ]
 * @endcode
 */
RWGenMat<double> RW_MATH_SYMBOLIC abs(const RWGenMat<double>& M);

/**
 * @relates RWGenMat
 *
 * @copydoc abs(const RWGenMat<DComplex>&)
 *
 * See the example in abs(const RWGenMat<double>&)
 */
RWGenMat<float> RW_MATH_SYMBOLIC abs(const RWGenMat<float>& M);

/**
 * @relates RWGenMat
 *
 * @copydoc abs(const RWGenMat<DComplex>&)
 *
 * See the example in abs(const RWGenMat<double>&)
 */
RWGenMat<int> RW_MATH_SYMBOLIC abs(const RWGenMat<int>& M);

/**
 * @relates RWGenMat
 *
 * @copydoc abs(const RWGenMat<DComplex>&)
 *
 * See the example in abs(const RWGenMat<double>&)
 */
RWGenMat<SChar> RW_MATH_SYMBOLIC abs(const RWGenMat<SChar>& M);

// Return sum_ij (Aij*Bij)
template <class T>
T      dot(const RWGenMat<T>&, const RWGenMat<T>&);

/**
 * @relates RWGenMat
 *
 * Return the \c index of the maximum element of the vector.
 * If instead you want the maximum \c value use function maxValue().
 */
template <class T>
void   maxIndex(const RWGenMat<T>&, int* i, int* j);

/**
 * @relates RWGenMat
 *
 * Return the \c index of the minimum element of the vector.
 * If instead you want the minimum \c value use function minValue().
 */
template <class T>
void   minIndex(const RWGenMat<T>&, int* i, int* j);

/**
 * @relates RWGenMat
 *
 * Return the maximum value.
 */
template <class T>
T      maxValue(const RWGenMat<T>&);

/**
 * @relates RWGenMat
 *
 * Return the minimum value.
 */
template <class T>
T      minValue(const RWGenMat<T>&);

template <class T>
T      prod(const RWGenMat<T>&);

/**
 * @relates RWGenMat
 *
 * Returns the inner product of its two arguments.
 * Takes a matrix \a A and a vector \a v as arguments returns:
 *
 * \f[
 * y_i = \sum_{j=0}^{n-1} A_{ij}v_j
 * \f]
 *
 * The function checks to make sure that the number of columns in
 * \a A equals the number of elements in \a v. Otherwise,
 * an exception with value \c MATX_MATVECPROD occurs.
 */
template <class T>
RWMathVec<T>   product(const RWGenMat<T>& A, const RWMathVec<T>& v);

template <class T>
void   product(const RWGenMat<T>&, const RWMathVec<T>&, RWMathVec<T>&);

template <class T>
RWMathVec<T>   product(const RWMathVec<T>&, const RWGenMat<T>&);

template <class T>
void   product(const RWMathVec<T>&, const RWGenMat<T>&, RWMathVec<T>&);

/**
 * @relates RWGenMat
 *
 * Returns the inner product of its two arguments.
 * Takes two matrices \a A and \a B returns:
 *
 * \f[
 * C_{ij} = \sum_{k=0}^{P-1} A_{ik}B_{kj}
 * \f]
 *
 * The function checks to make sure that the number of columns in
 * \a A equals the number of rows in \a B. Otherwise, an
 * exception with value \c MATX_MATMATPROD occurs.
 */
template <class T>
RWGenMat<T> product(const RWGenMat<T>& A, const RWGenMat<T>& B);

template <class T>
void product(const RWGenMat<T>&, const RWGenMat<T>&, RWGenMat<T>&);

template <class T>
T      sum(const RWGenMat<T>&);

/**
 * @relates RWGenMat
 *
 * Takes a matrix \a x as an argument and returns the transpose
 * \f$ y_{ik} = x_{ki} \f$. The matrix returned is
 * a new view of the same data as the argument matrix. Use RWGenMat::copy()
 * or RWGenMat::deepenShallowCopy() to make the views distinct.
 */
template <class T>
RWGenMat<T> transpose(const RWGenMat<T>& x);


/**** Functions for both real and complex ****/

/**
 * @relates RWGenMat
 *
 * Takes \a x as an argument and returns \c y such that
 * \f$ y_{i} = cos(x_{i}) \f$. The
 * <i>x</i><SUB>i</SUB> are in radians. For complex arguments,
 * the complex cosine is returned.
 */
template <class T>
RWGenMat<T> cos(const RWGenMat<T>& x);

/**
 * @relates RWGenMat
 *
 * Takes \a x as an argument and returns hyperbolic cosines
 * \c y such that \f$ y_{i} = cosh(x_{i}) \f$.
 * For complex arguments, the complex hyperbolic cosine is
 * returned.
 */
template <class T>
RWGenMat<T> cosh(const RWGenMat<T>& x);

/**
 * @relates RWGenMat
 *
 * Takes an argument \a x and returns \c y such that:
 *
 * \f[
 * y_i = e^{x_i}
 * \f]
 *
 * If class \c T is complex, the complex exponential is returned.
 */
template <class T>
RWGenMat<T> exp(const RWGenMat<T>& x);

/**
 * @relates RWGenMat
 *
 * Takes an RWGenMat as an argument and returns \<\a x\>, the mean value,
 * where:
 *
 * \f[
 * \langle x \rangle = \frac{1}{n}\sum_{i=0}^{n-1}x_i
 * \f]
 *
 * For example, if <tt>x = [1, 4, 3, 4]</tt>, then \<\a x\> <tt>= 3</tt>.
 */
template <class T>
T      mean(const RWGenMat<T>& x);

/**
 * @relates RWGenMat
 *
 * The function pow() takes two arguments: pow( \a x, \a y ).
 *
 * Returns \c z such that:
 *
 * \f$ z_i = (x_i)^{y_i} \f$
 *
 * If the number of elements in \a x does not match the number
 * of elements in \a y,  an exception with value \c RWMATH_MNMATCH
 * will occur.
 */
template <class T>
RWGenMat<T> pow(const RWGenMat<T>& x, const RWGenMat<T>& y);

/**
 * @relates RWGenMat
 *
 * The function pow() takes two arguments: pow( \a x, \a y ).
 *
 * Returns \c z such that:
 *
 * \f$ z_i = (x_i)^{y} \f$
 */
template <class T>
RWGenMat<T> pow(const RWGenMat<T>& x, T y);

// internal helper overload for complex
template <class T>
RWGenMat<std::complex<T> > pow(const RWGenMat<std::complex<T> >& x, std::complex<T> y);

/**
 * @relates RWGenMat
 *
 * The function pow() takes two arguments: pow( \a x, \a y ).
 *
 * Returns \c z such that:
 *
 * \f$ z_i = (x)^{y_i} \f$
 */
template <class T>
RWGenMat<T> pow(T x, const RWGenMat<T>& y);

// internal helper overload for complex
template <class T>
RWGenMat<std::complex<T> > pow(std::complex<T> x, const RWGenMat<std::complex<T> >& y);


/**
 * @relates RWGenMat
 *
 * The function sin() takes \a x as
 * an argument and returns \c y such that \f$ y_{i}
 * = sin(x_{i}) \f$. The <i>x</i><SUB>i</SUB> are in radians.
 * For complex classes, the complex sine is returned.
 */
template <class T>
RWGenMat<T> sin(const RWGenMat<T>& x);

/**
 * @relates RWGenMat
 *
 * The function sinh() takes
 * \a x as an argument and returns \c y such that \f$ y_{i}
 * = sinh(x_{i}) \f$. For complex classes, the complex hyperbolic
 * sine is returned.
 */
template <class T>
RWGenMat<T> sinh(const RWGenMat<T>& x);

/**
 * @relates RWGenMat
 *
 * The square root function. Takes \a x as an argument and returns
 * \c y such that
 * \f$ y_{i} = (x_{i})^{1/2} \f$.
 * For complex classes, the principal value of the complex square
 * root is returned.
 */
template <class T>
RWGenMat<T> sqrt(const RWGenMat<T>& x);

/**
 * @relates RWGenMat
 *
 * Takes a matrix \a x as an argument and returns its variance \c y as in:
 *
 * \f[
 * y=\frac{1}{N}\sum_{i=0}^{N-1}(x_i-<x>)^2
 * \f]
 *
 * where <\a x> is the mean of the matrix and \c N is the number of rows times
 * columns. Note that this is the biased variance.
 */
double RW_MATH_SYMBOLIC variance(const RWGenMat<DComplex>& x);

/**
 * @relates RWGenMat
 * @copydoc variance
 */
double RW_MATH_SYMBOLIC variance(const RWGenMat<double>& x);

/**
 * @relates RWGenMat
 * @copydoc variance
 */
float  RW_MATH_SYMBOLIC variance(const RWGenMat<float>& x);

/**** Functions for real matrices ****/

/**
 * @relates RWGenMat
 *
 * Returns arc cosines \c y such that \f$ y_{i} =
 * cos^{-1} (x_{i}) \f$. The \c y<SUB>i</SUB> (in
 * radians) are in the range \f$ 0 < y_{i} \leq \pi \f$,
 * for elements <i>x</i><SUB>i</SUB>
 * with absolute values \f$ |x_{i}| \leq 1 \f$.
 */
template <class T>
RWGenMat<T> acos(const RWGenMat<T>& x);

/**
 * @relates RWGenMat
 *
 * Takes \a x as an argument and returns arc sines \c y such
 * that \f$ y_{i} = sin^{-1}(x_{i}) \f$.
 * The \c y<SUB>i</SUB> (in radians) are in the range
 * \f$ -\pi/2 < y_{i} \leq \pi/2 \f$, for elements
 * <i>x</i><SUB>i</SUB> with absolute values \f$ |x_{i}|
 * \leq 1 \f$.
 */
template <class T>
RWGenMat<T> asin(const RWGenMat<T>& x);

/**
 * @relates RWGenMat
 *
 * Takes \a x and returns \c y of arc
 * tangents (in radians), such that \f$ y_{i} =
 * tan^{-1}x_{i} \f$,
 * where \f$ -\pi/2 < y_{i} \leq \pi/2 \f$.
 */
template <class T>
RWGenMat<T> atan(const RWGenMat<T>& x);

/**
 * @relates RWGenMat
 *
 * Takes two arguments and returns quadrant
 * correct arc tangents \c z (in radians), such that \f$ z_{i}
 * = tan^{-1} ( x_{i}/y_{i}) \f$. For each
 * element \c i, the expression \f$ atan2(x_{i}, y_{i}) \f$ is
 * mathematically equivalent to:
 *
 * \f[
 * tan^{-1}\left(\frac{x_i}{y_i}\right)
 * \f]
 *
 * At least one of the arguments \a x<SUB>i</SUB> or \a y<SUB>i</SUB>
 * must be nonzero.
 */
template <class T>
RWGenMat<T> atan2(const RWGenMat<T>& x, const RWGenMat<T>& y);

/**
 * @relates RWGenMat
 *
 * Takes \a x as an argument and returns \c y such that
 * \c y<SUB>i</SUB> corresponds to the next integer greater
 * than or equal to <i>x</i><SUB>i</SUB>. For example,  if
 * <tt>x = [ -1.3, 4.3, 7.9]</tt>then <tt>y = [ -1, 5, 8].</tt>
 */
template <class T>
RWGenMat<T> ceil(const RWGenMat<T>& x);

/**
 * @relates RWGenMat
 *
 * Takes \a x as an argument and returns \c y such that
 * \c y<SUB>i</SUB> corresponds to the largest integer less
 * than or equal to <i>x</i><SUB>i</SUB>. For example, if <tt>x =
 * [ 1.3, 4.4, -3.2]</tt>, then <tt>y = [ 1, 4, -4].</tt>
 */
template <class T>
RWGenMat<T> floor(const RWGenMat<T>& x);

/**
 * @relates RWGenMat
 *
 * Takes \a x as an argument and returns natural logarithms
 * \c y such that \f$ y_{i}=log(x_{i}) \f$.
 * The arguments <i>x</i><SUB>i</SUB> must not be 0. For complex
 * arguments, the principal value of the complex natural logarithm
 * is returned.
 */
template <class T>
RWGenMat<T> log(const RWGenMat<T>& x);

/**
 * @relates RWGenMat
 *
 * Takes \a x as an argument and returns base 10 logarithms
 * \c y such that \f$ y_{i}=log_{10}(x_{i}) \f$.
 * The arguments <i>x</i><SUB>i</SUB> must not be 0.
 */
template <class T>
RWGenMat<T> log10(const RWGenMat<T>& x);

/**
 * @relates RWGenMat
 *
 * The function tan() takes argument \a x and returns \c y such that
 * \f$ y_{i} = tan(x_{i}) \f$.
 */
template <class T>
RWGenMat<T> tan(const RWGenMat<T>& x);

/**
 * @relates RWGenMat
 *
 * The function tanh() takes argument \a x and returns \c y such that
 * \f$ y_{i} = tanh(x_{i}) \f$.
 *
 */
template <class T>
RWGenMat<T> tanh(const RWGenMat<T>& x);


/**** Functions for complex matrices ****/

/**
 * @relates RWGenMat
 *
 * Returns the arguments \c a<SUB>i</SUB> (in radians) of \a x
 * in the range  \f$ -\pi < a_{i} \leq \pi \f$,
 * where \f$ a_{i} =
 * tan^{-1}{[Im(x_{i})/Re(x_{i})]} \f$.
 * Multiple valued functions of complex arguments, such as sqrt()
 * and log(), are therefore constrained to their principle value.
 */
RWGenMat<double> RW_MATH_SYMBOLIC arg(const RWGenMat<DComplex>& x);

/**
 * @relates RWGenMat
 *
 * Takes complex \a x as an argument and returns the complex
 * conjugates \f$ x^{*} \f$. For example, if \f$ x_{i} =
 * (2.3, 1.4) \f$, then \f$ x_{i}^{*} = (2.3, -1.4) \f$.
 */
RWGenMat<DComplex> RW_MATH_SYMBOLIC conj(const RWGenMat<DComplex>& x);
inline const RWGenMat<double> RW_MATH_SYMBOLIC conj(const RWGenMat<double>& V)
{
    return V;
}
inline const RWGenMat<float> RW_MATH_SYMBOLIC conj(const RWGenMat<float>& V)
{
    return V;
}

/**
 * @relates RWGenMat
 *
 * Takes complex \a x as an argument and returns \c y containing
 * the imaginary parts \f$ y_{i} = Im(x_{i}) \f$.
 * With most versions of complex, the results can be used as an
 * l-value:
 *
 * @code
 * RWGenMat<DComplex>  v(5,  0);         //  (0,  0),  (0,  0),  ...
 * imag(v)  =  1.0;                      //  (0,  1),  (0,  1),  ...
 * @endcode
 */
RWGenMat<double> RW_MATH_SYMBOLIC imag(const RWGenMat<DComplex>& x);

/**
 * @relates RWGenMat
 *
 * Takes a complex argument \a x and returns real \c y containing
 * the real part \f$ y_{i}=Re(x_{i}) \f$. With most
 * versions of complex, the results can be used as an l-value:
 *
 * @code
 * RWGenMat<DComplex>  v(5,  0);         //  (0,  0),  (0,  0),  ...
 * real(v)  =  1.0;                      //  (1,  0),  (1,  0),  ...
 * @endcode
 */
RWGenMat<double> RW_MATH_SYMBOLIC real(const RWGenMat<DComplex>& x);

/**
 * @relates RWGenMat
 *
 * Takes complex \a x as an argument and returns real \c y
 * containing the norm:
 *
 * \f$ y_{i} = [Re(x_{i})]^{2} +
 * [Im(x_{i})]^{2} \f$.
 */
RWGenMat<double> RW_MATH_SYMBOLIC norm(const RWGenMat<DComplex >& x);

// For LAPACK.h++ compatibility
/**
 * @relates RWGenMat
 *
 * Takes a matrix \a A (of \c M rows by \c N columns)
 * and a matrix \a B (of \c M rows by \c P columns)
 * as arguments, and returns a matrix \c C (of \c N rows
 * by \c P columns) that is equal to <i>A</i>TB, that is, the
 * inner product of the transpose of \a A with \a B:
 *
 * \f[
 * C_{np} = \sum_{m=0}^M A_{mn}B_{mp}
 * \f]
 *
 * If the number of rows in \a A does not match the number of
 * rows in \a B, an exception with value \c RWLAPK_MATMATPROD
 * occurs.
 *
 * Note that calling this function is equivalent to calling:
 *
 * @code
 * C  =  product(  transpose(A),  B);
 * @endcode
 */
template <class T>
RWGenMat<T> transposeProduct(const RWGenMat<T>& A, const RWGenMat<T>& B);

template <class T>
void transposeProduct(const RWGenMat<T>&, const RWGenMat<T>&, RWGenMat<T>&);

// For LAPACK.h++ compatibility
/**
 * @relates RWGenMat
 *
 * Takes a matrix \a A (of \c M rows by \c N columns)
 * and a matrix \a B (of \c M rows by \c P columns)
 * as arguments and returns a matrix \c C (of \c N rows
 * by \c P columns) which is equal to <tt>conj(\a A)T\a B</tt>, that
 * is, the inner product of the conjugate transpose of \a A
 * with \a B.
 *
 * If the number of rows in \a A does not match the number of
 * rows in \a B, an exception with value \c RWLAPK_MATMATPROD
 * occurs.
 *
 * @note Calling this function is equivalent to calling
 * <tt>C = product(conj(transpose(\a A)), \a B)</tt>.
 */
RWGenMat<DComplex> RW_MATH_SYMBOLIC conjTransposeProduct(const RWGenMat<DComplex>& A, const RWGenMat<DComplex>& B);

#if defined(__SUNPRO_CC) && (__SUNPRO_CC >= 0x500)

inline RWGenMat<float> RW_MATH_SYMBOLIC adjoint(const RWGenMat<float>& A)
{
    return transpose(A);
}

inline RWGenMat<double> RW_MATH_SYMBOLIC adjoint(const RWGenMat<double>& A)
{
    return transpose(A);
}

inline RWGenMat<DComplex> RW_MATH_SYMBOLIC adjoint(const RWGenMat<DComplex>& A)
{
    return conj(transpose(A));
}

#else
RWGenMat<float> RW_MATH_SYMBOLIC adjoint(const RWGenMat<float>& A);

RWGenMat<double> RW_MATH_SYMBOLIC adjoint(const RWGenMat<double>& A);

RWGenMat<DComplex> RW_MATH_SYMBOLIC adjoint(const RWGenMat<DComplex>& A);
#endif // __SUNPRO_CC

/**** Functions for type conversion ****/

/**
 * @relates RWGenMat
 *
 * Converts an \link RWGenMat RWGenMat<int>\endlink instance into a
 * corresponding \link RWGenMat RWGenMat<signed char>\endlink instance. Note
 * that this is a narrowing operation; high order bits are removed.
 * @see RWConvertGenMat
 */
RWGenMat<SChar> RW_MATH_SYMBOLIC toChar(const RWGenMat<int>& V);


/**
 * @relates RWGenMat
 *
 * Converts an \link RWGenMat RWGenMat<double>\endlink instance into a
 * corresponding \link RWGenMat RWGenMat<float>\endlink instance. Note that
 * this is a narrowing operation; high order bits are removed.
 * @see RWConvertGenMat
 */
RWGenMat<float> RW_MATH_SYMBOLIC toFloat(const RWGenMat<double>& V);

/**
 * @relates RWGenMat
 *
 * Converts an \link RWGenMat RWGenMat<double>\endlink instance into
 * a corresponding \link RWGenMat RWGenMat<int>\endlink instance. Note that
 * truncation occurs.
 * @see RWConvertGenMat
 */
RWGenMat<int> RW_MATH_SYMBOLIC toInt(const RWGenMat<double>& V);

/**
 * @relates RWGenMat
 *
 * Converts an \link RWGenMat RWGenMat<float>\endlink instance into
 * a corresponding \link RWGenMat RWGenMat<int>\endlink instance. Note that
 * truncation occurs.
 * @see RWConvertGenMat
 */
RWGenMat<int> RW_MATH_SYMBOLIC toInt(const RWGenMat<float>& V);

/**
 * @ingroup math_collection_classes
 * @brief Converts between RWGenMat types with different datatypes.
 *
 * Class \link RWConvertGenMat RWConvertGenMat<From,To> \endlink
 * converts between RWGenMat types with different datatypes. It assumes
 * that there is a conversion operator from From to To.
 *
 * @section synopsis Synopsis
 *
 * @code
 * template <class From, class To>
 * class RWConvertGenMat;
 * @endcode

 * @section example Example
 *
 * @code
 * #include <rw/math/genmat.h>
 *
 * int main()
 * {
 *        // create a 2x2 matrix
 *    RWGenMat<int> d("2x2[1 2 3 4]");
 *
 *        // convert int matrix d to DComplex
 *        // matrix c with imag part 0
 *    RWGenMat<DComplex> c = RWConvertGenMat<int,DComplex>(d);
 *
 *        // convert int matrix d to a double
 *        // matrix and then call sin
 *    RWGenMat<double> s = RWConvertGenMat<int,double>(d);
 *    s = sin(s);
 * }
 * @endcode
 *
 */
template <class From, class To>
struct RWConvertGenMat {
    /**
     * Constructs a new matrix of type \link RWGenMat RWGenMat<To> \endlink
     * that can be used anywhere as a type \link RWGenMat RWGenMat<To>
     * \endlink. Note that a temporary matrix is created every time
     * this constructor is invoked, but the conversion operator below
     * can be used repeatedly without creating additional temporary
     * matrices.
     */
    RWConvertGenMat(const RWGenMat<From>& v) : to(v.rows(), v.cols(), rwUninitialized) {
        typename RWGenMat<From>::const_iterator beginV = v.begin();
        typename RWGenMat<From>::const_iterator endV   = v.end();
        rw_math_transform(beginV, endV, to.begin(), RW_convert<From, To>());
        //    rw_math_transform(v.begin(), v.end(), to.begin(), RW_convert<From,To>());
    }

    /**
     * Automatic conversion operator that enables class \link RWConvertGenMat
     * RWConvertGenMat<From,To> \endlink to be used as an \link RWGenMat
     * RWGenMat<To> \endlink. While the actual conversion takes place
     * in the constructor, this operator provides a reference to that
     * converted matrix.
     */
    operator RWGenMat<To>() const {
        return to;
    }

private:
    RWGenMat<To> to;
};

#ifdef __GNUG__
template <class T>
RWGenMat<T> toGenMat(const RWMathArray<T>&);
#endif

/**
 * @relates RWGenMat
 *
 * The function l1Norm()is the maximum of the \c l1Norms of its columns:
 *
 * \f[
 * \left \| A \right \|_1 =
 * \begin{array}{c c}
 * \scriptstyle max \\ \scriptstyle  j=0,...,N-1
 * \end{array}
 *  \sum_{i=0}^{N-1}\left | a_i \right |
 * \f]
 */
double RW_MATH_SYMBOLIC l1Norm(const RWGenMat<DComplex>&); // Largest column sum

/**
 * @relates RWGenMat
 *
 * @copydoc l1Norm(const RWGenMat<DComplex>&)
 */
double RW_MATH_SYMBOLIC l1Norm(const RWGenMat<double>&); // Largest column sum

/**
 * @relates RWGenMat
 *
 * @copydoc l1Norm(const RWGenMat<DComplex>&)
 */
float RW_MATH_SYMBOLIC l1Norm(const RWGenMat<float>&); // Largest column sum

/**
 * @relates RWGenMat
 *
 * The function l1Norm()is the maximum of the \c l1fNorms of its rows:
 *
 * \f[
 * \left \| xA \right \|_{\infty} =
 * \begin{array}{c c}
 * \scriptstyle max \\ \scriptstyle  i=0,...,N-1
 * \end{array}
 *  \sum_{j=0}^{N-1}\left | a_{ij} \right |
 * \f]
 */
double RW_MATH_SYMBOLIC linfNorm(const RWGenMat<DComplex>&); // Largest row sum

/**
 * @relates RWGenMat
 *
 * @copydoc linfNorm(const RWGenMat<DComplex>&)
 */
double RW_MATH_SYMBOLIC linfNorm(const RWGenMat<double>&); // Largest row sum

/**
 * @relates RWGenMat
 *
 * @copydoc linfNorm(const RWGenMat<DComplex>&)
 */
float RW_MATH_SYMBOLIC linfNorm(const RWGenMat<float>&); // Largest row sum

// Root of sum of squares
/**
 * @relates RWGenMat
 *
 * Computes the Frobenius norm, which is the square root of the
 * sum of squares of its entries. For a matrix, the formula is:\par
 *
 * \f[
 *   \left \| A \right \|_{\text{Frob}} =
 *   \sqrt{\sum_{i=0}^{M-1} \sum_{j=0}^{N-1} \left | \text{a}_{ij} \right |^2}
 * \f]
 *
 */
double RW_MATH_SYMBOLIC frobNorm(const RWGenMat<DComplex>&);

/**
 * @relates RWGenMat
 *
 * @copydoc frobNorm(const RWGenMat<DComplex>&)
 */
double RW_MATH_SYMBOLIC frobNorm(const RWGenMat<double>&);

/**
 * @relates RWGenMat
 *
 * @copydoc frobNorm(const RWGenMat<DComplex>&)
 */
float RW_MATH_SYMBOLIC frobNorm(const RWGenMat<float>&);

/**
 * @relates RWGenMat
 *
 * Returns the value of the element with largest absolute value.
 * Note that this is not a norm in the mathematical sense of the word.
 */
double RW_MATH_SYMBOLIC maxNorm(const RWGenMat<DComplex>&);

/**
 * @relates RWGenMat
 *
 * @copydoc maxNorm(const RWGenMat<DComplex>&)
 */
double RW_MATH_SYMBOLIC maxNorm(const RWGenMat<double>&);

/**
 * @relates RWGenMat
 *
 * @copydoc maxNorm(const RWGenMat<DComplex>&)
 */
float RW_MATH_SYMBOLIC maxNorm(const RWGenMat<float>&);

/***************************
 * Inline Access Functions *
 ***************************/


template <class T>
inline T& RWGenMat<T>::operator()(int i, int j)
{
#ifdef RWBOUNDS_CHECK
    boundsCheck(i, j);
#endif
    return data()[i * rowstep + j * colstep];
}

template <class T>
inline T RWGenMat<T>::operator()(int i, int j) const
{
#ifdef RWBOUNDS_CHECK
    boundsCheck(i, j);
#endif
    return data()[i * rowstep + j * colstep];
}

template <class T>
inline RWMathVec<T> RWGenMat<T>::operator()(int i, const RWSlice& j)
{
#ifdef RWBOUNDS_CHECK
    rowBoundsCheck(i);
    j.boundsCheck(ncols);
#endif
    return fastSlice(i, j.begin(), static_cast<unsigned>(j.len(static_cast<unsigned>(ncols))), 0, j.stride());
}

template <class T>
inline const RWMathVec<T> RWGenMat<T>::operator()(int i, const RWSlice& j) const
{
#ifdef RWBOUNDS_CHECK
    rowBoundsCheck(i);
    j.boundsCheck(ncols);
#endif
    return fastSlice(i, j.begin(),
                     static_cast<unsigned>(j.len(static_cast<int>(ncols))),
                     0, j.stride());
}

template <class T>
inline RWMathVec<T> RWGenMat<T>::operator()(const RWSlice& i, int j)
{
#ifdef RWBOUNDS_CHECK
    colBoundsCheck(j);
    i.boundsCheck(nrows);
#endif
    return fastSlice(i.begin(), j, static_cast<unsigned>(i.len(static_cast<unsigned>(nrows))), i.stride(), 0);
}

template <class T>
inline const RWMathVec<T> RWGenMat<T>::operator()(const RWSlice& i, int j) const
{
#ifdef RWBOUNDS_CHECK
    colBoundsCheck(j);
    i.boundsCheck(nrows);
#endif
    return fastSlice(i.begin(), j, static_cast<unsigned>(i.len(static_cast<unsigned>(nrows))), i.stride(), 0);
}

template <class T>
inline RWGenMat<T> RWGenMat<T>::operator()(const RWSlice& i, const RWSlice& j)
{
#ifdef RWBOUNDS_CHECK
    i.boundsCheck(nrows);
    j.boundsCheck(ncols);
#endif
    return fastSlice(i.begin(), j.begin(), static_cast<unsigned>(i.len(static_cast<unsigned>(nrows))), static_cast<unsigned>(j.len(static_cast<unsigned>(ncols))), i.stride(), 0, 0, j.stride());
}

template <class T>
inline const RWGenMat<T> RWGenMat<T>::operator()(const RWSlice& i, const RWSlice& j) const
{
#ifdef RWBOUNDS_CHECK
    i.boundsCheck(nrows);
    j.boundsCheck(ncols);
#endif
    return fastSlice(i.begin(), j.begin(), static_cast<unsigned>(i.len(static_cast<unsigned>(nrows))), static_cast<unsigned>(j.len(static_cast<unsigned>(ncols))), i.stride(), 0, 0, j.stride());
}


// These next functions predate the nice subscripting mechanisms
// available with the current math.h++

template <class T>
inline const RWMathVec<T> RWGenMat<T>::col(int j) const
{
    return (*this)(RWAll, j);
}
template <class T>
inline       RWMathVec<T> RWGenMat<T>::col(int j)
{
    return (*this)(RWAll, j);
}
template <class T>
inline const RWMathVec<T> RWGenMat<T>::row(int i) const
{
    return (*this)(i, RWAll);
}
template <class T>
inline       RWMathVec<T> RWGenMat<T>::row(int i)
{
    return (*this)(i, RWAll);
}


// The following functions are for compatibility with the other matrix
// types provided by matrix.h++

template <class T>
inline T& RWGenMat<T>::ref(int i, int j)
{
    return (*this)(i, j);
}

template <class T>
inline void RWGenMat<T>::set(int i, int j, const T& x)
{
    (*this)(i, j) = x;
}

template <class T>
inline T RWGenMat<T>::val(int i, int j) const
{
    return (*this)(i, j);
}

template <class T>
inline T& RWGenMat<T>::bcref(int i, int j)
{
    boundsCheck(i, j);
    return data()[i * rowstep + j * colstep];
}

template <class T>
inline void RWGenMat<T>::bcset(int i, int j, const T& x)
{
    boundsCheck(i, j);
    data()[i * rowstep + j * colstep] = x;
}

template <class T>
inline T RWGenMat<T>::bcval(int i, int j) const
{
    boundsCheck(i, j);
    return data()[i * rowstep + j * colstep];
}


// Here are the definitions of operator% for inner products.  We
// had to postpone them because in the operator section product()
// was not yet declared.
/**
 * @relates RWGenMat
 *
 * Conventional mathematical linear algebra multiplication operator;
 * that is, the <tt>i,j</tt> element of the result of a matrix product
 * <i>A%B</i> is the dot product of the <tt>i<SUP>th</SUP></tt> row
 * of \a A and the <tt>j<SUP>th</SUP></tt> column of \a B.
 */
template <class T>
inline RWGenMat<T>         operator%(const RWGenMat<T>& A, const RWGenMat<T>& B)
{
    return product(A, B);
}

/**
 * @relates RWGenMat
 *
 * @copydoc operator%(const RWGenMat<T>&,const RWGenMat<T>&)
 */
template <class T>
inline RWMathVec<T>      operator%(const RWGenMat<T>& A, const RWMathVec<T>& x)
{
    return product(A, x);
}

/**
 * @relates RWGenMat
 *
 * @copydoc operator%(const RWGenMat<T>&,const RWGenMat<T>&)
 */
template <class T>
inline RWMathVec<T>      operator%(const RWMathVec<T>& x, const RWGenMat<T>& A)
{
    return product(x, A);
}




#if defined(RW_TEMP_NO_STD_BASE) /* avoid preprocessor warning */
#  undef RW_TEMP_NO_STD_BASE
#endif

#if defined(_MSC_VER) && (_MSC_VER == 1500)
#  pragma warning(disable:4910)
#endif

RWMATH_INSTANTIATE(RW_MATH_GLOBAL, RWGenMat<DComplex>);

#if defined(_MSC_VER) && (_MSC_VER == 1500)
#  pragma warning(default:4910)
#endif

#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/math/genmat.cc>
#endif

#endif /* __RWDGENMAT_H__ */
