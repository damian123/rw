#ifndef  RW_TOOLS_QUEUECOL_H
#define  RW_TOOLS_QUEUECOL_H


/**********************************************************************
 *
 * Declarations for a Queue of RWCollectables,
 *   implemented as a Singly-linked list.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/queuecol.h#1 $
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
#include <rw/slistcol.h>

/**
 * @ingroup rwcollectable_classes
 * @brief  A restricted interface to class RWSlistCollectables
 * in order to implement a first in first out (FIFO) queue.
 *
 * Class RWSlistCollectablesQueue represents a restricted interface
 * to class RWSlistCollectables to implement a first in first out
 * (FIFO) queue.  A \b queue is a sequential list for which
 * all insertions are made at one end (the "tail"), and all removals
 * are made at the other end (the "head").  Hence, the ordering
 * is determined externally by the ordering of the insertions.
 * Duplicates are allowed.
 *
 * An object stored by RWSlistCollectablesQueue must inherit from abstract
 * base class RWCollectable.  You must use the virtual function
 * \link RWCollectable::isEqual() isEqual() \endlink (see
 * class RWCollectable) to find a match between a target
 * and an item in the queue.
 *
 * This class corresponds to the Smalltalk class \b \e Queue.
 *
 * @section synopsis Synopsis
 *
 * @code
 * // Smalltalk typedef:
 * typedef RWSlistCollectablesQueue Queue ;
 * #include <rw/queuecol.h>
 * RWSlistCollectablesQueue a;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWSlistCollectablesQueue : public RWSlistCollectables
{
private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWSlistCollectablesQueue);
    friend RWCollectable* rwCreateFN(RWSlistCollectablesQueue)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns \c __RWSLISTCOLLECTABLESQUEUE, i.e. a <i>class identifier</i>,
     * that identifies this object's class.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    // Documented in base class.
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWSlistCollectablesQueue*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWSlistCollectablesQueue*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWSlistCollectablesQueue& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWSlistCollectablesQueue& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWSlistCollectablesQueue& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWSlistCollectablesQueue& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Constructs an empty queue.
     */
    RWSlistCollectablesQueue();

    /**
     * Constructs a queue with single item \a a.
     */
    RWSlistCollectablesQueue(RWCollectable* a) : RWSlistCollectables(a) { }

    /**
     * Makes a shallow copy of \a queue.
     */
    RWSlistCollectablesQueue(const RWSlistCollectablesQueue& queue) :
        RWSlistCollectables(queue) { }

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed RWSlistCollectablesQueue takes
     * ownership of the data owned by \a queue.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWSlistCollectablesQueue(RWSlistCollectablesQueue && queue) :
        RWSlistCollectables(rw_move(queue)) { }

    /**
     * Move assignment. Self takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWSlistCollectablesQueue& operator=(RWSlistCollectablesQueue && rhs) {
        if (&rhs != this) {
            RWSlistCollectables::operator=(rw_move(rhs));
        }
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Assignment operator.  Makes a shallow copy of \a rhs.
     */
    RWSlistCollectablesQueue& operator=(const RWSlistCollectablesQueue& rhs) {
        if (&rhs != this) {
            RWSlistCollectables::operator=(rhs);
        }
        return *this;
    }

    // Documented in base class.
    virtual bool             isEqual(const RWCollectable*) const;

    /**
     * Calls get().  The argument is ignored.
     */
    virtual RWCollectable*        remove(const RWCollectable*) {
        return RWSlistCollectables::get();
    }

private:

    RWCollectable*        prepend(RWCollectable*) {
        return rwnil;
    }
    RWCollectable*        removeReference(const RWCollectable*) {
        return rwnil;
    }
    virtual void          removeAndDestroy(const RWCollectable*) {
        ;
    }
};


#endif /* RW_TOOLS_QUEUECOL_H */
