#ifndef RW_TOOLS_STDEX_HASHTABLE_H_
#define RW_TOOLS_STDEX_HASHTABLE_H_

/**********************************************************************
 *
 * $Id: //tools/13/rw/stdex/hashtable.h#17 $
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
#include <rw/rwassert.h>
#include <rw/stdex/slist.h>
#include <rw/tools/algorithm.h>
#include <rw/tools/hash.h>

#include <algorithm>
#include <iterator>
#include <list>
#include <memory>
#include <vector>


/**
 * @internal
 * @relates rw_hashtable<T, Hash, EQ, A, DP, KP>
 *
 * @brief Iterator over items contained in an rw_hashtable.
 *
 * rw_hashtable_iterator provides a forward iterator implementation over the
 * items in an rw_hashtable.
 */
template <typename C, typename I>
class rw_hashtable_iterator
    : public std::iterator < std::forward_iterator_tag,
      typename I::value_type,
      typename I::difference_type,
      typename I::pointer, typename I::reference >
{
public:
    /**
     * @internal
     *
     * A type representing the base class.
     */
    typedef std::iterator < std::forward_iterator_tag, typename I::value_type,
            typename I::difference_type, typename I::pointer,
            typename I::reference > base_type;

    /**
     * @internal
     *
     * A type representing the underlying container type.
     */
    typedef C container_type;

    /**
     * @internal
     *
     * A type representing the inner iterator.
     */
    typedef I inner_iterator;

    /**
     * A type representing the value_type of the iterator.
     */
    typedef typename base_type::value_type value_type;

    /**
     * A type representing a signed integral value when calculating the
     * difference between two iterators.
     */
    typedef typename base_type::difference_type difference_type;

    /**
     * A type representing the type returned by operator-> for the iterator.
     */
    typedef typename base_type::pointer pointer;

    /**
     * A type representing the type returned by operator* for the iterator.
     */
    typedef typename base_type::reference reference;

    /**
     * A type representing the iterator category for the iterator.
     */
    typedef typename base_type::iterator_category iterator_category;

    /**
     * Constructs an invalid rw_hashtable_iterator. The behavior of
     * any operations are undefined until the iterator has been assigned to
     * a valid iterator instance.
     */
    rw_hashtable_iterator();

    /**
     * @internal
     *
     * Constructs an rw_hashtable_iterator positioned at the first element
     * in \a container.
     */
    explicit rw_hashtable_iterator(C& container);

    /**
     * @internal
     *
     * Constructs an rw_hashtable_iterator positioned at the internal
     * iterator \a inner.
     *
     * If \a inner is not an iterator in the container \a container
     * and in the bucket \a bucket, the behavior is undefined.
     */
    rw_hashtable_iterator(C& container, typename C::size_type bucket,
                          inner_iterator inner);

    /**
     * Copy constructor.
     */
    rw_hashtable_iterator(const rw_hashtable_iterator& other);

    /**
     * Conversion constructor. Allows conversion from rw_hashtable_iterator
     * instances where the underlying iterator types are convertible.
     */
    template <typename C2, typename I2>
    rw_hashtable_iterator(const rw_hashtable_iterator<C2, I2>& other);

    /**
     * Assignment operator.
     */
    rw_hashtable_iterator&
    operator=(const rw_hashtable_iterator& other);

    /**
     * Conversion assignment operator. Allows conversion from
     * rw_hashtable_iterator instances where the underlying iterator types
     * are convertible.
     */
    template <typename C2, typename I2>
    rw_hashtable_iterator&
    operator=(const rw_hashtable_iterator<C2, I2>& other);

    /**
     * Returns a reference to the object pointed to by this iterator.
     */
    reference operator*() const;

    /**
     * Returns a pointer to the object pointed to by this iterator.
     */
    pointer operator->() const;

    /**
     * Increments the iterator to the next position, returning the new
     * iterator.
     */
    rw_hashtable_iterator& operator++();

    /**
     * Increments the iterator to the next position, returning the previous
     * iterator.
     */
    rw_hashtable_iterator operator++(int);

    /**
     * @internal
     *
     * Returns the container the iterator is walking.
     */
    C* container() const;

    /**
     * @internal
     *
     * Returns the bucket the iterator is currently positioned on.
     */
    typename C::size_type bucket() const;

    /**
     * @internal
     *
     * Returns the current inner iterator.
     */
    inner_iterator inner() const;

private:
    /**
     * @internal
     *
     * When an inner iterator is incremented and reaches the end of the
     * inner container, this function iterates over the outer container
     * until a non-empty inner container is found.
     */
    void skip_empty();

    C* container_;
    typename C::size_type bucket_;
    inner_iterator inner_iter_;

    template <typename C1, typename I1, typename C2, typename I2>
    friend bool operator==(const rw_hashtable_iterator<C1, I1>& lhs,
                           const rw_hashtable_iterator<C2, I2>& rhs);
};


/**
 * @internal
 * @relates rw_hashtable<T, Hash, EQ, A, DP, KP>
 *
 * @brief Defines a behavioral policy for containers that do not allow
 * duplicate values to be inserted.
 *
 * rw_no_duplicates defines a behavior policy for containers that do not
 * allow duplicate values to be inserted. The container is responsible for
 * calling functions on this policy to control inserting, removing and
 * searching for elements within the container.
 */
struct rw_no_duplicates {
    /**
     * @internal
     *
     * Inserts a copy of \a val into the container \a container. If \a val
     * is already found in the container, an iterator to the existing value
     * is returned along with the status \c false. If \a val was not
     * present, it is inserted and an iterator to the new element is
     * returned along with the status \c true.
     */
    template <typename C, typename V, typename EQ>
    static std::pair<typename C::iterator, bool>
    insert(C& container, const V& val, EQ key_equal);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @internal
     *
     * Inserts \a val into the container \a container. If \a val is already
     * found in the container, an iterator to the existing value is returned
     * along with the status \c false. If \a val was not present, it is
     * inserted and an iterator to the new element is returned along with
     * the status \c true.
     */
    template <typename C, typename V, typename EQ>
    static std::pair<typename C::iterator, bool>
    insert(C& container, V && val, EQ key_equal);
#endif

    /**
     * @internal
     *
     * If \a key is found in \a container, returns 1. Otherwise, returns 0.
     */
    template <typename C, typename K, typename EQ>
    static typename C::size_type
    count(const C& container, const K& key, EQ key_equal);

    /**
     * @internal
     *
     * Returns the first iterator in [\a start, \a end) where \a key_equal
     * returns \c false when compared against \a start. If an item matching
     * this criterion is not found, returns \a end.
     */
    template <typename I, typename EQ>
    static I next_not_equal(I start, I end, EQ key_equal);

    /**
     * @internal
     *
     * If an item matching \a key is found in \a container, the item is
     * removed and 1 is returned. Otherwise 0 is returned and the container
     * is unchanged.
     *
     * @note
     * The \a key_equal argument is ignored.
     */
    template <typename C, typename K, typename EQ>
    static typename C::size_type
    erase(C& container, const K& key, EQ key_equal);
};


/**
 * @internal
 * @relates rw_hashtable<T, Hash, EQ, A, DP, KP>
 *
 * @brief Defines a behavioral policy for containers that allow duplicate
 * values to be inserted.
 *
 * rw_allow_duplicates defines a behavior policy for containers that allow
 * duplicate values to be inserted. The container is responsible for calling
 * functions on this policy to control inserting, removing and searching for
 * elements within the container.
 */
struct rw_allow_duplicates {
    /**
     * @internal
     *
     * Inserts a copy of \a val into the container \a container. An
     * iterator pointing to the newly inserted object is returned, along
     * with the status \c true.
     */
    template <typename C, typename V, typename EQ>
    static std::pair<typename C::iterator, bool>
    insert(C& container, const V& val, EQ key_equal);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @internal
     *
     * Inserts \a val into the container \a container. An iterator pointing
     * to the newly inserted object is returned, along with the status
     * \c true.
     */
    template <typename C, typename V, typename EQ>
    static std::pair<typename C::iterator, bool>
    insert(C& container, V && val, EQ key_equal);
#endif

    /**
     * @internal
     *
     * Returns the number of items in \a container that compare equal to
     * \a val according to the equality function object \a key_equal.
     */
    template <typename C, typename K, typename EQ>
    static typename C::size_type
    count(const C& container, const K& val, EQ key_equal);

    /**
     * @internal
     *
     * Returns the first iterator in [\a start, \a end) where \a key_equal
     * returns \c false when compared against \a start. If an item matching
     * this criterion is not found, returns \a end.
     */
    template <typename I, typename EQ>
    static I next_not_equal(I start, I end, EQ key_equal);

    /**
     * @internal
     *
     * All items in \a container that compare equal to \a key according to
     * the \a key_equal equality function are removed from \a container.
     * The number of items that were removed is returned.
     */
    template <typename C, typename K, typename EQ>
    static typename C::size_type
    erase(C& container, const K& key, EQ key_equal);
};


/**
 * @internal
 * @relates rw_hashtable<T, Hash, EQ, A, DP, KP>
 *
 * @brief Defines a behavioral policy for containers that compare for
 * equality based on the stored value.
 *
 * Defines a behavioral policy for containers that compare for equality
 * based on the stored value.
 */
template <typename T>
struct rw_value_based_key {
    /**
     * The type representing the value used in key comparisons in the
     * container.
     */
    typedef T key_type;

    /**
     * The type representing the value stored in the container.
     */
    typedef T value_type;

    /**
     * Returns the value of the key portion of \a val.
     */
    static
    const key_type& key_from_value(const value_type& val);

    /**
     * Equality function object for determining if a value_type contains a
     * key_type that matches some key. Returns \c true if the value_type has
     * a key_type matching the key, \c false otherwise.
     */
    template <typename EQ>
    struct value_to_key_equal
            : public std::binary_function<value_type, key_type, bool> {
        value_to_key_equal(EQ key_equal);
        bool operator()(const value_type& lhs, const key_type& rhs) const;

    private:
        EQ key_equal_;
    };

    /**
     * Returns a comparison object based on \a key_equal that compares a
     * value_type and key_type.
     */
    template <typename EQ>
    static
    value_to_key_equal<EQ> value_to_key_eq(EQ key_equal) {
        return value_to_key_equal<EQ>(key_equal);
    }

    /**
     * Equality function object for determining if a two value_types are
     * equal according to their keys. Returns \c true if the value_types
     * have equal keys, \c false otherwise.
     */
    template <typename EQ>
    struct value_to_value_equal
            : public std::binary_function<value_type, value_type, bool> {
        value_to_value_equal(EQ key_equal);
        bool operator()(const value_type& lhs, const value_type& rhs) const;

    private:
        EQ key_equal_;
    };

    /**
     * Returns a comparison object based on \a key_equal that compares a
     * value_type and a value_type based only on their keys.
     */
    template <typename EQ>
    static
    value_to_value_equal<EQ> value_to_value_eq(EQ key_equal) {
        return value_to_value_equal<EQ>(key_equal);
    }
};


/**
 * @internal
 * @relates rw_hashtable<T, Hash, EQ, A, DP, KP>
 *
 * @brief Defines a behavioral policy for containers that compare for
 * equality based on the first member of a pair.
 *
 * Defines a behavioral policy for containers that compare for equality
 * based on the first member of a pair.
 */
template <typename T>
struct rw_pair_based_key {
    /**
     * The type representing the value used in key comparisons in the
     * container.
     */
    typedef typename RWTRemoveConst<typename T::first_type>::type key_type;

    /**
     * The type representing the mapped value stored in the container.
     */
    typedef typename T::second_type mapped_type;

    /**
     * The type representing the value stored in the container.
     */
    typedef T value_type;

    /**
     * Returns the value of the key portion of \a val.
     */
    static
    const key_type& key_from_value(const value_type& val);

    /**
     * Equality function object for determining if a value_type contains a
     * key_type that matches some key. Returns \c true if the value_type has
     * a key_type matching the key, \c false otherwise.
     */
    template <typename EQ>
    struct value_to_key_equal
            : public std::binary_function<value_type, key_type, bool> {
        value_to_key_equal(EQ key_equal);

        bool operator()(const value_type& lhs, const key_type& rhs) const;

    private:
        EQ key_equal_;
    };

    /**
     * Returns a comparison object based on \a key_equal that compares a
     * value_type and key_type.
     */
    template <typename EQ>
    static
    value_to_key_equal<EQ> value_to_key_eq(EQ key_equal) {
        return value_to_key_equal<EQ>(key_equal);
    }

    /**
     * Equality function object for determining if a two value_types are
     * equal according to their keys. Returns \c true if the value_types
     * have equal keys, \c false otherwise.
     */
    template <typename EQ>
    struct value_to_value_equal
            : public std::binary_function<value_type, value_type, bool> {
        value_to_value_equal(EQ key_equal);

        bool operator()(const value_type& lhs, const value_type& rhs) const;

    private:
        EQ key_equal_;
    };

    /**
     * Returns a comparison object based on \a key_equal that compares a
     * value_type and a value_type based only on their keys.
     */
    template <typename EQ>
    static
    value_to_value_equal<EQ> value_to_value_eq(EQ key_equal) {
        return value_to_value_equal<EQ>(key_equal);
    }

    /**
     * Hash function object for determining the hash value of a value_type
     * based only on its key. Returns the hash value of the value_type.
     */
    template <typename Hash>
    struct value_hasher
            : public std::unary_function<value_type, size_t> {
        value_hasher(Hash hasher);

        size_t operator()(const value_type& val) const;

    private:
        Hash hash_;
    };

    /**
     * Returns a function object based on \a hash that returns the hash
     * value of value_type based only on its key.
     */
    template <typename Hash>
    static
    value_hasher<Hash> value_hash_function(Hash hash) {
        return value_hasher<Hash>(hash);
    }
};


/**
 * @internal
 * @relates rw_hashtable<T, Hash, EQ, A, DP, KP>
 *
 * Equality function object for comparing two containers based on their
 * duplicates_policy and key_policy.
 */
template <typename C, typename DP, typename KP>
struct rw_container_equal;


/**
 * @internal
 * @relates rw_hashtable<T, Hash, EQ, A, DP, KP>
 *
 * Equality function object for comparing two containers that do not allow
 * duplicates and whose values are also their keys.
 */
template <typename C, typename T>
struct rw_container_equal<C, rw_no_duplicates, rw_value_based_key<T> > {
    bool operator()(const C& lhs, const C& rhs) const;
};


/**
 * @internal
 * @relates rw_hashtable<T, Hash, EQ, A, DP, KP>
 *
 * Equality function object for comparing two containers that allow
 * duplicates and whose values are also their keys.
 */
template <typename C, typename T>
struct rw_container_equal<C, rw_allow_duplicates, rw_value_based_key<T> > {
    bool operator()(const C& lhs, const C& rhs) const;
};


/**
 * @internal
 * @relates rw_hashtable<T, Hash, EQ, A, DP, KP>
 *
 * Equality function object for comparing two containers that do not allow
 * duplicates and whose values are pairs, where the first part is the key.
 */
template <typename C, typename T>
struct rw_container_equal<C, rw_no_duplicates, rw_pair_based_key<T> > {
    bool operator()(const C& lhs, const C& rhs) const;
};


/**
 * @internal
 * @relates rw_hashtable<T, Hash, EQ, A, DP, KP>
 *
 * Equality function object for comparing two containers that allow
 * duplicates and whose values are pairs, where the first part is the key.
 */
template <typename C, typename T>
struct rw_container_equal<C, rw_allow_duplicates, rw_pair_based_key<T> > {
    bool operator()(const C& lhs, const C& rhs) const;
};


/**
 * @internal
 * @ingroup stl_extension_based_collection_classes
 *
 * @brief Maintains a hash-based collection of type T
 *
 * Class rw_hashtable maintains a collection of \c T, implemented as a hash
 * table. Since this is a \e value-based container, objects are \e copied
 * into and out of the container. As with all classes that meet the ANSI
 * <i>associative container</i> specification, rw_hashtable provides for
 * iterators that reference its elements. Since the contents of rw_hashtable
 * are in pseudo-random order, the only iterator ranges that will usually
 * make sense are the results of calling equal_range(), and the entire range
 * from begin() to end().
 *
 * The behavior of rw_hashtable in regards to how items are inserted,
 * removed or found in the container is controlled by a policy trait.
 *
 * \c Hash must provide a \c const function that takes a single argument
 * convertible to type \c T and returns a value of type \c size_t.
 *
 * @note
 * Any two elements that are equivalent \e must hash to the same value.
 *
 * Element equality is determined by an equality function of type \c EQ,
 * which takes two arguments convertible to type \c T and returns a value of
 * type \c bool.
 */
template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
class rw_hashtable
{
public:

    /**
     * A type representing the allocator type for the container.
     */
    typedef A allocator_type;

    /**
     * @internal
     *
     * A type representing the policy describing how duplicate entries
     * should be handled.
     */
    typedef DP duplicates_policy;

    /**
     * @internal
     *
     * A type representing the policy describing how values are compared.
     */
    typedef KP<T> key_policy;

    /**
     * A type representing the type of keys used in the container.
     */
    typedef typename key_policy::key_type key_type;

    /**
     * A type representing the type stored in the container.
     */
    typedef typename key_policy::value_type value_type;

    /**
     * A type representing the hash function.
     */
    typedef Hash hasher;

    /**
     * A type representing the equality function.
     */
    typedef EQ key_equal;

    /**
     * A type that provides a reference to an element in the container.
     */
    typedef value_type& reference;

    /**
     * A type that provides a \c const reference to an element in the
     * container.
     */
    typedef const value_type& const_reference;

    /**
     * A type that provides a pointer to an element in the container.
     */
    typedef typename allocator_type::pointer pointer;

    /**
     * A type that provides a \c const pointer to an element in the
     * container.
     */
    typedef typename allocator_type::const_pointer const_pointer;

    /**
     * An unsigned integral type used for counting the number of elements in
     * the container.
     */
    typedef size_t size_type;

    /**
     * A signed integral type used to indicate the distance between two
     * valid iterators on the same container.
     */
    typedef ptrdiff_t difference_type;

    /**
     * @internal
     *
     * A type indicating the underlying container for a bucket within the
     * container.
     */
    typedef rw_slist<value_type, allocator_type> bucket_type;

    /**
     * @internal
     *
     * An allocator for an instance of the containers bucket type.
     */
#  ifndef RW_ALLOC_INTERFACE_STLV2X_HACK
    typedef typename allocator_type::template rebind<bucket_type>::other
    bucket_allocator_type;
#  else
    typedef std::allocator<bucket_type> bucket_allocator_type;
#endif

    /**
     * @internal
     *
     * A type indicating the underlying collection of buckets.
     */
    typedef std::vector<bucket_type, bucket_allocator_type> bucket_list_type;

    /**
     * A type that provides a forward iterator over the elements in the
     * container.
     */
    typedef rw_hashtable_iterator < bucket_list_type,
            typename bucket_type::iterator > iterator;

    /**
     * A type that provides a \c const forward iterator over the elements in
     * the container.
     */
    typedef rw_hashtable_iterator < const bucket_list_type,
            typename bucket_type::const_iterator > const_iterator;

    /**
     * Returns the hash function object used by the container.
     */
    hasher hash_function() const;

    /**
     * Returns the key equality function object used by the container.
     */
    key_equal key_eq() const;

    /**
     * Constructs an empty rw_hashtable with \a cap buckets, using \a h as
     * the hash function object, and \a eq as the equality function object.
     *
     * @note If the value specified for \a cap is zero, the default number
     * of buckets is used.
     */
    rw_hashtable(size_type cap = RW_DEFAULT_CAPACITY,
                 const hasher& h = hasher(),
                 const key_equal& eq = key_equal());

    /**
     * Constructs an rw_hashtable containing a copy of the elements in the
     * range [\a first, \a last). The rw_hashtable instance has \a cap
     * buckets, uses \a h as its hash function object, and \a eq as the
     * equality function object.
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     *
     * @note If the value specified for \a cap is zero, the default number
     * of buckets is used.
     */
    // Sunpro is unable to parse out-lined member templates of a class with
    // template templates.
    template <typename InputIterator>
    rw_hashtable(InputIterator first, InputIterator last,
                 size_type cap = RW_DEFAULT_CAPACITY,
                 const hasher& h = hasher(),
                 const key_equal& eq = key_equal())
        : buckets_(cap ? cap : RW_DEFAULT_CAPACITY), hash_(h), eq_(eq) {
        std::copy(first, last, std::inserter(*this, this->end()));
    }

    /**
     * Constructs an rw_hashtable that is a copy of \a other. Each element
     * from \a other is copied into self.
     */
    rw_hashtable(const rw_hashtable& other);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed list takes ownership of the data
     * owned by \a other.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference
     * support.
     */
    rw_hashtable(rw_hashtable && other);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * The destructor releases the memory used by the container's
     * implementation.
     */
    ~rw_hashtable();

    /**
     * Replaces the contents of self with the contents of \a rhs. The
     * capacity, hash function object and equality function object are
     * replaced by the respective objects from \a rhs.
     */
    rw_hashtable& operator=(const rw_hashtable& rhs);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Replaces the contents of self with the contents moved from \a rhs.
     * The capacity, hash function object and equality function object are
     * moved from the respective objects in \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference
     * support.
     */
    rw_hashtable& operator=(rw_hashtable && rhs);
#endif // !RW_NO_RVALUE_REFERENCES

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
    bool empty() const;

    /**
     * Returns the number of items in the container.
     */
    size_type size() const;

    /**
     * Returns the number of items in self that compare equal to \a key
     * according to the associated equality function object.
     */
    size_type
    count(const key_type& key) const;

    /**
     * Returns the first item in self that compares equal to \a key
     * according to the associated equality function object. If no items in
     * the container compare equal to \a key, returns end().
     */
    iterator
    find(const key_type& key);

    /**
     * Returns the first item in self that compares equal to \a key
     * according to the associated equality function object. If no items in
     * the container compare equal to \a key, returns end().
     */
    const_iterator
    find(const key_type& key) const;

    /**
     * Returns \c true if self and \a rhs have the same number of elements,
     * and for each item in self there is an item in \a rhs that compares
     * equal according to the equality function object.
     */
    bool
    equal_by_keys(const rw_hashtable& rhs) const;

    /**
     * Returns the bounds of the subrange representing all of the values in
     * the container that compare equal to \a key according to the
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
    resize(size_type cap);

    /**
     * Exchanges the contents of self with \a other, including the \c Hash
     * and \c EQ objects. This method does not copy or destroy any of the
     * items exchanged but exchanges the underlying hash tables.
     */
    void
    swap(rw_hashtable& other);

    /**
     * Inserts a copy of \a val into the container. If an element in the
     * container is equivalent to \a val, and the duplicates_policy for the
     * container prohibits duplicates, an iterator to the existing element
     * in the container is returned. Otherwise, the value \a val is
     * inserted into the container and an iterator to the new item is
     * returned.
     */
    std::pair<iterator, bool>
    insert(const_reference val);

    /**
     * Inserts a copy of \a val into the container. If an element in the
     * container is equivalent to \a val, and the duplicates_policy for the
     * container prohibits duplicates, an iterator to the existing element
     * in the container is returned. Otherwise, the value \a val is
     * inserted into the container and an iterator to the new item is
     * returned.
     */
    iterator insert(const_iterator hint, const_reference val);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Inserts \a val into the container. If an element in the container is
     * equivalent to \a val, and the duplicates_policy for the container
     * prohibits duplicates, an iterator to the existing element in the
     * container is returned. Otherwise, the value \a val is inserted into
     * the container and an iterator to the new item is returned.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference
     * support.
     */
    std::pair<iterator, bool>
    insert(value_type && val);

    /**
     * Inserts \a val into the container. If an element in the container is
     * equivalent to \a val, and the duplicates_policy for the container
     * prohibits duplicates, an iterator to the existing element in the
     * container is returned. Otherwise, the value \a val is inserted into
     * the container and an iterator to the new item is returned.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference
     * support.
     */
    iterator insert(const_iterator hint, value_type && val);
#endif

    /**
     * For each value in the range [\a first, \a last), inserts a copy of
     * the value into self. If an element in the container is equivalent to
     * the value, and the duplicates_policy for the container prohibits
     * duplicates, the value is not inserted. Returns the number of elements
     * inserted.
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     *
     * @note
     * \a first and \a last must not be iterators into self.
     */
    // Sunpro is unable to parse out-lined member templates of a class with
    // template templates.
    template <typename InputIterator>
    size_type
    insert(InputIterator first, InputIterator last) {
        size_type count = 0;
        for (InputIterator iter = first; iter != last; ++iter) {
            const std::pair<iterator, bool> res = insert(*iter);
            count += res.second ? 1 : 0;
        }
        return count;
    }

    /**
     * Removes each item in the container that compares equal to \a key
     * according to the equality function object. Returns the number of
     * items removed from the container.
     */
    size_type
    erase(const key_type& key);

    /**
     * Removes the element referenced by \a iter from the container and
     * returns an iterator referencing the next element. If \a iter does not
     * reference an item in self, the result is undefined.
     */
    iterator
    erase(const_iterator iter);

    /**
     * Removes each element in the range [\a first, \a last). Returns an
     * iterator referencing \a last. If \a first does not reference an item
     * in self (and if \a first and \a last are not equal), the behavior is
     * undefined.
     */
    iterator
    erase(const_iterator first, const_iterator last);

    /**
     * Removes all of the items in the container.
     */
    void clear();

private:

    bucket_list_type buckets_;

    hasher hash_;
    key_equal eq_;
};


template <typename C, typename I>
inline
rw_hashtable_iterator<C, I>::rw_hashtable_iterator()
    : container_(0), bucket_(0)
{
}


template <typename C, typename I>
inline
rw_hashtable_iterator<C, I>::rw_hashtable_iterator(C& container)
    : container_(&container), bucket_(0)
{
    if (bucket_ != container_->size()) {
        inner_iter_ = (*container_)[bucket_].begin();
        skip_empty();
    }
}


template <typename C, typename I>
inline
rw_hashtable_iterator<C, I>::rw_hashtable_iterator(C& container,
        typename C::size_type bucket, inner_iterator inner)
    : container_(&container), bucket_(bucket), inner_iter_(inner)
{
    skip_empty();
}


template <typename C, typename I>
inline
rw_hashtable_iterator<C, I>::rw_hashtable_iterator(
    const rw_hashtable_iterator& other)
    : container_(other.container_), bucket_(other.bucket_),
      inner_iter_(other.inner_iter_)
{
}


template <typename C, typename I>
template <typename C2, typename I2>
inline
rw_hashtable_iterator<C, I>::rw_hashtable_iterator(
    const rw_hashtable_iterator<C2, I2>& other)
    : container_(other.container()), bucket_(other.bucket()),
      inner_iter_(other.inner())
{
}


template <typename C, typename I>
inline
rw_hashtable_iterator<C, I>&
rw_hashtable_iterator<C, I>::operator=(const rw_hashtable_iterator& other)
{
    if (&other != this) {
        container_ = other.container_;
        bucket_ = other.bucket_;
        inner_iter_ = other.inner_iter_;
    }
    return *this;
}


template <typename C, typename I>
template <typename C2, typename I2>
inline
rw_hashtable_iterator<C, I>&
rw_hashtable_iterator<C, I>::operator=(const rw_hashtable_iterator<C2, I2>& other)
{
    container_ = other.container();
    bucket_ = other.bucket();
    inner_iter_ = other.inner();
    return *this;
}


template <typename C, typename I>
inline
typename rw_hashtable_iterator<C, I>::reference
rw_hashtable_iterator<C, I>::operator*() const
{
    return *inner_iter_;
}


template <typename C, typename I>
inline
typename rw_hashtable_iterator<C, I>::pointer
rw_hashtable_iterator<C, I>::operator->() const
{
    return &*inner_iter_;
}


template <typename C, typename I>
inline
rw_hashtable_iterator<C, I>&
rw_hashtable_iterator<C, I>::operator++()
{
    ++inner_iter_;
    skip_empty();
    return *this;
}


template <typename C, typename I>
inline
rw_hashtable_iterator<C, I>
rw_hashtable_iterator<C, I>::operator++(int)
{
    const rw_hashtable_iterator ret(*this);
    ++(*this);
    return ret;
}


template <typename C, typename I>
inline
C*
rw_hashtable_iterator<C, I>::container() const
{
    return container_;
}


template <typename C, typename I>
inline
typename C::size_type
rw_hashtable_iterator<C, I>::bucket() const
{
    return bucket_;
}


template <typename C, typename I>
inline
typename rw_hashtable_iterator<C, I>::inner_iterator
rw_hashtable_iterator<C, I>::inner() const
{
    return inner_iter_;
}


template <typename C, typename I>
inline
void
rw_hashtable_iterator<C, I>::skip_empty()
{
    while (bucket_ + 1 < container_->size() &&
            inner_iter_ == (*container_)[bucket_].end()) {
        ++bucket_;
        inner_iter_ = (*container_)[bucket_].begin();
    }
}


/**
 * @relates rw_hashtable_iterator
 *
 * Returns \c true if \a lhs and \a rhs point to the same value within a
 * container, \c false otherwise.
 *
 * If \a lhs and \a rhs are not iterators to the same container, the
 * behavior is undefined.
 */
template <typename C1, typename I1, typename C2, typename I2>
inline
bool
operator==(const rw_hashtable_iterator<C1, I1>& lhs,
           const rw_hashtable_iterator<C2, I2>& rhs)
{
    if (lhs.container_ != rhs.container_ || lhs.bucket_ != rhs.bucket_ ||
            lhs.inner_iter_ != rhs.inner_iter_) {
        return false;
    }
    return true;
}


/**
 * @relates rw_hashtable_iterator
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <typename C1, typename I1, typename C2, typename I2>
inline
bool
operator!=(const rw_hashtable_iterator<C1, I1>& lhs,
           const rw_hashtable_iterator<C2, I2>& rhs)
{
    return !(lhs == rhs);
}


template <typename C, typename V, typename EQ>
inline
std::pair<typename C::iterator, bool>
rw_no_duplicates::insert(C& container, const V& val, EQ key_equal)
{
    typename C::iterator inner_iter =
        std::find_if(container.begin(), container.end(),
                     rw_bind2nd_ref(key_equal, val));
    if (inner_iter != container.end()) {
        return std::make_pair(inner_iter, false);
    }

    inner_iter = container.insert(container.end(), val);
    return std::make_pair(inner_iter, true);
}


#if !defined(RW_NO_RVALUE_REFERENCES)
template <typename C, typename V, typename EQ>
inline
std::pair<typename C::iterator, bool>
rw_no_duplicates::insert(C& container, V && val, EQ key_equal)
{
    typename C::iterator inner_iter =
        std::find_if(container.begin(), container.end(),
                     rw_bind2nd_ref(key_equal, val));
    if (inner_iter != container.end()) {
        return std::make_pair(inner_iter, false);
    }

    inner_iter = container.insert(container.end(), rw_move(val));
    return std::make_pair(inner_iter, true);
}
#endif


template <typename C, typename K, typename EQ>
inline
typename C::size_type
rw_no_duplicates::count(const C& container, const K& val, EQ key_equal)
{
    const typename C::const_iterator inner_iter =
        std::find_if(container.begin(), container.end(),
                     rw_bind2nd_ref(key_equal, val));
    return inner_iter != container.end() ? 1U : 0U;
}


template <typename I, typename EQ>
inline
I
rw_no_duplicates::next_not_equal(I iter, I, EQ)
{
    return ++iter;
}


template <typename C, typename K, typename EQ>
inline
typename C::size_type
rw_no_duplicates::erase(C& container, const K& key, EQ)
{
    const typename C::const_iterator iter = container.find(key);
    if (iter != container.end()) {
        container.erase(iter);
        return 1;
    }
    return 0;
}


template <typename C, typename V, typename EQ>
inline
std::pair<typename C::iterator, bool>
rw_allow_duplicates::insert(C& container, const V& val, EQ key_equal)
{
    typename C::iterator inner_iter =
        std::find_if(container.begin(), container.end(),
                     rw_bind2nd_ref(key_equal, val));
    inner_iter = container.insert(inner_iter, val);
    return std::make_pair(inner_iter, true);
}


#if !defined(RW_NO_RVALUE_REFERENCES)
template <typename C, typename V, typename EQ>
inline
std::pair<typename C::iterator, bool>
rw_allow_duplicates::insert(C& container, V && val, EQ key_equal)
{
    typename C::iterator inner_iter =
        std::find_if(container.begin(), container.end(),
                     rw_bind2nd_ref(key_equal, val));
    inner_iter = container.insert(inner_iter, rw_move(val));
    return std::make_pair(inner_iter, true);
}
#endif


template <typename C, typename K, typename EQ>
inline
typename C::size_type
rw_allow_duplicates::count(const C& container, const K& val, EQ key_equal)
{
    typename C::const_iterator inner_iter =
        std::find_if(container.begin(), container.end(),
                     rw_bind2nd_ref(key_equal, val));
    typename C::size_type count = 0;
    while (inner_iter != container.end() && key_equal(*inner_iter, val)) {
        ++count;
        ++inner_iter;
    }
    return count;
}


template <typename I, typename EQ>
inline
I
rw_allow_duplicates::next_not_equal(I start, I end, EQ key_equal)
{
    return std::find_if(start, end, rw_bind2nd_ref(std::not2(key_equal), *start));
}


template <typename C, typename K, typename EQ>
inline
typename C::size_type
rw_allow_duplicates::erase(C& container, const K& key, EQ key_equal)
{
    typename C::size_type count = 0;
    typename C::iterator iter = container.find(key);

    while (iter != container.end() && key_equal(*iter, key)) {
        ++count;
        iter = container.erase(iter);
    }

    return count;
}


template <typename T>
inline
const typename rw_value_based_key<T>::key_type&
rw_value_based_key<T>::key_from_value(const value_type& val)
{
    return val;
}


template <typename T>
template <typename EQ>
rw_value_based_key<T>::value_to_key_equal<EQ>::value_to_key_equal(EQ key_equal)
    : key_equal_(key_equal)
{}


template <typename T>
template <typename EQ>
bool
rw_value_based_key<T>::value_to_key_equal<EQ>::operator()(
    const value_type& lhs, const key_type& rhs) const
{
    return key_equal_(lhs, rhs);
}


template <typename T>
template <typename EQ>
rw_value_based_key<T>::value_to_value_equal<EQ>::value_to_value_equal(EQ key_equal)
    : key_equal_(key_equal)
{}


template <typename T>
template <typename EQ>
bool
rw_value_based_key<T>::value_to_value_equal<EQ>::operator()(
    const value_type& lhs, const value_type& rhs) const
{
    return key_equal_(lhs, rhs);
}


template <typename T>
inline
const typename rw_pair_based_key<T>::key_type&
rw_pair_based_key<T>::key_from_value(const value_type& val)
{
    return val.first;
}


template <typename T>
template <typename EQ>
rw_pair_based_key<T>::value_to_key_equal<EQ>::value_to_key_equal(EQ key_equal)
    : key_equal_(key_equal)
{}


template <typename T>
template <typename EQ>
bool
rw_pair_based_key<T>::value_to_key_equal<EQ>::operator()(
    const value_type& lhs, const key_type& rhs) const
{
    return key_equal_(lhs.first, rhs);
}


template <typename T>
template <typename EQ>
rw_pair_based_key<T>::value_to_value_equal<EQ>::value_to_value_equal(EQ key_equal)
    : key_equal_(key_equal)
{}


template <typename T>
template <typename EQ>
bool
rw_pair_based_key<T>::value_to_value_equal<EQ>::operator()(
    const value_type& lhs, const value_type& rhs) const
{
    return key_equal_(lhs.first, rhs.first);
}


template <typename T>
template <typename Hash>
rw_pair_based_key<T>::value_hasher<Hash>::value_hasher(Hash hash)
    : hash_(hash)
{}


template <typename T>
template <typename Hash>
size_t
rw_pair_based_key<T>::value_hasher<Hash>::operator()(
    const value_type& val) const
{
    return hash_(val.first);
}


template <typename C, typename T>
inline
bool
rw_container_equal<C, rw_no_duplicates, rw_value_based_key<T> >::operator()(
    const C& lhs, const C& rhs) const
{
    typedef typename C::const_iterator const_iterator;

    if (lhs.size() != rhs.size()) {
        return false;
    }

    for (const_iterator lhs_iter = lhs.begin();
            lhs_iter != lhs.end(); ++lhs_iter) {
        const const_iterator rhs_iter =
            rhs.find(rw_value_based_key<T>::key_from_value(*lhs_iter));
        if (rhs_iter == rhs.end()) {
            return false;
        }
    }
    return true;
}


template <typename C, typename T>
inline
bool
rw_container_equal<C, rw_allow_duplicates, rw_value_based_key<T> >::operator()(
    const C& lhs, const C& rhs) const
{
    typedef typename C::const_iterator const_iterator;
    typedef typename C::size_type size_type;
    typedef typename C::difference_type difference_type;

    if (lhs.size() != rhs.size()) {
        return false;
    }

    const_iterator lhs_iter = lhs.begin();

    while (lhs_iter != lhs.end()) {
        const const_iterator lhs_next = std::find_if(lhs_iter, lhs.end(),
                                        rw_bind2nd_ref(std::not2(lhs.key_eq()),
                                                *lhs_iter));
        const difference_type count = rw_distance(lhs_iter, lhs_next);
        RW_ASSERT(count >= 0);
        if (static_cast<size_type>(count) != rhs.count(*lhs_iter)) {
            return false;
        }
        lhs_iter = lhs_next;
    };
    return true;
}


template <typename C, typename T>
inline
bool
rw_container_equal<C, rw_no_duplicates, rw_pair_based_key<T> >::operator()(
    const C& lhs, const C& rhs) const
{
    typedef typename C::const_iterator const_iterator;

    if (lhs.size() != rhs.size()) {
        return false;
    }

    for (const_iterator i = lhs.begin(); i != lhs.end(); ++i) {
        const const_iterator j = rhs.find(i->first);
        if (j == rhs.end()) {       // must have same keys
            return false;
        }
        // note use of op==() below. No relationship to EQ
        if (!(j->second == i->second)) { // and same values
            return false;
        }
    }
    return true;
}


template <typename C, typename T>
inline
bool
rw_container_equal<C, rw_allow_duplicates, rw_pair_based_key<T> >::operator()(
    const C& lhs, const C& rhs) const
{
    typedef typename C::const_iterator const_iterator;
    typedef typename C::key_equal key_equal;
    typedef typename C::key_type key_type;
    typedef rw_slist<typename C::const_iterator> iter_list;
    typedef typename iter_list::iterator iter_list_iterator;

    if (lhs.size() != rhs.size()) {
        return false;
    }

    // Check lhs map against rhs map for equality. Two multimaps are
    // considered equal if they have identical multisets of <key, value>
    // pairs. But the keys may be stored in any order depending on the Hash
    // function. However, for any key, all the values that match that key
    // will be clustered together.
    const_iterator lhs_iter = lhs.begin();  // lhs_iter iterates over lhs map.
    iter_list rhs_iter_list;
    iter_list node_cache;
    const key_equal eq = lhs.key_eq();

    // The <key, value> pairs in rhs map
    // that match a single <key, value> pair in lhs map.
    while (lhs_iter != lhs.end()) {
        const key_type& key = lhs_iter->first;  // Key in lhs map to check.

        const std::pair<const_iterator, const_iterator> rhs_range =
            rhs.equal_range(key);

        // If a key in lhs map is not equal to any key in rhs map
        // then the maps are not equal.
        if (rhs_range.first == rhs.end()) {
            return false;
        }

        for (const_iterator i = rhs_range.first; i != rhs_range.second; ++i) {
            // If a cached list node is available, re-use it.
            if (!node_cache.empty()) {
                node_cache.front() = i;
                rhs_iter_list.splice(rhs_iter_list.end(),
                                     node_cache, node_cache.begin());
            }
            else {
                rhs_iter_list.push_back(i);
            }
        }

        // Check each key in lhs map with all the values
        // that match lhs key in the rhs map.
        while (!rhs_iter_list.empty() && eq(lhs_iter->first, key)) {
            bool found = false;

            // Check to see that each candidate has the same value
            // as the pair in lhs map
            for (iter_list_iterator rhs_iter = rhs_iter_list.begin();
                    rhs_iter != rhs_iter_list.end(); ++rhs_iter) {
                if ((*rhs_iter)->second == lhs_iter->second) {
                    // Candidate value matches, go to next candidate and
                    // store the node in the cache.
                    node_cache.splice(node_cache.end(),
                                      rhs_iter_list, rhs_iter);
                    found = true;
                    break;
                }
            }

            if (!found) {  // Same key has different value so maps not equal.
                return false;
            }

            if (++lhs_iter == lhs.end()) {
                break;
            }
        }

        // If there are still value candidates left for the rhs map for lhs
        // key, then maps not equal. If the end of lhs map has not been
        // reached and there are still keys to check in lhs map, then the
        // maps aren't equal.
        if (!rhs_iter_list.empty() ||
                (lhs_iter != lhs.end() && eq(lhs_iter->first, key))) {
            return false;
        }
    }
    return true;
}

/**
 * @internal
 * @relates rw_container_equal
 *
 * Compares \a lhs and \a rhs for equality. Returns \c true if \a lhs and
 * \a rhs are equal according to their duplicates_policy and key_policy,
 * \c false otherwise.
 */
template <typename C>
static
bool rw_container_eq(const C& lhs, const C& rhs)
{
    return rw_container_equal<C, typename C::duplicates_policy, typename C::key_policy>()(lhs, rhs);
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
typename rw_hashtable<T, Hash, EQ, A, DP, KP>::hasher
rw_hashtable<T, Hash, EQ, A, DP, KP>::hash_function() const
{
    return hash_;
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
typename rw_hashtable<T, Hash, EQ, A, DP, KP>::key_equal
rw_hashtable<T, Hash, EQ, A, DP, KP>::
key_eq() const
{
    return eq_;
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
rw_hashtable<T, Hash, EQ, A, DP, KP>::rw_hashtable(size_type cap,
        const hasher& h, const key_equal& eq)
    : buckets_(cap ? cap : RW_DEFAULT_CAPACITY), hash_(h), eq_(eq)
{
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
rw_hashtable<T, Hash, EQ, A, DP, KP>::rw_hashtable(
    const rw_hashtable& other)
    : buckets_(other.buckets_), hash_(other.hash_), eq_(other.eq_)
{
}


#if !defined(RW_NO_RVALUE_REFERENCES)
template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
rw_hashtable<T, Hash, EQ, A, DP, KP>::rw_hashtable(rw_hashtable && other)
#  if !defined(RW_NO_STDVECTOR_RVALUE_SUPPORT)
    : buckets_(rw_move(other.buckets_)),
      hash_(rw_move(other.hash_)), eq_(rw_move(other.eq_))
{
}
#  else
{
    swap(other);
}
#  endif
#endif // !RW_NO_RVALUE_REFERENCES


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
rw_hashtable<T, Hash, EQ, A, DP, KP>::~rw_hashtable()
{
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
rw_hashtable<T, Hash, EQ, A, DP, KP>&
rw_hashtable<T, Hash, EQ, A, DP, KP>::operator=(const rw_hashtable& rhs)
{
    if (&rhs != this) {
        buckets_ = rhs.buckets_;
        hash_ = rhs.hash_;
        eq_ = rhs.eq_;
    }
    return *this;
}


#if !defined(RW_NO_RVALUE_REFERENCES)
template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
rw_hashtable<T, Hash, EQ, A, DP, KP>&
rw_hashtable<T, Hash, EQ, A, DP, KP>::operator=(rw_hashtable && rhs)
{
    if (&rhs != this) {
#  if !defined(RW_NO_STDVECTOR_RVALUE_SUPPORT)
        buckets_ = rw_move(rhs.buckets_);
        hash_ = rw_move(rhs.hash_);
        eq_ = rw_move(rhs.eq_);
#  else
        swap(rhs);
#  endif
    }
    return *this;
}
#endif // !RW_NO_RVALUE_REFERENCES


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
typename rw_hashtable<T, Hash, EQ, A, DP, KP>::iterator
rw_hashtable<T, Hash, EQ, A, DP, KP>::begin()
{
    return iterator(buckets_);
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
typename rw_hashtable<T, Hash, EQ, A, DP, KP>::const_iterator
rw_hashtable<T, Hash, EQ, A, DP, KP>::begin() const
{
    return const_iterator(buckets_);
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
typename rw_hashtable<T, Hash, EQ, A, DP, KP>::const_iterator
rw_hashtable<T, Hash, EQ, A, DP, KP>::cbegin() const
{
    return const_iterator(buckets_);
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
typename rw_hashtable<T, Hash, EQ, A, DP, KP>::iterator
rw_hashtable<T, Hash, EQ, A, DP, KP>::end()
{
    const size_t bucket = buckets_.size() - 1;
    return iterator(buckets_, bucket, buckets_[bucket].end());
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
typename rw_hashtable<T, Hash, EQ, A, DP, KP>::const_iterator
rw_hashtable<T, Hash, EQ, A, DP, KP>::end() const
{
    const size_t bucket = buckets_.size() - 1;
    return const_iterator(buckets_, bucket, buckets_[bucket].end());
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
typename rw_hashtable<T, Hash, EQ, A, DP, KP>::const_iterator
rw_hashtable<T, Hash, EQ, A, DP, KP>::cend() const
{
    const size_t bucket = buckets_.size() - 1;
    return const_iterator(buckets_, bucket, buckets_[bucket].end());
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
bool
rw_hashtable<T, Hash, EQ, A, DP, KP>::empty() const
{
    return !size();
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
typename rw_hashtable<T, Hash, EQ, A, DP, KP>::size_type
rw_hashtable<T, Hash, EQ, A, DP, KP>::size() const
{
    size_type size = 0;
    for (typename bucket_list_type::const_iterator iter = buckets_.begin();
            iter != buckets_.end(); ++iter) {
        size += iter->size();
    }
    return size;
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
typename rw_hashtable<T, Hash, EQ, A, DP, KP>::size_type
rw_hashtable<T, Hash, EQ, A, DP, KP>::count(const key_type& key) const
{
    const size_type bucket = hash_function()(key) % buckets_.size();
    return duplicates_policy::count(buckets_[bucket], key,
                                    key_policy::value_to_key_eq(key_eq()));
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
typename rw_hashtable<T, Hash, EQ, A, DP, KP>::iterator
rw_hashtable<T, Hash, EQ, A, DP, KP>::find(const key_type& key)
{
    const size_type bucket = hash_(key) % buckets_.size();

    typename bucket_list_type::iterator outer_iter = buckets_.begin();
    std::advance(outer_iter, bucket);

    const typename bucket_type::iterator inner_iter =
        std::find_if(outer_iter->begin(), outer_iter->end(),
                     rw_bind2nd_ref(key_policy::value_to_key_eq(key_eq()), key));
    if (inner_iter == outer_iter->end()) {
        return end();
    }
    return iterator(buckets_, bucket, inner_iter);
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
typename rw_hashtable<T, Hash, EQ, A, DP, KP>::const_iterator
rw_hashtable<T, Hash, EQ, A, DP, KP>::find(const key_type& key) const
{
    const size_type bucket = hash_(key) % buckets_.size();

    typename bucket_list_type::const_iterator outer_iter = buckets_.begin();
    std::advance(outer_iter, bucket);

    const typename bucket_type::const_iterator inner_iter =
        std::find_if(outer_iter->begin(), outer_iter->end(),
                     rw_bind2nd_ref(key_policy::value_to_key_eq(key_eq()), key));
    if (inner_iter == outer_iter->end()) {
        return end();
    }
    return const_iterator(buckets_, bucket, inner_iter);
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
bool
rw_hashtable<T, Hash, EQ, A, DP, KP>::equal_by_keys(
    const rw_hashtable& rhs) const
{
    typedef typename rw_hashtable<T, Hash, EQ, A, DP, KP>::const_iterator const_iterator;

    if (size() != rhs.size()) {
        return false;
    }

    for (const_iterator lhs_iter = begin();
            lhs_iter != end(); ++lhs_iter) {
        const const_iterator rhs_iter =
            rhs.find(key_policy::key_from_value(*lhs_iter));
        if (rhs_iter == rhs.end()) {
            return false;
        }
    }

    return true;
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
std::pair < typename rw_hashtable<T, Hash, EQ, A, DP, KP>::iterator,
    typename rw_hashtable<T, Hash, EQ, A, DP, KP>::iterator >
    rw_hashtable<T, Hash, EQ, A, DP, KP>::equal_range(
        const key_type& key)
{
    const iterator lower = find(key);
    if (lower == end()) {
        return std::make_pair(lower, lower);
    }
    const iterator upper =
        duplicates_policy::next_not_equal(lower, end(),
                                          key_policy::value_to_value_eq(key_eq()));
    return std::make_pair(lower, upper);
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
std::pair < typename rw_hashtable<T, Hash, EQ, A, DP, KP>::const_iterator,
    typename rw_hashtable<T, Hash, EQ, A, DP, KP>::const_iterator >
    rw_hashtable<T, Hash, EQ, A, DP, KP>::equal_range(
        const key_type& key) const
{
    const const_iterator lower = find(key);
    if (lower == end()) {
        return std::make_pair(lower, lower);
    }
    const const_iterator upper =
        duplicates_policy::next_not_equal(lower, end(),
                                          key_policy::value_to_value_eq(key_eq()));
    return std::make_pair(lower, upper);
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
typename rw_hashtable<T, Hash, EQ, A, DP, KP>::size_type
rw_hashtable<T, Hash, EQ, A, DP, KP>::capacity() const
{
    return buckets_.size();
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
float
rw_hashtable<T, Hash, EQ, A, DP, KP>::fill_ratio() const
{
    return float(size()) / capacity();
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
void
rw_hashtable<T, Hash, EQ, A, DP, KP>::resize(size_type cap)
{
    if (cap == 0 || cap == capacity()) {
        return;
    }

    rw_hashtable other(cap, hash_function(), key_eq());
    bucket_list_type& other_buckets = other.buckets_;
    for (typename bucket_list_type::iterator outer = buckets_.begin();
            outer != buckets_.end(); ++outer) {
        while (!outer->empty()) {
            const typename bucket_type::iterator inner = outer->begin();
            const size_type loc =
                hash_function()(key_policy::key_from_value(*inner)) % other_buckets.size();
            bucket_type& bucket = other_buckets[loc];
            bucket.splice(bucket.end(), *outer, inner);
        }
    }
    swap(other);
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
void
rw_hashtable<T, Hash, EQ, A, DP, KP>::swap(rw_hashtable& other)
{
    std::swap(buckets_, other.buckets_);
    std::swap(hash_, other.hash_);
    std::swap(eq_, other.eq_);
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
std::pair<typename rw_hashtable<T, Hash, EQ, A, DP, KP>::iterator, bool>
rw_hashtable<T, Hash, EQ, A, DP, KP>::insert(const_reference val)
{
    const size_type bucket = hash_function()(key_policy::key_from_value(val)) % buckets_.size();
    typename bucket_list_type::iterator outer_iter = buckets_.begin();
    std::advance(outer_iter, bucket);

    const std::pair<typename bucket_type::iterator, bool> ret =
        duplicates_policy::insert(*outer_iter, val,
                                  key_policy::value_to_value_eq(key_eq()));
    return std::make_pair(iterator(buckets_, bucket, ret.first), ret.second);
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
typename rw_hashtable<T, Hash, EQ, A, DP, KP>::iterator
rw_hashtable<T, Hash, EQ, A, DP, KP>::insert(const_iterator, const_reference val)
{
    const std::pair<iterator, bool> ret = insert(val);
    return ret.first;
}


#if !defined(RW_NO_RVALUE_REFERENCES)
template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
std::pair<typename rw_hashtable<T, Hash, EQ, A, DP, KP>::iterator, bool>
rw_hashtable<T, Hash, EQ, A, DP, KP>::insert(value_type && val)
{
    const size_type bucket = hash_(key_policy::key_from_value(val)) % buckets_.size();
    typename bucket_list_type::iterator outer_iter = buckets_.begin();
    std::advance(outer_iter, bucket);

    const std::pair<typename bucket_type::iterator, bool> ret =
        duplicates_policy::insert(*outer_iter, rw_move(val),
                                  key_policy::value_to_value_eq(key_eq()));
    return std::make_pair(iterator(buckets_, bucket, ret.first), ret.second);
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
typename rw_hashtable<T, Hash, EQ, A, DP, KP>::iterator
rw_hashtable<T, Hash, EQ, A, DP, KP>::insert(const_iterator, value_type && val)
{
    const std::pair<iterator, bool> ret = insert(rw_move(val));
    return ret.first;
}
#endif


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
typename rw_hashtable<T, Hash, EQ, A, DP, KP>::size_type
rw_hashtable<T, Hash, EQ, A, DP, KP>::erase(const key_type& key)
{
    return duplicates_policy::erase(*this, key,
                                    key_policy::value_to_key_eq(key_eq()));
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
typename rw_hashtable<T, Hash, EQ, A, DP, KP>::iterator
rw_hashtable<T, Hash, EQ, A, DP, KP>::erase(const_iterator iter)
{
    const typename bucket_type::iterator inner_iter =
        buckets_[iter.bucket()].erase(iter.inner());
    return iterator(buckets_, iter.bucket(), inner_iter);
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
typename rw_hashtable<T, Hash, EQ, A, DP, KP>::iterator
rw_hashtable<T, Hash, EQ, A, DP, KP>::erase(const_iterator first,
        const_iterator last)
{
    if (first != end()) {
        // Retrieve a non-const iterator for first by erasing an empty
        // range from the bucket containing first.
        const typename bucket_type::iterator inner =
            buckets_[first.bucket()].erase(first.inner(), first.inner());
        iterator iter(buckets_, first.bucket(), inner);
        while (const_iterator(iter) != last) {
            iter = erase(iter);
        }
        return iter;
    }
    return end();
}


template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
void
rw_hashtable<T, Hash, EQ, A, DP, KP>::clear()
{
    for (typename bucket_list_type::iterator iter = buckets_.begin();
            iter != buckets_.end(); ++iter) {
        iter->clear();
    }
}


/**
 * @relates rw_hashtable
 *
 * Returns \c true if \a lhs and \a rhs have the same number of elements,
 * and for each item in \a lhs there is an item in \a rhs that compares
 * equal according to the equality function object.
 */
template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
bool
operator==(const rw_hashtable<T, Hash, EQ, A, DP, KP>& lhs,
           const rw_hashtable<T, Hash, EQ, A, DP, KP>& rhs)
{
    return rw_container_eq(lhs, rhs);
}


/**
 * @relates rw_hashtable
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T, class Hash, class EQ, class A, class DP, template <class T2> class KP>
inline
bool
operator!=(const rw_hashtable<T, Hash, EQ, A, DP, KP>& lhs,
           const rw_hashtable<T, Hash, EQ, A, DP, KP>& rhs)
{
    return !(lhs == rhs);
}

#endif /* RW_TOOLS_STDEX_HASHTABLE_H_ */
