#ifndef RW_TOOLS_TPDLIST_H
#define RW_TOOLS_TPDLIST_H

/**********************************************************************
 *
 * tpdlist.h - RWTPtrDlist<T>
 *     : pointer-based doubly-linked list wrapper for STL list
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tpdlist.h#2 $
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
#  include "rw/xpdlist.h"
#else
#  include <rw/epaux.h>
#  include <rw/epersist.h>
#  include <rw/epfunc.h>
#  include <rw/tools/algorithm.h>
#  include <rw/tools/traits/RWTEnableIf.h>
#  include <rw/tools/traits/RWTIsIntegral.h>

#  include <algorithm>
#  include <list>

#if !defined(RW_DISABLE_DEPRECATED)

template <class T, class A>
class RWTPtrDlistIterator;
template <class T, class A>
class RWTPtrDlistConstIterator;

#endif

/**
 * @ingroup stl_based_collection_classes
 * @brief A pointer-based collection of values, implemented
 * as a doubly-linked list.
 *
 * This class maintains a pointer-based collection of values, implemented
 * as a doubly-linked list. Class \c T is the type pointed to by the
 * items in the collection. Class \c A is an allocator of objects of
 * class \c T. For more information about using custom allocators, please
 * see the <i>Essential Tools Module User's Guide</i>.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tpdlist.h>
 * RWTPtrDlist<T,A> dlist;
 * @endcode
 *
 * @section related Related Classes
 *
 * Classes \link RWTPtrDeque RWTPtrDeque<T,A>\endlink,
 * \link RWTPtrSlist RWTPtrSlist<T,A>\endlink, and
 * \link RWTPtrOrderedVector RWTPtrOrderedVector<T,A>\endlink
 * also provide a Rogue Wave pointer-based interface to C++ Standard Library
 * sequence collections.
 *
 * Class list<T*,A> is the C++ Standard Library collection that serves as
 * the underlying implementation for this class.
 *
 * @section persistence Persistence
 *
 * Isomorphic
 *
 * @section example Example
 *
 *
 * In this example, a pointer-based doubly-linked list of user type
 * \c Dog is exercised.
 *
 * @code
 *
 * //
 * // tpdlist.cpp
 * //
 * #include <rw/tpdlist.h>
 * #include <iostream>
 * #include <string.h>
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
 *         if (this == &ref)
 *             return *this;
 *
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
 * int main ()
 * {
 *     RWTPtrDlist<Dog> terriers;
 *
 *     terriers.insert(new Dog("Cairn Terrier"));
 *     terriers.insert(new Dog("Irish Terrier"));
 *     terriers.insert(new Dog("Schnauzer"));
 *
 *     Dog key1("Schnauzer");
 *     std::cout << "The list "
 *               << (terriers.contains (&key1) ? "does " : "does not ")
 *               << "contain a Schnauzer\n";
 *
 *     Dog key2("Irish Terrier");
 *     terriers.insertAt(terriers.index(&key2), new Dog("Fox Terrier"));
 *
 *     Dog* d;
 *     while (!terriers.isEmpty()) {
 *         d = terriers.get();
 *         std::cout << *d << std::endl;
 *         delete d;
 *     }
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 *
 * The list does contain a Schnauzer
 * Cairn Terrier
 * Fox Terrier
 * Irish Terrier
 * Schnauzer
 * @endcode
 *
 */
template <class T, class A = std::allocator<T*> >
class RWTPtrDlist
{
public:

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


#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @deprecated As of SourcePro 12.5, use <tt>const T*</tt> instead.
     * A type representing the container's \c const data type.
     */
    typedef RW_DEPRECATE_TYPE("Use const T* instead") const T* const_value;
#endif


    /**
     * @internal
     * A type representing the same type as self.
     */
    typedef RWTPtrDlist<T, A> this_type;

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


    // STL-level access
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
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0 and
     * one less than the number of entries in self.
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
     * Returns \c true if there are no items in the collection, \c false
     * otherwise.
     */
    bool isEmpty() const {
        return std().empty();
    }

    /**
     * Returns a reference to the \a i <sup>th</sup> element of self.
     * Index \a i must be between \c 0 and one less than the number
     * of entries, otherwise the results are undefined. Note that
     * <i>no bounds checking is performed</i>.
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
     * of entries, otherwise the results are undefined. Note that
     * <i>no bounds checking is performed</i>.
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
     * Returns a reference to the \a i <sup>th</sup> element of self.
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0
     * and one less than the number of entries in self.
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
     * Invokes the function pointer \a fn on each item in the collection.
     * Client data may be passed through parameter \a d.
     */
    void apply(void (*fn)(const T*, void*), void* d) const {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * @copydoc apply()
     */
    void apply(void (*fn)(value_type, void*), void* d) {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * @copydoc apply()
     */
    void apply(void (*fn)(reference, void*), void* d) {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * Returns \c true if there exists an element \c t in self such
     * that the expression <tt>((*\a fn)(t,\a d))</tt> is \c true,
     * otherwise \c false. Client data may be passed through
     * parameter \a d.
     */
    bool contains(bool(*fn)(value_type, void*), void* d) const {
        return std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d)) != end();
    }

    // see doxygen bug #627884
    /**
     * Returns \c true if there exists an element \c t in self such
     * that the expression <tt>((*\a fn)(t,\a d))</tt> is \c true,
     * otherwise \c false. Client data may be passed through
     * parameter \a d.
     */
    bool contains(bool(*fn)(const T*, void*), void* d) const {
        return std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d)) != end();
    }

    /**
     * Returns \c true if there exists an element \c t in self such
     * that the expression <tt>(*t == *\a a)</tt> is \c true, otherwise
     * \c false.
     */
    bool contains(const T* a) const {
        return std::find_if(begin(), end(), rw_deref_match<const T*>(a)) != end();
    }

    /**
     * If there exists an element \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, returns \c t, otherwise
     * returns #rwnil. Client data may be passed through parameter \a d.
     */
    value_type find(bool(*fn)(value_type, void*), void* d) const {
        const_iterator ret = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        return (ret != end()) ? *ret : rwnil;
    }

    // see doxygen bug #627884
    /**
     * If there exists an element \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, returns \c t, otherwise
     * returns #rwnil. Client data may be passed through parameter \a d.
     */
    value_type find(bool(*fn)(const T*, void*), void* d) const {
        const_iterator ret = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        return (ret != end()) ? *ret : rwnil;
    }

    /**
     * If there exists an element \c t in self such that the expression
     * <tt>(*t == *\a a)</tt> is \c true, returns \c t, otherwise
     * returns #rwnil.
     */
    value_type find(const T* a) const {
        const_iterator ret = std::find_if(begin(), end(), rw_deref_match<const T*>(a));
        return (ret != end()) ? *ret : rwnil;
    }

    /**
     * Returns the position of the first item \c t in self such that
     * <tt>(*t == *\a a)</tt>, or returns #RW_NPOS if no such item
     * exists.
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
     * Adds the item \a a to the end of the collection.
     */
    void append(value_type a) {
        std().insert(std().end(), a);
    }

    /**
     * Clears the collection by removing all items from self.
     */
    void clear() {
        std().clear();
    }

    /**
     * @copydoc removeFirst()
     */
    value_type get() {
        RW_PRECONDITION(!isEmpty());
        return removeFirst();
    }

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
     * Adds the item \a a to the beginning of the collection.
     */
    void prepend(value_type a) {
        std().insert(std().begin(), a);
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
     * Replaces with \a newVal all elements \c t in self such that the
     * expression <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns the
     * number of items replaced. Client data may be passed through
     * parameter \a d.
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
     * parameter \a d.
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
     * Removes all elements \c t in self such that the expression
     * <tt>(*t == *\a a)</tt> is \c true. Returns the number of
     * items removed.
     */
    size_type removeAll(const T* a) {
        size_type count = std().size();

        for (iterator it = std().begin(); it != std().end();) {
            if (**it == *a) {
                it = std().erase(it);
            }
            else {
                ++it;
            }
        }

        return (count - std().size());
    }

    /**
     * Removes all elements \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns the number
     * of items removed. Client data may be passed through parameter
     * \a d.
     */
    size_type removeAll(bool (*fn)(const T*, void*), void* d) {
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
    size_type removeAll(bool (*fn)(value_type, void*), void* d) {
        size_type size = entries();
        iterator iter = std::remove_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        std().erase(iter, end());
        return size - entries();
    }

    /**
     * Removes all items from the collection and uses <tt>operator
     * delete</tt> to destroy the objects pointed to by those items. Do
     * not use this method if multiple pointers to the same object are
     * stored.
     */
    void clearAndDestroy() {
        rw_destroy(begin(), end());
        clear();
    }

    // additional interface components
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
     * Sorts the collection using the less-than operator to compare
     * elements. Elements are dereferenced before being compared.
     */
    void sort() {
        rw_list_sort(impl_, deref_less());
    }

    // assignment operators
    /**
     * Clears all elements of self and replaces them by copying all
     * elements of \a rhs.
     */
    RWTPtrDlist<T, A>&
    operator=(const RWTPtrDlist<T, A>& rhs) {
        return operator=(rhs.impl_);
    }

    /**
     * @copydoc operator=()
     */
    RWTPtrDlist<T, A>&
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
    RWTPtrDlist<T, A>&
    operator=(RWTPtrDlist<T, A> && rhs) {
        return operator=(rw_move(rhs.impl_));
    }

    /**
     * @copydoc operator=(RWTPtrDlist<T,A>&&)
     */
    RWTPtrDlist<T, A>&
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

    // constructors
    /**
     * Constructs an empty, doubly-linked list.
     */
    RWTPtrDlist() : impl_() {}

    /**
     * Constructs a pointer-based doubly linked list by copying all
     * elements of \a lst.
     */
    RWTPtrDlist(const container_type& lst) : impl_(lst) {}

    /**
     * Copy constructor.
     */
    RWTPtrDlist(const RWTPtrDlist<T, A>& lst)
        : impl_(lst.impl_) {}

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed list takes ownership of the
     * data owned by \a lst.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrDlist(RWTPtrDlist<T, A> && lst)
#    if !defined(RW_NO_STDLIST_RVALUE_SUPPORT)
        : impl_(rw_move(lst.impl_)) { }
#    else
    {
        lst.std().swap(impl_);
    }
#    endif

    /**
     * @copydoc RWTPtrDlist(RWTPtrDlist<T,A>&&)
     */
    RWTPtrDlist(container_type && lst)
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
     * to \a a.
     */
    RWTPtrDlist(size_type n, value_type a = 0) : impl_(n, a) {}

    /**
     * Constructs a double-ended queue by copying elements from the range
     * [\a first, \a last).
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
#if !defined(RW_BROKEN_STD_LIST_CTOR_RANGE)
    template <typename InputIterator>
    RWTPtrDlist(InputIterator first, InputIterator last)
        : impl_(first, last) { }
#else
    template <typename InputIterator>
    RWTPtrDlist(InputIterator first, InputIterator last,
                typename RWTEnableIf < !RWTIsIntegral<InputIterator>::value >::type** = 0)
        : impl_() {
        assign(first, last);
    }
#endif

    //  ~RWTPtrDlist(); // handled by compiler-generated destructor

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWTPtrDlist<T, A>& rhs) {
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
        if (!(i < std().size())) {
            RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEXERR, i, entries())));
        }
    }

    container_type impl_;

#if !defined(RW_DISABLE_DEPRECATED)

    friend class RWTPtrDlistIterator<T, A>;
    friend class RWTPtrDlistConstIterator<T, A>;

#endif

};


/**
 * @relates RWTPtrDlist
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and
 * iterating through both collections produces individual elements that,
 * in turn, compare equal to each other. Elements are dereferenced before
 * being compared.
 */
template <class T , class A>
bool operator==(const RWTPtrDlist<T, A>& lhs, const RWTPtrDlist<T, A>& rhs)
{
    return lhs.entries() == rhs.entries() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrDlist<T, A>::deref_equal());
}

/**
 * @relates RWTPtrDlist
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T , class A>
bool operator!=(const RWTPtrDlist<T, A>& lhs, const RWTPtrDlist<T, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrDlist
 *
 * Returns \c true if \a lhs is lexicographically less than \a rhs,
 * otherwise \c false. Items in each collection are dereferenced before
 * being compared. Assumes that type \c T has well-defined less-than
 * semantics.
 */
template <class T , class A>
bool operator<(const RWTPtrDlist<T, A>& lhs, const RWTPtrDlist<T, A>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrDlist<T, A>::deref_less());
}

/**
 * @relates RWTPtrDlist
 *
 * Equivalent to <tt>(\a rhs < \a lhs)</tt>.
 */
template <class T , class A>
bool operator>(const RWTPtrDlist<T, A>& lhs, const RWTPtrDlist<T, A>& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrDlist
 *
 * Equivalent to <tt>!(\a rhs < \a lhs)</tt>.
 */
template <class T , class A>
bool operator<=(const RWTPtrDlist<T, A>& lhs, const RWTPtrDlist<T, A>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrDlist
 *
 * Equivalent to <tt>!(\a lhs < \a rhs)</tt>.
 */
template <class T , class A>
bool operator>=(const RWTPtrDlist<T, A>& lhs, const RWTPtrDlist<T, A>& rhs)
{
    return !(lhs < rhs);
}

/**
 * @relates RWTPtrDlist
 *
 * @copydoc operator==(const RWTPtrDlist&, const RWTPtrDlist&);
 */
template <class T , class A>
bool operator==(const RWTPtrDlist<T, A>& lhs,
                const typename RWTPtrDlist<T, A>::container_type& rhs)
{
    return lhs.entries() == rhs.size() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrDlist<T, A>::deref_equal());
}

/**
 * @relates RWTPtrDlist
 *
 * @copydoc operator!=(const RWTPtrDlist&, const RWTPtrDlist&);
 */
template <class T , class A>
bool operator!=(const RWTPtrDlist<T, A>& lhs,
                const typename RWTPtrDlist<T, A>::container_type& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrDlist
 *
 * @copydoc operator<(const RWTPtrDlist&, const RWTPtrDlist&);
 */
template <class T , class A>
bool operator<(const RWTPtrDlist<T, A>& lhs,
               const typename RWTPtrDlist<T, A>::container_type& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrDlist<T, A>::deref_less());
}

/**
 * @relates RWTPtrDlist
 *
 * @copydoc operator>(const RWTPtrDlist&, const RWTPtrDlist&);
 */
template <class T , class A>
bool operator>(const RWTPtrDlist<T, A>& lhs,
               const typename RWTPtrDlist<T, A>::container_type& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrDlist
 *
 * @copydoc operator<=(const RWTPtrDlist&, const RWTPtrDlist&);
 */
template <class T , class A>
bool operator<=(const RWTPtrDlist<T, A>& lhs,
                const typename RWTPtrDlist<T, A>::container_type& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrDlist
 *
 * @copydoc operator>=(const RWTPtrDlist&, const RWTPtrDlist&);
 */
template <class T , class A>
bool operator>=(const RWTPtrDlist<T, A>& lhs,
                const typename RWTPtrDlist<T, A>::container_type& rhs)
{
    return !(lhs < rhs);
}


/**
 * @relates RWTPtrDlist
 *
 * @copydoc operator==(const RWTPtrDlist&, const RWTPtrDlist&);
 */
template <class T , class A>
bool operator==(const typename RWTPtrDlist<T, A>::container_type& lhs,
                const RWTPtrDlist<T, A>& rhs)
{
    return lhs.size() == rhs.entries() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrDlist<T, A>::deref_equal());
}

/**
 * @relates RWTPtrDlist
 *
 * @copydoc operator!=(const RWTPtrDlist&, const RWTPtrDlist&);
 */
template <class T , class A>
bool operator!=(const typename RWTPtrDlist<T, A>::container_type& lhs,
                const RWTPtrDlist<T, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrDlist
 *
 * @copydoc operator<(const RWTPtrDlist&, const RWTPtrDlist&);
 */
template <class T , class A>
bool operator<(const typename RWTPtrDlist<T, A>::container_type& lhs,
               const RWTPtrDlist<T, A>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrDlist<T, A>::deref_less());
}

/**
 * @relates RWTPtrDlist
 *
 * @copydoc operator>(const RWTPtrDlist&, const RWTPtrDlist&);
 */
template <class T , class A>
bool operator>(const typename RWTPtrDlist<T, A>::container_type& lhs,
               const RWTPtrDlist<T, A>& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrDlist
 *
 * @copydoc operator<=(const RWTPtrDlist&, const RWTPtrDlist&);
 */
template <class T , class A>
bool operator<=(const typename RWTPtrDlist<T, A>::container_type& lhs,
                const RWTPtrDlist<T, A>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrDlist
 *
 * @copydoc operator>=(const RWTPtrDlist&, const RWTPtrDlist&);
 */
template <class T , class A>
bool operator>=(const typename RWTPtrDlist<T, A>::container_type& lhs,
                const RWTPtrDlist<T, A>& rhs)
{
    return !(lhs < rhs);
}

RW_DECLARE_TLS_PERSISTABLE_TEMPLATE(RWTPtrDlist)

/**
 * @relates RWTPtrDlist
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T , class A>
RWvostream& operator<<(RWvostream& strm, const RWTPtrDlist<T , A>& coll);

/**
 * @relates RWTPtrDlist
 *
 * Saves the collection \a coll onto the RWFile \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T , class A>
RWFile& operator<<(RWFile& strm, const RWTPtrDlist<T , A>& coll);

/**
 * @relates RWTPtrDlist
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class T , class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrDlist<T , A>& coll);

/**
 * @relates RWTPtrDlist
 *
 * Restores the contents of the collection \a coll from the RWFile
 * \a strm.
 */
template <class T , class A>
RWFile& operator>>(RWFile& strm, RWTPtrDlist<T , A>& coll);


/**
 * @relates RWTPtrDlist
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T , class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrDlist<T , A>*& p);

/**
 * @relates RWTPtrDlist
 *
 * Looks at the next object on the RWFile \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T , class A>
RWFile& operator>>(RWFile& strm, RWTPtrDlist<T , A>*& p);



/****************************************************************
 *                                                              *
 *      Declarations for RWTPtrDlistIterator<T>                 *
 *                                                              *
 ****************************************************************/

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @ingroup stl_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTPtrDlist::iterator to iterate over
 * the elements in this collection.
 *
 * @brief Deprecated. Provides an iterator for RWTPtrDlist.
 *
 * RWTPtrDlistIterator provides an iterator interface to the C++
 * Standard Library-based collection represented by RWTPtrDlist.
 * (An STL iterator interface is provided through \c RWTPtrDlist::iterator.)
 *
 * The order of iteration over an RWTPtrDlist is dependent on the
 * order of the values in the container.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset(). The iterator becomes
 * valid after being advanced with either pre-increment or \c operator().
 *
 * For both \c operator++ and \c operator(), iterating past the last
 * element returns a value equivalent to boolean \c false. Continued
 * increments return a value equivalent to \c false until reset()
 * is called. For \c operator--, decrementing past the first element
 * returns a value equivalent to \c false.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include<rw/tpdlist.h>
 * RWTPtrDlist<T,A> dl;
 * RWTPtrDlistIterator<T,A> itr(dl);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/tpdlist.h>
 * #include <iostream>
 * #include <rw/cstring.h>
 *
 * int main() {
 *    RWTPtrDlist<RWCString> a;
 *    RWTPtrDlistIterator<RWCString> itr(a);
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
 *
 * John
 * Steve
 * Mark
 * Steve
 * @endcode
 *
 */
template <class T, class A = std::allocator<T*> >
class RW_DEPRECATE_TYPE("Use RWTPtrDlist::iterator instead") RWTPtrDlistIterator
{
private:
    RWTPtrDlist<T, A>* list_;
    typename RWTPtrDlist<T, A>::iterator iter_;
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
     * Creates an iterator for the list \a l. The iterator begins in
     * an undefined state and must be advanced before the first element
     * is accessible.
     */
    RWTPtrDlistIterator(RWTPtrDlist<T, A>& l)
        : list_(&l), head_(true), tail_(false) { }

    // Operators:
    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self will reference the first element.
     * If, before iteration, self referenced the last value in the
     * list, self will now reference an undefined value distinct from
     * the reset value, and a value equivalent to \c false will be returned.
     * Otherwise, a value equivalent to \c true is returned.
     */
    bool operator++();

    /**
     * Moves self back to the immediately previous element. If the
     * iterator has been reset or just created, this operator returns
     * a value equivalent to \c false, otherwise it returns a value
     * equivalent to \c true. If self references the first element,
     * it will now be in the reset state. If self has been iterated
     * past the last value in the list, it will now reference the last
     * item in the list.
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
     * past the last item in the container, #rwnil is returned.
     */
    T* operator()() {
        return ++(*this) ? key() : rwnil;
    }

    // Methods:
    /**
     * Returns a pointer to the collection being iterated over.
     */
    RWTPtrDlist<T, A>* container() const {
        return list_;
    }

    /**
     * Returns the first element \c t encountered while iterating self
     * forward, such that the expression <tt>(*t == *\a a)</tt> is
     * \c true. If no such element exists, returns #rwnil equivalent to
     * \c false. Leaves self referencing the found item, or "past the
     * end."
     */
    T* findNext(const T* a);

    /**
     * Returns the first element \c t encountered by iterating self
     * forward such that the expression <tt>((*\a fn)(t,\a d))</tt> is
     * \c true. Client data may be passed through parameter \a d. If no
     * such element exists, returns #rwnil. Leaves self referencing the
     * found item, or "past the end."
     */
    T* findNext(bool (*fn)(T*, void*), void* d);

    /**
     * Inserts the pointer \a p into the container directly after the
     * element referenced by self.
     */
    void insertAfterPoint(T* p);

    /**
     * Returns the stored value referenced by self. Undefined if self
     * is not referencing a value within the list.
     */
    T* key() const {
        return *((RWTPtrDlistIterator<T, A>*)this)->iter_;
    }

    /**
     * Returns the stored value referenced by self and removes it from
     * the collection. Undefined if self is not referencing a value
     * within the list.
     */
    T* remove();

    /**
     * Returns and removes the first element \c t, encountered by iterating
     * self forward, such that the expression <tt>(*t == *\a a)</tt> is \c true.
     * If no such element exists, returns #rwnil.
     */
    T* removeNext(const T* a);

    /**
     * Returns and removes the first element \c t, encountered by iterating
     * self forward, such that the expression <tt>((*\a fn)(t,\a d))</tt>
     * is \c true. Client data may be passed through parameter \a d. If no
     * such element exists, returns #rwnil.
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
     * the first element of the collection specified by \a s.
     */
    void reset(RWTPtrDlist<T, A>& s) {
        head_ = true;
        tail_ = false;
        list_ = &s;
    }

private:

    // Disallow postfix operators. Unless we hide it, some compilers will
    // substitute the prefix operator in its place.
    bool operator++(int);
    bool operator--(int);

    friend class RWTPtrDlistConstIterator<T, A>;
};


/****************************************************************
 *                                                              *
 *      Declarations for RWTPtrDlistConstIterator<T,A>          *
 *                                                              *
 ****************************************************************/

/**
 * @ingroup stl-based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTPtrDlist::const_iterator to iterate
 * over the elements in this collection.
 *
 * @brief Deprecated. Provides a \c const iterator for RWTPtrDlist.
 *
 * RWTPtrDlistConstIterator provides a \c const iterator interface
 * to the C++ Standard Library-based collection represented by RWTPtrDlist.
 * This iterator prohibits any changes to the underlying collection.
 *
 * The order of iteration over an RWTPtrDlist is dependent on the
 * order of the values in the container.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset(). The iterator becomes
 * valid after being advanced with either pre-increment or \c operator().
 *
 * For both \c operator++ and \c operator(), iterating past the last
 * element returns a value equivalent to boolean \c false. Continued
 * increments return a value equivalent to \c false until reset()
 * is called. For \c operator--, decrementing past the first element
 * returns a value equivalent to \c false.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tpdlist.h>
 * template <class T, class A=std::allocator<T*> >
 * class RWTPtrDlistConstIterator
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class T, class A = std::allocator<T*> >
class RW_DEPRECATE_TYPE("Use RWTPtrDlist::const_iterator instead") RWTPtrDlistConstIterator
{
private:
    const RWTPtrDlist<T, A>* list_;
    typename RWTPtrDlist<T, A>::const_iterator iter_;
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
     * Constructs a \c const iterator for the list \a l. The iterator
     * begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTPtrDlistConstIterator(const RWTPtrDlist<T, A>& l)
        : list_(&l), head_(true), tail_(false) { }

    /**
     * Constructs a \c const iterator from the iterator \a itr. The
     * iterator begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTPtrDlistConstIterator(const RWTPtrDlistIterator<T, A>& itr)
        : list_(itr.list_), iter_(itr.iter_),
          head_(itr.head_), tail_(itr.tail_) { }

    // Operators:
    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self references the first element. If,
     * before iteration, self referenced the last value in the list,
     * self now references an undefined value distinct from the reset
     * value, and a value equivalent to \c false is returned. Otherwise,
     * a value equivalent to \c true is returned.
     */
    bool operator++();

    /**
     * Moves self back to the immediately previous element. If the
     * iterator has been reset or just created, this operator returns
     * a value equivalent to \c false, otherwise it returns a value
     * equivalent to \c true. If self references the first element,
     * it is now in the reset state. If self has been iterated past
     * the last value in the list, it now references the last item in
     * the list.
     */
    bool operator--();

    /**
     * Behaves as if operator++() had been applied \a n times.
     */
    bool operator+=(size_t n);

    /**
     * Behaves as if operator--() had been applied \a n
     * times.
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

    // Methods:
    /**
     * Returns a pointer to the collection being iterated over.
     */
    const RWTPtrDlist<T, A>* container() const {
        return list_;
    }

    /**
     * Returns the first element \c t encountered while iterating self
     * forward, such that the expression <tt>(*t == *\a a)</tt> is
     * \c true. If no such element exists, returns #rwnil. Leaves self
     * referencing the found item, or "past the end."
     */
    const T* findNext(const T* a);

    /**
     * Returns the first element \c t encountered by iterating self
     * forward such that the expression <tt>((*\a fn)(t,\a d))</tt>
     * is \c true. Client data may be passed through parameter \a d.
     * If no such element exists, returns #rwnil. Leaves self
     * referencing the found item, or "past the end."
     */
    const T* findNext(bool (*fn)(T*, void*), void* d);

    /**
     * Returns the stored value referenced by self. Undefined if
     * self is not referencing a value within the list.
     */
    const T* key() const {
        return *(const_cast<RWTPtrDlistConstIterator<T, A>*>(this))->iter_;
    }

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
     * the first element of the collection specified by \a s.
     */
    void reset(const RWTPtrDlist<T, A>& s) {
        head_ = true;
        tail_ = false;
        list_ = &s;
    }

private:

    // Disallow postfix operators. Unless we hide it, some compilers will
    // substitute the prefix operator in its place.
    bool operator++(int);
    bool operator--(int);

};
#endif

#  if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#    pragma implementation ("tpdlist.cc")
#  endif

#  ifdef RW_NO_IMPLICIT_INCLUSION
#    include <rw/tpdlist.cc>
#  endif

#endif /* RW_CLASSIC_TEMPLATE_CONTAINERS */

#endif /* RW_TOOLS_TPDLIST_H */
