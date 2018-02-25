#ifndef __RW_APPLICATION_H
#define __RW_APPLICATION_H
/*********************************************************************
*
*  $Id: App.h,v 1.20 1993/10/24 01:55:36 keffer Exp $
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
*  File        : App.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the declaration of the root node
*                of View.h++.  It is an abstract type and should not be
*                instantiated directly.  All View.h++ controller objects
*                are derived types of RWApplication.  Likewise,
*                the top level user application object(s) should be
*                derived from RWApplication.
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

  
/*********************************************************************
* #include files
*********************************************************************/

#ifndef __RWVIEWDEFS_H__
#include <rw/Xrw/viewdefs.h>
#endif

#include <X11/Intrinsic.h>

#include <rw/model.h>
#include <rw/hashdict.h>
#include <rw/ordcltn.h>
#include <rw/Xrw/TypeId.h>
#include <rw/Xrw/RWAssoc.h>

class RWCString;
class RWCollectableString;
class RWCallback;

/*********************************************************************
* RWApplication : The root node object of the View.h++ system.  It is
* simply an abstract type from which all other objects, View.h++
* controller objects and end user application objects, will be
* derived.  A user building an application in View.h++ should include
* this header file in their application root object header and make
* the root object a public derived type of this RWApplication object.
*********************************************************************/

class RWApplication  : public RWModelClient
{
 public:
  RWApplication() {;}

  virtual ~RWApplication();


  virtual void			updateFrom(RWModel*, void*) {;}

  virtual RWCallback *          getCallbackObject
					(
					RWXtPointer
					) { return(0); }

  virtual void                  associateWith
					(
					RWApplication *    obj, 
					const RWCString &  assocName,
					const RWCString &  inverseName
					);
  virtual void                  associateWith
					(
					RWApplication *    obj, 
					const RWCString &  assocName
					);
  virtual const RWOrdered       getAssociation
					(
					const RWCString & assocName
					);
  virtual const RWApplication * getFirstAssociation
					(
					const RWCString & assocName
					);
  virtual const RWAssociation * getAssociation
					(
					const RWCString & assocName,
				 	RWApplication * obj
					);
  virtual void 			deleteAssociation
					(
					RWApplication * obj, 
					RWCString *     assocName
					);

#ifdef RDEBUG
  void printAssocs();
#endif

// RW TOOLS MEMBER FUNCTIONS
 
  virtual RWClassID 		isA () const { return __RWAPPLICATION; }

protected:
  virtual void 			insertAssoc
					(
					RWAssociation &       theAssoc,
			   		RWCollectableString & assocName
					);
  virtual void 			deleteAssociation
					(
					RWAssociation &       theAssoc,
				 	RWCollectableString & assocName,
				 	int                   recursed = 0
					);

  RWHashDictionary associations;
  
};


class RWFileSelectionBox;
class RWFileSelectionBoxCallback;


typedef void    (RWApplication::*RWWrapMethod)            (RWXtPointer);
typedef void    (RWApplication::*RWMethodPtr)             (RWApplication *a,
							   void *v,
							   RWCallback *c);
typedef Boolean (RWApplication::*RWConvertMethodPtr)      (RWApplication *, 
							   Atom *, 
							   Atom *, 
							   Atom *,
							   XtPointer *, 
							   unsigned long *,
							   int *, 
							   unsigned long *, 
							   XtPointer, 
							   XtRequestId *);   
typedef void    (RWApplication::*RWEventMethodPtr)        (RWApplication *, void *, 
							   XEvent *, Boolean *);
typedef void    (RWApplication::*RWTimerMethodPtr)        (RWXtPointer, XtIntervalId *);
typedef Boolean (RWApplication::*RWWorkProcMethodPtr)     (RWXtPointer);
typedef void    (RWApplication::*RWXtSelectionMethodProc) (RWApplication *, RWXtPointer, 
							   Atom *, Atom *, RWXtPointer, 
							   unsigned long *,int *);
typedef void    (RWApplication::*RWSearchProcMethod)      (RWFileSelectionBox *, 
							   RWFileSelectionBoxCallback *);

typedef void    (*RWCallbackPtr)             (RWApplication *, void *, RWCallback *);
typedef void    (*RWXtSelectionCallbackProc) (RWApplication *, RWXtPointer, Atom *, Atom *,
					      RWXtPointer, unsigned long *,int *);
typedef Boolean (*RWConvertPtr)              (RWApplication *,
					      Atom *,
					      Atom *,
					      Atom *,
					      XtPointer *,
					      unsigned long *,
					      int *,
					      unsigned long *,
					      XtPointer,
					      XtRequestId *);
typedef void    (*RWEventPtr)                (RWApplication *, void *, XEvent*, Boolean *);
typedef void    (*RWTimerCallbackPtr)        (RWXtPointer, XtIntervalId *);
typedef Boolean (*RWWorkProcCallbackPtr)     (RWXtPointer);
typedef void    (*RWSearchProc)              (RWFileSelectionBox *, 
					      RWFileSelectionBoxCallback *);


/*Enum for Mouse Notifier events*/
enum MouseState { NoKeys = 0, ButtonOne = 1, ButtonTwo = 2, ButtonThree = 4,
		  ShiftKey = 8, ControlKey = 16, MetaKey = 32 };


#endif
