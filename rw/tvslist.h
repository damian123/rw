#ifndef RW_TOOLS_TVSLIST_H
#define RW_TOOLS_TVSLIST_H

/**********************************************************************
 *
 *     : value-based singly-linked list wrapper for STL list
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvslist.h#2 $
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
#  include "rw/xvslist.h"
#else
// STL-based implementation:

#  include <rw/edefs.h> // for rw_move
#  include <rw/epfunc.h>
#  include <rw/rwerr.h>
#  include <rw/stdex/slist.h>  // Rogue Wave STL-type singly linked list
#  include <rw/toolerr.h>
#  include <rw/tools/algorithm.h>

#  include <functional>

#if !defined(RW_DISABLE_DEPRECATED)

template <class T, class A>
class RWTValSlistConstIterator;

#endif

/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @brief Maintains a collection of values implemented as a
 * singly-linked list.
 *
 * This class maintains a collection of values, implemented as a
 * singly-linked list. The \c value type must have \c operator==
 * and \c operator< defined. This requirement is imposed by the
 * C++ Standard Library.
 *
 * Class \c T is the type of items in the collection.
 * Class \c A is an allocator of objects of class \c T. For more
 * information about using custom allocators, please see the
 * <i>Essential Tools Module User's Guide</i>.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvslist.h>
 * RWTValSlist<T,A> lst;
 * @endcode
 *
 * @section related Related Classes
 *
 * Classes \link RWTValDeque RWTValDeque<T,A>\endlink,
 * \link RWTValDlist RWTValDlist<T,A>\endlink, and
 * \link RWTValOrderedVector RWTValOrderedVector<T,A>\endlink also provide a
 * Rogue Wave interface to C++ Standard Library sequence collections.
 *
 * The Rogue Wave supplied, Standard Library-compliant class
 * \link rw_slist rw_slist<T,A>\endlink
 * is the collection that serves as the underlying implementation
 * for this class.
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
 * // tvslint.cpp
 * //
 * #include<iostream>
 * #include<rw/tvslist.h>
 *
 * void div5 (int& x, void*)
 * {
 *     x /= 5;
 * }
 *
 * int main()
 * {
 *     const int vec[10] = {45,10,5,15,25,30,35,20,40,50};
 *
 *     RWTValSlist<int> lst(vec, vec+10);
 *     RWTValSlistIterator<int> itr(lst);
 *
 *     lst.apply(div5, 0);
 *     lst.sort();
 *
 *     for(;itr ();)
 *         std::cout << itr.key() << " ";
 *
 *     std::cout << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 *     1 2 3 4 5 6 7 8 9 10
 * @endcode
 */
template <class T, class A = std::allocator<T> >
class RWTValSlist
{
public:

    /**
     * A type representing the underlying implementation container.
     */
    typedef rw_slist<T, A> container_type;

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
    typedef RWTValSlist<T, A> this_type;


    /**
     * Returns a reference to the underlying collection that serves
     * as the implementation for self. This reference may be used freely,
     * providing access to the C++ Standard Library interface as well as
     * interoperability with other software components that make use of
     * the C++ Standard Library collections.
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
    void append(const_reference a) {
        std().insert(std().end(), a);
    }

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @copydoc append()
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    void append(value_type && a) {
        std().insert(std().end(), rw_move(a));
    }
#  endif // !RW_NO_RVALUE_REFERENCES

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
     * Clears the collection by removing all items from self. Each
     * item has its destructor called.
     */
    void clear() {
        std().erase(std().begin(), std().end());
    }

    /**
     * Returns \c true if there exists an element \c t in self such
     * that the expression <tt>((*\a fn)(t, \a d))</tt> is \c true,
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
     * <tt>((*\a fn)(t, \a d))</tt> is \c true, assigns \c t to \a k and
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
     * (\c t == \a a), or returns #RW_NPOS if no such item exists.
     */
    size_type index(const_reference a) const;

    /**
     * Returns the position of the first item \c t in self such that
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, or returns #RW_NPOS
     * if no such item exists. Client data may be passed through
     * parameter \a d.
     */
    size_type index(bool(*fn)(const_reference, void*), void* d) const;

    /**
     * Adds the item \a a to the end of the collection. Returns \c true.
     */
    bool insert(const_reference a) {
        std().insert(std().end(), a);
        return true;
    }

    /**
     * Inserts \a val into self before the element at position \a pos.
     * Returns an iterator for the newly inserted element.
     */
    iterator insert(iterator pos, const_reference val) {
        return std().insert(pos, val);
    }

    /**
     * Inserts \a n instances of \a val into self before the element at
     * position \a pos.
     */
    void insert(iterator pos, size_type n, const_reference val) {
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

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @copydoc insert(const_reference)
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    bool insert(value_type && a) { // fail only thru exception
        std().insert(std().end(), rw_move(a));
        return true;
    }

    /**
     * @copydoc insert(iterator, const_reference)
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    iterator insert(iterator pos, value_type && val) {
        return std().insert(pos, rw_move(val));
    }

#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Inserts the item \a a in front of the item at position \a i in
     * self.
     * @throw RWBoundsErr Thrown if this position is not between
     * \c 0 and the number of entries in the collection.
     */
    void insertAt(size_type i, const_reference a) {
        // index equal to number of entries OK (inserts at end)
        if (i > 0) {
            boundsCheck(i - 1);
        }

        iterator iter = begin();
        std::advance(iter, i);
        std().insert(iter, a);
    }

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @copydoc insertAt()
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    void insertAt(size_type i, value_type && a) {
        // index equal to number of entries OK (inserts at end)
        if (i > 0) {
            boundsCheck(i - 1);
        }

        iterator iter = begin();
        std::advance(iter, i);
        std().insert(iter, rw_move(a));
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Returns \c true if there are no items in the collection, \c false
     * otherwise.
     */
    bool isEmpty() const {
        return std().empty();
    }

    /**
     * Returns a reference to the minimum element in the collection.
     * Type \c T must have well-defined less-than semantics
     * <tt>(T::operator<(const T&)</tt> or equivalent).
     */
    reference minElement() {
        RW_PRECONDITION(entries() != 0);
        return *std::min_element(begin(), end(), std::less<value_type>());
    }

    /**
     * @copydoc minElement()
     */
    const_reference minElement() const {
        RW_PRECONDITION(entries() != 0);
        return *std::min_element(begin(), end(), std::less<value_type>());
    }

    /**
     * Returns a reference to the maximum element in the collection.
     * Type \c T must have well-defined less-than semantics
     * <tt>(T::operator<(const T&)</tt> or equivalent).
     */
    reference maxElement() {
        RW_PRECONDITION(entries() != 0);
        return *std::max_element(begin(), end(), std::less<value_type>());
    }

    /**
     * @copydoc maxElement()
     */
    const_reference maxElement() const {
        RW_PRECONDITION(entries() != 0);
        return *std::max_element(begin(), end(), std::less<value_type>());
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
     * Returns a reference to the \a i <sup>th</sup> element of self.
     * Index \a i should be between \c 0 and one less than the number
     * of entries, otherwise the results are undefined.
     * @note
     * No bounds checking is performed.
     */
    reference operator()(size_type i) {
        RW_PRECONDITION(i < entries());
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
        RW_PRECONDITION(i < entries());
        const_iterator iter = begin();
        std::advance(iter, i);
        return *iter;
    }

    /**
     * Returns a reference to the \a i <sup>th</sup> element of self.
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
     * Adds the item \a a to the beginning of the collection.
     */
    void prepend(const_reference a) {
        std().insert(std().begin(), a);
    }

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @copydoc prepend()
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    void prepend(value_type && a) {
        std().insert(std().begin(), rw_move(a));
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Removes the first element \c t in self such that the expression
     * <tt>(t == \a a)</tt> is \c true and returns \c true. Returns
     * \c false if there is no such element.
     */
    bool remove(const_reference a) {
        iterator iter = std::find(begin(), end(), a);
        if (iter != end()) {
            std().erase(iter);
            return true;
        }
        return false;
    }

    /**
     * Removes the first element \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true and returns \c true.
     * Returns \c false if there is no such element. Client data may
     * be passed through parameter \a d.
     */
    bool remove(bool(*fn)(const_reference, void*), void* d) {
        iterator iter = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        if (iter != end()) {
            std().erase(iter);
            return true;
        }
        return false;
    }

    /**
     * Removes all elements \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns the number
     * of items removed. Client data may be passed through parameter
     * \a d.
     */
    size_type removeAll(bool(*fn)(const_reference, void*), void* d) {
        size_type size = entries();
        iterator iter = std::remove_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        std().erase(iter, end());
        return size - entries();
    }

    /**
     * Removes all elements \c t in self such that the expression <tt>(t
     * == \a a)</tt> is \c true. Returns the number of items removed.
     */
    size_type removeAll(const_reference a) {
        size_type size = entries();
        iterator iter = std::remove(begin(), end(), a);
        std().erase(iter, end());
        return size - entries();
    }

    /**
     * Removes and returns the item at position \a i in self.
     *
     * @throw RWBoundsErr Thrown if the index \a i is not between
     * \c 0 and one less than the number of entries in the collection.
     */
    value_type removeAt(size_type i);

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
     * Replaces all elements \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true with the value \a newVal.
     * Returns the number of items replaced. Client data may be
     * passed through parameter \a d.
     */
    size_type replaceAll(bool(*fn)(const value_type&, void*), void* d, const value_type& newVal) {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_and_replace_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d), newVal);
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Replaces all elements \c t in self such that the expression <tt>(t
     * == \a oldVal)</tt> is \c true with \a newVal. Returns the number
     * of items replaced.
     */
    size_type replaceAll(const value_type& oldVal, const value_type& newVal) {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_and_replace(begin(), end(), oldVal, newVal);
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Invokes the function pointer \a fn on each item in the collection.
     * Client data may be passed through parameter \a d.
     */
    void apply(void (*fn)(const value_type&, void*), void* d) const {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * @copydoc apply()
     */
    void apply(void (*fn)(value_type&, void*), void* d) const {
        this_type& self = const_cast<this_type&>(*this);
        std::for_each(self.begin(), self.end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * Sorts the collection using the less-than operator to compare
     * elements.
     */
    void sort() {
        std().sort();
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
     * Calls the destructor on all elements of self and replaces them
     * by copying all elements of \a rhs.
     */
    RWTValSlist<T, A>&
    operator=(const RWTValSlist<T, A>& rhs) {
        return operator=(rhs.impl_);
    }

    /**
     * @copydoc operator=()
     */
    RWTValSlist<T, A>&
    operator=(const rw_slist<T, A>& rhs) {
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
    RWTValSlist<T, A>&
    operator=(RWTValSlist<T, A> && rhs) {
        return operator=(rw_move(rhs.impl_));
    }

    /**
     * @copydoc operator=(RWTValSlist<T,A>&&)
     */
    RWTValSlist<T, A>&
    operator=(container_type && rhs) {
        if (&impl_ != &rhs) {
            impl_ = rw_move(rhs);
        }
        return *this;
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs an empty, singly-linked list.
     */
    RWTValSlist() : impl_() {}

    /**
     * Constructs a singly-linked list by copying all elements of
     * \a lst.
     */
    RWTValSlist(const rw_slist<T, A>& lst)
        : impl_(lst) {}

    /**
     * Copy constructor.
     */
    RWTValSlist(const RWTValSlist<T, A>& lst)
        : impl_(lst.impl_) {}

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed list takes ownership of the
     * data owned by \a lst.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValSlist(RWTValSlist<T, A> && lst)
        : impl_(rw_move(lst.impl_)) { }

    /**
     * @copydoc RWTValSlist(RWTValSlist<T,A>&&)
     */
    RWTValSlist(container_type && lst) : impl_(rw_move(lst)) {}
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs a singly-linked list with \a n elements, each initialized
     * to \a val.
     */
    RWTValSlist(size_type n, const_reference val = value_type()) : impl_(n, val) {}

    /**
     * Constructs a double-ended queue by copying elements from the range
     * [\a first, \a last).
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
    template <typename InputIterator>
#if !defined(DOXYGEN)
    RWTValSlist(InputIterator first, InputIterator last,
                typename RWTEnableIf < !RWTIsIntegral<InputIterator>::value >::type** = 0)
        : impl_(first, last) {}
#else
    RWTValSlist(InputIterator first, InputIterator last) : impl_(first, last) {}
#endif

    //  ~RWTValSlist(); // handled by auto-generated destructor

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWTValSlist<T, A>& rhs) {
        std().swap(rhs.impl_);
    }

private:
    void boundsCheck(size_type i) const {
        if (i >= entries()) {
            RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEXERR, i, entries())));
        }
    }

    container_type impl_;
};


/**
 * @relates RWTValSlist
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and
 * iterating through both collections produces individual elements that,
 * in turn, compare equal to each other.
 */
template <class T , class A>
bool operator==(const RWTValSlist<T, A>& lhs, const RWTValSlist<T, A>& rhs)
{
    return lhs.std() == rhs.std();
}

/**
 * @relates RWTValSlist
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T , class A>
bool operator!=(const RWTValSlist<T, A>& lhs, const RWTValSlist<T, A>& rhs)
{
    return lhs.std() != rhs.std();
}

/**
 * @relates RWTValSlist
 *
 * Returns \c true if \a lhs is lexicographically less than \a rhs,
 * otherwise \c false. Assumes that type \c T has well-defined less-than
 * semantics.
 */
template <class T , class A>
bool operator<(const RWTValSlist<T, A>& lhs, const RWTValSlist<T, A>& rhs)
{
    return lhs.std() < rhs.std();
}

/**
 * @relates RWTValSlist
 *
 * Equivalent to <tt>(\a rhs < \a lhs)</tt>.
 */
template <class T , class A>
bool operator>(const RWTValSlist<T, A>& lhs, const RWTValSlist<T, A>& rhs)
{
    return lhs.std() > rhs.std();
}

/**
 * @relates RWTValSlist
 *
 * Equivalent to <tt>!(\a rhs < \a lhs)</tt>.
 */
template <class T , class A>
bool operator<=(const RWTValSlist<T, A>& lhs, const RWTValSlist<T, A>& rhs)
{
    return lhs.std() <= rhs.std();
}

/**
 * @relates RWTValSlist
 *
 * Equivalent to <tt>!(\a lhs < \a rhs)</tt>.
 */
template <class T , class A>
bool operator>=(const RWTValSlist<T, A>& lhs, const RWTValSlist<T, A>& rhs)
{
    return lhs.std() >= rhs.std();
}

/**
 * @relates RWTValSlist
 *
 * @copydoc operator==(const RWTValSlist&, const RWTValSlist&);
 */
template <class T , class A>
bool operator==(const RWTValSlist<T, A>& lhs,
                const typename RWTValSlist<T, A>::container_type& rhs)
{
    return lhs.std() == rhs;
}

/**
 * @relates RWTValSlist
 *
 * @copydoc operator!=(const RWTValSlist&, const RWTValSlist&);
 */
template <class T , class A>
bool operator!=(const RWTValSlist<T, A>& lhs,
                const typename RWTValSlist<T, A>::container_type& rhs)
{
    return lhs.std() != rhs;
}

/**
 * @relates RWTValSlist
 *
 * @copydoc operator<(const RWTValSlist&, const RWTValSlist&);
 */
template <class T , class A>
bool operator<(const RWTValSlist<T, A>& lhs,
               const typename RWTValSlist<T, A>::container_type& rhs)
{
    return lhs.std() < rhs;
}

/**
 * @relates RWTValSlist
 *
 * @copydoc operator>(const RWTValSlist&, const RWTValSlist&);
 */
template <class T , class A>
bool operator>(const RWTValSlist<T, A>& lhs,
               const typename RWTValSlist<T, A>::container_type& rhs)
{
    return lhs.std() > rhs;
}

/**
 * @relates RWTValSlist
 *
 * @copydoc operator<=(const RWTValSlist&, const RWTValSlist&);
 */
template <class T , class A>
bool operator<=(const RWTValSlist<T, A>& lhs,
                const typename RWTValSlist<T, A>::container_type& rhs)
{
    return lhs.std() <= rhs;
}

/**
 * @relates RWTValSlist
 *
 * @copydoc operator>=(const RWTValSlist&, const RWTValSlist&);
 */
template <class T , class A>
bool operator>=(const RWTValSlist<T, A>& lhs,
                const typename RWTValSlist<T, A>::container_type& rhs)
{
    return lhs.std() >= rhs;
}


/**
 * @relates RWTValSlist
 *
 * @copydoc operator==(const RWTValSlist&, const RWTValSlist&);
 */
template <class T , class A>
bool operator==(const typename RWTValSlist<T, A>::container_type& lhs,
                const RWTValSlist<T, A>& rhs)
{
    return lhs == rhs.std();
}

/**
 * @relates RWTValSlist
 *
 * @copydoc operator!=(const RWTValSlist&, const RWTValSlist&);
 */
template <class T , class A>
bool operator!=(const typename RWTValSlist<T, A>::container_type& lhs,
                const RWTValSlist<T, A>& rhs)
{
    return lhs != rhs.std();
}

/**
 * @relates RWTValSlist
 *
 * @copydoc operator<(const RWTValSlist&, const RWTValSlist&);
 */
template <class T , class A>
bool operator<(const typename RWTValSlist<T, A>::container_type& lhs,
               const RWTValSlist<T, A>& rhs)
{
    return lhs < rhs.std();
}

/**
 * @relates RWTValSlist
 *
 * @copydoc operator>(const RWTValSlist&, const RWTValSlist&);
 */
template <class T , class A>
bool operator>(const typename RWTValSlist<T, A>::container_type& lhs,
               const RWTValSlist<T, A>& rhs)
{
    return lhs > rhs.std();
}

/**
 * @relates RWTValSlist
 *
 * @copydoc operator<=(const RWTValSlist&, const RWTValSlist&);
 */
template <class T , class A>
bool operator<=(const typename RWTValSlist<T, A>::container_type& lhs,
                const RWTValSlist<T, A>& rhs)
{
    return lhs <= rhs.std();
}

/**
 * @relates RWTValSlist
 *
 * @copydoc operator>=(const RWTValSlist&, const RWTValSlist&);
 */
template <class T , class A>
bool operator>=(const typename RWTValSlist<T, A>::container_type& lhs,
                const RWTValSlist<T, A>& rhs)
{
    return lhs >= rhs.std();
}

/**
 * @relates RWTValSlist
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T , class A>
RWvostream& operator<<(RWvostream& strm, const RWTValSlist<T , A>& coll);

/**
 * @relates RWTValSlist
 *
 * Saves the collection \a coll onto the RWFile \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T , class A>
RWFile& operator<<(RWFile& strm, const RWTValSlist<T , A>& coll);

/**
 * @relates RWTValSlist
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class T , class A>
RWvistream& operator>>(RWvistream& strm, RWTValSlist<T , A>& coll);

/**
 * @relates RWTValSlist
 *
 * Restores the contents of the collection \a coll from the RWFile
 * \a strm.
 */
template <class T , class A>
RWFile& operator>>(RWFile& strm, RWTValSlist<T , A>& coll);

/**
 * @relates RWTValSlist
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T , class A>
RWvistream& operator>>(RWvistream& strm, RWTValSlist<T , A>*& p);

/**
 * @relates RWTValSlist
 *
 * Looks at the next object on the RWFile \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T , class A>
RWFile& operator>>(RWFile& strm, RWTValSlist<T , A>*& p);

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTValSlist::iterator to iterate over
 * the elements in this collection.
 *
 * @brief Deprecated. Provides an iterator for RWTValSlist.
 *
 * RWTValSlistIterator provides an iterator interface to the
 * C++ Standard Library-based collection represented by RWTValSlist. (An STL
 * iterator interface is provided through RWTValSlist::iterator.)
 *
 * The order of iteration over an RWTValSlist is dependent on the
 * order of insertion of the values into the container.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset(). The iterator becomes
 * valid after being advanced with either a pre-increment or operator()().
 *
 * For both operator++() and operator()(), iterating past the last
 * element returns a value equivalent to \c false. Continued
 * increments return a value equal to \c false until reset() is
 * called.
 *
 * The \c value type must have \c operator== and \c operator< defined.
 * This requirement is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include<rw/tvslist.h>
 * RWTValSlist<T,A> dl;
 * RWTValSlistIterator<T,A> itr(dl);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include<rw/tvslist.h>
 * #include<iostream>
 * #include<rw/cstring.h>
 *
 * int main() {
 *    RWTValSlist<RWCString> a;
 *    RWTValSlistIterator<RWCString> itr(a);
 *
 *    a.insert("John");
 *    a.insert("Steve");
 *    a.insert("Mark");
 *    a.insert("Steve");
 *
 *    while(itr()) {
 *      std::cout << itr.key() << std::endl;
 *    }
 *
 *    return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 *     John
 *     Steve
 *     Mark
 *     Steve
 * @endcode
 */
template <class T, class A = std::allocator<T> >
class RW_DEPRECATE_TYPE("Use RWTValSlist::iterator instead") RWTValSlistIterator
{
private:
    friend class RWTValSlistConstIterator<T, A>;
    RWTValSlist<T, A>* list_;
    typename RWTValSlist<T, A>::iterator iter_;
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
     * Creates an iterator for the singly linked list \a s. The iterator
     * begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTValSlistIterator(RWTValSlist<T, A>& s)
        : list_(&s), head_(true), tail_(false) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self references the first element.
     * If, before iteration, self referenced the last value in the
     * list, self now references an undefined value distinct from
     * the reset value, and the operator returns \c false. Otherwise,
     * returns \c true.
     */
    bool operator++();

    /**
     * Behaves as if the operator++() member function had been applied
     * \a n times.
     */
    bool operator+=(size_t n);

    /**
     * Advances self to the next element and returns \c true. If the iterator
     * has advanced past the last element in the collection, returns \c false.
     */
    bool operator()() {
        return ++(*this);
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    RWTValSlist<T, A>* container() const {
        return list_;
    }

    /**
     * Advances self to the first element \c t encountered by iterating
     * forward, such that the expression <tt>(t == \a a)</tt> is \c true.
     * Returns \c true if an element was found, otherwise returns \c false.
     */
    bool findNext(const T& a);

    /**
     * Advances self to the first element \c t encountered by iterating
     * forward such that the expression <tt>((*\a fn)(t,\a d))</tt> is
     * \c true. Client data may be passed through parameter \a d. Returns
     * \c true if an element was found, otherwise returns \c false.
     */
    bool findNext(bool (*fn)(const T&, void*), void* d);

    /**
     * Inserts the pointer \a p into the container directly after the
     * element referenced by self.
     */
    void insertAfterPoint(const T& p);

    /**
     * Returns a reference to the stored value.
     */
    T& key() const {
        return *iter_;
    }

    /**
     * Removes the value referenced by self from the collection.
     * Returns \c true if the removal is successful, otherwise \c false.
     */
    bool remove();  // Remove item at cursor

    /**
     * Removes the first element \c t, encountered by iterating self
     * forward such that the expression <tt>(t == \a a)</tt> is \c true.
     * Returns \c true if an element was found and removed, otherwise returns
     * \c false.
     */
    bool removeNext(const T& a);

    /**
     * Removes the first element \c t, encountered by iterating self
     * forward such that the expression <tt>((*\a fn)(t,\a d))</tt> is
     * \c true. Client data may be passed through parameter \a d. Returns
     * \c true if an element was found and removed, otherwise returns \c false.
     */
    bool removeNext(bool (*fn)(const T&, void*), void* d);

    /**
     * Resets the iterator so that after being advanced, it references
     * the first element of the collection.
     */
    void reset() {
        head_ = true;
        tail_ = false;
    }

    /**
     * Resets the iterator so that after being advanced, it references
     * the first element of the collection \a l.
     */
    void reset(RWTValSlist<T, A>& l) {
        head_ = true;
        tail_ = false;
        list_ = &l;
    }

private:

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int) {
        return false;
    }
};


/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTValSlist::const_iterator to iterate
 * over the elements in this collection.
 *
 * @brief Deprecated. Provides a \c const iterator for RWTValSlist.
 *
 * RWTValSlistConstIterator provides a \c const iterator interface for
 * RWTValSlist. A \c const iterator prohibits any changes to the
 * collection over which it iterates.
 *
 * The order of iteration over an RWTValSlist is dependent on the
 * order of insertion of the values into the container.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset(). The iterator becomes
 * valid after being advanced with either a pre-increment or operator()().
 *
 * For both operator++() and operator()(), iterating past the last
 * element returns a value equivalent to boolean \c false. Continued
 * increments return a value equal to \c false until reset() is
 * called.
 *
 * The \c value type must have \c operator== and \c operator< defined.
 * This requirement is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvslist.h>
 * template <class T, class A=std::allocator<T> > class
 * RWTValSlistConstIterator
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class T, class A = std::allocator<T> >
class RW_DEPRECATE_TYPE("Use RWTValSlist::const_iterator instead") RWTValSlistConstIterator
{
private:
    const RWTValSlist<T, A>* list_;
    typename RWTValSlist<T, A>::const_iterator iter_;
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
     * Creates a \c const iterator for the singly linked list \a s.
     * The iterator begins in an undefined state and must be advanced
     * before the first element is accessible.
     */
    RWTValSlistConstIterator(const RWTValSlist<T, A>& s)
        : list_(&s), head_(true), tail_(false) { }

    /**
     * Creates a \c const iterator from the iterator \a itr.
     * The \c const iterator begins in an undefined state and must be advanced
     * before the first element is accessible.
     */
    RWTValSlistConstIterator(const RWTValSlistIterator<T, A>& itr)
        : list_(itr.list_), iter_(itr.iter_),
          head_(itr.head_), tail_(itr.tail_) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self references the first element. If,
     * before iteration, self referenced the last value in the list,
     * self now references an undefined value distinct from the reset
     * value and the operator returns \c false. Otherwise, returns \c true.
     */
    bool operator++();

    /**
     * Behaves as if the operator++() member function had been applied
     * \a n times.
     */
    bool operator+=(size_t n);

    /**
     * Advances self to the next element and returns \c true. If the iterator
     * has advanced past the last element in the collection, returns \c false.
     */
    bool operator()() {
        return ++(*this);
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    const RWTValSlist<T, A>* container() const {
        return list_;
    }

    /**
     * Advances self to the first element \c t encountered by iterating
     * forward, such that the expression <tt>(t == \a a)</tt> is \c true.
     * Returns \c true if an element was found, otherwise returns \c false.
     */
    bool findNext(const T& a);

    /**
     * Advances self to the first element \c t encountered by iterating
     * forward such that the expression <tt>((*\a fn)(t,\a d))</tt> is
     * \c true. Client data may be passed through parameter \a d. Returns
     * \c true if an element was found, otherwise returns \c false.
     */
    bool findNext(bool (*fn)(const T&, void*), void* d);

    /**
     * Returns the stored value referenced by self.
     */
    const T& key() const {
        return *iter_;
    }

    /**
     * Resets the iterator so that after being advanced, it references
     * the first element of the collection.
     */
    void reset() {
        head_ = true;
        tail_ = false;
    }

    /**
     * Resets the iterator so that after being advanced, it references
     * the first element of the collection \a l.
     */
    void reset(const RWTValSlist<T, A>& l) {
        head_ = true;
        tail_ = false;
        list_ = &l;
    }


private:

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);
};
#endif

#  if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#    pragma implementation ("tvslist.cc")
#  endif

#  ifdef RW_NO_IMPLICIT_INCLUSION
#    include <rw/tvslist.cc>
#  endif

#endif /* RW_CLASSIC_TEMPLATE_CONTAINERS */

#endif /* RW_TOOLS_TVSLIST_H */
