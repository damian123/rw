#ifndef RW_TOOLS_REF_H
#define RW_TOOLS_REF_H

/**********************************************************************
 *
 * Base class for reference counted objects.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/ref.h#1 $
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
#include <rw/tools/atomic.h>

class RW_TOOLS_SYMBOLIC RWReference
{

protected:

    /*
     * The internal reference count is one less than the actual reference
     * count. This allows a statically initialized RWReference instance to
     * start with an external count of one (internal count of 0)
     */
    RWTAtomic<unsigned long> refs_;

public:

    enum RWReferenceFlag {
        STATIC_INIT
    };

    RWReference(unsigned long n = 0)
        : refs_() {
        refs_.store(n - 1, rw_mem_order_relaxed);
    }

    RWReference(RWReferenceFlag) {
    }

    void  setRefCount(unsigned long n) {
        refs_.store(n - 1);
    }

    unsigned long  references() const {
        return refs_.load(rw_mem_order_acquire) + 1;
    }

    void addReference() {
        ++refs_;
    }

    unsigned long  removeReference() {
        return refs_--;
    }

    void unsafeAddReference() {
        addReference();
    }

    unsigned long  unsafeRemoveReference() {
        return removeReference();
    }

    unsigned long unsafeReferences() const {
        return refs_.load(rw_mem_order_relaxed) + 1;
    }

};


#endif  /* RW_TOOLS_REF_H */
