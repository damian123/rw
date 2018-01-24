#ifndef RW_TOOLS_TPHSET_H
#define RW_TOOLS_TPHSET_H

/**********************************************************************
 *
 * tphset.h - RWTPtrHashSet<T,H,EQ>
 *     : pointer-based set wrapper for STL-like rw_hashset
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tphset.h#3 $
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
#  include <rw/xphset.h>
#else
#  include <rw/edefs.h> // for rw_move
#  include <rw/epaux.h>
#  include <rw/epfunc.h>
#  include <rw/stdex/hashset.h>
#  include <rw/tools/algorithm.h>

#if !defined(RW_DISABLE_DEPRECATED)

template <class T, class H, class EQ, class A>
class RWTPtrHashSetConstIterator;

#endif

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief A hash-based container for pointer types.
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
 * \link RWTPtrHashSet RWTPtrHashSet<T,H,EQ,A>\endlink will not accept an item
 * that is equivalent to an item already in the collection.
 * (\link RWTPtrHashMultiSet RWTPtrHashMultiSet<T,H,EQ,A>\endlink
 * may contain multiple items that are equivalent to each other.)
 * Equality is based on the equality object and not on the equality
 * operator.
 *
 * The \c value type must have operator==() defined. This requirement
 * is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tphset.h>
 * RWTPtrHashSet<T,H,EQ,A> s;
 * @endcode
 *
 * @section related Related Classes
 *
 * Class \link RWTPtrHashMultiSet RWTPtrHashMultiSet<T,H,EQ,A>\endlink offers
 * the same interface to a pointer-based collection that accepts multiple items
 * that compare equal to each other.
 *
 * Class
 * \link rw_hashset rw_hashset<T*,hasher,key_equal,A>\endlink
 * is the C++ Standard Library collection that serves as the underlying
 * implementation for \link RWTPtrHashSet RWTPtrHashSet<T,H,EQ,A>\endlink.
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
 * #include <rw/tphset.h>
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
 * void printit (const RWCString* s, void*)
 * {
 *     std::cout << *s << " ";
 * }
 *
 * int main()
 * {
 *     RWTPtrHashSet<RWCString,simple_hash, std::equal_to <RWCString> > myset;
 *     RWCString s;
 *
 *     myset.insert(new RWCString("one"));
 *     myset.insert(new RWCString("two"));
 *     myset.insert(new RWCString("three"));
 *
 *     RWCString* entry = new RWCString("one");
 *     if (!myset.insert(entry)) // Rejected: duplicate entry
 *     {
 *       delete entry;
 *     }
 *
 *     std::cout << "There are " << myset.entries()
 *               << " items in the hash set and they are: \n"; // Prints "3"
 *
 *
 *     myset.apply(printit, NULL);
 *
 *     std::cout << std::endl;
 *
 *     myset.clearAndDestroy();
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 *
 * There are 3 items in the hash set and they are:
 * one two three
 *
 * @endcode
 *
 */
template <class T, class H = RWTHash<T>, class EQ = std::equal_to<T>, class A = std::allocator<T*> >
class RWTPtrHashSet
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
    typedef rw_hashset < T*, hasher, key_equal, A> container_type;

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
     * \link RWTPtrHashSet::key_equal \c key_equal \endlink instead.
     *
     * A type representing an equality function that dereferences the
     * arguments before comparing.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrHashSet::key_equal instead") rw_deref_compare<EQ, T> container_eq;

    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrHashSet::hasher \c hasher \endlink instead.
     *
     * A type representing a hash function that dereferences the arguments
     * before comparing.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrHashSet::hasher instead") rw_deref_hash<H, T> container_hash;
#endif


    /**
     * @internal
     * A type representing the same type as self.
     */
    typedef RWTPtrHashSet<T, H, EQ, A> this_type;

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
     * Removes all items from the collection and uses <tt>operator delete</tt>
     * to destroy the objects pointed to by those items. Do
     * not use this method if multiple pointers to the same object are
     * stored. (If the equality operator is reflexive, the container
     * cannot hold such multiple pointers.)
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
    bool contains(bool(*fn)(const T* , void*), void* d) const {
        return std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d)) != end();
    }

    /**
     * Returns \c true if there exists an element \c t in self such that
     * the expression <tt>(*t == *\a a)</tt> is \c true, otherwise returns
     * \c false.
     */
    bool contains(const T* a) const {
        return std().find((value_type)a) != std().end();
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
     * If there exists an element \c t in self such that <tt>*\a a</tt>
     * compares equal to <tt>*\a a</tt>, returns \c t. Otherwise, returns #rwnil.
     */
    value_type find(const T* key) {
        iterator iter = std().find((value_type)key);
        return (iter != end()) ? *iter : rwnil;
    }

    /**
     * @copydoc find(const T*)
     */
    const T* find(const T* a) const {
        const_iterator iter = std().find((value_type)a);
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
     * Returns the number of elements \c t that compare equal to <tt>*\a a</tt>
     */
    size_type occurrencesOf(const T* a) const {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_if(begin(), end(), rw_bind2nd(deref_equal(), a));
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
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
     * Returns \c true if self is a subset of \a s or if self is set
     * equivalent to \a s, otherwise \c false.
     */
    bool isSubsetOf(const RWTPtrHashSet<T, H, EQ, A>& s) const {
        if (entries() > s.entries()) {
            return false;
        }

        const_iterator itr = begin();
        while (itr != end() && s.contains(*itr)) {
            ++itr;
        }

        return end() == itr;
    }

    /**
     * Returns \c true if self is a proper subset of \a s, otherwise
     * \c false.
     */
    bool isProperSubsetOf(const RWTPtrHashSet<T, H, EQ, A>& s) const {
        return ((entries() < s.entries()) && isSubsetOf(s));
    }

    /**
     * Returns \c true if self and \a s are identical, otherwise returns
     * \c false.
     */
    bool isEquivalent(const RWTPtrHashSet<T, H, EQ, A>& s) const {
        return ((entries() == s.entries()) && isSubsetOf(s));
    }

    // selective inheritance (hides other same-name components)

    /**
     * Invokes the function pointer \a fn on each item in the collection.
     * Client data may be passed through parameter \a d.
     */
    void apply(void (*fn)(const T&, void*), void* d) const {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }


    /**
     * @copydoc apply()
     */
    void apply(void (*fn)(const T*, void*), void* d) const {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * Destructively performs a set theoretic union operation on self
     * and \a s. Self is replaced by the result.
     * @note
     * The uppercase "U" in \c Union avoids conflict with the C++ reserved word.
     */
    RWTPtrHashSet<T, H, EQ, A>&
    Union(const RWTPtrHashSet<T, H, EQ, A>& s) {
        Union(s.std());
        return *this;
    }

    /**
     * Destructively performs a set theoretic intersection of self and
     * \a s, replacing the contents of self with the result.
     */
    RWTPtrHashSet<T, H, EQ, A>&
    intersection(const RWTPtrHashSet<T, H, EQ, A>& s) {
        intersection(s.std());
        return *this;
    }

    /**
     * Sets self to the set-theoretic difference given by (self - \a s).
     * Elements from each set are dereferenced before being compared.
     */
    RWTPtrHashSet<T, H, EQ, A>&
    difference(const RWTPtrHashSet<T, H, EQ, A>& s) {
        difference(s.std());
        return *this;
    }

    /**
     * Destructively performs a set theoretic symmetric difference operation
     * on self and \a s. Self is replaced by the result. A symmetric
     * difference can be defined as @f$ (A \cup B) / (A \cap B) @f$.
     */
    RWTPtrHashSet<T, H, EQ, A>&
    symmetricDifference(const RWTPtrHashSet<T, H, EQ, A>& s) {
        symmetricDifference(s.std());
        return *this;
    }

    /**
     * Adds the item \a a to the collection. Returns \c true if the
     * insertion is successful, otherwise returns \c false. Insertion will
     * fail if the collection already holds an element with an equivalent key.
     */
    bool insert(value_type a) {
        return (std().insert(a)).second;
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
    iterator insert(iterator hint, value_type val) {
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

    /**
     * Returns the number of buckets (slots) available in the underlying
     * hash representation. See resize().
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
     * Changes the capacity of self by creating a new hashed set with
     * a capacity of \a sz. This function copies every element of self
     * into the new container and finally swaps the internal representation
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
    swap(RWTPtrHashSet<T, H, EQ, A>& other) {
        std().swap(other.impl_);
    }

    /**
     * Clears all elements of self and replaces them by copying all
     * elements of \a rhs.
     */
    RWTPtrHashSet<T, H, EQ, A>&
    operator=(const RWTPtrHashSet<T, H, EQ, A>& rhs) {
        return operator=(rhs.impl_);
    }

    /**
     * @copydoc operator=()
     */
    RWTPtrHashSet<T, H, EQ, A>&
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
    RWTPtrHashSet<T, H, EQ, A>&
    operator=(RWTPtrHashSet<T, H, EQ, A> && rhs) {
        return operator=(rw_move(rhs.impl_));
    }

    /**
     * @copydoc operator=(RWTPtrHashSet<T,H,EQ,A>&&)
     */
    RWTPtrHashSet<T, H, EQ, A>&
    operator=(container_type && rhs) {
        if (&impl_ != &rhs) {
            impl_ = rw_move(rhs);
        }
        return *this;
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    // constructors

    /**
     * Constructs an empty hashed set. The underlying hash table representation
     * has \a sz buckets, uses \a h for its hashing function,
     * and uses \a eq to determine equality between elements.
     *
     * @note If the value specified for \a sz is zero, the default bucket
     * count is used.
     */
    RWTPtrHashSet(size_type sz = RW_DEFAULT_CAPACITY,
                  const H& h = H(), const EQ& eq = EQ())
        : impl_(sz ? sz : RW_DEFAULT_CAPACITY, h, eq) { }

    /**
     * Copy constructor.
     */
    RWTPtrHashSet(const RWTPtrHashSet<T, H, EQ, A>& rws) : impl_(rws.impl_) { }

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed list takes ownership of the
     * data owned by \a rws.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrHashSet(RWTPtrHashSet<T, H, EQ, A> && rws)
        : impl_(rw_move(rws.impl_)) { }

    /**
     * @copydoc RWTPtrHashSet(RWTPtrHashSet<T,H,EQ,A> &&)
     */
    RWTPtrHashSet(container_type && rws) : impl_(rw_move(rws)) { }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs a pointer-based hash set by copying all elements from
     * \a s.
     */
    RWTPtrHashSet(const container_type& s) : impl_(s) { }

    /**
     * Constructs a set by copying elements from the range
     * [\a first, \a last). The underlying hash table representation
     * has \a sz buckets, uses \a h for its hashing function,
     * and uses \a eq to determine equality between elements.
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     *
     * @note If the value specified for \a sz is zero, the default bucket
     * count is used.
     */
    template <typename InputIterator>
    RWTPtrHashSet(InputIterator first, InputIterator last,
                  size_type sz = RW_DEFAULT_CAPACITY,
                  const H& h = H(), const EQ& eq = EQ())
        : impl_(first, last, sz ? sz : RW_DEFAULT_CAPACITY, h, eq) { }

    // backward compatible constructor:
    /**
     * Creates an empty hashed set which uses the hash object \a h and
     * has an initial hash table capacity of \a sz.
     *
     * @note If the value specified for \a sz is zero, the default capacity
     * is used.
     */
    RWTPtrHashSet(const H& h, size_type sz = RW_DEFAULT_CAPACITY)
        : impl_(sz ? sz : RW_DEFAULT_CAPACITY, h) { }

private:
    void Union(const container_type&);
    void intersection(const container_type& s);
    void difference(const container_type& s);
    void symmetricDifference(const container_type& s);

    container_type impl_;
};


/**
 * @relates RWTPtrHashSet
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and each
 * is a subset of the other. Elements are dereferenced before being compared.
 */
template <class T, class H, class EQ , class A>
bool operator==(const RWTPtrHashSet<T, H, EQ, A>& lhs, const RWTPtrHashSet<T, H, EQ, A>& rhs)
{
    return lhs.isEquivalent(rhs);
}

/**
 * @relates RWTPtrHashSet
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T, class H, class EQ , class A>
bool operator!=(const RWTPtrHashSet<T, H, EQ, A>& lhs, const RWTPtrHashSet<T, H, EQ, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrHashSet
 *
 * @copydoc operator==(const RWTPtrHashSet&, const RWTPtrHashSet&);
 */
template <class T, class H, class EQ , class A>
bool operator==(const RWTPtrHashSet<T, H, EQ, A>& lhs,
                const typename RWTPtrHashSet<T, H, EQ, A>::container_type& rhs)
{
    return lhs.isEquivalent(rhs);
}

/**
 * @relates RWTPtrHashSet
 *
 * @copydoc operator!=(const RWTPtrHashSet&, const RWTPtrHashSet&);
 */
template <class T, class H, class EQ , class A>
bool operator!=(const RWTPtrHashSet<T, H, EQ, A>& lhs,
                const typename RWTPtrHashSet<T, H, EQ, A>::container_type& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrHashSet
 *
 * @copydoc operator==(const RWTPtrHashSet&, const RWTPtrHashSet&);
 */
template <class T, class H, class EQ , class A>
bool operator==(const typename RWTPtrHashSet<T, H, EQ, A>::container_type& lhs,
                const RWTPtrHashSet<T, H, EQ, A>& rhs)
{
    return rhs.isEquivalent(lhs);
}

/**
 * @relates RWTPtrHashSet
 *
 * @copydoc operator!=(const RWTPtrHashSet&, const RWTPtrHashSet&);
 */
template <class T, class H, class EQ , class A>
bool operator!=(const typename RWTPtrHashSet<T, H, EQ, A>::container_type& lhs,
                const RWTPtrHashSet<T, H, EQ, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrHashSet
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T, class H, class EQ , class A>
RWvostream& operator<<(RWvostream& strm, const RWTPtrHashSet<T, H, EQ , A>& coll);

/**
 * @relates RWTPtrHashSet
 *
 * Saves the collection \a coll onto the RWFile \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T, class H, class EQ , class A>
RWFile& operator<<(RWFile& strm, const RWTPtrHashSet<T, H, EQ , A>& coll);

/**
 * @relates RWTPtrHashSet
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class T, class H, class EQ , class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrHashSet<T, H, EQ , A>& coll);

/**
 * @relates RWTPtrHashSet
 *
 * Restores the contents of the collection \a coll from the RWFile \a strm.
 */
template <class T, class H, class EQ , class A>
RWFile& operator>>(RWFile& strm, RWTPtrHashSet<T, H, EQ , A>& coll);

/**
 * @relates RWTPtrHashSet
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T, class H, class EQ , class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrHashSet<T, H, EQ , A>*& p);

/**
 * @relates RWTPtrHashSet
 *
 * Looks at the next object on the RWFile \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T, class H, class EQ , class A>
RWFile& operator>>(RWFile& strm, RWTPtrHashSet<T, H, EQ , A>*& p);

#  if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#    pragma implementation ("tphset.cc")
#  endif

#  ifdef RW_NO_IMPLICIT_INCLUSION
#    include "rw/tphset.cc"
#  endif

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTPtrHashSet::iterator to iterate over
 * the elements in this collection.
 *
 * @brief Deprecated. Provides an iterator for RWTPtrHashSet.
 *
 * RWTPtrHashSetIterator provides an iterator interface to the
 * C++ Standard Library-based collection represented by RWTPtrHashSet. (An STL
 * iterator interface is provided through \c RWTPtrHashSet::iterator.)
 *
 * Iteration over an RWTPtrHashSet is pseudorandom and dependent
 * on the capacity of the underlying hash table and the hash function
 * being used.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset(). The iterator becomes
 * valid after being advanced with either a pre-increment or an
 * operator()().
 *
 * For both operator++() and operator()(), iterating past the last
 * element will return a value equivalent to boolean \c false.
 * Continued increments will return a value equivalent to \c false
 * until reset() is called.
 *
 * The \c value type must have \c operator== defined. This requirement
 * is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include<rw/tphset.h>
 * RWTPtrHashSet<T,H,EQ,A> m;
 * RWTPtrHashSet<T,H,EQ,A> itr(m);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/tphset.h>
 * #include <iostream>
 * #include <rw/cstring.h>
 *
 * int main() {
 *    RWTPtrHashSet
 *      <RWCString,RWTHasher<RWCString>,std::equal_to<RWCString> >
 *        age(RWCString::hash);
 *    RWTPtrHashSetIterator
 *      <RWCString,RWTHasher<RWCString>,std::equal_to<RWCString> >
 *        itr(age);
 *
 *    age.insert(new RWCString("John"));
 *    age.insert(new RWCString("Steve"));
 *    age.insert(new RWCString("Mark"));
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
 *
 * John
 * Mark
 * Steve
 * @endcode
 *
 */
template <class T, class H = RWTHash<T>, class EQ = std::equal_to<T>, class A = std::allocator<T*> >
class RW_DEPRECATE_TYPE("Use RWTPtrHashSet::iterator instead") RWTPtrHashSetIterator
{

public:
    /**
     * Creates an iterator for the hashed set \a h. The iterator begins
     * in an undefined state and must be advanced before the first element
     * will be accessible.
     */
    RWTPtrHashSetIterator(RWTPtrHashSet<T, H, EQ, A>& h)
        : table_(&h), head_(true), tail_(false) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self will now reference the first element.
     * If, before iteration, self referenced the last association in
     * the multi-map, self will now point to an undefined value and
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
     * iterator and returns its value. If the iterator has advanced
     * past the last item in the container, #rwnil will be returned.
     */
    T* operator()() {
        return (++(*this)) ? key() : rwnil;
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    RWTPtrHashSet<T, H, EQ, A>* container() const {
        return table_;
    }

    /**
     * Returns the element referenced by self. Undefined if self is
     * not referencing a value within the set.
     */
    T* key() const {
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
    void reset(RWTPtrHashSet<T, H, EQ, A>& t) {
        head_ = true;
        tail_ = false;
        table_ = &t;
    }

private:

    RWTPtrHashSet<T, H, EQ, A>* table_;
    typename RWTPtrHashSet<T, H, EQ, A>::iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);

    friend class RWTPtrHashSetConstIterator<T, H, EQ, A>;
};

/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTPtrHashSet::const_iterator to iterate
 * over the elements in this collection.
 *
 * @brief Deprecated. Provides a \c const iterator for RWTPtrHashSet.
 *
 * RWTPtrHashSetConstIterator provides a \c const iterator interface
 * to C++ Standard Library-based RWTPtrHashSet collections.
 *
 * Iteration over an RWTPtrHashSet is pseudorandom and dependent
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
 * The \c value type must have \c operator== defined. This requirement
 * is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tphset.h>
 * template <class T, class H, class EQ,
 *           class A=std::allocator<T*> >
 * class RWTPtrHashSetConstIterator
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class T, class H = RWTHash<T>, class EQ = std::equal_to<T>, class A = std::allocator<T*> >
class RW_DEPRECATE_TYPE("Use RWTPtrHashSet::const_iterator instead") RWTPtrHashSetConstIterator
{

public:
    /**
     * Creates a \c const iterator for the hashed set \a h. The iterator
     * begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTPtrHashSetConstIterator(const RWTPtrHashSet<T, H, EQ, A>& h)
        : table_(&h), head_(true), tail_(false) { }

    /**
     * Creates a \c const iterator for the iterator \a itr. The \c const
     * iterator begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTPtrHashSetConstIterator(const RWTPtrHashSetIterator<T, H, EQ, A>& itr)
        : table_(itr.table_), iter_(itr.iter_),
          head_(itr.head_), tail_(itr.tail_) { }

    /**
     * Advances self to the next element. If the iterator has just
     * been reset or created, self now references the first element.
     * If, before iteration, self referenced the last association in
     * the multi-map, self now points to an undefined value and a value
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
     * iterator and returns its value. If the iterator has advanced
     * past the last item in the container, #rwnil will be returned.
     */
    const T* operator()() {
        return (++(*this)) ? key() : rwnil;
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    const RWTPtrHashSet<T, H, EQ, A>* container() const {
        return table_;
    }

    /**
     * Returns the element referenced by self. Undefined if self is
     * not referencing a value within the set.
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
    void reset(const RWTPtrHashSet<T, H, EQ, A>& t) {
        head_ = true;
        tail_ = false;
        table_ = &t;
    }

private:

    const RWTPtrHashSet<T, H, EQ, A>* table_;
    typename RWTPtrHashSet<T, H, EQ, A>::const_iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);

};

#endif
#endif /* RW_CLASSIC_TEMPLATE_CONTAINERS */
#endif /* RW_TOOLS_TPHSET_H */
