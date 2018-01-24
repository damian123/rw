#ifndef RW_TOOLS_FACTORY_H
#define RW_TOOLS_FACTORY_H



/**********************************************************************
 *
 * RWFactory --- can create an instance of a registered class
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/factory.h#5 $
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
#include <rw/rwset.h>

class RW_TOOLS_GLOBAL RWCollectable;

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Creates an instance of an RWCollectable object,
 * given a class ID
 *
 * Class RWFactory can create an instance of an RWCollectable object,
 * given a class ID.  It does this by maintaining a table of class
 * IDs and associated "creator functions."  A creator function has
 * the prototype:
 *
 * @code
 * RWCollectable* aCreatorFunction();
 * @endcode
 *
 * This function creates an instance of a particular class.
 * For a given #RWClassID tag, the appropriate function is selected,
 * invoked and the resultant pointer returned.  Because any object
 * created this way is created off the heap, you are responsible
 * for deleting it when done.
 *
 * A one-of-a-kind global RWFactory can be accessed
 * using getRWFactory(). It is guaranteed to have creator functions
 * in it for all of the classes referenced by your program.  See
 * the section in the <i>Essential Tools Module User's Guide</i> about
 * RWFactory.
 *
 * @section synopsis Synopsis
 *
 * @code
 * typedef unsigned short RWClassID;
 * typedef RWCollectable* (*RWuserCreator)();
 * #include <rw/factory.h>
 * RWFactory* theFactory;
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 *
 * #include <rw/tools/ctdatetime.h>
 * #include <rw/factory.h>
 * #include <rw/rwbag.h>
 *
 * int main ()
 * {
 *     std::cout << "This program creates an RWBag using theFactory.\n";
 *
 *     // Dummy access to RWBag interface to stimulate
 *     // the registration of RWBag with the factory.
 *     RWBag ().clear ();
 *
 *     // Create the RWBag instance - use the special rwCreateFromFactory.
 *     RWBag* ptr = (RWBag*)(rwCreateFromFactory (__RWBAG));
 *     if (0 == ptr) {
 *         std::cout << "RWBag instance creation failed.\n";
 *         return 1;
 *     }
 *
 *     std::cout << "RWBag instance has been created.\n";
 *
 *     // Insert today's date
 *     ptr->insert (new RWCollectableDateTime (RWCollectableDateTime::setCurrentTime));
 *     std::cout << "RWBag instance contains an instance of RWCollectableDateTime.\n";
 *
 *     // First delete members ...
 *     ptr->clearAndDestroy();
 *     std::cout << "RWBag instance content cleared.\n";
 *
 *     // ... then the bag itself.
 *     delete ptr;
 *     std::cout << "RWBag instance deleted.\n";
 *
 *     return 0;
 * }
 *
 * @endcode
 *
 * Program output:
 *
 * @code
 * This program creates an RWBag using theFactory.
 * RWBag instance has been created.
 * RWBag instance contains an instance of RWCollectableDateTime.
 * RWBag instance content cleared.
 * RWBag instance deleted.
 * @endcode
 */
class RW_TOOLS_GLOBAL RWFactory : public RWSet
{

public:

    /**
     * Constructs an RWFactory.
     */
    RWFactory();

    /**
     * Calls clearAndDestroy(), which is inherited from RWSet.
     */
    ~RWFactory();

    /**
     * Adds to RWFactory the global function pointed to by \a uc,
     * which creates an instance of an object with #RWClassID \a id.
     */
    void addFunction(RWuserCreator uc, RWClassID id);

    /**
     * Allocates a new instance of the class with #RWClassID \a id
     * off the heap and returns a pointer to it.  Returns #rwnil if
     * \a id does not exist.  Because this instance is allocated <i>off
     * the heap</i>, you are responsible for deleting it when done.
     */
    RWCollectable* create(RWClassID id) const;

    /**
     * Returns from RWFactory a pointer to the global function associated
     * with #RWClassID \a id.  Returns #rwnil if \a id does not exist.
     */
    RWuserCreator getFunction(RWClassID id) const;

    /**
     * Removes from the RWFactory the global function associated with
     * #RWClassID \a id.  If \a id does not exist in the factory,
     * no action is taken.
     */
    void removeFunction(RWClassID id);

    /**
     * Adds to RWFactory the global function pointed to by \a uc,
     * which creates an instance of an object with #RWClassID \a id
     * and RWStringID \a sid.
     */
    void addFunction(RWuserCreator uc, RWClassID id, RWStringID sid);

    /**
     * Allocates a new instance of the class with RWStringID \a sid
     * off the heap and returns a pointer to it.  Returns #rwnil if
     * \a sid does not exist. Because this instance is allocated <i>off
     * the heap</i>, you are responsible for deleting it when done.
     */
    RWCollectable* create(RWStringID sid) const;

    /**
     * Returns from RWFactory a pointer to the global function associated
     * with RWStringID \a sid. Returns #rwnil if \a sid does not
     * exist.
     */
    RWuserCreator getFunction(RWStringID sid) const;

    /**
     * Removes from RWFactory the global function associated with
     * RWStringID \a sid.  If \a sid does not exist in the factory,
     * no action is taken.
     */
    void removeFunction(RWStringID sid);

    /**
     * Looks up the RWStringID associated with \a id and returns it.
     *  If there is no such association, returns  RWStringID("NoID").
     */
    RWStringID getStringID(RWClassID id) const;

    /**
     * Looks up the #RWClassID associated with \a sid and returns it.
     *  If there is no such association, returns \c __RWUNKNOWN.
     */
    RWClassID getClassID(RWStringID sid) const;

    //  #2437 - Forward relational operator calls in presence of C++ Std. Lib.

    /**
     * Returns \c true if self is a subset of \a h, that is, every
     * element of self has a counterpart in \a h which isEqual(). \par
     *
     * This operator is included to fix an inconsistency in the C++
     * language. It is not explicitly present unless you are compiling
     * with an implementation of the C++ Standard Library. It would
     * normally be inherited from RWSet.
     */
    bool operator<=(const RWFactory& h) {
        return RWSet::operator<=(h);
    }
    /**
     * @copydoc operator<=(const RWFactory&)
     */
    bool operator<=(const RWSet& h) const {
        return RWSet::operator<=(h);
    }
    /**
     * @copydoc operator<=(const RWFactory&)
     */
    bool operator<=(const RWHashTable& h) const {
        return RWSet::operator<=(h);
    }

    /**
     * @internal
     * A collection of all of the registered functions for creating and
     * serializing an RWCollectable instance.
     */
    struct RW_TOOLS_SYMBOLIC MetaFunctions {
        RWuserCreator create;
        RWCollectableSaveToRWFile saveToRWFile;
        RWCollectableSaveToRWvostream saveToRWvostream;
        RWCollectableRestoreFromRWFile restoreFromRWFile;
        RWCollectableRestoreFromRWvistream restoreFromRWvistream;
    };

private:

    /**
     * @internal
     * Register RWClassID and RWStringID with the factory along with
     * functions for creating an instance of the associated object
     * (\a create), saving the associated object (\a saveFile, \a saveStream)
     * and restoring the associated object (\a restoreFile, \a
     * restoreStream).
     */
    void addFunctions(MetaFunctions, RWClassID cid, RWStringID sid);

    /**
     * @internal
     * Return the list of functions associated with \a cid.
     */
    MetaFunctions getFunctions(RWClassID cid) const;

    /**
     * @internal
     * Return the list of functions associated with \a sid.
     */
    MetaFunctions getFunctions(RWStringID sid) const;

    friend struct RW_TOOLS_SYMBOLIC RWClassIDAssociation;
    friend struct RW_TOOLS_SYMBOLIC RWStringIDAssociation;
    friend RW_TOOLS_SYMBOLIC void
    rwAddToFactory(RWuserCreator, RWCollectableSaveToRWFile,
                   RWCollectableSaveToRWvostream,
                   RWCollectableRestoreFromRWFile,
                   RWCollectableRestoreFromRWvistream, RWClassID, RWStringID);
    friend RW_TOOLS_SYMBOLIC MetaFunctions
    rwGetFunctionsFromFactory(RWClassID);
    friend RW_TOOLS_SYMBOLIC MetaFunctions
    rwGetFunctionsFromFactory(RWStringID);
};

/**
 * @relates RWFactory
 *
 * Returns a pointer to the global RWFactory object instance, creating
 * it if necessary.
 */
RW_TOOLS_SYMBOLIC RWFactory* getRWFactory();

extern RW_TOOLS_GLOBAL RWFactory* theFactory; // The one-of-a-kind global factory.

#endif /* RW_TOOLS_FACTORY_H */
