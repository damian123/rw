#ifndef RW_TOOLS_XPHDICT_H
#define RW_TOOLS_XPHDICT_H


/**********************************************************************
 *
 * RWClassicTPtrHashDictionary: A parameterized hashing dictionary using pointers
 *                       to keys of type K and pointers to values of type V.
 * Assumes that KP has:
 *   - well-defined equality semantics (KP::operator==(const KP&)).
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xphdict.h#1 $
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
#include <rw/tpsldict.h>
#include <rw/tpvector.h>

template <class KP, class VP> class  RWClassicTPtrHashDictionaryIterator;


template <class KP, class VP> class  RWClassicTPtrHashDictionary
{

public:

    // Constructors:
    RWClassicTPtrHashDictionary
    (
        RW_HASH_FUNC(KP, hashFun),  // Hashing function
        size_t size = RW_DEFAULT_CAPACITY   // No. of buckets
    );

    RWClassicTPtrHashDictionary
    (
        const RWClassicTPtrHashDictionary<KP, VP>&
    );

    ~RWClassicTPtrHashDictionary();

    // Operators:
    RWClassicTPtrHashDictionary<KP, VP>&   operator=(const RWClassicTPtrHashDictionary<KP, VP>&);

    // Look up key, add if not there:
    VP*&          operator[](KP* key);

    // Member functions:
    void          applyToKeyAndValue(void (*applyFun)(KP*, VP*&, void*), void*);

    void          clear();
    void          clearAndDestroy();

    bool      contains(const KP*) const;  // Contain key?

    size_t        entries() const {
        return nitems_;
    }

    bool      isEmpty() const {
        return nitems_ == 0;
    }

    KP*           find(const KP* key) const;
    VP*           findValue(const KP* key) const;
    KP*           findKeyAndValue(const KP* key, VP*& retVal) const;

    void          insertKeyAndValue(KP* key, VP* value) {
        (*this)[key] = value;
    }

    KP*           remove(const KP* k);
    KP*           removeKeyAndValue(const KP* key, VP*& retVal);

    void          resize(size_t); // Change # of buckets
    size_t            buckets() const { // report # of buckets
        return table_.entries();
    }

protected:

    size_t        hashIndex(const KP* val) const {
        return (0 == table_.entries()) ? 0 : (size_t)(*hashFun_)(*val) % table_.entries();
    }

    RWTPtrVector<RWTPtrSlistDictionary<KP, VP> > table_;
    size_t        nitems_;
    RW_HASH_FUNC(KP, hashFun_);

private:

    friend class RWClassicTPtrHashDictionaryIterator<KP, VP>;

};

/****************************************************************
 *                              *
 *  Declarations for RWClassicTPtrHashDictionaryIterator<KP,VP> *
 *                              *
 ****************************************************************/

template <class KP, class VP>
class  RWClassicTPtrHashDictionaryIterator
{

public:

    RWClassicTPtrHashDictionaryIterator(RWClassicTPtrHashDictionary<KP, VP>& s);
    RWClassicTPtrHashDictionaryIterator(const RWClassicTPtrHashDictionaryIterator<KP, VP>& h);
    ~RWClassicTPtrHashDictionaryIterator() {
        delete iterator_;
    }

    RWClassicTPtrHashDictionaryIterator<KP, VP>&
    operator=(const RWClassicTPtrHashDictionaryIterator<KP, VP>& h);

    bool      operator++();       // Advance and test
    KP*           operator()() {
        return ++(*this) ? iterator_->key() : rwnil;
    }

    RWClassicTPtrHashDictionary<KP, VP>* container() const {
        return myDict_;
    }

    KP*           key() const {
        return iterator_->key();
    }

    void          reset();
    void          reset(RWClassicTPtrHashDictionary<KP, VP>& s);

    VP*           value() const {
        return iterator_->value();
    }

private:

    RWClassicTPtrHashDictionary<KP, VP>* myDict_;
    size_t idx_;
    RWTPtrSlistDictionaryIterator<KP, VP>* iterator_;

    void          nextChain();        // Advance to the next chain

    // Disallow postfix increment.  Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool      operator++(int);

};

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("xphdict.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/xphdict.cc"
#endif


#endif  /* RW_TOOLS_XPHDICT_H */
