#ifndef RW_TOOLS_STDEX_HASHMAP_H_
#define RW_TOOLS_STDEX_HASHMAP_H_


/**********************************************************************
 *
 * $Id: //tools/13/rw/stdex/hashmap.h#6 $
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

#include <rw/stdex/hashtable.h>
#include <rw/tools/hash.h>


/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @brief Maintains a collection of mappings between two types \c K and
 * * \c V, implemented as a hash table of \b std::pair<const K,V> instances.
 *
 * Class rw_hashmap maintains a collection mapping between instances of \c K
 * (the key) and \c V (the value), implemented as a hash table of
 * \b std::pair<const K,V> in which there may not be more than one instance
 * of any given \c K. Since this is a \e value-based collection, objects are
 * \e copied into and out of the collection. As with all classes that meet
 * the ANSI <i>associative container</i> specification, rw_hashmap provides
 * for iterators that reference its elements. Since the contents of
 * rw_hashset are in psuedo-random order, the only iterator ranges that will
 * usually make sense are the results of calling equal_range(), and the
 * entire range from begin() to end().
 *
 * \c Hash must provide a \c const function that takes a single argument
 * convertible to type \c K and returns a value of type \c size_t.
 *
 * @note
 * Any two keys that are equivalent \e must hash to the same value.
 *
 * Key equality is determined by an equality function of type \c EQ, which
 * takes two arguments convertible to type \c K and returns a value of type
 * \c bool.
 *
 * @note
 * Any two keys that are equivalent are disallowed for this container.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/stdex/hashmap.h>
 * rw_hashmap<K,V>;
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class K, class V, class Hash = RWTHash<K>, class EQ = std::equal_to<K>, class A = std::allocator<std::pair<const K, V> > >
class rw_hashmap
{

public:

    /**
     * @internal
     *
     * A type representing the underlying container type.
     */
    typedef rw_hashtable<std::pair<const K, V>, Hash, EQ, A, rw_no_duplicates, rw_pair_based_key> impl_type;

    /**
     * A type representing the allocator type for the container.
     */
    typedef typename impl_type::allocator_type allocator_type;

    /**
     * A type representing the type of keys used in the container.
     */
    typedef typename impl_type::key_type key_type;

    /**
     * A type representing the value stored in the container.
     */
    typedef typename impl_type::value_type value_type;

    /**
     * A type representing the type of mapped values used in the container.
     */
    typedef typename impl_type::key_policy::mapped_type mapped_type;

    /**
     * An unsigned integral type used for counting the number of elements in
     * the container.
     */
    typedef typename impl_type::size_type size_type;

    /**
     * A signed integral type used to indicate the distance between two
     * valid iterators on the same container.
     */
    typedef typename impl_type::difference_type difference_type;

    /**
     * A type that provides a reference to an element in the container.
     */
    typedef typename impl_type::reference reference;

    /**
     * A type that provides a \c const reference to an element in the
     * container.
     */
    typedef typename impl_type::const_reference const_reference;

    /**
     * A type that provides a forward iterator over the elements in the
     * container.
     */
    typedef typename impl_type::iterator iterator;

    /**
     * A type that provides a \c const forward iterator over the elements in
     * the container.
     */
    typedef typename impl_type::const_iterator const_iterator;

    /**
     * A type that provides a pointer to an element in the container.
     */
    typedef typename impl_type::pointer pointer;

    /**
     * A type that provides a \c const pointer to an element in the
     * container.
     */
    typedef typename impl_type::const_pointer const_pointer;

    /**
     * A type representing the hash function.
     */
    typedef typename impl_type::hasher hasher;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @deprecated As of SourcePro 12.5, use
     * \link rw_hashmap::hasher hasher \endlink instead.
     *
     * A type representing the hash function.
     */
    typedef typename impl_type::hasher
    RW_DEPRECATE_TYPE("Use rw_hashmap::hasher instead") key_hash_type;
#endif

    /**
     * A type representing the key equality function.
     *
     * @internal
     * For backward compatibility with the key_equal() member function, we
     * cannot provide typedef named in a consistent manner with the C++
     * Standard Library unordered associative containers.
     */
    typedef typename impl_type::key_equal key_equal_type;

    /**
     * @internal
     * A type representing the key hash function, as applied to a
     * #value_type.
     */
    typedef typename impl_type::key_policy::template value_hasher<hasher> value_hash_type;

    /**
     * @internal
     * A type representing the key equality function, as applied to a
     * #value_type.
     */
    typedef typename impl_type::key_policy::template value_to_value_equal<key_equal_type> value_equal_type;

    hasher key_hash() const;

    key_equal_type key_equal() const;

    value_hash_type value_hash() const;

    value_equal_type value_equal() const;

public:
    /**
     * Constructs an empty rw_hashmap with \a cap buckets, using \a h as the
     * hash function object, and \a eq as the equality function object.
     *
     * @note If the value specified for \a cap is zero, the default number
     * of buckets is used.
     */
    rw_hashmap(size_type cap = RW_DEFAULT_CAPACITY,
               const hasher& h = hasher(),
               const key_equal_type& eq = key_equal_type());

    /**
     * Constructs an rw_hashmap that is a copy of \a other. Each element
     * from \a other is copied into self.
     */
    rw_hashmap(const rw_hashmap& other);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed rw_hashmap takes ownership of the
     * data owned by \a other.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference
     * support.
     */
    rw_hashmap(rw_hashmap && other);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs an rw_hashmap containing a copy of the pair elements in
     * the range [\a first, \a last). The rw_hashmap instance has \a cap
     * buckets, uses \a h as its hash function object, and \a eq as its
     * equality function object.
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     *
     * @note If the value specified for \a cap is zero, the default number
     * of buckets is used.
     */
    template <typename InputIterator>
    rw_hashmap(InputIterator first, InputIterator last,
               size_type cap = RW_DEFAULT_CAPACITY, const hasher& h = hasher(),
               const key_equal_type& eq = key_equal_type());

    /**
     * The destructor releases the memory used by the container's
     * implementation.
     */
    ~rw_hashmap();

    /**
     * Replaces the contents of self with copies of the contents of \a rhs.
     * The capacity, hash function object and equality function object are
     * replaced by the respective objects from \a rhs.
     */
    rw_hashmap& operator=(const rw_hashmap& rhs);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Replaces the contents of self with the contents moved from \a rhs.
     * The capacity, hash function object and equality function objects are
     * moved from the respective objects in \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference
     * support.
     */
    rw_hashmap& operator=(rw_hashmap && rhs);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Returns a reference to a #mapped_type in the container whose key is
     * equal to \a key according to the associated equality function object.
     * If a key equal to \a key is not found in the collection, a default
     * constructed #mapped_type is created and inserted, and a reference to
     * that instance is returned.
     *
     * @note
     * This function requires that #mapped_type is default constructible.
     */
    mapped_type& operator[](const key_type& key);

    /**
     * Returns an iterator referring to the first element in the container.
     *
     * If the container is empty, returns end().
     */
    iterator begin();

    /**
     * @copydoc begin()
     */
    const_iterator begin() const;

    /**
     * @copydoc begin()
     */
    const_iterator cbegin() const;

    /**
     * Returns an iterator referring to the element after the last element
     * in the container.
     *
     * Dereferencing the iterator returned by this function results in
     * undefined behavior.
     */
    iterator end();

    /**
     * @copydoc end()
     */
    const_iterator end() const;

    /**
     * @copydoc end()
     */
    const_iterator cend() const;

    /**
     * Returns \c true if there are no items in the container.
     */
    bool
    empty() const;

    /**
     * Returns \c true if the container and \a rhs have the same number of
     * elements, and for each #value_type in \a lhs, there is a #value_type
     * in \a rhs that has a first part (the key) for which the equality
     * function object returns \c true. The second part (the value) of the
     * pair is not compared.
     */
    bool
    equal_by_keys(const rw_hashmap& rhs) const;

    /**
     * Returns the number of items in the container.
     */
    size_type
    size() const;

    /**
     * Returns the number of items in self whose key compares equal to
     * \a key according to the associated equality function object.
     */
    size_type
    count(const key_type& key) const;

    /**
     * Returns the first item in self whose key compares equal to \a key
     * according to the associated equality function object. If no items in
     * the container compare equal to \a key, returns end().
     */
    iterator
    find(const key_type& key);

    /**
     * @copydoc find()
     */
    const_iterator
    find(const key_type& key) const;

    /**
     * Equivalent to <tt>equal_range(\a key).first</tt>.
     */
    iterator
    lower_bound(const key_type& key);

    /**
     * @copydoc lower_bound()
     */
    const_iterator
    lower_bound(const key_type& key) const;

    /**
     * Equivalent to <tt>equal_range(\a key).second</tt>.
     */
    iterator
    upper_bound(const key_type& key);

    /**
     * @copydoc upper_bound()
     */
    const_iterator
    upper_bound(const key_type& key) const;

    /**
     * Returns the bounds of the subrange representing all of the values in
     * the container whose key compares equal to \a key according to the
     * associated equality function object. If no items in the container
     * compare equal to \a key, returns end() for both iterators.
     */
    std::pair<iterator, iterator>
    equal_range(const key_type& key);

    /**
     * @copydoc equal_range()
     */
    std::pair<const_iterator, const_iterator>
    equal_range(const key_type& key) const;

    /**
     * Returns the number of buckets in the container.
     */
    size_type
    capacity() const;

    /**
     * Returns the ratio of the number of items in the container to the
     * number of buckets.
     */
    float
    fill_ratio() const;

    /**
     * Sets the number of buckets to \a cap. Each item in the container is
     * rehashed according to the new number of buckets.
     *
     * @note
     * If \a cap is 0 it is ignored.
     */
    void
    resize(size_t cap);

    /**
     * Exchanges the contents of self with \a other, including the \c Hash
     * and \c EQ objects. This method does not copy or destroy any of the
     * items exchanged but exchanges the underlying hash tables.
     */
    void
    swap(rw_hashmap& other);

    /**
     * Inserts a copy of \a val into the container. If an element in the
     * container has the same key as \a val, an iterator to the existing
     * element in the collection is returned with an associated status of
     * \c false. Otherwise, the value \a val is inserted into the collection
     * and an iterator to the new item is returned along with the status
     * \c true.
     */
    std::pair<iterator, bool>
    insert(const value_type& val);

    /**
     * Inserts a copy of \a val into the container. If an element in the
     * container has the same key as \a val, an iterator to the existing
     * element in the collection is returned with an associated status of
     * \c false. Otherwise, the value \a val is inserted into the collection
     * and an iterator to the new item is returned along with the status
     * \c true.
     *
     * The parameter \a hint is ignored.
     */
    iterator
    insert(iterator hint, const value_type& val);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Inserts \a val into the container. If an element in the container has
     * the same key as \a val, an iterator to the existing element in the
     * collection is returned with an associated status of \c false.
     * Otherwise, the value \a val is inserted into the collection and an
     * iterator to the new item is returned along with the status \c true.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference
     * support.
     */
    std::pair<iterator, bool>
    insert(value_type && val);

    /**
     * Inserts \a val into the container. If an element in the container has
     * the same key as \a val, an iterator to the existing element in the
     * collection is returned with an associated status of \c false.
     * Otherwise, the value \a val is inserted into the collection and an
     * iterator to the new item is returned along with the status \c true.
     *
     * The parameter \a hint is ignored.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference
     * support.
     */
    iterator
    insert(iterator, value_type && val);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * For each value in the range [\a first, \a last), inserts a copy of
     * the value into self. If an element in the container has the same key
     * as the value, the value is not inserted. Returns the number of
     * elements inserted.
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     *
     * @note
     * \a first and \a last must not be iterators into self.
     */
    template <typename InputIterator>
    size_type
    insert(InputIterator first, InputIterator last);

    /**
     * Removes the item in the container whose key compares equal to \a key
     * according to the equality function object. Returns 1 if the item was
     * found and removed, 0 otherwise.
     */
    size_type
    erase(const key_type& key);

    /**
     * Removes the element referenced by \a iter and returns an iterator
     * referencing the next element. If \a iter does not reference an item
     * in self, the result is undefined.
     */
    iterator
    erase(iterator iter);

    /**
     * Removes each element in the range [\a first, \a last). Returns an
     * iterator referencing \a last. If \a first does not reference an item
     * in self (and if \a first and \a last are not equal), the behavior is
     * undefined.
     */
    iterator
    erase(iterator iter, iterator bound);

    /**
     * Removes all of the items in the container.
     */
    void
    clear();

private:
    impl_type impl_;

    template <class K1, class V1, class H1, class E1, class A1>
    friend bool operator==(const rw_hashmap<K1, V1, H1, E1, A1>& lhs,
                           const rw_hashmap<K1, V1, H1, E1, A1>& rhs);

    template <class K1, class V1, class H1, class E1, class A1>
    friend bool operator!=(const rw_hashmap<K1, V1, H1, E1, A1>& lhs,
                           const rw_hashmap<K1, V1, H1, E1, A1>& rhs);
};


template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::hasher
rw_hashmap<K, V, Hash, EQ, A>::key_hash() const
{
    return impl_.hash_function();
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::key_equal_type
rw_hashmap<K, V, Hash, EQ, A>::key_equal() const
{
    return impl_.key_eq();
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::value_hash_type
rw_hashmap<K, V, Hash, EQ, A>::value_hash() const
{
    return impl_type::key_policy::value_hash_function(impl_.hash_function());
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::value_equal_type
rw_hashmap<K, V, Hash, EQ, A>::value_equal() const
{
    return impl_type::key_policy::value_to_value_eq(impl_.key_eq());
}

template <class K, class V, class Hash, class EQ, class A>
inline
rw_hashmap<K, V, Hash, EQ, A>::rw_hashmap(size_type cap,
        const hasher& h,
        const key_equal_type& eq)
    : impl_(cap, h, eq)
{
}

template <class K, class V, class Hash, class EQ, class A>
inline
rw_hashmap<K, V, Hash, EQ, A>::rw_hashmap(const rw_hashmap& other)
    : impl_(other.impl_)
{
}

#if !defined(RW_NO_RVALUE_REFERENCES)
template <class K, class V, class Hash, class EQ, class A>
inline
rw_hashmap<K, V, Hash, EQ, A>::rw_hashmap(rw_hashmap && other)
    : impl_(rw_move(other.impl_))
{
}
#endif // !RW_NO_RVALUE_REFERENCES

template <class K, class V, class Hash, class EQ, class A>
template <typename InputIterator>
inline
rw_hashmap<K, V, Hash, EQ, A>::rw_hashmap(InputIterator first,
        InputIterator last, size_type cap, const hasher& h,
        const key_equal_type& eq)
    : impl_(first, last, cap, h, eq)
{
}

template <class K, class V, class Hash, class EQ, class A>
inline
rw_hashmap<K, V, Hash, EQ, A>::~rw_hashmap()
{
}

template <class K, class V, class Hash, class EQ, class A>
inline
rw_hashmap<K, V, Hash, EQ, A>&
rw_hashmap<K, V, Hash, EQ, A>::operator=(const rw_hashmap& rhs)
{
    if (&rhs != this) {
        impl_ = rhs.impl_;
    }
    return *this;
}

#if !defined(RW_NO_RVALUE_REFERENCES)
template <class K, class V, class Hash, class EQ, class A>
inline
rw_hashmap<K, V, Hash, EQ, A>&
rw_hashmap<K, V, Hash, EQ, A>::operator=(rw_hashmap && rhs)
{
    if (&rhs != this) {
        impl_ = rw_move(rhs.impl_);
    }
    return *this;
}
#endif // !RW_NO_RVALUE_REFERENCES

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::mapped_type&
rw_hashmap<K, V, Hash, EQ, A>::operator[](const key_type& key)
{
    value_type val(key, mapped_type());
    std::pair<iterator, bool> ret = impl_.insert(val);
    return ret.first->second;
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::iterator
rw_hashmap<K, V, Hash, EQ, A>::begin()
{
    return impl_.begin();
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::const_iterator
rw_hashmap<K, V, Hash, EQ, A>::begin() const
{
    return impl_.begin();
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::const_iterator
rw_hashmap<K, V, Hash, EQ, A>::cbegin() const
{
    return impl_.cbegin();
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::iterator
rw_hashmap<K, V, Hash, EQ, A>::end()
{
    return impl_.end();
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::const_iterator
rw_hashmap<K, V, Hash, EQ, A>::end() const
{
    return impl_.end();
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::const_iterator
rw_hashmap<K, V, Hash, EQ, A>::cend() const
{
    return impl_.cend();
}

template <class K, class V, class Hash, class EQ, class A>
inline
bool
rw_hashmap<K, V, Hash, EQ, A>::empty() const
{
    return impl_.empty();
}

template <class K, class V, class Hash, class EQ, class A>
inline
bool
rw_hashmap<K, V, Hash, EQ, A>::equal_by_keys(const rw_hashmap& rhs) const
{
    return impl_.equal_by_keys(rhs.impl_);
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::size_type
rw_hashmap<K, V, Hash, EQ, A>::size() const
{
    return impl_.size();
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::size_type
rw_hashmap<K, V, Hash, EQ, A>::count(const key_type& key) const
{
    return impl_.count(key);
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::const_iterator
rw_hashmap<K, V, Hash, EQ, A>::find(const key_type& key) const
{
    return impl_.find(key);
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::iterator
rw_hashmap<K, V, Hash, EQ, A>::find(const key_type& key)
{
    return impl_.find(key);
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::iterator
rw_hashmap<K, V, Hash, EQ, A>::lower_bound(const key_type& key)
{
    return impl_.find(key);
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::const_iterator
rw_hashmap<K, V, Hash, EQ, A>::lower_bound(const key_type& key) const
{
    return impl_.find(key);
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::iterator
rw_hashmap<K, V, Hash, EQ, A>::upper_bound(const key_type& key)
{
    return impl_.equal_range(key).second;
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::const_iterator
rw_hashmap<K, V, Hash, EQ, A>::upper_bound(const key_type& key) const
{
    return impl_.equal_range(key).second;
}

template <class K, class V, class Hash, class EQ, class A>
inline
std::pair<typename rw_hashmap<K, V, Hash, EQ, A>::iterator, typename rw_hashmap<K, V, Hash, EQ, A>::iterator>
rw_hashmap<K, V, Hash, EQ, A>::equal_range(const key_type& key)
{
    return impl_.equal_range(key);
}

template <class K, class V, class Hash, class EQ, class A>
inline
std::pair<typename rw_hashmap<K, V, Hash, EQ, A>::const_iterator, typename rw_hashmap<K, V, Hash, EQ, A>::const_iterator>
rw_hashmap<K, V, Hash, EQ, A>::equal_range(const key_type& key) const
{
    return impl_.equal_range(key);
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::size_type
rw_hashmap<K, V, Hash, EQ, A>::capacity() const
{
    return impl_.capacity();
}

template <class K, class V, class Hash, class EQ, class A>
inline
float
rw_hashmap<K, V, Hash, EQ, A>::fill_ratio() const
{
    return impl_.fill_ratio();
}

template <class K, class V, class Hash, class EQ, class A>
inline
void
rw_hashmap<K, V, Hash, EQ, A>::resize(size_t cap)
{
    impl_.resize(cap);
}

template <class K, class V, class Hash, class EQ, class A>
inline
void
rw_hashmap<K, V, Hash, EQ, A>::swap(rw_hashmap& other)
{
    impl_.swap(other.impl_);
}

template <class K, class V, class Hash, class EQ, class A>
inline
std::pair<typename rw_hashmap<K, V, Hash, EQ, A>::iterator, bool>
rw_hashmap<K, V, Hash, EQ, A>::insert(const value_type& val)
{
    return impl_.insert(val);
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::iterator
rw_hashmap<K, V, Hash, EQ, A>::insert(iterator hint, const value_type& val)
{
    return impl_.insert(hint, val);
}

#if !defined(RW_NO_RVALUE_REFERENCES)
template <class K, class V, class Hash, class EQ, class A>
inline
std::pair<typename rw_hashmap<K, V, Hash, EQ, A>::iterator, bool>
rw_hashmap<K, V, Hash, EQ, A>::insert(value_type && val)
{
    return impl_.insert(rw_move(val));
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::iterator
rw_hashmap<K, V, Hash, EQ, A>::insert(iterator hint, value_type && val)
{
    return impl_.insert(hint, rw_move(val));
}
#endif // !RW_NO_RVALUE_REFERENCES

template <class K, class V, class Hash, class EQ, class A>
template <typename InputIterator>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::size_type
rw_hashmap<K, V, Hash, EQ, A>::insert(InputIterator first, InputIterator last)
{
    return impl_.insert(first, last);
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::size_type
rw_hashmap<K, V, Hash, EQ, A>::erase(const key_type& key)
{
    return impl_.erase(key);
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::iterator
rw_hashmap<K, V, Hash, EQ, A>::erase(iterator iter)
{
    return impl_.erase(iter);
}

template <class K, class V, class Hash, class EQ, class A>
inline
typename rw_hashmap<K, V, Hash, EQ, A>::iterator
rw_hashmap<K, V, Hash, EQ, A>::erase(iterator iter, iterator bound)
{
    return impl_.erase(iter, bound);
}

template <class K, class V, class Hash, class EQ, class A>
inline
void
rw_hashmap<K, V, Hash, EQ, A>::clear()
{
    impl_.clear();
}

/**
 * @relates rw_hashmap
 *
 * Returns \c true if \a lhs and \a rhs have the same number of elements,
 * and for each item in \a lhs there is an item in \a rhs whose first part
 * (the key) compares equal according to the equality function object and
 * whose second part (the value) compares equal according to operator==().
 *
 * @note
 * If only the keys of the values need to be compared for equality use
 * rw_hashmap::equal_by_keys() instead.
 */
template <class K, class V, class Hash, class EQ, class A>
bool
operator==(const rw_hashmap<K, V, Hash, EQ, A>& lhs,
           const rw_hashmap<K, V, Hash, EQ, A>& rhs)
{
    return lhs.impl_ == rhs.impl_;
}

/**
 * @relates rw_hashmap
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class K, class V, class Hash, class EQ, class A>
bool
operator!=(const rw_hashmap<K, V, Hash, EQ, A>& lhs,
           const rw_hashmap<K, V, Hash, EQ, A>& rhs)
{
    return lhs.impl_ != rhs.impl_;
}


#endif /* RW_TOOLS_STDEX_HASHMAP_H_ */
