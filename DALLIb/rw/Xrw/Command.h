#ifndef __RW_COMMAND_H
#define __RW_COMMAND_H
/*********************************************************************
*
*  $Id: Command.h,v 1.14 1993/10/07 01:42:57 keffer Exp $
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
*  File        : Command.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : December 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                Command class.
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

#include <rw/Xrw/SelectioB.h>
#include <rw/Xrw/CommCB.h>
	 
class RWText;
class RWList;
class RWLabel;

/*********************************************************************
* RWCommand : Class object which represents the XmCommand
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWCommand : public RWSelectionBox
{
 public:
  RWCommand
	(
	RWController *    the_parent_of_this_widget, 
	const RWCString & the_name_for_resource_files, 
	RWBoolean         manage_it = False
	);
  RWCommand
	(
	RWController *        the_parent_of_this_widget, 
	const RWCString &     the_name_for_resource_files, 
	const RWResourceObj & resources_to_be_set_before_instantiation,
	RWBoolean             manage_it = False
	);
  RWCommand
	(
	RWController *  the_parent_of_this_widget, 
	RWResourceObj * resources_to_be_set_before_instantiation = 0
	);
  RWCommand
	(
	Widget     the_parent_widget_of_this_widget, 
	RWBoolean  manage_it = False
	);  // Internal.

  ~RWCommand();

  void                createWidget               (char *, RWBoolean managed = False);

// RESOURCE MEMBER FUNCTIONS
  void 		      setCommand		 (RWXmString *);
  RWXmString *	      getCommand		 (RWXmString **val = 0);
  void 		      setCommand		 (const RWCString &);
  RWCString	      getCommandC		 (RWCString *val = 0);
  void 		      setCommandChangedCallback  (RWXtCallbackProc, RWXtPointer);
  void 		      setCommandChangedMethod    (RWApplication *, RWMethodPtr, void *);
  void 		      setCommandChangedCallback  (XtCallbackList);
  void 		      setCommandEnteredCallback  (RWXtCallbackProc, RWXtPointer);
  void 		      setCommandEnteredMethod    (RWApplication *, RWMethodPtr, void *);
  void 		      setCommandEnteredCallback  (XtCallbackList);
  void 		      setHistoryItems	         (RWXmStringList);
  void 		      setHistoryItems	         (RWXmStringList, int);
  RWXmStringList      getHistoryItems	         (RWXmStringList *val = 0);
  void 		      setHistoryItems	         (RWCStringOrderedVec &);
  RWCStringOrderedVec getHistoryItemsC           ();
  void                getHistoryItemsC           (RWCStringOrderedVec &);
  void 		      setHistoryItemsC	         (char **, int);
  void 		      setHistoryItemCount	 (int);
  int 		      getHistoryItemCount	 (int *val = 0);
  void 		      setHistoryMaxItems	 (int);
  int 		      getHistoryMaxItems	 (int *val = 0);
  void 		      setHistoryVisibleItemCount (int);
  int 		      getHistoryVisibleItemCount (int *val = 0);
  void 		      setPromptString	         (RWXmString *);
  RWXmString *	      getPromptString	         (RWXmString **val = 0);
  void 		      setPromptString	         (const RWCString &);
  RWCString 	      getPromptStringC	         (RWCString *val = 0);

// MOTIF MEMBER FUNCTIONS  
  void 		 appendValue    (RWXmString *);
  void 		 appendValue    (const RWCString &);
  void 		 error          (RWXmString *);
  void 		 error          (const RWCString &);
  RWController * getChild       (unsigned char);
  RWText *       getCommandText ();
  RWLabel *      getPromptLabel ();
  RWList *       getHistoryList ();
  RWController * getWorkArea    ();
  void 		 setValue       (RWXmString *);
  void 		 setValue       (const RWCString &);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const 
	{
	return __RWCOMMAND;
	}
  
 private:
  RWCallback * getCallbackObject (RWXtPointer);
};

#endif  /* End of file Command.h - Do not add beyond this point! */
