/*
 * Messaging facility
 *
 * $Id: message.cpp,v 6.4 1994/07/18 20:39:22 jims Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 * Voice: (503) 754-3010	FAX: (503) 757-6650
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
 * These routines rationalize the different messaging routines available.
 *
 ***************************************************************************
 *
 * $Log: message.cpp,v $
 * Revision 6.4  1994/07/18  20:39:22  jims
 * Fix typo
 *
 * Revision 6.3  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.2  1994/05/16  17:36:45  jims
 * Port to Win32 DLL
 *
 * Revision 6.1  1994/04/15  19:07:17  vriezen
 * Move all files to 6.1
 *
 * Revision 1.3  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.2  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 1.1  1993/05/18  00:45:50  keffer
 * Initial revision
 *
 *
 */

#include "rw/message.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: message.cpp,v $ $Revision: 6.4 $ $Date: 1994/07/18 20:39:22 $");

/* For sun, stdio.h must be included BEFORE stdarg.h;
   For Glock, it must be included AFTER stdarg.h. */


STARTWRAP
#ifndef __GLOCK__
# include <stdio.h>
#endif
#include <stdarg.h>
#ifdef __GLOCK__
# include <stdio.h>
#endif
#include <string.h>
ENDWRAP

#define RWXMSGBUFSIZE 504

// Typedefs are the easiest way to make more compilers happy
// about where the "rwexport" should be:
typedef ostream& ostreamRef;
typedef istream& istreamRef;

#if !defined(RW_MESSAGE) || (RW_MESSAGE==RW_NOMSG)
#ifdef RW_TRAILING_RWEXPORT
   const char* rwexport rwMsgLookup(RWMsgId msgId)
#else
   rwexport const char* rwMsgLookup(RWMsgId msgId)
#endif
   {
     return msgId->msg_;
   }
#endif


#if defined(RW_MESSAGE) && (RW_MESSAGE==RW_CATGETS)
#  include <nl_types.h>
   const char* rwexport rwMsgLookup(RWMsgId msgId)
   {
     nl_catd descriptor = ::catopen((char*)msgId->domainName_, 0);
     const char* ret = ::catgets(descriptor, 0, msgId->msgNumber_, (char*)msgId->msg_);
     ::catclose(descriptor);
     return ret;
   }
#endif

#if defined(RW_MESSAGE) && (RW_MESSAGE==RW_GETTEXT)
#  include <libintl.h>
   const char* rwexport rwMsgLookup(RWMsgId msgId)
   {
     ::textdomain(msgId->domainName_);
     return ::gettext(msgId->msg_);
   }
#endif


#if defined(RW_MESSAGE) && (RW_MESSAGE==RW_DGETTEXT)
#  include <libintl.h>
   const char* rwexport rwMsgLookup(RWMsgId msgId)
   {
     return ::dgettext(msgId->domainName_, msgId->msg_);
   }
#endif

RWMessage::RWMessage(const char* str)
{
  RWPRECONDITION(str!=0);
  msg_ = new char[strlen(str)+1];
  strcpy(msg_, str);
}

RWMessage::RWMessage(RWMsgId msgId ...)
{
  va_list arglist;
  va_start(arglist, msgId);

  char msgbuf[RWXMSGBUFSIZE];
  vsprintf(msgbuf, rwMsgLookup(msgId), arglist);

  // Sanity check:
  RWASSERT(strlen(msgbuf) < RWXMSGBUFSIZE);

  msg_ = new char[strlen(msgbuf)+1];
  strcpy(msg_, msgbuf);

#ifdef RW_MSC_BACKEND
  arglist = 0;
#else
  va_end(arglist);
#endif
}

RWMessage::RWMessage(const RWMessage& msg)
{
  RWPRECONDITION(msg.msg_!=0);
  msg_ = new char[strlen(msg.msg_)+1];
  strcpy(msg_, msg.msg_);
}

RWMessage::~RWMessage()
{
  delete msg_;
}

RWMessage&
RWMessage::operator=(const RWMessage& msg)
{
  if(this != &msg)
  {
    RWPRECONDITION(msg.msg_!=0);
    delete msg_;
    msg_ = new char[strlen(msg.msg_)+1];
    strcpy(msg_, msg.msg_);
  }
  return *this;
}
