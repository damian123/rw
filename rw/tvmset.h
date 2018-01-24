#ifndef RW_TOOLS_TVMSET_H
#define RW_TOOLS_TVMSET_H

/**********************************************************************
 *
 *     : value-based set wrapper for STL multiset
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvmset.h#3 $
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
#include <rw/epfunc.h>
#include <rw/stdex/algo.h>
#include <rw/tools/algorithm.h>

#include <set>

#if !defined(RW_DISABLE_DEPRECATED)

template <class T, class C, class A>
class RWTValMultiSetConstIterator;

#endif

/**
 * @ingroup stl_based_collection_classes
 * @brief Maintains a collection of values ordered
 * according to a comparison object.
 *
 * This class maintains a collection of values, which are ordered
 * according to a comparison object of type \c C. Class \c T is
 * the type of items in the collection. Class \c A is an allocator
 * of objects of class \c T. For more information about using
 * custom allocators, please see the <i>Essential Tools Module
 * User's Guide</i>.
 *
 * \c C must induce
 * a total ordering on elements of type \c T via a public member
 *
 * @code
 * bool operator()(const T& x, const T& y) const
 * @endcode
 *
 * which returns \c true if \c x should precede \c y within the
 * collection. The structure \c std::less<T> from the C++ Standard Library
 * header file \c \<functional\> is an example.
 *
 * \link RWTValMultiSet RWTValMultiSet<T,C,A>\endlink may contain multiple
 * items that compare equal to each other.
 * (\link RWTValSet RWTValSet<T,C,A>\endlink will not accept an item
 * that compares equal to an item already in the collection.)
 *
 * The \c value type must have \c operator== and \c operator< defined.
 * This requirement is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvmset.h>
 * RWTValMultiSet<T,C,A>
 * @endcode
 *
 * @section related Related Classes
 *
 * Class \link RWTValSet RWTValSet<T,C,A>\endlink offers the same interface to
 * a collection that will not accept multiple items that compare equal to each
 * other. \link RWTValMultiMap RWTValMultiMap<K,T,C,A>\endlink maintains a
 * collection of key-value pairs.
 *
 * Class <b>std::multiset<T,C,A></b> is the C++ Standard Library collection
 * that serves as the underlying implementation for
 * \link RWTValMultiSet RWTValMultiSet<T,C,A>\endlink.
 *
 * @section persistence Persistence
 *
 * Isomorphic.
 *
 * @section example Example
 *
 * @code
 * #include <rw/tvmset.h>
 * #include <rw/cstring.h>
 * #include <iostream>
 * #include <functional>
 *
 * int main()
 * {
 *  RWTValMultiSet<RWCString,std::less<RWCString> > set;
 *
 *   set.insert("one");
 *   set.insert("two");
 *   set.insert("three");
 *   set.insert("one");               // OK, duplicates allowed
 *
 *   std::cout << set.entries() << std::endl;   // Prints "4"
 *   return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 * 4
 * @endcode
 */
template <class T, class C = std::less<T>, class A = std::allocator<T> >
class RWTValMultiSet
{
public:

    /**
     * A type representing the underlying implementation container.
     */
    typedef std::multiset<T, C, A> container_type;

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
     * A type representing the key of the container.
     */
    typedef typename container_type::key_type key_type;

    /**
     * A type representing the comparison function.
     */
    typedef typename container_type::key_compare key_compare;

    /**
     * A type representing the comparison function.
     */
    typedef typename container_type::value_compare value_compare;


    /**
     * @internal
     * A type representing the same type as self.
     */
    typedef RWTValMultiSet<T, C, A> this_type;


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
     * Returns \c true if there are no items in the collection, otherwise
     * \c false.
     */
    bool isEmpty() const {
        return std().empty();
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
#if !defined(RW_NO_STD_MULTISET_ERASE_RETURN)
        return std().erase(pos);
#else
        iterator ret = pos;
        ++ret;
        std().erase(pos);
        return ret;
#endif
    }

    /**
     * Removes the items in the range [\a first, \a last) from the
     * collection. Returns an iterator that points to the next item in the
     * collection, or #end() if the last item in the collection was removed.
     */
    iterator erase(iterator first, iterator last) {
#if !defined(RW_NO_STD_MULTISET_ERASE_RETURN)
        return std().erase(first, last);
#else
        std().erase(first, last);
        return last;
#endif
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
     * that the expression <tt>((*\a fn)(a, \a d))</tt> is \c true,
     * otherwise returns \c false. Client data may be passed through
     * parameter \a d.
     */
    bool contains(bool(*fn)(const_reference, void*), void* d) const {
        return std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d)) != end();
    }

    /**
     * Returns \c true if there exists an element \c t in self that
     * compares equal to \a a, otherwise returns \c false.
     */
    bool contains(const_reference a) const {
        return (const_iterator) std().find(a) != std().end();
    }

    /**
     * If there exists an element \c t in self such that the expression
     * <tt>((*\a fn)(a, \a d))</tt> is \c true, assigns \c t to \a k and
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
     * If there exists an element \c t in self that compares equal to
     * \a a, assigns \c t to \a k and returns \c true. Otherwise, returns
     * \c false and leaves the value of \a k unchanged.
     */
    bool find(const_reference a, value_type& k) const {
        const_iterator iter = std().find(a);
        if (iter != end()) {
            k = *iter;
            return true;
        }
        return false;
    }

    /**
     * Returns the number of elements \c t in self such that the expression
     * <tt>((*\a fn)(a, \a d))</tt> is \c true. Client data may be passed
     * through parameter \a d.
     */
    size_type occurrencesOf(bool(*fn)(const_reference, void*), void* d) const {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Returns the number of elements \c t in self that compare equal
     * to \a a.
     */
    size_type occurrencesOf(const_reference a) const {
        return std().count(a);
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
     * <tt>((*\a fn)(a, \a d))</tt> is \c true and returns \c true.
     * Returns \c false if there is no such element. Client data may be
     * passed through parameter \a d.
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
     * Removes the first element \c t in self that compares equal to
     * \a a and returns \c true. Returns \c false if there is no such
     * element.
     */
    bool remove(const_reference a) {
        iterator hit = std().find(a);
        if (hit != end()) {
            std().erase(hit);
            return true;
        }
        return false;
    }

    /**
     * Removes all elements \c t in self such that the expression
     * <tt>((*\a fn)(a, \a d))</tt> is \c true. Returns the number
     * of items removed. Client data may be passed through parameter \a d.
     */
    size_type removeAll(bool(*fn)(const_reference, void*), void* d);

    /**
     * Removes all elements \c t in self that compare equal to \a a.
     * Returns the number of items removed.
     */
    size_type removeAll(const_reference a) {
        return std().erase(a);
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
     * Sets self to the union of self and \a s.
     * @note
     * The uppercase "U" in \c Union avoids conflict with the
     * C++ reserved word.
     */
    void Union(const RWTValMultiSet<T, C, A>& s) {
        Union(s.std());
    }

    /**
     * @copydoc Union()
     */
    void Union(const container_type& s) {
        // use algorithm we supply
        std::insert_iterator<container_type> putHere(std(), end());
        set_union_inplace(begin(), end(), s.begin(), s.end(),
                          putHere, std().key_comp());
    }

    /**
     * Sets self to the intersection of self and \a s.
     */
    void intersection(const RWTValMultiSet<T, C, A>& s) {
        intersection(s.std());
    }

    /**
     * @copydoc intersection()
     */
    void intersection(const container_type& s) {
        // use algorithm we supply
        set_intersection_inplace(std(), begin(), end(),
                                 s.begin(), s.end(), std().key_comp());
    }

    /**
     * Sets self to the set-theoretic difference given by (self - \a s).
     */
    void difference(const RWTValMultiSet<T, C, A>& s) {
        difference(s.std());
    }

    /**
     * @copydoc difference()
     */
    void difference(const container_type& s) {
        // use algorithm we supply
        set_difference_inplace(std(), begin(), end(),
                               s.begin(), s.end(), std().key_comp());
    }

    /**
     * Sets self to the symmetric difference of self and \a s.
     */
    void symmetricDifference(const RWTValMultiSet<T, C, A>& s) {
        symmetricDifference(s.std());
    }

    /**
     * @copydoc symmetricDifference()
     */
    void symmetricDifference(const container_type& s) {
        // use algorithm we supply
        set_symmetric_difference_inplace(std(), begin(), end(),
                                         s.begin(), s.end(), std().key_comp());
    }

    /**
     * Returns \c true if self is a subset of \a s or if self is set
     * equivalent to \c s, otherwise \c false.
     */
    bool
    isSubsetOf(const RWTValMultiSet<T, C, A>& s) const {
        return (entries() <= s.entries()) &&
               rw_is_subset(begin(), end(), s.begin(), s.end(), key_compare());
    }

    /**
     * Returns \c true if self is a proper subset of \a s, otherwise
     * returns \c false.
     */
    bool
    isProperSubsetOf(const RWTValMultiSet<T, C, A>& s) const {
        return (entries() < s.entries()) &&
               rw_is_subset(begin(), end(), s.begin(), s.end(), key_compare());
    }

    /**
     * Returns \c true if self and \a s are identical, otherwise returns
     * \c false.
     */
    bool
    isEquivalent(const RWTValMultiSet<T, C, A>& s) const {
        return (*this == s);
    }

    /**
     * Invokes the function pointer \a fn on each item in the collection.
     * Client data may be passed through
     * parameter \a d.
     */
    void apply(void (*fn)(const_reference, void*), void* d) const {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * Adds the item \a a to the collection. Returns \c true.
     */
    bool insert(const_reference a) {
        std().insert(a);
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
    iterator insert(iterator hint, const_reference val) {
        return std().insert(hint, val);
    }

    /**
     * Inserts the elements in the range [\a first, \a last) into self.
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last) {
#if !defined(RW_BROKEN_STD_MULTISET_INSERT_RANGE)
        std().insert(first, last);
#else
        std::copy(first, last, std::inserter(*this, end()));
#endif
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @copydoc insert()
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    bool insert(value_type && a) {
        std().insert(rw_move(a));
        return true;
    }

    /**
     * @copydoc insert(iterator, const_reference)
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    iterator insert(iterator hint, value_type && val) {
        return std().insert(hint, rw_move(val));
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Swaps the data held by self with the data held by \a other, including
     * the \c C object. This method does not copy or destroy any of the items
     * swapped; rather, it swaps the underlying hash tables.
     */
    void
    swap(RWTValMultiSet<T, C, A>& other) {
        std().swap(other.impl_);
    }

    /**
     * Destroys all elements of self and replaces them by copying all
     * elements of \a rhs.
     */
    RWTValMultiSet<T, C, A>&
    operator=(const RWTValMultiSet<T, C, A>& rhs) {
        return operator=(rhs.impl_);
    }

    /**
     * @copydoc operator=()
     */
    RWTValMultiSet<T, C, A>&
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
    RWTValMultiSet<T, C, A>&
    operator=(RWTValMultiSet<T, C, A> && rhs) {
        return operator=(rw_move(rhs.impl_));
    }

    /*
     * @copydoc operator=(RWTValMultiSet<T,C,A>&&)
     */
    RWTValMultiSet<T, C, A>&
    operator=(container_type && rhs) {
        if (&impl_ != &rhs) {
#    if !defined(RW_NO_STDSET_RVALUE_SUPPORT)
            impl_ = rw_move(rhs);
#    else
            rhs.swap(impl_);
#    endif
        }
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs an empty set.
     */
    RWTValMultiSet(const key_compare& cmp = key_compare()) : impl_(cmp) { }

    /**
     * Constructs a set by copying all elements of \a s.
     */
    RWTValMultiSet(const container_type& s) : impl_(s) { }

    /**
     * Copy constructor.
     */
    RWTValMultiSet(const RWTValMultiSet<T, C, A>& rws)
        : impl_(rws.impl_) { }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed list takes ownership of the
     * data owned by \a rws.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValMultiSet(RWTValMultiSet<T, C, A> && rws)
#    if !defined(RW_NO_STDSET_RVALUE_SUPPORT)
        : impl_(rw_move(rws.impl_)) { }
#    else
    {
        rws.std().swap(impl_);
    }
#    endif

    /**
     * @copydoc RWTValMultiSet(RWTValMultiSet<T,C,A>&&)
     */
    RWTValMultiSet(container_type && rws)
#    if !defined(RW_NO_STDSET_RVALUE_SUPPORT)
        : impl_(rw_move(rws)) { }
#    else
    {
        rws.swap(impl_);
    }
#    endif
#endif // !RW_NO_RVALUE_REFERENCES


    /**
     * Constructs a set by copying elements from the range
     * [\a first, \a last).
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
    template <typename InputIterator>
    RWTValMultiSet(InputIterator first, InputIterator last, const key_compare& cmp = key_compare())
#if !defined(RW_BROKEN_STD_SET_CTOR_RANGE)
        : impl_(first, last, cmp) { }
#else
        :
        impl_(cmp) {
        insert(first, last);
    }
#endif

private:
    container_type impl_;  // clients use std()
};

/**
 * @relates RWTValMultiSet
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and
 * iterating through both collections produces individual elements that,
 * in turn, compare equal to each other.
 */
template <class T, class C, class A>
bool operator==(const RWTValMultiSet<T, C, A>& lhs, const RWTValMultiSet<T, C, A>& rhs)
{
    return lhs.std() == rhs.std();
}

/**
 * @relates RWTValMultiSet
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T, class C, class A>
bool operator!=(const RWTValMultiSet<T, C, A>& lhs, const RWTValMultiSet<T, C, A>& rhs)
{
    return lhs.std() != rhs.std();
}

/**
 * @relates RWTValMultiSet
 *
 * Returns \c true if \a lhs is lexicographically less than \a rhs,
 * otherwise \c false. Assumes that type \c T has well-defined less-than
 * semantics.
 */
template <class T, class C, class A>
bool operator<(const RWTValMultiSet<T, C, A>& lhs, const RWTValMultiSet<T, C, A>& rhs)
{
    return lhs.std() < rhs.std();
}

/**
 * @relates RWTValMultiSet
 *
 * Equivalent to <tt>(\a rhs < \a lhs)</tt>.
 */
template <class T, class C, class A>
bool operator>(const RWTValMultiSet<T, C, A>& lhs, const RWTValMultiSet<T, C, A>& rhs)
{
    return lhs.std() > rhs.std();
}

/**
 * @relates RWTValMultiSet
 *
 * Equivalent to <tt>!(\a rhs < \a lhs)</tt>.
 */
template <class T, class C, class A>
bool operator<=(const RWTValMultiSet<T, C, A>& lhs, const RWTValMultiSet<T, C, A>& rhs)
{
    return lhs.std() <= rhs.std();
}

/**
 * @relates RWTValMultiSet
 *
 * Equivalent to <tt>!(\a lhs < \a rhs)</tt>.
 */
template <class T, class C, class A>
bool operator>=(const RWTValMultiSet<T, C, A>& lhs, const RWTValMultiSet<T, C, A>& rhs)
{
    return lhs.std() >= rhs.std();
}

/**
 * @relates RWTValMultiSet
 *
 * @copydoc operator==(const RWTValMultiSet&, const RWTValMultiSet&);
 */
template <class T, class C, class A>
bool operator==(const RWTValMultiSet<T, C, A>& lhs,
                const typename RWTValMultiSet<T, C, A>::container_type& rhs)
{
    return lhs.std() == rhs;
}

/**
 * @relates RWTValMultiSet
 *
 * @copydoc operator!=(const RWTValMultiSet&, const RWTValMultiSet&);
 */
template <class T, class C, class A>
bool operator!=(const RWTValMultiSet<T, C, A>& lhs,
                const typename RWTValMultiSet<T, C, A>::container_type& rhs)
{
    return lhs.std() != rhs;
}

/**
 * @relates RWTValMultiSet
 *
 * @copydoc operator<(const RWTValMultiSet&, const RWTValMultiSet&);
 */
template <class T, class C, class A>
bool operator<(const RWTValMultiSet<T, C, A>& lhs,
               const typename RWTValMultiSet<T, C, A>::container_type& rhs)
{
    return lhs.std() < rhs;
}

/**
 * @relates RWTValMultiSet
 *
 * @copydoc operator>(const RWTValMultiSet&, const RWTValMultiSet&);
 */
template <class T, class C, class A>
bool operator>(const RWTValMultiSet<T, C, A>& lhs,
               const typename RWTValMultiSet<T, C, A>::container_type& rhs)
{
    return lhs.std() > rhs;
}

/**
 * @relates RWTValMultiSet
 *
 * @copydoc operator<=(const RWTValMultiSet&, const RWTValMultiSet&);
 */
template <class T, class C, class A>
bool operator<=(const RWTValMultiSet<T, C, A>& lhs,
                const typename RWTValMultiSet<T, C, A>::container_type& rhs)
{
    return lhs.std() <= rhs;
}

/**
 * @relates RWTValMultiSet
 *
 * @copydoc operator>=(const RWTValMultiSet&, const RWTValMultiSet&);
 */
template <class T, class C, class A>
bool operator>=(const RWTValMultiSet<T, C, A>& lhs,
                const typename RWTValMultiSet<T, C, A>::container_type& rhs)
{
    return lhs.std() >= rhs;
}


/**
 * @relates RWTValMultiSet
 *
 * @copydoc operator==(const RWTValMultiSet&, const RWTValMultiSet&);
 */
template <class T, class C, class A>
bool operator==(const typename RWTValMultiSet<T, C, A>::container_type& lhs,
                const RWTValMultiSet<T, C, A>& rhs)
{
    return lhs == rhs.std();
}

/**
 * @relates RWTValMultiSet
 *
 * @copydoc operator!=(const RWTValMultiSet&, const RWTValMultiSet&);
 */
template <class T, class C, class A>
bool operator!=(const typename RWTValMultiSet<T, C, A>::container_type& lhs,
                const RWTValMultiSet<T, C, A>& rhs)
{
    return lhs != rhs.std();
}

/**
 * @relates RWTValMultiSet
 *
 * @copydoc operator<(const RWTValMultiSet&, const RWTValMultiSet&);
 */
template <class T, class C, class A>
bool operator<(const typename RWTValMultiSet<T, C, A>::container_type& lhs,
               const RWTValMultiSet<T, C, A>& rhs)
{
    return lhs < rhs.std();
}

/**
 * @relates RWTValMultiSet
 *
 * @copydoc operator>(const RWTValMultiSet&, const RWTValMultiSet&);
 */
template <class T, class C, class A>
bool operator>(const typename RWTValMultiSet<T, C, A>::container_type& lhs,
               const RWTValMultiSet<T, C, A>& rhs)
{
    return lhs > rhs.std();
}

/**
 * @relates RWTValMultiSet
 *
 * @copydoc operator<=(const RWTValMultiSet&, const RWTValMultiSet&);
 */
template <class T, class C, class A>
bool operator<=(const typename RWTValMultiSet<T, C, A>::container_type& lhs,
                const RWTValMultiSet<T, C, A>& rhs)
{
    return lhs <= rhs.std();
}

/**
 * @relates RWTValMultiSet
 *
 * @copydoc operator>=(const RWTValMultiSet&, const RWTValMultiSet&);
 */
template <class T, class C, class A>
bool operator>=(const typename RWTValMultiSet<T, C, A>::container_type& lhs,
                const RWTValMultiSet<T, C, A>& rhs)
{
    return lhs >= rhs.std();
}

/**
 * @relates RWTValMultiSet
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T, class C, class A>
RWvostream& operator<<(RWvostream& strm, const RWTValMultiSet<T, C, A>& coll);

/**
 * @relates RWTValMultiSet
 *
 * Saves the collection \a coll onto the RWFile \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T, class C, class A>
RWFile& operator<<(RWFile& strm, const RWTValMultiSet<T, C, A>& coll);

/**
 * @relates RWTValMultiSet
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class T, class C, class A>
RWvistream& operator>>(RWvistream& strm, RWTValMultiSet<T, C, A>& coll);

/**
 * @relates RWTValMultiSet
 *
 * Restores the contents of the collection \a coll from the RWFile \a strm.
 */
template <class T, class C, class A>
RWFile& operator>>(RWFile& strm, RWTValMultiSet<T, C, A>& coll);

/**
 * @relates RWTValMultiSet
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T, class C, class A>
RWvistream& operator>>(RWvistream& strm, RWTValMultiSet<T, C, A>*& p);

/**
 * @relates RWTValMultiSet
 *
 * Looks at the next object on the RWFile \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T, class C, class A>
RWFile& operator>>(RWFile& strm, RWTValMultiSet<T, C, A>*& p);

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tvmset.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tvmset.cc"
#endif

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @ingroup stl_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTValMultiSet::iterator instead.
 *
 * @brief Deprecated. Provides an iterator for RWTValMultiSet.
 *
 * RWTValMultiSetIterator provides an iterator interface to the
 * C++ Standard Library-based collection represented by RWTValMultiSet. (An STL
 * iterator interface is provided through RWTValMultiSet::iterator.)
 *
 * The order of iteration over an RWTValMultiSet is dependent on
 * the supplied comparator object parameter \c C as applied to the
 * values stored in the container.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset(). The iterator becomes
 * valid after being advanced with either a pre-increment or
 * operator()().
 *
 * For both operator++() and operator()(), iterating past the last
 * element returns a value equivalent to boolean \c false.
 * Continued increments return a value equivalent to \c false
 * until reset() is called.
 *
 * The \c value type must have \c operator== and \c operator< defined.
 * This requirement is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvmset.h>
 * RWTValMultiSet<T,C> vs;
 * RWTValMultiSetIterator<T,C> itr(vs);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/tvmset.h>
 * #include <iostream>
 * #include <rw/cstring.h>
 * #include <functional>
 *
 * int main()
 * {
 *    RWTValMultiSet<RWCString, std::greater<RWCString> > a;
 *    RWTValMultiSetIterator<RWCString,
 *      std::greater<RWCString> > itr(a);
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
 * Steve
 * Steve
 * Mark
 * John
 * @endcode
 */
template <class T, class C = std::less<T>, class A = std::allocator<T> >
class RW_DEPRECATE_TYPE("Use RWTValMultiSet::iterator instead") RWTValMultiSetIterator
{

public:
    /**
     * Creates an iterator for the multi-set \a h. The iterator begins
     * in an undefined state and must be advanced before the first element
     * is accessible.
     */
    RWTValMultiSetIterator(RWTValMultiSet<T, C, A>& h)
        : table_(&h), head_(true), tail_(false) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self now references the first element.
     * If, before iteration, self referenced the last association in
     * the multi-set, self now references an undefined value, and
     * \c false is returned. Otherwise, returns \c true.
     */
    bool operator++() {  // Advance and test
        RW_ASSERT(!(head_ == true && tail_ == true));

        if (tail_) {
            /* do nothing */
        }
        else if (head_) {
            iter_ = table_->begin();
            head_ = false;
        }
        else {
            ++iter_;
        }

        if (!tail_ && iter_ == table_->end()) {
            tail_ = true;
        }

        RW_ASSERT(!(head_ == true && tail_ == true));

        return !tail_;
    }

    /**
     * Advances self to the next element, and returns \c true. Returns
     * \c false if the iterator has advanced past the last element in
     * the collection.
     */
    bool operator()() {
        return ++(*this);
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    RWTValMultiSet<T, C, A>* container() const {
        return table_;
    }

    /**
     * Returns the value pointed to by self.
     */
    T key() const {
        return *(RW_CONST_CAST(RWTValMultiSetIterator*, this)->iter_);
    }

    /**
     * Resets the iterator so that after being advanced, it will point to the
     * first element of the collection.
     */
    void reset() {
        head_ = true;
        tail_ = false;
    }

    /**
     * Resets the iterator so that after being advanced, it will point to the
     * first element of the collection \a h.
     */
    void reset(RWTValMultiSet<T, C, A>& h) {
        head_ = true;
        tail_ = false;
        table_ = &h;
    }

private:

    RWTValMultiSet<T, C, A>* table_;
    typename RWTValMultiSet<T, C, A>::iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);
    friend class RWTValMultiSetConstIterator<T, C, A>;
};

/**
 * @ingroup stl_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTValMultiSet::const_iterator instead.
 *
 * @brief Deprecated. Provides a \c const iterator for RWTValMultiSet.
 *
 * RWTValMultiSetConstIterator provides a \c const iterator for
 * RWTValMultiSet. A \c const iterator prohibits any changes to
 * the collection over which it iterates.
 *
 * The order of iteration over an RWTValMultiSet is dependent on
 * the supplied comparator object parameter \c C as applied to the
 * values stored in the container.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset(). The iterator becomes
 * valid after being advanced with either a pre-increment or
 * operator()().
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
 * #include <rw/tvmset.h>
 * RWTValMultiSet<T,C> vs;
 * RWTValMultiSetConstIterator<T,C> itr(vs);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class T, class C = std::less<T>, class A = std::allocator<T> >
class RW_DEPRECATE_TYPE("use RWTValMultiSet::const_iterator instead") RWTValMultiSetConstIterator
{

public:
    /**
     * Creates a \c const iterator for the multi-set \a h. The iterator begins
     * in an undefined state and must be advanced before the first element
     * is accessible.
     */
    RWTValMultiSetConstIterator(const RWTValMultiSet<T, C, A>& h)
        : table_(&h), head_(true), tail_(false) { }

    /**
     * Creates a \c const iterator from the iterator \a itr. The \c const
     * iterator begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTValMultiSetConstIterator(const RWTValMultiSetIterator<T, C, A>& itr)
        : table_(itr.table_), iter_(itr.iter_),
          head_(itr.head_), tail_(itr.tail_) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self now references the first element.
     * If, before iteration, self referenced the last association in
     * the multi-set, self now references an undefined value and
     * \c false is returned. Otherwise, returns \c true.
     */
    bool operator++() {  // Advance and test
        RW_ASSERT(!(head_ == true && tail_ == true));

        if (tail_) {
            /* do nothing */
        }
        else if (head_) {
            iter_ = table_->begin();
            head_ = false;
        }
        else {
            ++iter_;
        }

        if (!tail_ && iter_ == table_->end()) {
            tail_ = true;
        }

        RW_ASSERT(!(head_ == true && tail_ == true));

        return !tail_;
    }

    /**
     * Advances self to the next element and returns \c true. Returns
     * \c false if the iterator has advanced past the last element in
     * the collection.
     */
    bool operator()() {
        return ++(*this);
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    const RWTValMultiSet<T, C, A>* container() const {
        return table_;
    }

    /**
     * Returns the value pointed to by self.
     */
    const T& key() const {
        return *iter_;
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
     * the first element of the collection \a h.
     */
    void reset(const RWTValMultiSet<T, C, A>& h) {
        head_ = true;
        tail_ = false;
        table_ = &h;
    }

private:

    const RWTValMultiSet<T, C, A>* table_;
    typename RWTValMultiSet<T, C, A>::const_iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);
};
#endif

#endif /* RW_TOOLS_TVMSET_H */
