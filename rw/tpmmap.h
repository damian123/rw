#ifndef RW_TOOLS_TPMMAP_H
#define RW_TOOLS_TPMMAP_H

/**********************************************************************
 *
 * tpmmap.h - RWTPtrMultiMap<K,T,Compare>
 *     : pointer-based key/data dictionary wrapper for STL mulitmap
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tpmmap.h#3 $
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
#include <rw/epaux.h>
#include <rw/epfunc.h>
#include <rw/tools/algorithm.h>
#include <rw/tools/traits/RWTEnableIf.h>
#include <rw/tools/traits/RWTIsConvertible.h>

#include <algorithm>
#include <map>

#if !defined(RW_NO_MAP_PAIR_ALLOC)
#  define RW_ALLOC_PARAM_VALUE std::pair<K* const, T*>
#else
#  define RW_ALLOC_PARAM_VALUE T*
#endif

#if !defined(RW_DISABLE_DEPRECATED)

template <class K, class T, class C, class A>
class RWTPtrMultiMapConstIterator;

#endif

/**
 * @ingroup stl_based_collection_classes
 *
 * @brief A pointer-based collection of associations ordered according to
 * a comparison object, and allowing duplicate keys.
 *
 * This class maintains a pointer-based collection of associations
 * of type <b>std::pair<K* const, T*></b>. The first part of the association
 * is a key of type \c K*, the second is its associated item of
 * type \c T*. Class \c A is an allocator of objects of
 * class \c T. For more information about using custom allocators, please
 * see the <i>Essential Tools Module User's Guide</i>.
 *
 * Order is determined by the key according to a comparison
 * object of type \c C. \c C must induce a total ordering on elements
 * of type \c K via a public member
 *
 * @code
 * bool operator()(const K& x, const K& y)
 * @endcode
 *
 * which returns \c true if \c x and its partner should precede
 * \c y and its partner within the collection. The structure \b std::less<T>
 * from the C++ Standard Library header file \c &lt;functional&gt; is
 * an example. Note that keys will be dereferenced before being
 * compared.
 *
 * \link RWTPtrMultiMap RWTPtrMultiMap<K,T,C,A> \endlink may contain multiple
 * keys that compare equal to each other.
 * (\link RWTPtrMap RWTPtrMap<K,T,C,A>\endlink will not accept a key
 * that compares equal to any key already in the collection.) Equality
 * is based on the comparison object and not on the equality operator.
 * Given a comparison object \c comp, keys \c a and \c b are equal
 * if
 *
 * @code
 * !comp(a,b) && !comp(b,a).
 * @endcode
 *
 * The \c value type must have \c operator== and \c operator< defined.
 * This requirement is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tpmmap.h>
 * RWTPtrMultiMap<K,T,C,A> m;
 * @endcode
 *
 * @section related Related Classes
 *
 * Class \link RWTPtrMap RWTPtrMap<K,T,C,A>\endlink offers the same interface
 * to a pointer-based collection that will not accept multiple keys that
 * compare equal to each other.
 * \link RWTPtrMultiSet RWTPtrMultiSet<T,C,A>\endlink maintains a pointer-based
 * collection of keys without the associated values.
 *
 * Class <b>std::multimap<K*,T*,deref_compare<C,K,A> ></b> is the C++ Standard Library
 * collection that serves as the underlying implementation for this
 * collection.
 *
 * @section persistence Persistence
 *
 * Isomorphic
 *
 * @section example Example
 *
 * @code
 *
 * //
 * // tpmmap.cpp
 * //
 * #include <iostream>
 * #include <functional>
 *
 * #include <rw/tpmmap.h>
 * #include <rw/cstring.h>
 * #include <rw/tools/datetime.h>
 *
 * int main ()
 * {
 *     typedef RWTPtrMultiMap<RWCString, RWDateTime, std::less<RWCString> >
 *         RWMMap;
 *
 *     RWMMap birthdays;
 *
 *     birthdays.insert(new RWCString("John"),
 *                      new RWDateTime("April 12, 1975", RWDateTime::setDate));
 *
 *     birthdays.insert(new RWCString("Ivan"),
 *                      new RWDateTime("Nov 2, 1980", RWDateTime::setDate));
 *
 *     birthdays.insert(new RWCString("Mary"),
 *                      new RWDateTime("Oct 22, 1987", RWDateTime::setDate));
 *
 *     birthdays.insert(new RWCString("Ivan"),
 *                      new RWDateTime("June 19, 1971", RWDateTime::setDate));
 *
 *     birthdays.insert(new RWCString("Sally"),
 *                      new RWDateTime("March 15, 1976", RWDateTime::setDate));
 *
 *     birthdays.insert(new RWCString("Ivan"),
 *                      new RWDateTime("July 6, 1950", RWDateTime::setDate));
 *
 *     // How many "Ivan"s?
 *     RWCString ivanstr("Ivan");
 *     RWMMap::size_type n = birthdays.occurrencesOf(&ivanstr);
 *     RWMMap::size_type idx = 0;
 *
 *     std::cout << "There are " << n << " Ivans:" << std::endl;
 *     RWMMap::const_iterator iter =
 *         birthdays.std().lower_bound(&ivanstr);
 *
 *     while (++idx <= n)
 *         std::cout << idx << ". " << (*iter++).second->asString('x') << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 *
 * There are 3 Ivans:
 * 1.  11/02/80
 * 2.  06/19/71
 * 3.  07/06/50
 * @endcode
 *
 */
template < class K, class T, class C = std::less<K>, class A = std::allocator<RW_ALLOC_PARAM_VALUE> >
class RWTPtrMultiMap
{
public:

    /**
     * A type representing a comparison function for \c #key_type that
     * dereferences the arguments before comparing.
     */
    typedef rw_deref_compare<C, K> key_compare;

    /**
     * A type representing the underlying implementation container.
     */
    typedef std::multimap < K*, T*, key_compare, A> container_type;

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

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrMultiMap::key_type \c key_type \endlink instead.
     *
     * Typedef for the type of key.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrMultiMap::key_type instead") K* value_type_key;

    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrMultiMap::mapped_type \c mapped_type \endlink instead.
     *
     * Typedef for the type of value.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrMultiMap::mapped_type instead") T* value_type_data;

    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrMultiMap::key_type \c key_type& \endlink instead.
     *
     * Typedef for a non-\c const reference to a key in this container.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrMultiMap::key_type& instead") K*& reference_key;

    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrMultiMap::mapped_type \c mapped_type& \endlink instead.
     *
     * Typedef for a non-\c const reference to the type of data in this container.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrMultiMap::mapped_type& instead") T*& reference_data;

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

    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrMultiMap::key_compare \c key_compare \endlink instead.
     *
     * A type representing a comparison function that dereferences the
     * arguments before comparing.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrMultiMap::key_compare instead") rw_deref_compare<C, K> container_comp;
#endif

    /**
     * A type representing a comparison function for \c #value_type that
     * dereferences the arguments before comparing.
     */
    typedef typename container_type::value_compare value_compare;


    /**
     * @internal
     * A type representing the same type as self.
     */
    typedef RWTPtrMultiMap<K, T, C, A> this_type;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrMultiMap::mapped_type \c mapped_type \endlink instead.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrMultiMap::mapped_type instead") T data_type;
#endif

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

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12.5, use RWTPtrMultiMap::value_compare instead.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrMultiMap::value_compare instead") rw_deref_pair_compare<C, value_type > deref_pair_compare;
#endif


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
    RW_DEPRECATE_FUNC("Use RWTPtrMultiMap::apply() instead")
    void applyToKeyAndValue(void(*fn)(const K*, const T*, void*), void* d) const {
        apply(fn, d);
    }

    /**
     * @copydoc applyToKeyAndValue()
     */
    RW_DEPRECATE_FUNC("Use RWTPtrMultiMap::apply() instead")
    void applyToKeyAndValue(void(*fn)(const K*, mapped_type&, void*), void* d) {
        apply(fn, d);
    }
#endif

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
     * Removes all associations from the collection and uses
     * <tt>operator delete</tt> to destroy the objects pointed to by the
     * keys and their associated items. Do not use this method if multiple
     * pointers to the same object are stored.
     */
    void clearAndDestroy();

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
     * Clears the collection by removing all items from self.
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
     * <tt>((*\a fn)(a,\a d))</tt> is \c true, then returns \c a. Otherwise,
     * returns <tt>#value_type(#rwnil,#rwnil)</tt>. Client data may be passed through
     * parameter \a d.
     */
    value_type find(bool(*fn)(value_type, void*), void* d) const {
        const_iterator ret = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        return (ret != end()) ? *ret : value_type();
    }

    /**
     * If there exists a key \c j in self that compares equal to
     * <tt>*\a key</tt>, then returns \c j. Otherwise, returns #rwnil.
     */
    const K* find(const K* key) const {
        const_iterator i = std().find(const_cast<key_type>(key));
        return (i != end()) ? (*i).first : rwnil;
    }


    /**
     * If there exists a key \c j in self such that the expression
     * <tt>(*j == *\a key)</tt> is \c true, returns the item associated
     * with \c j. Otherwise, returns #rwnil.
     */
    mapped_type findValue(const K* key)  {
        iterator i = std().find(const_cast<key_type>(key));
        return (i != end()) ? (*i).second : rwnil;
    }

    /**
     * @copydoc findValue()
     */
    const T* findValue(const K* key) const {
        const_iterator i = std().find(const_cast<key_type>(key));
        return (i != end()) ? (*i).second : rwnil;
    }

    /**
     * If there exists a key \c j in self that compares equal to
     * <tt>*\a key</tt>, assigns the item associated with \c j
     * to \c tr, and returns \c j. Otherwise, returns #rwnil and
     * leaves the value of \a tr unchanged.
     */
    const K* findKeyAndValue(const K* key, mapped_type& tr) {
        iterator i = std().find(const_cast<key_type>(key));
        if (i != end()) {
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
     * Order is determined according to the comparison object
     * described by the template parameter \c C.
     */
    const K* minElement() const {
        RW_PRECONDITION(entries() != 0);
        return (*std::min_element(begin(), end(),
                                  impl_.value_comp())).first;
    }

    /**
     * Returns a pointer to the maximum key in the collection.
     * Order is determined according to the comparison object
     * described by the template parameter \c C.
     */
    const K* maxElement() const {
        RW_PRECONDITION(entries() != 0);
        return (*std::max_element(begin(), end(),
                                  impl_.value_comp())).first;
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
     * Removes the first association with key \c j in self such that the
     * expression <tt>(*j == *\a key)</tt> is \c true and returns \c j.
     * Returns #rwnil if there is no such association.
     */
    key_type remove(const K* key) {
        key_type ret = rwnil;
        iterator iter = std().find(const_cast<key_type>(key));
        if (iter != end()) {
            ret = (*iter).first;
            std().erase(iter);
        }
        return ret;
    }

    /**
     * Removes all associations \c a in self such that the expression
     * <tt>((*\a fn)(a,\a d))</tt> is \c true. Returns the number removed.
     * Client data may be passed through parameter \a d.
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
    bool insert(key_type key, mapped_type a) {
        std().insert(value_type(key, a));
        return true;
    }

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @deprecated As of SourcePro 12.5, use insert() instead.
     */
    RW_DEPRECATE_FUNC("Use RWTPtrMultiMap::insert(key_type, mapped_type) instead")
    bool insertKeyAndValue(key_type key, mapped_type a) {
        return insert(key, a);
    }
#endif

    /**
     * Destroys all associations in self and replaces them by copying
     * all associations from \a rhs.
     */
    RWTPtrMultiMap<K, T, C, A>&
    operator=(const RWTPtrMultiMap<K, T, C, A>& rhs) {
        return operator=(rhs.impl_);
    }

    /**
     * @copydoc operator=()
     */
    RWTPtrMultiMap<K, T, C, A>&
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
    RWTPtrMultiMap<K, T, C, A>&
    operator=(RWTPtrMultiMap<K, T, C, A> && rhs) {
        return operator=(rw_move(rhs.impl_));
    }

    /**
     * @copydoc operator=(RWTPtrMultiMap<K,T,C,A>&&)
     */
    RWTPtrMultiMap<K, T, C, A>&
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
     * Constructs an empty map with comparator \a comp.
     */
    RWTPtrMultiMap(const key_compare& comp = key_compare())
        : impl_(comp) { }

    /**
     * Constructs a multimap by copying all elements from \a m.
     */
    RWTPtrMultiMap(const container_type& m) : impl_(m) { }

    /**
     * Copy constructor.
     */
    RWTPtrMultiMap(const RWTPtrMultiMap<K, T, C, A>& rws)
        : impl_(rws.impl_) { }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed list takes ownership of the
     * data owned by \a map.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrMultiMap(RWTPtrMultiMap<K, T, C, A> && map)
#    if !defined(RW_NO_STDMMAP_RVALUE_SUPPORT)
        : impl_(rw_move(map.impl_)) { }
#    else
    {
        map.std().swap(impl_);
    }
#    endif

    /**
      * @copydoc RWTPtrMultiMap(RWTPtrMultiMap<K,T,C,A>&&)
      */
    RWTPtrMultiMap(container_type && map)
#    if !defined(RW_NO_STDMMAP_RVALUE_SUPPORT)
        : impl_(rw_move(map)) { }
#    else
    {
        map.swap(impl_);
    }
#    endif
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs a map by copying elements from the range
     * [\a first, \a last).
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
    template <typename InputIterator>
    RWTPtrMultiMap(InputIterator first, InputIterator last, const key_compare& cmp = key_compare())
#if !defined(RW_BROKEN_STD_MULTIMAP_CTOR_RANGE)
        : impl_(first, last, cmp) { }
#else
        :
        impl_(cmp) {
        insert(first, last);
    }
#endif

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWTPtrMultiMap<K, T, C, A>& rhs) {
        std().swap(rhs.impl_);
    }

private:
    container_type impl_;
};

/**
 * @relates RWTPtrMultiMap
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and if for
 * each key in the first container, the set of values associated with that
 * key is equal to the set of values associated with that key in the second
 * container. Keys and values are dereferenced before being compared.
 */
template <class K, class T, class C , class A>
bool operator==(const RWTPtrMultiMap<K, T, C, A>& lhs, const RWTPtrMultiMap<K, T, C, A>& rhs)
{
    return lhs.entries() == rhs.entries() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrMultiMap<K, T, C, A>::deref_equal());
}

/**
 * @relates RWTPtrMultiMap
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class K, class T, class C , class A>
bool operator!=(const RWTPtrMultiMap<K, T, C, A>& lhs, const RWTPtrMultiMap<K, T, C, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrMultiMap
 *
 * Returns \c true if \a lhs is lexicographically less than \a rhs,
 * otherwise \c false. Items in each collection are dereferenced before
 * being compared. Assumes that types \c K and \c T have well-defined
 * less-than semantics.
 */
template <class K, class T, class C , class A>
bool operator<(const RWTPtrMultiMap<K, T, C, A>& lhs, const RWTPtrMultiMap<K, T, C, A>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrMultiMap<K, T, C, A>::deref_less());
}

/**
 * @relates RWTPtrMultiMap
 *
 * Equivalent to <tt>(\a rhs < \a lhs)</tt>.
 */
template <class K, class T, class C , class A>
bool operator>(const RWTPtrMultiMap<K, T, C, A>& lhs, const RWTPtrMultiMap<K, T, C, A>& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrMultiMap
 *
 * Equivalent to <tt>!(\a rhs < \a lhs)</tt>.
 */
template <class K, class T, class C , class A>
bool operator<=(const RWTPtrMultiMap<K, T, C, A>& lhs, const RWTPtrMultiMap<K, T, C, A>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrMultiMap
 *
 * Equivalent to <tt>!(\a lhs < \a rhs)</tt>.
 */
template <class K, class T, class C , class A>
bool operator>=(const RWTPtrMultiMap<K, T, C, A>& lhs, const RWTPtrMultiMap<K, T, C, A>& rhs)
{
    return !(lhs < rhs);
}

/**
 * @relates RWTPtrMultiMap
 *
 * @copydoc operator==(const RWTPtrMultiMap&, const RWTPtrMultiMap&);
 */
template <class K, class T, class C , class A>
bool operator==(const RWTPtrMultiMap<K, T, C, A>& lhs,
                const typename RWTPtrMultiMap<K, T, C, A>::container_type& rhs)
{
    return lhs.entries() == rhs.size() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrMultiMap<K, T, C, A>::deref_equal());
}

/**
 * @relates RWTPtrMultiMap
 *
 * @copydoc operator!=(const RWTPtrMultiMap&, const RWTPtrMultiMap&);
 */
template <class K, class T, class C , class A>
bool operator!=(const RWTPtrMultiMap<K, T, C, A>& lhs,
                const typename RWTPtrMultiMap<K, T, C, A>::container_type& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrMultiMap
 *
 * @copydoc operator<(const RWTPtrMultiMap&, const RWTPtrMultiMap&);
 */
template <class K, class T, class C , class A>
bool operator<(const RWTPtrMultiMap<K, T, C, A>& lhs,
               const typename RWTPtrMultiMap<K, T, C, A>::container_type& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrMultiMap<K, T, C, A>::deref_less());
}

/**
 * @relates RWTPtrMultiMap
 *
 * @copydoc operator>(const RWTPtrMultiMap&, const RWTPtrMultiMap&);
 */
template <class K, class T, class C , class A>
bool operator>(const RWTPtrMultiMap<K, T, C, A>& lhs,
               const typename RWTPtrMultiMap<K, T, C, A>::container_type& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrMultiMap
 *
 * @copydoc operator<=(const RWTPtrMultiMap&, const RWTPtrMultiMap&);
 */
template <class K, class T, class C , class A>
bool operator<=(const RWTPtrMultiMap<K, T, C, A>& lhs,
                const typename RWTPtrMultiMap<K, T, C, A>::container_type& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrMultiMap
 *
 * @copydoc operator>=(const RWTPtrMultiMap&, const RWTPtrMultiMap&);
 */
template <class K, class T, class C , class A>
bool operator>=(const RWTPtrMultiMap<K, T, C, A>& lhs,
                const typename RWTPtrMultiMap<K, T, C, A>::container_type& rhs)
{
    return !(lhs < rhs);
}


/**
 * @relates RWTPtrMultiMap
 *
 * @copydoc operator==(const RWTPtrMultiMap&, const RWTPtrMultiMap&);
 */
template <class K, class T, class C , class A>
bool operator==(const typename RWTPtrMultiMap<K, T, C, A>::container_type& lhs,
                const RWTPtrMultiMap<K, T, C, A>& rhs)
{
    return lhs.size() == rhs.entries() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrMultiMap<K, T, C, A>::deref_equal());
}

/**
 * @relates RWTPtrMultiMap
 *
 * @copydoc operator!=(const RWTPtrMultiMap&, const RWTPtrMultiMap&);
 */
template <class K, class T, class C , class A>
bool operator!=(const typename RWTPtrMultiMap<K, T, C, A>::container_type& lhs,
                const RWTPtrMultiMap<K, T, C, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrMultiMap
 *
 * @copydoc operator<(const RWTPtrMultiMap&, const RWTPtrMultiMap&);
 */
template <class K, class T, class C , class A>
bool operator<(const typename RWTPtrMultiMap<K, T, C, A>::container_type& lhs,
               const RWTPtrMultiMap<K, T, C, A>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrMultiMap<K, T, C, A>::deref_less());
}

/**
 * @relates RWTPtrMultiMap
 *
 * @copydoc operator>(const RWTPtrMultiMap&, const RWTPtrMultiMap&);
 */
template <class K, class T, class C , class A>
bool operator>(const typename RWTPtrMultiMap<K, T, C, A>::container_type& lhs,
               const RWTPtrMultiMap<K, T, C, A>& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrMultiMap
 *
 * @copydoc operator<=(const RWTPtrMultiMap&, const RWTPtrMultiMap&);
 */
template <class K, class T, class C , class A>
bool operator<=(const typename RWTPtrMultiMap<K, T, C, A>::container_type& lhs,
                const RWTPtrMultiMap<K, T, C, A>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrMultiMap
 *
 * @copydoc operator>=(const RWTPtrMultiMap&, const RWTPtrMultiMap&);
 */
template <class K, class T, class C , class A>
bool operator>=(const typename RWTPtrMultiMap<K, T, C, A>::container_type& lhs,
                const RWTPtrMultiMap<K, T, C, A>& rhs)
{
    return !(lhs < rhs);
}

/**
 * @relates RWTPtrMultiMap
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class K, class T, class C , class A>
RWvostream& operator<<(RWvostream& strm, const RWTPtrMultiMap<K, T, C , A>& coll);

/**
 * @relates RWTPtrMultiMap
 *
 * Saves the collection \a coll onto the RWFile \a strm,
 * or a reference to it if it has already been saved.
 */
template <class K, class T, class C , class A>
RWFile& operator<<(RWFile& strm, const RWTPtrMultiMap<K, T, C , A>& coll);

/**
 * @relates RWTPtrMultiMap
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class K, class T, class C , class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrMultiMap<K, T, C , A>& coll);

/**
 * @relates RWTPtrMultiMap
 *
 * Restores the contents of the collection \a coll from the RWFile
 * \a strm.
 */
template <class K, class T, class C , class A>
RWFile& operator>>(RWFile& strm, RWTPtrMultiMap<K, T, C , A>& coll);

/**
 * @relates RWTPtrMultiMap
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class K, class T, class C , class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrMultiMap<K, T, C , A>*& p);

/**
 * @relates RWTPtrMultiMap
 *
 * Looks at the next object on the RWFile \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class K, class T, class C , class A>
RWFile& operator>>(RWFile& strm, RWTPtrMultiMap<K, T, C , A>*& p);


#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tpmmap.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tpmmap.cc"
#endif

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @ingroup stl_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTPtrMultiMap::iterator to iterate
 * over the elements in this collection.
 *
 * @brief Deprecated. Provides an iterator for RWTPtrMultiMap.
 *
 * RWTPtrMultiMapIterator provides an iterator interface to the
 * C++ Standard Library-based collection represented by RWTPtrMultiMap.(An STL
 * iterator interface is provided through \c RWTPtrMultiMap::iterator.)
 *
 * The order of iteration over an RWTPtrMultiMap is dependent on
 * the comparator object of the container as applied to the key
 * values of the stored associations.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset(). The iterator becomes
 * valid after being advanced with either a pre-increment or operator().
 *
 * For both operator++() and operator()(), iterating past the last
 * element will return a value equivalent to boolean \c false.
 * Continued increments will return a value equivalent to \c false
 * until reset() is called.
 *
 * The \c value type must have \c operator== and \c operator< defined.
 * This requirement is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include<rw/tpmmap.h>
 * RWTPtrMultiMap<K,T,C,A> map;
 * RWTPtrMultiMapIterator<K,T,C,A> itr(map);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/tpmmap.h>
 * #include <iostream>
 * #include <rw/cstring.h>
 * #include <utility>
 *
 * int main() {
 *    RWTPtrMultiMap<RWCString,int,std::less<RWCString> > age;
 *    RWTPtrMultiMapIterator<RWCString,int,std::less<RWCString> > itr(age);
 *
 *    age.insert(new RWCString("John"), new int(30));
 *    age.insert(new RWCString("Steve"),new int(17));
 *    age.insert(new RWCString("Mark"), new int(24));
 *    age.insert(new RWCString("Steve"),new int(24));
 *
 *    while(itr()) {
 *      std::cout << *itr.key() << "\'s age is " << *itr.value()
 *                << std::endl;
 *    }
 *
 *    age.clearAndDestroy();
 *    return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 *
 * John's age is 30
 * Mark's age is 24
 * Steve's age is 17
 * Steve's age is 24
 * @endcode
 *
 */
template <class K, class T, class C = std::less<K>, class A = std::allocator<RW_ALLOC_PARAM_VALUE> >
class RW_DEPRECATE_TYPE("Use RWTPtrMultiMap::iterator instead") RWTPtrMultiMapIterator
{
public:
    /**
     * Creates an iterator for the multimap \a m. The iterator begins
     * in an undefined state and must be advanced before the first element
     * will be accessible.
     */
    RWTPtrMultiMapIterator(RWTPtrMultiMap<K, T, C, A>& m)
        : rwmap_(&m), head_(true), tail_(false) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self will now reference the first element.
     * If, before iteration, self referenced the last association in
     * the multimap, self will now point to an undefined value and a
     * value equivalent to \c false will be returned. Otherwise, a
     * value equivalent to \c true is returned.
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
     * iterator, and returns its key. If the iterator has advanced past
     * the last item in the container, #rwnil will be returned.
     */
    K* operator()() {
        return ++(*this) ? key() : rwnil;
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    RWTPtrMultiMap<K, T, C, A>* container() const {
        return rwmap_;
    }

    /**
     * Returns the key portion of the association currently referenced
     * by self. Undefined if self is not referencing a value within
     * the multimap.
     */
    K* key() const {
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
     * the first element of the collection \a m.
     */
    void reset(RWTPtrMultiMap<K, T, C, A>& m) {
        head_ = true;
        tail_ = false;
        rwmap_ = &m;
    }

    /**
     * Returns the value portion of the association referenced by
     * self. Undefined if self is not referencing a value within the
     * multimap.
     */
    T* value() const {
        return (*iter_).second;
    }

private:
    RWTPtrMultiMap<K, T, C, A>* rwmap_;
    typename RWTPtrMultiMap<K, T, C, A>::iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);
    friend class RWTPtrMultiMapConstIterator<K, T, C, A>;
};

/**
 * @ingroup stl_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTPtrMultiMap::const_iterator to
 * iterate over the elements in this collection.
 *
 * @brief Deprecated. Provides a \c const iterator for RWTPtrMultiMap.
 *
 * RWTPtrMultiMapConstIterator provides a \c const iterator interface
 * to the C++ Standard Library-based RWTPtrMultiMap collection. A
 * \c const iterator prohibits changes to the collection it iterates
 * through.
 *
 * The order of iteration over an RWTPtrMultiMap is dependent on
 * the comparator object of the container as applied to the key
 * values of the stored associations.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset(). The iterator becomes
 * valid after being advanced with either a pre-increment or operator().
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
 * #include <rw/tpmmap.h>
 * template <class K, class T, class C,
 *           class A=std::allocator<T*> >
 * class RWTPtrMultiMapConstIterator
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class K, class T, class C = std::less<K>, class A = std::allocator<RW_ALLOC_PARAM_VALUE> >
class RW_DEPRECATE_TYPE("Use RWTPtrMultiMap::const_iterator instead") RWTPtrMultiMapConstIterator
{
public:
    /**
     * Creates a \c const iterator for the multimap \a m. The iterator
     * begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTPtrMultiMapConstIterator(const RWTPtrMultiMap<K, T, C, A>& m)
        : rwmap_(&m), head_(true), tail_(false) { }

    /**
     * Creates a \c const iterator from the iterator \a itr. The \c const
     * iterator begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTPtrMultiMapConstIterator(const RWTPtrMultiMapIterator<K, T, C, A>& itr)
        : rwmap_(itr.rwmap_), iter_(itr.iter_),
          head_(itr.head_), tail_(itr.tail_) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created, self now references the first element.
     * If, before iteration, self referenced the last association in
     * the multimap, self now points to an undefined value, and a value
     * equivalent to \c false is returned. Otherwise, a value equivalent
     * to \c true is returned.
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
     * iterator, and returns its key. If the iterator has advanced past
     * the last item in the container, #rwnil will be returned.
     */
    const K* operator()() {
        return ++(*this) ? key() : rwnil;
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    const RWTPtrMultiMap<K, T, C, A>* container() const {
        return rwmap_;
    }

    /**
     * Returns the key portion of the association currently referenced
     * by self. Undefined if self is not referencing a value within
     * the multimap.
     */
    const K* key() const {
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
     * the first element of the collection \a m.
     */
    void reset(const RWTPtrMultiMap<K, T, C, A>& m) {
        head_ = true;
        tail_ = false;
        rwmap_ = &m;
    }

    /**
     * Returns the value portion of the association referenced by
     * self. Undefined if self is not referencing a value within the
     * multimap.
     */
    const T* value() const {
        return (*iter_).second;
    }

private:
    const RWTPtrMultiMap<K, T, C, A>* rwmap_;
    typename RWTPtrMultiMap<K, T, C, A>::const_iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);
};

#endif

#undef RW_ALLOC_PARAM_VALUE

#endif /* RW_TOOLS_TPMMAP_H */
