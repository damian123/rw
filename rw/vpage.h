#ifndef RW_TOOLS_VPAGE_H
#define RW_TOOLS_VPAGE_H



/**********************************************************************
 *
 * RWVirtualPageHeap: Abstraction of a page heap, swapped to unknown parts
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/vpage.h#1 $
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

typedef unsigned RWHandle;

#if !defined(RW_DISABLE_DEPRECATED)
RW_SUPPRESS_DEPRECATED_WARNINGS
/**
 * @ingroup file_system_classes
 * @brief Deprecated. Abstract base class representing an abstract page
 * heap of fixed-sized pages.
 *
 * @deprecated As of SourcePro 11.1.
 * RWVirtualPageHeap is deprecated and is no longer supported.
 * It may be removed from future releases.
 *
 * RWVirtualPageHeap is an abstract base class representing an abstract page
 * heap of fixed sized pages.  Specializing classes should work as
 * follows:
 *
 * Allocate a page off the abstract heap by calling member function
 * allocate(), which returns a memory "handle," an object of
 * type \c size_t.  This handle logically represents the page.
 *
 * In order to use the page, it must first be "locked" by calling
 * member function lock() with the handle as an argument. The
 * specializing class of RWVirtualPageHeap must make
 * whatever arrangements are necessary to swap in the page associated
 * with the handle and bring it into physical memory. The actual
 * swapping medium could be disk, expanded or extended memory, or
 * a machine on a network. Upon return, lock() returns
 * a pointer to the page, now residing in memory.
 *
 * Once a page is in memory, you are free to do anything you want
 * with it, although if you change the contents, you must call member
 * function dirty() before unlocking the page.
 *
 * Locked pages use up memory.  In fact, some specializing classes
 * have only a fixed number of buffers in which to do their
 * swapping.  If you are not using the page, you should call unlock().
 * After calling unlock() the original address returned by lock()
 * is no longer valid  --  to use the page again, it must be locked
 * again with lock().
 *
 * When you are completely done with the page, then call deallocate()
 * to return it to the abstract heap.
 *
 * In practice, managing this locking and unlocking and the inevitable
 * type casts can be difficult.  It is usually easier to design
 * a class that can work with an abstract heap to bring things in
 * and out of memory automatically.  Indeed, this is what has been
 * done with class \link RWTValVirtualArray RWTValVirtualArray<T>\endlink,
 * which represents a virtual array of elements of type \c T.  Elements are
 * automatically swapped in as necessary as they are addressed.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/vpage.h>
 * (Abstract base class)
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RW_DEPRECATE_TYPE("") RWVirtualPageHeap
{
    RW_DECLARE_HOME(RWVirtualPageHeap)
protected:
    unsigned              pageSize_;
public:

    /**
     * Sets the size of a page.
     */
    RWVirtualPageHeap(unsigned pgsize) : pageSize_(pgsize) { }

    /**
     * Virtual destructor, allowing specializing classes
     * a chance to deallocate any resources that they may have allocated.
     */
    virtual               ~RWVirtualPageHeap() { }

    /**
     * Returns the page size for this abstract page heap.
     */
    unsigned              pageSize() const {
        return pageSize_;
    }

    /**
     * Allocates a page off the abstract heap and returns a handle for
     * it.  If the specializing class is unable to honor the request,
     * then it should return a zero handle.
     */
    virtual size_t        allocate()           = 0;

    /**
     * Deallocate the page associated with handle \a h.  It is not an
     * error to deallocate a zero handle.
     */
    virtual void          deallocate(size_t h) = 0;

    /**
     * Declare the page associated with handle \a h to be "dirty." That
     * is, it has changed since it was last locked.  The page must be
     * locked before calling this function.
     */
    virtual void          dirty(size_t h)      = 0;

    /**
     * Lock the page, swapping it into physical memory, and return an
     * address for it.  Returns #rwnil if the specializing
     * class is unable to honor the lock.  The returned pointer should
     * be regarded as pointing to a buffer of the page size.
     */
    virtual void*         lock(size_t)       = 0;

    /**
     * Unlock a page.  A page must be locked before calling this function.
     * After calling this function, the address returned by lock() is
     * no longer valid.
     */
    virtual void          unlock(size_t)     = 0;
};
RW_RESTORE_DEPRECATED_WARNINGS
#endif

#endif  /* RW_TOOLS_VPAGE_H */
