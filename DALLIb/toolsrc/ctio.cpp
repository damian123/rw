/*
 * Definitions related to I/O for class RWCollectable
 *
 * $Id: ctio.cpp,v 6.5 1994/07/18 20:51:00 jims Exp $
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
 *
 ***************************************************************************
 *
 * $Log: ctio.cpp,v $
 * Revision 6.5  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.4  1994/07/14  15:39:36  jims
 * Add #ifdefs for RW_TRAILING_RWEXPORT
 *
 * Revision 6.3  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/06/13  23:43:09  jims
 * getRWNilCollectable now defined for all builds.  Creates a new
 * RWCollectable on first reference.
 *
 * Revision 6.1  1994/04/15  19:06:12  vriezen
 * Move all files to 6.1
 *
 * Revision 2.13  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.12  1993/08/05  11:47:26  jims
 * Distinguish between WIN16 DLLs and WIN32 DLLs by checking
 * for __WIN16__
 *
 * Revision 2.11  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 2.10  1993/05/18  00:48:57  keffer
 * Introduced new exception handling classes
 *
 * Revision 2.9  1993/04/23  00:44:27  keffer
 * Removed unused variable nRead.
 *
 * Revision 2.8  1993/04/22  20:39:12  keffer
 * Persistence operators now store and restore nil pointers.
 *
 * Revision 2.7  1993/04/12  11:59:38  jims
 * Read and Store tables now thread-specific in MT Situation
 *
 * Revision 2.6  1993/04/09  02:48:28  keffer
 * Added support for restoring into a preallocated object.
 *
 * Revision 2.5  1993/03/17  21:21:15  keffer
 * Return type of binaryStoreSize is now RWspace
 *
 * Revision 2.4  1993/03/13  02:29:48  keffer
 * ClassID->RWClassID
 *
 * Revision 2.3  1993/03/13  01:09:40  keffer
 * near->rwnear
 *
 * Revision 2.2  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.1  1992/11/15  21:51:11  keffer
 * Removed old V1.2 stream support
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.12   29 May 1992 09:46:42   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.11   28 May 1992 15:35:14   KEFFER
 * Introduced RWhashAddress() for identity hashes.
 * 
 *    Rev 1.10   27 May 1992 18:08:58   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.9   25 May 1992 15:34:42   KEFFER
 * saveOn -> recursiveSaveOn; restoreFrom -> recursiveRestoreFrom.
 * 
 *    Rev 1.8   04 Mar 1992 09:17:20   KEFFER
 * nil replaced with rwnil
 * 
 *    Rev 1.7   18 Feb 1992 19:28:08   KEFFER
 * shallowStoreSize() now includes magic number in total count.
 * 
 *    Rev 1.6   28 Jan 1992 18:48:46   KEFFER
 * RWnilCollectable is now appended rather that set to readTable(0).
 * 
 *    Rev 1.5   13 Nov 1991 12:56:24   keffer
 * 
 *    Rev 1.4   13 Nov 1991 12:15:28   keffer
 * NOCREATE prints hex version of ID as well.
 * 
 *    Rev 1.3   13 Nov 1991 11:47:18   keffer
 * Store and read tables now handled by a manager.
 * 
 *    Rev 1.2   29 Oct 1991 13:57:54   keffer
 * Improved error handling.
 * 
 */

#include "rw/collect.h"
#include "rw/factory.h"
#include "rw/vstream.h"
#include "rw/rwfile.h"
#include "rw/rwerr.h"
#include "rw/toolerr.h"
#include "rwstore.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: ctio.cpp,v $ $Revision: 6.5 $ $Date: 1994/07/18 20:51:00 $");

#if (defined(__WIN16__) && defined(__DLL__)) || defined(RW_MULTI_THREAD)
# include "storemgr.h"
# include "readmgr.h"
#else
  static RWStoreTable* theStoreTable = rwnil;
  static RWReadTable*  theReadTable  = rwnil;
#endif

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

const RWClassID REFFLAG   = 0xFFFF; // Signal reference, instead of ClassID
const long MAGIC_CONSTANT = 0x8888; // Safety check on binary files.

/*
 * For Win16 DLLs, getRWNilCollectable is defined in instdata.cpp so
 * that each task gets its own.  Otherwise:
 */  
#if !(defined(__DLL__) && defined(__WIN16__))

static RWCollectable* theNilCollectable;   // Static init to 0

RWCollectableP rwexport getRWNilCollectable()
{
  if (!theNilCollectable) theNilCollectable = new RWCollectable;
  return theNilCollectable;
}
#endif

/********************************************************
 *							*
 *	STORE AND READ TABLE MANAGEMENT ROUTINES	*
 *							*
 ********************************************************/

static RWStoreTable* rwnear
getStoreTable()
{
#if (defined(__DLL__) && defined(__WIN16__)) || defined(RW_MULTI_THREAD)
  RWStoreTable* theStoreTable = rwStoreManager.currentStoreTable();
#endif
  return theStoreTable;
}

static RWStoreTable* rwnear
newStoreTable()
{
#if (defined(__DLL__) && defined(__WIN16__)) || defined(RW_MULTI_THREAD)
  RWStoreTable* theStoreTable = rwStoreManager.newStoreTable();
#else
  theStoreTable = new RWStoreTable;
#endif
  return theStoreTable;
}

static void rwnear
freeStoreTable()
{
#if (defined(__DLL__) && defined(__WIN16__)) || defined(RW_MULTI_THREAD)
  rwStoreManager.freeValue();
#else
  delete theStoreTable;
  theStoreTable = rwnil;
#endif
}

static RWReadTable* rwnear
getReadTable()
{
#if (defined(__DLL__) && defined(__WIN16__)) || defined(RW_MULTI_THREAD)
  RWReadTable* theReadTable = rwReadManager.currentReadTable();
#endif
  return theReadTable;
}

static RWReadTable* rwnear
newReadTable()
{
#if (defined(__DLL__) && defined(__WIN16__)) || defined(RW_MULTI_THREAD)
  RWReadTable* theReadTable = rwReadManager.newReadTable();
#else
  theReadTable = new RWReadTable;
#endif
  return theReadTable;
}

static void rwnear
freeReadTable()
{
#if (defined(__DLL__) && defined(__WIN16__)) || defined(RW_MULTI_THREAD)
  rwReadManager.freeValue();
#else
  delete theReadTable;
  theReadTable = rwnil;
#endif
}

 /***************************************************************
 *								*
 *		 recursiveStoreSize() definition		*
 *								*
 ****************************************************************/

RWspace
RWCollectable::recursiveStoreSize() const
{
  RWspace total = 0;
  int objectNum;
  RWBoolean firstRecursion = FALSE;	// Detect whether this recursion is the first

  RWStoreTable* storeTable = getStoreTable();

  if(storeTable == rwnil){
    storeTable = newStoreTable();
    firstRecursion = TRUE;
    total = sizeof(MAGIC_CONSTANT);
  }

  if(storeTable->add(this, objectNum))
    total += sizeof(RWClassID) + binaryStoreSize();
  else
    total += sizeof(REFFLAG) + sizeof(objectNum);

  if (firstRecursion) {
    freeStoreTable();
  }
  return total;
}

 /***************************************************************
 *								*
 *	 	recursiveSaveOn() definition			*
 *								*
 ****************************************************************/

void
RWCollectable::recursiveSaveOn(RWvostream& strm) const
{
  int objectNum;
  RWBoolean firstRecursion = FALSE;	// Detect whether this recursion is the first

  RWStoreTable* storeTable = getStoreTable();

  if(storeTable == rwnil){
    storeTable = newStoreTable();
    firstRecursion = TRUE;
  }

  if(storeTable->add(this, objectNum)){
    // Object has not been previously stored.
    RWClassID clid = isA();
    strm << ':' << clid << '{';	// Store the ClassID
    saveGuts(strm);	 	// Store the guts of the object.
    strm << '}';		// Closing brace
  }
  else 
    strm << '@' << objectNum; // Object has been previously stored.  Just output a reference

  if (firstRecursion) {
    freeStoreTable();
  }
}

void
RWCollectable::recursiveSaveOn(RWFile& file) const
{
  int objectNum;
  RWBoolean firstRecursion = FALSE;	// Detect whether this recursion is the first
  
  RWStoreTable* storeTable = getStoreTable();
  
  if(storeTable == rwnil){
    storeTable = newStoreTable();
    file.Write(MAGIC_CONSTANT);
    firstRecursion = TRUE;
  }
  
  if(storeTable->add(this, objectNum)){
    file.Write(isA());		// Object has not been previously stored.
    saveGuts(file);		// Write the guts of the object.
  }
  else {
    file.Write(REFFLAG);	// Object has been previously stored.  Use a flag.
    file.Write(objectNum);	// Just write the object number.
  }
  
  if (firstRecursion) {
    freeStoreTable();
  }
}

 /***************************************************************
 *								*
 *		 recursiveRestoreFrom() definitions		*
 *								*
 ****************************************************************/


RWCollectable*
RWCollectable::recursiveRestoreFrom(RWvistream& strm, RWCollectable* obj)
{
  int            objectNum;
  RWClassID      clID;
  RWReadTable*   readTable = rwnil;
  RWBoolean      firstRecursion = FALSE; // Record whether this is the first recursion.
  char           refFlag, brace;

  if( !strm.good() ) return rwnil;	// Do not proceed if we have a bad input stream.

  strm >> refFlag;
  if( strm.eof() ) return rwnil;
  if( !strm.good() ) goto badstream;

  readTable = getReadTable();

  if(readTable == rwnil)
  {
    readTable       = newReadTable();
    readTable->append(RWnilCollectable);// Object 0 is always nil object
    firstRecursion  = TRUE;
  }

  if(refFlag == '@')
  {
    // Object has already been read.  In this case, the user cannot
    // have supplied an address, unless it matches what is in the
    // readTable.
    strm >> objectNum;
    if( !strm.good() ) goto badstream;

    // Check for a bad object number or an object address
    // that does not match what is in the readTable:
    if (objectNum >= readTable->entries() ||
	(obj && obj != (*readTable)(objectNum)) )
      RWTHROW(RWInternalErr(RWMessage(RWTOOL_REF)));
    else
      obj = (*readTable)(objectNum);

    if (obj==RWnilCollectable)	// Detect the nil object and convert it to a nil pointer
      obj = rwnil;
  }
  else if (refFlag == ':')
  {
    // Object has not been read.  

    strm >> clID >> brace;		// Get the classID & opening brace.

    if( strm.good() && brace == '{' )	// Check stream state.
    {				
      // If the user has not passed in a preallocated object, 
      // ask the factory to make one for us, given the class ID:
      if (!obj)
	obj = rwCreateFromFactory(clID);

      if(obj)
      {
	RWASSERT(obj!=RWnilCollectable);
        readTable->append(obj);		// All is ok.  Add to read table...
        obj->restoreGuts(strm);		// ...then restore object.
      }
      else 
      {
        // Oops.  We don't know how to make an object of this type.
	// Throw an exception:
	RWTHROW( RWInternalErr(RWMessage(RWTOOL_NOCREATE, (unsigned)clID, (unsigned)clID) ));
      }
      strm >> brace;		// Get closing brace
    }
    else 
    {
      // Bad input stream.  Probably a corrupted character or something...
      goto badstream;
    }
  }
  else
  {
    // Neither '@' nor ':' was found.  Bad input stream.
    goto badstream;
  }

  if (firstRecursion)
  {
    freeReadTable();
  }
  return obj;

badstream:

  // Bad input stream.  Probably a corrupted character or something...
  strm.clear(ios::failbit | strm.rdstate() );	// Set the fail bit
  if (firstRecursion) {
    freeReadTable();
  }
  return rwnil;
}

RWCollectable*
RWCollectable::recursiveRestoreFrom(RWFile& file, RWCollectable* obj)
{
  int            objectNum;
  RWClassID      clID;
  RWReadTable*   readTable = rwnil;
  RWBoolean      firstRecursion = FALSE;	// Record whether this is the first recursion.

  readTable = getReadTable();

  if(readTable == rwnil)
  {
    long magic;
    file.Read(magic);
    if(magic != MAGIC_CONSTANT)
      RWTHROW(RWExternalErr(RWMessage(RWTOOL_MAGIC, magic, MAGIC_CONSTANT)));
    readTable       = newReadTable();
    readTable->append(RWnilCollectable);// Object 0 is always nil object
    firstRecursion = TRUE;
  }

  // Get the class ID.
  file.Read(clID);

  // Check to see if it's the special flag, signally a previously read object:
  if(clID == REFFLAG)
  {
    // Object has already been read.  In this case, the user cannot
    // have supplied an address, unless it matches what is in the
    // readTable.
    file.Read(objectNum);

    // Check for a bad object number or an object address
    // that does not match what is in the readTable:
    if (objectNum >= readTable->entries() ||
	(obj && obj != (*readTable)(objectNum)) )
      RWTHROW(RWInternalErr(RWMessage(RWTOOL_REF)));
    else
      obj = (*readTable)(objectNum);

    if (obj==RWnilCollectable)	// Detect the nil object and convert it to a nil pointer
      obj = rwnil;
  }
  else
  {
    // Object has not been read.
    // If the user has not passed in a preallocated object, 
    // ask the factory to make one for us, given the class ID:
    if (!obj)
      obj = rwCreateFromFactory(clID);	

    if(obj)
    {
      RWASSERT(obj!=RWnilCollectable);
      readTable->append(obj);		// All is ok.  Add to read table...
      obj->restoreGuts(file);		// ... then restore its internals
    }
    else
    {
      // Oops.  We don't know how to make an object of this type.
      // Throw an exception:
      RWTHROW( RWInternalErr(RWMessage(RWTOOL_NOCREATE,
				       (unsigned)clID,
				       (unsigned)clID) ));
    }
  }
  if (firstRecursion)
  {
    freeReadTable();
  }
  return obj;
}


#ifdef RW_TRAILING_RWEXPORT
RWvostream& rwexport
#else
rwexport RWvostream&
#endif
operator<<(RWvostream& vstream, const RWCollectable* p)
{
  if (p)
    p->recursiveSaveOn(vstream);
  else
    RWnilCollectable->recursiveSaveOn(vstream);

  return vstream;
}

#ifdef RW_TRAILING_RWEXPORT
RWFile& rwexport
#else
rwexport RWFile&
#endif
operator<<(RWFile& file, const RWCollectable* p)
{
  if (p)
    p->recursiveSaveOn(file);
  else
    RWnilCollectable->recursiveSaveOn(file);

  return file;
}

 /***************************************************************
 *								*
 *		 	RWStoreTable definitions		*
 *								*
 ****************************************************************/


RWStoreTable::RWStoreTable()
{
  int dummy = 0;
  add(RWnilCollectable, dummy);	// Add the nil object.
}

RWStoreTable::~RWStoreTable()
{
  clearAndDestroy();		// Will delete all the RWStoreEntry entries
}

RWBoolean
RWStoreTable::add(const RWCollectable* item, int& objectNum)
{
  RWStoreEntry temp(item, 0);	// Dummy to use as key.
  const RWCollectable* v = find(&temp);
  if(v){
    objectNum = ((const RWStoreEntry*)v)->objectNumber;
    return FALSE;
  }
  else {
    objectNum = entries();
    insert(new RWStoreEntry(item, objectNum));
    return TRUE;
  }
}

 /***************************************************************
 *								*
 *		 	RWStoreEntry definitions		*
 *								*
 ****************************************************************/

unsigned
RWStoreEntry::hash() const
{
  return RWhashAddress((void*)item);
}

RWBoolean
RWStoreEntry::isEqual(const RWCollectable* c) const
{
  RWPRECONDITION( c!=rwnil );
  return item==((const RWStoreEntry*)c)->item;
}


 /***************************************************************
 *								*
 *	 	RWCollectable virtual functions			*
 *								*
 ****************************************************************/

void
RWCollectable::restoreGuts(RWFile&) { }

void
RWCollectable::restoreGuts(RWvistream&) { }

void
RWCollectable::saveGuts(RWFile&) const { }

void
RWCollectable::saveGuts(RWvostream&) const { }
