#ifndef RW_TOOLS_TVDLIST_H
#define RW_TOOLS_TVDLIST_H

/**********************************************************************
 *
 *     : value-based doubly-linked list wrapper for STL list
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvdlist.h#2 $
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
#include <rw/epersist.h>

#if defined(RW_CLASSIC_TEMPLATE_CONTAINERS)
#  include "rw/xvdlist.h"
#else
#  include <rw/edefs.h> // for rw_move
#  include <rw/epfunc.h>
#  include <rw/tools/algorithm.h>
#  include <rw/tools/traits/RWTEnableIf.h>
#  include <rw/tools/traits/RWTIsIntegral.h>

#  include <algorithm>
#  include <list>

#if !defined(RW_DISABLE_DEPRECATED)

template <class T, class A>
class RWTValDlistConstIterator;

#endif

/**
 * @ingroup stl_based_collection_classes
 * @brief Maintains a collection of values implemented as a
 * doubly-linked list.
 *
 * This class maintains a collection of values, implemented as a
 * doubly-linked list. Class \c T is the type of items in the collection.
 * Class \c A is an allocator of objects of
 * class \c T. For more information about using custom allocators, please
 * see the <i>Essential Tools Module User's Guide</i>.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvdlist.h>
 * RWTValDlist<T,A> dlist;
 * @endcode
 *
 * @section related Related Classes
 *
 * Classes \link RWTValDeque RWTValDeque<T,A>\endlink,
 * \link RWTValSlist RWTValSlist<T,A>\endlink, and
 * \link RWTValOrderedVector RWTValOrderedVector<T,A>\endlink also provide a
 * Rogue Wave interface to C++ Standard Library sequence collections.
 *
 * Class <b>std::list<T,A<T*> ></b> is the C++ Standard Library collection
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
 * // tvdldog.cpp
 * //
 * #include <iostream>
 * #include <cstring>
 *
 * #include <rw/tvdlist.h>
 *
 * class Dog
 * {
 *     RWCString name_;
 *
 * public:
 *
 *     Dog( const char* s) : name_ (s) { }
 *
 *     // Define a copy constructor:
 *     Dog(const Dog& ref) : name_ (ref.name_) { }
 *
 *     // Define an assignment operator:
 *     Dog& operator= (const Dog& ref) {
 *         if (this == &ref) {
 *             return *this;
 *         }
 *         name_ = ref.name_;
 *         return *this;
 *     }
 *
 *     // Define an equality test operator:
 *     bool operator== (const Dog& ref) const {
 *         return name_ == ref.name_;
 *     }
 *
 *     // Order alphabetically by name:
 *     bool operator< (const Dog& ref) const {
 *         return name_ < ref.name_;
 *     }
 *
 *     friend std::ostream&
 *     operator<< (std::ostream& s, const Dog& ref) {
 *         s << ref.name_;
 *         return s;
 *     }
 * };
 *
 * int main()
 * {
 *     RWTValDlist<Dog> terriers;
 *
 *     terriers.insert("Cairn Terrier"); // NB: type conversion occurs
 *     terriers.insert("Irish Terrier");
 *     terriers.insert("Schnauzer");
 *
 *     std::cout << "The list "
 *               << (terriers.contains("Schnauzer") ? "does " : "does not ")
 *               << "contain a Schnauzer\n";
 *
 *     terriers.insertAt(terriers.index("Irish Terrier"),
 *                       "Fox Terrier");
 *
 *     while (!terriers.isEmpty())
 *         std::cout << terriers.get() << "\n";
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 * The list does contain a Schnauzer
 * Cairn Terrier
 * Fox Terrier
 * Irish Terrier
 * Schnauzer
 * @endcode
 *
 */
template <class T, class A = std::allocator<T> >
class RWTValDlist
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
     * @internal
     * A type representing the same type as self.
     */
    typedef RWTValDlist<T, A> this_type;


    /**
     * Returns a reference to the underlying C++ Standard Library collection
     * that serves as the implementation for self. This reference may
     * be used freely, providing access to the C++ Standard Library interface
     * as well as interoperability with other software components that
     * make use of the C++ Standard Library collections.
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
#if !defined(RW_BROKEN_STD_LIST_ASSIGN_RANGE)
        std().assign(first, last);
#else
        typedef typename rw_iterator_traits<InputIterator>::iterator_category cat;
        assign(first, last, cat());
#endif
    }

    /**
     * Returns a reference to the \a i <sup>th</sup> element of self.
     *
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
     * Returns \c true if there exists an element \c t in self such that
     * the expression <tt>(t == \a a)</tt> is \c true, otherwise returns
     * \c false.
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
     * <tt>(t == \a a)</tt>, or returns #RW_NPOS if no such item exists.
     */
    size_type index(const_reference a) const;

    /**
     * Returns the position of the first item \c t in self such that
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, or returns #RW_NPOS if
     * no such item exists. Client data may be passed through parameter
     * \a d.
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
#if !defined(RW_BROKEN_STD_LIST_INSERT_RANGE)
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
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0 and
     * the number of entries in the collection.
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
     * Adds the item \a a to the beginning of the collection.
     */
    void prepend(const_reference a) {
        std().insert(begin(), a);
    }

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @copydoc prepend()
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    void prepend(value_type && a) {
        std().insert(begin(), rw_move(a));
    }
#  endif // !RW_NO_RVALUE_REFERENCES

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
     * Removes all elements \c t in self such that the expression
     * <tt>(t == \a a)</tt> is \c true. Returns the number of items removed.
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
        const value_type ret = last();
        std().pop_back();
        return ret;
    }

    /**
     * Calls the destructor on all elements of self and replaces them
     * by copying all elements of \a rhs.
     */
    RWTValDlist<T, A>&
    operator=(const RWTValDlist<T, A>& rhs) {
        return operator=(rhs.impl_);
    }

    /**
     * @copydoc operator=()
     */
    RWTValDlist<T, A>&
    operator=(const container_type& rhs) {
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
    RWTValDlist<T, A>&
    operator=(RWTValDlist<T, A> && rhs) {
        return operator=(rw_move(rhs.impl_));
    }

    /**
     * @copydoc operator=(RWTValDlist<T,A>&&)
     */
    RWTValDlist<T, A>&
    operator=(container_type && rhs) {
        if (&impl_ != &rhs) {
#    if !defined(RW_NO_STDLIST_RVALUE_SUPPORT)
            impl_ = rw_move(rhs);
#    else
            rhs.swap(impl_);
#    endif
        }
        return *this;
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs an empty, doubly-linked list.
     */
    RWTValDlist() : impl_() {}

    /**
     * Constructs a doubly-linked list by copying all elements of
     * \a lst.
     */
    RWTValDlist(const container_type& lst) : impl_(lst) {}

    /**
     * Copy constructor.
     */
    RWTValDlist(const RWTValDlist<T, A>& rwlst)
        : impl_(rwlst.impl_) {}

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed list takes ownership of the
     * data owned by \a lst.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValDlist(RWTValDlist<T, A> && lst)
#    if !defined(RW_NO_STDLIST_RVALUE_SUPPORT)
        : impl_(rw_move(lst.impl_)) { }
#    else
    {
        lst.std().swap(impl_);
    }
#    endif

    /**
     * @copydoc RWTValDlist(RWTValDlist<T,A>&&)
     */
    RWTValDlist(container_type && lst)
#    if !defined(RW_NO_STDLIST_RVALUE_SUPPORT)
        : impl_(rw_move(lst)) { }
#    else
    {
        lst.swap(impl_);
    }
#    endif
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs a doubly-linked list with \a n elements, each initialized
     * to \a val.
     */
    RWTValDlist(size_type n, const_reference val = value_type()) : impl_(n, val) {}

    /**
     * Constructs a double-ended queue by copying elements from the range
     * [\a first, \a last).
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
#if !defined(RW_BROKEN_STD_LIST_CTOR_RANGE)
    template <typename InputIterator>
    RWTValDlist(InputIterator first, InputIterator last)
        : impl_(first, last) { }
#else
    template <typename InputIterator>
    RWTValDlist(InputIterator first, InputIterator last,
                typename RWTEnableIf < !RWTIsIntegral<InputIterator>::value >::type** = 0)
        : impl_() {
        assign(first, last);
    }
#endif

    // ~RWTValDlist(); // handled by auto-generated destructor

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWTValDlist<T, A>& rhs) {
        std().swap(rhs.impl_);
    }

private:
#if defined(RW_BROKEN_STD_LIST_ASSIGN_RANGE)
    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last, const std::input_iterator_tag&) {
        clear();
        insert(end(), first, last);
    }

#  if !defined(RW_BROKEN_STD_LIST_ASSIGN_RANGE_RANDOM_ACCESS)
    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last, const std::random_access_iterator_tag&) {
        std().assign(first, last);
    }
#  endif
#endif

#if defined(RW_BROKEN_STD_LIST_INSERT_RANGE)
    template <typename InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last, const std::input_iterator_tag&) {
        while (first != last) {
            pos = insert(pos, *(first++));
            ++pos;
        }
    }

#  if !defined(RW_BROKEN_STD_LIST_INSERT_RANGE_RANDOM_ACCESS)
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
 * @relates RWTValDlist
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and
 * iterating through both collections produces individual elements that,
 * in turn, compare equal to each other.
 */
template <class T , class A>
bool operator==(const RWTValDlist<T, A>& lhs, const RWTValDlist<T, A>& rhs)
{
    return lhs.std() == rhs.std();
}

/**
 * @relates RWTValDlist
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T , class A>
bool operator!=(const RWTValDlist<T, A>& lhs, const RWTValDlist<T, A>& rhs)
{
    return lhs.std() != rhs.std();
}

/**
 * @relates RWTValDlist
 *
 * Returns \c true if \a lhs is lexicographically less than \a rhs,
 * otherwise \c false. Assumes that type \c T has well-defined less-than
 * semantics.
 */
template <class T , class A>
bool operator<(const RWTValDlist<T, A>& lhs, const RWTValDlist<T, A>& rhs)
{
    return lhs.std() < rhs.std();
}

/**
 * @relates RWTValDlist
 *
 * Equivalent to <tt>(\a rhs < \a lhs)</tt>.
 */
template <class T , class A>
bool operator>(const RWTValDlist<T, A>& lhs, const RWTValDlist<T, A>& rhs)
{
    return lhs.std() > rhs.std();
}

/**
 * @relates RWTValDlist
 *
 * Equivalent to <tt>!(\a rhs < \a lhs)</tt>.
 */
template <class T , class A>
bool operator<=(const RWTValDlist<T, A>& lhs, const RWTValDlist<T, A>& rhs)
{
    return lhs.std() <= rhs.std();
}

/**
 * @relates RWTValDlist
 *
 * Equivalent to <tt>!(\a lhs < \a rhs)</tt>.
 */
template <class T , class A>
bool operator>=(const RWTValDlist<T, A>& lhs, const RWTValDlist<T, A>& rhs)
{
    return lhs.std() >= rhs.std();
}

/**
 * @relates RWTValDlist
 *
 * @copydoc operator==(const RWTValDlist&, const RWTValDlist&);
 */
template <class T , class A>
bool operator==(const RWTValDlist<T, A>& lhs,
                const typename RWTValDlist<T, A>::container_type& rhs)
{
    return lhs.std() == rhs;
}

/**
 * @relates RWTValDlist
 *
 * @copydoc operator!=(const RWTValDlist&, const RWTValDlist&);
 */
template <class T , class A>
bool operator!=(const RWTValDlist<T, A>& lhs,
                const typename RWTValDlist<T, A>::container_type& rhs)
{
    return lhs.std() != rhs;
}

/**
 * @relates RWTValDlist
 *
 * @copydoc operator<(const RWTValDlist&, const RWTValDlist&);
 */
template <class T , class A>
bool operator<(const RWTValDlist<T, A>& lhs,
               const typename RWTValDlist<T, A>::container_type& rhs)
{
    return lhs.std() < rhs;
}

/**
 * @relates RWTValDlist
 *
 * @copydoc operator>(const RWTValDlist&, const RWTValDlist&);
 */
template <class T , class A>
bool operator>(const RWTValDlist<T, A>& lhs,
               const typename RWTValDlist<T, A>::container_type& rhs)
{
    return lhs.std() > rhs;
}

/**
 * @relates RWTValDlist
 *
 * @copydoc operator<=(const RWTValDlist&, const RWTValDlist&);
 */
template <class T , class A>
bool operator<=(const RWTValDlist<T, A>& lhs,
                const typename RWTValDlist<T, A>::container_type& rhs)
{
    return lhs.std() <= rhs;
}

/**
 * @relates RWTValDlist
 *
 * @copydoc operator>=(const RWTValDlist&, const RWTValDlist&);
 */
template <class T , class A>
bool operator>=(const RWTValDlist<T, A>& lhs,
                const typename RWTValDlist<T, A>::container_type& rhs)
{
    return lhs.std() >= rhs;
}


/**
 * @relates RWTValDlist
 *
 * @copydoc operator==(const RWTValDlist&, const RWTValDlist&);
 */
template <class T , class A>
bool operator==(const typename RWTValDlist<T, A>::container_type& lhs,
                const RWTValDlist<T, A>& rhs)
{
    return lhs == rhs.std();
}

/**
 * @relates RWTValDlist
 *
 * @copydoc operator!=(const RWTValDlist&, const RWTValDlist&);
 */
template <class T , class A>
bool operator!=(const typename RWTValDlist<T, A>::container_type& lhs,
                const RWTValDlist<T, A>& rhs)
{
    return lhs != rhs.std();
}

/**
 * @relates RWTValDlist
 *
 * @copydoc operator<(const RWTValDlist&, const RWTValDlist&);
 */
template <class T , class A>
bool operator<(const typename RWTValDlist<T, A>::container_type& lhs,
               const RWTValDlist<T, A>& rhs)
{
    return lhs < rhs.std();
}

/**
 * @relates RWTValDlist
 *
 * @copydoc operator>(const RWTValDlist&, const RWTValDlist&);
 */
template <class T , class A>
bool operator>(const typename RWTValDlist<T, A>::container_type& lhs,
               const RWTValDlist<T, A>& rhs)
{
    return lhs > rhs.std();
}

/**
 * @relates RWTValDlist
 *
 * @copydoc operator<=(const RWTValDlist&, const RWTValDlist&);
 */
template <class T , class A>
bool operator<=(const typename RWTValDlist<T, A>::container_type& lhs,
                const RWTValDlist<T, A>& rhs)
{
    return lhs <= rhs.std();
}

/**
 * @relates RWTValDlist
 *
 * @copydoc operator>=(const RWTValDlist&, const RWTValDlist&);
 */
template <class T , class A>
bool operator>=(const typename RWTValDlist<T, A>::container_type& lhs,
                const RWTValDlist<T, A>& rhs)
{
    return lhs >= rhs.std();
}

/**
 * @relates RWTValDlist
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T , class A>
RWvostream& operator<<(RWvostream& strm, const RWTValDlist<T , A>& coll);

/**
 * @relates RWTValDlist
 *
 * Saves the collection \a coll onto the RWFile \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T , class A>
RWFile& operator<<(RWFile& strm, const RWTValDlist<T , A>& coll);

/**
 * @relates RWTValDlist
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class T , class A>
RWvistream& operator>>(RWvistream& strm, RWTValDlist<T , A>& coll);

/**
 * @relates RWTValDlist
 *
 * Restores the contents of the collection \a coll from the RWFile \a strm.
 */
template <class T , class A>
RWFile& operator>>(RWFile& strm, RWTValDlist<T , A>& coll);

/**
 * @relates RWTValDlist
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T , class A>
RWvistream& operator>>(RWvistream& strm, RWTValDlist<T , A>*& p);

/**
 * @relates RWTValDlist
 *
 * Looks at the next object on the RWFile \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T , class A>
RWFile& operator>>(RWFile& strm, RWTValDlist<T , A>*& p);

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @ingroup stl_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTValDlist::iterator to iterate over
 * the elements in this collection.
 *
 * @brief Deprecated. Provides an iterator for RWTValDlist.
 *
 * RWTValDlistIterator provides an iterator interface to
 * the C++ Standard Library-based collection represented by
 * RWTValDlist. (An STL iterator interface is provided
 * through RWTValDlist::iterator.)
 *
 * The order of iteration over an RWTValDlist is dependent on the
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
 * @section synopsis Synopsis
 *
 * @code
 * #include<rw/tvdlist.h>
 * RWTValDlist<T,A> dl;
 * RWTValDlistIterator<T,A> itr(dl);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include<rw/tvdlist.h>
 * #include<iostream>
 * #include<rw/cstring.h>
 * int main() {
 *    RWTValDlist<RWCString> a;
 *    RWTValDlistIterator<RWCString> itr(a);
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
 * John
 * Steve
 * Mark
 * Steve
 * @endcode
 */
template <class T, class A = std::allocator<T> >
class RW_DEPRECATE_TYPE("Use RWTValDlist::iterator instead") RWTValDlistIterator
{
private:
    RWTValDlist<T, A>* list_;
    typename RWTValDlist<T, A>::iterator iter_;
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
     * Creates an iterator for the dlist \a s. The iterator begins
     * in an undefined state and must be advanced before the first element
     * is accessible.
     */
    RWTValDlistIterator(RWTValDlist<T, A>& s)
        : list_(&s), head_(true), tail_(false) { }

    /**
     * Advances self to the next element. If the iterator has been reset
     * or just created, self will reference the first element. If, before
     * iteration, self referenced the last value in the list, self will
     * now reference an undefined value distinct from the reset value and
     * \c false is returned. Otherwise, returns \c true.
     */
    bool operator++();

    /**
     * Moves self back to the immediately previous element. If the
     * iterator has been reset or just created, this operator returns
     * \c false, otherwise it returns \c true. If self references
     * the first element, it will now be in the reset state. If self
     * has been iterated past the last value in the list, it will now
     * reference the last item in the list.
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
     * Advances self to the next element and returns its value. If
     * the iterator has advanced past the last item in the container,
     * returns #rwnil.
     */
    bool operator()() {
        return ++(*this);
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    RWTValDlist<T, A>* container() const {
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
     * \c true. Returns \c true if an element was found, otherwise returns
     * \c false. Client data may be passed through parameter \a d.
     */
    bool findNext(bool (*fn)(const T&, void*), void* d);

    void insertAfterPoint(const T& a);

    /**
     * Returns a reference to the stored value.
     */
    T& key() const {
        return *(RW_CONST_CAST(RWTValDlistIterator*, this)->iter_);
    }

    /**
     * Removes the value referenced by self from the collection.
     * Returns \c true if the removal is successful, otherwise \c false.
     */
    bool remove();  // Remove item at cursor

    /**
     * Removes the first element \c t, encountered by iterating self forward,
     * such that the expression <tt>(t == \a a)</tt> is \c true. Returns
     * \c true if an element was found and removed, otherwise returns \c false.
     */
    bool removeNext(const T& a);

    /**
     * Removes the first element \c t, encountered by iterating self
     * forward, such that the expression <tt>((*\a fn)(t,\a d))</tt>
     * is \c true. Returns \c true if an element was found and removed,
     * otherwise returns \c false. Client data may be passed through
     * parameter \a d.
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
    void reset(RWTValDlist<T, A>& l) {
        head_ = true;
        tail_ = false;
        list_ = &l;
    }

private:

    // Disallow postfix increment & decrement. Unless we hide it, some
    // compilers will substitute the prefix operator in its place.
    bool operator++(int) {
        return false;
    }
    bool operator--(int) {
        return false;
    }

    friend class RWTValDlistConstIterator<T, A>;
};

/**
 * @ingroup stl_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTValDlist::const_iterator to iterate
 * over the elements in this collection.
 *
 * @brief Deprecated. Provides a \c const iterator for RWTValDlist.
 *
 * RWTValDlistConstIterator provides a \c const iterator for Standard
 * Library-based RWTValDlist collections. The \c const iterator
 * prohibits changes to the underlying collection.
 *
 * The order of iteration over an RWTValDlist is dependent on the
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
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvdlist.h>
 * template <class T, class A=std::allocator<T> > class
 * RWTValDlistConstIterator
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class T, class A = std::allocator<T> >
class RW_DEPRECATE_TYPE("Use RWTValDlist::const_iterator instead") RWTValDlistConstIterator
{
private:
    const RWTValDlist<T, A>* list_;
    typename RWTValDlist<T, A>::const_iterator iter_;
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
     * Creates an iterator for the dlist \a s. The iterator begins
     * in an undefined state and must be advanced before the first element
     * is accessible.
     */
    RWTValDlistConstIterator(const RWTValDlist<T, A>& s)
        : list_(&s), head_(true), tail_(false) { }

    /**
     * Creates an iterator from the iterator \a itr. The \c const iterator
     * begins in an undefined state and must be advanced before the first
     * element is accessible.
     */
    RWTValDlistConstIterator(const RWTValDlistIterator<T, A>& itr)
        : list_(itr.list_), iter_(itr.iter_),
          head_(itr.head_), tail_(itr.tail_) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self now references the first element.
     * If, before iteration, self referenced the last value in the
     * list, self now references an undefined value distinct from the
     * reset value, and this operator returns \c false. Otherwise,
     * returns \c true.
     */
    bool operator++();

    /**
     * Moves self back to the immediately previous element. If the
     * iterator has been reset or just created, returns
     * \c false, otherwise it returns \c true. If self now references
     * the first element, it is now in the reset state. If self has
     * been iterated past the last value in the list, it now references
     * the last item in the list.
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
     * Advances self to the next element and returns its value. If
     * the iterator has advanced past the last item in the container,
     * returns #rwnil.
     */
    bool operator()() {
        return ++(*this);
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    const RWTValDlist<T, A>* container() const {
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
     * \c true. Returns \c true if an element was found, otherwise \c false.
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
    void reset(const RWTValDlist<T, A>& l) {
        head_ = true;
        tail_ = false;
        list_ = &l;
    }

private:

    // Disallow postfix increment & decrement. Unless we hide it, some
    // compilers will substitute the prefix operator in its place.
    bool operator++(int);
    bool operator--(int);

};
#endif

#  if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#    pragma implementation ("tvdlist.cc")
#  endif

#  ifdef RW_NO_IMPLICIT_INCLUSION
#    include "rw/tvdlist.cc"
#  endif

#endif /* RW_CLASSIC_TEMPLATE_CONTAINERS */

#endif /* RW_TOOLS_TVDLIST_H */
