

/**********************************************************************
 *
 * Template definitions for RWClassicTPtrHashDictionary<KP,VP>
 * and RWClassicTPtrHashDictionaryIterator<KP,VP>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xphdict.cc#1 $
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



template <class KP, class VP>
RWClassicTPtrHashDictionary<KP, VP>::RWClassicTPtrHashDictionary
(
    RW_HASH_FUNC(KP, hashFun),
    size_t size
)
    :  table_(size ? size : 2, rwnil),
       nitems_(0),
       hashFun_(hashFun)
{
    ;
}

template <class KP, class VP>
RWClassicTPtrHashDictionary<KP, VP>::RWClassicTPtrHashDictionary
(
    const RWClassicTPtrHashDictionary<KP, VP>& v
)
    :  table_(v.table_.entries(), rwnil),
       nitems_(v.nitems_),
       hashFun_(v.hashFun_)
{
    size_t N = table_.entries();
    for (size_t i = 0; i < N; i++) {
        // Use copy constructor:
        if (v.table_(i))
            table_(i) = new
            RWTPtrSlistDictionary<KP, VP>(*v.table_(i));
    }
}

template <class KP, class VP>
RWClassicTPtrHashDictionary<KP, VP>::~RWClassicTPtrHashDictionary()
{
    clear();
}

template <class KP, class VP> RWClassicTPtrHashDictionary<KP, VP>&
RWClassicTPtrHashDictionary<KP, VP>::operator=(const RWClassicTPtrHashDictionary<KP, VP>& v)
{
    if (this != &v) {
        clear();
        hashFun_ = v.hashFun_;  // Transfer hashing function
        size_t N;
        table_.reshape(N = v.table_.entries());
        for (size_t i = 0; i < N; i++) { // Fill the table as needed
            table_(i) = v.table_(i) ? new
                        RWTPtrSlistDictionary<KP, VP>(*v.table_(i)) : rwnil;
        }
        nitems_ = v.nitems_;    // Fix up counter
    }
    return *this;
}

template <class KP, class VP> VP*&
RWClassicTPtrHashDictionary<KP, VP>::operator[](KP* key)
{
    RW_ASSERT(0 != table_.entries());
    size_t idx = hashIndex(key);
    RWTPtrSlistDictionary<KP, VP>* chain = table_(idx);

    if (chain == rwnil)
        table_(idx) = chain = new
        RWTPtrSlistDictionary<KP, VP>;

    size_t N = chain->entries();

    VP*& val = chain->operator[](key);
    if (N != chain->entries()) {
        nitems_++;    // Ugly, I know.
    }

    return val;
}

template <class KP, class VP> void
RWClassicTPtrHashDictionary<KP, VP>::applyToKeyAndValue(void (*applyFun)(KP*, VP*&, void*), void* a)
{
    size_t N = table_.entries();
    for (size_t i = 0; i < N; i++) {
        if (table_(i)) {
            table_(i)->applyToKeyAndValue(applyFun, a);
        }
    }
}

template <class KP, class VP> void
RWClassicTPtrHashDictionary<KP, VP>::clear()
{
    size_t i = table_.entries();
    while (i--) {
        delete table_(i);       // Always ok to delete nil
        table_(i) = rwnil;
    }

    nitems_ = 0;
}

template <class KP, class VP> void
RWClassicTPtrHashDictionary<KP, VP>::clearAndDestroy()
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

template <class KP, class VP> bool
RWClassicTPtrHashDictionary<KP, VP>::contains(const KP* key) const
{
    if (0 == table_.entries()) {
        return false;
    }
    // else
    RWTPtrSlistDictionary<KP, VP>* chain = table_(hashIndex(key));
    return chain ? chain->contains(key) : false;
}

template <class KP, class VP> KP*
RWClassicTPtrHashDictionary<KP, VP>::find(const KP* key) const
{
    if (0 == table_.entries()) {
        return 0;
    }
    // else
    RWTPtrSlistDictionary<KP, VP>* chain = table_(hashIndex(key));
    return chain ? chain->find(key) : rwnil;
}

template <class KP, class VP> VP*
RWClassicTPtrHashDictionary<KP, VP>::findValue(const KP* key) const
{
    if (0 == table_.entries()) {
        return 0;
    }
    // else
    RWTPtrSlistDictionary<KP, VP>* chain = table_(hashIndex(key));
    return chain ? chain->findValue(key) : rwnil;
}

template <class KP, class VP> KP*
RWClassicTPtrHashDictionary<KP, VP>::findKeyAndValue(const KP* key, VP*& retVal) const
{
    if (0 == table_.entries()) {
        return 0;
    }
    // else
    RWTPtrSlistDictionary<KP, VP>* chain = table_(hashIndex(key));
    return chain ? chain->findKeyAndValue(key, retVal) : rwnil;
}

template <class KP, class VP> KP*
RWClassicTPtrHashDictionary<KP, VP>::removeKeyAndValue(const KP* key, VP*& retVal)
{
    if (0 == table_.entries()) {
        return 0;
    }
    // else
    RWTPtrSlistDictionary<KP, VP>* chain = table_(hashIndex(key));
    return chain ? chain->removeKeyAndValue(key, retVal) : rwnil;
}

template <class KP, class VP> KP*
RWClassicTPtrHashDictionary<KP, VP>::remove(const KP* key)
{
    if (0 == table_.entries()) {
        return 0;
    }
    // else
    RWTPtrSlistDictionary<KP, VP>* chain = table_(hashIndex(key));
    KP* ret = chain ? chain->remove(key) : rwnil;
    if (ret) {
        nitems_--;
    }
    return ret;
}

template <class KP, class VP> void
RWClassicTPtrHashDictionary<KP, VP>::resize(size_t N)
{
    if (table_.entries() == N) {
        return;    // save the effort
    }

#ifdef RW_DEBUG
    size_t oldNitems = nitems_;
#endif

    // Save old values -- NB: this involves a copy
    RWTPtrVector<RWTPtrSlistDictionary<KP, VP> > tempTable = table_;

    table_.reshape(N);        // Resize internal table
    table_  = rwnil;      // Zero it
    nitems_ = 0;

    // Now iterate through the old collection, inserting each item
    for (size_t i = 0; i < tempTable.entries(); i++) {
        if (tempTable(i)) {
            RWTPtrSlistDictionaryIterator<KP, VP> next(*tempTable(i));
            while (++next) {
                insertKeyAndValue(next.key(), next.value());
            }
            delete tempTable(i);
        }
    }
#ifdef RW_DEBUG
    assert(nitems_ == oldNitems); // Make sure we got 'em all.
#endif
}

/****************************************************************
 *                              *
 *     Definitions for RWClassicTPtrHashDictionaryIterator<KP,VP>   *
 *                              *
 ****************************************************************/

template <class KP, class VP>
RWClassicTPtrHashDictionaryIterator<KP, VP>::RWClassicTPtrHashDictionaryIterator
(
    RWClassicTPtrHashDictionary<KP, VP>& d
)
    :  myDict_(&d),
       idx_(RW_NPOS),
       iterator_(rwnil)
{
    reset();
}

// Copy constructor:
template <class KP, class VP>
RWClassicTPtrHashDictionaryIterator<KP, VP>::RWClassicTPtrHashDictionaryIterator
(
    const RWClassicTPtrHashDictionaryIterator<KP, VP>& h
)
    :  myDict_(h.myDict_),
       idx_(h.idx_)
{
    iterator_ = h.iterator_ ? new
                RWTPtrSlistDictionaryIterator<KP, VP>(*h.iterator_) : rwnil;
}

// Assignment operator:
template <class KP, class VP> RWClassicTPtrHashDictionaryIterator<KP, VP>&
RWClassicTPtrHashDictionaryIterator<KP, VP>::operator=
(
    const RWClassicTPtrHashDictionaryIterator<KP, VP>& h
)
{
    if (this != &h) {
        delete iterator_;
        myDict_  = h.myDict_;
        idx_     = h.idx_;
        iterator_ = h.iterator_ ? new
                    RWTPtrSlistDictionaryIterator<KP, VP>(*h.iterator_) : rwnil;
    }
    return *this;
}

template <class KP, class VP> bool
RWClassicTPtrHashDictionaryIterator<KP, VP>::operator++()
{
    while (iterator_ && ++(*iterator_) == false) {
        nextChain();
    }
    return iterator_ != rwnil;
}

template <class KP, class VP> void
RWClassicTPtrHashDictionaryIterator<KP, VP>::reset()
{
    delete iterator_;
    iterator_ = rwnil;

    // Advance to the first non-empty bucket:
    for (idx_ = 0; idx_ < myDict_->table_.entries(); idx_++) {
        if (myDict_->table_(idx_)) {
            iterator_ = new
            RWTPtrSlistDictionaryIterator<KP, VP>(*myDict_->table_(idx_));
            return;
        }
    }
}

template <class KP, class VP> void
RWClassicTPtrHashDictionaryIterator<KP, VP>::reset(RWClassicTPtrHashDictionary<KP, VP>& t)
{
    myDict_ = &t;
    reset();
}

// Advance the iterator to work on the next chain.
template <class KP, class VP> void
RWClassicTPtrHashDictionaryIterator<KP, VP>::nextChain()
{
    while (++idx_ < myDict_->table_.entries()) {
        if (myDict_->table_(idx_)) {
            if (iterator_) {
                iterator_->reset(*myDict_->table_(idx_));
            }
            else
                iterator_ = new
                RWTPtrSlistDictionaryIterator<KP, VP>(*myDict_->table_(idx_));
            return;
        }
    }

    delete iterator_;     // Invalidate old iterator
    iterator_ = rwnil;

}
