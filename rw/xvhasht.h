#ifndef RW_TOOLS_XVHASHT_H
#define RW_TOOLS_XVHASHT_H


/**********************************************************************
 *
 * RWClassicTValHashTable<T>:  A Bag of values T, implemented using a hashed lookup
 * This class implements a parameterized hash table of types T.
 * It uses chaining to resolve hash collisions.
 * Example use of this class:
 *   #include <rw/cstring.h>
 *   #include <rw/tvhasht.h>
 *   unsigned myHash(const RWCString& s){ return s.hash(); }
 *   RWClassicTValHashTable<RWCString> bag(myHash); // A bag of RWCStrings
 *   bag.insert("a string");    // Type conversion: char* to RWCString happens
 *   bag.insert("another string");
 *   bag.contains("a string");  // Returns true.
 * Note that the constructor for RWClassicTValHashTable<T> takes a function with
 * prototype
 *   unsigned hashFun(const T&);
 * The argument is a reference to a const object of type T.
 * This function should return a suitable hashing value for an instance
 * of class T.
 * Usually, the definition for such a function is trivial because hashing
 * functions have been defined for all Rogue Wave supplied classes.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xvhasht.h#1 $
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
#include <rw/xvslist.h>
#include <rw/tpvector.h>

template <class T> class  RWClassicTValHashTableIterator;

/****************************************************************
 *                              *
 *      Declarations for RWClassicTValHashTable<T>      *
 *                              *
 ****************************************************************/

template <class T> class  RWClassicTValHashTable
{

public:

    // Constructors, destructors, etc:
    RWClassicTValHashTable
    (
        RW_HASH_FUNC(T, hashFun),   // Hashing function
        size_t size = RW_DEFAULT_CAPACITY   // No. of buckets
    );

    RWClassicTValHashTable
    (
        const RWClassicTValHashTable<T>&
    );

    virtual ~RWClassicTValHashTable();

    RWClassicTValHashTable<T>&    operator=(const RWClassicTValHashTable<T>&);

    // Member functions:
    void      apply(void (*applyFun)(const T&, void*), void*);

    void      clear();

    bool  contains(const T& val) const;

    size_t    entries() const {
        return nitems_;
    }

    bool  find(const T& a, T& k) const;  // First occurrence

    virtual void  insert(const T& val);

    bool  isEmpty() const {
        return nitems_ == 0;
    }

    size_t    occurrencesOf(const T& val) const;

    virtual bool  remove(const T& val);
    virtual size_t    removeAll(const T& val);

    void      resize(size_t); // Change # of buckets
    size_t        buckets() const { // report # of buckets
        return table_.entries();
    }

protected:  //methods

    size_t    hashIndex(const T& val) const {
        return (0 == table_.entries()) ? 0 : (size_t)(*hashFun_)(val) % table_.entries();
    }

    bool  insertOnce(const T& val);   // Useful for Sets

protected:  //data

    RWTPtrVector<RWClassicTValSlist<T> >  table_;
    size_t            nitems_;
    RW_HASH_FUNC(T, hashFun_);
private:

    friend class RWClassicTValHashTableIterator<T>;

};

/****************************************************************
 *                              *
 *  Declarations for RWClassicTValHashTableIterator<T>      *
 *                              *
 ****************************************************************/

template <class T> class  RWClassicTValHashTableIterator
{

public:

    RWClassicTValHashTableIterator(RWClassicTValHashTable<T>& t);
    RWClassicTValHashTableIterator(const RWClassicTValHashTableIterator<T>& h);
    ~RWClassicTValHashTableIterator() {
        delete iterator_;
    }

    RWClassicTValHashTableIterator<T>&    operator=(const RWClassicTValHashTableIterator<T>& h);

    bool  operator++();       // Advance and test
    bool  operator()() {
        return ++(*this);
    }

    RWClassicTValHashTable<T>* container() const {
        return hashTable_;
    }

    T     key() const {
        return iterator_->key();
    }

    void      reset();
    void      reset(RWClassicTValHashTable<T>& t);

private:

    RWClassicTValHashTable<T>*        hashTable_;
    size_t            idx_;
    RWClassicTValSlistIterator<T>*    iterator_;

    void          nextIterator();

};


#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("xvhasht.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/xvhasht.cc"
#endif


#endif  /* RW_TOOLS_TVHASHT_H */
