#ifndef RW_TOOLS_STORETABLE_H
#define RW_TOOLS_STORETABLE_H



/**********************************************************************
 *
 * Definitions for internal classes used to store collectables
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/rwstore.h#1 $
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
 *******************  RWStoreTable declarations ***************************
 **/

#include <rw/defs.h>
#include <rw/rwassert.h>
#include <rw/rwset.h>
#include <rw/stringid.h>

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

extern RW_TOOLS_SYMBOLIC RWCollectableP getRWNilCollectable();

#define RWnilCollectable (getRWNilCollectable())

class RW_TOOLS_GLOBAL RWStoreTable : public RWSet
{
public:
    RWStoreTable();
    ~RWStoreTable();
    bool             add(const void*, size_t&);

    // RWStringID is by-reference to accommodate bug in Borland 5.0 compiler
    bool             add(const RWStringID&, size_t&);

    bool             operator<=(const RWStoreTable& rws) {
        return RWSet::operator<=(rws);
    }
    bool             operator<=(const RWSet& rws) const {
        return RWSet::operator<=(rws);
    }
    bool             operator<=(const RWHashTable& rwht) const {
        return RWSet::operator<=(rwht);
    }

    static RWStoreTable* getStoreTable();
    static RWStoreTable* newStoreTable();
    static void freeStoreTable();
};

class RW_TOOLS_SYMBOLIC RWUseStoreTable
{
    bool  firstUse_;
    RWStoreTable* table_;
    RWUseStoreTable(const RWUseStoreTable&); // Not defined!
    void operator=(const RWUseStoreTable&);  // Not defined!
public:
    RWUseStoreTable();
    bool firstUse() {
        return firstUse_;
    }
    bool add(const void* p, size_t& i) {
        assert(0 != table_);
        return table_->add(p, i);
    }
    bool add(const RWStringID& p, size_t& i) {
        assert(0 != table_);
        return table_->add(p, i);
    }
    ~RWUseStoreTable();
};

class RW_TOOLS_GLOBAL RWStoreEntry : public RWCollectable
{
    RW_DECLARE_COLLECTABLE_CLASS(RW_TOOLS_SYMBOLIC, RWStoreEntry);
    friend class RWStoreTable;
    const void*           item;
    RWssize_t             objectNumber;
public:
    RWStoreEntry() {}
    ~RWStoreEntry() {}
    RWStoreEntry(const void* c, RWssize_t n): item(c), objectNumber(n) {}
    unsigned              hash() const;
    bool             isEqual(const RWCollectable* c) const;
};


class RW_TOOLS_GLOBAL RWStoreStringID : public RWCollectable
{
    RW_DECLARE_COLLECTABLE_CLASS(RW_TOOLS_SYMBOLIC, RWStoreStringID);
    friend class RWStoreTable;
    const RWStringID      item;
    RWssize_t             objectNumber;

private:

    // not defined
    RWStoreStringID& operator=(const RWStoreStringID&);

public:
    RWStoreStringID() {}
    ~RWStoreStringID() {}
    RWStoreStringID(const RWStringID& id, RWssize_t n) : item(id), objectNumber(n) {}
    unsigned              hash() const;
    bool             isEqual(const RWCollectable* c) const;
};


/*
 *******************  RWReadTable declarations ***************************
 */

class RW_TOOLS_SYMBOLIC RWReadTable
{
    // For the read table, just use a simple extendable vector of pointers or
    // RWClassID. We may assume that both are numeric types and that RWClassID
    // is no greater than a pointer. So the union works fine.
    union ConstVoidPtr {
        const void*   ptr_;
        RWClassID    id_;
    };
    RWTScopedPointer<ConstVoidPtr[]> table_;
    size_t entries_;
    size_t capacity_;
    enum {InitialSize = 30, IncrementSize = 100};
    void append(ConstVoidPtr cvp);
public:
    RWReadTable();
    ~RWReadTable();
    void append(void* vp);
    void append(RWClassID cid);
    size_t entries() {
        return entries_;
    }
    RWClassID getId(size_t i) {
        return table_[i].id_;
    }
    const void* getPtr(size_t i) {
        return table_[i].ptr_;
    }

    static RWReadTable* getReadTable();
    static RWReadTable* newReadTable();
    static void freeReadTable();
};

class RW_TOOLS_SYMBOLIC RWUseReadTable
{
    bool firstUse_;
    RWReadTable* table_;
    RWUseReadTable(const RWUseReadTable&); // Not defined
    void operator=(const RWUseReadTable&); // Not defined
public:
    RWUseReadTable();
    ~RWUseReadTable();
    bool firstUse() const {
        return firstUse_;
    }
    void append(void* vp) {
        assert(0 != table_);
        table_->append(vp);
    }
    void append(RWClassID cid) {
        assert(0 != table_);
        table_->append(cid);
    }
    size_t entries() {
        return table_->entries();
    }
    RWClassID getId(size_t i) {
        return table_->getId(i);
    }
    const void* getPtr(size_t i) {
        return table_->getPtr(i);
    }
};

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif  /* RW_TOOLS_STORETABLE_H */
