#ifndef _RW_LAPACK_FUNCTIONS_
#define _RW_LAPACK_FUNCTIONS_

/*
 * This file contains functions-wrappers for the lapack functions declared in "rw/lapack.h"
 *
 * $Id: //lapack/13/rw/lapack/lapkfunc.h#1 $
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
 */

#include <rw/lapkdefs.h>

/*
 * Need to actually include dcomplex.h --- since DComplex may just
 * be a typedef we can't just forward declare it as a class.
 */
#include <rw/mathdefs.h>
#include <rw/dcomplex.h>
#include <rw/fcomplex.h>

// these are used in bandfct.cc:

double RW_LAPK_SYMBOLIC langb(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, DComplex* p5, const rw_lapack_int_t& p6, double* p7);
double RW_LAPK_SYMBOLIC langb(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, double* p5, const rw_lapack_int_t& p6, double* p7);
float RW_LAPK_SYMBOLIC langb(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, float* p5, const rw_lapack_int_t& p6, float* p7);

void RW_LAPK_SYMBOLIC gbtrf(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, DComplex* p5, const rw_lapack_int_t& p6, rw_lapack_int_t* p7, rw_lapack_int_t& p8);
void RW_LAPK_SYMBOLIC gbtrf(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, double* p5, const rw_lapack_int_t& p6, rw_lapack_int_t* p7, rw_lapack_int_t& p8);
void RW_LAPK_SYMBOLIC gbtrf(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, float* p5, const rw_lapack_int_t& p6, rw_lapack_int_t* p7, rw_lapack_int_t& p8);

void RW_LAPK_SYMBOLIC gbcon(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, DComplex* p5, const rw_lapack_int_t& p6, rw_lapack_int_t* p7, const double& p8, double& p9, DComplex* p10, void* p11, rw_lapack_int_t& p12);
void RW_LAPK_SYMBOLIC gbcon(const char& p1, rw_lapack_int_t& p2, rw_lapack_int_t& p3, rw_lapack_int_t& p4, double* p5, rw_lapack_int_t& p6, rw_lapack_int_t* p7, double& p8, double& p9, double* p10, void* p11, rw_lapack_int_t& p12);
void RW_LAPK_SYMBOLIC gbcon(const char& p1, rw_lapack_int_t& p2, rw_lapack_int_t& p3, rw_lapack_int_t& p4, float* p5, rw_lapack_int_t& p6, rw_lapack_int_t* p7, float& p8, float& p9, float* p10, void* p11, rw_lapack_int_t& p12);

void RW_LAPK_SYMBOLIC gbtrs(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, DComplex* p6, const rw_lapack_int_t& p7, rw_lapack_int_t* p8, DComplex* p9, const rw_lapack_int_t& p10, rw_lapack_int_t& p11);
void RW_LAPK_SYMBOLIC gbtrs(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, double* p6, const rw_lapack_int_t& p7, rw_lapack_int_t* p8, double* p9, const rw_lapack_int_t& p10, rw_lapack_int_t& p11);
void RW_LAPK_SYMBOLIC gbtrs(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, float* p6, const rw_lapack_int_t& p7, rw_lapack_int_t* p8, float* p9, const rw_lapack_int_t& p10, rw_lapack_int_t& p11);


// these are used in bandtd.cc:

void RW_LAPK_SYMBOLIC sbtrd(const char& p1, const char& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, DComplex* p5, const rw_lapack_int_t& p6, double* p7, double* p8, DComplex* p9, const rw_lapack_int_t& p10, DComplex* p11, rw_lapack_int_t& p12);
void RW_LAPK_SYMBOLIC sbtrd(const char& p1, const char& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, double* p5, const rw_lapack_int_t& p6, double* p7, double* p8, double* p9, const rw_lapack_int_t& p10, double* p11, rw_lapack_int_t& p12);
void RW_LAPK_SYMBOLIC sbtrd(const char& p1, const char& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, float*  p5, const rw_lapack_int_t& p6, float*  p7, float*  p8, float*  p9, const rw_lapack_int_t& p10, float*  p11, rw_lapack_int_t& p12);


// these are used in densetd.cc:

void RW_LAPK_SYMBOLIC sptrd(const char& p1, const rw_lapack_int_t& p2, DComplex* p3, double* p4, double* p5, DComplex* p6, rw_lapack_int_t& p7);
void RW_LAPK_SYMBOLIC sptrd(const char& p1, const rw_lapack_int_t& p2, double* p3, double* p4, double* p5, double* p6, rw_lapack_int_t& p7);
void RW_LAPK_SYMBOLIC sptrd(const char& p1, const rw_lapack_int_t& p2, float*  p3, float*  p4, float*  p5, float*  p6, rw_lapack_int_t& p7);


void RW_LAPK_SYMBOLIC opmtr(const char& p1, const char& p2, const char& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, DComplex* p6, DComplex* p7, DComplex* p8, const rw_lapack_int_t& p9, DComplex* p10, rw_lapack_int_t& p11);
void RW_LAPK_SYMBOLIC opmtr(const char& p1, const char& p2, const char& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, double* p6, double* p7, double* p8, const rw_lapack_int_t& p9, double* p10, rw_lapack_int_t& p11);
void RW_LAPK_SYMBOLIC opmtr(const char& p1, const char& p2, const char& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, float*  p6, float*  p7, float*  p8, const rw_lapack_int_t& p9, float*  p10, rw_lapack_int_t& p11);


// these are used in pdbdfct.cc:

double RW_LAPK_SYMBOLIC lansb(const char& p1, const char& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, DComplex* p5, const rw_lapack_int_t& p6, double* p7);
double RW_LAPK_SYMBOLIC lansb(const char& p1, const char& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, double* p5, const rw_lapack_int_t& p6, double* p7);
float RW_LAPK_SYMBOLIC lansb(const char& p1, const char& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, float* p5, const rw_lapack_int_t& p6, float* p7);


void RW_LAPK_SYMBOLIC pbtrf(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, DComplex* p4, const rw_lapack_int_t& p5, rw_lapack_int_t& p6);
void RW_LAPK_SYMBOLIC pbtrf(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, double* p4, const rw_lapack_int_t& p5, rw_lapack_int_t& p6);
void RW_LAPK_SYMBOLIC pbtrf(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, float* p4, const rw_lapack_int_t& p5, rw_lapack_int_t& p6);


void RW_LAPK_SYMBOLIC pbcon(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, DComplex* p4, const rw_lapack_int_t& p5, const double& p6, double& p7, DComplex* p8, double* p9, rw_lapack_int_t& p10);
void RW_LAPK_SYMBOLIC pbcon(const char& p1, rw_lapack_int_t& p2, rw_lapack_int_t& p3, double* p4, rw_lapack_int_t& p5, double& p6, double& p7, double* p8, rw_lapack_int_t* p9, rw_lapack_int_t& p10);
void RW_LAPK_SYMBOLIC pbcon(const char& p1, rw_lapack_int_t& p2, rw_lapack_int_t& p3, float* p4, rw_lapack_int_t& p5, float& p6, float& p7, float* p8, rw_lapack_int_t* p9, rw_lapack_int_t& p10);


void RW_LAPK_SYMBOLIC pbtrs(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, DComplex* p5, const rw_lapack_int_t& p6, DComplex* p7, const rw_lapack_int_t& p8, rw_lapack_int_t& p9);
void RW_LAPK_SYMBOLIC pbtrs(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, double*   p5, const rw_lapack_int_t& p6, double*   p7, const rw_lapack_int_t& p8, rw_lapack_int_t& p9);
void RW_LAPK_SYMBOLIC pbtrs(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, float*    p5, const rw_lapack_int_t& p6, float*    p7, const rw_lapack_int_t& p8, rw_lapack_int_t& p9);


// these are used in pdfct.cc:


void RW_LAPK_SYMBOLIC pptri(const char& p1, const rw_lapack_int_t& p2, DComplex* p3, rw_lapack_int_t& p4);

void RW_LAPK_SYMBOLIC pptri(const char& p1, const rw_lapack_int_t& p2, double* p3, rw_lapack_int_t& p4);

void RW_LAPK_SYMBOLIC pptri(const char& p1, const rw_lapack_int_t& p2, float* p3, rw_lapack_int_t& p4);


double RW_LAPK_SYMBOLIC lansp(const char& p1, const char& p2, const rw_lapack_int_t& p3, DComplex* p4, double* p5);
double RW_LAPK_SYMBOLIC lansp(const char& p1, const char& p2, const rw_lapack_int_t& p3, double* p4, double* p5);
float RW_LAPK_SYMBOLIC lansp(const char& p1, const char& p2, const rw_lapack_int_t& p3, float* p4, float* p5);

void RW_LAPK_SYMBOLIC pptrf(const char& p1, const rw_lapack_int_t& p2, DComplex* p3, rw_lapack_int_t& p4);

void RW_LAPK_SYMBOLIC pptrf(const char& p1, const rw_lapack_int_t& p2, double* p3, rw_lapack_int_t& p4);

void RW_LAPK_SYMBOLIC pptrf(const char& p1, const rw_lapack_int_t& p2, float* p3, rw_lapack_int_t& p4);


void RW_LAPK_SYMBOLIC ppcon(const char& p1, const rw_lapack_int_t& p2, DComplex* p3, const double& p4, double& p5, DComplex* p6, double* p7, rw_lapack_int_t& p8);

void RW_LAPK_SYMBOLIC ppcon(const char& p1, rw_lapack_int_t& p2, double* p3, double& p4, double& p5, double* p6, rw_lapack_int_t* p7, rw_lapack_int_t& p8);

void RW_LAPK_SYMBOLIC ppcon(const char& p1, rw_lapack_int_t& p2, float* p3, float& p4, float& p5, float* p6, rw_lapack_int_t* p7, rw_lapack_int_t& p8);

void RW_LAPK_SYMBOLIC pptrs(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, DComplex* p4, DComplex* p5, const rw_lapack_int_t& p6, rw_lapack_int_t& p7);

void RW_LAPK_SYMBOLIC pptrs(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, double* p4, double* p5, const rw_lapack_int_t& p6, rw_lapack_int_t& p7);

void RW_LAPK_SYMBOLIC pptrs(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, float* p4, float* p5, const rw_lapack_int_t& p6, rw_lapack_int_t& p7);


// these are used in pdtdfct.cc:

double RW_LAPK_SYMBOLIC lanst(const char& p1, const rw_lapack_int_t& p2, double* p3, DComplex* p4);
double RW_LAPK_SYMBOLIC lanst(const char& p1, const rw_lapack_int_t& p2, double* p3, double* p4);
float RW_LAPK_SYMBOLIC lanst(const char& p1, const rw_lapack_int_t& p2, float* p3, float* p4);

double RW_LAPK_SYMBOLIC lanht(const char& p1, const rw_lapack_int_t& p2, double* p3, DComplex* p4);

void RW_LAPK_SYMBOLIC pttrf(const rw_lapack_int_t& p1, double* p2, DComplex* p3, rw_lapack_int_t& p4);
void RW_LAPK_SYMBOLIC pttrf(const rw_lapack_int_t& p1, double* p2, double* p3, rw_lapack_int_t& p4);
void RW_LAPK_SYMBOLIC pttrf(const rw_lapack_int_t& p1, float* p2, float* p3, rw_lapack_int_t& p4);

void RW_LAPK_SYMBOLIC ptcon(const rw_lapack_int_t& p1, double* p2, DComplex* p3, const double& p4, double& p5, double* p6, rw_lapack_int_t& p7);
void RW_LAPK_SYMBOLIC ptcon(const rw_lapack_int_t& p1, double* p2, double* p3, const double& p4, double& p5, double* p6, rw_lapack_int_t& p7);
void RW_LAPK_SYMBOLIC ptcon(const rw_lapack_int_t& p1, float* p2, float* p3, const float& p4, float& p5, float* p6, rw_lapack_int_t& p7);


void RW_LAPK_SYMBOLIC pttrs(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, double* p4, DComplex* p5, DComplex* p6, const rw_lapack_int_t& p7, rw_lapack_int_t& p8);
void RW_LAPK_SYMBOLIC pttrs(const char& dummy, const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, double* p3, double* p4, double* p5, const rw_lapack_int_t& p6, rw_lapack_int_t& p7);
void RW_LAPK_SYMBOLIC pttrs(const char& dummy, const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, float*  p3, float*  p4, float*  p5, const rw_lapack_int_t& p6, rw_lapack_int_t& p7);


// these are used in symfct.cc:

/*
double RW_LAPK_SYMBOLIC lansp ( const char& p1,const char& p2,const rw_lapack_int_t& p3,DComplex* p4,double* p5 );
double RW_LAPK_SYMBOLIC lansp ( const char& p1,const char& p2,const rw_lapack_int_t& p3,double* p4,double* p5 );
float RW_LAPK_SYMBOLIC lansp ( const char& p1,const char& p2,const rw_lapack_int_t& p3,float* p4,float* p5 );
// defined above */

void RW_LAPK_SYMBOLIC sptrf(const char& p1, const rw_lapack_int_t& p2, DComplex* p3, rw_lapack_int_t* p4, rw_lapack_int_t& p5);

void RW_LAPK_SYMBOLIC sptrf(const char& p1, const rw_lapack_int_t& p2, double* p3, rw_lapack_int_t* p4, rw_lapack_int_t& p5);

void RW_LAPK_SYMBOLIC sptrf(const char& p1, const rw_lapack_int_t& p2, float* p3, rw_lapack_int_t* p4, rw_lapack_int_t& p5);


void RW_LAPK_SYMBOLIC spcon(const char& p1, const rw_lapack_int_t& p2, DComplex* p3, rw_lapack_int_t* p4, const double& p5, double& p6, DComplex* p7, rw_lapack_int_t* dummy, rw_lapack_int_t& p8);

void RW_LAPK_SYMBOLIC spcon(const char& p1, rw_lapack_int_t& p2, double* p3, rw_lapack_int_t* p4, double& p5, double& p6, double* p7, rw_lapack_int_t* p8, rw_lapack_int_t& p9);

void RW_LAPK_SYMBOLIC spcon(const char& p1, rw_lapack_int_t& p2, float* p3, rw_lapack_int_t* p4, float& p5, float& p6, float* p7, rw_lapack_int_t* p8, rw_lapack_int_t& p9);


void RW_LAPK_SYMBOLIC sptrs(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, DComplex* p4, rw_lapack_int_t* p5, DComplex* p6, const rw_lapack_int_t& p7, rw_lapack_int_t& p8);

void RW_LAPK_SYMBOLIC sptrs(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, double* p4, rw_lapack_int_t* p5, double* p6, const rw_lapack_int_t& p7, rw_lapack_int_t& p8);

void RW_LAPK_SYMBOLIC sptrs(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, float* p4, rw_lapack_int_t* p5, float* p6, const rw_lapack_int_t& p7, rw_lapack_int_t& p8);


void RW_LAPK_SYMBOLIC sptri(const char& p1, const rw_lapack_int_t& p2, DComplex* p3, rw_lapack_int_t* p4, DComplex* p5, rw_lapack_int_t& p6);

void RW_LAPK_SYMBOLIC sptri(const char& p1, const rw_lapack_int_t& p2, double* p3, rw_lapack_int_t* p4, double* p5, rw_lapack_int_t& p6);

void RW_LAPK_SYMBOLIC sptri(const char& p1, const rw_lapack_int_t& p2, float* p3, rw_lapack_int_t* p4, float* p5, rw_lapack_int_t& p6);


// these are used in trdgfct.cc:

double RW_LAPK_SYMBOLIC langt(const char& p1, const rw_lapack_int_t& p2, DComplex* p3, DComplex* p4, DComplex* p5);
double RW_LAPK_SYMBOLIC langt(const char& p1, const rw_lapack_int_t& p2, double*   p3, double*   p4, double* p5);
float RW_LAPK_SYMBOLIC langt(const char& p1, const rw_lapack_int_t& p2, float*    p3, float*    p4, float* p5);


void RW_LAPK_SYMBOLIC gttrf(const rw_lapack_int_t& p1, DComplex* p2, DComplex* p3, DComplex* p4, DComplex* p5, rw_lapack_int_t* p6, rw_lapack_int_t& p7);
void RW_LAPK_SYMBOLIC gttrf(const rw_lapack_int_t& p1, double*   p2, double*   p3, double*   p4, double*   p5, rw_lapack_int_t* p6, rw_lapack_int_t& p7);
void RW_LAPK_SYMBOLIC gttrf(const rw_lapack_int_t& p1, float*    p2, float*    p3, float*    p4, float*    p5, rw_lapack_int_t* p6, rw_lapack_int_t& p7);


void RW_LAPK_SYMBOLIC gtcon(const char& p1, const rw_lapack_int_t& p2, DComplex* p3, DComplex* p4, DComplex* p5, DComplex* p6, rw_lapack_int_t* p7, const double& p8, double& p9, DComplex* p10, rw_lapack_int_t* dummy, rw_lapack_int_t& p11);
void RW_LAPK_SYMBOLIC gtcon(const char& p1, rw_lapack_int_t& p2, double* p3, double* p4, double* p5, double* p6, rw_lapack_int_t* p7, double& p8, double& p9, double* p10, rw_lapack_int_t* p11, rw_lapack_int_t& p12);
void RW_LAPK_SYMBOLIC gtcon(const char& p1, rw_lapack_int_t& p2, float*  p3, float*  p4, float*  p5, float*  p6, rw_lapack_int_t* p7, float&  p8, float&  p9, float*  p10, rw_lapack_int_t* p11, rw_lapack_int_t& p12);


void RW_LAPK_SYMBOLIC gttrs(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, DComplex* p4, DComplex* p5, DComplex* p6, DComplex* p7, rw_lapack_int_t* p8, DComplex* p9, const rw_lapack_int_t& p10, rw_lapack_int_t& p11);
void RW_LAPK_SYMBOLIC gttrs(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, double*   p4, double*   p5, double*   p6, double*   p7, rw_lapack_int_t* p8, double*   p9, const rw_lapack_int_t& p10, rw_lapack_int_t& p11);
void RW_LAPK_SYMBOLIC gttrs(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, float*    p4, float*    p5, float*    p6, float*    p7, rw_lapack_int_t* p8, float*    p9, const rw_lapack_int_t& p10, rw_lapack_int_t& p11);


// these are used in seigbis.cc and heigbis.cc:

void RW_LAPK_SYMBOLIC stebz(const char& p1, const char& p2, const rw_lapack_int_t& p3, const double& p4, const double& p5, const rw_lapack_int_t& p6, const rw_lapack_int_t& p7, const double& p8, double* p9, double* p10, rw_lapack_int_t& p11, rw_lapack_int_t& p12, double* p13, rw_lapack_int_t* p14, rw_lapack_int_t* p15, double* p16, rw_lapack_int_t* p17, rw_lapack_int_t& p18);
void RW_LAPK_SYMBOLIC stebz(const char& p1, const char& p2, const rw_lapack_int_t& p3, const float&  p4, const float&  p5, const rw_lapack_int_t& p6, const rw_lapack_int_t& p7, const float&  p8, float*  p9, float*  p10, rw_lapack_int_t& p11, rw_lapack_int_t& p12, float*  p13, rw_lapack_int_t* p14, rw_lapack_int_t* p15, float*  p16, rw_lapack_int_t* p17, rw_lapack_int_t& p18);

void RW_LAPK_SYMBOLIC stein(const rw_lapack_int_t& p1, double* p2, double* p3, const rw_lapack_int_t& p4, double* p5, rw_lapack_int_t* p6, rw_lapack_int_t* p7, DComplex* p8, const rw_lapack_int_t& p9, double* p10, rw_lapack_int_t* p11, rw_lapack_int_t* p12, rw_lapack_int_t& p13);
void RW_LAPK_SYMBOLIC stein(const rw_lapack_int_t& p1, double* p2, double* p3, const rw_lapack_int_t& p4, double* p5, rw_lapack_int_t* p6, rw_lapack_int_t* p7, double*   p8, const rw_lapack_int_t& p9, double* p10, rw_lapack_int_t* p11, rw_lapack_int_t* p12, rw_lapack_int_t& p13);
void RW_LAPK_SYMBOLIC stein(const rw_lapack_int_t& p1, float*  p2, float*  p3, const rw_lapack_int_t& p4, float*  p5, rw_lapack_int_t* p6, rw_lapack_int_t* p7, float*    p8, const rw_lapack_int_t& p9, float*  p10, rw_lapack_int_t* p11, rw_lapack_int_t* p12, rw_lapack_int_t& p13);



// these are used in seigrf.cc and heigrf.cc:

void RW_LAPK_SYMBOLIC sterf(const rw_lapack_int_t& p1, double* p2, double* p3, rw_lapack_int_t& p4);
void RW_LAPK_SYMBOLIC sterf(const rw_lapack_int_t& p1, float*  p2, float*  p3, rw_lapack_int_t& p4);


// these are used in seigpd.cc and heigpd.cc:

//void RW_LAPK_SYMBOLIC pteqr ( const char& p1, const rw_lapack_int_t& p2, double* p3, double* p4, DComplex* p5, const rw_lapack_int_t& p6, double* p7, rw_lapack_int_t& p8 );
void RW_LAPK_SYMBOLIC pteqr(const char& p1, const rw_lapack_int_t& p2, double* p3, double* p4, double*   p5, const rw_lapack_int_t& p6, double* p7, rw_lapack_int_t& p8);
void RW_LAPK_SYMBOLIC pteqr(const char& p1, const rw_lapack_int_t& p2, float*  p3, float*  p4, float*    p5, const rw_lapack_int_t& p6, float*  p7, rw_lapack_int_t& p8);


// these are used in seigqr.cc and heigqr.cc:

//void RW_LAPK_SYMBOLIC steqr ( const char& p1, const rw_lapack_int_t& p2, double* p3, double* p4, DComplex* p5, const rw_lapack_int_t& p6, double* p7, rw_lapack_int_t& p8 );
void RW_LAPK_SYMBOLIC steqr(const char& p1, const rw_lapack_int_t& p2, double* p3, double* p4, double*   p5, const rw_lapack_int_t& p6, double* p7, rw_lapack_int_t& p8);
void RW_LAPK_SYMBOLIC steqr(const char& p1, const rw_lapack_int_t& p2, float*  p3, float*  p4, float*    p5, const rw_lapack_int_t& p6, float*  p7, rw_lapack_int_t& p8);


// these are used in qr.cc:


void RW_LAPK_SYMBOLIC ungqr(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, DComplex* p4, const rw_lapack_int_t& p5, DComplex* p6, DComplex* p7, const rw_lapack_int_t& p8, rw_lapack_int_t& p9);
void RW_LAPK_SYMBOLIC orgqr(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, double* p4, const rw_lapack_int_t& p5, double* p6, double* p7, const rw_lapack_int_t& p8, rw_lapack_int_t& p9);
void RW_LAPK_SYMBOLIC orgqr(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, float*  p4, const rw_lapack_int_t& p5, float*  p6, float*  p7, const rw_lapack_int_t& p8, rw_lapack_int_t& p9);
void RW_LAPK_SYMBOLIC orgqr(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, DComplex*  p4, const rw_lapack_int_t& p5, DComplex*  p6, DComplex*  p7, const rw_lapack_int_t& p8, rw_lapack_int_t& p9);


void RW_LAPK_SYMBOLIC trtrs(const char& p1, const char& p2, const char& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, DComplex* p6, const rw_lapack_int_t& p7, DComplex* p8, const rw_lapack_int_t& p9, rw_lapack_int_t& p10);
void RW_LAPK_SYMBOLIC trtrs(const char& p1, const char& p2, const char& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, double*   p6, const rw_lapack_int_t& p7, double*   p8, const rw_lapack_int_t& p9, rw_lapack_int_t& p10);
void RW_LAPK_SYMBOLIC trtrs(const char& p1, const char& p2, const char& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, float*    p6, const rw_lapack_int_t& p7, float*    p8, const rw_lapack_int_t& p9, rw_lapack_int_t& p10);


void RW_LAPK_SYMBOLIC mqr(const char& p1, const char& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, DComplex* p6, const rw_lapack_int_t& p7, DComplex* p8, DComplex* p9, const rw_lapack_int_t& p10, DComplex* p11, const rw_lapack_int_t& p12, rw_lapack_int_t& p13);
void RW_LAPK_SYMBOLIC mqr(const char& p1, const char& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, double* p6, const rw_lapack_int_t& p7, double* p8, double* p9, const rw_lapack_int_t& p10, double* p11, const rw_lapack_int_t& p12, rw_lapack_int_t& p13);
void RW_LAPK_SYMBOLIC mqr(const char& p1, const char& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, float*  p6, const rw_lapack_int_t& p7, float*  p8, float*  p9, const rw_lapack_int_t& p10, float*  p11, const rw_lapack_int_t& p12, rw_lapack_int_t& p13);


void RW_LAPK_SYMBOLIC geqpf(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, DComplex* p3, const rw_lapack_int_t& p4, rw_lapack_int_t* p5, DComplex* p6, DComplex* p7, double* p8, rw_lapack_int_t& p9);
void RW_LAPK_SYMBOLIC geqpf(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, double*   p3, const rw_lapack_int_t& p4, rw_lapack_int_t* p5, double*   p6, double*   p7, double* p8, rw_lapack_int_t& p9);
void RW_LAPK_SYMBOLIC geqpf(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, float*    p3, const rw_lapack_int_t& p4, rw_lapack_int_t* p5, float*    p6, float*    p7, double* p8, rw_lapack_int_t& p9);

void RW_LAPK_SYMBOLIC geqp3(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, float* p3, const rw_lapack_int_t& p4, rw_lapack_int_t* p5, float* p6,
                            float* p7, const rw_lapack_int_t& p8, rw_lapack_int_t& p9);

void RW_LAPK_SYMBOLIC geqp3(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, double* p3, const rw_lapack_int_t& p4, rw_lapack_int_t* p5, double* p6,
                            double* p7, const rw_lapack_int_t& p8, rw_lapack_int_t& p9);

void RW_LAPK_SYMBOLIC geqp3(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, DComplex* p3, const rw_lapack_int_t& p4, rw_lapack_int_t* p5,
                            DComplex* p6, DComplex* p7, const rw_lapack_int_t& p8, double* p9, rw_lapack_int_t& p10);

void RW_LAPK_SYMBOLIC geqrf(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, DComplex* p3, const rw_lapack_int_t& p4, DComplex* p5, DComplex* p6, const rw_lapack_int_t& p7, rw_lapack_int_t& p8);
void RW_LAPK_SYMBOLIC geqrf(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, double*   p3, const rw_lapack_int_t& p4, double*   p5, double*   p6, const rw_lapack_int_t& p7, rw_lapack_int_t& p8);
void RW_LAPK_SYMBOLIC geqrf(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, float*    p3, const rw_lapack_int_t& p4, float*    p5, float*    p6, const rw_lapack_int_t& p7, rw_lapack_int_t& p8);

// these are used in co.cc:

void RW_LAPK_SYMBOLIC latzm(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, DComplex* p4, const rw_lapack_int_t& p5, const DComplex& p6, DComplex* p7, DComplex* p8, const rw_lapack_int_t& p9, DComplex* p10);
void RW_LAPK_SYMBOLIC latzm(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, double*   p4, const rw_lapack_int_t& p5, const double&   p6, double*   p7, double*   p8, const rw_lapack_int_t& p9, double* p10);
void RW_LAPK_SYMBOLIC latzm(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, float*    p4, const rw_lapack_int_t& p5, const float&    p6, float*    p7, float*    p8, const rw_lapack_int_t& p9, float*  p10);

/* defined above
void RW_LAPK_SYMBOLIC trtrs (const char& p1,const char& p2,const char& p3,const rw_lapack_int_t& p4,const rw_lapack_int_t& p5,DComplex* p6,const rw_lapack_int_t& p7,DComplex* p8,const rw_lapack_int_t& p9,rw_lapack_int_t& p10);
void RW_LAPK_SYMBOLIC trtrs (const char& p1,const char& p2,const char& p3,const rw_lapack_int_t& p4,const rw_lapack_int_t& p5,double*   p6,const rw_lapack_int_t& p7,double*   p8,const rw_lapack_int_t& p9,rw_lapack_int_t& p10);
void RW_LAPK_SYMBOLIC trtrs (const char& p1,const char& p2,const char& p3,const rw_lapack_int_t& p4,const rw_lapack_int_t& p5,float*    p6,const rw_lapack_int_t& p7,float*    p8,const rw_lapack_int_t& p9,rw_lapack_int_t& p10);
*/

void RW_LAPK_SYMBOLIC tzrqf(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, DComplex* p3, const rw_lapack_int_t& p4, DComplex* p5, rw_lapack_int_t& p6);
void RW_LAPK_SYMBOLIC tzrqf(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, double*   p3, const rw_lapack_int_t& p4, double*   p5, rw_lapack_int_t& p6);
void RW_LAPK_SYMBOLIC tzrqf(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, float*    p3, const rw_lapack_int_t& p4, float*    p5, rw_lapack_int_t& p6);


//these are used in svdcalc.cc and svddccalc.cc:
void RW_LAPK_SYMBOLIC gesdd(const char& jobz, const rw_lapack_int_t& m, const rw_lapack_int_t& n, double* a,
                            const rw_lapack_int_t& lda, double* s, double* u, const rw_lapack_int_t& ldu,
                            double* vt, const rw_lapack_int_t& ldvt, double* work, const rw_lapack_int_t& lwork,
                            rw_lapack_int_t* iwork, rw_lapack_int_t& info);

void RW_LAPK_SYMBOLIC gesdd(const char& jobz, const rw_lapack_int_t& m, const rw_lapack_int_t& n, float* a,
                            const rw_lapack_int_t& lda, float* s, float* u, const rw_lapack_int_t& ldu,
                            float* vt, const rw_lapack_int_t& ldvt, float* work, const rw_lapack_int_t& lwork,
                            rw_lapack_int_t* iwork, rw_lapack_int_t& info);

void RW_LAPK_SYMBOLIC gesdd(const char& jobz, const rw_lapack_int_t& m, const rw_lapack_int_t& n, DComplex* a,
                            const rw_lapack_int_t& lda, double* s, DComplex* u, const rw_lapack_int_t& ldu,
                            DComplex* vt, const rw_lapack_int_t& ldvt, DComplex* work, const rw_lapack_int_t& lwork,
                            double* rwork, rw_lapack_int_t* iwork, rw_lapack_int_t& info);

void RW_LAPK_SYMBOLIC gebrd(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, DComplex* p3, const rw_lapack_int_t& p4, double* p5, double* p6, DComplex* p7, DComplex* p8, DComplex* p9, const rw_lapack_int_t& p10, rw_lapack_int_t& p11);
void RW_LAPK_SYMBOLIC gebrd(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, double*   p3, const rw_lapack_int_t& p4, double* p5, double* p6, double*   p7, double*   p8, double*   p9, const rw_lapack_int_t& p10, rw_lapack_int_t& p11);
void RW_LAPK_SYMBOLIC gebrd(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, float*    p3, const rw_lapack_int_t& p4, float*  p5, float*  p6, float*    p7, float*    p8, float*    p9, const rw_lapack_int_t& p10, rw_lapack_int_t& p11);

void RW_LAPK_SYMBOLIC bdsqr(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, double* p6, double* p7, DComplex* p8, const rw_lapack_int_t& p9, DComplex* p10, const rw_lapack_int_t& p11, DComplex* p12, const rw_lapack_int_t& p13, double*  p14, rw_lapack_int_t& p15);
void RW_LAPK_SYMBOLIC bdsqr(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, double* p6, double* p7, double*   p8, const rw_lapack_int_t& p9, double*   p10, const rw_lapack_int_t& p11, double*   p12, const rw_lapack_int_t& p13, double*  p14, rw_lapack_int_t& p15);
void RW_LAPK_SYMBOLIC bdsqr(const char& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, float*  p6, float*  p7, float*    p8, const rw_lapack_int_t& p9, float*    p10, const rw_lapack_int_t& p11, float*    p12, const rw_lapack_int_t& p13, float*   p14, rw_lapack_int_t& p15);

void RW_LAPK_SYMBOLIC mbr(const char& p1, const char& p2, const char& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, const rw_lapack_int_t& p6, DComplex* p7, const rw_lapack_int_t& p8, DComplex* p9, DComplex* p10, const rw_lapack_int_t& p11, DComplex* p12, const rw_lapack_int_t& p13, rw_lapack_int_t& p14);
void RW_LAPK_SYMBOLIC mbr(const char& p1, const char& p2, const char& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, const rw_lapack_int_t& p6, double*   p7, const rw_lapack_int_t& p8, double*   p9, double*   p10, const rw_lapack_int_t& p11, double*   p12, const rw_lapack_int_t& p13, rw_lapack_int_t& p14);
void RW_LAPK_SYMBOLIC mbr(const char& p1, const char& p2, const char& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, const rw_lapack_int_t& p6, float*    p7, const rw_lapack_int_t& p8, float*    p9, float*    p10, const rw_lapack_int_t& p11, float*    p12, const rw_lapack_int_t& p13, rw_lapack_int_t& p14);


//these are used in bal.cc:

void RW_LAPK_SYMBOLIC gebal(const char& p1, const rw_lapack_int_t& p2, DComplex* p3, const rw_lapack_int_t& p4, rw_lapack_int_t& p5, rw_lapack_int_t& p6, double* p7, rw_lapack_int_t& p8);
void RW_LAPK_SYMBOLIC gebal(const char& p1, const rw_lapack_int_t& p2, double*   p3, const rw_lapack_int_t& p4, rw_lapack_int_t& p5, rw_lapack_int_t& p6, double* p7, rw_lapack_int_t& p8);

void RW_LAPK_SYMBOLIC gebak(const char& p1, const char& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, double* p6, const rw_lapack_int_t& p7, DComplex* p8, const rw_lapack_int_t& p9, rw_lapack_int_t& p10);
void RW_LAPK_SYMBOLIC gebak(const char& p1, const char& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, double* p6, const rw_lapack_int_t& p7, double*   p8, const rw_lapack_int_t& p9, rw_lapack_int_t& p10);


//these are used in hess.cc:

void RW_LAPK_SYMBOLIC gehrd(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, DComplex* p4, const rw_lapack_int_t& p5, DComplex* p6, DComplex* p7, const rw_lapack_int_t& p8, rw_lapack_int_t& p9);
void RW_LAPK_SYMBOLIC gehrd(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, double*   p4, const rw_lapack_int_t& p5, double*   p6, double*   p7, const rw_lapack_int_t& p8, rw_lapack_int_t& p9);

void RW_LAPK_SYMBOLIC ghr(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, DComplex* p4, const rw_lapack_int_t& p5, DComplex* p6, DComplex* p7, const rw_lapack_int_t& p8, rw_lapack_int_t& p9);
void RW_LAPK_SYMBOLIC ghr(const rw_lapack_int_t& p1, const rw_lapack_int_t& p2, const rw_lapack_int_t& p3, double*   p4, const rw_lapack_int_t& p5, double*   p6, double*   p7, const rw_lapack_int_t& p8, rw_lapack_int_t& p9);

void RW_LAPK_SYMBOLIC mhr(const char& p1, const char& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, const rw_lapack_int_t& p6, DComplex* p7, const rw_lapack_int_t& p8, DComplex* p9, DComplex* p10, const rw_lapack_int_t& p11, DComplex* p12, const rw_lapack_int_t& p13, rw_lapack_int_t& p14);
void RW_LAPK_SYMBOLIC mhr(const char& p1, const char& p2, const rw_lapack_int_t& p3, const rw_lapack_int_t& p4, const rw_lapack_int_t& p5, const rw_lapack_int_t& p6, double*   p7, const rw_lapack_int_t& p8, double*   p9, double*   p10, const rw_lapack_int_t& p11, double*   p12, const rw_lapack_int_t& p13, rw_lapack_int_t& p14);

//these are used in schur.cc:

void RW_LAPK_SYMBOLIC trexc(const char& p1, const rw_lapack_int_t& p2, DComplex* p3, const rw_lapack_int_t& p4, DComplex* p5, const rw_lapack_int_t& p6, const rw_lapack_int_t& p7, const rw_lapack_int_t& p8, double* dm, rw_lapack_int_t& p9);
void RW_LAPK_SYMBOLIC trexc(const char& p1, const rw_lapack_int_t& p2, double*   p3, const rw_lapack_int_t& p4, double*   p5, const rw_lapack_int_t& p6, rw_lapack_int_t&       p7, rw_lapack_int_t&       p8, double* p9, rw_lapack_int_t& p10);

void RW_LAPK_SYMBOLIC hseqr(const char& job, const char& compz, const rw_lapack_int_t& n, const rw_lapack_int_t& ilo, const rw_lapack_int_t& ihi, DComplex* h__, const rw_lapack_int_t& ldh, DComplex w[], DComplex* z__, const rw_lapack_int_t& ldz, DComplex work[], const rw_lapack_int_t& lwork, rw_lapack_int_t& info);

void RW_LAPK_SYMBOLIC hsein(const char& side, const char& eigsrc, const char& initv, rw_lapack_select_t select[], rw_lapack_int_t& n, double* h__, rw_lapack_int_t& ldh, double wr[], double wi[], double* vl, rw_lapack_int_t& ldvl, double* vr, rw_lapack_int_t& ldvr, rw_lapack_int_t& mm, rw_lapack_int_t& m, double work[], rw_lapack_int_t ifaill[], rw_lapack_int_t ifailr[], rw_lapack_int_t& info);
void RW_LAPK_SYMBOLIC hsein(const char& side, const char& eigsrc, const char& initv, rw_lapack_select_t select[], const rw_lapack_int_t& n, DComplex* h__, const rw_lapack_int_t& ldh, DComplex w[], DComplex* vl, const rw_lapack_int_t& ldvl, DComplex* vr, const rw_lapack_int_t& ldvr, const rw_lapack_int_t& mm, rw_lapack_int_t& m, DComplex work[], double rwork[], rw_lapack_int_t ifaill[], rw_lapack_int_t ifailr[], rw_lapack_int_t& info);

void RW_LAPK_SYMBOLIC trsen(const char& job, const char& compq, rw_lapack_select_t select[], const rw_lapack_int_t& n, float* t, const rw_lapack_int_t& ldt, float* q, const rw_lapack_int_t& ldq, float wr[], float wi[], rw_lapack_int_t& m, float& s, float& sep, float work[], const rw_lapack_int_t& lwork, rw_lapack_int_t* iwork, const rw_lapack_int_t& liwork, rw_lapack_int_t& info);

void RW_LAPK_SYMBOLIC trsen(const char& job, const char& compq, rw_lapack_select_t select[], const rw_lapack_int_t& n, double* t, const rw_lapack_int_t& ldt, double* q, const rw_lapack_int_t& ldq, double wr[], double wi[], rw_lapack_int_t& m, double& s, double& sep, double work[], const rw_lapack_int_t& lwork, rw_lapack_int_t* iwork, const rw_lapack_int_t& liwork, rw_lapack_int_t& info);

void RW_LAPK_SYMBOLIC trsen(const char& job, const char& compq, rw_lapack_select_t select[], const rw_lapack_int_t& n, DComplex* t, const rw_lapack_int_t& ldt, DComplex* q, const rw_lapack_int_t& ldq, DComplex w[], rw_lapack_int_t& m, double& s, double& sep, DComplex work[], const rw_lapack_int_t& lwork, rw_lapack_int_t& info);

void RW_LAPK_SYMBOLIC trevc(const char& side, const char& howmny, rw_lapack_select_t select[], const rw_lapack_int_t& n, double* t, const rw_lapack_int_t& ldt, double* vl, const rw_lapack_int_t& ldvl, double* vr, const rw_lapack_int_t& ldvr, const rw_lapack_int_t& mm, rw_lapack_int_t& m, double work[], rw_lapack_int_t& info);
void RW_LAPK_SYMBOLIC trevc(const char& side, const char& howmny, rw_lapack_select_t select[], const rw_lapack_int_t& n, DComplex* t, const rw_lapack_int_t& ldt, DComplex* vl, const rw_lapack_int_t& ldvl, DComplex* vr, const rw_lapack_int_t& ldvr, const rw_lapack_int_t& mm, rw_lapack_int_t& m, DComplex work[], double rwork[], rw_lapack_int_t& info);

void RW_LAPK_SYMBOLIC hseqr(const char& job, const char& compz, const rw_lapack_int_t& n, const rw_lapack_int_t& ilo, const rw_lapack_int_t& ihi, double* h__, const rw_lapack_int_t& ldh, double wr[], double wi[], double* z__, const rw_lapack_int_t& ldz, double work[], const rw_lapack_int_t& lwork, rw_lapack_int_t& info);

double RW_LAPK_SYMBOLIC lanhp(const char& norm, const char& uplo, const rw_lapack_int_t& n, DComplex ap[], double work[]);

void RW_LAPK_SYMBOLIC hptrf(const char& uplo, const rw_lapack_int_t& n, DComplex ap[], rw_lapack_int_t ipiv[], rw_lapack_int_t& info);

void RW_LAPK_SYMBOLIC hpcon(const char& uplo, const rw_lapack_int_t& n, DComplex ap[], rw_lapack_int_t ipiv[], const double& anorm, double& rcond, DComplex work[], rw_lapack_int_t& info);

void RW_LAPK_SYMBOLIC hptrs(const char& uplo, const rw_lapack_int_t& n, const rw_lapack_int_t& nrhs, DComplex ap[], rw_lapack_int_t ipiv[], DComplex* b, const rw_lapack_int_t& ldb, rw_lapack_int_t& info);

void RW_LAPK_SYMBOLIC hptri(const char& uplo, const rw_lapack_int_t& n, DComplex ap[], rw_lapack_int_t ipiv[], DComplex work[], rw_lapack_int_t& info);

void RW_LAPK_SYMBOLIC trti2(const char& uplo, const char& diag, const rw_lapack_int_t& n, double* a, const rw_lapack_int_t& lda, rw_lapack_int_t& info);

void RW_LAPK_SYMBOLIC lauu2(const char& uplo, const rw_lapack_int_t& n, double* a, const rw_lapack_int_t& lda, rw_lapack_int_t& info);

void RW_LAPK_SYMBOLIC larfg(const rw_lapack_int_t& n, double& alpha, double x[], const rw_lapack_int_t& incx, double& tau);

// these are used in qr.cc, co.cc:

template <class TypeT>
class RW_LAPK_GLOBAL lapk_param_traits
{
public:
    static const char*  char_param;
    static const char* gqr_name;
    static const char* mqr_name;
    static const char* geqrf_name;
    static const char* gebrd_name;
    static const char* geqp3_name;
    static const char* gesdd_name;
    static const char* gehrd_name;
};

#if defined(_MSC_VER) && (_MSC_VER == 1500)
#  pragma warning (disable:4910)
#endif

RWLAPK_INSTANTIATE_1(class RW_LAPK_GLOBAL lapk_param_traits<double>);
RWLAPK_INSTANTIATE_1(class RW_LAPK_GLOBAL lapk_param_traits<float>);
RWLAPK_INSTANTIATE_1(class RW_LAPK_GLOBAL lapk_param_traits<DComplex>);

#if defined(_MSC_VER) && (_MSC_VER == 1500)
#  pragma warning(default:4910)
#endif

//these are used in qr.cc, sv.cc, hess.cc, pdtdfct.cc
inline int& real(int& val)
{
    return val;
}
inline float& real(float& val)
{
    return val;
}
inline double& real(double& val)
{
    return val;
}

inline int& conj(int& val)
{
    return val;
}
inline float& conj(float& val)
{
    return val;
}
inline double& conj(double& val)
{
    return val;
}

#endif
