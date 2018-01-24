

/**********************************************************************
 *
 * Template definitions for RWClassicTPtrHashTable<TP> and RWClassicTPtrHashTableIterator<TP>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xphasht.cc#1 $
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



template <class TP>
RWClassicTPtrHashTable<TP>::RWClassicTPtrHashTable
(
    RW_HASH_FUNC(TP, hashFun),
    size_t size
)
    :  table_(size ? size : 2, rwnil), // Minimum of two buckets
       nitems_(0),
       hashFun_(hashFun)
{
    ;
}

template <class TP>
RWClassicTPtrHashTable<TP>::RWClassicTPtrHashTable(const RWClassicTPtrHashTable<TP>& v)
    :  table_(v.table_.entries(), rwnil),
       nitems_(v.nitems_),
       hashFun_(v.hashFun_)
{
    size_t N = table_.entries();
    for (size_t i = 0; i < N; i++) {
        if (v.table_(i))
            table_(i) = new
            RWClassicTPtrSlist<TP>(*v.table_(i));
    }
}

template <class TP>
RWClassicTPtrHashTable<TP>::~RWClassicTPtrHashTable()
{
    clear();
}

template <class TP> RWClassicTPtrHashTable<TP>&
RWClassicTPtrHashTable<TP>::operator=(const RWClassicTPtrHashTable<TP>& v)
{
    if (&v != this) {
        clear();
        hashFun_ = v.hashFun_;      // get the hash function
        size_t N;
        table_.reshape(N = v.table_.entries()); // get the size, and use it
        for (size_t i = 0; i < N; i++) {       // fill the table as needed
            table_(i) =
                v.table_(i)
                ? new RWClassicTPtrSlist<TP>(*v.table_(i))
                : rwnil;
        }
        nitems_ = v.nitems_;        // fix up counter
    }
    return *this;
}

template <class TP> void
RWClassicTPtrHashTable<TP>::apply(void (*applyFun)(TP*, void*), void* a)
{
    size_t N = table_.entries();
    for (size_t i = 0; i < N; i++) {
        if (table_(i)) {
            table_(i)->apply(applyFun, a);
        }
    }
}

template <class TP> void
RWClassicTPtrHashTable<TP>::clear()
{
    size_t i = table_.entries();
    while (i--) {
        delete table_(i);           // Always ok to delete nil
        table_(i) = rwnil;
    }
    nitems_ = 0;
}

template <class TP> void
RWClassicTPtrHashTable<TP>::clearAndDestroy()
{
    size_t i = table_.entries();
    while (i--) {
        if (table_(i)) {
            table_(i)->clearAndDestroy();
            delete table_(i);
            table_(i) = rwnil;
        }
    }
    nitems_ = 0;
}

template <class TP> bool
RWClassicTPtrHashTable<TP>::contains(const TP* val) const
{
    if (0 == table_.entries()) {
        return false;
    }
    RWClassicTPtrSlist<TP>* p = table_(hashIndex(val));
    return p ? p->contains(val) : false;
}

template <class TP> TP*
RWClassicTPtrHashTable<TP>::find(const TP* val) const
{
    if (0 == table_.entries()) {
        return 0;
    }
    RWClassicTPtrSlist<TP>* p = table_(hashIndex(val));
    return p ? p->find(val) : rwnil;
}

template <class TP> void
RWClassicTPtrHashTable<TP>::insert(TP* val)
{
    if (0 == table_.entries()) {
        return;
    }
    size_t idx = hashIndex(val);

    // Allocate a chain if one doesn't already exist:
    if (table_(idx) == rwnil) {
        table_(idx) = new RWClassicTPtrSlist<TP>;
    }

    table_(idx)->insert(val);     // Insert into the chain

    nitems_++;                    // Update count
}

template <class TP> size_t
RWClassicTPtrHashTable<TP>::occurrencesOf(const TP* val) const
{
    if (0 == table_.entries()) {
        return 0;
    }
    RWClassicTPtrSlist<TP>* p = table_(hashIndex(val));
    return  p ? p->occurrencesOf(val) : 0;
}

template <class TP> TP*
RWClassicTPtrHashTable<TP>::remove(const TP* val)
{
    if (0 == table_.entries()) {
        return 0;
    }
    RWClassicTPtrSlist<TP>* p = table_(hashIndex(val));
    TP* ret = p ? p->remove(val) : rwnil;
    if (ret) {
        nitems_--;
    }
    return ret;
}

template <class TP> size_t
RWClassicTPtrHashTable<TP>::removeAll(const TP* val)
{
    if (0 == table_.entries()) {
        return 0;
    }
    RWClassicTPtrSlist<TP>* p = table_(hashIndex(val));
    size_t count = p ? p->removeAll(val) : 0;
    nitems_ -= count;
    return count;
}

template <class TP> void
RWClassicTPtrHashTable<TP>::resize(size_t N)
{
    if (table_.entries() == N) {
        return;    // save the effort
    }

#ifdef RW_DEBUG
    size_t oldNitems = nitems_;
#endif

    // Save old values -- NB: this involves a copy
    RWTPtrVector<RWClassicTPtrSlist<TP> > tempTable = table_;

    table_.reshape(N);            // Resize internal table
    table_  = rwnil;              // Zero it
    nitems_ = 0;

    // Now iterate through the old collection, inserting each item
    for (size_t i = 0; i < tempTable.entries(); i++) {
        if (tempTable(i)) {
            RWClassicTPtrSlistIterator<TP> next(*tempTable(i));
            while (++next) {
                insert(next.key());
            }
            delete tempTable(i);
        }
    }
#ifdef RW_DEBUG
    assert(nitems_ == oldNitems); // Make sure we got 'em all.
#endif
}

// Insert a value only if it doesn't already exist in the collection
// (this function is useful for implementing sets).
template <class TP> bool
RWClassicTPtrHashTable<TP>::insertOnce(TP* val)
{
    if (0 == table_.entries()) {
        return false;
    }
    size_t idx = hashIndex(val);

    if (table_(idx) == rwnil) {
        table_(idx) = new RWClassicTPtrSlist<TP>;
    }

    else if (table_(idx)->contains(val)) {
        return false;
    }

    table_(idx)->insert(val);
    nitems_++;
    return true;
}

/****************************************************************
 *                                                              *
 *      Definitions for RWClassicTPtrHashTableIterator<TP>             *
 *                                                              *
 ****************************************************************/

template <class TP>
RWClassicTPtrHashTableIterator<TP>::RWClassicTPtrHashTableIterator(RWClassicTPtrHashTable<TP>& t)
    : hashTable_(&t),
      idx_(RW_NPOS),
      iterator_(rwnil)
{
    reset();
}

template <class TP>
RWClassicTPtrHashTableIterator<TP>::RWClassicTPtrHashTableIterator(const RWClassicTPtrHashTableIterator<TP>& h)
    : hashTable_(h.hashTable_),
      idx_(h.idx_)
{
    iterator_ = h.iterator_ ? new
                RWClassicTPtrSlistIterator<TP>(*h.iterator_)
                : rwnil;
}

template <class TP> RWClassicTPtrHashTableIterator<TP>&
RWClassicTPtrHashTableIterator<TP>::operator=(const RWClassicTPtrHashTableIterator<TP>& h)
{
    if (this != &h) {
        delete iterator_;
        hashTable_ = h.hashTable_;
        idx_       = h.idx_;
        iterator_ = h.iterator_ ? new
                    RWClassicTPtrSlistIterator<TP>(*h.iterator_)
                    : rwnil;
    }
    return *this;
}

template <class TP> bool
RWClassicTPtrHashTableIterator<TP>::operator++()
{
    // either spin over empty slots or advance into a non-empty one
    while (iterator_ && ++(*iterator_) == false) {
        nextIterator();
    }
    return iterator_ != rwnil;
}

template <class TP> void
RWClassicTPtrHashTableIterator<TP>::reset()
{
    delete iterator_;
    iterator_ = rwnil;

    // Advance to the first non-empty bucket:
    for (idx_ = 0; idx_ < hashTable_->table_.entries(); idx_++) {
        if (hashTable_->table_(idx_)) {
            iterator_ = new RWClassicTPtrSlistIterator<TP>(*hashTable_->table_(idx_));
            return;
        }
    }
}

template <class TP> void
RWClassicTPtrHashTableIterator<TP>::reset(RWClassicTPtrHashTable<TP>& t)
{
    hashTable_ = &t;
    reset();
}


// private function ---

template <class TP> void
RWClassicTPtrHashTableIterator<TP>::nextIterator()
{
    while (++idx_ < hashTable_->table_.entries()) {
        if (hashTable_->table_(idx_)) {
            if (iterator_) {
                iterator_->reset(*hashTable_->table_(idx_));
            }
            else {
                iterator_ = new RWClassicTPtrSlistIterator<TP>(*hashTable_->table_(idx_));
            }
            return;
        }
    }

    delete iterator_;             // Invalidate old iterator
    iterator_ = rwnil;

}
