
/*
 * Definitions for B-Trees on disk.
 *
 * $Id: disktree.cpp,v 6.4 1994/07/18 20:51:00 jims Exp $
 *
 ***************************************************************************
 *
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
 * Algorithms are based in part on programs presented in Chapter 6 of:
 * L. Ammeraal, Programs and Data Structures in C
 * John Wiley & Sons
 * Chichester, NY, 1989, 206 pp.
 * ISBN 0-471-91751-6 
 *
 * (or see also Knuth: Vol. 3)
 *
 ***************************************************************************
 *
 * $Log: disktree.cpp,v $
 * Revision 6.4  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.3  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/05/05  16:58:54  vriezen
 * Change pkey to key in RWBTreeOnDisk::ins function (under RDEBUG code)
 *
 * Revision 6.1  1994/04/15  19:06:27  vriezen
 * Move all files to 6.1
 *
 * Revision 2.25  1994/03/31  22:45:37  griswolf
 * Fixed ability to detect and read V5Style files
 *
 * Revision 2.24  1994/01/10  21:41:32  griswolf
 * fixed findKeyAndValue and remove to correctly notice ignoreNull()
 *
 * Revision 2.23  1994/01/03  19:39:56  griswolf
 * fixed destructor to use array delete for moreKey
 *
 * Revision 2.22  1994/01/03  18:58:01  griswolf
 * fix cacheCount(unsigned)
 *
 * Revision 2.21  1993/10/29  06:15:58  myersn
 * fix RW_NO_CONST_OVERLOAD[S] macros.
 *
 * Revision 2.20  1993/09/14  18:15:51  griswolf
 * fix problem with handling imbedded nulls
 *
 * Revision 2.19  1993/09/13  22:54:22  griswolf
 * fixed disktree and disktreeNode to correctly access ignoreNull info
 *
 * Revision 2.18  1993/09/12  16:28:34  griswolf
 * replaced memcpy with strncpy during inserts when ignoreNull is false
 *
 * Revision 2.17  1993/09/11  22:10:14  griswolf
 * Change constructor logic to match V6 class reference
 *
 * Revision 2.16  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.15  1993/08/17  18:23:03  griswolf
 * replaced strncXX with memcXX functions to allow imbedded nulls
 *
 * Revision 2.14  1993/07/30  00:35:04  keffer
 * Corrected ambiguous overload resolution for Centerline V2.0
 *
 * Revision 2.13  1993/07/29  16:28:24  keffer
 * Changed uses of RWCString(const char*, size_t) to reflect new
 * semantics.
 *
 * Revision 2.12  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 2.11  1993/05/19  16:12:52  keffer
 * Replaced badly formed RCS keyword
 *
 * Revision 2.10  1993/05/18  00:48:57  keffer
 * Introduced new exception handling classes
 *
 * Revision 2.9  1993/04/22  20:31:31  griswolf
 * added RWCString arguments to some members.
 *
 * Revision 2.8  1993/04/21  23:47:58  griswolf
 * Moved RWDiskTreeNode from disktree.h to disktree.cpp,
 * changed several inline functions to called functions.
 *
 * Revision 2.7  1993/04/13  07:16:05  randall
 * modified constructor to work on different style BTrees
 *
 * Revision 2.6  1993/02/24  16:17:57  griswolf
 * Replaces the old RWBTreeOnDisk with a new implementation that
 * has considerable flexibility wrt keylength, file usage, order.
 *
 * 
 *    Rev 2.10   18 Nov 1992 10:44:34   KEFFER
 * Extended previous fix to all comparisons against MINORDER.
 * 
 *    Rev 2.9   09 Oct 1992 10:31:48   KEFFER
 * Fixed delete problem in RWDiskTree::rem().
 * 
 *    Rev 2.8   14 Sep 1992 15:29:36   KEFFER
 * 
 *    Rev 2.7   09 Sep 1992 10:31:24   KEFFER
 * Fixed problem in RWDiskTreeNode::splitNode.
 * 
 *    Rev 2.6   30 May 1992 16:23:20   KEFFER
 * 
 *    Rev 2.5   30 May 1992 16:02:08   KEFFER
 * Added STARTWRAP/ENDWRAP around <string.h> for Glockenspiel.
 * 
 *    Rev 2.4   29 May 1992 09:46:44   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 2.3   28 May 1992 16:42:26   KEFFER
 * Ported to AT&T cfront V3.0.
 * 
 *    Rev 2.2   27 May 1992 18:09:00   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 2.1   25 May 1992 15:24:40   KEFFER
 * Reduced size a little further.
 * 
 *    Rev 2.0   24 May 1992 12:22:16   KEFFER
 * Rewrote, improving readability and performance.
 * 
 *    Rev 1.3   05 Nov 1991 14:06:04   keffer
 * Removed RWXXXXErr macros --- now done by error package
 * 
 *    Rev 1.2   28 Oct 1991 09:24:06   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.1   28 Jul 1991 12:37:24   keffer
 * No longer uses macro Const
 * 
 *    Rev 1.0   28 Jul 1991 08:39:16   keffer
 * Tools.h++ V4.0.5 PVCS baseline version
 *
 */

#include "rw/disktree.h"
#include "rw/rwerr.h"
#include "rw/toolerr.h"
#ifdef RDEBUG
# include "rw/cstring.h"
# include <rw/rstream.h>
#endif
STARTWRAP
#include <string.h> /* looking for strncmp and memcmp */
ENDWRAP

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: disktree.cpp,v $ $Revision: 6.4 $ $Date: 1994/07/18 20:51:00 $");

#ifndef RW_NO_CPP_RECURSION
# define new rwnew
#endif

/****************************************************************
 *								*
 *			RWDiskTreeNode				*
 *								*
 ****************************************************************/

class RWExport RWDiskTreeNode {
friend class RWExport RWBTreeOnDisk;
private:
// private constructors
  RWDiskTreeNode(unsigned size, RWBTreeOnDisk* tree);
  RWDiskTreeNode(unsigned size, RWBTreeOnDisk* tree, const char* key, 
                 RWstoredValue n);
  ~RWDiskTreeNode() { delete[] nodeRef; }

  int		binarySearch(const char* key, RWdiskTreeCompare) const;
  void		initialize();
  void		insert(const char* key, const RWstoredValue, int, RWoffset);
#ifdef RDEBUG
  void		showOff(RWoffset);
#endif
// data accessors
  unsigned&	 counter();		// How many fields in use
  RWoffset&	 sons(int i);		// \"Pointers\" to children
  RWstoredValue& items(int i);	    // usually offsets in file to key\'s data.
  char*		 keys(int i);  		// Array of key strings.
#ifndef RW_NO_CONST_OVERLOAD
  unsigned	 counter() const;
  RWoffset	 sons(int i) const;
  RWstoredValue items(int i) const;
  char*		 keys(int i) const;
#endif  
private:  //data
  RWBTreeOnDisk* tree;
  void*		 nodeRef;
};


/****************************************************************
 *								*
 *		RWDiskTreeNode inline accessors			*
 *								*
 ****************************************************************/

/*
 * Structure used to help us look into the void* nodeRef with the
 * correct alignment factors for this compiler and architecture.
 */
struct AlignToNodeRef {
  unsigned	u;		// corresponds to count
  RWoffset	a[3];		// corresponds to sons
  RWstoredValue	b[2];		// corresponds to items
  char		c[16];		// corresponds to keys
};

// Access the first unsigned in the buffer
inline unsigned&
RWDiskTreeNode::counter()		{
  struct AlignToNodeRef *p = (AlignToNodeRef*)nodeRef;
  return p->u;
}

// access the i'th RWoffset following an unsigned in the buffer
inline RWoffset&
RWDiskTreeNode::sons(int i) {
  struct AlignToNodeRef *p = (AlignToNodeRef*)nodeRef;
  return (p->a)[i];
}

/*
 * access the i'th RWstoredValue by doing arithmetic related to the
 * start location in the Alignment struct.
 */
inline RWstoredValue&
RWDiskTreeNode::items(int i)	{
  unsigned fixit = (2*tree->baseInfo.order - 2) * sizeof(RWoffset);
  struct AlignToNodeRef *p = (AlignToNodeRef*)nodeRef;
  return *((p->b) + fixit / sizeof(RWstoredValue) + i);
 }

/*
 * access the i'th char* in keys by doing arithmetic related to the
 * start location in the Alignment struct.
 */
inline char*
RWDiskTreeNode::keys(int i)	{
  struct AlignToNodeRef *p = (AlignToNodeRef*)nodeRef;
  unsigned fixit = ((tree->baseInfo.order)*2 -2)
		    * (sizeof(RWstoredValue)+sizeof(RWoffset));
  return (p->c)	+ fixit + i*(tree->baseInfo.keylen);
}

#ifndef RW_NO_CONST_OVERLOAD

inline unsigned
RWDiskTreeNode::counter() const	{
  struct AlignToNodeRef *p = (AlignToNodeRef*)nodeRef;
  return p->u;
}

// access the i'th RWoffset following an unsigned in the buffer
inline RWoffset
RWDiskTreeNode::sons(int i) const	{
  struct AlignToNodeRef *p = (AlignToNodeRef*)nodeRef;
  return (p->a)[i];
}

/*
 * access the i'th RWstoredValue by doing arithmetic related to the
 * start location in the Alignment struct.
 */
inline RWstoredValue
RWDiskTreeNode::items(int i) const	{
  unsigned fixit = (2*tree->baseInfo.order - 2) * sizeof(RWoffset);
  struct AlignToNodeRef *p = (AlignToNodeRef*)nodeRef;
  return *((p->b) + fixit / sizeof(RWstoredValue) + i);
 }

/*
 * access the i'th char* in keys by doing arithmetic related to the
 * start location in the Alignment struct.
 */
inline char*
RWDiskTreeNode::keys(int i) const	{
  struct AlignToNodeRef *p = (AlignToNodeRef*)nodeRef;
  unsigned fixit = ((tree->baseInfo.order)*2 -2)
		    * (sizeof(RWstoredValue)+sizeof(RWoffset));
  return (p->c)	+ fixit + i*(tree->baseInfo.keylen);
}

#endif /*  RW_NO_CONST_OVERLOAD */

/*
 * useful macros for disktree.cpp only. 
 */
// utility macro (ONLY for RWDiskTreeNode to use)
#define nKEYLEN	(tree->baseInfo.keylen)
// utility macros (ONLY for RWBTreeOnDisk to use)
#define KEYLEN	 (baseInfo.keylen)
#define MINORDER (baseInfo.minorder)
#define ORDER    (baseInfo.order)
#define ORDER2   (2*baseInfo.order)
const size_t	 oplussv = sizeof(RWoffset)+sizeof(RWstoredValue);


/*********************************************************
*					 		  *
*	      Methods for Class RWDiskTreeNode		  *
*							  *
**********************************************************/

RWDiskTreeNode::RWDiskTreeNode(unsigned size, RWBTreeOnDisk* t) :
	  tree(t)
{
  nodeRef = new char[size];
  initialize();
}

RWDiskTreeNode::RWDiskTreeNode(unsigned size, RWBTreeOnDisk* t,
			       const char* key, RWstoredValue n)  :
	tree(t)
{
  nodeRef = new char[size];
  initialize();
  if(t->ignoreNulls())
    memcpy(keys(0),key,nKEYLEN);
  else
    strncpy(keys(0),key,nKEYLEN);
  items(0) = n;
  counter() = 1;
} 

/*
 * Binary search of node for key a in node with counter keys. Returns
 *		0  where	     a <  every key
 *	  counter  where	     a >  every key
 *	 	r  where  key[r-1] < a <= key[r]
 */
int 
RWDiskTreeNode::binarySearch(const char* a, RWdiskTreeCompare compareKeys) const
{
  register unsigned klen = nKEYLEN;
  int n = counter() - 1;
  if ( (*compareKeys)(keys(0),a,klen) >= 0 ) 
  {
    return 0;
  }
  if ( (*compareKeys)(keys(n),a,klen) < 0 ) 
  {
    return counter();
  }
  int l = 0;
  int r = n;
  while (r-l > 1) {
    int i = r+l >> 1;     // bitwise divide by two
    if ( (*compareKeys)(keys(i),a,klen) >= 0 )
      r=i;
    else
      l=i;
  }
  return r;
}

void
RWDiskTreeNode::initialize()
{
  counter() = 0;
  for (register int i = 0; i<= 2*tree->baseInfo.order; i++) //order*2 + 1 loops
    sons(i) = RWNIL;
}

/*
 * insert the key and its associated value at the loc'th spot in the
 * paired arrays. Off is the offset to the *right* son of the new key.
 */
void
RWDiskTreeNode::insert(const char* k, const RWstoredValue v, int loc, 
                       RWoffset off)
{
  for(int i = counter(); i > loc; i--)
  {
    memcpy(keys(i), keys(i-1), nKEYLEN);
    items(i) = items(i-1);
    sons(i+1) = sons(i); // leave loc'th son behind.
  }
//now have gaps at keys(loc), items(loc) and sons(loc+1)
  if(tree->ignoreNulls())
    memcpy(keys(loc), k, nKEYLEN);
  else
    strncpy(keys(loc), k, nKEYLEN);
  items(loc) = v;
  sons(loc+1) = off;
  ++(counter());
}

#ifdef RDEBUG
void
RWDiskTreeNode::showOff(RWoffset l)
{
  cout << "---- Node at " << l << " has " << counter() << " items ----" << endl;
  for(int i = 0; i < counter(); i++)
  {
    cout<<"[";
    cout.fill(' ');
    cout.width(5);
    cout<<sons(i)<<"]";
    cout.width(3);
    cout.fill('"');
    cout << keys(i)<<"\",";
    cout.width(2);
    cout.fill('0');
    cout << items(i);
    if(i==3 || i==7 || i==11 || i == 15)
      cout << endl;
    else if(i == counter()-1) 
    {
      cout << "[";
      cout.fill(' ');
      cout.width(5);
      cout<<sons(counter())<<"]"<<endl;
    }
  }
}
#endif    

/*********************************************************
*					 		  *
*	  Public methods for Class RWBTreeOnDisk	  *
*							  *
**********************************************************/

/*
 * This constructor requires RWFileManager argument, will use
 * a default value for all other args.
 */
RWBTreeOnDisk::RWBTreeOnDisk(RWFileManager& filemgr,
			     unsigned blocks,
			     createMode omode,
			     unsigned keylength,
			     RWBoolean ignoreNull,
			     RWoffset start,
			     styleMode smode,
			     unsigned ord,
			     unsigned minord )
    :
    baseLoc((smode!=V5Style) ? start : RWNIL), //flag oldStyle if needed
    fmgr(&filemgr),
    cacheBlocks(blocks)
{
  if(smode == V5Style)
  // assume that if V5 is specified it will work
  {
    infoInit(RWNIL,		// RWNIL=="entries() not cached"
	     keylength,ord,minord, ignoreNull);
    if(RWNIL ==  fmgr->start()) // new file
    {
      fmgr->allocate(sizeof(RWoffset)); // at start()
      writeInfo(); //puts RWNIL at start(): == empty tree
    }
  }
  else // if V6 is specified, we have to check
  {
    if(baseLoc == RWNIL) // default -- ask fmgr for start()
    {
      if(omode == create || (RWNIL == (baseLoc = fmgr->start()))) 
      {
	// new file: use _ctor args to fill baseInfo
	infoInit(RWBTreeOnDiskCurrentVersion,keylength,ord,minord,ignoreNull);
	// Now get some place to put it from file manager
	baseLoc = fmgr->allocate(sizeof(baseInfo));
	// and write it where-ever that was
	writeInfo();
      }
    }
  }
  // if this is old file: ignore _ctor args. If new, re-read info
  readInfo();
  if(baseInfo.version != RWBTreeOnDiskCurrentVersion)
  {
    // then assume V5 style
    infoInit(RWNIL,		// RWNIL=="entries() not cached"
	     keylength,ord,minord, ignoreNull);
    smode = V5Style;
    baseLoc = RWNIL;
    readInfo(); // try it again
  }

// finally we know enough to build the nodeRef stuff
  nodeRefSize = sizeof(AlignToNodeRef)+(ORDER2-2)*oplussv-16+KEYLEN*ORDER2;
  // and we know about style
  if(smode == V5Style || ignoreNulls() == FALSE)
  {
    compareKeys = (RWdiskTreeCompare)strncmp;
  }
  else
  {
    compareKeys = (RWdiskTreeCompare)memcmp;
  }
  startup();

#ifdef RDEBUG
  cout << "==========================================\n";
  if(smode==V5Style) {
    cout << "Built (old) RWBTreeOnDisk using:\n";
    cout << "baseLoc:	" << "(base not available)" << endl;
  }
  else {
    cout << "Built (new) RWBTreeOnDisk using:\n";
    cout << "baseLoc:	" << baseLoc << endl;
  }
  cout << "rootNode loc	" << baseInfo.rootLoc << endl;
  cout << "version:	0x" << hex <<baseInfo.version << endl << dec;
  if(smode == V5Style) 
    cout << "entries:	" << "(not cached)" << endl;
  else
    cout << "entries:	" << (long)(baseInfo.entries) << endl;
  cout << "Key length:	" << baseInfo.keylen << endl;
  cout << "Order:		" << baseInfo.order << endl;
  cout << "Min. Order	" << baseInfo.minorder << endl;
  cout << "Node Size:	" << nodeRefSize << endl;
  cout << "==========================================\n\n";
#endif
}

/*
 * No need to write the root, workNodes back to disk: We keep them
 * updated whenever we change them in the code. But save baseInfo
 */
RWBTreeOnDisk::~RWBTreeOnDisk()
{
#ifdef RDEBUG
  cout << "deleting RWBTreeOnDisk with " << entries() << " entries." << endl;
#endif
  writeInfo();		// (== writeRoot, if old version)
  RWVECTOR_DELETE(baseInfo.keylen) moreKey;
  delete workNode;
  delete root;		
  delete cmgr;		// automatically flushes the cache during _dtor
}

/*
 * Update the number of blocks being managed by the cacheManager.
 * It makes sense to do this under two conditions:
 *  1: The tree is much deeper than the current number of blocks, so 
 *     that many searches cause disk accesses -- increase the count.
 *  2: Memory is very tight, so that speed is less essential than space,
 *     or the tree is now much smaller than before -- decrease the count.
 */
unsigned
RWBTreeOnDisk::cacheCount(unsigned blocks)
{
  unsigned retblocks = cacheBlocks;	// Save old value
  if (blocks != cacheBlocks) {
    delete cmgr;		// delete automatically flushes
    cmgr = new RWCacheManager(fmgr,nodeRefSize,blocks);
    cacheBlocks = blocks;
  }
  return retblocks;
}

void			
RWBTreeOnDisk::clear()
{
  if(isEmpty() ) return;
  del(baseInfo.rootLoc);
  infoReInit();
  writeInfo();
  root->initialize();
}

/* Used by the entries() method */
void 
RWBTreeCountEntries(const char*, RWstoredValue, void* x)
{
  ++(*(unsigned long *)x);
}

/* 
 * count the number of entries in the tree: Can be very costly. Lots
 * of options for other ways to do this. Left as exercise...
 */
unsigned long	
RWBTreeOnDisk::entries() const
{
  if(baseLoc == RWNIL) // old version -- check if we have data cached:
  {
    if(RWNIL == baseInfo.cacheFlag) // not cached. Do it now.
    {
      unsigned long count = 0;
      if(RWNIL != baseInfo.rootLoc)
	((RWBTreeOnDisk *)this)->applyToKeyAndValue(RWBTreeCountEntries,&count);
        // cast const away in order to cache entries...
      ((RWBTreeOnDisk*)this)->baseInfo.entries = count; 
      ((RWBTreeOnDisk*)this)->baseInfo.cacheFlag = 0; // got it now
    }
  }
  return baseInfo.entries;
}

/*
 * findKeyAndValue is analogous to RWDictionary::findKeyAndValue
 */
RWBoolean
RWBTreeOnDisk::findKeyAndValue(const char* key, RWCString& retK, RWstoredValue& val) const
{
  // It would be better to unroll the root access here.
  val = RWNIL;
  RWoffset tOff = baseInfo.rootLoc;
  while(tOff != RWNIL)
  {
    readcache(tOff,workNode);
    int i = workNode->binarySearch(key, compareKeys);
    if (    i < workNode->counter()
	 && (*compareKeys)(key,workNode->keys(i),KEYLEN) == 0)
    {			// found it!
      val  = workNode->items(i);
      const char* pkey = workNode->keys(i);
      if(ignoreNulls())
	retK = RWCString(pkey,(unsigned)KEYLEN);
      else
	retK = RWCString(pkey,rwmin((unsigned)KEYLEN,(unsigned)strlen(pkey)));
      return TRUE;
    }
    tOff = workNode->sons(i);
  }
  // if we ever get here, we have failed to find the key
  return FALSE;
}

/*
 * the height of the smallest branch is equal to the height of the
 * tree because in a B Tree, all leaves are at the same level.
 */
unsigned		
RWBTreeOnDisk::height() const
{
  int h = 0;
  RWoffset toff = baseInfo.rootLoc;
  while(RWNIL != toff)
  {
    ++h;
    readcache(toff, workNode);
    toff = workNode->sons(0);
  }
  return h;
}

RWBoolean
RWBTreeOnDisk::insertKeyAndValue(const char*key, RWstoredValue val)
{
#ifdef RDEBUG
  cout << "InsertK&V: Entries = " << entries() << endl;
#endif
  retStatus status = ins(key,val,baseInfo.rootLoc);
  if(more == status)  // new root node. Look in moreXXX for data
  {
    memcpy(root->keys(0), moreKey, KEYLEN);
    root->items(0) = moreItem;
    root->sons(0) = baseInfo.rootLoc; 
    root->sons(1) = moreOffset;
    root->counter() = 1;
    baseInfo.rootLoc = fmgr->allocate(nodeRefSize);
    writecache(baseInfo.rootLoc,root);
    writeInfo();
  }
  baseInfo.entries += (ignored != status);
  return ignored != status;
}

RWBoolean
RWBTreeOnDisk::removeKeyAndValue(const char* key, RWCString& retKey, RWstoredValue& retVal)
{
  retStatus status = rem(key, baseInfo.rootLoc, retKey, retVal);
  if(more == status) // underflow
  {
    fmgr->deallocate(baseInfo.rootLoc);
    baseInfo.rootLoc = root->sons(0);
    writeInfo();
    if(RWNIL == baseInfo.rootLoc)
      root->initialize();
    else
      readcache(baseInfo.rootLoc,root);
  }
  if(ignored != status)
  {
    baseInfo.entries--;
    return TRUE;
  }
  else
    return FALSE;
}

/*
 * replaceValue(key,newV,&oldV) is somewhat analogous to 
 * RWDictionary::findValue(key,newV)
 */
RWBoolean
RWBTreeOnDisk::replaceValue(const RWCString& key, const RWstoredValue newval, RWstoredValue& oldVal)
{
  // see comment at findKeyAndValue per unrolling? the first iter of the loop
  oldVal = RWNIL;
  workOffset = baseInfo.rootLoc;
  while(workOffset != RWNIL)
  {
    readcache(workOffset, workNode);
    int i = workNode->binarySearch(key, compareKeys);
    if (   i < workNode->counter()
	&& (*compareKeys)(key,workNode->keys(i),KEYLEN) == 0)
    {			// found it!
      oldVal = workNode->items(i);
      workNode->items(i) = newval;
      writecache(workOffset,workNode);
      return TRUE;
    }
    workOffset = workNode->sons(i);
  }
  return FALSE;
}

RWdiskTreeCompare
RWBTreeOnDisk::setComparison(RWdiskTreeCompare cf)
{
  RWdiskTreeCompare ret = compareKeys;
  compareKeys = cf;
  return ret;
}

/*********************************************************
 *					 		 *
 *	 protected Methods for Class RWBTreeOnDisk	 *
 *							 *
 *********************************************************/
			 
void
RWBTreeOnDisk::infoInit(RWstoredValue ver, unsigned keyl,
		     unsigned ord, unsigned min, RWBoolean ignoreNull)
{
  baseInfo.rootLoc = RWNIL;
  baseInfo.extraLoc = RWNIL;
  baseInfo.version = ver; // union ==> sometimes: baseInfo.cacheFlag = ver
  baseInfo.entries = 0;
  baseInfo.keylen = keyl;
  baseInfo.order = ord;
  baseInfo.minorder = (min <= ord) ? min : ord;
  baseInfo.reserved1 = RWNIL;
  // other flags may be or\'ed into flags, but *set* the first one
  baseInfo.flags = ignoreNull ? 1 : 0;
}


void
RWBTreeOnDisk::infoReInit()
{
  baseInfo.rootLoc = RWNIL;
  baseInfo.extraLoc = RWNIL;
  baseInfo.entries = 0;
  // check if we're looking at old-style tree, and if so, flag 
  // that the entries() cache is correct: Empty tree has 0 entries
  baseInfo.cacheFlag = (RWNIL == baseLoc)? 0 : baseInfo.version;
}

void
RWBTreeOnDisk::startup()
{
  cmgr        = new RWCacheManager(fmgr,nodeRefSize,cacheBlocks);
  root        = new RWDiskTreeNode(nodeRefSize,this);
  workNode    = new RWDiskTreeNode(nodeRefSize,this); // space to work
  readRoot();
  workOffset  = RWNIL;
  moreKey = new char[baseInfo.keylen];
}

void
RWBTreeOnDisk::readInfo()
{
  if(RWNIL == baseLoc) // old version: read rootLoc from start()
  {
    if(fmgr->SeekTo(fmgr->start()) == FALSE)
      RWTHROW(RWFileErr(RWMessage( RWTOOL_SEEKERR ),
			fmgr->GetStream(),
			RWFileErr::seekErr) );
    if(fmgr->Read(baseInfo.rootLoc) == FALSE)
      RWTHROW(RWFileErr(RWMessage( RWTOOL_READERR ),
			fmgr->GetStream(),
			RWFileErr::readErr) );
  }
  else
  {
    if(fmgr->SeekTo(baseLoc) == FALSE)
      RWTHROW(RWFileErr(RWMessage( RWTOOL_SEEKERR ),
			fmgr->GetStream(),
			RWFileErr::seekErr) );
    if(fmgr->Read((char*)&baseInfo, sizeof(baseInfo)) == FALSE)
      RWTHROW(RWFileErr(RWMessage( RWTOOL_READERR ),
			fmgr->GetStream(),
			RWFileErr::readErr) );
  }
}

void
RWBTreeOnDisk::writeInfo() const
{
  // Don't write if openend in RO mode.
  if(fmgr->isReadOnly())
    return;

  if(RWNIL == baseLoc) // old version: write rootLoc at start()
  {
    if(fmgr->SeekTo(fmgr->start()) == FALSE)
      RWTHROW(RWFileErr(RWMessage( RWTOOL_SEEKERR ),
			fmgr->GetStream(),
			RWFileErr::seekErr) );
    if(fmgr->Write(baseInfo.rootLoc) == FALSE)
      RWTHROW(RWFileErr(RWMessage( RWTOOL_WRITEERR ),
			fmgr->GetStream(),
			RWFileErr::writeErr) );
  }
  else
  {
    if(fmgr->SeekTo(baseLoc) == FALSE)
      RWTHROW(RWFileErr(RWMessage( RWTOOL_SEEKERR ),
			fmgr->GetStream(),
			RWFileErr::seekErr) );
    if(fmgr->Write((char*)&baseInfo, sizeof(baseInfo)) == FALSE)
      RWTHROW(RWFileErr(RWMessage( RWTOOL_WRITEERR ),
			fmgr->GetStream(),
			RWFileErr::writeErr) );
  }
}


/*********************************************************
 *					 		 *
 *	 private Methods for Class RWBTreeOnDisk	 *
 *							 *
 *********************************************************/

void
RWBTreeOnDisk::apl(RWoffset start, RWdiskTreeApply ap, void* x)
{
  if(start != RWNIL) 
  {
    readcache((workOffset = start),workNode);
    for(int i = 0; i < workNode->counter(); i++)
    {		
      // first do "smaller" subtree
      if(RWNIL != workNode->sons(i)) // avoid extra memcopies if possible
      {
	apl(workNode->sons(i), ap, x); // this can alter *workNode
	readcache((workOffset=start),workNode);    // so put it back after
      }
      (*ap)(workNode->keys(i),workNode->items(i), x); // do this level
      writecache(workOffset,workNode); // ap may have changed values.
      // and loop back to do everything "larger"
    }
    apl(workNode->sons(workNode->counter()), ap, x); // do very last son too
  }
}

/*
 * combine this node into its left sibling, along with the value at
 * parent node's topLoc, delete this node from the file manager,
 * fix up the parent node, and leave it parent in the work area.
 */
void
RWBTreeOnDisk::combineNodes(int topLoc, RWoffset topOff)
{
  RWDiskTreeNode *tempNode = new RWDiskTreeNode(nodeRefSize,this);
  workOffset = topOff;				// work on parent first
  readcache(workOffset, tempNode);
  memcpy(moreKey,tempNode->keys(topLoc),KEYLEN);
  moreItem = tempNode->items(topLoc);
  RWoffset killOff = tempNode->sons(topLoc+1); // this node's offset
  RWoffset leftOff = tempNode->sons(topLoc);   // left sib's offset
  // now shift higher data down to remove the datum in parent
  for(int i = topLoc; i < tempNode->counter() -1; i++)
  {
    memcpy(tempNode->keys(i),tempNode->keys(i+1), KEYLEN);
    tempNode->items(i) = tempNode->items(i+1);
    tempNode->sons(i+1) = tempNode->sons(i+2);
  }
  (tempNode->counter())--;
  writecache(workOffset,tempNode);

  readcache(leftOff,tempNode);
  // now append moreXXX, then rightSon to leftSon (note strict "<")
#ifdef RDEBUG
  cout << "Combining nodes " << leftOff << " size " << tempNode->counter();
  cout << " and " << killOff << " size " << workNode->counter() << endl;
#endif
  RWPRECONDITION(tempNode->counter()+workNode->counter() < ORDER2);
  memcpy(tempNode->keys(tempNode->counter()),moreKey,KEYLEN);
  tempNode->items(tempNode->counter()) = moreItem;
  int tcount = ++(tempNode->counter());
  for(i=0;i<workNode->counter();i++)
  {
    memcpy(tempNode->keys(tcount+i),workNode->keys(i),KEYLEN);
    tempNode->items(tcount+i) = workNode->items(i);
    tempNode->sons(tcount+i) = workNode->sons(i);
  }
  tempNode->sons(tcount+workNode->counter())
	      = workNode->sons(workNode->counter());
  (tempNode->counter()) += workNode->counter();
  writecache(leftOff,tempNode);
  delete tempNode;
  fmgr->deallocate(killOff);
  // now put parent back into work area
  readcache(workOffset,workNode);
}

void
RWBTreeOnDisk::del(RWoffset start)
{
  if(start != RWNIL)
  {
    readcache(start,workNode);
    if(workNode->sons(0) != RWNIL) { 
      // not a leaf yet
      for(int i = 0; i < workNode->counter();i++) {
	if(RWNIL != workNode->sons(i))
	{
	  del(workNode->sons(i));
	  readcache(start,workNode);
	}
      } /* for */
      del(workNode->sons(workNode->counter()));
    } /* if */
    fmgr->deallocate(start);
  } /* if */
}

/*
 * recursively descend tree looking for correct node to insert
 * new key and value into.
 */
#ifndef RW_GLOBAL_ENUMS
RWBTreeOnDisk::retStatus
#else
retStatus
#endif
RWBTreeOnDisk::ins(const char* key, RWstoredValue val, RWoffset start)
{
#ifdef RDEBUG
  cout << "DiskT::ins(" << key << ", " << val << ", " << start << ")" << endl;
#endif
  if(RWNIL == start) // then we need to pop up one to finish.
  {
    if(ignoreNulls())
      memcpy(moreKey, key, KEYLEN);
    else
      strncpy(moreKey, key, KEYLEN);
    moreItem = val;
    moreOffset = RWNIL;
    return more;
  }

  /******
   * can only arrive here if we have a real node to work on
   ******/

  workOffset = start;
  readcache(workOffset, workNode);

  int loc = workNode->binarySearch(key, compareKeys);
  if(   loc < workNode->counter()
     && (*compareKeys)(key, workNode->keys(loc), KEYLEN)==0)
    return ignored; // found a duplicate
  RWoffset check = workNode->sons(loc);
  retStatus status = ins(key,val,check);
  if(more != status)
    return status;

  /******
   * can only arrive here if status is "more" -- insert into this node
   ******/

  if(RWNIL != check || workOffset != start) // then get right.
  {
    workOffset = start;
    readcache(workOffset,workNode);
  }
  if(workNode->counter() < ORDER2)
  {
    // easy: room to insert right here
#ifdef RDEBUG
    RWCString mk;
    if(ignoreNulls())
	mk = RWCString(moreKey,(unsigned)KEYLEN);
      else
	mk = RWCString(moreKey,rwmin((unsigned)KEYLEN,(unsigned)strlen(key)));

    cout << "Insert " << mk << " at location "<< loc << endl;
    workNode->showOff(workOffset);
#endif
    workNode->insert(moreKey,moreItem,loc,moreOffset);
    writecache(workOffset,workNode);
    return success;
  }

  // else we need to insert here, but can't: no room
  RWPRECONDITION(workOffset != RWNIL);
  splitNode(loc); // Split knows that workNode and moreXXX have all the info
  return more;
}

/*
 * remove smallest set of data from this node, put it in parent (at
 * disk offset topOff), and insert parent's data from topLoc into our
 * left sibling node. Leave parent node in work area.
 */
void 
RWBTreeOnDisk::moveItLeft(int topLoc, RWoffset topOff)
{
  memcpy(moreKey,workNode->keys(0),KEYLEN);
  moreItem = workNode->items(0);
  moreOffset = workNode->sons(0);
  RWPRECONDITION(workNode->counter() <= ORDER2);
  for(int i = 0; i < workNode->counter() - 1; i++)
  {
    memcpy(workNode->keys(i),workNode->keys(i+1),KEYLEN);
    workNode->items(i) = workNode->items(i+1);
    workNode->sons(i) = workNode->sons(i+1);
  }
  workNode->sons(workNode->counter() - 1)=workNode->sons(workNode->counter());
  (workNode->counter())--;
  writecache(workOffset,workNode);
  workOffset = topOff;
  readcache(workOffset,workNode);
  char *tempKey = new char[KEYLEN];
  memcpy(tempKey,workNode->keys(topLoc),KEYLEN);
  memcpy(workNode->keys(topLoc),moreKey,KEYLEN);
  RWstoredValue tempItem = workNode->items(topLoc);
  workNode->items(topLoc) = moreItem;
  writecache(topOff,workNode);
  RWoffset off = workNode->sons(topLoc);
  readcache(off,workNode);
  // insert at largest point
  workNode->insert(tempKey,tempItem,workNode->counter(),moreOffset);
  writecache(off,workNode);
  readcache(workOffset,workNode);
  delete[] tempKey;
}

/*
 * remove largest set of data from this node, put it in parent (at
 * disk offset topOff), and insert parent's data from topLoc into our
 * right sibling node. Leave parent node in work area.
 */
void RWBTreeOnDisk::moveItRight(int topLoc, RWoffset topOff)
{
   int big = workNode->counter() - 1;
   memcpy(moreKey,workNode->keys(big),KEYLEN);
   moreItem = workNode->items(big);
   moreOffset = workNode->sons(big+1);
   (workNode->counter())--;
   writecache(workOffset,workNode);
   workOffset = topOff;
   readcache(workOffset,workNode);
   char *tempKey = new char[KEYLEN];
   memcpy(tempKey,workNode->keys(topLoc-1),KEYLEN);
   memcpy(workNode->keys(topLoc-1),moreKey,KEYLEN);
   RWstoredValue tempItem = workNode->items(topLoc-1);
   workNode->items(topLoc-1) = moreItem;
   writecache(topOff,workNode);
   RWoffset off = workNode->sons(topLoc);
   readcache(off,workNode);
   // insert at smallest point, shifting sons array up too.
   workNode->insert(tempKey,tempItem,0,workNode->sons(0));
   // now put the correct son-pointer in place.
   workNode->sons(0) = moreOffset;
   writecache(off,workNode);
   readcache(workOffset,workNode);
   delete[] tempKey;
}

void
RWBTreeOnDisk::readcache(RWoffset a, RWDiskTreeNode* b) const
{
  if(cmgr->read(a,b->nodeRef) == FALSE)
      RWTHROW(RWFileErr( RWMessage( RWTOOL_READERR ),
			 fmgr->GetStream(),
			 RWFileErr::readErr) );
}

void
RWBTreeOnDisk::writecache(RWoffset a, RWDiskTreeNode* b)
{
  if(cmgr->write(a,b->nodeRef) == FALSE)
    RWTHROW(RWFileErr( RWMessage(RWTOOL_WRITEERR ),
		       fmgr->GetStream(),
		       RWFileErr::writeErr) );
}

void
RWBTreeOnDisk::readRoot()
{
#ifdef RDEBUG
  cout << "RWBTreeOnDisk::readRoot from offset " << baseInfo.rootLoc << endl;
#endif
  if(baseInfo.rootLoc == RWNIL) 
    return;	// no root node here
  readcache(baseInfo.rootLoc, root);
}

#ifndef RW_GLOBAL_ENUMS
RWBTreeOnDisk::retStatus
#else
retStatus
#endif
RWBTreeOnDisk::rem(const char* key, RWoffset start, RWCString& retK, RWstoredValue& val)
{
  if(RWNIL == start)
    return ignored; // can't remove if can't find
  /******
   * If we get here, we have a valid node
   ******/
  workOffset = start; 
  readcache(workOffset,workNode);
  int k = workNode->binarySearch(key, compareKeys);
  retStatus retval;
  // test to see if key is in this node
  if(   k < workNode->counter()
     && (*compareKeys)(key,workNode->keys(k),KEYLEN) == 0)
  {
    if(RWNIL != workNode->sons(0)) // this isn't a leaf
    {
      // k++ so that on recursion our parent will cope correctly.
      swapWithSuccessor(k++);
      retval = rem(key, workNode->sons(k), retK, val);
    }
    else // this is a leaf: remove the item here and now
    {
      val = workNode->items(k);
      const char* pkey = workNode->keys(k);
      if(ignoreNulls())
	retK = RWCString(pkey,(unsigned)KEYLEN);
      else
	retK = RWCString(pkey,rwmin((unsigned)KEYLEN,(unsigned)strlen(pkey)));

      int n = workNode->counter() - 1;
      for(int i = k; i < n; i++)
      {
	memcpy(workNode->keys(i),workNode->keys(i+1),KEYLEN);
	workNode->items(i) = workNode->items(i+1);
	workNode->sons(i) = workNode->sons(i+1);
      }
      workNode->sons(n)=workNode->sons(n+1);
      (workNode->counter())--;
      writecache(workOffset,workNode);
      // return now so that parent node can clean up if needed.
      if(workOffset == baseInfo.rootLoc)
	return (workNode->counter() > 0) ? success : more;
      else
	return (workNode->counter() >= MINORDER) ? success : more;
    } /* swapped and removed else removed here */
  } /* did we find it in this node? */
  else  // k == counter or compareKeys != 0
  {
    if(RWNIL == workNode->sons(k)) // leaf node: if not found -- not in tree
      return ignored;
    else
      retval = rem(key, workNode->sons(k), retK, val);
  }
  /******
   * If we are here, retval has been set during a recursion
   ******/
  if(retval != more)
    return retval;
  else
  {
    // presumably, rem has dirtied our work area, so on return:
    workOffset = start;
    readcache(workOffset,workNode);
    return restoreNode(k);
  }
}

/*
 * Restore the node at sons(whichSon) to minimum correct size. Doing so
 * may make this node too small, so be prepared to tell this node's 
 * parent to do similar work again.
 */
#ifndef RW_GLOBAL_ENUMS
RWBTreeOnDisk::retStatus
#else
retStatus
#endif
RWBTreeOnDisk::restoreNode(int whichSon)
{
#ifdef RDEBUG
  cout << "restoreNode(" << whichSon << ") ";
#endif
  RWoffset keepWorkOffset = workOffset;
  if(0 == whichSon) // leftmost node too small
  {
    RWPRECONDITION(workNode->sons(whichSon+1) != RWNIL);
    workOffset = workNode->sons(whichSon+1);
    readcache(workOffset,workNode);
    if(workNode->counter() > MINORDER)
    {
      moveItLeft(0,keepWorkOffset);  // get from sons(1), add to sons(0)
      return success;
    }
    else
    {
      // combineNodes expects workNode to be pointed at right sib
      combineNodes(whichSon, keepWorkOffset);
      if(workOffset == baseInfo.rootLoc)
	return (workNode->counter() > 0) ? success : more;
      else
	return (workNode->counter() >= MINORDER) ? success : more;
    }
  } /* working on leftmost son? */
  else if(workNode->counter() == whichSon) // rightmost node too small
  {
    RWPRECONDITION(workNode->sons(whichSon-1) != RWNIL);
    workOffset = workNode->sons(whichSon-1);
    RWoffset sonOffset = workNode->sons(whichSon);
    readcache(workOffset,workNode);
    if(workNode->counter() > MINORDER)
    {
      moveItRight(whichSon,keepWorkOffset);
      return success;
    }
    else
    {
      // combineNodes expects workNode to be pointed at right sib
      workOffset = sonOffset;
      readcache(workOffset,workNode);
      combineNodes(whichSon-1, keepWorkOffset);
      if(workOffset == baseInfo.rootLoc)
	return (workNode->counter() > 0) ? success : more;
      else
	return (workNode->counter() >= MINORDER) ? success : more;
    }
  } /* working on rightmost son? */
  else /* we are somewhere in the middle: try left sibling first */
  {
    RWPRECONDITION(workNode->sons(whichSon-1) != RWNIL);
    RWPRECONDITION(workNode->sons(whichSon+1) != RWNIL);
    workOffset = workNode->sons(whichSon-1);
    RWoffset rightSibOffset = workNode->sons(whichSon+1); //save in case

    readcache(workOffset,workNode);
    if(workNode->counter() > MINORDER) // left sib here
    {
      moveItRight(whichSon,keepWorkOffset);
      return success;
    }
    else // try right sibling
    {
      readcache((workOffset = rightSibOffset),workNode);
      if(workNode->counter() > MINORDER)
      {
	moveItLeft(whichSon,keepWorkOffset);
	return success;
      }
      else // we have to combine then
      {
	// combineNodes expects workNode to be pointed at right sib
	combineNodes(whichSon, keepWorkOffset);
	if(workOffset == baseInfo.rootLoc)
	  return (workNode->counter() > 0) ? success : more;
	else
	  return(workNode->counter() >= MINORDER) ? success : more;
      } // end of combine nodes
    } // end of try right sibling
  } // end of (not an extremum of the node)
}

/*
 * split workNode in two, pass back a set of key,val,*right*son 
 * (via moreXXX) for insertion in parent node.
 */
void
RWBTreeOnDisk::splitNode(int loc)
{
  char *tempKey = new char[KEYLEN];
  RWstoredValue tempItem;
  RWoffset tempOffset;
  RWDiskTreeNode *tempNode = new RWDiskTreeNode(nodeRefSize,this);
#ifdef RDEBUG
  cout << "splitNode(" << loc<< ")" << endl;
  workNode->showOff(workOffset);
#endif
  if(loc <= ORDER) // new item goes left or middle: copy left half over
  {
    for(int i = 0; i < ORDER; i++)
    {
      memcpy(tempNode->keys(i),workNode->keys(i+ORDER),KEYLEN);
      tempNode->items(i) = workNode->items(i+ORDER);
      tempNode->sons(i) = workNode->sons(i+ORDER);
    }
    tempNode->sons(ORDER) = workNode->sons(ORDER2);
    tempNode->counter() = ORDER;
    workNode->counter() = ORDER;

    if(loc < ORDER) // then we need to insert moreXXX in left half
    {
      workNode->insert(moreKey,moreItem,loc,moreOffset);
      // now there are order+1 items, in 0 thru order'th spots,
      // fill moreXXX with the largest, and clean up.
      memcpy(moreKey,workNode->keys(ORDER),KEYLEN);
      moreItem = workNode->items(ORDER);
      (workNode->counter())--;
    }	
    else //loc == order: we can leave the info in moreXXX ...
    {
      tempNode->sons(0) = moreOffset; // ... except offset
    }
  } /* end of loc <= order: put new item left or center */
  else // new item goes to right, don't work too hard
  {
    // swap inserting item for the smallest of top half
    memcpy(tempKey,moreKey,KEYLEN);
    memcpy(moreKey,workNode->keys(ORDER),KEYLEN);
    tempItem = moreItem;
    moreItem = workNode->items(ORDER);
    tempOffset = moreOffset;
    // copy all but smallest of top half
    for(int i = 0; i < ORDER -1; i++)
    {
      memcpy(tempNode->keys(i),workNode->keys(i+ORDER+1),KEYLEN);
      tempNode->items(i) = workNode->items(i+ORDER+1);
      tempNode->sons(i) = workNode->sons(i+ORDER+1);
    }
    workNode->counter() = ORDER;
    tempNode->sons(ORDER-1) = workNode->sons(ORDER2);
    tempNode->counter() = ORDER-1;
    tempNode->insert(tempKey,tempItem,loc-ORDER-1,tempOffset);
  }
  // all is copied over, and moreXX has data. Save it, and 
  // get ready to un-recurse.
  moreOffset = fmgr->allocate(nodeRefSize);
#ifdef RDEBUG
  cout << "(old), new nodes are:" << endl;
  workNode->showOff(workOffset);
  tempNode->showOff(moreOffset);
#endif
  writecache(moreOffset,tempNode);
  delete tempNode;
  writecache(workOffset,workNode);
  delete[] tempKey;
}

/*
 * workNode[k]'s data is put into it's successor's location on the tree.
 * the successor's data is put into workNode[k]. Otherwise, everything
 * is put back like it was.
 */
void
RWBTreeOnDisk::swapWithSuccessor(int k)
{
#ifdef RDEBUG
  cout << "RWBTreeOnDisk::swapWithSuccessor(" << k << ") of node " << 
    workOffset << endl;
#endif
  memcpy(moreKey,workNode->keys(k),KEYLEN); // save off moving data
  moreItem = workNode->items(k);
  RWoffset keepWorkOffset = workOffset;       // and "home"

  workOffset = workNode->sons(k+1);	// find greater subtree's
  readcache(workOffset,workNode);
  while(RWNIL != workNode->sons(0))		// ...
  {
    workOffset = workNode->sons(0);	//least element
    readcache(workOffset,workNode);
  }
  char *tempKey = new char[KEYLEN];		// now swap new for old
  memcpy(tempKey,workNode->keys(0),KEYLEN);
  memcpy(workNode->keys(0),moreKey,KEYLEN);
  RWstoredValue tempItem = workNode->items(0);
  workNode->items(0) = moreItem;
  writecache(workOffset,workNode);		// and save the change

  workOffset = keepWorkOffset;			// get back home
  readcache(workOffset,workNode);
  memcpy(workNode->keys(k),tempKey,KEYLEN);	//and swap old for new
  workNode->items(k) = tempItem;
  writecache(workOffset,workNode);		// and save the change
  delete[] tempKey;
}

#ifdef RDEBUG
void
RWBTreeOnDisk::tp(RWoffset o)
{
  workOffset = o;
  readcache(workOffset,workNode);
  cout << "==================================================" << endl;
  cout << "Node " << workOffset;
  cout << " -- " << workNode->counter() << " items.";
  if(workOffset == baseInfo.rootLoc) 
    cout << " (ROOT)" << endl;
  else
    cout << endl;
  cout << "--------------------------------------------------" << endl;
  cout.width(16); cout << "key" << "\t";
  cout.width(5); cout << "value" << "\t";
  cout.width(12); cout << "sons" << endl;
  cout << "--------------------------------------------------" << endl;
  RWoffset *keepOffs = new RWoffset[ORDER2+1];
  int keepCount = workNode->counter();
  memcpy(keepOffs,&(workNode->sons(0)),(keepCount +1)*sizeof(RWoffset));
  for(int i = 0; i < keepCount; i++)
  {
    cout.width(16); cout << workNode->keys(i) << "\t";
    cout.width(5); cout << workNode->items(i) << "\t";
    cout.width(12); cout << workNode->sons(i) << endl;
  }
  cout << "                \t     \t";
  cout.width(12); cout << workNode->sons(keepCount) << endl << endl;
  if(keepOffs[0] != RWNIL) 
  {
    for(i = 0; i <= keepCount; i++)
    {
      tp(keepOffs[i]);
    }
  }
  delete[] keepOffs;
}
#endif
