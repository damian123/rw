#ifndef RW_TOOLS_STDEX_HASHMSET_H_
#define RW_TOOLS_STDEX_HASHMSET_H_

/**********************************************************************
 *
 * $Id: //tools/13/rw/stdex/hashmset.h#6 $
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
 * @brief Maintains a collection of some type \c T, in which there may be
 * many equivalent instances of \c T.
 *
 * Class rw_hashmultiset maintains a collection of \c T, implemented as a
 * hash table in which there may be many equal instances of \c T. Since this
 * is a \e value-based collection, objects are \e copied into and out of the
 * collection. As with all classes that meet the ANSI <i>associative
 * container</i> specification, rw_hashmultiset provides for iterators that
 * reference its elements. Since the contents of rw_hashmultiset are in
 * pseudo-random order, the only iterator ranges that will usually make
 * sense are the results of calling equal_range(), and the entire range from
 * begin() to end().
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
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/stdex/hashmset.h>
 * rw_hashmultiset<T>;
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class T, class Hash = RWTHash<T>, class EQ = std::equal_to<T>, class A = std::allocator<T> >
class rw_hashmultiset
{
public:

    /**
     * @internal
     *
     * A type representing the underlying container type.
     */
    typedef rw_hashtable<T, Hash, EQ, A, rw_allow_duplicates, rw_value_based_key> impl_type;

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
     * A type representing the hash function.
     */
    typedef typename impl_type::hasher hasher;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @deprecated As of SourcePro 12.5, use
     * \link rw_hashset::hasher hasher \endlink instead.
     *
     * A type representing the hash function.
     */
    typedef typename impl_type::hasher
    RW_DEPRECATE_TYPE("Use rw_hashmultiset::hasher instead") key_hash_type;
#endif

    /**
     * A type representing the equality function.
     *
     * @internal
     * For backward compatibility with the key_equal() member function, we
     * cannot provide a typedef named in a consistent manner with the C++
     * Standard Library unordered associative containers.
     */
    typedef typename impl_type::key_equal key_equal_type;

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
     * A type that provides a pointer to an element in the container.
     */
    typedef typename impl_type::pointer pointer;

    /**
     * A type that provides a \c const pointer to an element in the
     * container.
     */
    typedef typename impl_type::const_pointer const_pointer;

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
     * A type that provides a forward iterator over the elements in the
     * container.
     */
    typedef typename impl_type::const_iterator iterator;

    /**
     * A type that provides a \c const forward iterator over the elements in
     * the container.
     */
    typedef typename impl_type::const_iterator const_iterator;

    /**
     * @internal
     *
     * There is no standard specification for hash tables in the STL, but
     * they are more similar to Associative Containers than anything else.
     * STL containers are supposed to have a typedef key_compare for their
     * comparators. Since items in a hash table are not ordered in that way,
     * we are providing a trivial comparator that always returns false (from
     * STL's point of view that means everything is equivalent). This is
     * mainly to guard against over-achieving compilers that insist on
     * compiling even those template functions that are never used. This
     * will help in using hashed collections in code that makes use of their
     * commonality of interface with the standard Associative Containers.
     */
    struct pseudo_compare : public std::binary_function<T, T, bool> {
        /**
         * Returns false regardless of the arguments.
         */
        bool operator()(const T&, const T&) const {
            return false;
        }
    };

    typedef pseudo_compare key_compare;

    key_compare key_comp();

    hasher key_hash() const;

    key_equal_type key_equal() const;

    /**
     * Constructs an empty rw_hashmultiset with \a cap buckets, using \a h
     * as the hash function object, and \a eq as the equality function
     * object.
     *
     * @note If the value specified for \a cap is zero, the default number
     * of buckets is used.
     */
    rw_hashmultiset(size_type cap = RW_DEFAULT_CAPACITY,
                    const hasher& h = hasher(),
                    const key_equal_type& eq = key_equal_type());

    /**
     * Constructs an rw_hashmultiset containing a copy of the elements in the
     * range [\a first, \a last). The rw_hashmultiset instance has \a cap
     * buckets, uses \a h as its hash function object, and \a eq as the
     * equality function object.
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     *
     * @note If the value specified for \a cap is zero, the default number
     * of buckets is used.
     */
    template <typename InputIterator>
    rw_hashmultiset(InputIterator first,
                    InputIterator last,
                    size_type cap = RW_DEFAULT_CAPACITY,
                    const hasher& h = hasher(),
                    const key_equal_type& eq = key_equal_type());

    /**
     * Constructs an rw_hashmultiset that is a copy of \a other. Each
     * element from \a other is copied into self.
     */
    rw_hashmultiset(const rw_hashmultiset& other);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed rw_hashmultiset takes ownership of
     * the data owned by \a other.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference
     * support.
     */
    rw_hashmultiset(rw_hashmultiset && other);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * The destructor releases the memory used by the container's
     * implementation.
     */
    ~rw_hashmultiset();

    /**
     * Replaces the contents of self with copies of the contents of \a rhs.
     * The capacity, hash function object and equality function object are
     * replaced by the respective objects from \a rhs.
     */
    rw_hashmultiset& operator=(const rw_hashmultiset& rhs);

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
    rw_hashmultiset& operator=(rw_hashmultiset && rhs);
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
    bool
    empty() const;

    /**
     * Returns the number of items in the container.
     */
    size_type
    size() const;

    /**
     * Returns the number of items in self that compare equal to \a key
     * according to the associated equality function object.
     */
    size_type
    count(const_reference key) const;

    /**
     * Returns the first item in self that compares equal to \a key
     * according to the associated equality function object. If no items in
     * the container compare equal to \a key, returns end().
     */
    iterator
    find(const_reference key) const;

    /**
     * Equivalent to <tt>equal_range(\a key).first</tt>.
     */
    iterator
    lower_bound(const_reference key) const;

    /**
     * Equivalent to <tt>equal_range(\a key).second</tt>.
     */
    iterator
    upper_bound(const_reference key) const;

    /**
     * Returns the bounds of the subrange representing all of the values in
     * the container that compare equal to \a key according to the
     * associated equality function object. If no items in the container
     * compare equal to \a key, returns end() for both iterators.
     */
    std::pair<iterator, iterator>
    equal_range(const_reference key) const;

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
    swap(rw_hashmultiset& other);

    /**
     * Inserts a copy of \a val into the container. Returns an iterator to
     * the new item along with the status \c true.
     */
    std::pair<iterator, bool>
    insert(const_reference val);

    /**
     * Inserts a copy of \a val into the container. Returns an iterator to
     * the new item along with the status \c true.
     *
     * The parameter \a hint is ignored.
     */
    iterator
    insert(iterator hint, const_reference val);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Inserts \a val into the container. Returns an iterator to the new
     * item along with the status \c true.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference
     * support.
     */
    std::pair<iterator, bool>
    insert(value_type && val);

    /**
     * Inserts \a val into the container. Returns an iterator to the new
     * item along with the status \c true.
     *
     * The parameter \a hint is ignored.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference
     * support.
     */
    iterator
    insert(iterator hint, value_type && val);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * For each value in the range [\a first, \a last), inserts a copy of
     * the value into self. Returns the number of elements inserted.
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
     * Removes each item in the container that compares equal to \a key
     * according to the equality function object. Returns the number of
     * items that were removed.
     */
    size_type
    erase(const_reference key);

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
    erase(iterator first, iterator last);

    /**
     * Removes all of the items in the container.
     */
    void clear();

private:

    impl_type impl_;

    template <class T1, class H1, class E1, class A1>
    friend bool operator==(const rw_hashmultiset<T1, H1, E1, A1>& lhs,
                           const rw_hashmultiset<T1, H1, E1, A1>& rhs);

    template <class T1, class H1, class E1, class A1>
    friend bool operator!=(const rw_hashmultiset<T1, H1, E1, A1>& lhs,
                           const rw_hashmultiset<T1, H1, E1, A1>& rhs);
};


template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::key_compare
rw_hashmultiset<T, Hash, EQ, A>::key_comp()
{
    return key_compare();
}

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::hasher
rw_hashmultiset<T, Hash, EQ, A>::key_hash() const
{
    return impl_.hash_function();
}

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::key_equal_type
rw_hashmultiset<T, Hash, EQ, A>::key_equal() const
{
    return impl_.key_eq();
}

template <class T, class Hash, class EQ, class A>
template <typename InputIterator>
inline
rw_hashmultiset<T, Hash, EQ, A>::rw_hashmultiset(InputIterator first, InputIterator last,
        size_type cap, const hasher& h,
        const key_equal_type& eq)
    : impl_(first, last, cap, h, eq)
{
}


template <class T, class Hash, class EQ, class A>
inline
rw_hashmultiset<T, Hash, EQ, A>::rw_hashmultiset(size_type cap, const hasher& h,
        const key_equal_type& eq)
    : impl_(cap, h, eq)
{
}

template <class T, class Hash, class EQ, class A>
inline
rw_hashmultiset<T, Hash, EQ, A>::rw_hashmultiset(const rw_hashmultiset& set)
    : impl_(set.impl_)
{
}

#if !defined(RW_NO_RVALUE_REFERENCES)
template <class T, class Hash, class EQ, class A>
inline
rw_hashmultiset<T, Hash, EQ, A>::rw_hashmultiset(rw_hashmultiset && other)
    : impl_(rw_move(other.impl_))
{
}
#endif // !RW_NO_RVALUE_REFERENCES

template <class T, class Hash, class EQ, class A>
inline
rw_hashmultiset<T, Hash, EQ, A>::~rw_hashmultiset()
{
}

template <class T, class Hash, class EQ, class A>
inline
rw_hashmultiset<T, Hash, EQ, A>&
rw_hashmultiset<T, Hash, EQ, A>::operator=(const rw_hashmultiset& rhs)
{
    if (&rhs != this) {
        impl_ = rhs.impl_;
    }
    return *this;
}

#if !defined(RW_NO_RVALUE_REFERENCES)
template <class T, class Hash, class EQ, class A>
inline
rw_hashmultiset<T, Hash, EQ, A>&
rw_hashmultiset<T, Hash, EQ, A>::operator=(rw_hashmultiset && rhs)
{
    if (&rhs != this) {
        impl_ = rw_move(rhs.impl_);
    }
    return *this;
}
#endif // !RW_NO_RVALUE_REFERENCES

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::iterator
rw_hashmultiset<T, Hash, EQ, A>::begin()
{
    return impl_.begin();
}

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::const_iterator
rw_hashmultiset<T, Hash, EQ, A>::begin() const
{
    return impl_.begin();
}

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::const_iterator
rw_hashmultiset<T, Hash, EQ, A>::cbegin() const
{
    return impl_.cbegin();
}

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::iterator
rw_hashmultiset<T, Hash, EQ, A>::end()
{
    return impl_.end();
}

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::const_iterator
rw_hashmultiset<T, Hash, EQ, A>::end() const
{
    return impl_.end();
}

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::const_iterator
rw_hashmultiset<T, Hash, EQ, A>::cend() const
{
    return impl_.cend();
}

template <class T, class Hash, class EQ, class A>
inline
bool
rw_hashmultiset<T, Hash, EQ, A>::empty() const
{
    return impl_.empty();
}

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::size_type
rw_hashmultiset<T, Hash, EQ, A>::size() const
{
    return impl_.size();
}

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::size_type
rw_hashmultiset<T, Hash, EQ, A>::count(const_reference key) const
{
    return impl_.count(key);
}

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::iterator
rw_hashmultiset<T, Hash, EQ, A>::find(const_reference key) const
{
    return impl_.find(key);
}

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::iterator
rw_hashmultiset<T, Hash, EQ, A>::lower_bound(const_reference key) const
{
    return impl_.find(key);
}

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::iterator
rw_hashmultiset<T, Hash, EQ, A>::upper_bound(const_reference key) const
{
    return impl_.equal_range(key).second;
}

template <class T, class Hash, class EQ, class A>
inline
std::pair < typename rw_hashmultiset<T, Hash, EQ, A>::iterator,
    typename rw_hashmultiset<T, Hash, EQ, A>::iterator >
    rw_hashmultiset<T, Hash, EQ, A>::equal_range(const_reference key) const
{
    return impl_.equal_range(key);
}

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::size_type
rw_hashmultiset<T, Hash, EQ, A>::capacity() const
{
    return impl_.capacity();
}

template <class T, class Hash, class EQ, class A>
inline
float
rw_hashmultiset<T, Hash, EQ, A>::fill_ratio() const
{
    return impl_.fill_ratio();
}

template <class T, class Hash, class EQ, class A>
inline
void
rw_hashmultiset<T, Hash, EQ, A>::resize(size_type sz)
{
    impl_.resize(sz);
}

template <class T, class Hash, class EQ, class A>
inline
void
rw_hashmultiset<T, Hash, EQ, A>::swap(rw_hashmultiset& other)
{
    impl_.swap(other.impl_);
}

template <class T, class Hash, class EQ, class A>
inline
std::pair<typename rw_hashmultiset<T, Hash, EQ, A>::iterator, bool>
rw_hashmultiset<T, Hash, EQ, A>::insert(const_reference val)
{
#if defined(_RWSTD_NO_MEMBER_TEMPLATES)
    std::pair<typename impl_type::iterator, bool> ret = impl_.insert(val);
    return std::pair<iterator, bool>(ret.first, ret.second);
#else
    return impl_.insert(val);
#endif
}

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::iterator
rw_hashmultiset<T, Hash, EQ, A>::insert(iterator hint, const_reference val)
{
    return impl_.insert(hint, val);
}

#if !defined(RW_NO_RVALUE_REFERENCES)
template <class T, class Hash, class EQ, class A>
inline
std::pair<typename rw_hashmultiset<T, Hash, EQ, A>::iterator, bool>
rw_hashmultiset<T, Hash, EQ, A>::insert(value_type && val)
{
    return impl_.insert(rw_move(val));
}

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::iterator
rw_hashmultiset<T, Hash, EQ, A>::insert(iterator hint, value_type && val)
{
    return impl_.insert(hint, rw_move(val));
}
#endif // !RW_NO_RVALUE_REFERENCES

template <class T, class Hash, class EQ, class A>
template <typename InputIterator>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::size_type
rw_hashmultiset<T, Hash, EQ, A>::insert(InputIterator first, InputIterator last)
{
    return impl_.insert(first, last);
}

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::size_type
rw_hashmultiset<T, Hash, EQ, A>::erase(const_reference key)
{
    return impl_.erase(key);
}

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::iterator
rw_hashmultiset<T, Hash, EQ, A>::erase(iterator iter)
{
    return impl_.erase(iter);
}

template <class T, class Hash, class EQ, class A>
inline
typename rw_hashmultiset<T, Hash, EQ, A>::iterator
rw_hashmultiset<T, Hash, EQ, A>::erase(iterator first, iterator last)
{
    return impl_.erase(first, last);
}

template <class T, class Hash, class EQ, class A>
inline
void
rw_hashmultiset<T, Hash, EQ, A>::clear()
{
    impl_.clear();
}

/**
 * @relates rw_hashmultiset
 *
 * Returns \c true if \a lhs and \a rhs have the same number of elements,
 * and for each item in \a lhs there is an item in \a rhs which compares
 * equal according to the equality function object.
 */
template <class T, class Hash, class EQ, class A>
inline
bool
operator==(const rw_hashmultiset<T, Hash, EQ, A>& lhs,
           const rw_hashmultiset<T, Hash, EQ, A>& rhs)
{
    return lhs.impl_ == rhs.impl_;
}

/**
 * @relates rw_hashmultiset
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T, class Hash, class EQ, class A>
inline
bool
operator!=(const rw_hashmultiset<T, Hash, EQ, A>& lhs,
           const rw_hashmultiset<T, Hash, EQ, A>& rhs)
{
    return lhs.impl_ != rhs.impl_;
}

#endif /* RW_TOOLS_STDEX_HASHMSET_H_ */
