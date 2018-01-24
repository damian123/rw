#ifndef __RWBLAX_H__
#define __RWBLAX_H__
/***********************************************************************
 *
 * Prototypes for level 1,2,3 blas in float, double, FComplex, and DComplex
 * precisions.  Also includes prototypes for lsame() and xerbla().
 *
 * $Id: //math/13/rw/bla.h#1 $
 *
 * The Headerstop macro above contains the X to distinguish it from
 * the (now missing) math module wrapper for rw/rwbla.h.
 *
 **********************************************************************
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
 **********************************************************************/

/*
 * Need to actually include dcomplex.h --- since DComplex may just
 * be a typedef we can't just forward declare it as a class.
 */
#include <rw/mathdefs.h>
#include <rw/dcomplex.h>
#include <rw/fcomplex.h>

void RW_MATH_SYMBOLIC rw_caxpy(rw_lapack_int_t n , const FComplex* alpha , const FComplex* x , rw_lapack_int_t incx , FComplex* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_ccopy(rw_lapack_int_t n , const FComplex* x , rw_lapack_int_t incx , FComplex* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_cgbmv(char trans , rw_lapack_int_t m , rw_lapack_int_t n , rw_lapack_int_t kl , rw_lapack_int_t ku , const FComplex* alpha , const FComplex* a , rw_lapack_int_t lda , const FComplex* x , rw_lapack_int_t incx , const FComplex* beta , FComplex* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_cgemm(char transa , char transb , rw_lapack_int_t m , rw_lapack_int_t n , rw_lapack_int_t k , const FComplex* alpha , const FComplex* a , rw_lapack_int_t lda , const FComplex* b , rw_lapack_int_t ldb , const FComplex* beta , FComplex* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_cgemv(char trans , rw_lapack_int_t m , rw_lapack_int_t n , const FComplex* alpha , const FComplex* a , rw_lapack_int_t lda , const FComplex* x , rw_lapack_int_t incx , const FComplex* beta , FComplex* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_cgerc(rw_lapack_int_t m , rw_lapack_int_t n , const FComplex* alpha , const FComplex* x , rw_lapack_int_t incx , const FComplex* y , rw_lapack_int_t incy , FComplex* a , rw_lapack_int_t lda);
void RW_MATH_SYMBOLIC rw_cgeru(rw_lapack_int_t m , rw_lapack_int_t n , const FComplex* alpha , const FComplex* x , rw_lapack_int_t incx , const FComplex* y , rw_lapack_int_t incy , FComplex* a , rw_lapack_int_t lda);
void RW_MATH_SYMBOLIC rw_chbmv(char uplo , rw_lapack_int_t n , rw_lapack_int_t k , const FComplex* alpha , const FComplex* a , rw_lapack_int_t lda , const FComplex* x , rw_lapack_int_t incx , const FComplex* beta , FComplex* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_chemm(char side , char uplo , rw_lapack_int_t m , rw_lapack_int_t n , const FComplex* alpha , const FComplex* a , rw_lapack_int_t lda , const FComplex* b , rw_lapack_int_t ldb , const FComplex* beta , FComplex* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_chemv(char uplo , rw_lapack_int_t n , const FComplex* alpha , const FComplex* a , rw_lapack_int_t lda , const FComplex* x , rw_lapack_int_t incx , const FComplex* beta , FComplex* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_cher(char uplo , rw_lapack_int_t n , float alpha , const FComplex* x , rw_lapack_int_t incx , FComplex* a , rw_lapack_int_t lda);
void RW_MATH_SYMBOLIC rw_cher2(char uplo , rw_lapack_int_t n , const FComplex* alpha , const FComplex* x , rw_lapack_int_t incx , const FComplex* y , rw_lapack_int_t incy , FComplex* a , rw_lapack_int_t lda);
void RW_MATH_SYMBOLIC rw_cher2k(char uplo , char trans , rw_lapack_int_t n , rw_lapack_int_t k , const FComplex* alpha , const FComplex* a , rw_lapack_int_t lda , const FComplex* b , rw_lapack_int_t ldb , float beta , FComplex* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_cherk(char uplo , char trans , rw_lapack_int_t n , rw_lapack_int_t k , float alpha , const FComplex* a , rw_lapack_int_t lda , float beta , FComplex* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_chpmv(char uplo , rw_lapack_int_t n , const FComplex* alpha , const FComplex* ap , const FComplex* x , rw_lapack_int_t incx , const FComplex* beta , FComplex* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_chpr(char uplo , rw_lapack_int_t n , float alpha , const FComplex* x , rw_lapack_int_t incx , FComplex* ap);
void RW_MATH_SYMBOLIC rw_chpr2(char uplo , rw_lapack_int_t n , const FComplex* alpha , const FComplex* x , rw_lapack_int_t incx , const FComplex* y , rw_lapack_int_t incy , FComplex* ap);
void RW_MATH_SYMBOLIC rw_crotg(FComplex* a , const FComplex* b , float* c , FComplex* s);
void RW_MATH_SYMBOLIC rw_cscal(rw_lapack_int_t n , const FComplex* a , FComplex* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_csrot(rw_lapack_int_t n , FComplex* x , rw_lapack_int_t incx , FComplex* y , rw_lapack_int_t incy , float c , float s);
void RW_MATH_SYMBOLIC rw_csscal(rw_lapack_int_t n , float a , FComplex* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_cswap(rw_lapack_int_t n , FComplex* x , rw_lapack_int_t incx , FComplex* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_csymm(char side , char uplo , rw_lapack_int_t m , rw_lapack_int_t n , const FComplex* alpha , const FComplex* a , rw_lapack_int_t lda , const FComplex* b , rw_lapack_int_t ldb , const FComplex* beta , FComplex* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_csyr2k(char uplo , char trans , rw_lapack_int_t n , rw_lapack_int_t k , const FComplex* alpha , const FComplex* a , rw_lapack_int_t lda , const FComplex* b , rw_lapack_int_t ldb , const FComplex* beta , FComplex* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_csyrk(char uplo , char trans , rw_lapack_int_t n , rw_lapack_int_t k , const FComplex* alpha , const FComplex* a , rw_lapack_int_t lda , const FComplex* beta , FComplex* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_ctbmv(char uplo , char trans , char diag , rw_lapack_int_t n , rw_lapack_int_t k , const FComplex* a , rw_lapack_int_t lda , FComplex* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_ctbsv(char uplo , char trans , char diag , rw_lapack_int_t n , rw_lapack_int_t k , const FComplex* a , rw_lapack_int_t lda , FComplex* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_ctpmv(char uplo , char trans , char diag , rw_lapack_int_t n , const FComplex* ap , FComplex* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_ctpsv(char uplo , char trans , char diag , rw_lapack_int_t n , const FComplex* ap , FComplex* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_ctrmm(char side , char uplo , char transa , char diag , rw_lapack_int_t m , rw_lapack_int_t n , const FComplex* alpha , const FComplex* a , rw_lapack_int_t lda , FComplex* b , rw_lapack_int_t ldb);
void RW_MATH_SYMBOLIC rw_ctrmv(char uplo , char transa , char diag , rw_lapack_int_t n , const FComplex* a , rw_lapack_int_t lda , FComplex* b , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_ctrsm(char side , char uplo , char transa , char diag , rw_lapack_int_t m , rw_lapack_int_t n , const FComplex* alpha , const FComplex* a , rw_lapack_int_t lda , FComplex* b , rw_lapack_int_t ldb);
void RW_MATH_SYMBOLIC rw_ctrsv(char uplo , char trans , char diag , rw_lapack_int_t n , const FComplex* a , rw_lapack_int_t lda , FComplex* x , rw_lapack_int_t incx);
double RW_MATH_SYMBOLIC rw_dasum(rw_lapack_int_t n , const double* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_daxpy(rw_lapack_int_t n , double alpha , const double* x , rw_lapack_int_t incx , double* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_dcopy(rw_lapack_int_t n , const double* x , rw_lapack_int_t incx , double* y , rw_lapack_int_t incy);
double RW_MATH_SYMBOLIC rw_ddot(rw_lapack_int_t n , const double* x , rw_lapack_int_t incx , const double* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_dgbmv(char trans , rw_lapack_int_t m , rw_lapack_int_t n , rw_lapack_int_t kl , rw_lapack_int_t ku , double alpha , const double* a , rw_lapack_int_t lda , const double* x , rw_lapack_int_t incx , double beta , double* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_dgemm(char transa , char transb , rw_lapack_int_t m , rw_lapack_int_t n , rw_lapack_int_t k , double alpha , const double* a , rw_lapack_int_t lda , const double* b , rw_lapack_int_t ldb , double beta , double* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_dgemv(char trans , rw_lapack_int_t m , rw_lapack_int_t n , double alpha , const double* a , rw_lapack_int_t lda , const double* x , rw_lapack_int_t incx , double beta , double* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_dger(rw_lapack_int_t m , rw_lapack_int_t n , double alpha , const double* x , rw_lapack_int_t incx , const double* y , rw_lapack_int_t incy , double* a , rw_lapack_int_t lda);
double RW_MATH_SYMBOLIC rw_dnrm2(rw_lapack_int_t n , const double* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_drot(rw_lapack_int_t n , double* x , rw_lapack_int_t incx , double* y , rw_lapack_int_t incy , double c , double s);
void RW_MATH_SYMBOLIC rw_drotg(double* a , double* b , double* c , double* s);
void RW_MATH_SYMBOLIC rw_dsbmv(char uplo , rw_lapack_int_t n , rw_lapack_int_t k , double alpha , const double* a , rw_lapack_int_t lda , const double* x , rw_lapack_int_t incx , double beta , double* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_dscal(rw_lapack_int_t n , double a , double* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_dspmv(char uplo , rw_lapack_int_t n , double alpha , const double* ap , const double* x , rw_lapack_int_t incx , double beta , double* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_dspr(char uplo , rw_lapack_int_t n , double alpha , const double* x , rw_lapack_int_t incx , double* ap);
void RW_MATH_SYMBOLIC rw_dspr2(char uplo , rw_lapack_int_t n , double alpha , const double* x , rw_lapack_int_t incx , const double* y , rw_lapack_int_t incy , double* ap);
void RW_MATH_SYMBOLIC rw_dswap(rw_lapack_int_t n , double* x , rw_lapack_int_t incx , double* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_dsymm(char side , char uplo , rw_lapack_int_t m , rw_lapack_int_t n , double alpha , const double* a , rw_lapack_int_t lda , const double* b , rw_lapack_int_t ldb , double beta , double* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_dsymv(char uplo , rw_lapack_int_t n , double alpha , const double* a , rw_lapack_int_t lda , const double* x , rw_lapack_int_t incx , double beta , double* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_dsyr(char uplo , rw_lapack_int_t n , double alpha , const double* x , rw_lapack_int_t incx , double* a , rw_lapack_int_t lda);
void RW_MATH_SYMBOLIC rw_dsyr2(char uplo , rw_lapack_int_t n , double alpha , const double* x , rw_lapack_int_t incx , const double* y , rw_lapack_int_t incy , double* a , rw_lapack_int_t lda);
void RW_MATH_SYMBOLIC rw_dsyr2k(char uplo , char trans , rw_lapack_int_t n , rw_lapack_int_t k , double alpha , const double* a , rw_lapack_int_t lda , const double* b , rw_lapack_int_t ldb , double beta , double* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_dsyrk(char uplo , char trans , rw_lapack_int_t n , rw_lapack_int_t k , double alpha , const double* a , rw_lapack_int_t lda , double beta , double* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_dtbmv(char uplo , char trans , char diag , rw_lapack_int_t n , rw_lapack_int_t k , const double* a , rw_lapack_int_t lda , double* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_dtbsv(char uplo , char trans , char diag , rw_lapack_int_t n , rw_lapack_int_t k , const double* a , rw_lapack_int_t lda , double* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_dtpmv(char uplo , char trans , char diag , rw_lapack_int_t n , const double* ap , double* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_dtpsv(char uplo , char trans , char diag , rw_lapack_int_t n , const double* ap , double* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_dtrmm(char side , char uplo , char transa , char diag , rw_lapack_int_t m , rw_lapack_int_t n , double alpha , const double* a , rw_lapack_int_t lda , double* b , rw_lapack_int_t ldb);
void RW_MATH_SYMBOLIC rw_dtrmv(char uplo , char transa , char diag , rw_lapack_int_t n , const double* a , rw_lapack_int_t lda , double* b , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_dtrsm(char side , char uplo , char transa , char diag , rw_lapack_int_t m , rw_lapack_int_t n , double alpha , const double* a , rw_lapack_int_t lda , double* b , rw_lapack_int_t ldb);
void RW_MATH_SYMBOLIC rw_dtrsv(char uplo , char trans , char diag , rw_lapack_int_t n , const double* a , rw_lapack_int_t lda , double* x , rw_lapack_int_t incx);
double RW_MATH_SYMBOLIC rw_dzasum(rw_lapack_int_t n , const DComplex* x , rw_lapack_int_t incx);
double RW_MATH_SYMBOLIC rw_dznrm2(rw_lapack_int_t n , const DComplex* x , rw_lapack_int_t incx);
rw_lapack_int_t RW_MATH_SYMBOLIC rw_icamax(rw_lapack_int_t n , const FComplex* x , rw_lapack_int_t incx);
rw_lapack_int_t RW_MATH_SYMBOLIC rw_idamax(rw_lapack_int_t n , const double* x , rw_lapack_int_t incx);
rw_lapack_int_t RW_MATH_SYMBOLIC rw_isamax(rw_lapack_int_t n , const float* x , rw_lapack_int_t incx);
rw_lapack_int_t RW_MATH_SYMBOLIC rw_izamax(rw_lapack_int_t n , const DComplex* x , rw_lapack_int_t incx);
double RW_MATH_SYMBOLIC rw_sasum(rw_lapack_int_t n , const float* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_saxpy(rw_lapack_int_t n , float alpha , const float* x , rw_lapack_int_t incx , float* y , rw_lapack_int_t incy);
double RW_MATH_SYMBOLIC rw_scasum(rw_lapack_int_t n , const FComplex* x , rw_lapack_int_t incx);
double RW_MATH_SYMBOLIC rw_scnrm2(rw_lapack_int_t n , const FComplex* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_scopy(rw_lapack_int_t n , const float* x , rw_lapack_int_t incx , float* y , rw_lapack_int_t incy);
double RW_MATH_SYMBOLIC rw_sdot(rw_lapack_int_t n , const float* x , rw_lapack_int_t incx , const float* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_sgbmv(char trans , rw_lapack_int_t m , rw_lapack_int_t n , rw_lapack_int_t kl , rw_lapack_int_t ku , float alpha , const float* a , rw_lapack_int_t lda , const float* x , rw_lapack_int_t incx , float beta , float* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_sgemm(char transa , char transb , rw_lapack_int_t m , rw_lapack_int_t n , rw_lapack_int_t k , float alpha , const float* a , rw_lapack_int_t lda , const float* b , rw_lapack_int_t ldb , float beta , float* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_sgemv(char trans , rw_lapack_int_t m , rw_lapack_int_t n , float alpha , const float* a , rw_lapack_int_t lda , const float* x , rw_lapack_int_t incx , float beta , float* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_sger(rw_lapack_int_t m , rw_lapack_int_t n , float alpha , const float* x , rw_lapack_int_t incx , const float* y , rw_lapack_int_t incy , float* a , rw_lapack_int_t lda);
double RW_MATH_SYMBOLIC rw_snrm2(rw_lapack_int_t n , const float* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_srot(rw_lapack_int_t n , float* x , rw_lapack_int_t incx , float* y , rw_lapack_int_t incy , float c , float s);
void RW_MATH_SYMBOLIC rw_srotg(float* a , float* b , float* c , float* s);
void RW_MATH_SYMBOLIC rw_ssbmv(char uplo , rw_lapack_int_t n , rw_lapack_int_t k , float alpha , const float* a , rw_lapack_int_t lda , const float* x , rw_lapack_int_t incx , float beta , float* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_sscal(rw_lapack_int_t n , float a , float* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_sspmv(char uplo , rw_lapack_int_t n , float alpha , const float* ap , const float* x , rw_lapack_int_t incx , float beta , float* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_sspr(char uplo , rw_lapack_int_t n , float alpha , const float* x , rw_lapack_int_t incx , float* ap);
void RW_MATH_SYMBOLIC rw_sspr2(char uplo , rw_lapack_int_t n , float alpha , const float* x , rw_lapack_int_t incx , const float* y , rw_lapack_int_t incy , float* ap);
void RW_MATH_SYMBOLIC rw_sswap(rw_lapack_int_t n , float* x , rw_lapack_int_t incx , float* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_ssymm(char side , char uplo , rw_lapack_int_t m , rw_lapack_int_t n , float alpha , const float* a , rw_lapack_int_t lda , const float* b , rw_lapack_int_t ldb , float beta , float* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_ssymv(char uplo , rw_lapack_int_t n , float alpha , const float* a , rw_lapack_int_t lda , const float* x , rw_lapack_int_t incx , float beta , float* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_ssyr(char uplo , rw_lapack_int_t n , float alpha , const float* x , rw_lapack_int_t incx , float* a , rw_lapack_int_t lda);
void RW_MATH_SYMBOLIC rw_ssyr2(char uplo , rw_lapack_int_t n , float alpha , const float* x , rw_lapack_int_t incx , const float* y , rw_lapack_int_t incy , float* a , rw_lapack_int_t lda);
void RW_MATH_SYMBOLIC rw_ssyr2k(char uplo , char trans , rw_lapack_int_t n , rw_lapack_int_t k , float alpha , const float* a , rw_lapack_int_t lda , const float* b , rw_lapack_int_t ldb , float beta , float* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_ssyrk(char uplo , char trans , rw_lapack_int_t n , rw_lapack_int_t k , float alpha , const float* a , rw_lapack_int_t lda , float beta , float* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_stbmv(char uplo , char trans , char diag , rw_lapack_int_t n , rw_lapack_int_t k , const float* a , rw_lapack_int_t lda , float* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_stbsv(char uplo , char trans , char diag , rw_lapack_int_t n , rw_lapack_int_t k , const float* a , rw_lapack_int_t lda , float* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_stpmv(char uplo , char trans , char diag , rw_lapack_int_t n , const float* ap , float* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_stpsv(char uplo , char trans , char diag , rw_lapack_int_t n , const float* ap , float* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_strmm(char side , char uplo , char transa , char diag , rw_lapack_int_t m , rw_lapack_int_t n , float alpha , const float* a , rw_lapack_int_t lda , float* b , rw_lapack_int_t ldb);
void RW_MATH_SYMBOLIC rw_strmv(char uplo , char transa , char diag , rw_lapack_int_t n , const float* a , rw_lapack_int_t lda , float* b , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_strsm(char side , char uplo , char transa , char diag , rw_lapack_int_t m , rw_lapack_int_t n , float alpha , const float* a , rw_lapack_int_t lda , float* b , rw_lapack_int_t ldb);
void RW_MATH_SYMBOLIC rw_strsv(char uplo , char trans , char diag , rw_lapack_int_t n , const float* a , rw_lapack_int_t lda , float* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_xerbla(const char* srname , rw_lapack_int_t info);
void RW_MATH_SYMBOLIC rw_zaxpy(rw_lapack_int_t n , const DComplex* alpha , const DComplex* x , rw_lapack_int_t incx , DComplex* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_zcopy(rw_lapack_int_t n , const DComplex* x , rw_lapack_int_t incx , DComplex* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_zdotc(DComplex* pres , rw_lapack_int_t n , const DComplex* x , rw_lapack_int_t incx , const DComplex* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_zdotu(DComplex* pres , rw_lapack_int_t n , const DComplex* x , rw_lapack_int_t incx , const DComplex* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_zdrot(rw_lapack_int_t n , DComplex* x , rw_lapack_int_t incx , DComplex* y , rw_lapack_int_t incy , double c , double s);
void RW_MATH_SYMBOLIC rw_zdscal(rw_lapack_int_t n , double a , DComplex* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_zgbmv(char trans , rw_lapack_int_t m , rw_lapack_int_t n , rw_lapack_int_t kl , rw_lapack_int_t ku , const DComplex* alpha , const DComplex* a , rw_lapack_int_t lda , const DComplex* x , rw_lapack_int_t incx , const DComplex* beta , DComplex* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_zgemm(char transa , char transb , rw_lapack_int_t m , rw_lapack_int_t n , rw_lapack_int_t k , const DComplex* alpha , const DComplex* a , rw_lapack_int_t lda , const DComplex* b , rw_lapack_int_t ldb , const DComplex* beta , DComplex* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_zgemv(char trans , rw_lapack_int_t m , rw_lapack_int_t n , const DComplex* alpha , const DComplex* a , rw_lapack_int_t lda , const DComplex* x , rw_lapack_int_t incx , const DComplex* beta , DComplex* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_zgerc(rw_lapack_int_t m , rw_lapack_int_t n , const DComplex* alpha , const DComplex* x , rw_lapack_int_t incx , const DComplex* y , rw_lapack_int_t incy , DComplex* a , rw_lapack_int_t lda);
void RW_MATH_SYMBOLIC rw_zgeru(rw_lapack_int_t m , rw_lapack_int_t n , const DComplex* alpha , const DComplex* x , rw_lapack_int_t incx , const DComplex* y , rw_lapack_int_t incy , DComplex* a , rw_lapack_int_t lda);
void RW_MATH_SYMBOLIC rw_zhbmv(char uplo , rw_lapack_int_t n , rw_lapack_int_t k , const DComplex* alpha , const DComplex* a , rw_lapack_int_t lda , const DComplex* x , rw_lapack_int_t incx , const DComplex* beta , DComplex* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_zhemm(char side , char uplo , rw_lapack_int_t m , rw_lapack_int_t n , const DComplex* alpha , const DComplex* a , rw_lapack_int_t lda , const DComplex* b , rw_lapack_int_t ldb , const DComplex* beta , DComplex* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_zhemv(char uplo , rw_lapack_int_t n , const DComplex* alpha , const DComplex* a , rw_lapack_int_t lda , const DComplex* x , rw_lapack_int_t incx , const DComplex* beta , DComplex* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_zher(char uplo , rw_lapack_int_t n , double alpha , const DComplex* x , rw_lapack_int_t incx , DComplex* a , rw_lapack_int_t lda);
void RW_MATH_SYMBOLIC rw_zher2(char uplo , rw_lapack_int_t n , const DComplex* alpha , const DComplex* x , rw_lapack_int_t incx , const DComplex* y , rw_lapack_int_t incy , DComplex* a , rw_lapack_int_t lda);
void RW_MATH_SYMBOLIC rw_zher2k(char uplo , char trans , rw_lapack_int_t n , rw_lapack_int_t k , const DComplex* alpha , const DComplex* a , rw_lapack_int_t lda , const DComplex* b , rw_lapack_int_t ldb , double beta , DComplex* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_zherk(char uplo , char trans , rw_lapack_int_t n , rw_lapack_int_t k , double alpha , const DComplex* a , rw_lapack_int_t lda , double beta , DComplex* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_zhpmv(char uplo , rw_lapack_int_t n , const DComplex* alpha , const DComplex* ap , const DComplex* x , rw_lapack_int_t incx , const DComplex* beta , DComplex* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_zhpr(char uplo , rw_lapack_int_t n , double alpha , const DComplex* x , rw_lapack_int_t incx , DComplex* ap);
void RW_MATH_SYMBOLIC rw_zhpr2(char uplo , rw_lapack_int_t n , const DComplex* alpha , const DComplex* x , rw_lapack_int_t incx , const DComplex* y , rw_lapack_int_t incy , DComplex* ap);
void RW_MATH_SYMBOLIC rw_zrotg(DComplex* a , const DComplex* b , double* c , DComplex* s);
void RW_MATH_SYMBOLIC rw_zscal(rw_lapack_int_t n , const DComplex* a , DComplex* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_zswap(rw_lapack_int_t n , DComplex* x , rw_lapack_int_t incx , DComplex* y , rw_lapack_int_t incy);
void RW_MATH_SYMBOLIC rw_zsymm(char side , char uplo , rw_lapack_int_t m , rw_lapack_int_t n , const DComplex* alpha , const DComplex* a , rw_lapack_int_t lda , const DComplex* b , rw_lapack_int_t ldb , const DComplex* beta , DComplex* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_zsyr2k(char uplo , char trans , rw_lapack_int_t n , rw_lapack_int_t k , const DComplex* alpha , const DComplex* a , rw_lapack_int_t lda , const DComplex* b , rw_lapack_int_t ldb , const DComplex* beta , DComplex* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_zsyrk(char uplo , char trans , rw_lapack_int_t n , rw_lapack_int_t k , const DComplex* alpha , const DComplex* a , rw_lapack_int_t lda , const DComplex* beta , DComplex* c , rw_lapack_int_t ldc);
void RW_MATH_SYMBOLIC rw_ztbmv(char uplo , char trans , char diag , rw_lapack_int_t n , rw_lapack_int_t k , const DComplex* a , rw_lapack_int_t lda , DComplex* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_ztbsv(char uplo , char trans , char diag , rw_lapack_int_t n , rw_lapack_int_t k , const DComplex* a , rw_lapack_int_t lda , DComplex* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_ztpmv(char uplo , char trans , char diag , rw_lapack_int_t n , const DComplex* ap , DComplex* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_ztpsv(char uplo , char trans , char diag , rw_lapack_int_t n , const DComplex* ap , DComplex* x , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_ztrmm(char side , char uplo , char transa , char diag , rw_lapack_int_t m , rw_lapack_int_t n , const DComplex* alpha , const DComplex* a , rw_lapack_int_t lda , DComplex* b , rw_lapack_int_t ldb);
void RW_MATH_SYMBOLIC rw_ztrmv(char uplo , char transa , char diag , rw_lapack_int_t n , const DComplex* a , rw_lapack_int_t lda , DComplex* b , rw_lapack_int_t incx);
void RW_MATH_SYMBOLIC rw_ztrsm(char side , char uplo , char transa , char diag , rw_lapack_int_t m , rw_lapack_int_t n , const DComplex* alpha , const DComplex* a , rw_lapack_int_t lda , DComplex* b , rw_lapack_int_t ldb);
void RW_MATH_SYMBOLIC rw_ztrsv(char uplo , char trans , char diag , rw_lapack_int_t n , const DComplex* a , rw_lapack_int_t lda , DComplex* x , rw_lapack_int_t incx);

#endif
