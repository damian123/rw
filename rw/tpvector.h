#ifndef RW_TOOLS_TPVECTOR_H
#define RW_TOOLS_TPVECTOR_H

/**********************************************************************
 *
 * RWTPtrVector: Parameterized vector of pointers to T
 * Stores a *pointer* to the item in the vector. Basically, a typesafe
 * interface to RWPtrVector.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tpvector.h#2 $
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

#include <rw/pvector.h>
#include <rw/edefs.h>  // for rw_move()
#include <rw/epfunc.h>
#include <rw/epaux.h>

#include <rw/tools/algorithm.h>
#include <rw/tools/iterator.h>

/**
 * @ingroup traditional_collection_classes
 *
 * @brief A pointer-based collection of values implemented as a vector.
 *
 * Class \link RWTPtrVector RWTPtrVector<T>\endlink is a simple parameterized
 * vector of pointers to objects of type \c T. It is most useful when you know
 * precisely how many pointers must be held in the collection. If the intention
 * is to insert an unknown number of objects into a collection,
 * then class \link RWTPtrOrderedVector RWTPtrOrderedVector<T,A>\endlink may
 * be a better choice. Class \c T can be of any type.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tpvector.h>
 * RWTPtrVector<T> vec;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Isomorphic
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/tpvector.h>
 * #include <rw/tools/datetime.h>
 *
 * int main ()
 * {
 *     RWTPtrVector<RWDateTime> week(7);
 *
 *     RWDateTime begin(RWDateTime::setCurrentTime); // Today's date
 *
 *     for (size_t i = 0; i < 7; ++i) {
 *         week[i] = new RWDateTime(begin);
 *         begin.incrementDay(1);
 *     }
 *
 *     for (size_t i = 0; i < 7; ++i) {
 *         std::cout << week[i]->asString('x') << std::endl;
 *         delete week[i];
 *     }
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program output (if run on June 18, 2009):
 *
 * @code
 *     06/18/09
 *     06/19/09
 *     06/20/09
 *     06/21/09
 *     06/22/09
 *     06/23/09
 *     06/24/09
 * @endcode
 *
 */
template<class T> class RWTPtrVector : private RWPtrVector
{

public:

    /**
     * A type representing the container's data type.
     */
    typedef T* value_type;

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
     * @internal
     * A type representing a comparison function that dereferences the
     * arguments before comparing.
     */
    typedef rw_deref_compare<std::less<T>, T> deref_less;

    /**
     * @internal
     * A type representing an equality function that dereferences the
     * arguments before comparing.
     */
    typedef rw_deref_compare<std::equal_to<T>, T> deref_equal;

    /**
     * Constructs an empty vector of length zero.
     */
    RWTPtrVector() {
    }

    /**
     * Constructs a vector of length \a n. The initial values of the
     * elements are undefined. Hence, they can (and probably will)
     * be garbage.
     */
    RWTPtrVector(size_type n) : RWPtrVector(n) {
    }

    /**
     * Constructs a vector of length \a n, with each element pointing
     * to the item \a ival.
     */
    RWTPtrVector(size_type n, value_type ival) : RWPtrVector(n, ival) {
    }

    /**
     * Copy constructor. The constructed vector is a shallow copy of \a rhs.
     * The two vectors will then have the same length, and pointers held
     * by the two vectors will point to the same items.
     */
    RWTPtrVector<T>(const RWTPtrVector<T>& rhs) : RWPtrVector(rhs) {
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed vector takes ownership of the
     * data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrVector<T>(RWTPtrVector<T> && rhs) : RWPtrVector(rw_move(rhs)) {
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs a vector by copying elements from the range
     * [\a first, \a last).
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
    template <typename InputIterator>
    RWTPtrVector(InputIterator first, InputIterator last)
        : RWPtrVector(first, last) {
    }

    /**
     * Sets self to a shallow copy of \a rhs. The two vectors will
     * then have the same length, and pointers held by the two vectors
     * will point to the same items.
     */
    RWTPtrVector<T>& operator=(const RWTPtrVector<T>& rhs) {
        return (RWTPtrVector<T>&)RWPtrVector::operator=(rhs);
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrVector<T>& operator=(RWTPtrVector<T> && rhs) {
        return (RWTPtrVector<T>&)RWPtrVector::operator=(rw_move(rhs));
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Sets all elements in self to point to the item \a p.
     */
    RWTPtrVector<T>& operator=(value_type p) {
        return (RWTPtrVector<T>&)RWPtrVector::operator=(p);
    }

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
    void apply(void(*fn)(value_type, void*), void* d) {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * @copydoc apply()
     */
    void apply(void(*fn)(const T*, void*), void* d) const {
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
        RWPtrVector::assign(first, last);
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
        return data();
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
        return data();
    }

    /**
     * Clears the collection by removing all items from self. Does not
     * call <tt>operator delete</tt> on the individual items.
     */
    void clear() {
        RWPtrVector::clear();
    }

    /**
     * Removes all items from the collection and uses <tt>operator delete</tt>
     * to destroy the objects pointed to by those items. Do
     * not use this method if multiple pointers to the same object are
     * stored.
     */
    void clearAndDestroy() {
        rw_destroy(begin(), end());
        clear();
    }

    /**
     * Returns \c true if there exists an element \c t in self such that
     * the expression <tt>((*\a fn)(t,\a d))</tt> is \c true, otherwise
     * returns \c false. Client data may be passed through parameter \a d.
     */
    bool contains(bool(*fn)(value_type, void*), void* d) const {
        return std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d)) != end();
    }

    /**
     * Returns \c true if there exists an element \c t in self such that
     * the expression <tt>((*\a fn)(t,\a d))</tt> is \c true, otherwise
     * returns \c false. Client data may be passed through parameter \a d.
     */
    bool contains(bool(*fn)(const T*, void*), void* d) const {
        return std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d)) != end();
    }

    /**
     * Returns \c true if there exists an element \c t in self such that
     * the expression <tt>(*t == *\a a)</tt> is \c true, otherwise returns
     * \c false.
     */
    bool contains(const T* a) const {
        return std::find_if(begin(), end(), rw_deref_match<const T*>(a)) != end();
    }

    /**
     * Returns a pointer to the first element of the vector. The value returned
     * is undefined if the vector is empty.
     */
    pointer data() {
        return (pointer)RWPtrVector::data();
    }

    /**
     * @copydoc data()
     */
    const_pointer data() const {
        return (const_pointer)RWPtrVector::data();
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
        return cbegin() + entries();
    }

    /**
     * Returns the number of items in self.
     */
    size_type entries() const {
        return RWPtrVector::entries();
    }

    /**
     * If there exists an element \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, returns \c t. Otherwise,
     * returns #rwnil. Client data may be passed through parameter \a d.
     */
    value_type find(bool(*fn)(value_type, void*), void* d) const {
        const_iterator ret = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        return (ret != end()) ? *ret : rwnil;
    }

    // see doxygen bug #627884
    /**
     * If there exists an element \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, returns \c t. Otherwise,
     * returns #rwnil. Client data may be passed through parameter \a d.
     */
    value_type find(bool(*fn)(const T*, void*), void* d) const {
        const_iterator ret = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        return (ret != end()) ? *ret : rwnil;
    }

    /**
     * If there exists an element \c t in self such that the expression
     * <tt>(*t == *\a a)</tt> is \c true, returns \c t. Otherwise,
     * returns #rwnil.
     */
    value_type find(const T* a) const {
        const_iterator ret = std::find_if(begin(), end(), rw_deref_match<const T*>(a));
        return (ret != end()) ? *ret : rwnil;
    }

    /**
     * Returns a reference to the first item in the collection.
     *
     * Calling this function on an empty collection results in
     * undefined behavior.
     */
    reference first() {
        RW_PRECONDITION(!isEmpty());
        return ((pointer)array_)[0];
    }

    /**
     * @copydoc first()
     */
    const_reference first() const {
        RW_PRECONDITION(!isEmpty());
        return ((const_pointer)array_)[0];
    }

    /**
     * Returns the position of the first item \c t in self such that
     * <tt>(*t == *\a a)</tt>, or returns #RW_NPOS if no such item exists.
     */
    size_type index(const T* a) const {
        const_iterator res = std::find_if(begin(), end(), rw_deref_match<const T*>(a));
        return res != end() ? res - begin() : RW_NPOS;
    }

    /**
     * Returns the position of the first item \c t in self such that
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, or returns #RW_NPOS
     * if no such item exists. Client data may be passed through
     * parameter \a d.
     */
    size_type index(bool(*fn)(value_type, void*), void* d) const {
        const_iterator res = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        return res != end() ? res - begin() : RW_NPOS;
    }

    /**
     * Returns the position of the first item \c t in self such that
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, or returns #RW_NPOS
     * if no such item exists. Client data may be passed through
     * parameter \a d.
     */
    size_type index(bool(*fn)(const T*, void*), void* d) const {
        const_iterator res = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        return res != end() ? res - begin() : RW_NPOS;
    }

    /**
     * Returns \c true if there are no items in the collection, otherwise
     * \c false.
     */
    bool isEmpty() const {
        return 0 == entries();
    }

    /**
     * Returns a reference to the last item in the collection.
     *
     * Calling this function on an empty collection results in
     * undefined behavior.
     */
    reference last() {
        RW_PRECONDITION(!isEmpty());
        return ((pointer)array_)[npts_ - 1];
    }

    /**
     * @copydoc last()
     */
    const_reference last() const {
        RW_PRECONDITION(!isEmpty());
        return ((const_pointer)array_)[npts_ - 1];
    }

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * Returns the number of items in self.
     *
     * @deprecated As of SourcePro 13, use entries() instead.
     */
    RW_DEPRECATE_FUNC("Use RWTPtrVector::entries() instead")
    size_type length() const {
        return RWPtrVector::entries();
    }
#endif

    /**
     * Returns a reference to the maximum element in self.
     */
    reference maxElement() {
        RW_PRECONDITION(entries() != 0);
        return *std::max_element(begin(), end(), deref_less());
    }

    /**
     * @copydoc maxElement()
     */
    const_reference maxElement() const {
        RW_PRECONDITION(entries() != 0);
        return *std::max_element(begin(), end(), deref_less());
    }

    /**
     * Returns a reference to the minimum element in self.
     */
    reference minElement() {
        RW_PRECONDITION(entries() != 0);
        return *std::min_element(begin(), end(), deref_less());
    }

    /**
     * @copydoc minElement()
     */
    const_reference minElement() const {
        RW_PRECONDITION(entries() != 0);
        return *std::min_element(begin(), end(), deref_less());
    }

    /**
     * Returns the number of elements \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true. Client data may be passed
     * through parameter \a d.
     */
    size_type occurrencesOf(bool(*fn)(value_type, void*), void* d) const {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Returns the number of elements \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true. Client data may be passed
     * through parameter \a d.
     */
    size_type occurrencesOf(bool(*fn)(const T*, void*), void* d) const {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Returns the number of elements \c t in self such that the expression
     * <tt>(*t == *\a a)</tt> is \c true.
     */
    size_type occurrencesOf(const T* a) const {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_if(begin(), end(), rw_deref_match<const T*>(a));
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
        return const_reference(array_[i]);
    }

    /**
     * Replaces with \a newVal all elements \c t in self such that the
     * expression <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns the
     * number of items replaced. Client data may be passed through
     * parameter \c d.
     */
    size_type replaceAll(bool(*fn)(value_type, void*), void* d, value_type newVal) {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_and_replace_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d), newVal);
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Replaces with \a newVal all elements \c t in self such that the
     * expression <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns the
     * number of items replaced. Client data may be passed through
     * parameter \c d.
     */
    size_type replaceAll(bool(*fn)(const T*, void*), void* d, value_type newVal) {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_and_replace_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d), newVal);
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Replaces with \a newVal all elements \c t in self such that the
     * expression <tt>(*t == *\a oldVal)</tt> is \c true. Returns the
     * number of items replaced.
     */
    size_type replaceAll(const T* oldVal, value_type newVal) {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_and_replace_if(begin(), end(), rw_deref_match<const T*>(oldVal), newVal);
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Changes the length of the vector to \a N. If this increases the
     * length of the vector, then the initial value of the additional
     * elements is undefined.
     */
    void reshape(size_type N) {
        RWPtrVector::reshape(N);
    }

    /**
     * Changes the length of the vector to \a N. If this increases the
     * length of the vector, then the initial value of the
     * additional elements is set to #rwnil.
     */
    void resize(size_type N) {
        RWPtrVector::resize(N);
    }

    /**
     * Sorts the collection using the less-than operator to compare
     * elements. Elements are dereferenced before being compared.
     */
    void sort() {
        std::sort(begin(), end(), deref_less());
    }

    /**
     * Returns the \a i <sup>th</sup> value in the vector. The index
     * \a i must be between zero and the length of the vector, less
     * one. No bounds checking is performed.
     */
    reference operator()(size_type i) {
        return (reference)RWPtrVector::operator()(i);
    }

    // see doxygen bug #612458
    /**
     * Returns the \a i <sup>th</sup> value in the vector. The index
     * \a i must be between zero and the length of the vector, less
     * one. No bounds checking is performed.
     */
    const_reference operator()(size_type i) const {
        return (const_reference)RWPtrVector::operator()(i);
    }

    /**
     * Returns the \a i <sup>th</sup> value in the vector.
     *
     * @throw RWBoundsErr Thrown if the index \a i is not between
     * zero and the length of the vector, less one.
     */
    reference operator[](size_type i) {
        return (reference)RWPtrVector::operator[](i);
    }

    /**
     * @copydoc operator[](size_t)
     */
    const_reference operator[](size_type i) const {
        return (const_reference)RWPtrVector::operator[](i);
    }

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWTPtrVector<T>& rhs) {
        RWPtrVector::swap(rhs);
    }

};

/**
 * @relates RWTPtrVector
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and
 * iterating through both collections produces individual elements that,
 * in turn, compare equal to each other. Elements are dereferenced before
 * being compared.
 */
template <class T>
bool operator==(const RWTPtrVector<T>& lhs, const RWTPtrVector<T>& rhs)
{
    if (lhs.entries() != rhs.entries()) {
        return false;
    }
    else if (rhs.isEmpty()) {
        return true;
    }

    return std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrVector<T>::deref_equal());
}

/**
 * @relates RWTPtrVector
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T>
bool operator!=(const RWTPtrVector<T>& lhs, const RWTPtrVector<T>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrVector
 *
 * Returns \c true if \a lhs is lexicographically less than \a rhs,
 * otherwise \c false. Items in each collection are dereferenced before
 * being compared. Assumes that type \c T has well-defined less-than
 * semantics.
 */
template <class T>
bool operator<(const RWTPtrVector<T>& lhs, const RWTPtrVector<T>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrVector<T>::deref_less());
}

/**
 * @relates RWTPtrVector
 *
 * Equivalent to <tt>(\a rhs < \a lhs)</tt>.
 */
template <class T>
bool operator>(const RWTPtrVector<T>& lhs, const RWTPtrVector<T>& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrVector
 *
 * Equivalent to <tt>!(\a rhs < \a lhs)</tt>.
 */
template <class T>
bool operator<=(const RWTPtrVector<T>& lhs, const RWTPtrVector<T>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrVector
 *
 * Equivalent to <tt>!(\a lhs < \a rhs)</tt>.
 */
template <class T>
bool operator>=(const RWTPtrVector<T>& lhs, const RWTPtrVector<T>& rhs)
{
    return !(lhs < rhs);
}

#include <rw/defs.h>
#include <rw/edefs.h>
RW_DECLARE_PERSISTABLE_TEMPLATE(RWTPtrVector)

#ifdef RW_NO_TEMPLINST_ON_BASE
class RW_TOOLS_GLOBAL RWpistream;
class RW_TOOLS_GLOBAL RWpostream;
class RW_TOOLS_GLOBAL RWbistream;
class RW_TOOLS_GLOBAL RWbostream;
class RW_TOOLS_GLOBAL RWeistream;
class RW_TOOLS_GLOBAL RWeostream;
RW_DECLARE_PERSIST_TEMPLATE_IO(RWTPtrVector, RWpistream, RWpostream)
RW_DECLARE_PERSIST_TEMPLATE_IO(RWTPtrVector, RWbistream, RWbostream)
RW_DECLARE_PERSIST_TEMPLATE_IO(RWTPtrVector, RWeistream, RWeostream)
#endif

template <class T, class S>
void rwSaveGuts(S& str, const RWTPtrVector<T>& c);

template <class T, class S>
void rwRestoreGuts(S& str, RWTPtrVector<T>& c);


#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tpvector.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tpvector.cc"
#endif


#endif /* RW_TOOLS_TPVECTOR_H */
