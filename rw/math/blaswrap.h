/* CLAPACK 3.0 BLAS wrapper macros
 * Feb 5, 2000
 */

/***************************************************************************
 *
 * blaswrap.h
 *
 * $Id: //math/13/rw/math/blaswrap.h#1 $
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

#ifndef __BLASWRAP_H
#define __BLASWRAP_H

#include <rw/mathdefs.h>

//#ifndef NO_BLAS_WRAP
#if defined(RW_USING_MKL) && defined(_WIN32)

#  define xerbla_ xerbla
#  define lsame_  lsame

/* BLAS1 routines */
#  define srotg_ srotg
#  define drotg_ drotg
#  define srotmg_ srotmg
#  define drotmg_ drotmg
#  define srot_ srot
#  define drot_ drot
#  define srotm_ srotm
#  define drotm_ drotm
#  define sswap_ sswap
#  define dswap_ dswap
#  define cswap_ cswap
#  define zswap_ zswap
#  define sscal_ sscal
#  define dscal_ dscal
#  define cscal_ cscal
#  define zscal_ zscal
#  define csscal_ csscal
#  define zdscal_ zdscal
#  define scopy_ scopy
#  define dcopy_ dcopy
#  define ccopy_ ccopy
#  define zcopy_ zcopy
#  define saxpy_ saxpy
#  define daxpy_ daxpy
#  define caxpy_ caxpy
#  define zaxpy_ zaxpy
#  define sdot_ sdot
#  define ddot_ ddot
#  define cdotu_ cdotu
#  define zdotu_ zdotu
#  define cdotc_ cdotc
#  define zdotc_ zdotc
#  define snrm2_ snrm2
#  define dnrm2_ dnrm2
#  define scnrm2_ scnrm2
#  define dznrm2_ dznrm2
#  define sasum_ sasum
#  define dasum_ dasum
#  define scasum_ scasum
#  define dzasum_ dzasum
#  define isamax_ isamax
#  define idamax_ idamax
#  define icamax_ icamax
#  define izamax_ izamax

/* BLAS2 routines */
#  define sgemv_ sgemv
#  define dgemv_ dgemv
#  define cgemv_ cgemv
#  define zgemv_ zgemv
#  define sgbmv_ sgbmv
#  define dgbmv_ dgbmv
#  define cgbmv_ cgbmv
#  define zgbmv_ zgbmv
#  define chemv_ chemv
#  define zhemv_ zhemv
#  define chbmv_ chbmv
#  define zhbmv_ zhbmv
#  define chpmv_ chpmv
#  define zhpmv_ zhpmv
#  define ssymv_ ssymv
#  define dsymv_ dsymv
#  define ssbmv_ ssbmv
#  define dsbmv_ dsbmv
#  define sspmv_ sspmv
#  define dspmv_ dspmv
#  define strmv_ strmv
#  define dtrmv_ dtrmv
#  define ctrmv_ ctrmv
#  define ztrmv_ ztrmv
#  define stbmv_ stbmv
#  define dtbmv_ dtbmv
#  define ctbmv_ ctbmv
#  define ztbmv_ ztbmv
#  define stpmv_ stpmv
#  define dtpmv_ dtpmv
#  define ctpmv_ ctpmv
#  define ztpmv_ ztpmv
#  define strsv_ strsv
#  define dtrsv_ dtrsv
#  define ctrsv_ ctrsv
#  define ztrsv_ ztrsv
#  define stbsv_ stbsv
#  define dtbsv_ dtbsv
#  define ctbsv_ ctbsv
#  define ztbsv_ ztbsv
#  define stpsv_ stpsv
#  define dtpsv_ dtpsv
#  define ctpsv_ ctpsv
#  define ztpsv_ ztpsv
#  define sger_ sger
#  define dger_ dger
#  define cgeru_ cgeru
#  define zgeru_ zgeru
#  define cgerc_ cgerc
#  define zgerc_ zgerc
#  define cher_ cher
#  define zher_ zher
#  define chpr_ chpr
#  define zhpr_ zhpr
#  define cher2_ cher2
#  define zher2_ zher2
#  define chpr2_ chpr2
#  define zhpr2_ zhpr2
#  define ssyr_ ssyr
#  define dsyr_ dsyr
#  define sspr_ sspr
#  define dspr_ dspr
#  define ssyr2_ ssyr2
#  define dsyr2_ dsyr2
#  define sspr2_ sspr2
#  define dspr2_ dspr2

/* BLAS3 routines */
#  define sgemm_ sgemm
#  define dgemm_ dgemm
#  define cgemm_ cgemm
#  define zgemm_ zgemm
#  define ssymm_ ssymm
#  define dsymm_ dsymm
#  define csymm_ csymm
#  define zsymm_ zsymm
#  define chemm_ chemm
#  define zhemm_ zhemm
#  define ssyrk_ ssyrk
#  define dsyrk_ dsyrk
#  define csyrk_ csyrk
#  define zsyrk_ zsyrk
#  define cherk_ cherk
#  define zherk_ zherk
#  define ssyr2k_ ssyr2k
#  define dsyr2k_ dsyr2k
#  define csyr2k_ csyr2k
#  define zsyr2k_ zsyr2k
#  define cher2k_ cher2k
#  define zher2k_ zher2k
#  define strmm_ strmm
#  define dtrmm_ dtrmm
#  define ctrmm_ ctrmm
#  define ztrmm_ ztrmm
#  define strsm_ strsm
#  define dtrsm_ dtrsm
#  define ctrsm_ ctrsm
#  define ztrsm_ ztrsm

#endif /* NO_BLAS_WRAP */

#endif /* __BLASWRAP_H */
