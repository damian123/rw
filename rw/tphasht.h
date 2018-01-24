#ifndef RW_TOOLS_TPHMSET_H
#define RW_TOOLS_TPHMSET_H

/**********************************************************************
 *
 * tphasht.h - RWTPtrHashMultiSet<T,H,EQ>
 *     : pointer-based set wrapper for STL-like rw_hashmultiset
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tphasht.h#3 $
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
#  include "rw/xphasht.h"
#else
#  include <rw/edefs.h> // for rw_move
#  include <rw/epaux.h>
#  include <rw/epfunc.h>
#  include <rw/stdex/hashmset.h>
#  include <rw/tools/algorithm.h>

/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @brief A pointer-based collection of values stored according to
 * a hash object, which allows duplicate values.
 *
 * This class maintains a pointer-based collection of values, which
 * are stored according to a hash object of type \c H. Class \c T
 * is the type pointed to by the items in the collection.
 * Class \c A is an allocator of objects of class \c T. For more information
 * about using custom allocators, please see the <i>Essential Tools Module
 * User's Guide</i>.
 *
 * \c H must provide a \c const function that takes a single argument
 * convertible to type \c T and returns a value of type \c size_t.
 *
 * Equivalent elements within the collection are grouped together based on an
 * equality object of type \c EQ. \c EQ must ensure this grouping via a
 * function that takes two arguments convertible to type \c T and
 * returns a value of type \c bool.
 *
 * @note
 * Any two elements that are equivalent \e must hash to the same value.
 *
 * This class allows multiple items that are equivalent to each other.
 * Use \link RWTPtrHashSet RWTPtrHashSet<T,H,EQ,A>\endlink if you
 * need a class that does not allow equivalent values.
 *
 * The \c value type must have \c operator==() defined. This requirement
 * is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tphasht.h>
 * RWTPtrHashMultiSet<T,H,EQ,A> hmset;
 * @endcode
 *
 * @section related Related Classes
 *
 * Class \link RWTPtrHashSet RWTPtrHashSet<T,H,EQ,A>\endlink offers the same
 * interface to a pointer-based collection that does not allow multiple items
 * that compare equal to each other.
 *
 * Class
 * \link rw_hashmultiset rw_hashmultiset<T*,hasher,key_equal,A>\endlink
 * is the C++ Standard Library collection that serves as the underlying
 * implementation for this collection.
 *
 * @section persistence Persistence
 *
 * Isomorphic
 *
 * @section example Example
 *
 * @code
 * //
 * // tphmset.cpp
 * //
 * #include <iostream>
 * #include <functional>
 * #include <rw/tphasht.h>
 * #include <rw/cstring.h>
 *
 * struct simple_hash
 * {
 *     unsigned long
 *     operator() (const RWCString& ref) const {
 *         size_t n = ref.length ();
 *         return (unsigned long)(n ? n * size_t (ref [0]) : 0);
 *     }
 * };
 *
 * int main ()
 * {
 *     RWTPtrHashMultiSet<RWCString,simple_hash,std::equal_to<RWCString> > set1;
 *     RWTPtrHashMultiSet<RWCString,simple_hash,std::equal_to<RWCString> > set2;
 *
 *     set1.insert(new RWCString("one"));
 *     set1.insert(new RWCString("two"));
 *     set1.insert(new RWCString("three"));
 *     set1.insert(new RWCString("one"));    // OK: duplicates allowed
 *
 *     std::cout << set1.entries() << "\n";  // Prints "4"
 *
 *     set2 = set1;
 *     std::cout << ((set1.isEquivalent(set2)) ? "true" : "false") << "\n";
 *     // Prints "true"
 *
 *     set2.difference(set1);
 *
 *     set1.clearAndDestroy();
 *     std::cout << set1.entries() << "\n"; // Prints "0"
 *     std::cout << set2.entries() << "\n"; // Prints "0"
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 * 4
 * true
 * 0
 * 0
 * @endcode
 */
template <class T, class H = RWTHash<T>, class EQ = std::equal_to<T>, class A = std::allocator<T*> >
class RWTPtrHashMultiSet
{
public:

    /**
     * A type representing an equality function that dereferences the
     * arguments before comparing.
     */
    typedef rw_deref_compare<EQ, T> key_equal;

    /**
     * A type representing a hash function that dereferences the arguments
     * before comparing.
     */
    typedef rw_deref_hash<H, T> hasher;

    /**
     * A type representing the underlying implementation container.
     */
    typedef rw_hashmultiset < T*, hasher, key_equal, A> container_type;

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
     * A type representing the key of the container.
     */
    typedef typename container_type::key_type key_type;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrHashMultiSet::key_equal \c key_equal \endlink instead.
     *
     * A type representing an equality function that dereferences the
     * arguments before comparing.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrHashMultiSet::key_equal instead") rw_deref_compare<EQ, T> container_eq;

    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrHashMultiSet::hasher \c hasher \endlink instead.
     *
     * A type representing a hash function that dereferences the arguments
     * before comparing.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrHashMultiSet::hasher instead") rw_deref_hash<H, T> container_hash;
#endif

    /**
     * @internal
     * A type representing the same type as self.
     */
    typedef RWTPtrHashMultiSet<T, H, EQ, A> this_type;

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
     * Returns \c true if there are no items in the collection,
     * otherwise returns \c false.
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
     * Clears the collection by removing all items from self.
     */
    void clear() {
        std().clear();
    }

    /**
     * Removes all items from the collection and uses <tt>operator
     * delete</tt> to destroy the elements pointed to by those items. Do
     * not use this method if multiple pointers to the same element are
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
        const_iterator iter = std().find((value_type)a);
        return iter != std().end();
    }

    /**
     * If there exists an element \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, returns \c t, otherwise
     * returns #rwnil. Client data may be passed through parameter \a d.
     */
    const T* find(bool(*fn)(const T*, void*), void* d) const {
        const_iterator ret = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        return (ret != end()) ? *ret : rwnil;
    }

    /**
     * If there exists an element \c t in self that compares equal to
     * <tt>*\a a</tt>, returns \c t, otherwise returns #rwnil.
     */
    value_type find(const T* a) {
        iterator iter = std().find((value_type)a);
        return (iter != end()) ? *iter : 0;
    }

    /**
     * @copydoc find(const T*)
     */
    const T* find(const T* a) const {
        const_iterator iter = std().find((value_type)a);
        return (iter != end()) ? *iter : 0;
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
     * Removes and returns the first element \c t in self that compares
     * equal to <tt>*\a a</tt>. Returns #rwnil if there is no such element.
     */
    value_type remove(const T* a) {
        const T* ret = 0;
        iterator hit = std().find((value_type)a);
        if (hit != end()) {
            ret = *iterator(hit);
            std().erase(hit);
        }
        return (value_type)ret;
    }

    /**
     * Removes all elements \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns the number
     * of elements removed. Client data may be passed through parameter
     * \a d.
     */
    size_type removeAll(bool(*fn)(const T*, void*), void* d);

    /**
     * Removes all elements \c t in self that compare equal to <tt>*\a a</tt>.
     * Returns the number of elements removed.
     */
    size_type removeAll(const T* a) {
        return std().erase((value_type)a);
    }

    // selective inheritance (hides other same-name components)
    void apply(void (*fn)(const T&, void*), void* d) const {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
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
     * equivalent to \a s, otherwise returns \c false.
     */
    bool
    isSubsetOf(const RWTPtrHashMultiSet<T, H, EQ, A>& s) const {
        if (entries() > s.entries()) {
            return false;
        }

        const_iterator itr = begin();
        while (itr != end()) {
            size_type s1 = std().count(*itr);
            size_type s2 = s.std().count(*itr);
            if (s1 <= s2) {
                std::advance(itr, s1);
            }
            else {
                return false;
            }
        }
        return itr == end();
    }

    /**
     * Returns \c true if self is a proper subset of \a s, otherwise returns
     * \c false.
     */
    bool
    isProperSubsetOf(const RWTPtrHashMultiSet<T, H, EQ, A>& s) const {
        return ((entries() < s.entries()) && isSubsetOf(s));
    }

    /**
     * Returns \c true if self and \a s are identical, otherwise returns
     * \c false.
     */
    bool
    isEquivalent(const RWTPtrHashMultiSet<T, H, EQ, A>& s) const {
        return ((entries() == s.entries()) && isSubsetOf(s));
    }

    /**
     * Destructively performs a set-theoretic union operation on self
     * and \a rhs. Self is replaced by the result. Note the uppercase
     * "U" in \c Union to avoid conflict with the C++ reserved word.
     */
    void
    Union(const RWTPtrHashMultiSet<T, H, EQ, A>& rhs) {
        Union(rhs.std());
    }

    /**
     * Destructively performs a set-theoretic intersection of self and
     * \a s, replacing the contents of self with the result.
     */
    void
    intersection(const RWTPtrHashMultiSet<T, H, EQ, A>& s) {
        intersection(s.std());
    }

    /**
     * Sets self to the set-theoretic difference given by (self - \a s).
     * Elements from each set are dereferenced before being compared.
     */
    void
    difference(const RWTPtrHashMultiSet<T, H, EQ, A>& s) {
        difference(s.std());
    }

    /**
     * Destructively performs a set-theoretic symmetric difference operation
     * on self and \a rhs. Self is replaced by the result. A symmetric difference
     * can be informally defined as @f$ (A \cup B) / (A \cap B) @f$.
     */
    void
    symmetricDifference(const RWTPtrHashMultiSet<T, H, EQ, A>& rhs) {
        symmetricDifference(rhs.std());
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
     * container's hash object.
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
        std().insert(first, last);
    }

    /**
     * Returns the number of buckets (slots) available in the underlying
     * hash representation.
     *
     * @see resize()
     */
    size_type
    capacity() const {
        return std().capacity();
    }

    /**
     * Returns the ratio entries() / capacity().
     */
    float
    fillRatio() const {
        return std().fill_ratio();
    }

    /**
     * Changes the capacity of self by creating a new hashed multi-set
     * with a capacity of \a sz. Copies every element of self into the
     * new container and finally swaps the internal representation
     * of the new container with the internal representation of self.
     */
    void
    resize(size_type sz) {
        std().resize(sz);
    }

    /**
     * Swaps the data held by self with the data held by \a other, including
     * the \c T and \c EQ objects. This method does not copy or destroy
     * any of the items swapped; rather, it swaps the underlying hash
     * tables.
     */
    void
    swap(RWTPtrHashMultiSet<T, H, EQ, A>& other) {
        std().swap(other.impl_);
    }

    /**
     * Clears all elements of self and replaces them by copying all
     * elements of \a rhs.
     */
    RWTPtrHashMultiSet<T, H, EQ, A>&
    operator=(const RWTPtrHashMultiSet<T, H, EQ, A>& rhs) {
        return operator=(rhs.impl_);
    }

    /**
     * @copydoc operator=()
     */
    RWTPtrHashMultiSet<T, H, EQ, A>&
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
    RWTPtrHashMultiSet<T, H, EQ, A>&
    operator=(RWTPtrHashMultiSet<T, H, EQ, A> && rhs) {
        return operator=(rw_move(rhs.impl_));
    }

    /**
     * @copydoc operator=(RWTPtrHashMultiSet<T,H,EQ,A>&&)
     */
    RWTPtrHashMultiSet<T, H, EQ, A>&
    operator=(container_type && rhs) {
        if (&impl_ != &rhs) {
            impl_ = rw_move(rhs);
        }
        return *this;
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs an empty multi-set. The hash table representation
     * used by this multi-set has \a sz buckets, uses \a h as a
     * hashing function, and \a eq to test for equality between stored
     * elements.
     *
     * @note If the value specified for \a sz is zero, the default bucket
     * count is used.
     */
    RWTPtrHashMultiSet(size_type sz = RW_DEFAULT_CAPACITY,
                       const H& h = H(), const EQ& eq = EQ())
        : impl_(sz ? sz : RW_DEFAULT_CAPACITY, h, eq) { }

    /**
     * Copy constructor.
     */
    RWTPtrHashMultiSet(const RWTPtrHashMultiSet<T, H, EQ, A>& rws)
        : impl_(rws.impl_) { }

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed list takes ownership of the
     * data owned by \a rws.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrHashMultiSet(RWTPtrHashMultiSet<T, H, EQ, A> && rws)
        : impl_(rw_move(rws.impl_)) { }

    /**
     * @copydoc RWTPtrHashMultiSet(RWTPtrHashMultiSet<T,H,EQ,A> &&)
     */
    RWTPtrHashMultiSet(container_type && rws) : impl_(rw_move(rws)) { }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs a hashed multi-set, copying all element from \a s.
     */
    RWTPtrHashMultiSet(const container_type& s) : impl_(s) { }

    /**
     * Constructs a set by copying elements from the range
     * [\a first, \a last). The hash table representation used by
     * this multi-set has \a sz buckets, uses \a h as a hashing function,
     * and \a eq to test for equality between stored elements.
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     *
     * @note If the value specified for \a sz is zero, the default bucket
     * count is used.
     */
    template <typename InputIterator>
    RWTPtrHashMultiSet(InputIterator first, InputIterator last,
                       size_type sz = RW_DEFAULT_CAPACITY,
                       const H& h = H(), const EQ& eq = EQ())
        : impl_(first, last, sz ? sz : RW_DEFAULT_CAPACITY, h, eq) { }

    // backward compatible constructor:
    /**
     * Creates an empty hashed multi-set that uses the hash object
     * \a h and has an initial hash table capacity of \a sz.
     *
     * @note If the value specified for \a sz is zero, the default capacity
     * is used.
     */
    RWTPtrHashMultiSet(const H& h, size_type sz = RW_DEFAULT_CAPACITY)
        : impl_(sz ? sz : RW_DEFAULT_CAPACITY, h) { }

private:
    void Union(const container_type&);
    void intersection(const container_type&);
    void difference(const container_type&);
    void symmetricDifference(const container_type&);

    container_type impl_;
};


/**
 * @relates RWTPtrHashMultiSet
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and each
 * is a subset of the other. Elements are dereferenced before being compared.
 */
template <class T, class H, class EQ , class A>
bool operator==(const RWTPtrHashMultiSet<T, H, EQ, A>& lhs, const RWTPtrHashMultiSet<T, H, EQ, A>& rhs)
{
    return lhs.isEquivalent(rhs);
}

/**
 * @relates RWTPtrHashMultiSet
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T, class H, class EQ , class A>
bool operator!=(const RWTPtrHashMultiSet<T, H, EQ, A>& lhs, const RWTPtrHashMultiSet<T, H, EQ, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrHashMultiSet
 *
 * @copydoc operator==(const RWTPtrHashMultiSet&, const RWTPtrHashMultiSet&);
 */
template <class T, class H, class EQ , class A>
bool operator==(const RWTPtrHashMultiSet<T, H, EQ, A>& lhs,
                const typename RWTPtrHashMultiSet<T, H, EQ, A>::container_type& rhs)
{
    return lhs.isEquivalent(rhs);
}

/**
 * @relates RWTPtrHashMultiSet
 *
 * @copydoc operator!=(const RWTPtrHashMultiSet&, const RWTPtrHashMultiSet&);
 */
template <class T, class H, class EQ , class A>
bool operator!=(const RWTPtrHashMultiSet<T, H, EQ, A>& lhs,
                const typename RWTPtrHashMultiSet<T, H, EQ, A>::container_type& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrHashMultiSet
 *
 * @copydoc operator==(const RWTPtrHashMultiSet&, const RWTPtrHashMultiSet&);
 */
template <class T, class H, class EQ , class A>
bool operator==(const typename RWTPtrHashMultiSet<T, H, EQ, A>::container_type& lhs,
                const RWTPtrHashMultiSet<T, H, EQ, A>& rhs)
{
    return rhs.isEquivalent(lhs);
}

/**
 * @relates RWTPtrHashMultiSet
 *
 * @copydoc operator!=(const RWTPtrHashMultiSet&, const RWTPtrHashMultiSet&);
 */
template <class T, class H, class EQ , class A>
bool operator!=(const typename RWTPtrHashMultiSet<T, H, EQ, A>::container_type& lhs,
                const RWTPtrHashMultiSet<T, H, EQ, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrHashMultiSet
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T, class H, class EQ , class A>
RWvostream& operator<<(RWvostream& strm, const RWTPtrHashMultiSet<T, H, EQ , A>& coll);

/**
 * @relates RWTPtrHashMultiSet
 *
 * Streams the collection \a coll to the RWFile \a file,
 * or a reference to it if it has already been saved.
 */
template <class T, class H, class EQ , class A>
RWFile& operator<<(RWFile& file, const RWTPtrHashMultiSet<T, H, EQ , A>& coll);

/**
 * @relates RWTPtrHashMultiSet
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class T, class H, class EQ , class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrHashMultiSet<T, H, EQ , A>& coll);

/**
 * @relates RWTPtrHashMultiSet
 *
 * Restores the contents of the collection \a coll by streaming
 * from the RWFile \a file.
 */
template <class T, class H, class EQ , class A>
RWFile& operator>>(RWFile& file, RWTPtrHashMultiSet<T, H, EQ , A>& coll);

/**
 * @relates RWTPtrHashMultiSet
 *
 * Looks at the next element on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, the application is
 * responsible for deleting it.
 */
template <class T, class H, class EQ , class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrHashMultiSet<T, H, EQ , A>*& p);

/**
 * @relates RWTPtrHashMultiSet
 *
 * Looks at the next element from the streamed RWFile \a file and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap,the application is
 * responsible for deleting it.
 */
template <class T, class H, class EQ , class A>
RWFile& operator>>(RWFile& file, RWTPtrHashMultiSet<T, H, EQ , A>*& p);

#  if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#    pragma implementation ("tphasht.cc")
#  endif

#  ifdef RW_NO_IMPLICIT_INCLUSION
#    include "rw/tphasht.cc"
#  endif

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTPtrHashMultiSet::iterator to iterate
 * over the elements in this collection.
 *
 * @brief Deprecated. Provides an iterator for RWTPtrHashMultiSet.
 *
 * RWTPtrHashMultiSetIterator provides an iterator interface to the
 * C++ Standard Library-based collection represented by RWTPtrHashMultiSet.
 * (An STL iterator interface is provided through \c RWTPtrHashMultiSet::iterator.)
 *
 * Iteration over an RWTPtrHashMultiSet is pseudo-random and dependent
 * on the capacity of the underlying hash table and the hash function
 * being used. The only usable relationship between consecutive
 * elements is that all elements that are defined to be equivalent
 * by the equivalence object, \c EQ, will remain adjacent.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset() operation. The iterator
 * becomes valid after being advanced with either a pre-increment
 * or operator()().
 *
 * For both operator++() and operator()(), iterating past the last
 * element returns a value equivalent to boolean \c false.
 * Continued increments return a value equivalent to \c false
 * until reset() is called.
 *
 * The value type must have \c operator==() defined. This requirement
 * is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include<rw/tphasht.h>
 * RWTPtrHashMultiSet<T,H,EQ,A> m;
 * RWTPtrHashMultiSet<T,H,EQ,A> itr(m);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/tphasht.h>
 * #include <iostream>
 * #include <rw/cstring.h>
 *
 * int main() {
 *    RWTPtrHashMultiSet<RWCString,RWTHasher<RWCString>,
 *      std::equal_to<RWCString> > age(RWCString::hash);
 *
 *    RWTPtrHashMultiSetIterator
 *      <RWCString,RWTHasher<RWCString>,std::equal_to<RWCString> >
 *        itr(age);
 *
 *    age.insert(new RWCString("John"));
 *    age.insert(new RWCString("Steve"));
 *    age.insert(new RWCString("Mark"));
 *    age.insert(new RWCString("Steve"));
 *
 *    for(;++itr;) {
 *      std::cout << *itr.key() << std::endl;
 *    }
 *
 *    age.clearAndDestroy();
 *    return 0;
 * }
 * @endcode
 *
 * Program Output (not necessarily in this order):
 *
 * @code
 * John
 * Mark
 * Steve
 * Steve
 * @endcode
 */
template <class T, class H = RWTHash<T>, class EQ = std::equal_to<T>, class A = std::allocator<T*> >
class RW_DEPRECATE_TYPE("Use RWTPtrHashMultiSet::iterator instead") RWTPtrHashMultiSetIterator
{

public:
    /**
     * Creates an iterator for the hashed multi-set \a h. The iterator
     * begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTPtrHashMultiSetIterator(RWTPtrHashMultiSet<T, H, EQ, A>& h)
        : table_(&h), head_(true), tail_(false) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self references the first element.
     * If used when self references the last association in the multiset,
     * self references an undefined value and the method returns a value
     * equivalent to \c false. Otherwise, the method returns a value
     * equivalent to \c true.
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
     * iterator and returns its value. If the iterator has advanced
     * past the last item in the container, returns #rwnil.
     */
    T* operator()() {
        return (++(*this)) ? key() : rwnil;
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    RWTPtrHashMultiSet<T, H, EQ, A>* container() const {
        return table_;
    }

    /**
     * Returns the value currently referenced by self. The return value
     * is undefined if self is not referencing an element within the multiset.
     */
    T* key() const {
        return *iter_;
    }

    /**
     * Resets the iterator so that after being advanced it references
     * the first element of the collection.
     */
    void reset() {
        head_ = true;
        tail_ = false;
    }

    /**
     * Resets the iterator so that after being advanced it references
     * the first element of the collection \a t.
     */
    void reset(RWTPtrHashMultiSet<T, H, EQ, A>& t) {
        head_ = true;
        tail_ = false;
        table_ = &t;
    }

private:

    RWTPtrHashMultiSet<T, H, EQ, A>* table_;
    typename RWTPtrHashMultiSet<T, H, EQ, A>::iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);

};


/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTPtrHashMultiSet::const_iterator to
 * iterate over the elements in this collection.
 *
 * @brief Deprecated. Provides a \c const iterator for RWTPtrHashMultiSet.
 *
 * RWTPtrHashMultiSetConstIterator provides a \c const iterator interface
 * to the C++ Standard Library-based collection represented by RWTPtrHashMultiSet.
 * This iterator prohibits changes to the collection that is being
 * iterated through.
 *
 * Iteration over an RWTPtrHashMultiSet is pseudo-random and dependent
 * on the capacity of the underlying hash table and the hash function
 * being used. The only usable relationship between consecutive
 * elements is that elements which are defined to be equivalent
 * by the equivalence object, \c EQ, will remain adjacent.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset() operation. The iterator
 * becomes valid after being advanced with either a pre-increment
 * or operator()().
 *
 * For both operator++() and operator()(), iterating past the last
 * element returns a value equivalent to boolean \c false. Continued
 * increments return a value equivalent to \c false until reset()
 * is called.
 *
 * The value type must have \c operator==() defined. This requirement
 * is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tphasht.h>
 * template <class T, class H, class EQ,
 *           class A=std::allocator<T*> >
 * class RWTPtrHashMultiSetConstIterator
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class T, class H = RWTHash<T>, class EQ = std::equal_to<T>, class A = std::allocator<T*> >
class RW_DEPRECATE_TYPE("Use RWTPtrHashMultiSet::const_iterator instead") RWTPtrHashMultiSetConstIterator
{

public:
    /**
     * Constructs a \c const iterator for the hashed multiset \a h. The
     * iterator begins in an undefined state and must be advanced before
     * the first element is accessible.
     */
    RWTPtrHashMultiSetConstIterator(const RWTPtrHashMultiSet<T, H, EQ, A>& h)
        : table_(&h), head_(true), tail_(false)
    { }

    /**
     * Constructs a \c const iterator from the iterator \a itr. The \c const
     * iterator begins in an undefined state and must be advanced before
     * the first element is accessible.
     */
    RWTPtrHashMultiSetConstIterator(const RWTPtrHashMultiSetIterator<T, H, EQ, A>& itr)
        : table_(itr.table_), iter_(itr.iter_),
          head_(itr.head_), tail_(itr.tail_)
    { }

    /**
     * Advanced self to the next element. If the iterator has been
     * reset or just created, self references the first element.
     * If used when self references the last association in
     * the multi-set, self references an undefined value and the method
     * returns a value equivalent to \c false. Otherwise, the method
     * returns a value equivalent to \c true.
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
     * iterator and returns its value. If the iterator has advanced
     * past the last item in the container, returns #rwnil.
     */
    const T* operator()() {
        return (++(*this)) ? key() : rwnil;
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    const RWTPtrHashMultiSet<T, H, EQ, A>* container() const {
        return table_;
    }

    /**
     * Returns the value currently referenced by self. The return value is
     * undefined if self is not referencing an association within the multiset.
     */
    const T* key() const {
        return *iter_;
    }

    /**
     * Resets the iterator so that after being advanced it references
     * the first element of the collection.
     */
    void reset() {
        head_ = true;
        tail_ = false;
    }

    /**
     * Resets the iterator so that after being advanced it references
     * the first element of the collection \a t.
     */
    void reset(const RWTPtrHashMultiSet<T, H, EQ, A>& t) {
        head_ = true;
        tail_ = false;
        table_ = &t;
    }

private:

    const RWTPtrHashMultiSet<T, H, EQ, A>* table_;
    typename RWTPtrHashMultiSet<T, H, EQ, A>::const_iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);

};
#endif

#  define RWTPtrHashTable RWTPtrHashMultiSet
#  define RWTPtrHashTableIterator RWTPtrHashMultiSetIterator
#  define RWTPtrHashTableConstIterator RWTPtrHashMultiSetConstIterator



#endif /* RW_CLASSIC_TEMPLATE_CONTAINERS */
#endif /* RW_TOOLS_TPHMSET_H */
