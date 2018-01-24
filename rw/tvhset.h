#ifndef RW_TOOLS_TVHSET_H
#define RW_TOOLS_TVHSET_H

/**********************************************************************
 *
 * tvhset.h - RWTValHashSet<T,Hash,Eq>
 *     : value-based hash-set wrapper for STL-like rw_hashset
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvhset.h#3 $
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
#  include <rw/xvhset.h>
#else
// STL-based implementation:

#  include <rw/edefs.h>
#  include <rw/epfunc.h>
#  include <rw/stdex/hashset.h>
#  include <rw/tools/algorithm.h>

#if !defined(RW_DISABLE_DEPRECATED)

template <class T, class H, class EQ, class A>
class RWTValHashSetConstIterator;

#endif

/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @brief Maintains a collection of values stored according to a hash object.
 *
 * This class maintains a collection of values, which are stored
 * according to a hash object of type \c H. Class \c T is the
 * type of items in the collection. Class \c A is an allocator of
 * objects of class \c T. For more information about using custom
 * allocators, please see the <i>Essential Tools Module User's Guide</i>.
 *
 * \c H must provide a \c const function that takes a single argument
 * convertible to type \c T and returns a value of type \c size_t.
 *
 * @note
 * Any two elements that are equivalent \e must hash to the same value.
 *
 * Element equality is determined by an equality function of type \c EQ,
 * which takes two arguments convertible to type \c T and returns a
 * value of type \c bool.
 *
 * @note
 * Any two elements that are equivalent are disallowed for this container.
 *
 * The value type must have \c operator==() defined. This requirement
 * is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvhset.h>
 * RWTValHashSet<T,H,EQ,A> s;
 * @endcode
 *
 * @section related Related Classes
 *
 * Class \link RWTValHashMultiSet RWTValHashMultiSet<T,H,EQ,A>\endlink offers
 * the same interface to a collection that does accept multiple items that compare
 * equal to each other.
 *
 * Class rw_hashset<T,H,EQ,A> is the C++ Standard Library collection
 * that serves as the underlying implementation for \link RWTValHashSet
 * RWTValHashSet<T,H,EQ,A>\endlink.
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
 * #include <rw/tvhset.h>
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
 *     RWTValHashSet<RWCString,simple_hash,std::equal_to<RWCString> > set1;
 *     RWTValHashSet<RWCString,simple_hash,std::equal_to<RWCString> > set2;
 *
 *     set1.insert("one");
 *     set1.insert("two");
 *     set1.insert("three");
 *
 *     // Rejected, no duplicates allowed
 *     set1.insert("one");
 *
 *     std::cout << set1.entries() << std::endl; // Prints "3"
 *
 *     set2.insert("one");
 *     set2.insert("five");
 *
 *     // Rejected, no duplicates allowed
 *     set2.insert("one");
 *
 *     std::cout << ((set1.isEquivalent(set2)) ? "true" : "false") << std::endl;
 *     // Prints "false"
 *
 *     set2.intersection(set1);
 *
 *     set1.clear();
 *     std::cout << set1.entries() << std::endl; // Prints "0"
 *     std::cout << set2.entries() << std::endl; // Prints "1"
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 * 3
 * false
 * 0
 * 1
 * @endcode
 */
template <class T, class H = RWTHash<T>, class EQ = std::equal_to<T>, class A = std::allocator<T> >
class RWTValHashSet
{
public:

    /**
     * A type representing the underlying implementation container.
     */
    typedef rw_hashset<T, H, EQ, A> container_type;

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
    typedef RWTValHashSet<T, H, EQ, A> this_type;


    /**
     * Returns a reference to the underlying collection that serves as
     * the implementation for self. This reference may be used freely,
     * providing access to the C++ Standard Library interface as well as
     * interoperability with other software components that make use of
     * C++ Standard Library collections.
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
     * Returns \c true if there exists an element \c t in self such that
     * the expression <tt>((*\a fn)(t,\a d))</tt> is \c true, otherwise
     * returns \c false. Client data may be passed through parameter \a d.
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
     * Returns the number of elements \c t in self that compare equal
     * to \a a.
     */
    size_type occurrencesOf(const_reference a) const {
        return std().count(a);
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
     * <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns the number
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
     * Returns \c true if self is a subset of \a s or if self is set
     * equivalent to \a s, otherwise returns \c false.
     */
    bool isSubsetOf(const RWTValHashSet<T, H, EQ, A>& s) const {
        if (entries() > s.entries()) {
            return false;
        }

        const_iterator itr = begin();
        while (itr != end() && s.contains(*itr)) {
            ++itr;
        }

        return itr == end();
    }

    /**
     * Returns \c true if self is a proper subset of \a s, otherwise returns
     * \c false.
     */
    bool isProperSubsetOf(const RWTValHashSet<T, H, EQ, A>& s) const {
        return ((entries() < s.entries()) && isSubsetOf(s));
    }

    /**
     * Returns \c true if self and \a s are identical, otherwise returns
     * \c false.
     */
    bool isEquivalent(const RWTValHashSet<T, H, EQ, A>& s) const {
        return ((entries() == s.entries()) && isSubsetOf(s));
    }

    /**
     * Destructively performs a set-theoretic union operation on self
     * and \a s. Self is replaced by the result. Note the use of the
     * uppercase \c 'U' in \c Union to avoid conflict with the C++
     * reserved word.
     */
    RWTValHashSet<T, H, EQ, A>&
    Union(const RWTValHashSet<T, H, EQ, A>& rhs) {
        return Union(rhs.std());
    }

    /**
     * @copydoc Union()
     */
    RWTValHashSet<T, H, EQ, A>&
    Union(const rw_hashset<T, H, EQ, A>& rhs);

    /**
     * Destructively performs a set-theoretic intersection of self and
     * \a rhs, replacing the contents of self with the result.
     */
    RWTValHashSet<T, H, EQ, A>&
    intersection(const RWTValHashSet<T, H, EQ, A>& rhs) {
        return intersection(rhs.std());
    }


    /**
     * @copydoc intersection()
     */
    RWTValHashSet<T, H, EQ, A>&
    intersection(const rw_hashset<T, H, EQ, A>& rhs);

    /**
     * Sets self to the set-theoretic difference given by <tt>(self - \a s)</tt>.
     */
    RWTValHashSet<T, H, EQ, A>&
    difference(const RWTValHashSet<T, H, EQ, A>& s) {
        return difference(s.std());
    }

    /**
     * @copydoc difference()
     */
    RWTValHashSet<T, H, EQ, A>&
    difference(const rw_hashset<T, H, EQ, A>& s);

    /**
     * Destructively performs a set-theoretic symmetric difference operation
     * on self and \a s. Self is replaced by the result. A symmetric
     * difference can be defined as @f$ (A \cup B) \setminus (A \cap B) @f$.
     */
    RWTValHashSet<T, H, EQ, A>&
    symmetricDifference(const RWTValHashSet<T, H, EQ, A>& rhs) {
        return symmetricDifference(rhs.std());
    }

    /**
     * @copydoc symmetricDifference()
     */
    RWTValHashSet<T, H, EQ, A>&
    symmetricDifference(const rw_hashset<T, H, EQ, A>& rhs);

    /**
     * Invokes the function pointer \a fn on each item in the collection.
     * Client data may be passed through parameter \a d.
     */
    void apply(void (*fn)(const_reference, void*), void* d) const {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * Adds the item \a a to the collection. Returns \c true if the
     * insertion is successful, otherwise returns \c false. The function
     * returns \c true unless the collection already holds an element
     * with the equivalent key.
     */
    bool insert(const_reference datum) {
        return (std().insert(datum)).second;
    }

    /**
     * Attempts to insert \a val into self. If an existing item with an
     * equivalent key is already in the collection, \a val is not inserted,
     * and an iterator to the existing item is returned. Otherwise inserts
     * \a val and returns an iterator to the new item.
     *
     * The \a hint parameter indicates where in the collection the new value
     * should be inserted. This value is only a hint, and will be ignored if
     * the value should be inserted at a different location according to the
     * container's hash object.
     */
    iterator insert(iterator hint, const_reference val) {
        RW_UNUSED(hint);
        return std().insert(val).first;
    }

    /**
     * Inserts the elements in the range [\a first, \a last) into self. If
     * the element is already present in the collection, it will be ignored.
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
        return (std().insert(rw_move(datum))).second;
    }

    /**
     * @copydoc insert(iterator, const_reference)
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    iterator insert(iterator hint, value_type && val) {
        RW_UNUSED(hint);
        return std().insert(rw_move(val)).first;
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Returns the number of buckets (slots) available in the underlying
     * hash representation. See resize().
     */
    size_type
    capacity() const {
        return std().capacity();
    }

    /**
     * Returns the ratio <tt>entries() / capacity()</tt>.
     */
    float
    fillRatio() const {
        return std().fill_ratio();
    }

    /**
     * Changes the capacity of self by creating a new hashed set with
     * a capacity of \a sz. resize() copies every element of self into
     * the new container and finally swaps the internal representation
     * of the new container with the internal representation of self.
     */
    void
    resize(size_type sz) {
        std().resize(sz);
    }

    /**
     * Swaps the data held by self with the data held by \a other, including
     * the \c H and \c EQ objects. This method does not copy or destroy
     * any of the items swapped; rather, it swaps the underlying hash
     * tables.
     */
    void
    swap(RWTValHashSet<T, H, EQ, A>& other) {
        std().swap(other.impl_);
    }

    /**
     * Destroys all elements of self and replaces them by copying all
     * elements of \a rhs.
     */
    RWTValHashSet<T, H, EQ, A>&
    operator=(const RWTValHashSet<T, H, EQ, A>& rhs) {
        return operator=(rhs.impl_);
    }

    /**
     * @copydoc operator=()
     */
    RWTValHashSet<T, H, EQ, A>&
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
    RWTValHashSet<T, H, EQ, A>&
    operator=(RWTValHashSet<T, H, EQ, A> && rhs) {
        return operator=(rw_move(rhs.impl_));
    }

    /**
     * @copydoc operator=(RWTValHashSet<T,H,EQ,A>&&)
     */
    RWTValHashSet<T, H, EQ, A>&
    operator=(container_type && rhs) {
        if (&impl_ != &rhs) {
            impl_ = rw_move(rhs);
        }
        return *this;
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs an empty hashed set. The underlying hash table representation
     * has \a sz buckets, uses \a h for its hashing function,
     * and uses \a eq to determine equality between elements.
     *
     * @note If the value specified for \a sz is zero, the default bucket
     * count is used.
     */
    RWTValHashSet(size_type sz = RW_DEFAULT_CAPACITY,
                  const hasher& h = hasher(), const key_equal& eq = key_equal())
        : impl_(sz ? sz : RW_DEFAULT_CAPACITY, h, eq) { }

    /**
     * Constructs a set by copying all elements of \a s.
     */
    RWTValHashSet(const container_type& s)
        : impl_(s) { }

    /**
     * Copy constructor.
     */
    RWTValHashSet(const RWTValHashSet<T, H, EQ, A>& rws)
        : impl_(rws.impl_) { }

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed list takes ownership of the
     * data owned by \a rws.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValHashSet(RWTValHashSet<T, H, EQ, A> && rws)
        : impl_(rw_move(rws.impl_)) { }

    /**
     * @copydoc RWTValHashSet(RWTValHashSet<T,H,EQ,A>&&)
     */
    RWTValHashSet(container_type && rws)
        : impl_(rw_move(rws)) { }
#  endif

    /**
     * Constructs a set by copying elements from the range
     * [\a first, \a last). The underlying hash table representation
     * has \a sz buckets, uses \a h for its hashing function, and uses
     * \a eq to determine equality between elements.
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     *
     * @note If the value specified for \a sz is zero, the default bucket
     * count is used.
     */
    template <typename InputIterator>
    RWTValHashSet(InputIterator first, InputIterator last,
                  size_type sz = RW_DEFAULT_CAPACITY,
                  const hasher& h = hasher(), const key_equal& eq = key_equal())
        : impl_(first, last, sz ? sz : RW_DEFAULT_CAPACITY, h, eq) { }

    // For backward compatibility
    /**
     * Creates an empty hashed set that uses the hash object \a h and
     * has an initial hash table capacity of \a sz.
     *
     * @note If the value specified for \a sz is zero, the default capacity
     * is used.
     */
    RWTValHashSet(const hasher& h, size_type sz = RW_DEFAULT_CAPACITY)
        : impl_(sz ? sz : RW_DEFAULT_CAPACITY, h) { }

private:
    container_type impl_;
};


/**
 * @relates RWTValHashSet
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and
 * iterating through both collections produces, in turn, individual elements
 * that compare equal to each other.
 */
template <class T, class H, class EQ , class A>
bool operator==(const RWTValHashSet<T, H, EQ , A>& lhs, const RWTValHashSet<T, H, EQ , A>& rhs)
{
    return lhs.std() == rhs.std();
}

/**
 * @relates RWTValHashSet
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T, class H, class EQ , class A>
bool operator!=(const RWTValHashSet<T, H, EQ , A>& lhs, const RWTValHashSet<T, H, EQ , A>& rhs)
{
    return lhs.std() != rhs.std();
}

/**
 * @relates RWTValHashSet
 *
 * @copydoc operator==(const RWTValHashSet&, const RWTValHashSet&);
 */
template <class T, class H, class EQ , class A>
bool operator==(const RWTValHashSet<T, H, EQ , A>& lhs,
                const typename RWTValHashSet<T, H, EQ , A>::container_type& rhs)
{
    return lhs.std() == rhs;
}

/**
 * @relates RWTValHashSet
 *
 * @copydoc operator!=(const RWTValHashSet&, const RWTValHashSet&);
 */
template <class T, class H, class EQ , class A>
bool operator!=(const RWTValHashSet<T, H, EQ , A>& lhs,
                const typename RWTValHashSet<T, H, EQ , A>::container_type& rhs)
{
    return lhs.std() != rhs;
}

/**
 * @relates RWTValHashSet
 *
 * @copydoc operator==(const RWTValHashSet&, const RWTValHashSet&);
 */
template <class T, class H, class EQ , class A>
bool operator==(const typename RWTValHashSet<T, H, EQ , A>::container_type& lhs,
                const RWTValHashSet<T, H, EQ , A>& rhs)
{
    return lhs == rhs.std();
}

/**
 * @relates RWTValHashSet
 *
 * @copydoc operator!=(const RWTValHashSet&, const RWTValHashSet&);
 */
template <class T, class H, class EQ , class A>
bool operator!=(const typename RWTValHashSet<T, H, EQ , A>::container_type& lhs,
                const RWTValHashSet<T, H, EQ , A>& rhs)
{
    return lhs != rhs.std();
}

/**
 * @relates RWTValHashSet
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T, class H, class EQ , class A>
RWvostream& operator<<(RWvostream& strm, const RWTValHashSet<T, H, EQ , A>& coll);

/**
 * @relates RWTValHashSet
 *
 * Saves the collection \a coll to the RWFile \a file,
 * or a reference to it if it has already been saved.
 */
template <class T, class H, class EQ , class A>
RWFile& operator<<(RWFile& file, const RWTValHashSet<T, H, EQ , A>& coll);

/**
 * @relates RWTValHashSet
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class T, class H, class EQ , class A>
RWvistream& operator>>(RWvistream& strm, RWTValHashSet<T, H, EQ , A>& coll);

/**
 * @relates RWTValHashSet
 *
 * Restores the contents of the collection \a coll from the RWFile \a file.
 */
template <class T, class H, class EQ , class A>
RWFile& operator>>(RWFile& file, RWTValHashSet<T, H, EQ , A>& coll);

/**
 * @relates RWTValHashSet
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, the application is
 * responsible for deleting it.
 */
template <class T, class H, class EQ , class A>
RWvistream& operator>>(RWvistream& strm, RWTValHashSet<T, H, EQ , A>*& p);

/**
 * @relates RWTValHashSet
 *
 * Looks at the next object on the stream represented by the RWFile \a file
 * and either creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, the application is
 * responsible for deleting it.
 */
template <class T, class H, class EQ , class A>
RWFile& operator>>(RWFile& file, RWTValHashSet<T, H, EQ , A>*& p);

#  if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#    pragma implementation ("tvhset.cc")
#  endif

#  ifdef RW_NO_IMPLICIT_INCLUSION
#    include "rw/tvhset.cc"
#  endif

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTValHashSet::iterator to iterate over
 * the elements in this collection.
 *
 * @brief Deprecated. Provides an iterator for RWTValHashSet.
 *
 * RWTValHashSetIterator provides an iterator interface to the
 * C++ Standard Library collection represented by RWTValHashSet.
 * (An STL iterator interface is provided through RWTValHashSet::iterator.)
 *
 * Iteration over an RWTValHashSet is pseudorandom and dependent
 * on the capacity of the underlying hash table and the hash function
 * being used.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset(). The iterator becomes
 * valid after being advanced with either a pre-increment or an
 * operator()().
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
 * #include <rw/tvhset.h>
 * RWTValHashSet<T,H,EQ,A> m;
 * RWTValHashSetIterator<T,H,EQ,A> itr(m);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/tvhset.h>
 * #include <rw/cstring.h>
 * #include <iostream>
 * #include <functional>
 *
 * int main()
 * {
 *    RWTValHashSet
 *      <RWCString, RWTHasher<RWCString>, std::equal_to<RWCString> >
 *        age(RWCString::hash);
 *    RWTValHashSetIterator
 *      <RWCString, RWTHasher<RWCString>, std::equal_to<RWCString > >
 *        itr(age);
 *
 *    age.insert("John");
 *    age.insert("Steve");
 *    age.insert("Mark");
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
 * @endcode
 */
template <class T, class H = RWTHash<T>, class EQ = std::equal_to<T>, class A = std::allocator<T> >
class RW_DEPRECATE_TYPE("Use RWTValHashSet::iterator instead") RWTValHashSetIterator
{

public:
    /**
     * Creates an iterator for the hashset \a h. The iterator begins
     * in an undefined state and must be advanced before the first element
     * is accessible.
     */
    RWTValHashSetIterator(RWTValHashSet<T, H, EQ, A>& h)
        : table_(&h), head_(true), tail_(false) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self will reference the first element.
     * If, before iteration, self referenced the last value in the
     * multiset, self will reference an undefined value and will
     * return \c false. Otherwise, it returns \c true.
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
    RWTValHashSet<T, H, EQ, A>* container() const {
        return table_;
    }

    /**
     * Returns the value currently pointed to by self.
     */
    T key() const {
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
     * the first element of the collection \a h.
     */
    void reset(RWTValHashSet<T, H, EQ, A>& h) {
        head_ = true;
        tail_ = false;
        table_ = &h;
    }

private:

    RWTValHashSet<T, H, EQ, A>* table_;
    typename RWTValHashSet<T, H, EQ, A>::iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);
    friend class RWTValHashSetConstIterator<T, H, EQ, A>;
};


/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTValHashSet::const_iterator to
 * iterate over the elements in this collection.
 *
 * @brief Deprecated. Provides a \c const iterator for RWTValHashSet.
 *
 * RWTValHashSetConstIterator provides a \c const iterator for RWTValHashSet.
 * A \c const iterator prohibits any changes to the underlying collection.
 *
 * Iteration over an RWTValHashSet is pseudorandom and dependent
 * on the capacity of the underlying hash table and the hash function
 * being used.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset(). The iterator becomes
 * valid after being advanced with either a pre-increment or an
 * operator()().
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
 * #include <rw/tvhset.h>
 * RWTValHashSet<T,H,EQ,A> m;
 * RWTValHashSetConstIterator<T,H,EQ,A> itr(m);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class T, class H = RWTHash<T>, class EQ = std::equal_to<T>, class A = std::allocator<T> >
class RW_DEPRECATE_TYPE("Use RWTValHashSet::const_iterator") RWTValHashSetConstIterator
{

public:
    /**
     * Creates a \c const iterator for the hashset \a h. The iterator begins
     * in an undefined state and must be advanced before the first element
     * is accessible.
     */
    RWTValHashSetConstIterator(const RWTValHashSet<T, H, EQ, A>& h)
        : table_(&h), head_(true), tail_(false) { }

    /**
     * Creates a \c const iterator from the iterator \a itr. The \c const
     * iterator begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTValHashSetConstIterator(const RWTValHashSetIterator<T, H, EQ, A>& itr)
        : table_(itr.table_), iter_(itr.iter_),
          head_(itr.head_), tail_(itr.tail_) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created self references the first element.
     * If, before iteration, self referenced the last value in the
     * multiset, self references an undefined value and returns \c false.
     * Otherwise, it returns \c true.
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
    const RWTValHashSet<T, H, EQ, A>* container() const {
        return table_;
    }

    /**
     * Returns the value currently pointed to by self.
     */
    const T& key() const {
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
     * the first element of the collection \a h.
     */
    void reset(const RWTValHashSet<T, H, EQ, A>& h) {
        head_ = true;
        tail_ = false;
        table_ = &h;
    }

private:

    const RWTValHashSet<T, H, EQ, A>* table_;
    typename RWTValHashSet<T, H, EQ, A>::const_iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);

};
#endif

#endif /* RW_CLASSIC_TEMPLATE_CONTAINERS */

#endif /* RW_TOOLS_TVHSET_H */
