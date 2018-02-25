#ifndef __RW_BUTTONDLGH
#define __RW_BUTTONDLGH

/*********************************************************************
*
*  $Id: ButtonDlg.h,v 1.13 1994/01/10 23:30:06 cynthia Exp $
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
*  File        : ButtonDlg.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the 
*                Dialog class which accepts users input via button 
*                selection, via radio- or toggle-button styles
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

#include <rw/Xrw/Dialog.h>
#include <rw/Xrw/RowColumn.h>

class RWFrame;
class RWScrolledWindow;

/*********************************************************************
*  Resources for components of an RWButtonsDialog may be set in an
*  app-defaults file using strings composed of the "name" given in
*  the dialog constructor with identifying strings for each
*  component appended.  The names for each component are created
*  as follows:
*
*  Example: if name =    "myDialog":
*
*  COMPONENT              NAME   ID STRING       COMPONENT NAME
*  ---------              ----   ---------       --------------
*  buttonFrame_         : name + "TBF"           myDialogTBF
*  buttonScrWin_        : name + "BSCRWIN"       myDialogBSCRWIN
*  buttonBox_           : name + "BBOX"          myDialogBBOX
*  <tgl buttons>        : name + "TBUTTONS"      myDialogTBUTTONS
*
*  name defaults to:     "RWButtonDialog"
*
*  See Dialoh.h for identifying strings of inherited components.
*********************************************************************/

class RWRowColumn;


class RWButtonDialog : public RWDialogView {
public:

  RWButtonDialog (RWView *, 
						const char* name = "RWButtonDialog",
						RWButtonStyle style = RWRadioButtonStyle,
						RWButtonConfig buttonConfig = RWOkCancelButton);
  virtual ~RWButtonDialog();

#ifdef RWVIEW1_COMPATIBLE
  RWOrdered   *getResponse (const RWOrdered &, 
									 RWDialogStyle modal = RWModalDialog);
  RWOrdered   *getSelection ();
#else
  virtual RWDialogReply getResponse (RWCStringOrderedVec& answer,
												 const RWCStringOrderedVec& choices);
  virtual RWDialogReply getResponse (const RWCStringOrderedVec& choices,
												 RWDialogStyle modal = RWModalDialog);
  virtual RWDialogReply getResponse (RWDialogStyle modal = RWModalDialog)
	                           {return getResponseToDialog(modal);}

  RWCStringOrderedVec getSelection () const;
#endif

  void         setButtonBoxColor (const char * color);
  RWButtonStyle  setStyle          (RWButtonStyle newStyle);
  void         resetButtonState  ();

  virtual RWClassID isA() const { return(__RWBUTTONDIALOG); }

// RESOURCE MEMBER FUNCTIONS
  inline void           setLabelAlignment 	      (RWLabelAlignment);
  inline RWLabelAlignment getLabelAlignment        () const;
  inline void           setMinorDimensionExtent		(short);
  inline short          getMinorDimensionExtent		() const;
  inline void           setOrientation		         (RWOrientation);
  inline RWOrientation  getOrientation		         () const;
  inline void           setSpacing		            (int);
  inline int            getSpacing		            () const;
  
protected:


// CALLBACK METHODS
  virtual void              buttonMethod       (RWApplication*, //control
																RWXtPointer,    //clientData
																RWXtPointer)    //callData
	                         {}

  RWFrame          *buttonFrame_;
  RWScrolledWindow *buttonScrWin_;
  RWRowColumn      *buttonBox_;
  RWOrdered        *buttons_;
  RWButtonStyle    dialogButtonType_;
  
};


/****
*   Inlines
****/

inline void RWButtonDialog::setLabelAlignment(RWLabelAlignment alignment)
{
  buttonBox_->setEntryAlignment((unsigned char)alignment);
}

inline RWLabelAlignment RWButtonDialog::getLabelAlignment() const
{
  return ((RWLabelAlignment)buttonBox_->getEntryAlignment());
}


inline void RWButtonDialog::setMinorDimensionExtent(short extent)
{
  buttonBox_->setNumColumns(extent);
}

inline short RWButtonDialog::getMinorDimensionExtent() const
{
  return (buttonBox_->getNumColumns());
}


inline void RWButtonDialog::setOrientation(RWOrientation orient)
{
  buttonBox_->setOrientation((unsigned char)orient);
}

inline RWOrientation RWButtonDialog::getOrientation() const
{
  return ((RWOrientation)buttonBox_->getOrientation());
}


inline void RWButtonDialog::setSpacing(int spacing)
{
  buttonBox_->setSpacing((Dimension)spacing);
}

inline int RWButtonDialog::getSpacing() const
{
  return ((int)buttonBox_->getSpacing());
}


#endif /*__RW_BUTTONDLGH*/
