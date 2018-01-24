#ifndef RW_TOOLS_XVHSET_H
#define RW_TOOLS_XVHSET_H


/**********************************************************************
 *
 * RWClassicTValHashSet<T>:  A Set of values of type T, using a hashed lookup
 * This class implements a parameterized Set of types T.  In a Set,
 * only one instance of an object of a given value can be inserted into
 * the collection.
 * The implementation uses a hash table.
 * Example use of this class:
 *   #include <rw/cstring.h>
 *   #include <rw/tvhset.h>
 *   unsigned myHash(const RWCString& s){ return s.hash(); }
 *   RWClassicTValHashSet<RWCString> set(myHash);   // A Set of RWCStrings
 *   set.insert("a string");    // Type conversion: char* to RWCString happens
 *   set.insert("another string");
 *   set.insert("a string");    // Rejected (already exists in collection)
 *   set.contains("a string");  // Returns true.
 * Note that the constructor for RWClassicTValHashSet<T> takes a function with
 * prototype
 *   unsigned hashFun(const T&);
 * It should return a suitable hashing value for an instance of class T.
 * Usually, the definition for such a function is trivial because hashing
 * functions have been defined for all Rogue Wave supplied classes.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xvhset.h#1 $
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
#include <rw/xvhasht.h>

/****************************************************************
 *                              *
 *      Declarations for RWClassicTValHashSet<T>        *
 *                              *
 ****************************************************************/


template <class T> class  RWClassicTValHashSet : public RWClassicTValHashTable<T>
{

public:

    RWClassicTValHashSet
    (
        RW_HASH_FUNC(T, hashFun),
        size_t size = RW_DEFAULT_CAPACITY
    ) : RWClassicTValHashTable<T>(hashFun, size) {
        ;
    }

    virtual ~RWClassicTValHashSet();

    // Member functions:
    virtual void      insert(const T& val);

    // Implement set operation primitives union, disjunction, intersection,
    // subset, proper subset, and equivalence; respectively:
    //***********************************************************************
    //                    Pre-STDLIB interface
    //***********************************************************************
    //  RWClassicTValHashSet<T> &    operator+=(const RWClassicTValHashSet<T> & h);
    //  RWClassicTValHashSet<T> &    operator-=(const RWClassicTValHashSet<T> & h);
    //  RWClassicTValHashSet<T> &    operator*=(const RWClassicTValHashSet<T> & h);
    //  bool             operator<=(const RWClassicTValHashSet<T> & h) const;
    //  bool             operator< (const RWClassicTValHashSet<T> & h) const;
    //  bool             operator==(const RWClassicTValHashSet<T> & h) const;
    //  bool             operator!=(const RWClassicTValHashSet<T> & h) const
    //                        { return !operator==(h); };
    //***********************************************************************
    RWClassicTValHashSet<T>&   Union(const RWClassicTValHashSet<T>& h);
    RWClassicTValHashSet<T>&   difference(const RWClassicTValHashSet<T>& h);
    RWClassicTValHashSet<T>&   intersection(const RWClassicTValHashSet<T>& h);
    RWClassicTValHashSet<T>&   symmetricDifference(const RWClassicTValHashSet<T>& h);
    bool           isSubsetOf(const RWClassicTValHashSet<T>& h) const;
    bool           isProperSubsetOf(const RWClassicTValHashSet<T>& h) const;
    bool           isEquivalent(const RWClassicTValHashSet<T>& h) const;

};

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("xvhset.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/xvhset.cc"
#endif

#define RWClassicTValHashSetIterator RWClassicTValHashTableIterator


#endif  /* RW_TOOLS_XVHSET_H */
