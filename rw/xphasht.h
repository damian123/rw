#ifndef RW_TOOLS_XPHASHT_H
#define RW_TOOLS_XPHASHT_H


/**********************************************************************
 *
 * RWClassicTPtrHashTable<TP>:  A Bag of pointers to objects of type TP,
 *   implemented using a hashed lookup
 * This class implements a parameterized hash table of pointers to types TP.
 * It uses chaining to resolve hash collisions.
 * Example use of this class:
 *   #include <rw/cstring.h>
 *   #include <rw/tphasht.h>
 *   unsigned myHash(const RWCString& s){ return s.hash(); }
 *   RWClassicTPtrHashTable<RWCString> bag(myHash); // Bag of ptrs to RWCStrings
 *   bag.insert(new RWCString("a string"));
 *   bag.insert(new RWCString("another string"));
 *   RWCString key("a string");
 *   bag.contains(&key);    // Returns true.
 * Note that the constructor for RWClassicTPtrHashTable<TP> takes a function with
 * prototype
 *   unsigned hashFun(const TP& s);
 * The argument is a reference to a const object of type TP.
 * This function should return a suitable hashing value for an instance
 * of class TP.
 * Usually, the definition for such a function is trivial because hashing
 * functions have been defined for all Rogue Wave supplied classes.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xphasht.h#1 $
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
#include <rw/xpslist.h>
#include <rw/tpvector.h>

template <class TP> class  RWClassicTPtrHashTableIterator;

/****************************************************************
 *                              *
 *      Declarations for RWClassicTPtrHashTable<TP>     *
 *                              *
 ****************************************************************/


template <class TP> class  RWClassicTPtrHashTable
{

public:

    // Constructors, destructors, etc:
    RWClassicTPtrHashTable
    (
        RW_HASH_FUNC(TP, hashFun),
        size_t size = RW_DEFAULT_CAPACITY
    );

    RWClassicTPtrHashTable
    (
        const RWClassicTPtrHashTable<TP>&
    );

    virtual ~RWClassicTPtrHashTable();

    RWClassicTPtrHashTable<TP>&   operator=(const RWClassicTPtrHashTable<TP>&);

    // Member functions:
    void      apply(void (*applyFun)(TP*, void*), void*);

    void      clear();

    void      clearAndDestroy();

    bool  contains(const TP* val) const;

    size_t    entries() const {
        return nitems_;
    }

    TP*       find(const TP* p) const;

    virtual void  insert(TP* val);

    bool  isEmpty() const {
        return nitems_ == 0;
    }

    size_t    occurrencesOf(const TP* val) const;

    virtual TP*       remove(const TP* val);
    virtual size_t    removeAll(const TP* val);

    void      resize(size_t);  // Change # of buckets
    size_t        buckets() const { // report # of buckets
        return table_.entries();
    }

protected:  //methods

    size_t    hashIndex(const TP* val) const {
        return (0 == table_.entries()) ? 0 : (size_t)(*hashFun_)(*val) % table_.entries();
    }

    bool  insertOnce(TP* val);    // Useful for Sets

protected:  //data

    RWTPtrVector<RWClassicTPtrSlist<TP> > table_;
    size_t                nitems_;
    RW_HASH_FUNC(TP, hashFun_);

private:

    friend class RWClassicTPtrHashTableIterator<TP>;

};


/****************************************************************
 *                              *
 *  Declarations for RWClassicTPtrHashTableIterator<TP>     *
 *                              *
 ****************************************************************/

template <class TP> class  RWClassicTPtrHashTableIterator
{

public:

    RWClassicTPtrHashTableIterator(RWClassicTPtrHashTable<TP>& t);
    RWClassicTPtrHashTableIterator(const RWClassicTPtrHashTableIterator<TP>& h);
    ~RWClassicTPtrHashTableIterator() {
        delete iterator_;
    }

    RWClassicTPtrHashTableIterator<TP>&   operator=(const RWClassicTPtrHashTableIterator<TP>& h);

    bool  operator++();       // Advance and test
    TP*       operator()() {
        return ++(*this) ? key() : rwnil;
    }

    RWClassicTPtrHashTable<TP>*   container() const {
        return hashTable_;
    }

    TP*       key() const {
        return iterator_->key();
    }

    void          reset();
    void          reset(RWClassicTPtrHashTable<TP>& t);

private:

    RWClassicTPtrHashTable<TP>*       hashTable_;
    size_t            idx_;
    RWClassicTPtrSlistIterator<TP>*   iterator_;

    void          nextIterator();

};

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("xphasht.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/xphasht.cc"
#endif


#endif  /* RW_TOOLS_XPHASHT_H */
