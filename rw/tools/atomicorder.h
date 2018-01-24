#ifndef rw_tools_atomicorder_h_
#define rw_tools_atomicorder_h_
/**********************************************************************
 *
 * $Id: //tools/13/rw/tools/atomicorder.h#1 $
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

/**
 * @ingroup atomics
 *
 * Specifies the ordering semantics of atomic operations. Specifying memory
 * ordering semantics indicates the \e minimum ordering constraints on the
 * operation. Implementations may impose stricter ordering constraints.
 */
enum RWAtomicMemoryOrder {
    /**
     * No memory ordering is required.
     */
    rw_mem_order_relaxed,

    /**
     * Ensure that previous "release" operations on this variable are
     * visible.
     */
    rw_mem_order_consume,

    /**
     * @copydoc RWAtomicMemoryOrder::rw_mem_order_consume
     */
    rw_mem_order_acquire,

    /**
     * Ensure that changes to this variable are visible to later operations
     * that perform a "consume" or "acquire" operation.
     */
    rw_mem_order_release,

    /**
     * Ensure that prior "release" operations on this variable are visible
     * and that changes resulting from this operation are visible to future
     * "consume" and "acquire" operations.
     */
    rw_mem_order_acq_rel,

    /**
     * Ensure that there is a total ordering of operations. This enforces a
     * "happens before" relationship between all operations on the object.
     */
    rw_mem_order_seq_cst
};

#endif
