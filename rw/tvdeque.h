#ifndef RW_TOOLS_TVDEQUE_H
#define RW_TOOLS_TVDEQUE_H

/**********************************************************************
 *
 *     : value-based double-ended queue wrapper for STL deque
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvdeque.h#2 $
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

#include <algorithm>
#include <deque>

/**
 * @ingroup stl_based_collection_classes
 * @brief Collection of values implemented as a
 * double-ended queue, or \e deque.
 *
 * This class maintains a collection of values implemented as a
 * double-ended queue, or \e deque. Order is determined externally
 * and elements are accessible by index. Use this class when insertions
 * and deletions usually occur at either the beginning or the end
 * of the collection. Class \c T is the type of items in the collection.
 * Class \c A is an allocator of objects of class \c T. For more information
 * about using custom allocators, please see the <i>Essential Tools
 * Module User's Guide</i>.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvdeque.h>
 * RWTValDeque<T,A> deq;
 * @endcode
 *
 * @section related Related Classes
 *
 * Classes \link RWTValSlist RWTValSlist<T,A>\endlink,
 * \link RWTValDlist RWTValDlist<T,A>\endlink,
 * \link RWTValSortedDlist RWTValSortedDlist<T>\endlink,
 * and \link RWTValOrderedVector RWTValOrderedVector<T,A>\endlink also provide
 * a Rogue Wave interface to C++ Standard Library sequence collections. The
 * list classes should be considered for frequent insertions (or removals) in
 * the interior of the collection. Vectors may be more efficient if most
 * insertions and removals occur at the end of the collection.
 *
 * Class <b>std::deque<T, A<T*> ></b> is the C++ Standard Library collection
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
 * // tvdqint.cpp
 * //
 * #include <rw/tvdeque.h>
 * #include <iostream>
 *
 * int main ()
 * {
 *     RWTValDeque<int> numbers;
 *
 *     int n;
 *
 *     std::cout << "Input an assortment of integers (EOF to end):" << std::endl;
 *
 *     while (std::cin >> n) {
 *         if (n % 2 == 0)
 *             numbers.pushFront(n);
 *         else
 *             numbers.pushBack(n);
 *     }
 *
 *     std::cout << "Partitioned numbers are: ";
 *     for (int i = 0; numbers.entries(); ++i) {
 *         if (i)
 *             std::cout << ", ";
 *         std::cout << numbers.popFront();
 *     }
 *
 *     std::cout << "\n";
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Input:
 *
 * @code
 * 1 2 3 4 5
 * <eof>
 * @endcode
 *
 * Program Output:
 *
 * @code
 * Input an assortment of integers (EOF to end):
 * Partitioned numbers are: 4, 2, 1, 3, 5
 * @endcode
 *
 */
template <class T, class A = std::allocator<T> >
class RWTValDeque
{
public:

    /**
     * A type representing the underlying implementation container.
     */
    typedef std::deque<T, A> container_type;

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
    typedef RWTValDeque<T, A> this_type;


    /**
     * Returns a reference to the underlying C++ Standard Library collection
     * that serves as the implementation for self. This reference may
     * be used freely, providing access to the C++ Standard Library interface
     * as well as interoperability with other software components that
     * make use of C++ Standard collections.
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
        std().insert(end(), a);
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @copydoc append()
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    void append(value_type && a) {
        std().insert(end(), rw_move(a));
    }
#endif // !RW_NO_RVALUE_REFERENCES

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
#if !defined(RW_BROKEN_STD_DEQUE_ASSIGN_RANGE)
        std().assign(first, last);
#else
        typedef typename rw_iterator_traits<InputIterator>::iterator_category cat;
        assign(first, last, cat());
#endif
    }

    /**
     * Returns a reference to the \a i <sup>th</sup> element of self.
     *
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0
     * and one less than the number of entries in self.
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
     * item will have its destructor called.
     */
    void clear() {
        std().clear();
    }

    /**
     * Returns \c true if there exists an element \c t in self such that
     * the expression <tt>((*\a fn)(t,\a d))</tt> is \c true, otherwise
     * returns \c false. Client data may be passed through parameter \a d.
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
#if defined(RW_STDLIB_VER) && (RW_STDLIB_VER >= 0x02010100 && RW_STDLIB_VER <= 0x02020100)
        iterator ret = std().erase(first, last);
        return entries() ? ret : end();
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
     * Returns the position of the first item \c t in self such that
     * <tt>(t == \a a)</tt>, or returns #RW_NPOS if no such item exists.
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
     * Adds the item \a a to the end of the collection. Returns
     * \c true.
     */
    bool insert(const_reference a) { // fail only thru exception
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
#if !defined(RW_BROKEN_STD_DEQUE_INSERT_RANGE)
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
    *
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

#if !defined(RW_NO_RVALUE_REFERENCES)
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
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Returns \c true if there are no items in the collection, otherwise
     * \c false.
     */
    bool isEmpty() const {
        return std().empty();
    }

    /**
     * Returns a reference to the minimum element in the collection.
     * Type \c T must have well-defined less-than semantics
     * (<tt>T::operator<(const T&)</tt> or equivalent).
     */
    reference minElement() {
        RW_PRECONDITION(!isEmpty());
        return *std::min_element(begin(), end());
    }

    /**
     * @copydoc minElement()
     */
    const_reference minElement() const {
        RW_PRECONDITION(!isEmpty());
        return *std::min_element(begin(), end());
    }

    /**
     * Returns a reference to the maximum element in the collection.
     * Type \c T must have well-defined less-than semantics
     * (<tt>T::operator<(const T&)</tt> or equivalent).
     */
    reference maxElement() {
        RW_PRECONDITION(!isEmpty());
        return *std::max_element(begin(), end());
    }

    /**
     * @copydoc maxElement()
     */
    const_reference maxElement() const {
        RW_PRECONDITION(!isEmpty());
        return *std::max_element(begin(), end());
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
     *
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0 and one
     * less than the number of entries in self.
     */
    reference operator()(size_type i) {
        RW_PRECONDITION(i < entries());
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
        RW_PRECONDITION(i < entries());
        return std()[i];
    }

    /**
     * Returns a reference to the \a i <sup>th</sup> element of self.
     *
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
    void prepend(const_reference a) {
        std().insert(begin(), a);
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @copydoc prepend()
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    void prepend(value_type && a) {
        std().insert(begin(), rw_move(a));
    }
#endif // !RW_NO_RVALUE_REFERENCES

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
     * <tt>(t == \a a)</tt> is \c true and returns \c true. Returns \c false if
     * there is no such element.
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
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, and returns \c true. Returns
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
     * Removes all elements \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns the number of
     * items removed. Client data may be passed through parameter
     * \a d.
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
    size_type removeAll(const_reference a) {
        size_type size = entries();
        iterator iter = std::remove(begin(), end(), a);
        std().erase(iter, end());
        return size - entries();
    }

    /**
     * Removes and returns the item at position \a i in self.
     *
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0 and one
     * less than the number of entries in self.
     */
    value_type removeAt(size_type index);

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
     * Replaces all elements \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true with \a newVal. Returns
     * the number of items replaced. Client data may be passed
     * through parameter \a d.
     */
    size_type replaceAll(bool(*fn)(const value_type&, void*), void* d, const value_type& newVal) {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_and_replace_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d), newVal);
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Replaces all elements \c t in self such that the expression
     * <tt>(t == \a oldVal)</tt> is \c true with \a newVal. Returns
     * the number of items replaced.
     */
    size_type replaceAll(const value_type& oldVal, const value_type& newVal) {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_and_replace(begin(), end(), oldVal, newVal);
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
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
     * Adds the item \a a to the end of the collection.
     */
    void pushBack(const_reference a) {
        std().push_back(a);
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @copydoc pushBack()
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    void pushBack(value_type && a) {
        std().push_back(rw_move(a));
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Adds the item \a a to the beginning of the collection.
     */
    void pushFront(const_reference a) {
        std().push_front(a);
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @copydoc pushFront()
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    void pushFront(value_type && a) {
        std().push_front(rw_move(a));
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * @copydoc removeLast()
     */
    value_type popBack() {
        RW_PRECONDITION(!isEmpty());
        return removeLast();
    }

    /**
     * @copydoc removeFirst()
     */
    value_type popFront() {
        RW_PRECONDITION(!isEmpty());
        return removeFirst();
    }

    /**
     * Sorts the collection using the less-than operator (\c <) to compare
     * elements.
     */
    void sort() {
        std::sort(begin(), end());
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
     * Calls the destructor on all elements of self and replaces them
     * by copying all elements of \a rhs.
     */
    RWTValDeque<T, A>&
    operator=(const RWTValDeque<T, A>& rhs) {
        return operator=(rhs.impl_);
    }

    /**
     * @copydoc operator=()
     */
    RWTValDeque<T, A>&
    operator=(const container_type& rhs) {
        if (&impl_ != &rhs) {
            impl_ = rhs;
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
    RWTValDeque<T, A>&
    operator=(RWTValDeque<T, A> && rhs) {
        return operator=(rw_move(rhs.impl_));
    }

    /**
     * @copydoc operator=(RWTValDeque<T,A>&&)
     */
    RWTValDeque<T, A>&
    operator=(container_type && rhs) {
        if (&impl_ != &rhs) {
#    if !defined(RW_NO_STDDEQUE_RVALUE_SUPPORT)
            impl_ = rw_move(rhs);
#    else
            rhs.swap(impl_);
#    endif
        }
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs an empty, double-ended queue.
     */
    RWTValDeque() : impl_() { }

    /**
     * Constructs a double-ended queue by copying all elements of \a deq.
     */
    RWTValDeque(const container_type& deq) : impl_(deq) { }

    /**
     * Copy constructor.
     */
    RWTValDeque(const RWTValDeque<T, A>& rwdeq) : impl_(rwdeq.impl_) { }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed deque takes ownership of the
     * data owned by \a deq.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValDeque(RWTValDeque<T, A> && deq)
#    if !defined(RW_NO_STDDEQUE_RVALUE_SUPPORT)
        : impl_(rw_move(deq.impl_)) { }
#    else
    {
        deq.std().swap(impl_);
    }
#    endif

    /**
     * @copydoc RWTValDeque(RWTValDeque<T,A>&&)
     */
    RWTValDeque(container_type && deq)
#    if !defined(RW_NO_STDDEQUE_RVALUE_SUPPORT)
        : impl_(rw_move(deq)) { }
#    else
    {
        deq.swap(impl_);
    }
#    endif
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs a double-ended queue with \a n elements, each initialized
     * to \a val.
     */
    RWTValDeque(size_type n, const_reference val = value_type())
        : impl_(n, val) { }

    /**
     * Constructs a double-ended queue by copying elements from the range
     * [\a first, \a last).
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
#if !defined(RW_BROKEN_STD_DEQUE_CTOR_RANGE)
    template <typename InputIterator>
    RWTValDeque(InputIterator first, InputIterator last)
        : impl_(first, last) { }
#else
    template <typename InputIterator>
    RWTValDeque(InputIterator first, InputIterator last,
                typename RWTEnableIf < !RWTIsIntegral<InputIterator>::value >::type** = 0)
        : impl_() {
        assign(first, last);
    }
#endif

    //  ~RWTValDeque(); // handled by compiler-generated destructor

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWTValDeque<T, A>& rhs) {
        std().swap(rhs.impl_);
    }

private:
#if defined(RW_BROKEN_STD_DEQUE_ASSIGN_RANGE)
    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last, const std::input_iterator_tag&) {
        clear();
        insert(end(), first, last);
    }

#  if !defined(RW_BROKEN_STD_DEQUE_ASSIGN_RANGE_RANDOM_ACCESS)
    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last, const std::random_access_iterator_tag&) {
        std().assign(first, last);
    }
#  endif
#endif

#if defined(RW_BROKEN_STD_DEQUE_INSERT_RANGE)
    template <typename InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last, const std::input_iterator_tag&) {
        while (first != last) {
            pos = insert(pos, *(first++));
            ++pos;
        }
    }

#  if !defined(RW_BROKEN_STD_DEQUE_INSERT_RANGE_RANDOM_ACCESS)
    template <typename InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last, const std::random_access_iterator_tag&) {
        std().insert(pos, first, last);
    }
#  endif
#endif

    void boundsCheck(size_type i) const {
        if (i >= entries()) {
            RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEXERR, i, entries())));
        }
    }


    container_type impl_;
};


/**
 * @relates RWTValDeque
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and
 * iterating through both collections produces individual elements that,
 * in turn, compare equal to each other.
 */
template <class T , class A>
bool operator==(const RWTValDeque<T, A>& lhs, const RWTValDeque<T, A>& rhs)
{
    return lhs.std() == rhs.std();
}

/**
 * @relates RWTValDeque
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T , class A>
bool operator!=(const RWTValDeque<T, A>& lhs, const RWTValDeque<T, A>& rhs)
{
    return lhs.std() != rhs.std();
}

/**
 * @relates RWTValDeque
 *
 * Returns \c true if \a lhs is lexicographically less than \a rhs,
 * otherwise \c false. Assumes that type \c T has well-defined less-than
 * semantics.
 */
template <class T , class A>
bool operator<(const RWTValDeque<T, A>& lhs, const RWTValDeque<T, A>& rhs)
{
    return lhs.std() < rhs.std();
}

/**
 * @relates RWTValDeque
 *
 * Equivalent to <tt>(\a rhs < \a lhs)</tt>.
 */
template <class T , class A>
bool operator>(const RWTValDeque<T, A>& lhs, const RWTValDeque<T, A>& rhs)
{
    return lhs.std() > rhs.std();
}

/**
 * @relates RWTValDeque
 *
 * Equivalent to <tt>!(\a rhs < \a lhs)</tt>.
 */
template <class T , class A>
bool operator<=(const RWTValDeque<T, A>& lhs, const RWTValDeque<T, A>& rhs)
{
    return lhs.std() <= rhs.std();
}

/**
 * @relates RWTValDeque
 *
 * Equivalent to <tt>!(\a lhs < \a rhs)</tt>.
 */
template <class T , class A>
bool operator>=(const RWTValDeque<T, A>& lhs, const RWTValDeque<T, A>& rhs)
{
    return lhs.std() >= rhs.std();
}

/**
 * @relates RWTValDeque
 *
 * @copydoc operator==(const RWTValDeque&, const RWTValDeque&);
 */
template <class T , class A>
bool operator==(const RWTValDeque<T, A>& lhs,
                const typename RWTValDeque<T, A>::container_type& rhs)
{
    return lhs.std() == rhs;
}

/**
 * @relates RWTValDeque
 *
 * @copydoc operator!=(const RWTValDeque&, const RWTValDeque&);
 */
template <class T , class A>
bool operator!=(const RWTValDeque<T, A>& lhs,
                const typename RWTValDeque<T, A>::container_type& rhs)
{
    return lhs.std() != rhs;
}

/**
 * @relates RWTValDeque
 *
 * @copydoc operator<(const RWTValDeque&, const RWTValDeque&);
 */
template <class T , class A>
bool operator<(const RWTValDeque<T, A>& lhs,
               const typename RWTValDeque<T, A>::container_type& rhs)
{
    return lhs.std() < rhs;
}

/**
 * @relates RWTValDeque
 *
 * @copydoc operator>(const RWTValDeque&, const RWTValDeque&);
 */
template <class T , class A>
bool operator>(const RWTValDeque<T, A>& lhs,
               const typename RWTValDeque<T, A>::container_type& rhs)
{
    return lhs.std() > rhs;
}

/**
 * @relates RWTValDeque
 *
 * @copydoc operator<=(const RWTValDeque&, const RWTValDeque&);
 */
template <class T , class A>
bool operator<=(const RWTValDeque<T, A>& lhs,
                const typename RWTValDeque<T, A>::container_type& rhs)
{
    return lhs.std() <= rhs;
}

/**
 * @relates RWTValDeque
 *
 * @copydoc operator>=(const RWTValDeque&, const RWTValDeque&);
 */
template <class T , class A>
bool operator>=(const RWTValDeque<T, A>& lhs,
                const typename RWTValDeque<T, A>::container_type& rhs)
{
    return lhs.std() >= rhs;
}


/**
 * @relates RWTValDeque
 *
 * @copydoc operator==(const RWTValDeque&, const RWTValDeque&);
 */
template <class T , class A>
bool operator==(const typename RWTValDeque<T, A>::container_type& lhs,
                const RWTValDeque<T, A>& rhs)
{
    return lhs == rhs.std();
}

/**
 * @relates RWTValDeque
 *
 * @copydoc operator!=(const RWTValDeque&, const RWTValDeque&);
 */
template <class T , class A>
bool operator!=(const typename RWTValDeque<T, A>::container_type& lhs,
                const RWTValDeque<T, A>& rhs)
{
    return lhs != rhs.std();
}

/**
 * @relates RWTValDeque
 *
 * @copydoc operator<(const RWTValDeque&, const RWTValDeque&);
 */
template <class T , class A>
bool operator<(const typename RWTValDeque<T, A>::container_type& lhs,
               const RWTValDeque<T, A>& rhs)
{
    return lhs < rhs.std();
}

/**
 * @relates RWTValDeque
 *
 * @copydoc operator>(const RWTValDeque&, const RWTValDeque&);
 */
template <class T , class A>
bool operator>(const typename RWTValDeque<T, A>::container_type& lhs,
               const RWTValDeque<T, A>& rhs)
{
    return lhs > rhs.std();
}

/**
 * @relates RWTValDeque
 *
 * @copydoc operator<=(const RWTValDeque&, const RWTValDeque&);
 */
template <class T , class A>
bool operator<=(const typename RWTValDeque<T, A>::container_type& lhs,
                const RWTValDeque<T, A>& rhs)
{
    return lhs <= rhs.std();
}

/**
 * @relates RWTValDeque
 *
 * @copydoc operator>=(const RWTValDeque&, const RWTValDeque&);
 */
template <class T , class A>
bool operator>=(const typename RWTValDeque<T, A>::container_type& lhs,
                const RWTValDeque<T, A>& rhs)
{
    return lhs >= rhs.std();
}

/**
 * @relates RWTValDeque
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T , class A>
RWvostream& operator<<(RWvostream& strm, const RWTValDeque<T , A>& coll);

/**
 * @relates RWTValDeque
 *
 * Saves the collection \a coll onto the RWFile \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T , class A>
RWFile& operator<<(RWFile& strm, const RWTValDeque<T , A>& coll);

/**
 * @relates RWTValDeque
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class T , class A>
RWvistream& operator>>(RWvistream& strm, RWTValDeque<T , A>& coll);

/**
 * @relates RWTValDeque
 *
 * Restores the contents of the collection \a coll from the RWFile \a strm.
 */
template <class T , class A>
RWFile& operator>>(RWFile& strm, RWTValDeque<T , A>& coll);

/**
 * @relates RWTValDeque
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T , class A>
RWvistream& operator>>(RWvistream& strm, RWTValDeque<T , A>*& p);

/**
 * @relates RWTValDeque
 *
 * Looks at the next object on the RWFile \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T , class A>
RWFile& operator>>(RWFile& strm, RWTValDeque<T , A>*& p);


#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tvdeque.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tvdeque.cc"
#endif


#endif /* RW_TOOLS_TVDEQUE_H */
