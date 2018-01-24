#ifndef RW_TOOLS_TPSRTVEC_H
#define RW_TOOLS_TPSRTVEC_H

/**********************************************************************
 *
 *     : pointer-based sorted vector wrapper for STL vector
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tpsrtvec.h#2 $
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
#  include "rw/xpsrtvec.h"
#else

#  include <rw/edefs.h>
#  include <rw/epaux.h>
#  include <rw/epfunc.h>
#  include <rw/rwerr.h>
#  include <rw/toolerr.h>
#  include <rw/tools/algorithm.h>
#  include <rw/tools/traits/RWTEnableIf.h>
#  include <rw/tools/traits/RWTIsIntegral.h>

#  include <algorithm>
#  include <vector>

/**
 * @ingroup stl_based_collection_classes
 *
 * @brief A pointer-based collection of values sorted according to
 * a comparison object and implemented as a vector.
 *
 * This class maintains an always-sorted pointer-based collection
 * of values, implemented as a vector. Items are ordered according
 * to a comparison object of type \c C. Class \c T is the type pointed
 * to by the items in the collection. Class \c A is an allocator of objects of
 * class \c T. For more information about using custom allocators, please
 * see the <i>Essential Tools Module User's Guide</i>.

 * \c C must induce a total ordering
 * on elements of type \c T via a public member
 *
 * @code
 * bool operator()(const T& x, const T& y)
 * @endcode
 *
 * which returns \c true if \c x should precede \c y within the
 * collection. The structure \b std::less<T> from the C++ Standard Library header
 * file \c &lt;functional&gt; is an example. Note that items in the collection
 * will be dereferenced before being compared.
 *
 * The \c value type must have \c operator== and \c operator< defined.
 * This requirement is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tpsrtvec.h>
 * RWTPtrSortedVector<T,C,A> srtvec;
 * @endcode
 *
 * @section related Related Classes
 *
 * \link RWTPtrSortedDlist RWTPtrSortedDlist<T,C,A>\endlink is an alternative
 * always-sorted pointer-based collection.
 * \link RWTPtrOrderedVector RWTPtrOrderedVector<T,A>\endlink is an unsorted
 * pointer-based vector.
 *
 * Class <b>std::vector<T*,A<T*> ></b> is the C++ Standard Library collection
 * that serves as the underlying implementation for this class.
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
 * // tpsrtvec.cpp
 * //
 * #include <iostream>
 *
 * #include <rw/tpsrtvec.h>
 * #include <rw/tools/datetime.h>
 *
 * int main()
 * {
 *     RWTPtrSortedVector<RWDateTime RWDefCArgs(RWDateTime)> vec;
 *
 *     vec.insert(new RWDateTime("Aug 10, 1991", RWDateTime::setDate));
 *     vec.insert(new RWDateTime("Aug 9, 1991", RWDateTime::setDate));
 *     vec.insert(new RWDateTime("Sep 1, 1991", RWDateTime::setDate));
 *     vec.insert(new RWDateTime("May 14, 1990", RWDateTime::setDate));
 *     vec.insert(new RWDateTime("Sep 1, 1991", RWDateTime::setDate)); // Add a duplicate
 *     vec.insert(new RWDateTime("June 2, 1991", RWDateTime::setDate));
 *
 *     std::cout << "There are " << vec.entries() << " entries, they are: "
 *               << std::endl << std::endl;
 *
 *     for (size_t i = 0; i < vec.entries(); ++i)
 *         std::cout << vec[i]->asString('x') << std::endl;
 *
 *     vec.clearAndDestroy();
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 *     There are 6 entries, they are:
 *
 *     05/14/90
 *     06/02/91
 *     08/09/91
 *     08/10/91
 *     09/01/91
 *     09/01/91
 * @endcode
 *
 */
template <class T, class C = std::less<T>, class A = std::allocator<T*> >
class RWTPtrSortedVector
{
public:

    /**
     * A type representing a comparison function that dereferences the
     * arguments before comparing.
     */
    typedef rw_deref_compare<C, T> key_compare;

    /**
     * A type representing the underlying implementation container.
     */
    typedef std::vector<T*, A> container_type;

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
     * A type that provides a random-access iterator over the elements
     * in the container.
     */
    typedef typename container_type::iterator iterator;

    /**
     * A type that provides a \c const random-access iterator over the
     * elements in the container.
     */
    typedef typename container_type::const_iterator const_iterator;

    /**
     * A type that provides a random-access, reverse-order iterator over
     * the elements in the container.
     */
    typedef typename container_type::reverse_iterator reverse_iterator;

    /**
     * A type that provides a \c const random-access, reverse-order iterator
     * over the elements in the container.
     */
    typedef typename container_type::const_reverse_iterator const_reverse_iterator;

    /**
     * A type representing a comparison function that dereferences the
     * arguments before comparing.
     */
    typedef key_compare value_compare;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @deprecated As of SourcePro 12.5, use
     * \link RWTPtrSortedVector::key_compare \c key_compare \endlink instead.
     *
     * A type representing a comparison function that dereferences the
     * arguments before comparing.
     */
    typedef RW_DEPRECATE_TYPE("Use RWTPtrSortedVector::key_compare instead") rw_deref_compare<C, T> container_comp;
#endif


    /**
     * @internal
     * A type representing the same type as self.
     */
    typedef RWTPtrSortedVector<T, C, A> this_type;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12.5, use <tt>const T*</tt> instead.
     */
    typedef RW_DEPRECATE_TYPE("Use const T* instead") const T* const_value;
#endif

    /**
     * @internal
     * A type representing a comparison function that dereferences the
     * arguments before comparing.
     */
    typedef rw_deref_compare<std::less<T>, T> deref_less;

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
     * Clears the collection by removing all items from self.
     */
    void clear() {
        std().clear();
    }

    /**
     * Removes all items from the collection and uses <tt>operator delete</tt>
     * to destroy the objects pointed to by those items. Do
     * not use this method if multiple pointers to the same object are
     * stored.
     */
    void clearAndDestroy() {
        rw_destroy(begin(), end());
        clear();
    }

    /**
     * Returns \c true if there exists an element \c t in self such that
     * the expression <tt>((*\a fn)(t,\a d))</tt> is \c true, otherwise
     * returns \c false. Client data may be passed through parameter \a d.
     */
    bool contains(bool(*fn)(const T*, void*), void* d) const {
        return std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d)) != end();
    }

    /**
     * Returns \c true if there exists an element \c t in self such
     * that the expression <tt>(*t == *\a a)</tt> is \c true, otherwise returns
     * \c false.
     */
    bool contains(const T* a) const {
        const_iterator hit = std::lower_bound(begin(), end(), a, key_compare());
        if (hit == end()) {
            return false;
        }
        return (**hit == *a);
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
     * If there exists an element \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, returns \c t. Otherwise,
     * returns #rwnil. Client data may be passed through parameter \a d.
     */
    const T* find(bool(*fn)(const T*, void*), void* d) const {
        const_iterator ret = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        return (ret != end()) ? *ret : rwnil;
    }

    /**
     * If there exists an element \c t in self such that the expression
     * <tt>(*t == *\a a)</tt> is \c true, returns \c t.
     * Otherwise, returns #rwnil.
     */
    value_type find(const T* a) {
        iterator it = std::lower_bound(begin(), end(), a, key_compare());

        return (it != end() && (**it == *a)) ? *it : rwnil;
    }

    /**
     * @copydoc find(const T*)
     */
    const T* find(const T* a) const {
        const_iterator it = std::lower_bound(begin(), end(), a, key_compare());

        return (it != end() && (**it == *a)) ? *it : rwnil;
    }

    /**
     * Returns a reference to the first item in the collection.
     *
     * Calling this function on an empty collection results in
     * undefined behavior.
     */
    reference first() {
        RW_PRECONDITION(!isEmpty());
        return std().front();
    }

    /**
     * @copydoc first()
     */
    const_reference first() const {
        RW_PRECONDITION(!isEmpty());
        return std().front();
    }

    /**
     * Returns the position of the first item \c t in self such that
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, or returns #RW_NPOS if no
     * such item exists. Client data may be passed through parameter
     * \a d.
     */
    size_type index(bool(*fn)(const T*, void*), void* d) const {
        const_iterator res = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        return res != end() ? res - begin() : RW_NPOS;
    }

    /**
     * Returns the position of the first item \c t in self such that
     * <tt>(*t == *\a a)</tt>, or returns #RW_NPOS if no such item exists.
     */
    size_type index(const T* a) const {
        const_iterator it = std::lower_bound(begin(), end(), a, key_compare());
        if (it == end() || !(**it == *a)) {
            return RW_NPOS;
        }
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            it - begin();
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Adds the item \a a to self. The collection remains sorted. Returns
     * \c true.
     */
    bool insert(value_type a) {
        // only fail thru exception
        insertSorted(begin(), a);
        return true;
    }

    /**
     * Inserts \a val into self. Returns an iterator to the new item.
     *
     * The \a hint parameter indicates where in the collection the new value
     * should be inserted. This value is only a hint, and will be ignored if
     * the value should be inserted at a different location according to the
     * container's comparison object.
     */
    iterator insert(iterator hint, value_type val) {
        return insertSorted(hint, val);
    }

    /**
     * Inserts the elements in the range [\a first, \a last) into self.
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last) {
#if !defined(RW_BROKEN_STD_VECTOR_INSERT_RANGE)
        std().insert(std().end(), first, last);
#else
        std::copy(first, last, std::inserter(*this, end()));
#endif
        std::sort(begin(), end(), key_compare());
    }

    /**
     * Inserts all elements of \a a into self. The collection remains
     * sorted. Returns the number of items inserted.
     */
    size_type insert(const container_type& a);

    /**
     * Returns \c true if there are no items in the collection, otherwise
     * \c false.
     */
    bool isEmpty() const {
        return std().empty();
    }

    /**
     * Returns \c true if the collection is sorted relative to the supplied
     * comparator object, otherwise \c false.
     */
    bool isSorted() const {
        return rw_is_sorted(begin(), end(), key_compare());
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
     * Returns the number of elements \c t in self such that the expression
     * <tt>(*t == *\a a)</tt> is \c true.
     */
    size_type occurrencesOf(const T* a) const {
        std::pair<const_iterator, const_iterator> range =
            std::equal_range(begin(), end(), a, key_compare());
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_distance(range.first, range.second);
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Returns a reference to the \a i <sup>th</sup> element of self.
     *
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0 and one
     * less than the number of entries in self.
     */
    reference operator[](size_type i) {
        boundsCheck(i);
        return std()[i];
    }

    /**
     * @copydoc operator[](size_type)
     */
    const_reference operator[](size_type i) const {
        boundsCheck(i);
        return std()[i];
    }

    /**
     * Returns a reference to the \a i <sup>th</sup> element of self.
     * Index \a i must be between \c 0 and one less than the number
     * of entries in self, otherwise the results are undefined.
     * @note
     * No bounds checking is performed.
     */
    reference operator()(size_type i) {
#ifdef RW_DEBUG
        boundsCheck(i);
#endif
        return std()[i];
    }

    // see doxygen bug #612458
    /**
     * Returns a reference to the \a i <sup>th</sup> element of self.
     * Index \a i must be between \c 0 and one less than the number
     * of entries in self, otherwise the results are undefined.
     * @note
     * No bounds checking is performed.
     */
    const_reference operator()(size_type i) const {
#ifdef RW_DEBUG
        boundsCheck(i);
#endif
        return std()[i];
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
     * Removes and returns the first element \c t in self such that the
     * expression <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns #rwnil
     * if there is no such element. Client data may be passed through
     * parameter \a d.
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
     * Removes and returns the first element \c t in self such that the
     * the expression <tt>(*t == *\a a)</tt> is \c true. Returns #rwnil
     * if there is no such element.
     */
    value_type remove(const T* a) {
        value_type ret = rwnil;
        iterator hit = std::lower_bound(begin(), end(), a, key_compare());
        if (hit != end() && (**hit == *a)) {
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
    size_type removeAll(bool(*fn)(const T*, void*), void* d) {
        size_type size = entries();
        iterator iter = std::remove_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        std().erase(iter, end());
        return size - entries();
    }

    /**
     * Removes all elements \c t in self such that the expression
     * <tt>(*t == *\a a)</tt> is \c true. Returns the number of
     * items removed.
     */
    size_type removeAll(const T* a);

    /**
     * Removes and returns the item at position \a i in self.
     *
     * @throw RWBoundsErr Thrown if index \a i is not between
     * \c 0 and one less than the number of entries in the collection.
     */
    value_type removeAt(size_type i) {
        boundsCheck(i);
        iterator iter = begin();
        std::advance(iter, i);
        value_type ret = *iter;
        std().erase(iter);
        return ret;
    }

    /**
     * Removes and returns the first item in the collection.
     *
     * Calling this function on an empty collection results in
     * undefined behavior.
     */
    value_type removeFirst() {
        RW_PRECONDITION(!isEmpty());
        const value_type ret = first();
        erase(begin());
        return ret;
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

    size_type insertAndLocate(value_type a) {
        // Cast the result of subtracting these two indices to a size_type
        // this should be safe as begin will always be less than or
        // equal to the value of insertSorted
        iterator position = insertSorted(begin(), a);
        return static_cast<size_type>(position - begin());
    }

    /*
     * Be careful with these, misuse can lead to the collection
     * becoming out of sorts, so to speak. (We need to allow the
     * index ops for source compatibility )
     */
    /**
     * Invokes the function pointer \a fn on each item in the collection.
     * Client data may be passed through parameter \a d.
     */
    void apply(void(*fn)(reference, void*), void* d) {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * @copydoc apply()
     */
    void apply(void(*fn)(value_type, void*), void* d) {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * @copydoc apply()
     */
    void apply(void(*fn)(const T*, void*), void* d) const {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * Returns a reference to the \a i <sup>th</sup> element of self.
     *
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0 and one
     * less than the number of entries in self.
     */
    reference at(size_type i) {
        return (*this)[i];
    }

    /**
     * @copydoc at()
     */
    const_reference at(size_type i) const {
        return (*this)[i];
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
     * Returns a pointer to the first element of the vector. The value returned
     * is undefined if the vector is empty.
     */
    const_pointer data() const {
#if !defined(RW_NO_STD_VECTOR_DATA)
        // this is the best implementation, but it relies on c++11 behavior
        return std().data();
#else
        // this implementation avoids undefined behavior if vector is empty
        return std().empty() ? 0 : &std().front();
#endif
    }

    /**
     * Returns a reference to the last item in the collection.
     *
     * Calling this function on an empty collection results in
     * undefined behavior.
     */
    reference last() {
        RW_PRECONDITION(!isEmpty());
        return std().back();
    }

    /**
     * @copydoc last()
     */
    const_reference last() const {
        RW_PRECONDITION(!isEmpty());
        return std().back();
    }

    /**
     * Inserts all elements of \a vec into self, preserving sorted order.
     * Returns the number of items inserted.
     */
    size_type merge(const RWTPtrSortedVector<T, C, A>& vec);

    /**
     * Removes and returns the last item in the collection.
     *
     * Calling this function on an empty collection results in
     * undefined behavior.
     */
    value_type removeLast() {
        RW_PRECONDITION(!isEmpty());
        const value_type ret = last();
        std().pop_back();
        return ret;
    }

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * Returns the number of elements in self.
     *
     * @deprecated As of SourcePro 12.5, use entries() instead.
     */
    RW_DEPRECATE_FUNC("Use RWTPtrSortedVector::entries() instead")
    size_type length() const {
        return std().size();
    }
#endif

    /**
     * Modifies, if necessary, the capacity of the vector to be at least
     * as large as \a n.
     */
    void resize(size_type n) {
        std().reserve(n);
    }

    /**
     * Returns the maximum number of elements that can be stored in
     * self without first resizing.
     */
    size_type capacity() const {
        return std().capacity();
    }

    /**
     * Constructs an empty vector.
     */
    RWTPtrSortedVector() { }

    /**
     * Copy constructor.
     */
    RWTPtrSortedVector(const RWTPtrSortedVector<T, C, A>& t)
        : impl_(t.impl_) { }

    /**
     * Constructs a vector by copying and sorting all elements of
     * \a vec.
     */
    RWTPtrSortedVector(const container_type& vec) : impl_(vec) {
        std::sort(begin(), end(), key_compare());
    }

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed vector takes ownership of the
     * data owned by \a vec.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrSortedVector(RWTPtrSortedVector<T, C, A> && vec)
#    if !defined(RW_NO_STDVECTOR_RVALUE_SUPPORT)
        : impl_(rw_move(vec.impl_)) { }
#    else
    {
        vec.std().swap(impl_);
    }
#    endif

    /**
     * Move constructor. The constructed vector sorts and takes ownership of
     * the data owned by \a vec.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrSortedVector(container_type && vec) {
#    if !defined(RW_NO_STDVECTOR_RVALUE_SUPPORT)
        impl_ = rw_move(vec);
#    else
        vec.swap(impl_);
#    endif
        std::sort(begin(), end(), key_compare());
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs a vector with \a n elements, each initialized to
     * \a p.
     */
    RWTPtrSortedVector(size_type n, value_type p) : impl_(n, p) { }

    /**
     * Constructs a vector by copying elements from the range
     * [\a first, \a last).
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
    template <typename InputIterator>
#if !defined(DOXYGEN)
    RWTPtrSortedVector(InputIterator first, InputIterator last,
                       typename RWTEnableIf < !RWTIsIntegral<InputIterator>::value >::type** = 0)
#else
    RWTPtrSortedVector(InputIterator first, InputIterator last)
#endif
#if !defined(RW_BROKEN_STD_VECTOR_CTOR_RANGE)
        : impl_(first, last) {
        std::sort(begin(), end(), key_compare());
    }
#else
        :
        impl_() {
        insert(first, last);
    }
#endif

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @deprecated As of SourcePro 11.1.
     *
     * Constructs an empty vector with a capacity of \a n elements.
     */
    RW_DEPRECATE_FUNC("")
    RWTPtrSortedVector(size_type n) {
        resize(n);
    }
#endif

    // ~RWTPtrSortedVector(); // handled by auto-generated destructor

    /**
     * Empties self, then inserts all elements of \a rhs.
     */
    RWTPtrSortedVector&
    operator=(const RWTPtrSortedVector& rhs) {
        if (&rhs != this) {
#  if defined(_RWSTD_VER) && (_RWSTD_VER < 0x02020300)
            // workaround vector assignment operator bug
            container_type(rhs.impl_).swap(impl_);
#  else
            impl_ = rhs.impl_;
#  endif
        }
        return *this;
    }

    /**
     * Empties self, then sorts and copies all elements of \a rhs.
     */
    RWTPtrSortedVector<T, C, A>&
    operator=(const container_type& rhs) {
        if (&impl_ != &rhs) {
            impl_ = rhs;
            std::sort(begin(), end(), key_compare());
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
    RWTPtrSortedVector<T, C, A>&
    operator=(RWTPtrSortedVector<T, C, A> && rhs) {
        if (&rhs != this) {
#    if !defined(RW_NO_STDVECTOR_RVALUE_SUPPORT)
            impl_ = rw_move(rhs.impl_);
#    else
            rhs.std().swap(impl_);
#    endif
        }
        return *this;
    }

    /**
     * Move assignment. Self sorts and takes ownership of the data owned
     * by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTPtrSortedVector<T, C, A>&
    operator=(container_type && rhs) {
        if (&impl_ != &rhs) {
#    if !defined(RW_NO_STDVECTOR_RVALUE_SUPPORT)
            impl_ = rw_move(rhs);
#    else
            rhs.swap(impl_);
#    endif
            std::sort(begin(), end(), key_compare());
        }
        return *this;
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWTPtrSortedVector<T, C, A>& rhs) {
        std().swap(rhs.impl_);
    }

private:
    void boundsCheck(size_type i) const {
        if (!(i < std().size())) {
            RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEXERR, i, entries())));
        }
    }

    iterator insertSorted(iterator, value_type);

    container_type impl_;
};


/**
 * @relates RWTPtrSortedVector
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and
 * iterating through both collections produces individual elements that,
 * in turn, compare equal to each other. Elements are dereferenced before
 * being compared.
 */
template <class T, class C, class A>
bool operator==(const RWTPtrSortedVector<T, C, A>& lhs, const RWTPtrSortedVector<T, C, A>& rhs)
{
    return lhs.entries() == rhs.entries() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrSortedVector<T, C, A>::deref_equal());
}

/**
 * @relates RWTPtrSortedVector
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T, class C, class A>
bool operator!=(const RWTPtrSortedVector<T, C, A>& lhs, const RWTPtrSortedVector<T, C, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrSortedVector
 *
 * Returns \c true if \a lhs is lexicographically less than \a rhs,
 * otherwise \c false. Items in each collection are dereferenced before
 * being compared. Assumes that type \c T has well-defined less-than
 * semantics.
 */
template <class T, class C, class A>
bool operator<(const RWTPtrSortedVector<T, C, A>& lhs, const RWTPtrSortedVector<T, C, A>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrSortedVector<T, C, A>::deref_less());
}

/**
 * @relates RWTPtrSortedVector
 *
 * Equivalent to <tt>(\a rhs < \a lhs)</tt>.
 */
template <class T, class C, class A>
bool operator>(const RWTPtrSortedVector<T, C, A>& lhs, const RWTPtrSortedVector<T, C, A>& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrSortedVector
 *
 * Equivalent to <tt>!(\a rhs < \a lhs)</tt>.
 */
template <class T, class C, class A>
bool operator<=(const RWTPtrSortedVector<T, C, A>& lhs, const RWTPtrSortedVector<T, C, A>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrSortedVector
 *
 * Equivalent to <tt>!(\a lhs < \a rhs)</tt>.
 */
template <class T, class C, class A>
bool operator>=(const RWTPtrSortedVector<T, C, A>& lhs, const RWTPtrSortedVector<T, C, A>& rhs)
{
    return !(lhs < rhs);
}

/**
 * @relates RWTPtrSortedVector
 *
 * @copydoc operator==(const RWTPtrSortedVector&, const RWTPtrSortedVector&);
 */
template <class T, class C, class A>
bool operator==(const RWTPtrSortedVector<T, C, A>& lhs,
                const typename RWTPtrSortedVector<T, C, A>::container_type& rhs)
{
    return lhs.entries() == rhs.size() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrSortedVector<T, C, A>::deref_equal());
}

/**
 * @relates RWTPtrSortedVector
 *
 * @copydoc operator!=(const RWTPtrSortedVector&, const RWTPtrSortedVector&);
 */
template <class T, class C, class A>
bool operator!=(const RWTPtrSortedVector<T, C, A>& lhs,
                const typename RWTPtrSortedVector<T, C, A>::container_type& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrSortedVector
 *
 * @copydoc operator<(const RWTPtrSortedVector&, const RWTPtrSortedVector&);
 */
template <class T, class C, class A>
bool operator<(const RWTPtrSortedVector<T, C, A>& lhs,
               const typename RWTPtrSortedVector<T, C, A>::container_type& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrSortedVector<T, C, A>::deref_less());
}

/**
 * @relates RWTPtrSortedVector
 *
 * @copydoc operator>(const RWTPtrSortedVector&, const RWTPtrSortedVector&);
 */
template <class T, class C, class A>
bool operator>(const RWTPtrSortedVector<T, C, A>& lhs,
               const typename RWTPtrSortedVector<T, C, A>::container_type& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrSortedVector
 *
 * @copydoc operator<=(const RWTPtrSortedVector&, const RWTPtrSortedVector&);
 */
template <class T, class C, class A>
bool operator<=(const RWTPtrSortedVector<T, C, A>& lhs,
                const typename RWTPtrSortedVector<T, C, A>::container_type& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrSortedVector
 *
 * @copydoc operator>=(const RWTPtrSortedVector&, const RWTPtrSortedVector&);
 */
template <class T, class C, class A>
bool operator>=(const RWTPtrSortedVector<T, C, A>& lhs,
                const typename RWTPtrSortedVector<T, C, A>::container_type& rhs)
{
    return !(lhs < rhs);
}


/**
 * @relates RWTPtrSortedVector
 *
 * @copydoc operator==(const RWTPtrSortedVector&, const RWTPtrSortedVector&);
 */
template <class T, class C, class A>
bool operator==(const typename RWTPtrSortedVector<T, C, A>::container_type& lhs,
                const RWTPtrSortedVector<T, C, A>& rhs)
{
    return lhs.size() == rhs.entries() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                      typename RWTPtrSortedVector<T, C, A>::deref_equal());
}

/**
 * @relates RWTPtrSortedVector
 *
 * @copydoc operator!=(const RWTPtrSortedVector&, const RWTPtrSortedVector&);
 */
template <class T, class C, class A>
bool operator!=(const typename RWTPtrSortedVector<T, C, A>::container_type& lhs,
                const RWTPtrSortedVector<T, C, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWTPtrSortedVector
 *
 * @copydoc operator<(const RWTPtrSortedVector&, const RWTPtrSortedVector&);
 */
template <class T, class C, class A>
bool operator<(const typename RWTPtrSortedVector<T, C, A>::container_type& lhs,
               const RWTPtrSortedVector<T, C, A>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        typename RWTPtrSortedVector<T, C, A>::deref_less());
}

/**
 * @relates RWTPtrSortedVector
 *
 * @copydoc operator>(const RWTPtrSortedVector&, const RWTPtrSortedVector&);
 */
template <class T, class C, class A>
bool operator>(const typename RWTPtrSortedVector<T, C, A>::container_type& lhs,
               const RWTPtrSortedVector<T, C, A>& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates RWTPtrSortedVector
 *
 * @copydoc operator<=(const RWTPtrSortedVector&, const RWTPtrSortedVector&);
 */
template <class T, class C, class A>
bool operator<=(const typename RWTPtrSortedVector<T, C, A>::container_type& lhs,
                const RWTPtrSortedVector<T, C, A>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates RWTPtrSortedVector
 *
 * @copydoc operator>=(const RWTPtrSortedVector&, const RWTPtrSortedVector&);
 */
template <class T, class C, class A>
bool operator>=(const typename RWTPtrSortedVector<T, C, A>::container_type& lhs,
                const RWTPtrSortedVector<T, C, A>& rhs)
{
    return !(lhs < rhs);
}

/**
 * @relates RWTPtrSortedVector
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T, class C , class A>
RWvostream& operator<<(RWvostream& strm, const RWTPtrSortedVector<T, C , A>& coll);

/**
 * @relates RWTPtrSortedVector
 *
 * Saves the collection \a coll onto the RWFile \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T, class C , class A>
RWFile& operator<<(RWFile& strm, const RWTPtrSortedVector<T, C , A>& coll);

/**
 * @relates RWTPtrSortedVector
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class T, class C , class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrSortedVector<T, C , A>& coll);

/**
 * @relates RWTPtrSortedVector
 *
 * Restores the contents of the collection \a coll from the RWFile
 * \a strm.
 */
template <class T, class C , class A>
RWFile& operator>>(RWFile& strm, RWTPtrSortedVector<T, C , A>& coll);

/**
 * @relates RWTPtrSortedVector
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T, class C , class A>
RWvistream& operator>>(RWvistream& strm, RWTPtrSortedVector<T, C , A>*& p);

/**
 * @relates RWTPtrSortedVector
 *
 * Looks at the next object on the RWFile \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T, class C , class A>
RWFile& operator>>(RWFile& strm, RWTPtrSortedVector<T, C , A>*& p);

#  if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#    pragma implementation ("tpsrtvec.cc")
#  endif

#  ifdef RW_NO_IMPLICIT_INCLUSION
#    include "rw/tpsrtvec.cc"
#  endif

#endif /* RW_CLASSIC_TEMPLATE_CONTAINERS */

#endif /* RW_TOOLS_TPSRTVEC_H */
