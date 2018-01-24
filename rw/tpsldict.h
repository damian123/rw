#ifndef RW_TOOLS_TPSLDICT_H
#define RW_TOOLS_TPSLDICT_H


/**********************************************************************
 *
 * RWTPtrSlistDictionary<KP,VP>: A dictionary of pointers to keys of type KP,
 *   and pointers to values of type VP, implemented as a singly-linked list.
 * Assumes that KP has:
 *   - well-defined equality semantics (equiv of KP::operator==(const KP&)).
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tpsldict.h#1 $
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

template <class KP, class VP> class  RWTPtrSlistDictionaryIterator;

/****************************************************************
 *                                                              *
 *      Declarations for RWTPtrSlistDictionary<KP,VP>           *
 *                                                              *
 ****************************************************************/


template <class KP, class VP>
class  RWTPtrSlistDictionary
    : private RWTIsvSlist< RWTPtrAssocLink<KP, VP> >
{

public:

    RWTPtrSlistDictionary() {
        ;
    }
    RWTPtrSlistDictionary(const RWTPtrSlistDictionary<KP, VP>&);
    ~RWTPtrSlistDictionary() {
        RWTIsvSlist<RWTPtrAssocLink<KP, VP> >::clearAndDestroy();
    }

    typedef RWTIsvSlist< RWTPtrAssocLink<KP, VP> > base_type;

    // Operators:
    RWTPtrSlistDictionary<KP, VP>&
    operator=(const RWTPtrSlistDictionary<KP, VP>&);
    VP*&                  operator[](KP* key); // Look up key, return value

    // Member functions:
    void                  applyToKeyAndValue(void (*applyFun)(KP*, VP*&, void*), void*);

    void                  clear() {
        RWTIsvSlist<RWTPtrAssocLink<KP, VP> >::clearAndDestroy();
    }

    void                  clearAndDestroy();

    bool             contains(const KP*) const;      // Contain key?

    size_t                entries() const {
        return RWTIsvSlist<RWTPtrAssocLink<KP, VP> >::entries();
    }

    bool             isEmpty() const {
        return RWTIsvSlist<RWTPtrAssocLink<KP, VP> >::isEmpty();
    }

    KP*                   find(const KP* key) const;
    KP*                   findKeyAndValue(const KP* key, VP*& retVal) const;
    VP*                   findValue(const KP* key) const;

    void                  insertKeyAndValue(KP* key, VP* value) {
        (*this)[key] = value;
    }

    KP*                   remove(const KP* key);
    KP*                   removeKeyAndValue(const KP* key, VP*& retVal);
    void                  swap(RWTPtrSlistDictionary& lst) {
        RWTIsvSlist< RWTPtrAssocLink<KP, VP> >::swap(lst);
    }

protected:

    RWTPtrAssocLink<KP, VP >*      findLink(const KP*) const;
    RWTPtrAssocLink<KP, VP >*      removeLink(const KP*);

private:

    friend class  RWTPtrSlistDictionaryIterator<KP, VP>;

};


/****************************************************************
 *                                                              *
 *     Declarations for RWTPtrSlistDictionaryIterator<KP,VP>    *
 *                                                              *
 ****************************************************************/

template <class KP, class VP>
class  RWTPtrSlistDictionaryIterator :
    private RWTIsvSlistIterator<RWTPtrAssocLink<KP, VP> >
{

public:

    // Constructor:
    RWTPtrSlistDictionaryIterator(RWTPtrSlistDictionary<KP, VP>& d)
        : RWTIsvSlistIterator<RWTPtrAssocLink<KP, VP> >(d) {
        ;
    }

    RWTPtrSlistDictionaryIterator(const RWTPtrSlistDictionaryIterator<KP, VP>&);
    // Added to address BUG#117

    // Operators:
    bool     operator++() {
        this->advance();
        return this->cursor() != container()->tailLink();
    }

    bool     operator+=(size_t n) {
        return RWTIsvSlistIterator<RWTPtrAssocLink<KP, VP> >::operator+=(n) != rwnil;
    }

    KP*           operator()() {
        this->advance();
        return this->cursor() == container()->tailLink() ?
               rwnil : this->cursor()->key_;
    }

    RWTPtrSlistDictionary<KP, VP>* container() const {
        return (RWTPtrSlistDictionary<KP, VP>*)RWTIsvSlistIterator<RWTPtrAssocLink<KP, VP> >::container();
    }

    KP*           key() const {
        return this->cursor()->key_;
    }

    void          reset() {
        RWTIsvSlistIterator<RWTPtrAssocLink<KP, VP> >::reset();
    }
    void          reset(RWTPtrSlistDictionary<KP, VP>& s) {
        RWTIsvSlistIterator<RWTPtrAssocLink<KP, VP> >::reset(s);
    }

    VP*           value() const {
        return this->cursor()->value_;
    }

private:

    // Disallow postfix increment.  Unless we hide it, some compilers will
    // substitute the prefix increment operator in its place.
    bool             operator++(int);

};

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tpsldict.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tpsldict.cc"
#endif


#endif  /* RW_TOOLS_TPSLDICT_H */
