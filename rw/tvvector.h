#ifndef RW_TOOLS_TVVECTOR_H
#define RW_TOOLS_TVVECTOR_H

/**********************************************************************
 *
 * RWTValVector: Parameterized vectors of values
 * Stores a *copy* of the inserted item into the collection.
 * Assumes that T has:
 *   - well-defined copy semantics (T::T(const T&) or equiv.);
 *   - well-defined assignment semantics (T::operator=(const T&) or equiv.);
 * Note that while these are automatically defined for builtin types
 * (such as "int", "double", or any pointer), you may need to provide
 * appropriate operators for your own classes, particularly those with
 * constructors and/or pointers to other objects.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvvector.h#2 $
 *
 **********************************************************************
 *
 * Copyright (c) 1989-2015 Rogue Wave Software, Inc.  All Rights Reserved.
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
 **********************************************************************/

#include <rw/defs.h>
#include <rw/edefs.h>  // for rw_move()
#include <rw/epfunc.h>

#include <rw/tools/algorithm.h>
#include <rw/tools/iterator.h>

#include <rw/tools/traits/RWTEnableIf.h>
#include <rw/tools/traits/RWTIsIntegral.h>

/**
 * @ingroup traditional_collection_classes
 *
 * @brief A value-based collection of values implemented as a vector.
 *
 * Class \link RWTValVector RWTValVector<T>\endlink is a simple parameterized
 * vector of objects of type \c T. It is most useful when you know precisely
 * how many objects have to be held in the collection. If the intention
 * is to "insert" an unknown number of objects, then class
 * \link RWTValOrderedVector RWTValOrderedVector<T,A>\endlink may be a better
 * choice.
 *
 * The class \c T must have:
 *
 * -  well-defined copy semantics (<tt>T::T(const T&)</tt> or equivalent),
 * -  well-defined assignment semantics (<tt>T::operator=(const T&</tt>
 *    or equivalent), and
 * -  a default constructor.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvvector.h>
 * RWTValVector<T> vec;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Isomorphic
 *
 * @section example Example
 *
 * @code
 * //
 * // tvvector.cpp
 * //
 * #include <iostream>
 *
 * #include <rw/tvvector.h>
 * #include <rw/tools/datetime.h>
 *
 * int main ()
 * {
 *     RWTValVector<RWDateTime> week(7);
 *
 *     RWDateTime begin(RWDateTime::setCurrentTime); // Today's date
 *
 *     for (size_t i = 0; i < 7; ++i) {
 *         begin.incrementDay(1);
 *         week[i] = begin;
 *     }
 *
 *     for (size_t i = 0; i < 7; ++i) {
 *         std::cout << week[i].asString('x') << "\n";
 *     }
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program output (if this is run on June 19, 2009):
 *
 * @code
 *     06/19/09
 *     06/20/09
 *     06/21/09
 *     06/22/09
 *     06/23/09
 *     06/24/09
 *     06/25/09
 * @endcode
 */
template <class T> class RWTValVector
{

public:

    /**
     * A type representing the container's data type.
     */
    typedef T value_type;

    /**
     * An unsigned integral type used for counting the number of elements
     * in the container.
     */
    typedef size_t size_type;

    /**
     * A signed integral type used to indicate the distance between two
     * valid iterators on the same container.
     */
    typedef ptrdiff_t difference_type;

    /**
     * A type that provides a reference to an element in the container.
     */
    typedef value_type& reference;

    /**
     * A type that provides a \c const reference to an element in the
     * container.
     */
    typedef const value_type& const_reference;

    /**
     * A type that provides a pointer to an element in the container.
     */
    typedef value_type* pointer;

    /**
     * A type that provides a \c const pointer to an element in the
     * container.
     */
    typedef const value_type* const_pointer;

    /**
     * A type that provides a random-access iterator over the elements
     * in the container.
     */
    typedef pointer iterator;

    /**
     * A type that provides a \c const random-access iterator over the
     * elements in the container.
     */
    typedef const_pointer const_iterator;

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


    /**
     * Constructs an empty vector of length zero.
     */
    RWTValVector() : npts_(0), array_(0) {
    }

    /**
     * Constructs a vector of length \a n. The values of the elements
     * are set by the default constructor of class \c #value_type.
     */
    RWTValVector(size_type n) : npts_(n) {
        array_ = (0 == n) ? 0 : new T[n];
    }

    /**
     * Constructs a vector of length \a n, with each element initialized
     * to the value \a ival.
     */
    RWTValVector(size_type n, const_reference ival);

    /**
     * Constructs self as a copy of \a rhs. Each element in \a rhs is
     * copied into self.
     */
    RWTValVector(const RWTValVector<T>& rhs);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed vector takes ownership of the
     * data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValVector(RWTValVector<T> && rhs);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Copies each element in the range [\a first, \a last) into self,
     * replacing any existing items.
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
    template <typename InputIterator>
    RWTValVector(InputIterator first, InputIterator last,
                 typename RWTEnableIf < !RWTIsIntegral<InputIterator>::value >::type** = 0)
        : npts_(0), array_(0) {
        assign(first, last);
    }

    /**
     * Calls the destructor for every element in self.
     */
    ~RWTValVector() {
        delete [] array_;
    }

    /**
     * Sets self to the same length as \a rhs and then copies all elements
     * of \a rhs into self.
     */
    RWTValVector<T>& operator=(const RWTValVector<T>& rhs);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValVector<T>& operator=(RWTValVector<T> && rhs);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Sets all elements in self to the value \a ival.
     */
    RWTValVector<T>& operator=(const_reference ival);

    /**
     * Invokes the function pointer \a fn on each item in the collection.
     * Client data may be passed through parameter \a d.
     */
    void apply(void(*fn)(reference, void*), void* d) {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * @copydoc apply()
     */
    void apply(void(*fn)(const_reference, void*), void* d) const {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * Copies each element in the range [\a first, \a last) into self,
     * replacing any existing items.
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     *
     * @note
     * \a first and \a last must not be iterators into self.
     */
    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last) {
        typedef typename rw_iterator_traits<InputIterator>::iterator_category cat;
        assign(first, last, cat());
    }

    /**
     * Returns a reference to the \a i <sup>th</sup> element of self.
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0 and one
     * less than the number of entries in self.
     */
    reference at(size_type i) {
        return (*this)[i];
    }

    /**
     * @copydoc at()
     */
    const_reference at(size_type i) const {
        return (*this)[i];
    }

    /**
     * Returns an iterator referring to the first element in the container.
     *
     * If the container is empty, returns end().
     */
    iterator begin() {
        return array_;
    }

    /**
     * @copydoc begin()
     */
    const_iterator begin() const {
        return cbegin();
    }

    /**
     * @copydoc begin()
     */
    const_iterator cbegin() const {
        return array_;
    }

    /**
     * Clears the collection by removing all items from self. Each
     * item has its destructor called.
     */
    void clear() {
        reshape(0);
    }

    /**
     * Returns \c true if there exists an element \c t in self such
     * that the expression <tt>((*\a fn)(t,\a d))</tt> is \c true,
     * otherwise returns \c false. Client data may be passed through
     * parameter \a d.
     */
    bool contains(bool(*fn)(const_reference, void*), void* d) const {
        return std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d)) != end();
    }

    /**
     * Returns \c true if there exists an element \c t in self such
     * that the expression <tt>(t == \a a)</tt> is \c true, otherwise
     * returns \c false.
     */
    bool contains(const_reference a) const {
        return std::find(begin(), end(), a) != end();
    }

    /**
     * Returns a pointer to the first element of the vector. The value returned
     * is undefined if the vector is empty.
     */
    pointer data() {
        return array_;
    }

    /**
     * @copydoc data()
     */
    const_pointer data() const {
        return array_;
    }

    /**
     * Returns an iterator referring to the element after the last element
     * in the container.
     *
     * Dereferencing the iterator returned by this function results in
     * undefined behavior.
     */
    iterator end() {
        return begin() + entries();
    }

    /**
     * @copydoc end()
     */
    const_iterator end() const {
        return cend();
    }

    /**
     * @copydoc end()
     */
    const_iterator cend() const {
        return begin() + entries();
    }

    /**
     * Returns the number of elements in self.
     */
    size_type entries() const {
        return npts_;
    }

    /**
     * If there exists an element \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, assigns \c t to \a k and
     * returns \c true. Otherwise, returns \c false and leaves the value
     * of \a k unchanged. Client data may be passed through parameter \a d.
     */
    bool find(bool(*fn)(const_reference, void*), void* d, value_type& k) const {
        const_iterator ret = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        if (ret != end()) {
            k = *ret;
            return true;
        }
        return false;
    }

    /**
     * If there exists an element \c t in self such that the expression
     * <tt>(t == \a a)</tt> is \c true, assigns \c t to \a k and returns
     * \c true. Otherwise, returns \c false and leaves the value of \a k
     * unchanged.
     */
    bool find(const_reference a, value_type& k) const {
        const_iterator ret = std::find(begin(), end(), a);
        if (ret != end()) {
            k = *ret;
            return true;
        }
        return false;
    }

    /**
     * Returns a reference to the first item in the collection.
     *
     * Calling this function on an empty collection results in
     * undefined behavior.
     */
    reference first() {
        RW_PRECONDITION(!isEmpty());
        return array_[0];
    }

    /**
     * @copydoc first()
     */
    const_reference first() const {
        RW_PRECONDITION(!isEmpty());
        return array_[0];
    }

    /**
     * Returns the position of the first item \c t in self such that
     * (t == a), or returns #RW_NPOS if no such item exists.
     */
    size_type index(const_reference a) const {
        const_iterator res = std::find(begin(), end(), a);
        return res != end() ? res - begin() : RW_NPOS;
    }

    /**
     * Returns the position of the first item \c t in self such that
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, or returns #RW_NPOS if
     * no such item exists. Client data may be passed through parameter
     * \a d.
     */
    size_type index(bool(*fn)(const_reference, void*), void* d) const {
        const_iterator res = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        return res != end() ? res - begin() : RW_NPOS;
    }

    /**
     * Returns \c true if there are no items in the collection, otherwise
     * \c false.
     */
    bool isEmpty() const {
        return npts_ == 0;
    }

    /**
     * Returns a reference to the last item in the collection.
     *
     * Calling this function on an empty collection results in
     * undefined behavior.
     */
    reference last() {
        RW_PRECONDITION(!isEmpty());
        return array_[npts_ - 1];
    }

    /**
     * @copydoc last()
     */
    const_reference last() const {
        RW_PRECONDITION(!isEmpty());
        return array_[npts_ - 1];
    }

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * Returns the number of elements in self.
     *
     * @deprecated As of SourcePro 13, use entries() instead.
     */
    RW_DEPRECATE_FUNC("Use RWTValVector::entries() instead")
    size_type length() const {
        return npts_;
    }
#endif

    /**
     * Returns a reference to the maximum element in the collection.
     * Type \c T must have well-defined less-than semantics
     * (<tt>T::operator<(const T&)</tt> or equivalent).
     */
    reference maxElement() {
        RW_PRECONDITION(!isEmpty());
        return *std::max_element(begin(), end(), std::less<value_type>());
    }

    /**
     * @copydoc maxElement()
     */
    const_reference maxElement() const {
        RW_PRECONDITION(!isEmpty());
        return *std::max_element(begin(), end(), std::less<value_type>());
    }

    /**
     * Returns a reference to the minimum element in the collection.
     * Type \c T must have well-defined less-than semantics
     * (<tt>T::operator<(const T&)</tt> or equivalent).
     */
    reference minElement() {
        RW_PRECONDITION(!isEmpty());
        return *std::min_element(begin(), end(), std::less<value_type>());
    }

    /**
     * @copydoc minElement()
     */
    const_reference minElement() const {
        RW_PRECONDITION(!isEmpty());
        return *std::min_element(begin(), end(), std::less<value_type>());
    }

    /**
     * Returns the number of elements \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true. Client data may be passed through
     * parameter \a d.
     */
    size_type occurrencesOf(bool(*fn)(const_reference, void*), void* d) const {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Returns the number of elements \c t in self such that the expression
     * <tt>(t == \a a)</tt> is \c true.
     */
    size_type occurrencesOf(const_reference a) const {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count(begin(), end(), a);
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Returns an iterator referring to the last element in the container.
     *
     * If the container is empty, returns rend().
     */
    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    /**
     * @copydoc rbegin()
     */
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    /**
     * @copydoc rbegin()
     */
    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(end());
    }

    /**
     * Returns an iterator referring to the element before the first element
     * in the container.
     *
     * Dereferencing the iterator returned by this function results in
     * undefined behavior.
     */
    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    /**
     * @copydoc rend()
     */
    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    /**
     * @copydoc rend()
     */
    const_reverse_iterator crend() const {
        return const_reverse_iterator(begin());
    }

    const_reference ref(size_type i) const {
        return array_[i];
    }

    /**
     * Replaces all elements \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns the number
     * of items replaced. Client data may be passed through parameter
     * \a d.
     */
    size_type replaceAll(bool(*fn)(const value_type&, void*), void* d, const value_type& newVal) {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_and_replace_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d), newVal);
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Replaces all elements \c t in self with \a newVal such that the expression
     * <tt>(t == \a oldVal)</tt> is \c true. Returns the number of items
     * replaced.
     */
    size_type replaceAll(const value_type& oldVal, const value_type& newVal) {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_and_replace(begin(), end(), oldVal, newVal);
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Changes the length of the vector to \a N. If this increases
     * the length of the vector, then the initial value of the additional
     * elements is set by the default constructor of \c #value_type.
     */
    void reshape(size_type N);

    /**
     * Sorts the collection using the less-than operator to compare
     * elements.
     */
    void sort() {
        std::sort(begin(), end());
    }

    /**
     * Returns a reference to the \a i <sup>th</sup> value in the vector.
     * Index \a i should be between \c 0 and one less than the number
     * of entries, otherwise the results are undefined.
     * @note
     * No bounds checking is performed.
     */
    reference operator()(size_type i) {
#ifdef RWBOUNDS_CHECK
        boundsCheck(i);
#endif
        return array_[i];
    }

    // see doxygen bug #612458
    /**
     * Returns a reference to the \a i <sup>th</sup> value in the vector.
     * Index \a i should be between \c 0 and one less than the number
     * of entries, otherwise the results are undefined.
     * @note
     * No bounds checking is performed.
     */
    const_reference operator()(size_type i) const {
#ifdef RWBOUNDS_CHECK
        boundsCheck(i);
#endif
        return array_[i];
    }

    /**
     * Returns a reference to the \a i <sup>th</sup> value in the vector.
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0 and one
     * less than the number of entries in self.
     */
    reference operator[](size_type i) {
        boundsCheck(i);
        return array_[i];
    }

    /**
     * @copydoc operator[](size_t)
     */
    const_reference operator[](size_type i) const {
        boundsCheck(i);
        return array_[i];
    }

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWTValVector<T>& rhs);

private:

    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last, const std::input_iterator_tag&) {
        size_t length = 0;
        size_t capacity = 16;

        RWTValVector<T> buf(capacity);
        for (/**/; first != last; ++first) {

            if (length == capacity) {
                buf.reshape(capacity *= 2);
            }

            buf.array_[length++] = *first;
        }

        assign(buf.array_, buf.array_ + length);
    }

    template <typename ForwardIterator>
    void assign(ForwardIterator first, ForwardIterator last, const std::forward_iterator_tag&) {
        const typename rw_iterator_traits<ForwardIterator>::difference_type n = rw_distance(first, last);
        RW_ASSERT(0 <= n);

        RWTValVector<T> buf(static_cast<size_t>(n));
        for (pointer dst = buf.array_; first != last; ++first) {
            *dst++ = *first;
        }

        buf.swap(*this);
    }

protected:

    void boundsCheck(size_t i) const;

    size_type npts_;
    pointer array_;

};

/**
 * @relates RWTValVector
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and
 * iterating through both collections produces individual elements that,
 * in turn, compare equal to each other.
 */
template <class T>
bool operator==(const RWTValVector<T>& lhs, const RWTValVector<T>& rhs)
{
    if (lhs.entries() != rhs.entries()) {
        return false;
    }
    else if (rhs.isEmpty()) {
        return true;
    }

    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/**
 * @relates RWTValVector
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T>
bool operator!=(const RWTValVector<T>& lhs, const RWTValVector<T>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTValVector
 *
 * Returns \c true if \a lhs is lexicographically less than \a rhs,
 * otherwise \c false. Assumes that type \c T has well-defined less-than
 * semantics.
 */
template <class T>
bool operator<(const RWTValVector<T>& lhs, const RWTValVector<T>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

/**
 * @relates RWTValVector
 *
 * Equivalent to <tt>(\a rhs < \a lhs)</tt>.
 */
template <class T>
bool operator>(const RWTValVector<T>& lhs, const RWTValVector<T>& rhs)
{
    return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}

/**
 * @relates RWTValVector
 *
 * Equivalent to <tt>!(\a rhs < \a lhs)</tt>.
 */
template <class T>
bool operator<=(const RWTValVector<T>& lhs, const RWTValVector<T>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTValVector
 *
 * Equivalent to <tt>!(\a lhs < \a rhs)</tt>.
 */
template <class T>
bool operator>=(const RWTValVector<T>& lhs, const RWTValVector<T>& rhs)
{
    return !(lhs < rhs);
}

RW_DECLARE_PERSISTABLE_TEMPLATE(RWTValVector)

template <class T, class S>
void rwSaveGuts(S& str, const RWTValVector<T>& c);

template <class T, class S>
void rwRestoreGuts(S& str, RWTValVector<T>& c);


#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tvvector.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tvvector.cc"
#endif


#endif /* RW_TOOLS_TVVECTOR_H */
