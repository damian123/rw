#ifndef __RWSTOREMGR_H__
#define __RWSTOREMGR_H__

/*
 * RWStoreManager: returns a unique RWStoreTable for each thread.
 *
 * $Id: storemgr.h,v 6.2 1994/07/12 21:14:53 vriezen Exp $
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
 * $Log: storemgr.h,v $
 * Revision 6.2  1994/07/12  21:14:53  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:07:56  vriezen
 * Move all files to 6.1
 *
 * Revision 1.2  1993/08/05  01:07:10  jims
 * Use rwfar instead of FAR
 *
 * Revision 1.1  1993/04/12  12:11:57  jims
 * Initial revision
 *
 * Revision 2.1  1993/02/05  07:28:23  jims
 * Modified to use new version of Instance Manager
 *
 * Revision 2.0  1992/10/23  03:36:49  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.1   12 Nov 1991 15:39:38   keffer
 * 
 */

#include "rwstore.h"
#include "rw/instmgr.h"

class RWStoreManager : public RWInstanceManager {
public:
  RWStoreTable*		currentStoreTable();
  RWStoreTable*		newStoreTable();

  // Inherited from RWInstanceManager:
  virtual void rwfar*	newValue();
  virtual void          deleteValue(void rwfar*);
};


extern RWStoreManager rwStoreManager;

#endif	/* __RWSTOREMGR_H__ */
