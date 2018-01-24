#ifndef RW_TOOLS_TPSRTDLI_H
#define RW_TOOLS_TPSRTDLI_H

/**********************************************************************
 *
 * tpsrtdli.h - RWTPtrSortedDlist<T,Compare>
 *     : pointer-based sorted doubly-linked list wrapper for STL list
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tpsrtdli.h#2 $
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
#include <rw/edefs.h>
#include <rw/epaux.h>
#include <rw/epfunc.h>
#include <rw/rwerr.h>
#include <rw/toolerr.h>
#include <rw/tools/algorithm.h>
#include <rw/tools/traits/RWTEnableIf.h>
#include <rw/tools/traits/RWTIsIntegral.h>

#include <algorithm>
#include <list>

#if !defined(RW_DISABLE_DEPRECATED)

template <class T, class C, class A>
class RWTPtrSortedDlistIterator;
template <class T, class C, class A>
class RWTPtrSortedDlistConstIterator;

#endif

/**
 * @ingroup stl_based_collection_classes
 *
 * @brief A sorted pointer-based collection of values implemented
 * as a doubly-linked list.
 *
 * This class maintains an always-sorted pointer-based collection
 * of values, implemented as a doubly-linked list. Items are ordered
 * according to a comparison object of type \c C. Class \c T is the type
 * pointed to by the items in the collection. Class \c A is an allocator
 * of objects of type \c T. For more information about using custom
 * allocators, please see the <i>Essential Tools Module User's Guide</i>.

 * \c C must induce a total
 * ordering on elements of type \c T via a public member
 * <tt>bool operator()(const T& x, const T& y) </tt>
 * that returns \c true if \c x should precede
 * \c y within the collection. The structure \b std::less<T> from the
 * C++ Standard Library header file \c &lt;functional&gt; is an example. Note
 * that items in the collection will be dereferenced before being
 * compared.
 *
 * The \c value type must have \c operator== and \c operator< defined.
 * This requirement is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tpsrtdli.h>
 * RWTPtrSortedDlist<T,C,A> srtdlist;
 * @endcode
 *
 * @section related Related Classes
 *
 * Class \link RWTPtrSortedVector RWTPtrSortedVector<T>\endlink is an
 * alternative always-sorted pointer-based collection.
 * \link RWTPtrDlist RWTPtrDlist<T,A>\endlink is an unsorted pointer-based
 * doubly-linked list.
 *
 * Class <b>std::list<T*,A<T*> ></b> is the C++ Standard Library collection that
 * serves as the underlying implementation for this class.
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
 * // tpsrtdli.cpp
 * //
 * #include <iostream>
 * #include <functional>
 * #include <rw/tpsrtdli.h>
 * #include <rw/tools/datetime.h>
 *
 * int main ()
 * {
 *     RWTPtrSortedDlist<RWDateTime, std::greater<RWDateTime> > lst;
 *
 *     lst.insert(new RWDateTime("Aug 10, 1991", RWDateTime::setDate));
 *     lst.insert(new RWDateTime("Aug 9, 1991", RWDateTime::setDate));
 *     lst.insert(new RWDateTime("Sep 1, 1991", RWDateTime::setDate));
 *     lst.insert(new RWDateTime("May 14, 1990", RWDateTime::setDate));
 *     lst.insert(new RWDateTime("Sep 1, 1991", RWDateTime::setDate)); // Add a duplicate
 *     lst.insert(new RWDateTime("June 2, 1991", RWDateTime::setDate));
 *
 *     for (size_t i = 0; i < lst.entries(); ++i)
 *         std::cout << lst[i]->asString('x') << std::endl;
 *
 *     lst.clearAndDestroy();
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 * 09/01/91
 * 09/01/91
 * 08/10/91
 * 08/09/91
 * 06/02/91
 * 05/14/90
 * @endcode
 *
 */
template <class T, class C = std::less<T>, class A = std::allocator<T*> >
class RWTPtrSortedDlist
{
public:

    /**
     * A type representing a comparison function that dereferences the
     * arguments before comparing.
     */
    typedef rw_deref_compare<C, T> key_compare;

    /**
     * A type representing the underlying implementation container.
     */
    typedef std::list<T*, A> container_type;

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
     * A type that provides a bidirectional iterator over the elements
     * in the container.
     */
    typedef typename container_type::iterator iterator;

    /**
     * A type that provides a \c const bidirectional iterator over the
     * elements in the container.
     */
    typedef typename container_type::const_iterator const_iterator;

    /**
     * A type that provides a bidirectional, reverse-order iterator over
     * the elements in the container.
     */
    typedef typename container_type::reverse_iterator reverse_iterator;

    /**
     * A type that provides a \c const bidirectional, reverse-order iterator
     * over the elements in the container.
     */
    typedef typename container_type::const_reverse_iterator const_reverse_iterator;

    /**
     * A type representing a comparison function that dereferences the
     * arguments before comparing.
     */
    typedef key_compare value_compare;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrSortedDlist::key_compare \c key_compare \endlink instead.
     *
     * A type representing a comparison function that dereferences the
     * arguments before comparing.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrSortedDlist::key_compare instead") rw_deref_compare<C, T> container_comp;
#endif


    /**
     * @internal
     * A type representing the same type as self.
     */
    typedef RWTPtrSortedDlist<T, C, A> this_type;

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

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrSortedDlist::key_compare \c key_compare \endlink instead.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrSortedDlist::key_compare instead") rw_deref_compare<C, T> deref_compare;
#endif

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
    bool contains(bool(*fn)(const T*, void*), void* d) const {
        return std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d)) != end();
    }

    /**
     * Returns \c true if there exists an element \c t in self such
     * that the expression <tt>(*t == *\a a)</tt> is \c true, otherwise
     * returns \c false.
     */
    bool contains(const T* a) const {
        const_iterator hit = std::lower_bound(begin(), end(), a, key_compare());
        if (hit == end()) {
            return false;
        }
        return (**hit == *a);
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
#if defined(RW_STDLIB_VER) && (RW_STDLIB_VER >= 0x02010100 && RW_STDLIB_VER <= 0x02020100)
        std().erase(first, last);
        return last;
#else
        return std().erase(first, last);
#endif
    }

    /**
     * If there exists an element \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, returns \c t. Otherwise,
     * returns #rwnil. Client data may be passed through parameter \a d.
     */
    const T* find(bool(*fn)(const T*, void*), void* d) const {
        const_iterator ret = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        return (ret != end()) ? *ret : rwnil;
    }

    /**
     * If there exists an element \c t in self such that the expression
     * <tt>(*t == *\a a)</tt> is \c true, returns \c t. Otherwise,
     * returns #rwnil.
     */
    value_type find(const T* a) {
        iterator it = std::lower_bound(begin(), end(), a, key_compare());

        return (it != end() && (**it == *a)) ? *it : rwnil;
    }

    /**
     * @copydoc find(const T*)
     */
    const T* find(const T* a) const {
        const_iterator it = std::lower_bound(begin(), end(), a, key_compare());

        return (it != end() && (**it == *a)) ? *it : rwnil;
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
     * Adds the item \a a to self. The collection remains sorted. Returns
     * \c true.
     */
    bool insert(value_type a) {
        // only fail thru exception
        insertSorted(begin(), a);
        return true;
    }

    /**
     * Inserts \a val into self. Returns an iterator to the new item.
     *
     * The \a hint parameter indicates where in the collection the new value
     * should be inserted. This value is only a hint, and will be ignored if
     * the value should be inserted at a different location according to the
     * container's comparison object.
     */
    iterator insert(iterator hint, value_type val) {
        return insertSorted(hint, val);
    }

    /**
     * Inserts the elements in the range [\a first, \a last) into self.
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last) {
#if !defined(RW_BROKEN_STD_LIST_INSERT_RANGE)
        std().insert(std().end(), first, last);
#else
        std::copy(first, last, std::inserter(*this, end()));
#endif
        rw_list_sort(impl_, key_compare());
    }

    /**
     * Adds the items from \a a to self in an order preserving way.
     * Returns the number of items inserted.
     */
    size_type insert(const container_type& a);

    /**
     * Returns \c true if there are no items in the collection, otherwise
     * \c false.
     */
    bool isEmpty() const {
        return std().empty();
    }

    /**
     * Returns \c true if the collection is sorted relative to the supplied
     * comparator object, otherwise \c false.
     */
    bool isSorted() const {
        return rw_is_sorted(begin(), end(), key_compare());
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
        size_type ret = 0;
        const_iterator iter = std::lower_bound(begin(), end(), a, key_compare());
        while (iter != end() && (**iter == *a)) {
            ++iter;
            ++ret;
        }
        return ret;
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
     * Removes and returns the first element \c t in self such that
     * the expression <tt>(*t == *\a a)</tt> is \c true. Returns #rwnil
     * if there is no such element.
     */
    value_type remove(const T* key) {
        value_type ret = rwnil;
        iterator hit = std::lower_bound(begin(), end(), key, key_compare());
        if (hit != end() &&** hit == *key) {
            ret = *hit;
            std().erase(hit);
        }
        return ret;
    }

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
     * <tt>(*t == *\a a)</tt> is \c true. Returns the number of items
     * removed.
     */
    size_type removeAll(const T* a);

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
        std().pop_front();
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
     * Returns a reference to the \a i <sup>th</sup> element of self.
     *
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0 and one
     * less than the number of entries in self.
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
        const_iterator iter = begin();
        std::advance(iter, i);
        return *iter;
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
        iterator iter = begin();
        std::advance(iter, i);
        return *iter;
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
        const_iterator iter = begin();
        std::advance(iter, i);
        return *iter;
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
     * Returns the position of the first item \c t in self such that
     * <tt>(*t == *\a a)</tt>, or returns #RW_NPOS if no such item exists.
     */
    size_type index(const T* a) const;


    /**
     * Returns the position of the first item \c t in self such that
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, or returns #RW_NPOS
     * if no such item exists. Client data may be passed through
     * parameter \a d.
     */
    size_type index(bool (*fn)(const T*, void*), void* d) const;

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
     * Inserts all elements of \a coll into self, preserving sorted order.
     * Returns the number of items inserted.
     */
    size_type merge(const RWTPtrSortedDlist<T, C, A>& coll);

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

    // assignment operators
    /**
     * Clears all elements of self and replaces them by copying all
     * elements of \a rhs.
     */
    RWTPtrSortedDlist<T, C, A>&
    operator=(const RWTPtrSortedDlist<T, C, A>& rhs) {
        if (&rhs != this) {
            impl_ = rhs.impl_;
        }
        return *this;
    }

    /**
     * Destroys all elements of self and replaces them by copying (and
     * sorting, if necessary) all elements of \a rhs.
     */
    RWTPtrSortedDlist<T, C, A>&
    operator=(const container_type& rhs) {
        if (&impl_ != &rhs) {
            impl_ = rhs;
            rw_list_sort(impl_, key_compare());
        }
        return *this;
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrSortedDlist<T, C, A>&
    operator=(RWTPtrSortedDlist<T, C, A> && rhs) {
        if (&rhs != this) {
#    if !defined(RW_NO_STDLIST_RVALUE_SUPPORT)
            impl_ = rw_move(rhs.impl_);
#    else
            rhs.std().swap(impl_);
#    endif
        }
        return *this;
    }

    /**
     * Move assignment. Self sorts and takes ownership of the data owned
     * by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrSortedDlist<T, C, A>&
    operator=(container_type && rhs) {
        if (&impl_ != &rhs) {
#    if !defined(RW_NO_STDLIST_RVALUE_SUPPORT)
            impl_ = rw_move(rhs);
#    else
            rhs.swap(impl_);
#    endif
            rw_list_sort(impl_, key_compare());
        }
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs an empty doubly-linked list.
     */
    RWTPtrSortedDlist() {}

    /**
     * Copy constructor.
     */
    RWTPtrSortedDlist(const RWTPtrSortedDlist<T, C, A>& t)
        : impl_(t.impl_) {}

    /**
     * Constructs a doubly-linked list by iterating over all elements
     * in \a lst and performing an order preserving insertion on self
     * for each.
     */
    RWTPtrSortedDlist(const container_type& lst) : impl_(lst) {
        rw_list_sort(impl_, key_compare());
    }

    /**
     * Constructs a doubly-linked list with \a n elements, each initialized
     * to \a p.
     */
    RWTPtrSortedDlist(size_type n, value_type p) : impl_(n, p) {}

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed list takes ownership of the
     * data owned by \a lst.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrSortedDlist(RWTPtrSortedDlist<T, C, A> && lst)
#    if !defined(RW_NO_STDLIST_RVALUE_SUPPORT)
        : impl_(rw_move(lst.impl_)) { }
#    else
    {
        lst.std().swap(impl_);
    }
#    endif

    /**
     * Move constructor. The constructed list sorts and takes ownership of the
     * data owned by \a lst.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrSortedDlist(container_type && lst) {
        lst.sort(key_compare());
#    if !defined(RW_NO_STDLIST_RVALUE_SUPPORT)
        impl_ = rw_move(lst);
#    else
        lst.swap(impl_);
#    endif
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
#if !defined(DOXYGEN)
    RWTPtrSortedDlist(InputIterator first, InputIterator last,
                      typename RWTEnableIf < !RWTIsIntegral<InputIterator>::value >::type** = 0)
#else
    RWTPtrSortedDlist(InputIterator first, InputIterator last)
#endif
#if !defined(RW_BROKEN_STD_LIST_CTOR_RANGE)
        : impl_(first, last) {
        rw_list_sort(impl_, key_compare());
    }
#else
        :
        impl_() {
        insert(first, last);
    }
#endif

    // ~RWTPtrSortedDlist(); // handled by compiler-generated destructor

    /**
     * Swaps the data owned by self with the data owned by \a lst.
     */
    void swap(RWTPtrSortedDlist<T, C, A>& lst) {
        std().swap(lst.impl_);
    }

private:
    void boundsCheck(size_type i) const {
        if (!(i < std().size())) {
            RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEXERR, i, entries())));
        }
    }

    iterator insertSorted(iterator, value_type);

    container_type impl_;

#if !defined(RW_DISABLE_DEPRECATED)

    friend class RWTPtrSortedDlistIterator<T, C, A>;

#endif
};



/**
 * @relates RWTPtrSortedDlist
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and
 * iterating through both collections produces individual elements that,
 * in turn, compare equal to each other. Elements are dereferenced before
 * being compared.
 */
template <class T, class C, class A>
bool operator==(const RWTPtrSortedDlist<T, C, A>& lhs, const RWTPtrSortedDlist<T, C, A>& rhs)
{
    return lhs.entries() == rhs.entries() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrSortedDlist<T, C, A>::deref_equal());
}

/**
 * @relates RWTPtrSortedDlist
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T, class C, class A>
bool operator!=(const RWTPtrSortedDlist<T, C, A>& lhs, const RWTPtrSortedDlist<T, C, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrSortedDlist
 *
 * Returns \c true if \a lhs is lexicographically less than \a rhs,
 * otherwise \c false. Items in each collection are dereferenced before
 * being compared. Assumes that type \c T has well-defined less-than
 * semantics.
 */
template <class T, class C, class A>
bool operator<(const RWTPtrSortedDlist<T, C, A>& lhs, const RWTPtrSortedDlist<T, C, A>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrSortedDlist<T, C, A>::deref_less());
}

/**
 * @relates RWTPtrSortedDlist
 *
 * Equivalent to <tt>(\a rhs < \a lhs)</tt>.
 */
template <class T, class C, class A>
bool operator>(const RWTPtrSortedDlist<T, C, A>& lhs, const RWTPtrSortedDlist<T, C, A>& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrSortedDlist
 *
 * Equivalent to <tt>!(\a rhs < \a lhs)</tt>.
 */
template <class T, class C, class A>
bool operator<=(const RWTPtrSortedDlist<T, C, A>& lhs, const RWTPtrSortedDlist<T, C, A>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrSortedDlist
 *
 * Equivalent to <tt>!(\a lhs < \a rhs)</tt>.
 */
template <class T, class C, class A>
bool operator>=(const RWTPtrSortedDlist<T, C, A>& lhs, const RWTPtrSortedDlist<T, C, A>& rhs)
{
    return !(lhs < rhs);
}

/**
 * @relates RWTPtrSortedDlist
 *
 * @copydoc operator==(const RWTPtrSortedDlist&, const RWTPtrSortedDlist&);
 */
template <class T, class C, class A>
bool operator==(const RWTPtrSortedDlist<T, C, A>& lhs,
                const typename RWTPtrSortedDlist<T, C, A>::container_type& rhs)
{
    return lhs.entries() == rhs.size() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrSortedDlist<T, C, A>::deref_equal());
}

/**
 * @relates RWTPtrSortedDlist
 *
 * @copydoc operator!=(const RWTPtrSortedDlist&, const RWTPtrSortedDlist&);
 */
template <class T, class C, class A>
bool operator!=(const RWTPtrSortedDlist<T, C, A>& lhs,
                const typename RWTPtrSortedDlist<T, C, A>::container_type& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrSortedDlist
 *
 * @copydoc operator<(const RWTPtrSortedDlist&, const RWTPtrSortedDlist&);
 */
template <class T, class C, class A>
bool operator<(const RWTPtrSortedDlist<T, C, A>& lhs,
               const typename RWTPtrSortedDlist<T, C, A>::container_type& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrSortedDlist<T, C, A>::deref_less());
}

/**
 * @relates RWTPtrSortedDlist
 *
 * @copydoc operator>(const RWTPtrSortedDlist&, const RWTPtrSortedDlist&);
 */
template <class T, class C, class A>
bool operator>(const RWTPtrSortedDlist<T, C, A>& lhs,
               const typename RWTPtrSortedDlist<T, C, A>::container_type& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrSortedDlist
 *
 * @copydoc operator<=(const RWTPtrSortedDlist&, const RWTPtrSortedDlist&);
 */
template <class T, class C, class A>
bool operator<=(const RWTPtrSortedDlist<T, C, A>& lhs,
                const typename RWTPtrSortedDlist<T, C, A>::container_type& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrSortedDlist
 *
 * @copydoc operator>=(const RWTPtrSortedDlist&, const RWTPtrSortedDlist&);
 */
template <class T, class C, class A>
bool operator>=(const RWTPtrSortedDlist<T, C, A>& lhs,
                const typename RWTPtrSortedDlist<T, C, A>::container_type& rhs)
{
    return !(lhs < rhs);
}


/**
 * @relates RWTPtrSortedDlist
 *
 * @copydoc operator==(const RWTPtrSortedDlist&, const RWTPtrSortedDlist&);
 */
template <class T, class C, class A>
bool operator==(const typename RWTPtrSortedDlist<T, C, A>::container_type& lhs,
                const RWTPtrSortedDlist<T, C, A>& rhs)
{
    return lhs.size() == rhs.entries() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrSortedDlist<T, C, A>::deref_equal());
}

/**
 * @relates RWTPtrSortedDlist
 *
 * @copydoc operator!=(const RWTPtrSortedDlist&, const RWTPtrSortedDlist&);
 */
template <class T, class C, class A>
bool operator!=(const typename RWTPtrSortedDlist<T, C, A>::container_type& lhs,
                const RWTPtrSortedDlist<T, C, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrSortedDlist
 *
 * @copydoc operator<(const RWTPtrSortedDlist&, const RWTPtrSortedDlist&);
 */
template <class T, class C, class A>
bool operator<(const typename RWTPtrSortedDlist<T, C, A>::container_type& lhs,
               const RWTPtrSortedDlist<T, C, A>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrSortedDlist<T, C, A>::deref_less());
}

/**
 * @relates RWTPtrSortedDlist
 *
 * @copydoc operator>(const RWTPtrSortedDlist&, const RWTPtrSortedDlist&);
 */
template <class T, class C, class A>
bool operator>(const typename RWTPtrSortedDlist<T, C, A>::container_type& lhs,
               const RWTPtrSortedDlist<T, C, A>& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrSortedDlist
 *
 * @copydoc operator<=(const RWTPtrSortedDlist&, const RWTPtrSortedDlist&);
 */
template <class T, class C, class A>
bool operator<=(const typename RWTPtrSortedDlist<T, C, A>::container_type& lhs,
                const RWTPtrSortedDlist<T, C, A>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrSortedDlist
 *
 * @copydoc operator>=(const RWTPtrSortedDlist&, const RWTPtrSortedDlist&);
 */
template <class T, class C, class A>
bool operator>=(const typename RWTPtrSortedDlist<T, C, A>::container_type& lhs,
                const RWTPtrSortedDlist<T, C, A>& rhs)
{
    return !(lhs < rhs);
}

/**
 * @relates RWTPtrSortedDlist
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T, class C , class A>
RWvostream& operator<<(RWvostream& strm, const RWTPtrSortedDlist<T, C , A>& coll);

/**
 * @relates RWTPtrSortedDlist
 *
 * Saves the collection \a coll onto the RWFile \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T, class C , class A>
RWFile& operator<<(RWFile& strm, const RWTPtrSortedDlist<T, C , A>& coll);

/**
 * @relates RWTPtrSortedDlist
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class T, class C , class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrSortedDlist<T, C , A>& coll);

/**
 * @relates RWTPtrSortedDlist
 *
 * Restores the contents of the collection \a coll from the RWFile
 * \a strm.
 */
template <class T, class C , class A>
RWFile& operator>>(RWFile& strm, RWTPtrSortedDlist<T, C , A>& coll);

/**
 * @relates RWTPtrSortedDlist
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T, class C , class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrSortedDlist<T, C , A>*& p);

/**
 * @relates RWTPtrSortedDlist
 *
 * Looks at the next object on the RWFile \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T, class C , class A>
RWFile& operator>>(RWFile& strm, RWTPtrSortedDlist<T, C , A>*& p);

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @ingroup stl_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTPtrSortedDlist::iterator to iterate
 * over the elements in this collection.
 *
 * @brief Deprecated. Provides an iterator for RWTPtrSortedDlist.
 *
 * RWTPtrSortedDlistIterator provides an iterator interface to
 * the C++ Standard Library-based collection represented by RWTPtrSortedDlist.
 * (An STL iterator interface is provided through RWTPtrSortedDlist::iterator.)
 *
 * The order of iteration over an RWTPtrSortedDlist is dependent
 * on the comparator object parameter \c C as applied to the values
 * stored in the container.
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
 * The \c value type must have \c operator== and \c operator< defined.
 * This requirement is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include<rw/tpsrtdli.h>
 * RWTPtrSortedDlist<T,C,A> dl;
 * RWTPtrSortedDlistIterator<T,C,A> itr(dl);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include<rw/tpsrtdli.h>
 * #include<iostream>
 * #include<rw/cstring.h>
 * #include<functional>
 *
 * int main() {
 *    RWTPtrSortedDlist<RWCString,std::less<RWCString> > a;
 *    RWTPtrSortedDlistIterator<RWCString,std::less<RWCString> > itr(a);
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
 * Program Output:
 *
 * @code
 *     John
 *     Mark
 *     Steve
 *     Steve
 * @endcode
 */
template <class T, class C = std::less<T>, class A = std::allocator<T*> >
class RW_DEPRECATE_TYPE("Use RWTPtrSortedDlist::iterator instead") RWTPtrSortedDlistIterator
{
public:
    /**
     * Creates an iterator for the list \a l. The iterator begins in
     * an undefined state and must be advanced before the first element
     * will be accessible.
     */
    RWTPtrSortedDlistIterator(RWTPtrSortedDlist<T, C, A>& l)
        : list_(&l), head_(true), tail_(false) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self will now reference the first element.
     * If, before iteration, self referenced the last association in
     * the list, self will now point to an undefined value, and a value
     * equivalent to \c false will be returned. Otherwise, a value
     * equivalent to \c true is returned.
     */
    bool operator++();

    /**
     * Moves self back to the immediately previous element. If the
     * iterator has been reset or just created, this operator will return
     * \c false, otherwise it will return \c true. If self references
     * the first element, it will now be in the reset state. If self
     * has been iterated past the last value in the list, it will now
     * reference the last item in the list.
     */
    bool operator--();

    /**
     * Behaves as if operator++() had been applied \a n times.
     */
    bool operator+=(size_t n);

    /**
     * Behaves as if operator--() had been applied \a n times.
     */
    bool operator-=(size_t n);

    /**
     * Advances self to the next element, dereferences the resulting
     * iterator and returns its value. If the iterator has advanced
     * past the last item in the container, #rwnil will be returned.
     */
    T* operator()() {
        return ++(*this) ? key() : rwnil;
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    RWTPtrSortedDlist<T, C, A>* container() const {
        return list_;
    }

    /**
     * Returns the first element \c t encountered by iterating self
     * forward, such that the expression <tt>(*t == *\a a)</tt> is \c true.
     * Otherwise returns #rwnil. Leaves self referencing found item or "off
     * the end."
     */
    T* findNext(const T* a);

    /**
     * Returns the first element \c t encountered by iterating self
     * forward, such that the expression <tt>((*\a fn)(t,\a d))</tt>
     * is \c true. Otherwise returns #rwnil. Leaves self referencing
     * found item or "off the end." Client data may be passed through
     * parameter \a d.
     */
    T* findNext(bool (*fn)(T*, void*), void* d);

    /**
     * Returns the stored value pointed to by self. Undefined if self
     * is not referencing a value within the list.
     */
    T* key() const {
        return *((RWTPtrSortedDlistIterator<T, C, A>*)this)->iter_;
    }

    /**
     * Returns the stored value pointed to by self and removes it from
     * the collection. Undefined if self is not referencing a value
     * within the list. Leaves self referencing prior item or in reset
     * state.
     */
    T* remove();

    /**
     * Returns and removes the first element \c t, encountered by iterating
     * self forward, such that the expression <tt>(*t == *\a a)</tt> is \c true.
     * Otherwise returns #rwnil. Leaves self referencing prior item or in reset
     * state.
     */
    T* removeNext(const T* a);

    /**
     * Returns and removes the first element \c t, encountered by iterating
     * self forward, such that the expression <tt>((*\a fn)(t,\a d))</tt> is
     * true. Otherwise returns #rwnil. Leaves self referencing prior item or
     * in reset state. Client data may be passed through parameter \a d.
     */
    T* removeNext(bool (*fn)(T*, void*), void* d);

    /**
     * Resets the iterator so that after being advanced it will point
     * to the first element of the collection.
     */
    void reset() {
        head_ = true;
        tail_ = false;
    }

    /**
     * Resets the iterator so that after being advanced it will point
     * to the first element of the collection \a l.
     */
    void reset(RWTPtrSortedDlist<T, C, A>& l) {
        head_ = true;
        tail_ = false;
        list_ = &l;
    }

private:
    RWTPtrSortedDlist<T, C, A>* list_;
    typename RWTPtrSortedDlist<T, C, A>::iterator iter_;
    bool head_;  // simulate head sentinel
    bool tail_;  // simulate tail sentinel

    // Disallow postfix operators. Unless we hide it, some compilers will
    // substitute the prefix operator in its place.
    bool operator++(int);
    bool operator--(int);
    friend class RWTPtrSortedDlistConstIterator<T, C, A>;
};

/**
 * @ingroup stl_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTPtrSortedDlist::const_iterator to
 * iterate over the elements in this collection.
 *
 * @brief Deprecated. Provides a \c const iterator for RWTPtrSortedDlist.
 *
 * RWTPtrSortedDlistConstIterator provides a \c const iterator interface
 * to C++ Standard Library-based RWTPtrSortedDlist collections. A
 * \c const iterator does not modify anything in the underlying
 * collection.
 *
 * The order of iteration over an RWTPtrSortedDlist is dependent
 * on the comparator object parameter \c C as applied to the values
 * stored in the container.
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
 * The \c value type must have \c operator== and \c operator< defined.
 * This requirement is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tpsrtdli.h>
 * template <class T, class C, class A=std::allocator<T*> > class
 * RWTPtrSortedDlistConstIterator
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class T, class C = std::less<T>, class A = std::allocator<T*> >
class RW_DEPRECATE_TYPE("Use RWTPtrSortedDlist::const_iterator instead") RWTPtrSortedDlistConstIterator
{
public:
    /**
     * Creates a \c const iterator for the list \a l. The iterator
     * begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTPtrSortedDlistConstIterator(const RWTPtrSortedDlist<T, C, A>& l)
        : list_(&l), head_(true), tail_(false) { }

    /**
     * Creates a \c const iterator from the iterator \a itr. The \c const
     * iterator begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTPtrSortedDlistConstIterator(const RWTPtrSortedDlistIterator<T, C, A>& itr)
        : list_(itr.list_), iter_(itr.iter_),
          head_(itr.head_), tail_(itr.tail_) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self now references the first element.
     * If, before iteration, self referenced the last association in
     * the list, self now points to an undefined value and a value equivalent
     * to \c false is returned. Otherwise, a value equivalent to \c true
     * is returned.
     */
    bool operator++();

    /**
     * Moves self back to the immediately previous element. If the
     * iterator has been reset or just created, this operator returns
     * \c false, otherwise it returns \c true. If self references
     * the first element, it is now in the reset state. If self
     * has been iterated past the last value in the list, it now references
     * the last item in the list.
     */
    bool operator--();

    /**
     * Behaves as if operator++() had been applied \a n times.
     */
    bool operator+=(size_t n);

    /**
     * Behaves as if operator--() had been applied \a n times.
     */
    bool operator-=(size_t n);

    /**
     * Advances self to the next element, dereferences the resulting
     * iterator, and returns its value. If the iterator has advanced
     * past the last item in the container, #rwnil is returned.
     */
    const T* operator()() {
        return ++(*this) ? key() : rwnil;
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    const RWTPtrSortedDlist<T, C, A>* container() const {
        return list_;
    }

    /**
     * Returns the first element \c t encountered by iterating self
     * forward, such that the expression <tt>(*t == *\a a)</tt> is \c
     * true. Otherwise returns #rwnil. Leaves self-referencing found
     * item or "off the end."
     */
    const T* findNext(const T* a);

    /**
     * Returns the first element \c t encountered by iterating self
     * forward such that the expression <tt>((*\a fn)(t,\a d))</tt> is
     * \c true. Otherwise returns #rwnil. Leaves self referencing found
     * item or "off the end." Client data may be passed through parameter
     * \a d.
     */
    const T* findNext(bool (*fn)(T*, void*), void* d);

    /**
     * Returns the stored value pointed to by self. Undefined if
     * self is not referencing a value within the list.
     */
    const T* key() const {
        return *((RWTPtrSortedDlistConstIterator<T, C, A>*)this)->iter_;
    }

    /**
     * Resets the iterator so that after being advanced, it points to
     * the first element of the collection.
     */
    void reset() {
        head_ = true;
        tail_ = false;
    }

    /**
     * Resets the iterator so that after being advanced, it points to
     * the first element of the collection \a l.
     */
    void reset(const RWTPtrSortedDlist<T, C, A>& l) {
        head_ = true;
        tail_ = false;
        list_ = &l;
    }

private:
    const RWTPtrSortedDlist<T, C, A>* list_;
    typename RWTPtrSortedDlist<T, C, A>::const_iterator iter_;
    bool head_;  // simulate head sentinel
    bool tail_;  // simulate tail sentinel

    // Disallow postfix operators. Unless we hide it, some compilers will
    // substitute the prefix operator in its place.
    bool operator++(int);
    bool operator--(int);
};
#endif

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tpsrtdli.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include <rw/tpsrtdli.cc>
#endif


#endif /* RW_TOOLS_TPSRTDLI_H */
