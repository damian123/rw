#ifndef RW_TOOLS_TVHMMAP_H
#define RW_TOOLS_TVHMMAP_H

/**********************************************************************
 *
 * tvhmmap.h - RWTValHashMultiMap<Key,T,H,EQ>
 *    : value-based key/data dictionary wrapper for STL-like rw_hashmulitmap
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvhmmap.h#3 $
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
#include <rw/stdex/hashmmap.h>
#include <rw/tools/algorithm.h>
#include <rw/tools/traits/RWTEnableIf.h>
#include <rw/tools/traits/RWTIsConvertible.h>

#if !defined(RW_DISABLE_DEPRECATED)

template <class K, class T, class H, class EQ, class A>
class RWTValHashMultiMapConstIterator;

#endif

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Maintains a collection of keys, each with an associated
 * item stored according to a hash object.
 *
 * This class maintains a collection of keys, each with an associated
 * item of type \c T. These pairs are stored according to a hash
 * object of type \c H. Class \c T is the
 * type of items in the collection. Class \c A is an allocator of
 * objects of class \c T. For more information about using custom
 * allocators, please see the <i>Essential Tools
 * Module User's Guide</i>.
 *
 * \c H must provide a \c const function that takes a single argument
 * convertible to type \c K and returns a value of type \c size_t.
 *
 * Equivalent keys within the collection are grouped together based on an
 * equality object of type \c EQ. \c EQ must ensure this grouping via a
 * function that takes two arguments convertible to type \c K and
 * returns a value of type \c bool.
 *
 * @note
 * Any two keys that are equivalent \e must hash to the
 * same value.
 *
 * \link RWTValHashMultiMap RWTValHashMultiMap<K,T,H,EQ,A>\endlink may contain
 * multiple keys that are equivalent to each other.
 * (\link RWTValHashMap RWTValHashMap<K,T,H,EQ,A>\endlink will
 * not accept a key that is equivalent to any key already in the
 * collection.) Equality is based on the comparison object and
 * not on the equality operator.
 *
 * The \c value type must have \c operator== defined. This requirement
 * is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvhmmap.h>
 * RWTValHashMultiMap<K,T,H,EQ,A> m;
 * @endcode
 *
 * @section related Related Classes
 *
 * Class \link RWTValHashMap RWTValHashMap<K,T,H,EQ,A>\endlink offers the same
 * interface to a collection that will not accept multiple keys that compare
 * equal to each other.
 *
 * Class rw_hashmultimap<K,T,H,EQ,A> is the C++ Standard Library collection
 * that serves as the underlying implementation for this collection.
 *
 * @section persistence Persistence
 *
 * Isomorphic.
 *
 * @section example Example
 *;
 * @code
 * #include <iostream>
 * #include <functional>
 *
 * #include <rw/tvhmmap.h>
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
 *     RWCString trd = "Third";
 *     RWTValHashMultiMap<RWCString, int, simple_hash, std::equal_to<RWCString> >
 *         contest;
 *
 *     contest.insert("First", 7);
 *     contest.insert(trd,3);
 *     contest.insert(trd,6);      // this contains two distinct values
 *                                 // equivalent to trd
 *     contest.insert("Second",2);
 *     contest.resize(8);
 *
 *     std::cout << contest.capacity()  << "\n"
 *               << contest.entries()   << "\n";
 *     std::cout << contest.fillRatio() << "\n";
 *
 *     std::cout << "The table is "
 *               << contest.fillRatio () * 100.0F << "% full\n";
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 * 8
 * 4
 * 0.5
 * The table is 50% full
 * @endcode
 */
template <class K, class T, class H = RWTHash<K>, class EQ = std::equal_to<K>, class A = std::allocator<K> >
class RWTValHashMultiMap
{
public:

    /**
     * A type representing the underlying implementation container.
     */
    typedef rw_hashmultimap<K, T, H, EQ, A> container_type;

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
     * A type representing the mapped value of the container.
     */
    typedef typename container_type::mapped_type mapped_type;

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
    typedef RWTValHashMultiMap<K, T, H, EQ, A> this_type;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12.5, use
     * \link RWTValHashMultiMap::mapped_type \c mapped_type \endlink instead.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTValHashMultiMap::mapped_type instead") T data_type;

    /**
     * @internal
     * @deprecated As of SourcePro 12.5, use
     * \link RWTValHashMultiMap::key_type \c key_type \endlink instead.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTValHashMultiMap::key_type instead") K value_type_key;

    /**
     * @internal
     * @deprecated As of SourcePro 12.5, use
     * \link RWTValHashMultiMap::mapped_type \c mapped_type \endlink instead.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTValHashMultiMap::mapped_type instead") T value_type_data;

    /**
     * @internal
     * @deprecated As of SourcePro 12.5, use
     * \link RWTValHashMultiMap::key_type \c key_type& \endlink instead.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTValHashMultiMap::key_type& instead") K& reference_key;

    /**
     * @internal
     * @deprecated As of SourcePro 12.5, use
     * \link RWTValHashMultiMap::mapped_type \c mapped_type& \endlink instead.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTValHashMultiMap::mapped_type& instead") T& reference_data;

    /**
     * @internal
     * @deprecated As of SourcePro 12.5, use
     * \link RWTValHashMultiMap::key_type <tt>const key_type&</tt> \endlink instead.
     */
    typedef RW_DEPRECATE_TYPE("Use const RWTValHashMultiMap::key_type& instead") const K& const_reference_key;

    /**
     * @internal
     * @deprecated As of SourcePro 12.5, use
     * \link RWTValHashMultiMap::mapped_type <tt>const mapped_type&</tt> \endlink instead.
     */
    typedef RW_DEPRECATE_TYPE("Use const RWTValHashMultiMap::mapped_type& instead") const T& const_reference_data;
#endif

    /**
     * Returns a reference to the underlying collection that serves
     * as the implementation for self. This reference may be used
     * freely, providing accessibility to the C++ Standard Library interface
     * as well as interoperability with other software components that make
     * use of the C++ Standard Library collections.
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
    void apply(void(*fn)(const key_type&, const mapped_type&, void*), void* d) const;

    /**
     * @copydoc apply()
     */
    void apply(void(*fn)(const key_type&, mapped_type&, void*), void* d);

    // deprecated
    void applyToKeyAndValue(void(*fn)(const key_type&, const mapped_type&, void*), void* x) const {
        apply(fn, x);
    }

    // deprecated
    void applyToKeyAndValue(void(*fn)(const key_type&, mapped_type&, void*), void* x) {
        apply(fn, x);
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
     * Returns \c true if there are no items in the collection,
     * otherwise \c false.
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
     * Clears the collection by removing all items from self. Each
     * key and its associated item will have its destructor called.
     */
    void clear() {
        std().clear();
    }

    /**
     * Returns \c true if there exists an association \c a in self such
     * that the expression <tt>((*\a fn)(a,\a d))</tt> is \c true,
     * otherwise returns \c false. Client data may be passed through
     * parameter \a d.
     */
    bool contains(bool(*fn)(const_reference, void*), void* d) const {
        return std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d)) != end();
    }


    /**
     * Returns \c true if there exists a key \c j in self that compares
     * equal to \a key, otherwise returns \c false.
     *
     * @note
     * This method requires the type \c #mapped_type to have a default constructor.
     */
    bool contains(const key_type& key) const {
        return std().find(key) != std().end();
    }

    /**
     * If there exists an association \c a in self such that the
     * expression <tt>((*\a fn)(a,\a d))</tt> is \c true, assigns
     * \c a to \a r and returns \c true. Otherwise, returns \c false
     * and leaves the value of \a r unchanged. Client data may be
     * passed through parameter \a d.
     */
    bool find(bool(*fn)(const_reference, void*), void* d, std::pair<K, T>& r) const {
        const_iterator ret = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        if (ret != end()) {
            r = std::pair<K, T>((*ret).first, (*ret).second);
            return true;
        }
        return false;
    }

    /**
     * If there exists a key \c j in self that compares equal to
     * \a key, assigns \c j to \a r and returns \c true. Otherwise, returns
     * \c false and leaves the value of \a r unchanged.
     *
     * @note
     * This method requires the type \c #mapped_type to have a default constructor.
     */
    bool find(const key_type& key, key_type& r) const {
        const_iterator i = std().find(key);
        if (i != end()) {
            r = (*i).first;
            return true;
        }
        return false;
    }

    /**
     * If there exists a key \c j in self that compares equal to \a
     * key, assigns the item associated with \c j to \a r and returns
     * \c true. Otherwise, returns \c false and leaves the value of
     * \a r unchanged.
     *
     * @note
     * This method requires the type \c #mapped_type to have a default constructor.
     */
    bool findValue(const key_type& key, mapped_type& r) const {
        const_iterator i = std().find(key);
        if (i != end()) {
            r = (*i).second;
            return true;
        }
        return false;
    }

    /**
     * If there exists a key \c j in self that compares equal to \a
     * key, assigns \c j to \a kr, assigns the item associated with
     * \c j to \a tr, and returns \c true. Otherwise, returns \c false
     * and leaves the values of \a kr and \a tr unchanged.
     *
     * @note
     * This method requires the type \c #mapped_type to have a default constructor.
     */
    bool findKeyAndValue(const key_type& key, key_type& kr, mapped_type& tr) const {
        const_iterator i = std().find(key);
        if (i != end()) {
            kr = (*i).first;
            tr = (*i).second;
            return true;
        }
        return false;
    }

    /**
     * Returns a reference to the minimum key in the
     * collection. Type \c #key_type must have well-defined less-than
     * semantics (<tt>K::operator<(const K&)</tt> or equivalent).
     */
    const key_type& minElement() const {
        RW_PRECONDITION(entries() != 0);
        return (*std::min_element(begin(), end(),
                                  rw_pair_compare<std::less<K>, value_type>())).first;
    }

    /**
     * Returns a reference to the maximum key in the
     * collection. Type \c #key_type must have well-defined less-than
     * semantics (<tt>K::operator<(const &)</tt> or equivalent).
     */
    const key_type& maxElement() const {
        RW_PRECONDITION(entries() != 0);
        return (*std::max_element(begin(), end(),
                                  rw_pair_compare<std::less<K>, value_type>())).first;
    }

    /**
     * Returns the number of associations \c a in self such that the
     * expression <tt>((*\a fn)(a,\a d))</tt> is \c true. Client data
     * may be passed through parameter \a d.
     */
    size_type occurrencesOf(bool(*fn)(const_reference, void*), void* d) const {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Returns the number of keys \c j in self that compares equal
     * to \a key.
     *
     * @note
     * This method requires the type \c #mapped_type to have a default constructor.
     */
    size_type occurrencesOf(const key_type& key) const {
        return std().count(key);
    }

    /**
     * Removes the first association \c a in self such that the
     * expression <tt>((*\a fn)(a,\a d))</tt> is \c true and returns
     * \c true. Returns \c false if there is no such element. Client
     * data may be passed through parameter \a d.
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
     * Removes the first association with key \c j in self such that
     * \c j compares equal to \a key and returns \c true. Returns
     * \c false if there is no such association.
     *
     * @note
     * This method requires the type \c #mapped_type to have a default constructor.
     */
    bool remove(const key_type& key) {
        iterator i = std().find(key);
        if (i != end()) {
            std().erase(i);
            return true;
        }
        return false;
    }

    /**
     * Removes all associations \c a in self such that the expression
     * <tt>((*\a fn)(a,\a d))</tt> is \c true. Returns the number of
     * items removed. Client data may be passed through parameter \a d.
     */
    size_type removeAll(bool(*fn)(const_reference, void*), void* d);

    /**
     * Removes all associations with key \c j in self where \c j compares
     * equal to \a key. Returns the number of items removed.
     *
     * @note
     * This method requires the type \c #mapped_type to have a default constructor.
     */
    size_type removeAll(const key_type& key) {
        return std().erase(key);
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
     * \c true.
     */
    bool insert(const key_type& key, const mapped_type& a) {
        std().insert(value_type(key, a));
        return true; // fails only via exception
    }

    bool insertKeyAndValue(const key_type& key, const mapped_type& val) { // deprecated
        return insert(key, val);
    }

#if !defined(RW_NO_RVALUE_REFERENCES) && !defined(RW_BROKEN_RVALUE_OVERLOAD_RESOLUTION)
    /**
     * @copydoc insert(const key_type&, const mapped_type&)
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    bool insert(K && key, T && a) {
        std().insert(value_type(rw_move(key), rw_move(a)));
        return true; // fails only via exception
    }

    /**
     * @copydoc insert(iterator, const_reference)
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     *
     * \c P is a type that is convertible to #value_type.
     */
    template <typename P>
    iterator insert(iterator hint, P && val) {
        return std().insert(hint, rw_forward<P>(val));
    }
#endif // !RW_NO_RVALUE_REFERENCES && !RW_BROKEN_RVALUE_OVERLOAD_RESOLUTION

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
     * Changes the capacity of self by creating a new hashed multimap
     * with a capacity of \a sz. This method then copies every element
     * of self into the new container and finally swaps the internal
     * representation of the new container with self.
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
    swap(RWTValHashMultiMap<K, T, H, EQ, A>& other) {
        std().swap(other.impl_);
    }

    /**
     * Destroys all elements of self and replaces them by copying all
     * associations from \a rhs.
     */
    RWTValHashMultiMap<K, T, H, EQ, A>&
    operator=(const RWTValHashMultiMap<K, T, H, EQ, A>& rhs) {
        return operator=(rhs.impl_);
    }

    /**
     * @copydoc operator=()
     */
    RWTValHashMultiMap<K, T, H, EQ, A>&
    operator=(const rw_hashmultimap<K, T, H, EQ, A>& rhs) {
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
    RWTValHashMultiMap<K, T, H, EQ, A>&
    operator=(RWTValHashMultiMap<K, T, H, EQ, A> && rhs) {
        return operator=(rw_move(rhs.impl_));
    }

    /**
    * @copydoc operator=(RWTValHashMultiMap<K,T,H,EQ,A>&&)
    */
    RWTValHashMultiMap<K, T, H, EQ, A>&
    operator=(container_type && rhs) {
        if (&impl_ != &rhs) {
            impl_ = rw_move(rhs);
        }
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs an empty map.
     */
    RWTValHashMultiMap() : impl_(RW_DEFAULT_CAPACITY) { }

    /**
     * Constructs a map by copying all elements of \a m.
     */
    RWTValHashMultiMap(const rw_hashmultimap<K, T, H, EQ, A>& m)
        : impl_(m) { }

    /**
     * Copy constructor.
     */
    RWTValHashMultiMap(const RWTValHashMultiMap<K, T, H, EQ, A>& rws)
        : impl_(rws.impl_) { }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed hash map takes ownership of the
     * data owned by \a rws.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValHashMultiMap(RWTValHashMultiMap<K, T, H, EQ, A> && rws)
        : impl_(rw_move(rws.impl_)) { }

    /**
    * @copydoc RWTValHashMultiMap(RWTValHashMultiMap<K,T,H,EQ,A>&&)
    */
    RWTValHashMultiMap(container_type && rws)
        : impl_(rw_move(rws)) { }
#endif

    /**
     * Constructs a map by copying elements from the range
     * [\a first, \a last).
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
    template <typename InputIterator>
    RWTValHashMultiMap(InputIterator first, InputIterator last)
        : impl_(first, last, RW_DEFAULT_CAPACITY) { }

private:
    rw_hashmultimap<K, T, H, EQ, A> impl_;
};


/**
 * @relates RWTValHashMultiMap
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and if for
 * each key in the first container, the set of values associated with that
 * key is equal to the set of values associated with that key in the second
 * container.
 *
 * @note
 * This method requires the type \c #mapped_type to have a default constructor.
 */
template <class K, class T, class H, class EQ, class A>
bool operator==(const RWTValHashMultiMap<K, T, H, EQ, A>& lhs, const RWTValHashMultiMap<K, T, H, EQ, A>& rhs)
{
    return lhs.std() == rhs.std();
}

/**
 * @relates RWTValHashMultiMap
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class K, class T, class H, class EQ, class A>
bool operator!=(const RWTValHashMultiMap<K, T, H, EQ, A>& lhs, const RWTValHashMultiMap<K, T, H, EQ, A>& rhs)
{
    return lhs.std() != rhs.std();
}

/**
 * @relates RWTValHashMultiMap
 *
 * @copydoc operator==(const RWTValHashMultiMap&, const RWTValHashMultiMap&);
 */
template <class K, class T, class H, class EQ, class A>
bool operator==(const RWTValHashMultiMap<K, T, H, EQ, A>& lhs,
                const typename RWTValHashMultiMap<K, T, H, EQ, A>::container_type& rhs)
{
    return lhs.std() == rhs;
}

/**
 * @relates RWTValHashMultiMap
 *
 * @copydoc operator!=(const RWTValHashMultiMap&, const RWTValHashMultiMap&);
 */
template <class K, class T, class H, class EQ, class A>
bool operator!=(const RWTValHashMultiMap<K, T, H, EQ, A>& lhs,
                const typename RWTValHashMultiMap<K, T, H, EQ, A>::container_type& rhs)
{
    return lhs.std() != rhs;
}

/**
 * @relates RWTValHashMultiMap
 *
 * @copydoc operator==(const RWTValHashMultiMap&, const RWTValHashMultiMap&);
 */
template <class K, class T, class H, class EQ, class A>
bool operator==(const typename RWTValHashMultiMap<K, T, H, EQ, A>::container_type& lhs,
                const RWTValHashMultiMap<K, T, H, EQ, A>& rhs)
{
    return lhs == rhs.std();
}

/**
 * @relates RWTValHashMultiMap
 *
 * @copydoc operator!=(const RWTValHashMultiMap&, const RWTValHashMultiMap&);
 */
template <class K, class T, class H, class EQ, class A>
bool operator!=(const typename RWTValHashMultiMap<K, T, H, EQ, A>::container_type& lhs,
                const RWTValHashMultiMap<K, T, H, EQ, A>& rhs)
{
    return lhs != rhs.std();
}

/**
 * @relates RWTValHashMultiMap
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class K, class T, class H, class EQ , class A>
RWvostream& operator<<(RWvostream& strm, const RWTValHashMultiMap<K, T, H, EQ , A>& coll);

/**
 * @relates RWTValHashMultiMap
 *
 * Saves the collection \a coll onto the RWFile \a strm,
 * or a reference to it if it has already been saved.
 */
template <class K, class T, class H, class EQ , class A>
RWFile& operator<<(RWFile& strm, const RWTValHashMultiMap<K, T, H, EQ , A>& coll);

/**
 * @relates RWTValHashMultiMap
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class K, class T, class H, class EQ , class A>
RWvistream& operator>>(RWvistream& strm, RWTValHashMultiMap<K, T, H, EQ , A>& coll);

/**
 * @relates RWTValHashMultiMap
 *
 * Restores the contents of the collection \a coll from the RWFile \a strm.
 */
template <class K, class T, class H, class EQ , class A>
RWFile& operator>>(RWFile& strm, RWTValHashMultiMap<K, T, H, EQ , A>& coll);

/**
 * @relates RWTValHashMultiMap
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class K, class T, class H, class EQ , class A>
RWvistream& operator>>(RWvistream& strm, RWTValHashMultiMap<K, T, H, EQ , A>*& p);

/**
 * @relates RWTValHashMultiMap
 *
 * Looks at the next object on the RWFile \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class K, class T, class H, class EQ , class A>
RWFile& operator>>(RWFile& strm, RWTValHashMultiMap<K, T, H, EQ , A>*& p);

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tvhmmap.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tvhmmap.cc"
#endif

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTValHashMultiMap::iterator to iterate
 * over the elements in this collection.
 *
 * @brief Deprecated. Provides an iterator for RWTValHashMultiMap.
 *
 * RWTValHashMultiMapIterator provides an iterator interface to
 * the C++ Standard Library-based collection represented by
 * RWTValHashMultiMap. (An STL iterator interface is provided through
 * RWTValHashMultiMap::iterator.)
 *
 * Iteration over an RWTValHashMultiMap is pseudorandom and dependent
 * on the capacity of the underlying hash table and the hash function
 * being used. The only usable relationship between consecutive
 * elements is that elements which are defined to be equivalent
 * by the equivalence object, \c EQ, will remain adjacent.
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
 * The \c value type must have \c operator== defined. This requirement
 * is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvhmmap.h>
 * RWTValHashMultiMap<K,T,H,EQ,A> m;
 * RWTValHashMultiMapIterator<K,T,H,EQ,A> itr(m);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/tvhmmap.h>
 * #include <rw/cstring.h>
 * #include <iostream>
 * #include <functional>
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
 * int main() {
 *    RWTValHashMultiMap
 *      <RWCString, int, simple_hash, std::equal_to<RWCString> >
 *        age;
 *    RWTValHashMultiMapIterator
 *      <RWCString, int, simple_hash, std::equal_to<RWCString > >
 *        itr(age);
 *
 *    age.insert(RWCString("John"), 30);
 *    age.insert(RWCString("Steve"),17);
 *    age.insert(RWCString("Mark"),24);
 *    age.insert(RWCString("Steve"),24);
 *
 *    while(itr()) {
 *      std::cout << itr.key()
 *                << "\'s age is "
 *                << itr.value()
 *                << std::endl;
 *    }
 *
 *    return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 * John's age is 30
 * Steve's age is 24
 * Steve's age is 17
 * Mark's age is 24
 * @endcode
 */
template <class K, class T, class H = RWTHash<K>, class EQ = std::equal_to<K>, class A = std::allocator<K> >
class RW_DEPRECATE_TYPE("Use RWTValHashMultiMap::iterator instead") RWTValHashMultiMapIterator
{
public:
    /**
     * Creates an iterator for the hash multimap \a h. The iterator
     * begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTValHashMultiMapIterator(RWTValHashMultiMap<K, T, H, EQ, A>& h)
        : rwmap_(&h), head_(true), tail_(false) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self will now reference the first element.
     * If, before iteration, self referenced the last association in
     * the multimap, self will now reference an undefined value and
     * \c false is returned. Otherwise, returns \c true.
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
     * iterator, and returns \c false if the iterator has advanced past
     * the last item in the container; otherwise returns \c true.
     */
    bool operator()() {
        return ++(*this);
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    RWTValHashMultiMap<K, T, H, EQ, A>* container() const {
        return rwmap_;
    }

    /**
     * Returns the key portion of the association currently referenced
     * by self.
     */
    K key() const {
        return (*iter_).first;
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
     * the first element of the collection \a h.
     */
    void reset(RWTValHashMultiMap<K, T, H, EQ, A>& h) {
        head_ = true;
        tail_ = false;
        rwmap_ = &h;
    }

    /**
     * Returns the value portion of the association referenced by self.
     */
    T value() const {
        return (*iter_).second;
    }

private:
    RWTValHashMultiMap<K, T, H, EQ, A>* rwmap_;
    typename RWTValHashMultiMap<K, T, H, EQ, A>::iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);
    friend class RWTValHashMultiMapConstIterator<K, T, H, EQ, A>;
};

/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTValHashMultiMap::const_iterator to
 * iterate over the elements in this collection.
 *
 * @brief Deprecated. Provides a \c const iterator for RWTValHashMultiMap.
 *
 * RWTValHashMultiMapConstIterator provides a \c const iterator
 * for RWTValHashMultiMap. A \c const iterator prohibits any changes
 * to the collection it is iterating over.
 *
 * Iteration over an RWTValHashMultiMap is pseudorandom and dependent
 * on the capacity of the underlying hash table and the hash function
 * being used. The only usable relationship between consecutive
 * elements is that elements which are defined to be equivalent
 * by the equivalence object, \c EQ, will remain adjacent.
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
 * The \c value type must have \c operator== defined. This requirement
 * is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvhmmap.h>
 * RWTValHashMultiMap<K,T,H,EQ,A> m;
 * RWTValHashMultiMapConstIterator<K,T,H,EQ,A> itr(m);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class K, class T, class H = RWTHash<K>, class EQ = std::equal_to<K>, class A = std::allocator<K> >
class RW_DEPRECATE_TYPE("Use RWTValHashMultiMap::const_iterator instead") RWTValHashMultiMapConstIterator
{
public:
    /**
     * Creates a \c const iterator for the hash multimap \a h. The iterator
     * begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTValHashMultiMapConstIterator(const RWTValHashMultiMap<K, T, H, EQ, A>& h)
        : rwmap_(&h), head_(true), tail_(false) { }

    /**
     * Creates a \c const iterator from the iterator \a itr. The \c const
     * iterator begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTValHashMultiMapConstIterator(const RWTValHashMultiMapIterator
                                    <K, T, H, EQ, A>& itr)
        : rwmap_(itr.rwmap_), iter_(itr.iter_),
          head_(itr.head_), tail_(itr.tail_) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created self now references the first element.
     * If, before iteration, self referenced the last association in
     * the multimap, self now references an undefined value and \c false
     * is returned. Otherwise, \c true is returned.
     */
    bool operator++() { // Advance and test
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
     * iterator and returns \c false if the iterator has advanced past
     * the last item in the container and \c true otherwise.
     */
    bool operator()() {
        return ++(*this);
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    const RWTValHashMultiMap<K, T, H, EQ, A>* container() const {
        return rwmap_;
    }

    /**
     * Returns the key portion of the association currently referenced
     * by self.
     */
    const K& key() const {
        return (*iter_).first;
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
    void reset(const RWTValHashMultiMap<K, T, H, EQ, A>& h) {
        head_ = true;
        tail_ = false;
        rwmap_ = &h;
    }

    /**
     * Returns the value portion of the association referenced by
     * self.
     */
    const T& value() const {
        return (*iter_).second;
    }

private:
    const RWTValHashMultiMap<K, T, H, EQ, A>* rwmap_;
    typename RWTValHashMultiMap<K, T, H, EQ, A>::const_iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);
};
#endif

#endif /* RW_TOOLS_TVHMMAP_H */
