#ifndef RW_TOOLS_MEMPOOL_H
#define RW_TOOLS_MEMPOOL_H



/**********************************************************************
 *
 * RWMemoryPool: Manages small object pool via inheritance
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/mempool.h#1 $
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
 * This class maintains a pool of free memory allocations for small objects.
 * Objects inheriting from it will be managed by it.  It can greatly improve
 * the performance of objects that are rapidly allocated and deleted, such
 * as nodes in a linked list or binary tree.  It can also help memory
 * fragmentation.
 *
 * Two static variables affect its performance:
 *
 *  RWMAXPOOLS:     The number of different sizes managed.  The
 *                  default (5) will manage objects with sizes
 *                  from 1 to 10 bytes big.  Objects bigger than this
 *                  will be handled by the operating system.
 *
 *  RWPOOLSIZE:     The maximum number of objects retained within a size class.
 *                  Excess objects are returned to the operating system.
 */

#include <rw/defs.h>

#include <stddef.h>     /* Looking for size_t*/

#ifdef RW_DONT_USE_MEMPOOL
#  define RWMemoryPool_OPTION
#else

const int RWPOOLSIZE = 5;
const int RWMAXPOOLS = 5;


class RW_TOOLS_SYMBOLIC RWMemoryPool
{
    // If not memcheck and not debug
    // If following changes, also change #if in mempool.cpp
#  if !defined(RW_DEBUG)
    static void*  stash[RWMAXPOOLS][RWPOOLSIZE];  // The private stash
    static short  nstash[RWMAXPOOLS]; // Number of free objects in each size.
#  endif
public:
#  ifdef RW_TCC_DELETE_SIZE_BUG
    ~RWMemoryPool() { }   // Superfluous destructor required for Borland bug
#  endif
    static void   releaseCachedMemory(); // drain memory pool

    // workaround for false new/delete mismatch
#  if !defined(RW_FALSE_NEW_DELETE_MISMATCH)
    void*         operator new(size_t);
#  else
    void*         operator new(size_t, size_t t = 0);
#  endif
    void          operator delete(void*, size_t);

};

// Multithreaded programs can't afford conflicts in the static
// RWMemoryPool free-block pool.  We inherit differently because
// otherwise every derived object would grow by (up to) 4 bytes,
// for no reason.

#  ifdef RW_MULTI_THREAD
#    define RWMemoryPool_OPTION
#  else
#    define RWMemoryPool_OPTION   : public RWMemoryPool
#  endif

#endif /* RW_DONT_USE_MEMORY_POOL */


#endif  /* RW_TOOLS_MEMPOOL_H */
