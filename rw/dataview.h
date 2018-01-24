#ifndef __RWDATAVIEW_H__
#define __RWDATAVIEW_H__

/***************************************************************************
 *
 * dataview.h - Data block classes and base view class
 *
 * This is the base class for vector, matrix, and array view
 * classes.  The classes used to represent data blocks are
 * also declared here.
 *
 * $Id: //math/13/rw/dataview.h#1 $
 *
 * Copyright (c) 1991-2015 Rogue Wave Software, Inc.  All Rights Reserved.
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
 **************************************************************************
 *
 * The RWBlock hierarchy of classes provide a handle to a data block.
 * Each data block has associated with it one RWBlock object.  The
 * RWBlock object provides reference counting on the data block and
 * also is responsible for allocating and cleaning up the memory in
 * the data block.  This allocation and deallocation occurs in the
 * constructor and destructor of the RWBlock.  The base class in the
 * hierarchy, RWBlock, does no allocation of memory or cleaning up.
 * This allows you to use memory allocated from some other source with
 * Math.h++.  The derived class RWNewBlock is used in most circumstances;
 * it allocates memory off the heap using new, and restores the memory
 * using delete.  The class RWMappedBlock provides a way to access
 * large arrays via memory mapped files.  You can create your own memory
 * management schemes by deriving new classes from RWBlock.
 *
 * The RWDataView class collects together things common to
 * all classes which view a data block (ie vectors, matrices,
 * arrays).
 *
 **************************************************************************/

#include <rw/mathdefs.h>
#include <rw/ref.h>


class RW_MATH_GLOBAL RWBlock : public RWReference   // An unmanaged block of data
{
private:
    void*        data_;    // The data
    size_t       len_;     // The number of bytes of data

    // disable copy
    RWBlock(const RWBlock&);               // reference count gets set to zero
    RWBlock& operator=(const RWBlock&);

protected:
    RWBlock() {}                           // This ctor and init are for derived class constructors
    void init(void* data, size_t len) {
        data_ = data;
        len_ = len;
    }
public:
    RWBlock(void* data, size_t len) {
        data_ = data;
        len_ = len;
    }

    RWBlock(RWReference::RWReferenceFlag); // special constructor for statically initialized RWBlocks
    virtual  ~RWBlock() {}
    void*     data() const {
        return data_;
    }
    size_t   length() const {
        return len_;   // length in bytes
    }
};

class RW_MATH_GLOBAL RWNewBlock : public RWBlock   // A data block on the heap
{
public:
    RWNewBlock(size_t nelem, size_t elemsize);
    ~RWNewBlock();
};


//---------------------------------------------------------------------------
//
//  Class RWAllocBlock
//
// RWAllocBlock is similiar to RWNewBlock, except that it allocates space in
// it's constructor using Allocator::allocate provided as the template
// parameter.  RWAllocBlock deletes this space using Allocator::deallocate in
// it's destructor.
// Constructor and destructor implementations are defined in "allocblk.cc".

// This class should replace RWNewBlock eventually.  RWNewBlock is being
// retained for backward compatibility reasons.
//
//---------------------------------------------------------------------------

template< class T, class Allocator >
class RWAllocBlock : public RWBlock
{
public:
    RWAllocBlock(size_t);
    ~RWAllocBlock();

private:
    Allocator the_allocator;
};


//---------------------------------------------------------------------------
//
// Class RWMappedBlock
//
// For OS's that support memory mapped files.
//
//---------------------------------------------------------------------------

#if defined(RW_HAS_MAPPED_FILES)
// Memory mapped file data block
class RW_MATH_GLOBAL RWMappedBlock : public RWBlock
{
public:
    // mode defaults to O_RDONLY (not using default arg avoids including fcntl.h)
    RWMappedBlock(const char* filename, size_t offset = 0);
    RWMappedBlock(const char* filename, size_t offset, int mode);
    ~RWMappedBlock();
    bool good() const {
        return data() != 0;
    }
    bool fail() const {
        return data() == 0;
    }
};
#endif

/**
 * @ingroup math_collection_classes
 * @brief Base class for all classes that provide a view into a contiguous
 * block of memory.
 *
 * RWDataView is a base class for all classes that are a view into a contiguous
 * block of memory.
 */
class RW_MATH_SYMBOLIC RWDataView
{
private:
    RWBlock*      block;                     // The data block now being viewed

protected:
    void*         begin_;                    // Beginning of the data being viewed

    RWDataView();                            // Sets block to nilblock
    RWDataView(RWBlock* block);              // Use any block you like
    RWDataView(size_t n, size_t s);          // Uses RWNewBlock
    RWDataView(const RWDataView& x);
    RWDataView(const RWDataView& v, void* b);// refers to offsets from a block
    ~RWDataView();

    void         reference(const RWDataView&); // refers to a different block
    RWDataView& operator=(const RWDataView&); // refers to a different block

public:
    // These functions need to be public so that they can be called by global
    // functions

    // Error: version number wrong in restoreFrom
    static void  versionErr(int, int);

public:
    // Do not change the enum values: Cfront 2.0 needs them in other files!
    /**
     * Enumeration to control the manner in which array and matrix data will
     * be stored.
     */
    enum Storage {

        /**
         * Data will be stored in Column Major order
         */
        COLUMN_MAJOR = 0,

        /**
         * Data will be stored in Row Major order
         */
        ROW_MAJOR = 1,

        /**
         * RWEITHER is for internal use
         */
        RWEITHER = 2
    };


    bool    sameDataBlock(const RWDataView& x) {
        return block == x.block;
    }
    bool    isSimpleView();
    // True if only 1 ref & begin points to block beginning
};

/*
 * The following routines can be used to log allocation and
 * deallocation of data blocks for debugging.  See the large
 * comment in dataview.cpp for details.  Note that this is
 * primarily intended for debugging Rogue Wave(R) code internally,
 * not for debugging user level code.
 */

#include <iosfwd>

RW_MATH_SYMBOLIC std::ostream* rwblockSetLogStream(std::ostream* s);
void          RW_MATH_SYMBOLIC rwblockReport(std::ostream&);
unsigned long RW_MATH_SYMBOLIC rwblockBytesAllocated();
size_t        RW_MATH_SYMBOLIC rwblockBlocksAllocated();


#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/dataview.cc>
#endif


#endif /* __RWDATAVIEW_H__ */
