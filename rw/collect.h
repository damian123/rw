#ifndef RW_TOOLS_COLLECT_H
#define RW_TOOLS_COLLECT_H


/**********************************************************************
 *
 * Declarations for RWCollectables: an abstract base class.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/collect.h#3 $
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

#include <rw/stringid.h>
#include <rw/mempool.h>

#include <rw/tools/atomic.h>
#include <rw/tools/ristream.h>
#include <rw/tools/rostream.h>


// Some of the following global functions are needed below :

// Old version
RW_TOOLS_SYMBOLIC void  rwAddToFactory(RWuserCreator, RWClassID);
RW_TOOLS_SYMBOLIC RWCollectableP  rwCreateFromFactory(RWClassID);
RW_TOOLS_SYMBOLIC void  rwRemoveFromFactory(RWClassID);

// New version
RW_TOOLS_SYMBOLIC void rwAddToFactory(RWuserCreator, RWClassID, RWStringID);
RW_TOOLS_SYMBOLIC RWCollectableP  rwCreateFromFactory(RWStringID);
RW_TOOLS_SYMBOLIC void  rwRemoveFromFactory(RWStringID);
RW_TOOLS_SYMBOLIC RWClassID  rwGetClassID(RWStringID);
RW_TOOLS_SYMBOLIC RWStringID  rwGetStringID(RWClassID);

// Compatibility version
RW_TOOLS_SYMBOLIC void rwAddToFactory(RWuserCreator,
                                      RWCollectableSaveToRWFile,
                                      RWCollectableSaveToRWvostream,
                                      RWCollectableRestoreFromRWFile,
                                      RWCollectableRestoreFromRWvistream,
                                      RWClassID, RWStringID);

// Miscellaneous
RW_TOOLS_SYMBOLIC void  rwDestroy(RWCollectable*, void*);
RW_TOOLS_SYMBOLIC bool  rwIsEqualFun(const void*, const void*);

/**
 * @ingroup rwcollectable_classes
 *
 * The macro RW_DECLARE_ABSTRACT_COLLECTABLE_CLASS should be used in the
 * declaration of any abstract class that derives from RWCollectable.
 */
#define RW_DECLARE_ABSTRACT_COLLECTABLE_CLASS(libName,className)    \
    public:                                                         \
    virtual void* self();                                           \
    virtual const void* self() const;                               \
                                                                    \
    friend libName RWvistream&                                      \
    operator>>(RWvistream&,className*&);                            \
                                                                    \
    friend libName RWFile&                                          \
    operator>>(RWFile&,className*&);                                \
                                                                    \
    RW_INLINE_FRIEND                                                \
    RWvostream& operator<<(RWvostream& v,const className& Cl) {     \
        Cl.recursiveSaveOn(v);                                      \
        return v;                                                   \
    }                                                               \
                                                                    \
    RW_INLINE_FRIEND                                                \
    RWFile& operator<<(RWFile& f,const className& Cl) {             \
        Cl.recursiveSaveOn(f);                                      \
        return f;                                                   \
    }                                                               \
                                                                    \
    RW_INLINE_FRIEND                                                \
    RWvistream& operator>>(RWvistream& v, className& Cl) {          \
        RWCollectable::recursiveRestoreFrom(v,&Cl);                 \
        return v;                                                   \
    }                                                               \
                                                                    \
    RW_INLINE_FRIEND                                                \
    RWFile& operator>>(RWFile& f, className& Cl) {                  \
        RWCollectable::recursiveRestoreFrom(f,&Cl);                 \
        return f;                                                   \
    }


/**
 * @ingroup rwcollectable_classes
 *
 * The macro RW_DEFINE_ABSTRACT_COLLECTABLE_CLASS should be used in the
 * definition of any abstract class that derives from RWCollectable.
 */
#define RW_DEFINE_ABSTRACT_COLLECTABLE_CLASS(libName,className)           \
    void* className::self() {                                             \
        className* ptr = this;                                            \
        return ptr;                                                       \
    }                                                                     \
                                                                          \
    const void* className::self() const {                                 \
        const className *ptr = this;                                      \
        return ptr;                                                       \
    }                                                                     \
                                                                          \
    libName RWvistream&                                                   \
    operator>>(RWvistream& s, className*& pCl) {                          \
        RWCollectable* p=0;                                               \
        try {                                                             \
            RWCollectable::tryRecursiveRestore(s,p);                      \
        } catch (...) {                                                   \
            pCl = (p ? RW_STATIC_CAST(className*,(p->self())) : 0);       \
            throw;                                                        \
        }                                                                 \
        pCl = (p ? RW_STATIC_CAST(className*,(p->self())) : 0);           \
        return s;                                                         \
    }                                                                     \
                                                                          \
    libName RWFile&                                                       \
    operator>>(RWFile& f, className*& pCl)                                \
    {                                                                     \
        RWCollectable* p=0;                                               \
        try {                                                             \
            RWCollectable::tryRecursiveRestore(f,p);                      \
        } catch (...) {                                                   \
            pCl = (p ? RW_STATIC_CAST(className*,(p->self())) : 0);       \
            throw;                                                        \
        }                                                                 \
        pCl = (p ? RW_STATIC_CAST(className*,(p->self())) : 0);           \
        return f;                                                         \
    }


#define RWInit(className)     name2(RWInitCtor,className)
#define rwCreateFN(className) name2(rwCreate,className)

/**
 * @ingroup rwcollectable_classes
 *
 * The macro RW_DECLARE_COLLECTABLE_CLASS should be used in the
 * declaration of any class that derives from RWCollectable.
 *
 * @see RW_DEFINE_COLLECTABLE_CLASS_BY_ID
 * @see RW_DEFINE_COLLECTABLE_CLASS_BY_NAME
 */
#define RW_DECLARE_COLLECTABLE_CLASS(libName,className)         \
    private:                                                    \
    static RWClassID myAtom;                                    \
                                                                \
    private:                                                    \
    friend struct RWInit(className);                            \
    friend RWCollectable* rwCreateFN(className) ();             \
                                                                \
    public:                                                     \
    virtual RWCollectable* newSpecies() const;                  \
    virtual RWCollectable* copy() const;                        \
    virtual RWClassID isA() const;                              \
                                                                \
    virtual void* self();                                       \
    virtual const void* self() const;                           \
                                                                \
    static RWClassID classIsA();                                \
                                                                \
    friend libName RWvistream&                                  \
    operator>>(RWvistream& s, className*& pCl);                 \
                                                                \
    friend libName RWFile&                                      \
    operator>>(RWFile& f, className*& pCl);                     \
                                                                \
    RW_INLINE_FRIEND                                            \
    RWvostream& operator<<(RWvostream& v,const className& Cl) { \
        Cl.recursiveSaveOn(v);                                  \
        return v;                                               \
    }                                                           \
                                                                \
    RW_INLINE_FRIEND                                            \
    RWFile& operator<<(RWFile& f,const className& Cl) {         \
        Cl.recursiveSaveOn(f);                                  \
        return f;                                               \
    }                                                           \
                                                                \
    RW_INLINE_FRIEND                                            \
    RWvistream& operator>>(RWvistream& v, className& Cl) {      \
        RWCollectable::recursiveRestoreFrom(v,&Cl);             \
        return v;                                               \
    }                                                           \
                                                                \
    RW_INLINE_FRIEND                                            \
    RWFile& operator>>(RWFile& f, className& Cl) {              \
        RWCollectable::recursiveRestoreFrom(f,&Cl);             \
        return f;                                               \
    }


/************************************************************************
 *                                                                      *
 * For each class derived from RWCollectable, EXACTLY ONE macro:        *
 *   RW_DEFINE_COLLECTABLE_CLASS_BY_ID                                  *
 *   RW_DEFINE_COLLECTABLE_CLASS_BY_NAME                                *
 *                                                                      *
 * should be included in ONE ".cpp" file to implement several common    *
 * functions for classing deriving from RWCollectable.  These macros    *
 * serve several purposes:                                              *
 * 1) To provide a definition for newSpecies().                         *
 * 2) To provide a definition for copy().                               *
 * 3) To provide a definition for isA().                                *
 * 4) To define a "creator function" to be inserted into the            *
 *    one-of-a-kind global RWFactory pointed to by theFactory.          *
 * 5) For classes with RWStringIDs, to provide a map between the        *
 *    RWClassID and the RWStringID, (which also lives in the unique     *
 *    global RWFactory.                                                 *
 *                                                                      *
 * RW_DEFINE_COLLECTABLE_CLASS_BY_ID                                    *
 *    is the version 6 macro. If you use this macro,                    *
 *    you will be defining an RWClassID that is "forever" associated    *
 *    with your class. The stringID() method will return the token      *
 *    which is the class's name. Method isA() will work as before.      *
 *                                                                      *
 * RW_DEFINE_COLLECTABLE_CLASS_BY_NAME                                  *
 *    is provided so you can give your class                            *
 *    your choice of RWStringID. If you use this macro, your class may  *
 *    have a different RWClassID (returned from isA()) with each run    *
 *    of the executable, but it will be "forever" associated with the   *
 *    RWStringID. The isA() method will return some RWClassID (the      *
 *    same throughout any given run of the executable) while the method *
 *    stringID() will return the same RWStringID every time.            *
 *                                                                      *
 ************************************************************************/

#define rwInitDestructor(id, strClassName)        \
    rwRemoveFromFactory(id);                        \
    rwRemoveFromFactory(RWStringID(strClassName));

/**
 * @ingroup rwcollectable_classes
 *
 * The macro RW_DEFINE_COLLECTABLE_CLASS_BY_ID should only be used when
 * your class needs to have the same collectable ID or for compatibility
 * with older versions of Tools.h++.
 *
 * If you use this macro, you will be defining an RWClassID that is
 * forever associated with your class.
 *
 * @section example Example
 *
 * @code
 * #include <rw/collect.h>
 *
 * // See the section in the Essential Tools Module User's Guide
 * // about exporting symbols for more information about this macro.
 * #define EXPORT
 *
 * class MyCollectable1 : public RWCollectable {
 *   RW_DECLARE_COLLECTABLE_CLASS(EXPORT, MyCollectable1);
 * };
 *
 * RW_DEFINE_COLLECTABLE_CLASS_BY_ID(EXPORT,MyCollectable1,0x1000)
 * @endcode
 *
 * @see RW_DECLARE_COLLECTABLE_CLASS
 */
#define RW_DEFINE_COLLECTABLE_CLASS_BY_ID(libName,className,id)         \
    RWClassID className::myAtom = id;                                   \
                                                                        \
    RWCollectable* className::newSpecies() const {                      \
        return new className;                                           \
    }                                                                   \
                                                                        \
    RWCollectable* className::copy() const {                            \
        return new className(*this);                                    \
    }                                                                   \
                                                                        \
    RWClassID className::isA() const {                                  \
        return myAtom;                                                  \
    }                                                                   \
                                                                        \
    void* className::self() {                                           \
        className* ptr = this;                                          \
        return ptr;                                                     \
    }                                                                   \
                                                                        \
    const void* className::self() const {                               \
        const className * ptr = this;                                   \
        return ptr;                                                     \
    }                                                                   \
                                                                        \
    /* static */                                                        \
    RWClassID className::classIsA() {                                   \
        return myAtom;                                                  \
    }                                                                   \
                                                                        \
    /* Global function to create an instance of the class:*/            \
    RWCollectable* rwCreateFN(className)() {                            \
        return new className;                                           \
    }                                                                   \
                                                                        \
    /* Static constructor to insert above into factory: */              \
    struct RWInit(className) {                                          \
        RWInit(className)();                                            \
        ~RWInit(className)();                                           \
    };                                                                  \
                                                                        \
    RWInit(className)::RWInit(className)() {                            \
        rwAddToFactory(rwCreateFN(className), id, RWSTRINGIFY(className)); \
    }                                                                   \
                                                                        \
    RWInit(className)::~RWInit(className)() {                           \
        rwInitDestructor(id, RWSTRINGIFY(className));                   \
    }                                                                   \
                                                                        \
    RWInit(className) name2(rwDummy,className);                         \
                                                                        \
    libName RWvistream&                                                 \
    operator>>(RWvistream& s, className*& pCl) {                        \
        RWCollectable* p=0;                                             \
        try {                                                           \
            RWCollectable::tryRecursiveRestore(s,p);                    \
        } catch (...) {                                                 \
            pCl = (p ? RW_STATIC_CAST(className*,(p->self())) : 0);     \
            throw;                                                      \
        }                                                               \
        pCl = (p ? RW_STATIC_CAST(className*,(p->self())) : 0);         \
        return s;                                                       \
    }                                                                   \
                                                                        \
    libName RWFile&                                                     \
    operator>>(RWFile& f, className*& pCl) {                            \
        RWCollectable* p=0;                                             \
        try {                                                           \
            RWCollectable::tryRecursiveRestore(f,p);                    \
        } catch (...) {                                                 \
            pCl = (p ? RW_STATIC_CAST(className*,(p->self())) : 0);     \
            throw;                                                      \
        }                                                               \
        pCl = (p ? RW_STATIC_CAST(className*,(p->self())) : 0);         \
        return f;                                                       \
    }

#define rwInitNamedDestructor(id, str)        \
    rwRemoveFromFactory(id);                    \
    rwRemoveFromFactory(str);

/**
 * @ingroup rwcollectable_classes
 *
 * The macro RW_DEFINE_COLLECTABLE_CLASS_BY_NAME should be used in the
 * definition of any class that derives from RWCollectable that doesn't
 * use RW_DEFINE_COLLECTABLE_CLASS_BY_ID.
 *
 * If you use this macro, your class may have a different RWClassID
 * with each run, but it will be forever associated with the given
 * RWStringID.
 *
 * @section example Example
 *
 * @code
 * #include <rw/collect.h>
 *
 * // See the section in the Essential Tools Module User's Guide
 * // about exporting symbols for more information about this macro.
 * #define EXPORT
 *
 * class MyCollectable2 : public RWCollectable {
 *   RW_DECLARE_COLLECTABLE_CLASS(EXPORT, MyCollectable2);
 * };
 *
 * RW_DEFINE_COLLECTABLE_CLASS_BY_NAME(EXPORT, MyCollectable2, "Second Collectable")
 * @endcode
 *
 * @see RW_DECLARE_COLLECTABLE_CLASS
 */
#define RW_DEFINE_COLLECTABLE_CLASS_BY_NAME(libName,className,str)      \
    RWClassID className::myAtom = RWCollectable::nextAtom();            \
                                                                        \
    RWCollectable* className::newSpecies() const {                      \
        return new className;                                           \
    }                                                                   \
                                                                        \
    RWCollectable* className::copy() const {                            \
        return new className(*this);                                    \
    }                                                                   \
                                                                        \
    RWClassID className::isA() const {                                  \
        return myAtom;                                                  \
    }                                                                   \
                                                                        \
    void* className::self() {                                           \
        className* ptr = this;                                          \
        return ptr;                                                     \
    }                                                                   \
                                                                        \
    const void* className::self() const {                               \
        const className * ptr = this;                                   \
        return ptr;                                                     \
    }                                                                   \
                                                                        \
    /*static*/                                                          \
    RWClassID className::classIsA() {                                   \
        return myAtom;                                                  \
    }                                                                   \
                                                                        \
    /* Global function to create an instance of the class:*/            \
    RWCollectable* rwCreateFN(className)() {                            \
        return new className;                                           \
    }                                                                   \
                                                                        \
    /* Static constructor to insert above into factory: */              \
    struct RWInit(className)                                            \
    {                                                                   \
        RWInit(className)();                                            \
        ~RWInit(className)();                                           \
    };                                                                  \
                                                                        \
    RWInit(className)::RWInit(className)() {                            \
        rwAddToFactory(rwCreateFN(className),className::myAtom,str);    \
    }                                                                   \
                                                                        \
    RWInit(className)::~RWInit(className)() {                           \
        rwInitNamedDestructor(className::myAtom,str);                   \
    }                                                                   \
                                                                        \
    RWInit(className) name2(rwDummy,className);                         \
                                                                        \
    libName RWvistream&                                                 \
    operator>>(RWvistream& s, className*& pCl) {                        \
                                                                        \
        RWCollectable* p=0;                                             \
        try {                                                           \
            RWCollectable::tryRecursiveRestore(s,p);                    \
        } catch (...) {                                                 \
            pCl = (p ? RW_STATIC_CAST(className*,(p->self())) : 0);     \
            throw;                                                      \
        }                                                               \
        pCl = (p ? RW_STATIC_CAST(className*,(p->self())) : 0);         \
        return s;                                                       \
    }                                                                   \
                                                                        \
    libName RWFile&                                                     \
    operator>>(RWFile& f, className*& pCl) {                            \
                                                                        \
        RWCollectable* p=0;                                             \
        try {                                                           \
            RWCollectable::tryRecursiveRestore(f,p);                    \
        } catch (...) {                                                 \
            pCl = (p ? RW_STATIC_CAST(className*,(p->self())) : 0);     \
            throw;                                                      \
        }                                                               \
        pCl = (p ? RW_STATIC_CAST(className*,(p->self())) : 0);         \
        return f;                                                       \
    }

/**
 * @internal
 * @ingroup rwcollectable_classes
 *
 * The macro RW_DEFINE_COLLECTABLE_CLASS_COMPAT allows an RWCollectable
 * serialized with one collectable ID to be restored into an RWCollectable
 * with a different collectable ID via custom save and restore functions.
 *
 * Unlike the other RW_DEFINE_COLLECTABLE macros,
 * RW_DEFINE_COLLECTABLE_CLASS_COMPAT is not used in association with
 * RW_DECLARE_COLLECTABLE_CLASS. Instead, it assumes that the class
 * specified is already associated with a different RWClassID and RWStringID
 * via existing RW_DECLARE_COLLECTABLE and RW_DEFINE_COLLECTABLE instances.
 * This RW_DEFINE_COLLECTABLE is in addition to those, and is intended to
 * provide compatibility with an existing serialized format.
 */
#define RW_DEFINE_COLLECTABLE_CLASS_COMPAT(libName,className,cid,sid,   \
                                           crt,stf,sts,rff,rfs)         \
                                                                        \
    /* Static constructor to insert above into factory: */              \
    struct RWInit(className) {                                          \
        RWInit(className)();                                            \
        ~RWInit(className)();                                           \
    };                                                                  \
                                                                        \
    RWInit(className)::RWInit(className)() {                            \
        rwAddToFactory(crt, stf, sts, rff, rfs, cid, sid);              \
    }                                                                   \
                                                                        \
    RWInit(className)::~RWInit(className)() {                           \
        rwInitDestructor(cid, sid);                                     \
    }                                                                   \
                                                                        \
    RWInit(className) name2(rwDummy,className);


/**
 * @ingroup rwcollectable_classes
 *
 * @brief Contains virtual functions for identifying, hashing, comparing,
 * storing and retrieving collectable objects
 *
 * Class RWCollectable is an abstract base class for collectable
 * objects. This class contains virtual functions for identifying,
 * hashing, comparing, storing and retrieving collectable objects.
 * While these virtual functions have simple default definitions,
 * objects that inherit this base class typically redefine
 * one or more of them.
 *
 * @section synopsis Synopsis
 *
 * @code
 * typedef RWCollectable Object;  // Smalltalk typedef
 * #include <rw/collect.h>
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWCollectable
{
private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWCollectable);
    friend RWCollectable* rwCreateFN(RWCollectable)();

public:

    /**
     * Allocates a new object off the heap of the same type as self
     * and returns a pointer to it. You are responsible for deleting
     * the object when done with it.
     */
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns a class identification number (with a typedef of <tt> unsigned
     * short</tt>). The default definition returns \c __RWCOLLECTABLE.
     * Identification numbers greater than or equal to 0x8000 (hex)
     * are reserved for Rogue Wave objects. User-defined classes should
     * define isA() to return a number between 0 and 0x7FFF.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    /**
     * Returns the #RWClassID of this class.
     */
    static RWClassID classIsA();

    /**
     * Looks at the next object on the input stream or RWFile, respectively,
     * and either creates a new object of the proper type off the heap
     * and returns a pointer to it, or else returns a pointer to a previously
     * read instance. Recursively calls the virtual function restoreGuts(),
     * taking duplicate objects into account. If an object is created
     * off the heap, then you are responsible for deleting it. For more
     * information, see the discussion on Persistence in the <i>Essential Tools
     * Module User's Guide</i>. \par
     *
     * Various exceptions that could be thrown are RWInternalErr (if the
     * RWFactory does not know how to make the object), and RWExternalErr
     * (corrupted stream or file). If an exception is thrown during this call,
     * the pointer to the partly restored object may be lost, creating a
     * memory leak. For this reason, you may prefer to use the static
     * methods tryRecursiveRestore().
     */
    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWCollectable*& pCl);

    /**
     * @copydoc operator>>(RWvistream&,RWCollectable*&)
     */
    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWCollectable*& pCl);

    /**
     * Saves the object \a obj to a virtual stream.
     * Recursively calls the virtual function saveGuts(),
     * taking duplicate objects into account. See the discussion on Persistence
     * in the <i>Essential Tools Module User's Guide</i> for more information.
     */
    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWCollectable& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    /**
     * Saves the object \a obj to an RWFile.
     * Recursively calls the virtual function saveGuts(),
     * taking duplicate objects into account. See the discussion on Persistence
     * in the <i>Essential Tools Module User's Guide</i> for more information.
     */
    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWCollectable& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    /**
     * Restores an object inheriting from RWCollectable into \a obj
     * from a virtual stream, replacing the previous contents of \a obj.
     * Recursively calls the virtual function restoreGuts(),
     * taking duplicate objects into account.
     * For more information, see the discussion on Persistence in the
     * <i>Essential Tools Module User's Guide</i>.
     *
     * Various exceptions that could be thrown are RWInternalErr (if the
     * RWFactory does not know how to make the object), and RWExternalErr
     * (corrupted stream or file).
     */
    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWCollectable& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    /**
     * Restores an object inheriting from RWCollectable into \a obj
     * from an RWFile, replacing the previous contents of \a obj.
     * Recursively calls the virtual function restoreGuts(),
     * taking duplicate objects into account.
     * For more information, see the discussion on Persistence in the
     * <i>Essential Tools Module User's Guide</i>.
     *
     * Various exceptions that could be thrown are RWInternalErr (if the
     * RWFactory does not know how to make the object), and RWExternalErr
     * (corrupted stream or file).
     */
    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWCollectable& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:
    RWCollectable();

    /**
     * All functions that inherit class RWCollectable have virtual
     * destructors. This allows them to be deleted by such member functions as
     * removeAndDestroy() without knowing their type.
     */
    virtual ~RWCollectable();

    /**
     * Returns the number of bytes used by the virtual function
     * \link saveGuts(RWFile&)const saveGuts(RWFile&) \endlink
     * to store an object. Typically, this involves
     * adding up the space required to store all primitives, plus the
     * results of calling recursiveStoreSize() for all objects inheriting
     * from RWCollectable. See the <i>Essential Tools Module User's Guide</i>
     * for details.
     */
    virtual RWspace binaryStoreSize() const;

    /**
     * The function compareTo() is necessary to sort the items in a
     * collection. If \c p1 and \c p2 are pointers to RWCollectable
     * objects, the statement:
     *
     * @code
     * p1->compareTo(p2);
     * @endcode
     *
     * should return:
     *
     *   0   if \c *p1 "is equal to" \c *p2;
     *
     *  >0   if \c *p1 is "larger" than \c *p2;
     *
     *  <0   if \c *p1 is "smaller" than \c *p2.
     *
     * Note that the meaning of "is equal to," "larger" and "smaller"
     * is left to the user. The default definition provided by the base
     * class is based on the addresses, i.e.,
     *
     * @code
     * return this == p2 ? 0 : (this > p2 ? 1 : -1);
     * @endcode
     *
     * and is probably not very useful.
     */
    virtual int compareTo(const RWCollectable*) const;

    /**
     * Returns a hash value. This function is necessary for collection
     * classes that use hash table look-up. The default definition provided
     * by the base class hashes the object's address:
     *
     * @code
     * return (unsigned)this;
     * @endcode
     *
     * It is important that the hash value be the same for all objects
     * that return \c true to isEqual().
     */
    virtual unsigned hash() const;

    /**
     * Returns \c true if the collectable object "matches" the object
     * at address \a t. The default definition is:
     *
     * @code
     * return this == t;
     * @endcode
     *
     * i.e., both objects have the same address (a test for identity).
     * The definition may be redefined in any consistent way.
     */
    virtual bool isEqual(const RWCollectable* t) const;

    /**
     * Returns the number of bytes required to store an #rwnil pointer
     * in an RWFile.
     */
    static RWspace nilStoreSize();

    /**
     * Reads an object's state from a binary file, using class RWFile,
     * replacing the previous state.
     */
    virtual void restoreGuts(RWFile&);

    /**
     * Reads an object's state from an input stream, replacing the previous
     * state.
     */
    virtual void restoreGuts(RWvistream&);

    /**
     * Writes an object's state to a binary file, using class RWFile.
     */
    virtual void saveGuts(RWFile&) const;

    /**
     * Writes an object's state to an output stream.
     */
    virtual void saveGuts(RWvostream&) const;

    /**
     * Returns the number of bytes required to store the object using
     * the global operator:
     *
     * @code
     * RWFile& operator<<(RWFile&, const RWCollectable&);
     * @endcode
     *
     * Recursively calls binaryStoreSize(), taking duplicate objects
     * into account.
     */
    RWspace recursiveStoreSize() const;

    static
    RWCollectable* recursiveRestoreFrom(RWFile&, RWCollectable* = 0);

    static
    RWCollectable* recursiveRestoreFrom(RWvistream&, RWCollectable* = 0);

    static
    void tryRecursiveRestore(RWFile&, RWCollectable*&);

    static
    void tryRecursiveRestore(RWvistream&, RWCollectable*&);

    void recursiveSaveOn(RWFile&) const;
    void recursiveSaveOn(RWvostream&) const;

    /**
     * Returns the identification string for the class. Acts virtual,
     * although it is not. See the discussion on RWStringID in the <i>Essential
     * Tools Module User's Guide</i> for more information on how to make
     * a non-virtual function act like a virtual function.
     */
    RWStringID stringID() const {
        return rwGetStringID(isA());
    }

    /**
     * Returns the result of looking up the #RWClassID associated
     * with \a name in the global RWFactory.
     */
    static
    RWClassID classID(const RWStringID& name) {
        return rwGetClassID(name);
    }

    /**
     * Returns \c true if \a id is the #RWClassID that is associated
     * with an RWCollectable class that has a programmer-chosen #RWStringID.
     */
    static
    bool isAtom(RWClassID id);

protected:

    inline static RWClassID nextAtom() {
        RWClassID next = (RWClassID)(atomBottom + RWCollectable::currentAtom_++);
        // the new atom must be in range [atomBottom,atomTop)
        RW_ASSERT(atomBottom <= next && next < atomTop);
        return next;
    }

private:

#ifdef RW_MULTI_THREAD
    static RWTAtomic<RWClassID> currentAtom_;
#else
    static RWClassID currentAtom_;
#endif

    // the following are here to provide some name scoping
    static const RWClassID atomBottom; // "smallest unused" RWClassID
    static const RWClassID atomTop; // "largest unused" RWClassID
    static const long tool6Magic; // was MAGIC_CONSTANT (for RWFile)
    static const RWClassID dupStridMarker; // seen this RWStringID before
    static const RWClassID stridMarker; // persisting RWStringID first time
    static const RWClassID fileRefFlag; // RWFile: seen this object before
};


RW_TOOLS_SYMBOLIC RWvostream&
operator<<(RWvostream& vstream, const RWCollectable* p);

RW_TOOLS_SYMBOLIC RWFile&
operator<<(RWFile& file, const RWCollectable* p);

RW_TOOLS_SYMBOLIC RWCollectableP
getRWNilCollectable();

#define RWnilCollectable (getRWNilCollectable())

#endif /* RW_TOOLS_COLLECT_H */
