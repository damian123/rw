#ifndef RW_TOOLS_TVHASHT_H
#define RW_TOOLS_TVHASHT_H

/**********************************************************************
 *
 * tvhasht.h - RWTValHashTable<T,Compare>
 *     : value-based multi hash set wrapper for STL-like rw_multihashset
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvhasht.h#4 $
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
#  include <rw/xvhasht.h>
#else
// STL-based implementation:

#  include <rw/epfunc.h>
#  include <rw/stdex/hashmset.h>
#  include <rw/tools/algorithm.h>

/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @brief Maintains a collection of values stored according to a hash object,
 * and with multiple equivalent values allowed.
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
 * Any two elements that are equivalent must hash to the same value.
 *
 * The \c value type must have \c operator==() defined. This requirement
 * is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvhasht.h>
 * RWTValHashMultiSet<T,H,EQ,A> m;
 * @endcode
 *
 * @section related Related Classes
 *
 * Class \link RWTValHashSet RWTValHashSet<T,H,EQ,A>\endlink offers the same
 * interface to a collection that does \b not accept multiple items that compare
 * equal to each other.
 *
 * Class rw_hashmultiset<T,H,EQ,A> is the C++ Standard Library collection
 * class that serves as the underlying implementation for
 * \link RWTValHashMultiSet RWTValHashMultiSet<T,H,EQ,A>\endlink.
 *
 * @section persistence Persistence
 *
 * Isomorphic
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <functional>
 *
 * #include <rw/tvhasht.h>
 * #include <rw/cstring.h>
 *
 * struct simple_hash
 * {
 *     unsigned long
 *     operator()(const RWCString& ref) const {
 *         size_t n = ref.length ();
 *         return (unsigned long)(n ? n * size_t (ref [0]) : 0);
 *     }
 * };
 *
 * int main ()
 * {
 *     RWTValHashMultiSet<RWCString,simple_hash,std::equal_to<RWCString> > set1;
 *     RWTValHashMultiSet<RWCString,simple_hash,std::equal_to<RWCString> > set2;
 *
 *     set1.insert("one");
 *     set1.insert("two");
 *     set1.insert("three");
 *     set1.insert("one");        // OK: duplicates allowed
 *     set1.insert("one");
 *
 *     std::cout << set1.entries() << std::endl; // Prints "5"
 *
 *     set2.insert("one");
 *     set2.insert("five");
 *     set2.insert("one");
 *
 *     std::cout << ((set1.isEquivalent(set2)) ? "true" : "false") << std::endl;
 *     // Prints "false"
 *
 *     set2.intersection(set1);
 *     set1.clear();
 *
 *     std::cout << set1.entries() << std::endl; // Prints "0"
 *     std::cout << set2.entries() << std::endl; // Prints "2"
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 * 5
 * false
 * 0
 * 2
 * @endcode
 */
template <class T, class H = RWTHash<T>, class EQ = std::equal_to<T>, class A = std::allocator<T> >
class RWTValHashMultiSet
{
public:

    /**
     * A type representing the underlying implementation container.
     */
    typedef rw_hashmultiset<T, H, EQ, A> container_type;

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

    /**
     * A type representing the hash function.
     */
    typedef typename container_type::hasher hasher;

    /**
     * A type representing the equality function.
     */
    typedef typename container_type::key_equal_type key_equal;


    /**
     * @internal
     * A type representing the same type as self.
     */
    typedef RWTValHashMultiSet<T, H, EQ, A> this_type;


    /**
     * Returns a reference to the underlying collection that serves as
     * the implementation for self. This reference may be used freely,
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
     * returns \c false.
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
     * Clears the collection by removing all items from self. Each
     * item has its destructor called.
     */
    void clear() {
        std().clear();
    }

    /**
     * Returns \c true if there exists an element \c t in self such that the
     * expression <tt>((*\a fn)(a,\a d))</tt> is \c true, otherwise returns
     * \c false. Client data may be passed through parameter \a d.
     */
    bool contains(bool(*fn)(const_reference, void*), void* d) const {
        return std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d)) != end();
    }

    /**
     * Returns \c true if there exists an element \c t in self that
     * compares equal to \a a, otherwise returns \c false.
     */
    bool contains(const_reference a) const {
        return std().find(a) != std().end();
    }

    /**
     * If there exists an element \c t in self that compares equal to \c a,
     * assigns \c t to \a k and returns \c true. Otherwise, returns \c false
     * and leaves the value of \a k unchanged. Client data may be passed
     * through parameter \a d.
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
        const_iterator iter = std().find(a);
        if (iter != end()) {
            k = *iter;
            return true;
        }
        return false;
    }

    /**
     * Returns the number of elements \c t in self such that the expression
     * <tt>((*\a fn)(a,\a d))</tt> is \c true. Client data may be passed
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
     * Removes the first element \c t in self such that the expression
     * <tt>((*\a fn)(a,\a d))</tt> is \c true and returns \c true. If there
     * is no such element, returns \c false. Client data may be passed
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
     * Removes the first element \c t in self that compares equal to
     * \a a and returns \c true. If there is no such element, returns \c false.
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
     * <tt>((*\a fn)(a,\a d))</tt> is \c true. Returns the number of
     * items removed. Client data may be passed through parameter \a d.
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
     * Destructively performs a set-theoretic union operation on self
     * and \a rhs. Self is replaced by the result. Note the uppercase
     * \c 'U' in \c Union to avoid conflict with the C++ reserved word.
     */
    void
    Union(const RWTValHashMultiSet<T, H, EQ, A>& rhs) {
        Union(rhs.std());
    }

    /**
     * Destructively performs a set-theoretic intersection of self and
     * \a s, replacing the contents of self with the result.
     */
    void
    intersection(const RWTValHashMultiSet<T, H, EQ, A>& s) {
        intersection(s.std());
    }

    /**
     * Sets self to the set-theoretic difference given by <tt>(self - \a s)</tt>.
     */
    void
    difference(const RWTValHashMultiSet<T, H, EQ, A>& s) {
        difference(s.std());
    }

    /**
     * Returns \c true if self is a subset of \a s, otherwise returns \c false.
     */
    bool
    isSubsetOf(const RWTValHashMultiSet<T, H, EQ, A>& s) const {
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
     * Returns \c true if self is a proper subset of \a s, otherwise
     * returns \c false.
     */
    bool
    isProperSubsetOf(const RWTValHashMultiSet<T, H, EQ, A>& s) const {
        return ((entries() < s.entries()) && isSubsetOf(s));
    }

    /**
     * Returns \c true if self and \a s are identical, otherwise returns
     * \c false.
     */
    bool
    isEquivalent(const RWTValHashMultiSet<T, H, EQ, A>& s) const {
        return ((entries() == s.entries()) && isSubsetOf(s));
    }

    /**
     * Destructively performs a set-theoretic symmetric difference
     * operation on self and \a s. Self is replaced by the result.
     * A symmetric difference can be informally defined as
     * @f$ (A \cup B) \setminus (A \cap B) @f$.
     */
    void
    symmetricDifference(const RWTValHashMultiSet<T, H, EQ, A>& s) {
        symmetricDifference(s.std());
    }

    /**
     * Invokes the function pointer \a fn on each item in the collection.
     * Client data may be passed through parameter \a d.
     */
    void apply(void (*fn)(const_reference, void*), void* d) const {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * Adds the item \a a to the collection. Returns \c true.
     */
    bool insert(const_reference datum) {
        std().insert(datum);
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
        std().insert(first, last);
    }

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @copydoc insert()
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    bool insert(value_type && datum) {
        std().insert(rw_move(datum));
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
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Returns the number of buckets (slots) available in the underlying
     * hash representation. See resize().
     */
    size_type capacity() const {
        return std().capacity();
    }

    /**
     * Returns the ratio <tt>entries() / capacity()</tt>.
     */
    float fillRatio() const {
        return std().fill_ratio();
    }

    /**
     * Changes the capacity of self by creating a new hashed multi-set
     * with a capacity of \a sz. resize() copies every element of self
     * into the new container and finally swaps the internal representation
     * of the new container with the internal representation of self.
     */
    void resize(size_type sz) {
        std().resize(sz);
    }

    /**
     * Swaps the data held by self with the data held by \a other, including
     * the \c H and \c EQ objects. This method does not copy or destroy
     * any of the items swapped; rather, it swaps the underlying hash
     * tables.
     */
    void
    swap(RWTValHashMultiSet<T, H, EQ, A>& other) {
        std().swap(other.impl_);
    }

    /**
     * Destroys all elements of self and replaces them by copying all
     * elements of \a rhs.
     */
    RWTValHashMultiSet<T, H, EQ, A>&
    operator=(const RWTValHashMultiSet<T, H, EQ, A>& rhs) {
        return operator=(rhs.impl_);
    }

    /**
     * @copydoc operator=()
     */
    RWTValHashMultiSet<T, H, EQ, A>&
    operator=(const rw_hashmultiset<T, H, EQ, A>& rhs) {
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
    RWTValHashMultiSet<T, H, EQ, A>&
    operator=(RWTValHashMultiSet<T, H, EQ, A> && rhs) {
        return operator=(rw_move(rhs.impl_));
    }

    /**
     * @copydoc operator=(RWTValHashMultiSet<T,H,EQ,A>&&)
     */
    RWTValHashMultiSet<T, H, EQ, A>&
    operator=(container_type && rhs) {
        if (&impl_ != &rhs) {
            impl_ = rw_move(rhs);
        }
        return *this;
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs an empty set. The underlying hash table representation
     * has \a sz buckets, uses \a h as its hashing function, and uses
     * \a eq to determine equivalence between elements.
     *
     * @note If the value specified for \a sz is zero, the default bucket
     * count is used.
     */
    RWTValHashMultiSet(size_type sz = RW_DEFAULT_CAPACITY,
                       const hasher& h = hasher(), const key_equal& eq = key_equal())
        : impl_(sz ? sz : RW_DEFAULT_CAPACITY, h, eq) { }

    /**
     * Constructs a set by copying all elements of \a s.
     */
    RWTValHashMultiSet(const rw_hashmultiset<T, H, EQ, A>& s)
        : impl_(s) { }

    /**
     * Copy constructor.
     */
    RWTValHashMultiSet(const RWTValHashMultiSet<T, H, EQ, A>& rws)
        : impl_(rws.impl_) { }

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed list takes ownership of the
     * data owned by \a rws.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValHashMultiSet(RWTValHashMultiSet<T, H, EQ, A> && rws)
        : impl_(rw_move(rws.impl_)) { }

    /**
     * @copydoc RWTValHashMultiSet(RWTValHashMultiSet<T,H,EQ,A>&&)
     */
    RWTValHashMultiSet(container_type && rws)
        : impl_(rw_move(rws)) { }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs a set by copying elements from the range
     * [\a first, \a last). The underlying hash table representation
     * has \a sz buckets, uses \a h as its hashing function, and uses
     * \a eq to determine equivalence between elements.
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     *
     * @note If the value specified for \a sz is zero, the default bucket
     * count is used.
     */
    template <typename InputIterator>
    RWTValHashMultiSet(InputIterator first, InputIterator last,
                       size_type sz = RW_DEFAULT_CAPACITY,
                       const hasher& h = hasher(), const key_equal& eq = key_equal())
        : impl_(first, last, sz ? sz : RW_DEFAULT_CAPACITY, h, eq) { }

    // backward compatible constructor:
    /**
     * Creates an empty hashed multi-set that uses the hash object
     * \a h and has an initial hash table capacity of \a sz.
     *
     * @note If the value specified for \a sz is zero, the default capacity
     * is used.
     */
    RWTValHashMultiSet(const hasher& h, size_type sz = RW_DEFAULT_CAPACITY)
        : impl_(sz ? sz : RW_DEFAULT_CAPACITY, h) { }

private:
    void
    difference(const container_type& s);

    void
    intersection(const container_type& s);

    void
    Union(const container_type& rhs);

    void
    symmetricDifference(const container_type& s);

    container_type impl_;
};


/**
 * @relates RWTValHashMultiSet
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and
 * iterating through both collections produces, in turn, individual elements
 * that compare equal to each other.
 */
template <class T, class H, class EQ, class A>
bool operator==(const RWTValHashMultiSet<T, H, EQ, A>& lhs, const RWTValHashMultiSet<T, H, EQ, A>& rhs)
{
    return lhs.std() == rhs.std();
}

/**
 * @relates RWTValHashMultiSet
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T, class H, class EQ, class A>
bool operator!=(const RWTValHashMultiSet<T, H, EQ, A>& lhs, const RWTValHashMultiSet<T, H, EQ, A>& rhs)
{
    return lhs.std() != rhs.std();
}

/**
 * @relates RWTValHashMultiSet
 *
 * @copydoc operator==(const RWTValHashMultiSet&, const RWTValHashMultiSet&);
 */
template <class T, class H, class EQ, class A>
bool operator==(const RWTValHashMultiSet<T, H, EQ, A>& lhs,
                const typename RWTValHashMultiSet<T, H, EQ, A>::container_type& rhs)
{
    return lhs.std() == rhs;
}

/**
 * @relates RWTValHashMultiSet
 *
 * @copydoc operator!=(const RWTValHashMultiSet&, const RWTValHashMultiSet&);
 */
template <class T, class H, class EQ, class A>
bool operator!=(const RWTValHashMultiSet<T, H, EQ, A>& lhs,
                const typename RWTValHashMultiSet<T, H, EQ, A>::container_type& rhs)
{
    return lhs.std() != rhs;
}

/**
 * @relates RWTValHashMultiSet
 *
 * @copydoc operator==(const RWTValHashMultiSet&, const RWTValHashMultiSet&);
 */
template <class T, class H, class EQ, class A>
bool operator==(const typename RWTValHashMultiSet<T, H, EQ, A>::container_type& lhs,
                const RWTValHashMultiSet<T, H, EQ, A>& rhs)
{
    return lhs == rhs.std();
}

/**
 * @relates RWTValHashMultiSet
 *
 * @copydoc operator!=(const RWTValHashMultiSet&, const RWTValHashMultiSet&);
 */
template <class T, class H, class EQ, class A>
bool operator!=(const typename RWTValHashMultiSet<T, H, EQ, A>::container_type& lhs,
                const RWTValHashMultiSet<T, H, EQ, A>& rhs)
{
    return lhs != rhs.std();
}

/**
 * @relates RWTValHashMultiSet
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T, class H, class EQ, class A>
RWvostream& operator<<(RWvostream& strm, const RWTValHashMultiSet<T, H, EQ, A>& coll);

/**
 * @relates RWTValHashMultiSet
 *
 * Saves the collection \a coll to the RWFile \a file, or a reference to
 * the file if the collection has already been saved.
 */
template <class T, class H, class EQ, class A>
RWFile& operator<<(RWFile& file, const RWTValHashMultiSet<T, H, EQ, A>& coll);

/**
 * @relates RWTValHashMultiSet
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class T, class H, class EQ, class A>
RWvistream& operator>>(RWvistream& strm, RWTValHashMultiSet<T, H, EQ, A>& coll);

/**
 * @relates RWTValHashMultiSet
 *
 * Restores the contents of the collection \a coll from the RWFile \a file.
 */
template <class T, class H, class EQ, class A>
RWFile& operator>>(RWFile& file, RWTValHashMultiSet<T, H, EQ, A>& coll);

/**
 * @relates RWTValHashMultiSet
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, the application is
 * responsible for deleting it.
 */
template <class T, class H, class EQ, class A>
RWvistream& operator>>(RWvistream& strm, RWTValHashMultiSet<T, H, EQ, A>*& p);

/**
 * @relates RWTValHashMultiSet
 *
 * Looks at the next object on the stream represented by RWFile \a file
 * and either creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, the application is
 * responsible for deleting it.
 */
template <class T, class H, class EQ, class A>
RWFile& operator>>(RWFile& file, RWTValHashMultiSet<T, H, EQ, A>*& p);

#  if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#    pragma implementation ("tvhasht.cc")
#  endif

#  ifdef RW_NO_IMPLICIT_INCLUSION
#    include "rw/tvhasht.cc"
#  endif

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTValHashMultiSet::iterator to iterate
 * over the elements in this collection.
 *
 * @brief Deprecated. Provides an iterator for RWTValHashMultiSet.
 *
 * RWTValHashMultiSetIterator provides an iterator interface to
 * the C++ Standard Library-based collection represented by RWTValHashMultiSet.
 * (An STL iterator interface is provided through RWTValHashMultiSet::iterator.)
 *
 * Iteration over an RWTValHashMultiSet is pseudorandom and dependent
 * on the capacity of the underlying hash table and the hash function
 * being used. The only usable relationship between consecutive elements is
 * that elements defined to be equivalent by the equivalence object, \c EQ,
 * will be adjacent.
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
 * The value type must have \c operator== defined. This requirement
 * is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvhasht.h>
 * RWTValHashMultiSet<T,H,EQ,A> m;
 * RWTValHashMultiSetIterator<T,H,EQ,A> itr(m);
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
 * John
 * Steve
 * Mark
 * Steve
 * @endcode
 */
template <class T, class H = RWTHash<T>, class EQ = std::equal_to<T>, class A = std::allocator<T> >
class RW_DEPRECATE_TYPE("Use RWTValHashMultiSet::iterator instead") RWTValHashMultiSetIterator
{

public:
    /**
     * Creates an iterator for the hashed multiset \a h. The iterator
     * begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTValHashMultiSetIterator(RWTValHashMultiSet<T, H, EQ, A>& h)
        : table_(&h), head_(true), tail_(false) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self now references the first element.
     * If, before iteration, self referenced the last value in the
     * multiset, self now references an undefined value and the operator
     * returns \c false, Otherwise, it returns \c true.
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
     * Advances self to the next element. Returns \c false if the iterator
     * has advanced past the last element in the container, otherwise returns
     * \c true.
     */
    bool operator()() {
        return ++(*this);
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    RWTValHashMultiSet<T, H, EQ, A>* container() const {
        return table_;
    }

    /**
     * Returns the value currently referenced by self.
     */
    T key() const {
        return *iter_;
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
     * the first element of the collection \a h.
     */
    void reset(RWTValHashMultiSet<T, H, EQ, A>& h) {
        head_ = true;
        tail_ = false;
        table_ = &h;
    }

private:

    RWTValHashMultiSet<T, H, EQ, A>* table_;
    typename RWTValHashMultiSet<T, H, EQ, A>::iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);

};


/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTValHashMultiSet::const_iterator to
 * iterate over the elements in this collection.
 *
 * @brief Deprecated. Provides a \c const iterator for RWTValHashMultiSet.
 *
 * RWTValHashMultiSetConstIterator provides a \c const iterator interface
 * to the C++ Standard Library-based collection represented by RWTValHashMultiSet.
 * A \c const iterator prohibits any changes to the underlying collection.
 *
 * Iteration over an RWTValHashMultiSet is pseudorandom and dependent
 * on the capacity of the underlying hash table and the hash function
 * being used. The only usable relationship between consecutive
 * elements is that elements defined to be equivalent by the equivalence
 * object, \c EQ, will be adjacent.
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
 * The value type must have \c operator== defined. This requirement
 * is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvhasht.h>
 * RWTValHashMultiSet<T,H,EQ,A> m;
 * RWTValHashMultiSetConstIterator<T,H,EQ,A> itr(m);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class T, class H = RWTHash<T>, class EQ = std::equal_to<T>, class A = std::allocator<T> >
class RW_DEPRECATE_TYPE("Use RWTValHashMultiSet::const_iterator instead") RWTValHashMultiSetConstIterator
{

public:
    /**
     * Creates a \c const iterator for the hashed multiset \a h. The iterator
     * begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTValHashMultiSetConstIterator(const RWTValHashMultiSet<T, H, EQ, A>& h)
        : table_(&h), head_(true), tail_(false) { }

    /**
     * Creates a \c const iterator from the iterator \a itr. The \c const
     * iterator begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTValHashMultiSetConstIterator(const RWTValHashMultiSetIterator<T, H, EQ, A>& itr)
        : table_(itr.table_), iter_(itr.iter_),
          head_(itr.head_), tail_(itr.tail_) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self now references the first element.
     * If, before iteration, self referenced the last value in the
     * multiset, self now references an undefined value and returns
     * \c false, otherwise, returns \c true.
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
     * Advances self to the next element. Returns \c false if the
     * iterator has advanced past the last item in the container,
     * otherwise returns \c true.
     */
    bool operator()() {
        return ++(*this);
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    const RWTValHashMultiSet<T, H, EQ, A>* container() const {
        return table_;
    }

    /**
     * Returns the value currently referenced by self.
     */
    const T& key() const {
        return *iter_;
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
     * the first element of the collection \a h.
     */
    void reset(const RWTValHashMultiSet<T, H, EQ, A>& h) {
        head_ = true;
        tail_ = false;
        table_ = &h;
    }

private:

    const RWTValHashMultiSet<T, H, EQ, A>* table_;
    typename RWTValHashMultiSet<T, H, EQ, A>::const_iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);

};
#endif

#  define RWTValHashTable RWTValHashMultiSet
#  define RWTValHashTableIterator RWTValHashMultiSetIterator
#  define RWTValHashTableConstIterator RWTValHashMultiSetConstIterator

#endif /* RW_CLASSIC_TEMPLATE_CONTAINERS */

#endif /* RW_TOOLS_TVHASHT_H */
