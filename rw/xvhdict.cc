

/**********************************************************************
 *
 * Template definitions for RWClassicTValHashDictionary<K,V> and RWClassicTValHashDictionaryIterator<K,V>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xvhdict.cc#1 $
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


template <class K, class V>
RWClassicTValHashDictionary<K, V>::RWClassicTValHashDictionary
(
    RW_HASH_FUNC(K, hashFun),
    size_t size
)
    :  table_(size ? size : 2, rwnil), // Minimum of two buckets
       nitems_(0),
       hashFun_(hashFun)
{
    ;
}

template <class K, class V>
RWClassicTValHashDictionary<K, V>::RWClassicTValHashDictionary(const RWClassicTValHashDictionary<K, V>& v)
    :  table_(v.table_.entries(), rwnil),
       nitems_(v.nitems_),
       hashFun_(v.hashFun_)
{
    size_t N = table_.entries();
    for (size_t i = 0; i < N; i++) {
        // Use copy constructor:
        if (v.table_(i)) {
            table_(i) = new RWTValSlistDictionary<K, V>(*v.table_(i));
        }
    }
}

template <class K, class V>
RWClassicTValHashDictionary<K, V>::~RWClassicTValHashDictionary()
{
    clear();
}

template <class K, class V> RWClassicTValHashDictionary<K, V>&
RWClassicTValHashDictionary<K, V>::operator=(const RWClassicTValHashDictionary<K, V>& v)
{
    if (this != &v) {
        clear();
        hashFun_ = v.hashFun_;  // Transfer hashing function
        size_t N;
        table_.reshape(N = v.table_.entries());
        for (size_t i = 0; i < N; i++) { // Fill the table as needed
            table_(i) = v.table_(i) ? new
                        RWTValSlistDictionary<K, V>(*v.table_(i)) : rwnil;
        }
        nitems_ = v.nitems_;    // Fix up counter
    }
    return *this;
}

template <class K, class V> V&
RWClassicTValHashDictionary<K, V>::operator[](const K& key)
{
    RW_ASSERT(0 != table_.entries());
    size_t idx = hashIndex(key);
    RWTValSlistDictionary<K, V>* chain = table_(idx);

    if (chain == rwnil)
        table_(idx) = chain = new
        RWTValSlistDictionary<K, V>;

    size_t N = chain->entries();

    V& val = chain->operator[](key);
    if (N != chain->entries()) {
        nitems_++;    // Ugly, I know.
    }

    return val;
}

template <class K, class V> void
RWClassicTValHashDictionary<K, V>::applyToKeyAndValue(void (*applyFun)(const K&, V&, void*), void* a)
{
    size_t N = table_.entries();
    for (size_t i = 0; i < N; i++) {
        if (table_(i)) {
            table_(i)->applyToKeyAndValue(applyFun, a);
        }
    }
}

template <class K, class V> void
RWClassicTValHashDictionary<K, V>::clear()
{
    size_t i = table_.entries();
    while (i--) {
        delete table_(i);
        table_(i) = rwnil;
    }

    nitems_ = 0;
}

template <class K, class V> bool
RWClassicTValHashDictionary<K, V>::contains(const K& key) const
{
    if (0 == table_.entries()) {
        return false;
    }
    // else
    RWTValSlistDictionary<K, V>* chain = table_(hashIndex(key));
    return chain ? chain->contains(key) : false;
}

template <class K, class V> bool
RWClassicTValHashDictionary<K, V>::find(const K& key, K& retKey) const
{
    if (0 == table_.entries()) {
        return false;
    }
    // else
    RWTValSlistDictionary<K, V>* chain = table_(hashIndex(key));
    return chain ? chain->find(key, retKey) : false;
}

template <class K, class V> bool
RWClassicTValHashDictionary<K, V>::findValue(const K& key, V& retVal) const
{
    if (0 == table_.entries()) {
        return false;
    }
    // else
    RWTValSlistDictionary<K, V>* chain = table_(hashIndex(key));
    return chain ? chain->findValue(key, retVal) : false;
}

template <class K, class V> bool
RWClassicTValHashDictionary<K, V>::findKeyAndValue(const K& key, K& retKey, V& retVal) const
{
    if (0 == table_.entries()) {
        return false;
    }
    // else
    RWTValSlistDictionary<K, V>* chain = table_(hashIndex(key));
    return chain ? chain->findKeyAndValue(key, retKey, retVal) : false;
}

template <class K, class V> bool
RWClassicTValHashDictionary<K, V>::remove(const K& key)
{
    if (0 == table_.entries()) {
        return false;
    }
    // else
    RWTValSlistDictionary<K, V>* chain = table_(hashIndex(key));
    if (chain) {
        if (chain->remove(key)) {
            nitems_--;
            return true;
        }
    }
    return false;
}

template <class K, class V> void
RWClassicTValHashDictionary<K, V>::resize(size_t N)
{
    if (table_.entries() == N) {
        return;    // save the effort
    }

#ifdef RW_DEBUG
    size_t oldNitems = nitems_;
#endif

    // Save old values -- NB: this involves a copy
    RWTPtrVector<RWTValSlistDictionary<K, V> > tempTable = table_;

    table_.reshape(N);        // Resize internal table
    table_  = rwnil;      // Zero it
    nitems_ = 0;

    // Now iterate through the old collection, inserting each item
    for (size_t i = 0; i < tempTable.entries(); i++) {
        if (tempTable(i)) {
            RWTValSlistDictionaryIterator<K, V> next(*tempTable(i));
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
 *     Definitions for RWClassicTValHashDictionaryIterator<K,V> *
 *                              *
 ****************************************************************/

template <class K, class V>
RWClassicTValHashDictionaryIterator<K, V>::RWClassicTValHashDictionaryIterator(RWClassicTValHashDictionary<K, V>& d) :
    myDict_(&d),
    idx_(RW_NPOS),
    iterator_(rwnil)
{
    nextChain();  // Advance to the first chain
}

// Copy constructor:
template <class K, class V>
RWClassicTValHashDictionaryIterator<K, V>::RWClassicTValHashDictionaryIterator
(
    const RWClassicTValHashDictionaryIterator<K, V>& h
)
    :  myDict_(h.myDict_),
       idx_(h.idx_)
{
    iterator_ = h.iterator_ ? new
                RWTValSlistDictionaryIterator<K, V>(*h.iterator_) : rwnil;
}

// Assignment operator:
template <class K, class V> RWClassicTValHashDictionaryIterator<K, V>&
RWClassicTValHashDictionaryIterator<K, V>::operator=
(
    const RWClassicTValHashDictionaryIterator<K, V>& h
)
{
    if (this != &h) {
        delete iterator_;
        myDict_  = h.myDict_;
        idx_     = h.idx_;
        iterator_ = h.iterator_ ? new
                    RWTValSlistDictionaryIterator<K, V>(*h.iterator_) : rwnil;
    }
    return *this;
}

template <class K, class V> bool
RWClassicTValHashDictionaryIterator<K, V>::operator++()
{
    while (iterator_ && ++(*iterator_) == false) {
        nextChain();
    }
    return iterator_ != rwnil;
}

template <class K, class V> void
RWClassicTValHashDictionaryIterator<K, V>::reset()
{
    idx_ = RW_NPOS;
    nextChain();
}

template <class K, class V> void
RWClassicTValHashDictionaryIterator<K, V>::reset(RWClassicTValHashDictionary<K, V>& t)
{
    myDict_ = &t;
    idx_ = RW_NPOS;
    nextChain();
}

// Advance the iterator to work on the next chain.
template <class K, class V> void
RWClassicTValHashDictionaryIterator<K, V>::nextChain()
{
    while (++idx_ < myDict_->table_.entries()) {
        if (myDict_->table_(idx_)) {
            if (iterator_) {
                iterator_->reset(*myDict_->table_(idx_));
            }
            else
                iterator_ = new
                RWTValSlistDictionaryIterator<K, V>(*myDict_->table_(idx_));
            return;
        }
    }
    delete iterator_;
    iterator_ = rwnil;
}
