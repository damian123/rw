#ifndef RW_TOOLS_BUFPAGE_H
#define RW_TOOLS_BUFPAGE_H



/**********************************************************************
 *
 * RWBufferedPageHeap: A virtual page heap accessed through a buffer
 * $Log: bufpage.h,v $
 * Revision 7.4  1996/05/08 18:53:36  kyle
 * Added export pragma for Mac code fragments
 * Revision 7.3  1996/02/18 01:36:17  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 * Revision 7.2  1995/09/05 18:11:45  jims
 * Use new copyright macro
 * Revision 7.1  1994/10/16  00:36:11  josh
 * Merged 6.1 and 7.0 development trees
 * Revision 6.2  1994/07/12  18:45:56  vriezen
 * Update Copyright notice
 * Revision 6.1  1994/04/15  19:09:31  vriezen
 * Move all files to 6.1
 * Revision 2.4  1994/02/15  18:37:29  jims
 * Change return types from unsigned to size_t
 * Revision 2.3  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 * Revision 2.2  1993/03/23  02:45:12  keffer
 * Changed variable names; eliminated int to unsigned conversions.
 * Revision 2.0  1992/10/23  03:27:32  keffer
 * RCS Baseline version
 * Revision 1.0   11 Mar 1992 14:09:58  keffer
 * Initial revision.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/bufpage.h#1 $
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
#include <rw/vpage.h>
#include <stddef.h>

#if !defined(RW_DISABLE_DEPRECATED)
RW_SUPPRESS_DEPRECATED_WARNINGS
/**
 * @ingroup file_system_classes
 * @brief Deprecated. Abstract base class representing a virtual
 * page heap accessed through a buffer.
 *
 * @deprecated As of SourcePro 1.
 *
 * RWBufferedPageHeap is deprecated and is no longer supported.
 * It may be removed from future releases.
 *
 * This is an abstract base class that represents an abstract page
 * heap buffered through a set of memory buffers. It inherits from
 * the abstract base class RWVirtualPageHeap, which represents an
 * abstract page heap.
 *
 * RWBufferedPageHeap supplies and maintains a set of memory buffers.
 * Specializing classes should supply the actual physical mechanism
 * to swap pages in and out of these buffers by supplying definitions
 * for the pure virtual functions swapIn(size_t,void*) and
 * swapOut(size_t,void*).
 *
 * The specializing class should also supply appropriate definitions
 * for the public functions allocate() and deallocate(size_t).
 *
 * For a sample implementation of a specializing class, see class
 * RWDiskPageHeap.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/bufpage.h>
 * (Abstract base class )
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_DEPRECATE_TYPE("") RW_TOOLS_GLOBAL RWBufferedPageHeap : public RWVirtualPageHeap
{
    unsigned              nBuffers_;      // Number of buffers (each is pageSize() big)
    RWvoid*               buffers_;       // Points to an array of pointers to buffers
    size_t*             handles_;       // Page handle associated with each buffer
    short*                lockCounts_;    // Lock count for each buffer
    unsigned*             age_;           // Age of buffer since accessed
    bool*            dirty_;         // Whether this buffer has changed since swap in
protected:
    size_t                ageAndFindHandle(size_t);     // Find slot for given handle and age all slots
    size_t                findHandle(size_t);           // Find slot for given handle
    size_t                findUnusedSlot();               // Find an unused slot
    size_t                swapPageIn(size_t);           // Swap in page with given handle
    size_t                swapOutLRUSlot();               // Swap out the Least Recently Used page

    /**
     * It is the responsibility of the specializing class to supply a
     * definition for this pure virtual function. The Function swapIn() should
     * copy the page with handle \a h into the buffer pointed to by \a buf.
     */
    virtual bool     swapIn(size_t h, void* buf)  = 0;

    /**
     * It is the responsibility of the specializing class to supply a
     * definition for this pure virtual function. The function swapOut()
     * should copy the page with handle \a h from the buffer pointed
     * to by \a buf to the swapping medium.
     */
    virtual bool     swapOut(size_t h, void* buf) = 0;

public:

    /**
     * Constructs a buffered page heap with page size \a pgsize. The
     * number of buffers (each of size \a pgsize) that are allocated
     * on the heap is specified by \a nbufs. If there is insufficient
     * memory to satisfy the request, the state of the resultant object
     * as returned by member function isValid() is \c false,
     * otherwise \c true.
     */
    RWBufferedPageHeap(unsigned pgsize, unsigned nbufs = 10);

    /**
     * Deallocates all internal buffers.
     */
    virtual               ~RWBufferedPageHeap();

    /**
     * Returns \c true if self is in a valid state. A possible reason
     * why the object might not be valid is insufficient memory to allocate
     * the internal buffers.
     */
    bool             isValid() {
        return buffers_ != NULL;
    }

    /**
     * It is the responsibility of the specializing class to supply
     * a definition for this pure virtual function. The specializing
     * class should allocate a page and return a unique handle for it.
     * It should return zero if it cannot satisfy the request. The size
     * of the page is set by the constructor.
     */
    virtual size_t        allocate()           = 0;

    /**
     * Redefined from class RWVirtualPageHeap. It is never an error
     * to call this function with argument zero. Even though this is
     * not a pure virtual function, it is the responsibility of the
     * specializing class to supply an appropriate definition for this
     * function. All this definition does is release any buffers associated
     * with the handle \a h. Just as the actual page allocation is done
     * by the specializing class through virtual function allocate(),
     * so must the actual deallocation be done by overriding deallocate().
     */
    virtual void          deallocate(size_t h);


    // The following classes automatically inherit documentation from
    // the base class.
    virtual void          dirty(size_t h);
    virtual void*         lock(size_t h);                 // Lock a page
    virtual void          unlock(size_t h);               // Unlock a page
};
RW_RESTORE_DEPRECATED_WARNINGS
#endif


#endif  /* RW_TOOLS_BUFPAGE_H */
