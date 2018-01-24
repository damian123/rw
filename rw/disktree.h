#ifndef RW_TOOLS_DISKTREE_H
#define RW_TOOLS_DISKTREE_H


/**********************************************************************
 *
 * RWBTreeOnDisk -- disk-based B-Tree. Uses cache, filemanager.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/disktree.h#1 $
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
#include <rw/cacheman.h>
#include <rw/filemgr.h>
#include <rw/cstring.h>

/**
 * @relates RWBTreeOnDisk
 *
 * A typedef for the function used in the method
 * applyToKeyAndValue(RWdiskTreeApply, void*).
 * This function <i>may not</i> change the first argument, i.e. the key.
 * The last argument can be anything and is passed through from the call to
 * applyToKeyAndValue(RWdiskTreeApply, void*).
 */
typedef void (*RWdiskTreeApply)(const char*, RWstoredValue, void*);

extern "C" {
    /**
     * @relates RWBTreeOnDisk
     *
     * A typedef for the comparison function to be used with RWBTreeOnDisk.
     * It should return a number less than, equal to, or greater than zero,
     * depending on whether the first argument argument is less than, equal to,
     * or greater than the second argument, respectively. The third argument
     * is the key length.
     *
     * @see setComparison(RWdiskTreeCompare)
     */
    typedef int (*RWdiskTreeCompare)(const char*, const char*, size_t);
}

const RWstoredValue RWBTreeOnDiskCurrentVersion = 0x200;

class RW_TOOLS_SYMBOLIC RWDiskTreeNode;

/****************************************************************
 *                                                              *
 *                      RWBTreeOnDisk                           *
 *                                                              *
 ****************************************************************/

/**
 * @ingroup file_system_classes
 *
 * @brief Represents an ordered collection of associations of keys
 * and values, with keys ordered based on an external function and
 * with duplicate keys not allowed.
 *
 * Class RWBTreeOnDisk represents an ordered collection of associations
 * of keys and values, where the ordering is determined by comparing
 * keys using an external function. The user can set this function.
 * Duplicate keys are not allowed. Given a key, the corresponding
 * value can be found.
 *
 * This class is specifically designed for managing a B-tree in
 * a disk file. Keys, defined to be arrays of \c chars, and values,
 * defined by the typedef \c RWstoredValue, are stored and retrieved
 * from a B-tree. The values can represent offsets to locations
 * in a file where the objects are stored.
 *
 * The key length is set by the constructor. By default, this value
 * is 16 characters. By default, keys are null-terminated. However,
 * the tree can be used with embedded nulls, allowing multibyte
 * and binary data to be used as keys. To do so you must:
 *
 * -  Specify \c true for parameter \c ignoreNull in the constructor.
 * -  Make sure all buffers used for keys are at least as long as the
 *    key length. (Remember, storage and comparison will \e not
 *    stop with a null value.)
 * -  Use a comparison function (such as \c memcmp()) that ignores nulls.
 *
 * This class is meant to be used with class RWFileManager, which
 * manages the allocation and deallocation of space in a disk file.
 *
 * When you construct an RWBTreeOnDisk, you give the location of
 * the root node in the constructor as argument \c start. If this
 * value is #RWNIL (the default), the location is retrieved
 * using RWFileManager::start(). You can also use the enumeration
 * #createMode to set whether to use an existing tree (creating one
 * if one doesn't exist) or to force the creation of a new tree.
 * The location of the resultant root node can be retrieved using
 * member function baseLocation().
 *
 * More than one B-tree can exist in a disk file. Each must have
 * its own separate root node. This can be done by constructing
 * more than one RWBTreeOnDisk, each with #createMode set to \c create.
 *
 * The \e order of the B-tree can be set in the constructor.
 * Larger values result in shallower trees, but less efficient
 * use of disk space. The minimum number of entries in a node can
 * also be set. Smaller values may result in less time spent balancing
 * the tree, but less efficient use of disk space.
 *
 * @section synopsis Synopsis
 *
 * @code
 * typedef long RWstoredValue ;
 * typedef int (*RWdiskTreeCompare)(const char*, const char*, size_t);
 * #include <rw/disktree.h>
 * #include <rw/filemgr.h>
 * RWFileManager fm("filename.dat");
 * RWBTreeOnDisk bt(fm);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWBTreeOnDisk
{
    friend class RW_TOOLS_SYMBOLIC RWDiskTreeNode;

public:

    /**
     * This enumeration is used by the constructor to allow backwards
     * compatibility with older V5.X style trees, which supported only
     * 16-byte key lengths. It is used only when creating a new tree.
     * If opening a tree for update, its type is determined automatically
     * at runtime.
     */
    enum styleMode {
        /**
         * Initialize a new tree using V6.X style trees. This is the
         * default.
         */
        V6Style,

        /**
         * Initialize a new tree using V5.X style trees. The key length
         * is fixed at 16 bytes.
         */
        V5Style
    };

    /**
     * This enumeration is used by the constructor to determine whether
     * to force the creation of a new tree.
     */
    enum createMode {

        /**
         * Look in the location given by the constructor argument
         * \c start for the root node. If valid, use it. Otherwise,
         * allocate a new tree. This is the default.
         */
        autoCreate,

        /**
         * Forces the creation of a new tree. The argument \c start
         * is ignored.
         */
        create
    };

    /**
     * Constructs a B-tree on disk.
     *
     * @param f The file in which the B-tree is to be managed. This is
     * the only required parameter.
     *
     * @param cacheBlocks The maximum number of nodes that can be cached in memory.
     *
     * @param cmode Determines whether to force the creation of a new tree
     * or to attempt to open an existing tree for update (the default).
     *
     * @param keylen The length of a key in bytes. Ignored when opening an existing
     * tree.
     *
     * @param ignoreNull Controls whether to allow embedded nulls in keys. If
     * \c false (the default), then keys end with a terminating null. If
     * \c true, then all \a keylen bytes are significant. Ignored when
     * opening an existing tree.
     *
     * @param start Where to find the root node. If set to #RWNIL (the default),
     * uses the value returned by the RWFileManager::start() member
     * function. Ignored when creating a new tree.
     *
     * @param smode Sets the type of B-tree to create, allowing backwards
     * compatibility, as described in the general class description.
     * The default specifies new V6.X style B-trees. Ignored
     * when opening an existing tree.
     *
     * @param halfOrder One half the order of the B-tree (that is, one half the
     * number of entries in a node). Ignored when opening an existing
     * tree.
     *
     * @param minFill The minimum number of entries allowed in a node (must be
     * less than or equal to \a halfOrder). Ignored when opening an
     * existing tree.
     */
    RWBTreeOnDisk(RWFileManager& f, // No default possible!
                  unsigned cacheBlocks = 10,    // how many nodes to cache in RAM
                  createMode cmode = autoCreate, // use existing data in filemgr?
                  unsigned keylen = 16,         // default is 16 chars
                  bool ignoreNull = false,      // embedded nulls ok? default to "no"
                  RWoffset start = RWNIL,       // if RWNIL, use fmanager's start();
                  styleMode smode = V6Style,    // default to current style
                  unsigned halfOrder = 10,      // default is 10-20 tree
                  unsigned minFill = 10         // default == order, may be less
                 );

    /**
     * Releases memory resources, and completes any pending I/O.
     */
    ~RWBTreeOnDisk();

    /**
     * Visits all items in the collection in order, from smallest to
     * largest, calling the user-provided function pointed to by
     * \a ap with the key and value as arguments. This function should
     * have the prototype:
     *
     * @code
     * void yourApplyFunction(const char* ky,
     * RWstoredValue val, void*  x);
     * @endcode
     *
     * The function \c yourApplyFunction \b cannot change the key.
     * The value \a x can be anything and is passed through from the
     * call to this member function. This member function may throw
     * an RWFileErr exception.
     */
    void applyToKeyAndValue(RWdiskTreeApply ap, void* x) {
        apl(baseInfo.rootLoc, ap, x);
    }

    /**
     * Returns the offset of this tree's starting location within the
     * RWFileManager. This is the value you will pass to a constructor
     * as the \c start argument when you want to open one of several
     * trees stored in one managed file.
     */
    RWoffset baseLocation() const {
        return baseLoc;
    }

    /**
     * Removes all items from the collection. This member function may
     * throw an RWFileErr exception.
     */
    void clear();

    /**
     * Returns the maximum number of nodes that may currently be cached.
     */
    unsigned cacheCount() const {
        return cacheBlocks;
    }

    /**
     * Sets the number of nodes that should be cached to \a newcount.
     * Returns the old number.
     */
    unsigned cacheCount(unsigned newcount);

    /**
     * Returns \c true if the tree contains a key that is equal to the
     * string pointed to by \a key, or \c false otherwise. This member
     * function may throw an RWFileErr exception.
     */
    bool contains(const char* key) const {
        RWCString rK;
        RWstoredValue rV;
        return (findKeyAndValue(key, rK, rV) ? true : false);
    }

    /**
     * Returns the number of items in the RWBTreeOnDisk. This member
     * function may throw an RWFileErr exception.
     */
    unsigned long entries() const;                // Total entries

    /**
     * Sets the location where this RWBTreeOnDisk keeps your own application-specific
     * information to \a newlocation. Returns the previous value.
     */
    RWoffset extraLocation(RWoffset newlocation) {
        RWoffset r = baseInfo.extraLoc;
        baseInfo.extraLoc = newlocation;
        return r;
    }

    /**
     * This is a synonym for \link contains(const char*)const contains(key) \endlink.
     */
    bool find(const char* key) const {
        return contains(key);
    }

    /**
     * Returns \c true if \a key is found, otherwise \c false. If successful,
     * the found key is returned as a reference in \a retKey. This
     * member function may throw an RWFileErr exception.
     */
    bool findKey(const char* key, RWCString& retKey) const {
        RWstoredValue d;
        return findKeyAndValue(key, retKey, d);
    }

    /**
     * Returns \c true if \a key is found, otherwise \c false. If successful,
     * the found key is returned as a reference in \a retKey, and the
     * value is returned as a reference in \a foundVal. This member
     * function may throw an RWFileErr exception.
     */
    bool findKeyAndValue(const char* key, RWCString& retKey, RWstoredValue& foundVal) const;

    /**
     * Returns the value for the key that compares equal to the string
     * pointed to by \a key. Returns #RWNIL if no key is found. This
     * member function may throw an RWFileErr exception.
     */
    RWstoredValue findValue(const char* key) const {
        RWCString retK;
        RWstoredValue ret;
        return(findKeyAndValue(key, retK, ret)) ? ret : RWNIL ;
    }

    /**
     * Returns the height of the RWBTreeOnDisk. This member function
     * may throw an RWFileErr exception.
     */
    unsigned height() const;

    /**
     * Adds a key-value pair to the B-tree. Returns \c true for successful
     * insertion, \c false otherwise. This member function may throw
     * an RWFileErr exception.
     */
    bool insertKeyAndValue(const char* key, RWstoredValue val);

    /**
     * Returns \c true if this RWBTreeOnDisk contains no data, \c false
     * otherwise.
     */
    bool isEmpty() const {
        return baseInfo.rootLoc == RWNIL;
    }

    /**
     * Returns the length of the keys for this RWBTreeOnDisk. This number
     * is set when the tree is first constructed, and cannot be changed.
     */
    unsigned keyLength() const {
        return baseInfo.keylen;
    }

    /**
     * Returns the minimum number of items that may be found in any
     * non-root node in this RWBTreeOnDisk. This number is set when
     * the tree is first constructed, and cannot be changed.
     */
    unsigned minOrder() const {
        return baseInfo.minorder;
    }

    /**
     * Returns the number of bytes used by each node of this RWBTreeOnDisk.
     * This number is calculated from the length of the keys and the
     * order of the tree, and cannot be changed. We make it available
     * for your calculations about how many nodes to cache.
     */
    unsigned nodeSize() const {
        return nodeRefSize;
    }

    /**
     * Returns \c 1 if \a key is found, \c 0 otherwise.
     */
    unsigned occurrencesOf(const char* key) const {
        return contains(key) ? 1U : 0U;
    }

    /**
     * Return half the maximum number of items that may be stored in
     * any node in this RWBTreeOnDisk. This number is set when the tree
     * is first constructed and cannot be changed. This method should
     * have been renamed \c halfOrder but is still called order()
     * for backward compatibility.
     */
    unsigned order() const {
        return baseInfo.order;
    }

    /**
     * Removes the key and value pair with the key that matches
     * \a key. This member function may throw an RWFileErr exception.
     */
    void remove(const char* key) {
        RWstoredValue d;
        RWCString k;
        removeKeyAndValue(key, k, d);
    }

    /**
     * Removes the key and value pair with the key that matches \a key.
     * The removed data is returned in the reference arguments \a retKey
     * and \a retVal. The function returns \c true if the remove
     * operation succeeds, or otherwise \c false. This function
     * may throw an RWFileErr exception.
     */
    bool removeKeyAndValue(const char* key, RWCString& retKey, RWstoredValue& retVal);

    /**
     * Removes the key and value pair with key that matches \a key.
     * The removed value is returned in the reference argument
     * \a retVal. The function returns \c true if the remove operation
     * succeeds; or otherwise \c false. This function may throw
     * an RWFileErr exception.
     */
    bool removeKeyAndValue(const char* key, RWstoredValue& retVal) {
        RWCString retK;
        return removeKeyAndValue(key, retK, retVal);
    }

    /**
     * Attempts to replace the \c RWstoredValue now associated with
     * \a key by the value \a newVal. If successful, the previous value
     * is returned by reference in \a oldVal, and the method returns
     * \c true, or otherwise \c false.
     */
    bool replaceValue(const RWCString& key, const RWstoredValue newVal, RWstoredValue& oldVal);

    RWoffset rootLocation() const {
        return baseLoc;
    }

    /**
     * Changes the comparison function to \a fun and returns the old
     * function. The function should return a number less than, equal
     * to, or greater than zero, depending on whether the first argument
     * is less than, equal to or greater than the second argument,
     * respectively. The third argument is the key length. Possible
     * choices (among others) are \c std::strncmp() (the default), or
     * \c std::strnicmp() (for case-independent comparisons). If you set
     * the comparison function to behave differently after data has
     * already been stored, RWBTreeOnDisk will exhibit undefined behavior
     * that is probably incorrect.
     */
    RWdiskTreeCompare setComparison(RWdiskTreeCompare fun);

    RWstoredValue         version() {
        return (RWNIL == baseInfo.version) ? 0 : baseInfo.version;
    }

public:

    /******* used as signal during recursion unwrap ******/
    enum retStatus {more, success, ignored};

private:
    RWDiskTreeNode*       root;                   // root = first node in tree.

    // the following 2 members were used in a way that broke re-entrancy.
    // they are retained here to keep the binary image the same size.
    RWDiskTreeNode*       deadNode;               // was workNode
    RWoffset              deadWorkOff;            // was workOffset

    RWdiskTreeCompare     compareKeys;            // Compare function for keys

    // the following 3 members were used in a way that broke re-entrancy.
    // they are retained here to keep the binary image the same size.
    char*                 deadKey;                // was "moreKey"
    RWstoredValue         deadItem;               // was "moreItem"
    RWoffset              deadOffset;             // was "moreOffset"

    RWoffset              baseLoc;                // offset to baseInfo in fmgr

    // information about the nodeRef pseudo-struct:
    unsigned              nodeRefSize;

private:
    // recursive functions to: apply, delete, insert or remove
    // doApl is so we can take apply...\'s call to apl out of line... ugh.
    void                  apl(RWoffset, RWdiskTreeApply, void*);
    void                  doApl(RWoffset, RWDiskTreeNode&, RWdiskTreeApply, void*);
    void                  del(RWoffset, RWDiskTreeNode&);
    retStatus             ins(const char*, RWstoredValue, RWoffset,
                              RWoffset& workO, RWDiskTreeNode&,
                              RWCString&, RWstoredValue&, RWoffset&);
    retStatus             rem(const char* key, RWoffset start, RWCString&,
                              RWstoredValue&);
    // utility functions
    // opposite of splitNode
    void                  combineNodes(int, RWoffset, RWDiskTreeNode&, RWoffset&);
    // move a key,value pair from node at this offset left or right
    // leave the parent node in the reference arguments.
    void                  moveItLeft(int, RWoffset,
                                     RWoffset&, RWDiskTreeNode&);
    void                  moveItRight(int, RWoffset,
                                      RWoffset&, RWDiskTreeNode&);
    void                  readcache(RWoffset, RWDiskTreeNode*) const;
    void                  writecache(RWoffset, RWDiskTreeNode*);
    void                  readRoot();             // get root node from file

    // if node got too small, restore it to size by stealing from a neighbor
    retStatus             restoreNode(int, RWoffset&, RWDiskTreeNode&);

    // split a node at location loc passing new data back up call stack
    void                  splitNode(int loc, RWoffset&, RWDiskTreeNode&,
                                    RWCString&, RWstoredValue&, RWoffset&);

    void                  swapWithSuccessor(int, RWoffset&, RWDiskTreeNode&);
    /*
     * The next two functions are private to prevent copies: only one
     * RWBTreeOnDisk can look at the data file at a time. Implementing locks so
     * that multiple accessors to the same data could co-exist might be
     * interesting.
     */
    void                  operator=(const RWBTreeOnDisk&);
    RWBTreeOnDisk(const RWBTreeOnDisk&);

protected:
    RWFileManager*        fmgr;                   // knows where data is
    RWCacheManager*       cmgr;                   // optimizes reads/writes
    unsigned              cacheBlocks;            // how many nodes cmgr keeps
    struct baseInfoStruct {
        RWoffset            rootLoc;        // offset to root node in file
        RWoffset            extraLoc;       // offset to "extra" info in file
        union {
            RWstoredValue     version;        // version of this B-Tree
            RWstoredValue     cacheFlag;      // RWNIL if entries() not cached
        };
        unsigned long       entries;        // number of items in this B-Tree
        unsigned            keylen;         // length of keys
        unsigned            order;          // 1/2 max  entries per node
        unsigned            minorder;       // fewest allowed entries per node
        RWoffset            reserved1;      // In case we ever want it
        RWstoredValue       reserved2;      // ditto
        unsigned long       flags;          // holds ignoreNulls for now
    } baseInfo;
protected:
    /* ignoreNulls() prototypes getting individual flags from flags */
    bool             ignoreNulls() const {
        return baseInfo.flags & 1 ? true : false;
    }
    void                  infoInit(RWstoredValue version,
                                   unsigned keylen,
                                   unsigned order,
                                   unsigned minorder,
                                   bool ignoreNull
                                  );
    void                  infoReInit();
    void                  readInfo();
    void                  startup();
    void                  writeInfo() const;      // has version knowledge

};


#endif /*  RW_TOOLS_DISKTREE_H */
