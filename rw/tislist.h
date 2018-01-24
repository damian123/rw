#ifndef RW_TOOLS_TISLIST_H
#define RW_TOOLS_TISLIST_H


/**********************************************************************
 *
 * RWTIsvSlist<T>: Parameterized intrusive list of Ts (which must derive from RWIsvSlink)
 * Declares the parameterized class RWTIsvSlist<T>, an intrusive
 * singly-linked list of links of type T, which MUST inherit from RWIsvSlink.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tislist.h#1 $
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
#include <rw/islist.h>

// forward declaration of the iterator:
template <class TL> class  RWTIsvSlistIterator;
template <class TL> class  RWTIsvSlistConstIterator;

/****************************************************************
 *                                                              *
 *              Declarations for RWTIsvSlist<T>                 *
 *                                                              *
 ****************************************************************/

/**
 * @ingroup traditional_collection_classes
 * @brief Implements intrusive singly-linked lists.
 *
 * Class \link RWTIsvSlist RWTIsvSlist<T>\endlink implements
 * intrusive singly-linked lists.
 *
 * An intrusive list requires that all members of the list inherit
 * from a common base class, in this case RWIsvSlink.  The advantage
 * of such a list is that memory and space requirements are
 * minimized.  The disadvantage is that the inheritance hierarchy
 * is inflexible, possibly complicating its use with
 * an existing class.  Class \link RWTValSlist RWTValSlist<T,A>\endlink is
 * offered as an alternative, non-intrusive, linked list.
 *
 * See Stroustrup (1991; Section 8.3.1) for more information about
 * intrusive lists.
 *
 * Note that when you insert an item into an intrusive list, the
 * actual item (not a copy) is inserted.  Because each item carries
 * only one link field, the same item cannot be inserted into more
 * than one list, nor can it be inserted into the same list more
 * than once.
 * This class is not CopyConstructible or CopyAssignable but is
 * MoveConstructible and MoveAssignable.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tislist.h>
 * RWTIsvSlist<T> list;
 * @endcode
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/cstring.h>
 * #include <rw/tislist.h>
 *
 * struct MySymbol : public RWIsvSlink
 * {
 *     MySymbol (const char* s) : s_ (s) { }
 *
 *     const RWCString& name () const {
 *         return s_;
 *     }
 *
 *   private:
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
 *     RWTIsvSlist<MySymbol> list;
 *
 *     list.insert ( new MySymbol("one") );
 *     list.insert ( new MySymbol("two") );
 *     list.prepend( new MySymbol("zero") );
 *
 *     list.apply (printem, 0);
 *     list.clearAndDestroy(); // Deletes the items inserted into the list
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
template <class TL> class  RWTIsvSlist : private RWIsvSlist
{

public:

    /**
     * Constructs an empty list.
     */
    RWTIsvSlist() { }

    /**
     * Constructs a list containing the single item pointed to by \a a.
     */
    RWTIsvSlist(TL* a) : RWIsvSlist(a) { }

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed RWTIsvDlist takes ownership of
     * the data owned by \a lst.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTIsvSlist(RWTIsvSlist && lst) : RWIsvSlist() {
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
    RWTIsvSlist& operator=(RWTIsvSlist && lst) {
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
        RWIsvSlist::clear();
    }

    /**
     * Returns the number of items currently in the list.
     */
    size_t        entries() const {
        return RWIsvSlist::entries();
    }

    /**
     * Returns \c true if there are no items in the list, \c false otherwise.
     */
    bool          isEmpty() const {
        return RWIsvSlist::isEmpty();
    }

    /**
     * Appends the item pointed to by \a a to the end of the list.
     */
    void          append(TL* a) {
        RWIsvSlist::append(a);
    }

    /**
     * Calls the function pointed to by \a applyFun to every item in the
     * collection. The item will be passed in as argument \c item. Client
     * data may be passed through as parameter \a d.
     */
    void          apply(void (*applyFun)(TL*, void*), void* d);

    /**
     * Returns the item at index \a i.
     * @throw RWBoundsErr Thrown with a message of \c RWTOOL_INDEX if
     * the index \a i is not between zero and the number of items in
     * the collection less one.
     */
    TL*           at(size_t i) const {
        return (TL*)RWIsvSlist::at(i);
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
        return RWIsvSlist::containsReference(a);
    }

    /**
     * Returns the first item in the list for which the user-defined
     * function pointed to by \a testFun returns \c true.  If there
     * is no such item, then returns #rwnil.
     *
     * For each item in the list, this function is called with the
     * item as the first argument.  Client data may be passed through
     * as parameter \a d.
     */
    TL*           find(bool (*testFun)(const TL*, void*), void* d) const;

    /**
     * Returns (but does not remove) the first item in the list, or
     * #rwnil if the list is empty.
     */
    TL*           first() const {
        return (TL*)RWIsvSlist::first();
    }

    /**
     * Returns <i>and removes</i> the first item in the list, or #rwnil
     * if the list is empty.
     */
    TL*           get() {
        return (TL*)RWIsvSlist::get();
    }

    /**
     * Returns the index of the first item in the list for which the
     * user-defined function pointed to by \a testFun returns \c true.
     * If there is no such item, then returns #RW_NPOS.
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
        RWIsvSlist::insert(a);
    }

    /**
     * Inserts the item pointed to by \a a at the index position \a i.
     *
     * The item cannot be inserted into more than one list, nor can
     * it be inserted into the same list more than once.
     * @throw RWBoundsErr Thrown with a message of \c RWTOOL_INDEX if
     * the index \a i is not between zero and the number of items in
     * the list.
     */
    void          insertAt(size_t i, TL* a) {
        RWIsvSlist::insertAt(i, a);
    }

    /**
     * Returns (but does not remove) the last item in the list, or #rwnil
     * if the list is empty.
     */
    TL*           last() const {
        return (TL*)RWIsvSlist::last();
    }

    /**
     * Traverses the list and returns the number of times for which the
     * user-defined function pointed to by \a testFun returned \c true.
     *
     * For each item in the list, this function is called with the
     * item as the first argument.  Client data may be passed through
     * as parameter \a d.
     */
    size_t        occurrencesOf(bool (*testFun)(const TL*, void*), void* d) const;

    /**
     * Returns the number of times which the item pointed to by \a a
     * occurs in the list.  Because items cannot be inserted into a
     * list more than once, this function can return only zero or one.
     */
    size_t        occurrencesOfReference(const TL* a) const {
        return RWIsvSlist::occurrencesOfReference(a);
    }

    /**
     * Prepends the item pointed to by \a a to the beginning of the
     * list.
     */
    void          prepend(TL* a) {
        RWIsvSlist::prepend(a);
    }

    /**
     * Removes and returns the first item for which the user-defined
     * function pointed to by \a testFun returns \c true, or #rwnil
     * if there is no such item.
     *
     * For each item in the list, this function is called with the
     * item as the first argument.  Client data may be passed through
     * as parameter \a d.
     */
    TL*           remove(bool (*testFun)(const TL*, void*), void* d);

    /**
     * Removes and returns the item at index \a i.
     * @throw RWBoundsErr Thrown with a message of \c RWTOOL_INDEX if
     * the index \a i is not between zero and the number of items in the
     * collection less one.
     */
    TL*           removeAt(size_t i) {
        return (TL*)RWIsvSlist::removeAt(i);
    }

    /**
     * Removes and returns the first item in the list, or #rwnil if
     * there are no items in the list.
     */
    TL*           removeFirst() {
        return (TL*)RWIsvSlist::removeFirst();
    }

    /**
     * Removes and returns the last item in the list, or #rwnil if there
     * are no items in the list.  Note that this function is relatively slow
     * because the enter list must be searched in order to remove of the
     * last link in a singly-linked list.
     */
    TL*           removeLast() {
        return (TL*)RWIsvSlist::removeLast();
    }

    /**
     * Removes and returns the link with address \a a.  The link must
     * be in the list.  In a singly-linked list this function is not
     * very efficient.
     */
    TL*           removeReference(TL* a) {
        return (TL*)RWIsvSlist::removeReference(a);
    }

    /**
     * Swaps the data owned by self with the data owned by \a lst.
     */
    void          swap(RWTIsvSlist<TL>& lst) {
        RWIsvSlist::swap(lst);
    }

protected:

    TL* headLink() const {
        return (TL*)(const_cast<RWIsvSlink*>(&head_));
    }

    TL* tailLink() const {
        return (TL*)(const_cast<RWIsvSlink*>(&tail_));
    }

    TL* firstLink() const {
        return (TL*)(const_cast<RWIsvSlink*>(head_.next_));
    }

    TL* lastLink() const {
        return (TL*)(const_cast<RWIsvSlink*>(last_));
    }

    TL* removeRight(TL* a) {
        return (TL*)RWIsvSlist::removeRight(a);
    }

    void init() {
        RWIsvSlist::init();
    }

private:

    friend class  RWTIsvSlistIterator<TL>;
    friend class  RWTIsvSlistConstIterator<TL>;

};

/****************************************************************
 *
 *    Declarations for RWTIsvSlistIterator
 *
 ****************************************************************/

/**
 * @ingroup traditional_collection_classes
 * @brief Iterator for class \link RWTIsvSlist RWTIsvSlist<T> \endlink
 *
 * RWTIsvSlistIterator provides an iterator interface for
 * class RWTIsvSlist<T>, allowing sequential access
 * to all the elements of a singly-linked parameterized intrusive
 * list.  Elements are accessed in order, from first to last.
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
 * #include <rw/tislist.h>
 * RWTIsvSlist<T> list;
 * RWTIsvSlistIterator<T> iterator(list);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class TL> class  RWTIsvSlistIterator :
    private RWIsvSlistIterator
{

public:

    // Constructor: starts with iterator positioned at headLink
    /**
     * Constructs an iterator to be used with the list \a c.
     */
    RWTIsvSlistIterator(RWTIsvSlist<TL>& c) : RWIsvSlistIterator(c) {
        ;
    }

    // Operators:
    /**
     * Advances the iterator one position, returning a pointer to the
     * new link, or #rwnil if the end of the list has been reached.
     */
    TL*   operator++() {
        return (TL*)RWIsvSlistIterator::operator++();
    }

    /**
     * Advances the iterator \a n positions, returning a pointer to
     * the new link, or #rwnil if the end of the list has been reached.
     */
    TL*   operator+=(size_t n) {
        return (TL*)RWIsvSlistIterator::operator+=(n);
    }

    /**
     * Advances the iterator one position, returning a pointer to the
     * new link, or #rwnil if the end of the list has been reached.
     */
    TL*   operator()() {
        return (TL*)RWIsvSlistIterator::operator()();
    }

    // Member functions:
    /**
     * Returns a pointer to the collection over which this iterator
     * is iterating.
     */
    RWTIsvSlist<TL>* container() const {
        return (RWTIsvSlist<TL>*)slist_;
    }

    /**
     * Advances the iterator to the first link for which the function pointed
     * to by \a testFun returns \c true and returns it, or #rwnil if there is
     * no such link.
     */
    TL*           findNext(bool (*testFun)(const TL*, void*), void*);

    /**
     * Inserts the link pointed to by \a a into the iterator's associated
     * collection in the position immediately after the iterator's current
     * position.
     */
    void          insertAfterPoint(TL* a) {
        RWIsvSlistIterator::insertAfterPoint(a);
    }

    /**
     * Returns the link at the iterator's current position.  Returns
     * #rwnil if the iterator is not valid.
     */
    TL*           key() const {
        return (TL*)RWIsvSlistIterator::key();
    }

    /**
     * Removes and returns the current link from the iterator's associated
     * collection.  Returns #rwnil if unsuccessful.  If
     * successful, the iterator will be positioned at the element immediately
     * before the removed link.  This function is relatively inefficient
     * for a singly-linked list.
     */
    TL*           remove() {      // Relatively slow
        return (TL*)RWIsvSlistIterator::remove();
    }

    /**
     * Advances the iterator to the first link for which the function pointed
     * to by \a testFun returns \c true, removes and returns it. Returns
     * \c false if unsuccessful.  If successful, the iterator will
     * be positioned at the element immediately before the removed element.
     */
    TL*           removeNext(bool (*testFun)(const TL*, void*), void*);

    /**
     * Resets the iterator to the state it had immediately after construction.
     */
    void          reset() {
        RWIsvSlistIterator::reset();
    }

    /**
     * Resets the iterator to iterate over the collection \a c.
     */
    void          reset(RWTIsvSlist<TL>& c) {
        RWIsvSlistIterator::reset(c);
    }

protected:

    void          advance() {
        RWIsvSlistIterator::advance();
    }
    bool          isActive() const {
        return RWIsvSlistIterator::isActive();
    }

    TL*           cursor() const {
        // check that a null pointer is not going to be returned accidentally
        // but instead explicitly.  This makes GCC 4.4 strict aliasing happy.
#if defined(RW_GCC_VERSION) && (RW_GCC_VERSION >= 40400 && RW_GCC_VERSION < 40500)
        if (shere_) {
            return (TL*)shere_;
        }
        return rwnil;
#else
        return (TL*)shere_;
#endif
    }

private:
    friend class RWTIsvSlistConstIterator<TL>;

    // Disallow postfix increment.  Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    TL*           operator++(int);

};

/****************************************************************
 *
 *              Declarations for RWTIsvSlistConstIterator
 *
 ****************************************************************/

template <class TL>
class  RWTIsvSlistConstIterator : private RWIsvSlistConstIterator
{
public:

    // Constructor: starts with iterator positioned at headLink
    RWTIsvSlistConstIterator(const RWTIsvSlist<TL>& s) :
        RWIsvSlistConstIterator(const_cast<RWTIsvSlist<TL>&>(s)) {
        ;
    }

    RWTIsvSlistConstIterator(const RWTIsvSlistIterator<TL>& itr) :
        RWIsvSlistConstIterator(*itr.slist_) {
        shere_ = itr.shere_;
    }

    // Operators:
    const TL* operator++() {
        return static_cast<const TL*>(RWIsvSlistConstIterator::operator++());
    }
    const TL* operator+=(size_t n) {
        return static_cast<const TL*>(RWIsvSlistConstIterator::operator+=(n));
    }
    const TL* operator()() {
        return static_cast<const TL*>(RWIsvSlistConstIterator::operator()());
    }

    // Member functions:
    const RWTIsvSlist<TL>* container() const {
        return static_cast<RWTIsvSlist<TL>*>(const_cast<RWIsvSlist*>(slist_));
    }

    const TL*     findNext(bool (*testFun)(const TL*, void*), void*);

    const TL*     key() const {
        return static_cast<const TL*>(RWIsvSlistConstIterator::key());
    }

    void          reset() {
        RWIsvSlistConstIterator::reset();
    }

    void          reset(const RWTIsvSlist<TL>& s) {
        RWIsvSlistConstIterator::reset(s);
    }

protected:

    void          advance() {
        RWIsvSlistConstIterator::advance();
    }
    bool          isActive() const {
        return RWIsvSlistConstIterator::isActive();
    }

    const TL*           cursor() const {
        return reinterpret_cast<const TL*>
               (const_cast<RWIsvSlink*>(shere_));
    }

private:

    // Disallow postfix increment.  Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    const TL*           operator++(int);

};

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tislist.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tislist.cc"
#endif


#endif  /* RW_TOOLS_TISLIST_H */
