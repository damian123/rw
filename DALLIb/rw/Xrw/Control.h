#ifndef __RW_CONTROLLER_H
#define __RW_CONTROLLER_H
/*********************************************************************
*
*  $Id: Control.h,v 1.36 1994/06/14 18:15:34 steves Exp $
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
*  File        : Control.h
*
*  Developed By: Rogue Wave Software, Inc.
*
*  Created     : November 1990
*
*  Project     : View.h++
*
*  Description : This file contains the C++ class definition for the
*                Controller class.  This C++ class represents both
*                OSF/Motif widget classes Object and RectObj.
*                From a C++ perspective, there was no need to
*                implement both of these because they are not directly
*                instantiated.  Subclasses merely need access to their
*                resources; the proper inheritance of which are handled
*                by Motif.  Member functions to access the combined
*                Object and RectObj resources are provided on
*                this class object.
*
*
*  Revision History:
*
*  09-17-91      Added the following member functions
*                (Motif 1.1):
*                  CvtCTToXmString
*                  SetFontUnits
*
*********************************************************************/

/*********************************************************************
* #include files
*********************************************************************/


#include <rw/Xrw/App.h>
#include <rw/Xrw/AnyCB.h>
#include <rw/Xrw/Util.h>
#include <rw/Xrw/DynArgArr.h>

#if defined(__DEC__) || (RWX11Version>=5)
#include <Xm/Protocols.h>
#include <Xm/AtomMgr.h>
#else
#include <Xm/Protocols.h>
#include <Xm/AtomMgr.h>
#endif
#include <Xm/CutPaste.h>

#if RWX11Version>=5
#include <Xm/RepType.h>
#endif

#ifdef ESIX_PRE_404
// The ESIX Intrinsic.h prior to V. 4.0.4 has XtTransformCoords but not XtTranslateCoords
extern "C" void XtTranslateCoords(Widget,Position,Position,Position *,Position *);
#endif

#if RWX11Version>=5

class RWXmScreen;

/*
 * The following evaluates true every time.  It is possible to use 
 * templates with compilers (like the IBM xlC compiler) that can
 * instantiate from a library, but this makes life much more difficult
 * when porting because now we need to supply a version of Tools.h++ which
 * includes the template classes.  So why bother?
 */
#if 1 || defined(RW_NO_TEMPLATES) || defined(RW_NO_LIBRARY_INSTANTIATE)
#   include <rw/gordvec.h>
    declare(RWGVector,Dimension)
    declare(RWGOrderedVector,Dimension)
    declare(RWGVector,Atom)
    declare(RWGOrderedVector,Atom)
    typedef RWGOrderedVector(Dimension) RWDimensionOrderedVec;
    typedef RWGOrderedVector(Atom) RWAtomOrderedVec;
# else
#   include <rw/tvordvec.h>
    typedef RWTValOrderedVector<Dimension> RWDimensionOrderedVec;
    typedef RWTValOrderedVector<Atom> RWAtomOrderedVec;
# endif
#endif

class RWResourceObj;

/*********************************************************************
* RWController : The top level GUI object of the View.h++ system.  It contains
* data members and methods that will be inherited by all other widget
* objects in the system.  It is meant to serve as a base class and
* not to be instantiated directly.
*********************************************************************/

class RWController : public RWApplication
{
 public:
  RWController();                                      // Internal.  
  RWController(Widget, Boolean managed = False);       // Internal.
  virtual ~RWController();

// RESOURCE MEMBER FUNCTIONS
  void       setDestroyCallback   (RWXtCallbackProc, RWXtPointer);
  void       setDestroyMethod     (RWApplication *, RWMethodPtr, void *);
  void       setDestroyCallback   (XtCallbackList);
  Boolean    getAncestorSensitive (Boolean *val = 0);
  void       setBorderWidth       (Dimension);
  Dimension  getBorderWidth       (Dimension *val = 0);
  void       setHeight            (Dimension);
  Dimension  getHeight            (Dimension *val = 0);
  void       setSensitive         (Boolean);
  Boolean    getSensitive         (Boolean *val = 0);
  void       setWidth             (Dimension);
  Dimension  getWidth             (Dimension *val = 0);
  void       setX                 (Position);
  Position   getX                 (Position *val = 0);
  void       setY                 (Position);
  Position   getY                 (Position *val = 0);


// FORM CONSTRAINT RESOURCES
  void           setBottomAttachment (unsigned char);
  void           setBottomAttachment (unsigned char, int);
  void           setBottomAttachment (unsigned char, RWController *);
  unsigned char  getBottomAttachment (unsigned char *val = 0);
  void           setBottomOffset     (int);
  int            getBottomOffset     (int *val = 0);
  void           setBottomPosition   (int);
  int            getBottomPosition   (int *val = 0);
  void           setBottomWidget     (RWController *);
  RWController * getBottomWidget     (RWController **val = 0);
  void           setBottomObject     (RWController *);
  RWController * getBottomObject     (RWController **val = 0);
  void           setLeftAttachment   (unsigned char);
  void           setLeftAttachment   (unsigned char, int);
  void           setLeftAttachment   (unsigned char, RWController *);
  unsigned char  getLeftAttachment   (unsigned char *val = 0);
  void           setLeftOffset       (int);
  int            getLeftOffset       (int *val = 0);
  void           setLeftPosition     (int);
  int            getLeftPosition     (int *val = 0);
  void           setLeftWidget       (RWController *);
  RWController * getLeftWidget       (RWController **val = 0);
  void           setLeftObject       (RWController *);
  RWController * getLeftObject       (RWController **val = 0);
  void           setResizable        (Boolean);
  Boolean        getResizable        (Boolean *val = 0);
  void           setRightAttachment  (unsigned char);
  void           setRightAttachment  (unsigned char, int);
  void           setRightAttachment  (unsigned char, RWController *);
  unsigned char  getRightAttachment  (unsigned char *val = 0);
  void           setRightOffset      (int);
  int            getRightOffset      (int *val = 0);
  void           setRightPosition    (int);
  int            getRightPosition    (int *val = 0);
  void           setRightWidget      (RWController *);
  RWController * getRightWidget      (RWController **val = 0);
  void           setRightObject      (RWController *);
  RWController * getRightObject      (RWController **val = 0);
  void           setTopAttachment    (unsigned char);
  void           setTopAttachment    (unsigned char, int);
  void           setTopAttachment    (unsigned char, RWController *);
  unsigned char  getTopAttachment    (unsigned char *val = 0);
  void           setTopOffset        (int);
  int            getTopOffset        (int *val = 0);
  void           setTopPosition      (int);
  int            getTopPosition      (int *val = 0);
  void           setTopWidget        (RWController *);
  RWController * getTopWidget        (RWController **val = 0);
  void           setTopObject        (RWController *);
  RWController * getTopObject        (RWController **val = 0);


// PANED WINDOW CONSTRAINT RESOURCES  
  void           setAllowResize      (Boolean);
  Boolean        getAllowResize      (Boolean *val = 0);
  void           setPaneMaximum      (int);
  int            getPaneMaximum      (int *val = 0);
  void           setPaneMinimum      (int);
  int            getPaneMinimum      (int *val = 0);
  void           setSkipAdjust       (Boolean);
  Boolean        getSkipAdjust       (Boolean *val = 0);


#if RWX11Version>=5
// FRAME CONSTRAINT FUNCTIONS - Motif 1.2
  void          setChildType                (unsigned char);
  unsigned char getChildType                (unsigned char *val = 0);
  void          setChildHorizontalAlignment (unsigned char);
  unsigned char getChildHorizontalAlignment (unsigned char *val = 0);
  void          setChildHorizontalSpacing   (Dimension);
  Dimension     getChildHorizontalSpacing   (Dimension *val = 0);
  void          setChildVerticalAlignment   (unsigned char);
  unsigned char getChildVerticalAlignment   (unsigned char *val = 0);
#endif


// ROWCOLUMN CONSTRAINT FUNCTIONS 
  void          setPositionIndex( short int );
  short int     getPositionIndex( short int *val = 0 );


// XT INTRINSICS MEMBER FUNCTION
  virtual void   manage              ();
  virtual void   unmanage            ();

  
/*****************************************************************/
/*****************************************************************/
/* START OF XTINTRINSICS FUNCTIONS                               */
/*****************************************************************/
  
  WidgetClass    getClass                      ();
  WidgetClass    getSuperClass                 ();
  Boolean        isSubclass                    (WidgetClass);
  void           destroyApplicationContext     (XtAppContext);
  XtAppContext   objectToApplicationContext    ();
  void           realize                       ();
  Boolean        isRealized                    ();
  void           createWindow                  (
						unsigned int, 
						Visual *,
						XtValueMask, 
						XSetWindowAttributes *
					       );
  Display      * getDisplay                    ();
  RWController * getParent                     ();
  Screen       * getScreen                     ();
  Window         getWindow                     ();
  void           unrealize                     ();
  void           destroy                       ();
  Boolean        isComposite                   ();
  void           manageChild                   ();
  void           unmanageChild                 ();
  Boolean        isManaged                     ();
  void           map                           ();
  void           unmap                         ();
  void           popup                         (XtGrabKind);
  void           popdown                       ();
  void           callbackNone                  (RWXtPointer, RWXtPointer);
  void           callbackNonexclusive          (RWXtPointer, RWXtPointer);
  void           callbackExclusive             (RWXtPointer, RWXtPointer);
  void           callbackPopdown               (RWXtPointer, RWXtPointer);
  void           move                          (Position, Position);
  void           resize                        (int, int, int);
  void           configure                     (int, int, int, int, int);
  void           resizeWindow                  ();
  void           addGrab                       (Boolean, Boolean);
  void           removeGrab                    ();
  void           setKeyboardFocus              (RWController *);
  Boolean        callAcceptFocus               (Time *);
  void           sensitive                     (Boolean);
  Boolean        isSensitive                   ();
  void           addEventHandler               (
						EventMask, 
						Boolean, 
						void *,
						void *
					       );
  void           addEventMethod                (
						EventMask, 
						Boolean, 
						RWApplication *,
						RWEventMethodPtr, 
						void *
					       );
  void           removeEventMethod             (
						EventMask, 
						Boolean, 
						RWApplication *,
						RWEventMethodPtr, 
						void *
					       );
  void           removeEventHandler            (
						EventMask, 
						Boolean, 
						void *,
						void *
					       );
  void           addRawEventHandler            (
						EventMask, 
						Boolean, 
						void *,
						void *
					       );
  void           removeRawEventHandler         (
						EventMask, 
					        Boolean, 
					        void *, 
					        void *
					       );
  EventMask      buildEventMask                ();
  void           addMethod                     (
						char *,
					        RWApplication *, 
					        RWMethodPtr, 
					        void *
					       );
  void           removeMethod                  (
					        char *,
					        RWApplication *, 
					        RWMethodPtr, 
					        void *
					       );
  void           addCallback                   (
						char *,
						RWXtCallbackProc, 
						RWXtPointer
					       );
  void           addCallbacks                  (char *, XtCallbackList);
  void           removeCallback                (char *, 
						RWXtCallbackProc, 
						RWXtPointer);
  void           removeCallbacks               (char *, XtCallbackList);
  void           removeAllCallbacks            (char *);
  void           callCallbacks                 (char *, RWXtPointer);
  
  XtCallbackStatus hasCallbacks                (char *);

  void           getSubresources               (
						RWXtPointer, 
						char *, 
						char *, 
						XtResourceList,
						Cardinal, 
						ArgList, 
						Cardinal
					       );
  void           getApplicationResources       (
						RWXtPointer, 
						XtResourceList, 
						Cardinal,
						ArgList, 
						Cardinal 
					       );
  void           convert                      (char *, 
					       XrmValuePtr, 
					       char *, 
					       XrmValuePtr
					      );
  void           getValues                    (ArgList, Cardinal);
  void           getValues                    ();
  void           setValues                    (const RWResourceObj &);
  void           setValues                    (ArgList, Cardinal);
  void           setValues                    ();
  void           augmentTranslations          (XtTranslations);
  void           overrideTranslations         (XtTranslations);
  void           uninstallTranslations        ();
  void           installAccelerators          (RWController *);
  void           installAllAccelerators       (RWController *);
  RWController * nameToObject                 (const RWCString &);
  GC             getGC                        (XtGCMask, XGCValues *);
  void           releaseGC                    (GC);
  void           getSelectionValue            (Atom, 
					       Atom, 
					       XtSelectionCallbackProc, 
					       RWXtPointer, 
					       Time
					      );
  void           getSelectionValues           (Atom, 
					       Atom *,
					       int,
					       XtSelectionCallbackProc,
					       RWXtPointer *,
					       Time
					      );
  Boolean        ownSelection                 (Atom, 
					       Time,
					       XtConvertSelectionProc,
					       XtLoseSelectionProc,
					       XtSelectionDoneProc
					      );
  void           disownSelection              (Atom, Time);
  void           translateCoords              (Position, 
					       Position,
					       Position *, 
					       Position *
					      );
  

  static void         toolkitInitialize        (); 
  static XtAppContext createApplicationContext ();
  static void         displayInitialize        (XtAppContext, 
						Display *,
						char *, 
						char *,
						XrmOptionDescRec *, 
						Cardinal,
#if RWX11Version>=5
						int *,
#else
						Cardinal *,
#endif
						char **
					       );
  static Display *    openDisplay              (XtAppContext, 
						char *, 
						char *,
						char *, 
						XrmOptionDescRec *, 
						Cardinal,
#if RWX11Version>=5
						int *,
#else
						Cardinal *,
#endif 
						char **
					       );
  static void         closeDisplay             (Display *);
  static ArgList      mergeArgLists            (ArgList, 
						Cardinal, 
						ArgList, 
						Cardinal
					       );

  static XtInputId    appAddInput              (XtAppContext, 
						int,
						RWXtPointer,
						void *, 
						RWXtPointer 
					       );
  static XtInputId    addInput                 (int, 
						RWXtPointer, 
						void *, 
						RWXtPointer
					       );
  static void         removeInput              (XtInputId);
  static XtIntervalId appAddTimeOut            (XtAppContext, 
						unsigned long,
						void *, 
						RWXtPointer 
					       );
  static XtIntervalId addTimeOut               (unsigned long,
						void *, 
						RWXtPointer
					       );
  static void         removeTimeOut            (XtIntervalId);
  static XtInputMask  appPending               (XtAppContext);
  static Boolean      appPeekEvent             (XtAppContext, XEvent *);
  static void         appNextEvent             (XtAppContext, XEvent *);
  static void         appProcessEvent          (XtAppContext, XtInputMask);
  static void         dispatchEvent            (XEvent *);
  static void         appMainLoop              (XtAppContext);
  static void         mainLoop                 ();
  static XtWorkProcId appAddWorkProc           (XtAppContext, void *, RWXtPointer);
  static XtWorkProcId addWorkProc              (void *, RWXtPointer);
  static void         removeWorkProc           (XtWorkProcId id);

  static void         getResourceList          (WidgetClass, 
						XtResourceList *,
						Cardinal *
					       );
  static void         stringConversionWarning  (char *, char *);
  static void         appAddConverter          (XtAppContext, 
						char *, 
						char *,
						void *, 
						XtConvertArgList, 
						Cardinal
					       );
  static void         addConverter             (char *, 
						char *,
						void *, 
						XtConvertArgList, 
						Cardinal
					       );
  static void         directConvert            (void *, 
						XrmValuePtr,
						Cardinal, 
						XrmValuePtr, 
						XrmValuePtr
					       );
  static void         getSubvalues             (RWXtPointer, 
						XtResourceList,
						Cardinal,
						ArgList, 
						Cardinal
					       );
  static void         setSubvalues             (RWXtPointer, 
						XtResourceList, 
						Cardinal,
						ArgList, 
						Cardinal
					       );
  static void         addActions               (XtActionList, Cardinal);
  static void         appAddActions            (XtAppContext, 
						XtActionList, 
						Cardinal
					       );
  static XtTranslations parseTranslationTable  (const RWCString &);
  static XtAccelerators parseAcceleratorTable  (const RWCString &);
  
  static void         setKeyTranslator         (Display *, void *);
  static void         translateKeycode         (Display *, 
					        KeyCode, 
						Modifiers, 
						Modifiers *, 
						KeySym *
					       );
  static void         registerCaseConverter    (Display *, 
					        void *,
						KeySym,
						KeySym
					       );
  static void         convertCase              (Display *, 
						KeySym,
						KeySym *,
						KeySym *
					       );
  static char *       xtMalloc                 (unsigned int);
  static char *       xtCalloc                 (unsigned int, unsigned int);
  static char *       xtRealloc                (char *, unsigned int);
  static void         xtFree                   (char *);
  
  static String         newString               (const RWCString &);
  static void           appSetSelectionTimeout  (XtAppContext, unsigned long);
  static unsigned long  appGetSelectionTimeout  (XtAppContext);
  
  static void           addExposureToRegion     (XEvent *, Region);
  
  static RWController * windowToObject          (Display *, Window);
  static XrmDatabase *  appGetErrorDatabase     (XtAppContext);
  static void           appGetErrorDatabaseText (XtAppContext,
						 char *, 
						 char *, 
						 char *,
						 char *, 
						 char *,
						 int,
						 XrmDatabase
						);
  
  static void           appSetErrorMsgHandler   (XtAppContext, void *);
  static void           setErrorMsgHandler      (void *);
  static void           appErrorMsg             (XtAppContext,
						 char *,
						 char *,
						 char *,
						 char *,
						 char **,
						 Cardinal *
						);
  static void           appSetWarningMsgHandler (XtAppContext, void *);
  static void           setWarningMsgHandler    (void *);
  static void           appWarningMsg           (XtAppContext, 
						 char *, 
						 char *, 
						 char *,
						 char *, 
						 char **, 
						 Cardinal *
						);
  static void           appSetErrorHandler      (XtAppContext, void *);
  static void           setErrorHandler         (void *);
  static void           appError                (XtAppContext, char *);
  static void           appSetWarningHandler    (XtAppContext, void *);
  static void           setWarningHandler       (void *);
  static void           appWarning              (XtAppContext, char *);

/*****************************************************************/
/*****************************************************************/
/* START NEW X11R4 XTINTRINSICS FUNCTIONS                        */  
/*****************************************************************/


  static XtActionHookId appAddActionHook        (XtAppContext, 
						 void *,
						 RWXtPointer
						);

  static void           appReleaseCacheRefs     (XtAppContext, XtCacheRef *);
  
  static void           appSetFallbackResources (XtAppContext, char **);
  static void           appSetTypeConverter     (XtAppContext, 
						 char *, 
						 char *,
						 void *,
						 XtConvertArgList,
						 Cardinal,
						 XtCacheType,
						 void *
						);
  static Boolean        callConverter           (Display *, 
						 void *, 
						 XrmValuePtr, 
						 Cardinal,
						 XrmValuePtr, 
						 XrmValuePtr, 
						 XtCacheRef *
						);
  static XrmDatabase    database                (Display*);
  static void           destroyGC               (GC);
  static void           displayStringConversionWarning (Display *, char *, char *);
  static XtAppContext   displayToApplicationContext (Display *);
  static void           error                   (const RWCString &);
  static void           errorMsg                (char *, 
						 char *, 
						 char *, 
						 char *, 
						 char **,
						 Cardinal *
						);
  static char *         findFile                (const RWCString &, 
						 Substitution, 
						 Cardinal, 
						 XtFilePredicate
						);
  static KeySym         getActionKeysym         (XEvent *, Modifiers *);
  static void           getApplicationNameAndClass (Display *, char **, char **);
  static void           getConstraintResourceList (WidgetClass, 
						   XtResourceList *,
						   Cardinal *
						  );
  static XrmDatabase *  getErrorDatabase          ();
  static void           getErrorDatabaseText      (char *, 
						   char *, 
						   char *,
						   char *,
						   char *,
						   int
						  );
  static KeySym *       getKeysymTable            (Display *, 
						   KeyCode *, 
						   int *
						  );
  static int            getMultiClickTime         (Display *);
  static unsigned int   getSelectionTimeout       ();
  static void           initializeWidgetClass     (WidgetClass);
  static void           keysymToKeycodeList       (Display *, 
						   KeySym, 
						   KeyCode **,
						   Cardinal *
						  );
  static Time           lastTimestampProcessed    (Display *);

/* Not in our X11R4
  static void           menuPopdown               (char *);
  static void           menuPopup                 (char *);
*/
  static void           nextEvent                 (XEvent *);
  static Boolean        peekEvent                 (XEvent *);
  static XtInputMask    pending                   ();
  static void           processEvent              (XtInputMask);
  static void           registerGrabAction        (void *, 
						   Boolean,
						   unsigned int,
						   int,
						   int
						  );
  static void           removeActionHook          (XtActionHookId);
  static char *         resolvePathname           (Display *,
						   const RWCString &,
						   const RWCString &,
						   const RWCString &,
						   const RWCString &,
						   Substitution,
						   Cardinal,
						   XtFilePredicate
						  );
  static void           setMultiClickTime         (Display *, int);
  static void           setSelectionTimeout       (unsigned int);
  static void           setTypeConverter          (char *,
						   char *,
						   void *,
						   XtConvertArgList,
						   Cardinal,
						   XtCacheType,
						   void *
						  );
  static void           warning                   (const RWCString &);

  static void           warningMsg                (char *,
						   char *,
						   char *,
						   char *,
						   char **,
						   Cardinal *
						  );


  void           callActionProc               (char*, 
					       XEvent *, 
					       char **, 
					       Cardinal
					      );
  void           callbackReleaseCacheRef      (RWXtPointer, RWXtPointer);
  void           callbackReleaseCacheRefList  (RWXtPointer, RWXtPointer);
  void           callCallbackList             (XtCallbackList, RWXtPointer);


#ifdef DEBUG
//NOTE: This function is only available when compiling with the
//      DEBUG compiler directive.
  void           checkSubclass                (WidgetClass, char *);
#endif
  
  void           configureObject              (Position, 
					       Position, 
					       Dimension, 
					       Dimension,
					       Dimension
					      );
  Boolean        convertAndStore              (char *, 
					       XrmValuePtr, 
					       char *, 
					       XrmValuePtr
					      );
  Display *      displayOfObject              ();
  XSelectionRequestEvent *getSelectionRequest (Atom, XtRequestId);

  void           getSelectionValueIncremental (Atom, 
					       Atom, 
					       void *, 
					       RWXtPointer,
					       Time
					      );
  void           grabButton                   (int, 
					       Modifiers, 
					       Boolean, 
					       unsigned int, 
					       int, 
					       int,
					       Window,
					       Cursor
					      );
  void           grabKey                      (KeyCode, 
					       Modifiers,
					       Boolean,
					       int,
					       int
					      );
  int            grabKeyboard                 (Boolean, int, int, Time);
  int            grabPointer                  (Boolean, 
					       unsigned int, 
					       int,
					       int,
					       Window,
					       Cursor,
					       Time
					      );

  void           insertEventHandler           (EventMask, 
					       Boolean, 
					       void *, 
					       RWXtPointer,
					       XtListPosition
					      );
  void           insertRawEventHandler        (EventMask, 
					       Boolean, 
					       void *, 
					       RWXtPointer,
					       XtListPosition
					      );
  Boolean        isApplicationShell           ();
  Boolean        isConstraint                 ();
  Boolean        isObject                     ();
  Boolean        isOverrideShell              ();
  Boolean        isRectObj                    ();
  Boolean        isShell                      ();
  Boolean        isTopLevelShell              ();
  Boolean        isTransientShell             ();
  Boolean        isVendorShell                ();
  Boolean        isCore                       ();
  Boolean        isWMShell                    ();


  XtGeometryResult makeGeometryRequest        (XtWidgetGeometry *,
				               XtWidgetGeometry *
					      );
/*
  XtGeometryResult makeGeometryResult         (Dimension, 
					       Dimension,
					       Dimension *, 
					       Dimension *
					      );
*/

  char *           name                       ();

  Boolean          ownSelectionIncremental    (Atom, 
					       Time,
					       void *,
					       void *,
					       void *,
					       void *,
					       RWXtPointer
					      );
  void             popupSpringLoaded          ();
  XtGeometryResult queryGeometry              (XtWidgetGeometry *,
					       XtWidgetGeometry *
					      );
  Screen *         screenOfObject             ();
  void             setMappedWhenManaged       (Boolean);
  void             setWMColormapWindows       (RWController **, Cardinal);
  void             ungrabButton               (unsigned int, Modifiers);
  void             ungrabKey                  (KeyCode, Modifiers);
  void             ungrabKeyboard             (Time);
  void             ungrabPointer              (Time);
  Window           windowOfObject             ();

/*****************************************************************/
/* END NEW X11R4 XTINTRINSICS FUNCTIONS                          */  
/*****************************************************************/
/*****************************************************************/


/*****************************************************************/
/* END OF XTINTRINSICS MEMBER FUNCTIONS                          */  
/*****************************************************************/
/*****************************************************************/


/*****************************************************************/
/*****************************************************************/
/* START OF MOTIF MEMBER FUNCTIONS                               */
/*****************************************************************/

  static RWXmString     cvtCTToXmString           (const RWCString &);
  static void           cvtStringToUnitType       (XrmValuePtr, 
						   Cardinal *,
						   XrmValue *,
						   XrmValue *
						  );
  static char *         cvtXmStringToCT           (XmString);
  static Boolean        destroyPixmap             (Screen *, Pixmap);
  static char *         getAtomName               (Display *, Atom);
  static XmColorProc    getColorCalculation       ();
  static Cursor         getMenuCursor             (Display *);
  static Pixmap         getPixmap                 (Screen *,
						   const RWCString &,
						   Pixel,
						   Pixel
						  );
  static Boolean        installImage              (XImage *, const RWCString &);
  static Atom           internAtom                (Display *, String, Boolean);
  static void           resolveAllPartOffsets     (WidgetClass, 
						   XmOffsetPtr *,
						   XmOffsetPtr *
						  );
  static void           resolvePartOffsets        (WidgetClass, XmOffsetPtr *);
  static XmColorProc    setColorCalculation       (XmColorProc);
  static void           setFontUnit               (Display *, int);
  static void           setFontUnits              (Display *, int, int);
  static void           setMenuCursor             (Display *, Cursor);
  static Boolean        uninstallImage            (XImage *);

  void           addTabGroup                  (RWController *);
  void           removeTabGroup               (RWController *);
  int            convertUnits                 (int, int, int, int);
  void           updateDisplay                ();
  Boolean        destroyPixmap                (Pixmap);
  void           getColors                    (Screen *, 
					       Colormap,
					       Pixel,
					       Pixel *,
					       Pixel *,
					       Pixel *,
					       Pixel *
					      );
  void           getColors                    (Screen *,
					       Colormap,
					       const RWCString &,
					       Pixel *,
					       Pixel *,
					       Pixel *,
					       Pixel *
					      );
  RWController * getDestination               (Display *);
  Cursor         getMenuCursor                ();
  Pixmap         getPixmap                    (const RWCString &, Pixel, Pixel);
  Pixmap         getPixmap                    (const RWCString &);
  Boolean        processTraversal             (
#if RWX11Version>=5
					       XmTraversalDirection
#else
					       int
#endif
					       );
  void           resolveAllPartOffsets        (XmOffsetPtr *, XmOffsetPtr *);
  void           resolvePartOffsets           (XmOffsetPtr *);
  RWController * trackingLocate               (Cursor, Boolean confine = False);

/*****************************************************************/
/* END OF MOTIF MEMBER FUNCTIONS                                 */
/*****************************************************************/
/*****************************************************************/

#if RWX11Version>=5

/*****************************/
/* DropSite                  */
/*****************************/

  void 			setAnimationMask (Pixmap);
  Pixmap 		getAnimationMask (Pixmap * val = 0);
  void 			setAnimationPixmap (Pixmap);
  Pixmap 		getAnimationPixmap (Pixmap * val = 0);
  void 			setAnimationPixmapDepth (int);
  int  			getAnimationPixmapDepth (int * val = 0);
  void 			setAnimationStyle (unsigned char);
  unsigned char 	getAnimationStyle (unsigned char * val = 0);
  void			setDragProc (RWXtCallbackProc);
  void                  setDragMethod (RWApplication *, RWMethodPtr);
  RWXtCallbackProc      getDragProc (RWXtCallbackProc * val = 0);
  void			setDropProc (RWXtCallbackProc);
  void                  setDropMethod (RWApplication *, RWMethodPtr);
  RWXtCallbackProc      getDropProc (RWXtCallbackProc * val = 0);
  void			setDropRectangles (XRectangle *);
  XRectangle *  	getDropRectangles (XRectangle ** val = 0);
  void			setDropSiteActivity (unsigned char);
  unsigned char		getDropSiteActivity (unsigned char * val = 0);
  void			setDropSiteOperations (unsigned char);
  unsigned char		getDropSiteOperations (unsigned char *val = 0);
  unsigned char		getDropSiteType (unsigned char * val = 0);
  void			setImportTargets (Atom *);
  Atom *		getImportTargets (Atom ** val = 0);
  void			setNumDropRectangles (Cardinal);
  Cardinal		getNumDropRectangles (Cardinal * val = 0);
  void			setNumImportTargets (Cardinal);
  Cardinal		getNumImportTargets (Cardinal * val = 0);

  Atom                  addImportTarget (Atom);
  Atom                  addImportTarget (const RWCString &);

  RWAtomOrderedVec*	getImportList(){return importList_;}

// RW TOOLS MEMBER FUNCTIONS
  void   configureStackingOrder (RWController *, Cardinal);
  void   endUpdate		();
  void   retrieve		(ArgList, Cardinal);
  void   retrieve		();
  void   startUpdate		();
  void   update			(const RWResourceObj&);
  void   update                 ();
  void   dropsiteRegister       (const RWResourceObj&);
  void   dropsiteUnregister     ();

  static Status queryStackingOrder (RWController *, RWController *&, RWOrdered &);

  RWCallbackInfo * getDropProcCB () { return dropProcCB; }
  RWCallbackInfo * getDragProcCB () { return dragProcCB; }

private:
  RWCallbackInfo * dropProcCB;
  RWCallbackInfo * dragProcCB;

  RWAtomOrderedVec *    importList_;

public:

/*****************************************************************/
/* Motif 1.2 Functions                                           */
/*****************************************************************/
  void           changeColor                  (Pixel);
  void           changeColor                  (const RWCString &);
  RWBoolean      getBaselines                 (RWDimensionOrderedVec*, int *);
  RWBoolean      getDisplayRect               (XRectangle *);
  RWController * getFocusObject               ();
  RWController * getTabGroup                  ();
  RWBoolean      IsTraversable                ();
  XmVisibility   getVisibility                ();
  RWController * trackingEvent                (Cursor, Boolean, XEvent *);

  static void    translateKey                 (Display *, KeyCode, Modifiers,
					       Modifiers *, KeySym *);

// Representation Type methods
  static void           rtAddReverse                   (XmRepTypeId);
  static XmRepTypeId    rtGetId                        (const RWCString &);
  static RWOrdered *    rtGetNameList                  (XmRepTypeId, RWBoolean);
  static XmRepTypeEntry rtGetRecord                    (XmRepTypeId);
  static XmRepTypeList  rtGetRegistered                ();
  static void           rtInstallTearOffModelConverter ();
  static XmRepTypeId    rtRegister                     (const RWCString &, 
							RWOrdered *, 
						        unsigned char *, 
						        unsigned char);
  static RWBoolean      rtValidValue                   (XmRepTypeId,
						        unsigned char,
						        RWController *);
  RWXmScreen *   getXmScreen                    (Screen *);

#endif

// INTERNAL View.h++ FUNCTIONS
  void           setWidget                    (Widget);
  Widget         getWidget                    ();
  void           setBufferResources           (Boolean);
  Boolean        getBufferResources           () { return bufferResources; }
  void           beginWidget                  (RWController *, RWResourceObj * ro = 0);
  void           concludeWidget               (Boolean);

  RWController*  theParent;      // View Parent widget
  RWController*  getTheParent    () { return theParent; }

  virtual void   setFontList (RWFontList *) {;}

  Arg * getArgs   () const { return &sdargs; }
  int   getArgCnt () const { return sargcnt; }

  virtual RWCallback * getCallbackObject      (RWXtPointer);

  RWCallback * getDSCallbackObject    (RWXtPointer);

  // Set pointer from widget to RWController
  void         setBackpointer    ();

// RW TOOLS MEMBER FUNCTIONS
  virtual RWClassID      isA                    () const {return __RWCONTROLLER;}
	 
 protected:
  RWDynamicArgArray sdargs;  // Setfns dynamic resource argument array
  int               sargcnt; // Count of set resource arguments
  RWDynamicArgArray gdargs;  // Getfns dynamic resource argument array
  int               gargcnt; // Count of get resource arguments

  Boolean           bufferResources;   // Are resources buffered? (True/False)
  Widget            widget;            // Motif Widget

  Boolean checkResourceBufferSize(int);

  // Return the endUserData data member
  RWXtPointer getEndUserData       ()              { return endUserData; }

  // Set the endUserData data member
  void      setEndUserData       (RWXtPointer eud) { endUserData = eud; }


  

 private:
  RWXtPointer endUserData;     // User data resource

  void              createResourceBuffer(unsigned);
  RWCallbackCB *    getCBData(char*, RWCallbackCB&); 
  RWMethodCB *      getCBData(char*, RWMethodCB&);
  RWEventCB *       getCBData(EventMask, RWEventCB&);
  RWEventMethodCB * getCBData(EventMask, RWEventMethodCB&);
  void              removeCBListClientData(XtCallbackList);
  void              removeCBListClientData(char*);
};

#endif  /* End of file Control.h - Do not add beyond this point! */
