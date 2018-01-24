#ifndef RW_TOOLS_TVSLDICT_H
#define RW_TOOLS_TVSLDICT_H


/**********************************************************************
 *
 * RWTValSlistDictionary<K,V>: A dictionary of keys of type K, values of type V,
 *   implemented as a singly-linked list
 * Stores a *copy* of the inserted items into the collection.
 * Assumes that K and V have:
 *   - well-defined copy constructor (T::T(const T&) or equiv.);
 *   - well-defined assignment operator (T::operator=(const T&) or equiv.);
 * Assumes that K has:
 *   - well-defined equality semantics (T::operator==(const T&) or equiv.);
 * Assumes that V has:
 *   - a default constructor.
 * Note that while these are automatically defined for builtin types
 * (such as "int", "double", or any pointer), you may need to provide
 * appropriate operators for your own classes, particularly those with
 * constructors and/or pointers to other objects.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvsldict.h#1 $
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
#include <rw/tislist.h>
#include <rw/tasslnk.h>

template <class K, class V> class  RWTValSlistDictionaryIterator;

/****************************************************************
 *                                                              *
 *      Declarations for RWTValSlistDictionary<K,V>             *
 *                                                              *
 ****************************************************************/


template <class K, class V>
class  RWTValSlistDictionary
    : private RWTIsvSlist< RWTValAssocLink<K, V> >
{

public:

    RWTValSlistDictionary() {
        ;
    }
    RWTValSlistDictionary(const RWTValSlistDictionary<K, V>&);
    ~RWTValSlistDictionary() {
        RWTIsvSlist<RWTValAssocLink<K, V> >::clearAndDestroy();
    }

    typedef RWTIsvSlist< RWTValAssocLink<K, V> > base_type;

    // Operators:
    RWTValSlistDictionary<K, V>&
    operator=(const RWTValSlistDictionary<K, V>&);

    // Look up key, add if not there:
    V&            operator[](const K& key);

    // Member functions:
    void          applyToKeyAndValue(void (*applyFun)(const K&, V& , void*), void*);

    void          clear() {
        RWTIsvSlist<RWTValAssocLink<K, V> >::clearAndDestroy();
    }

    bool     contains(const K&) const;       // Contain key?

    size_t        entries() const {
        return RWTIsvSlist<RWTValAssocLink<K, V> >::entries();
    }

    bool     isEmpty() const {
        return RWTIsvSlist<RWTValAssocLink<K, V> >::isEmpty();
    }

    bool     find(const K& key, K& retKey) const;
    bool     findValue(const K& key, V& retVal) const;
    bool     findKeyAndValue(const K& key, K& retKey, V& retVal) const;

    void          insertKeyAndValue(const K& key, const V& value) {
        (*this)[key] = value;
    }

    bool     remove(const K& key);
    bool     remove(const K& key, K& retKey);
    bool     removeKeyAndValue(const K& key, V& retVal);
    void     swap(RWTValSlistDictionary& lst) {
        RWTIsvSlist< RWTValAssocLink<K, V> >::swap(lst);
    }

protected:

    RWTValAssocLink<K, V>* findLink(const K&) const;
    RWTValAssocLink<K, V>* removeLink(const K&);

private:

    friend class  RWTValSlistDictionaryIterator<K, V>;

};

/****************************************************************
 *                                                              *
 *     Declarations for RWTValSlistDictionaryIterator<K,V>      *
 *                                                              *
 ****************************************************************/

template <class K, class V>
class  RWTValSlistDictionaryIterator :
    private RWTIsvSlistIterator<RWTValAssocLink<K, V> >
{

public:

    // Constructor:
    RWTValSlistDictionaryIterator(RWTValSlistDictionary<K, V>& d)
        : RWTIsvSlistIterator<RWTValAssocLink<K, V> >(d) {
        ;
    }

    RWTValSlistDictionaryIterator(const RWTValSlistDictionaryIterator<K, V>&);
    // Copy ctor added to address BUG#117

    // Operators:
    bool     operator++() {
        this->advance();
        return this->cursor() != container()->tailLink();
    }
    bool     operator+=(size_t n) {
        return RWTIsvSlistIterator<RWTValAssocLink<K, V> >::operator+=(n) != rwnil;
    }
    bool     operator()() {
        this->advance();
        return this->cursor() != container()->tailLink();
    }

    RWTValSlistDictionary<K, V>*   container() const {
        return (RWTValSlistDictionary<K, V>*)RWTIsvSlistIterator<RWTValAssocLink<K, V> >::container();
    }

    K             key() const {
        return this->cursor()->key_;
    }

    void          reset() {
        RWTIsvSlistIterator<RWTValAssocLink<K, V> >::reset();
    }
    void          reset(RWTValSlistDictionary<K, V>& s) {
        RWTIsvSlistIterator<RWTValAssocLink<K, V> >::reset(s);
    }

    V             value() const {
        return this->cursor()->value_;
    }

private:

    // Disallow postfix increment.  Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool             operator++(int);

};

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tvsldict.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tvsldict.cc"
#endif


#endif  /* RW_TOOLS_TVSLDICT_H */
