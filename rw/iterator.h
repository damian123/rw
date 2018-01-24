#ifndef RW_TOOLS_ITERATOR_H
#define RW_TOOLS_ITERATOR_H


/**********************************************************************
 *
 * Abstract base class for Iterator classes.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/iterator.h#1 $
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

class RW_TOOLS_GLOBAL RWCollectable;

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Contains virtual functions for positioning and
 * resetting the iterator
 *
 * Class RWIterator is an abstract base class for iterators used
 * by the Smalltalk-like collection classes. The class contains
 * virtual functions for positioning and resetting the iterator.
 * They are all <i>pure virtual</i> functions, meaning that deriving
 * classes must supply a definition. The descriptions below are
 * intended to be generic  --  all inheriting iterators generally
 * follow the described pattern.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/iterator.h>
 * (Abstract base class)
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWIterator
{
public:
    virtual ~RWIterator() {}

    /**
     * Moves the iterator forward to the next item which "matches" the
     * object pointed to by \a target and returns it or #rwnil if no item
     * was found. For most collections, an item "matches" the target
     * if either RWCollectable::isEqual() or RWCollectable::compareTo()
     * indicate equivalence, whichever is appropriate for the actual
     * collection type. However, when an iterator is used with an
     * "identity collection" (i.e., RWIdentitySet and RWIdentityDictionary),
     * it looks for an item with the same address (i.e., "is identical to").
     */
    virtual RWCollectable*  findNext(const RWCollectable* target) = 0; //next match

    /**
     * Returns the item at the current iterator position.
     */
    virtual RWCollectable*  key() const = 0;            //Return current key

    /**
     * Advances the iterator and returns the next item, or #rwnil
     * if the end of the collection has been reached.
     */
    virtual RWCollectable*  operator()() = 0;           //Advance iterator

    /**
     * Resets the iterator to the state it had immediately after construction.
     */
    virtual void            reset() = 0;
};

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Contains virtual functions for positioning and
 * resetting the \c const iterator
 *
 * Class RWConstIterator is an abstract base class for \c const
 * iterators used by the Smalltalk-like collection classes. The
 * \c const iterators do not change any information in the data
 * structure upon which they iterate.
 *
 * The class contains virtual functions for positioning  and resetting
 * the iterator. They are all <i>pure virtual</i> functions, meaning
 * that deriving classes must supply a definition. The descriptions
 * below are intended to be generic  --  all inheriting iterators
 * generally follow the described pattern.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/iterator.h>
 * (Abstract base class)
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWConstIterator
{
public:
    virtual ~RWConstIterator() {}

    /**
     * Moves the iterator forward to the next item which "matches" the
     * object pointed to by \a target and returns it or #rwnil if no item
     * was found. For most collections, an item "matches" the target
     * if either RWCollectable::isEqual() or RWCollectable::compareTo()
     * indicates equivalence, whichever is appropriate for the actual
     * collection type. However, when an iterator is used with an
     * "identity collection" (i.e., RWIdentitySet and RWIdentityDictionary),
     * it looks for an item with the same address (i.e., "is identical to").
     */
    virtual const RWCollectable*  findNext(const RWCollectable* target) = 0;

    /**
     * Returns the item at the current iterator position.
     */
    virtual const RWCollectable*  key() const = 0;

    /**
     * Advances the iterator and returns the next item, or #rwnil if
     * the end of the collection has been reached.
     */
    virtual const RWCollectable*  operator()() = 0;

    /**
     * Resets the iterator to the state it had immediately after construction.
     */
    virtual void                  reset() = 0;
};

#endif /* RW_TOOLS_ITERATOR_H */
