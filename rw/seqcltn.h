#ifndef RW_TOOLS_SEQCLTN_H
#define RW_TOOLS_SEQCLTN_H


/**********************************************************************
 *
 * Abstract base class for sequenceable collections.
 * This is an abstract base class, inherited by collections whose
 * elements are ordered and accessible by an index.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/seqcltn.h#1 $
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
#include <rw/colclass.h>


/**
 * @ingroup rwcollectable_classes
 *
 * @brief Abstract base class for collections
 * that can be accessed by an index
 *
 * Class RWSequenceable is an abstract base class for collections
 * that can be accessed by an index. It inherits class RWCollection
 * as a public base class and adds a few extra virtual functions.
 * This documentation only describes these extra functions.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/seqcltn.h>
 * typedef RWSequenceable SequenceableCollection;
 *               // Smalltalk typedef
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWSequenceable : public RWCollection
{
public:
    virtual void* self();
    virtual const void* self() const;

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream&, RWSequenceable*&);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile&, RWSequenceable*&);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWSequenceable& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWSequenceable& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWSequenceable& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWSequenceable& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:
    virtual ~RWSequenceable();

    // Documented in base class.
    virtual void apply(RWapplyCollectable, void*) = 0;

    // Documented in base class.
    virtual void clear() = 0;

    // Documented in base class.
    virtual size_t entries() const = 0;

    // Documented in base class.
    virtual RWCollectable* find(const RWCollectable*) const = 0;

    // Documented in base class.
    virtual RWCollectable* insert(RWCollectable*) = 0;

    // Documented in base class.
    virtual bool isEmpty() const = 0;

    // Documented in base class.
    virtual size_t occurrencesOf(const RWCollectable*) const = 0;

    // Documented in base class.
    virtual RWCollectable* remove(const RWCollectable*) = 0;

public:

    /**
     * Adds \a a to the end of the collection and returns it.  Returns
     * #rwnil if the insertion was unsuccessful.
     */
    virtual RWCollectable* append(RWCollectable* a) = 0;

    /**
     * Allows access to the \a i <sup>th</sup> element of the collection.
     * The return value can be used as an lvalue. The index
     * \a i must be between zero and the number of items in the collection
     * less one, or the function throws an exception of type RWBoundsErr.
     */
    virtual RWCollectable*& at(size_t i) = 0; // Some collections can use as lvalue

    /**
     * Allows access to the \a i <sup>th</sup> element of the collection.
     * The return value cannot be used as an lvalue.  The index
     * \a i must be between zero and the number of items in the collection
     * less one, or an exception of type RWBoundsErr will be thrown.
     */
    virtual const RWCollectable* at(size_t i) const = 0; // Cannot use as lvalue

    /**
     * Returns the first item in the collection.
     */
    virtual RWCollectable* first() const = 0;

    /**
     * Returns the index number of the first item that "matches" the
     * item pointed to by \a c.  If there is no such item, returns
     * #RW_NPOS.  For most collections, an item "matches" the target
     * if either isEqual() or compareTo() find equivalence, whichever
     * is appropriate for the actual collection type.
     */
    virtual size_t index(const RWCollectable* c) const = 0;

    /**
     * Adds a new item to the collection at position \a indx.  The item
     * previously at position \c i is moved to \c i+1, etc.  The index
     * \a indx must be between \c 0 and the number of items in the collection,
     * or an exception of type RWBoundsErr will be thrown.
     */
    virtual RWCollectable* insertAt(size_t indx, RWCollectable* e) = 0;

    /**
     * Returns the last item in the collection.
     */
    virtual RWCollectable* last() const = 0;

    /**
     * Adds \a c to the beginning of the collection and returns
     * it.  Returns #rwnil if the insertion was unsuccessful.
     */
    virtual RWCollectable* prepend(RWCollectable* c) = 0;

    // For backward compatibility.
    virtual RWCollectable* insertAfter(int, RWCollectable*) = 0;
};


#endif /* RW_TOOLS_SEQCLTN_H */
