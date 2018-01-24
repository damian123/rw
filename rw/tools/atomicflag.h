#ifndef rw_tools_atomicflag_h_
#define rw_tools_atomicflag_h_
/**********************************************************************
 *
 * $Id: //tools/13/rw/tools/atomicflag.h#1 $
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

#if defined(_MSC_VER)
#  include <rw/tools/atomics/atomicflag_msvc.h>
#elif defined(__GNUG__) && __GNUG__ >= 4
#  include <rw/tools/atomics/atomicflag_gcc.h>
#elif defined(__SUNPRO_CC)
#  include <rw/tools/atomics/atomicflag_sunpro.h>
#elif defined(__xlC__)
#  include <rw/tools/atomics/atomicflag_xlc.h>
#elif defined(__HP_aCC)
#  include <rw/tools/atomics/atomicflag_acc.h>
#else
#  error "A lock-free implementation of RWAtomicFlag is not supported on this platform."
#endif

/**
 * @relates RWAtomicFlag
 *
 * Initializes an RWAtomicFlag to a cleared state.
 */
#define RW_ATOMIC_FLAG_INIT { { 0 } }

/**
 * @ingroup atomics
 * @brief Atomically sets and clears a bit flag.
 *
 * RWAtomicFlag manages a bit flag (set or cleared) allowing for atomic
 * manipulation of the bit, including providing test-and-set functionality.
 *
 * @section example Example
 *
 * @code
 * RWAtomicFlag flag = RW_ATOMIC_FLAG_INIT;
 * while (!flag.testAndSet()) { ... }
 * ...
 * flag.clear();
 * @endcode
 *
 * @note
 * While copy construction and assignment are not prohibited, these
 * operations are not performed in an atomic manner, and may result in
 * undefined behavior. These operations may be prohibited in
 * a future release and should be avoided.
 */
class RWAtomicFlag
{
public:
    /**
     * Sets the flag and returns \c true if the flag was previously set,
     * \c false otherwise.
     */
    inline bool testAndSet(RWAtomicMemoryOrder order = rw_mem_order_seq_cst) {
        return imp_.testAndSet(order);
    }

    /**
     * @copydoc RWAtomicFlag::testAndSet(RWAtomicMemoryOrder)
     */
    inline bool testAndSet(RWAtomicMemoryOrder order = rw_mem_order_seq_cst) volatile {
        return imp_.testAndSet(order);
    }

    /**
     * Unsets the flag.
     *
     * @note
     * \a order values of #rw_mem_order_consume, #rw_mem_order_acquire
     * and #rw_mem_order_acq_rel are not supported and will result in
     * undefined behavior.
     */
    inline void clear(RWAtomicMemoryOrder order = rw_mem_order_seq_cst) {
        RW_PRECONDITION(order != rw_mem_order_consume);
        RW_PRECONDITION(order != rw_mem_order_acquire);
        RW_PRECONDITION(order != rw_mem_order_acq_rel);
        return imp_.clear(order);
    }

    /**
     * @copydoc RWAtomicFlag::clear(RWAtomicMemoryOrder)
     */
    inline void clear(RWAtomicMemoryOrder order = rw_mem_order_seq_cst) volatile {
        RW_PRECONDITION(order != rw_mem_order_consume);
        RW_PRECONDITION(order != rw_mem_order_acquire);
        RW_PRECONDITION(order != rw_mem_order_acq_rel);
        return imp_.clear(order);
    }

    RWAtomicFlagImp imp_;
};

#endif
