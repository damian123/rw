#ifndef RW_TOOLS_TVSRTDLI_H
#define RW_TOOLS_TVSRTDLI_H

/**********************************************************************
 *
 * tvsrtdli.h - RWTValSortedDlist<T,Compare>
 *     : value-based sorted doubly-linked list wrapper for STL list
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvsrtdli.h#2 $
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
#include <rw/edefs.h> // for rw_move
#include <rw/epfunc.h>
#include <rw/rwerr.h>
#include <rw/toolerr.h>
#include <rw/tools/algorithm.h>
#include <rw/tools/traits/RWTEnableIf.h>
#include <rw/tools/traits/RWTIsIntegral.h>

#include <functional>
#include <list>

#if !defined(RW_DISABLE_DEPRECATED)

template <class T, class C, class A>
class RWTValSortedDlistConstIterator;

#endif

/**
 * @ingroup stl_based_collection_classes
 *
 * @brief Maintains a sorted collection of values implemented
 * as a doubly-linked list.
 *
 * This class maintains an always-sorted collection of values, implemented
 * as a doubly-linked list.
 *
 * Class \c T is the type of items in the collection.
 * Class \c A is an allocator of objects of class \c T. For more
 * information about using custom allocators, please see the
 * <i>Essential Tools Module User's Guide</i>.
 *
 * The \c value type must have \c operator== and \c operator< defined.
 * This requirement is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvsrtdli.h>
 * RWTValSortedDlist<T,C,A> srtdlist;
 * @endcode
 *
 * @section related Related Classes
 *
 * \link RWTValSortedVector RWTValSortedVector<T>\endlink is an alternative
 * always-sorted collections.
 * \link RWTValDlist RWTValDlist<T,A>\endlink is an unsorted doubly-linked list
 * of values.
 *
 * Class list<T,A<T> > is the C++ Standard Library collection that serves
 * as the underlying implementation for this class.
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
 * // tvsdldat.cpp
 * //
 * #include <iostream>
 * #include <functional>
 *
 * #include <rw/tvsrtdli.h>
 * #include <rw/tools/datetime.h>
 *
 * int main ()
 * {
 *     RWTValSortedDlist<RWDateTime, std::less<RWDateTime> > lst;
 *
 *     lst.insert(RWDateTime("Aug 10, 1991", RWDateTime::setDate));
 *     lst.insert(RWDateTime("Aug 9, 1991", RWDateTime::setDate));
 *     lst.insert(RWDateTime("Sep 1, 1991", RWDateTime::setDate));
 *     lst.insert(RWDateTime("May 14, 1990", RWDateTime::setDate));
 *     lst.insert(RWDateTime("Sep 1, 1991", RWDateTime::setDate)); // Add a duplicate
 *     lst.insert(RWDateTime("June 2, 1991", RWDateTime::setDate));
 *
 *     for (size_t i = 0; i < lst.entries (); ++i)
 *         std::cout << lst[i].asString('x') << " ";
 *
 *     std::cout << "\n\n";
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 *     05/14/90 06/02/91 08/09/91 08/10/91 09/01/91 09/01/91
 * @endcode
 */
template <class T, class C = std::less<T>, class A = std::allocator<T> >
class RWTValSortedDlist
{
public:

    /**
     * A type representing the underlying implementation container.
     */
    typedef std::list<T, A> container_type;

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
     * A type representing the comparison function.
     */
    typedef C key_compare;

    /**
     * A type representing the comparison function.
     */
    typedef C value_compare;


    /**
     * @internal
     * A type representing the same type as self.
     */
    typedef RWTValSortedDlist<T, C, A> this_type;


    /**
     * Returns a reference to the underlying C++ Standard Library collection
     * that serves as the implementation for self. It is the application's
     * responsibility to not violate the ordering of the elements within
     * the collection.
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
        std().clear();
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
        const_iterator hit = std::lower_bound(begin(), end(), a, key_compare());
        if (hit == end()) {
            return false;
        }
        return (*hit == a);
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
#if defined(RW_STDLIB_VER) && (RW_STDLIB_VER >= 0x02010100 && RW_STDLIB_VER <= 0x02020100)
        std().erase(first, last);
        return last;
#else
        return std().erase(first, last);
#endif
    }

    /**
     * If there exists an element \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, assigns \c t to \a k and
     * returns \c true. Otherwise, returns \c false and leaves the value
     * of \a k unchanged. Client data may be passed through parameter \a d.
     */
    bool find(bool(*fn)(const_reference, void*), void* d, reference k) const {
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
    bool find(const_reference a, reference k) const {
        const_iterator hit = std::lower_bound(begin(), end(), a, key_compare());
        if (hit != end() && *hit == a) {
            k = *hit;
            return true;
        }
        return false;
    }

    /**
     * Returns a reference to the first element of self.
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
     * Adds the item \a a to self. The collection remains sorted.
     * Returns \c true.
     */
    bool insert(const_reference a) {
        insertSorted(begin(), a);  // only fail thru exception
        return true;
    }

    /**
     * Inserts \a val into self. Returns an iterator to the new item.
     *
     * The \a hint parameter indicates where in the collection the new value
     * should be inserted. This value is only a hint, and will be ignored if
     * the value should be inserted at a different location according to the
     * containers comparison object.
     */
    iterator insert(iterator hint, const_reference val) {
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

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @copydoc insert()
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    bool insert(value_type && a) {
        insertSorted(begin(), rw_move(a));  // only fail thru exception
        return true;
    }

    /**
     * @copydoc insert(iterator, const_reference)
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    iterator insert(iterator hint, value_type && val) {
        return insertSorted(hint, rw_move(val));
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Adds the items from \a a to self, preserving order.
     * Returns the number of items inserted into self.
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
        std::pair<const_iterator, const_iterator> range =
            std::equal_range(begin(), end(), a, key_compare());
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_distance(range.first, range.second);
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
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
     * Removes the first element \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true and returns \c true. Returns
     * \c false if there is no such element. Client data may be passed
     * through parameter \a d.
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
     * Removes the first element \c t in self such that the expression
     * <tt>(t == \a a)</tt> is \c true and returns \c true. Returns \c false
     * if there is no such element.
     */
    bool remove(const_reference a) {
        iterator hit = std::lower_bound(begin(), end(), a, key_compare());
        if (hit != end() && *hit == a) {
            std().erase(hit);
            return true;
        }
        return false;
    }

    /**
     * Removes all elements \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns the number of
     * items removed. Client data may be passed through parameter \a d.
     */
    size_type removeAll(bool(*fn)(const_reference, void*), void* d) {
        size_type size = entries();
        iterator iter = std::remove_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        std().erase(iter, end());
        return size - entries();
    }

    /**
     * Removes all elements \c t in self such that the expression
     * <tt>(t == \a a)</tt> is \c true. Returns the number of items
     * removed.
     */
    size_type removeAll(const_reference a);

    /**
     * Removes and returns the item at position \a i in self.
     *
     * @throw RWBoundsErr Thrown if index \a i is not between
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
     * Returns the position of the first item \c t in self such that
     * <tt>(t == \a a)</tt>, or returns #RW_NPOS if no such item exists.
     */
    size_type index(const_reference a) const;

    /**
     * Returns the position of the first item \c t in self such that
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, or returns #RW_NPOS if
     * no such item exists. Client data may be passed through parameter
     * \a d.
     */
    size_type index(bool (*fn)(const_reference, void*), void* d) const;

    /**
     * Invokes the function pointer \a fn on each item in the collection.
     * Client data may be passed through parameter \a d.
     */
    void apply(void (*fn)(const_reference, void*), void* d) const {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * Returns a reference to the \a i <sup>th</sup> element of self.
     *
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0 and one
     * less than the number of entries in self.
     */
    const_reference operator[](size_type i) const {
        boundsCheck(i);
        const_iterator iter = begin();
        std::advance(iter, i);
        return *iter;
    }

    /**
     * Returns a reference to the \a i <sup>th</sup> element of self. Index
     * \a i should be between \c 0 and one less than the number of entries,
     * otherwise the results are undefined.
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
     *
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0 and one
     * less than the number of entries in self.
     */
    const_reference at(size_type i) const {
        return (*this)[i];
    }


    /**
     * Returns a reference to the last item in the collection.
     *
     * Calling this function on an empty collection results in
     * undefined behavior.
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
        const value_type ret = last();
        std().pop_back();
        return ret;
    }


    // (Can't use STL list<T>::merge mf until member function templates)
    /**
     * Inserts all elements of \a dl into self, preserving sorted order.
     */
    size_type merge(const RWTValSortedDlist<T, C, A>& coll);

    /**
     * Destroys all elements of self and replaces them by copying (and
     * sorting, if necessary) all elements of \a rhs.
     */
    RWTValSortedDlist<T, C, A>&
    operator=(const RWTValSortedDlist<T, C, A>& rhs) {
        if (&rhs != this) {
            impl_ = rhs.impl_;
        }
        return *this;
    }

    /**
     * @copydoc operator=()
     */
    RWTValSortedDlist<T, C, A>&
    operator=(const container_type& rhs) {
        if (&impl_ != &rhs) {
            impl_ = rhs;
            rw_list_sort(impl_, key_compare());
        }
        return *this;
    }

    /**
     * Constructs an empty doubly-linked list.
     */
    RWTValSortedDlist() { }

    /**
     * Copy constructor.
     */
    RWTValSortedDlist(const RWTValSortedDlist<T, C, A>& t) : impl_(t.impl_) { }

    /**
     * Constructs a doubly-linked list by copying and sorting all elements
     * of \a lst.
     */
    RWTValSortedDlist(const container_type& lst) : impl_(lst) {
        rw_list_sort(impl_, key_compare());
    }

    /**
     * Constructs a doubly-linked list with \a n elements, each initialized
     * to \a val.
     */
    RWTValSortedDlist(size_type n, const_reference val = value_type()) : impl_(n, val) {}

    /**
     * Constructs a doubly-linked list by copying elements from the range
     * [\a first, \a last).
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
    template <typename InputIterator>
#if !defined(DOXYGEN)
    RWTValSortedDlist(InputIterator first, InputIterator last,
                      typename RWTEnableIf < !RWTIsIntegral<InputIterator>::value >::type** = 0)
#else
    RWTValSortedDlist(InputIterator first, InputIterator last)
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

#if !defined(RW_NO_RVALUE_REFERENCES)

    /**
     * Move constructor. The constructed instance takes ownership of the
     * data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValSortedDlist(RWTValSortedDlist && rhs)
#    if !defined(RW_NO_STDLIST_RVALUE_SUPPORT)
        : impl_(rw_move(rhs.impl_)) { }
#    else
    {
        rhs.std().swap(impl_);
    }
#    endif

    /**
     * Move constructor. The constructed instance sorts and takes ownership of
     * the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValSortedDlist(container_type && rhs) {
#    if !defined(RW_NO_STDLIST_RVALUE_SUPPORT)
        impl_ = rw_move(rhs);
#    else
        rhs.swap(impl_);
#    endif
        rw_list_sort(impl_, key_compare());
    }

    /**
     * Move assignment. Self takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValSortedDlist&
    operator=(RWTValSortedDlist && rhs) {
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
    RWTValSortedDlist<T, C, A>&
    operator=(container_type && rhs) {
#    if !defined(RW_NO_STDLIST_RVALUE_SUPPORT)
        impl_ = rw_move(rhs);
#    else
        rhs.swap(impl_);
#    endif
        rw_list_sort(impl_, key_compare());
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    // ~RWTValSortedDlist(); // handled by auto-generated destructor

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWTValSortedDlist& rhs) {
        std().swap(rhs.impl_);
    }

private:
    void boundsCheck(size_type i) const {
        if (i >= entries()) {
            RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEXERR, i, entries())));
        }
    }

    iterator insertSorted(iterator, const_reference a) {
        iterator hit = std::upper_bound(begin(), end(), a, key_compare());
        return std().insert(hit, a);
    }

#  if !defined(RW_NO_RVALUE_REFERENCES)
    iterator insertSorted(iterator, value_type && a) {
        iterator hit = std::upper_bound(begin(), end(), a, key_compare());
        return std().insert(hit, rw_move(a));
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    container_type impl_;
};


/**
 * @relates RWTValSortedDlist
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and
 * iterating through both collections produces individual elements that,
 * in turn, compare equal to each other.
 */
template <class T, class C, class A>
bool operator==(const RWTValSortedDlist<T, C, A>& lhs, const RWTValSortedDlist<T, C, A>& rhs)
{
    return lhs.std() == rhs.std();
}

/**
 * @relates RWTValSortedDlist
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T, class C, class A>
bool operator!=(const RWTValSortedDlist<T, C, A>& lhs, const RWTValSortedDlist<T, C, A>& rhs)
{
    return lhs.std() != rhs.std();
}

/**
 * @relates RWTValSortedDlist
 *
 * Returns \c true if \a lhs is lexicographically less than \a rhs,
 * otherwise \c false. Assumes that type \c T has well-defined less-than
 * semantics.
 */
template <class T, class C, class A>
bool operator<(const RWTValSortedDlist<T, C, A>& lhs, const RWTValSortedDlist<T, C, A>& rhs)
{
    return lhs.std() < rhs.std();
}

/**
 * @relates RWTValSortedDlist
 *
 * Equivalent to <tt>(\a rhs < \a lhs)</tt>.
 */
template <class T, class C, class A>
bool operator>(const RWTValSortedDlist<T, C, A>& lhs, const RWTValSortedDlist<T, C, A>& rhs)
{
    return lhs.std() > rhs.std();
}

/**
 * @relates RWTValSortedDlist
 *
 * Equivalent to <tt>!(\a rhs < \a lhs)</tt>.
 */
template <class T, class C, class A>
bool operator<=(const RWTValSortedDlist<T, C, A>& lhs, const RWTValSortedDlist<T, C, A>& rhs)
{
    return lhs.std() <= rhs.std();
}

/**
 * @relates RWTValSortedDlist
 *
 * Equivalent to <tt>!(\a lhs < \a rhs)</tt>.
 */
template <class T, class C, class A>
bool operator>=(const RWTValSortedDlist<T, C, A>& lhs, const RWTValSortedDlist<T, C, A>& rhs)
{
    return lhs.std() >= rhs.std();
}

/**
 * @relates RWTValSortedDlist
 *
 * @copydoc operator==(const RWTValSortedDlist&, const RWTValSortedDlist&);
 */
template <class T, class C, class A>
bool operator==(const RWTValSortedDlist<T, C, A>& lhs,
                const typename RWTValSortedDlist<T, C, A>::container_type& rhs)
{
    return lhs.std() == rhs;
}

/**
 * @relates RWTValSortedDlist
 *
 * @copydoc operator!=(const RWTValSortedDlist&, const RWTValSortedDlist&);
 */
template <class T, class C, class A>
bool operator!=(const RWTValSortedDlist<T, C, A>& lhs,
                const typename RWTValSortedDlist<T, C, A>::container_type& rhs)
{
    return lhs.std() != rhs;
}

/**
 * @relates RWTValSortedDlist
 *
 * @copydoc operator<(const RWTValSortedDlist&, const RWTValSortedDlist&);
 */
template <class T, class C, class A>
bool operator<(const RWTValSortedDlist<T, C, A>& lhs,
               const typename RWTValSortedDlist<T, C, A>::container_type& rhs)
{
    return lhs.std() < rhs;
}

/**
 * @relates RWTValSortedDlist
 *
 * @copydoc operator>(const RWTValSortedDlist&, const RWTValSortedDlist&);
 */
template <class T, class C, class A>
bool operator>(const RWTValSortedDlist<T, C, A>& lhs,
               const typename RWTValSortedDlist<T, C, A>::container_type& rhs)
{
    return lhs.std() > rhs;
}

/**
 * @relates RWTValSortedDlist
 *
 * @copydoc operator<=(const RWTValSortedDlist&, const RWTValSortedDlist&);
 */
template <class T, class C, class A>
bool operator<=(const RWTValSortedDlist<T, C, A>& lhs,
                const typename RWTValSortedDlist<T, C, A>::container_type& rhs)
{
    return lhs.std() <= rhs;
}

/**
 * @relates RWTValSortedDlist
 *
 * @copydoc operator>=(const RWTValSortedDlist&, const RWTValSortedDlist&);
 */
template <class T, class C, class A>
bool operator>=(const RWTValSortedDlist<T, C, A>& lhs,
                const typename RWTValSortedDlist<T, C, A>::container_type& rhs)
{
    return lhs.std() >= rhs;
}


/**
 * @relates RWTValSortedDlist
 *
 * @copydoc operator==(const RWTValSortedDlist&, const RWTValSortedDlist&);
 */
template <class T, class C, class A>
bool operator==(const typename RWTValSortedDlist<T, C, A>::container_type& lhs,
                const RWTValSortedDlist<T, C, A>& rhs)
{
    return lhs == rhs.std();
}

/**
 * @relates RWTValSortedDlist
 *
 * @copydoc operator!=(const RWTValSortedDlist&, const RWTValSortedDlist&);
 */
template <class T, class C, class A>
bool operator!=(const typename RWTValSortedDlist<T, C, A>::container_type& lhs,
                const RWTValSortedDlist<T, C, A>& rhs)
{
    return lhs != rhs.std();
}

/**
 * @relates RWTValSortedDlist
 *
 * @copydoc operator<(const RWTValSortedDlist&, const RWTValSortedDlist&);
 */
template <class T, class C, class A>
bool operator<(const typename RWTValSortedDlist<T, C, A>::container_type& lhs,
               const RWTValSortedDlist<T, C, A>& rhs)
{
    return lhs < rhs.std();
}

/**
 * @relates RWTValSortedDlist
 *
 * @copydoc operator>(const RWTValSortedDlist&, const RWTValSortedDlist&);
 */
template <class T, class C, class A>
bool operator>(const typename RWTValSortedDlist<T, C, A>::container_type& lhs,
               const RWTValSortedDlist<T, C, A>& rhs)
{
    return lhs > rhs.std();
}

/**
 * @relates RWTValSortedDlist
 *
 * @copydoc operator<=(const RWTValSortedDlist&, const RWTValSortedDlist&);
 */
template <class T, class C, class A>
bool operator<=(const typename RWTValSortedDlist<T, C, A>::container_type& lhs,
                const RWTValSortedDlist<T, C, A>& rhs)
{
    return lhs <= rhs.std();
}

/**
 * @relates RWTValSortedDlist
 *
 * @copydoc operator>=(const RWTValSortedDlist&, const RWTValSortedDlist&);
 */
template <class T, class C, class A>
bool operator>=(const typename RWTValSortedDlist<T, C, A>::container_type& lhs,
                const RWTValSortedDlist<T, C, A>& rhs)
{
    return lhs >= rhs.std();
}

/**
 * @relates RWTValSortedDlist
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T, class C , class A>
RWvostream& operator<<(RWvostream& strm, const RWTValSortedDlist<T, C , A>& coll);

/**
 * @relates RWTValSortedDlist
 *
 * Saves the collection \a coll onto the RWFile \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T, class C , class A>
RWFile& operator<<(RWFile& strm, const RWTValSortedDlist<T, C , A>& coll);

/**
 * @relates RWTValSortedDlist
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class T, class C , class A>
RWvistream& operator>>(RWvistream& strm, RWTValSortedDlist<T, C , A>& coll);

/**
 * @relates RWTValSortedDlist
 *
 * Restores the contents of the collection \a coll from the RWFile
 * \a strm.
 */
template <class T, class C , class A>
RWFile& operator>>(RWFile& strm, RWTValSortedDlist<T, C , A>& coll);

/**
 * @relates RWTValSortedDlist
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T, class C , class A>
RWvistream& operator>>(RWvistream& strm, RWTValSortedDlist<T, C , A>*& p);

/**
 * @relates RWTValSortedDlist
 *
 * Looks at the next object on the RWFile \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T, class C , class A>
RWFile& operator>>(RWFile& strm, RWTValSortedDlist<T, C , A>*& p);

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @ingroup stl_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTValSortedDlist::iterator to iterate
 * over the elements in this collection.
 *
 * @brief Deprecated. Provides an iterator for RWTValSortedDlist.
 *
 * RWTValSortedDlistIterator provides an iterator interface to the
 * C++ Standard Library-based collection represented by RWTValSortedDlist.
 * (An STL iterator interface is provided through RWTValSortedDlist::iterator.)
 *
 * The order of iteration over an RWTValSortedDlist is dependent
 * on the supplied comparator object supplied as applied to the
 * values stored in the container.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset(). The iterator becomes
 * valid after being advanced with either a pre-increment or operator()().
 *
 * For both operator++() and operator()(), iterating past the last
 * element returns a value equivalent to \c false. Continued
 * increments returns a value equivalent to \c false until reset()
 * is called.
 *
 * The \c value type must have \c operator== and \c operator< defined.
 * This requirement is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include<rw/tvsrtdli.h>
 * RWTValSortedDlist<T,C,A> dl;
 * RWTValSortedDlistIterator<T,C,A> itr(dl);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/tvsrtdli.h>
 * #include <iostream>
 * #include <rw/cstring.h>
 * #include <functional>
 *
 * int main() {
 *    RWTValSortedDlist<RWCString, std::less<RWCString> > a;
 *    RWTValSortedDlistIterator<RWCString, std::less<RWCString> > itr(a);
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
 *     Mark
 *     Steve
 *     Steve
 * @endcode
 */
template <class T, class C = std::less<T>, class A = std::allocator<T> >
class RW_DEPRECATE_TYPE("Use RWTValSortedDlist::iterator instead") RWTValSortedDlistIterator
{

public:
    /**
     * Creates an iterator for the sorted dlist \a s. The iterator
     * begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTValSortedDlistIterator(RWTValSortedDlist<T, C, A>& s)
        : list_(&s), head_(true), tail_(false) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self references the first element.
     * If, before iteration, self referenced the last value in the
     * list, self now points to an undefined value distinct from
     * the reset value, and the operator returns \c false. Otherwise, returns
     * \c true.
     */
    bool operator++();

    /**
     * Moves self back to the immediately previous element. If the
     * iterator has been reset or just created, returns
     * \c false, otherwise returns \c true. If self references
     * the first element, it is now in the reset state. If self
     * has been iterated past the last value in the list, it now
     * points to the last item in the list.
     */
    bool operator--();

    /**
     * Behaves as if the operator++() member function had been applied
     * \a n times.
     */
    bool operator+=(size_t n);

    /**
     * Behaves as if the operator--() member function had been applied
     * \a n times.
     */
    bool operator-=(size_t n);

    /**
     * Advances self to the next element and returns \c true. If the
     * iterator has advanced past the last element in the collection,
     * returns \c false.
     */
    bool operator()() {
        return ++(*this);
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    RWTValSortedDlist<T, C, A>* container() const {
        return list_;
    }

    /**
     * Advances self to the first element \c t encountered by iterating
     * forward, such that the expression <tt>(t == \a a)</tt> is \c true.
     * Returns \c true if such an element if found, otherwise \c false.
     */
    bool findNext(const T& a);

    /**
     * Advances self to the first element \c t encountered by iterating
     * forward, such that the expression <tt>((*\a fn)(t,\a d))</tt> is
     * \c true. Returns \c true if such an element if found, otherwise \c false.
     * Client data may be passed through parameter \a d.
     */
    bool findNext(bool (*fn)(const T&, void*), void* d);

    /**
     * Returns the stored value referenced by self.
     */
    T key() const {
        return *(RW_CONST_CAST(RWTValSortedDlistIterator*, this)->iter_);
    }

    /**
     * Removes the stored value referenced by self from the collection.
     * Returns \c true if the value was successfully removed,
     * otherwise \c false.
     */
    bool remove(); // Remove item at cursor

    /**
     * Removes the first element \c t, encountered by iterating self
     * forward, such that the expression <tt>(t == \a a)</tt> is \c true.
     * Returns \c true if such an element is successfully removed,
     * otherwise \c false.
     */
    bool removeNext(const T& a);

    /**
     * Removes the first element \c t, encountered by iterating self forward
     * such that the expression <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns
     * \c true if such an element is successfully removed, otherwise \c false.
     * Client data may be passed through parameter \a d.
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
    void reset(RWTValSortedDlist<T, C, A>& l) {
        head_ = true;
        tail_ = false;
        list_ = &l;
    }

private:
    friend class RWTValSortedDlistConstIterator<T, C, A>;
    RWTValSortedDlist<T, C, A>* list_;
    typename std::list<T, A>::iterator iter_;
    bool head_;  // simulate head sentinel
    bool tail_;  // simulate tail sentinel

    // Disallow postfix increment & decrement. Unless we hide it, some
    // compilers will substitute the prefix operator in its place.
    bool operator++(int);
    bool operator--(int);

};


/**
 * @ingroup stl_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTValSortedDlist::const_iterator to
 * iterate over the elements in this collection.
 *
 * @brief Deprecated. Provides a \c const iterator for RWTValSortedDlist.
 *
 * RWTValSortedDlistConstIterator provides a \c const iterator to
 * RWTValSortedDlist. A \c const iterator prohibits changes to the
 * collection over which it iterates.
 *
 * The order of iteration over an RWTValSortedDlist is dependent
 * on the supplied comparator object as applied to the
 * values stored in the container.
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
 * #include <rw/tvsrtdli.h>
 * template <class T, class C, class A=std::allocator<T> >
 * class RWTValSortedDlistConstIterator
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class T, class C = std::less<T>, class A = std::allocator<T> >
class RW_DEPRECATE_TYPE("Use RWTValSortedDlist::const_iterator instead") RWTValSortedDlistConstIterator
{
public:
    /**
     * Creates a \c const iterator for the sorted dlist \a s. The iterator
     * begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTValSortedDlistConstIterator(const RWTValSortedDlist<T, C, A>& s)
        : list_(&s), head_(true), tail_(false) { }

    /**
     * Creates a \c const iterator from the iterator \a itr. The \c const
     * iterator begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTValSortedDlistConstIterator(const RWTValSortedDlistIterator<T, C, A>& itr)
        : list_(itr.list_), iter_(itr.iter_),
          head_(itr.head_), tail_(itr.tail_) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self references the first element. If,
     * before iteration, self referenced the last value in the list,
     * self now points to an undefined value distinct from the reset
     * value and the operator returns \c false. Otherwise, the operator
     * returns \c true.
     */
    bool operator++();

    /**
     * Moves self back to the immediately previous element. If the
     * iterator has been reset or just created, this operator returns
     * \c false, otherwise it returns \c true. If self references the
     * first element, it is now in the reset state. If self has been
     * iterated past the last value in the list, it now points to the
     * last item in the list.
     */
    bool operator--();

    /**
     * Behaves as if the operator++() member function had been applied
     * \a n times.
     */
    bool operator+=(size_t n);

    /**
     * Behaves as if the operator--() member function had been applied
     * \a n times.
     */
    bool operator-=(size_t n);

    /**
     * Advances self to the next element and returns \c true. If the
     * iterator has advanced past the last element in the collection,
     * returns \c false.
     */
    bool operator()() {
        return ++(*this);
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    const RWTValSortedDlist<T, C, A>* container() const {
        return list_;
    }

    /**
     * Advances self to the first element \c t encountered by iterating
     * forward, such that the expression <tt>(t == \a a)</tt> is \c true.
     * Returns \c true if such an element if found, otherwise \c false.
     */
    bool findNext(const T& a);

    /**
     * Advances self to the first element \c t encountered by iterating
     * forward, such that the expression <tt>((*\a fn)(t,\a d))</tt> is
     * \c true. Returns \c true if such an element if found, otherwise \c false.
     * Client data may be passed through parameter \a d.
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
    void reset(const RWTValSortedDlist<T, C, A>& l) {
        head_ = true;
        tail_ = false;
        list_ = &l;
    }

private:
    const RWTValSortedDlist<T, C, A>* list_;
    typename std::list<T, A>::const_iterator iter_;
    bool head_;  // simulate head sentinel
    bool tail_;  // simulate tail sentinel

    // Disallow postfix increment & decrement. Unless we hide it, some
    // compilers will substitute the prefix operator in its place.
    bool operator++(int);
    bool operator--(int);
};
#endif

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tvsrtdli.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tvsrtdli.cc"
#endif


#endif /* RW_TOOLS_TVSRTDLI_H */
