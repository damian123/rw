#ifndef RW_TOOLS_BTREE_H
#define RW_TOOLS_BTREE_H



/**********************************************************************
 *
 * RWBTree -- in memory B-Tree.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/btree.h#1 $
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
#include <rw/colclass.h>
#include <rw/mempool.h>

// forward declaration
class RW_TOOLS_GLOBAL RWBTree;

const unsigned rworder  = 50; /* realistic "production" value */
const unsigned rworder2 = 2 * rworder;


/****************************************************************
 *                                                              *
 *                      RWBTreeNode                             *
 *                                                              *
 ****************************************************************/


class RW_TOOLS_GLOBAL RWBTreeNode  RWMemoryPool_OPTION
{

    friend class RW_TOOLS_GLOBAL RWBTree;

    RWBTreeNode();                        // Private constructors.
    RWBTreeNode(RWCollectable*);

    unsigned      counter;                // count of [rworder2] fields in use.

    RWCollectableP key[rworder2];         // Array of pointers keys.
    RWBTreeNode*   next[rworder2 + 1];     // Array of pointers to child nodes.

    unsigned binarySearch(const RWCollectable*) const;
    void initialize();
    bool subSetOf(const RWBTree& bt) const;

};

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Represents a group of ordered elements not accessible
 * by an external key, and for which duplicates are not allowed.
 *
 * Class RWBTree represents a group of ordered elements, not accessible
 * by an external key. Duplicates are not allowed. An object stored
 * by class RWBTree must inherit abstract base class RWCollectable.
 * The elements are ordered internally according to the value
 * returned by virtual function RWCollectable::compareTo().
 *
 * This class has certain advantages over class RWBinaryTree. First,
 * the B-tree is automatically \e balanced. (With class RWBinaryTree,
 * you must call member function balance() explicitly to balance
 * the tree.) Nodes are never allowed to have less than a certain
 * number of items (called the \e order). The default order
 * is 50, but may be changed by resetting the value of the static
 * constant \c order in the header file \c <rw/btree.h> and recompiling.
 * Larger values result in shallower trees, but less efficient
 * use of memory.
 *
 * Because many keys are held in a single node, class RWBTree also
 * tends to fragment memory less.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/btree.h>
 * RWBTree a;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWBTree : public RWCollection
{
private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWBTree);
    friend RWCollectable* rwCreateFN(RWBTree)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns \c __RWBTREE.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    /**
     * Returns the #RWClassID of this class.
     */
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWBTree*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWBTree*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWBTree& Cl) {
        Cl.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWBTree& Cl) {
        Cl.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWBTree& Cl) {
        RWCollectable::recursiveRestoreFrom(v, &Cl);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWBTree& Cl) {
        RWCollectable::recursiveRestoreFrom(f, &Cl);
        return f;
    }

public:

    /**
     * Constructs an empty B-tree.
     */
    RWBTree();

    /**
     * Calls clear().
     */
    virtual ~RWBTree();

    /**
     * Constructs self as a shallow copy of \a btr.
     */
    RWBTree(const RWBTree& btr);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed RWBTree takes ownership of the
     * data owned by \a btr.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWBTree(RWBTree && btr);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Sets self to a shallow copy of \a btr.
     */
    RWBTree& operator=(const RWBTree& btr);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a btr.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWBTree& operator=(RWBTree && btr);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Returns \c true if self is a subset of \a btr. That is, for every
     * item in self, there must be an item in \a btr that compares equal.
     */
    bool  operator<=(const RWBTree& btr) const;

    /**
     * Returns \c true if self and \a btr are equivalent. That is, they
     * must have the same number of items and for every item in self,
     * there must be an item in \a btr that compares equal.
     */
    bool  operator==(const RWBTree& btr) const;

    /**
     * Returns the height of the tree, defined as the number of nodes
     * traversed while descending from the root node to an
     * external (leaf) node.
     */
    unsigned height() const;

    /**
     * Invokes the function pointer \a ap on each item in the collection, in
     * order, from smallest to largest. This supplied function should not do
     * anything to the items that could change the ordering of the collection.
     */
    virtual void apply(RWapplyCollectable ap, void*);

    // Documented in base class.
    virtual void clear();

    // Documented in base class.
    virtual size_t entries() const;

    /**
     * The first item that compares equal to the object pointed to by
     * \a target is returned or #rwnil if no item is found.
     */
    virtual RWCollectable* find(const RWCollectable* target) const;

    /**
     * Inserts the item \a c into the collection and returns it. The
     * item \a c is inserted according to the value returned by
     * compareTo(). If an item is already in the collection that
     * isEqual() to \a c, then the old item is returned and the new
     * item is not inserted. Returns #rwnil if the insertion
     * was unsuccessful.
     */
    virtual RWCollectable* insert(RWCollectable* c);

    // Documented in base class.
    virtual bool isEqual(const RWCollectable* a) const;

    // Documented in base class.
    virtual bool isEmpty() const {
        return root_ == rwnil;
    }

    /**
     * Returns the number of items that compare equal to \a target.
     * Since duplicates are not allowed, this function can only
     * return 0 or 1.
     */
    virtual size_t occurrencesOf(const RWCollectable* target) const;

    /**
     * Removes and returns the first item that compares equal to the
     * object pointed to by \a target. Returns #rwnil if no item was
     * found.
     */
    virtual RWCollectable* remove(const RWCollectable* target);

    /**
     * Returns #rwnil
     */
    virtual RWIterator* newIterator() {
        return 0;
    }

    /**
     * Returns #rwnil
     */
    virtual RWConstIterator* newConstIterator() const {
        return 0;
    }

    /**
     * Swaps the data owned by self with the data owned by \a btr.
     */
    void swap(RWBTree& btr);

private:

    RWBTreeNode* root_; // root = first node in tree.

    size_t size_; // number of items in tree.

protected:

    // Apply to all children
    void apl(RWBTreeNode*, RWapplyCollectable, void*);

    // Delete all children.
    void del(RWBTreeNode*);

    // insert recursively
    int ins(RWCollectable* a, RWBTreeNode*,
            RWCollectable*&, RWBTreeNode*&);

    // remove all children of this node
    int rem(const RWCollectable* a, RWBTreeNode*, RWCollectable*&);
};


#endif /* RW_TOOLS_BTREE_H */
