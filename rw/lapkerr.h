#ifndef __RWLAPK_ERROR_H__
#define __RWLAPK_ERROR_H__

/*
 * Error messages for LAPACK.h++
 *
 * $Id: //math/13/rw/lapkerr.h#1 $
 *
 * Copyright (c) 1991-2015 Rogue Wave Software, Inc.  All Rights Reserved.
 * 
 * This computer software is owned by Rogue Wave Software, Inc. and is
 * protected by U.S. copyright laws and other laws and by international
 * treaties.  This computer software is furnished by Rogue Wave Software, Inc.
 * pursuant to a written license agreement and may be used, copied, transmitted,
 * and stored only in accordance with the terms of such license agreement and 
 * with the inclusion of the above copyright notice.  This computer software or
 * any other copies thereof may not be provided or otherwise made available to
 * any other person.
 * 
 * U.S. Government Restricted Rights.  This computer software: (a) was
 * developed at private expense and is in all respects the proprietary
 * information of Rogue Wave Software, Inc.; (b) was not developed with 
 * government funds; (c) is a trade secret of Rogue Wave Software, Inc. for all
 * purposes of the Freedom of Information Act; and (d) is a commercial item and
 * thus, pursuant to Section 12.212 of the Federal Acquisition Regulations (FAR)
 * and DFAR Supplement Section 227.7202, Government's use, duplication or
 * disclosure of the computer software is subject to the restrictions set forth
 * by Rogue Wave Software, Inc.
 *
 ***************************************************************************
 *
 * $Log: lapkerr.h,v $
 * Revision 1.2  1993/10/15 05:52:03  alv
 * updated for Math.h++ v5.1 error handling
 *
 *
 */

#include <rw/message.h>
#include <rw/lapkwind.h>

extern RW_LAPK_GLOBAL RWMsgId RWLAPK_BALANCE;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_BANDSIZE;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_CANTFREE;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_CANTFREEZE;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_CANTSOLVE;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_CANTSOLVELS;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_CHANGEDCOMPLEXCONSTANT;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_CHANGEDCONSTANT;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_CONDITION;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_DIAGOUTOFBOUNDS;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_LEADINGSUBMATRIXORDER;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_MATMATPROD;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_MATSIZE;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_MATVECPROD;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_NOEIG;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_NOSV;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_NOTHERM;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_NOTSETABLE;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_NOTSQUARE;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_NOTSYM;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_NUMBERPOINTS;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_NUMBERPOINTSBAND;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_OFFDIAG;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_OUTOFBOUNDS;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_QNOTCOMPUTED;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_RESTORESIZE;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_RSINGULAR;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_SCHURREORDER;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_VECLENGTH;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_VECMATPROD;
extern RW_LAPK_GLOBAL RWMsgId RWLAPK_WRONGNUMPOINTS;

int RW_LAPK_SYMBOLIC emptyLAPKErr(void);

#endif /* __RWLAPK_ERROR_H__ */
