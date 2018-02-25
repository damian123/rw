#ifndef __RW_DRAGICON_H
#define __RW_DRAGICON_H
/*********************************************************************
*
*  $Id: DragIcon.h,v 1.5 1993/10/07 01:42:57 keffer Exp $
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
*  File        : DragIcon.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : December 1992
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                drag icon class.
*
*
*  Revision History:
*
*     None.
*
*********************************************************************/

/*********************************************************************
*
* #include files
*
*********************************************************************/

#include <rw/Xrw/Control.h>

/********************************************************************
* RWDragIcon : Class object which represents the DragIcon
* widget in the OSF/Motif widget set.  This is an internal class
* which is not meant to be instantiated directly.
*********************************************************************/

class RWDragIcon : public RWController {
public:
  RWDragIcon 
	(
	Widget,
	Boolean managed = False
	);
  RWDragIcon 
	(
	RWController *, 
	const RWCString &,
	Boolean managed = False
	);
  RWDragIcon 
	(
	RWController *, 
	const RWCString &,
	const RWResourceObj &, 
	Boolean managed = False
	);
  ~RWDragIcon();

// RESOURCE MEMBER FUNCTIONS
  void 		setAttachment (unsigned char);
  unsigned char getAttachment (unsigned char * val = 0);
  void          setDepth (int);
  int           getDepth (int * val = 0);
  void          setHeight (Dimension);
  Dimension     getHeight (Dimension * val = 0);
  void		setHotX (Position);
  Position	getHotX (Position * val = 0);
  void		setHotY (Position);
  Position	getHotY (Position * val = 0);
  void		setMask (Pixmap);
  Pixmap	getMask (Pixmap * val = 0);
  void		setOffsetX (Position);
  Position	getOffsetX (Position * val = 0);
  void		setOffsetY (Position);
  Position	getOffsetY (Position * val = 0);
  void		setPixmap (Pixmap);
  Pixmap	getPixmap (Pixmap * val = 0);
  void          setWidth (Dimension);
  Dimension     getWidth (Dimension * val = 0);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const 
	{
	return __RWDRAGICON;
	}
  
private:
};

#endif  /* End of file DragIcon.h - Do not add beyond this point! */
