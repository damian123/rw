#ifndef __RW_UTIL_H
#define __RW_UTIL_H
/*********************************************************************
*
*  $Id: Util.h,v 1.48 1994/04/28 14:56:35 steves Exp $
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
*  File        : Util.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains utility variables, macros,
*                function prototypes, etc., for use in View.h++.
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
#ifndef __RW_XMSTRING_H
#include <rw/Xrw/String.h>
#endif
#include <rw/Xrw/cstrordv.h>
#include <rw/generic.h>


class RWCallbackCB;
class RWCallbackInfo;
class RWController;
class RWEventCB;
class RWEventMethodCB;
class RWLabel;
class RWMethodCB;
class RWOrdered;

/*********************************************************************
* typedefs
*********************************************************************/

typedef XmString *XmStringList;
extern void StringToStringArray(char*, String**);


/*********************************************************************
* constants
*********************************************************************/

const char * const RWDefaultFont    = "fixed";
const char * const RWDefaultColor   = "white";
const char * const RWDefaultFGColor = "black";

/*********************************************************************
* externs
*********************************************************************/

extern RWController *  getSurrogateObject (Widget);
extern RWXmStringList  xmStrListToRWXmStrList (XmStringList, int);
extern RWXmStringList  strListToRWXmStrList (char **, int);
extern XmStringList    strListToXmStrList (char **, int);
extern XmStringList    rworderedToXmStrList (const RWOrdered &);
extern XmStringList    rwcstrvecToXmStrList (const RWCStringOrderedVec &);
extern char **         xmStrListToStrList (XmStringList, int );
extern RWOrdered *     xmStrListToRWOrdered (XmStringList, int);
extern void            xmStrListToRWCStringOrdVec (RWCStringOrderedVec &, XmStringList, int);
extern XmStringList    rwXmStrListToXmStrList (RWXmStringList, int);
extern XmString        strToXmStr (String);
extern XmString        rwcstrToXmStr (const RWCString &);
extern String          xmStrToStr (XmString);
extern RWCString       xmStrToCStr (XmString);
extern void            xmStrListFree (XmStringList, int);
extern void            rwFreeLabelPixmap(Widget, XtPointer, XtPointer);
extern void            rwAddFreeLabelPixmapCallback(RWLabel *);
extern Pixel           strToPix (const char *, RWController *);

extern void  RWMethodDispatcher          (Widget, RWMethodCB *,      RWXtPointer             );
extern void  RWEventMethodDispatcher     (Widget, RWEventMethodCB *, RWXtPointer,  Boolean * );
extern void  RWCallbackDispatcher        (Widget, RWCallbackCB *,    RWXtPointer             );
extern void  RWEventDispatcher           (Widget, RWEventCB *,       XEvent*,      Boolean * );
extern void  rwDeleteSurrogateController (Widget, RWController*,     RWXtPointer             );
extern void  rwDeleteCallbackInfo        (Widget, RWCallbackInfo *,  RWXtPointer             );

#if RWX11Version>=5
extern Boolean RWConvertProcDispatcher   (Widget, Atom *, Atom *, Atom *, XtPointer *, 
					  unsigned long *, int *, unsigned long *, 
					  XtPointer, XtRequestId * );
extern void RWDropDispatcher             (Widget, RWXtPointer, RWXtPointer);
extern void RWDragDispatcher             (Widget, RWXtPointer, RWXtPointer);
#endif

extern XFontStruct *  rwXLoadQueryFont (Display *, const char *);
extern RWController * widgetToControllerObj (Widget);

/*********************************************************************
* End user system macros
*********************************************************************/

#define RWMethod(METHOD) (RWMethodPtr)METHOD
#define RWEventMethod(METHOD) (RWEventMethodPtr)METHOD
/*
#define RWTimerMethod(METHOD) (RWTimerMethodPtr)METHOD
#define RWWorkProcMethod(METHOD) (RWWorkProcMethodPtr)METHOD
*/


/*********************************************************************
* #define(s)
*********************************************************************/

#define XmATTACH_OBJECT           XmATTACH_WIDGET
#define XmATTACH_OPPOSITE_OBJECT  XmATTACH_OPPOSITE_WIDGET


// Cannot be buffered.
#define C_CALLBACK(CLASS_NAME,MEMBER_FUNCTION,METHOD_NAME,RESOURCE)	\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)				\
	(RWXtCallbackProc callback,					\
	 RWXtPointer client_data)					\
  {									\
    addCallback (RESOURCE, callback, client_data);			\
  }									\
  void CLASS_NAME:: name2(set,METHOD_NAME)				\
	(RWApplication *obj,						\
	 RWMethodPtr mymethod,						\
	 void *client_data)						\
  {									\
    addMethod(RESOURCE, obj, mymethod, client_data);			\
  }									\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)				\
	(XtCallbackList val)						\
  {									\
    XtAddCallbacks(widget, RESOURCE, val);				\
  }


// Buffering implemented.
#define CSG_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE,RES_TYPE)	\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(RES_TYPE val)		\
  {									\
    if (checkResourceBufferSize(sargcnt)) {				\
      XtSetArg(sdargs[sargcnt], RESOURCE, val); sargcnt++;		\
      if (!bufferResources) {						\
	XtSetValues(widget, &sdargs, sargcnt); sargcnt=0;		\
      }									\
    }									\
  }									\
  RES_TYPE CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RES_TYPE* val)	\
  {									\
    if (checkResourceBufferSize(gargcnt)) {				\
      RES_TYPE gval;							\
      RES_TYPE *tmpval = &gval;						\
      if (val)								\
	tmpval = val;							\
      XtSetArg(gdargs[gargcnt], RESOURCE, tmpval); gargcnt++;		\
      if (!bufferResources) {						\
	XtGetValues(widget, &gdargs, gargcnt);				\
	gargcnt = 0;							\
	return (*tmpval);						\
      }									\
      return (RES_TYPE) 0;						\
    }									\
    return (RES_TYPE) 0;						\
  }

// Cannot be buffered.
// User must delete memory returned or passed back from get method.
#define CSG_RWCS_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE)		\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(const RWCString & val)	\
  {									\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, (char *)val.data());			\
    XtSetValues(widget, tmparg, 1);					\
  }									\
  RWCString * CLASS_NAME:: name3(get,MEMBER_FUNCTION,C)(RWCString ** val) \
  {									\
    String gval;							\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, &gval);				\
    XtGetValues(widget, tmparg, 1);					\
    RWCString *tmpcstr = new RWCString(gval);				\
    XtFree(gval);							\
    if (val)								\
      *val = tmpcstr;							\
    return tmpcstr;							\
  }

//  name2(set,MEMBER_FUNCTION) buffering implemented (but with leak)
//  name2(get,MEMBER_FUNCTION) cannot be buffered.
#define CSG_RWCSTRING_RES(CLASS_NAME,MEMBER_FUNCTION,RESOURCE)		\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(const RWCString & val)	\
  {									\
    if (checkResourceBufferSize(sargcnt)) {				\
        XtSetArg(sdargs[sargcnt], RESOURCE, (char *)val.data()); sargcnt++;	\
        if (!bufferResources) {						\
            XtSetValues(widget, &sdargs, sargcnt);			\
            sargcnt=0;							\
        }								\
    }									\
  }									\
  RWCString CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RWCString * val)	\
  {									\
    String gval;							\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, &gval);				\
    XtGetValues(widget, tmparg, 1);					\
    RWCString tmpcstr(gval);						\
    XtFree(gval);							\
    if (val)								\
      *val = tmpcstr;							\
    return tmpcstr;							\
  }

// Buffering implemented.
#define CG_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE,RES_TYPE)	\
  RES_TYPE CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RES_TYPE* val)	\
  {									\
    if (checkResourceBufferSize(gargcnt)) {				\
      RES_TYPE gval;							\
      RES_TYPE *tmpval = &gval;						\
      if (val)								\
	tmpval = val;							\
      XtSetArg(gdargs[gargcnt], RESOURCE, tmpval); gargcnt++;		\
      if (!bufferResources) {						\
	XtGetValues(widget, &gdargs, gargcnt); gargcnt=0;		\
	return(*tmpval);						\
      }									\
      return (RES_TYPE) 0;						\
    }									\
    return (RES_TYPE) 0;						\
  }


// Buffering implemented.
#define G_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE,RES_TYPE)	\
  RES_TYPE CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RES_TYPE* val)	\
  {									\
    if (checkResourceBufferSize(gargcnt)) {				\
      RES_TYPE gval;							\
      RES_TYPE *tmpval = &gval;						\
      if (val)								\
	tmpval = val;							\
      XtSetArg(gdargs[gargcnt], RESOURCE, tmpval); gargcnt++;		\
      if (!bufferResources) {						\
	XtGetValues(widget, &gdargs, gargcnt); gargcnt=0;		\
	return(*tmpval);						\
      }									\
      return (RES_TYPE) 0;						\
    }									\
    return (RES_TYPE) 0;						\
  }


// Buffering implemented.
#define C_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE,RES_TYPE)	\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(RES_TYPE val)		\
  {									\
    if (checkResourceBufferSize(sargcnt)) {				\
      XtSetArg(sdargs[sargcnt], RESOURCE, val); sargcnt++;		\
      if (!bufferResources) {						\
	XtSetValues(widget, &sdargs, sargcnt); sargcnt=0;		\
      }									\
    }									\
  }


// Cannot be buffered.
#define CSG_UD_RESOURCE(CLASS_NAME)					\
  void CLASS_NAME::setUData(RWXtPointer val)				\
  {									\
    setEndUserData(val);						\
  }									\
  RWXtPointer CLASS_NAME::getUserData(void *val)			\
  {									\
    if (!val)								\
      return(getUData());						\
    else								\
      return(getUData((RWXtPointer*)val));				\
  }									\
  RWXtPointer CLASS_NAME::getUData(RWXtPointer *val)			\
  {									\
    if (!val)								\
      return(getEndUserData());						\
    else {								\
      *val = getEndUserData();						\
      return((RWXtPointer)0);						\
    }									\
  }


#define setUserData(ARG) setUData((RWXtPointer)ARG)


// Buffering implemented.
#define CSG_STRING_PTR_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE,RES_TYPE) \
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(RES_TYPE val)		\
  {									\
    if (checkResourceBufferSize(sargcnt)) {				\
      XtSetArg(sdargs[sargcnt], RESOURCE, val); sargcnt++;		\
      if (!bufferResources) {						\
	XtSetValues(widget, &sdargs, sargcnt); sargcnt=0;		\
      }									\
    }									\
  }									\
  RES_TYPE CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RES_TYPE *val)	\
  {									\
    if (checkResourceBufferSize(gargcnt)) {				\
      RES_TYPE gval;							\
      RES_TYPE *tmpval = &gval;						\
      if (val)								\
	tmpval = val;							\
      XtSetArg(gdargs[gargcnt], RESOURCE, tmpval); gargcnt++;		\
      if (!bufferResources) {						\
	XtGetValues(widget, &gdargs, gargcnt); gargcnt=0;		\
	return(*tmpval);						\
      }									\
      return (RES_TYPE) 0;						\
    }									\
    return (RES_TYPE) 0;						\
  }


// Buffering implemented.
#define CSG_PTR_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE,RES_TYPE)	\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(RES_TYPE *val)		\
  {									\
    if (checkResourceBufferSize(sargcnt)) {				\
      XtSetArg(sdargs[sargcnt], RESOURCE, val); sargcnt++;		\
      if (!bufferResources) {						\
	XtSetValues(widget, &sdargs, sargcnt); sargcnt=0;		\
      }									\
    }									\
  }									\
  RES_TYPE *CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RES_TYPE **val)	\
  {									\
    if (checkResourceBufferSize(gargcnt)) {				\
      RES_TYPE *gval;							\
      RES_TYPE **tmpval = &gval;					\
      if (val)								\
	tmpval=val;							\
      XtSetArg(gdargs[gargcnt], RESOURCE, tmpval); gargcnt++;		\
      if (!bufferResources) {						\
	XtGetValues(widget, &gdargs, gargcnt); gargcnt=0;		\
	return(*tmpval);						\
      }									\
      return 0;								\
    }									\
    return 0;								\
  }


//  name2(set,MEMBER_FUNCTION) buffering implemented.
//  name2(get,MEMBER_FUNCTION) cannot be buffered.
#define CSG_OBJ_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE)		\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(RWController *obj)	\
  {									\
    if (checkResourceBufferSize(sargcnt)) {				\
      if (obj)								\
	XtSetArg(sdargs[sargcnt], RESOURCE, obj->getWidget());		\
      else								\
	XtSetArg(sdargs[sargcnt], RESOURCE, 0);				\
      sargcnt++;							\
      if (!bufferResources) {						\
	XtSetValues(widget, &sdargs, sargcnt); sargcnt=0;		\
      }									\
    }									\
  }									\
  RWController *CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RWController **obj) \
  {									\
    Widget w;								\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, &w);					\
    XtGetValues(widget, tmparg, 1);					\
    if (!obj)								\
      return(getSurrogateObject(w));					\
    else {								\
      *obj = getSurrogateObject(w);					\
      return(*obj);							\
    }									\
  }

//  name2(set,MEMBER_FUNCTION) buffering implemented.
//  name2(get,MEMBER_FUNCTION) cannot be buffered.
#define CSG_DRAGICON_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE)	\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(RWDragIcon *obj)		\
  {									\
    if (checkResourceBufferSize(sargcnt)) {				\
      if (obj)								\
	XtSetArg(sdargs[sargcnt], RESOURCE, obj->getWidget());		\
      else								\
	XtSetArg(sdargs[sargcnt], RESOURCE, 0);				\
      sargcnt++;							\
      if (!bufferResources) {						\
	XtSetValues(widget, &sdargs, sargcnt); sargcnt=0;		\
      }									\
    }									\
  }									\
  RWDragIcon *CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RWDragIcon **obj)	\
  {									\
    Widget w;								\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, &w);					\
    XtGetValues(widget, tmparg, 1);					\
    if (!obj)								\
      return ((RWDragIcon *)getSurrogateObject(w));			\
    else {								\
      *obj = (RWDragIcon *)getSurrogateObject(w);			\
      return(*obj);							\
    }									\
  }

//  name2(set,MEMBER_FUNCTION) buffering implemented.
//  name2(get,MEMBER_FUNCTION) cannot be buffered.
#define CSG_SPEC_OBJ_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE,CONTROLLER) \
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(CONTROLLER *obj)		\
  {									\
    if (checkResourceBufferSize(sargcnt)) {				\
      XtSetArg(sdargs[sargcnt], RESOURCE, obj->getWidget()); sargcnt++;	\
      if (!bufferResources) {						\
	XtSetValues(widget, &sdargs, sargcnt); sargcnt=0;		\
      }									\
    }									\
  }									\
  CONTROLLER *CLASS_NAME:: name2(get,MEMBER_FUNCTION)(CONTROLLER **obj)	\
  {									\
    Widget w;								\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, &w);					\
    XtGetValues(widget, tmparg, 1);					\
    if (!obj)								\
      return((CONTROLLER *)getSurrogateObject(w));			\
    else {								\
      *obj = (CONTROLLER *)getSurrogateObject(w);			\
      return(*obj);							\
    }									\
  }


//  name2(set,MEMBER_FUNCTION) buffering implemented.
//  name2(get,MEMBER_FUNCTION) cannot be buffered.
#define SG_OBJ_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE)		\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(RWController *obj)	\
  {									\
    if (checkResourceBufferSize(sargcnt)) {				\
      XtSetArg(sdargs[sargcnt], RESOURCE, obj->getWidget()); sargcnt++;	\
      if (!bufferResources) {						\
	XtSetValues(widget, &sdargs, sargcnt); sargcnt=0;		\
      }									\
    }									\
  }									\
  RWController *CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RWController **obj)   \
  {									\
    Widget w;								\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, &w);					\
    XtGetValues(widget, tmparg, 1);					\
    if (!obj)								\
      return(getSurrogateObject(w));					\
    else {								\
      *obj = getSurrogateObject(w);					\
      return(*obj);							\
    }									\
  }


// Cannot be buffered.
#define CG_OBJ_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE)		\
  RWController *CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RWController **obj)   \
  {									\
    Widget w;								\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, &w);					\
    XtGetValues(widget, tmparg, 1);					\
    if (!obj)								\
      return(getSurrogateObject(w));					\
    else {								\
      *obj = getSurrogateObject(w);					\
      return(*obj);							\
    }									\
  }


// Cannot be buffered.
#define G_OBJ_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE)		\
  RWController *CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RWController **obj)   \
  {									\
    Widget w;								\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, &w);					\
    XtGetValues(widget, tmparg, 1);					\
    if (!obj)								\
      return(getSurrogateObject(w));					\
    else {								\
      *obj = getSurrogateObject(w);					\
      return(*obj);							\
    }									\
  }


#define S_STR2PIX_RESOBJ(MEMBER_FUNCTION,RESOURCE)			\
  void RWResourceObj:: name2(set,MEMBER_FUNCTION)			\
			(const RWCString & val, RWController * object)	\
  {									\
    if (checkResourceBufferSize()) {					\
      Pixel pixel = strToPix(val.data(), object);			\
      if (pixel != (Pixel)9999) {					\
	XtSetArg(dargs[argcnt], RESOURCE, pixel); argcnt++;		\
      }									\
    }									\
  }


// Buffering implemented.
#define S_STR2PIX_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE)		\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(const RWCString & val)	\
  {									\
    if (checkResourceBufferSize(sargcnt)) {				\
      Pixel pixel = strToPix(val.data(), this);				\
      if (pixel != (Pixel)9999) {					\
	XtSetArg(sdargs[sargcnt], RESOURCE, pixel); sargcnt++;		\
	if (!bufferResources) {						\
	  XtSetValues(widget, &sdargs, sargcnt); sargcnt=0;		\
	}								\
      }									\
    }									\
  }


// Buffering implemented.
#define S_STR2PIX_GAD_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE)	\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(const RWCString & val)	\
  {									\
    if (checkResourceBufferSize(sargcnt)) {				\
      Pixel pixel = strToPix(val.data(), this->getTheParent());		\
      if (pixel != (Pixel)9999) {					\
	XtSetArg(sdargs[sargcnt], RESOURCE, pixel); sargcnt++;		\
	if (!bufferResources) {						\
	  XtSetValues(widget, &sdargs, sargcnt); sargcnt=0;		\
	}								\
      }									\
    }									\
  }


// Cannot be buffered.
// The user must delete the memory returned or passed back from the get method.
#define CSG_STR2XMSTR_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE)	\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(char *val)		\
  {									\
    XmString tmpxmstr = strToXmStr(val);				\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, tmpxmstr);				\
    XtSetValues(widget, tmparg, 1);					\
    XmStringFree(tmpxmstr);						\
  }									\
  char *CLASS_NAME:: name2(get,MEMBER_FUNCTION)(char **val)		\
  {									\
    XmString tmpxmstr;							\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, &tmpxmstr);				\
    XtGetValues(widget, tmparg, 1);					\
    char * tmpstr = xmStrToStr(tmpxmstr);				\
    XmStringFree(tmpxmstr);						\
    if (val)								\
      *val = tmpstr;							\
    return tmpstr;							\
  }


// Cannot be buffered.
// The user must delete the memory returned or passed back from the get method.
#define CSG_OPTION_STR2XMSTR_RESOURCE(MEMBER_FUNCTION,RESOURCE)	  \
  void RWOptionMenu:: name2(set,MEMBER_FUNCTION)(char *val)	  \
  {								  \
    Arg tmparg[1];						  \
    XmString tmpxmstr = strToXmStr(val);			  \
    XtSetArg(tmparg[0], RESOURCE, tmpxmstr);			  \
    XtSetValues((optionLabelGadget())->getWidget(), tmparg, 1);	  \
    XmStringFree(tmpxmstr);					  \
  }								  \
  char *RWOptionMenu:: name2(get,MEMBER_FUNCTION)(char **val)	  \
  {								  \
    XmString tmpxmstr;						  \
    Arg tmparg[1];						  \
    XtSetArg(tmparg[0], RESOURCE, &tmpxmstr);			  \
    XtGetValues((optionLabelGadget())->getWidget(), tmparg, 1);	  \
    char * tmpstr = xmStrToStr(tmpxmstr);			  \
    XmStringFree(tmpxmstr);					  \
    if (val)							  \
      *val = tmpstr;						  \
    return tmpstr;						  \
  }


//  name2(set,MEMBER_FUNCTION) buffering implemented (but with leak)
//  name2(get,MEMBER_FUNCTION) cannot be buffered.
#define CSG_OPTION_RWCSTR2XMSTR_RESOURCE(MEMBER_FUNCTION,RESOURCE)	\
  void RWOptionMenu:: name2(set,MEMBER_FUNCTION)(const RWCString & val) \
  {									\
    if (checkResourceBufferSize(sargcnt)) {				\
      XmString tmpxmstr = rwcstrToXmStr(val);				\
      XtSetArg(sdargs[sargcnt], RESOURCE, tmpxmstr); sargcnt++;		\
      if (!bufferResources) {						\
        XtSetValues(optionLabelGadget()->getWidget(), &sdargs, sargcnt);\
        sargcnt=0;							\
        XmStringFree(tmpxmstr);						\
      }									\
    }									\
  }									\
  RWCString RWOptionMenu:: name3(get,MEMBER_FUNCTION,C)(RWCString *val) \
  {									\
    XmString tmpxmstr;							\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, &tmpxmstr);				\
    XtGetValues((optionLabelGadget())->getWidget(), tmparg, 1);		\
    RWCString tmpcstr = xmStrToCStr( tmpxmstr );                        \
    XmStringFree(tmpxmstr);						\
    if (val)								\
      *val=tmpcstr;							\
    return tmpcstr;							\
  }


// Cannot be buffered.
#define S_RWCSTR2XMSTR_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE)	\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(const RWCString & val)	\
  {									\
    Arg tmparg[1];							\
    XmString tmpxmstr = rwcstrToXmStr(val);				\
    XtSetArg(tmparg[0], RESOURCE, tmpxmstr);				\
    XtSetValues(widget, tmparg, 1);					\
    XmStringFree(tmpxmstr);						\
  }

//  name2(set,MEMBER_FUNCTION) buffering implemented (but with leak)
//  name2(get,MEMBER_FUNCTION) cannot be buffered.
#define CSG_RWCSTR2XMSTR_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE)	\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(const RWCString & val)	\
  {									\
    if (checkResourceBufferSize(sargcnt)) {				\
      XmString tmpxmstr = rwcstrToXmStr(val);				\
      XtSetArg(sdargs[sargcnt], RESOURCE, tmpxmstr); sargcnt++;		\
      if (!bufferResources) {						\
	XtSetValues(widget, &sdargs, sargcnt); sargcnt=0;		\
        XmStringFree(tmpxmstr);						\
      }									\
    }									\
  }									\
  RWCString CLASS_NAME:: name3(get,MEMBER_FUNCTION,C)(RWCString *val)	\
  {									\
    XmString tmpxmstr;							\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, &tmpxmstr);				\
    XtGetValues(widget, tmparg, 1);                                     \
    RWCString tmpcstr = xmStrToCStr( tmpxmstr );                 	\
    XmStringFree(tmpxmstr);						\
   if (val)								\
      *val = tmpcstr;							\
    return tmpcstr;							\
  }


//  name2(set,MEMBER_FUNCTION) buffering implemented (but with leak)
//  name2(get,MEMBER_FUNCTION) cannot be buffered.
#define CSG_RWCSTR2XMSTR_NF_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE)	\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(const RWCString & val)	\
  {									\
    if (checkResourceBufferSize(sargcnt)) {				\
      XmString tmpxmstr = rwcstrToXmStr(val);				\
      XtSetArg(sdargs[sargcnt], RESOURCE, tmpxmstr); sargcnt++;		\
      if (!bufferResources) {						\
	XtSetValues(widget, &sdargs, sargcnt); sargcnt=0;		\
        XmStringFree(tmpxmstr);						\
      }									\
    }									\
  }									\
  RWCString CLASS_NAME:: name3(get,MEMBER_FUNCTION,C)(RWCString *val)	\
  {									\
    XmString tmpxmstr;							\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, &tmpxmstr);				\
    XtGetValues(widget, tmparg, 1);                                     \
    RWCString tmpcstr = xmStrToCStr( tmpxmstr );                 	\
   if (val)								\
      *val = tmpcstr;							\
    return tmpcstr;							\
  }


// Cannot be buffered.
// The user must delete the memory returned or passed back from the get method.
#define CSG_XSTR_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE)		\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(RWXmString *rwxmstr)	\
  {									\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, rwxmstr->getString());		\
    XtSetValues(widget, tmparg, 1);					\
  }									\
  RWXmString *CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RWXmString **val)	\
  {									\
    XmString tmpxmstr;							\
    RWXmString *(tmprwxmstr) = new RWXmString("");			\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, &tmpxmstr);				\
    XtGetValues(widget, tmparg, 1);					\
    (tmprwxmstr)->setString(tmpxmstr);					\
    if (val)								\
      *val = tmprwxmstr;						\
    return tmprwxmstr;							\
  }


// Cannot be buffered.
// The user must delete the memory returned or passed back from the get method.
#define CSG_OPTION_XSTR_RESOURCE(MEMBER_FUNCTION,RESOURCE)		\
  void RWOptionMenu:: name2(set,MEMBER_FUNCTION)(RWXmString *rwxmstr)	\
  {									\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, rwxmstr->getString());		\
    XtSetValues((optionLabelGadget())->getWidget(), tmparg, 1);		\
  }									\
  RWXmString *RWOptionMenu:: name2(get,MEMBER_FUNCTION)(RWXmString **val)\
  {									\
    XmString tmpxmstr;							\
    RWXmString *(tmprwxmstr) = new RWXmString("");			\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, &tmpxmstr);				\
    XtGetValues((optionLabelGadget())->getWidget(), tmparg, 1);		\
    (tmprwxmstr)->setString(tmpxmstr);					\
    if (val)								\
      *val = tmprwxmstr;						\
    return tmprwxmstr;							\
  }


// Cannot be buffered.
#define C_XSTR_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE)		\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(RWXmString *rwxmstr)	\
  {									\
    Arg tmparg[1];							\
    XtSetArg(tmparg[0], RESOURCE, rwxmstr->getString());		\
    XtSetValues(widget, tmparg, 1);					\
  }


//  name2(set,MEMBER_FUNCTION) buffering implemented.
//  name2(get,MEMBER_FUNCTION) cannot be buffered.
// The user must delete the memory returned or passed back from the get method.
#define CSG_XSTRDIR_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE)		\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(RWXmStringDirection *rwxmstr)	\
  {										\
    if (checkResourceBufferSize(sargcnt)) {					\
      XtSetArg(sdargs[sargcnt], RESOURCE, rwxmstr->getDirection()); sargcnt++;	\
      if (!bufferResources) {							\
	XtSetValues(widget, &sdargs, sargcnt); sargcnt=0;			\
      }										\
    }										\
  }										\
  RWXmStringDirection *CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RWXmStringDirection **rwxmstr) \
  {										\
    XmStringDirection tmpxmstr=0;						\
    RWXmStringDirection *tmprwxmstr;						\
    Arg tmparg[1];								\
    XtSetArg(tmparg[0], RESOURCE, &tmpxmstr);					\
    XtGetValues(widget, tmparg, 1);						\
    tmprwxmstr = new RWXmStringDirection(tmpxmstr);				\
    if (!rwxmstr)								\
      return tmprwxmstr;							\
    else {									\
      *rwxmstr=tmprwxmstr;							\
      return *rwxmstr;								\
    }										\
  }


// Cannot be buffered.
// The user must delete the memory returned or passed back from the get method.
#define CG_XSTRDIR_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE)	\
RWXmStringDirection *CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RWXmStringDirection **rwxmstr) \
{									\
  XmStringDirection tmpxmstr=0;						\
  RWXmStringDirection *tmprwxmstr;					\
  Arg tmparg[1];							\
  XtSetArg(tmparg[0], RESOURCE, &tmpxmstr);				\
  XtGetValues(widget, tmparg, 1);					\
  tmprwxmstr = new RWXmStringDirection(tmpxmstr);			\
  if (!rwxmstr)								\
    return tmprwxmstr;							\
  else {								\
    *rwxmstr=tmprwxmstr;						\
    return *rwxmstr;							\
  }									\
}


// Cannot be buffered.
// The user must delete the memory returned or passed back from the get method.
#define CSG_XSTR_LIST_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE,CNT_RES,CNT,ARG_TYPE) \
void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(RWXmStringList list)		\
{										\
   int itemCount = this-> name2(get,CNT)();					\
  Arg tmparg[1];								\
  ARG_TYPE xmstrlist=rwXmStrListToXmStrList(list, itemCount);			\
  XtSetArg(tmparg[0], RESOURCE, xmstrlist);					\
  XtSetValues(widget, tmparg, 1);						\
  for (int i=0; i<itemCount; i++)						\
    delete xmstrlist[i];							\
  delete [] xmstrlist;								\
}										\
void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(RWXmStringList list, int itemCount)\
{										\
  Arg tmparg[2];								\
  ARG_TYPE xmstrlist=rwXmStrListToXmStrList(list, itemCount);			\
  XtSetArg(tmparg[0], CNT_RES, itemCount);					\
  XtSetArg(tmparg[1], RESOURCE, xmstrlist);					\
  XtSetValues(widget, tmparg, 2);						\
  for (int i=0; i<itemCount; i++)						\
    delete xmstrlist[i];							\
  delete [] xmstrlist;								\
}										\
RWXmStringList CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RWXmStringList *rwxmstrlist) \
{										\
  int itemCount = this-> name2(get,CNT)();					\
  ARG_TYPE xmstrlist;								\
  Arg tmparg[1];								\
  XtSetArg(tmparg[0], RESOURCE, &xmstrlist);					\
  XtGetValues(widget, tmparg, 1);						\
  if (!rwxmstrlist)								\
    return (xmStrListToRWXmStrList(xmstrlist, itemCount));			\
  else {									\
    *rwxmstrlist = xmStrListToRWXmStrList(xmstrlist, itemCount);		\
    return *rwxmstrlist;							\
  }										\
}
  
// Cannot be buffered.
// The user must delete the memory returned or passed back from the get method.
#define CSG_RWORDERED_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE1,RESOURCE2)	\
void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(const RWOrdered & list)		\
{										\
  XmStringList xmstrlst = rworderedToXmStrList (list);				\
  Arg tmparg[2];								\
  int len = list.entries();							\
  XtSetArg(tmparg[0], RESOURCE1, xmstrlst);					\
  XtSetArg(tmparg[1], RESOURCE2, len);						\
  XtSetValues(widget, tmparg, 2);						\
  for(int i=0; i<len; i++)							\
    XmStringFree(xmstrlst[i]);							\
  delete []xmstrlst;								\
}										\
RWOrdered * CLASS_NAME:: name3(get,MEMBER_FUNCTION,C)(RWOrdered ** val)		\
{										\
  RWOrdered * list = rwnil;							\
  XmStringList gval;								\
  int numItems;									\
  Arg tmparg[2];								\
  XtSetArg(tmparg[0], RESOURCE1, &gval);					\
  XtSetArg(tmparg[1], RESOURCE2, &numItems);					\
  XtGetValues(widget, tmparg, 2);						\
  list = xmStrListToRWOrdered (gval, numItems);					\
  if (val)									\
    *val = list;								\
  return list;									\
}										\
void CLASS_NAME:: name3(set,MEMBER_FUNCTION,C)(char **list, int cnt)		\
{										\
  XmStringList xmstrlst = strListToXmStrList (list,cnt);			\
  Arg tmparg[2];								\
  XtSetArg(tmparg[0], RESOURCE1, xmstrlst);					\
  XtSetArg(tmparg[1], RESOURCE2, cnt);						\
  XtSetValues(widget, tmparg, 2);						\
  for(int i=0; i<cnt; i++)							\
    XmStringFree(xmstrlst[i]);							\
  delete []xmstrlst;								\
}


//  Cannot be buffered.
// The user must delete the memory returned or passed back from the get method.
#define CSG_RWCSTRORDVEC_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE1,RESOURCE2) \
void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(RWCStringOrderedVec & list)	\
{										\
  XmStringList xmstrlst = rwcstrvecToXmStrList (list);				\
  Arg tmparg[2];								\
  int len = list.length();							\
  XtSetArg(tmparg[0], RESOURCE1, xmstrlst);					\
  XtSetArg(tmparg[1], RESOURCE2, len);						\
  XtSetValues(widget, tmparg, 2);						\
  for(int i=0; i<len; i++)							\
    XmStringFree(xmstrlst[i]);							\
  delete []xmstrlst;								\
}										\
void CLASS_NAME:: name3(get,MEMBER_FUNCTION,C)(RWCStringOrderedVec & val)	\
{										\
  XmStringList gval;								\
  int numItems;									\
  Arg tmparg[2];								\
  XtSetArg(tmparg[0], RESOURCE1, &gval);					\
  XtSetArg(tmparg[1], RESOURCE2, &numItems);					\
  XtGetValues(widget, tmparg, 2);						\
  xmStrListToRWCStringOrdVec (val, gval, numItems);				\
}										\
RWCStringOrderedVec CLASS_NAME:: name3(get,MEMBER_FUNCTION,C)()			\
{										\
  RWCStringOrderedVec list;							\
  XmStringList gval;								\
  int numItems;									\
  Arg tmparg[2];								\
  XtSetArg(tmparg[0], RESOURCE1, &gval);					\
  XtSetArg(tmparg[1], RESOURCE2, &numItems);					\
  XtGetValues(widget, tmparg, 2);						\
  xmStrListToRWCStringOrdVec (list, gval, numItems);				\
  return list;									\
}										\
void CLASS_NAME:: name3(set,MEMBER_FUNCTION,C)(char **list, int cnt)		\
{										\
  XmStringList xmstrlst = strListToXmStrList (list,cnt);			\
  Arg tmparg[2];								\
  XtSetArg(tmparg[0], RESOURCE1, xmstrlst);					\
  XtSetArg(tmparg[1], RESOURCE2, cnt);						\
  XtSetValues(widget, tmparg, 2);						\
  for(int i=0; i<cnt; i++)							\
    XmStringFree(xmstrlst[i]);							\
  delete []xmstrlst;								\
}


// Cannot be buffered.
// The user must delete the memory returned or passed back from the get method.
#define CSG_STR_LIST_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE,CNT_RES,CNT,ARG_TYPE) \
void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(String *list)			\
{										\
  int itemCount = this-> name2(get,CNT)();					\
  Arg tmparg[2];								\
  ARG_TYPE xmstrlist=strListToXmStrList(list, itemCount);			\
  XtSetArg(tmparg[0], CNT_RES,	itemCount);					\
  XtSetArg(tmparg[1], RESOURCE, xmstrlist);					\
  XtSetValues(widget, tmparg, 2);						\
  for(int i=0; i<itemCount; i++)						\
    XmStringFree(xmstrlst[i]);							\
  delete [] xmstrlist;								\
}										\
void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(String *list, int itemCount)	\
{										\
  Arg tmparg[2];								\
  ARG_TYPE xmstrlist=strListToXmStrList(list, itemCount);			\
  XtSetArg(tmparg[0], CNT_RES, itemCount);					\
  XtSetArg(tmparg[1], RESOURCE, xmstrlist);					\
  XtSetValues(widget, tmparg, 2);						\
  for(int i=0; i<itemCount; i++)						\
    XmStringFree(xmstrlst[i]);							\
  delete [] xmstrlist;								\
}										\
char** CLASS_NAME:: name2(get,MEMBER_FUNCTION)(String **list)			\
{										\
  int itemCount = this-> name2(get,CNT)();					\
  ARG_TYPE xmstrlist;								\
  Arg tmparg[1];								\
  XtSetArg(tmparg[0], RESOURCE, &xmstrlist);					\
  XtGetValues(widget, tmparg, 1);						\
  if (!list)									\
    return (xmStrListToStrList(xmstrlist, itemCount));				\
  else {									\
    *list = xmStrListToStrList(xmstrlist, itemCount);				\
    return(*list);								\
  }										\
}


// name2(set,MEMBER_FUNCTION) buffering implemented
// name2(get,MEMBER_FUNCTION) cannot be buffered.
// The user must delete the memory returned or passed back from the get method.
#define CSG_FONT_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE)		\
void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(RWFontList *fnt)		\
{									\
  if (checkResourceBufferSize(sargcnt)) {				\
    XtSetArg(sdargs[sargcnt], RESOURCE, fnt->getFontList()); sargcnt++;	\
    if (!bufferResources) {						\
      XtSetValues(widget, &sdargs, sargcnt);				\
      sargcnt = 0;							\
    }									\
  }									\
}									\
RWFontList *CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RWFontList **fnt)	\
{									\
  XmFontList tmpfnt;							\
  RWFontList *fntlist;							\
  Arg tmparg[1];							\
  XtSetArg(tmparg[0], RESOURCE, &tmpfnt);				\
  XtGetValues(widget, tmparg, 1);					\
  fntlist = new RWFontList(tmpfnt);					\
  if (!fnt)								\
    return(fntlist);							\
  else {								\
    *fnt=fntlist;							\
    return(*fnt);							\
  }									\
}


// Buffering implemented.
#define S_FORM_POSITION_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE,ATTACH_POSITION) \
void CLASS_NAME:: name2(set,MEMBER_FUNCTION)					\
  (unsigned char attach, int position)						\
{										\
  if (checkResourceBufferSize(sargcnt)) {					\
    XtSetArg(sdargs[sargcnt], RESOURCE, attach); sargcnt++;			\
    XtSetArg(sdargs[sargcnt],  ATTACH_POSITION, position); sargcnt++;		\
    if (!bufferResources) {							\
      XtSetValues(widget, &sdargs, sargcnt);					\
      sargcnt = 0;								\
    }										\
  }										\
}
#define S_FORM_ATTACHMENT_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE,ATTACH_WIDGET) \
void CLASS_NAME:: name2(set,MEMBER_FUNCTION)					\
  (unsigned char attach, RWController *obj)					\
{										\
  if (checkResourceBufferSize(sargcnt)) {					\
    XtSetArg(sdargs[sargcnt], RESOURCE, attach); sargcnt++;			\
    XtSetArg(sdargs[sargcnt],  ATTACH_WIDGET, obj->getWidget()); sargcnt++;	\
    if (!bufferResources) {							\
      XtSetValues(widget, &sdargs, sargcnt);					\
      sargcnt = 0;								\
    }										\
  }										\
}


// Cannot be buffered.
// The user must delete the RWOrdered returned or passed back from the get method.
#define G_ORDERED_WIDGETLIST_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE) \
RWOrdered* CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RWOrdered** val)	\
{									\
  RWOrdered *wl = 0;							\
  RWController *co = 0;							\
  WidgetList tmpval;							\
  int length=0;								\
  Arg tmpargs[2];							\
  XtSetArg(tmpargs[0], RESOURCE, &tmpval);				\
  XtSetArg(tmpargs[1], XmNnumChildren, &length);			\
  XtGetValues(widget, tmpargs, 2);					\
  if (length>0) {							\
    wl = new RWOrdered();						\
    for(int i=0; i<length; i++) {					\
      co = getSurrogateObject(tmpval[i]);				\
      wl->insert(co);							\
    }									\
  }									\
  if (val)								\
    *val=wl;								\
  return wl;								\
}


#define C_CALLBACK_RESOURCE_OBJ(MEMBER_FUNCTION,RESOURCE)		\
void RWResourceObj:: name2(set,MEMBER_FUNCTION) (XtCallbackList val)	\
{									\
  if (checkResourceBufferSize()) {					\
    XtSetArg(dargs[argcnt], RESOURCE, val); argcnt++;			\
  }									\
}


#define C_RESOURCE_OBJ(MEMBER_FUNCTION,RESOURCE,RES_TYPE)		\
void RWResourceObj:: name2(set,MEMBER_FUNCTION)(RES_TYPE val)		\
{									\
  if (checkResourceBufferSize()) {					\
    XtSetArg(dargs[argcnt], RESOURCE, val); argcnt++;			\
  }									\
}


#define C_DRAGICON_RESOURCE_OBJ(MEMBER_FUNCTION,RESOURCE)		\
void RWResourceObj:: name2(set,MEMBER_FUNCTION)(RWDragIcon * val)	\
{									\
  if (checkResourceBufferSize()) {					\
    XtSetArg(dargs[argcnt], RESOURCE, val->getWidget()); argcnt++;	\
  }									\
}


#define C_OBJ_RESOURCE_OBJ(MEMBER_FUNCTION,RESOURCE)			\
void RWResourceObj:: name2(set,MEMBER_FUNCTION)(RWController *obj)	\
{									\
  if (checkResourceBufferSize()) {					\
    XtSetArg(dargs[argcnt], RESOURCE, obj->getWidget()); argcnt++;	\
  }									\
}

#define C_SPEC_OBJ_RESOURCE_OBJ(MEMBER_FUNCTION,RESOURCE,CONTROLLER)	\
void RWResourceObj:: name2(set,MEMBER_FUNCTION)(CONTROLLER *obj)	\
{									\
  if (checkResourceBufferSize()) {					\
    XtSetArg(dargs[argcnt], RESOURCE, obj->getWidget()); argcnt++;	\
  }									\
}


#define C_FONT_RESOURCE_OBJ(MEMBER_FUNCTION,RESOURCE)			\
void RWResourceObj:: name2(set,MEMBER_FUNCTION)(RWFontList *fnt)	\
{									\
  if (checkResourceBufferSize()) {					\
    XtSetArg(dargs[argcnt], RESOURCE, fnt->getFontList()); argcnt++;	\
  }									\
}


#define C_XSTRDIR_RESOURCE_OBJ(MEMBER_FUNCTION,RESOURCE)		\
void RWResourceObj:: name2(set,MEMBER_FUNCTION)(RWXmStringDirection *rwxmstr) \
{									\
  if (checkResourceBufferSize()) {					\
    XtSetArg(dargs[argcnt], RESOURCE, rwxmstr->getDirection());		\
    argcnt++;								\
  }									\
}

#define CSG_DND_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE,RES_TYPE)	\
  void CLASS_NAME:: name2(set,MEMBER_FUNCTION)(RES_TYPE val)		\
  {									\
    if (checkResourceBufferSize(sargcnt)) {				\
      XtSetArg(sdargs[sargcnt], RESOURCE, val); sargcnt++;		\
      if (!bufferResources) {						\
	XmDropSiteUpdate(widget, &sdargs, sargcnt); sargcnt=0;		\
      }									\
    }									\
  }									\
  RES_TYPE CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RES_TYPE* val)	\
  {									\
    if (checkResourceBufferSize(gargcnt)) {				\
      RES_TYPE gval;							\
      RES_TYPE *tmpval = &gval;						\
      if (val)								\
	tmpval = val;							\
      XtSetArg(gdargs[gargcnt], RESOURCE, tmpval); gargcnt++;		\
      if (!bufferResources) {						\
	XmDropSiteRetrieve(widget, &gdargs, gargcnt);			\
	gargcnt = 0;							\
	return (*tmpval);						\
      }									\
      return (RES_TYPE) 0;						\
    }									\
    return (RES_TYPE) 0;						\
  }

#define CG_DND_RESOURCE(CLASS_NAME,MEMBER_FUNCTION,RESOURCE,RES_TYPE)	\
  RES_TYPE CLASS_NAME:: name2(get,MEMBER_FUNCTION)(RES_TYPE* val)	\
  {									\
    if (checkResourceBufferSize(gargcnt)) {				\
      RES_TYPE gval;							\
      RES_TYPE *tmpval = &gval;						\
      if (val)								\
	tmpval = val;							\
      XtSetArg(gdargs[gargcnt], RESOURCE, tmpval); gargcnt++;		\
      if (!bufferResources) {						\
	XmDropSiteRetrieve(widget, &gdargs, gargcnt); gargcnt=0;	\
	return(*tmpval);						\
      }									\
      return (RES_TYPE) 0;						\
    }									\
    return (RES_TYPE) 0;						\
  }



#endif  /* End of file Util.h - Do not add beyond this point! */
