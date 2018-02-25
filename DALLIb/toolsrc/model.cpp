/*
 * RWModel --- maintains a list of dependent clients
 *
 * $Id: model.cpp,v 6.5 1994/07/18 20:51:00 jims Exp $
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
 ***************************************************************************
 *
 * $Log: model.cpp,v $
 * Revision 6.5  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.4  1994/07/12  21:18:13  vriezen
 * Updated Copyright.
 *
 * Revision 6.3  1994/06/16  03:33:48  vriezen
 * Added outlined virtual dtors
 *
 * Revision 6.2  1994/04/15  21:08:12  vriezen
 * Backed out (until next release) RW_DEFINE_HOME and RW_DECLARE_HOME support.
 *
 * Revision 6.1  1994/04/15  19:07:18  vriezen
 * Move all files to 6.1
 *
 * Revision 2.3  1994/03/31  23:15:22  vriezen
 * Add RW_DECLARE_HOME and RW_DEFINE_HOME for class RWModelClient
 *
 * Revision 2.2  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.1  1992/11/16  04:24:00  keffer
 * Broke out RWModel functionality into RWModel and RWModelClient
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.1   18 Feb 1992 19:28:38   KEFFER
 * Suppressed bogus "possible unintended assignment" warning.
 * 
 *    Rev 1.0   17 Nov 1991 13:34:48   keffer
 * Initial revision.
 */

#include "rw/model.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: model.cpp,v $ $Revision: 6.5 $ $Date: 1994/07/18 20:51:00 $");

RWModel::RWModel()
{

}

void
RWModel::addDependent(RWModelClient* m)
{
  dependList.append(m);
}

void
RWModel::removeDependent(RWModelClient* m)
{
  dependList.remove(m);
}


void
RWModel::changed(void* pData)
{
  RWModelClient* m;
  RWOrderedIterator next(dependList);
  while ((m = (RWModelClient*)next())!=0) m->updateFrom(this, pData);
}

RWModel::~RWModel() { ; }

RWModelClient::~RWModelClient() { ; } 

