#ifndef RW_TOOLS_CACHEMAN_H
#define RW_TOOLS_CACHEMAN_H



/**********************************************************************
 *
 * RWCacheManager --- caches fixed length blocks to and from disk
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/cacheman.h#1 $
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
#include <rw/tools/pointer.h>

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

/**
 * @ingroup file_system_classes
 *
 * @brief  Caches fixed-length blocks to and from an associated RWFile.
 *
 * Class RWCacheManager caches fixed-length blocks to and from an
 * associated RWFile. The block size can be of any length and is
 * set at construction time. The number of cached blocks can also
 * be set at construction time.
 *
 * Writes to the file may be deferred. Use member function flush()
 * to have any pending writes performed.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/cacheman.h>
 * RWFile f("file.dat");       // Construct a file
 * RWCacheManager(&f, 100);    // Cache 100 byte blocks
 *                             // to file.dat
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/cacheman.h>
 * #include <rw/rwfile.h>
 *
 * struct Record {
 *   int i;
 *   float f;
 *   char str[15];
 * };
 *
 * int main() {
 *    RWFile file("file.dat");     // Construct a file
 *
 *    // Construct a cache, using 20 slots for struct Record:
 *    RWCacheManager cache(&file, sizeof(Record), 20);
 *
 *    Record r;
 *    r.i = 1;
 *    r.f = 3.14f;
 *    r.str [0] = 0;
 *
 *    // ...
 *    cache.write(0, &r);
 *    // ...
 *    cache.read(0, &r);
 *
 *    return 0;
 * }
 * @endcode
 */
class RW_TOOLS_SYMBOLIC RWCacheManager
{

public:

    /**
     * Constructs a cache for the RWFile pointed to by \a file. The length
     * of the fixed-sized blocks is given by \a blocksz. The number of
     * cached blocks is given by \a mxblks. If the total number of bytes
     * cached would exceed the maximum value of an unsigned \c int,
     * RWCacheManager quietly caches a smaller number of blocks.
     */
    RWCacheManager(RWFile* file, unsigned blocksz, size_t mxblks = 10);

    /**
     * Performs any pending I/O operations (i.e., calls flush()) and
     * deallocates any allocated memory.
     */
    ~RWCacheManager();

    /**
     * Performs any pending I/O operations. Returns \c true if the flush
     * was successful, otherwise \c false.
     */
    bool             flush();        // Perform any pending writes.

    /**
     * Invalidates the cache.
     */
    void                  invalidate();   // Invalidate the entire cache

    /**
     * Returns the data located at offset \a locn of the associated RWFile.
     * The data is put in the buffer pointed to by \a dat. This buffer
     * must be at least as long as the block size specified when the
     * cache was constructed. Returns \c true if the operation was successful,
     * otherwise \c false.
     */
    bool             read(RWoffset locn, void* dat);

    /**
     * Writes the block of data pointed to by \a dat to the offset
     * \a locn of the associated RWFile. The number of bytes written is
     * given by the block size specified when the cache was constructed.
     * The actual write to disk may be deferred. Use member function
     * flush() to perform any pending writes. Returns \c true if the
     * operation was successful, otherwise \c false.
     */
    bool             write(RWoffset locn, void* dat);

private:

    RWCacheManager(const RWCacheManager&); // Private to ensure no copies
    void                  operator=(const RWCacheManager&); // Ditto
    size_t                ageAndFindSlot(RWoffset);
    bool             flush(size_t);
    size_t                getFreeSlot();
    size_t                LRU() const; // Find least recently used block

private:

    RWFile*                      theFile_;
    size_t                       maxblocks_; // Max # blocks in buffer
    size_t                       nused_;     // Number being used.
    size_t                       blocksize_; // Size of a block
    RWTScopedPointer<RWoffset[]> diskAddrs_; // Its disk address
    RWTScopedPointer<size_t[]>   useCounts_; // Use count for the block
    RWTScopedPointer<char[]>     buff_;      // The set of blocks.
};


#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif /* RW_TOOLS_CACHEMAN_H */
