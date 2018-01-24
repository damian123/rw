#ifndef  RW_TOOLS_STACKCOL_H
#define  RW_TOOLS_STACKCOL_H



/**********************************************************************
 *
 * Declarations for a Stack of RWCollectables,
 *   implemented as a Singly-linked list.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/stackcol.h#1 $
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


/*
 * Restricted interface to class RWSlistCollectables to implement a
 * last in first out (LIFO) stack.
 */

#include <rw/defs.h>
#include <rw/slistcol.h>

/**
 * @ingroup rwcollectable_classes
 * @brief A restricted interface to class RWSlistCollectables to
 * implement a last in first out (LIFO) stack.
 *
 * Class RWSlistCollectablesStack represents a restricted interface
 * to class RWSlistCollectables to implement a last in first out
 * (LIFO) stack.  A stack is a sequential list for which all insertions
 * and deletions are made at one end (the beginning of the list).
 * Hence, the ordering is determined externally by the ordering
 * of the insertions.  Duplicates are allowed.
 *
 * An object stored by RWSlistCollectablesStack must inherit from abstract
 * base class RWCollectable. You must use the virtual function isEqual()
 * (see class RWCollectable) to find a match between a target
 * and an item in the stack.
 *
 * This class corresponds to the Smalltalk class \b \e Stack.
 *
 * @section synopsis Synopsis
 *
 * @code
 * // Smalltalk typedef:
 * typedef RWSlistCollectablesStack Stack;
 * #include <rw/stackcol.h>
 * RWSlistCollectablesStack a;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWSlistCollectablesStack : public RWSlistCollectables
{

private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWSlistCollectablesStack);
    friend RWCollectable* rwCreateFN(RWSlistCollectablesStack)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns \c __RWSLISTCOLLECTABLESSTACK, i.e. a <i>class identifier</i>,
       * that identifies this object's class.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    // Documented in base class.
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWSlistCollectablesStack*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWSlistCollectablesStack*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWSlistCollectablesStack& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWSlistCollectablesStack& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWSlistCollectablesStack& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWSlistCollectablesStack& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Constructs an empty stack.
     */
    RWSlistCollectablesStack();

    /**
     * Constructs a stack with one entry \a a.
     */
    RWSlistCollectablesStack(RWCollectable* a) : RWSlistCollectables(a) { }

    /**
     * Makes a shallow copy of \a stk.
     */
    RWSlistCollectablesStack(const RWSlistCollectablesStack& stk) :
        RWSlistCollectables(stk) { }

#  if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed RWSlistCollectablesStack takes
     * ownership of the data owned by \a stk.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWSlistCollectablesStack(RWSlistCollectablesStack && stk) :
        RWSlistCollectables(rw_move(stk)) { }

    /**
     * Move assignment. Self takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWSlistCollectablesStack& operator=(RWSlistCollectablesStack && rhs) {
        if (&rhs != this) {
            RWSlistCollectables::operator=(rw_move(rhs));
        }
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Assignment operator.  Makes a shallow copy of \a rhs.
     */
    RWSlistCollectablesStack& operator=(const RWSlistCollectablesStack& rhs) {
        if (&rhs != this) {
            RWSlistCollectables::operator=(rhs);
        }
        return *this;
    }

    /**
     * Same as push().
     */
    virtual RWCollectable*        insert(RWCollectable* a) {      // Same as push()
        return prepend(a);
    }

    //virtual bool             isEmpty() const

    // Documented in base class.
    virtual bool             isEqual(const RWCollectable*) const;

    // Documented in base class.
    virtual void                  restoreGuts(RWvistream&);

    // Documented in base class.
    virtual void                  restoreGuts(RWFile&);

    /**
     * Calls pop().  The argument is ignored.
     */
    virtual RWCollectable*        remove(const RWCollectable*) { // Treated as pop(); argument ignored
        return RWSlistCollectables::get();
    }

    /**
     * Adds an item to the top of the stack.
     */
    void                          push(RWCollectable* a) {
        RWSlistCollectables::prepend(a);
    }

    /**
     * Removes and returns the item at the top of the stack, or returns
     * #rwnil if the stack is empty.
     */
    RWCollectable*                pop() {
        return RWSlistCollectables::get();
    }

    /**
     * Returns the item at the top of the stack or returns \c nil if the stack
     * is empty.
     */
    RWCollectable*                top() const {
        return RWSlistCollectables::first();
    }

private:

    /******************** Disallowed functions ************************/
    virtual RWCollectable*        append(RWCollectable*) {
        return rwnil;
    }
    RWCollectable*                removeReference(const RWCollectable*) {
        return rwnil;
    }
    virtual void                  removeAndDestroy(const RWCollectable*) {
        ;
    }
};


#endif /* RW_TOOLS_STACKCOL_H */
