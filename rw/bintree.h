#ifndef RW_TOOLS_BINTREE_H
#define RW_TOOLS_BINTREE_H


/**********************************************************************
 *
 * Binary tree of pointers to RWCollectable objects
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/bintree.h#1 $
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

#include <stddef.h>

#include <rw/defs.h>
#include <rw/colclass.h>
#include <rw/iterator.h>
#include <rw/mempool.h>
#include <rw/tpslist.h>
#include <rw/tqueue.h>
#include <rw/tstack.h>

// Forward declarations:
class RW_TOOLS_GLOBAL RWTreeNode;

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Represents a group of ordered elements, as sorted by a comparison
 * method, and allowing duplicates.
 *
 * Class RWBinaryTree represents a group of ordered elements, internally
 * sorted by the compareTo() function. Duplicates are allowed. An
 * object stored by an RWBinaryTree must inherit abstract base class
 * RWCollectable.
 *
 * @section synopsis Synopsis
 *
 * @code
 * typedef RWBinaryTree SortedCollection;  // Smalltalk typedef.
 * #include <rw/bintree.h>
 * RWBinaryTree bt;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWBinaryTree : public RWCollection
{

    friend class RW_TOOLS_GLOBAL RWBinaryTreeIterator;
    friend class RW_TOOLS_GLOBAL RWBinaryTreeConstIterator;

private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWBinaryTree);
    friend RWCollectable* rwCreateFN(RWBinaryTree)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns \c __RWBINARYTREE.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    /**
     * Returns the #RWClassID of this class.
     */
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWBinaryTree*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWBinaryTree*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWBinaryTree& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWBinaryTree& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWBinaryTree& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWBinaryTree& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Constructs an empty sorted collection.
     */
    RWBinaryTree();

    /**
     * Copy constructor. Constructs a shallow copy from \a t. Member
     * function balance() is called before returning.
     */
    RWBinaryTree(const RWBinaryTree& t);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed RWBinaryTree takes ownership of the
     * data owned by \a bt.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWBinaryTree(RWBinaryTree && bt);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Calls clear().
     */
    virtual ~RWBinaryTree();

    /**
     * Sets self to a shallow copy of \a bt.
     */
    RWBinaryTree& operator=(const RWBinaryTree& bt);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a bt.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWBinaryTree& operator=(RWBinaryTree && bt);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Returns \c true if self is a subset of the collection \a bt.
     * That is, every item in self must compare equal to a unique item
     * in \a bt.
     */
    bool operator<=(const RWBinaryTree& bt) const;

    /**
     * Returns \c true if self and \a bt are equivalent. That is, they
     * must have the same number of items and every item in self must
     * compare equal to a unique item in \a bt.
     */
    bool operator==(const RWBinaryTree& bt) const;

    /**
     * Special function to balance the tree. In a perfectly balanced
     * binary tree with no duplicate elements, the number of nodes from
     * the root to any external (leaf) node differs by at most one node.
     * Since this collection allows duplicate elements, a perfectly
     * balanced tree is not always possible. Preserves the order of
     * duplicate elements.
     */
    void balance();

    /**
     * Returns the number of nodes between the root node and the farthest
     * leaf. A RWBinaryTree with one entry will have a height of 1.
     * Note that the entire tree is traversed to discover this value.
     */
    size_t height() const;

    /**
     * Invokes the function pointer \a ap on each item in the collection,
     * in order, from smallest to largest. The function pointed to by \a ap
     * should take no action that could change the ordering of items in the
     * collection.
     */
    virtual void apply(RWapplyCollectable ap, void*);

    // Documented in base class.
    virtual void clear();

    // Documented in base class.
    virtual size_t entries() const;

    /**
     * Returns the first item that
     * compares equal to the item pointed to by \a target, or #rwnil
     * if no item was found.
     */
    virtual RWCollectable* find(const RWCollectable* target) const;

    /**
     * Inserts the item \a c into
     * the collection and returns it. Returns #rwnil if the insertion
     * was unsuccessful. The item \a c is inserted according to the
     * value returned by compareTo(). insert() does not automatically
     * balance the RWBinaryTree. Be careful not to call insert() on a
     * long sequence of sorted items without also calling balance()
     * since the result will be very unbalanced (and therefore inefficient).
     */
    virtual RWCollectable* insert(RWCollectable* c);

    // Documented in base class.
    virtual bool isEmpty() const {
        return root == rwnil;
    }

    // Documented in base class.
    virtual bool isEqual(const RWCollectable* a) const;

    /**
     * Returns the number of items
     * that compare equal to the item pointed to by \a target.
     */
    virtual size_t occurrencesOf(const RWCollectable* target) const;

    /**
     * Removes the first item that
     * compares equal to the object pointed to by \a target and returns
     * it. Returns #rwnil if no item was found.
     */
    virtual RWCollectable* remove(const RWCollectable* target);

    // Documented in base class.
    virtual void saveGuts(RWvostream&) const;

    // Documented in base class.
    virtual void saveGuts(RWFile&) const;

    // Documented in base class.
    virtual RWIterator* newIterator();

    // Documented in base class.
    virtual RWConstIterator* newConstIterator() const;

    /**
     * Swaps the data owned by self with the data owned by \a bt.
     */
    void swap(RWBinaryTree& bt);

protected:

    void applyChildren(const RWTreeNode*, RWapplyCollectable, void*);
    RWTreeNode* balanceChildren(size_t, RWTQueue<RWTreeNode*, RWTPtrSlist<RWTreeNode> >&);
    void deleteChildren(RWTreeNode*);
    RWCollectable* deleteNode(RWTreeNode* victim, RWTreeNode* parent);
    void insertChildrenOf(const RWTreeNode*);
    void insertNode(RWTreeNode*, const RWTreeNode*);
    void levelApply(RWapplyCollectable, void*);
    size_t nodeHeight(RWTreeNode*) const;
    void queueNodes(RWTreeNode*, RWTQueue<RWTreeNode*, RWTPtrSlist<RWTreeNode> >&, RWTQueue<RWTreeNode*, RWTPtrSlist<RWTreeNode> >&);

    //////////////////////////////////////////////////////////////////////
    // The following functions are now unused by internal code, and
    // will go away in a subsequent release.
protected:

    RWTreeNode* balanceChildren(size_t, RWTQueue<RWCollectable*, RWTPtrSlist<RWCollectable> >&);
    void balanceUnique();
    void countChildren(const RWTreeNode*, size_t&) const;

    //////////////////////////////////////////////////////////////////////
private:

    RWTreeNode* root;      // root = top-level item in tree

};

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Iterator for class RWBinaryTree.
 *
 * Iterator for class RWBinaryTree. Traverses the tree from the
 * smallest to largest element, where "smallest" and "largest"
 * are defined by the virtual function RWBinaryTree::compareTo(). Note that
 * this approach is generally less efficient than using the member function
 * RWBinaryTree::apply().
 *
 * Like all Rogue Wave iterators, the "current item" is undefined
 * immediately after construction. You must define it by using
 * #operator()() or some other (valid) operation.
 *
 * Once the iterator has advanced beyond the end of the collection
 * it is no longer valid. Continuing to use it leads to undefined
 * results.
 *
 * @section synopsis Synopsis
 *
 * @code
 * // Smalltalk typedef:
 * typedef RWBinaryTreeIterator SortedCollectionIterator;
 * #include <rw/bintree.h>
 * RWBinaryTree bt;
 * RWBinaryTreeIterator iterate(bt);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWBinaryTreeIterator : public RWIterator
{
    friend class RWBinaryTreeConstIterator;

protected:

    const RWBinaryTree* tree; // Binary tree for this iterator.
    const RWTreeNode* here; // Current node.
    RWTStack<RWTreeNode*, RWTPtrSlist<RWTreeNode> > stack;

private:

    void descendLeft();

public:

    /**
     * Constructs an iterator for an RWBinaryTree. Immediately after
     * construction, the position of the iterator is undefined until
     * positioned.
     */
    RWBinaryTreeIterator(const RWBinaryTree&);

    /**
     * Empty destructor.
     */
    virtual ~RWBinaryTreeIterator();

    /**
     * Moves iterator to the next item
     * that compares equal to the object pointed to by \a target and
     * returns it. If no item is found, returns #rwnil and the position
     * of the iterator is undefined.
     */
    virtual RWCollectable* findNext(const RWCollectable* target);

    // Documented in base class.
    virtual RWCollectable* key() const;

    // Documented in base class.
    virtual RWCollectable* operator()();

    // Documented in base class.
    virtual void reset();

};

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Iterator for class RWBinaryTree that allows no changes
 * to the values in the binary tree.
 *
 * Provides a \c const iterator for class RWBinaryTree.  Traverses
 * the tree from the smallest to the largest element, where "smallest"
 * and "largest" are defined by the virtual function RWBinaryTree::compareTo().
 * Note that this approach is generally less efficient than using
 * the member function RWBinaryTree::apply().
 *
 * Like all Rogue Wave iterators, the "current item" is undefined
 * immediately after construction. You must define it by using
 * #operator()() or some other (valid) operation.
 *
 * Once the iterator has advanced beyond the end of the collection
 * it is no longer valid. Continuing to use it leads to undefined
 * results.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/bintree.h>
 * RWBinaryTree bt;
 * RWBinaryTreeConstIterator iterate(bt);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWBinaryTreeConstIterator : public RWConstIterator
{

protected:

    const RWBinaryTree* tree; // Binary tree for this iterator.
    const RWTreeNode* here; // Current node.
    RWTStack<RWTreeNode*, RWTPtrSlist<RWTreeNode> > stack;

private:

    void descendLeft();

public:

    /**
     * Constructs a \c const iterator for an RWBinaryTree. Immediately after
     * construction, the position of the iterator is undefined until
     * positioned.
     */
    RWBinaryTreeConstIterator(const RWBinaryTree&);

    /**
     * Constructs a \c const iterator from an iterator.
     * Immediately after construction, the position of the \c const iterator is
     * undefined until positioned.
     */
    RWBinaryTreeConstIterator(const RWBinaryTreeIterator& it)
        : tree(it.tree), here(it.here), stack(it.stack) { }

    virtual ~RWBinaryTreeConstIterator();

    /**
     * Moves iterator to the
     * next item that compares equal to the object pointed to by
     * \a target and returns it. If no item is found, returns #rwnil and
     * the position of the iterator is undefined.
     */
    virtual const RWCollectable* findNext(const RWCollectable* target);

    // Documented in base class.
    virtual const RWCollectable* key() const;

    /**
     * Advances iterator to the
     * next largest element and returns a pointer to it.  Returns
     * #rwnil when the end of the collection is reached.
     */
    virtual const RWCollectable* operator()();

    // Documented in base class.
    virtual void reset();
    virtual void reset(RWBinaryTree& t) {
        tree = const_cast<const RWBinaryTree*>(&t);
        reset();
    }

    virtual const RWBinaryTree* container() const {
        return tree;
    }

};


/****************************************************************
 *                                                              *
 *                      RWTreeNode                              *
 *                                                              *
 ****************************************************************/

// Node in binary tree:
class RW_TOOLS_SYMBOLIC RWTreeNode   RWMemoryPool_OPTION
{

private:

    friend class RW_TOOLS_GLOBAL RWBinaryTree;
    friend class RW_TOOLS_GLOBAL RWBinaryTreeIterator;
    friend class RW_TOOLS_GLOBAL RWBinaryTreeConstIterator;

    RWTreeNode*           right;  // Pointer to right node.
    RWTreeNode*           left;   // Pointer to left node.
    RWCollectable*        e;      // Pointer to RWCollectable object.

private:

    // Private constructor:
    RWTreeNode(RWCollectable* a, RWTreeNode* p = rwnil, RWTreeNode* n = rwnil) {
        e = a;
        left = p;
        right = n;
    }

};

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif
#endif /* RW_TOOLS_BINTREE_H */
