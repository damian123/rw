#ifndef __RW_REQUESTH
#define __RW_REQUESTH

/*********************************************************************
*
*  $Id: Request.h,v 1.15 1994/01/11 00:13:11 cynthia Exp $
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
*  File        : Request.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for a 
*                dialog that displays an informational message to the user
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

#include <rw/Xrw/Dialog.h>
#include <rw/Xrw/Text.h>

/*********************************************************************
*  Resources for components of an RWRequestDialog may be set in an
*  app-defaults file using strings composed of the "name" given in
*  the dialog constructor with identifying strings for each
*  component appended.  The names for each component are created
*  as follows:
*
*  Example: if name =    "myDialog":
*
*  COMPONENT              NAME   ID STRING       COMPONENT NAME
*  ---------              ----   ---------       --------------
*  textEntry_           : name + "TEXT"          myDialogTEXT
*
*  name defaults to:     "RWRequestDialog"
*
*  See Dialoh.h for identifying strings of inherited components.
*********************************************************************/

class RWRequestDialog : public RWDialogView {

public:
  RWRequestDialog(RWView * parentView,
		  const char* name = "RWRequestDialog",
		  RWButtonConfig buttons = RWOkCancelButton);
  virtual ~RWRequestDialog();

#ifdef RWVIEW1_COMPATIBLE
  RWCString *getResponse (RWCString *initDisplay = rwnil,
			  RWDialogStyle modal = RWModalDialog);
  RWCString *getSelection();
#else

  virtual RWDialogReply getResponse (RWCString& answer,
				     const char * initDisplay);
  virtual RWDialogReply getResponse (const char * initDisplay,
				     RWDialogStyle modal = RWModalDialog);
  virtual RWDialogReply getResponse (RWDialogStyle modal = RWModalDialog)
	                             {return getResponseToDialog(modal);}

  virtual RWCString getSelection() const;
#endif

  void setScrollable(RWBoolean scrollable,
		     RWBoolean wordWrap = TRUE);

  virtual RWClassID isA() const { return(__RWREQUESTDIALOG); }
  
// RESOURCE MEMBER FUNCTIONS
  inline void             setEditable			(RWBoolean editable);
  inline RWBoolean        getEditable			() const;
  inline void             setMarginHeight		(int hei);
  inline int              getMarginHeight		() const;
  inline void             setMarginWidth		(int wid);
  inline int              getMarginWidth		() const;
  inline void             setMaxLength			(int maxLength);
  inline int              getMaxLength			() const;
  inline void             setTopCharacter		(int topChar);
  inline int              getTopCharacter		() const;
  inline void             setColumns 			(short numCols);
  inline short            getColumns			() const;
         void             setTextFont			(const char * fontName);
         void             setRows			(short numRows);
  inline short            getRows			() const;
         void             setWordWrap			(RWBoolean wrap);
  inline RWBoolean        getWordWrap			() const;

// TEXT MEMBER FUNCTIONS  
  inline RWCString        getTextSelection		() const;
  inline void             setString			(const char * str);
  inline RWCString        getString			() const;
  inline void             insert			(int pos,
							       const char * str);
  inline void             scroll		      	(int nlines);
  inline void             showPosition			(int pos);

protected:
// Callback Methods
  virtual void              textEntryActivateMethod(RWApplication*, //control
						    RWXtPointer,    //clientData
						    RWXtPointer)    //callData
	                         {;}
  
  RWText *textEntry_;

private:
  RWBoolean isScrolledText_;
  void                    createTextEntry     (RWController    * parent,
					       const RWCString & name,
					       RWBoolean         scrollable,
					       RWBoolean         wordWrap);
};


/****************************************
 *		INLINES			*
 ****************************************/

inline void RWRequestDialog::setEditable(RWBoolean editable)
{
  textEntry_->setEditable(editable);
}

inline RWBoolean RWRequestDialog::getEditable() const
{
  return (textEntry_->getEditable());
}

inline void RWRequestDialog::setMarginHeight(int hei)
{
  textEntry_->setMarginHeight((Dimension)hei);
}

inline int RWRequestDialog::getMarginHeight() const
{
  return ((int)textEntry_->getMarginHeight());
}

inline void RWRequestDialog::setMarginWidth(int wid)
{
  textEntry_->setMarginWidth((Dimension)wid);
}

inline int RWRequestDialog::getMarginWidth() const
{
  return ((int)textEntry_->getMarginWidth());
}

inline void RWRequestDialog::setMaxLength(int maxLength)
{
  textEntry_->setMaxLength(maxLength);
}

inline int RWRequestDialog::getMaxLength() const
{
  return (textEntry_->getMaxLength());
}

inline void RWRequestDialog::setTopCharacter(int topChar)
{
  textEntry_->setTopCharacter((XmTextPosition)topChar);
}

inline int RWRequestDialog::getTopCharacter() const
{
  return ((int)textEntry_->textGetTopCharacter());
}

inline void RWRequestDialog::setColumns(short numCols)
{
  textEntry_->setColumns(numCols);
}

inline short RWRequestDialog::getColumns() const
{
  return (textEntry_->getColumns());
}

inline short RWRequestDialog::getRows() const
{
  return (textEntry_->getRows());
}


inline RWBoolean RWRequestDialog::getWordWrap() const
{
  return (textEntry_->getWordWrap());
}

inline RWCString RWRequestDialog::getTextSelection() const
{
  return (textEntry_->getSelection());
}

inline void RWRequestDialog::setString(const char * str)
{
  textEntry_->setString(str);
}

inline RWCString RWRequestDialog::getString() const
{
  return (textEntry_->getString());
}

inline void RWRequestDialog::insert (int pos,
				     const char * str)
{
  textEntry_->insert((XmTextPosition)pos, (char*)str);
}

inline void RWRequestDialog::scroll(int nlines)
{
  textEntry_->scroll(nlines);
}

inline void RWRequestDialog::showPosition(int pos)
{
  textEntry_->showPosition((XmTextPosition)pos);
}

#endif
