#ifndef RW_TOOLS_TPORDVEC_H
#define RW_TOOLS_TPORDVEC_H

/**********************************************************************
 *
 *     : pointer-based ordered vector wrapper for STL vector
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tpordvec.h#2 $
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

#if defined(RW_CLASSIC_TEMPLATE_CONTAINERS)
#  include "rw/xpordvec.h"
#else
#  include <rw/epaux.h>
#  include <rw/epersist.h>
#  include <rw/epfunc.h>
#  include <rw/tools/algorithm.h>
#  include <rw/tools/traits/RWTEnableIf.h>
#  include <rw/tools/traits/RWTIsIntegral.h>

#  include <algorithm>
#  include <vector>

/**
 * @ingroup stl_based_collection_classes
 *
 * @brief A pointer-based collection of values implemented
 * as an ordered vector.
 *
 * This class maintains a pointer-based collection of values, implemented
 * as a vector. Class \c T is the type pointed to by the items in
 * the collection. Class \c A is an allocator of objects of
 * class \c T. For more information about using custom allocators, please
 * see the <i>Essential Tools Module User's Guide</i>.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tpordvec.h>
 * RWTPtrOrderedVector<T,A> ordvec;
 * @endcode
 *
 * @section related Related Classes
 *
 * Classes \link RWTPtrDeque RWTPtrDeque<T,A>\endlink,
 * \link RWTPtrSlist RWTPtrSlist<T,A>\endlink, and
 * \link RWTPtrDlist RWTPtrDlist<T,A>\endlink
 * also provide a Rogue Wave pointer-based interface to C++ Standard
 * sequence collections.
 *
 * Class <tt>vector<T*,A<T*> ></tt> is the C++ Standard Library collection
 * that serves as the underlying implementation for this class.
 *
 * @section persistence Persistence
 *
 * Isomorphic
 *
 * @section example Example
 *
 * @code
 *
 * //
 * // tporddat.cpp
 * //
 * #include <iostream>
 * #include <rw/tpordvec.h>
 * #include <rw/tools/datetime.h>
 *
 * int main ()
 * {
 *     RWTPtrOrderedVector<RWDateTime> week(7);
 *
 *     RWDateTime begin(RWDateTime::setCurrentTime); // Today's date
 *
 *     for (size_t i = 0; i < 7; ++i) {
 *         week.insert(new RWDateTime(begin));
 *         begin.incrementDay(1);
 *     }
 *
 *     for (size_t i = 0; i < 7; ++i) {
 *         std::cout << week[i]->asString('x') << std::endl;
 *     }
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Output (if program is run on
 * June 18, 2009):
 *
 * @code
 * 06/18/09
 * 06/19/09
 * 06/20/09
 * 06/21/09
 * 06/22/09
 * 06/23/09
 * 06/24/09
 * @endcode
 *
 */
template <class T, class A = std::allocator<T*> >
class RWTPtrOrderedVector
{
public:

    /**
     * A type representing the underlying implementation container.
     */
    typedef std::vector<T*, A> container_type;

    /**
     * A type representing the allocator type for the container.
     */
    typedef typename container_type::allocator_type allocator_type;

    /**
     * A type representing the container's data type.
     */
    typedef typename container_type::value_type value_type;

    /**
     * An unsigned integral type used for counting the number of elements
     * in the container.
     */
    typedef typename container_type::size_type size_type;

    /**
     * A signed integral type used to indicate the distance between two
     * valid iterators on the same container.
     */
    typedef typename container_type::difference_type difference_type;

    /**
     * A type that provides a reference to an element in the container.
     */
    typedef typename container_type::reference reference;

    /**
     * A type that provides a \c const reference to an element in the
     * container.
     */
    typedef typename container_type::const_reference const_reference;

    /**
     * A type that provides a pointer to an element in the container.
     */
    typedef typename container_type::pointer pointer;

    /**
     * A type that provides a \c const pointer to an element in the
     * container.
     */
    typedef typename container_type::const_pointer const_pointer;

    /**
     * A type that provides a random-access iterator over the elements
     * in the container.
     */
    typedef typename container_type::iterator iterator;

    /**
     * A type that provides a \c const random-access iterator over the
     * elements in the container.
     */
    typedef typename container_type::const_iterator const_iterator;

    /**
     * A type that provides a random-access, reverse-order iterator over
     * the elements in the container.
     */
    typedef typename container_type::reverse_iterator reverse_iterator;

    /**
     * A type that provides a \c const random-access, reverse-order iterator
     * over the elements in the container.
     */
    typedef typename container_type::const_reverse_iterator const_reverse_iterator;


    /**
     * @internal
     * A type representing the same type as self.
     */
    typedef RWTPtrOrderedVector<T, A> this_type;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12.5, use <tt>const T*</tt> instead.
     */
    typedef RW_DEPRECATE_TYPE("Use const T* instead") const T* const_value;
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
     * Returns a reference to the underlying C++ Standard Library collection
     * that serves as the implementation for self.
     */
    container_type& std() {
        return RW_EXPOSE(impl_);
    }

    /**
     * @copydoc std()
     */
    const container_type& std() const {
        return impl_;
    }

    /**
     * Adds the item \a a to the end of the collection.
     */
    void append(value_type a) {
        std().insert(std().end(), a);
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
#if !defined(RW_BROKEN_STD_VECTOR_ASSIGN_RANGE)
        std().assign(first, last);
#else
        typedef typename rw_iterator_traits<InputIterator>::iterator_category cat;
        assign(first, last, cat());
#endif
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
        return std().begin();
    }

    /**
     * @copydoc begin()
     */
    const_iterator begin() const {
        return std().begin();
    }

    /**
     * @copydoc begin()
     */
    const_iterator cbegin() const {
        return std().begin();
    }

    /**
     * Clears the collection by removing all items from self. Does not
     * call <tt>operator delete</tt> on the individual items.
     */
    void clear() {
        std().clear();
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

    // see doxygen bug #627884
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
     * Returns an iterator referring to the element after the last element
     * in the container.
     *
     * Dereferencing the iterator returned by this function results in
     * undefined behavior.
     */
    iterator end() {
        return std().end();
    }

    /**
     * @copydoc end()
     */
    const_iterator end() const {
        return std().end();
    }

    /**
     * @copydoc end()
     */
    const_iterator cend() const {
        return std().end();
    }

    /**
     * Returns the number of elements in self.
     */
    size_type entries() const {
        return std().size();
    }

    /**
     * Removes the item pointed to by \a pos from the collection. Returns an
     * iterator that points to the next item in the collection, or #end() if
     * the last item in the collection was removed.
     */
    iterator erase(iterator pos) {
        return std().erase(pos);
    }

    /**
     * Removes the items in the range [\a first, \a last) from the
     * collection. Returns an iterator that points to the next item in the
     * collection, or #end() if the last item in the collection was removed.
     */
    iterator erase(iterator first, iterator last) {
        return std().erase(first, last);
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
        return std().front();
    }

    /**
     * @copydoc first()
     */
    const_reference first() const {
        RW_PRECONDITION(!isEmpty());
        return std().front();
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

    // see doxygen bug #627884
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
     * Adds the item \a a to the end of the collection. Returns
     * \c true.
     */
    bool insert(value_type a) {
        std().insert(std().end(), a);
        return true;
    }

    /**
     * Inserts \a val into self before the element at position \a pos.
     * Returns an iterator for the newly inserted element.
     */
    iterator insert(iterator pos, value_type val) {
        return std().insert(pos, val);
    }

    /**
     * Inserts \a n instances of \a val into self before the element at
     * position \a pos.
     */
    void insert(iterator pos, size_type n, value_type val) {
        std().insert(pos, n, val);
    }

    /**
     * Inserts the elements in the range [\a first, \a last) into self
     * before the element at position \a pos.
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
#if !defined(RW_BROKEN_STD_VECTOR_INSERT_RANGE)
    template <typename InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last) {
        std().insert(pos, first, last);
    }
#else
    template <typename InputIterator>
    typename RWTEnableIf < !RWTIsIntegral<InputIterator>::value >::type
    insert(iterator pos, InputIterator first, InputIterator last) {
        typedef typename rw_iterator_traits<InputIterator>::iterator_category cat;
        insert(pos, first, last, cat());
    }
#endif

    /**
     * Inserts the item \a a in front of the item at position \a i in
     * self.
     * @throw RWBoundsErr Thrown if this position is not between
     * zero and the number of entries in the collection.
     */
    void insertAt(size_type i, value_type a) {
        if (i > 0) {
            boundsCheck(i - 1);
        }

        iterator iter = begin();
        std::advance(iter, i);
        std().insert(iter, a);
    }

    /**
     * Returns \c true if there are no items in the collection, otherwise
     * \c false.
     */
    bool isEmpty() const {
        return std().empty();
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

    // see doxygen bug #627884
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
     * Returns a reference to the \a i <sup>th</sup> element of self.
     * Index \a i should be between \c 0 and one less than the number
     * of entries, otherwise the results are undefined.
     * @note
     * No bounds checking is performed.
     */
    reference operator()(size_type i) {
#ifdef RW_DEBUG
        boundsCheck(i);
#endif
        return std()[i];
    }

    // see doxygen bug #612458
    /**
     * Returns a reference to the \a i <sup>th</sup> element of self.
     * Index \a i should be between \c 0 and one less than the number
     * of entries, otherwise the results are undefined.
     * @note
     * No bounds checking is performed.
     */
    const_reference operator()(size_type i) const {
#ifdef RW_DEBUG
        boundsCheck(i);
#endif
        return std()[i];
    }

    /**
     * Returns a reference to the \a i <sup>th</sup> element of self.
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0 and one
     * less than the number of entries in self.
     */
    reference operator[](size_type i) {
        boundsCheck(i);
        return std()[i];
    }

    /**
     * @copydoc operator[](size_type)
     */
    const_reference operator[](size_type i) const {
        boundsCheck(i);
        return std()[i];
    }

    /**
     * Adds the item \a a to the beginning of the collection.
     */
    void prepend(value_type a) {
        std().insert(begin(), a);
    }

    /**
     * Returns an iterator referring to the last element in the container.
     *
     * If the container is empty, returns rend().
     */
    reverse_iterator rbegin() {
        return std().rbegin();
    }

    /**
     * @copydoc rbegin()
     */
    const_reverse_iterator rbegin() const {
        return std().rbegin();
    }

    /**
     * @copydoc rbegin()
     */
    const_reverse_iterator crbegin() const {
        return std().rbegin();
    }

    /**
     * Removes and returns the first element \c t in self such that
     * the expression <tt>(*t == *\a a)</tt> is \c true. Returns
     * #rwnil if there is no such element.
     */
    value_type remove(const T* a) {
        value_type ret = rwnil;
        iterator iter = std::find_if(begin(), end(), rw_deref_match<const T*>(a));
        if (iter != end()) {
            ret = *iter;
            std().erase(iter);
        }
        return ret;
    }

    /**
     * Removes and returns the first element \c t in self such that
     * the expression <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns
     * #rwnil if there is no such element. Client data may be passed
     * through parameter \a d.
     */
    value_type remove(bool(*fn)(value_type, void*), void* d) {
        value_type ret = rwnil;
        iterator iter = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        if (iter != end()) {
            ret = *iter;
            std().erase(iter);
        }
        return ret;
    }

    // see doxygen bug #627884
    /**
     * Removes and returns the first element \c t in self such that
     * the expression <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns
     * #rwnil if there is no such element. Client data may be passed
     * through parameter \a d.
     */
    value_type remove(bool(*fn)(const T*, void*), void* d) {
        value_type ret = rwnil;
        iterator iter = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        if (iter != end()) {
            ret = *iter;
            std().erase(iter);
        }
        return ret;
    }

    /**
     * Removes all elements \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns the number
     * of items removed. Client data may be passed through parameter
     * \a d.
     */
    size_type removeAll(bool(*fn)(value_type, void*), void* d) {
        size_type size = entries();
        iterator iter = std::remove_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        std().erase(iter, end());
        return size - entries();
    }
    // see doxygen bug #627884
    /**
     * Removes all elements \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns the number
     * of items removed. Client data may be passed through parameter
     * \a d.
     */
    size_type removeAll(bool(*fn)(const T*, void*), void* d) {
        size_type size = entries();
        iterator iter = std::remove_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        std().erase(iter, end());
        return size - entries();
    }

    /**
     * Removes all elements \c t in self such that the expression
     * <tt>(*t == *\a a)</tt> is \c true. Returns the number of
     * items removed.
     */
    size_type removeAll(const T* a) {
        size_type size = entries();
        iterator iter = std::remove_if(begin(), end(), rw_deref_match<const T*>(a));
        std().erase(iter, end());
        return size - entries();
    }

    /**
     * Removes and returns the item at position \a i in self.
     * @throw RWBoundsErr Thrown if this position is not between
     * zero and one less than the number of entries in the collection.
     */
    value_type removeAt(size_type i) {
        boundsCheck(i);
        iterator iter = begin();
        std::advance(iter, i);
        value_type ret = *iter;
        std().erase(iter);
        return ret;
    }

    /**
     * Removes and returns the first item in the collection.
     *
     * Calling this function on an empty collection results in
     * undefined behavior.
     */
    value_type removeFirst() {
        RW_PRECONDITION(!isEmpty());
        const value_type ret = first();
        erase(begin());
        return ret;
    }

    /**
     * Returns an iterator referring to the element before the first element
     * in the container.
     *
     * Dereferencing the iterator returned by this function results in
     * undefined behavior.
     */
    reverse_iterator rend() {
        return std().rend();
    }

    /**
     * @copydoc rend()
     */
    const_reverse_iterator rend() const {
        return std().rend();
    }

    /**
     * @copydoc rend()
     */
    const_reverse_iterator crend() const {
        return std().rend();
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

    // see doxygen bug #627884
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
     * Returns a pointer to the first element of the vector. The value returned
     * is undefined if the vector is empty.
     */
    pointer data() {
#if !defined(RW_NO_STD_VECTOR_DATA)
        // this is the best implementation, but it relies on c++11 behavior
        return std().data();
#else
        // this implementation avoids undefined behavior if vector is empty
        return std().empty() ? 0 : &std().front();
#endif
    }

    /**
     * @copydoc data()
     */
    const_pointer data() const {
#if !defined(RW_NO_STD_VECTOR_DATA)
        return std().data();
#else
        return std().empty() ? 0 : &std().front();
#endif
    }

    /**
     * Returns a reference to the last item in the collection.
     *
     * Calling this function on an empty collection results in
     * undefined behavior.
     */
    reference last() {
        RW_PRECONDITION(!isEmpty());
        return std().back();
    }

    /**
     * @copydoc last()
     */
    const_reference last() const {
        RW_PRECONDITION(!isEmpty());
        return std().back();
    }

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * Returns the number of elements in self.
     *
     * @deprecated As of SourcePro 12.5, use entries() instead.
     */
    RW_DEPRECATE_FUNC("Use RWTPtrOrderedVector::entries() instead")
    size_type length() const {
        return std().size();
    }
#endif

    /**
     * Modifies the capacity of the vector to be at least as large as
     * \a n. The function has no effect if the capacity is already
     * as large as \a n.
     */
    void resize(size_t n) {
        std().reserve(n);
    }

    /**
     * Returns the maximum number of elements that can be stored in
     * self without first resizing.
     */
    size_type capacity() const {
        return std().capacity();
    }

    /**
     * Removes and returns the last item in the collection.
     *
     * Calling this function on an empty collection results in
     * undefined behavior.
     */
    value_type removeLast() {
        RW_PRECONDITION(!isEmpty());
        const value_type ret = last();
        std().pop_back();
        return ret;
    }

    /**
     * Sorts the collection using the less-than operator to compare
     * elements. Elements are dereferenced before being compared.
     */
    void sort() {
        std::sort(begin(), end(), deref_less());
    }

    /**
     * Clears all elements of self and replaces them by copying all
     * elements of \a rhs.
     */
    RWTPtrOrderedVector<T, A>&
    operator=(const RWTPtrOrderedVector<T, A>& rhs) {
        return operator=(rhs.impl_);
    }

    /**
     * @copydoc operator=()
     */
    RWTPtrOrderedVector<T, A>&
    operator=(const container_type& rhs) {
        if (&impl_ != &rhs) {
#  if defined(_RWSTD_VER) && (_RWSTD_VER < 0x02020300)
            // workaround vector assignment operator bug
            container_type(rhs).swap(impl_);
#  else
            impl_ = rhs;
#  endif
        }
        return *this;
    }

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrOrderedVector<T, A>&
    operator=(RWTPtrOrderedVector<T, A> && rhs) {
        return operator=(rw_move(rhs.impl_));
    }

    /**
     * @copydoc operator=(RWTPtrOrderedVector<T,A>&&)
     */
    RWTPtrOrderedVector<T, A>&
    operator=(container_type && rhs) {
        if (&impl_ != &rhs) {
#    if (defined(_RWSTD_VER) && (_RWSTD_VER < 0x02020300)) || \
         defined(RW_NO_STDVECTOR_RVALUE_SUPPORT)
            // workaround vector assignment operator bug
            rhs.swap(impl_);
#    else
            impl_ = rw_move(rhs);
#    endif
        }
        return *this;
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs an empty vector.
     */
    RWTPtrOrderedVector() : impl_() {}

    /**
     * Constructs an ordered vector by copying all elements of \a vec.
     */
    RWTPtrOrderedVector(const container_type& vec) : impl_(vec) {}

    /**
     * Copy constructor.
     */
    RWTPtrOrderedVector(const RWTPtrOrderedVector<T, A>& vec)
        : impl_(vec.impl_) {}

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed vector takes ownership of the
     * data owned by \a vec.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrOrderedVector(RWTPtrOrderedVector<T, A> && vec)
#    if !defined(RW_NO_STDVECTOR_RVALUE_SUPPORT)
        : impl_(rw_move(vec.impl_)) { }
#    else
    {
        vec.std().swap(impl_);
    }
#    endif

    /**
     * @copydoc RWTPtrOrderedVector(RWTPtrOrderedVector<T,A>&&)
     */
    RWTPtrOrderedVector(container_type && vec)
#    if !defined(RW_NO_STDVECTOR_RVALUE_SUPPORT)
        : impl_(rw_move(vec)) { }
#    else
    {
        vec.swap(impl_);
    }
#    endif
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs a vector with \a n elements, each initialized to
     * \a a.
     */
    RWTPtrOrderedVector(size_type n, value_type a) : impl_(n, a) {}

    /**
     * Constructs a double-ended queue by copying elements from the range
     * [\a first, \a last).
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
#if !defined(RW_BROKEN_STD_VECTOR_CTOR_RANGE)
    template <typename InputIterator>
    RWTPtrOrderedVector(InputIterator first, InputIterator last)
        : impl_(first, last) { }
#else
    template <typename InputIterator>
    RWTPtrOrderedVector(InputIterator first, InputIterator last,
                        typename RWTEnableIf < !RWTIsIntegral<InputIterator>::value >::type** = 0)
        : impl_() {
        assign(first, last);
    }
#endif

    // For backward compatibility (not particularly efficient):
    RWTPtrOrderedVector(size_type n) {
        resize(n);
    }

    // ~RWTPtrOrderedVector(); // handled by compiler-generated destructor

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWTPtrOrderedVector<T, A>& rhs) {
        std().swap(rhs.impl_);
    }

private:
    void boundsCheck(size_type i) const {
        if (!(i < std().size())) {
            RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEXERR, i, entries())));
        }
    }

#if defined(RW_BROKEN_STD_VECTOR_ASSIGN_RANGE)
    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last, const std::input_iterator_tag&) {
        clear();
        insert(end(), first, last);
    }

#  if !defined(RW_BROKEN_STD_VECTOR_ASSIGN_RANGE_RANDOM_ACCESS)
    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last, const std::random_access_iterator_tag&) {
        std().assign(first, last);
    }
#  endif
#endif

#if defined(RW_BROKEN_STD_VECTOR_INSERT_RANGE)
    template <typename InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last, const std::input_iterator_tag&) {
        while (first != last) {
            pos = insert(pos, *(first++));
            ++pos;
        }
    }

#  if !defined(RW_BROKEN_STD_VECTOR_INSERT_RANGE_RANDOM_ACCESS)
    template <typename InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last, const std::random_access_iterator_tag&) {
        std().insert(pos, first, last);
    }
#  else
    template <typename InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last, const std::random_access_iterator_tag&) {
        difference_type offset = pos - begin();
        std().reserve(std().capacity() + (last - first));
        pos = begin();
        std::advance(pos, offset);
        while (first != last) {
            pos = insert(pos, *(first++));
            ++pos;
        }
    }
#  endif
#endif

    container_type impl_;
};


/**
 * @relates RWTPtrOrderedVector
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and
 * iterating through both collections produces individual elements that,
 * in turn, compare equal to each other. Elements are dereferenced before
 * being compared.
 */
template <class T , class A>
bool operator==(const RWTPtrOrderedVector<T, A>& lhs, const RWTPtrOrderedVector<T, A>& rhs)
{
    return lhs.entries() == rhs.entries() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrOrderedVector<T, A>::deref_equal());
}

/**
 * @relates RWTPtrOrderedVector
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T , class A>
bool operator!=(const RWTPtrOrderedVector<T, A>& lhs, const RWTPtrOrderedVector<T, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrOrderedVector
 *
 * Returns \c true if \a lhs is lexicographically less than \a rhs,
 * otherwise \c false. Items in each collection are dereferenced before
 * being compared. Assumes that type \c T has well-defined less-than
 * semantics.
 */
template <class T , class A>
bool operator<(const RWTPtrOrderedVector<T, A>& lhs, const RWTPtrOrderedVector<T, A>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrOrderedVector<T, A>::deref_less());
}

/**
 * @relates RWTPtrOrderedVector
 *
 * Equivalent to <tt>(\a rhs < \a lhs)</tt>.
 */
template <class T , class A>
bool operator>(const RWTPtrOrderedVector<T, A>& lhs, const RWTPtrOrderedVector<T, A>& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrOrderedVector
 *
 * Equivalent to <tt>!(\a rhs < \a lhs)</tt>.
 */
template <class T , class A>
bool operator<=(const RWTPtrOrderedVector<T, A>& lhs, const RWTPtrOrderedVector<T, A>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrOrderedVector
 *
 * Equivalent to <tt>!(\a lhs < \a rhs)</tt>.
 */
template <class T , class A>
bool operator>=(const RWTPtrOrderedVector<T, A>& lhs, const RWTPtrOrderedVector<T, A>& rhs)
{
    return !(lhs < rhs);
}

/**
 * @relates RWTPtrOrderedVector
 *
 * @copydoc operator==(const RWTPtrOrderedVector&, const RWTPtrOrderedVector&);
 */
template <class T , class A>
bool operator==(const RWTPtrOrderedVector<T, A>& lhs,
                const typename RWTPtrOrderedVector<T, A>::container_type& rhs)
{
    return lhs.entries() == rhs.size() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrOrderedVector<T, A>::deref_equal());
}

/**
 * @relates RWTPtrOrderedVector
 *
 * @copydoc operator!=(const RWTPtrOrderedVector&, const RWTPtrOrderedVector&);
 */
template <class T , class A>
bool operator!=(const RWTPtrOrderedVector<T, A>& lhs,
                const typename RWTPtrOrderedVector<T, A>::container_type& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrOrderedVector
 *
 * @copydoc operator<(const RWTPtrOrderedVector&, const RWTPtrOrderedVector&);
 */
template <class T , class A>
bool operator<(const RWTPtrOrderedVector<T, A>& lhs,
               const typename RWTPtrOrderedVector<T, A>::container_type& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrOrderedVector<T, A>::deref_less());
}

/**
 * @relates RWTPtrOrderedVector
 *
 * @copydoc operator>(const RWTPtrOrderedVector&, const RWTPtrOrderedVector&);
 */
template <class T , class A>
bool operator>(const RWTPtrOrderedVector<T, A>& lhs,
               const typename RWTPtrOrderedVector<T, A>::container_type& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrOrderedVector
 *
 * @copydoc operator<=(const RWTPtrOrderedVector&, const RWTPtrOrderedVector&);
 */
template <class T , class A>
bool operator<=(const RWTPtrOrderedVector<T, A>& lhs,
                const typename RWTPtrOrderedVector<T, A>::container_type& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrOrderedVector
 *
 * @copydoc operator>=(const RWTPtrOrderedVector&, const RWTPtrOrderedVector&);
 */
template <class T , class A>
bool operator>=(const RWTPtrOrderedVector<T, A>& lhs,
                const typename RWTPtrOrderedVector<T, A>::container_type& rhs)
{
    return !(lhs < rhs);
}


/**
 * @relates RWTPtrOrderedVector
 *
 * @copydoc operator==(const RWTPtrOrderedVector&, const RWTPtrOrderedVector&);
 */
template <class T , class A>
bool operator==(const typename RWTPtrOrderedVector<T, A>::container_type& lhs,
                const RWTPtrOrderedVector<T, A>& rhs)
{
    return lhs.size() == rhs.entries() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrOrderedVector<T, A>::deref_equal());
}

/**
 * @relates RWTPtrOrderedVector
 *
 * @copydoc operator!=(const RWTPtrOrderedVector&, const RWTPtrOrderedVector&);
 */
template <class T , class A>
bool operator!=(const typename RWTPtrOrderedVector<T, A>::container_type& lhs,
                const RWTPtrOrderedVector<T, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrOrderedVector
 *
 * @copydoc operator<(const RWTPtrOrderedVector&, const RWTPtrOrderedVector&);
 */
template <class T , class A>
bool operator<(const typename RWTPtrOrderedVector<T, A>::container_type& lhs,
               const RWTPtrOrderedVector<T, A>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrOrderedVector<T, A>::deref_less());
}

/**
 * @relates RWTPtrOrderedVector
 *
 * @copydoc operator>(const RWTPtrOrderedVector&, const RWTPtrOrderedVector&);
 */
template <class T , class A>
bool operator>(const typename RWTPtrOrderedVector<T, A>::container_type& lhs,
               const RWTPtrOrderedVector<T, A>& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrOrderedVector
 *
 * @copydoc operator<=(const RWTPtrOrderedVector&, const RWTPtrOrderedVector&);
 */
template <class T , class A>
bool operator<=(const typename RWTPtrOrderedVector<T, A>::container_type& lhs,
                const RWTPtrOrderedVector<T, A>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrOrderedVector
 *
 * @copydoc operator>=(const RWTPtrOrderedVector&, const RWTPtrOrderedVector&);
 */
template <class T , class A>
bool operator>=(const typename RWTPtrOrderedVector<T, A>::container_type& lhs,
                const RWTPtrOrderedVector<T, A>& rhs)
{
    return !(lhs < rhs);
}

/**
 * @relates RWTPtrOrderedVector
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T , class A>
RWvostream& operator<<(RWvostream& strm, const RWTPtrOrderedVector<T , A>& coll);

/**
 * @relates RWTPtrOrderedVector
 *
 * Saves the collection \a coll onto the RWFile \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T , class A>
RWFile& operator<<(RWFile& strm, const RWTPtrOrderedVector<T , A>& coll);

/**
 * @relates RWTPtrOrderedVector
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class T , class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrOrderedVector<T , A>& coll);

/**
 * @relates RWTPtrOrderedVector
 *
 * Restores the contents of the collection \a coll from the RWFile
 * \a strm.
 */
template <class T , class A>
RWFile& operator>>(RWFile& strm, RWTPtrOrderedVector<T , A>& coll);

/**
 * @relates RWTPtrOrderedVector
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T , class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrOrderedVector<T , A>*& p);

/**
 * @relates RWTPtrOrderedVector
 *
 * Looks at the next object on the RWFile \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T , class A>
RWFile& operator>>(RWFile& strm, RWTPtrOrderedVector<T , A>*& p);

#  if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#    pragma implementation ("tpordvec.cc")
#  endif

#  ifdef RW_NO_IMPLICIT_INCLUSION
#    include "rw/tpordvec.cc"
#  endif

#endif /* RW_CLASSIC_TEMPLATE_CONTAINERS */

#endif /* RW_TOOLS_TPORDVEC_H */
