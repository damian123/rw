#ifndef RW_TOOLS_VREF_H
#define RW_TOOLS_VREF_H



/**********************************************************************
 *
 * RWVirtualRef: Reference counted virtual array reference.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/vref.h#1 $
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


#include <rw/ref.h>

#if !defined(RW_DISABLE_DEPRECATED)
RW_SUPPRESS_DEPRECATED_WARNINGS

#include <rw/defs.h>
#include <rw/vpage.h>
#include <rw/tools/pointer.h>
#include <stdlib.h>

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

typedef unsigned RWPageSlot;

class RW_TOOLS_GLOBAL RWVirtualRef : public RWReference
{

public:

    RWVirtualRef(long length, size_t elementSize, RWVirtualPageHeap* h);
    RWVirtualRef(const RWVirtualRef&);    // Deep copy
    virtual ~RWVirtualRef();

    RWssize_t               capacity() const {
        return nSlots_ * nPerPage_;
    }
    RWVirtualPageHeap*    heap() const {
        return myHeap_;
    }
    long                  length() const {
        return length_;
    }
    void                  slide(long start, long delta);
    void                  setSlice(long start1, long extent1,
                                   RWVirtualRef& v, long start2, long extent2);
    void                  reshape(long newLength);

public:

    // The following three functions are formally intended to be
    // protected, but to avoid many friend functions have been made part
    // of the public interface.

    void                  findLocation(long, RWPageSlot&, unsigned&) const;
    void*                 lock(RWPageSlot p) {
        return myHeap_->lock(handles_[p]);
    }
    void                  unlock(RWPageSlot p) {
        myHeap_->unlock(handles_[p]);
    }

protected:

    // Neither augmentLeft nor augmentRight change the vector length.
    // They just add capacity to one side or the other.
    void                  augmentLeft(RWssize_t);      // Add capacity on left
    void                  augmentRight(RWssize_t);     // Add capacity on right
    void                  init();
    unsigned              pages() {
        return nSlots_;
    }
    unsigned              pageSize() {
        return myHeap_->pageSize();
    }
    void                  dirty(RWPageSlot p) {
        myHeap_->dirty(handles_[p]);
    }

    // Supplied by specializing class:
    virtual void          conformalCopy(long start1, RWVirtualRef& v2, long start2, long N) = 0;

protected:

    // Protected data.
    // Note that the length of the vector handles_ is nSlots_.

    long length_;                         // Total number of elements
    RWssize_t baseIndex_;                 // Start of actual data
    RWssize_t nPerPage_;                  // Number of elements per page
    RWVirtualPageHeap* myHeap_;           // The heap I'm getting my pages from
    RWTScopedPointer<size_t[]> handles_;  // Array of handles
    RWPageSlot nSlots_;                   // Length of handles_.

#ifdef RW_MULTI_THREAD
public:
    void addReference() {
        RWReference::addReference();
    }
    unsigned removeReference() {
        return unsigned(RWReference::removeReference());
    }
#endif

};

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

RW_RESTORE_DEPRECATED_WARNINGS
#endif

#endif  /* RW_TOOLS_VREF_H */
