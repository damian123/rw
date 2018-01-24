#ifndef __RWMATHVEC_H__
#define __RWMATHVEC_H__

/***************************************************************************
 *
 * mathvec.h
 *   Defining the preprocessor directive RWBOUNDS_CHECK will
 *   cause bounds checking on the subscripting operators.
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

#include <rw/defs.h>

#include <iterator>
#include <rw/vector.h>
#include <rw/rwslice.h>
#include <rw/tools/iterator.h>

#include <rw/math/rwbla.h>
#include <rw/math/rwalloc.h>
#include <rw/math/numtrait.h>
#include <rw/math/funcobj.h>

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
template<class T> struct __promote_2 <RWMathVec<std::complex<T> >, T> { };

template<class T> struct __promote_2 <T, RWMathVec<std::complex<T> > > { };
}
#endif

class RW_MATH_SYMBOLIC RWRandInterface;

class RW_MATH_SYMBOLIC RWRandGenerator;

template< class T >
class RWMathVec;

template< class T >
class RWMathArray;

template< class T >
class RWMathVecPick;

template< class T >
class RWGenMat;

// **************************************************************************
//
// Iterator classes --------------------------------------------------------
//
// **************************************************************************

/**
 * @ingroup math_collection_classes
 * @brief A base class for RWMathVec iterators.
 *
 * A base class from which the iterators for RWMathVec are derived.
 */
template<class T>
class RWMathVecIteratorBase
{
public:

    typedef typename RWMathVec<T>::difference_type difference_type;
    typedef typename RWMathVec<T>::value_type      value_type;
    typedef typename RWMathVec<T>::pointer         pointer;
    typedef typename RWMathVec<T>::const_pointer   const_pointer;

    // Comparison operators
    /**
     * Equality operator.  Returns \c true if the \link RWMathVecIteratorBase
     * RWMathVecIteratorBase<T>\endlink objects point to the same element.
     */
    bool operator==(const RWMathVecIteratorBase<T>& x) const {
        return (data_ == x.data_);
    }

    /**
     * Inequality operator.  Returns \c false if the \link RWMathVecIteratorBase
     * RWMathVecIteratorBase<T>\endlink objects point to the same element.
     */
    bool operator!=(const RWMathVecIteratorBase<T>& x) const {
        return !(*this == x);
    }

    /**
     * Less than operator.  Determines the relative logical position
     * between two \link RWMathVecIteratorBase RWMathVecIteratorBase<T>\endlink
     * objects, not necessarily the relative memory location of the elements to
     * which they point.
     */
    bool operator<(const RWMathVecIteratorBase<T>& x) const {
        return (stride_ >= 0) ? (data_ < x.data_) : (data_ > x.data_);
    }

    /**
     * Greater than operator.  Determines the relative logical position
     * between two \link RWMathVecIteratorBase RWMathVecIteratorBase<T>\endlink
     * objects, not necessarily the relative memory location of the elements to
     * which they point.
     */
    bool operator>(const RWMathVecIteratorBase<T>& x) const {
        return !(*this < x) && !(*this == x);
    }

    /**
     * Less than or equal to operator.  Determines the relative logical position
     * between two \link RWMathVecIteratorBase RWMathVecIteratorBase<T>\endlink
     * objects, not necessarily the relative memory location of the elements to
     * which they point.
     */
    bool operator<=(const RWMathVecIteratorBase<T>& x) const {
        if (stride_ >= 0) {
            return (*this < x) || (*this == x);
        }

        return !(*this < x);
    }

    /**
     * Greater than or equal to operator.  Determines the relative logical
     * position between two \link RWMathVecIteratorBase
     * RWMathVecIteratorBase<T>\endlink objects, not necessarily the relative
     * memory location of the elements to which they point.
     */
    bool operator>=(const RWMathVecIteratorBase<T>& x) const {
        if (stride_ >= 0) {
            return !(*this < x);
        }

        return (*this < x) || (*this == x);
    }

protected:
    RWMathVecIteratorBase() {
        ;
    }
    RWMathVecIteratorBase(T* d, int s): data_(d), stride_(s) {
        ;
    }

#if defined(__SUNPRO_CC) && ((__SUNPRO_CC == 0x580) || (__SUNPRO_CC == 0x590))
    /* Fix:
       Warning (Anachronism), badcopyacc2w: Formal argument x of type const
       RWMathVecIteratorBase<std::complex<double>>& in call to
       RWMathVecIteratorBase<std::complex<double>>::operator!=(const
       RWMathVecIteratorBase<std::complex<double>>&) const has an inaccessible
       copy constructor.
    */

public:
#endif
    RWMathVecIteratorBase(const RWMathVecIteratorBase& x) {
        data_ = x.data_;
        stride_ = x.stride_;
    }

protected:
    difference_type operator_minus(const RWMathVecIteratorBase<T>& x) const {
        return (data_ - x.data_) / stride_;
    }

    // Prefix increment and decrement operators
    RWMathVecIteratorBase<T>& operator_plus_plus() {
        data_ += stride_;
        return *this;
    }
    RWMathVecIteratorBase<T>& operator_minus_minus() {
        data_ -= stride_;
        return *this;
    }

    // Assignment operators
    RWMathVecIteratorBase<T>& operator_plus_equal(difference_type d) {
        data_ += d * stride_;
        return *this;
    }

    RWMathVecIteratorBase<T>& operator_minus_equal(difference_type d) {
        this->operator_plus_equal(-d);
        return *this;
    }

    // Subscripting operator
    value_type& operator_square_brackets(difference_type n) const {
        return *(data_ + n * stride_);
    }

    // Indirection operator
    value_type& operator_star() const {
        return *data_;
    }

    // pointer operator
    pointer operator_arrow() const {
        return &(*data_);
    }

private:
    T*  data_;
    int stride_;
};


/**
 * @ingroup math_collection_classes
 * @brief The random access iterator for the
 * \link RWMathVec RWMathVec<T>\endlink collection class.
 *
 * Class \link RWMathVecIterator RWMathVecIterator<T>\endlink is the random
 * access iterator for the \link RWMathVec RWMathVec<T>\endlink collection
 * class. The \link RWMathVecIterator RWMathVecIterator<T>\endlink class
 * behaves like a pointer to type T and is used to increment through elements
 * of an \link RWMathVec RWMathVec<T>\endlink.
 * @note This class should be accessed through RWMathVec<T>::iterator.
 * @section example Example
 *
 * @code
 * #include <rw/math/mathvec.h>
 *
 * int main()
 * {
 *     RWMathVec<int> V(5,rwUninitialized);
 *     RWMathVec<int>::iterator i = V.begin();
 *     RWMathVec<int>::iterator stop = V.end();

 *
 *     // Assign a unique int value to each element of V
 *     for (int j=0; i != stop; ++i, ++j) *i = j;
 * }
 * @endcode
 *
 */
template <class T>
class RWMathVecIterator
    : public RWMathVecIteratorBase<T>,
  public std::iterator < std::random_access_iterator_tag,
      typename RWMathVec<T>::value_type,
      typename RWMathVec<T>::difference_type
      >
{
public:

    typedef typename RWMathVec<T>::difference_type difference_type;
    typedef typename RWMathVec<T>::value_type      value_type;
    typedef typename RWMathVec<T>::pointer         pointer;
    typedef typename RWMathVec<T>::const_pointer   const_pointer;

    /**
     * Constructs an \link RWMathVecIterator RWMathVecIterator<T>\endlink that is
     * in an uninitialized state. The iterator must be initialized before it is
     * dereferenced.
     */
    RWMathVecIterator() {
        ;
    }
    RWMathVecIterator(T* d, int s): RWMathVecIteratorBase<T>(d, s) {
        ;
    }

    /**
     * Constructs an \link RWMathVecIterator RWMathVecIterator<T>\endlink from
     * another \link RWMathVecIterator RWMathVecIterator<T>\endlink. Initialized
     * iterators can be obtained via the RWMathVec<T>::begin() and
     * RWMathVec<T>::end() member functions.
     */
    RWMathVecIterator(const RWMathVecIterator<T>& x): RWMathVecIteratorBase<T>(x) {
        ;
    }

    //Operators --------------------------------------------------------

    // Prefix increment and decrement operators
    /**
     * Prefix or postfix increment or decrement operator. The \link
     * RWMathVecIterator RWMathVecIterator<T>\endlink
     * points to the next or previous element in the \link RWMathVec
     * RWMathVec<T> \endlink. No error condition is set if the \link
     * RWMathVecIterator RWMathVecIterator<T>\endlink
     * goes past the bounds of the vector. Compare to RWMathVec<T>::begin()
     * and RWMathVec<T>::end() to check that the iterator position is
     * valid.
     */
    RWMathVecIterator<T>& operator++() {
        this->operator_plus_plus();
        return *this;
    }

    /**
     * @copydoc operator++
     */
    RWMathVecIterator<T>& operator--() {
        this->operator_minus_minus();
        return *this;
    }

    // Postfix increment and decrement operators if ya got'em
#if   !defined(RW_NO_POSTFIX)
    /**
     * @copydoc operator++
     */
    RWMathVecIterator<T> operator++(int) {
        RWMathVecIterator<T> tmp = *this;
        ++(*this);
        return tmp;
    }

    /**
     * @copydoc operator++
     */
    RWMathVecIterator<T> operator--(int) {
        RWMathVecIterator<T> tmp = *this;
        --(*this);
        return tmp;
    }
#endif

    // Binary operators
    /**
     * If the return value is positive, moves the iterator that many
     * elements past the iterator \a i. If the return value is negative,
     * moves the iterator that many elements before the iterator \a
     * i.
     */
    difference_type operator-(const RWMathVecIteratorBase<T>& i) const {
        return this->operator_minus(i);
    }

    // Assignment operators
    /**
     * Moves the iterator forward or backward \a d elements in the \link
     * RWMathVec RWMathVec<T>\endlink. No error condition is set if
     * the iterator goes past the bounds of the vector. Incrementing
     * by \a d and then decrementing by \a d returns the iterator to
     * its original position, even if doing so takes it past the bounds
     * of the vector.
     */
    RWMathVecIterator<T>& operator+=(difference_type d) {
        this->operator_plus_equal(d);
        return *this;
    }

    RWMathVecIterator<T>& operator-=(difference_type d) {
        return *this += -d;;
    }

    // Binary operators
    /**
     * Returns an iterator that is \a d elements past (or before) self.
     */
    RWMathVecIterator<T> operator-(difference_type d) const {
        RWMathVecIterator<T> tmp = *this;
        return tmp -= d;
    }

    RWMathVecIterator<T> operator+(difference_type n) const {
        RWMathVecIterator<T> tmp = *this;
        return tmp += n;
    }

    // Subscripting operator
    /**
     * Returns a reference to the element \a n elements after self if
     * \a n is positive, or \a n elements before self if \a n is negative.
     */
    value_type& operator[](difference_type n) const {
        return this->operator_square_brackets(n);
    }

    // Indirection operator
    /**
     * Returns a reference to the element pointed to by self.
     */
    value_type& operator*() const {
        return this->operator_star();
    }

    // pointer operator
    /**
     *   For iterator a, and identifier m, a->m is equivalent to (*a).m.
     */
    pointer operator->() const {
        return this->operator_arrow();
    }
};


// *****************************************************************
// RWMathVecConstIterator -------------------------------------------------
// *****************************************************************

/**
 * @ingroup math_collection_classes
 * @brief The \c const random access iterator for the
 * \link RWMathVec RWMathVec<T> \endlink collection class.
 *
 * Class \link RWMathVecConstIterator RWMathVecConstIterator<T> \endlink is the
 * \c const random access iterator for the \link RWMathVec RWMathVec<T> \endlink
 * collection class. The \link RWMathVecConstIterator RWMathVecConstIterator<T>
 * \endlink class behaves like a pointer to type \c T and is used to increment
 * through elements of an \link RWMathVec RWMathVec<T> \endlink. The \link
 * RWMathVecConstIterator RWMathVecConstIterator<T> \endlink differs
 * from \link RWMathVecIterator RWMathVecIterator<T> \endlink in that the value
 * in a dereferenced \link RWMathVecConstIterator RWMathVecConstIterator<T>
 * \endlink may not be changed.
 *
 * @note This class should be used through RWMathVec<T>::const_iterator.
 *
 * @section example Example
 *
 * @code
 * #include <rw/math/mathvec.h>
 *
 * int main()
 * {
 *     RWMathVec<int> V(5,1); // Create a length 5 vector of 1's
 *     RWMathVec<int>::const_iterator i = V.begin();
 *     RWMathVec<int>::const_iterator stop = V.end();

 *
 *     // Print each element of V
 *     while(i != stop) {
 *         cout << *i << endl;
 *         ++i;
 *     }
 * }
 * @endcode
 *
 */
template <class T>
class RWMathVecConstIterator
    : public RWMathVecIteratorBase<T>,
  public std::iterator < std::random_access_iterator_tag,
      typename RWMathVec<T>::value_type,
      typename RWMathVec<T>::difference_type
      >
{
public:

    typedef typename RWMathVec<T>::difference_type difference_type;
    typedef typename RWMathVec<T>::value_type      value_type;
    typedef typename RWMathVec<T>::pointer         pointer;
    typedef typename RWMathVec<T>::const_pointer   const_pointer;

    /**
     * Constructs a \link RWMathVecConstIterator RWMathVecConstIterator<T>
     * \endlink that is in an uninitialized state. The \link
     * RWMathVecConstIterator RWMathVecConstIterator<T> \endlink must be
     * initialized before it is dereferenced.
     */
    RWMathVecConstIterator() {
        ;
    }
    RWMathVecConstIterator(T* d, int s): RWMathVecIteratorBase<T>(d, s) {
        ;
    }

    /**
     * Constructs a \link RWMathVecConstIterator RWMathVecConstIterator<T>
     * \endlink from another \link RWMathVecConstIterator
     * RWMathVecConstIterator<T> \endlink. Initialized \link
     * RWMathVecConstIterator RWMathVecConstIterator<T> \endlink
     * can be obtained via the RWMathVec<T>::begin() and RWMathVec<T>::end().
     */
    RWMathVecConstIterator(const RWMathVecConstIterator<T>& x): RWMathVecIteratorBase<T>(x) {
        ;
    }

    /**
     * Constructs a \link RWMathVecConstIterator RWMathVecConstIterator<T>
     * \endlink from another \link RWMathVecIterator
     * RWMathVecIterator<T> \endlink. Initialized \link
     * RWMathVecIterator RWMathVecIterator<T> \endlink
     * can be obtained via the RWMathVec<T>::begin() and RWMathVec<T>::end().
     */
    RWMathVecConstIterator(const RWMathVecIterator<T>& x): RWMathVecIteratorBase<T>(x) {
        ;
    }

    RWMathVecConstIterator(const RWMathVecIteratorBase<T>& x): RWMathVecIteratorBase<T>(x) {
        ;
    }

    //Operators --------------------------------------------------------

    // Prefix increment and decrement operators
    /**
     * Prefix or postfix increment or decrement operator. The \link
     * RWMathVecConstIterator RWMathVecConstIterator<T> \endlink
     * points to the next or previous element in the \link RWMathVec
     * RWMathVec<T> \endlink. No error condition is set if the \link
     * RWMathVecConstIterator RWMathVecConstIterator<T> \endlink
     * goes past the bounds of the vector. Compare to RWMathVec<T>::begin()
     * and RWMathVec<T>::end() to check that the \link RWMathVecConstIterator
     * RWMathVecConstIterator<T> \endlink position is valid.
     */
    RWMathVecConstIterator<T>& operator++() {
        this->operator_plus_plus();
        return *this;
    }

    /**
     * @copydoc operator++()
     */
    RWMathVecConstIterator<T>& operator--() {
        this->operator_minus_minus();
        return *this;
    }

    // Postfix increment and decrement operators if ya got'em
#if   !defined(RW_NO_POSTFIX)
    /**
     * @copydoc operator++()
     */
    RWMathVecConstIterator<T> operator++(int) {
        RWMathVecConstIterator<T> tmp = *this;
        ++(*this);
        return tmp;
    }

    /**
     * @copydoc operator++()
     */
    RWMathVecConstIterator<T> operator--(int) {
        RWMathVecConstIterator<T> tmp = *this;
        --(*this);
        return tmp;
    }
#endif

    // Binary operators
    /**
     * If the return value is positive, sets the \link RWMathVecConstIterator
     * RWMathVecConstIterator<T> \endlink  that many elements past the \link
     * RWMathVecConstIterator RWMathVecConstIterator<T> \endlink  \a i. If the
     * return value is negative, sets the \link RWMathVecConstIterator
     * RWMathVecConstIterator<T> \endlink  that many elements before the \link
     * RWMathVecConstIterator RWMathVecConstIterator<T> \endlink  \a i.
     */
    difference_type operator-(const RWMathVecIteratorBase<T>& i) const {
        return this->operator_minus(i);
    }

    // Assignment operators
    /**
     * Moves the \link RWMathVecConstIterator RWMathVecConstIterator<T> \endlink
     * forward or backward \a d elements in the \link RWMathVec RWMathVec<T>
     * \endlink. No error condition is set if the \link RWMathVecConstIterator
     * RWMathVecConstIterator<T> \endlink goes past the bounds of the vector.
     * Incrementing by \a d and then decrementing by \a d returns the
     * \link RWMathVecConstIterator RWMathVecConstIterator<T> \endlink to its
     * original position, even if doing so takes it past the bounds of the vector.
     */
    RWMathVecConstIterator<T>& operator+=(difference_type d) {
        this->operator_plus_equal(d);
        return *this;
    }

    /**
     * @copydoc operator+=(difference_type)
     */
    RWMathVecConstIterator<T>& operator-=(difference_type d) {
        return *this += -d;
    }

    // Binary operators
    /**
     * Returns a \link RWMathVecConstIterator RWMathVecConstIterator<T> \endlink
     * that is \a d elements before self.
     */
    RWMathVecConstIterator<T> operator-(difference_type d) const {
        RWMathVecConstIterator<T> tmp = *this;
        return tmp -= d;
    }

    /**
     * Returns a \link RWMathVecConstIterator RWMathVecConstIterator<T> \endlink
     * that is \a d elements past self.
     */
    RWMathVecConstIterator<T> operator+(difference_type d) const {
        RWMathVecConstIterator<T> tmp = *this;
        return tmp += d;
    }

    // Subscripting operator
    /**
     * Returns a reference to the element \a n elements after self if
     * \a n is positive, or \a n elements before self if \a n is negative.
     */
    const value_type& operator[](difference_type n) const {
        return this->operator_square_brackets(n);
    }

    // Indirection operator
    /**
     * Returns a reference to the element pointed to by self.
     */
    const value_type& operator*() const {
        return this->operator_star();
    }

    // pointer operator
    /**
     *   For iterator a, and identifier m, a->m is equivalent to (*a).m.
     */
    const_pointer operator->() const {
        return this->operator_arrow();
    }
};

template <class T>
RWMathVecIterator<T> operator+(const typename RWMathVecIterator<T>::difference_type i, const RWMathVecIterator<T>& it)
{
    return it + i;
}

template <class T>
RWMathVecConstIterator<T> operator+(const typename RWMathVecConstIterator<T>::difference_type i, const RWMathVecConstIterator<T>& it)
{
    return it + i;
}

//**************************************************************************
//
//-----------------------  END OF ITERATOR CLASSES -------------------------
//
//**************************************************************************

typedef RWMathVec<int> RWIntVec;

// predeclare friend function
template<class T> RWMathVec<T> toVec(const RWMathArray<T>&);

/**
 * @ingroup math_collection_classes
 * @brief A templatized vector class.
 *
 * Class \link RWMathVec RWMathVec<T> \endlink is a templatized
 * vector class.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/math/mathvec.h>
 * RWMathVec<T> vector;
 * @endcode

 * @section example Example
 *
 * @code
 * #include <rw/math/mathvec.h>
 *
 * int main()
 * {
 *    RWMathVec<int> v(5, rwUninitialized);
 *    v = 3; // Set all elements in v to 3
 *    RWMathVec<int> w = v + 1;
 *    RWMathVec<int> y = v + w;
 * }
 * @endcode
 *
 *
 * @see \link RWConvertMathVec RWConvertMathVec<From,To> \endlink
 *
 */
template< class T >
class RWMathVec : public RWVecView
{
public:

    //
    // Typedefs
    //
    // STL style typedefs ---------------------------------------------------
    typedef size_t    size_type;
    typedef ptrdiff_t difference_type;
    typedef T*        pointer;
    typedef const T*  const_pointer;
    typedef T         value_type;

    /**
     * A type that provides a random-access iterator over the elements
     * in the container.
     */
    typedef RWMathVecIterator<T> iterator;

    /**
     * A type that provides a \c const random-access iterator over the
     * elements in the container.
     */
    typedef RWMathVecConstIterator<T> const_iterator;

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

    // Numeric traits typedefs ----------------------------------------------

    /**
     * Typedef for the promotion type. For more information,
     * see rw_numeric_traits<T>::promote_type.
     */
    typedef typename rw_numeric_traits<T>::promote_type   promote_type;

    /**
     * Typedef for the usual return type of numerical <tt>norm</tt>-like
     * functions. For more information, see rw_numeric_traits<T>::norm_type.
     */
    typedef typename rw_numeric_traits<T>::norm_type      norm_type;

    /**
     * Typedef for the function pointer used in the method apply().
     * For more information, see rw_numeric_traits<T>::mathFunType.
     */
    typedef typename rw_numeric_traits<T>::mathFunType    mathFunType;

    /**
     * Typedef for the function pointer used in the method apply2().
     * For more information, see rw_numeric_traits<T>::mathFunType2.
     */
    typedef typename rw_numeric_traits<T>::mathFunType2    mathFunType2;


#ifdef RW_NATIVE_EXTENDED
    typedef extended(*XmathFunType)(extended);
#endif


    //****************
    // Constructors
    //****************
    /**
     * Constructs a vector with \a n elements initialized with random
     * numbers generated by \a r.
     */
    RWMathVec(size_t n, RWRandInterface& r);

#ifdef RW_ALLOW_OLD_ARRAY_CONSTRUCTORS
    RWMathVec(size_t n) : RWVecView(new RWAllocBlock< T, RWDefaultAllocator<T> >(n), n) {
        ;
    }
#endif

    /**
     * Constructs a 0-length vector, useful for declaring arrays of
     * vectors. Like any other vector, this vector can subsequently
     * be reshaped or resized (see member functions reshape() and resize()).
     */
    RWMathVec() : RWVecView() {
        ;
    }

    /**
     * Copy constructor. The new vector and the old vector both view
     * the same data.
     */
    RWMathVec(const RWMathVec<T>& a): RWVecView((const RWVecView&)a) {
        ;
    }

    /**
     * Constructs a vector with \a n elements. The #RWUninitialized type
     * is an enumeration type with only one value, #rwUninitialized.
     * The #rwUninitialized argument is used to distinguish the last
     * dimension size from an initial value.
     */
    RWMathVec(size_t n, RWUninitialized)
        : RWVecView(new RWAllocBlock< T, RWDefaultAllocator<T> >(n), n) {
        ;
    }

    /**
     * Constructs a vector with \a n elements, initialized to \a initval.
     */
    RWMathVec(size_t n, const T& initval);

    /**
     * Constructs a vector with \a n elements. The first has value \a
     * initval; each succeeding element is incremented by \a incr.
     */
    RWMathVec(size_t n, const T& initval, const T& incr);

    /**
     * Constructs a matrix from the null terminated character string
     * \a s. The format of the character string is the same as that
     * expected by the global operator operator>>() described in this
     * entry.
     */
    RWMathVec(const char* s);

    /**
     * Constructs a new vector from an \link RWMathVecPick RWMathVecPick<T>
     * \endlink. A copy of \a p's data is made.
     */
    RWMathVec(const RWMathVecPick<T>& p);

    /**
     * Constructs a vector with \a n elements, using the data in the
     * vector \a dat as initial data. A copy of \a dat is made. The
     * vector \a dat must have at least \a n elements.
     */
    RWMathVec(const T* dat, size_t n);

    // Pass a custom block on the free store
    RWMathVec(RWBlock* block, size_t n);

    /**
     * A complex \c vec is constructed from the double precision arrays
     * \a re and \a im, with the real part of the \c vec equal to \a
     * re and the imaginary part equal to \a im. A new copy of the data
     * is made.
     */
    RWMathVec(const RWMathVec<double>& re, const RWMathVec<double>& im);

    friend class RWGenMat<T>;    // For row,col,diagonal


    //**************************************************************************
    //
    //  Member functions
    //
    //**************************************************************************

    // had to put inline so that Borland Builder 4 could find function apply

#ifdef RW_NATIVE_EXTENDED
    RWMathVec<T>           apply(XmathFunType) const;
#endif

    /**
     * Returns the result of applying the passed function to every element
     * in the vector. A function of type RWMathVec<T>::mathFunType
     * takes and returns a \c T; a function of type RWMathVec<T>::mathFunType2
     * takes a \c T and returns an RWMathVec<T>::norm_type.
     * @see rw_numeric_traits for a description of RWMathVec<T>::norm_type.
     */
    RWMathVec<T>   apply(typename rw_numeric_traits<T>::mathFunType f) const {
        register size_t i = length();
        RWMathVec<T> temp(i, rwUninitialized);
        register const T* sp = data();
        register T* dp = temp.data();
        register int j = stride();
        while (i--) {
            *dp++ = (T)(*f)(*sp);
            sp += j;
        }
        return temp;
    }

    // Some compilers (Borland 4.5) only understand functions
    // templatized on a traits type if they are defined in line
    // at class scope.
#ifdef RW_INLINE_TRAITS_TEMPLATES
    RWMathVec<norm_type> apply2(mathFunType2 f) const {
        register size_t i = length();
        RWMathVec<norm_type> temp(i, rwUninitialized);
        register T* sp = (T*)data();
        register norm_type* dp = (norm_type*)temp.data();
        register int j = stride();
        while (i--) {
            *dp++ = (*f)(*sp);
            sp += j;
        }
        return temp;
    }
#else
    /**
     * @copydoc apply
     */
    RWMathVec<norm_type> apply2(mathFunType2 f) const;
#endif

    /**
     * Returns an iterator that points to the first element in the vector.
     */
    iterator begin() {
        return iterator((T*)data(), stride());
    }

    /**
     * @copydoc begin
     */
    const_iterator begin() const {
        return const_iterator((T*)data(), stride());
    }

    /**
     * @copydoc begin
     */
    const_iterator cbegin() const {
        return begin();
    }

    /**
     * Returns the number of bytes required to store the vector to an
     * RWFile using member function
     * \link saveOn(RWFile&) const saveOn(RWFile&)\endlink.
     */
    size_t               binaryStoreSize() const;

    /**
     * Returns a copy with distinct instance variables.
     */
    RWMathVec<T>           copy() const;

    /**
     * Returns a pointer to the start of a vector's data. Should be
     * used with care, as this accesses the vector's data directly.
     * You are responsible for recognizing the length and the stride
     * of the vector.
     */
    T*                     data() {
        return (T*)begin_;
    }

    /**
     * @copydoc data
     */
    const T*               data() const {
        return (const T*)begin_;
    }

    /**
     * Alias for copy().
     */
    RWMathVec<T>           deepCopy() const;

    /**
     * When invoked for a vector, guarantees that there is only one
     * reference to that object and that its data are in contiguous
     * storage.
     */
    void                   deepenShallowCopy();

    /**
     * Returns an iterator that points to one element past the last
     * element in the vector.
     */
    iterator end() {
        return iterator((T*)(data() + stride() * length()), stride());
    }

    /**
     * @copydoc end
     */
    const_iterator end() const {
        return const_iterator((T*)(data() + stride() * length()), stride());
    }

    /**
     * @copydoc end
     */
    const_iterator cend() const {
        return end();
    }

    /**
     * Returns the number of elements in the vector.
     */
    size_t               length() const {
        return RWVecView::length();
    };

    /**
     * Returns a vector \c pick. The results can be used as an lvalue.
     * You can think of the picked elements as representing a vector
     * in the order specified by the vector of indices \a v. Before
     * using this function, you must include the header file \c rw/math/mthvecpk.h.
     */
    RWMathVecPick<T>       pick(const RWIntVec& v);

    // const version can only be used in vec ctor
    const RWMathVecPick<T> pick(const RWIntVec& x) const;

    void                   printOn(std::ostream& s, size_t numberPerLine = 5) const;

    /**
     * Returns an iterator that points to the last element in the vector.
     */
    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    /**
     * @copydoc begin
     */
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    /**
     * @copydoc begin
     */
    const_reverse_iterator crbegin() const {
        return rbegin();
    }

    /**
     * Returns an iterator that points to one element past the first
     * element in the vector.
     */
    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    /**
     * @copydoc rend
     */
    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    /**
     * @copydoc rend
     */
    const_reverse_iterator crend() const {
        return rend();
    }

    /**
     * Makes self a view of \a v's data. The view currently associated
     * with the vector is lost.
     */
    RWMathVec<T>&          reference(const RWMathVec<T>& v);

    /**
     * Changes the size of the vector to \a n elements, adding 0s or
     * truncating as necessary.
     */
    void resize(size_t n);

    /**
     * Changes the length of the vector to \a n elements. After reshaping,
     * the contents of the vector are undefined; that is, they can be
     * and probably will be garbage.
     */
    void reshape(size_t n);

    /**
     * Restores self from a virtual stream or an RWFile. To use these
     * functions with a user-defined type T, the corresponding \c operator>>
     * must be defined:
     *
     * @code
     * RWvistream&  operator>>(RWvistream& T&);
     * RWFile&      operator>>(RWFile&, T&);
     *
     * @endcode
     */
    void restoreFrom(RWFile&);      // Restore from binary

    /**
     * @copydoc restoreFrom
     */
    void restoreFrom(RWvistream&);  // Restore from virtual stream

    /**
     * Stores self to a virtual stream, or in a binary format to an
     * RWFile. To use these functions with a user-defined type T, the
     * corresponding \c operator<< must be defined:
     *
     * @code
     * RWvostream&  operator<<(RWvostream& T&);
     * RWFile&      operator<<(RWFile&, T&);
     *
     * @endcode
     */
    void saveOn(RWFile&) const;     // Store using binary

    /**
     * @copydoc saveOn
     */
    void saveOn(RWvostream&) const; // Store to virtual stream

    void scanFrom(std::istream& s);      // Read to eof or delimit with []

    // static int              setFormatting(int); // defined in base class RWVecView

    /**
     * Returns a vector which views a slice of the data. The slice begins
     * at element \a start and extends for \a n elements. The increment
     * between successive elements in the slice is \a stride.
     */
    RWMathVec<T>  slice(int start, size_t n, int stride = 1) const;

    /**
     * Returns the distance between successive elements of the vector. The stride can be computed using <tt>&v[i+1]-&v[i]</tt>.
     */
    int                     stride() const {
        return RWVecView::stride();
    };

    //**************************************************************************
    //
    // Member operators
    //
    //**************************************************************************

    /**
     * Subscripting operator for the vector, with bounds checking. All
     * subscripting operators return a new view of the same data as
     * the vector being subscripted. An object of type #RWRange or #RWToEnd,
     * the global object #RWAll, or a character string may be substituted
     * for an RWSlice.
     */
    T&     operator[](int i);          // With bounds checking

    /**
     * @copydoc operator[](int)
     */
    T      operator[](int i) const;    // With bounds checking

    /**
     * Subscripting operators for the vector, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file. All subscripting
     * operators return a new view of the same data as the vector being
     * subscripted. An object of type #RWRange or #RWToEnd, the global
     * object #RWAll, or a character string may be substituted for an
     * RWSlice.
     */
    T&     operator()(int i);          // With optional bounds checking

    /**
     * Subscripting operators for the vector, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file. All subscripting
     * operators return a new view of the same data as the vector being
     * subscripted. An object of type #RWRange or #RWToEnd, the global
     * object #RWAll, or a character string may be substituted for an
     * RWSlice.
     */
    T      operator()(int i) const;    // With optional bounds checking


    // With bounds checking
    /**
     * @copydoc operator[](int)
     */
    RWMathVec<T>       operator[](const RWSlice&);

    /**
     * @copydoc operator[](int)
     */
    const RWMathVec<T> operator[](const RWSlice&) const;

    // With optional bounds checking
    /**
     * Subscripting operators for the vector, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file. All subscripting
     * operators return a new view of the same data as the vector being
     * subscripted. An object of type #RWRange or #RWToEnd, the global
     * object #RWAll, or a character string may be substituted for an
     * RWSlice.
     */
    RWMathVec<T>       operator()(const RWSlice&);

    /**
     * Subscripting operators for the vector, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file. All subscripting
     * operators return a new view of the same data as the vector being
     * subscripted. An object of type #RWRange or #RWToEnd, the global
     * object #RWAll, or a character string may be substituted for an
     * RWSlice.
     */
    const RWMathVec<T> operator()(const RWSlice&) const;

    /**
     * Assignment operator for vectors. The vectors must have the same
     * dimensions.  The expression \f$ this = v; \f$
     * implies \f$ this_{i} = v_{i} \f$
     */
    RWMathVec<T>&  operator=(const RWMathVec<T>& v);

    /**
     * @copydoc operator=()
     */
    RWMathVec<T>&  operator=(const RWMathVecPick<T>& v);

    /**
     * Assignment operator for scalars.
     * The expression \f$ this = v; \f$
     * implies \f$ this_{i} = v \f$
     */
    RWMathVec<T>&  operator=(const T& v);

    /**
     * Assignment by addition operator for scalars.
     * The expression \f$ this += v; \f$
     * implies \f$ this_{i} = this_{i} + v \f$
     */
    RWMathVec<T>&  operator+=(const T& v);

    /**
     * Assignment by subtraction operator for scalars.
     * The expression \f$ this -= v; \f$
     * implies \f$ this_{i} = this_{i} - v \f$
     */
    RWMathVec<T>&  operator-=(const T& v);

    /**
     * Assignment by multiplication operator for scalars.
     * The expression \f$ this *= v; \f$
     * implies \f$ this_{i} = this_{i} * v \f$
     */
    RWMathVec<T>&  operator*=(const T& v);

    /**
     * Assignment by division operator for scalars.
     * The expression \f$ this /= v; \f$
     * implies \f$ this_{i} = this_{i} / v \f$
     */
    RWMathVec<T>&  operator/=(const T& v);

    /**
     * Assignment by addition operator for vectors. The vectors must have the
     * same dimensions. The expression \f$ this += v; \f$
     * implies \f$ this_{i} = this_{i} + v_{i} \f$
     */
    RWMathVec<T>&  operator+=(const RWMathVec<T>& v);

    /**
     * Assignment by subtraction operator for vectors. The vectors must have the
     * same dimensions. The expression \f$ this -= v; \f$
     * implies \f$ this_{i} = this_{i} - v_{i} \f$
     */
    RWMathVec<T>&  operator-=(const RWMathVec<T>& v);

    /**
     * Assignment by multiplication operator for vectors. The vectors must have
     * the same dimensions. The expression \f$ this *= v; \f$
     * implies \f$ this_{i} = this_{i} * v_{i} \f$
     */
    RWMathVec<T>&  operator*=(const RWMathVec<T>& v);

    /**
     * Assignment by division operator for vectors. The vectors must have the
     * same dimensions. The expression \f$ this /= v; \f$
     * implies \f$ this_{i} = this_{i} / v_{i} \f$
     */
    RWMathVec<T>&  operator/=(const RWMathVec<T>& v);

    /**
     * Increments each element of self. The functions
     * taking an integer parameter are invoked if the operator is used
     * as a postfix operator.
     */
    RWMathVec<T>&  operator++(); // Prefix operator

    /**
     * Returns \c true if self and the argument are equivalent.  To be
     * equivalent, they must have the same number of entries and each element in
     * self must equal the corresponding element in the argument.
     */
    bool      operator==(const RWMathVec<T>&) const;

    /**
     * Returns \c false if self and the argument are equivalent.  To be
     * equivalent, they must have the same number of entries and each element in
     * self must equal the corresponding element in the argument.
     */
    bool      operator!=(const RWMathVec<T>& v) const;

    /**
     * Decrements each element of self. The functions
     * taking an integer parameter are invoked if the operator is used
     * as a postfix operator.
     */
    RWMathVec<T>&  operator --(); // Prefix operator

    /**
     * @copydoc operator++()
     */
    void           operator++(int) {
        ++(*this);    // Postfix operator
    }

    /**
     * @copydoc operator--()
     */
    void           operator--(int) {
        --(*this);    // Postfix operator
    }

#if defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 1100
    // Suppress Intel warning:
    // warning #597 regarding operator() for T=DComplex will not be called
    // for implicit or explicit conversions
#  pragma warning push
#  pragma warning (disable:597)
#endif

    // Conversion operator for converting to rw_numeric_traits<T>::promote_type
#if !defined(RW_NO_CONVERSION_TO_SELF)
    RW_SUPPRESS_CONVERSION_OP_NEVER_CALLED_WARNING
    /**
     * Implicit conversion operator to \link RWMathVec RWMathVec<promote_type>
     * \endlink. The \c promote_type is rw_numeric_traits<T>::promote_type.
     * @see \link rw_numeric_traits rw_numeric_traits<T> \endlink for
     * more information.
     */
    operator RWMathVec<promote_type>() {
        RWMathVec<promote_type> tmp(length(), rwUninitialized);
        rw_math_transform(begin(), end(), tmp.begin(), RW_convert<T, promote_type>());
        return tmp;
    }
    RW_RESTORE_CONVERSION_OP_NEVER_CALLED_WARNING
#endif

#if defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 1100
#  pragma warning pop
#endif

    friend RWMathVec<double> RW_MATH_SYMBOLIC real(const RWMathVec<DComplex>& v);
    friend RWMathVec<double> RW_MATH_SYMBOLIC imag(const RWMathVec<DComplex>& v);
    // Doxygen Enhancement Request 648302
    // Disable friend declaration so DOXYGEN will associate this method with
    // RWMathArray
    // If we could override the friend relationship to document this method
    // on RWMathArray this macro code would be unnecessary.
#if !defined(DOXYGEN)
    friend RWMathVec<T> toVec RWMATH_FRIEND_TEMPLATE_SPECIFIER(const RWMathArray<T>&);
#endif

private:
    // For constructing new views
    inline RWMathVec(const RWDataView& v, T* b, size_t l, int s)
        : RWVecView(v, b, l, s) {
        ;
    }
};

/**
 * @ingroup math_collection_classes
 * @brief Converts between RWMathVec types with different datatypes.
 *
 * Class \link RWConvertMathVec RWConvertMathVec<From,To> \endlink
 * converts between RWMathVec types with different datatypes.
 *
 * @section synopsis Synopsis
 *
 * @code
 * template <class From, class To>
 * class RWConvertMathVec;
 * @endcode

 * @section example Example
 *
 * @code
 * #include <rw/math/mathvec.h>
 *
 * int main()
 * {
 *        // create a length 5 vector of 1's
 *   RWMathVec<int> d(5,1);
 *
 *        // convert int vector d to a DComplex
 *        // vector c with all real parts 1
 *   RWMathVec<DComplex> c = RWConvertMathVec<int,DComplex>(d);
 *
 *        // convert int vector d to a double vector
 *        //and then call sin
 *   RWMathVec<double> s = RWConvertMathVec<int,double>(d);
 *   s = sin(s);
 * }
 * @endcode
 *
 */
template <class From, class To>
struct RWConvertMathVec {
    /**
     * Constructs a new vector of type \link RWMathVec RWMathVec<To>
     * \endlink that can be used anywhere as a type \link RWMathVec
     * RWMathVec<To> \endlink. Note that a temporary array is created
     * every time this constructor is invoked, but the conversion operator
     * below can be used repeatedly without creating additional temporaries.
     */
    RWConvertMathVec(const RWMathVec<From>& v): to(v.length(), rwUninitialized) {
        rw_math_transform(v.begin(), v.end(), to.begin(), RW_convert<From, To>());
    }

    /**
     * Automatic conversion operator that enables class \link RWConvertMathVec
     * RWConvertMathVec<From,To> \endlink to be used as an \link RWMathVec
     * RWMathVec<To> \endlink. While the actual conversion takes place
     * in the constructor, this operator provides a reference to that
     * converted vector.
     */
    operator RWMathVec<To>() {
        return to;
    }

private:
    RWMathVec<To> to;
};

//*********************************************************************
// Global Operators
// ********************************************************************

/**
 * @relates RWMathVec
 * Unary minus operator, applied element-by-element.
 * For example, for vectors \c u, and \a v, the
 * expression \f$ u = -v \f$ implies \f$ u_{i} = -v_{i} \f$.
 */
template< class T > RWMathVec<T>
operator-(const RWMathVec<T>& v);

/**
 * @relates RWMathVec
 * Unary plus operator, applied element-by-element.
 * For example, for vectors \c u, and \a v, the
 * expression \f$ u = +v \f$ implies \f$ u_{i} = +v_{i} \f$.
 */
template< class T > inline RWMathVec<T>
operator+(const RWMathVec<T>& v)
{
    return v;
}

/**
 * @relates RWMathVec
 * Addition operator, applied element-by-element.
 * For example, for vectors \a u, \a v, and \c w, the
 * expression \f$ w = u + v \f$ implies \f$ w_{i}
 * = u_{i} + v_{i} \f$.
 */
template< class T > RWMathVec<T>
operator+(const RWMathVec<T>& u, const RWMathVec<T>& v);

/**
 * @relates RWMathVec
 * Subtraction operator, applied element-by-element.
 * For example, for vectors \a u, \a v, and \c w, the
 * expression \f$ w = u - v \f$ implies \f$ w_{i}
 * = u_{i} - v_{i} \f$.
 */
template< class T > RWMathVec<T>
operator-(const RWMathVec<T>& u, const RWMathVec<T>& v);

/**
 * @relates RWMathVec
 * Multiplication operator, applied element-by-element.
 * For example, for vectors \a u, \a v, and \c w, the
 * expression \f$ w = u * v \f$ implies \f$ w_{i}
 * = u_{i} * v_{i} \f$. This implies an element-by-element
 * multiply, \e not the inner product. If you want the inner product, use global
 * function dot() or conjDot().
 */
template< class T > RWMathVec<T>
operator*(const RWMathVec<T>& u, const RWMathVec<T>& v);

/**
 * @relates RWMathVec
 * Division operator, applied element-by-element.
 * For example, for vectors \a u, \a v, and \c w, the
 * expression \f$ w = u / v \f$ implies \f$ w_{i}
 * = u_{i} / v_{i} \f$.
 */
template< class T >  RWMathVec<T>
operator/(const RWMathVec<T>& u, const RWMathVec<T>& v);

/**
 * @relates RWMathVec
 * Addition operator, applied element-by-element.
 * For example, for vectors \a u, \c w, and scaler \a s, the
 * expression \f$ w = u + s \f$ implies \f$ w_{i}
 * = u_{i} + s \f$.
 */
template< class T > RWMathVec<T>
operator+(const RWMathVec<T>& u, const T& s);

/**
 * @relates RWMathVec
 * Subraction operator, applied element-by-element.
 * For example, for scaler \c s, and vectors \a v, and \c w, the
 * expression \f$ w = s - v \f$ implies \f$ w_{i}
 * = s - v_{i} \f$.
 */
template< class T > RWMathVec<T>
operator-(const T& s, const RWMathVec<T>& v);

/**
 * @relates RWMathVec
 * Multiplication operator, applied element-by-element.
 * For example, for vectors \a u, \c w, and scaler \a s, the
 * expression \f$ w = u * s \f$ implies \f$ w_{i}
 * = u_{i} * s \f$.
 */
template< class T > RWMathVec<T>
operator*(const RWMathVec<T>& u, const T& s);

/**
 * @relates RWMathVec
 * Division operator, applied element-by-element.
 * For example, for scaler \a s, and vectors \a v, and \c w, the
 * expression \f$ w = s / v \f$ implies \f$ w_{i}
 * = s / v_{i} \f$.
 */
template< class T > RWMathVec<T>
operator/(const T& s, const RWMathVec<T>& v);

/**
 * @relates RWMathVec
 * Outputs a vector \a v to \b std::ostream \a s beginning with a left
 * bracket \c [ and terminating with a right bracket \c ]. The output
 * numbers are separated by spaces. If \c T is a user-defined type,
 * the following operator must be defined for RWvostream and/or
 * RWFile:
 *
 * <tt>operator<<(ostream&s,constT&t); </tt>
 */
template< class T >
std::ostream& operator<< (std::ostream& s, const RWMathVec<T>& v);

/**
 * @relates RWMathVec
 * Reads vector \a v from stream \a s. It reads a sequence of numbers
 * separated by white space. If the sequence of numbers begins with
 * a left bracket \c [, the operator reads to a matching right bracket
 * \c ]. If no left bracket is present, it reads to end of file.
 * The vector \a v is initialized to the correct length. If \c T
 * is a user-defined type, the following operator must be defined
 * for RWvistream and/or RWFile:
 *
 * <tt>operator>>(ostream&s,constT&t);</tt>
 */
template< class T >
std::istream& operator>> (std::istream& s, RWMathVec<T>& v);

/**
 * @relates RWMathVec
 * Multiplication operator, applied element-by-element.
 * For example, for scaler \a s, and vectors \a v, and \c w, the
 * expression \f$ w = s * v \f$ implies \f$ w_{i}
 * = s * v_{i} \f$.
 */
template< class T >   RWMathVec<T>
operator*(const T& s, const RWMathVec<T>& v);

/**
 * @relates RWMathVec
 * Addition operator, applied element-by-element.
 * For example, for scaler \a s, and vectors \a v, and \c w, the
 * expression \f$ w = s + v \f$ implies \f$ w_{i}
 * = s + v_{i} \f$.
 */
template< class T >   RWMathVec<T>
operator+(const T& s, const RWMathVec<T>& v);

/**
 * @relates RWMathVec
 * Subtraction operator, applied element-by-element.
 * For example, for vectors \a u, \c w, and scaler \a s, the
 * expression \f$ w = u - s \f$ implies \f$ w_{i}
 * = u_{i} - s \f$.
 */
template< class T >   RWMathVec<T>
operator-(const RWMathVec<T>& u, const T& s);

/**
 * @relates RWMathVec
 * Division operator, applied element-by-element.
 * For example, for vectors \a u, \c w, and scaler \a s, the
 * expression \f$ w = u / s \f$ implies \f$ w_{i}
 * = u_{i} / s \f$.
 */
template< class T >   RWMathVec<T>
operator/(const RWMathVec<T>& u, const T& s);


//******************************************************************
//
// Global Functions
//
//******************************************************************


// Norm Functions-----------------------------------------------------------
//
// These functions are not templatized on type T because they potentially
// return a type different than T.  Furthermore, the type they return is
// dependent on the the type T, e.g. abs( RWMathVec<double> x ) returns
// a vector of doubles, while abs( RWMathVec<DComplex> x ) returns a double.
// Ideally, we would define the "norm type" for each type in a traits class
// and declare these functions as
// RWMathVec< traits<T>::norm_type > abs( const RWMathVec<T>& );
// Unfortunately, almost no current compilers understand this syntax.
// So for now we'll just do the non template solution.
//
//--------------------------------------------------------------------------

/**
 * @relates RWMathVec
 *
 * Returns the absolute values of each element.
 *
 * @section example Example
 *
 * @code
 * #include <rw/math/mathvec.h>
 * #include <iostream>
 * const double adata[] = {1.2,2.4,-1.2,0.8,-4.5};
 * int main() {
 *         RWMathVec<double> a(adata,5);
 *         RWMathVec<double> b = abs(a);
 *         std::cout << b;
 *         return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 * [ 1.2  2.4  1.2  0.8  4.5 ]
 * @endcode
 *
 */
RWMathVec<double>  RW_MATH_SYMBOLIC abs(const RWMathVec<double>&);

/**
 * @relates RWMathVec
 *
 * Returns the absolute values of each element.
 */
RWMathVec<float>   RW_MATH_SYMBOLIC abs(const RWMathVec<float>&);

/**
 * @relates RWMathVec
 *
 * Returns the absolute values of each element.
 */
RWMathVec<int>     RW_MATH_SYMBOLIC abs(const RWMathVec<int>&);

/**
 * @relates RWMathVec
 *
 * Returns the absolute values of each element.
 */
RWMathVec<double>  RW_MATH_SYMBOLIC abs(const RWMathVec<DComplex>&);

/**
 * @relates RWMathVec
 *
 *
 * Returns the absolute values of each element.
 *
 * @note The absolute value of a complex number is of type \c double. Therefore,
 * if abs() is invoked for class \link RWMathVec RWMathVec<DComplex>
 * \endlink, a vector of class \link RWMathVec RWMathVec<double>
 * \endlink is returned.
 */
RWMathVec<SChar>   RW_MATH_SYMBOLIC abs(const RWMathVec<SChar>&);

RWMathVec<double>  RW_MATH_SYMBOLIC norm(const RWMathVec<DComplex>&);


// Conversion Functions ---------------------------------------------

/**
 * @relates RWMathVec
 *
 * Converts an \link RWMathVec RWMathVec<double>\endlink instance into a
 * corresponding \link RWMathVec RWMathVec<float>\endlink instance. Note that
 * this is a narrowing operation; high order bits are removed.
 * @see RWConvertMathVec
 */
RWMathVec<float> RW_MATH_SYMBOLIC toFloat(const RWMathVec<double>& v);

/**
 * @relates RWMathVec
 *
 * Converts an \link RWMathVec RWMathVec<double>\endlink instance into
 * a corresponding \link RWMathVec RWMathVec<int>\endlink instance. Note that
 * truncation occurs.
 * @see RWConvertMathVec
 */
RWMathVec<int>   RW_MATH_SYMBOLIC toInt(const RWMathVec<double>& v);

/**
 * @relates RWMathVec
 *
 * Converts an \link RWMathVec RWMathVec<float>\endlink instance into
 * a corresponding \link RWMathVec RWMathVec<int>\endlink instance. Note that
 * truncation occurs.
 * @see RWConvertMathVec
 */
RWMathVec<int>   RW_MATH_SYMBOLIC toInt(const RWMathVec<float>& v);

/**
 * @relates RWMathVec
 *
 * Converts an \link RWMathVec RWMathVec<int>\endlink instance into a
 * corresponding \link RWMathVec RWMathVec<signed char>\endlink instance. Note
 * that this is a narrowing operation; high order bits are removed.
 * @see RWConvertMathVec
 */
RWMathVec<SChar> RW_MATH_SYMBOLIC toChar(const RWMathVec<int>& v);

// Miscellaneous Functions ------------------------------------------

/**
 * @relates RWMathVec
 *
 * Takes a vector \a x and returns a vector \e y of values
 * corresponding to the cumulative sum of \a x:
 *
 * \f[
 * y_i = \sum_{j=0}^{i} x_j
 * \f]
 *
 * @section example Example
 *
 * @code
 * #include <rw/math/mathvec.h>
 * #include <iostream>
 *
 * int main()
 * {
 *    const double adata[] = { 1.2, 2.4, -1.2, 0.8, 4.5 };
 *    RWMathVec<double> a(adata, 5);
 *    RWMathVec<double> b = cumsum(a);
 *    std::cout << b;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 * [ 1.2 3.6 2.4 3.2 7.7]
 * @endcode
 */
template< class T >
RWMathVec<T>  cumsum(const RWMathVec<T>& x);

/**
 * @relates RWMathVec
 *
 * Takes a vector \a x as an argument and returns a vector \e y of element
 * differences. The first element in \e y is set equal to the first element
 * of \a x. An element <i>y(i)</i> where \e i is not \c 0 is set equal to
 * \f$ x(i) - x(i-1) \f$.
 *
 * For example, if  <i>x = [2, 4, 5, 8]</i> then <i>y = [2, 2, 1, 3].</i>
 */
template< class T >
RWMathVec<T>  delta(const RWMathVec<T>& x);

/**
 * @relates RWMathVec
 *
 * Returns the vector dot product \e z of two vectors, \a x and \a y. The
 * two vectors must have the same number of elements or an exception with
 * value \c MATH_VNMATCH occurs.
 *
 * \f[
 * z = \sum_{j=0}^{N-1} x_j y_j
 * \f]
 *
 * For example, if <i>x = [ 0, 1, 2]</i> and <i>y = [ 1, 3, 2]</i>, then
 * <i>z = 0 + 3 + 4 = 7</i>. For vectors of type complex, see also function
 * conjDot().
 */
template< class T >
T  dot(const RWMathVec<T>& x, const RWMathVec<T>& y);

/**
 * @relates RWMathVec
 *
 * Return the \e index  of the maximum element of the vector.
 * If instead you want the maximum \e value use function maxValue().
 */
template< class T >
int  maxIndex(const RWMathVec<T>&); // Returns *index* of max value

/**
 * @relates RWMathVec
 *
 * Return the maximum value.
 */
template< class T >
T  maxValue(const RWMathVec<T>&);

/**
 * @relates RWMathVec
 *
 * Return the \e index of the minimum element of the vector.
 * If instead you want the minimum \e value use function minValue().
 */
template< class T >
int  minIndex(const RWMathVec<T>&); // Returns *index* of min value

/**
 * @relates RWMathVec
 *
 * Return the minimum value.
 */
template< class T >
T  minValue(const RWMathVec<T>&);

/**
 * @relates RWMathVec
 *
 * Takes a vector <i>x</i><SUB>i</SUB> with <i>n</i> elements as
 * an argument and returns the product of the elements of the vector.
 * For example, if <i>x = [1, 4, 3]</i>,<i> </i>then <i>y </i>=
 * 12.
 */
template< class T >
T  prod(const RWMathVec<T>&);

template< class T >
RWMathVec<T>  crossProduct(const RWMathVec<T>& x, const RWMathVec<T>& y);

/**
 * @relates RWMathVec
 *
 * Returns the outer product of two vectors. The outer product of
 * the vectors  \f$ x = (x_{1}, x_{2},\ldots ,
 * x_{n}) \f$ and \f$ y = (y_{1}, y_{2},\ldots , y_{m}) \f$
 * is the matrix <i>A</i> whose <i>ij</i>th entry is <i>x</i><i><SUB>i</SUB></i><i>y</i><i><SUB>j</SUB></i>:
 * that is, the <i>j</i>th column of <i>A</i> is <i>y</i><i><SUB>j</SUB></i><i>x</i>.
 */
template< class T >
RWGenMat<T>  outerProduct(const RWMathVec<T>& x, const RWMathVec<T>& y);

/**
 * @relates RWMathVec
 *
 * Takes a vector <i>x</i> as an argument and returns a vector <i>y</i>
 * whose elements are in reversed order. For example, if
 * <i>x = [2, 5, 1, 6]</i>, then <i>y = [6, 1, 5, 2]</i>. The reversed
 * vector <i>y</i> is actually a reversed <i>slice</i> of the original
 * vector. Hence, the results of this function can be used as an
 * lvalue.
 */
template< class T >
RWMathVec<T>  reverse(const RWMathVec<T>&);

/**
 * @relates RWMathVec
 *
 * Returns the sum of the elements of the vector. For example, if<i>
 * x = [1, 4, 3, 4]</i>, then <i>y = 12</i>.
 */
template< class T >
T  sum(const RWMathVec<T>&);

/**
 * @relates RWMathVec
 *
 * Takes \a x as an argument and returns \e y such that
 * \f$ y_{i} = cos(x_{i}) \f$. The
 * <i>x</i><SUB>i</SUB> are in radians. For complex arguments,
 * the complex cosine is returned.
 */
template< class T >
RWMathVec<T>  cos(const RWMathVec<T>& x);

/**
 * @relates RWMathVec
 *
 * Takes \a x as an argument and returns hyperbolic cosines
 * \e y such that \f$ y_{i} = cosh(x_{i}) \f$.
 * For complex arguments, the complex hyperbolic cosine is
 * returned.
 */
template< class T >
RWMathVec<T>  cosh(const RWMathVec<T>& x);

/**
 * @relates RWMathVec
 *
 * Takes an argument \a x and returns \e y such that:
 *
 * \f[
 * y_i = e^{x_i}
 * \f]
 *
 * If class \c T is complex, the complex exponential is returned.
 */
template< class T >
RWMathVec<T>  exp(const RWMathVec<T>& x);

/**
 * @relates RWMathVec
 *
 * Takes a RWMathVec an argument and returns <i>\<x\>,</i> the mean value,
 * where:
 *
 * \f[
 * \langle x \rangle = \frac{1}{n}\sum_{i=0}^{n-1}x_i
 * \f]
 *
 * For example, if <i>x = [1, 4, 3, 4]</i>, then <i>\<x\> = 3</i>.
 */
template< class T >
T  mean(const RWMathVec<T>& V);

/**
 * @relates RWMathVec
 *
 * The function pow() takes two arguments: pow( \a x, \a y ).
 *
 * Returns <i>z</i> such that:
 *
 * \f$ z_i = (x_i)^{y_i} \f$
 *
 * If the number of elements in \a x does not match the number
 * of elements in \a y,  an exception with value \c RWMATH_MNMATCH
 * will occur.
 */
template< class T >
RWMathVec<T> pow(const RWMathVec<T>& x, const RWMathVec<T>& y);

/**
 * @relates RWMathVec
 *
 * The function pow() takes two arguments: pow( \a x, \a y ).
 *
 * Returns <i>z</i> such that:
 *
 * \f$ z_i = (x_i)^{y} \f$
 */
template <class T>
RWMathVec<T> pow(const RWMathVec<T>& x, T y);

// internal helper overload for complex
template <class T>
RWMathVec<std::complex<T> > pow(const RWMathVec<std::complex<T> >& x, std::complex<T> y);

/**
 * @relates RWMathVec
 *
 * The function pow() takes two arguments: pow( \a x, \a y ).
 *
 * Returns <i>z</i> such that:
 *
 * \f$ z_i = (x)^{y_i} \f$
 */
template <class T>
RWMathVec<T>  pow(T x, const RWMathVec<T>& y);

// internal helper overload for complex
template <class T>
RWMathVec<std::complex<T> > pow(std::complex<T> x, const RWMathVec<std::complex<T> >& y);

/**
 * @relates RWMathVec
 *
 * The function sin() takes <i>x </i>as
 * an argument and returns <i>y </i>such that \f$ y_{i}
 * = sin(x_{i}) \f$. The <i>x</i><SUB>i</SUB> are in radians.
 * For complex classes, the complex sine is returned.
 */
template< class T >
RWMathVec<T>  sin(const RWMathVec<T>& V);

/**
 * @relates RWMathVec
 *
 * The function sinh() takes
 * <i>x</i> as an argument and returns <i>y</i> such that \f$ y_{i}
 * = sinh(x_{i}) \f$. For complex classes, the complex hyperbolic
 * sine is returned.
 */
template< class T >
RWMathVec<T>  sinh(const RWMathVec<T>& V);

/**
 * @relates RWMathVec
 *
 * The square root function. Takes <i>x</i> as an argument and returns
 * <i>y</i> such that
 * \f$ y_{i} = (x_{i})^{1/2} \f$.
 * For complex classes, the principal value of the complex square
 * root is returned.
 */
template< class T >
RWMathVec<T>  sqrt(const RWMathVec<T>& V);

template< class T, class OutputPointer >
void rwVariance(const RWMathVec<T>& vec, OutputPointer var);

/**
 * @relates RWMathVec
 *
 * Takes a vector \a x as an argument and returns its variance \e y as in:
 *
 * \f[
 * y=\frac{1}{N}\sum_{i=0}^{N-1}(x_i-<x>)^2
 * \f]
 *
 * where <\a x> is the mean of the vector and \e N is the length of the vector.
 * Note that this is the biased variance.
 */
double RW_MATH_SYMBOLIC variance(const RWMathVec<double>&);

/**
 * @relates RWMathVec
 * @copydoc variance
 */
double RW_MATH_SYMBOLIC variance(const RWMathVec<DComplex>&);

/**
 * @relates RWMathVec
 * @copydoc variance
 */
float RW_MATH_SYMBOLIC  variance(const RWMathVec<float>&);


// Functions for complex vector -----------------------------------------

/**
 * @relates RWMathVec
 *
 * Returns the arguments <i>a</i><SUB>i</SUB> (in radians) of \e x
 * in the range  -\f$\pi\f$<i> < a</i><SUB>i</SUB> \f$\leq \pi\f$,
 * where \f$ a_{i} =
 * tan_{-1}[Im(x_{i})/Re(x_{i})] \f$.
 * Multiple valued functions of complex arguments, such as sqrt()
 * and log(), are therefore constrained to their principle value.
 */
RWMathVec<double>   RW_MATH_SYMBOLIC arg(const RWMathVec<DComplex>& v);

inline const RWMathVec<float> RW_MATH_SYMBOLIC conj(const RWMathVec<float>& v)
{
    return v;
}
inline const RWMathVec<double> RW_MATH_SYMBOLIC conj(const RWMathVec<double>& v)
{
    return v;
}

/**
 * @relates RWMathVec
 *
 * Takes complex \a x as an argument and returns the complex
 * conjugates <i>x</i><SUP>*</SUP>. For example, if <i>x</i><SUB>i</SUB> =
 * (2.3, 1.4), then <i>x</i><SUB>i</SUB><SUP>*</SUP> = (2.3, -1.4).
 */
RWMathVec<DComplex> RW_MATH_SYMBOLIC conj(const RWMathVec<DComplex>& x);


/**
 * @relates RWMathVec
 *
 * Returns the complex-conjugate dot product of two vectors \a x and \a y:
 *
 * \f[
 * z = \sum_{j=0}^{N-1} x_j y_j
 * \f]
 *
 * The two vectors must have the same number of elements or an exception
 * with value \c MATH_VNMATCH occurs.
 */
DComplex RW_MATH_SYMBOLIC conjDot(const RWMathVec<DComplex>& x,
                                  const RWMathVec<DComplex>& y);

/**
 * @relates RWMathVec
 *
 * Takes complex <i>x</i> as an argument and returns <i>y</i> containing
 * the imaginary parts \f$ y_{i} = Im(x_{i}) \f$.
 * With most versions of complex, the results can be used as an
 * l-value:
 *
 * @code
 * RWMathVec<DComplex>  v(5,  0);        //  (0,  0),  (0,  0),  ...
 * imag(v)  =  1.0;                      //  (0,  1),  (0,  1),  ...
 * @endcode
 */
RWMathVec<double>   RW_MATH_SYMBOLIC imag(const RWMathVec<DComplex>& v);

/**
 * @relates RWMathVec
 *
 * Given a complex sequence \f$ C(n), n=0, \ldots , 2N \f$ points long,
 * it is a complex conjugate-even sequence if:
 *
 * \f$ C(n) = conj[C(-n)] = conj[C(2N - n)] \f$,
 *
 * Note that for a sequence to be complex conjugate-even, the imaginary
 * parts of \f$ C(0) \f$ and \f$ C(N) \f$ must be 0.
 * Given the lower half of the sequence,
 * that is, \f$ C(n), n=0, \ldots , N, \f$ this function expands it
 * out to the entire sequence \f$ C(n), n=0, ..., 2N \f$.
 */
RWMathVec<DComplex> RW_MATH_SYMBOLIC expandConjugateEven(const RWMathVec<DComplex>&);

/**
 * @relates RWMathVec
 *
 * Given a complex sequence \f$ C(n), n=0, \ldots, 2N \f$ points long,
 * it is a complex conjugate-odd sequence if:
 *
 * \f$ C(n) = - conj[C(-n)] = - conj[C(2N - n)] \f$.
 *
 * Note that for a sequence to be complex conjugate-odd, the real parts of
 * \f$ C(0) \f$ and \f$ C(N)\f$ must be 0.
 * Given the lower half of the sequence,
 * that is, \f$ C(n), n=0, \ldots , N, \f$ this function expands it
 * out to the entire sequence \f$ C(n), n=0, ..., 2N \f$.
 */
RWMathVec<DComplex> RW_MATH_SYMBOLIC expandConjugateOdd(const RWMathVec<DComplex>&);

/**
 * @relates RWMathVec
 *
 * Takes a complex argument <i>x</i> and returns real <i>y</i> containing
 * the real part \f$ y_{i}=Re(x_{i}) \f$. With most
 * versions of complex, the results can be used as an l-value:
 *
 * @code
 * RWMathVec<DComplex>  v(5,  0);        //  (0,  0),  (0,  0),  ...
 * real(v)  =  1.0;                      //  (1,  0),  (1,  0),  ...
 * @endcode
 */
RWMathVec<double>   RW_MATH_SYMBOLIC real(const RWMathVec<DComplex>& V);

/**
 * @relates RWMathVec
 * @brief Returns a complex vector <i>v</i> containing the complex
 * <i>N</i><SUP>th</SUP> roots of unity.
 *
 * Returns a complex vector <i>v</i> containing the complex
 * <i>N</i><SUP>th</SUP> roots of unity, that is:
 *
 * \f[
 * \begin{array}{l r}
 * v_k = e^{(2k\pi) / N} & \text{    k = 0, ..., nterms - 1}
 * \end{array}
 * \f]
 *
 * If rootsOfOne() is invoked with arguments \a N and \a nterms, only the
 * the first \a nterms of the \a N roots are returned in a vector of type
 * \link RWMathVec RWMathVec<DComplex> \endlink with \a nterms elements.
 */
RWMathVec<DComplex> RW_MATH_SYMBOLIC rootsOfOne(int N, size_t nterms);

/**
 * @relates RWMathVec
 * @brief Returns a complex vector <i>v</i> containing the complex
 * <i>N</i><SUP>th</SUP> roots of unity.
 *
 * Returns a complex vector <i>v</i> containing the complex
 * <i>N</i><SUP>th</SUP> roots of unity, that is:
 *
 * \f[
 * \begin{array}{l r}
 * v_k = e^{(2k\pi) / N} & \text{    k = 0, ..., nterms - 1}
 * \end{array}
 * \f]
 *
 * If rootsOfOne() is invoked with argument \a N, the
 * \a N roots are returned in a vector of type \link RWMathVec
 * RWMathVec<DComplex> \endlink with \a N elements.
 */
RWMathVec<DComplex> RW_MATH_SYMBOLIC rootsOfOne(int N);

/**
 * @relates RWMathVec
 *
 * Returns the spectral variance of a Fourier transformed series.
 * If \e v is a complex series containing the complex spectral
 * estimates, the spectral variance \e S is given by:
 *
 * \f[
 * \text{S} = \sum_{i=0}^{N-1}\left ( \text{Re}(v_{i})^2 + \text{Im}(v_{i})^2 \right )
 * \f]
 *
 * Note that the first term, the mean, is excluded from the sum.
 */
double              RW_MATH_SYMBOLIC spectralVariance(const RWMathVec<DComplex>&);


// Functions for real vectors -----------------------------------------

/**
 * @relates RWMathVec
 *
 * Returns arc cosines \e y such that \f$ y_{i} =
 * cos^{-1} (x_{i}) \f$. The <i>y</i><SUB>i</SUB>(in
 * radians) are in the range \f$ 0 < y_{i} \leq \pi \f$,
 * for elements <i>x</i><SUB>i</SUB>
 * with absolute values \f$ |x_{i}| \leq
 * 1 \f$.
 */
template< class T >
RWMathVec<T>  acos(const RWMathVec<T>& x);

/**
 * @relates RWMathVec
 *
 * Takes \a x as an argument and returns arc sines \e y such
 * that \f$ y_{i} = sin^{-1} (x_{i}) \f$.
 * The <i>y</i><SUB>i</SUB> (in radians) are in the range
 * -\f$ \pi/2 < y_{i} \leq \pi/2 \f$, for elements
 * <i>x</i><SUB>i</SUB> with absolute values \f$ |x_{i}|
 * \leq 1 \f$.
 */
template< class T >
RWMathVec<T>  asin(const RWMathVec<T>& x);

/**
 * @relates RWMathVec
 *
 * Takes \a x and returns \e y of arc
 * tangents (in radians), such that \f$ y_{i} =
 * tan^{-1}x_{i} \f$,
 * where \f$ -\pi/2 < y_{i} \leq \pi/2 \f$.
 *
 * At least one of the arguments <i>x</i><SUB>i</SUB> or <i>y</i><SUB>i</SUB>
 * must be nonzero.
 */
template< class T >
RWMathVec<T>  atan(const RWMathVec<T>& x);

/**
 * @relates RWMathVec
 *
 * Takes two arguments and returns quadrant
 * correct arc tangents \e z (in radians), such that \f$ z_{i}
 * = tan^{-1} ( x_{i}/y_{i}) \f$. For each
 * element \e i, the expression \f$ atan2(x_{i}, y_{i}) \f$ is
 * mathematically equivalent to:
 *
 * \f[
 * tan^{-1}\left(\frac{x_i}{y_i}\right)
 * \f]
 *
 * At least one of the arguments <i>x</i><SUB>i</SUB> or <i>y</i><SUB>i</SUB>
 * must be nonzero.
 */
template< class T >
RWMathVec<T>  atan2(const RWMathVec<T>& x, const RWMathVec<T>& y);

/**
 * @relates RWMathVec
 *
 * Takes \a x as an argument and returns \e y such that
 * <i>y</i><SUB>i</SUB> corresponds to the next integer greater
 * than or equal to <i>x</i><SUB>i</SUB>. For example,  if
 * <i>x = [ -1.3, 4.3, 7.9] </i>then <i>y = [ -1, 5, 8].</i>
 */
template< class T >
RWMathVec<T>  ceil(const RWMathVec<T>& x);

/**
 * @relates RWMathVec
 *
 * Given a real sequence \f$ V(n), n=0, \ldots , 2N \f$ points long,
 * it is an even sequence if:
 *
 * \f$ V(n) = V(-n) = V(2N - n) \f$,
 *
 * Given the lower half of an even sequence, that is, \f$ V(n), n=0, \ldots ,
 * N, \f$ expandEven() expands it out to the entire sequence \f$ V(n),
 * n=0, \ldots, 2N \f$.
 */
template< class T >
RWMathVec<T>  expandEven(const RWMathVec<T>&);

/**
 * @relates RWMathVec
 *
 * Given a real sequence \f$ V(n), n=0, \ldots , 2N \f$ points long,
 * it is an odd sequence if:
 *
 * \f$ V(n) = - V(-n) = - V(2N - n) \f$.
 *
 * Note that for a sequence to be odd, \f$ V(0) = V(N) = 0 \f$.
 * Given the lower half of an odd sequence, that
 * is, \f$ V(n), n=1, \ldots , N-1 \f$, expandOdd() expands it out to
 * the entire sequence \f$ V(n), n=0, \ldots , 2N \f$.
 */
template< class T >
RWMathVec<T>  expandOdd(const RWMathVec<T>&);

/**
 * @relates RWMathVec
 *
 * Takes <i>x</i> as an argument and returns <i>y</i> such that
 * <i>y</i><SUB>i</SUB> corresponds to the largest integer less
 * than or equal to <i>x</i><SUB>i</SUB>. For example, if <i>x =
 * [ 1.3, 4.4, -3.2]</i>, then <i>y = [ 1, 4, -4].</i>
 */
template< class T >
RWMathVec<T>  floor(const RWMathVec<T>& V);

/**
 * @relates RWMathVec
 *
 * Takes \a x as an argument and returns natural logarithms
 * <i>y</i> such that \f$ y_{i}=log(x_{i}) \f$.
 * The arguments \a x <SUB>i</SUB> must not be 0. For complex
 * arguments, the principal value of the complex natural logarithm
 * is returned.
 */
template< class T >
RWMathVec<T>  log(const RWMathVec<T>& x);

/**
 * @relates RWMathVec
 *
 * Takes \a x as an argument and returns base 10 logarithms
 * <i>y</i> such that \f$ y_{i} = log_{10}(x_{i}) \f$.
 * The arguments \a x <SUB>i</SUB> must not be 0.
 */
template< class T >
RWMathVec<T>  log10(const RWMathVec<T>& x);

/**
 * @relates RWMathVec
 *
 * The function tan() takes argument <i>x</i> and returns <i>y</i> such that
 * \f$ y_{i} = tan(x_{i}) \f$.
 */
template< class T >
RWMathVec<T>  tan(const RWMathVec<T>& V);

/**
 * @relates RWMathVec
 *
 * The function tanh() takes argument <i>x</i> and returns <i>y</i> such that
 * \f$ y_{i} = tanh(x_{i}) \f$.
 *
 */
template< class T >
RWMathVec<T>  tanh(const RWMathVec<T>& V);

/**
 * @relates RWMathVec
 *
 * Takes complex \a x as an argument and returns real \e y
 * containing the norm:
 *
 * \f$ y_{i} = [Re(x_{i})]^{2} + [Im(x_{i}]^{2} \f$
 */
RWMathVec<double>  RW_MATH_SYMBOLIC norm(const RWMathVec<DComplex>& x);

/**
 * @relates RWMathVec
 *
 * The function l2Norm() is the square root of the sum of the squares
 * of its entries:
 *
 * \f[
 * \left \| x \right \|_2 = \sqrt{ \sum_{i=0}^{N-1} \left (x_i \right )^2 }
 * \f]
 */
double RW_MATH_SYMBOLIC l2Norm(const RWMathVec<double>& v);

/**
 * @relates RWMathVec
 *
 * @copydoc l2Norm(const RWMathVec<double>&)
 */
float  RW_MATH_SYMBOLIC l2Norm(const RWMathVec<float>& v);

/**
 * @relates RWMathVec
 *
 * @copydoc l2Norm(const RWMathVec<double>&)
 */
double RW_MATH_SYMBOLIC l2Norm(const RWMathVec<DComplex>& v);


/**
 * @relates RWMathVec
 *
 * The function l1Norm() is the sum of absolute values of its entries:
 *
 * \f[
 * \left \| x \right \| = \sum_{i=0}^{N-1}\left | x_i \right |
 * \f]
 */
double RW_MATH_SYMBOLIC l1Norm(const RWMathVec<double>& v);

/**
 * @relates RWMathVec
 *
 * @copydoc l1Norm(const RWMathVec<double>&)
 */
float  RW_MATH_SYMBOLIC l1Norm(const RWMathVec<float>& v);

/**
 * @relates RWMathVec
 *
 * @copydoc l1Norm(const RWMathVec<double>&)
 */
double RW_MATH_SYMBOLIC l1Norm(const RWMathVec<DComplex>& v);

/**
 * @relates RWMathVec
 *
 * The function linfNorm() is the maximum of absolute values of its entries:
 *
 * \f[
 * \left \| x \right \|_{\infty} =
 * \begin{array}{c c}
 * \scriptstyle max \\ \scriptstyle  j=0,...,N-1
 * \end{array}
 * \left | x_i \right |
 * \f]
 */
double RW_MATH_SYMBOLIC linfNorm(const RWMathVec<double>& v);

/**
 * @relates RWMathVec
 *
 * @copydoc linfNorm(const RWMathVec<double>&)
 */
float  RW_MATH_SYMBOLIC linfNorm(const RWMathVec<float>& v);

/**
 * @relates RWMathVec
 *
 * @copydoc linfNorm(const RWMathVec<double>&)
 */
double RW_MATH_SYMBOLIC linfNorm(const RWMathVec<DComplex>& v);

/**
 * @relates RWMathVec
 *
 * Returns the value of the element with largest absolute value.
 * Note that this is not a norm in the mathematical sense of the word.
 */
double RW_MATH_SYMBOLIC maxNorm(const RWMathVec<double>& v);

/**
 * @relates RWMathVec
 *
 * @copydoc maxNorm(const RWMathVec<double>&)
 */
float  RW_MATH_SYMBOLIC maxNorm(const RWMathVec<float>& v);

/**
 * @relates RWMathVec
 *
 * @copydoc maxNorm(const RWMathVec<double>&)
 */
double RW_MATH_SYMBOLIC maxNorm(const RWMathVec<DComplex>& v);

/**
 * @relates RWMathVec
 *
 * Computes the Frobenius norm, which is the square root of the
 * sum of squares of its entries. For a vector, the formula is:\par
 *
 * \f[
 *  \left \| x \right \|_{\text{Frob}} =
 *  \sqrt{\sum_{i=0}^{N-1}\left | x_{i} \right |^2}
 * \f]
 *
 */
double RW_MATH_SYMBOLIC frobNorm(const RWMathVec<double>& v);

/**
 * @relates RWMathVec
 *
 * @copydoc frobNorm(const RWMathVec<double>&)
 */
float  RW_MATH_SYMBOLIC frobNorm(const RWMathVec<float>& v);

/**
 * @relates RWMathVec
 *
 * @copydoc frobNorm(const RWMathVec<double>&)
 */

double RW_MATH_SYMBOLIC frobNorm(const RWMathVec<DComplex>& v);



/***************************
 * Inline Access Functions *
 ***************************/
template< class T >
inline T& RWMathVec<T>::operator[](int i)
{
    boundsCheck(i);
    return data()[i * step];
}

template< class T >
inline T RWMathVec<T>::operator[](int i) const
{
    boundsCheck(i);
    return data()[i * step];
}

template< class T >
inline T& RWMathVec<T>::operator()(int i)
{
#ifdef RWBOUNDS_CHECK
    boundsCheck(i);
#endif
    return data()[i * step];
}

template< class T >
inline T RWMathVec<T>::operator()(int i) const
{
#ifdef RWBOUNDS_CHECK
    boundsCheck(i);
#endif
    return data()[i * step];
}

template< class T >
inline RWMathVec<T> RWMathVec<T>::operator[](const RWSlice& s)
{
    s.boundsCheck(static_cast<unsigned>(npts));
    return RWMathVec<T>(*this, data() + s.begin() * stride(), (size_t)s.len(int(npts)), stride() * s.stride());
}

template< class T >
inline const RWMathVec<T> RWMathVec<T>::operator[](const RWSlice& s) const
{
    s.boundsCheck(static_cast<unsigned>(npts));
    return RWMathVec<T>(*this, (T*)(data() + s.begin() * stride()), (size_t)s.len(int(npts)), stride() * s.stride());
}

template< class T >
inline RWMathVec<T> RWMathVec<T>::operator()(const RWSlice& s)
{
#ifdef RWBOUNDS_CHECK
    s.boundsCheck(static_cast<unsigned>(npts));
#endif
    return RWMathVec<T>(*this, data() + s.begin() * stride(), (size_t)s.len(int(npts)), stride() * s.stride());
}

template< class T >
inline const RWMathVec<T> RWMathVec<T>::operator()(const RWSlice& s) const
{
#ifdef RWBOUNDS_CHECK
    s.boundsCheck(static_cast<unsigned>(npts));
#endif
    return RWMathVec<T>(*this, (T*)(data() + s.begin() * stride()), (size_t)s.len(int(npts)), stride() * s.stride());
}

// Here is the definition of operator% for inner products.  Had to
// be postponed because dot() was not yet declared in the operator
// section.
template< class T >
inline T operator%(const RWMathVec<T>& x, const RWMathVec<T>& y)
{
    return dot(x, y);
}


#if defined(RW_TEMP_NO_STD_BASE) /* avoid preprocessor warning */
#  undef RW_TEMP_NO_STD_BASE
#endif


#if defined(_MSC_VER) && (_MSC_VER == 1500)
#  pragma warning(disable:4910)
#endif

RWMATH_INSTANTIATE(RW_MATH_GLOBAL, RWMathVec<DComplex>); // For the specialized constructor
RWMATH_INSTANTIATE(RW_MATH_GLOBAL, RWMathVec<int>); // avoid instantiating here and in Analytics

#if defined(_MSC_VER) && (_MSC_VER == 1500)
#  pragma warning(default:4910)
#endif

#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/math/mathvec.cc>
#endif

#endif  // __RWMATHVEC_H__
