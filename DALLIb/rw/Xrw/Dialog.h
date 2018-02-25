#ifndef __RW_DIALOGH
#define __RW_DIALOGH

/*********************************************************************
*
*  $Id: Dialog.h,v 1.15 1994/01/10 23:52:01 cynthia Exp $
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
*  File        : Dialog.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                abstract base class from which all application class
*                dialogs are derived
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

#include <rw/Xrw/View.h>
#include <rw/Xrw/PushB.h>
#include <rw/cstring.h>

class RWLabel;
class RWForm;
class RWSeparator;
class RWManager;

/*********************************************************************
*  Resources for components of an RWDialog may be set in an
*  app-defaults file using strings composed of the "name" given in
*  the dialog constructor with identifying strings for each
*  component appended.  The names for each component are created
*  as follows:
*
*  Example: if name =    "myDialog":
*
*  COMPONENT              NAME   ID STRING       COMPONENT NAME
*  ---------              ----   ---------       --------------
*  buttonForm           : name + "BF"            myDialogBF
*  buttonSep            : name + "BS"            myDialogBS
*  okControl            : name + "OK"            myDialogOK
*  cancelControl        : name + "CANCEL"        myDialogCANCEL
*  applyControl         : name + "APPLY"         myDialogAPPLY
*  otherControl         : name + "OTHER"         myDialogOTHER
*  messageDisplay       : name + "MD"            myDialogMD
*
*  name defaults to:     "RWDialogView"
*
*  See View.h for identifying strings of inherited components.
*********************************************************************/


class RWDialogView : public RWView {

 public:
  RWDialogView (RWView           *parentView, 
                const RWCString  &name,
		RWButtonConfig    buttons = RWOkButton,
		RWBoolean         hasMenu = FALSE);
  RWDialogView (RWManager        *parent, 
                const RWCString  &name,
		RWButtonConfig    buttons = RWOkButton,
		RWBoolean         hasMenu = FALSE);
  virtual ~RWDialogView();

  virtual void startView();

#ifdef RWVIEW1_COMPATIBLE
  virtual void setPart (RWController *part, 
			int xPos = 0, 
			int yPos = 0);
#endif
  
  virtual void setDisplayMessage    (const char * message)
                                    { messageDisplay->setLabelString(message); }
  virtual void setColor             (const char* color);
  virtual void setFont              (const char* font);
  virtual void setDimensions        (int wid, int hei);

  virtual void setOkLabelString     (const char * label);
  virtual void setCancelLabelString (const char * label);
  virtual void setApplyLabelString  (const char * label);
  virtual void setOtherLabelString  (const char * label);

  virtual RWClassID isA() const { return(__RWDIALOG); }

  RWPushButton *getOkControl() const               { return okControl; }
  RWPushButton *getCancelControl() const           { return cancelControl; }
  RWPushButton *getApplyControl() const            { return applyControl; }
  RWPushButton *getOtherControl() const            { return otherControl; }

  void cancelDialog(RWApplication*, 
		    RWXtPointer, 
		    RWXtPointer)                   { stopView(); }
  
  void            useUniformButtons (RWBoolean b = TRUE)
	                                                { uniformButtons = b; }
  RWBoolean       usingUniformButtons () const     { return uniformButtons; }
  void            setDialogStyle(RWDialogStyle modal = RWModalDialog);
  RWDialogStyle   getDialogStyle() const           { return dialogModality; }
  RWDialogReply   getUserResponse() const          { return userResponse_; }
#ifdef RWVIEW1_COMPATIBLE
  RWDialogStyle   dialogStyle() const              { return dialogModality; }
  RWDialogReply   userResponse() const             { return userResponse_; }
#endif

// RESOURCE MEMBER FUNCTIONS
  void                  setAlignment (RWLabelAlignment alignment);
  RWLabelAlignment      getAlignment () const;

// Add and delete methods for default buttons
  virtual  RWBoolean addOkMethod     (RWApplication *obj, 
				      RWMethodPtr method,
				      RWXtPointer data);
  virtual  RWBoolean addCancelMethod (RWApplication *obj, 
				      RWMethodPtr method,
				      RWXtPointer data);
  virtual  RWBoolean addApplyMethod  (RWApplication *obj, 
				      RWMethodPtr method,
				      RWXtPointer data);
  virtual  RWBoolean addOtherMethod  (RWApplication *obj, 
				      RWMethodPtr method,
				      RWXtPointer data);
  
  virtual  RWBoolean removeOkMethod  (RWApplication *obj, 
				      RWMethodPtr method,
				      RWXtPointer data);
  virtual  RWBoolean removeCancelMethod (RWApplication *obj, 
					 RWMethodPtr method,
					 RWXtPointer data);
  virtual  RWBoolean removeApplyMethod  (RWApplication *obj, 
					 RWMethodPtr method,
					 RWXtPointer data);
  virtual  RWBoolean removeOtherMethod  (RWApplication *obj, 
					 RWMethodPtr method,
					 RWXtPointer data);

protected:
// Callback Methods
  virtual void              replyAndExit       (RWApplication* control, 
						RWXtPointer    clientData, 
						RWXtPointer    callData);

  virtual void              wmCloseMethod      (RWApplication* control, 
						RWXtPointer    clientData, 
						RWXtPointer    callData)
	                         {
				   if (dialogModality == RWModalDialog)
				     replyAndExit(control, clientData, callData);
				   else
				     stopView();
				 }

  virtual void              dialogOkMethod     (RWApplication* control, 
						RWXtPointer    clientData, 
						RWXtPointer    callData)
				{ replyAndExit(control, clientData, callData); }
  
  virtual void              dialogApplyMethod  (RWApplication*, //control
						RWXtPointer,    //clientData
						RWXtPointer)    //callData
	                         {}
  
  virtual void              dialogCancelMethod (RWApplication* control, 
						RWXtPointer    clientData, 
						RWXtPointer    callData)
                            { replyAndExit(control, clientData, callData); }
  
  virtual void              dialogOtherMethod  (RWApplication*, //control
						RWXtPointer,    //clientData
						RWXtPointer)    //callData
				{;}
  
  virtual void              mapMethod          (RWApplication *, //control
						RWXtPointer,     //clientData
						RWXtPointer)     //callData
				{;}
  
  virtual void              unmapMethod        (RWApplication *, //control
						RWXtPointer,     //clientData
						RWXtPointer)     //callData
				{;}


  virtual void updateDisplayParams () { defaultUpdateDisplayParams(); }
  void         defaultUpdateDisplayParams ();

  virtual void setDialogLayout()      {;}

  void beginModal();
  RWDialogReply getResponseToDialog (RWDialogStyle modal = RWModalDialog);

  RWButtonConfig buttonArrangement;
  RWForm       * buttonForm;
  RWSeparator  * buttonSep;
  RWPushButton * okControl;
  RWPushButton * cancelControl;
  RWPushButton * applyControl;
  RWPushButton * otherControl;
  RWLabel      * messageDisplay;
  XFontStruct  * localFont;
  RWDialogStyle  dialogModality;
  RWDialogReply  userResponse_;
  RWCString      dialogColor;
  RWCString      dialogFont;
  RWCString      okLabelString;
  RWCString      cancelLabelString;
  RWCString      applyLabelString;
  RWCString      otherLabelString;
  RWBoolean      uniformButtons;

private:
  RWBoolean captured;

  void createDialog(const RWCString & name);
  void createDialogButtons(const RWCString & name);

  void positionButtons();
  void positionButtons(RWPushButton* b1);
  void positionButtons(RWPushButton* b1,
		       RWPushButton* b2);
  void positionButtons(RWPushButton* b1,
		       RWPushButton* b2,
		       RWPushButton* b3);
  void positionButtons(RWPushButton* b1,
		       RWPushButton* b2,
		       RWPushButton* b3,
		       RWPushButton* b4);
  void enforceSpacing();
  
};

#endif /* __RW_DIALOGH */
