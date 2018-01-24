#ifndef RW_TOOLS_TPHMAP_H
#define RW_TOOLS_TPHMAP_H

/**********************************************************************
 *
 * tphdict.h - RWHMapPtr<Key,T,H,EQ>
 *     : pointer-based key/data dictionary wrapper for STL-like rw_hashmap
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tphdict.h#4 $
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
#  include "rw/xphdict.h"
#else
#  include <rw/epaux.h>
#  include <rw/epfunc.h>
#  include <rw/stdex/hashmap.h>
#  include <rw/tools/algorithm.h>
#  include <rw/tools/traits/RWTEnableIf.h>
#  include <rw/tools/traits/RWTIsConvertible.h>

#if !defined(RW_DISABLE_DEPRECATED)

template <class K, class T, class H, class EQ, class A>
class RWTPtrHashMapConstIterator;

#endif

/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @brief A hash-based associative container of pointer types.
 *
 * This class maintains a pointer-based collection of associations
 * of type <b>std::pair<K* const, T*></b>. These pairs are stored
 * according to a hash object of type \c H. Class \c T
 * is the type pointed to by the items in the collection.
 * Class \c A is an allocator of objects of class \c T. For more information
 * about using custom allocators, please see the <i>Essential Tools Module
 * User's Guide</i>.
 *
 * \c H must provide a \c const function that takes a single argument
 * convertible to type \c K and returns a value of type \c size_t.
 *
 * @note
 * Any two keys that are equivalent \e must hash to the same value.
 *
 * Key equality is determined by an equality function of type \c EQ,
 * which takes two arguments convertible to type \c K and returns a
 * value of type \c bool.
 *
 * @note
 * Any two keys that are equivalent are disallowed for this container.
 *
 * This class does not allow duplicate keys, that is, keys that are
 * equivalent to a key already in the collection. Use the class
 * \link RWTPtrHashMultiMap RWTPtrHashMultiMap<K,T,H,EQ,A>\endlink
 * if you need multiple keys to be equivalent to each other. Equality
 * is based on the comparison object, not on \c operator==().
 *
 * The \c value type must have \c operator==() defined. This requirement
 * is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tphdict.h>
 * RWTPtrHashMap<K,T,H,EQ,A> m;
 * @endcode
 *
 * @section related Related Classes
 *
 * Class \link RWTPtrHashMultiMap RWTPtrHashMultiMap<K,T,H,EQ,A>\endlink
 * offers the same interface to a pointer-based collection that accepts
 * multiple keys that compare equal to each other.
 *
 * Class \link rw_hashmap rw_hashmap<K*,T*,hasher,key_equal,A>
 * \endlink is the C++ Standard Library-style collection that serves as
 * the underlying implementation for this collection.
 *
 * @section persistence Persistence
 *
 * Isomorphic
 *
 * @section example Example
 *
 * @code
 * //
 * // tphmap.cpp
 * //
 * #include<iostream>
 * #include<functional>
 * #include<rw/tphdict.h>
 * #include<rw/cstring.h>
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
 *     RWCString snd = "Second";
 *
 *     RWTPtrHashMap<RWCString, int, simple_hash, std::equal_to<RWCString> >
 *         contest;
 *
 *     contest.insert(new RWCString("First"), new int(7));
 *     contest.insert(&snd,new int(3));
 *
 *     // duplicate insertion rejected
 *     contest.insert(&snd,new int(6));
 *
 *     contest.insert(new RWCString("Third"), new int(2));
 *
 *     std::cout << "There was "
 *               << contest.occurrencesOf(new RWCString("Second"))
 *               << " second place winner." << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 * There was 1 second place winner.
 * @endcode
 */
template <class K, class T, class H = RWTHash<K>, class EQ = std::equal_to<K>, class A = std::allocator<K*> >
class RWTPtrHashMap
{
public:

    /**
     * A type representing an equality function that dereferences the
     * arguments before comparing.
     */
    typedef rw_deref_compare<EQ, K> key_equal;

    /**
     * A type representing a hash function that dereferences the arguments
     * before comparing.
     */
    typedef rw_deref_hash<H, K> hasher;

    /**
     * A type representing the underlying implementation container.
     */
    typedef rw_hashmap < K*, T*, hasher, key_equal, A> container_type;

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
     * A type representing the mapped type of the container.
     */
    typedef typename container_type::mapped_type mapped_type;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrHashMap::key_equal \c key_equal \endlink instead.
     *
     * A type representing an equality function that dereferences the
     * arguments before comparing.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrHashMap::key_equal instead") rw_deref_compare<EQ, T> container_eq;

    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrHashMap::hasher \c hasher \endlink instead.
     *
     * A type representing a hash function that dereferences the arguments
     * before comparing.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrHashMap::hasher instead") rw_deref_hash<H, T> container_hash;

    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrHashMap::key_type \c key_type \endlink instead.
     *
     * Typedef for the type of key.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrHashMap::key_type instead") K* value_type_key;

    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrHashMap::mapped_type \c mapped_type \endlink instead.
     *
     * Typedef for the type of value.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrHashMap::mapped_type instead") T* value_type_data;

    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrHashMap::key_type \c key_type& \endlink instead.
     *
     * Typedef for a non-\c const reference to a key in this container.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrHashMap::key_type& instead") K*& reference_key;

    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrHashMap::mapped_type \c mapped_type& \endlink instead.
     *
     * Typedef for a non-\c const reference to the type of data in this container.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrHashMap::mapped_type& instead") T*& reference_data;

    /**
     * @deprecated As of SourcePro 12.5, use <tt>const K* const&</tt> instead.
     *
     * Typedef for a \c const reference to a key in this container.
     */
    typedef RW_DEPRECATE_TYPE("Use const K* const& instead") const K* const& const_reference_key;

    /**
     * @deprecated As of SourcePro 12.5, use <tt>const T* const&</tt> instead.
     *
     * Typedef for a \c const reference to the type of data in this container.
     */
    typedef RW_DEPRECATE_TYPE("Use const T* const& instead") const T* const& const_reference_data;
#endif


    /**
     * @internal
     * A type representing the same type as self.
     */
    typedef RWTPtrHashMap<K, T, H, EQ, A> this_type;

    /**
     * @internal
     * A type representing a comparison function that dereferences the
     * arguments before comparing.
     */
    typedef rw_deref_pair_less<value_type, value_type> deref_less;

    /**
     * @internal
     * A type representing an equality function that dereferences the
     * arguments before comparing.
     */
    typedef rw_deref_pair_equal<value_type, value_type> deref_equal;

    /**
     * @internal
     * A type representing a comparison function that dereferences the
     * key argument before comparing.
     */
    typedef rw_deref_pair_compare<std::less<K>, value_type > deref_pair_compare;

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
     * Invokes the function pointer \a fn on each association in the collection.
     * Client data may be passed through parameter \a d.
     */
    void apply(void(*fn)(const K*, const T*, void*), void* d) const;

    /**
     * @copydoc apply()
     */
    void apply(void(*fn)(const K*, mapped_type&, void*), void* d);

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @deprecated As of SourcePro 1, use apply() instead.
     */
    RW_DEPRECATE_FUNC("Use RWTPtrHashMap::apply() instead")
    void applyToKeyAndValue(void(*fn)(const K*, const T*, void*), void* d) const {
        apply(fn, d);
    }
#endif

    /**
     * @copydoc applyToKeyAndValue()
     */
    void applyToKeyAndValue(void(*fn)(const K*, mapped_type&, void*), void* d) {
        apply(fn, d);
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
     * Returns the number of associations in self.
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
     * Returns \c true if there exists an association \c a in self such that
     * the expression <tt>((*\a fn)(a,\a d))</tt> is \c true, otherwise
     * returns \c false. Client data may be passed through parameter \a d.
     */
    bool contains(bool(*fn)(value_type, void*), void* d) const {
        return std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d)) != end();
    }

    /**
     * Returns \c true if there exists a key \c j in self that compares
     * equal to <tt>*\a key</tt>, otherwise returns \c false.
     */
    bool contains(const K* key) const {
        return std().find(const_cast<key_type>(key)) != std().end();
    }

    /**
     * If there exists an association \c a in self such that the expression
     * <tt>((*\a fn)(a,\a d))</tt> is \c true, returns \c a, otherwise
     * returns <tt>#value_type(#rwnil,#rwnil)</tt>. Client data may be passed through
     * parameter \a d.
     */
    value_type find(bool(*fn)(value_type, void*), void* d) const {
        const_iterator ret = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        return (ret != end()) ? *ret : value_type();
    }

    /**
     * If there exists a key \c j in self that compares equal to <tt>*\a key</tt>,
     * returns \c j, otherwise returns #rwnil.
     */
    const K* find(const K* key) const {
        const_iterator i = std().find(const_cast<key_type>(key));
        return (i != std().end()) ? (*i).first : rwnil;
    }

    value_type find(const_reference key) const {
        const_iterator i = std().find(const_cast<key_type>(key.first));
        return (i != end()) ? *i : value_type();
    }

    /**
     * If there exists a key \c j in self that compares equal to <tt>*\a key</tt>,
     * returns the item associated with \c j, otherwise returns #rwnil.
     */
    mapped_type findValue(const K* key) {
        iterator i = std().find(const_cast<key_type>(key));
        return (i != std().end()) ? (*i).second : rwnil;
    }

    /**
     * @copydoc findValue()
     */
    const T* findValue(const K* key) const {
        const_iterator i = std().find(const_cast<key_type>(key));
        return (i != end()) ? (*i).second : rwnil;
    }

    /**
     * If there exists a key \c j in self that compares equal to <tt>*\a key</tt>,
     * assigns the item associated with \c j to \a tr and returns \c j,
     * otherwise returns #rwnil and leaves the value of \a tr unchanged.
     */
    const K* findKeyAndValue(const K* key, mapped_type& tr) {
        iterator i = std().find(const_cast<key_type>(key));
        if (i != std().end()) {
            tr = (*i).second;
            return (*i).first;
        }
        return rwnil;
    }

    /**
     * @copydoc findKeyAndValue()
     */
    const K* findKeyAndValue(const K* key, const T*& tr) const {
        const_iterator i = std().find(const_cast<key_type>(key));
        if (i != end()) {
            tr = (*i).second;
            return (*i).first;
        }
        return rwnil;
    }

    /**
     * Returns a pointer to the minimum key in the collection.
     * Type \c K must have well-defined less-than semantics
     * (<tt>K::operator<(const K&)</tt> or equivalent).
     */
    const K* minElement() const {
        RW_PRECONDITION(entries() != 0);
        return (*std::min_element(begin(), end(),
                                  deref_pair_compare())).first;
    }

    /**
     * Returns a pointer to the maximum key in the collection.
     * Type \c K must have well-defined less-than semantics
     * (<tt>K::operator<(const K&)</tt> or equivalent).
     */
    const K* maxElement() const {
        RW_PRECONDITION(entries() != 0);
        return (*std::max_element(begin(), end(),
                                  deref_pair_compare())).first;
    }

    /**
     * If <tt>std::find(\a key)</tt> would succeed, erases the pair associated
     * with \a key, places the value part in \a retVal, and returns \a key.
     * Otherwise, returns #rwnil.
     */
    key_type removeKeyAndValue(const K* key, mapped_type& retVal) {
        key_type ret = rwnil;
        iterator i = std().find(const_cast<key_type>(key));
        if (i != end()) {
            ret = (*i).first;
            retVal = (*i).second;
            std().erase(i);
        }
        return ret;
    }

    /**
     * Returns the number of associations \c a in self such that the
     * expression <tt>((*\a fn)(a,\a d))</tt> is \c true. Client data
     * may be passed through parameter \a d.
     */
    size_type occurrencesOf(bool(*fn)(value_type, void*), void* d) const {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Returns the number of keys \c j in self that compare equal to
     * <tt>*\a key</tt>.
     */
    size_type occurrencesOf(const K* key) const {
        return std().count(const_cast <key_type>(key));
    }

    /**
     * Removes the first association \c a in self such that the expression
     * <tt>((*\a fn)(a,\a d))</tt> is \c true and returns its key. Returns
     * #rwnil if there is no such association. Client data may be passed
     * through parameter \a d.
     */
    key_type remove(bool(*fn)(value_type, void*), void* d) {
        key_type ret = rwnil;
        iterator iter = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        if (iter != end()) {
            ret = (*iter).first;
            std().erase(iter);
        }
        return ret;
    }

    /**
     * Removes the first association with key \c j in self that compares
     * equal to <tt>*\a key</tt> and returns \c j. Returns #rwnil if there
     * is no such association.
     */
    key_type remove(const K* key) {
        key_type ret = rwnil;
        iterator iter = std().find(const_cast<key_type>(key));
        if (iter != std().end()) {
            ret = (*iter).first;
            std().erase(iter);
        }
        return ret;
    }

    /**
     * Removes all associations \c a in self such that the expression
     * <tt>((*\a fn)(a,\a d))</tt> is \c true. Returns the number of
     * associations removed. Client data may be passed through parameter \a d.
     */
    size_type removeAll(bool(*fn)(value_type, void*), void* d);

    /**
     * Removes all associations with key \c j in self that compare equal
     * to <tt>*\a key</tt>. Returns the number of associations removed.
     */
    size_type removeAll(const K* key) {
        return std().erase(const_cast<key_type>(key));
    }

    /**
     * Clears the collection by removing all items from self.
     */
    void clear() {
        std().clear();
    }

    /**
     * Removes all associations from the collection and uses <tt>operator
     * delete</tt> to destroy the objects pointed to by the keys and their
     * associated items. It is important that the equality object associated
     * with this container successfully exclude duplicate entries since using
     * this method when there are duplicates results in unpredictable
     * behavior.
     */
    void clearAndDestroy() {
        for (iterator it = begin(); it != end(); ++it) {
            delete(*it).first;
            delete(*it).second;
        }
        clear();
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
#if !defined(DOXYGEN)
    typename RWTEnableIf < !(RWTIsConvertible<InputIterator, key_type>::value && RWTIsConvertible<InputIterator, mapped_type>::value) >::type
    insert(InputIterator first, InputIterator last) {
#else
    void insert(InputIterator first, InputIterator last) {
#endif
        std().insert(first, last);
    }

    /**
     * Adds \a key with associated item \a a to the collection. Returns
     * \c true if the insertion is successful, otherwise returns \c false.
     * The function will return \c true unless the collection already holds
     * an association with the equivalent key.
     */
    bool insert(key_type key, mapped_type a) {
        return (std().insert(value_type(key, a))).second;
    }

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @deprecated As of SourcePro 1, use insert() instead.
     */
    RW_DEPRECATE_FUNC("Use RWTPtrHashMap::insert(key_type, mapped_type) instead")
    bool insertKeyAndValue(key_type key, mapped_type a) {
        return insert(key, a);
    }
#endif

    /**
     * Looks up \a key and returns a reference to its associated item.
     * If the key is not in the dictionary, is is added with an
     * associated uninitialized pointer of type \c #mapped_type. Because of
     * this, if there is a possibility that a key will not be in the
     * dictionary, this operator should only be used as an lvalue.
     */
    mapped_type& operator[](key_type key) {
        return impl_[key];
    }

    /**
     * Returns the number of buckets (slots) available in the underlying
     * hash representation.
     *
     * @see resize()
     */
    size_type capacity() const {
        return std().capacity();
    }

    /**
     * Returns the ratio entries() / capacity().
     */
    float fillRatio() const {
        return std().fill_ratio();
    }

    /**
     * Changes the capacity of self by creating a new hashmap with
     * a capacity of \a sz. Then copies every association in self into the
     * new container and finally swaps the internal representation
     * of the new container with the internal representation of self.
     */
    void resize(size_type sz) {
        std().resize(sz);
    }

    /**
     * Destroys all associations in self and replaces them by copying
     * all associations from \a rhs into self.
     */
    RWTPtrHashMap<K, T, H, EQ, A>&
    operator=(const RWTPtrHashMap<K, T, H, EQ, A>& rhs) {
        return operator=(rhs.impl_);
    }

    /**
     * @copydoc operator=()
     */
    RWTPtrHashMap<K, T, H, EQ, A>&
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
    RWTPtrHashMap<K, T, H, EQ, A>&
    operator=(RWTPtrHashMap<K, T, H, EQ, A> && rhs) {
        return operator=(rw_move(rhs.impl_));
    }

    /**
     * @copydoc operator=(RWTPtrHashMap<K,T,H,EQ,A>&&)
     */
    RWTPtrHashMap<K, T, H, EQ, A>&
    operator=(container_type && rhs) {
        if (&impl_ != &rhs) {
            impl_ = rw_move(rhs);
        }
        return *this;
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs an empty hashmap.
     */
    RWTPtrHashMap()
        : impl_(RW_DEFAULT_CAPACITY) { }

    /**
     * Constructs a pointer-based hashmap by copying all associations from \a m.
     */
    RWTPtrHashMap(const container_type& m) : impl_(m) { }
    /**
     * Copy constructor.
     */
    RWTPtrHashMap(const RWTPtrHashMap<K, T, H, EQ, A>& rws)
        : impl_(rws.impl_) { }

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed list takes ownership of the
     * data owned by \a map.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrHashMap(RWTPtrHashMap<K, T, H, EQ, A> && map)
        : impl_(rw_move(map.impl_)) { }

    /**
     * @copydoc RWTPtrHashMap(RWTPtrHashMap<K,T,H,EQ,A>&&)
     */
    RWTPtrHashMap(container_type && map)
        : impl_(rw_move(map)) { }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs a map by copying elements from the range
     * [\a first, \a last).
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
    template <typename InputIterator>
    RWTPtrHashMap(InputIterator first, InputIterator last)
        : impl_(first, last, RW_DEFAULT_CAPACITY) { }

    // backward compatible constructor:
    /**
     * Creates an empty hashmap that uses the hash object \a h and
     * has an initial capacity of \a sz.
     */
    RWTPtrHashMap(const H& h, size_type sz = RW_DEFAULT_CAPACITY)
        : impl_(sz ? sz : RW_DEFAULT_CAPACITY, h) { }

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWTPtrHashMap<K, T, H, EQ, A>& rhs) {
        std().swap(rhs.impl_);
    }

private:
    container_type impl_;
};


/**
 * @relates RWTPtrHashMultiSet
 *
 * @copydoc operator==(const RWTPtrHashMultiSet&, const RWTPtrHashMultiSet&);
 */
template <class K, class T, class H, class EQ, class A>
bool operator==(const RWTPtrHashMap<K, T, H, EQ, A>& lhs,
                const typename RWTPtrHashMap<K, T, H, EQ, A>::container_type& rhs)
{
    typedef typename RWTPtrHashMap<K, T, H, EQ, A>::const_iterator const_iterator;

    if (lhs.entries() != rhs.size()) {
        return false;
    }
    for (const_iterator iter = lhs.begin(); iter != lhs.end(); ++iter) {
        const_iterator ret = rhs.find((*iter).first);
        if (ret == rhs.end() || !(*(*iter).second == *(*ret).second)) {
            return false;
        }
    }
    return true;
}

/**
 * @relates RWTPtrHashMultiSet
 *
 * @copydoc operator!=(const RWTPtrHashMultiSet&, const RWTPtrHashMultiSet&);
 */
template <class K, class T, class H, class EQ, class A>
bool operator!=(const RWTPtrHashMap<K, T, H, EQ, A>& lhs,
                const typename RWTPtrHashMap<K, T, H, EQ, A>::container_type& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrHashMultiSet
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and if for
 * each key in the first container, the set of values associated with that
 * key is equal to the set of values associated with that key in the second
 * container. Keys and values are dereferenced before being compared.
 */
template <class K, class T, class H, class EQ, class A>
bool operator==(const RWTPtrHashMap<K, T, H, EQ, A>& lhs, const RWTPtrHashMap<K, T, H, EQ, A>& rhs)
{
    return lhs == rhs.std();
}

/**
 * @relates RWTPtrHashMultiSet
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class K, class T, class H, class EQ, class A>
bool operator!=(const RWTPtrHashMap<K, T, H, EQ, A>& lhs, const RWTPtrHashMap<K, T, H, EQ, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrHashMultiSet
 *
 * @copydoc operator==(const RWTPtrHashMultiSet&, const RWTPtrHashMultiSet&);
 */
template <class K, class T, class H, class EQ, class A>
bool operator==(const typename RWTPtrHashMap<K, T, H, EQ, A>::container_type& lhs,
                const RWTPtrHashMap<K, T, H, EQ, A>& rhs)
{
    return rhs == lhs;
}

/**
 * @relates RWTPtrHashMultiSet
 *
 * @copydoc operator!=(const RWTPtrHashMultiSet&, const RWTPtrHashMultiSet&);
 */
template <class K, class T, class H, class EQ, class A>
bool operator!=(const typename RWTPtrHashMap<K, T, H, EQ, A>::container_type& lhs,
                const RWTPtrHashMap<K, T, H, EQ, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrHashMap
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class K, class T, class H, class EQ, class A>
RWvostream& operator<<(RWvostream& strm, const RWTPtrHashMap<K, T, H, EQ, A>& coll);

/**
 * @relates RWTPtrHashMap
 *
 * Streams the collection \a coll to the RWFile \a file,
 * or a reference to it if it has already been saved.
 */
template <class K, class T, class H, class EQ, class A>
RWFile& operator<<(RWFile& file, const RWTPtrHashMap<K, T, H, EQ, A>& coll);

/**
 * @relates RWTPtrHashMap
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class K, class T, class H, class EQ, class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrHashMap<K, T, H, EQ, A>& coll);

/**
 * @relates RWTPtrHashMap
 *
 * Restores the contents of the collection \a coll by streaming
 * from the RWFile \a file.
 */
template <class K, class T, class H, class EQ, class A>
RWFile& operator>>(RWFile& file, RWTPtrHashMap<K, T, H, EQ, A>& coll);

/**
 * @relates RWTPtrHashMap
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, the application is
 * responsible for deleting it.
 */
template <class K, class T, class H, class EQ, class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrHashMap<K, T, H, EQ, A>*& p);

/**
 * @relates RWTPtrHashMap
 *
 * Looks at the next object from the streamed RWFile \a file
 * and either creates a new collection off the heap and sets \a p
 * to point to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, the application is
 * responsible for deleting it.
 */
template <class K, class T, class H, class EQ, class A>
RWFile& operator>>(RWFile& file, RWTPtrHashMap<K, T, H, EQ, A>*& p);

#  if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#    pragma implementation ("tphdict.cc")
#  endif

#  ifdef RW_NO_IMPLICIT_INCLUSION
#    include "rw/tphdict.cc"
#  endif

#  define RWTPtrHashDictionary RWTPtrHashMap
#  define RWTPtrHashDictionaryIterator RWTPtrHashMapIterator
#  define RWTPtrHashDictionaryConstIterator RWTPtrHashMapConstIterator

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTPtrHashMap::iterator to iterate over
 * the elements in this collection.
 *
 * @brief Deprecated. Provides an iterator for RWTPtrHashMap.
 *
 * RWTPtrHashMapIterator provides an iterator interface to the
 * C++ Standard Library-based collection represented by RWTPtrHashMap.
 * (An STL iterator interface is provided through \c RWTPtrHashMap::iterator.)
 *
 * Iteration over an RWTPtrHashMap is pseudo-random and dependent
 * on the capacity of the underlying hash table and the hash function
 * being used.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset(). The iterator becomes
 * valid after being advanced with either a pre-increment or operator()().
 *
 * For both operator++() and operator()(), iterating past the last
 * element returns a value equivalent to boolean \c false.
 * Continued increments return a value equivalent to \c false until
 * reset() is called.
 *
 * The value type must have \c operator==() defined. This requirement
 * is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include<rw/tphdict.h>
 * RWTPtrHashMap<K,T,H,EQ,A>m;
 * RWTPtrHashMapIterator<K,T,H,EQ,A>itr(m);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/tphdict.h>
 * #include <iostream>
 * #include <rw/cstring.h>
 *
 * int main() {
 *    RWTPtrHashMap
 *      <RWCString,int,RWTHasher<RWCString>,std::equal_to<RWCString> >
 *        age(RWCString::hash);
 *
 *    RWTPtrHashMapIterator
 *      <RWCString,int,RWTHasher<RWCString>,std::equal_to<RWCString> >
 *        itr(age);
 *
 *    age.insert(new RWCString("John"),new int(30));
 *    age.insert(new RWCString("Steve"),new int(17));
 *    age.insert(new RWCString("Mark"),new int(24));
 *
 *    for(;++itr;) {
 *      std::cout << *itr.key()
 *                << "\'s age is "
 *                << *itr.value()
 *                << std::endl;
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
 * John's age is 30
 * Mark's age is 24
 * Steve's age is 17
 * @endcode
 *
 */
template <class K, class T, class H = RWTHash<K>, class EQ = std::equal_to<K>, class A = std::allocator<K*> >
class RW_DEPRECATE_TYPE("Use RWTPtrHashMap::iterator instead") RWTPtrHashMapIterator
{
public:
    /**
     * Creates an iterator for the hashmap \a h. The iterator begins
     * in an undefined state and must be advanced before the first element
     * is accessible.
     */
    RWTPtrHashMapIterator(RWTPtrHashMap<K, T, H, EQ, A>& h)
        : rwmap_(&h), head_(true), tail_(false) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self references the first element.
     * If used when self references the last association in
     * the hashmap, self references an undefined value and the method
     * returns a value equivalent to \c false. Otherwise, the method
     * returns a value equivalent to \c true.
     */
    bool operator++() {  // Advance and test
        RW_ASSERT(!(head_ == true && tail_ == true));

        if (tail_) {
            /* do nothing */
        }
        else if (head_) {
            iter_ = rwmap_->begin();
            head_ = false;
        }
        else {
            ++iter_;
        }

        if (!tail_ && iter_ == rwmap_->end()) {
            tail_ = true;
        }

        RW_ASSERT(!(head_ == true && tail_ == true));

        return !tail_;
    }

    /**
     * Advances self to the next element, dereferences the resulting
     * iterator and returns its key. If the iterator has advanced past
     * the last item in the hashmap, returns #rwnil.
     */
    K* operator()() {
        return ++(*this) ? key() : rwnil;
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    RWTPtrHashMap<K, T, H, EQ, A>* container() const {
        return rwmap_;
    }

    /**
     * Returns the key portion of the association pointed to by self.
     * The return value is undefined if self is not referencing an association
     * within the map.
     */
    K* key() const {
        return (*iter_).first;
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
     * the first element of the collection \a m.
     */
    void reset(RWTPtrHashMap<K, T, H, EQ, A>& m) {
        head_ = true;
        tail_ = false;
        rwmap_ = &m;
    }

    /**
     * Returns the value portion of the association pointed to by self.
     * The return value is undefined if self is not referencing an association
     * within the map.
     */
    T* value() const {
        return (*iter_).second;
    }

private:
    RWTPtrHashMap<K, T, H, EQ, A>* rwmap_;
    typename RWTPtrHashMap<K, T, H, EQ, A>::iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);

    friend class RWTPtrHashMapConstIterator<K, T, H, EQ, A>;
};

/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTPtrHashMap::const_iterator to iterate
 * over the elements in this collection.
 *
 * @brief Deprecated. Provides a \c const iterator for RWTPtrHashMap.
 *
 * RWTPtrHashMapConstIterator provides a \c const iterator interface
 * to the C++ Standard Library-based container RWTPtrHashMap.
 * This iterator prohibits any changes to the underlying collection.
 *
 * Iteration over an RWTPtrHashMap is pseudo-random and dependent
 * on the capacity of the underlying hash table and the hash function
 * being used.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset(). The iterator becomes
 * valid after being advanced with either a pre-increment or operator()().
 *
 * For both operator++() and operator()(), iterating past the last
 * element returns a value equivalent to boolean \c false. Once
 * this state is reached, continued increments return a value equivalent
 * to \c false until reset() is called.
 *
 * The value type must have \c operator==() defined. This requirement
 * is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tphdict.h>
 * template <class K, class T, class H, class EQ,
 *           class A=std::allocator<T*> >
 * class RWTPtrHashMapConstIterator
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class K, class T, class H = RWTHash<K>, class EQ = std::equal_to<K>, class A = std::allocator<K*> >
class RW_DEPRECATE_TYPE("Use RWTPtrHashMap::const_iterator instead") RWTPtrHashMapConstIterator
{
public:
    /**
     * Creates a \c const iterator for the hashmap \a h. The iterator begins
     * in an undefined state and must be advanced before the first association
     * is accessible.
     */
    RWTPtrHashMapConstIterator(const RWTPtrHashMap<K, T, H, EQ, A>& h)
        : rwmap_(&h), head_(true), tail_(false) { }

    /**
     * Creates a \c const iterator from the iterator \a itr. The \c const
     * iterator begins in an undefined state and must be advanced before the
     * first association is accessible.
     */
    RWTPtrHashMapConstIterator(const RWTPtrHashMapIterator<K, T, H, EQ, A>& itr)
        : rwmap_(itr.rwmap_), iter_(itr.iter_),
          head_(itr.head_), tail_(itr.tail_) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self references the first element.
     * If used when self references the last association in
     * the hashmap, self references an undefined value and the method
     * returns a value equivalent to \c false. Otherwise, the method
     * returns a value equivalent to \c true.
     */
    bool operator++() {  // Advance and test
        RW_ASSERT(!(head_ == true && tail_ == true));

        if (tail_) {
            /* do nothing */
        }
        else if (head_) {
            iter_ = rwmap_->begin();
            head_ = false;
        }
        else {
            ++iter_;
        }

        if (!tail_ && iter_ == rwmap_->end()) {
            tail_ = true;
        }

        RW_ASSERT(!(head_ == true && tail_ == true));

        return !tail_;
    }

    /**
     * Advances self to the next element, dereferences the resulting
     * iterator and returns its key. If the iterator has advanced past
     * the last item in the container, returns #rwnil.
     */
    const K* operator()() {
        return ++(*this) ? key() : rwnil;
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    const RWTPtrHashMap<K, T, H, EQ, A>* container() const {
        return rwmap_;
    }

    /**
     * Returns the key portion of the association currently referenced
     * by self. The return value is undefined if self is not referencing
     * an association within the hashmap.
     */
    const K* key() const {
        return (*iter_).first;
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
     * the first element of the collection \a m.
     */
    void reset(const RWTPtrHashMap<K, T, H, EQ, A>& m) {
        head_ = true;
        tail_ = false;
        rwmap_ = &m;
    }

    /**
     * Returns the value portion of the association currently referenced
     * by self. The return value is undefined if self is not referencing
     * an association within the hashmap.
     */
    const T* value() const {
        return (*iter_).second;
    }

private:
    const RWTPtrHashMap<K, T, H, EQ, A>* rwmap_;
    typename RWTPtrHashMap<K, T, H, EQ, A>::const_iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);
};
#endif

#endif /* RW_CLASSIC_TEMPLATE_CONTAINERS */

#endif /* RW_TOOLS_TPHMAP_H */
