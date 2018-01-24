#ifndef RW_TOOLS_TVSRTVEC_H
#define RW_TOOLS_TVSRTVEC_H

/**********************************************************************
 *
 *     : value-based sorted vector wrapper for STL vector
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvsrtvec.h#2 $
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
#include <rw/rwerr.h>
#include <rw/toolerr.h>

#if defined(RW_CLASSIC_TEMPLATE_CONTAINERS)
#  include "rw/xvsrtvec.h"
#else
// STL-based implementation:

#  include <rw/epfunc.h>
#  include <rw/tools/algorithm.h>
#  include <rw/tools/traits/RWTEnableIf.h>
#  include <rw/tools/traits/RWTIsIntegral.h>

#  include <algorithm>
#  include <functional>
#  include <vector>

/**
 * @ingroup stl_based_collection_classes
 *
 * @brief Maintains a sorted collection of values implemented as a vector.
 *
 * This class maintains an always-sorted collection of values, implemented
 * as a vector.
 *
 * Class \c T is the type of items in the collection.
 * Class \c A is an allocator of objects of class \c T. For more
 * information about using custom allocators, please see the
 * <i>Essential Tools Module User's Guide</i>.
 *
 * The \c value type must have \c operator== and \c operator< defined.
 * This requirement is imposed by the C++ Standard Library.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvsrtvec.h>
 * RWTValSortedVector<T,C,A> srtvec;
 * @endcode
 *
 * @section related Related Classes
 *
 * \link RWTValSortedDlist RWTValSortedDlist<T,C,A>\endlink is an alternative
 * always-sorted collection.
 * \link RWTValOrderedVector RWTValOrderedVector<T,A>\endlink is an unsorted
 * vector of values.
 *
 * Class RWTValSortedVector<bool,C,A> provides a space-efficient vector of
 * boolean values.
 *
 * Class <b>std::vector<T,A></b> is the C++ Standard Library collection that serves
 * as the underlying implementation for this class.
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
 * // tvsvcdat.cpp
 * //
 * #include <iostream>
 * #include <functional>
 *
 * #include <rw/tvsrtvec.h>
 * #include <rw/tools/datetime.h>
 *
 * int main ()
 * {
 *     RWTValSortedVector<RWDateTime, std::less<RWDateTime> > vec;
 *
 *     vec.insert(RWDateTime("Aug 10, 1991", RWDateTime::setDate));
 *     vec.insert(RWDateTime("Aug 9, 1991", RWDateTime::setDate));
 *     vec.insert(RWDateTime("Sep 1, 1991", RWDateTime::setDate));
 *     vec.insert(RWDateTime("May 14, 1990", RWDateTime::setDate));
 *     vec.insert(RWDateTime("Sep 1, 1991", RWDateTime::setDate)); // Add a duplicate
 *     vec.insert(RWDateTime("June 2, 1991", RWDateTime::setDate));
 *
 *     for (size_t i = 0; i < vec.entries (); ++i)
 *         std::cout << vec[i].asString('x') << "\n";
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Output:
 * @code
 *     05/14/90
 *     06/02/91
 *     08/09/91
 *     08/10/91
 *     09/01/91
 *     09/01/91
 * @endcode
 */
template <class T, class C = std::less<T>, class A = std::allocator<T> >
class RWTValSortedVector
{

public:

    /**
     * A type representing the underlying implementation container.
     */
    typedef std::vector<T, A> container_type;

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
     * A type representing the comparison function.
     */
    typedef C key_compare;

    /**
     * A type representing the comparison function.
     */
    typedef C value_compare;

    /**
     * @internal
     * A type representing the same type as self.
     */
    typedef RWTValSortedVector<T, C, A> this_type;


    /**
     * Returns a reference to the underlying C++ Standard Library collection
     * that serves as the implementation for self. It is the application's
     * responsibility to not violate the ordering of the elements within
     * the collection.
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
     * Clears the collection by removing all items from self. Each
     * item has its destructor called.
     */
    void clear() {
        std().erase(std().begin(), std().end());
    }

    /**
     * Returns \c true if there exists an element \c t in self such
     * that the expression <tt>((*\a fn)(t,\a d))</tt> is \c true; otherwise
     * returns \c false. Client data may be passed through parameter \a d.
     */
    bool contains(bool(*fn)(const_reference, void*), void* d) const {
        return std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d)) != end();
    }

    /**
     * Returns \c true if there exists an element \c t in self such
     * that the expression <tt>(t == \a a)</tt> is \c true; otherwise returns
     * \c false.
     */
    bool contains(const_reference a) const {
        const_iterator hit = std::lower_bound(begin(), end(), a, key_compare());
        if (hit == end()) {
            return false;
        }
        return (*hit == a);
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
     * If there exists an element \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, assigns \c t to \a k and
     * returns \c true. Otherwise, returns \c false and leaves the value
     * of \a k unchanged. Client data may be passed through parameter \a d.
     */
    bool find(bool(*fn)(const_reference, void*), void* d, value_type& k) const {
        const_iterator ret = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        if (ret != end()) {
            k = *ret;
            return true;
        }
        return false;
    }

    /**
     * If there exists an element \c t in self such that the expression
     * <tt>(t == \a a)</tt> is \c true, assigns \c t to \a k and returns
     * \c true. Otherwise, returns \c false and leaves the value of \a k
     * unchanged.
     */
    bool find(const_reference a, value_type& k) const {
        const_iterator hit = std::lower_bound(begin(), end(), a, key_compare());
        if (hit != end() && *hit == a) {
            k = *hit;
            return true;
        }
        return false;
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
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, or returns #RW_NPOS if
     * no such item exists. Client data may be passed through parameter
     * \a d.
     */
    size_type index(bool(*fn)(const_reference, void*), void* d) const {
        const_iterator res = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        return res != end() ? res - begin() : RW_NPOS;
    }

    /**
     * Returns the position of the first item \c t in self such that
     * <tt>(t == \a a)</tt>, or returns #RW_NPOS if no such item exists.
     */
    size_type index(const_reference a) const {
        const_iterator it = std::lower_bound(begin(), end(), a, key_compare());
        if (it == end() || !(*it == a)) {
            return RW_NPOS;
        }
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            it - begin();
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }


    /**
     * Adds the item \a a to self. The collection remains sorted.
     * Returns \c true.
     */
    bool insert(const_reference a) {
        insertSorted(begin(), a);  // only fail thru exception
        return true;
    }

    /**
     * Inserts \a val into self. Returns an iterator to the new item.
     *
     * The \a hint parameter indicates where in the collection the new value
     * should be inserted. This value is only a hint, and will be ignored if
     * the value should be inserted at a different location according to the
     * containers comparison object.
     */
    iterator insert(iterator hint, const_reference val) {
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

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @copydoc insert()
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    bool insert(value_type && a) {
        insertSorted(begin(), rw_move(a));  // only fail thru exception
        return true;
    }

    /**
     * @copydoc insert(iterator, const_reference)
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    iterator insert(iterator hint, value_type && val) {
        return insertSorted(hint, rw_move(val));
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Inserts all elements of \a a into self. The collection remains
     * sorted. Returns the number of items inserted.
     */
    size_type insert(const container_type& a);

    /**
     * Returns \c true if there are no items in the collection,
     * otherwise \c false.
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
    size_type occurrencesOf(bool(*fn)(const_reference, void*), void* d) const {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Returns the number of elements \c t in self such that the expression
     * <tt>(t == \a a)</tt> is \c true.
     */
    size_type occurrencesOf(const_reference a) const {
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
     * Index \a i should be between \c 0 and one less than the number
     * of entries; otherwise, the results are undefined.
     * @note
     * No bounds checking is performed.
     */
    reference operator()(size_type i) {
        RW_PRECONDITION(i < entries());
        return std()[i];
    }

    // see doxygen bug #612458
    /**
     * Returns a reference to the \a i <sup>th</sup> element of self.
     * Index \a i should be between \c 0 and one less than the number
     * of entries; otherwise, the results are undefined.
     * @note
     * No bounds checking is performed.
     */
    const_reference operator()(size_type i) const {
        RW_PRECONDITION(i < entries());
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
     * Removes the first element \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true and returns \c true. Returns
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
     * Removes the first element \c t in self such that the expression
     * <tt>(t == \a a)</tt> is \c true and returns \c true. Returns
     * \c false if there is no such element.
     */
    bool remove(const_reference a) {
        iterator hit = std::lower_bound(begin(), end(), a, key_compare());
        if (hit != end() && *hit == a) {
            std().erase(hit);
            return true;
        }
        return false;
    }

    /**
     * Removes all elements \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns the number of
     * items removed. Client data may be passed through parameter \a d.
     */
    size_type removeAll(bool(*fn)(const_reference, void*), void* d) {
        size_type size = entries();
        iterator iter = std::remove_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        std().erase(iter, end());
        return size - entries();
    }

    /**
     * Removes all elements \c t in self such that the expression <tt>(t
     * == \a a)</tt> is \c true. Returns the number of items removed.
     */
    size_type removeAll(const_reference a);

    /**
     * Removes and returns the item at position \a i in self.
     *
     * @throw RWBoundsErr Thrown if this position is not between
     * \c 0 and one less than the number of entries in the collection.
     */
    value_type removeAt(size_type i);

    /**
     * Removes and returns the first item in the collection.
     *
     * Calling this function on an empty collection results in
     * undefined behavior.
     */
    value_type removeFirst() {
        RW_PRECONDITION(!isEmpty());
        const value_type ret = first();
        std().erase(begin());
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

    size_type insertAndLocate(const_reference a) {
        // Cast the result of subtracting these two indices to a size_type
        // this should be safe as begin will always be less than or
        // equal to the value of insertSorted
        iterator position = insertSorted(begin(), a);
        return static_cast<size_type>(position - begin());
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
     * Invokes the function pointer \a fn on each item in the collection.
     * Client data may be passed through parameter \a d.
     */
    void apply(void (*fn)(const_reference, void*), void* d) const {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
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
     * Inserts all elements of \a coll into self, preserving sorted order.
     */
    size_type merge(const RWTValSortedVector<T, C, A>& coll);

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
     * @deprecated As of SourcePro 13, use entries() instead.
     */
    RW_DEPRECATE_FUNC("Use RWTValSortedVector::entries() instead")
    size_type length() const {
        return std().size();
    }
#endif

    /**
     * Modifies, if necessary, the capacity of the vector to be at least
     * as large as \a n.
     */
    void resize(size_t n) {
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
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWTValSortedVector<T, C, A>& rhs) {
        std().swap(rhs.impl_);
    }

    /**
     * Constructs an empty vector.
     */
    RWTValSortedVector() { }

    /**
     * Copy constructor.
     */
    RWTValSortedVector(const RWTValSortedVector<T, C, A>& t)
        : impl_(t.impl_) { }

    /**
     * Constructs a vector by copying and sorting all elements of
     * \a vec.
     */
    RWTValSortedVector(const container_type& vec) : impl_(vec) {
        std::sort(begin(), end(), key_compare());
    }

    /**
     * Constructs a vector with \a n elements, each initialized to
     * \a val.
     */
    RWTValSortedVector(size_type n, const_reference val) : impl_(n, val) { }

    /**
     * Constructs a vector by copying elements from the range
     * [\a first, \a last).
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
    template <typename InputIterator>
#if !defined(DOXYGEN)
    RWTValSortedVector(InputIterator first, InputIterator last,
                       typename RWTEnableIf < !RWTIsIntegral<InputIterator>::value >::type** = 0)
#else
    RWTValSortedVector(InputIterator first, InputIterator last)
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

    // For backward compatibility (not particularly efficient):
    /**
     * Constructs an empty vector with a capacity of \a n elements.
     */
    RWTValSortedVector(size_type n) {
        resize(n);
    }

    // ~RWTValSortedVector(); // handled by auto-generated destructor

    /**
     * Assignment operator.
     */
    RWTValSortedVector&
    operator=(const RWTValSortedVector& rhs) {
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
     * Assignment operator. Copies and sorts elements from the data in \a rhs.
     */
    RWTValSortedVector&
    operator=(const container_type& rhs) {
        if (&impl_ != &rhs) {
            impl_ = rhs;
            std::sort(begin(), end(), key_compare());
        }
        return *this;
    }

#  if !defined(RW_NO_RVALUE_REFERENCES)

    /**
     * Move constructor. The constructed instance takes ownership of the
     * data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValSortedVector(RWTValSortedVector && rhs)
#    if !defined(RW_NO_STDVECTOR_RVALUE_SUPPORT)
        : impl_(rw_move(rhs.impl_)) { }
#    else
    {
        rhs.std().swap(impl_);
    }
#    endif

    /**
     * Move constructor. The constructed instance sorts and takes ownership
     * of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValSortedVector(container_type && rhs) {
#    if !defined(RW_NO_STDVECTOR_RVALUE_SUPPORT)
        impl_ = rw_move(rhs);
#    else
        rhs.swap(impl_);
#    endif
        std::sort(begin(), end(), key_compare());
    }

    /**
     * Move assignment. Self takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValSortedVector&
    operator=(RWTValSortedVector && rhs) {
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
    RWTValSortedVector&
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

private:
    void boundsCheck(size_type i) const {
        if (i >= entries()) {
            RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEXERR, i, entries())));
        }
    }

    iterator insertSorted(iterator, const_reference a) {
        iterator hit = std::upper_bound(begin(), end(), a, key_compare());
        return std().insert(hit, a);
    }


#  if !defined(RW_NO_RVALUE_REFERENCES)
    iterator insertSorted(iterator, value_type && a) {
        iterator hit = std::upper_bound(begin(), end(), a, key_compare());
        return std().insert(hit, rw_move(a));
    }
#  endif // !RW_NO_RVALUE_REFERENCES

    container_type impl_;
};

/**
 * @ingroup stl_based_collection_classes
 *
 * @brief Maintains a sorted collection of boolean values implemented as
 * a vector.
 *
 * This class maintains an always-sorted collection of boolean values,
 * implemented as a vector.
 *
 * Class \c A is an allocator of objects of type \c bool.
 * For more information about using custom allocators, please
 * see the <i>Essential Tools Module User's Guide</i>.
 *
 * The specialization of RWTValSortedVector<bool,C,A> differs from the
 * primary template in that it provides a more efficient memory
 * representation of the boolean array and it does not provide a data()
 * member function.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvsrtvec.h>
 * RWTValSortedVector<bool,C,A> srtvec;
 * @endcode
 *
 * @section related Related Classes
 *
 * \link RWTValSortedDlist RWTValSortedDlist<T,C,A>\endlink is an alternative
 * always-sorted collection.
 * RWTValOrderedVector<bool,A> is an unsorted vector of boolean values.
 *
 * Class \link RWTValSortedVector RWTValSortedVector<T,C,A>\endlink provides
 * a generic always-sorted vector for non-boolean types.
 *
 * Class <b>std::vector<bool,A></b> is the C++ Standard Library collection
 * that serves as the underlying implementation for this class.
 *
 * @section persistence Persistence
 *
 * Isomorphic
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/tvsrtvec.h>
 *
 * int main ()
 * {
 *     RWTValSortedVector<bool> vec;
 *
 *     vec.insert(true);
 *     vec.insert(false);
 *     vec.insert(true);    // add a duplicate
 *
 *     for (size_t i = 0; i < vec.entries (); ++i)
 *         std::cout << vec[i] << "\n";
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Output:
 * @code
 *     false
 *     true
 *     true
 * @endcode
 */
template <class C, class A>
class RWTValSortedVector<bool, C, A>
{

public:

    /**
     * A type representing the underlying implementation container.
     */
    typedef std::vector<bool, A> container_type;

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
     * A type representing the comparison function.
     */
    typedef C key_compare;

    /**
     * A type representing the comparison function.
     */
    typedef C value_compare;

    /**
     * @internal
     * A type representing the same type as self.
     */
    typedef RWTValSortedVector<bool, C, A> this_type;


    /**
     * Returns a reference to the underlying C++ Standard Library collection
     * that serves as the implementation for self. It is the application's
     * responsibility to not violate the ordering of the elements within
     * the collection.
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
     * Clears the collection by removing all items from self. Each
     * item has its destructor called.
     */
    void clear() {
        std().erase(std().begin(), std().end());
    }

    /**
     * Returns \c true if there exists an element \c t in self such
     * that the expression <tt>((*\a fn)(t,\a d))</tt> is \c true; otherwise
     * returns \c false. Client data may be passed through parameter \a d.
     */
    bool contains(bool(*fn)(const_reference, void*), void* d) const {
        return std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d)) != end();
    }

    /**
     * Returns \c true if there exists an element \c t in self such
     * that the expression <tt>(t == \a a)</tt> is \c true; otherwise returns
     * \c false.
     */
    bool contains(const_reference a) const {
        const_iterator hit = std::lower_bound(begin(), end(), a, key_compare());
        if (hit == end()) {
            return false;
        }
        return (*hit == a);
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
     * If there exists an element \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, assigns \c t to \a k and
     * returns \c true. Otherwise, returns \c false and leaves the value
     * of \a k unchanged. Client data may be passed through parameter \a d.
     */
    bool find(bool(*fn)(const_reference, void*), void* d, value_type& k) const {
        const_iterator ret = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        if (ret != end()) {
            k = *ret;
            return true;
        }
        return false;
    }

    /**
     * If there exists an element \c t in self such that the expression
     * <tt>(t == \a a)</tt> is \c true, assigns \c t to \a k and returns
     * \c true. Otherwise, returns \c false and leaves the value of \a k
     * unchanged.
     */
    bool find(const_reference a, value_type& k) const {
        const_iterator hit = std::lower_bound(begin(), end(), a, key_compare());
        if (hit != end() && *hit == a) {
            k = *hit;
            return true;
        }
        return false;
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
     * <tt>((*\a fn)(t,\a d))</tt> is \c true, or returns #RW_NPOS if
     * no such item exists. Client data may be passed through parameter
     * \a d.
     */
    size_type index(bool(*fn)(const_reference, void*), void* d) const {
        const_iterator res = std::find_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        return res != end() ? res - begin() : RW_NPOS;
    }

    /**
     * Returns the position of the first item \c t in self such that
     * <tt>(t == \a a)</tt>, or returns #RW_NPOS if no such item exists.
     */
    size_type index(const_reference a) const {
        const_iterator it = std::lower_bound(begin(), end(), a, key_compare());
        if (it == end() || !(*it == a)) {
            return RW_NPOS;
        }
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            it - begin();
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Adds the item \a a to self. The collection remains sorted.
     * Returns \c true.
     */
    bool insert(const_reference a) {
        insertSorted(begin(), a);  // only fail thru exception
        return true;
    }

    /**
     * Inserts \a val into self. Returns an iterator to the new item.
     *
     * The \a hint parameter indicates where in the collection the new value
     * should be inserted. This value is only a hint, and will be ignored if
     * the value should be inserted at a different location according to the
     * containers comparison object.
     */
    iterator insert(iterator hint, const_reference val) {
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
        rw_bitvec_sort(impl_, key_compare());
    }

    /**
     * Inserts all elements of \a a into self. The collection remains
     * sorted. Returns the number of items inserted.
     */
    size_type insert(const container_type& a);

    /**
     * Returns \c true if there are no items in the collection,
     * otherwise \c false.
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
    size_type occurrencesOf(bool(*fn)(const_reference, void*), void* d) const {
        typename rw_iterator_traits<const_iterator>::difference_type ret =
            rw_count_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        RW_ASSERT(ret >= 0);
        return static_cast<size_type>(ret);
    }

    /**
     * Returns the number of elements \c t in self such that the expression
     * <tt>(t == \a a)</tt> is \c true.
     */
    size_type occurrencesOf(const_reference a) const {
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
     * Index \a i should be between \c 0 and one less than the number
     * of entries; otherwise, the results are undefined.
     * @note
     * No bounds checking is performed.
     */
    reference operator()(size_type i) {
        RW_PRECONDITION(i < entries());
        return std()[i];
    }

    // see doxygen bug #612458
    /**
     * Returns a reference to the \a i <sup>th</sup> element of self.
     * Index \a i should be between \c 0 and one less than the number
     * of entries; otherwise, the results are undefined.
     * @note
     * No bounds checking is performed.
     */
    const_reference operator()(size_type i) const {
        RW_PRECONDITION(i < entries());
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
     * Removes the first element \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true and returns \c true. Returns
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
     * Removes the first element \c t in self such that the expression
     * <tt>(t == \a a)</tt> is \c true and returns \c true. Returns
     * \c false if there is no such element.
     */
    bool remove(const_reference a) {
        iterator hit = std::lower_bound(begin(), end(), a, key_compare());
        if (hit != end() && *hit == a) {
            std().erase(hit);
            return true;
        }
        return false;
    }

    /**
     * Removes all elements \c t in self such that the expression
     * <tt>((*\a fn)(t,\a d))</tt> is \c true. Returns the number of
     * items removed. Client data may be passed through parameter \a d.
     */
    size_type removeAll(bool(*fn)(const_reference, void*), void* d) {
        size_type size = entries();
        iterator iter = std::remove_if(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
        std().erase(iter, end());
        return size - entries();
    }

    /**
     * Removes all elements \c t in self such that the expression <tt>(t
     * == \a a)</tt> is \c true. Returns the number of items removed.
     */
    size_type removeAll(const_reference a);

    /**
     * Removes and returns the item at position \a i in self.
     *
     * @throw RWBoundsErr Thrown if this position is not between
     * \c 0 and one less than the number of entries in the collection.
     */
    value_type removeAt(size_type i);

    /**
     * Removes and returns the first item in the collection.
     *
     * Calling this function on an empty collection results in
     * undefined behavior.
     */
    value_type removeFirst() {
        RW_PRECONDITION(!isEmpty());
        const value_type ret = first();
        std().erase(begin());
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

    /**
     * Returns a reference to the \a i <sup>th</sup> element of self.
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

    size_type insertAndLocate(const_reference a) {
        // Cast the result of subtracting these two indices to a size_type
        // this should be safe as begin will always be less than or
        // equal to the value of insertSorted
        iterator position = insertSorted(begin(), a);
        return static_cast<size_type>(position - begin());
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
     * Invokes the function pointer \a fn on each item in the collection.
     * Client data may be passed through parameter \a d.
     */
    void apply(void (*fn)(const_reference, void*), void* d) const {
        std::for_each(begin(), end(), rw_bind2nd(rw_ptr_fun(fn), d));
    }

    /**
     * Inserts all elements of \a coll into self, preserving sorted order.
     */
    size_type merge(const RWTValSortedVector<bool, C, A>& coll);

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
     * @deprecated As of SourcePro 13, use entries() instead.
     */
    RW_DEPRECATE_FUNC("Use RWTValSortedVector::entries() instead") size_type length() const {
        return std().size();
    }
#endif

    /**
     * Modifies, if necessary, the capacity of the vector to be at least
     * as large as \a n.
     */
    void resize(size_t n) {
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
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWTValSortedVector<bool, C, A>& rhs) {
        std().swap(rhs.impl_);
    }

    /**
     * Constructs an empty vector.
     */
    RWTValSortedVector() { }

    /**
     * Copy constructor.
     */
    RWTValSortedVector(const RWTValSortedVector<bool, C, A>& t)
        : impl_(t.impl_) { }

    /**
     * Constructs a vector by copying and sorting all elements of
     * \a vec.
     */
    RWTValSortedVector(const container_type& vec) : impl_(vec) {
        rw_bitvec_sort(impl_, key_compare());
    }

    /**
     * Constructs a vector with \a n elements, each initialized to
     * \a val.
     */
    RWTValSortedVector(size_type n, const bool& val) : impl_(n, val) { }

    /**
     * Constructs a vector by copying and sorting elements from the
     * array of <tt>T</tt>s pointed to by \a first, up to, but not including,
     * the element pointed to by \a last.
     */
    RWTValSortedVector(const bool* first, const bool* last): impl_(first, last) {
        rw_bitvec_sort(impl_, key_compare());
    }

    /**
     * Constructs a vector by copying elements from the range
     * [\a first, \a last).
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
    template <typename InputIterator>
#if !defined(DOXYGEN)
    RWTValSortedVector(InputIterator first, InputIterator last,
                       typename RWTEnableIf < !RWTIsIntegral<InputIterator>::value >::type** = 0)
#else
    RWTValSortedVector(InputIterator first, InputIterator last)
#endif
#if !defined(RW_BROKEN_STD_VECTOR_CTOR_RANGE)
        : impl_(first, last) {
        rw_bitvec_sort(impl_, key_compare());
    }
#else
        :
        impl_() {
        insert(first, last);
    }
#endif


    // For backward compatibility (not particularly efficient):
    /**
     * Constructs an empty vector with a capacity of \a n elements.
     */
    RWTValSortedVector(size_type n) {
        resize(n);
    }

    // ~RWTValSortedVector(); // handled by auto-generated destructor

    /**
     * Assignment operator.
     */
    RWTValSortedVector&
    operator=(const RWTValSortedVector& rhs) {
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
     * Assignment operator. Copies and sorts elements from the data in \a x.
     */
    RWTValSortedVector&
    operator=(const container_type& x) {
        if (&impl_ != &x) {
            impl_ = x;
            rw_bitvec_sort(impl_, key_compare());
        }
        return *this;
    }

#  if !defined(RW_NO_RVALUE_REFERENCES)

    /**
     * Move constructor. The constructed instance takes ownership of the
     * data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValSortedVector(RWTValSortedVector && rhs)
#    if !defined(RW_NO_STDVECTOR_RVALUE_SUPPORT)
        : impl_(rw_move(rhs.impl_)) { }
#    else
    {
        rhs.std().swap(impl_);
    }
#    endif

    /**
     * Move constructor. The constructed instance sorts and takes ownership
     * of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValSortedVector(container_type && rhs) {
#    if !defined(RW_NO_STDVECTOR_RVALUE_SUPPORT)
        impl_ = rw_move(rhs);
#    else
        rhs.swap(impl_);
#    endif
        rw_bitvec_sort(impl_, key_compare());
    }

    /**
     * Move assignment. Self takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTValSortedVector&
    operator=(RWTValSortedVector && rhs) {
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
    RWTValSortedVector&
    operator=(container_type && rhs) {
        if (&impl_ != &rhs) {
#    if !defined(RW_NO_STDVECTOR_RVALUE_SUPPORT)
            impl_ = rw_move(rhs);
#    else
            rhs.swap(impl_);
#    endif
            rw_bitvec_sort(impl_, key_compare());
        }
        return *this;
    }
#  endif // !RW_NO_RVALUE_REFERENCES

private:
    void boundsCheck(size_type i) const {
        if (i >= entries()) {
            RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEXERR, i, entries())));
        }
    }

    iterator insertSorted(iterator, const_reference a) {
#if defined(RW_STDLIB_VER) && (RW_STDLIB_VER == 0x02010100)
        iterator hit = std::upper_bound(std().begin(), std().end(), a, key_compare());
        size_type i = rw_distance(std().begin(), hit);
        std().resize(std().size() + 1, a);
        iterator iter = end();
        --iter;
        iterator beg = begin();
        std::advance(beg, i);
        while (iter != beg) {
            iterator prev = iter--;
            value_type tmp = *prev;
            *prev = *iter;
            *iter = tmp;
        }
        return beg;
#else
        iterator hit = std::upper_bound(std().begin(), std().end(), a, key_compare());
        return std().insert(hit, a);
#endif
    }

    container_type impl_;
};

/**
 * @relates RWTValSortedVector
 *
 * Returns \c true if \a lhs and \a rhs are equal, otherwise \c false. Two
 * collections are equal if both have the same number of entries, and
 * iterating through both collections produces individual elements that,
 * in turn, compare equal to each other.
 */
template <class T, class C, class A>
bool operator==(const RWTValSortedVector<T, C, A>& lhs, const RWTValSortedVector<T, C, A>& rhs)
{
    return lhs.std() == rhs.std();
}

/**
 * @relates RWTValSortedVector
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T, class C, class A>
bool operator!=(const RWTValSortedVector<T, C, A>& lhs, const RWTValSortedVector<T, C, A>& rhs)
{
    return lhs.std() != rhs.std();
}

/**
 * @relates RWTValSortedVector
 *
 * Returns \c true if \a lhs is lexicographically less than \a rhs,
 * otherwise \c false. Assumes that type \c T has well-defined less-than
 * semantics.
 */
template <class T, class C, class A>
bool operator<(const RWTValSortedVector<T, C, A>& lhs, const RWTValSortedVector<T, C, A>& rhs)
{
    return lhs.std() < rhs.std();
}

/**
 * @relates RWTValSortedVector
 *
 * Equivalent to <tt>(\a rhs < \a lhs)</tt>.
 */
template <class T, class C, class A>
bool operator>(const RWTValSortedVector<T, C, A>& lhs, const RWTValSortedVector<T, C, A>& rhs)
{
    return lhs.std() > rhs.std();
}

/**
 * @relates RWTValSortedVector
 *
 * Equivalent to <tt>!(\a rhs < \a lhs)</tt>.
 */
template <class T, class C, class A>
bool operator<=(const RWTValSortedVector<T, C, A>& lhs, const RWTValSortedVector<T, C, A>& rhs)
{
    return lhs.std() <= rhs.std();
}

/**
 * @relates RWTValSortedVector
 *
 * Equivalent to <tt>!(\a lhs < \a rhs)</tt>.
 */
template <class T, class C, class A>
bool operator>=(const RWTValSortedVector<T, C, A>& lhs, const RWTValSortedVector<T, C, A>& rhs)
{
    return lhs.std() >= rhs.std();
}

/**
 * @relates RWTValSortedVector
 *
 * @copydoc operator==(const RWTValSortedVector&, const RWTValSortedVector&);
 */
template <class T, class C, class A>
bool operator==(const RWTValSortedVector<T, C, A>& lhs,
                const typename RWTValSortedVector<T, C, A>::container_type& rhs)
{
    return lhs.std() == rhs;
}

/**
 * @relates RWTValSortedVector
 *
 * @copydoc operator!=(const RWTValSortedVector&, const RWTValSortedVector&);
 */
template <class T, class C, class A>
bool operator!=(const RWTValSortedVector<T, C, A>& lhs,
                const typename RWTValSortedVector<T, C, A>::container_type& rhs)
{
    return lhs.std() != rhs;
}

/**
 * @relates RWTValSortedVector
 *
 * @copydoc operator<(const RWTValSortedVector&, const RWTValSortedVector&);
 */
template <class T, class C, class A>
bool operator<(const RWTValSortedVector<T, C, A>& lhs,
               const typename RWTValSortedVector<T, C, A>::container_type& rhs)
{
    return lhs.std() < rhs;
}

/**
 * @relates RWTValSortedVector
 *
 * @copydoc operator>(const RWTValSortedVector&, const RWTValSortedVector&);
 */
template <class T, class C, class A>
bool operator>(const RWTValSortedVector<T, C, A>& lhs,
               const typename RWTValSortedVector<T, C, A>::container_type& rhs)
{
    return lhs.std() > rhs;
}

/**
 * @relates RWTValSortedVector
 *
 * @copydoc operator<=(const RWTValSortedVector&, const RWTValSortedVector&);
 */
template <class T, class C, class A>
bool operator<=(const RWTValSortedVector<T, C, A>& lhs,
                const typename RWTValSortedVector<T, C, A>::container_type& rhs)
{
    return lhs.std() <= rhs;
}

/**
 * @relates RWTValSortedVector
 *
 * @copydoc operator>=(const RWTValSortedVector&, const RWTValSortedVector&);
 */
template <class T, class C, class A>
bool operator>=(const RWTValSortedVector<T, C, A>& lhs,
                const typename RWTValSortedVector<T, C, A>::container_type& rhs)
{
    return lhs.std() >= rhs;
}


/**
 * @relates RWTValSortedVector
 *
 * @copydoc operator==(const RWTValSortedVector&, const RWTValSortedVector&);
 */
template <class T, class C, class A>
bool operator==(const typename RWTValSortedVector<T, C, A>::container_type& lhs,
                const RWTValSortedVector<T, C, A>& rhs)
{
    return lhs == rhs.std();
}

/**
 * @relates RWTValSortedVector
 *
 * @copydoc operator!=(const RWTValSortedVector&, const RWTValSortedVector&);
 */
template <class T, class C, class A>
bool operator!=(const typename RWTValSortedVector<T, C, A>::container_type& lhs,
                const RWTValSortedVector<T, C, A>& rhs)
{
    return lhs != rhs.std();
}

/**
 * @relates RWTValSortedVector
 *
 * @copydoc operator<(const RWTValSortedVector&, const RWTValSortedVector&);
 */
template <class T, class C, class A>
bool operator<(const typename RWTValSortedVector<T, C, A>::container_type& lhs,
               const RWTValSortedVector<T, C, A>& rhs)
{
    return lhs < rhs.std();
}

/**
 * @relates RWTValSortedVector
 *
 * @copydoc operator>(const RWTValSortedVector&, const RWTValSortedVector&);
 */
template <class T, class C, class A>
bool operator>(const typename RWTValSortedVector<T, C, A>::container_type& lhs,
               const RWTValSortedVector<T, C, A>& rhs)
{
    return lhs > rhs.std();
}

/**
 * @relates RWTValSortedVector
 *
 * @copydoc operator<=(const RWTValSortedVector&, const RWTValSortedVector&);
 */
template <class T, class C, class A>
bool operator<=(const typename RWTValSortedVector<T, C, A>::container_type& lhs,
                const RWTValSortedVector<T, C, A>& rhs)
{
    return lhs <= rhs.std();
}

/**
 * @relates RWTValSortedVector
 *
 * @copydoc operator>=(const RWTValSortedVector&, const RWTValSortedVector&);
 */
template <class T, class C, class A>
bool operator>=(const typename RWTValSortedVector<T, C, A>::container_type& lhs,
                const RWTValSortedVector<T, C, A>& rhs)
{
    return lhs >= rhs.std();
}

/**
 * @relates RWTValSortedVector
 *
 * Saves the collection \a coll onto the output stream \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T, class C , class A>
RWvostream& operator<<(RWvostream& strm, const RWTValSortedVector<T, C , A>& coll);

/**
 * @relates RWTValSortedVector
 *
 * Saves the collection \a coll onto the RWFile \a strm,
 * or a reference to it if it has already been saved.
 */
template <class T, class C , class A>
RWFile& operator<<(RWFile& strm, const RWTValSortedVector<T, C , A>& coll);

/**
 * @relates RWTValSortedVector
 *
 * Restores the contents of the collection \a coll from the input
 * stream \a strm.
 */
template <class T, class C , class A>
RWvistream& operator>>(RWvistream& strm, RWTValSortedVector<T, C , A>& coll);

/**
 * @relates RWTValSortedVector
 *
 * Restores the contents of the collection \a coll from the RWFile
 * \a strm.
 */
template <class T, class C , class A>
RWFile& operator>>(RWFile& strm, RWTValSortedVector<T, C , A>& coll);

/**
 * @relates RWTValSortedVector
 *
 * Looks at the next object on the input stream \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T, class C , class A>
RWvistream& operator>>(RWvistream& strm, RWTValSortedVector<T, C , A>*& p);

/**
 * @relates RWTValSortedVector
 *
 * Looks at the next object on the RWFile \a strm and either
 * creates a new collection off the heap and sets \a p to point
 * to it, or sets \a p to point to a previously read instance.
 * If a collection is created off the heap, then the application is
 * responsible for deleting it.
 */
template <class T, class C , class A>
RWFile& operator>>(RWFile& strm, RWTValSortedVector<T, C , A>*& p);

#  if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#    pragma implementation ("tvsrtvec.cc")
#  endif

#  ifdef RW_NO_IMPLICIT_INCLUSION
#    include "rw/tvsrtvec.cc"
#  endif




#endif /* RW_CLASSIC_TEMPLATE_CONTAINERS */

#endif /* RW_TOOLS_TVSRTVEC_H */
