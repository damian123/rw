#ifndef RW_TOOLS_TIDLIST_H
#define RW_TOOLS_TIDLIST_H



/**********************************************************************
 *
 * RWTIsvDlist<T>: Parameterized intrusive list of Ts
 *   (which must derive from RWIsvDlink)
 * Declares the parameterized class RWTIsvDlist<T>, an intrusive
 * doubly-linked list.  Items of type T are inserted into the list.
 * They must inherit from RWIsvSlink.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tidlist.h#1 $
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
#include <rw/idlist.h>

// forward declaration of the iterator:
template <class TL> class  RWTIsvDlistIterator;
template <class TL> class  RWTIsvDlistConstIterator;

/****************************************************************
 *                                                              *
 *              Declarations for RWTIsvDlist<T>                 *
 *                                                              *
 ****************************************************************/

/**
 * @ingroup traditional_collection_classes
 * @brief Implements intrusive doubly-linked lists.
 *
 * Class \link RWTIsvDlist RWTIsvDlist<T>\endlink implements
 * intrusive doubly-linked lists.
 *
 * An intrusive list requires that all members of the list inherit
 * from a common base class, in this case RWIsvDlink.  The advantage
 * of such a list is that memory and space requirements are
 * minimized.  The disadvantage is that the inheritance hierarchy
 * is inflexible, possibly complicating its use with
 * an existing class.  Class \link RWTValDlist RWTValDlist<T,A>\endlink is
 * offered as an alternative, non-intrusive, linked list.
 *
 * See Stroustrup (1991; Section 8.3.1) for more information about
 * intrusive lists.
 *
 * @note
 * When you insert an item into an intrusive list, the
 * <i>actual item </i>(not a copy) is inserted.  Because each item
 * carries only one link field, the same item cannot be inserted
 * into more than one list, nor can it be inserted into the same
 * list more than once.
 * This class is not CopyConstructible or CopyAssignable but is
 * MoveConstructible and MoveAssignable.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tidlist.h>
 * RWTIsvDlist<T> list;
 * @endcode
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/cstring.h>
 * #include <rw/tidlist.h>
 *
 * struct MySymbol : public RWIsvDlink
 * {
 *     MySymbol (const char* s) : s_ (s) { }
 *
 *     const RWCString& name () const {
 *         return s_;
 *     }
 *
 * private:
 *     RWCString s_;
 * };
 *
 * void printem(MySymbol* s, void*)
 * {
 *     std::cout << s->name () << std::endl;
 * }
 *
 * int main ()
 * {
 *     RWTIsvDlist<MySymbol> list;
 *
 *     list.insert ( new MySymbol("one") );
 *     list.insert ( new MySymbol("two") );
 *     list.prepend( new MySymbol("zero") );
 *
 *     list.apply (printem, 0);
 *     list.clearAndDestroy(); // Deletes the items inserted into
 *                             // the list
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 *
 * zero
 * one
 * two
 * @endcode
 *
 */
template <class TL> class  RWTIsvDlist : private RWIsvDlist
{

public:
    /**
     * Constructs an empty list.
     */
    RWTIsvDlist() { }

    /**
     * Constructs a list containing the single item pointed to by \a a.
     */
    RWTIsvDlist(TL* a) : RWIsvDlist(a) { }

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed RWTIsvDlist takes ownership of
     * the data owned by \a lst.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTIsvDlist(RWTIsvDlist && lst) : RWIsvDlist() {
        swap(lst);
    }
#  endif

    /********************* Member functions **************************/

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a lst.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTIsvDlist& operator=(RWTIsvDlist && lst) {
        if (&lst != this) {
            swap(lst);
        }
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Removes all items from the list.
     */
    void          clear() {
        RWIsvDlist::clear();
    }

    /**
     * Returns the number of items currently in the list.
     */
    size_t        entries() const {
        return RWIsvDlist::entries();
    }

    /**
     * Returns \c true if there are no items in the list, \c false otherwise.
     */
    bool          isEmpty() const {
        return RWIsvDlist::isEmpty();
    }

    /**
     * Appends the item pointed to by \a a to the end of the list.
     */
    void          append(TL* a) {
        RWIsvDlist::append(a);
    }

    /**
     * Calls the function pointed to by \a applyFun to every item in
     * the collection. Client data may be passed through as parameter
     * \a d.
     */
    void          apply(void (*applyFun)(TL*, void*), void* d);

    /**
     * Returns the item at index \a i.
     * @throw RWBoundsErr Thrown with a message of \c RWTOOL_INDEX if
     * the index \a i is not between zero and the number of items in
     * the collection less one.
     */
    TL*           at(size_t i) const {
        return (TL*)RWIsvDlist::at(i);
    }

    /**
     * Removes <i>and calls delete</i> for each item in the list.  Note
     * that this assumes that each item was allocated off the heap.
     */
    void          clearAndDestroy();

    /**
     * Returns \c true if the list contains an item for which the user-defined
     * function pointed to by \a testFun returns \c true.
     *
     * For each item in the list, this function is called with the
     * item as the first argument.  Client data may be passed through
     * as parameter \a d.
     */
    bool     contains(bool (*testFun)(const TL*, void*), void* d) const {
        return find(testFun, d) != rwnil;
    }

    /**
     * Returns \c true if the list contains an item with the address
     * \a a.
     */
    bool     containsReference(const TL* a) const {
        return RWIsvDlist::containsReference(a);
    }

    /**
     * Returns the first item in the list for which the user-defined
     * function pointed to by \a testFun returns \c true. If there is
     * no such item, then returns #rwnil.
     *
     * For each item in the list, this function is called with the
     * item as the first argument.  Client data may be passed through
     * as parameter \a d.
     */
    TL*           find(bool (*testFun)(const TL*, void*), void* d) const;

    /**
     * Returns (but does not remove) the first item in the list, or returns
     * #rwnil if the list is empty.
     */
    TL*           first() const {
        return (TL*)RWIsvDlist::first();
    }

    /**
     * Returns <i>and removes</i> the first item in the list, or
     * #rwnil if the list is empty.
     */
    TL*           get() {
        return (TL*)RWIsvDlist::get();
    }

    /**
     * Returns the index of the first item in the list for which the
     * user-defined function pointed to by \a testFun returns
     * \c true.  If there is no such item, then returns #RW_NPOS.
     *
     * For each item in the list, this function is called with the
     * item as the first argument.  Client data may be passed through
     * as parameter \a d.
     */
    size_t        index(bool (*testFun)(const TL*, void*), void* d) const;

    /**
     * Appends the item pointed to by \a a to the end of the list.
     * This item cannot be inserted into more than one list, nor can
     * it be inserted into the same list more than once.
     */
    void          insert(TL* a) {
        RWIsvDlist::insert(a);
    }

    /**
     * Inserts the item pointed to by \a a at the index position \a i.
     * The item cannot be inserted into more than one list, nor can
     * it be inserted into the same list more than once.
     * @throw RWBoundsErr Thrown with a message of \c RWTOOL_INDEX if
     * the index \a i is not between zero and the number of items in
     * the list.
     */
    void          insertAt(size_t i, TL* a) {
        RWIsvDlist::insertAt(i, a);
    }

    /**
     * Returns (but does not remove) the last item in the list, or
     * #rwnil if the list is empty.
     */
    TL*           last() const {
        return (TL*)RWIsvDlist::last();
    }

    /**
     * Traverses the list and returns the number of times for which
     * the user-defined function pointed to by \a testFun returned
     * \c true.
     *
     * For each item in the list, this function is called with the
     * item as the first argument.  Client data may be passed through
     * as parameter \a d.
     */
    size_t        occurrencesOf(bool (*testFun)(const TL*, void*), void* d) const;

    /**
     * Returns the number of times which the item pointed to by \a a
     * occurs in the list.  Because items cannot be inserted into a
     * list more than once, this function can only return zero or one.
     */
    size_t        occurrencesOfReference(const TL* a) const {
        return RWIsvDlist::occurrencesOfReference(a);
    }

    /**
     * Prepends the item pointed to by \a a to the beginning of the
     * list.
     */
    void          prepend(TL* a) {
        RWIsvDlist::prepend(a);
    }

    /**
     * Removes and returns the first item for which the user-defined
     * tester function pointed to by \a testFun returns \c true, or
     * #rwnil if there is no such item.
     *
     * For each item in the list, this function is called with the
     * item as the first argument.  Client data may be passed through
     * as parameter \a d.
     */
    TL*           remove(bool (*testFun)(const TL*, void*), void* d);

    /**
     * Removes and returns the item at index \a i.
     * @throw RWBoundsErr Thrown with a message of \c RWTOOL_INDEX if
     * the index \a i is not between zero and the number of items in
     * the collection less one.
     */
    TL*           removeAt(size_t i) {
        return (TL*)RWIsvDlist::removeAt(i);
    }

    /**
     * Removes and returns the first item in the list, or #rwnil if
     * there are no items in the list.
     */
    TL*           removeFirst() {
        return (TL*)RWIsvDlist::removeFirst();
    }

    /**
     * Removes and returns the last item in the list, or #rwnil if there
     * are no items in the list.
     */
    TL*           removeLast() {
        return (TL*)RWIsvDlist::removeLast();
    }

    /**
     * Removes and returns the item with address \a a, or #rwnil if
     * there is no such item.
     */
    TL*           removeReference(TL* a) {
        return (TL*)RWIsvDlist::removeReference(a);
    }

    /**
     * Swaps the data owned by self with the data owned by \a lst.
     */
    void          swap(RWTIsvDlist<TL>& lst) {
        RWIsvDlist::swap(lst);
    }
protected:

    TL*           headLink() const {
        return (TL*)&head_;
    }
    TL*           tailLink() const {
        return (TL*)&tail_;
    }
    TL*           firstLink() const {
        return (TL*)head_.next_;
    }
    TL*           lastLink() const {
        return (TL*)tail_.prev_;
    }

    void init() {
        RWIsvDlist::init();
    }

private:

    friend class  RWTIsvDlistIterator<TL>;
    friend class  RWTIsvDlistConstIterator<TL>;

};

/****************************************************************
 *                                                              *
 *      Declarations for RWTIsvDlistIterator<TL>                *
 *                                                              *
 ****************************************************************/

/**
 * @ingroup traditional_collection_classes
 * @brief Iterator for class \link RWTIsvDlist RWTIsvDlist<T> \endlink
 *
 * RWTIsvDlistIterator<TL> provides an iterator interface for
 * class \link RWTIsvDlist RWTIsvDlist<T>\endlink, allowing
 * sequential access to all the elements of a doubly-linked parameterized
 * intrusive list.  Elements are accessed in order, in either direction.
 *
 * The "current item" is undefined immediately after construction;
 * you must define it by using operator()() or some other (valid)
 * operation.
 *
 * Once the iterator has advanced beyond the end of the collection,
 * it is no longer valid; its continued use will result in undefined
 * behavior.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tidlist.h>
 * RWTIsvDlist<T> list;
 * RWTIsvDlistIterator<T> iterator(list);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class TL>
class  RWTIsvDlistIterator : private RWIsvDlistIterator
{

public:

    // Constructor: starts with iterator positioned at headLink
    /**
     * Constructs an iterator to be used with the list \a c.
     */
    RWTIsvDlistIterator(RWTIsvDlist<TL>& c) : RWIsvDlistIterator(c) {
        ;
    }

    // Operators:
    /**
     * Advances the iterator one position, returning a pointer to the
     * new link, or #rwnil if the end of the list has been reached.
     */
    TL*   operator++() {
        return (TL*)RWIsvDlistIterator::operator++();
    }

    /**
     * Reverses the iterator one position, returning a pointer to the
     * new link, or #rwnil if the beginning of the list has been reached.
     */
    TL*   operator--() {
        return (TL*)RWIsvDlistIterator::operator--();
    }

    /**
     * Advances the iterator \a n positions, returning a pointer to
     * the new link, or #rwnil if the end of the list has been reached.
     */
    TL*   operator+=(size_t n) {
        return (TL*)RWIsvDlistIterator::operator+=(n);
    }

    /**
     * Reverses the iterator \a n positions, returning a pointer to
     * the new link, or #rwnil if the beginning of the list has been
     * reached.
     */
    TL*   operator-=(size_t n) {
        return (TL*)RWIsvDlistIterator::operator-=(n);
    }

    /**
     * Advances the iterator one position, returning a pointer to the
     * new link, or #rwnil if the end of the list has been reached.
     */
    TL*   operator()() {
        return (TL*)RWIsvDlistIterator::operator()();
    }


    // Member functions:
    /**
     * Returns a pointer to the collection over which this iterator
     * is iterating.
     */
    RWTIsvDlist<TL>* container() const {
        return (RWTIsvDlist<TL>*)dlist_;
    }

    /**
     * Advances the iterator to the first link for which the tester
     * function pointed to by \a testFun returns \c true and returns
     * it, or #rwnil if there is no such link.
     */
    TL*           findNext(bool (*testFun)(const TL*, void*), void*);

    /**
     * Inserts the link pointed to by \a a into the iterator's associated
     * collection in the position immediately after the iterator's current
     * position.
     */
    void          insertAfterPoint(TL* a) {
        RWIsvDlistIterator::insertAfterPoint(a);
    }

    /**
     * Returns the link at the iterator's current position.  Returns
     * #rwnil if the iterator is not valid.
     */
    TL*           key() const {
        return (TL*)RWIsvDlistIterator::key();
    }

    /**
     * Removes and returns the current link from the iterator's associated
     * collection.  Returns #rwnil if unsuccessful.  If successful, the
     * iterator will be positioned at the element immediately
     * before the removed link.
     */
    TL*           remove() {
        return (TL*)RWIsvDlistIterator::remove();
    }

    /**
     * Advances the iterator to the first link for which the tester
     * function pointed to by \a testFun returns \c true, removes and
     * returns it.  Returns \c false if unsuccessful.  If
     * successful, the iterator will be positioned at the element immediately
     * before the removed element.
     */
    TL*           removeNext(bool (*testFun)(const TL*, void*), void*);

    /**
     * Resets the iterator to the state it had immediately after construction.
     */
    void          reset() {
        RWIsvDlistIterator::reset();
    }

    /**
     * Resets the iterator to iterate over the collection \a c.
     */
    void          reset(RWTIsvDlist<TL>& c) {
        RWIsvDlistIterator::reset(c);
    }

protected:

    TL*           cursor() const {
        return (TL*)dhere_;
    }

    void          advance() {
        RWIsvDlistIterator::advance();
    }
    void          backup() {
        RWIsvDlistIterator::backup();
    }
    bool          isActive() const {
        return RWIsvDlistIterator::isActive();
    }

private:
    friend class RWTIsvDlistConstIterator<TL>;

    // Disallow postfix increment.  Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool             operator++(int);
    bool             operator--(int);

};

/****************************************************************
 *                                                              *
 *      Declarations for RWTIsvDlistConstIterator<TL>           *
 *                                                              *
 ****************************************************************/

template <class TL>
class  RWTIsvDlistConstIterator : private RWIsvDlistConstIterator
{

public:

    // Constructor: starts with iterator positioned at headLink
    RWTIsvDlistConstIterator(const RWTIsvDlist<TL>& s) :
        RWIsvDlistConstIterator(const_cast<RWTIsvDlist<TL>&>(s)) {
        ;
    }

    RWTIsvDlistConstIterator(const RWTIsvDlistIterator<TL>& itr) :
        RWIsvDlistConstIterator(*itr.dlist_) {
        dhere_ = itr.dhere_;
    }

    // Operators:
    const TL* operator++() {
        return static_cast<const TL*>(RWIsvDlistConstIterator::operator++());
    }

    const TL* operator--() {
        return static_cast<const TL*>(RWIsvDlistConstIterator::operator--());
    }

    const TL* operator+=(size_t n) {
        return static_cast<const TL*>(RWIsvDlistConstIterator::operator+=(n));
    }

    const TL* operator-=(size_t n) {
        return static_cast<const TL*>(RWIsvDlistConstIterator::operator-=(n));
    }

    const TL* operator()() {
        return static_cast<const TL*>(RWIsvDlistConstIterator::operator()());
    }


    // Member functions:
    const RWTIsvDlist<TL>* container() const {
        return static_cast<const RWTIsvDlist<TL>*>(dlist_);
    }

    const TL*     findNext(bool (*testFun)(const TL*, void*), void*);

    const TL*     key() const {
        return static_cast<const TL*>(RWIsvDlistConstIterator::key());
    }

    void          reset() {
        RWIsvDlistConstIterator::reset();
    }

    void          reset(const RWTIsvDlist<TL>& s) {
        RWIsvDlistConstIterator::reset(s);
    }

protected:

    const TL*           cursor() const {
        return static_cast<TL*>(dhere_);
    }

    void          advance() {
        RWIsvDlistConstIterator::advance();
    }
    void          backup() {
        RWIsvDlistConstIterator::backup();
    }
    bool          isActive() const {
        return RWIsvDlistConstIterator::isActive();
    }

private:

    // Disallow postfix increment.  Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool             operator++(int);
    bool             operator--(int);

};


#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tidlist.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tidlist.cc"
#endif


#endif  /* RW_TOOLS_TIDLIST_H */
