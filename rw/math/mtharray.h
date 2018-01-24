#ifndef __RWMTHARRAY_H__
#define __RWMTHARRAY_H__

/***************************************************************************
 *
 * mtharray.h
 *
 * $Id: //math/13/rw/math/mtharray.h#2 $
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
 * cause bounds checking on the slice function, subscripting operators
 * and inlined math functions.
 *
 * Bounds checking is always done on subscripting operators returning
 * anything other than a T or T&.
 */

#include <rw/mathwind.h>

template <class T>
class RWMathArray;

#include <iterator>
#include <rw/array.h>
#include <rw/rand.h>
#include <rw/math/mathvec.h>
#include <rw/math/genmat.h>
#include <rw/math/numtrait.h>
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
template<class T> struct __promote_2 <RWMathArray<std::complex<T> >, T> { };

template<class T> struct __promote_2 <T, RWMathArray<std::complex<T> > > { };
}
#endif

/**
 * @ingroup math_collection_classes
 * @brief A base class for RWMathArray iterators.
 *
 * A base class from which the iterators for RWMathArray are derived.
 */
template <class T>
class RWMathArrayIteratorBase
{
public:
    /**
     * Typedef for the type of result from subtracting two iterators
     * obtained from this container.
     */
    typedef typename RWMathArray<T>::difference_type difference_type;

    /**
     * Typedef for the type of elements in this container.
     */
    typedef typename RWMathArray<T>::value_type value_type;

    typedef typename RWMathArray<T>::pointer pointer;

    /**
     * Typedef for a const pointer to a value in this container.
     */
    typedef typename RWMathArray<T>::const_pointer const_pointer;

    friend class RWMathArray<T>;

protected:
    RWMathArrayIteratorBase() {
        ;
    }

    RWMathArrayIteratorBase(T* p, const RWIntVec& size, const RWIntVec& stride):
        npts_(size),
        stride_(stride),
        state_(size.length(), 0),
        current_begin_(p),
        overflow_(0) {
        // First determine how long the initial contiguous chunks are.
        // Conglomerate as many initial indices as possible.
        if (npts_.length() == 0) { // empty array
            current_length_ = 0;
            current_stride_ = 1;
        }
        else {
            current_length_ = npts_(0);
            current_stride_ = stride_(0);
        }

        firstIndex_ = 1; // First index which it is necessary to loop over

        while (firstIndex_ < npts_.length() &&
                stride_(static_cast<int>(firstIndex_)) ==
                current_length_ * current_stride_) {
            current_length_ *= npts_(static_cast<int>(firstIndex_++));
        }

        current_ = current_begin_;
        current_end_ = current_begin_ + (current_stride_ * current_length_);
    }

#if defined(RW_MATH_REQUIRES_BASE_COPY_CONSTRUCTOR)
public:
#endif
    RWMathArrayIteratorBase(const RWMathArrayIteratorBase<T>& i):
        npts_(i.npts_),
        stride_(i.stride_),
        state_(i.state_.copy()),
        firstIndex_(i.firstIndex_),
        current_(i.current_),
        current_begin_(i.current_begin_),
        current_end_(i.current_end_),
        current_stride_(i.current_stride_),
        current_length_(i.current_length_),
        overflow_(i.overflow_) {
        ;
    }

protected:

    // Operators --------------------------------------------------------
    // Prefix increment and decrement operators

    inline RWMathArrayIteratorBase<T>& operator_plus_plus() {
        if ((current_ += current_stride_) == current_end_) {
            incrementState();
        }
        return *this;
    }


    inline RWMathArrayIteratorBase<T>& operator_minus_minus() {
        if (current_ == current_begin_) {
            decrementState(1);
            // now set current_ to last element of this new state
            current_ = current_end_ - current_stride_;
        }
        else {
            current_ -= current_stride_;
        }
        return *this;
    }

    // Assignment operators
    RWMathArrayIteratorBase<T>& operator_plus_equal(difference_type d) {
        if (d < 0) {
            return (this->operator_minus_equal(-d));
        }

        // first check simplest case
        current_ += d * current_stride_;
        if ((current_stride_ > 0 && current_ < current_end_)
                || (current_stride_ < 0 && current_ > current_end_)) {
            return *this;
        }
        else {
            // undo the addition above and find what the current index is
            current_ -= d * current_stride_;
            difference_type current_index = (current_ - current_begin_) / current_stride_;

            difference_type adjust = d % current_length_;
            d /= current_length_;

            // find new index, checking for overflow
            if ((current_index += adjust) >= current_length_) {
                current_index -= current_length_;
                ++d;
            }

            incrementState(static_cast<int>(d));

            // move current_ to current_index
            current_ = current_begin_ + current_index * current_stride_;
            return *this;
        }
    }

    RWMathArrayIteratorBase<T>& operator_minus_equal(difference_type d) {
        if (d < 0) {
            return (this->operator_plus_equal(-d));
        }

        // first check simplest case
        current_ -= d * current_stride_;
        if ((current_stride_ > 0 && current_ >= current_begin_)
                || (current_stride_ < 0 && current_ <= current_begin_)) {
            return *this;
        }
        else {
            // undo the subtraction above and find what the current index is
            current_ += d * current_stride_;
            difference_type current_index = (current_ - current_begin_) / current_stride_;

            difference_type adjust = d % current_length_;
            d /= current_length_;

            if ((current_index -= adjust) < 0) {
                current_index += current_length_;
                ++d;
            }

            decrementState(static_cast<int>(d));

            // move current_ to current_index
            current_ = current_begin_ + current_index * current_stride_;
            return *this;
        }
    }

    RWMathArrayIteratorBase<T>& operator =(const RWMathArrayIteratorBase<T>& x) {
        return this->operator_assignment(x), *this;
    }

    RWMathArrayIteratorBase<T>& operator_assignment(const RWMathArrayIteratorBase<T>& x) {
        if (&x != this) {
            npts_.reference(x.npts_);
            stride_.reference(x.stride_);
            state_.reference(x.state_.copy());

            firstIndex_ = x.firstIndex_;
            current_ = x.current_;
            current_begin_ = x.current_begin_;
            current_end_ = x.current_end_;
            current_stride_ = x.current_stride_;
            current_length_ = x.current_length_;
            overflow_ = x.overflow_;
        }

        return *this;
    }


    // Binary operators
    difference_type operator_minus(const RWMathArrayIteratorBase<T>& x) const {
        RWPRECONDITION(npts_.length() == x.npts_.length());

        difference_type current_index = (current_ - current_begin_) / current_stride_;
        difference_type x_index = (x.current_ - x.current_begin_) / x.current_stride_;

        difference_type diff = current_index - x_index;
        int len = current_length_;
        RWIntVec stateDiff = state_ - x.state_;
        for (unsigned index = static_cast<unsigned>(firstIndex_); index < static_cast<unsigned>(state_.length()); ++index) {
            diff += len * stateDiff(index);
            len *= npts_(index);
        }
        diff += len * (overflow_ - x.overflow_);

        return diff;
    }

    RWMathArrayIteratorBase<T> operator_minus(difference_type n) const {
        RWMathArrayIteratorBase<T> tmp = *this;
        return tmp.operator_minus_equal(n);
    }

    RWMathArrayIteratorBase<T> operator_plus(difference_type n) const {
        RWMathArrayIteratorBase<T> tmp = *this;
        return tmp.operator_plus_equal(n);
    }

    // Subscripting operator
    value_type& operator_square_brackets(difference_type n) const {
        return operator_plus(n).operator_star();
    }

    // Indirection operator
    value_type& operator_star() const {
        return *current_;
    }

    // Arrow operator
    pointer operator_arrow() const {
        return &(*current_);
    }

public:
    /**
     * Equality operator.  Returns \c true if the \link RWMathArrayIteratorBase
     * RWMathArrayIteratorBase<T>\endlink objects point to the same element.
     */
    bool operator ==(const RWMathArrayIteratorBase<T>& x) const {
        return (current_ == x.current_);
    }

    /**
     * Inequality operator.  Returns \c false if the \link RWMathArrayIteratorBase
     * RWMathArrayIteratorBase<T>\endlink objects point to the same element.
     */
    bool operator !=(const RWMathArrayIteratorBase<T>& x) const {
        return !(*this == x);;
    }

    /**
     * Less than operator.  Determines the relative logical position
     * between two \link RWMathArrayIteratorBase RWMathArrayIteratorBase<T>\endlink
     * objects, not necessarily the relative memory location of the elements to
     * which they point.
     */
    bool operator <(const RWMathArrayIteratorBase<T>& x) const {
        return !(*this >= x);
    }

    /**
     * Greater than operator.  Determines the relative logical position
     * between two \link RWMathArrayIteratorBase RWMathArrayIteratorBase<T>\endlink
     * objects, not necessarily the relative memory location of the elements to
     * which they point.
     */
    bool operator >(const RWMathArrayIteratorBase<T>& x) const {
        return !(*this <= x);
    }

    /**
     * Less than or equal to operator.  Determines the relative logical position
     * between two \link RWMathArrayIteratorBase RWMathArrayIteratorBase<T>\endlink
     * objects, not necessarily the relative memory location of the elements to
     * which they point.
     */
    bool operator <=(const RWMathArrayIteratorBase<T>& x) const {
        if (overflow_ != x.overflow_) {
            return overflow_ <= x.overflow_;
        }
        for (unsigned i = static_cast<unsigned>(state_.length() - 1); i >= static_cast<unsigned>(firstIndex_); --i) {
            if (state_(i) != x.state_(i)) {
                return (state_(i) < x.state_(i));
            }
        }
        // all state_ indices equal, compare current_
        difference_type diff = current_ - x.current_;
        return (current_stride_ > 0 && diff <= 0)
               || (current_stride_ < 0 && diff >= 0);
    }

    /**
     * Greater than or equal to operator.  Determines the relative logical
     * position between two \link RWMathArrayIteratorBase
     * RWMathArrayIteratorBase<T>\endlink objects, not necessarily the relative
     * memory location of the elements to which they point.
     */
    bool operator >=(const RWMathArrayIteratorBase<T>& x) const {
        if (overflow_ != x.overflow_) {
            return overflow_ >= x.overflow_;
        }
        for (unsigned i = static_cast<unsigned>(state_.length() - 1); i >= static_cast<unsigned>(firstIndex_); --i) {
            if (state_(i) != x.state_(i)) {
                return (state_(i) > x.state_(i));
            }
        }
        // all state_ indices equal, compare current_
        difference_type diff = current_ - x.current_;
        return (current_stride_ > 0 && diff >= 0)
               || (current_stride_ < 0 && diff <= 0);
    }


private:

    // incrementState() is the same as incrementState(1), but things have
    // been optimized a little.

    void incrementState() {
        // increment state_ by 1

        unsigned index = static_cast<unsigned>(firstIndex_);

        while (index < state_.length()) {
            // find new index
            current_begin_ += stride_(index);
            if (++state_(index) < npts_(index)) {
                break;
            }
            else {  // overflow
                state_(index) = 0;
                current_begin_ -= npts_(index) * stride_(index);
            }
            ++index;
        }

        // check for all indices overflowing
        if (index == static_cast<unsigned>(state_.length())) {

            ++overflow_;
            current_begin_ += npts_(index - 1) * stride_(index - 1);
        }

        current_ = current_begin_;
        current_end_ = current_begin_ + current_length_ * current_stride_;
    }

    // incrementState starts incrementing at firstIndex_.
    // The state_ member and the current_begin_, current_end_, and
    // current_ set of variables are updated.
    // current_ is set to current_begin.
    void incrementState(int d) {
        unsigned index = static_cast<unsigned>(firstIndex_);
        while (d != 0  &&  index < state_.length()) {
            int adjust = d % npts_(index);
            d /= npts_(index);

            // find new index, checking for overflow
            if (adjust + state_(index) >= npts_(index)) {
                adjust -= npts_(index);
                ++d;
            }

            state_(index) += adjust;
            current_begin_ += adjust * stride_(index);

            ++index;
        }
        // check for overflow...
        if (d != 0) {
            overflow_ += d;
            current_begin_ += d * npts_(index - 1) * stride_(index - 1);
        }

        current_ = current_begin_;
        current_end_ = current_begin_ + current_length_ * current_stride_;
    }

    // decrementState starts decrementing at firstIndex_.
    // The state_ member and the current_begin_, current_end_, and
    // current_ set of variables are updated.
    // current_ is set to current_begin.
    void decrementState(int d) {
        unsigned index = static_cast<unsigned>(firstIndex_);
        while (d != 0  &&  index < state_.length()) {
            int adjust = d % npts_(index);
            d /= npts_(index);

            // find new index, checking for overflow
            if (state_(index) - adjust < 0) {
                adjust -= npts_(index);
                ++d;
            }

            state_(index) -= adjust;
            current_begin_ -= adjust * stride_(index);

            ++index;
        }
        // check for overflow...
        if (d != 0) {
            overflow_ -= d;
            current_begin_ -= d * npts_(index - 1) * stride_(index - 1);
        }

        current_ = current_begin_;
        current_end_ = current_begin_ + current_length_ * current_stride_;
    }

    RWIntVec npts_;
    RWIntVec stride_;
    RWIntVec state_;
    size_t firstIndex_; // first uncollapsed dimension
    T* current_;
    T* current_begin_;
    T* current_end_;
    int current_stride_;
    int current_length_;
    int overflow_;
};

/**
 * @ingroup math_collection_classes
 * @brief The random access iterator for the
 * \link RWMathArray RWMathArray<T> \endlink collection class.
 *
 * Class \link RWMathArray RWMathArray<T> \endlink is the random
 * access iterator for the \link RWMathArray RWMathArray<T> \endlink
 * collection class. The RWMathArrayIterator class behaves like a pointer to
 * type T and is used to increment through elements of an \link
 * RWMathArray RWMathArray<T> \endlink.
 *
 * @section synopsis Synopsis
 *
 * @code
 * template <class T>
 * class RWMathArray<T>::iterator;
 * @endcode

 * @section example Example
 *
 * @code
 * #include <rw/math/mtharray.h>
 *
 * int main()
 * {
 *     RWMathArray<int> A(5,5,5,rwUninitialized);
 *     RWMathArray<int>::iterator i = A.begin();
 *     RWMathArray<int>::iterator stop = A.end();
 *
 *     // Assign a unique int value to each element of A
 *     for (int j=0; i != stop; ++i, ++j) *i = j;
 * }
 * @endcode
 *
 */
template <class T>
class RWMathArrayIterator
    : public RWMathArrayIteratorBase<T>,
  public std::iterator < std::random_access_iterator_tag,
      typename RWMathArray<T>::value_type,
      typename RWMathArray<T>::difference_type
      >
{
public:
    typedef typename RWMathArray<T>::difference_type difference_type;
    typedef typename RWMathArray<T>::value_type value_type;
    typedef typename RWMathArray<T>::pointer pointer;
    typedef typename RWMathArray<T>::const_pointer const_pointer;

    friend class RWMathArray<T>;

    /**
     * Constructs an iterator that is in an uninitialized state. The
     * iterator must be initialized before it is dereferenced.
     */
    RWMathArrayIterator() {
        ;
    }

    RWMathArrayIterator(T* p, const RWIntVec& size, const RWIntVec& stride):
        RWMathArrayIteratorBase<T>(p, size, stride) {
        ;
    }

    /**
     * Constructs an RWMathArrayIterator from another RWMathArrayIterator.
     * Initialized iterators can be obtained via the begin() and end() member
     * functions of \link RWMathArray RWMathArray<T> \endlink
     */
    RWMathArrayIterator(const RWMathArrayIterator<T>& x): RWMathArrayIteratorBase<T>(x) {
        ;
    }

    /**
     * Prefix or postfix increment or decrement operator. The RWMathArrayIterator
     * points to the next or previous element in the \link RWMathArray
     * RWMathArray<T> \endlink. No error condition is set if the iterator
     * goes past the bounds of the array. Compare to RWMathArray<T>::begin()
     * and RWMathArray<T>::end() to check that the iterator position
     * is valid.
     */
    inline RWMathArrayIterator<T>& operator ++() {
        this->operator_plus_plus();
        return *this;
    }

    /**
     * @copydoc operator++()
     */
    inline RWMathArrayIterator<T>& operator --() {
        this->operator_minus_minus();
        return *this;
    }

    /**
     * @copydoc operator++()
     */
    RWMathArrayIterator<T> operator++(int) {
        RWMathArrayIterator<T> tmp = *this;
        ++(*this);
        return tmp;
    }

    /**
     * @copydoc operator++()
     */
    RWMathArrayIterator<T> operator--(int) {
        RWMathArrayIterator<T> tmp = *this;
        --(*this);
        return tmp;
    }

    /**
     * Sets the RWMathArrayIterator state to be the same state as \a i.
     */
    RWMathArrayIterator<T>& operator =(const RWMathArrayIterator<T>& x);

    /**
     * Moves the RWMathArrayIterator forward or backward \a d elements in the
     * \link RWMathArray RWMathArray<T> \endlink. No error condition is set
     * if the iterator goes past the bounds of the array. Incrementing
     * by \a d and then decrementing by \a d returns the iterator to
     * its original position, even if doing so takes it past the bounds
     * of the array.
     */
    RWMathArrayIterator<T>& operator +=(difference_type d) {
        this->operator_plus_equal(d);
        return *this;
    }

    /**
     * @copydoc operator+=(difference_type)
     */
    RWMathArrayIterator<T>& operator -=(difference_type d) {
        this->operator_minus_equal(d);
        return *this;
    }

    /**
     * If the return value is positive, the RWMathArrayIterator is that many
     * elements past the iterator \a i. If the return value is negative, the
     * RWMathArrayIterator is that many elements before the iterator \a i.
     */
    difference_type operator -(const RWMathArrayIteratorBase<T>& i) const {
        return this->operator_minus(i);
    }

    /**
     * Returns an RWMathArrayIterator that is \a d elements past (or before) self.
     */
    RWMathArrayIterator<T> operator -(difference_type d) const {
        RWMathArrayIterator<T> tmp = *this;
        return tmp -= d;
    }

    /**
     * Returns an RWMathArrayIterator that is \a d elements past (or before) self.
     */
    RWMathArrayIterator<T> operator +(difference_type d) const {
        RWMathArrayIterator<T> tmp = *this;
        return tmp += d;
    }

    /**
     * Returns a reference to the element that is \c n elements after
     * self if \c n is positive, or \c n elements before self if \c
     * n is negative.
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
    pointer operator ->() const {
        return this->operator_arrow();
    }

    // Comparison operators are inherited
};

RW_SUPPRESS_OP_ASSIGN_SELF_CHECK_WARNING
template <class T>
inline RWMathArrayIterator<T>&
RWMathArrayIterator<T>::operator =(const RWMathArrayIterator<T>& x)
{
    this->operator_assignment(x);
    return *this;
}
RW_RESTORE_OP_ASSIGN_SELF_CHECK_WARNING

/**
 * @ingroup math_collection_classes
 * @brief The random access iterator for the
 * \link RWMathArray RWMathArray<T> \endlink collection class.
 *
 * Class \link RWMathArray RWMathArray<T> \endlink is the random
 * access iterator for the \link RWMathArray RWMathArray<T> \endlink
 * collection class. The RWMathArrayConstIterator class behaves like a
 * pointer to type T and is used to increment through elements of
 * an \link RWMathArray RWMathArray<T> \endlink. The RWMathArrayConstIterator
 * differs from a plain iterator in that the value in a dereferenced
 * RWMathArrayConstIterator may not be changed.
 *
 * @section synopsis Synopsis
 *
 * @code
 * template <class T>
 * class RWMathArray<T>::const_iterator;
 * @endcode

 * @section example Example
 *
 * @code
 * #include <rw/math/mtharray.h>
 *
 * int main()
 * {
 *     RWMathArray<int> A(5,5,5,1); // Create a 5x5x5 array of 1's
 *     RWMathArray<int>::const_iterator i = A.begin();
 *     RWMathArray<int>::const_iterator stop = A.end();

 *
 *                                  // Print each element of A
 *     while(i != stop) {
 *         cout << *i << endl;
 *         ++i;
 *     }
 * }
 *
 * @endcode
 *
 */
template <class T>
class RWMathArrayConstIterator
    : public RWMathArrayIteratorBase<T>,
  public std::iterator < std::random_access_iterator_tag,
      typename RWMathArray<T>::value_type,
      typename RWMathArray<T>::difference_type
      >
{
public:
    typedef typename RWMathArray<T>::difference_type difference_type;
    typedef typename RWMathArray<T>::value_type value_type;
    typedef typename RWMathArray<T>::pointer pointer;
    typedef typename RWMathArray<T>::const_pointer const_pointer;

    friend class RWMathArray<T>;

    /**
     * Constructs an iterator that is in an uninitialized state. The
     * iterator must be initialized before it is dereferenced.
     */
    RWMathArrayConstIterator() {
        ;
    }

    RWMathArrayConstIterator(T* p, const RWIntVec& size, const RWIntVec& stride):
        RWMathArrayIteratorBase<T>(p, size, stride) {
        ;
    }

    /**
     * Constructs a RWMathArrayConstIterator from another RWMathArrayIterator or
     * RWMathArrayConstIterator. Initialized RWMathArrayIterator and
     * RWMathArrayConstIterator instances
     * can be obtained via the RWMathArray<T>::begin() and RWMathArray<T>::end()
     * member functions of \link RWMathArray RWMathArray<T> \endlink.
     */
    RWMathArrayConstIterator(const RWMathArrayIteratorBase<T>& i):
        RWMathArrayIteratorBase<T>(i) {
        ;
    }


    /**
     * Prefix or postfix increment or decrement operator.
     * The RWMathArrayConstIterator
     * points to the next or previous element in the \link RWMathArray
     * RWMathArray<T> \endlink. No error condition is set if the
     * RWMathArrayConstIterator goes past the bounds of the array. Compare to
     * RWMathArray<T>::begin() and RWMathArray<T>::end() to check that
     * the RWMathArrayConstIterator position is valid.
     */
    inline RWMathArrayConstIterator& operator ++() {
        this->operator_plus_plus();
        return *this;
    }

    /**
     * @copydoc operator++()
     */
    inline RWMathArrayConstIterator& operator --() {
        this->operator_minus_minus();
        return *this;
    }

    /**
     * @copydoc operator++()
     */
    RWMathArrayConstIterator operator++(int) {
        RWMathArrayConstIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    /**
     * @copydoc operator++()
     */
    RWMathArrayConstIterator operator--(int) {
        RWMathArrayConstIterator tmp = *this;
        --(*this);
        return tmp;
    }

    /**
     * Sets the RWMathArrayConstIterator state to be the same state as \a i.
     */
    RWMathArrayConstIterator<T>& operator =(const RWMathArrayIteratorBase<T>& x);

    /**
     * The RWMathArrayConstIterator is moved forward or backward \a d elements
     * in the \link RWMathArray RWMathArray<T> \endlink. No error condition
     * is set if the RWMathArrayConstIterator goes past the bounds of the array.
     * Incrementing by \a d and then decrementing by \a d returns the
     * RWMathArrayConstIterator to its original position, even if doing so
     * takes it past the bounds of the array.
     */
    RWMathArrayConstIterator& operator +=(difference_type d) {
        this->operator_plus_equal(d);
        return *this;
    }

    /**
     * @copydoc operator+=(difference_type)
     */
    RWMathArrayConstIterator& operator -=(difference_type d) {
        this->operator_minus_equal(d);
        return *this;
    }

    /**
     * If the return value is positive, the RWMathArrayConstIterator is that
     * many elements past the RWMathArrayConstIterator \c i. If the return
     * value is negative, the RWMathArrayConstIterator is that many elements
     * before the RWMathArrayConstIterator \a i.
     */
    difference_type operator -(const RWMathArrayIteratorBase<T>& i) const {
        return this->operator_minus(i);
    }

    /**
     * Returns a RWMathArrayConstIterator that is \a d elements past (or before)
     * self.
     */
    RWMathArrayConstIterator operator -(difference_type d) const {
        RWMathArrayConstIterator tmp = *this;
        return tmp -= d;
    }

    /**
     * Returns a RWMathArrayConstIterator that is \a d elements past (or before)
     * self.
     */
    RWMathArrayConstIterator operator +(difference_type d) const {
        RWMathArrayConstIterator tmp = *this;
        return tmp += d;
    }

    /**
     * Returns a reference to the element that is \c n elements after
     * self if \c n is positive, or \c n elements before self if \c
     * n is negative.
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
    const_pointer operator ->() const {
        return this->operator_arrow();
    }

    // Comparison operators are inherited from RWMathArrayIteratorBase
};

RW_SUPPRESS_OP_ASSIGN_SELF_CHECK_WARNING
template <class T>
inline RWMathArrayConstIterator<T>&
RWMathArrayConstIterator<T>::operator =(const RWMathArrayIteratorBase<T>& x)
{
    this->operator_assignment(x);
    return *this;
}
RW_RESTORE_OP_ASSIGN_SELF_CHECK_WARNING


template <class T>
RWMathArrayIterator<T> operator+(const typename RWMathArrayIterator<T>::difference_type i, const RWMathArrayIterator<T>& it)
{
    return it + i;
}

template <class T>
RWMathArrayConstIterator<T> operator+(const typename RWMathArrayConstIterator<T>::difference_type i, const RWMathArrayConstIterator<T>& it)
{
    return it + i;
}


/**
 * @ingroup math_collection_classes
 * @brief A templatized arbitrary dimension array class.
 *
 * Class \link RWMathArray RWMathArray<T> \endlink is a templatized
 * arbitrary dimension array class.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/math/mtharray.h>
 * RWMathArray<T> array;
 * @endcode

 * @section example Example
 *
 * @code
 * #include <rw/math/mtharray.h>
 *
 * int main()
 * {
 *   RWMathArray<DComplex> A(3,3,3, DComplex(3,0));
 *   RWMathArray<DComplex> B(5,5,5,5,reUninitialized);
 *   // Set a plane of data to 5 + 2i
 *   A(RWAll,0,RWAll) = DComplex(5,2);
 * }
 * @endcode
 *
 *
 * @see \link RWConvertMathArray RWConvertMathArray<From,To>
 * \endlink
 *
 */
template <class T> class RWMathArray : public RWArrayView
{
public:

    //****************
    // Typedefs
    //****************

    // STL style typedefs ---------------------------------------------
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
    typedef T*          pointer;
    typedef const T* const_pointer;
    typedef T           value_type;

    /**
     * A type that provides a random-access iterator over the elements
     * in the container.
     */
    typedef RWMathArrayIterator<T> iterator;

    /**
     * A type that provides a \c const random-access iterator over the
     * elements in the container.
     */
    typedef RWMathArrayConstIterator<T> const_iterator;

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

    // Numeric traits typedefs ----------------------------------------
    /**
     * Typedef for the promotion type. For more information,
     * see rw_numeric_traits<T>::promote_type.
     */
    typedef typename rw_numeric_traits<T>::promote_type          promote_type;

    /**
     * Typedef for the usual return type of numerical <tt>norm</tt>-like
     * functions. For more information, see rw_numeric_traits<T>::norm_type.
     */
    typedef typename rw_numeric_traits<T>::norm_type             norm_type;

    /**
     * Typedef for the function pointer used in the method apply().
     * For more information, see rw_numeric_traits<T>::mathFunType.
     */
    typedef typename rw_numeric_traits<T>::mathFunType           mathFunType;

    /**
     * Typedef for the function pointer used in the method apply2().
     * For more information, see rw_numeric_traits<T>::mathFunType2.
     */
    typedef typename rw_numeric_traits<T>::mathFunType2          mathFunType2;

    /**
     * Returns an iterator pointing to the first element of self.
     */
    iterator begin() {
        if (length().length() == 0) { // empty array.
            return end();
        }
        return iterator(this->data(), this->length(), this->stride());
    }

    /**
     * Returns an iterator pointing to one element past the last element
     * of self.
     */
    iterator end() {
        iterator temp(this->data(), this->length(), this->stride());
        temp.overflow_ = 1;
        int wholeStride;
        if (temp.npts_.length() != 0) { // Not an empty array
            unsigned lastIndex = static_cast<unsigned>(temp.npts_.length() - 1);
            wholeStride = temp.stride_(lastIndex) * temp.npts_(lastIndex);
        }
        else {
            wholeStride = 0;
        }

        temp.current_begin_ += wholeStride;
        temp.current_end_   += wholeStride;
        temp.current_ = temp.current_begin_;

        return temp;
    }

    /**
     * @copydoc begin()
     */
    const_iterator begin() const {
        if (length().length() == 0) { // empty array.
            return end();
        }
        return const_iterator((T*)this->data(), this->length(), this->stride());
    }

    /**
     * @copydoc end()
     */
    const_iterator end() const {
        const_iterator temp((T*)this->data(), this->length(), this->stride());
        temp.overflow_ = 1;
        int wholeStride;
        if (temp.npts_.length() != 0) { // Not an empty array
            unsigned lastIndex = static_cast<unsigned>(temp.npts_.length() - 1);
            wholeStride = temp.stride_(lastIndex) * temp.npts_(lastIndex);
        }
        else {
            wholeStride = 0;
        }

        temp.current_begin_ += wholeStride;
        temp.current_end_   += wholeStride;
        temp.current_ = temp.current_begin_;

        return temp;
    }

    /**
     * @copydoc begin()
     */
    const_iterator cbegin() const {
        return begin();
    }

    /**
     * @copydoc end()
     */
    const_iterator cend() const {
        return end();
    }

    // ******************************************************************

#ifdef RW_NATIVE_EXTENDED
    typedef extended(*XmathFunType)(extended);
#endif

    /**
     * Constructs a 0-dimensional array, useful for declaring vectors
     * of arrays. Like any other array, this array can subsequently
     * be reshaped or resized (see member functions reshape() and resize()).
     * Note that since this is a 0-dimensional (not 0 length) array,
     * and since by definition a 0-dimensional array is a scalar, the
     * array has one element of data.
     */
    RWMathArray();

    /**
     * Constructs an uninitialized array with a specified size. The
     * constructor taking an integer vector is useful for constructing
     * arrays with more than four dimensions. The #RWUninitialized type
     * is an enumeration type with only one value, #rwUninitialized.
     * The #rwUninitialized argument is used to distinguish the last
     * dimension size from an initial value.
     */
    RWMathArray(const RWIntVec& n, RWUninitialized, Storage storage = COLUMN_MAJOR)
        : RWArrayView(new RWAllocBlock< T, RWDefaultAllocator<T> >(static_cast<unsigned>(prod(n))), n, storage) {
        ;
    }

    /**
     * @copydoc RWMathArray(const RWIntVec&,RWUninitialized,Storage)
     */
    RWMathArray(size_t m, size_t n, size_t o, RWUninitialized);

    /**
     * @copydoc RWMathArray(const RWIntVec&,RWUninitialized,Storage)
     */
    RWMathArray(size_t m, size_t n, size_t o, size_t p, RWUninitialized);

    /**
     * Constructs an array with a specified size initialized with random
     * numbers generated by \a r. The constructor taking an integer
     * vector is useful for constructing arrays with more than four
     * dimensions.
     */
    RWMathArray(const RWIntVec& vec, RWTRand<RWRandGenerator>& r, Storage s = COLUMN_MAJOR);

    /**
     * @copydoc RWMathArray(const RWIntVec&,RWTRand<RWRandGenerator>&,Storage)
     */
    RWMathArray(size_t m, size_t n, size_t o, RWTRand<RWRandGenerator>& r);

    /**
     * @copydoc RWMathArray(const RWIntVec&,RWTRand<RWRandGenerator>&,Storage)
     */
    RWMathArray(size_t m, size_t n, size_t o, size_t p, RWTRand<RWRandGenerator>& r);

    /**
     * Constructs an array with a specified size. Each element in the
     * array is initialized to \a val. The constructor taking an integer
     * vector is useful for constructing arrays with more than four
     * dimensions.
     */
    RWMathArray(const RWIntVec& vec, T val);

    /**
     * @copydoc RWMathArray(const RWIntVec&,T)
     */
    RWMathArray(size_t m, size_t n, size_t o, T val);

    /**
     * @copydoc RWMathArray(const RWIntVec&,T)
     */
    RWMathArray(size_t m, size_t n, size_t o, size_t p, T val);

    /**
     * Constructs an array from the null terminated character string
     * \a s. The format of the character string is the same as that
     * expected by the global operator operator>> described in this
     * entry.
     */
    RWMathArray(const char* s);

    /**
     * Copy constructor. The new array and the old array both view the
     * same data.
     */
    RWMathArray(const RWMathArray<T>& a)
        : RWArrayView(a) {
        ;
    }

    /**
     * Constructs an array with a specified size using the data in the
     * vector \a dat as initial data. A copy of \a dat is made. The
     * vector \a dat must have at least as many elements as the array.
     * The constructor taking an integer vector is useful for constructing
     * arrays with more than four dimensions.
     */
    RWMathArray(const T* dat, const RWIntVec& n);

    /**
     * @copydoc RWMathArray(const T*,const RWIntVec&)
     */
    RWMathArray(const T* dat, size_t m, size_t n, size_t o);

    /**
     * @copydoc RWMathArray(const T*,const RWIntVec&)
     */
    RWMathArray(const T* dat, size_t m, size_t n, size_t o, size_t p);

    /**
     * Constructs an array using the data in the vector \a vec. The array
     * is a new view of the same data as \a vec, so no copy of the data
     * is made. The vector \a vec must have the same number of elements
     * as the array. The constructor taking an integer vector is useful
     * for constructing arrays with more than four dimensions.
     */
    RWMathArray(const RWMathVec<T>& vec, const RWIntVec& n);

    /**
     * @copydoc RWMathArray(const RWMathVec<T>&,const RWIntVec&)
     */
    RWMathArray(const RWMathVec<T>& vec, size_t m, size_t n, size_t o);

    /**
     * @copydoc RWMathArray(const RWMathVec<T>&,const RWIntVec&)
     */
    RWMathArray(const RWMathVec<T>& vec, size_t m, size_t n, size_t o, size_t p);


    RWMathArray(RWBlock* block, const RWIntVec& vec);       // Use a custom block for the data
    RWMathArray(RWBlock* block, size_t, size_t, size_t);
    RWMathArray(RWBlock* block, size_t, size_t, size_t, size_t);

    /**
     * Constructs a 1-dimensional or 2-dimensional array from a vector
     * or matrix. The resulting array is an alternate view of the same
     * data. This constructor is most often used implicitly by the compiler
     * to pass vectors or matrices to subroutines written to operate
     * on arrays of arbitrary dimension. This allows you to write one
     * subroutine suitable for vectors, matrices, or arrays.
     */
    RWMathArray(const RWMathVec<T>&);

    /**
     * @copydoc RWMathArray(const RWMathVec<T>&)
     */
    RWMathArray(const RWGenMat<T>&);

    /**
     * Constructs a complex array from the double precision arrays \a
     * re and \a im, with the real part of the array equal to \a re
     * and the imaginary part equal to \a im. A new copy of the data
     * is made.
     */
    RWMathArray(const RWMathArray<double>& re, const RWMathArray<double>& im);

public:
    // had to put apply inline so that Borland Builder 4 could find the
    // apply method
    /**
     * Returns the result of applying the passed function to every element
     * in the array. A function of type RWMathArray<T>::mathFunType
     * takes and returns a \c T.
     */
    RWMathArray<T>            apply(mathFunType f) const {
        RWMathArray<T> temp(npts, rwUninitialized, RWDataView::COLUMN_MAJOR);
        register T* dp = temp.data();
        for (ArrayLooper l(npts, step); l; ++l) {
            register const T* sp = data() + l.start;
            RW_ASSERT(l.length <= RW_INT_MAX);
            register int i = static_cast<int>(l.length);
            register int j = l.stride;
            while (i--) {
                *dp++ = (*f)(*sp);
                sp += j;
            }
        }
        return temp;
    }

    // Some compilers (Borland 4.5) only understand functions
    // templatized on a traits type if they are defined in line
    // at class scope.
#ifdef RW_INLINE_TRAITS_TEMPLATES
    RWMathArray<norm_type> apply2(mathFunType2 f) const {
        RWMathArray<norm_type> temp(npts, rwUninitialized);
        typename RWMathArray<norm_type>::iterator dst = temp.begin();
        typename RWMathArray<T>::const_iterator src = this->begin();
        typename RWMathArray<T>::const_iterator end = this->end();

        for (; src != end; ++src, ++dst) {
            *dst = (*f)(*src);
        }
        return temp;
    }
#else
    /**
     * Returns the result of applying the passed function to every element
     * in the array. The function of type RWMathArray<T>::mathFunType2
     * takes a \c T and returns an RWMathArray<T>::norm_type. See class
     * rw_numeric_traits for a description of RWMathArray<T>::norm_type.
     */
    RWMathArray<norm_type>    apply2(mathFunType2 f) const;
#endif

#ifdef RW_NATIVE_EXTENDED
    RWMathArray<T>            apply(XmathFunType f) const;
#endif

    /**
     * Returns the number of bytes required to store the array to an
     * RWFile using member function
     * \link saveOn(RWFile&) const saveOn(RWFile&)\endlink.
     */
    size_t      binaryStoreSize() const;

    /**
     * Returns a copy with distinct instance variables.
     */
    RWMathArray<T>     copy() const;

    /**
     * Returns a pointer to the start of an array's data. Should be
     * used with care, as this accesses the array's data directly.
     */
    T*          data() {
        return (T*)begin_;
    }

    /**
     * @copydoc data()
     */
    const T*    data() const {
        return (T*)begin_;
    }

    /**
     * Alias for copy().
     */
    RWMathArray<T>           deepCopy() const;

    /**
     * When invoked for an array, guarantees that there is only one
     * reference to that object and that its data are in contiguous
     * memory.
     */
    void          deepenShallowCopy();

    /**
     * Returns the number of dimensions of the array.
     */
    size_t      dimension() const {
        return RWArrayView::dimension();
    }

    /**
     * Returns the number of entries in a dimension of the array, returning
     * all the dimension lengths at once.
     */
    RWIntVec    length() const {
        return RWArrayView::length();
    }

    /**
     * Returns the size of the indicated dimension.
     */
    int         length(int i) const {
        return RWArrayView::length(i);
    }

    void          printOn(std::ostream& s) const;   // Pretty print

    /**
     * Returns an iterator pointing to the last element of self.
     */
    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    /**
     * Returns an iterator pointing to one element past the first element
     * of self.
     */
    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    /**
     * @copydoc rbegin()
     */
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    /**
     * @copydoc rend()
     */
    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    /**
     * @copydoc rbegin()
     */
    const_reverse_iterator crbegin() const {
        return rbegin();
    }

    /**
     * @copydoc rend()
     */
    const_reverse_iterator crend() const {
        return rend();
    }

    /**
     * Makes self a view of data in \a v. The view currently associated
     * with the array is lost.
     */
    RWMathArray<T>&          reference(const RWMathArray<T>& v);

    /**
     * Changes the size of the array, adding 0s or truncating as necessary.
     */
    void          resize(const RWIntVec& v, Storage storage = COLUMN_MAJOR);

    /**
     * @copydoc resize(const RWIntVec&,Storage)
     */
    void          resize(size_t m, size_t n, size_t o, Storage storage = COLUMN_MAJOR);

    /**
     * @copydoc resize(const RWIntVec&,Storage)
     */
    void          resize(size_t m, size_t n, size_t o, size_t p, Storage storage = COLUMN_MAJOR);

    /**
     * Changes the size of the array. After reshaping, the contents
     * of the array are undefined; that is, they can be and probably
     * will be garbage.
     */
    void          reshape(const RWIntVec& v, Storage storage = COLUMN_MAJOR);

    /**
     * @copydoc reshape(const RWIntVec&, Storage)
     */
    void          reshape(size_t m, size_t n, size_t o, Storage storage = COLUMN_MAJOR);

    /**
     * @copydoc reshape(const RWIntVec&, Storage)
     */
    void          reshape(size_t m, size_t n, size_t o, size_t p, Storage storage = COLUMN_MAJOR);

    /**
     * Restores self from a RWFile. To use this function with a user-defined
     * type, the corresponding \c operator>> must be defined:
     *
     * @code
     * RWFile&      operator>>(RWFile&, T&);
     * @endcode
     */
    void          restoreFrom(RWFile&);

    /**
     * Restores self from a virtual stream. To use this function with a
     * user-defined type, the corresponding \c operator>> must be defined:
     *
     * @code
     * RWvistream&  operator>>(RWvistream& T&);
     * @endcode
     */
    void          restoreFrom(RWvistream&);

    /**
     * Stores self in a binary format to an RWFile. To use these functions with a
     * user-defined type, the corresponding \c operator<< must be defined:
     *
     * @code
     * RWFile&      operator<<(RWFile&, T&);
     * @endcode
     */
    void          saveOn(RWFile&) const;

    /**
     * Stores self to a RWvostream. To use these functions
     * with a user-defined type, the corresponding \c operator<< must be
     * defined:
     *
     * @code
     * RWvostream&  operator<<(RWvostream& T&);
     * @endcode
     */
    void          saveOn(RWvostream&) const;

    void          scanFrom(std::istream& s);       // Read to eof or delimit with []

    /**
     * Returns an array that views a slice of the array. The slice begins
     * at element \a start and is of size \a lgt. The increment between
     * successive elements in the slice's \c j<SUP>th</SUP> dimension is given
     * by the \c j<SUP>th</SUP> column of the matrix \a strider.  For
     * 3-dimensional and 4-dimensional arrays, most useful slices can be more
     * simply constructed using subscripting.
     */
    RWMathArray<T> slice(const RWIntVec& start, const RWIntVec& lgt, const RWGenMat<int>& strider) const;

    //  RWIntVec      stride() const    {return step;}
    //  int           stride(int i) const    {return step(i);}

    /**
     * Subscripting operator for the array, with bounds checking. Returns
     * a reference to the element indexed by \a i.
     */
    inline    T&            operator[](const RWIntVec& i);

    /**
     * Subscripting operator for the array, with bounds checking. Returns
     * the element indexed by \a i.
     */
    inline    T             operator[](const RWIntVec& i) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file. Returns a reference
     * to the element indexed by \a i.
     */
    inline    T&            operator()(const RWIntVec& i);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file. Returns the
     * element indexed by \a i.
     */
    inline    T             operator()(const RWIntVec& i) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file. Returns a reference
     * to the element at (\a i <sup>th</sup>, \a j <sup>th</sup>, \a k
     * <sup>th</sup>) position of the array.
     */
    inline    T&            operator()(int i, int j, int k);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file. Returns the element
     * at (\a i <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>) position
     * of the array.
     */
    inline    T             operator()(int i, int j, int k) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file. Returns a reference
     * to the element at (\a i <sup>th</sup>, \a j <sup>th</sup>, \a k
     * <sup>th</sup>, \a l <sup>th</sup>) position of the array.
     */
    inline    T&            operator()(int i, int j, int k, int l);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file. Returns the element
     * at (\a i <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array.
     */
    inline    T             operator()(int i, int j, int k, int l) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWMathVec that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>) position
     * of the array. The RWMathVec being returned is a new view of the same
     * data as the array being subscripted.
     */
    RWMathVec<T>          operator()(const RWSlice& i, int j, int k);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWMathVec that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>) position
     * of the array. The RWMathVec being returned is a new view of the same
     * data as the array being subscripted.
     */
    const RWMathVec<T>    operator()(const RWSlice& i, int j, int k) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWMathVec that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>) position
     * of the array. The RWMathVec being returned is a new view of the same
     * data as the array being subscripted.
     */
    RWMathVec<T>          operator()(int i, const RWSlice& j, int k);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWMathVec that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>) position
     * of the array. The RWMathVec being returned is a new view of the same
     * data as the array being subscripted.
     */
    const RWMathVec<T>    operator()(int i, const RWSlice& j, int k) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWMathVec that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>) position
     * of the array. The RWMathVec being returned is a new view of the same
     * data as the array being subscripted.
     */
    RWMathVec<T>          operator()(int i, int j, const RWSlice& k);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWMathVec that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>) position
     * of the array. The RWMathVec being returned is a new view of the same
     * data as the array being subscripted.
     */
    const RWMathVec<T>    operator()(int i, int j, const RWSlice& k) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWMathVec that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWMathVec being returned
     * is a new view of the same data as the array being subscripted.
     */
    RWMathVec<T>          operator()(const RWSlice& i, int j, int k, int l);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWMathVec that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWMathVec being returned
     * is a new view of the same data as the array being subscripted.
     */
    const RWMathVec<T>    operator()(const RWSlice& i, int j, int k, int l) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWMathVec that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWMathVec being returned
     * is a new view of the same data as the array being subscripted.
     */
    RWMathVec<T>          operator()(int i, const RWSlice& j, int k, int l);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWMathVec that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWMathVec being returned
     * is a new view of the same data as the array being subscripted.
     */
    const RWMathVec<T>    operator()(int i, const RWSlice& j, int k, int l) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWMathVec that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWMathVec being returned
     * is a new view of the same data as the array being subscripted.
     */
    RWMathVec<T>          operator()(int i, int j, const RWSlice& k, int l);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWMathVec that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWMathVec being returned
     * is a new view of the same data as the array being subscripted.
     */
    const RWMathVec<T>    operator()(int i, int j, const RWSlice& k, int l) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWMathVec that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWMathVec being returned
     * is a new view of the same data as the array being subscripted.
     */
    RWMathVec<T>          operator()(int i, int j, int k, const RWSlice& l);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWMathVec that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWMathVec being returned
     * is a new view of the same data as the array being subscripted.
     */
    const RWMathVec<T>    operator()(int i, int j, int k, const RWSlice& l) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWGenMat that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>) position
     * of the array. The RWGenMat being returned is a new view of the same
     * data as the array being subscripted.
     */
    RWGenMat<T>       operator()(int i, const RWSlice& j, const RWSlice& k);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWGenMat that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>) position
     * of the array. The RWGenMat being returned is a new view of the same
     * data as the array being subscripted.
     */
    const RWGenMat<T> operator()(int i, const RWSlice& j, const RWSlice& k) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWGenMat that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>) position
     * of the array. The RWGenMat being returned is a new view of the same
     * data as the array being subscripted.
     */
    RWGenMat<T>       operator()(const RWSlice& i, int j, const RWSlice& k);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWGenMat that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>) position
     * of the array. The RWGenMat being returned is a new view of the same
     * data as the array being subscripted.
     */
    const RWGenMat<T> operator()(const RWSlice& i, int j, const RWSlice& k) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWGenMat that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>) position
     * of the array. The RWGenMat being returned is a new view of the same
     * data as the array being subscripted.
     */
    RWGenMat<T>       operator()(const RWSlice& i, const RWSlice& j, int k);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWGenMat that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>) position
     * of the array. The RWGenMat being returned is a new view of the same
     * data as the array being subscripted.
     */
    const RWGenMat<T> operator()(const RWSlice& i, const RWSlice& j, int k) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWGenMat that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWGenMat being returned
     * is a new view of the same data as the array being subscripted.
     */
    RWGenMat<T>       operator()(int i, int j, const RWSlice& k, const RWSlice& l);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWGenMat that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWGenMat being returned
     * is a new view of the same data as the array being subscripted.
     */
    const RWGenMat<T> operator()(int i, int j, const RWSlice& k, const RWSlice& l) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWGenMat that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWGenMat being returned
     * is a new view of the same data as the array being subscripted.
     */
    RWGenMat<T>       operator()(int i, const RWSlice& j, int k, const RWSlice& l);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWGenMat that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWGenMat being returned
     * is a new view of the same data as the array being subscripted.
     */
    const RWGenMat<T> operator()(int i, const RWSlice& j, int k, const RWSlice& l) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWGenMat that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWGenMat being returned
     * is a new view of the same data as the array being subscripted.
     */
    RWGenMat<T>       operator()(int i, const RWSlice& j, const RWSlice& k, int l);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWGenMat that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWGenMat being returned
     * is a new view of the same data as the array being subscripted.
     */
    const RWGenMat<T> operator()(int i, const RWSlice& j, const RWSlice& k, int l) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWGenMat that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWGenMat being returned
     * is a new view of the same data as the array being subscripted.
     */
    RWGenMat<T>       operator()(const RWSlice& i, int j, int k, const RWSlice& l);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWGenMat that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWGenMat being returned
     * is a new view of the same data as the array being subscripted.
     */
    const RWGenMat<T> operator()(const RWSlice& i, int j, int k, const RWSlice& l) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWGenMat that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWGenMat being returned
     * is a new view of the same data as the array being subscripted.
     */
    RWGenMat<T>       operator()(const RWSlice& i, int j, const RWSlice& k, int l);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWGenMat that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWGenMat being returned
     * is a new view of the same data as the array being subscripted.
     */
    const RWGenMat<T> operator()(const RWSlice& i, int j, const RWSlice& k, int l) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWGenMat that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWGenMat being returned
     * is a new view of the same data as the array being subscripted.
     */
    RWGenMat<T>       operator()(const RWSlice& i, const RWSlice& j, int k, int l);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWGenMat that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWGenMat being returned
     * is a new view of the same data as the array being subscripted.
     */
    const RWGenMat<T> operator()(const RWSlice& i, const RWSlice& j, int k, int l) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWMathArray that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>) position
     * of the array. The RWMathArray being returned is a new view of the same
     * data as the array being subscripted.
     */
    RWMathArray<T>    operator()(const RWSlice& i, const RWSlice& j, const RWSlice& k);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWMathArray that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>) position
     * of the array. The RWMathArray being returned is a new view of the same
     * data as the array being subscripted.
     */
    const RWMathArray<T> operator()(const RWSlice& i, const RWSlice& j, const RWSlice& k) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWMathArray that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWMathArray being returned
     * is a new view of the same data as the array being subscripted.
     */
    RWMathArray<T>    operator()(int i, const RWSlice& j, const RWSlice& k, const RWSlice& l);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWMathArray that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWMathArray being returned
     * is a new view of the same data as the array being subscripted.
     */
    const RWMathArray<T> operator()(int i, const RWSlice& j, const RWSlice& k, const RWSlice& l) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWMathArray that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWMathArray being returned
     * is a new view of the same data as the array being subscripted.
     */
    RWMathArray<T>    operator()(const RWSlice& i, int j, const RWSlice& k, const RWSlice& l);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWMathArray that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWMathArray being returned
     * is a new view of the same data as the array being subscripted.
     */
    const RWMathArray<T>  operator()(const RWSlice& i, int j, const RWSlice& k, const RWSlice& l) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWMathArray that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWMathArray being returned
     * is a new view of the same data as the array being subscripted.
     */
    RWMathArray<T>    operator()(const RWSlice& i, const RWSlice& j, int k, const RWSlice& l);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWMathArray that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWMathArray being returned
     * is a new view of the same data as the array being subscripted.
     */
    const RWMathArray<T>  operator()(const RWSlice& i, const RWSlice& j, int k, const RWSlice& l) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWMathArray that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWMathArray being returned
     * is a new view of the same data as the array being subscripted.
     */
    RWMathArray<T>    operator()(const RWSlice& i, const RWSlice& j, const RWSlice& k, int l);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWMathArray that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWMathArray being returned
     * is a new view of the same data as the array being subscripted.
     */
    const RWMathArray<T>  operator()(const RWSlice& i, const RWSlice& j, const RWSlice& k, int l) const;

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a non-const RWMathArray that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWMathArray being returned
     * is a new view of the same data as the array being subscripted.
     */
    RWMathArray<T>    operator()(const RWSlice& i, const RWSlice& j, const RWSlice& k, const RWSlice& l);

    /**
     * Subscripting operator for the array, with optional bounds checking.
     * Bounds checking is enabled by defining the preprocessor macro
     * \c RWBOUNDS_CHECK before including the header file.
     * Returns a const RWMathArray that contains the elements at (\a i
     * <sup>th</sup>, \a j <sup>th</sup>, \a k <sup>th</sup>, \a l
     * <sup>th</sup>) position of the array. The RWMathArray being returned
     * is a new view of the same data as the array being subscripted.
     */
    const RWMathArray<T>  operator()(const RWSlice& i, const RWSlice& j, const RWSlice& k, const RWSlice& l) const;

    /**
     * Assignment operator with conventional meaning.  The expression
     * \f$ u = v; \f$ implies \f$ u_{i} = v_{i} \f$
     * The RWMathArray objects must \e conform, that is, have the same dimensions.
     */
    RWMathArray<T>&        operator=(const RWMathArray<T>& v);

    /**
     * Assignment operator with conventional meaning. The expression
     * \f$ u = s; \f$ implies \f$ u_{i} = s \f$.
     */
    RWMathArray<T>&        operator=(const T& s);

    /**
     * Assignment by addition operator with conventional meaning.  The expression
     * \f$ u += s; \f$ implies \f$ u_{i} = u_{i} + s \f$
     */
    RWMathArray<T>&        operator+=(const T& s);

    /**
     * Assignment by subtraction operator with conventional meaning.  The
     * expression \f$ u -= v; \f$ implies \f$ u_{i} = u_{i} - s \f$
     */
    RWMathArray<T>&        operator-=(const T& s) {
#ifdef RW_CONST_INCORRECT_COMPLEX
        T tmp(s);
        return operator+=(-tmp);
#else
        return operator+=(static_cast<T>(-s));
#endif
    }

    /**
     * Assignment by multiplication operator with conventional meaning.  The
     * expression \f$ u *= v; \f$ implies \f$ u_{i} = u_{i} * s \f$
     */
    RWMathArray<T>&        operator*=(const T& s);

    /**
     * Assignment by division operator with conventional meaning.  The expression
     * \f$ u /= v; \f$ implies \f$ u_{i} = u_{i} / s \f$
     */
    RWMathArray<T>&        operator/=(const T& s);

    /**
     * Assignment by addition operator with conventional meaning.  The expression
     * \f$ u += v; \f$ implies \f$ u_{i} = u_{i} + v_{i} \f$
     * The RWMathArray objects must \e conform, that is, have the same dimensions.
     */
    RWMathArray<T>&        operator+=(const RWMathArray<T>& v);

    /**
     * Assignment by subtraction operator with conventional meaning.  The expression
     *
     * \f$ u -= v; \f$ implies \f$ u_{i} = u_{i} - v_{i} \f$
     * The RWMathArray objects must \e conform, that is, have the same dimensions.
     */
    RWMathArray<T>&        operator-=(const RWMathArray<T>& v);

    /**
     * Assignment by multiplication operator with conventional meaning.  The
     * expression \f$ u *= v; \f$ implies \f$ u_{i} = u_{i} * v_{i} \f$
     * The RWMathArray objects must \e conform, that is, have the same dimensions.
     */
    RWMathArray<T>&        operator*=(const RWMathArray<T>& v);

    /**
     * Assignment by division operator with conventional meaning.  The
     * expression \f$ u /= v; \f$ implies \f$ u_{i} = u_{i} / v_{i} \f$
     * The RWMathArray objects must \e conform, that is, have the same dimensions.
     */
    RWMathArray<T>&        operator/=(const RWMathArray<T>& v);

    /**
     * Increments or decrements each element of self. The functions
     * taking an integer parameter are invoked if the operator is used
     * as a postfix operator.
     */
    RWMathArray<T>&        operator++();

    /**
     * @copydoc operator++()
     */
    RWMathArray<T>&        operator--();

    /**
     * @copydoc operator++()
     */
    void        operator++(int) {
        ++(*this);
    }

    /**
     * @copydoc operator++()
     */
    void        operator--(int) {
        --(*this);
    }

    /**
     * Returns \c true if self and the argument are equivalent (or not
     * equivalent). To be equivalent, they must have the same number
     * of rows as well as columns, and each element in self must equal
     * the corresponding element in the argument.
     */
    bool   operator==(const RWMathArray<T>& v) const;

    /**
     * Returns \c true if self and the argument are equivalent (or not
     * equivalent). To be equivalent, they must have the same number
     * of rows as well as columns, and each element in self must equal
     * the corresponding element in the argument.
     */
    bool   operator!=(const RWMathArray<T>& v) const;

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
     * Implicit conversion operator to
     * \link rw_numeric_traits::promote_type rw_numeric_traits<T>::promote_type\endlink.
     */
    operator RWMathArray<promote_type>() {
        RWMathArray<promote_type> tmp(length(), rwUninitialized);
        const_iterator b = begin();
        const_iterator e = end();
        typename RWMathArray<promote_type>::iterator tmpiter = tmp.begin();

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

    // real and imag need access to the RWBlock, so they must be friends
    friend RWMathArray<double> RW_MATH_SYMBOLIC real(const RWMathArray<DComplex>&);
    friend RWMathArray<double> RW_MATH_SYMBOLIC imag(const RWMathArray<DComplex>&);
private:
    // For real() and imag() and slices
    RWMathArray(const RWDataView& b, T* start, const RWMathVec<int>& n, const RWMathVec<int>& str)
        : RWArrayView(b, start, n, str) {
        ;
    }
};

/**
 * @ingroup math_collection_classes
 * @brief Converts between RWMathArray types with different datatypes.
 *
 * Class \link RWConvertMathArray RWConvertMathArray<From,To> \endlink
 * converts between RWMathArray types with different datatypes.
 *
 * @section synopsis Synopsis
 *
 * @code
 * template <class From, class To>
 * class RWConvertMathArray;
 * @endcode

 * @section example Example
 *
 * @code
 * #include <rw/math/mtharray.h>
 *
 * int main()
 * {
 *              // create a 5x5x5x5 array of 1's
 *    RWMathArray<int> d(5,5,5,5,1);
 *
 *             // convert int array d to DComplex
 *             // array c with all real parts 1
 *    RWMathArray<DComplex> c = RWConvertMathArray<int,DComplex>(d);
 *
 *             // convert int array d to a double
 *             // array and then call sin function
 *    RWMathArray<double>
 *    s = RWConvertMathArray<int,double>(d);
 *    s = sin(s);
 * }
 * @endcode
 *
 */
template <class From, class To>
struct RWConvertMathArray {
    /**
     * Constructs a new array of type \link RWMathArray RWMathArray<To>
     * \endlink that can be used anywhere as a type \link RWMathArray
     * RWMathArray<To> \endlink. Note that a temporary array is created
     * every time this constructor is invoked, but the conversion operator
     * below can be used repeatedly without creating additional temporaries.
     */
    RWConvertMathArray(const RWMathArray<From>& v) : to(v.length(), rwUninitialized) {
        rw_math_transform(v.begin(), v.end(), to.begin(), RW_convert<From, To>());
    }

    /**
     * Automatic conversion operator that enables the class
     * \link RWConvertMathArray RWConvertMathArray<From,To> \endlink to be
     * used as an \link RWMathArray RWMathArray<To> \endlink. While the actual
     * conversion takes place in the constructor, this operator provides a
     * reference to that converted array.
     */
    operator RWMathArray<To>() {
        return to;
    }

private:
    RWMathArray<To> to;
};

/********************
 * Global Operators *
 ********************/

/**
 * @relates RWMathArray
 *
 * Unary minus operator, with conventional meaning, applied element-by-element.
 * For matrix \a u, and \c w, the expression \f$ w = -u \f$ implies
 * \f$ w_{i} = -u_{i} \f$.
 */
template <class T>
RWMathArray<T> operator-(const RWMathArray<T>& u);

/**
 * @relates RWMathArray
 *
 * Unary plus operator, with conventional meaning, applied element-by-element.
 * For matrix \a u, and \c w, the expression \f$ w = +u \f$ implies
 * \f$ w_{i} = +u_{i} \f$.
 */
template <class T>
inline RWMathArray<T>  operator+(const RWMathArray<T>& u)
{
    return u;
}

/**
 * @relates RWMathArray
 *
 * Addition operator, with conventional meaning, applied element-by-element.
 * For matrices \a u, \a v, and \c w, the expression \f$ w = u + v \f$ implies
 * \f$ w_{i} = u_{i} + v_{i} \f$.
 * The arrays must \e conform, that is, have the same dimensions,
 * or an exception with value \c RWMATH_NMATCH occurs.
 */
template <class T>
RWMathArray<T>  operator+(const RWMathArray<T>& u, const RWMathArray<T>& v);

/**
 * @relates RWMathArray
 *
 * Subtraction operator, with conventional meaning, applied element-by-element.
 * For matrices \a u, \a v, and \c w, the expression \f$ w = u - v \f$ implies
 * \f$ w_{i} = u_{i} - v_{i} \f$.
 * The arrays must \e conform, that is, have the same dimensions,
 * or an exception with value \c RWMATH_NMATCH occurs.
 */
template <class T>
RWMathArray<T>  operator-(const RWMathArray<T>& u, const RWMathArray<T>& v);

/**
 * @relates RWMathArray
 *
 * Multiplication operator, with conventional meaning, applied
 * element-by-element.  For matrices \a u, \a v, and \c w, the expression
 * \f$ w = u * v \f$ implies
 * \f$ w_{i} = u_{i} * v_{i} \f$. Therefore,
 * operator*() implies an element-by-element multiply, \e not the inner product.
 * The arrays must \e conform, that is, have the same dimensions,
 * or an exception with value \c RWMATH_NMATCH occurs.
 */
template <class T>
RWMathArray<T>  operator*(const RWMathArray<T>& u, const RWMathArray<T>& v);

/**
 * @relates RWMathArray
 *
 * Division operator, with conventional meaning, applied element-by-element.
 * For matrices \a u, \a v, and \c w, the expression \f$ w = u / v \f$ implies
 * \f$ w_{i} = u_{i} / v_{i} \f$.
 * The arrays must \e conform, that is, have the same dimensions,
 * or an exception with value \c RWMATH_NMATCH occurs.
 */
template <class T>
RWMathArray<T>  operator/(const RWMathArray<T>& u, const RWMathArray<T>& v);

/**
 * @relates RWMathArray
 *
 * Addition operator, with conventional meaning, applied element-by-element.
 * For matrices \a u, \c w, and scaler \a s, the expression
 * \f$ w = u + s \f$ implies \f$ w_{i} = u_{i} + s \f$.
 */
template <class T>
RWMathArray<T>  operator+(const RWMathArray<T>& u, const T& s);

/**
 * @relates RWMathArray
 *
 * Subtraction operator, with conventional meaning, applied element-by-element.
 * For matrices \a v, \c w, and scaler \a s, the expression
 * \f$ w = s - v \f$ implies \f$ w_{i} = s - v_{i} \f$.
 */
template <class T>
RWMathArray<T>  operator-(const T& s, const RWMathArray<T>& v);

/**
 * @relates RWMathArray
 *
 * Multiplication operator, with conventional meaning, applied element-by-element.
 * For matrices \a u, \c w, and scaler \a s, the expression
 * \f$ w = u * s \f$ implies \f$ w_{i} = u_{i} * s \f$.
 */
template <class T>
RWMathArray<T>  operator*(const RWMathArray<T>& u, const T& s);

/**
 * @relates RWMathArray
 *
 * Division operator, with conventional meaning, applied element-by-element.
 * For matrices \a v, \c w, and scaler \a s, the expression
 * \f$ w = s / v \f$ implies \f$ w_{i} = s / v_{i} \f$.
 */
template <class T>
RWMathArray<T>  operator/(const T& s, const RWMathArray<T>& v);

/**
 * @relates RWMathArray
 *
 * Division operator, with conventional meaning, applied element-by-element.
 * For matrices \a u, \c w, and scaler \a s, the expression
 * \f$ w = u / s \f$ implies \f$ w_{i} = u_{i} / s \f$.
 */
template <class T>
RWMathArray<T>  operator/(const RWMathArray<T>& u, const T& s);

/**
 * Outputs an array \a v to \b std::ostream \a s. First, the size of
 * the array is output in the form 3 x 4 x 3, for example, and then
 * the elements of the array are output separated by spaces beginning
 * with a left bracket \c [ and terminating with a right bracket
 * \c ]. New lines are used to try and convey the structure of the
 * array.
 */
template <class T>
std::ostream&  operator<<(std::ostream& s, const RWMathArray<T>& v);

/**
 * Reads an array \a v from \b std::istream \c s. First, the size of
 * the array is read, then array values separated by white space.
 * The sequence of numbers begins with a size specification, such
 * as 3 x 4 x 3, followed by a left bracket \c [, and the operator
 * reads to a matching right bracket \c ].
 */
template <class T>
std::istream&  operator>>(std::istream& s, RWMathArray<T>& v);

#ifndef RW_NO_INLINED_TEMP_DESTRUCTORS
/**
 * @relates RWMathArray
 *
 * Multiplication operator, with conventional meaning, applied element-by-element.
 * For matrices \a v, \c w, and scaler \a s, the expression
 * \f$ w = s * v \f$ implies \f$ w_{i} = s * v_{i} \f$.
 */
template <class T>
inline RWMathArray<T>  operator*(const T& s, const RWMathArray<T>& v)
{
    return v * s;
}

/**
 * @relates RWMathArray
 *
 * Addition operator, with conventional meaning, applied element-by-element.
 * For matrices \a v, \c w, and scaler \a s, the expression
 * \f$ w = s + v \f$ implies \f$ w_{i} = s + v_{i} \f$.
 */
template <class T>
inline RWMathArray<T>  operator+(const T& s, const RWMathArray<T>& v)
{
    return v + s;
}

/**
 * @relates RWMathArray
 *
 * Subtraction operator, with conventional meaning, applied element-by-element.
 * For matrices \a u, \c w, and scaler \a s, the expression
 * \f$ w = u - s \f$ implies \f$ w_{i} = u_{i} - s \f$.
 */
template <class T>
inline RWMathArray<T>  operator-(const RWMathArray<T>& u, const T& s)
{
    return u + (T)(-s);
}

#else

template <class T>
RWMathArray<T>  operator*(const T&, const RWMathArray<T>&);

template <class T>
RWMathArray<T>  operator+(const T&, const RWMathArray<T>&);

template <class T>
RWMathArray<T>  operator-(const RWMathArray<T>&, const T&);
#endif

//******************************************************************
//
// Global Functions
//
//******************************************************************

// Norm Functions-----------------------------------------------------------
//
// These functions are not templatized on type T because they potentially
// return a type different than T.  Furthermore, the type they return is
// dependent on the the type T, e.g. abs(RWMathArray<double> x) returns
// a vector of doubles, while abs(RWMathArray<DComplex> x) returns a double.
// Ideally, we would define the "norm type" for each type in a traits class
// and declare these functions as
// RWMathArray< traits<T>::norm_type > abs(const RWMathArray<T>&);
// Unfortunately, almost no current compilers understand this syntax.
// So for now we'll just do the non template solution.
//
//--------------------------------------------------------------------------

/**
 * @relates RWMathArray
 *
 * Returns the absolute values of each element.
 *
 * @section example Example
 *
 * @code
 * #include <rw/math/mtharray.h>
 * #include <iostream>
 * const double adata[] = {1.2,2.4,-1.2,0.8,-4.5};
 * int main() {
 *         RWMathArray<double> a(adata,5, 1, 1);
 *         RWMathArray<double> b = abs(a);
 *         std::cout << b;
 *         return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 * 5x1x1 [
 * 1.2
 * 2.4
 * 1.2
 * 0.8
 * 4.5
 * ]
 * @endcode
 */
RWMathArray<double>  RW_MATH_SYMBOLIC abs(const RWMathArray<double>&);

/**
 * @relates RWMathArray
 *
 * @copydoc abs(const RWMathArray<double>&)
 *
 * See the example in abs(const RWMathArray<double>&).
 */
RWMathArray<float>   RW_MATH_SYMBOLIC abs(const RWMathArray<float>&);

/**
 * @relates RWMathArray
 *
 *
 * @copydoc abs(const RWMathArray<double>&)
 *
 * See the example in abs(const RWMathArray<double>&).
 */
RWMathArray<int>     RW_MATH_SYMBOLIC abs(const RWMathArray<int>&);

/**
 * @relates RWMathArray
 *
 * @copydoc abs(const RWMathArray<double>&)
 *
 * See the example in abs(const RWMathArray<double>&).
 *
 * @note The absolute value of a complex number is of type \c double. Therefore,
 * if abs() is invoked for class \link RWMathArray RWMathArray<DComplex>
 * \endlink, a vector of class \link RWMathArray RWMathArray<double>
 * \endlink is returned.
 */
RWMathArray<double>  RW_MATH_SYMBOLIC abs(const RWMathArray<DComplex>&);

/**
 * @relates RWMathArray
 *
 * @copydoc abs(const RWMathArray<double>&)
 *
 * See the example in abs(const RWMathArray<double>&).
 */
RWMathArray<SChar>   RW_MATH_SYMBOLIC abs(const RWMathArray<SChar>&);

/**
 * @relates RWMathArray
 *
 * Takes complex <i>x</i> as an argument and returns real <i>y</i>
 * containing the norm:
 *
 * \f$ y_{i} = [Re(x_{i})]^{2} + [Im(x_{i})]^{2} \f$.
 */
RWMathArray<double>  RW_MATH_SYMBOLIC norm(const RWMathArray<DComplex>&);

template< class T, class OutputPointer >
void rwVariance(const RWMathVec<T>& vec, OutputPointer var);

double RW_MATH_SYMBOLIC  variance(const RWMathVec<double>&);
double RW_MATH_SYMBOLIC  variance(const RWMathVec<DComplex>&);
float  RW_MATH_SYMBOLIC  variance(const RWMathVec<float>&);

/**
 * @relates RWMathArray
 *
 * Returns the value of the element with largest absolute value.
 * Note that this is not a norm in the mathematical sense of the word.
 */
double RW_MATH_SYMBOLIC maxNorm(const RWMathArray<double>& v);

/**
 * @relates RWMathArray
 *
 * @copydoc maxNorm(const RWMathArray<double>&)
 */
float  RW_MATH_SYMBOLIC maxNorm(const RWMathArray<float>& v);

/**
 * @relates RWMathArray
 *
 * @copydoc maxNorm(const RWMathArray<double>&)
 */
double RW_MATH_SYMBOLIC maxNorm(const RWMathArray<DComplex>& v);

/**
 * @relates RWMathArray
 *
 * Computes the Frobenius norm, which is the square root of the
 * sum of squares of its entries. For a vector, the formula is:\par
 *
 * \f[
 * \left \| A \right \|_{\text{Frob}} =
 * \sqrt{\sum_{i_{0}=0}^{M_{0}-1} \sum_{i_{n-1}=0}^{M_{n-1}-1} \left | \text{a}_{i_{o}...i_{n-1}} \right |^2}
 *
 * \f]
 *
 */
double RW_MATH_SYMBOLIC frobNorm(const RWMathArray<double>& v);

/**
 * @relates RWMathArray
 *
 * @copydoc frobNorm(const RWMathArray<double>&)
 */
float  RW_MATH_SYMBOLIC frobNorm(const RWMathArray<float>& v);

/**
 * @relates RWMathArray
 *
 * @copydoc frobNorm(const RWMathArray<double>&)
 */
double RW_MATH_SYMBOLIC frobNorm(const RWMathArray<DComplex>& v);


// Conversion Functions ---------------------------------------------

/**
 * @relates RWMathArray
 *
 * Converts a 0-dimensional array into a scalar. The array must
 * actually have 0 dimensions or a runtime error occurs.
 */
template<class T> T toScalar(const RWMathArray<T>&);

/**
 * @relates RWMathArray
 *
 * Converts a 1-dimensional array into a corresponding RWMathVec.
 * The array must actually have 1 dimension or a runtime error occurs.
 * The newly constructed vector is a new view of the same data as
 * the array; a copy of the data is not made.
 */
template<class T> RWMathVec<T> toVec(const RWMathArray<T>&);

/**
 * @relates RWMathArray
 *
 * Converts a 2-dimensional array into a corresponding general matrix
 * class. The array must actually have 2 dimensions or a runtime
 * error occurs. The newly constructed matrix is a new view of the
 * same data as the array; a copy of the data is not made.
 */
template<class T> RWGenMat<T> toGenMat(const RWMathArray<T>&);

/**
 * @relates RWMathArray
 *
 * Converts an \link RWMathArray RWMathArray<double>\endlink instance into a
 * corresponding \link RWMathArray RWMathArray<float>\endlink instance. Note
 * that this is a narrowing operation; high order bits are removed.
 * @see RWConvertMathArray
 */
RWMathArray<float> RW_MATH_SYMBOLIC toFloat(const RWMathArray<double>& v);

/**
 * @relates RWMathArray
 *
 * Converts an \link RWMathArray RWMathArray<double>\endlink instance into
 * a corresponding \link RWMathArray RWMathArray<int>\endlink instance. Note that
 * truncation occurs.
 * @see RWConvertMathArray
 */
RWMathArray<int>   RW_MATH_SYMBOLIC toInt(const RWMathArray<double>& v);

/**
 * @relates RWMathArray
 *
 * Converts an \link RWMathArray RWMathArray<float>\endlink instance into
 * a corresponding \link RWMathArray RWMathArray<int>\endlink instance. Note that
 * truncation occurs.
 * @see RWConvertMathArray
 */
RWMathArray<int>   RW_MATH_SYMBOLIC toInt(const RWMathArray<float>& v);

/**
 * @relates RWMathArray
 *
 * Converts an \link RWMathArray RWMathArray<int>\endlink instance into a
 * corresponding \link RWMathArray RWMathArray<signed char>\endlink instance.
 * Note that this is a narrowing operation; high order bits are removed.
 * @see RWConvertMathArray
 */
RWMathArray<SChar> RW_MATH_SYMBOLIC toChar(const RWMathArray<int>& v);

/**
 * @relates RWMathArray
 *
 * Return the \e index of the maximum element of the vector.
 * If instead you want the maximum \e value use function maxValue().
 */
template <class T>
RWIntVec  maxIndex(const RWMathArray<T>&);

/**
 * @relates RWMathArray
 *
 * Return the maximum value.
 */
template <class T>
T     maxValue(const RWMathArray<T>&);

/**
 * @relates RWMathArray
 *
 * Return the \e index of the minimum element of the vector.
 * If instead you want the minimum \e value use function minValue().
 */
template <class T>
RWIntVec  minIndex(const RWMathArray<T>&);

/**
 * @relates RWMathArray
 *
 * Return the minimum value.
 */
template <class T>
T     minValue(const RWMathArray<T>&);

template <class T>
T     prod(const RWMathArray<T>&);

template <class T>
T     sum(const RWMathArray<T>&);

// Functions for complex arrays ---------------------------------------

/**
 * @relates RWMathArray
 *
 * Returns the arguments <i>a</i><SUB>i</SUB> (in radians) of \e x
 * in the range  \f$ -\pi < a_{i} \leq \pi \f$,
 * where \f$ a_{i} = tan^{-1}[Im(x_{i})/Re(x_{i})] \f$.
 * Multiple valued functions of complex arguments, such as sqrt()
 * and log(), are therefore constrained to their principle value.
 */
RWMathArray<double>   RW_MATH_SYMBOLIC arg(const RWMathArray<DComplex>& v);

/**
 * @relates RWMathArray
 *
 * Takes complex \e x as an argument and returns the complex
 * conjugates <i>x</i><SUP>*</SUP>. For example, if <i>x</i><SUB>i</SUB> =
 * (2.3, 1.4), then <i>x</i><SUB>i</SUB><SUP>*</SUP> = (2.3, -1.4).
 */
RWMathArray<DComplex> RW_MATH_SYMBOLIC conj(const RWMathArray<DComplex>& v);

/**
 * @relates RWMathArray
 *
 * Takes complex <i>x</i> as an argument and returns <i>y</i> containing
 * the imaginary parts \f$ y_{i} = Im(x_{i}) \f$.
 * With most versions of complex, the results can be used as an
 * l-value:
 *
 * @code
 * RWMathArray<DComplex> v(3,3,3, DComplex(3,0));   //  (0,  0),  (0,  0),  ...
 * imag(v)  =  1.0;                                 //  (0,  1),  (0,  1),  ...
 * @endcode
 */
RWMathArray<double>   RW_MATH_SYMBOLIC imag(const RWMathArray<DComplex>& v);

/**
 * @relates RWMathArray
 *
 * Takes a complex argument <i>x</i> and returns real <i>y</i> containing
 * the real part \f$ y_{i}=Re(x_{i}) \f$. With most
 * versions of complex, the results can be used as an l-value:
 *
 * @code
 * RWMathArray<DComplex>  v(5,5,5,0);    //  (0,  0),  (0,  0),  ...
 * real(v)  =  1.0;                      //  (1,  0),  (1,  0),  ...
 * @endcode
 */
RWMathArray<double>   RW_MATH_SYMBOLIC real(const RWMathArray<DComplex>& v);


/**** Functions for both real and complex ****/

/**
 * @relates RWMathArray
 *
 * Takes \a x as an argument and returns \e y such that
 * \f$ y_{i} = cos(x_{i}) \f$. The
 * <i>x</i><SUB>i</SUB> are in radians. For complex arguments,
 * the complex cosine is returned.
 */
template <class T>
RWMathArray<T>  cos(const RWMathArray<T>& x);

/**
 * @relates RWMathArray
 *
 * Takes \a x as an argument and returns hyperbolic cosines
 * \e y such that \f$ y_{i} = cosh(x_{i}) \f$.
 * For complex arguments, the complex hyperbolic cosine is
 * returned.
 */
template <class T>
RWMathArray<T>  cosh(const RWMathArray<T>& x);

/**
 * @relates RWMathArray
 *
 * Takes an argument \a x and returns \e y such that:
 *
 * \f[
 * y_i = e^{x_i}
 * \f]
 *
 * If class \c T is complex, the complex exponential is returned.
 */
template <class T>
RWMathArray<T>  exp(const RWMathArray<T>& x);

/**
 * @relates RWMathArray
 *
 * Takes a RWMathArray an argument and returns <i>\<x\>,</i> the mean value,
 * where:
 *
 * \f[
 * \langle x \rangle = \frac{1}{n}\sum_{i=0}^{n-1}x_i
 * \f]
 *
 * For example, if <i>x = [1, 4, 3, 4]</i>, then <i>\<x\> = 3</i>.
 */
template <class T>
T  mean(const RWMathArray<T>& V);

/**
 * @relates RWMathArray
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
template <class T>
RWMathArray<T>  pow(const RWMathArray<T>& x, const RWMathArray<T>& y);

/**
 * @relates RWMathArray
 *
 * The function pow() takes two arguments: pow( \a x, \a y ).
 *
 * Returns <i>z</i> such that:
 *
 * \f$ z_i = (x_i)^{y} \f$
 */
template <class T>
RWMathArray<T> pow(const RWMathArray<T>& x, T y);

// internal helper overload for complex
template <class T>
RWMathArray<std::complex<T> > pow(const RWMathArray<std::complex<T> >& x, std::complex<T> y);

/**
 * @relates RWMathArray
 *
 * The function pow() takes two arguments: pow( \a x, \a y ).
 *
 * Returns <i>z</i> such that:
 *
 * \f$ z_i = (x)^{y_i} \f$
 */
template <class T>
RWMathArray<T> pow(T x, const RWMathArray<T>& y);

// internal helper overload for complex
template <class T>
RWMathArray<std::complex<T> > pow(std::complex<T> x, const RWMathArray<std::complex<T> >& y);

/**
 * @relates RWMathArray
 *
 * The function sin() takes <i>x </i>as
 * an argument and returns <i>y </i>such that \f$ y_{i}
 * = sin(x_{i}) \f$. The <i>x</i><SUB>i</SUB> are in radians.
 * For complex classes, the complex sine is returned.
 */
template <class T>
RWMathArray<T>  sin(const RWMathArray<T>& V);

/**
 * @relates RWMathArray
 *
 * The function sinh() takes
 * <i>x</i> as an argument and returns <i>y</i> such that \f$ y_{i}
 * = sinh(x_{i}) \f$. For complex classes, the complex hyperbolic
 * sine is returned.
 */
template <class T>
RWMathArray<T>  sinh(const RWMathArray<T>& V);

/**
 * @relates RWMathArray
 *
 * The square root function. Takes <i>x</i> as an argument and returns
 * <i>y</i> such that
 * \f$ y_{i} = (x_{i})^{1/2} \f$.
 * For complex classes, the principal value of the complex square
 * root is returned.
 */
template <class T>
RWMathArray<T>  sqrt(const RWMathArray<T>& V);

/**** Functions for real arrays ****/

/**
 * @relates RWMathArray
 *
 * Returns arc cosines \e y such that \f$ y_{i} =
 * cos^{-1} (x_{i}) \f$. The <i>y</i><SUB>i</SUB>(in
 * radians) are in the range \f$ 0 < y_{i} \leq \pi \f$,
 * for elements <i>x</i><SUB>i</SUB>
 * with absolute values \f$ |x_{i}| \leq
 * 1 \f$.
 */
template <class T>
RWMathArray<T>  acos(const RWMathArray<T>& x);

/**
 * @relates RWMathArray
 *
 * Takes \a x as an argument and returns arc sines \e y such
 * that \f$ y_{i} = sin^{-1} (x_{i}) \f$.
 * The <i>y</i><SUB>i</SUB>(in radians) are in the range
 * \f$ -\pi/2 < y_{i} \leq \pi/2 \f$ , for elements
 * <i>x</i><SUB>i</SUB> with absolute values \f$ |x_{i}|
 * \leq 1 \f$.
 */
template <class T>
RWMathArray<T>  asin(const RWMathArray<T>& x);

/**
 * @relates RWMathArray
 *
 * Takes \a x and returns \e y of arc
 * tangents (in radians), such that \f$ y_{i} =
 * tan^{-1}x_{i} \f$,
 * where -\f$ \pi/2 < y_{i} \leq \pi/2 \f$.
 *
 * At least one of the arguments <i>x</i><SUB>i</SUB> or <i>y</i><SUB>i</SUB>
 * must be nonzero.
 */
template <class T>
RWMathArray<T>  atan(const RWMathArray<T>& x);

/**
 * @relates RWMathArray
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
template <class T>
RWMathArray<T>  atan2(const RWMathArray<T>& x, const RWMathArray<T>& y);

/**
 * @relates RWMathArray
 *
 * Takes \a x as an argument and returns \e y such that
 * <i>y</i><SUB>i</SUB> corresponds to the next integer greater
 * than or equal to <i>x</i><SUB>i</SUB>. For example,  if
 * <i>x = [ -1.3, 4.3, 7.9] </i>then <i>y = [ -1, 5, 8].</i>
 */
template <class T>
RWMathArray<T>  ceil(const RWMathArray<T>& x);

/**
 * @relates RWMathArray
 *
 * Takes \a x as an argument and returns \e y such that
 * <i>y</i><SUB>i</SUB> corresponds to the largest integer less
 * than or equal to <i>x</i><SUB>i</SUB>. For example, if <i>x =
 * [ 1.3, 4.4, -3.2]</i>, then <i>y = [ 1, 4, -4].</i>
 */
template <class T>
RWMathArray<T>  floor(const RWMathArray<T>& x);

/**
 * @relates RWMathArray
 *
 * Takes \a x as an argument and returns natural logarithms
 * \e y such that \f$ y_{i}=log(x_{i}) \f$.
 * The arguments \a x <SUB>i</SUB> must not be 0. For complex
 * arguments, the principal value of the complex natural logarithm
 * is returned.
 */
template <class T>
RWMathArray<T>  log(const RWMathArray<T>& x);

/**
 * @relates RWMathArray
 *
 * Takes \a x as an argument and returns base 10 logarithms
 * \e y such that \f$ y_{i}=log_{10}(x_{i}) \f$.
 * The arguments \a x <SUB>i</SUB> must not be 0.
 */
template <class T>
RWMathArray<T>  log10(const RWMathArray<T>& x);

/**
 * @relates RWMathArray
 *
 * The function tan() takes argument \a x> and returns \e y such that
 * \f$ y_{i} = tan(x_{i}) \f$.
 */
template <class T>
RWMathArray<T>  tan(const RWMathArray<T>& x);

/**
 * @relates RWMathArray
 *
 * The function tanh() takes argument \a x and returns \e y such that
 * \f$ y_{i} = tanh(x_{i}) \f$.
 *
 */
template <class T>
RWMathArray<T>  tanh(const RWMathArray<T>& x);


/***************************
 * Inline Access Functions *
 ***************************/


template <class T>
inline T& RWMathArray<T>::operator[](const RWIntVec& i)
{
    boundsCheck(i);
    return data()[dot(i, step)];
}

template <class T>
inline T RWMathArray<T>::operator[](const RWIntVec& i) const
{
    boundsCheck(i);
    return data()[dot(i, step)];
}

template <class T>
inline T& RWMathArray<T>::operator()(const RWIntVec& i)
{
#ifdef RWBOUNDS_CHECK
    boundsCheck(i);
#endif
    return data()[dot(i, step)];
}

template <class T>
inline T RWMathArray<T>::operator()(const RWIntVec& i) const
{
#ifdef RWBOUNDS_CHECK
    boundsCheck(i);
#endif
    return data()[dot(i, step)];
}

template <class T>
inline T& RWMathArray<T>::operator()(int i, int j, int k)
{
#ifdef RWBOUNDS_CHECK
    int index[] = {i, j, k};
    boundsCheck(index, 3);
#endif
    return data()[i * step((int)0) + j * step(1) + k * step(2)];
}

template <class T>
inline T RWMathArray<T>::operator()(int i, int j, int k) const
{
#ifdef RWBOUNDS_CHECK
    int index[] = {i, j, k};
    boundsCheck(index, 3);
#endif
    return data()[i * step((int)0) + j * step(1) + k * step(2)];
}

template <class T>
inline T& RWMathArray<T>::operator()(int i, int j, int k, int l)
{
#ifdef RWBOUNDS_CHECK
    int index[] = {i, j, k, l};
    boundsCheck(index, 4);
#endif
    return data()[i * step((int)0) + j * step(1) + k * step(2) + l * step(3)];
}

template <class T>
inline T RWMathArray<T>::operator()(int i, int j, int k, int l) const
{
#ifdef RWBOUNDS_CHECK
    int index[] = {i, j, k, l};
    boundsCheck(index, 4);
#endif
    return data()[i * step((int)0) + j * step(1) + k * step(2) + l * step(3)];
}


#if defined(RW_TEMP_NO_STD_BASE) /* avoid preprocessor warning */
#  undef RW_TEMP_NO_STD_BASE
#endif

#if defined(_MSC_VER) && (_MSC_VER == 1500)
#  pragma warning(disable:4910)
#endif

RWMATH_INSTANTIATE(RW_MATH_GLOBAL, RWMathArray<DComplex>);

#if defined(_MSC_VER) && (_MSC_VER == 1500)
#  pragma warning(default:4910)
#endif

#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/math/mtharray.cc>
#endif

#endif /*__RWMTHARRAY_H__*/
