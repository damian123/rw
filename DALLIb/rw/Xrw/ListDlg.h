#ifndef __RW_LISTDLGH
#define __RW_LISTDLGH

/*********************************************************************
*
*  $Id: ListDlg.h,v 1.14 1994/01/10 23:52:01 cynthia Exp $
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
*  File        : ListDlg.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for a dialog 
*                allows the user to navigate through the file system to
*                select a file
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

#include <rw/Xrw/Dialog.h>
#include <rw/Xrw/List.h>

/*********************************************************************
*  Resources for components of an RWListDialog may be set in an
*  app-defaults file using strings composed of the "name" given in
*  the dialog constructor with identifying strings for each
*  component appended.  The names for each component are created
*  as follows:
*
*  Example: if name =    "myDialog":
*
*  COMPONENT              NAME   ID STRING       COMPONENT NAME
*  ---------              ----   ---------       --------------
*  listControl_         : name + "LIST"          myDialogLIST
*
*  name defaults to:     "RWListDialog"
*
*  See Dialoh.h for identifying strings of inherited components.
*********************************************************************/

class RWScrolledWindow;
class RWOrdered;

class RWListDialog : public RWDialogView {

public:
  RWListDialog (RWView        *parentView,
		const char    *name    = "RWListDialog",
		RWButtonConfig buttons = RWOkCancelButton);
  ~RWListDialog();

#ifdef RWVIEW1_COMPATIBLE
  RWOrdered *getResponse (const RWOrdered &, 
			  RWBoolean multi = FALSE,
			  RWDialogStyle modal = RWModalDialog);
  RWOrdered *getSelection ();
#else
  virtual RWDialogReply getResponse (RWCStringOrderedVec& answer,
				     const RWCStringOrderedVec& choices,
				     RWSelectionPolicy selPolicy = RWDefaultSelect,
				     int visibleItemCnt = RWDefaultVisibleItemCount);
  virtual RWDialogReply getResponse (const RWCStringOrderedVec& choices,
				     RWSelectionPolicy selPolicy = RWDefaultSelect,
				     RWDialogStyle modal = RWModalDialog,
				     int visibleItemCnt = RWDefaultVisibleItemCount);
  virtual RWDialogReply getResponse (RWDialogStyle modal = RWModalDialog)
	                             {return getResponseToDialog(modal);}

  virtual RWCStringOrderedVec getSelection () const;
#endif

  virtual RWClassID isA() const { return(__RWLISTDIALOG); }
  
// RESOURCE MEMBER FUNCTIONS
  inline void             setListFont              (const char * fontName);
  inline int              getItemCount             () const;
  inline void             setItems                 (const RWCStringOrderedVec & items);
  inline RWCStringOrderedVec     getItems                 () const;
  inline int              getSelectedItemCount     () const;
  inline void             setSelectedItems         (const RWCStringOrderedVec & selitems);
  inline RWCStringOrderedVec     getSelectedItems         () const;
  inline void             setSelectionPolicy       (RWSelectionPolicy selpol);
  inline RWSelectionPolicy getSelectionPolicy      () const;
  inline void             setTopItemPosition       (int topPos);
  inline int              getTopItemPosition       () const;

// <<< X/Motif only
  inline void             setAutomaticSelection    (RWBoolean autoSelect);
  inline RWBoolean        getAutomaticSelection    () const;
  inline void             setDoubleClickInterval   (int interval);
  inline int              getDoubleClickInterval   () const;
  inline void             setListMarginHeight      (int hei);
  inline int              getListMarginHeight      () const;
  inline void             setListMarginWidth       (int wid);
  inline int              getListMarginWidth       () const;
  inline void             setListSpacing           (int spacing);
  inline int              getListSpacing           () const;
  inline void             setVisibleItemCount      (int cnt);
  inline int              getVisibleItemCount      () const;
// >>>


// LIST MEMBER FUNCTIONS  
  inline void             addItem                  (const char * item,
						    int pos = 1);
  inline void             addItems                 (const RWCStringOrderedVec & items,
						    int pos = 1);
  inline void             addItemUnselected        (const char * item,
						    int pos = 1);
  inline void             deleteAllItems           ();
  inline void             deleteItem               (const char * item);
  inline void             deleteItems              (const RWCStringOrderedVec & items);
  inline void             deleteItemsPos           (int delCnt,
						    int pos = 1);
  inline void             deletePos                (int pos);
  inline void             deselectAllItems         ();
  inline void             deselectItem             (const char * item);
  inline void             deselectPos              (int pos);
  inline RWBoolean        getMatchPos              (const char * item,
						    int ** positionList,
						    int *  positionCnt) const;
  inline RWBoolean        getSelectedPos           (int ** positionList,
						    int *  positionCnt) const;
  inline RWBoolean        itemExists               (const char * item) const;
  inline int              itemPos                  (const char * item) const;
  inline void             selectItem               (const char * item,
						    RWBoolean = FALSE);
  inline void             selectPos                (int pos,
						    RWBoolean = FALSE);
  inline void             setBottomItem            (const char * item);
  inline void             setBottomPos             (int pos);
  inline void             setItem                  (const char * item);
  inline void             setPos                   (int pos);

#if (RWX11Version>=5)
  inline void             addItemsUnselected       (const RWCStringOrderedVec & items,
						    int pos = 0);
  inline void             deletePositions          (int * positionList,
						    int positionCnt);
  inline int              getKbdItemPos            ();
  inline RWBoolean        posToBounds              (int,
						    Position *,
						    Position *,
						    Dimension *,
						    Dimension *);
  inline RWBoolean        isPosSelected            (int pos);
  inline void             replacePositions         (int *,
						    const RWCStringOrderedVec &,
						    int);
  inline RWBoolean        setKbdItemPos            (int);
  inline void             updateSelectedList       ();
  inline int              yToPos                   (Position);
#endif

protected:

// Callback Methods
  virtual void          browseSelectionMethod   (RWApplication *control,
						 RWXtPointer    clientData,
						 RWXtPointer    callData)
	 {anySelectionMethod(control, clientData, callData);}
  
  virtual void          defaultActionMethod     (RWApplication *, //control
						 RWXtPointer,     //clientData
						 RWXtPointer)     //callData
	 {}
  
  virtual void          extendedSelectionMethod (RWApplication *control,
						 RWXtPointer    clientData,
						 RWXtPointer    callData)
	 {anySelectionMethod(control, clientData, callData);}
  
  virtual void          multipleSelectionMethod (RWApplication *control,
						 RWXtPointer    clientData,
						 RWXtPointer    callData)
	 {anySelectionMethod(control, clientData, callData);}
  
  virtual void          singleSelectionMethod   (RWApplication *control,
						 RWXtPointer    clientData,
						 RWXtPointer    callData)
	 {anySelectionMethod(control, clientData, callData);}
  
  virtual void          anySelectionMethod      (RWApplication *, //control
						 RWXtPointer,     //clientData
						 RWXtPointer)     //callData
	 {}

  RWScrolledWindow * scrollControl_;
  RWScrolledList   * listControl_;
  
};



/****
*   Inlines
****/

inline void RWListDialog::setListFont(const char * fontName)
{
  RWFontList localFontList(view->getDisplay(), fontName);

  listControl_->setFontList(&localFontList);
}

inline int RWListDialog::getItemCount() const
{
  return (listControl_->getItemCount());
}

inline void RWListDialog::setItems(const RWCStringOrderedVec & items)
{
  listControl_->setItems((RWCStringOrderedVec &)items);
}

inline RWCStringOrderedVec RWListDialog::getItems() const
{
  return (listControl_->getItemsC());
}

inline int RWListDialog::getSelectedItemCount() const
{
  return (listControl_->getSelectedItemCount());
}

inline void RWListDialog::setSelectedItems(const RWCStringOrderedVec & selitems)
{
  listControl_->setSelectedItems((RWCStringOrderedVec &)selitems);
}

inline RWCStringOrderedVec RWListDialog::getSelectedItems() const
{
  return (listControl_->getSelectedItemsC());
}

inline void RWListDialog::setSelectionPolicy(RWSelectionPolicy selpol)
{
  listControl_->setSelectionPolicy((unsigned char)selpol);
}

inline RWSelectionPolicy RWListDialog::getSelectionPolicy() const
{
  return ((RWSelectionPolicy)listControl_->getSelectionPolicy());
}

inline void RWListDialog::setTopItemPosition(int topPos)
{
  listControl_->setTopItemPosition(topPos);
}

inline int RWListDialog::getTopItemPosition() const
{
  return (listControl_->getTopItemPosition());
}

inline void RWListDialog::addItem(const char * item,
											 int pos)
{
  listControl_->addItem(item, pos);
}

inline void RWListDialog::addItems(const RWCStringOrderedVec & items,
				   int pos)
{
  listControl_->addItems (items, pos);
}

inline void RWListDialog::addItemUnselected(const char * item,
														  int pos)
{
  listControl_->addItemUnselected(item, pos);
}

inline void RWListDialog::deleteAllItems()
{
  listControl_->deleteAllItems();
}

inline void RWListDialog::deleteItem(const char * item)
{
  listControl_->deleteItem(item);
}

inline void RWListDialog::deleteItems(const RWCStringOrderedVec & items)
{
  listControl_->deleteItems(items);
}

inline void RWListDialog::deleteItemsPos(int delCnt,
					 int pos)
{
  listControl_->deleteItemsPos(delCnt, pos);
}

inline void RWListDialog::deletePos(int pos)
{
  listControl_->deletePos(pos);
}

inline void RWListDialog::deselectAllItems()
{
  listControl_->deselectAllItems();
}

inline void RWListDialog::deselectItem(const char * item)
{
  listControl_->deselectItem(item);
}

inline void RWListDialog::deselectPos(int pos)
{
  listControl_->deselectPos(pos);
}

inline RWBoolean RWListDialog::getMatchPos(const char * item,
														 int ** positionList,
														 int *  positionCnt) const
{
  return (listControl_->getMatchPos(item, positionList, positionCnt));
}

inline RWBoolean RWListDialog::getSelectedPos(int ** positionList,
															 int *  positionCnt) const
{
  return (listControl_->getSelectedPos(positionList, positionCnt));
}

inline RWBoolean RWListDialog::itemExists(const char * item) const
{
  return (listControl_->itemExists(item));
}

inline int RWListDialog::itemPos(const char * item) const
{
  return (listControl_->itemPos(item));
}

inline void RWListDialog::selectItem(const char * item,
												 RWBoolean notify)
{
  listControl_->selectItem(item, notify);
}

inline void RWListDialog::selectPos(int pos,
												RWBoolean notify)
{
  listControl_->selectPos(pos, notify);
}

inline void RWListDialog::setBottomItem(const char * item)
{
  listControl_->setBottomItem(item);
}


inline void RWListDialog::setBottomPos(int pos)
{
  listControl_->setBottomPos(pos);
}


inline void RWListDialog::setItem(const char * item)
{
  listControl_->setItem(item);
}


inline void RWListDialog::setPos(int pos)
{
  listControl_->setPos(pos);
}

// <<< X/Motif only

inline void RWListDialog::setAutomaticSelection(RWBoolean autoSelect)
{
  listControl_->setAutomaticSelection(autoSelect);
}

inline RWBoolean RWListDialog::getAutomaticSelection() const
{
  return (listControl_->getAutomaticSelection());
}

inline void RWListDialog::setDoubleClickInterval(int interval)
{
  listControl_->setDoubleClickInterval(interval);
}

inline int RWListDialog::getDoubleClickInterval() const
{
  return (listControl_->getDoubleClickInterval());
}

inline void RWListDialog::setListMarginHeight(int hei)
{
  listControl_->setListMarginHeight((Dimension)hei);
}

inline int RWListDialog::getListMarginHeight() const
{
  return ((int)listControl_->getListMarginHeight());
}

inline void RWListDialog::setListMarginWidth(int wid)
{
  listControl_->setListMarginWidth((Dimension)wid);
}

inline int RWListDialog::getListMarginWidth() const
{
  return ((int)listControl_->getListMarginWidth());
}

inline void RWListDialog::setListSpacing(int spacing)
{
  listControl_->setListSpacing((Dimension)spacing);
}

inline int RWListDialog::getListSpacing() const
{
  return ((int)listControl_->getListSpacing());
}

inline void RWListDialog::setVisibleItemCount(int cnt)
{
  listControl_->setVisibleItemCount(cnt);
}

inline int RWListDialog::getVisibleItemCount() const
{
  return (listControl_->getVisibleItemCount());
}

// >>>










#ifdef X11R5
inline void RWListDialog::addItemsUnselected(const RWCStringOrderedVec & items,
															int pos)
{
  listControl_->addItemsUnselected(items, pos);
}

inline void RWListDialog::deletePositions(int * positionList,
														int positionCnt)
{
  listControl_->deletePositions(positionList, positionCnt);
}

inline int RWListDialog::getKbdItemPos()
{
  return listControl_->getKbdItemPos();
}

/*??
inline RWBoolean RWListDialog::posToBounds(int,
														 Position *,
														 Position *,
														 Dimension *,
														 Dimension *)
{
  listControl_->posToBounds(int,
														 Position *,
														 Position *,
														 Dimension *,
														 Dimension *);
}
*/
inline RWBoolean RWListDialog::isPosSelected(int pos)
{
  return listControl_->posSelected(pos);
}

//??inline void             replacePositions             (int *,
//??																		  const RWCStrngVec &,
//??																		  int);

//??inline RWBoolean          setKbdItemPos                (int);

//??inline void             updateSelectedList           ();

//??inline int              yToPos                       (Position);
#endif


#endif
