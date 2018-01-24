#ifndef RW_TOOLS_FILEMGR_H
#define RW_TOOLS_FILEMGR_H



/**********************************************************************
 *
 * RWFileManager --- manages free space in a file
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/filemgr.h#1 $
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
#include <rw/rwfile.h>
#include <rw/rwerr.h>
#include <rw/tools/pointer.h>

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

class RWListManager;

/**
 * @ingroup file_system_classes
 *
 * @brief Allocates and deallocates storage in a disk
 * file, much like a "freestore" manager
 *
 * Class RWFileManager allocates and deallocates storage in a disk
 * file, much like a "freestore" manager. It does this by maintaining
 * a linked list of free space within the file.
  *
 * @note
  *Class RWFileManager inherits class RWFile as a public base class; hence
 * all the public member functions of RWFile are visible to RWFileManager.
 * They are not listed here.
 *
 * If a file is managed by an RWFileManager, then reading or writing
 * to unallocated space in the file has undefined results.
 * In particular, overwriting the end of allocated space is a common
 * problem which usually results in corrupted data. One way to encounter
 * this problem is to use
 * \link RWCollection::binaryStoreSize() binaryStoreSize() \endlink
 * to discover the amount
 * of space needed to store an RWCollection. For most purposes,
 * the storage size of an RWCollection is found using the RWCollectable
 * method \link RWCollection::recursiveStoreSize() recursiveStoreSize() \endlink.
 *
 * @section synopsis Synopsis
 *
 * @code
 * typedef long     RWoffset ;
 * typedef unsigned long   RWspace;  // (typically)
 * #include <rw/filemgr.h>
 * RWFileManager f("file.dat");
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWFileManager : public RWFile
{

public:

    /**
     * Constructs an RWFileManager for the file with path name \a filename
     * using mode \a mode. The mode is as given by the Standard C Library
     * function \b fopen(). If \a mode is zero (the default), then the
     * constructor attempts to open an existing file with the given filename
     * for update (mode <tt>"rb+"</tt>). If this is not possible, then it
     * attempts to create a new file with the given filename (mode
     * <tt>"wb+"</tt>). If the file exists and is not empty, then the constructor
     * assumes it contains an existing file manager; other contents
     * cause the function to throw an exception of type RWExternalErr. If
     * no file exists or if an existing file is empty, then the constructor
     * attempts to create the file (if necessary) and initialize
     * it with a new file manager. The resultant object should be checked
     * for validity using function isValid(). RWFileErr is a possible
     * exception that could be thrown.
     */
    RWFileManager(const char* filename, const char* mode = rwnil);

    /**
     * Empty Destructor.
     */
    ~RWFileManager();

    /**
     * Allocates \a s bytes of storage in the file. Returns the offset
     * to the start of the storage location. The very first allocation
     * for the file is considered "special" and can be returned at any
     * later time by the function start(). RWFileErr is a possible
     * exception that could be thrown.
     */
    RWoffset              allocate(RWspace s);

    /**
     * Deallocates (frees) the storage space starting at offset \a t.
     * This space must have been previously allocated by a call to allocate().
     * The very first allocation ever made in the file is considered
     * "special" and cannot be deallocated. RWFileErr is a possible
     * exception that could be thrown.
     */
    void                  deallocate(RWoffset t);

    /**
     * Returns an offset just past the end of the file.
     */
    RWoffset              endData() const {
        return endOfData_;
    }

    /**
     * Returns the offset of the first space ever allocated for data
     * in this file. If no space has ever been allocated, returns
     * #RWNIL. This is typically used to "get started" and find the rest
     * of the data in the file.
     */
    RWoffset              start() const {
        return startOfData_;
    }

protected:

    RWoffset              allocateAtEnd(RWspace);
    bool             deallocateFromEnd(RWoffset, RWspace);
    void                  readErr();
    void                  seekErr();
    void                  writeErr();
    RWoffset              rootOffset() const;

private:

    // Cannot have 2 managers for the same file.
    RWFileManager(const RWFileManager&);
    void                  operator=(const RWFileManager&);

private:

    RWTScopedPointer<RWListManager> filemgr_;  // Implementation
    RWoffset startOfData_;                     // Offset to first data in file.
    RWoffset endOfData_;                       // Offset to last data in file.

    friend class RWListManager;
};

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif /* RW_TOOLS_FILEMGR_H */
