#ifndef __RW_FILEDLGH
#define __RW_FILEDLGH

/*********************************************************************
*
*  $Id: FileDlg.h,v 1.12 1994/01/10 23:52:01 cynthia Exp $
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
*  File        : FileDlg.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for a 
*                dialog that allows the user to navigate through the
*                file system to select a file
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

#include <rw/Xrw/Dialog.h>
#include <rw/Xrw/FileSB.h>

class RWFileDialog : public RWDialogView {

 public:
  RWFileDialog (RWView        *parentView,
					 const char    *name    = "RWFileDialog",
                RWButtonConfig buttons = RWOkApplyCancelOtherButton);
  virtual ~RWFileDialog();

#ifdef RWVIEW1_COMPATIBLE
  RWCString    *getResponse  (RWDialogStyle modal = RWModalDialog);
  RWCString    *getSelection ();
#else
  virtual RWDialogReply getResponse  (RWCString &answer);
  virtual RWDialogReply getResponse  (RWDialogStyle modal = RWModalDialog)
	                          {return getResponseToDialog(modal);}
  virtual RWCString     getSelection () const;
#endif


  virtual void setColor             (const char *);
  virtual void setFilterLabelString (const char * filterLabel)
	                                 { setApplyLabelString(filterLabel); }
  virtual void setOkLabelString     (const char *);
  virtual void setCancelLabelString (const char *);
  virtual void setApplyLabelString  (const char *);
  virtual void setOtherLabelString  (const char *);

  virtual RWClassID isA() const { return(__RWFILEDIALOG); }
  
// RESOURCE MEMBER FUNCTIONS
  inline void           setDirListLabel          (const char * lbl);
  inline RWCString      getDirListLabel          () const;
  inline void           setDirMask               (const char * msk);
  inline RWCString      getDirMask               () const;
  inline void           setDirSpec               (const char * spec);
  inline RWCString      getDirSpec               () const;
  inline void           setFileListLabel         (const char * lbl);
  inline RWCString      getFileListLabel         () const;
  inline void           setFilterLabel           (const char * lbl);
  inline RWCString      getFilterLabel           () const;

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

  virtual  RWBoolean removeOkMethod     (RWApplication *obj, 
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

  RWFileSelectionBox *fileBox;
  
private:
  RWPushButton * fdOkControl;
  RWPushButton * fdCancelControl;
  RWPushButton * fdApplyControl;
  RWPushButton * fdOtherControl;

  void configureButtons(RWButtonConfig);

};


/****
*   Inlines
****/

inline void RWFileDialog::setDirListLabel(const char * lbl)
{
  fileBox->setDirListLabelString(lbl);
}

inline RWCString RWFileDialog::getDirListLabel() const
{
  return (fileBox->getDirListLabelStringC());
}

inline void RWFileDialog::setDirMask(const char * msk)
{
  fileBox->setDirMask(msk);
}

inline RWCString RWFileDialog::getDirMask() const
{
  return (fileBox->getDirMaskC());
}

inline void RWFileDialog::setDirSpec(const char * spec)
{
  fileBox->setDirSpec(spec);
}

inline RWCString RWFileDialog::getDirSpec() const
{
  return (fileBox->getDirSpecC());
}

inline void RWFileDialog::setFileListLabel(const char * lbl)
{
  fileBox->setFileListLabelString(lbl);
}

inline RWCString RWFileDialog::getFileListLabel() const
{
  return (fileBox->getFileListLabelStringC());
}

inline void RWFileDialog::setFilterLabel(const char * lbl)
{
  fileBox->setFilterLabelString(lbl);
}

inline RWCString RWFileDialog::getFilterLabel() const
{
  return (fileBox->getFilterLabelStringC());
}



#endif


