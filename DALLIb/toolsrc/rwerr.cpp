/*
 * Error routines.
 *
 * $Id: rwerr.cpp,v 6.9 1994/07/18 20:51:00 jims Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 *
 * (c) Copyright 1989, 1990, 1991, 1992, 1993, 1994 Rogue Wave Software, Inc.
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, Rogue Wave Software, Inc., which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 * Notwithstanding any other lease or license that may pertain to, or
 * accompany the delivery of, this computer software and information, the
 * rights of the Government regarding its use, reproduction and disclosure
 * are as set forth in Section 52.227-19 of the FARS Computer
 * Software-Restricted Rights clause.
 * 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 52.227-7013.
 * 
 * This computer software and information is distributed with "restricted
 * rights."  Use, duplication or disclosure is subject to restrictions as
 * set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial
 * Computer Software-Restricted Rights (April 1985)."  If the Clause at
 * 18-52.227-74 "Rights in Data General" is specified in the contract,
 * then the "Alternate III" clause applies.
 *
 ***************************************************************************
 *
 *
 ***************************************************************************
 *
 * $Log: rwerr.cpp,v $
 * Revision 6.9  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.8  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.7  1994/06/18  00:37:58  myersn
 * move virtual dtors out-of-line and explicit.
 *
 * Revision 6.6  1994/06/14  22:30:02  nevis
 * Exception simulation code always defined now.
 *
 * Revision 6.5  1994/06/10  18:33:19  vriezen
 * Remove non-const RWThrow functions.
 *
 * Revision 6.4  1994/06/10  17:43:04  nevis
 * Fixed the fix, maybe. Still need to find what xlC complains about...
 *
 * Revision 6.3  1994/06/10  15:54:51  nevis
 * Extended the visibility of rwSetErrHandler, corrected RWThrow
 * definitions.
 *
 * Revision 6.2  1994/06/09  19:45:36  nevis
 * Redefined RWThrow macro to deal with both const and nonconst
 * exception types, and to deal with shared library exception
 * throw bug.
 *
 * Revision 6.1  1994/04/15  19:07:36  vriezen
 * Move all files to 6.1
 *
 * Revision 3.7  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 3.6  1993/09/07  00:02:34  jims
 * Change "||" to "&&" in preprocessor check for Win16 DLL
 *
 * Revision 3.5  1993/09/06  21:48:09  jims
 * Use RWInstanceManager to make rwErrRoutine unique to each task
 * using Win16 DLL
 *
 * Revision 3.4  1993/08/04  22:02:51  jims
 * Use __WIN16__ || __WIN32__ instead of _Windows
 *
 * Revision 3.3  1993/06/25  17:51:25  north
 * added use of RW_GLOBAL_ENUMS
 *
 * Revision 3.2  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 3.1  1993/05/18  00:38:12  keffer
 * Rewrote to use new messaging and exception handling facility.
 *
 */

#include "rw/rwerr.h"
#include "rw/message.h"
#include "rw/coreerr.h"
STARTWRAP
#include <string.h>
ENDWRAP

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: rwerr.cpp,v $ $Revision: 6.9 $ $Date: 1994/07/18 20:51:00 $");

#if defined(_RWBUILDDLL) && defined(__WIN16__)
#include "rw/instmgr.h"
#endif

extern RWBoolean  rwerr_free_xmsg_ = TRUE;

void
rwerrFreeXmsg( RWBoolean  free_xmsg )
{ rwerr_free_xmsg_ = free_xmsg; }


//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                                RWxmsg                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifdef RW_NO_XMSG


    RWxmsg::RWxmsg(const char* msg)
    : doFree_(TRUE)
    {
      makeCopy(msg);
    }

    RWxmsg::RWxmsg(const RWMessage& msg)
    : doFree_(TRUE)
    {
      makeCopy(msg.str());
    }

    RWxmsg::RWxmsg(const RWxmsg& m)
    : doFree_(m.doFree_)
    {
      if(doFree_)
	makeCopy(m.msg_);
      else
	msg_ = m.msg_;
    }

    RWxmsg::~RWxmsg()
    {
      if (doFree_ && rwerr_free_xmsg_)
	delete (char*)msg_;
    }

    // Protected constructor:
    RWxmsg::RWxmsg(const char* msg, RWBoolean doCopy)
    : doFree_(doCopy)
    {
      if (doCopy)
	makeCopy(msg);
      else
	msg_ = msg;
    }


    void RWxmsg::makeCopy(const char* str)
    {
      if (rwerr_free_xmsg_) {
          char* tmp = new char[strlen(str)+1];
          strcpy(tmp, str);
          msg_ = tmp;
      } else {
          msg_ = "";
      }
    }

    void RWxmsg::raise() {RWTHROW(*this);}


#else	/* RW_NO_XMSG */

    RWxmsg::RWxmsg(const char* msg)
    : xmsg(msg)
    {;}

    RWxmsg::RWxmsg(const RWMessage& msg)
    : xmsg(msg.str())
    {;}

    RWxmsg::~RWxmsg() {}

#endif	/* RW_NO_XMSG */


//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                               RWxalloc                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifdef RW_NO_XMSG

    RWxalloc::RWxalloc(size_t size)
    : RWxmsg(rwMsgLookup(RWCORE_NOMEM), FALSE),
      size_(size)
    {;}

#else

    RWxalloc::RWxalloc(size_t size)
    : xalloc(rwMsgLookup(RWCORE_NOMEM), size)
    {;}

#endif	/* RW_NO_XMSG */

    RWxalloc::~RWxalloc() {}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                            RWExternalErr                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

RWExternalErr::RWExternalErr(const char* msg)
: RWxmsg(msg)
{;}

RWExternalErr::RWExternalErr(const RWMessage& msg)
: RWxmsg(msg)
{;}

RWExternalErr::~RWExternalErr() {}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              RWFileErr                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifdef RW_GLOBAL_ENUMS
RWFileErr::RWFileErr(const char* msg, FILE* fd,            errType type)
#else
RWFileErr::RWFileErr(const char* msg, FILE* fd, RWFileErr::errType type)
#endif
: RWExternalErr(msg),
  fd_(fd),
  type_(type)
{;}

#ifdef RW_GLOBAL_ENUMS
RWFileErr::RWFileErr(const RWMessage& msg, FILE* fd,            errType type)
#else
RWFileErr::RWFileErr(const RWMessage& msg, FILE* fd, RWFileErr::errType type)
#endif
: RWExternalErr(msg),
  fd_(fd),
  type_(type)
{;}

RWFileErr::~RWFileErr() {}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                             RWStreamErr                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

RWStreamErr::RWStreamErr(const char* msg, ios& stream)
: RWExternalErr(msg),
  stream_(stream)
{;}

RWStreamErr::RWStreamErr(const RWMessage& msg, ios& stream)
: RWExternalErr(msg),
  stream_(stream)
{;}

RWStreamErr::~RWStreamErr() {}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                             RWVstreamErr                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

RWVstreamErr::RWVstreamErr(const char* msg, RWios& stream)
: RWExternalErr(msg),
  stream_(stream)
{;}

RWVstreamErr::RWVstreamErr(const RWMessage& msg, RWios& stream)
: RWExternalErr(msg),
  stream_(stream)
{;}



//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                            RWInternalErr                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

RWInternalErr::RWInternalErr(const char* msg)
: RWxmsg(msg)
{;}


RWInternalErr::RWInternalErr(const RWMessage& msg)
: RWxmsg(msg)
{;}

RWInternalErr::~RWInternalErr() {}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                             RWBoundsErr                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

RWBoundsErr::RWBoundsErr(const char* msg)
: RWInternalErr(msg)
{;}


RWBoundsErr::RWBoundsErr(const RWMessage& msg)
: RWInternalErr(msg)
{;}

RWBoundsErr::~RWBoundsErr() {}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                     Code to simulate exceptions                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <stdlib.h>	/* Looking for exit() */

#if defined(__WIN16__) || defined(__WIN32__)

/************************************************
 *						*
 *	The default error handler; Windows	*
 *						*
 ************************************************/

#include <windows.h>

static void rwfar
rwDefaultErrHandler(const RWxmsg& err)
{
  MessageBox(NULL, err.why(), "Fatal Error", MB_TASKMODAL | MB_ICONSTOP | MB_OK);
  exit(1);
}

#else /* Not Windows */  

/************************************************
 *						*
 *	The default error handler; non-Windows	*
 *						*
 ************************************************/

static void rwfar
rwDefaultErrHandler(const RWxmsg& err)
{
  fputs("** "                          , stderr);
  fputs(err.why()                      , stderr);
  fputs("\n** Processing terminated.\n", stderr);
  exit(1);
}

#endif /* __WIN16__ || __WIN32__ */

#if defined(_RWBUILDDLL) && defined(__WIN16__)

/*
 * Maintain a unique Error Handler for each task using the DLL:
 */
class ErrHandlerManager : public RWInstanceManager
{
    // Redefined from RWInstanceManager; Supplies initial value:	
    virtual void rwfar*	newValue()
      { return (void rwfar*) new rwErrHandler(rwDefaultErrHandler); }

    // Redefined from RWInstanceManager; deletes value:
    virtual void deleteValue(void rwfar* p) { delete (rwErrHandler*) p; }
public:
    /*
     * Get current value.  If no currentValue, then this is a new task, so
     * initialize with default value and return that.
     */	
    rwErrHandler& getCurrentErrHandler()
      { rwErrHandler* eh = (rwErrHandler*) currentValue();
        if (eh)
          return *eh;
	else
          return *(rwErrHandler*) addValue();
      }
};

static ErrHandlerManager ehManager;

#define rwErrRoutine (ehManager.getCurrentErrHandler())

#else  /* Not building a Win16 DLL */

static rwErrHandler rwErrRoutine = rwDefaultErrHandler;

#endif /* defined(_RWBUILDDLL) && defined(__WIN16__) */

// BN: Moved rwSetErrHandler out of the block below for compilers
//     not supporting exceptions to that the test suite can
//     set a hander to be called in case the library was
//     compiled without exception support..

rwErrHandler rwexport
rwSetErrHandler(rwErrHandler routine)
{
  rwErrHandler temp = rwErrRoutine;
  rwErrRoutine = routine;
  return temp;
}


#if defined(RW_NO_EXCEPTIONS) || (defined(RW_NO_THROW_WITH_SHARED) && defined(_RWBUILDSHARED))

void rwexport RWThrow( const RWxmsg&        err) { (*rwErrRoutine)(err); }
void rwexport RWThrow( const RWxalloc&      err) { (*rwErrRoutine)(err); }
void rwexport RWThrow( const RWExternalErr& err) { (*rwErrRoutine)(err); }
void rwexport RWThrow( const RWFileErr&     err) { (*rwErrRoutine)(err); }
void rwexport RWThrow( const RWStreamErr&   err) { (*rwErrRoutine)(err); }
void rwexport RWThrow( const RWInternalErr& err) { (*rwErrRoutine)(err); }
void rwexport RWThrow( const RWBoundsErr&   err) { (*rwErrRoutine)(err); }

// Because of the way we hide the definition of these functions in
// rwerr.h, the only time the the compiler will know to call these
// nonconst methods are if
//   1. The compiler supports exceptions, and
//   2. RW_NO_THROW_WITH_SHARED is defined, and
//   3. _RWBUILDSHARED is defined.
// or
//   1. We do not support exceptions.
//
// Desired effect: turn nonconst throws into calls to the internal
// error routine which handles const.

#else	/* RW_NO_EXCEPTIONS || (RW_NO_THROW_WITH_SHARED && _RWBUILDSHARED) */

// We cast away const to preserve link compatibility at the call interface and
//  "catch" compatibility as well.
void rwexport RWThrow( const RWxmsg&        err) { throw (RWxmsg&) err; }
void rwexport RWThrow( const RWxalloc&      err) { throw (RWxalloc&) err; }
void rwexport RWThrow( const RWExternalErr& err) { throw (RWExternalErr&) err; }
void rwexport RWThrow( const RWFileErr&     err) { throw (RWFileErr&) err; }
void rwexport RWThrow( const RWStreamErr&   err) { throw (RWStreamErr&) err; }
void rwexport RWThrow( const RWInternalErr& err) { throw (RWInternalErr&) err; }
void rwexport RWThrow( const RWBoundsErr&   err) { throw (RWBoundsErr&) err; }


#endif	/* RW_NO_EXCEPTIONS || (RW_NO_THROW_WITH_SHARED && _RWBUILDSHARED) */



