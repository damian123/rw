#ifndef RW_TOOLS_TPMAP_H
#define RW_TOOLS_TPMAP_H

/**********************************************************************
 *
 * tpmap.h - RWTPtrMap<Key,T,Compare>
 *     : pointer-based key/data dictionary wrapper for STL map
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tpmap.h#3 $
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
class RWTPtrMapConstIterator;

#endif

/**
 * @ingroup stl_based_collection_classes
 *
 * @brief A pointer-based collection of associations ordered according to
 * a comparison object.
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
 * of type \c K via a public member.
 *
 * @code
 * bool operator()(const K& x, const K& y)
 * @endcode
 *
 * which returns \c true if \c x and its partner should precede
 * \c y and its partner within the collection. The structure \b std::less<T>
 * from the C++ Standard Library header file \c \<functional\> is
 * an example. Note that keys will be dereferenced before being
 * compared.
 *
 * \link RWTPtrMap RWTPtrMap<K,T,C,A>\endlink will not accept a key that
 * compares equal to any key already in the collection.
 * (\link RWTPtrMultiMap RWTPtrMultiMap<K,T,C,A>\endlink
 * may contain multiple keys that compare equal to each other.)
 * Equality is based on the comparison object and not on the equality
 * operator. Given a comparison object \c comp, keys \c a and
 * \c b are equal if <tt>!comp(a,b) && !comp(b,a)</tt>.
 *
 * The \c value type must have \c operator== and \c operator< defined.
 * This requirement is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tpmap.h>
 * RWTPtrMap<K,T,C,A> m;
 * @endcode
 *
 * @section related Related Classes
 *
 * Class \link RWTPtrMultiMap RWTPtrMultiMap<K,T,C,A>\endlink offers the same
 * interface to a pointer-based collection that accepts multiple keys that
 * compare equal to each other. \link RWTPtrSet RWTPtrSet<T,C,A>\endlink
 * maintains a pointer-based collection of keys without the associated items.
 *
 * Class <tt>map<K*,T*,deref_compare<C,K,A> ></tt> is the C++ Standard Library
 * collection that serves as the underlying implementation for this collection.
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
 * // tpmap.cpp
 * //
 * #include <iostream>
 * #include <functional>
 *
 * #include <rw/tpmap.h>
 * #include <rw/cstring.h>
 * #include <rw/tools/datetime.h>
 *
 * int main ()
 * {
 *     RWTPtrMap<RWCString, RWDateTime, std::less<RWCString> > birthdays;
 *
 *     birthdays.insert (new RWCString("John"),
 *                       new RWDateTime("April 12, 1975", RWDateTime::setDate));
 *
 *     birthdays.insert (new RWCString("Ivan"),
 *                       new RWDateTime("Nov 2, 1980", RWDateTime::setDate));
 *
 *     // Alternative syntax:
 *     birthdays[new RWCString("Susan")] = new RWDateTime("June 30, 1955", RWDateTime::setDate);
 *     birthdays[new RWCString("Gene")] = new RWDateTime("Jan 5, 1981", RWDateTime::setDate);
 *
 *     // Print a birthday:
 *     RWCString key("John");
 *     std::cout << birthdays[&key]->asString('x') << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 *
 * 04/12/75
 * @endcode
 *
 */
template <class K, class T, class C = std::less<K>, class A = std::allocator<RW_ALLOC_PARAM_VALUE> >
class RWTPtrMap
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
    typedef std::map < K*, T*, key_compare, A> container_type;

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
     * A type representing a comparison function for \c #value_type that
     * dereferences the arguments before comparing.
     */
    typedef typename container_type::value_compare value_compare;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrMap::key_type \c key_type \endlink instead.
     *
     * Typedef for the type of key.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrMap::key_type instead") K* value_type_key;

    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrMap::mapped_type \c mapped_type \endlink instead.
     *
     * Typedef for the type of value.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrMap::mapped_type instead") T* value_type_data;

    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrMap::key_type \c key_type& \endlink instead.
     *
     * Typedef for a non-\c const reference to a key in this container.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrMap::key_type& instead") K*& reference_key;

    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrMap::mapped_type \c mapped_type& \endlink instead.
     *
     * Typedef for a non-\c const reference to the type of data in this container.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrMap::mapped_type& instead") T*& reference_data;

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
     * \link RWTPtrMap::key_compare \c key_compare \endlink
     * instead.
     *
     * A type representing a comparison function that dereferences the
     * arguments before comparing.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrMap::key_compare instead") rw_deref_compare<C, K> container_comp;
#endif

    /**
     * @internal
     * A type representing the same type as self.
     */
    typedef RWTPtrMap<K, T, C, A> this_type;

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
     * @deprecated As of SourcePro 12.5, use RWTPtrMap::value_compare instead.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrMap::value_compare instead") rw_deref_pair_compare<C, value_type > deref_pair_compare;
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
    RW_DEPRECATE_FUNC("Use RWTPtrMap::apply() instead")
    void applyToKeyAndValue(void(*fn)(const K*, const T*, void*), void* d) const {
        apply(fn, d);
    }

    /**
     * @copydoc applyToKeyAndValue()
     */
    RW_DEPRECATE_FUNC("Use RWTPtrMap::apply() instead")
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
     * <tt>operator delete</tt> to destroy the objects pointed to by the keys
     * and their associated items. Do not use this method if multiple pointers
     * to the same object are stored. (This could happen even if keys
     * all compare different, since items are not considered during
     * comparison.)
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
#if !defined(RW_NO_STD_MAP_ERASE_RETURN)
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
#if !defined(RW_NO_STD_MAP_ERASE_RETURN)
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
     * <tt>*\a key</tt>, then returns \c j. Otherwise, returns
     * #rwnil.
     */
    const K* find(const K* key) const {
        const_iterator i = std().find(const_cast<key_type>(key));
        return (i != end()) ? (*i).first : rwnil;
    }

    /**
     * If there exists a key \c j in self that compares equal to
     * <tt>*\a key</tt>, returns the item associated with \c j.
     * Otherwise, returns #rwnil.
     */
    mapped_type findValue(const K* key) {
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
     * If there exists a key \c j in self that compares equal to <tt>*\a key</tt>,
     * assigns the item associated with \c j to \c tr, and returns \c j.
     * Otherwise, returns #rwnil and leaves the value of \a tr unchanged.
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
    size_type occurrencesOf(const K* k) const {
        return std().count(const_cast <key_type>(k));
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
     * Removes the first association with key \c j in self that compares
     * equal to <tt>*\a key</tt> and returns \c j. Returns #rwnil if there
     * is no such association.
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
     * Removes all associations with key \c j in self that compares equal
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
     * Attempts to insert \a val into self. If an existing item with an
     * equivalent key is already in the collection, \a val is not inserted,
     * and an iterator to the existing item is returned. Otherwise inserts
     * \a val and returns an iterator to the new item.
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
#if !defined(RW_BROKEN_STD_MAP_INSERT_RANGE)
        std().insert(first, last);
#else
        std::copy(first, last, std::inserter(*this, end()));
#endif
    }

    /**
     * Adds \a key with associated item \a a to the collection. Returns
     * \c true if the insertion is successful, otherwise returns \c false.
     * Insertion will fail if the collection already holds an association
     * with an equivalent key.
     */
    bool insert(key_type key, mapped_type a) {
        return (std().insert(value_type(key, a))).second;
    }

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @deprecated As of SourcePro 1, use insert() instead.
     */
    RW_DEPRECATE_FUNC("Use RWTPtrMap::insert(key_type, mapped_type) instead")
    bool insertKeyAndValue(key_type key, mapped_type a) { // deprecated
        return insert(key, a);
    }
#endif

    /**
     * Looks up \a key and returns a reference to its associated item.
     * If the key is not in the dictionary, then it will be added with
     * an associated uninitialized pointer of type \c #mapped_type. Because
     * of this, if there is a possibility that a key will not be in the
     * dictionary, then this operator should be used only as an lvalue.
     */
    mapped_type& operator[](const K* key) {
        return impl_[(key_type)key];
    }

    /**
     * Destroys all associations in self and replaces them by copying
     * all associations from \a rhs.
     */
    RWTPtrMap<K, T, C, A>&
    operator=(const RWTPtrMap<K, T, C, A>& rhs) {
        return operator=(rhs.impl_);
    }

    /**
     * @copydoc operator=()
     */
    RWTPtrMap<K, T, C, A>&
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
    RWTPtrMap<K, T, C, A>&
    operator=(RWTPtrMap<K, T, C, A> && rhs) {
        return operator=(rw_move(rhs.impl_));
    }

    /**
     * @copydoc operator=(RWTPtrMap<K,T,C,A>&&)
     */
    RWTPtrMap<K, T, C, A>&
    operator=(container_type && rhs) {
        if (&impl_ != &rhs) {
#    if !defined(RW_NO_STDMAP_RVALUE_SUPPORT)
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
    RWTPtrMap(const key_compare& comp = key_compare())
        : impl_(comp) { }
    /**
     * Constructs a map by copying all elements from \a m.
     */
    RWTPtrMap(const container_type& m) : impl_(m) { }

    /**
     * Copy constructor.
     */
    RWTPtrMap(const RWTPtrMap<K, T, C, A>& rws)
        : impl_(rws.impl_) { }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed list takes ownership of the
     * data owned by \a map.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrMap(RWTPtrMap<K, T, C, A> && map)
#    if !defined(RW_NO_STDMAP_RVALUE_SUPPORT)
        : impl_(rw_move(map.impl_)) { }
#    else
    {
        map.std().swap(impl_);
    }
#    endif

    /**
     * @copydoc RWTPtrMap(RWTPtrMap<K,T,C,A>&&)
     */
    RWTPtrMap(container_type && map)
#    if !defined(RW_NO_STDMAP_RVALUE_SUPPORT)
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
    RWTPtrMap(InputIterator first, InputIterator last, const key_compare& cmp = key_compare())
#if !defined(RW_BROKEN_STD_MAP_CTOR_RANGE)
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
    void swap(RWTPtrMap<K, T, C, A>& rhs) {
        std().swap(rhs.impl_);
    }

private:
    container_type impl_;
};


/**
 * @relates RWTPtrMap
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and if for
 * each key in the first container, the set of values associated with that
 * key is equal to the set of values associated with that key in the second
 * container. Keys and values are dereferenced before being compared.
 */
template <class K, class T, class C , class A>
bool operator==(const RWTPtrMap<K, T, C, A>& lhs, const RWTPtrMap<K, T, C, A>& rhs)
{
    return lhs.entries() == rhs.entries() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrMap<K, T, C, A>::deref_equal());
}

/**
 * @relates RWTPtrMap
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class K, class T, class C , class A>
bool operator!=(const RWTPtrMap<K, T, C, A>& lhs, const RWTPtrMap<K, T, C, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrMap
 *
 * Returns \c true if \a lhs is lexicographically less than \a rhs,
 * otherwise \c false. Items in each collection are dereferenced before
 * being compared. Assumes that types \c K and \c T have well-defined
 * less-than semantics.
 */
template <class K, class T, class C , class A>
bool operator<(const RWTPtrMap<K, T, C, A>& lhs, const RWTPtrMap<K, T, C, A>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrMap<K, T, C, A>::deref_less());
}

/**
 * @relates RWTPtrMap
 *
 * Equivalent to <tt>(\a rhs < \a lhs)</tt>.
 */
template <class K, class T, class C , class A>
bool operator>(const RWTPtrMap<K, T, C, A>& lhs, const RWTPtrMap<K, T, C, A>& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrMap
 *
 * Equivalent to <tt>!(\a rhs < \a lhs)</tt>.
 */
template <class K, class T, class C , class A>
bool operator<=(const RWTPtrMap<K, T, C, A>& lhs, const RWTPtrMap<K, T, C, A>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrMap
 *
 * Equivalent to <tt>!(\a lhs < \a rhs)</tt>.
 */
template <class K, class T, class C , class A>
bool operator>=(const RWTPtrMap<K, T, C, A>& lhs, const RWTPtrMap<K, T, C, A>& rhs)
{
    return !(lhs < rhs);
}

/**
 * @relates RWTPtrMap
 *
 * @copydoc operator==(const RWTPtrMap&, const RWTPtrMap&);
 */
template <class K, class T, class C , class A>
bool operator==(const RWTPtrMap<K, T, C, A>& lhs,
                const typename RWTPtrMap<K, T, C, A>::container_type& rhs)
{
    return lhs.entries() == rhs.size() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrMap<K, T, C, A>::deref_equal());
}

/**
 * @relates RWTPtrMap
 *
 * @copydoc operator!=(const RWTPtrMap&, const RWTPtrMap&);
 */
template <class K, class T, class C , class A>
bool operator!=(const RWTPtrMap<K, T, C, A>& lhs,
                const typename RWTPtrMap<K, T, C, A>::container_type& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrMap
 *
 * @copydoc operator<(const RWTPtrMap&, const RWTPtrMap&);
 */
template <class K, class T, class C , class A>
bool operator<(const RWTPtrMap<K, T, C, A>& lhs,
               const typename RWTPtrMap<K, T, C, A>::container_type& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrMap<K, T, C, A>::deref_less());
}

/**
 * @relates RWTPtrMap
 *
 * @copydoc operator>(const RWTPtrMap&, const RWTPtrMap&);
 */
template <class K, class T, class C , class A>
bool operator>(const RWTPtrMap<K, T, C, A>& lhs,
               const typename RWTPtrMap<K, T, C, A>::container_type& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrMap
 *
 * @copydoc operator<=(const RWTPtrMap&, const RWTPtrMap&);
 */
template <class K, class T, class C , class A>
bool operator<=(const RWTPtrMap<K, T, C, A>& lhs,
                const typename RWTPtrMap<K, T, C, A>::container_type& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrMap
 *
 * @copydoc operator>=(const RWTPtrMap&, const RWTPtrMap&);
 */
template <class K, class T, class C , class A>
bool operator>=(const RWTPtrMap<K, T, C, A>& lhs,
                const typename RWTPtrMap<K, T, C, A>::container_type& rhs)
{
    return !(lhs < rhs);
}


/**
 * @relates RWTPtrMap
 *
 * @copydoc operator==(const RWTPtrMap&, const RWTPtrMap&);
 */
template <class K, class T, class C , class A>
bool operator==(const typename RWTPtrMap<K, T, C, A>::container_type& lhs,
                const RWTPtrMap<K, T, C, A>& rhs)
{
    return lhs.size() == rhs.entries() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrMap<K, T, C, A>::deref_equal());
}

/**
 * @relates RWTPtrMap
 *
 * @copydoc operator!=(const RWTPtrMap&, const RWTPtrMap&);
 */
template <class K, class T, class C , class A>
bool operator!=(const typename RWTPtrMap<K, T, C, A>::container_type& lhs,
                const RWTPtrMap<K, T, C, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrMap
 *
 * @copydoc operator<(const RWTPtrMap&, const RWTPtrMap&);
 */
template <class K, class T, class C , class A>
bool operator<(const typename RWTPtrMap<K, T, C, A>::container_type& lhs,
               const RWTPtrMap<K, T, C, A>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrMap<K, T, C, A>::deref_less());
}

/**
 * @relates RWTPtrMap
 *
 * @copydoc operator>(const RWTPtrMap&, const RWTPtrMap&);
 */
template <class K, class T, class C , class A>
bool operator>(const typename RWTPtrMap<K, T, C, A>::container_type& lhs,
               const RWTPtrMap<K, T, C, A>& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrMap
 *
 * @copydoc operator<=(const RWTPtrMap&, const RWTPtrMap&);
 */
template <class K, class T, class C , class A>
bool operator<=(const typename RWTPtrMap<K, T, C, A>::container_type& lhs,
                const RWTPtrMap<K, T, C, A>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrMap
 *
 * @copydoc operator>=(const RWTPtrMap&, const RWTPtrMap&);
 */
template <class K, class T, class C , class A>
bool operator>=(const typename RWTPtrMap<K, T, C, A>::container_type& lhs,
                const RWTPtrMap<K, T, C, A>& rhs)
{
    return !(lhs < rhs);
}

/**
 * @relates RWTPtrMap
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class K, class T, class C , class A>
RWvostream& operator<<(RWvostream& strm, const RWTPtrMap<K, T, C , A>& coll);

/**
 * @relates RWTPtrMap
 *
 * Saves the collection \a coll onto the RWFile \a strm,
 * or a reference to it if it has already been saved.
 */
template <class K, class T, class C , class A>
RWFile& operator<<(RWFile& strm, const RWTPtrMap<K, T, C , A>& coll);

/**
 * @relates RWTPtrMap
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class K, class T, class C , class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrMap<K, T, C , A>& coll);

/**
 * @relates RWTPtrMap
 *
 * Restores the contents of the collection \a coll from the RWFile \a strm.
 */
template <class K, class T, class C , class A>
RWFile& operator>>(RWFile& strm, RWTPtrMap<K, T, C , A>& coll);

/**
 * @relates RWTPtrMap
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class K, class T, class C , class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrMap<K, T, C , A>*& p);

/**
 * @relates RWTPtrMap
 *
 * Looks at the next object on the RWFile \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class K, class T, class C , class A>
RWFile& operator>>(RWFile& strm, RWTPtrMap<K, T, C , A>*& p);

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tpmap.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tpmap.cc"
#endif

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @ingroup stl_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTPtrMap::iterator to iterate over the
 * elements in this collection.
 *
 * @brief Deprecated. Provides an iterator for RWTPtrMap.
 *
 * RWTPtrMapIterator provides an iterator interface to the
 * C++ Standard Library-based collection represented by RWTPtrMap. (An STL
 * iterator interface is provided through RWTPtrMap::iterator.)
 *
 * The order of iteration over an RWTPtrMap is dependent on the
 * comparator object supplied as applied to the key values of the
 * stored associations.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset(). The iterator becomes
 * valid after being advanced with either a pre-increment or operator()().
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
 * #include<rw/tpmap.h>
 * RWTPtrMap<K,T,C> map;
 * RWTPtrMapIterator<K,T,C> itr(map);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/tpmap.h>
 * #include <iostream>
 * #include <rw/cstring.h>
 *
 * int main() {
 *    RWTPtrMap<RWCString,int,std::less<RWCString> > age;
 *    RWTPtrMapIterator<RWCString,int,std::less<RWCString> > itr(age);
 *
 *    age.insert(new RWCString("John") ,new int(30));
 *    age.insert(new RWCString("Steve"),new int(17));
 *    age.insert(new RWCString("Mark") ,new int(24));
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
 * @endcode
 *
 */
template <class K, class T, class C = std::less<K>, class A = std::allocator<RW_ALLOC_PARAM_VALUE> >
class RW_DEPRECATE_TYPE("Use RWTPtrMap::iterator instead") RWTPtrMapIterator
{
public:
    /**
     * Creates an iterator for the map \a rwm. The iterator begins
     * in an undefined state and must be advanced before the first element
     * will be accessible.
     */
    RWTPtrMapIterator(RWTPtrMap<K, T, C, A>& rwm)
        : rwmap_(&rwm), head_(true), tail_(false) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created self will now reference the first element.
     * If, before iteration, self referenced the last association in
     * the multimap, self will now point to an undefined value, and a
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
    RWTPtrMap<K, T, C, A>* container() const {
        return rwmap_;
    }

    /**
     * Returns the key portion of the association currently referenced
     * by self. Undefined if self is not referencing a value within
     * the map.
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
    void reset(RWTPtrMap<K, T, C, A>& m) {
        head_ = true;
        tail_ = false;
        rwmap_ = &m;
    }

    /**
     * Returns the value portion of the association pointed to by self.
     * Undefined if self is not referencing a value within the map.
     */
    T* value() const {
        return (*iter_).second;
    }

private:
    RWTPtrMap<K, T, C, A>* rwmap_;
    typename RWTPtrMap<K, T, C, A>::iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);
    friend class RWTPtrMapConstIterator<K, T, C, A>;
};

/**
 * @ingroup stl_based_collection_classes
 *
 * @deprecated As of SourcePro 12.5, use RWTPtrMap::const_iterator to iterate
 * over the elements in this collection.
 *
 * @brief Deprecated. Provides a \c const iterator for RWTPtrMap.
 *
 * RWTPtrMapIterator provides a \c const iterator interface to the
 * C++ Standard Library-based RWTPtrMap collection. As a \c const iterator,
 * it prohibits any change to the collection upon which it iterates.
 *
 * The order of iteration over an RWTPtrMap is dependent on the
 * comparator object supplied as applied to the key values of the
 * stored associations.
 *
 * The current item referenced by this iterator is undefined after
 * construction or after a call to reset(). The iterator becomes
 * valid after being advanced with either a preincrement or operator()().
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
 * #include <rw/tpmap.h>
 * template <class K, class T, class C,
 *           class A=std::allocator<T*> >
 * class RWTPtrMapConstIterator
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class K, class T, class C = std::less<K>, class A = std::allocator<RW_ALLOC_PARAM_VALUE> >
class RW_DEPRECATE_TYPE("Use RWTPtrMap::const_iterator instead") RWTPtrMapConstIterator
{
public:
    /**
     * Creates a \c const iterator for the map \a rwm. The iterator begins
     * in an undefined state and must be advanced before the first element
     * is accessible.
     */
    RWTPtrMapConstIterator(const RWTPtrMap<K, T, C, A>& rwm)
        : rwmap_(&rwm), head_(true), tail_(false) { }

    /**
     * Creates a \c const iterator from the iterator \a itr. The \c const
     * iterator begins in an undefined state and must be advanced before the
     * first element is accessible.
     */
    RWTPtrMapConstIterator(const RWTPtrMapIterator<K, T, C, A>& itr)
        : rwmap_(itr.rwmap_), iter_(itr.iter_),
          head_(itr.head_), tail_(itr.tail_) { }

    /**
     * Advances self to the next element. If the iterator has been
     * reset or just created self now references the first element.
     * If, before iteration, self referenced the last association in
     * the multimap, self now points to an undefined value, and a value
     * equivalent to \c false is returned. Otherwise, a value equivalent
     * to \c true is returned.
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
    const K* operator()() {
        return ++(*this) ? key() : rwnil;
    }

    /**
     * Returns a pointer to the collection being iterated over.
     */
    const RWTPtrMap<K, T, C, A>* container() const {
        return rwmap_;
    }

    /**
     * Returns the key portion of the association currently referenced
     * by self. Undefined if self is not referencing a value within
     * the map.
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
    void reset(const RWTPtrMap<K, T, C, A>& m) {
        head_ = true;
        tail_ = false;
        rwmap_ = &m;
    }

    /**
     * Returns the value portion of the association pointed to by self.
     * Undefined if self is not referencing a value within the map.
     */
    const T* value() const {
        return (*iter_).second;
    }

private:
    const RWTPtrMap<K, T, C, A>* rwmap_;
    typename RWTPtrMap<K, T, C, A>::const_iterator iter_;
    bool head_;
    bool tail_;

    // Disallow postfix increment. Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool operator++(int);
};
#endif

#undef RW_ALLOC_PARAM_VALUE

#endif /* RW_TOOLS_TPMAP_H */
