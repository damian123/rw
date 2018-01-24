#ifndef RW_TOOLS_TQUEUE_H
#define RW_TOOLS_TQUEUE_H



/**********************************************************************
 *
 * Parameterized queue of T's, implemented using class C.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tqueue.h#1 $
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
#include <rw/edefs.h> // for rw_swap

/**
 * @ingroup traditional_collection_classes
 *
 * @brief A queue of templatized objects that supports
 * user-specified containers.
 *
 * Class RWTQueue represents a parameterized queue.  Not only can the
 * type of object inserted into the queue be parameterized, but
 * also the implementation.
 *
 * Parameter \c T represents the type of object in the queue, either
 * a class or built-in type.  The class \c T must have:
 *
 * -  well-defined copy semantics (<tt>T::T(const T&)</tt> or equivalent)
 * -  well-defined assignment semantics (<tt>T::operator=(const T&)</tt> or
 *    equivalent)
 * -  any other semantics required by class \c C.
 *
 * Parameter \c C represents the class used for implementation. The class
 * \c C must provide the following interface (or equivalent):
 *
 * -  <tt>void C::append(const T&)</tt>
 * -  <tt>void C::clear()</tt>
 * -  <tt>size_t C::entries() const</tt>
 * -  <tt>T C::first()</tt>
 * -  <tt>bool C::isEmpty() const</tt>
 * -  <tt>T C::last()</tt>
 * -  <tt>T C::removeFirst()</tt>
 * -  a default constructor (<tt>C::C()</tt>)
 * -  a copy constructor (<tt>C::C(const C&)</tt>)
 * -  an assignment operator (<tt>C& C::operator=(const C&)</tt>)
 *
 * These methods must behave in a manner that is consistent with that of
 * \link RWTValDlist RWTValDlist<T,A>\endlink for this class to function
 * as intended.
 *
 * Useful choices for \c C are \link RWTValSlist RWTValSlist<T,A>\endlink
 * or \link RWTValDlist RWTValDlist<T,A>\endlink. Vectors, such
 * as \link RWTValOrderedVector RWTValOrderedVector<T,A>\endlink, can
 * also be used, but tend to be less efficient at removing an object from
 * the front of the queue.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tqueue.h>
 * RWTQueue<T, C> queue;
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 *
 * #include <iostream>
 * #include <rw/tqueue.h>
 * #include <rw/cstring.h>
 * #include <rw/tvslist.h>
 *
 * int main()
 * {
 *     RWTQueue<RWCString, RWTValSlist<RWCString> > q;
 *
 *     q.insert("one"); // Type conversion occurs
 *     q.insert("two");
 *     q.insert("three");
 *
 *     while (!q.isEmpty()) {
 *         std::cout << q.get() << std::endl;
 *     }
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program output
 *
 * @code
 * one
 * two
 * three
 * @endcode
 *
 */
template <class T, class C> class  RWTQueue
{
public:

    /**
     * Constructs a queue.
     */
    RWTQueue()
        : container_() {
    }

    /**
     * Constructs a queue from the container \a rhs.
     */
    explicit RWTQueue(const C& rhs)
        : container_(rhs) {
    }

    /**
     * Copy constructor. Constructs self as a copy of \a rhs.
     */
    RWTQueue(const RWTQueue& rhs)
        : container_(rhs.container_) {
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. Takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    explicit RWTQueue(C && rhs)
        : container_(rw_move(rhs)) {
    }

    /**
     * @copydoc RWTQueue(C&&)
     */
    RWTQueue(RWTQueue && rhs)
        : container_(rw_move(rhs.container_)) {
    }
#endif

    /**
     * Assignment operator. Sets self to a copy of \a rhs.
     */
    RWTQueue& operator=(const RWTQueue& rhs) {
        if (&rhs != this) {
            container_ = rhs.container_;
        }
        return *this;
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTQueue& operator=(RWTQueue && rhs) {
        if (&rhs != this) {
            container_ = rw_move(rhs.container_);
        }
        return *this;
    }
#endif

    /**
     * Calls <tt>C::clear()</tt> on the underlying container.
     */
    void clear() {
        container_.clear();
    }

    /**
     * Returns the result of calling <tt>C::entries()</tt> on the underlying container.
     */
    size_t entries() const {
        return container_.entries();
    }

    /**
     * Returns the result of calling <tt>C::first()</tt> on the underlying container.
     */
    T first() const {
        return container_.first();
    }

    /**
     * Returns the result of calling <tt>C::removeFirst()</tt> on the underlying container.
     */
    T get() {
        return container_.removeFirst();
    }

    /**
     * Returns the result of calling <tt>C::isEmpty()</tt> on the underlying container.
     */
    bool isEmpty() const {
        return container_.isEmpty();
    }

    /**
     * Calls <tt>C::append(\a a)</tt> on the underlying container.
     */
    void insert(const T& a) {
        container_.append(a);
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * @copydoc insert()
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    void insert(T && a) {
        container_.append(rw_move(a));
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Returns the result of calling <tt>C::last()</tt> on the underlying container.
     */
    T last() const {
        return container_.last();
    }

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWTQueue<T, C>& rhs) {
        rw_swap(rhs.container_, container_);
    }

protected:

    C container_;

};

#endif  /* RW_TOOLS_TQUEUE_H */
