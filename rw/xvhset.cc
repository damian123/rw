

/**********************************************************************
 *
 * Template70
 definitions for RWClassicTValHashSet<T>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xvhset.cc#1 $
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


template <class T>
RWClassicTValHashSet<T>::~RWClassicTValHashSet() {}

template <class T> void
RWClassicTValHashSet<T>::insert(const T& val)
{
    RWClassicTValHashTable<T>::insertOnce(val);
}

/*
 * Perform subset test.
 */
template <class T> bool
RWClassicTValHashSet<T>::isSubsetOf(const RWClassicTValHashSet<T>& h) const
{
    // Cast away self-constness when making the iterator
    RWClassicTValHashTableIterator<T>        iterator(* (RWClassicTValHashSet<T>*)this);

    while (iterator())
        if (!h.contains(iterator.key())) {
            return false;
        }
    return true;
}

template <class T> bool
RWClassicTValHashSet<T>::isProperSubsetOf(const RWClassicTValHashSet<T>& h) const
{
    // A proper subset is a subset where sets are not equal, thus,
    // we can perform the subset test and exclude the case where they
    // are identical.  (Avoid == test because it must perform subset
    // operation also.)
    return (isSubsetOf(h) && (this->entries() != h.entries()));
}

/*
 * Perform insert operation on ourself for all members of second set.
 */
template <class T> RWClassicTValHashSet<T>&
RWClassicTValHashSet<T>::Union(const RWClassicTValHashSet<T>& h)
{
    // Cast away constness when making the iterator
    RWClassicTValHashTableIterator<T>      iterator(*((RWClassicTValHashSet<T>*) &h));

    while (iterator()) {
        insert(iterator.key());
    }
    return *this;
}

/*
 * Perform remove operation on ourself for all members of second set.
 */
template <class T> RWClassicTValHashSet<T>&
RWClassicTValHashSet<T>::difference(const RWClassicTValHashSet<T>& h)
{
    // Cast away constness when making the iterator
    RWClassicTValHashTableIterator<T>      iterator(*((RWClassicTValHashSet<T>*) &h));

    while (iterator()) {
        RWClassicTValHashTable<T>::remove(iterator.key());
    }
    return *this;
}

/*
 * Find all elements in the Union but not in the intersection of the two
 * sets.
 */
template <class T> RWClassicTValHashSet<T>&
RWClassicTValHashSet<T>::symmetricDifference(const RWClassicTValHashSet<T>& h)
{
    // Cast away constness when making the iterator
    RWClassicTValHashTableIterator<T>      iterator(*((RWClassicTValHashSet<T>*) &h));
    RWClassicTValHashSet<T>                insertionSet(this->hashFun_);

    //Remove from *h* all elements in common with *this* and place into
    //*insertionSet*
    while (iterator()) {
        if (!RWClassicTValHashTable<T>::contains(iterator.key())) {
            insertionSet.insert(iterator.key());
        }
    }

    iterator.reset();

    //Remove from *this* all elements of *h* in common with *this*
    while (iterator()) {
        RWClassicTValHashTable<T>::remove(iterator.key());
    }


    //Make *this* the symmetric difference
    Union(insertionSet);

    return *this;
}

/*
 * Set intersection (destructive) explanation:
 *    The template-based hash table iterators do not have facilities
 *    for deleting the current item being pointed at.  Attempts to
 *    delete the item from under the iterator will cause errant
 *    iterator behavior (It will never stop!).  Thus, we will
 *    build a deletion set while we are iterating through ourselves
 *    since we cannot delete as we go), and then perform a delete on ourself.
 *
 * Optimization: if h is the empty set, return the empty set immediately.
 */
template <class T> RWClassicTValHashSet<T>&
RWClassicTValHashSet<T>::intersection(const RWClassicTValHashSet<T>& h)
{
    // Check for case where h is empty first
    if (0 == h.entries()) {
        this->clear();
    }
    else {
        // Cast away self-constness when making the iterator
        RWClassicTValHashTableIterator<T>       selfiterator(* (RWClassicTValHashSet<T>*)this);
        RWClassicTValHashSet<T>                 deletionSet(this->hashFun_);
        T                                v;
        // Declare temp here to avoid stack allocation in loop below

        while (selfiterator()) {
            v = selfiterator.key();
            if (!h.contains(v)) {
                deletionSet.insert(v);
            }
        }

        difference(deletionSet);
    }
    return *this;
}

/*
 * Set comparison: Mathematically, if two sets are subsets of each other,
 * then they are identical.  We can perform an optimization whereby
 * we test to see if both sets are the same size, and then checking the
 * subset relation in one direction only.
 */
template <class T> bool
RWClassicTValHashSet<T>::isEquivalent(const RWClassicTValHashSet<T>& h) const
{
    return ((this->entries() == h.entries()) && isSubsetOf(h));
}
