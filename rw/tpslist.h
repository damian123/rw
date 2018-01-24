#ifndef RW_TOOLS_TPSLIST_H
#define RW_TOOLS_TPSLIST_H

/**********************************************************************
 *
 *     : pointer-based singly-linked list wrapper for STL-compliant rw_slist
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tpslist.h#3 $
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
#  include "rw/xpslist.h"
#else

// STL-based implementation:

#  include <rw/epaux.h>
#  include <rw/epfunc.h>
#  include <rw/rwerr.h>
#  include <rw/stdex/slist.h>
#  include <rw/toolerr.h>
#  include <rw/tools/algorithm.h>

#if !defined(RW_DISABLE_DEPRECATED)

template <class T, class A>
class RWTPtrSlistIterator;

template <class T, class A>
class RWTPtrSlistConstIterator;

#endif

/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @brief A pointer-based collection of values implemented
 * as a singly-linked list.
 *
 * This class maintains a pointer-based collection of values, implemented
 * as a singly-linked list. Class \c T is the type pointed to by the
 * items in the collection. Class \c A is an allocator of objects of
 * class \c T. For more information about using custom allocators, please
 * see the <i>Essential Tools Module User's Guide</i>.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tpslist.h>
 * RWTPtrSlist<T,A> slist;
 * @endcode
 *
 * @section related Related Classes
 *
 * Classes \link RWTPtrDlist RWTPtrDlist<T,A>\endlink,
 * \link RWTPtrDeque RWTPtrDeque<T,A>\endlink, and
 * \link RWTPtrOrderedVector RWTPtrOrderedVector<T,A>\endlink
 * also provide a Rogue Wave pointer-based interface to C++ Standard
 * sequence collections.
 *
 * Class \link rw_slist rw_slist<T*,A>\endlink is the C++ Standard Library
 * collection that serves as the underlying implementation for this class.
 *
 * @section persistence Persistence
 *
 * Isomorphic
 *
 * @section example Example
 *
 * @code
 * //
 * // tpslist.cpp
 * //
 * #include <rw/tpslist.h>
 * #include <rw/tools/datetime.h>
 *
 * int main ()
 * {
 *     RWTPtrSlist<RWDateTime> dates;
 *
 *     dates.insert(new RWDateTime("June 2, 52", RWDateTime::setDate));    // 6/2/52
 *     dates.insert(new RWDateTime("March 30, 46", RWDateTime::setDate));  // 3/30/46
 *     dates.insert(new RWDateTime("April 1, 90", RWDateTime::setDate));   // 4/1/90
 *
 *     // Now look for one of the dates:
 *     RWDateTime key("June 2, 52", RWDateTime::setDate);
 *
 *     RWDateTime* d = dates.find(&key);
 *
 *     if (d)
 *         std::cout << "Found date is: " << d->asString('x') << std::endl;
 *
 *     // Remove in reverse order:
 *     while (!dates.isEmpty()) {
 *         d = dates.removeLast();
 *         std::cout << d->asString('x') << "\n";
 *         delete d;
 *     }
 *     return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 *
 * Found date is: 6/02/52
 * 4/01/90
 * 3/30/46
 * 6/02/52
 * @endcode
 *
 */
template <class T, class A = std::allocator<T*> >
class RWTPtrSlist
{
public:

    /**
     * A type representing the underlying implementation container.
     */
    typedef rw_slist<T*, A> container_type;

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
     * A type that provides a forward iterator over the elements
     * in the container.
     */
    typedef typename container_type::iterator iterator;

    /**
     * A type that provides a \c const forward iterator over the
     * elements in the container.
     */
    typedef typename container_type::const_iterator const_iterator;


    /**
     * @internal
     * A type representing the same type as self.
     */
    typedef RWTPtrSlist<T, A> this_type;

    /**
     * @internal
     * @deprecated As of SourcePro 12.5, use <tt>const T*</tt> instead.
     */
    typedef const T* const_value;

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
        std().assign(first, last);
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
     * Clears the collection by removing all items from self.
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
     * Returns \c true if there exists an element \c t in self such
     * that the expression <tt>((*\a fn)(t,\a d))</tt> is \c true,
     * otherwise returns \c false. Client data may be passed through
     * parameter \a d.
     */
    bool contains(bool(*fn)(value_type, void*), void* d) const {
        return std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d)) != end();
    }

    // see doxygen bug #627884
    /**
     * Returns \c true if there exists an element \c t in self such
     * that the expression <tt>((*\a fn)(t,\a d))</tt> is \c true,
     * otherwise returns \c false. Client data may be passed through
     * parameter \a d.
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
     * @copydoc removeFirst()
     */
    value_type get() {
        RW_PRECONDITION(!isEmpty());
        return removeFirst();
    }

    /**
     * Returns the position of the first item \c t in self such that
     * <tt>(*t == *\a a)</tt>, or returns #RW_NPOS if no such item exists.
     */
    size_type index(const T* a) const;

    /**
     * Returns the position of the first item \c t in self such that
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, or returns #RW_NPOS if
     * no such item exists. Client data may be passed through parameter
     * \a d.
     */
    size_type index(bool(*fn)(value_type, void*), void* d) const;

    // see doxygen bug #627884
    /**
     * Returns the position of the first item \c t in self such that
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, or returns #RW_NPOS if
     * no such item exists. Client data may be passed through parameter
     * \a d.
     */
    size_type index(bool(*fn)(const T*, void*), void* d) const;

    /**
     * Adds the item \a a to the end of the collection. Returns \c true.
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
    template <typename InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last) {
        std().insert(pos, first, last);
    }

    /**
     * Inserts the item \a a in front of the item at position \a i in
     * self.
     * @throw RWBoundsErr Thrown if this position is not between zero
     * and the number of entries in the collection.
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
     * Index \a i must be between \c 0 and one less than the number
     * of entries, otherwise the results are undefined.
     * @note
     * No bounds checking is performed.
     */
    reference operator()(size_type i) {
#ifdef RW_DEBUG
        boundsCheck(i);
#endif
        iterator iter = begin();
        std::advance(iter, i);
        return *iter;
    }

    // see doxygen bug #612458
    /**
     * Returns a reference to the \a i <sup>th</sup> element of self.
     * Index \a i must be between \c 0 and one less than the number
     * of entries, otherwise the results are undefined.
     * @note
     * No bounds checking is performed.
     */
    const_reference operator()(size_type i) const {
#ifdef RW_DEBUG
        boundsCheck(i);
#endif
#ifndef _MSC_VER
        const_iterator iter = begin();
#else
        iterator iter = ((container_type&)std()).begin();
#endif
        std::advance(iter, i);
        return *iter;
    }

    /**
     * Returns a reference to the \a i <sup>th</sup> element of self.
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0 and
     * one less than the number of entries in self.
     */
    reference operator[](size_type i) {
        boundsCheck(i);
        iterator iter = begin();
        std::advance(iter, i);
        return *iter;
    }

    /**
     * @copydoc operator[](size_type)
     */
    const_reference operator[](size_type i) const {
        boundsCheck(i);
#ifndef _MSC_VER
        const_iterator iter = begin();
#else
        iterator iter = ((container_type&)std()).begin();
#endif
        std::advance(iter, i);
        return *iter;
    }

    /**
     * Adds the item \a a to the beginning of the collection.
     */
    void prepend(value_type a) {
        std().insert(begin(), a);
    }

    /**
     * Removes and returns the first element \c t in self such that
     * the expression <tt>(*t == *\a a)</tt> is \c true. Returns #rwnil
     * if there is no such element.
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
     * Removes and returns the first element \c t in self such that the
     * expression <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns #rwnil
     * if there is no such element. Client data may be passed through
     * parameter \a d.
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
     * Removes and returns the first element \c t in self such that the
     * expression <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns #rwnil
     * if there is no such element. Client data may be passed through
     * parameter \a d.
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
     * Removes and returns the item at position \a i in self.
     *
     * @throw RWBoundsErr Thrown if this position is not between
     * zero and one less than the number of entries in the collection.
     */
    value_type removeAt(size_type i) {
        boundsCheck(i);
        iterator iter = begin();
        std::advance(iter, i);
        value_type ret = *iter;
        erase(iter);
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
     * Removes and returns the first item in the collection.
     *
     * Calling this function on an empty collection results in
     * undefined behavior.
     */
    value_type removeFirst() {
        RW_PRECONDITION(!isEmpty());
        return removeAt(0);
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
     * Invokes the function pointer \a fn on each item in the collection.
     * Client data may be passed through parameter \a d.
     */
    void apply(void (*fn)(value_type&, void*), void* d) const {
        this_type& self = const_cast<this_type&>(*this);
        std::for_each(self.begin(), self.end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * @copydoc apply()
     */
    void apply(void (*fn)(value_type, void*), void* d) const {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * @copydoc apply()
     */
    void apply(void (*fn)(const T*, void*), void* d) const {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * Returns a reference to the last item in the collection.
     *
     * Calling this function on an empty collection results in
     * undefined behavior.
     */
    value_type& last() {
        RW_PRECONDITION(!isEmpty());
        return std().back();
    }

    /**
     * @copydoc last()
     */
    const value_type& last() const {
        RW_PRECONDITION(!isEmpty());
        return std().back();
    }

    /**
     * Removes and returns the last item in the collection.
     *
     * Calling this function on an empty collection results in
     * undefined behavior.
     */
    value_type removeLast() {
        RW_PRECONDITION(!isEmpty());
        return removeAt(entries() - 1);
    }

    /**
     * Sorts the collection using the greater-than operator to compare
     * elements so that the elements are sorted from smallest to largest.
     * Elements are dereferenced before being compared.
     */
    void sort();

    /**
     * Empties self, then inserts all elements of \a rhs.
     */
    RWTPtrSlist<T, A>&
    operator=(const RWTPtrSlist<T, A>& rhs) {
        return operator=(rhs.impl_);
    }

    /**
     * @copydoc operator=()
     */
    RWTPtrSlist<T, A>&
    operator=(const rw_slist<T*, A>& rhs) {
        if (&impl_ != &rhs) {
            impl_ = rhs;
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
    RWTPtrSlist<T, A>&
    operator=(RWTPtrSlist<T, A> && rhs) {
        return operator=(rw_move(rhs.impl_));
    }

    /**
     * @copydoc operator=(RWTPtrSlist<T,A>&&)
     */
    RWTPtrSlist<T, A>&
    operator=(rw_slist<T*, A> && rhs) {
        if (&impl_ != &rhs) {
            impl_ = rw_move(rhs);
        }
        return *this;
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs an empty, singly-linked list.
     */
    RWTPtrSlist() : impl_() {}

    /**
     * Constructs a singly-linked list by copying all elements of
     * \a lst.
     */
    RWTPtrSlist(const container_type& lst) : impl_(lst) {}

    /**
     * Copy constructor.
     */
    RWTPtrSlist(const RWTPtrSlist<T, A>& lst)
        : impl_(lst.impl_) {}

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed list takes ownership of the
     * data owned by \a lst.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrSlist(RWTPtrSlist<T, A> && lst)
        : impl_(rw_move(lst.impl_)) { }

    /**
     * @copydoc RWTPtrSlist(RWTPtrSlist<T,A>&&)
     */
    RWTPtrSlist(container_type && lst) : impl_(rw_move(lst)) { }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs a singly-linked list with \a n elements, each initialized
     * to \a a.
     */
    RWTPtrSlist(size_type n, value_type a = 0) : impl_(n, a) {}

    /**
     * Constructs a double-ended queue by copying elements from the range
     * [\a first, \a last).
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
    template <typename InputIterator>
    RWTPtrSlist(InputIterator first, InputIterator last) : impl_(first, last) {}

    // ~RWTPtrSlist(); // handled by compiler-generated destructor

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWTPtrSlist<T, A>& rhs) {
        std().swap(rhs.impl_);
    }

protected:
    //  void sortPairs();
    //  void pointerMerge(iterator, iterator, iterator);

private:
    void boundsCheck(size_type i) const {
        if (i >= entries()) {
            RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEXERR, i, entries())));
        }
    }

    container_type impl_;

#if !defined(RW_DISABLE_DEPRECATED)

    friend class RWTPtrSlistIterator<T, A>;

#endif

};

/**
 * @relates RWTPtrSlist
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and
 * iterating through both collections produces individual elements that,
 * in turn, compare equal to each other. Elements are dereferenced before
 * being compared.
 */
template <class T , class A>
bool operator==(const RWTPtrSlist<T, A>& lhs, const RWTPtrSlist<T, A>& rhs)
{
    return lhs.entries() == rhs.entries() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrSlist<T, A>::deref_equal());
}

/**
 * @relates RWTPtrSlist
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T , class A>
bool operator!=(const RWTPtrSlist<T, A>& lhs, const RWTPtrSlist<T, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrSlist
 *
 * Returns \c true if \a lhs is lexicographically less than \a rhs,
 * otherwise \c false. Items in each collection are dereferenced before
 * being compared. Assumes that type \c T has well-defined less-than
 * semantics.
 */
template <class T , class A>
bool operator<(const RWTPtrSlist<T, A>& lhs, const RWTPtrSlist<T, A>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrSlist<T, A>::deref_less());
}

/**
 * @relates RWTPtrSlist
 *
 * Equivalent to <tt>(\a rhs < \a lhs)</tt>.
 */
template <class T , class A>
bool operator>(const RWTPtrSlist<T, A>& lhs, const RWTPtrSlist<T, A>& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrSlist
 *
 * Equivalent to <tt>!(\a rhs < \a lhs)</tt>.
 */
template <class T , class A>
bool operator<=(const RWTPtrSlist<T, A>& lhs, const RWTPtrSlist<T, A>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrSlist
 *
 * Equivalent to <tt>!(\a lhs < \a rhs)</tt>.
 */
template <class T , class A>
bool operator>=(const RWTPtrSlist<T, A>& lhs, const RWTPtrSlist<T, A>& rhs)
{
    return !(lhs < rhs);
}

/**
 * @relates RWTPtrSlist
 *
 * @copydoc operator==(const RWTPtrSlist&, const RWTPtrSlist&);
 */
template <class T , class A>
bool operator==(const RWTPtrSlist<T, A>& lhs,
                const typename RWTPtrSlist<T, A>::container_type& rhs)
{
    return lhs.entries() == rhs.size() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrSlist<T, A>::deref_equal());
}

/**
 * @relates RWTPtrSlist
 *
 * @copydoc operator!=(const RWTPtrSlist&, const RWTPtrSlist&);
 */
template <class T , class A>
bool operator!=(const RWTPtrSlist<T, A>& lhs,
                const typename RWTPtrSlist<T, A>::container_type& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrSlist
 *
 * @copydoc operator<(const RWTPtrSlist&, const RWTPtrSlist&);
 */
template <class T , class A>
bool operator<(const RWTPtrSlist<T, A>& lhs,
               const typename RWTPtrSlist<T, A>::container_type& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrSlist<T, A>::deref_less());
}

/**
 * @relates RWTPtrSlist
 *
 * @copydoc operator>(const RWTPtrSlist&, const RWTPtrSlist&);
 */
template <class T , class A>
bool operator>(const RWTPtrSlist<T, A>& lhs,
               const typename RWTPtrSlist<T, A>::container_type& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrSlist
 *
 * @copydoc operator<=(const RWTPtrSlist&, const RWTPtrSlist&);
 */
template <class T , class A>
bool operator<=(const RWTPtrSlist<T, A>& lhs,
                const typename RWTPtrSlist<T, A>::container_type& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrSlist
 *
 * @copydoc operator>=(const RWTPtrSlist&, const RWTPtrSlist&);
 */
template <class T , class A>
bool operator>=(const RWTPtrSlist<T, A>& lhs,
                const typename RWTPtrSlist<T, A>::container_type& rhs)
{
    return !(lhs < rhs);
}


/**
 * @relates RWTPtrSlist
 *
 * @copydoc operator==(const RWTPtrSlist&, const RWTPtrSlist&);
 */
template <class T , class A>
bool operator==(const typename RWTPtrSlist<T, A>::container_type& lhs,
                const RWTPtrSlist<T, A>& rhs)
{
    return lhs.size() == rhs.entries() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrSlist<T, A>::deref_equal());
}

/**
 * @relates RWTPtrSlist
 *
 * @copydoc operator!=(const RWTPtrSlist&, const RWTPtrSlist&);
 */
template <class T , class A>
bool operator!=(const typename RWTPtrSlist<T, A>::container_type& lhs,
                const RWTPtrSlist<T, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrSlist
 *
 * @copydoc operator<(const RWTPtrSlist&, const RWTPtrSlist&);
 */
template <class T , class A>
bool operator<(const typename RWTPtrSlist<T, A>::container_type& lhs,
               const RWTPtrSlist<T, A>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrSlist<T, A>::deref_less());
}

/**
 * @relates RWTPtrSlist
 *
 * @copydoc operator>(const RWTPtrSlist&, const RWTPtrSlist&);
 */
template <class T , class A>
bool operator>(const typename RWTPtrSlist<T, A>::container_type& lhs,
               const RWTPtrSlist<T, A>& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrSlist
 *
 * @copydoc operator<=(const RWTPtrSlist&, const RWTPtrSlist&);
 */
template <class T , class A>
bool operator<=(const typename RWTPtrSlist<T, A>::container_type& lhs,
                const RWTPtrSlist<T, A>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrSlist
 *
 * @copydoc operator>=(const RWTPtrSlist&, const RWTPtrSlist&);
 */
template <class T , class A>
bool operator>=(const typename RWTPtrSlist<T, A>::container_type& lhs,
                const RWTPtrSlist<T, A>& rhs)
{
    return !(lhs < rhs);
}

/**
 * @relates RWTPtrSlist
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T , class A>
RWvostream& operator<<(RWvostream& strm, const RWTPtrSlist<T , A>& coll);

/**
 * @relates RWTPtrSlist
 *
 * Saves the collection \a coll onto the RWFile \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T , class A>
RWFile& operator<<(RWFile& strm, const RWTPtrSlist<T , A>& coll);

/**
 * @relates RWTPtrSlist
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class T , class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrSlist<T , A>& coll);

/**
 * @relates RWTPtrSlist
 *
 * Restores the contents of the collection \a coll from the RWFile
 * \a strm.
 */
template <class T , class A>
RWFile& operator>>(RWFile& strm, RWTPtrSlist<T , A>& coll);

/**
 * @relates RWTPtrSlist
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T , class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrSlist<T , A>*& p);

/**
 * @relates RWTPtrSlist
 *
 * Looks at the next object on the RWFile \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T , class A>
RWFile& operator>>(RWFile& strm, RWTPtrSlist<T , A>*& p);

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTPtrSlist::iterator to iterate over
 * the elements in this collection.
 *
 * @brief Deprecated. Provides an iterator for RWTPtrSlist
 *
 * RWTPtrSlistIterator provides an iterator interface to the C++
 * Standard Library-based collection represented by RWTPtrSlist. (An STL
 * iterator interface is provided through \c RWTPtrSlist::iterator.)
 *
 * The order of iteration over an RWTPtrSlist is dependent upon
 * the order of insertion of items into the container.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset(). The iterator becomes
 * valid after being advanced with either a pre-increment or operator()().
 *
 * For both operator++() and operator()(), iterating past the last
 * element will return a value equivalent to boolean \c false.
 * Continued increments will return a value equivalent to \c false
 * until reset() is called.
 *
 * If the \c slist is modified, the iterator is no longer valid.
 * This is generally true for all iterators.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include<rw/tpslist.h>
 * RWTPtrSlist<T,A> dl;
 * RWTPtrSlistIterator<T,A> itr(dl);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/tpslist.h>
 * #include <iostream>
 * #include <rw/cstring.h>
 *
 * int main() {
 *    RWTPtrSlist<RWCString> a;
 *    RWTPtrSlistIterator<RWCString> itr(a);
 *
 *    a.insert(new RWCString("John"));
 *    a.insert(new RWCString("Steve"));
 *    a.insert(new RWCString("Mark"));
 *    a.insert(new RWCString("Steve"));
 *
 *    while(itr()) {
 *      std::cout << *itr.key() << std::endl;
 *    }
 *
 *    a.clearAndDestroy();
 *    return 0;
 * }
 * @endcode
 *
 * Program Output
 *
 * @code
 *
 * John
 * Steve
 * Mark
 * Steve
 * @endcode
 *
 */
template <class T, class A = std::allocator<T*> >
class RW_DEPRECATE_TYPE("Use RWTPtrSlist::iterator instead") RWTPtrSlistIterator
{
private:
    RWTPtrSlist<T, A>* list_;
    typename RWTPtrSlist<T, A>::iterator iter_;
    bool head_;  // simulate head sentinel
    bool tail_;  // simulate tail sentinel
    /*
     * Invariants:
     *   head_ and tail_ can never both be true
     *   if head_ and tail_ are both false then iter_ is valid
     *     (unless invalidated from outside this class)
     */

public:
    /**
     * Creates an iterator for the list \a lst. The iterator begins
     * in an undefined state and must be advanced before the first element
     * will be accessible.
     */
    RWTPtrSlistIterator(RWTPtrSlist<T, A>& lst)
        : list_(&lst), head_(true), tail_(false) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self will now reference the first element.
     * If, before iteration, self referenced the last association in
     * the list, self will now reference an undefined value distinct
     * from the reset value, and a value equivalent to \c false will
     * be returned. Otherwise, a value equivalent to \c true is returned.
     */
    bool operator++();

    /**
     * Behaves as if the operator++() member function had been applied
     * \a n times.
     */
    bool operator+=(size_t n);

    /**
     * Advances self to the next element, dereferences the resulting
     * iterator, and returns its value. If the iterator has advanced
     * past the last item in the container, #rwnil is returned.
     */
    T* operator()() {
        return ++(*this) ? key() : rwnil;
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    RWTPtrSlist<T, A>* container() const {
        return list_;
    }

    /**
     * Returns the first element \c t encountered by iterating self
     * forward, such that the expression <tt>(*t == *\a a)</tt> is \c true. If no
     * such element is found, returns #rwnil. Leaves self referencing
     * the found item or "off the end."
     */
    T* findNext(const T* a);

    /**
     * Returns the first element \c t encountered by iterating self
     * forward such that the expression <tt>((*\a fn)(t,\a d))</tt> is \c true.
     * Client data may be passed through parameter \a d.
     */
    T* findNext(bool (*fn)(T*, void*), void* d);

    /**
     * Inserts the pointer \a p into the container directly after the
     * element pointed to by self. Leaves self referencing the prior
     * item, or in \c reset condition.
     */
    void insertAfterPoint(T* p);

    /**
     * Returns the stored value pointed to by self. Undefined if
     * self is not referencing a value within the list.
     */
    T* key() const {
        RWTPtrSlistIterator<T, A>* tmp =
            const_cast<RWTPtrSlistIterator<T, A>*>(this);
        return *tmp->iter_;
    }

    /**
     * Returns the stored value pointed to by self and removes it
     * from the collection. Undefined if self is not referencing a value
     * within the list. Leaves self referencing the prior item, or in
     * \c reset condition.
     */
    T* remove();

    /**
     * Returns and removes the first element \c t, encountered by iterating
     * self forward, such that the expression <tt>(*t == *\a a)</tt> is \c true.
     * Leaves self referencing the prior item, or in \c reset condition.
     */
    T* removeNext(const T* a);

    /**
     * Returns and removes the first element \c t, encountered by iterating
     * self forward, such that the expression <tt>((*\a fn)(t,\a d))</tt>
     * is \c true. Leaves self referencing the prior item, or in \c reset
     * condition. Client data may be passed through parameter \a d.
     */
    T* removeNext(bool (*fn)(T*, void*), void* d);

    /**
     * Resets the iterator so that after being advanced it will reference
     * the first element of the collection.
     */
    void reset() {
        head_ = true;
        tail_ = false;
    }

    /**
     * Resets the iterator so that after being advanced it will reference
     * the first element of the collection \a s.
     */
    void reset(RWTPtrSlist<T, A>& s) {
        head_ = true;
        tail_ = false;
        list_ = &s;
    }

private:

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);
    friend class RWTPtrSlistConstIterator<T, A>;
};

/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTPtrSlist::const_iterator to iterate
 * over the elements in this collection.
 *
 * @brief Deprecated. Provides a \c const iterator for RWTPtrSlist.
 *
 * RWTPtrSlistConstIterator provides a \c const iterator interface
 * to C++ Standard Library-based RWTPtrSlist collections. A \c const
 * iterator does not change anything in the data structure it iterates
 * over.
 *
 * The order of iteration over an RWTPtrSlist is dependent upon
 * the order of insertion of items into the container.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset(). The iterator becomes
 * valid after being advanced with either a pre-increment or operator()().
 *
 * For both operator++() and operator()(), iterating past the last
 * element returns a value equivalent to boolean \c false. Continued
 * increments return a value equivalent to \c false until reset()
 * is called.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tpslist.h>
 * template <class T, class A=std::allocator<T*> > class
 * RWTPtrSlistConstIterator
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class T, class A = std::allocator<T*> >
class RW_DEPRECATE_TYPE("Use RWTPtrSlist::const_iterator instead") RWTPtrSlistConstIterator
{
private:
    const RWTPtrSlist<T, A>* list_;
    typename RWTPtrSlist<T, A>::const_iterator iter_;
    bool head_;  // simulate head sentinel
    bool tail_;  // simulate tail sentinel
    /*
     * Invariants:
     *   head_ and tail_ can never both be true
     *   if head_ and tail_ are both false then iter_ is valid
     *     (unless invalidated from outside this class)
     */

public:
    /**
     * Creates a \c const iterator for the list \a lst. The iterator
     * begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTPtrSlistConstIterator(const RWTPtrSlist<T, A>& lst)
        : list_(&lst), head_(true), tail_(false) { }

    /**
     * Creates a \c const iterator from the iterator \a itr. The \c const
     * iterator begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTPtrSlistConstIterator(const RWTPtrSlistIterator<T, A>& itr)
        : list_(itr.list_), iter_(itr.iter_),
          head_(itr.head_), tail_(itr.tail_) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self now references the first element.
     * If, before iteration, self referenced the last association in
     * the list, self now references an undefined value distinct from
     * the reset value and a value equivalent to \c false is returned.
     * Otherwise, a value equivalent to \c true is returned.
     */
    bool operator++();

    /**
     * Behaves as if the operator++() member function had been applied
     * \c n times.
     */
    bool operator+=(size_t n);

    /**
     * Advances self to the next element, dereferences the resulting
     * iterator, and returns its value. If the iterator has advanced
     * past the last item in the container, #rwnil is returned.
     */
    const T* operator()() {
        return ++(*this) ? key() : rwnil;
    }

    // Methods
    const RWTPtrSlist<T, A>* container() const {
        return list_;
    }

    /**
     * Returns the first element \c t encountered by iterating self
     * forward, such that the expression (*t&nbsp;==&nbsp;*a) is
     * \c true. If no such element is found, returns #rwnil. Leaves self
     * referencing the found item or "off the end."
     */
    const T* findNext(const T* a);

    /**
     * Returns the first element \c t encountered by iterating self
     * forward such that the expression <tt>((*\a fn)(t,\a d))</tt>
     * is \c true. Client data may be passed through parameter \a d.
     */
    const T* findNext(bool (*fn)(T*, void*), void* d);

    /**
     * Returns the stored value pointed to by self. Undefined if
     * self is not referencing a value within the list.
     */
    const T* key() const {
        return *(const_cast<RWTPtrSlistConstIterator<T, A>*>(this))->iter_;
    }

    /**
     * Resets the iterator so that after being advanced, it will reference
     * the first element of the collection.
     */
    void reset() {
        head_ = true;
        tail_ = false;
    }

    /**
     * Resets the iterator so that after being advanced, it will reference
     * the first element of the collection \a s.
     */
    void reset(const RWTPtrSlist<T, A>& s) {
        head_ = true;
        tail_ = false;
        list_ = &s;
    }

private:

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);
};

#endif

#  if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#    pragma implementation ("tpslist.cc")
#  endif

#  ifdef RW_NO_IMPLICIT_INCLUSION
#    include "rw/tpslist.cc"
#  endif

#endif /* RW_CLASSIC_TEMPLATE_CONTAINERS */

#endif /* RW_TOOLS_TPDLIST_H */
