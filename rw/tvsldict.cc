

/**********************************************************************
 *
 * Template definitions for RWTValSlistDictionary<K,V>
 *   and RWTValSlistDictionaryIterator<K,V>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvsldict.cc#1 $
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


// Copy constructor (some of these names get pretty darn long, eh?)
template <class K, class V>
RWTValSlistDictionary<K, V>::RWTValSlistDictionary(const RWTValSlistDictionary<K, V>& d) :
    base_type()
{
    // Cast away "constness", which we will honor anyway...
    RWTValSlistDictionaryIterator<K, V> next((RWTValSlistDictionary<K, V>&)d);

    while (++next) {
        insertKeyAndValue(next.key(), next.value());
    }

    RW_POSTCONDITION(entries() == d.entries());
}

template <class K, class V> RWTValSlistDictionary<K, V>&
RWTValSlistDictionary<K, V>::operator=(const RWTValSlistDictionary<K, V>& d)
{
    if (this != &d) {

        clear();

        // Cast away "constness", which we will honor anyway...
        RWTValSlistDictionaryIterator<K, V> next((RWTValSlistDictionary<K, V>&)d);

        while (++next) {
            insertKeyAndValue(next.key(), next.value());
        }

    }
    RW_POSTCONDITION(entries() == d.entries());
    return *this;
}

template <class K, class V> V&
RWTValSlistDictionary<K, V>::operator[](const K& key)
{
    RWTValAssocLink<K, V>* assoc = findLink(key);

    if (assoc == rwnil)
        RWTIsvSlist<RWTValAssocLink<K, V> >
        ::insert(assoc = new RWTValAssocLink<K, V>(key));
    RW_POSTCONDITION(key == assoc->key_);
    return assoc->value_;
}

template <class K, class V> void
RWTValSlistDictionary<K, V>::applyToKeyAndValue(void (*applyFun)(const K&, V&, void*), void* a)
{
    RWTValAssocLink<K, V>* link = this->firstLink();
    while (link != this->tailLink()) {
        applyFun(link->key_, link->value_, a);
        link = link->next();
    }
}

template <class K, class V> bool
RWTValSlistDictionary<K, V>::contains(const K& key) const
{
    return findLink(key) != rwnil;
}

template <class K, class V> bool
RWTValSlistDictionary<K, V>::find(const K& key, K& retKey) const
{
    RWTValAssocLink<K, V>* assoc = findLink(key);
    if (assoc) {
        retKey   = assoc->key_;
        RW_POSTCONDITION(retKey == key);
        return true;
    }
    return false;
}

template <class K, class V> bool
RWTValSlistDictionary<K, V>::findKeyAndValue(const K& key, K& retKey, V& retValue) const
{
    RWTValAssocLink<K, V>* assoc = findLink(key);
    if (assoc) {
        retKey   = assoc->key_;
        retValue = assoc->value_;
        RW_POSTCONDITION(retKey == key);
        return true;
    }
    return false;
}

template <class K, class V> bool
RWTValSlistDictionary<K, V>::findValue(const K& key, V& retValue) const
{
    RWTValAssocLink<K, V>* assoc = findLink(key);
    if (assoc) {
        retValue = assoc->value_;
        RW_POSTCONDITION(assoc->key_ == key);
        return true;
    }
    return false;
}

template <class K, class V> bool
RWTValSlistDictionary<K, V>::remove(const K& key)
{
    RWTValAssocLink<K, V>* assoc = removeLink(key);
    return assoc ? (delete assoc, true) : false;
}

template <class K, class V> bool
RWTValSlistDictionary<K, V>::remove(const K& key, K& retKey)
{
    RWTValAssocLink<K, V>* assoc = removeLink(key);
    if (assoc) {
        retKey = assoc->key_;
        RW_POSTCONDITION(retKey == key);
        delete assoc;
        return true;
    }
    RW_POSTCONDITION(assoc == rwnil);
    return false;
}

template <class K, class V> bool
RWTValSlistDictionary<K, V>::removeKeyAndValue(const K& key, V& retVal)
{
    RWTValAssocLink<K, V>* assoc = removeLink(key);
    if (assoc) {
        retVal = assoc->value_;
        RW_POSTCONDITION(assoc->key_ == key);
        delete assoc;
        return true;
    }
    RW_POSTCONDITION(assoc == rwnil);
    return false;
}


// Protected functions ---

template <class K, class V> RWTValAssocLink<K, V>*
RWTValSlistDictionary<K, V>::findLink(const K& key) const
{
    RWTValAssocLink<K, V>* link = this->firstLink();
    while (link != this->tailLink()) {
        if (link->key_ == key) {
            return link;
        }
        link = link->next();
    }
    return rwnil;
}

template <class K, class V> RWTValAssocLink<K, V>*
RWTValSlistDictionary<K, V>::removeLink(const K& key)
{
    RW_PRECONDITION(this->lastLink()->next() == this->tailLink());

    // Set up a sentinel:
    RWTValAssocLink<K, V> newTail(key);
    this->lastLink()->next_ = &newTail;

    // Now search for it:
    RWTValAssocLink<K, V>* link = this->headLink();
    while (!(link->next()->key_ == key)) {
        link = link->next();
    }

    // Reset the pointer in the last link:
    this->lastLink()->next_ = this->tailLink();

    if (link != this->lastLink()) {
        RW_POSTCONDITION(link->next()->key_ == key);
        //return removeRight(link);
        return (RWTValAssocLink<K, V>*) this->removeRight(link);
    }
    return rwnil;
}

template <class K, class V>
RWTValSlistDictionaryIterator<K, V>::RWTValSlistDictionaryIterator
(const RWTValSlistDictionaryIterator<K, V>& sdi)
    : RWTIsvSlistIterator<RWTValAssocLink<K, V> >(sdi)
{
    ;
}
