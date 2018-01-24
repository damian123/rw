#ifndef RW_TOOLS_TPMSET_H
#define RW_TOOLS_TPMSET_H

/**********************************************************************
 *
 *     : pointer-based set wrapper for STL multiset
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tpmset.h#3 $
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
#include <rw/epaux.h>
#include <rw/epfunc.h>
#include <rw/stdex/algo.h>
#include <rw/tools/algorithm.h>

#include <set>

#if !defined(RW_DISABLE_DEPRECATED)

template <class T, class C, class A>
class RWTPtrMultiSetConstIterator;

#endif

/**
 * @ingroup stl_based_collection_classes
 *
 * @brief A pointer-based collection of values ordered according to
 * a comparison object, and allowing duplicate values.
 *
 * This class maintains a pointer-based collection of values, which
 * are ordered according to a comparison object of type \c C. Class
 * \c T is the type pointed to by the items in the collection.
 * Class \c A is an allocator of objects of class \c T. For more information
 * about using custom allocators, please see the <i>Essential Tools
 * Module User's Guide</i>.
 *
 * \c C must induce a total ordering on elements of type \c T via
 * a public member
 *
 * @code
 *
 * bool operator()(const T& x, const T& y)
 * @endcode
 *
 * that returns \c true if \c x should precede \c y within the collection.
 * The structure \b std::less<T> from the C++ Standard Library header file
 * \c \<functional> is an example. Note that items in the collection
 * will be dereferenced before being compared.
 *
 * \link RWTPtrMultiSet RWTPtrMultiSet<T,C,A>\endlink may contain multiple
 * items that compare equal to each other.
 * (\link RWTPtrSet RWTPtrSet<T,C,A>\endlink will not accept an item
 * that compares equal to an item already in the collection.)
 *
 * The \c value type must have \c operator== and \c operator< defined.
 * This requirement is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tpmset.h>
 * RWTPtrMultiSet<T,C,A> s;
 * @endcode
 *
 * @section related Related Classes
 *
 * Class \link RWTPtrSet RWTPtrSet<T,C,A>\endlink offers the same interface to
 * a pointer-based collection that will not accept multiple items that compare
 * equal to each other. \link RWTPtrMultiMap RWTPtrMultiMap<K,T,C,A>\endlink
 * maintains a pointer-based collection of key-value pairs.
 *
 * Class <tt>multiset<T*, key_compare,A ></tt> is the C++ Standard
 * Library collection that serves as the underlying implementation for
 * \link RWTPtrMultiSet RWTPtrMultiSet<T,C,A>\endlink.
 *
 * @section persistence Persistence
 *
 * Isomorphic.
 *
 * @section example Example
 *
 * @code
 *
 * // tpmset.cpp
 * //
 * #include <rw/tpmset.h>
 * #include <rw/cstring.h>
 * #include <iostream.h>
 * #include <functional>
 * int main(){
 *   RWTPtrMultiSet<RWCString, std::less<RWCString> > set;
 *   set.insert(new RWCString("one"));
 *   set.insert(new RWCString("two"));
 *   set.insert(new RWCString("three"));
 *   set.insert(new RWCString("one"));    // OK: duplicates allowed
 *   std::cout << set.entries() << std::endl;       // Prints "4"
 *   set.clearAndDestroy();
 *   std::cout << set.entries() << std::endl;       // Prints "0"
 *   return 0;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 *
 * 4
 * 0
 * @endcode
 *
 */
template <class T, class C = std::less<T>, class A = std::allocator<T*> >
class RWTPtrMultiSet
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
    typedef std::multiset<T*, key_compare, A> container_type;

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
     * A type representing a comparison function that dereferences the
     * arguments before comparing.
     */
    typedef typename container_type::value_compare value_compare;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrMultiSet::key_compare \c key_compare \endlink instead.
     *
     * A type representing a comparison function that dereferences the
     * arguments before comparing.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrMultiSet::key_compare instead") rw_deref_compare<C, T> container_comp;
#endif


    /**
     * @internal
     * A type representing the same type as self.
     */
    typedef RWTPtrMultiSet<T, C, A> this_type;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12.5, use <tt>const T*</tt> instead.
     */
    typedef RW_DEPRECATE_TYPE("Use const T* instead") const T* const_value;
#endif

    /**
     * @internal
     * A type representing an equality function that dereferences the
     * arguments before comparing.
     */
    typedef rw_deref_compare<std::equal_to<T>, T> deref_equal;

    /**
     * @internal
     * A type representing a comparison function that dereferences the
     * arguments before comparing.
     */
    typedef rw_deref_compare<std::less<T>, T> deref_less;


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
     * Returns \c true if there are no items in the collection, otherwise
     * \c false.
     */
    bool isEmpty() const {
        return std().empty();
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
     * Returns \c true if there exists an element \c t in self that
     * compares equal to <tt>*\a a</tt>, otherwise returns \c false.
     */
    bool contains(const T* a) const {
        return (const_iterator) std().find((value_type)a) != std().end();
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
    const T* find(const T* a) const {
        const_iterator iter = std().find((value_type)a);
        return (iter != end()) ? *iter : rwnil;
    }

    /**
     * @copydoc end()
     */
    value_type find(const T* a) {
        iterator iter = std().find((value_type)a);
        return (iter != end()) ? *iter : rwnil;
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
     * Returns the number of elements \c t in self that compare equal
     * to <tt>*\a a</tt>.
     */
    size_type occurrencesOf(const T* a) const {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_if(begin(), end(), rw_bind2nd(deref_equal(), a));
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
     * Removes and returns the first element \c t in self that compares
     * equal to <tt>*\a a</tt>. Returns #rwnil if there is no such element.
     */
    value_type remove(const T* a) {
        value_type ret = rwnil;
        iterator hit = std().find((value_type)a);
        if (hit != end()) {
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
    size_type removeAll(bool(*fn)(const T*, void*), void* d);

    /**
     * Removes all elements \c t in self that compare equal to <tt>*\a a</tt>.
     * Returns the number of items removed.
     */
    size_type removeAll(const T* a) {
        return std().erase((value_type)a);
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
     * Sets self to the union of self and \a s. Elements from each
     * set are dereferenced before being compared.
     * @note
     * The uppercase "U" in \c Union avoids conflict with the C++ reserved word.
     */
    void Union(const RWTPtrMultiSet<T, C, A>& s) {
        Union(s.std());
    }

    /**
     * Sets self to the intersection of self and \a s. Elements from
     * each set are dereferenced before being compared.
     */
    void intersection(const RWTPtrMultiSet<T, C, A>& s) {
        intersection(s.std());
    }

    /**
     * Sets self to the set-theoretic difference given by (self - \a s).
     * Elements from each set are dereferenced before being compared.
     */
    void difference(const RWTPtrMultiSet<T, C, A>& s) {
        difference(s.std());
    }

    /**
     * Sets self to the symmetric difference of self and \a s. Elements
     * from each set are dereferenced before being compared.
     */
    void symmetricDifference(const RWTPtrMultiSet<T, C, A>& s) {
        symmetricDifference(s.std());
    }

    /**
     * Invokes the function pointer \a fn on each item in the collection.
     * Client data may be passed through parameter \a d.
     */
    void apply(void (*fn)(const T*, void*), void* d) const {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * Returns \c true if self is a subset of \a s or if self is set
     * equivalent to \c rhs, otherwise \c false.
     */
    bool
    isSubsetOf(const RWTPtrMultiSet<T, C, A>& s) const {
        return (entries() <= s.entries()) &&
               rw_is_subset(begin(), end(), s.begin(), s.end(), key_compare());
    }

    /**
     * Returns \c true if self is a proper subset of \a s, otherwise returns
     * \c false.
     */
    bool
    isProperSubsetOf(const RWTPtrMultiSet<T, C, A>& s) const {
        return (entries() < s.entries()) &&
               rw_is_subset(begin(), end(), s.begin(), s.end(), key_compare());
    }

    /**
     * Returns \c true if self and \a s are identical, otherwise returns
     * \c false.
     */
    bool
    isEquivalent(const RWTPtrMultiSet<T, C, A>& s) const {
        return (*this == s);
    }

    /**
     * Adds the item \a a to the collection. Returns \c true.
     */
    bool insert(value_type a) {
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
    iterator insert(iterator hint, value_type val) {
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

    /**
     * Clears all elements of self and replaces them by copying all
     * elements of \a rhs.
     */
    RWTPtrMultiSet<T, C, A>&
    operator=(const RWTPtrMultiSet<T, C, A>& rhs) {
        return operator=(rhs.impl_);
    }

    /**
     * @copydoc operator=()
     */
    RWTPtrMultiSet<T, C, A>&
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
    RWTPtrMultiSet<T, C, A>&
    operator=(RWTPtrMultiSet<T, C, A> && rhs) {
        return operator=(rw_move(rhs.impl_));
    }

    /**
     * @copydoc operator=(RWTPtrMultiSet<T,C,A>&&)
     */
    RWTPtrMultiSet<T, C, A>&
    operator=(container_type && rhs) {
        if (&impl_ != &rhs) {
#    if !defined(RW_NO_STDMSET_RVALUE_SUPPORT)
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
    RWTPtrMultiSet(const key_compare& cmp = key_compare()) : impl_(cmp) {}
    /**
     * Constructs a multimap by copying all elements from \a ms.
     */
    RWTPtrMultiSet(const container_type& ms) : impl_(ms) { }

    /**
     * Copy constructor.
     */
    RWTPtrMultiSet(const RWTPtrMultiSet<T, C, A>& rws)
        : impl_(rws.impl_) { }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed list takes ownership of the
     * data owned by \a rws.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrMultiSet(RWTPtrMultiSet<T, C, A> && rws)
#    if !defined(RW_NO_STDMSET_RVALUE_SUPPORT)
        : impl_(rw_move(rws.impl_)) { }
#    else
    {
        rws.std().swap(impl_);
    }
#    endif

    /**
     * @copydoc RWTPtrMultiSet(RWTPtrMultiSet<T,C,A>&&)
     */
    RWTPtrMultiSet(container_type && rws)
#    if !defined(RW_NO_STDMSET_RVALUE_SUPPORT)
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
    RWTPtrMultiSet(InputIterator first, InputIterator last, const key_compare& cmp = key_compare())
#if !defined(RW_BROKEN_STD_SET_CTOR_RANGE)
        : impl_(first, last, cmp) { }
#else
        :
        impl_(cmp) {
        insert(first, last);
    }
#endif

    /**
     * Swaps the data held by self with the data held by \a other, including
     * the \c T and \c EQ objects. This method does not copy or destroy
     * any of the items swapped; rather, it swaps the underlying hash
     * tables.
     */
    void
    swap(RWTPtrMultiSet<T, C, A>& other) {
        std().swap(other.impl_);
    }

private:
    void Union(const container_type& s) {
        std::insert_iterator<container_type>  putHere(std(), end());
        // call the algorithm we provide
        set_union_inplace(begin(), end(),
                          s.begin(), s.end(),
                          putHere, std().key_comp());
    }

    void intersection(const container_type& s) {
        // call the algorithm we provide
        set_intersection_inplace(std(), begin(), end(),
                                 s.begin(), s.end(),
                                 std().key_comp());
    }

    void difference(const container_type& s) {
        // call the algorithm we provide
        set_difference_inplace(std(), begin(), end(),
                               s.begin(), s.end(), std().key_comp());
    }

    void symmetricDifference(const container_type& s) {
        // call the algorithm we provide
        set_symmetric_difference_inplace(
            std(), begin(), end(),
            s.begin(), s.end(), std().key_comp());
    }

    container_type impl_;
};


/**
 * @relates RWTPtrMultiSet
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and
 * iterating through both collections produces individual elements that,
 * in turn, compare equal to each other. Elements are dereferenced before
 * being compared.
 */
template <class T, class C, class A>
bool operator==(const RWTPtrMultiSet<T, C, A>& lhs, const RWTPtrMultiSet<T, C, A>& rhs)
{
    return lhs.entries() == rhs.entries() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrMultiSet<T, C, A>::deref_equal());
}

/**
 * @relates RWTPtrMultiSet
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T, class C, class A>
bool operator!=(const RWTPtrMultiSet<T, C, A>& lhs, const RWTPtrMultiSet<T, C, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrMultiSet
 *
 * Returns \c true if \a lhs is lexicographically less than \a rhs,
 * otherwise \c false. Items in each collection are dereferenced before
 * being compared. Assumes that type \c T has well-defined less-than
 * semantics.
 */
template <class T, class C, class A>
bool operator<(const RWTPtrMultiSet<T, C, A>& lhs, const RWTPtrMultiSet<T, C, A>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrMultiSet<T, C, A>::deref_less());
}

/**
 * @relates RWTPtrMultiSet
 *
 * Equivalent to <tt>(\a rhs < \a lhs)</tt>.
 */
template <class T, class C, class A>
bool operator>(const RWTPtrMultiSet<T, C, A>& lhs, const RWTPtrMultiSet<T, C, A>& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrMultiSet
 *
 * Equivalent to <tt>!(\a rhs < \a lhs)</tt>.
 */
template <class T, class C, class A>
bool operator<=(const RWTPtrMultiSet<T, C, A>& lhs, const RWTPtrMultiSet<T, C, A>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrMultiSet
 *
 * Equivalent to <tt>!(\a lhs < \a rhs)</tt>.
 */
template <class T, class C, class A>
bool operator>=(const RWTPtrMultiSet<T, C, A>& lhs, const RWTPtrMultiSet<T, C, A>& rhs)
{
    return !(lhs < rhs);
}

/**
 * @relates RWTPtrMultiSet
 *
 * @copydoc operator==(const RWTPtrMultiSet&, const RWTPtrMultiSet&);
 */
template <class T, class C, class A>
bool operator==(const RWTPtrMultiSet<T, C, A>& lhs,
                const typename RWTPtrMultiSet<T, C, A>::container_type& rhs)
{
    return lhs.entries() == rhs.size() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrMultiSet<T, C, A>::deref_equal());
}

/**
 * @relates RWTPtrMultiSet
 *
 * @copydoc operator!=(const RWTPtrMultiSet&, const RWTPtrMultiSet&);
 */
template <class T, class C, class A>
bool operator!=(const RWTPtrMultiSet<T, C, A>& lhs,
                const typename RWTPtrMultiSet<T, C, A>::container_type& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrMultiSet
 *
 * @copydoc operator<(const RWTPtrMultiSet&, const RWTPtrMultiSet&);
 */
template <class T, class C, class A>
bool operator<(const RWTPtrMultiSet<T, C, A>& lhs,
               const typename RWTPtrMultiSet<T, C, A>::container_type& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrMultiSet<T, C, A>::deref_less());
}

/**
 * @relates RWTPtrMultiSet
 *
 * @copydoc operator>(const RWTPtrMultiSet&, const RWTPtrMultiSet&);
 */
template <class T, class C, class A>
bool operator>(const RWTPtrMultiSet<T, C, A>& lhs,
               const typename RWTPtrMultiSet<T, C, A>::container_type& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrMultiSet
 *
 * @copydoc operator<=(const RWTPtrMultiSet&, const RWTPtrMultiSet&);
 */
template <class T, class C, class A>
bool operator<=(const RWTPtrMultiSet<T, C, A>& lhs,
                const typename RWTPtrMultiSet<T, C, A>::container_type& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrMultiSet
 *
 * @copydoc operator>=(const RWTPtrMultiSet&, const RWTPtrMultiSet&);
 */
template <class T, class C, class A>
bool operator>=(const RWTPtrMultiSet<T, C, A>& lhs,
                const typename RWTPtrMultiSet<T, C, A>::container_type& rhs)
{
    return !(lhs < rhs);
}


/**
 * @relates RWTPtrMultiSet
 *
 * @copydoc operator==(const RWTPtrMultiSet&, const RWTPtrMultiSet&);
 */
template <class T, class C, class A>
bool operator==(const typename RWTPtrMultiSet<T, C, A>::container_type& lhs,
                const RWTPtrMultiSet<T, C, A>& rhs)
{
    return lhs.size() == rhs.entries() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrMultiSet<T, C, A>::deref_equal());
}

/**
 * @relates RWTPtrMultiSet
 *
 * @copydoc operator!=(const RWTPtrMultiSet&, const RWTPtrMultiSet&);
 */
template <class T, class C, class A>
bool operator!=(const typename RWTPtrMultiSet<T, C, A>::container_type& lhs,
                const RWTPtrMultiSet<T, C, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrMultiSet
 *
 * @copydoc operator<(const RWTPtrMultiSet&, const RWTPtrMultiSet&);
 */
template <class T, class C, class A>
bool operator<(const typename RWTPtrMultiSet<T, C, A>::container_type& lhs,
               const RWTPtrMultiSet<T, C, A>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrMultiSet<T, C, A>::deref_less());
}

/**
 * @relates RWTPtrMultiSet
 *
 * @copydoc operator>(const RWTPtrMultiSet&, const RWTPtrMultiSet&);
 */
template <class T, class C, class A>
bool operator>(const typename RWTPtrMultiSet<T, C, A>::container_type& lhs,
               const RWTPtrMultiSet<T, C, A>& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrMultiSet
 *
 * @copydoc operator<=(const RWTPtrMultiSet&, const RWTPtrMultiSet&);
 */
template <class T, class C, class A>
bool operator<=(const typename RWTPtrMultiSet<T, C, A>::container_type& lhs,
                const RWTPtrMultiSet<T, C, A>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrMultiSet
 *
 * @copydoc operator>=(const RWTPtrMultiSet&, const RWTPtrMultiSet&);
 */
template <class T, class C, class A>
bool operator>=(const typename RWTPtrMultiSet<T, C, A>::container_type& lhs,
                const RWTPtrMultiSet<T, C, A>& rhs)
{
    return !(lhs < rhs);
}

/**
 * @relates RWTPtrMultiSet
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T, class C, class A>
RWvostream& operator<<(RWvostream& strm, const RWTPtrMultiSet<T, C, A>& coll);

/**
 * @relates RWTPtrMultiSet
 *
 * Saves the collection \a coll onto the RWFile \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T, class C, class A>
RWFile& operator<<(RWFile& strm, const RWTPtrMultiSet<T, C, A>& coll);

/**
 * @relates RWTPtrMultiSet
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class T, class C, class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrMultiSet<T, C, A>& coll);

/**
 * @relates RWTPtrMultiSet
 *
 * Restores the contents of the collection \a coll from the RWFile
 * \a strm.
 */
template <class T, class C, class A>
RWFile& operator>>(RWFile& strm, RWTPtrMultiSet<T, C, A>& coll);

/**
 * @relates RWTPtrMultiSet
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T, class C, class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrMultiSet<T, C, A>*& p);

/**
 * @relates RWTPtrMultiSet
 *
 * Looks at the next object on the RWFile \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T, class C, class A>
RWFile& operator>>(RWFile& strm, RWTPtrMultiSet<T, C, A>*& p);

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tpmset.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tpmset.cc"
#endif

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @ingroup stl_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTPtrMultiSet::iterator to iterate
 * over the elements in this collection.
 *
 * @brief Deprecated. Provides an iterator for RWTPtrMultiSet.
 *
 * RWTPtrMultiSetIterator provides an iterator interface to the
 * C++ Standard Library-based collection represented by RWTPtrMultiSet. (An STL
 * iterator interface is provided through \c RWTPtrMultiSet::iterator.)
 *
 * The order of iteration over an RWTPtrMultiSet is dependent upon
 * the comparator object parameter \c C as applied to the values
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
 * #include<rw/tpmset.h>
 * RWTPtrMultiSet<T,C,A> set;
 * RWTPtrMultiSetIterator<T,C,A> itr(set);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/tpmset.h>
 * #include <iostream>
 * #include <rw/cstring.h>
 *
 * int main() {
 *    RWTPtrMultiSet<RWCString, std::less<RWCString> > a;
 *    RWTPtrMultiSetIterator<RWCString, std::less<RWCString> > itr(a);
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
 * Mark
 * Steve
 * Steve
 * @endcode
 *
 */
template <class T, class C = std::less<T>, class A = std::allocator<T*> >
class RW_DEPRECATE_TYPE("Use RWTPtrMultiSet::iterator instead") RWTPtrMultiSetIterator
{

public:
    /**
     * Creates an iterator for the multiset \a m. The iterator begins
     * in an undefined state and must be advanced before the first element
     * will be accessible.
     */
    RWTPtrMultiSetIterator(RWTPtrMultiSet<T, C, A>& m)
        : table_(&m), head_(true), tail_(false) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self will now reference the first element.
     * If, before iteration, self referenced the last association in
     * the multiset, self will now point to an undefined value, and
     * a value equivalent to \c false will be returned. Otherwise,
     * a value equivalent to \c true is returned.
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
     * Advances self to the next element, dereferences the resulting
     * iterator, and returns its value. If the iterator has advanced
     * past the last item in the container, #rwnil will be returned.
     */
    T* operator()() {
        return (++(*this)) ? key() : rwnil;
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    RWTPtrMultiSet<T, C, A>*
    container() const {
        return table_;
    }

    /**
     * Returns the stored value referenced by self. Undefined if
     * self is not referencing a value within the list.
     */
    T* key() const {
        return *(RW_CONST_CAST(RWTPtrMultiSetIterator*, this)->iter_);
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
     * the first element of the collection \a t.
     */
    void reset(RWTPtrMultiSet<T, C, A>& t) {
        head_ = true;
        tail_ = false;
        table_ = &t;
    }

private:

    RWTPtrMultiSet<T, C, A>* table_;
    typename RWTPtrMultiSet<T, C, A>::iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);
    friend class RWTPtrMultiSetConstIterator<T, C, A>;
};

/**
 * @ingroup stl_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTPtrMultiSet::const_iterator to
 * iterate over the elements in this collection.
 *
 * @brief Deprecated. Provides a \c const iterator for RWTPtrMultiSet.
 *
 * RWTPtrMultiSetConstIterator provides a \c const iterator interface to the
 * C++ Standard Library-based collection represented by RWTPtrMultiSet.
 *
 * The order of iteration over an RWTPtrMultiSet is dependent upon
 * the comparator object parameter \c C as applied to the values
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
 * #include <rw/tpmset.h>
 * template <class T, class C, class A=std::allocator<T*> >
 * class RWTPtrMultiSetConstIterator
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class T, class C = std::less<T>, class A = std::allocator<T*> >
class RW_DEPRECATE_TYPE("Use RWTPtrMultiSet::const_iterator") RWTPtrMultiSetConstIterator
{

public:
    /**
     * Creates a \c const iterator for the multiset \a m. The iterator begins
     * in an undefined state and must be advanced before the first element
     * is accessible.
     */
    RWTPtrMultiSetConstIterator(const RWTPtrMultiSet<T, C, A>& m)
        : table_(&m), head_(true), tail_(false) { }

    /**
     * Creates a \c const iterator from the iterator \a itr. The \c const
     * iterator begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTPtrMultiSetConstIterator(const RWTPtrMultiSetIterator<T, C, A>& itr)
        : table_(itr.table_), iter_(itr.iter_),
          head_(itr.head_), tail_(itr.tail_) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self now references the first element.
     * If, before iteration, self referenced the last association in
     * the multiset, self now points to an undefined value and a value
     * equivalent to \c false is returned. Otherwise, a value equivalent
     * to \c true is returned.
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
     * Advances self to the next element, dereferences the resulting
     * iterator, and returns its value. If the iterator has advanced
     * past the last item in the container, #rwnil will be returned.
     */
    const T* operator()() {
        return (++(*this)) ? key() : rwnil;
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    const RWTPtrMultiSet<T, C, A>*
    container() const {
        return table_;
    }

    /**
     * Returns the stored value referenced by self. Undefined if
     * self is not referencing a value within the list.
     */
    const T* key() const {
        return *iter_;
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
     * the first element of the collection \a t.
     */
    void reset(const RWTPtrMultiSet<T, C, A>& t) {
        head_ = true;
        tail_ = false;
        table_ = &t;
    }

private:

    const RWTPtrMultiSet<T, C, A>* table_;
    typename RWTPtrMultiSet<T, C, A>::const_iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);
};
#endif

#endif /* RW_TOOLS_TPMSET_H */
