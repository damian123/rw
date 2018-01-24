#ifndef RW_TOOLS_HANDLE_BODY
#define RW_TOOLS_HANDLE_BODY

/**********************************************************************
 *
 * Declaration for RWHandle and RWBody
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/handlebody.h#1 $
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
#include <rw/edefs.h> // for rw_swap
#include <rw/coreerr.h>
#include <rw/rwerr.h>
#include <rw/tools/atomic.h>

// Forward declare handle class.
class RW_TOOLS_GLOBAL RWHandle;

/*
 * Base class for body classes. Each time an RWHandleBase instance
 * is bound to an instance of this class, the handle instance
 * increments the reference count inherited by the body instance.
 * Each time a handle instance detaches from an instance of this
 * class, the handle decrements the reference count. If the
 * reference count reaches zero, the handle deletes the body
 * instance. The reference counting relieves clients of the burden
 * of having to keep track of when it is safe to delete a body
 * instance.
 */

class RW_TOOLS_GLOBAL RWBody
{
    friend class RWHandle;

protected:

    enum StaticCtor { staticCtor };

    /*
     * Default constructor. Constructs a default body instance
     * (reference count starts at 0).
     */
    RWBody()
        : count_() {
    }

    /*
     * Constructs a static body instance. Uses a non-initializing ctor.
     */
    RWBody(StaticCtor) {
    }

    /*
     * Copy constructor.
     */
    RWBody(RWBody const&)
        : count_() {
    }

    /*
     * Assignment operator. Throws no exceptions.
     */
    RWBody&
    operator= (RWBody const&) {
        return *this;
    }

    /*
     * Virtual destructor. Throws no exceptions.
     */
    virtual
    ~RWBody() {
    }

    /*
     * @internal
     * Adds one to the reference count, and returns the previous
     * value (minus 1).  This class is used internally by the
     * RWHandle class to maintain the count of the number of
     * handles.
     */
    unsigned
    addReference() {
        return count_++;
    }

    /*
     * @internal
     * Subtracts one from the reference count, and returns the
     * previous value (minus 1). This class is used internally
     * by the RWHandle class to maintain the count of the number
     * of handles.
     */
    unsigned
    removeReference() {
        return count_--;
    }

private:

    RWTAtomic<unsigned> count_;

};

/*
 * Base class for handle classes. Each time an instance of this
 * class is bound to an instance of the RWBodyBase class, it
 * increments the reference count maintained by that body. Each
 * time an instance of this class detaches from a body instance,
 * it decrements the body's reference count, until the last handle
 * detaches, reaching zero, whereupon it deletes the body instance.
 * The reference counting relieves clients of the burden of keeping
 * track of when it is safe to delete a body instance.
 *
 * When a handle is copy-constructed, the new handle attaches to
 * the same body as the original. When one handle is assigned to
 * another handle, the handle on the left side detaches from its
 * current body and attaches to the body pointed to by the handle
 * on the right side.
 */
class RW_TOOLS_GLOBAL RWHandle
{

public:

    enum StaticCtor { staticCtor };

    /*
     * Returns true if the handle points to the same body instance,
     * if any, as pointed to by a second handle, and false otherwise.
     * Two empty handles are equal. Throws no exceptions.
     */
    friend bool
    operator== (RWHandle const&, RWHandle const&);

    /*
     * Returns true if the handle points to different body instance,
     * if any, than pointed to by a second handle, and otherwise false.
     * Throws no exceptions.
     */
    friend bool
    operator!= (RWHandle const&, RWHandle const&);

    /*
     * Returns true if this handle points to a valid body instance,
     * and false otherwise. Throws no exceptions.
     */
    bool
    isValid() const {
        return bodyAddress() != 0;
    }

    void
    swap(RWHandle& rhs) {
        rw_swap(body_, rhs.body_);
    }

protected:

    /*
     * Default constructor. Constructs an empty, invalid, handle
     * instance. Throws no exceptions.
     */
    RWHandle()
        : body_(0) {
    }

    /*
     * Constructs a global static handle instance (may be used prior
     * to construction). Throws no exceptions.
     */
    RWHandle(StaticCtor) {
    }

    /*
     * Binds a new handle instance to a body instance.
     */
    RWHandle(RWBody* ptr)
        : body_(ptr) {
        if (0 != body_) {
            body_->addReference();
        }
    }

    /*
     * Binds a new handle instance to the same body instance, if any,
     * that is pointed to by a second handle instance.
     */
    RWHandle(RWHandle const& second)
        : body_(second.body_) {
        if (0 != body_) {
            body_->addReference();
        }
    }

    /*
     * Virtual destructor. Throws no exceptions.
     */
    virtual
    ~RWHandle() {
        // If our body pointer is not null, decrement the handle
        // count in the body.  If we are the last handle, delete
        // the body.
        if (body_ && 1 == body_->removeReference()) {
            delete body_;
        }
    }

    /*
     * Binds this handle instance to the same body instance, if any,
     * that is pointed to by a second handle instance.
     */
    RWHandle&
    operator= (RWHandle const& second) {
        return operator= (second.body_);
    }

    /*
     * Binds this handle instance to body.
     */
    RWHandle&
    operator= (RWBody* ptr) {
        if (body_ == ptr) {
            return *this;
        }

        if (0 != ptr) {
            ptr->addReference();
        }

        if (body_ && 1 == body_->removeReference()) {
            delete body_;
        }

        body_ = ptr;

        return *this;
    }

    /*
     * Gets a reference for the body instance, if any; otherwise
     * throws an RWExternalErr exception.
     */
    RWBody&
    body() const {
        if (0 == body_) {
            RWTHROW(RWExternalErr(RWMessage(RWCORE_NULLPTR)));
        }

        return (*body_);
    }

    /*
     * Gets a reference for the body instance, if any; otherwise
     * throws an RWExternalErr exception.
     */
    RWBody&
    bodyReference() {
        if (0 == bodyAddress()) {
            RWTHROW(RWExternalErr(RWMessage(RWCORE_NULLPTR)));
        }

        return (*body_);
    }

    RWBody const&
    bodyReference() const {
        if (0 == bodyAddress()) {
            RWTHROW(RWExternalErr(RWMessage(RWCORE_NULLPTR)));
        }

        return (*body_);
    }

    /*
     * Returns the managed pointer.
     */
    RWBody*
    bodyAddress() {
        return body_;
    }

    RWBody const*
    bodyAddress() const {
        return body_;
    }

private:

    RWBody* body_;

};

/* friend */
inline bool
operator== (RWHandle const& lhs, RWHandle const& rhs)
{
    return lhs.bodyAddress() == rhs.bodyAddress();
}

/* friend */ inline bool
operator!= (RWHandle const& lhs, RWHandle const& rhs)
{
    return !(lhs == rhs);
}


#endif // RW_TOOLS_HANDLE_BODY
