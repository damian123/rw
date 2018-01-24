#ifndef RW_TOOLS_STDEX_SLIST_H__
#define RW_TOOLS_STDEX_SLIST_H__



/**********************************************************************
 *
 * STL Sequence-compliant singly-linked list
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/stdex/slist.h#3 $
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

#if defined(_WIN32)
#  pragma warning (disable: 4284)
#endif

#include <rw/defs.h>
#include <rw/edefs.h> // for rw_move
#include <rw/tools/traits/RWTEnableIf.h>
#include <rw/tools/traits/RWTIsIntegral.h>

#include <memory>
#include <iterator>
#include <algorithm> // equal filter

/**
 * @ingroup stl_extension_based_collection_classes
 *
 * @brief Maintains a collection of some type A implemented as a
 * singly-linked list.
 *
 * Class \link rw_slist rw_slist<T,A> \endlink maintains a collection of \c T,
 * implemented as a singly-linked list. Since this is a \e value-based
 * list, objects are \e copied into and out of the links that make
 * up the list. As with all classes that meet the ANSI \e sequence
 * specification, \link rw_slist rw_slist<T,A> \endlink provides for iterators that reference
 * its elements. Operations that alter the contents of \link rw_slist rw_slist<T,A> \endlink
 * invalidate iterators that reference items at or after the
 * location of change.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/stdex/slist.h>
 * rw_slist<T,A=std::allocator<T> >;
 * @endcode
 */
template <class T, class A = std::allocator<T> >
class rw_slist
{

    // internal class predeclarations
protected:
    struct Node;

public:
    class Iterator;
    class ConstIterator;

    friend class Iterator;
    friend class ConstIterator;

protected:

    struct Node {
        Node* next;
        T    item_;

        Node(const T& t, Node* nx)
            : next(nx),
              item_(t) {
        }

#if !defined(RW_NO_RVALUE_REFERENCES)
        Node(T && t, Node* nx)
            : next(nx),
              item_(rw_move(t)) {
        }
#endif // !RW_NO_RVALUE_REFERENCES

        Node** mynext() {
            return &next;
        }

        T& item() {
            return item_;
        }

        const T&  item() const {
            return item_;
        }

    };

public:

    /**
     * A type representing the container's data type.
     */
    typedef T             value_type;

    /**
     * A type that provides a reference to an element in the container.
     */
    typedef T&            reference;

    /**
     * A type that provides a \c const reference to an element in the
     * container.
     */
    typedef const T&      const_reference;

    /**
     * A type that provides a pointer to an element in the container.
     */
    typedef T* pointer;

    /**
     * A type that provides a \c const pointer to an element in the
     * container.
     */
    typedef const T* const_pointer;

    /**
     * A type representing the allocator type for the container.
     */
#ifdef RW_ALLOC_INTERFACE_STLV2X_HACK
    typedef std::allocator_interface < std::allocator< Node >, Node >
    allocator_type;
#else

#  if !defined(RW_ALLOCATOR_NO_REBIND)
    typedef typename A::template rebind<Node>::other allocator_type;
#  else
    typedef std::allocator<Node> allocator_type;
#  endif //  RW_ALLOCATOR_NO_REBIND

#endif  // RW_ALLOC_INTERFACE_STLV2X_HACK

    /**
     * A signed integral type used to indicate the distance between two
     * valid iterators on the same container.
     */
    typedef typename allocator_type::difference_type difference_type;

    /**
     * An unsigned integral type used for counting the number of elements
     * in the container.
     */
    typedef typename allocator_type::size_type       size_type;


    class Iterator : public

#ifndef RW_ITER_NO_TAGS
        std::iterator <
        std::forward_iterator_tag,
        T,
        typename allocator_type::difference_type,
        T*, T& >
#else
        std::forward_iterator <
        T,
        typename allocator_type::difference_type >
#endif
    {
        friend class ConstIterator;

    public:
        Iterator(const Iterator& i)
            : prelink_(i.prelink_) {
        }

        Iterator&  operator=(const Iterator& i) {
            prelink_ = i.prelink_;
            return *this;
        }

        bool  operator==(const Iterator& i) const {
            return *prelink_ == *i.prelink_;
        }

        bool operator!=(const Iterator& i) const {
            return *prelink_ != *i.prelink_;
        }

        T&  operator*() const {
            return (*prelink_)->item();
        }

        T*  operator->() const {
            return &(*prelink_)->item();
        }

        Iterator&  operator++() {
            prelink_ = (*prelink_)->mynext();
            return *this;
        }

        Iterator  operator++(int) {
            Node** tmp = prelink_;
            ++(*this);
            return Iterator(tmp);
        }

        bool  isValid() const {
            return prelink_ != 0 && *prelink_ != 0;
        }

        Iterator() : prelink_(0) {
        }

        Iterator(Node** spot) : prelink_(spot) {
        }

        Node**  prelink() const {
            return prelink_;
        }

        Node**&  prelink() {
            return RW_EXPOSE(prelink_);
        }

        bool   reset() {
            bool ret = isValid();
            prelink_ = 0;
            return ret;
        }

    private:
        Node**      prelink_;
    };


    class ConstIterator : public
#ifndef RW_ITER_NO_TAGS
        std::iterator <
        std::forward_iterator_tag,
        const T,
        typename allocator_type::difference_type,
        const T*, const T& >
#else
        std::forward_iterator <
        const T,
        typename allocator_type::difference_type >
#endif
    {
        friend class Iterator;

    public:
        ConstIterator(const ConstIterator& i)
            : prelink_(i.prelink_) {
        }

        ConstIterator(const Iterator& i) : prelink_(i.prelink_) {
        }

        ConstIterator& operator=(const ConstIterator& i) {
            prelink_ = i.prelink_;
            return *this;
        }
        ConstIterator& operator=(const Iterator& i) {
            prelink_ = i.prelink_;
            return *this;
        }

        bool operator==(const ConstIterator& i) const {
            return *prelink_ == *i.prelink_;
        }
        bool operator!=(const ConstIterator& i) const {
            return *prelink_ != *i.prelink_;
        }

        const T& operator*() const {
            return (*prelink_)->item();
        }
        const T* operator->() const {
            return &(*prelink_)->item();
        }

        ConstIterator& operator++() {
            prelink_ = (*prelink_)->mynext();
            return *this;
        }
        ConstIterator operator++(int) {
            Node** tmp = prelink_;
            ++(*this);
            return ConstIterator(tmp);
        }

        operator bool() const {
            return isValid();
        }

        bool isValid() const {
            return prelink_ != 0 && *prelink_ != 0;
        }

        ConstIterator() : prelink_(0) {
        }

        ConstIterator(Node** spot) : prelink_(spot) {
        }

        Node** prelink() const {
            return prelink_;
        }

        Node**& prelink() {
            return RW_EXPOSE(prelink_);
        }

        bool reset() {
            bool ret = isValid();
            prelink_ = 0;
            return ret;
        }

    private:
        Node** prelink_;

    };

    /**
     * A type that provides a forward iterator over the elements
     * in the container.
     */
    typedef Iterator iterator;

    /**
     * A type that provides a \c const forward iterator over the
     * elements in the container.
     */
    typedef ConstIterator const_iterator;

    /**
     * Constructs an empty \link rw_slist rw_slist<T,A> \endlink.
     */
    rw_slist();

    /**
     * Constructs an \link rw_slist rw_slist<T,A> \endlink that is a copy of \a list. Each
     * element from \a list is copied into self.
     */
    rw_slist(const rw_slist<T, A>& list);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed list takes ownership of the
     * data owned by \a list.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    rw_slist(rw_slist<T, A> && list);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructs an \link rw_slist rw_slist<T,A> \endlink containing a copy
     * of each element in the range [\a first, \a last).
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     */
    template <class InputIterator>
#if !defined(DOXYGEN)
    rw_slist(InputIterator first, InputIterator last,
             typename RWTEnableIf < !RWTIsIntegral<InputIterator>::value >::type** = 0)
#else
    rw_slist(InputIterator first, InputIterator last)
#endif
        : toHead_(0), head_(&toHead_), tail_(&toHead_), entries_(0) {
        insert(begin(), first, last);
    }

    /**
     * The destructor releases the memory used by the links.
     */
    ~rw_slist();

    /**
     * Copy assignment. Constructs an \link rw_slist rw_slist<T,A> \endlink containing a
     * copy of each element in \a list.
     */
    rw_slist<T, A>&
    operator=(const rw_slist<T, A>& list);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a list.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    rw_slist<T, A>&
    operator=(rw_slist<T, A> && list);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Copies each element in the range [\a first, \a last) into self,
     * replacing any existing items.
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     *
     * @note
     * \a first and \a last must not be iterators into self.
     */
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last);

    /**
     * Exchanges the contents of self with \a other, retaining the ordering
     * of each. This method does not copy or destroy any of the
     * items exchanged, but re-links the lists.
     */
    void swap(rw_slist<T, A>& other);

    /**
     * Returns an iterator referring to the first element in the container.
     *
     * If the container is empty, returns end().
     */
    iterator begin() {
        return iterator(head_);
    }

    /**
     * @copydoc begin()
     */
    const_iterator begin() const {
        return const_iterator(head_);
    }

    /**
     * @copydoc begin()
     */
    const_iterator cbegin() const {
        return const_iterator(head_);
    }

    /**
     * Returns an iterator referring to the element after the last element
     * in the container.
     *
     * Dereferencing the iterator returned by this function results in
     * undefined behavior.
     */
    iterator end() {
        return iterator(tail_);
    }

    /**
     * @copydoc end()
     */
    const_iterator end() const {
        return const_iterator(tail_);
    }

    /**
     * @copydoc end()
     */
    const_iterator cend() const {
        return const_iterator(tail_);
    }

    /**
     * Returns \c true if self is empty.
     */
    bool empty() const {
        return 0 == entries_;
    }

    /**
     * Returns the number of items currently held in self.
     */
    size_type size() const {
        return entries_;
    }

    // use todd\'s templatized stuff?
    // for the moment leave it undefined
    // or use Alex's Allocator::max_size() which doesn't seem right
    // since it limits us to one allocation buffer
    size_type max_size() const;

    /**
     * References the first item in the list as an L-value. If self
     * is empty, the behavior is undefined.
     */
    T& front() {
        return *(begin());
    }

    /**
     * @copydoc front()
     */
    const T& front() const {
        return *(begin());
    }

    RW_SUPPRESS_CAST_STRICTER_ALIGNMENT_WARNING
    T& back() {
        return ((Node*)tail_)->item();
    }
    const T& back() const {
        return ((Node*)tail_)->item();
    }
    RW_RESTORE_CAST_STRICTER_ALIGNMENT_WARNING

    /**
     * Inserts \a item as the first element of the list.
     */
    void push_front(const T& item) {
        insert(begin(), item);
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @copydoc push_front()
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    void push_front(T && item) {
        insert(begin(), rw_move(item));
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Inserts \a item as the last element of the list.
     */
    void push_back(const T& item) {
        insert(end(), item);
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @copydoc push_back()
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    void push_back(T && item) {
        insert(end(), rw_move(item));
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Inserts \a val just prior to the place referenced by \a loc.
     * Returns an iterator referencing the newly inserted element.
     *
     * @note
     * <tt>++(list.insert(\a loc,\a val)) == \a loc</tt>
     */
    iterator insert(iterator loc, const T& val);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @copydoc insert(iterator, const T&)
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    iterator insert(iterator loc, T && val);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Inserts a copy of each item in the range [\a first, \a last) into self
     * just prior to the place referenced by \a loc. Returns an iterator
     * referencing the first newly inserted element.
     *
     * \c InputIterator is an input iterator type that points to elements
     * that are convertible to #value_type objects.
     *
     * @note
     * \a first and \a last must not be iterators into self.
     */
    template <class InputIterator>
#if !defined(DOXYGEN)
    typename RWTEnableIf < !RWTIsIntegral<InputIterator>::value, iterator >::type
#else
    iterator
#endif
    insert(iterator loc, InputIterator first, InputIterator last) {
        if (first != last) {
            rw_slist<T, A> tmp;
            std::copy(first, last, std::back_inserter(tmp));
            splice(loc, tmp, tmp.begin(), tmp.end());
            return loc;
        }
        else {
            return end();
        }
    }

    // don't require def ctor:
    // void insert(iterator, size_type, const T& value = T());
    void insert(iterator, size_type, const T& value);

    rw_slist(size_type n, const T& value);

    /**
     * Erases the first element of self. If self is empty, the effect
     * is undefined.
     */
    void pop_front() {
        erase(begin());
    }

    /**
     * Removes from self the element referenced by \a iter. If \a iter
     * does not reference an actual item contained in self, the effect
     * is undefined. Returns an iterator referencing the location just
     * after the erased item.
     */
    iterator erase(const_iterator iter);

    /**
     * Removes from self the elements referenced by the range beginning
     * at \a first and bounded by \a bound. Returns an iterator referencing
     * a position just after the last erased item. If \a first does
     * not reference an item in self (and if \a first and \a bound are
     * not equal), the effect is undefined.
     */
    iterator erase(const_iterator first, const_iterator bound);

    void clear() {
        erase(begin(), end());
    }

    /**
     * Inserts the entire contents of \a donor into self, just before
     * the position referenced by \a to, leaving \a donor empty. This
     * method does not copy or destroy any of the items moved, but re-links
     * the list nodes from \a donor into self.
     */
    void splice(iterator to, rw_slist<T, A>& donor) {
        cutPaste(to, donor.size(), donor, donor.begin(), donor.end());
    }

    /**
     * Removes from \a donor and inserts into self, just before location
     * \a to, the item referenced by \a from. If \a from does not reference
     * an actual item contained in \a donor the effect is undefined.
     * This method does not copy or destroy the item referenced by
     * \a from, but re-links the node containing it from \a donor into self.
     */
    void splice(iterator to, rw_slist<T, A>& donor, iterator from);

    /**
     * Removes from \a donor and inserts into self just before location
     * \a to, the items referenced by the range beginning with \a from_start
     * and bounded by \a from_bound. If that range does not refer to
     * items contained by \a donor, the effect is undefined. This method
     * does not copy or destroy the items referenced by the range, but
     * re-links those list nodes from \a donor into self.
     */
    void splice(iterator to, rw_slist<T, A>& donor, iterator from_start,
                iterator from_bound);


    void remove(const T& val);

    /**
     * Removes from self all but the first element from each equal range.
     * A precondition is that any duplicate elements are adjacent.
     */
    void unique();

    /**
     * Assuming both \a donor and self are sorted, moves every item
     * from \a donor into self, leaving \a donor empty, and self sorted.
     * If either list is unsorted, the move will take place, but the
     * result may not be sorted. This method does not copy or destroy
     * the items in \a donor, but re-links list nodes into self.
     */
    void merge(rw_slist<T, A>& donor);

    /**
     * Reverses the order of the nodes containing the elements in self.
     */
    void reverse();

    /**
     * Sorts self according to <tt>T::operator<(T)</tt> or equivalent. Runs
     * in time proportional to \c N \c log(N), where \c N is the number
     * of elements. This method does not copy or destroy any of the
     * items exchanged during the sort, but adjusts the order of the
     * links in the list.
     */
    void sort();

    /* tempate<class Compare>
     * void sort(Comparison<T>&);
     */

    // RW internal debugging methods
#if 0

public:
    void showOff(const char* s = "LIST") const;
    void iprint(const char* p, const_iterator i, const char* t) const;

#endif

protected:
    iterator cutPaste(iterator, size_type, rw_slist<T, A>&, iterator, iterator);
    void sortPairs();
    iterator adjacentMerge(iterator, iterator, iterator, size_type, size_type);

protected:
    Node* toHead_;

private:
    Node** head_;
    Node** tail_;
    size_type entries_;

    allocator_type nodeAllocator;
};

/**
 * @relates rw_slist
 *
 * Returns true if \a lhs and \a rhs have the same number of elements
 * and each element of \a rhs tests equal (<tt>T::operator==()</tt> or
 * equivalent) to the corresponding element of \a lhs.
 */
template <class T, class A>
inline bool operator==(const rw_slist<T, A>& lhs, const rw_slist<T, A>& rhs)
{
    return lhs.size() == rhs.size() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/**
 * @relates rw_slist
 *
 * Equivalent to <tt>!(\a lhs == \a rhs)</tt>.
 */
template <class T, class A>
inline bool operator!=(const rw_slist<T, A>& lhs, const rw_slist<T, A>& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates rw_slist
 *
 * Returns the result of calling:
 *
 * @code
 * lexicographical_compare(lhs.begin(), lhs.end(),
 *                         rhs.begin(), rhs.end());
 * @endcode
 */
template <class T, class A>
inline bool
operator<(const rw_slist<T, A>& lhs, const rw_slist<T, A>& rhs)
{
    return std::lexicographical_compare(
               lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

/**
 * @relates rw_slist
 *
 * Equivalent to <tt>(\a rhs < \a lhs)</tt>.
 */
template <class T, class A>
inline bool operator>(const rw_slist<T, A>& lhs, const rw_slist<T, A>& rhs)
{
    return (rhs < lhs);
}

/**
 * @relates rw_slist
 *
 * Equivalent to <tt>!(\a rhs < \a lhs)</tt>.
 */
template <class T, class A>
inline bool operator<=(const rw_slist<T, A>& lhs, const rw_slist<T, A>& rhs)
{
    return !(rhs < lhs);
}

/**
 * @relates rw_slist
 *
 * Equivalent to <tt>!(\a lhs < \a rhs)</tt>.
 */
template <class T, class A>
inline bool operator>=(const rw_slist<T, A>& lhs, const rw_slist<T, A>& rhs)
{
    return !(lhs < rhs);
}


/*
 * This is (possibly) the second include of the same ".cc" file. It
 * may have been included from inside the class body above. Please see the
 * comments in the class body above and at the beginning of the ".cc" file
 * if you need to further understand what is accomplished with these macros.
 */

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("slist.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include <rw/stdex/slist.cc>
#endif // RW_NO_IMPLICIT_INCLUSION

#endif /* RW_TOOLS_STDEX_SLIST_H__ */
