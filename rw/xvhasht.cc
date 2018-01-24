

/**********************************************************************
 *
 * Template definitions for RWClassicTValHashTable<T> and RWClassicTValHashTableIterator<T>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xvhasht.cc#1 $
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
RWClassicTValHashTable<T>::RWClassicTValHashTable
(
    RW_HASH_FUNC(T, hashFun),
    size_t size
)
    :  table_(size ? size : 2, rwnil), // Minimum of two buckets
       nitems_(0),
       hashFun_(hashFun)
{
    ;
}

template <class T>
RWClassicTValHashTable<T>::RWClassicTValHashTable(const RWClassicTValHashTable<T>& v)
    :  table_(v.table_.entries(), rwnil),
       nitems_(v.nitems_),
       hashFun_(v.hashFun_)
{
    size_t N = table_.entries();
    for (size_t i = 0; i < N; i++) {
        if (v.table_(i)) {
            table_(i) = new RWClassicTValSlist<T>(*v.table_(i));    //Use list copy constructor
        }
    }
}

template <class T>
RWClassicTValHashTable<T>::~RWClassicTValHashTable()
{
    clear();
}

template <class T> RWClassicTValHashTable<T>&
RWClassicTValHashTable<T>::operator=(const RWClassicTValHashTable<T>& v)
{
    if (&v != this) {
        clear();
        hashFun_ = v.hashFun_;  // get the hash function
        size_t N;
        table_.reshape(N = v.table_.entries()); // get the size, and use it
        for (size_t i = 0; i < N; i++) {   // fill the table as needed
            table_(i) = v.table_(i) ? new RWClassicTValSlist<T>(*v.table_(i))
                        : rwnil;
        }
        nitems_ = v.nitems_;    // fix up counter
    }
    return *this;
}

template <class T> void
RWClassicTValHashTable<T>::apply(void (*applyFun)(const T&, void*), void* a)
{
    size_t N = table_.entries();
    for (size_t i = 0; i < N; i++) {
        if (table_(i)) {
            table_(i)->apply((void (*)(T&, void*))applyFun, a);
        }
    }
}

template <class T> void
RWClassicTValHashTable<T>::clear()
{
    register size_t i = table_.entries();
    while (i--) {
        delete table_(i);       // Always ok to delete nil
        table_(i) = rwnil;
    }
    nitems_ = 0;
}

template <class T> bool
RWClassicTValHashTable<T>::contains(const T& val) const
{
    if (0 == table_.entries()) {
        return false;
    }
    // else
    RWClassicTValSlist<T>* p = table_(hashIndex(val));
    return p ? p->contains(val) : false;
}

template <class T> bool
RWClassicTValHashTable<T>::find(const T& val, T& retVal) const
{
    if (0 == table_.entries()) {
        return false;
    }
    // else
    RWClassicTValSlist<T>* p = table_(hashIndex(val));
    return p ? p->find(val, retVal) : false;
}

template <class T> void
RWClassicTValHashTable<T>::insert(const T& val)
{
    if (0 == table_.entries()) {
        return;
    }
    // else
    size_t idx = hashIndex(val);
    RW_ASSERT(idx < table_.entries());

    // Allocate a chain if one doesn't already exist:
    if (table_(idx) == rwnil) {
        table_(idx) = new RWClassicTValSlist<T>;
    }

    table_(idx)->insert(val); // Insert into the chain

    nitems_++;            // Update count
}

template <class T> size_t
RWClassicTValHashTable<T>::occurrencesOf(const T& val) const
{
    if (0 == table_.entries()) {
        return 0;
    }
    // else
    RWClassicTValSlist<T>* p = table_(hashIndex(val));
    return  p ? p->occurrencesOf(val) : 0;
}

template <class T> bool
RWClassicTValHashTable<T>::remove(const T& val)
{
    if (0 == table_.entries()) {
        return false;
    }
    // else
    RWClassicTValSlist<T>* p = table_(hashIndex(val));
    bool ret = p ? p->remove(val) : false;
    if (ret) {
        nitems_--;
    }
    return ret;
}

template <class T> size_t
RWClassicTValHashTable<T>::removeAll(const T& val)
{
    if (0 == table_.entries()) {
        return 0;
    }
    // else
    RWClassicTValSlist<T>* p = table_(hashIndex(val));
    size_t count = p ? p->removeAll(val) : 0;
    nitems_ -= count;
    return count;
}

template <class T> void
RWClassicTValHashTable<T>::resize(size_t N)
{
    if (table_.entries() == N) {
        return;    // save the effort
    }

    // Save old values -- NB: this involves a copy
    RWTPtrVector<RWClassicTValSlist<T> > tempTable = table_;

    table_.reshape(N);        // Resize internal table
    table_  = rwnil;      // Zero it
    nitems_ = 0;

    // Now iterate through the old collection, inserting each item
    for (size_t i = 0; i < tempTable.entries(); i++) {
        if (tempTable(i)) {
            RWClassicTValSlistIterator<T> next(*tempTable(i));
            while (++next) {
                insert(next.key());
            }
            delete tempTable(i);
        }
    }
}

// protected methods

// Insert a value only if it doesn't already exist in the collection
// (this function is useful for implementing sets).
template <class T> bool
RWClassicTValHashTable<T>::insertOnce(const T& val)
{
    if (0 == table_.entries()) {
        return false;
    }
    // else
    size_t idx = hashIndex(val);
    RW_ASSERT(idx < table_.entries());

    if (table_(idx) == rwnil) {
        table_(idx) = new RWClassicTValSlist<T>;
    }
    else if (table_(idx)->contains(val)) {
        return false;
    }

    table_(idx)->insert(val);
    nitems_++;
    return true;
}

/****************************************************************
 *                              *
 *  Definitions for RWClassicTValHashTableIterator<T>       *
 *                              *
 ****************************************************************/

template <class T>
RWClassicTValHashTableIterator<T>::RWClassicTValHashTableIterator(RWClassicTValHashTable<T>& t)
    :  hashTable_(&t),
       idx_(RW_NPOS),
       iterator_(rwnil)
{
    reset();
}

template <class T>
RWClassicTValHashTableIterator<T>::RWClassicTValHashTableIterator(const RWClassicTValHashTableIterator<T>& h)
    : hashTable_(h.hashTable_),
      idx_(h.idx_)
{
    iterator_ = h.iterator_ ? new
                RWClassicTValSlistIterator<T>(*h.iterator_) : rwnil;
}

template <class T> RWClassicTValHashTableIterator<T>&
RWClassicTValHashTableIterator<T>::operator=(const RWClassicTValHashTableIterator<T>& h)
{
    if (this != &h) {
        delete iterator_;
        hashTable_ = h.hashTable_;
        idx_       = h.idx_;
        iterator_ = h.iterator_ ? new
                    RWClassicTValSlistIterator<T>(*h.iterator_) : rwnil;
    }
    return *this;
}

template <class T> bool
RWClassicTValHashTableIterator<T>::operator++()
{
    // either spin over empty slots or advance into a non-empty one
    while (iterator_ && ++(*iterator_) == false) {
        nextIterator();
    }
    return iterator_ != rwnil;
}

template <class T> void
RWClassicTValHashTableIterator<T>::reset()
{
    delete iterator_;
    iterator_ = rwnil;

    // Advance to the first non-empty bucket:
    for (idx_ = 0; idx_ < hashTable_->table_.entries(); idx_++) {
        if (hashTable_->table_(idx_)) {
            iterator_ = new RWClassicTValSlistIterator<T>(*hashTable_->table_(idx_));
            return;
        }
    }
}

template <class T> void
RWClassicTValHashTableIterator<T>::reset(RWClassicTValHashTable<T>& t)
{
    hashTable_ = &t;
    reset();
}


// private function ---

template <class T> void
RWClassicTValHashTableIterator<T>::nextIterator()
{
    while (++idx_ < hashTable_->table_.entries()) {
        if (hashTable_->table_(idx_)) {
            if (iterator_) {
                iterator_->reset(*hashTable_->table_(idx_));
            }
            else {
                iterator_ = new RWClassicTValSlistIterator<T>(*hashTable_->table_(idx_));
            }
            return;
        }
    }

    delete iterator_;     // Invalidate old iterator
    iterator_ = rwnil;

}
