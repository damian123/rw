#ifndef RW_TOOLS_TVMMAP_H
#define RW_TOOLS_TVMMAP_H

/**********************************************************************
 *
 * tvmmap.h - RWTValMultiMap<Key,T,Compare>
 *     : value-based key/data dictionary wrapper for STL mulitmap
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvmmap.h#3 $
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
#include <rw/tools/algorithm.h>
#include <rw/tools/traits/RWTEnableIf.h>
#include <rw/tools/traits/RWTIsConvertible.h>

#include <algorithm>
#include <map>

#if !defined(RW_NO_MAP_PAIR_ALLOC)
#  define RW_ALLOC_PARAM_VALUE std::pair<K const, T>
#else
#  define RW_ALLOC_PARAM_VALUE T
#endif

#if !defined(RW_DISABLE_DEPRECATED)

template <class K, class T, class C, class A>
class RWTValMultiMapConstIterator;

#endif

/**
 * @ingroup stl_based_collection_classes
 * @brief Maintains a collection of keys, each with an associated
 * item and ordered according to a comparison object.
 *
 * This class maintains a collection of keys, each with an associated
 * item of type \c T.
 * Class \c A is an allocator of objects of
 * class \c T. For more information about using custom allocators, please
 * see the <i>Essential Tools Module User's Guide</i>.
 *
 * Order is determined by the key according
 * to a comparison object of type \c C. \c C must induce a total
 * ordering on elements of type \c K via a public member
 *
 * @code
 * bool operator()(const K& x, const K& y) const
 * @endcode
 *
 * which returns \c true if \c x and its partner should precede
 * \c y and its partner within the collection. The structure \c
 * std::less<T> from the C++ Standard Library header file \c \<functional\> is
 * an example.
 *
 * \link RWTValMultiMap RWTValMultiMap<K,T,C,A>\endlink may contain multiple
 * keys that compare equal to each other.
 * (\link RWTValMap RWTValMap<K,T,C,A>\endlink will not accept a key
 * that compares equal to any key already in the collection.) Equality
 * is based on the comparison object and not on the operator==.
 * Given a comparison object \c comp, keys \c a and \c b are equal
 * if <tt>!comp(a,b) && !comp(b,a)</tt>.
 *
 * The \c value type must have \c operator== and \c operator< defined.
 * This requirement is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvmmap.h>
 * RWTValMultiMap<K,T,C,A> m;
 * @endcode
 *
 * @section related Related Classes
 *
 * Class \link RWTValMap RWTValMap<K,T,C,A>\endlink offers the same interface
 * to a collection that will not accept multiple keys that compare equal to
 * each other. \link RWTValMultiSet RWTValMultiSet<T,C,A>\endlink maintains a
 * collection of keys without the associated values.
 *
 * Class <b>std::multimap<K,T,C,A,std::pair<K const&,T> ></b> is
 * the C++ Standard Library collection that serves as the underlying
 * implementation for this collection.
 *
 * @section persistence Persistence
 *
 * Isomorphic.
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <functional>
 *
 * #include <rw/tvmmap.h>
 * #include <rw/cstring.h>
 * #include <rw/tools/datetime.h>
 *
 * int main ()
 * {
 *     typedef RWTValMultiMap<RWCString, RWDateTime, std::less<RWCString> >  RWMMap;
 *
 *     RWMMap birthdays;
 *
 *     birthdays.insert("John", RWDateTime("April 12, 1975", RWDateTime::setDate));
 *     birthdays.insert("Ivan", RWDateTime("Nov 2, 1980", RWDateTime::setDate));
 *     birthdays.insert("Mary", RWDateTime("Oct 22, 1987", RWDateTime::setDate));
 *     birthdays.insert("Ivan", RWDateTime("June 19, 1971", RWDateTime::setDate));
 *     birthdays.insert("Sally",RWDateTime("March 15, 1976", RWDateTime::setDate));
 *     birthdays.insert("Ivan", RWDateTime("July 6, 1950", RWDateTime::setDate));
 *
 *     // How many "Ivan"s?
 *     RWMMap::size_type n = birthdays.occurrencesOf("Ivan");
 *     RWMMap::size_type idx = 0;
 *
 *     std::cout << "There are " << n << " Ivans:" << std::endl;
 *
 *     RWMMap::iterator iter = birthdays.std().lower_bound("Ivan");
 *     while (++idx <= n)
 *         std::cout << idx << ". " << (*iter++).second.asString('x') << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 * There are 3 Ivans:
 * 1.  11/02/80
 * 2.  06/19/71
 * 3.  07/06/50
 * @endcode
 */
template <class K, class T, class C = std::less<K>, class A = std::allocator<RW_ALLOC_PARAM_VALUE> >
class RWTValMultiMap
{
public:

    /**
     * A type representing the underlying implementation container.
     */
    typedef std::multimap<K, T, C, A> container_type;

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
     * A type representing the mapped value of the container.
     */
    typedef typename container_type::mapped_type mapped_type;

    /**
     * A type representing the comparison function for \c #key_type.
     */
    typedef typename container_type::key_compare key_compare;

    /**
     * A type representing the comparison function for \c #value_type.
     */
    typedef typename container_type::value_compare value_compare;

    /**
     * @internal
     * A type representing the same type as self.
     */
    typedef RWTValMultiMap<K, T, C, A> this_type;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12.5, use
     * \link RWTValMultiMap::mapped_type \c mapped_type \endlink instead.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTValMultiMap::mapped_type instead") T data_type;

    /**
     * @internal
     * @deprecated As of SourcePro 12.5, use
     * \link RWTValMultiMap::key_compare \c key_compare \endlink instead.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTValMultiMap::key_compare instead") C container_comp;
#endif

    /**
     * Returns a reference to the underlying C++ Standard Library collection
     * that serves as the implementation for self. This reference may
     * be used freely, providing access to the C++ Standard Library interface
     * as well as interoperability with other software components that
     * make use of the C++ Standard Library collections.
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
     * Returns \c true if there are no items in the collection, otherwise
     * \c false.
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
#if !defined(RW_NO_STD_MULTIMAP_ERASE_RETURN)
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
#if !defined(RW_NO_STD_MULTIMAP_ERASE_RETURN)
        return std().erase(first, last);
#else
        std().erase(first, last);
        return last;
#endif
    }

    /**
     * Clears the collection by removing all items from self. Each
     * key and its associated item has its destructor called.
     */
    void clear() {
        std().clear();
    }

    /**
     * Returns \c true if there exists an association \c a in self such that
     * the expression <tt>((*\a fn)(a, \a d))</tt> is \c true, otherwise
     * returns \c false. Client data may be passed through parameter \a d.
     */
    bool contains(bool(*fn)(const_reference, void*), void* d) const {
        return std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d)) != end();
    }

    /**
     * Returns \c true if there exists a key \c j in self that compares
     * equal to \a key, otherwise returns \c false.
     */
    bool contains(const key_type& key) const {
        return  std().find(key) != std().end();
    }

    /**
     * If there exists an association \c a in self such that the expression
     * <tt>((*\a fn)(a,\a d))</tt> is \c true, assigns \c a to \a r and returns
     * \c true. Otherwise, returns \c false and leaves the value of \a r
     * unchanged. Client data may be passed through parameter \a d.
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
     * If there exists a key \c j in self that compares equal to
     * \a key, assigns the item associated with \c j to \a r and returns
     * \c true. Otherwise, returns \c false and leaves the value of
     * \a r unchanged.
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
     * If there exists a key \c j in self that compares equal to
     * \a key, assigns \c j to \a kr, assigns the item associated with
     * \c j to \a tr, and returns \c true. Otherwise, returns \c false
     * and leaves the values of \a kr and \a tr unchanged.
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
     * Returns a reference to the minimum key in the collection.
     * Order is determined according to the comparison object \c C.
     */
    const key_type& minElement() const {
        RW_PRECONDITION(entries() != 0);
        return (*begin()).first;
    }

    /**
     * Returns a reference to the maximum key in the collection.
     * Order is determined according to the comparison object \c C.
     */
    const key_type& maxElement() const {
        RW_PRECONDITION(entries() != 0);
        return (*std::max_element(begin(), end(), std().value_comp())).first;
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
     * Returns the number of keys \c j in self that compare equal to
     * \a key.
     */
    size_type occurrencesOf(const key_type& key) const {
        return std().count(key);
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
     * Removes the first association \c a in self such that the expression
     * <tt>((*\a fn)(a,\a d))</tt> is \c true and returns \c true. Returns
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
     * Removes the first association with key \c j in self where \c j compares
     * equal to \a key and returns \c true. Returns \c false if there
     * is no such association.
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
     * <tt>((*\a fn)(a,\a d))</tt> is \c true. Returns the number of items
     * removed. Client data may be passed through parameter \a d.
     */
    size_type removeAll(bool(*fn)(const_reference, void*), void* d);

    /**
     * Removes all associations in self that have a key \c j that compares
     * equal to \a key. Returns the number of items removed.
     */
    size_type removeAll(const key_type& key) {
        return std().erase(key);
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
     * Inserts \a val into self. Returns an iterator to the new item.
     *
     * The \a hint parameter indicates where in the collection the new value
     * should be inserted. This value is only a hint, and will be ignored if
     * the value should be inserted at a different location according to the
     * container's comparison object.
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
#if !defined(RW_BROKEN_STD_MULTIMAP_INSERT_RANGE)
        std().insert(first, last);
#else
        std::copy(first, last, std::inserter(*this, end()));
#endif
    }

    /**
     * Adds \a key with associated item \a a to the collection. Returns
     * \c true.
     */
    bool insert(const key_type& key, const mapped_type& a) {
        std().insert(value_type(key, a));
        return true; // fails only via exception
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

    bool insertKeyAndValue(const key_type& key, const mapped_type& val) { // deprecated
        return insert(key, val);
    }

    /**
     * Swaps the data held by self with the data held by \a other.
     * This method does not copy or destroy any of the items swapped;
     * rather, it swaps the underlying hash * tables.
     */
    void
    swap(RWTValMultiMap<K, T, C, A>& other) {
        std().swap(other.impl_);
    }

    /**
     * Destroys all elements of self and replaces them by copying all
     * associations from \a rhs.
     */
    RWTValMultiMap<K, T, C, A>&
    operator=(const RWTValMultiMap<K, T, C, A>& rhs) {
        return operator=(rhs.impl_);
    }

    /**
     * @copydoc operator=()
     */
    RWTValMultiMap<K, T, C, A>&
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
    RWTValMultiMap<K, T, C, A>&
    operator=(RWTValMultiMap<K, T, C, A> && rhs) {
        return operator=(rw_move(rhs.impl_));
    }

    /**
     * @copydoc operator=(RWTValMultiMap<K,T,C,A>&&)
     */
    RWTValMultiMap<K, T, C, A>&
    operator=(container_type && rhs) {
        if (&impl_ != &rhs) {
#    if !defined(RW_NO_STDMMAP_RVALUE_SUPPORT)
            impl_ = rw_move(rhs);
#    else
            rhs.swap(impl_);
#    endif
        }
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs an empty map with comparator \a cmp.
     */
    RWTValMultiMap(const key_compare& cmp = key_compare()) : impl_(cmp) { }

    /**
     * Constructs a map by copying all elements of \a m.
     */
    RWTValMultiMap(const container_type& m) : impl_(m) { }

    /**
     * Copy constructor.
     */
    RWTValMultiMap(const RWTValMultiMap<K, T, C, A>& rws)
        : impl_(rws.impl_) { }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed map takes ownership of the
     * data owned by \a rws.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValMultiMap(RWTValMultiMap<K, T, C, A> && rws)
#    if !defined(RW_NO_STDMMAP_RVALUE_SUPPORT)
        : impl_(rw_move(rws.impl_)) { }
#    else
    {
        rws.std().swap(impl_);
    }
#    endif

    /**
     * @copydoc RWTValMultiMap(RWTValMultiMap<K,T,C,A>&&)
     */
    RWTValMultiMap(container_type && rws)
#    if !defined(RW_NO_STDMMAP_RVALUE_SUPPORT)
        : impl_(rw_move(rws)) { }
#    else
    {
        rws.swap(impl_);
    }
#    endif
#endif

    /**
     * Constructs a map by copying elements from the range
     * [\a first, \a last).
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
    template <typename InputIterator>
    RWTValMultiMap(InputIterator first, InputIterator last, const key_compare& cmp = key_compare())
#if !defined(RW_BROKEN_STD_MULTIMAP_CTOR_RANGE)
        : impl_(first, last, cmp) { }
#else
        :
        impl_(cmp) {
        insert(first, last);
    }
#endif

private:
    container_type impl_;
};


/**
 * @relates RWTValMultiMap
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and if for
 * each key in the first container, the set of values associated with that
 * key is equal to the set of values associated with that key in the second
 * container.
 */
template <class K, class T, class C, class A>
bool operator==(const RWTValMultiMap<K, T, C, A>& lhs, const RWTValMultiMap<K, T, C, A>& rhs)
{
    return lhs.std() == rhs.std();
}

/**
 * @relates RWTValMultiMap
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class K, class T, class C, class A>
bool operator!=(const RWTValMultiMap<K, T, C, A>& lhs, const RWTValMultiMap<K, T, C, A>& rhs)
{
    return lhs.std() != rhs.std();
}

/**
 * @relates RWTValMultiMap
 *
 * Returns \c true if \a lhs is lexicographically less than \a rhs,
 * otherwise \c false. Assumes that type \c T has well-defined less-than
 * semantics.
 */
template <class K, class T, class C, class A>
bool operator<(const RWTValMultiMap<K, T, C, A>& lhs, const RWTValMultiMap<K, T, C, A>& rhs)
{
    return lhs.std() < rhs.std();
}

/**
 * @relates RWTValMultiMap
 *
 * Equivalent to <tt>(\a rhs < \a lhs)</tt>.
 */
template <class K, class T, class C, class A>
bool operator>(const RWTValMultiMap<K, T, C, A>& lhs, const RWTValMultiMap<K, T, C, A>& rhs)
{
    return lhs.std() > rhs.std();
}

/**
 * @relates RWTValMultiMap
 *
 * Equivalent to <tt>!(\a rhs < \a lhs)</tt>.
 */
template <class K, class T, class C, class A>
bool operator<=(const RWTValMultiMap<K, T, C, A>& lhs, const RWTValMultiMap<K, T, C, A>& rhs)
{
    return lhs.std() <= rhs.std();
}

/**
 * @relates RWTValMultiMap
 *
 * Equivalent to <tt>!(\a lhs < \a rhs)</tt>.
 */
template <class K, class T, class C, class A>
bool operator>=(const RWTValMultiMap<K, T, C, A>& lhs, const RWTValMultiMap<K, T, C, A>& rhs)
{
    return lhs.std() >= rhs.std();
}

/**
 * @relates RWTValMultiMap
 *
 * @copydoc operator==(const RWTValMultiMap&, const RWTValMultiMap&);
 */
template <class K, class T, class C, class A>
bool operator==(const RWTValMultiMap<K, T, C, A>& lhs,
                const typename RWTValMultiMap<K, T, C, A>::container_type& rhs)
{
    return lhs.std() == rhs;
}

/**
 * @relates RWTValMultiMap
 *
 * @copydoc operator!=(const RWTValMultiMap&, const RWTValMultiMap&);
 */
template <class K, class T, class C, class A>
bool operator!=(const RWTValMultiMap<K, T, C, A>& lhs,
                const typename RWTValMultiMap<K, T, C, A>::container_type& rhs)
{
    return lhs.std() != rhs;
}

/**
 * @relates RWTValMultiMap
 *
 * @copydoc operator<(const RWTValMultiMap&, const RWTValMultiMap&);
 */
template <class K, class T, class C, class A>
bool operator<(const RWTValMultiMap<K, T, C, A>& lhs,
               const typename RWTValMultiMap<K, T, C, A>::container_type& rhs)
{
    return lhs.std() < rhs;
}

/**
 * @relates RWTValMultiMap
 *
 * @copydoc operator>(const RWTValMultiMap&, const RWTValMultiMap&);
 */
template <class K, class T, class C, class A>
bool operator>(const RWTValMultiMap<K, T, C, A>& lhs,
               const typename RWTValMultiMap<K, T, C, A>::container_type& rhs)
{
    return lhs.std() > rhs;
}

/**
 * @relates RWTValMultiMap
 *
 * @copydoc operator<=(const RWTValMultiMap&, const RWTValMultiMap&);
 */
template <class K, class T, class C, class A>
bool operator<=(const RWTValMultiMap<K, T, C, A>& lhs,
                const typename RWTValMultiMap<K, T, C, A>::container_type& rhs)
{
    return lhs.std() <= rhs;
}

/**
 * @relates RWTValMultiMap
 *
 * @copydoc operator>=(const RWTValMultiMap&, const RWTValMultiMap&);
 */
template <class K, class T, class C, class A>
bool operator>=(const RWTValMultiMap<K, T, C, A>& lhs,
                const typename RWTValMultiMap<K, T, C, A>::container_type& rhs)
{
    return lhs.std() >= rhs;
}


/**
 * @relates RWTValMultiMap
 *
 * @copydoc operator==(const RWTValMultiMap&, const RWTValMultiMap&);
 */
template <class K, class T, class C, class A>
bool operator==(const typename RWTValMultiMap<K, T, C, A>::container_type& lhs,
                const RWTValMultiMap<K, T, C, A>& rhs)
{
    return lhs == rhs.std();
}

/**
 * @relates RWTValMultiMap
 *
 * @copydoc operator!=(const RWTValMultiMap&, const RWTValMultiMap&);
 */
template <class K, class T, class C, class A>
bool operator!=(const typename RWTValMultiMap<K, T, C, A>::container_type& lhs,
                const RWTValMultiMap<K, T, C, A>& rhs)
{
    return lhs != rhs.std();
}

/**
 * @relates RWTValMultiMap
 *
 * @copydoc operator<(const RWTValMultiMap&, const RWTValMultiMap&);
 */
template <class K, class T, class C, class A>
bool operator<(const typename RWTValMultiMap<K, T, C, A>::container_type& lhs,
               const RWTValMultiMap<K, T, C, A>& rhs)
{
    return lhs < rhs.std();
}

/**
 * @relates RWTValMultiMap
 *
 * @copydoc operator>(const RWTValMultiMap&, const RWTValMultiMap&);
 */
template <class K, class T, class C, class A>
bool operator>(const typename RWTValMultiMap<K, T, C, A>::container_type& lhs,
               const RWTValMultiMap<K, T, C, A>& rhs)
{
    return lhs > rhs.std();
}

/**
 * @relates RWTValMultiMap
 *
 * @copydoc operator<=(const RWTValMultiMap&, const RWTValMultiMap&);
 */
template <class K, class T, class C, class A>
bool operator<=(const typename RWTValMultiMap<K, T, C, A>::container_type& lhs,
                const RWTValMultiMap<K, T, C, A>& rhs)
{
    return lhs <= rhs.std();
}

/**
 * @relates RWTValMultiMap
 *
 * @copydoc operator>=(const RWTValMultiMap&, const RWTValMultiMap&);
 */
template <class K, class T, class C, class A>
bool operator>=(const typename RWTValMultiMap<K, T, C, A>::container_type& lhs,
                const RWTValMultiMap<K, T, C, A>& rhs)
{
    return lhs >= rhs.std();
}

/**
 * @relates RWTValMultiMap
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class K, class T, class C , class A>
RWvostream& operator<<(RWvostream& strm, const RWTValMultiMap<K, T, C , A>& coll);

/**
 * @relates RWTValMultiMap
 *
 * Saves the collection \a coll onto the RWFile \a strm,
 * or a reference to it if it has already been saved.
 */
template <class K, class T, class C , class A>
RWFile& operator<<(RWFile& strm, const RWTValMultiMap<K, T, C , A>& coll);

/**
 * @relates RWTValMultiMap
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class K, class T, class C , class A>
RWvistream& operator>>(RWvistream& strm, RWTValMultiMap<K, T, C , A>& coll);

/**
 * @relates RWTValMultiMap
 *
 * Restores the contents of the collection \a coll from the RWFile \a strm.
 */
template <class K, class T, class C , class A>
RWFile& operator>>(RWFile& strm, RWTValMultiMap<K, T, C , A>& coll);

/**
 * @relates RWTValMultiMap
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class K, class T, class C , class A>
RWvistream& operator>>(RWvistream& strm, RWTValMultiMap<K, T, C , A>*& p);

/**
 * @relates RWTValMultiMap
 *
 * Looks at the next object on the RWFile \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class K, class T, class C , class A>
RWFile& operator>>(RWFile& strm, RWTValMultiMap<K, T, C , A>*& p);

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tvmmap.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tvmmap.cc"
#endif

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @ingroup stl_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTValMultiMap::iterator to iterate
 * over the elements in this collection.
 *
 * @brief Deprecated. Provides an iterator for RWTValMultiMap.
 *
 * RWTValMultiMapIterator provides an iterator interface to the
 * C++ Standard Library-based collection represented by RWTValMultiMap. (An STL
 * iterator interface is provided through RWTValMultiMap::iterator.)
 *
 * The order of iteration for an RWTValMultiMap is dependent upon the
 * comparator object as applied to the keys of the stored associations.
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
 * The \c value type must have \c operator== and \c operator< defined.
 * This requirement is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvmmap.h>
 * RWTValMultiMap<K,T,C,A> vm;
 * RWTValMultiMapIterator<K,T,C,A> itr(vm);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/tvmmap.h>
 * #include <rw/cstring.h>
 * #include <iostream>
 * #include <functional>
 *
 * int main()
 * {
 *    RWTValMultiMap<RWCString, int, std::greater<RWCString> > age;
 *    RWTValMultiMapIterator
 *       <RWCString, int, std::greater<RWCString> > itr(age);
 *
 *    age.insert("John", 30);
 *    age.insert("Steve", 17);
 *    age.insert("Mark", 24);
 *    age.insert("Steve", 24);
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
template <class K, class T, class C = std::less<K>, class A = std::allocator<RW_ALLOC_PARAM_VALUE> >
class RW_DEPRECATE_TYPE("Use RWTValMultiMap::iterator instead") RWTValMultiMapIterator
{
public:
    /**
     * Creates an iterator for the multi-map \a m. The iterator begins in
     * an undefined state and must be advanced before the first association
     * is accessible.
     */
    RWTValMultiMapIterator(RWTValMultiMap<K, T, C, A>& m)
        : rwmap_(&m), head_(true), tail_(false) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self will now reference the first element.
     * If, before iteration, self referenced the last association in
     * the multi-map, self now references an undefined value and
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
     * Advances self to the next element and returns \c true. Returns
     * \c false if the iterator has advanced past the last element in
     * the collection.
     */
    bool operator()() {
        return ++(*this);
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    RWTValMultiMap<K, T, C, A>* container() const {
        return rwmap_;
    }

    /**
     * Returns the key portion of the association currently referenced
     * by self.
     */
    K key() const {
        return (*(RW_CONST_CAST(RWTValMultiMapIterator*, this)->iter_)).first;
    }

    /**
     * Resets the iterator so that after being advanced, it references
     * the first element of the collection.
     */
    void reset() {
        head_ = true;
        tail_ = false;
    }

    /**
     * Resets the iterator so that after being advanced, it references
     * the first element of the collection \a h.
     */
    void reset(RWTValMultiMap<K, T, C, A>& h) {
        head_ = true;
        tail_ = false;
        rwmap_ = &h;
    }

    /**
     * Returns the value portion of the association referenced by self.
     */
    T value() const {
        return (*(RW_CONST_CAST(RWTValMultiMapIterator*, this)->iter_)).second;
    }

private:
    RWTValMultiMap<K, T, C, A>* rwmap_;
    typename RWTValMultiMap<K, T, C, A>::iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);
    friend class RWTValMultiMapConstIterator<K, T, C, A>;
};

/**
 * @ingroup stl_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTValMultiMap::const_iterator to
 * iterate over the elements in this collection.
 *
 * @brief Deprecated. Provides a \c const iterator for RWTValMultiMap.
 *
 * RWTValMultiMapConstIterator provides a \c const iterator for
 * RWTValMultiMap. A \c const iterator prohibits any changes to
 * the collection it iterates over.
 *
 * The order of iteration for an RWTValMultiMap is dependent upon the
 * comparator object as applied to the keys of the stored associations.
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
 * The \c value type must have \c operator== and \c operator< defined.
 * This requirement is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvmmap.h>
 * RWTValMultiMap<K,T,C,A> vm;
 * RWTValMultiMapConstIterator<K,T,C,A> itr(vm);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class K, class T, class C = std::less<K>, class A = std::allocator<RW_ALLOC_PARAM_VALUE> >
class RW_DEPRECATE_TYPE("Use RWTValMultiMap::const_iterator instead") RWTValMultiMapConstIterator
{
public:
    /**
     * Creates a \c const iterator for the multi-map \a m. The iterator begins in
     * an undefined state and must be advanced before the first association
     * is accessible.
     */
    RWTValMultiMapConstIterator(const RWTValMultiMap<K, T, C, A>& m)
        : rwmap_(&m), head_(true), tail_(false) { }

    /**
     * Creates a \c const iterator from the iterator \a itr. The \c const
     * iterator begins in an undefined state and must be advanced before the first
     * association is accessible.
     */
    RWTValMultiMapConstIterator(const RWTValMultiMapIterator<K, T, C, A>& itr)
        : rwmap_(itr.rwmap_), iter_(itr.iter_),
          head_(itr.head_), tail_(itr.tail_) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self now references the first element.
     * If, before iteration, self referenced the last association in
     * the multi-map, self now references an undefined value and \c
     * false is returned. Otherwise, returns \c true.
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
     * Advances self to the next element and returns \c true. Returns
     * \c false if the iterator has advanced past the last element in
     * the collection.
     */
    bool operator()() {
        return ++(*this);
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    const RWTValMultiMap<K, T, C, A>* container() const {
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
     * Resets the iterator so that after being advanced, it references
     * the first element of the collection.
     */
    void reset() {
        head_ = true;
        tail_ = false;
    }

    /**
     * Resets the iterator so that after being advanced, it references
     * the first element of the collection \a h.
     */
    void reset(const RWTValMultiMap<K, T, C, A>& h) {
        head_ = true;
        tail_ = false;
        rwmap_ = &h;
    }

    /**
     * Returns the value portion of the association referenced by \c
     * self.
     */
    const T& value() const {
        return (*iter_).second;
    }

private:
    const RWTValMultiMap<K, T, C, A>* rwmap_;
    typename RWTValMultiMap<K, T, C, A>::const_iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);
};
#endif

#undef RW_ALLOC_PARAM_VALUE

#endif /* RW_TOOLS_TVMMAP_H */
