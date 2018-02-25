#ifndef __RW_FILE_SELECTION_BOX_H
#define __RW_FILE_SELECTION_BOX_H
/*********************************************************************
*
*  $Id: FileSB.h,v 1.12 1993/10/07 01:42:57 keffer Exp $
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
*  File        : FileSB.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : December 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                File Selection Box and File Selection Dialog classes.
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
#include <rw/Xrw/FileSBCB.h>
#include <rw/Xrw/Callback.h>

/*********************************************************************
* RWFileSelectionBox : Class object which represents the XmFileSelectionBox
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWFileSelectionBox : public RWSelectionBox
{
 public:
  RWFileSelectionBox
    (
    RWController *, 
    const RWCString &,
    RWBoolean managed = False
    );
  RWFileSelectionBox
    (
    RWController *, 
    const RWCString &,
    const RWResourceObj &,
    RWBoolean managed = False
    );
  RWFileSelectionBox
    (
    RWController *, 
    RWResourceObj * ro = 0
    );
  RWFileSelectionBox();
  RWFileSelectionBox
    (
    Widget, 
    RWBoolean managed = False
    );  // Internal.
  ~RWFileSelectionBox();

  void                 createWidget             (char *, RWBoolean managed = False);

// RESOURCE MEMBER FUNCTIONS
  void                 setDirectory             (RWXmString *);
  RWXmString *         getDirectory             (RWXmString **val = 0);
  void                 setDirectory             (const RWCString &);
  RWCString            getDirectoryC            (RWCString *val = 0);
  void                 setDirectoryValid        (Boolean);
  Boolean              getDirectoryValid        (Boolean *val = 0);
  void                 setDirListItems          (RWXmStringList);
  void                 setDirListItems          (RWXmStringList, int);
  RWXmStringList       getDirListItems          (RWXmStringList *val = 0);
  void                 setDirListItems          (RWCStringOrderedVec &);
  void                 setDirListItemsC         (char **, int);
  RWCStringOrderedVec  getDirListItemsC         ();
  void                 getDirListItemsC         (RWCStringOrderedVec &);
  void                 setDirListItemCount      (int);
  int                  getDirListItemCount      (int *val = 0);
  void                 setDirListLabelString    (RWXmString *);
  RWXmString *         getDirListLabelString    (RWXmString **val = 0);
  void                 setDirListLabelString    (const RWCString &);
  RWCString            getDirListLabelStringC   (RWCString *val = 0);
  void                 setDirMask               (RWXmString *);
  RWXmString *         getDirMask               (RWXmString **val = 0);
  void                 setDirMask               (const RWCString &);
  RWCString            getDirMaskC              (RWCString *val = 0);
  void                 setDirSearchProc         (RWSearchProc);
  void                 setDirSearchProc         (RWApplication *, RWSearchProcMethod);
  RWSearchProc         getDirSearchProc         (RWSearchProc *val = 0);
  void                 setDirSpec               (RWXmString *);
  RWXmString *         getDirSpec               (RWXmString **val = 0);
  void                 setDirSpec               (const RWCString &);
  RWCString            getDirSpecC              (RWCString *val = 0);
  void                 setFileListItemCount     (int);
  int                  getFileListItemCount     (int *val = 0);
  void                 setFileListItems         (RWXmStringList);
  void                 setFileListItems         (RWXmStringList, int);
  RWXmStringList       getFileListItems         (RWXmStringList *val = 0);
  void                 setFileListItems         (RWCStringOrderedVec &);
  void                 setFileListItemsC        (char **, int);
  void                 getFileListItemsC        (RWCStringOrderedVec &);
  RWCStringOrderedVec  getFileListItemsC        ();
  void                 setFileListLabelString   (RWXmString *);
  RWXmString *         getFileListLabelString   (RWXmString **val = 0);
  void                 setFileListLabelString   (const RWCString &);
  RWCString            getFileListLabelStringC  (RWCString *val = 0);
  void                 setFileSearchProc        (RWSearchProc);
  void                 setFileSearchProc        (RWApplication *, RWSearchProcMethod);
  RWSearchProc         getFileSearchProc        (RWSearchProc *val = 0);
  void                 setFileTypeMask          (unsigned char);
  unsigned char        getFileTypeMask          (unsigned char *val = 0);
  void                 setFilterLabelString     (RWXmString *);
  RWXmString *         getFilterLabelString     (RWXmString **val = 0);
  void                 setFilterLabelString     (const RWCString &);
  RWCString            getFilterLabelStringC    (RWCString *val = 0);
  void                 setListUpdated           (Boolean);
  Boolean              getListUpdated           (Boolean *val = 0);
  void                 setNoMatchString         (RWXmString *);
  RWXmString *         getNoMatchString         (RWXmString **val = 0);
  void                 setNoMatchString         (const RWCString &);
  RWCString            getNoMatchStringC        (RWCString *val = 0);
  void                 setPattern               (RWXmString *);
  RWXmString *         getPattern               (RWXmString **val = 0);
  void                 setPattern               (const RWCString &);
  RWCString            getPatternC              (RWCString *val = 0);
  void                 setQualifySearchDataProc (RWSearchProc);
  void                 setQualifySearchDataProc (RWApplication *, RWSearchProcMethod);
  RWSearchProc         getQualifySearchDataProc (RWSearchProc *val = 0);

// MOTIF MEMBER FUNCTIONS  
  RWController * getChild                 (unsigned char);
  void           doSearch                 (RWXmString *);
  void           doSearch                 (const RWCString &);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const { return __RWFILESELECTIONBOX; }
  
  RWCallbackInfo * dspcb_;
  RWCallbackInfo * fspcb_;
  RWCallbackInfo * qspcb_;

  RWCallbackInfo * dspcb()  { return dspcb_; }
  RWCallbackInfo * fspcb()  { return fspcb_; }
  RWCallbackInfo * qspcb()  { return qspcb_; }

  static void RWDirSearchProcDispatcher         (Widget, XmFileSelectionBoxCallbackStruct *);
  static void RWFileSearchProcDispatcher        (Widget, XmFileSelectionBoxCallbackStruct *);
  static void RWQualifySearchDataProcDispatcher (Widget, XmFileSelectionBoxCallbackStruct *);

 private:
  RWCallback *getCallbackObject (RWXtPointer);
};


#endif


#ifndef __RW_FILE_SELECTION_DIALOG_H
#define __RW_FILE_SELECTION_DIALOG_H
/*********************************************************************
* RWFileSelectionDialog : Class object which represents the XmFileSelectionDialog
* widget in the OSF/Motif widget set.
*********************************************************************/

class RWFileSelectionDialog : public RWFileSelectionBox
{
 public:
  RWFileSelectionDialog
    (
    RWController *, 
    const RWCString &,
    Boolean managed = False
    );
  RWFileSelectionDialog
    (
    RWController *, 
    const RWCString &,
    const RWResourceObj &,
    Boolean managed = False
    );
  RWFileSelectionDialog
    (
    RWController *, 
    RWResourceObj * ro = 0
    );
  ~RWFileSelectionDialog();

  void  createWidget (char *, RWBoolean managed = False);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const { return __RWFILESELECTIONDIALOG; }
};

#endif  /* End of file FileSB.h - Do not add beyond this point! */




