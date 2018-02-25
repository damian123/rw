#ifndef __RW_XMSTRING_H
#define __RW_XMSTRING_H
/*********************************************************************
*
*  $Id: String.h,v 1.13 1993/11/06 02:47:20 keffer Exp $
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
*  File        : String.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                RWXmString class.
*
*
*  Revision History:
*
*  09-15-91      Added the following member functions
*                (Motif 1.1):
*                   hasSubstring
*
*********************************************************************/

/*********************************************************************
*
* #include files
*
*********************************************************************/

#include <rw/Xrw/FntLst.h>
#include <rw/Xrw/TypeId.h>

/*********************************************************************
* typedefs
*********************************************************************/

class RWXmString;
class RWXmStringDirection;
typedef RWXmString *RWXmStringList;
typedef XmStringCharSet RWXmStringCharSet;
typedef XmStringComponentType RWXmStringComponentType;


/*********************************************************************
* RWXmString : View.h++ class object that encapsulates the XmString data
* type in OSF/Motif.
*********************************************************************/

class RWXmString : public RWApplication
{
 public:
  RWXmString ();
  RWXmString ( const char *,
	       Boolean LtoR = True,
	       RWXmStringCharSet charset = (RWXmStringCharSet)XmSTRING_DEFAULT_CHARSET);
#ifndef XMSTRING_IS_CHARSTAR
  RWXmString ( XmString );
  RWXmString& operator=(XmString);
#endif

  RWXmString(const RWXmString&);
  ~RWXmString ();
  RWXmString&              operator=         (const RWXmString&);
  RWXmString&              operator+=        (const RWXmString&);

  XmString                 getString 	     ();
  void                     setString	     (XmString);
  Dimension                baseline 	     (RWFontList *);
  Boolean                  byteCompare 	     (const RWXmString&);
  void                     draw 	     (Display *, 
					      Window, 
					      RWFontList *, 
					      GC,
					      Position, 
					      Position, 
					      Dimension,
					      unsigned char, 
					      unsigned char,
					      XRectangle *
					      );
  void                     drawImage 	     (Display *,
					      Window,
					      RWFontList *,
					      GC,
					      Position,
					      Position,
					      Dimension,
					      unsigned char,
					      unsigned char,
					      XRectangle *
					      );
  void                     drawUnderline      (
					       Display *,
					       Window,
					       RWFontList *,
					       GC,
					       Position,
					       Position,
					       Dimension,
					       unsigned char,
					       unsigned char,
					       XRectangle *,
					       RWXmString *
					       );
  Boolean                  empty 	     ();
  void                     extent 	     (RWFontList *,
					      Dimension *,
					      Dimension *);
  Boolean                  getLtoR 	     (RWCString&,
					      RWXmStringCharSet cset = XmSTRING_DEFAULT_CHARSET
					      );
  XmStringComponentType    getNextComponent  (char **,
					      XmStringCharSet *,
					      XmStringDirection *,
					      XmStringComponentType *,
					      unsigned short *,
					      unsigned char **
					      );
  RWXmStringComponentType  getNextComponent  (char **,
					      RWXmStringCharSet *,
					      RWXmStringDirection *,
					      RWXmStringComponentType *,
					      unsigned short *,
					      unsigned char **
					      );

//  RWXmStringComponentType getNextComponent (char **);

  Boolean                  getNextSegment    (char **,
					      XmStringCharSet *,
					      XmStringDirection *,
					      Boolean *
					      );
  Boolean                  getNextSegment    (char **,
					      RWXmStringCharSet *,
					      RWXmStringDirection *,
					      Boolean *
					      );
  Boolean                  getNextSegment    (char **, 
					      Boolean *
					      );
  Boolean                  getNextSegment    (char **);
  Boolean                  hasSubstring	     (RWXmString *);
  Dimension                height	     (RWFontList *);
  Boolean                  initContext	     ();
  int                      length	     ();
  int                      lineCount	     ();
  RWXmString *             nConcat	     (RWXmString *, int);
  RWXmString *             nCopy	     (int);
  RWXmStringComponentType  peekNextComponent ();
  Dimension                width	     (RWFontList *);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWXMSTRING;}

// Concatenation:
friend RWXmString operator+(const RWXmString&, const RWXmString&);

// Semantic equality:
friend RWBoolean operator==(const RWXmString&, const RWXmString&);


  // Deprecated interface: The following functions will eventually
  //                       not be supported

  void                     setString 	     (RWXmString *);
  Boolean                  byteCompare 	     (RWXmString *);
  Boolean                  compare  	     (RWXmString *);
  RWXmString *             concat 	     (RWXmString *);
  RWXmString *             Copy 	     ();
  void                     Free 	     ();
  void                     freeContext 	     ();
  Boolean                  getLtoR 	     (RWCString **,
					      RWXmStringCharSet cset = XmSTRING_DEFAULT_CHARSET
					      );
  // End deprecated interface.



protected:

  RWXmString(XmString x, XmStringContext c) : xmstring(x), context(c) {;}

private:

  XmString xmstring;
  XmStringContext context;
};


/*********************************************************************
* RWXmStringDirection : View.h++ class object that encapsulates the
* XmStringDirection data type in OSF/Motif.
*********************************************************************/

class RWXmStringDirection : public RWXmString
{
 public:
  RWXmStringDirection (int);
  RWXmStringDirection ();

  int  getDirection ();
  void setDirection (int);

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWXMSTRINGDIRECTION;}

private:
  int direction;
};


/*********************************************************************
* RWXmStringSegment : View.h++ class object that encapsulates the
* XmStringSegment data type in OSF/Motif.
*********************************************************************/

class RWXmStringSegment : public RWXmString
{
 public:
  RWXmStringSegment 
    (
    const char *,
    int,
    int,
    Boolean sep = False
    );
  RWXmStringSegment
    (
    const char *,
    int,
    RWXmStringDirection,
    Boolean sep = False
    );
  RWXmStringSegment
    (
    const char *
    );
  RWXmStringSegment();

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWXMSTRINGSEGMENT;}
};


/*********************************************************************
* RWXmStringSeparator : View.h++ class object that encapsulates the
* XmStringSeparator data type in OSF/Motif.
*********************************************************************/

class RWXmStringSeparator : public RWXmString
{
 public:
  RWXmStringSeparator();

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID isA () const {return __RWXMSTRINGSEPARATOR;}
};

#endif  /* End of file String.h - Do not add beyond this point! */
