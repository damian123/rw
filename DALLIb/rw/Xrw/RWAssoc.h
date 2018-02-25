#ifndef __RW_ASSOCH
#define __RW_ASSOCH

/*********************************************************************
*
*  $Id: RWAssoc.h,v 1.10 1994/01/11 00:13:11 cynthia Exp $
*
*  COPYRIGHT   : (c) Copyright 1991, Rogue Wave Software, Inc.
*                ALL RIGHTS RESERVED
*
*                The software and information contained herein
*                are proprietary to, and comprise valuable trade
*                secrets of, Rogue Wave Software, Inc., which
*                intends to preserve as trade secrets such software
*                and information.  This software is furnished pursuant
*                to a written license agreement and may be used,
*                copied, transmitted, and stored only in accordance
*                with the terms of such license and with the inclusion
*                of the above copyright notice.  This software and
*                information or any other copies thereof may not be
*                provided or otherwise made available to any other
*                person.
*  
*  NOTICE      : Notwithstanding any other lease or license that may
*                pertain to, or accompany the delivery of, this
*                computer software and information, the rights of the
*                Government regarding its use, reproduction and
*                disclosure are as set forth in Section 52.227-19 of
*                the FARS Computer Software-Restricted Rights clause.
*  
*  RESTRICTED 
*  RIGHTS 
*  NOTICE      : Use, duplication, or disclosure by the Government
*                is subject to restrictions as set forth in
*                subparagraph (c)(1)(ii) of the Rights in Technical
*                Data and Computer Software clause at DFARS
*                52.227-7013.
*  
*  RESTRICTED
*  RIGHTS
*  LEGEND      : This computer software and information is distributed
*                with "restricted rights."  Use, duplication or
*                disclosure is subject to restrictions as set forth
*                in NASA FAR SUP 18-52.227-79 (April 1985)
*                "Commercial Computer Software-Restricted Rights
*                (April 1985)."  If the Clause at 18-52.227-74
*                "Rights in Data General" is specified in the
*                contract, then the "Alternate III" clause applies.
*
*                (c) Copyright 1991, Rogue Wave Software, Inc.
*                ALL RIGHTS RESERVED
*
*  TRADEMARKS  : Rogue Wave is a trademark of
*                   Rogue Wave Software, Inc.
*                OSF/Motif is a trademark of
*                   Open Software Foundation, Inc.
*                Motif is a trademark of
*                   Open Software Foundation, Inc.
*                X Window System is a trademark of
*                   the Massachusetts Institute of Technology
*
*  Rogue Wave  : Rogue Wave Software, Inc.
*                P.O. Box 2328
*                Corvallis, OR 97330
*                (503) 754-2311
*
*  ------------------------------------------------------------------
*
*  File        : RWAssoc.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file containes the C++ class declaration for
*                associations
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

#ifndef __RWVIEWDEFS_H__
# include <rw/Xrw/viewdefs.h>
#endif
#include <rw/collect.h>
#include <rw/Xrw/TypeId.h>

class RWApplication;
class RWBag;
class RWCollectableString;
class RWCString;
class RWHashDictionary;

class RWAssociation : public RWCollectable {

public:
  RWAssociation
	(
	RWApplication *   assocObj, 
	RWApplication *   inverseObj,
	const RWCString & assocName, 
	const RWCString & inverseName
	);
  virtual ~RWAssociation();

  RWBoolean       associates
			(
			RWApplication * anObj,
			RWApplication * anotherObj
			);
  RWApplication * getAssocObject
			(
			RWCollectableString & assocName
			);

  virtual RWClassID isA() const 
	{ 
	return(__RWASSOCIATION); 
	}
  
  static const RWCollectableString & findInverse
					(
					const RWCString & assocName
					);
  static const RWCollectableString & findAssocName
					(
					const RWCString & assocName
					);
#ifdef RWDEBUG
  static void 			     printAssocNames ();
#endif

protected:
  RWBoolean alreadyDefined
		(
		RWCollectableString & prevAName,
		RWCollectableString & prevIName
		);
  RWBoolean insertAssocNames
		(
		RWCollectableString & assocName,
		RWCollectableString & invName
		);
  RWBoolean deleteAssocNames
		(
		RWCollectableString * oneName,
		RWCollectableString * otherName
		);

  RWCollectableString * forwardAssoc;
  RWApplication       * forwardApp;
  RWCollectableString * inverseAssoc;
  RWApplication       * inverseApp;

private:
  static RWBag            * definedAssocs;
  static RWHashDictionary * assocPairs;

};

#endif   /* __RW_ASSOCH */
