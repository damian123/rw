#ifndef RW_TOOLS_DISKPAGE_H
#define RW_TOOLS_DISKPAGE_H



/**********************************************************************
 *
 * RWDiskPageHeap
 *       : Specializing buffered page heap that swaps pages out to disk.
 * $Log: diskpage.h,v $
 * Revision 7.4  1996/05/08 16:59:47  kyle
 * Added export pragma for Mac code fragments
 * Revision 7.3  1996/02/18 01:39:52  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 * Revision 7.2  1995/09/05 18:29:19  jims
 * Use new copyright macro
 * Revision 7.1  1994/10/16  02:48:33  josh
 * Merged 6.1 and 7.0 development trees
 * Revision 6.2  1994/07/12  18:46:56  vriezen
 * Update Copyright notice
 * Revision 6.1  1994/04/15  19:09:48  vriezen
 * Move all files to 6.1
 * Revision 2.2  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 * Revision 2.0  1992/10/23  03:27:32  keffer
 * RCS Baseline version
 *    Rev 1.0   11 Mar 1992 14:09:58   KEFFER
 * Initial revision.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/diskpage.h#1 $
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
#include <rw/bufpage.h>
#include <rw/bitvec.h>
#include <stdio.h>

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @ingroup file_system_classes
 * @brief Deprecated. Specialized type of buffered page heap that
 * swaps its pages to disk as necessary.
 *
 * @deprecated As of SourcePro 11.1.
 *
 * RWDiskPageHeap is deprecated and is no longer supported.
 * It may be removed from future releases.
 *
 * Class RWDiskPageHeap is a specializing type of buffered page
 * heap.  It swaps its pages to disk as necessary.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/diskpage.h>
 * unsigned nbufs;
 * unsigned pagesize;
 * RWDiskPageHeap heap("filename", nbufs, pagesize) ;
 * @endcode
 *
 * @section persistence Persistence
 * None
 *
 * @section example Example
 * In this example, 100 nodes of a linked list are created and strung
 * together.  The list is then walked, confirming that it contains
 * 100 nodes.  Each node is a single page.  The "pointer" to the
 * next node is actually the handle for the next page.
 *
 * @code
 * #include <iostream>
 * #include <iomanip>
 * #include <rw/diskpage.h>
 * #include <rw/rstream.h>
 *
 * struct Node
 * {
 *     int   key;
 *     RWHandle      next;
 * };
 *
 * RWHandle head = 0;
 * const int N = 100;      // Exercise 100 Nodes
 *
 * int main()
 * {
 *     // Construct a disk-based page heap with page size equal
 *     // to the size of Node and with 10 buffers:
 *     RWDiskPageHeap heap("pageheap.tmp", 10, sizeof(Node));
 *
 *     if (!heap.isValid()) {
 *         std::cerr << "Unable to open temporary swap file.\n";
 *         std::cerr << "Do you have write privileges?\n" << std::flush;
 *         return 0;
 *     }
 *
 *     // Build the linked list:
 *     for (int i=0; i<N; i++) {
 *         RWHandle h = RWHandle(heap.allocate());
 *         Node* newNode = (Node*)heap.lock(h);
 *         newNode->key  = i;
 *         newNode->next = head;
 *         head = h;
 *         heap.dirty(h);
 *         heap.unlock(h);
 *     }
 *
 *     // Now walk the list:
 *     unsigned count = 0;
 *     RWHandle nodeHandle = head;
 *     while (nodeHandle) {
 *         Node* node = (Node*)heap.lock(nodeHandle);
 *         RWHandle nextHandle = node->next;
 *         heap.unlock(nodeHandle);
 *         heap.deallocate(nodeHandle);
 *         nodeHandle = nextHandle;
 *         count++;
 *     }
 *
 *     std::cout << "List with " << count << " nodes walked.\n";
 *
 *     // The following line should not be necessary,
 *     // but a bug in Sun C++ requires it:
 *     std::cout << std::flush;
 *
 *     return 0;
 * }
 * @endcode
 * *
 * Program output:
 *
 * @code
 *
 * List with 100 nodes walked.
 * @endcode
 *
 */
class RW_DEPRECATE_TYPE("") RW_TOOLS_GLOBAL RWDiskPageHeap
    : public RWBufferedPageHeap
{

public:

    /**
     * Constructs a new disk-based page heap.  The heap uses a file
     * with filename \a filename, otherwise it negotiates with the
     * operating system for a temporary filename.  The number of buffers,
     * each the size of the page size, is \a nbufs.  No more than
     * this many pages can be locked at any one time.  The size of each
     * page is given by \a pgsize.  To see whether a valid RWDiskPageHeap
     * has been constructed, call member function isValid().
     *
     * Windows users may be required to call \c AnsiToOEM on a Windows
     * character constant for the filename in this constructor, because
     * this constructor uses a C function call that may resolve to a
     * DOS system call. In that case, DOS might not be able to recognize
     * Windows character sets.
     *
     * @note
     * Passing a null pointer for the parameter \a filename may result
     * in a system call that requires administrator access.
     */
    RWDiskPageHeap(const char* filename = 0, unsigned nbufs = 10, unsigned pgsize = 512);

    /**
     * Returns any resources used by the disk page heap back to the
     * operating system.  All pages should have been deallocated before
     * the destructor is called.
     */
    virtual               ~RWDiskPageHeap();

    bool             isValid() const {
        return tempfp_ != 0;
    }

    // Inherited from RWPageBuffer:
    /**
     * Allocates a page off the disk page heap and returns a handle for it.
     * If there is no more space (for example, the disk is full), the function
     * returns 1.
     */
    virtual size_t        allocate();

    /**
     * Deallocates the page associated with handle \a h.  It is not an error to
     * deallocate a zero handle.
     */
    virtual void          deallocate(size_t h);

protected:

    enum HandleStatus { NotUsed, NoSwapSpace, HasSwapSpace };

    bool             allocateDiskPage(size_t);
    bool             handleValid(size_t);
    RWoffset              offsetOfHandle(size_t);
    void                  resize(size_t);

    // Inherited from RWBufferedPageHeap:
    virtual bool     swapIn(size_t, void*);
    virtual bool     swapOut(size_t, void*);

private:

    static const size_t         initialPages_;
    static const size_t         initialHandles_;
    static const size_t         pageIncrement_;
    static const size_t         handleIncrement_;
    RWBitVec                    freePageMap_;   // Bit flags for free disk pages
    size_t*                     handleMap_;     // Array that maps from handle number to disk page
    HandleStatus*               handleStatus_;  // Status of each slot in handleMap_
    size_t                      nHandles_;      // Length of handleMap_ and handleStatus_
    FILE*                       tempfp_;

};
#endif

#endif  /* RW_TOOLS_DISKPAGE_H */
