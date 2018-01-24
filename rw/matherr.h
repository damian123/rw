#ifndef __RWMATH_ERROR_H__
#define __RWMATH_ERROR_H__

/***************************************************************************
 *
 * math_error.h - Error messages for Math.h++
 *
 * $Id: //math/13/rw/matherr.h#1 $
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
 **************************************************************************/

#include <rw/message.h>
#include <rw/mathwind.h>


#ifndef RW_NO_RECURSIVE_MACRO_DEF
#  define  RWMATH_ARRDIM      RWMATH_ARRDIM()
#  define  RWMATH_BADPARITY   RWMATH_BADPARITY()
#  define  RWMATH_BADVERNO    RWMATH_BADVERNO()
#  define  RWMATH_CANTSOLVE   RWMATH_CANTSOLVE()
#  define  RWMATH_INDEX       RWMATH_INDEX()
#  define  RWMATH_LENPOS      RWMATH_LENPOS()
#  define  RWMATH_MNMATCH     RWMATH_MNMATCH()
#  define  RWMATH_NMATCH      RWMATH_NMATCH()
#  define  RWMATH_NOCONDITION RWMATH_NOCONDITION()
#  define  RWMATH_NPOINTS     RWMATH_NPOINTS()
#  define  RWMATH_NSQUARE     RWMATH_NSQUARE()
#  define  RWMATH_RWBDEALL    RWMATH_RWBDEALL()
#  define  RWMATH_SHORTBLOCK  RWMATH_SHORTBLOCK()
#  define  RWMATH_SLICEBEG    RWMATH_SLICEBEG()
#  define  RWMATH_SLICEEND    RWMATH_SLICEEND()
#  define  RWMATH_STORAGE     RWMATH_STORAGE()
#  define  RWMATH_STRCTOR     RWMATH_STRCTOR()
#  define  RWMATH_STRIDEPOS   RWMATH_STRIDEPOS()
#  define  RWMATH_STRIDENZ    RWMATH_STRIDENZ()
#  define  RWMATH_STRIDEMAT   RWMATH_STRIDEMAT()
#  define  RWMATH_VNMATCH     RWMATH_VNMATCH()
#  define  RWMATH_ZEROSD      RWMATH_ZEROSD()
#  define  RWMATH_ZEROVEC     RWMATH_ZEROVEC()
#  define  RWMATH_ZROOT       RWMATH_ZROOT()
#  define  RWMATH_XERBLA_INVALIDPARAM RWMATH_RWMATH_XERBLA_INVALIDPARAM()
#else
#  define  RWMATH_ARRDIM      rwmath_ARRDIM()
#  define  RWMATH_BADPARITY   rwmath_BADPARITY()
#  define  RWMATH_BADVERNO    rwmath_BADVERNO()
#  define  RWMATH_CANTSOLVE   rwmath_CANTSOLVE()
#  define  RWMATH_INDEX       rwmath_INDEX()
#  define  RWMATH_LENPOS      rwmath_LENPOS()
#  define  RWMATH_MNMATCH     rwmath_MNMATCH()
#  define  RWMATH_NMATCH      rwmath_NMATCH()
#  define  RWMATH_NOCONDITION rwmath_NOCONDITION()
#  define  RWMATH_NPOINTS     rwmath_NPOINTS()
#  define  RWMATH_NSQUARE     rwmath_NSQUARE()
#  define  RWMATH_RWBDEALL    rwmath_RWBDEALL()
#  define  RWMATH_SHORTBLOCK  rwmath_SHORTBLOCK()
#  define  RWMATH_SLICEBEG    rwmath_SLICEBEG()
#  define  RWMATH_SLICEEND    rwmath_SLICEEND()
#  define  RWMATH_STORAGE     rwmath_STORAGE()
#  define  RWMATH_STRCTOR     rwmath_STRCTOR()
#  define  RWMATH_STRIDEPOS   rwmath_STRIDEPOS()
#  define  RWMATH_STRIDENZ    rwmath_STRIDENZ()
#  define  RWMATH_STRIDEMAT   rwmath_STRIDEMAT()
#  define  RWMATH_VNMATCH     rwmath_VNMATCH()
#  define  RWMATH_ZEROSD      rwmath_ZEROSD()
#  define  RWMATH_ZEROVEC     rwmath_ZEROVEC()
#  define  RWMATH_ZROOT       rwmath_ZROOT()
#  define  RWMATH_XERBLA_INVALIDPARAM rwmath_RWMATH_XERBLA_INVALIDPARAM()
#endif

#ifdef RW_TRAILING_RWEXPORT
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_ARRDIM;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_BADPARITY;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_BADVERNO;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_CANTSOLVE;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_INDEX;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_LENPOS;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_MNMATCH;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_NMATCH;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_NOCONDITION;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_NPOINTS;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_NSQUARE;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_RWBDEALL;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_SHORTBLOCK;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_SLICEBEG;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_SLICEEND;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_STORAGE;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_STRCTOR;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_STRIDEPOS;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_STRIDENZ;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_STRIDEMAT;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_VNMATCH;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_ZEROSD;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_ZEROVEC;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_ZROOT;
extern RWMsgId RW_MATH_SYMBOLIC RWMATH_XERBLA_INVALIDPARAM;
#else
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_ARRDIM;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_BADPARITY;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_BADVERNO;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_CANTSOLVE;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_INDEX;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_LENPOS;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_MNMATCH;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_NMATCH;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_NOCONDITION;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_NPOINTS;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_NSQUARE;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_RWBDEALL;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_SHORTBLOCK;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_SLICEBEG;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_SLICEEND;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_STORAGE;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_STRCTOR;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_STRIDEPOS;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_STRIDENZ;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_STRIDEMAT;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_VNMATCH;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_ZEROSD;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_ZEROVEC;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_ZROOT;
extern RW_MATH_SYMBOLIC RWMsgId RWMATH_XERBLA_INVALIDPARAM;
#endif // RW_TRAILING_RWEXPORT

#endif /* __RWMATH_ERROR_H__ */
