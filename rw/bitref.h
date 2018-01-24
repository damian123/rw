

/**********************************************************************
 *
 * A reference to a bit in a bit vector.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/bitref.h#1 $
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
 * Although this class has two public constructors, it is not intended
 * to be used by itself, but rather as an lvalue to the BitVec
 * and GBitVec(sz) classes.  Do not "construct" a RWBitRef directly.
 * The constructor must be public to accommodate GBitVec(sz) whose name
 * is not known in advance, hence it can't be made a friendly class.
 */

#ifndef RW_TOOLS_BITREF_H
#define RW_TOOLS_BITREF_H

#include <rw/defs.h>
#include <rw/edefs.h> // for rw_swap

#include <rw/mempool.h>

class RW_TOOLS_SYMBOLIC RWBitVec;

/**
 * @ingroup traditional_collection_classes
 *
 * @brief Represents an l-value to the RWBitVec and RWTBitVec classes.
 *
 * Class RWBitRef is used as an l-value to the RWBitVec and \link RWTBitVec
 * RWTBitVec<N> \endlink classes. RWBitRef instances should \e not be
 * constructed directly.
 */
class RW_TOOLS_SYMBOLIC RWBitRef   RWMemoryPool_OPTION
{
public:

    RWBitRef(RWByte* p, size_t j) {
        pt_ = (j >> 3) + p;
        msk_ = (RWByte)(((RWByte)1) << (7 & j));
    }

    RWBitRef(const RWBitRef& br) {
        pt_ = br.pt_;
        msk_ = br.msk_;
    }

    /**
     * Returns the value of the bit.
     */
    operator      bool() const {
        return ((*pt_ & msk_) != 0);
    }

    /**
     * Set the bit to the value \a i
     */
    bool     operator=(bool i) {
        if (i) {
            *pt_ |= msk_;
        }
        else {
            *pt_ &= ~msk_;
        }
        return i;
    }

    RWBitRef&     operator=(const RWBitRef& br) {
        if (!(this == &br)) {
            *this = (bool)br;
        }
        return *this;
    }

    /**
     * Logical assignment operator.
     */
    void operator&=(bool i) {
        if (!i) {
            *pt_ &= ~msk_;
        }
    }

    /**
     * @copydoc operator&=(bool)
     */
    void operator|=(bool i) {
        if (i) {
            *pt_ |=  msk_;
        }
    }

    /**
     * @copydoc operator&=(bool)
     */
    void operator^=(bool i) {
        if (i) {
            *pt_ ^=  msk_;
        }
    }

    /**
     * Swaps the data owned by self with the data owned by \a br.
     */
    void swap(RWBitRef& br) {
        rw_swap(pt_, br.pt_);
        rw_swap(msk_, br.msk_);
    }

private:

    RWByte*       pt_;    // Pointer to the referenced byte
    RWByte        msk_;   // Mask for the bit within the byte

    friend class RW_TOOLS_SYMBOLIC RWBitVec;

};

// Useful prototypes for fn used in various bit vector routines
RW_TOOLS_SYMBOLIC void   rwOR(RWByte*, const RWByte*, size_t);
RW_TOOLS_SYMBOLIC void   rwXOR(RWByte*, const RWByte*, size_t);
RW_TOOLS_SYMBOLIC void   rwAND(RWByte*, const RWByte*, size_t);
RW_TOOLS_SYMBOLIC size_t rwFindOne(const RWByte*, size_t);
RW_TOOLS_SYMBOLIC size_t rwFindZero(const RWByte*, size_t);

#endif /* RW_TOOLS_BITREF_H */
