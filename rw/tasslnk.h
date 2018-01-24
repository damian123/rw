#ifndef RW_TOOLS_TVASSLNK_H
#define RW_TOOLS_TVASSLNK_H


/**********************************************************************
 *
 * RWTValAssocLink: Key / Value association link using values
 * RWTPtrAssocLink: Key / Value association link using pointers
 * Class RWTValAssocLink<K,V> defines an association between a key of
 * type K, and a value of type V in a singly-linked link, using value
 * semantics.
 * It has a single value constructor that takes just the key.
 * This means that the value will be constructed using the default
 * constructor for type V.  Usually this works just fine.  However, if the
 * value (type V) is a builtin, then its value will be left undefined.
 * Usually this also works fine.  However, if this is unsatisfactory,
 * then you can supply your own definition that overrides the template-
 * generated definition.  For an explanation of user-specified overrides
 * of template-generated definitions, see Stroustrup II, sec. 8.4.1.
 * Example:
 *   RWTValAssocLink<int,double>::RWTValAssocLink(int i) :
 *     key_(i)
 *    {
 *      value_ = 0.0;   // Explicitly set the value to zero.
 *    }
 * Class RWTPtrAssocLink<K,V> defines an association between a pointer to
 * a key of type K and a pointer to a value of type V in a singly-linked
 * link.
 * It also has a single value constructor which takes just a pointer to
 * a key.  It always sets the value pointer to nil:
 *   RWTPtrAssocLink<int,double>::RWTPtrAssocLink(int* i) :
 *     key_(i),
 *     value_(rwnil)
 *    {
 *    }
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tasslnk.h#1 $
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


#ifndef RW_TOOLS_ISLIST_H
#  include "rw/islist.h"
#endif


template <class K, class V> struct  RWTValAssocLink : public RWIsvSlink {
    K             key_;
    V             value_;
    RWTValAssocLink(const K& key);
    RWTValAssocLink(const K& key, const V& value) : key_(key), value_(value) { }

    RWTValAssocLink<K, V>* next() const {
        return (RWTValAssocLink<K, V>*)next_;
    }

};

template <class K, class V> struct  RWTPtrAssocLink : public RWIsvSlink {
    K*            key_;
    V*            value_;
    RWTPtrAssocLink(K* key) : key_(key), value_(rwnil) {
        ;
    }
    RWTPtrAssocLink(K* key, V* value) : key_(key), value_(value) {
        ;
    }

    RWTPtrAssocLink<K, V>* next() const {
        return (RWTPtrAssocLink<K, V>*)next_;
    }

};

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tasslnk.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tasslnk.cc"
#endif


#endif  /* RW_TOOLS_TVASSLNK_H */
