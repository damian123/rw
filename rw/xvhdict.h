#ifndef RW_TOOLS_XVHDICT_H
#define RW_TOOLS_XVHDICT_H


/**********************************************************************
 *
 * RWClassicTValHashDictionary: A parameterized hashing dictionary of keys K and values V.
 * Stores a *copy* of the inserted items into the collection.
 * Assumes that K and V have:
 *   - well-defined copy constructor (T::T(const T&) or equiv.);
 *   - well-defined assignment operator (T::operator=(const T&) or equiv.);
 * Assumes that K has:
 *   - well-defined equality semantics (T::operator==(const T&));
 * Assumes that V has:
 *   - a default constructor.
 * Note that while these are automatically defined for builtin types
 * (such as "int", "double", or any pointer), you may need to provide
 * appropriate operators for your own classes, particularly those with
 * constructors and/or pointers to other objects.
 * Note that the constructor takes a function with prototype
 *   unsigned hashFun(const K&);
 * The argument is a reference to a const object of type K.
 * This function should return a suitable hashing value for an instance
 * of class K.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xvhdict.h#1 $
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
#include <rw/tvsldict.h>
#include <rw/tpvector.h>

template <class K, class V> class  RWClassicTValHashDictionaryIterator;


template <class K, class V> class  RWClassicTValHashDictionary
{

public:

    // Constructors:
    RWClassicTValHashDictionary
    (
        RW_HASH_FUNC(K, hashFun),           // Hashing function
        size_t size = RW_DEFAULT_CAPACITY   // No. of buckets
    );

    RWClassicTValHashDictionary
    (
        const RWClassicTValHashDictionary<K, V>&
    );

    ~RWClassicTValHashDictionary();

    // Operators:
    RWClassicTValHashDictionary<K, V>&     operator=(const RWClassicTValHashDictionary<K, V>&);

    // Look up key, add if not there:
    V&            operator[](const K& key);

    // Member functions:
    void          applyToKeyAndValue(void (*applyFun)(const K&, V&, void*), void*);

    void          clear();

    bool      contains(const K&) const; // Contain key?

    size_t        entries() const {
        return nitems_;
    }

    bool      isEmpty() const {
        return nitems_ == 0;
    }

    bool      find(const K& key, K& retKey) const;
    bool      findValue(const K& key, V& retVal) const;
    bool      findKeyAndValue(const K& key, K& retKey, V& retVal) const;

    void          insertKeyAndValue(const K& key, const V& value) {
        (*this)[key] = value;
    }

    bool      remove(const K& k);

    void          resize(size_t); // Change # of buckets
    size_t            buckets() const { // report # of buckets
        return table_.entries();
    }

protected:

    size_t        hashIndex(const K& val) const {
        return (0 == table_.entries()) ?  0 : (size_t)(*hashFun_)(val) % table_.entries();
    }

    RWTPtrVector<RWTValSlistDictionary<K, V> > table_;
    size_t        nitems_;
    RW_HASH_FUNC(K, hashFun_);

private:

    friend class RWClassicTValHashDictionaryIterator<K, V>;

};

/****************************************************************
 *                              *
 *  Declarations for RWClassicTValHashDictionaryIterator<K,V>   *
 *                              *
 ****************************************************************/

template <class K, class V>
class  RWClassicTValHashDictionaryIterator
{

public:

    RWClassicTValHashDictionaryIterator(RWClassicTValHashDictionary<K, V>& s);
    RWClassicTValHashDictionaryIterator(const RWClassicTValHashDictionaryIterator<K, V>& h);
    ~RWClassicTValHashDictionaryIterator() {
        delete iterator_;
    }

    RWClassicTValHashDictionaryIterator<K, V>&
    operator=(const RWClassicTValHashDictionaryIterator<K, V>& h);

    bool      operator++();       // Advance and test
    bool      operator()() {
        return ++(*this);
    }

    RWClassicTValHashDictionary<K, V>* container() const {
        return myDict_;
    }

    K         key() const {
        return iterator_->key();
    }

    void          reset();
    void          reset(RWClassicTValHashDictionary<K, V>& s);

    V         value() const {
        return iterator_->value();
    }

private:

    RWClassicTValHashDictionary<K, V>* myDict_;
    size_t idx_;
    RWTValSlistDictionaryIterator<K, V>* iterator_;

    void          nextChain();        // Advance to the next chain

    // Disallow postfix increment.  Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool      operator++(int);

};

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("xvhdict.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/xvhdict.cc"
#endif


#endif  /* RW_TOOLS_XVHDICT_H */
