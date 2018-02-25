/*
 * Definitions for class RWFactory
 *
 * $Id: factory.cpp,v 6.8 1994/07/18 20:51:00 jims Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 *
 * (c) Copyright 1989, 1990, 1991, 1992, 1993, 1994 Rogue Wave Software, Inc.
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, Rogue Wave Software, Inc., which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 * Notwithstanding any other lease or license that may pertain to, or
 * accompany the delivery of, this computer software and information, the
 * rights of the Government regarding its use, reproduction and disclosure
 * are as set forth in Section 52.227-19 of the FARS Computer
 * Software-Restricted Rights clause.
 * 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 52.227-7013.
 * 
 * This computer software and information is distributed with "restricted
 * rights."  Use, duplication or disclosure is subject to restrictions as
 * set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial
 * Computer Software-Restricted Rights (April 1985)."  If the Clause at
 * 18-52.227-74 "Rights in Data General" is specified in the contract,
 * then the "Alternate III" clause applies.
 *
 ***************************************************************************
 *
 ***************************************************************************
 *
 * $Log: factory.cpp,v $
 * Revision 6.8  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.7  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.6  1994/06/16  03:31:02  vriezen
 * Added RW_DEFINE_HOME and/or RW_DECLARE_HOME
 *
 * Revision 6.5  1994/06/15  16:55:48  jims
 * Now uses dll factory for both DLLs and Unix shared libs
 *
 * Revision 6.4  1994/05/16  17:23:30  jims
 * Remove changes meant only for version 7
 *
 * Revision 6.3  1994/05/16  17:16:06  jims
 * Port to Win32 DLLL
 *
 * Revision 6.2  1994/04/15  21:08:12  vriezen
 * Backed out (until next release) RW_DEFINE_HOME and RW_DECLARE_HOME support.
 *
 * Revision 6.1  1994/04/15  19:06:35  vriezen
 * Move all files to 6.1
 *
 * Revision 2.9  1994/03/31  23:09:20  vriezen
 * Add RW_DECLARE_HOME and RW_DEFINE_HOME for RWFunctionAssociation
 *
 * Revision 2.8  1993/11/17  02:14:27  myersn
 * fix static mutex lock, use safe constructor.
 *
 * Revision 2.7  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.6  1993/08/05  11:47:26  jims
 * Distinguish between WIN16 DLLs and WIN32 DLLs by checking
 * for __WIN16__
 *
 * Revision 2.5  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 2.4  1993/05/18  00:48:57  keffer
 * Introduced new exception handling classes
 *
 * Revision 2.3  1993/04/12  12:01:43  jims
 * Factory shared among mulitple threads per process, member functions locked
 *
 * Revision 2.2  1993/03/13  02:22:56  keffer
 * ClassID->RWClassID
 *
 * Revision 2.1  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.8   29 May 1992 09:46:46   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.7   27 May 1992 18:09:02   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.6   25 May 1992 15:25:10   KEFFER
 * userCreator -> RWuserCreator
 * 
 *    Rev 1.5   09 Mar 1992 12:15:52   KEFFER
 * Changed from "DEBUG" to "RWDEBUG".
 * 
 *    Rev 1.4   04 Mar 1992 09:17:22   KEFFER
 * nil replaced with rwnil
 * 
 *    Rev 1.3   17 Nov 1991 13:33:36   keffer
 * Do not depend on static constructor to create initial (static) factory.
 * 
 *    Rev 1.2   13 Nov 1991 11:48:00   keffer
 * Separate factory for each program instance. No longer uses RWFixedMemory.
 * 
 */

#include "rw/rwerr.h"
#include "rw/toolerr.h"
#include "rw/factory.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: factory.cpp,v $ $Revision: 6.8 $ $Date: 1994/07/18 20:51:00 $");

#if defined(_RWBUILDDLL) || defined(_RWBUILDSHARED)
# include "dllfact.h"
#endif

#if defined (_RWBUILDDLL) && defined(__WIN16__)
# include "factmgr.h"
#else
  RWFactory* theFactory = rwnil; // Pointer to the one-of-a-kind global factory
#endif

#ifdef RW_MULTI_THREAD
# include "rw/mutex.h"
  //  Mutex for the one-of-a-kind global factory
  RWMutex theFactoryLock(RWMutex::staticCtor);
#endif

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

/****************************************************************
 *								*
 * RWFunctionAssociation:					*
 * Internal class used to bind the class ID with its associated	*
 * creator function.						*
 *								*
 ***************************************************************/

struct RWExport RWFunctionAssociation : public RWCollectable {
  RW_DECLARE_HOME(RWFunctionAssociation)

  RWClassID		theID;		// Used as the key
  RWuserCreator		theFunction;

  RWFunctionAssociation(RWuserCreator uc, RWClassID id){ theFunction=uc; theID=id;}

  virtual unsigned	hash() const {return (unsigned)theID;}
  virtual RWBoolean	isEqual(const RWCollectable* c) const
    { return theID == ((const RWFunctionAssociation*)c)->theID; }
};  

RW_DEFINE_HOME(RWFunctionAssociation)

/****************************************************************
 *								*
 *		RWFactory definitions				*
 *								*
 ***************************************************************/

RWFactory::RWFactory()
{
}

RWFactory::~RWFactory()
{
  RWGUARD(theFactoryLock);
  clearAndDestroy();	// Will delete all the RWFunctionAssociation entries
}
  
void
RWFactory::addFunction(RWuserCreator uc, RWClassID id)
{
  RWGUARD(theFactoryLock);
  RWFunctionAssociation* a = new RWFunctionAssociation(uc, id);
  // Delete the association if there is already an entry:
  if( insert(a) != a ) delete a;
}

void
RWFactory::removeFunction(RWClassID id)
{
  RWGUARD(theFactoryLock);
  RWFunctionAssociation dummy(0, id); // Temporary used as a key.
  removeAndDestroy(&dummy);
}

RWuserCreator
RWFactory::getFunction(RWClassID id) const
{
#ifdef RWDEBUG
  if( id==__RWCOLLECTABLE || id==__RWCOLLECTION || id==__RWSEQUENCEABLE)
    RWTHROW(RWInternalErr(RWMessage(RWTOOL_CRABS, id, id)));
#endif
  RWGUARD(theFactoryLock);
  RWFunctionAssociation dummy(0, id);	// Temporary used as a key.
  const RWFunctionAssociation* a = (const RWFunctionAssociation*)find(&dummy);
  return a ? a->theFunction : rwnil;
}

RWCollectable*
RWFactory::create(RWClassID id) const
{
  RWuserCreator fn = getFunction(id);	// Get the creator function
  return fn ? (*fn)() : rwnil;		// Invoke it if one was found.
}

/****************************************************************
 *								*
 *			FUNCTION INTERFACE			*
 *								*
 ****************************************************************/

#ifdef RW_MULTI_THREAD
static RWMutex creationLock(RWMutex::staticCtor);
#endif

#ifndef RW_TRAILING_RWEXPORT
rwexport RWFactory* 
#else
RWFactory* rwexport
#endif
getRWFactory()
{
#if defined(__DLL__) && defined(__WIN16__)
  RWFactory*  f = rwFactoryManager.currentFactory();
  if (f==rwnil) f = rwFactoryManager.newFactory();
  return f;
#else
  RWGUARD(creationLock);  // make sure only one thread creates a new factory
  if (theFactory==rwnil) theFactory = new RWFactory;
  return theFactory;
#endif
}

void rwexport
rwAddToFactory(RWuserCreator fn, RWClassID id)
{
  RWFactory* f = getRWFactory();
  if (f) f->addFunction(fn, id);
}

RWCollectableP rwexport
rwCreateFromFactory(RWClassID id)
{
  RWCollectable* ret = rwnil;

  /* First try the process factory */
  RWFactory* f = getRWFactory();
  if ( f ) {
    ret = f->create(id);
  }
#if defined(_RWBUILDDLL) || defined(_RWBUILDSHARED)
  /* Use for any sort of DLL or shared lib */
  /* If that didn't work, then try the global factory in the DLL: */
  if (ret==rwnil)
    ret = rwCreateFromDLLFactory(id);
#endif

  return ret;
}

void rwexport
rwRemoveFromFactory(RWClassID id)
{
  RWFactory* f = getRWFactory();
  if (f) {
    f->removeFunction(id);
  }
}
