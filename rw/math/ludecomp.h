#ifndef __RWLUDECOMP_H__
#define __RWLUDECOMP_H__

/***************************************************************************
 *
 * ludecomp.h
 *
 * $Id: //math/13/rw/math/ludecomp.h#1 $
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

#include <rw/mathdefs.h>
#include <rw/math/numtrait.h>

//---------------------------------------------------------------------------
//
//  class RWLUDecompMethod<T>
//
// This class encapsulates the methods used by the class RWGenFact<T> to
// compute the LU decomposition of a general matrix and use it to solve
// a linear system, compute determinates, inverses, and condition
// numbers.  The implementation of these methods are from the old public
// domain Fortran library Linpack.  They have been run through a Fortran
// to C++ translator and templatized.  If Math.h++ is being built as a
// part of LAPack.h++, then specializations of this class for types double,
// float, and DComplex are included that directly call the corresponding
// LAPack routines directly.
//
//----------------------------------------------------------------------------
template< class T >
class RWLUDecompMethod
{
public:
    /**************************************************************************
    *  getFactorization
    *
    *      Factors a T matrix by gaussian elimination.
    *
    *
    *      getFactorization is usually called by getConditionNumber, but it can be called
    *      directly with a saving in time if  rcond  is not needed.
    *      (time for getConditionNumber) = (1 + 9/n)*(time for getFactorization) .
    *
    *      on entry
    *
    *         a       T[lda*n]
    *                 the matrix to be factored stored in column-major format
    *
    *         lda     int
    *                 the leading dimension of the array  a .
    *
    *         n       int
    *                 the order of the matrix  a .
    *
    *      on return
    *
    *         a       an upper triangular matrix and the multipliers
    *                 which were used to obtain it.
    *                 the factorization can be written  a = l*u  where
    *                 l  is a product of permutation and unit lower
    *                 triangular matrices and  u  is upper triangular.
    *
    *         ipvt    int(n)
    *                 an int vector of pivot indices.
    *
    *  return info    int
    *                 = 0  normal value.
    *                 = k  if  u(k,k) .eq. 0.0 .  this is not an error
    *                      condition for this subroutine, but it does
    *                      indicate that dgesl or dgedi will divide by zero
    *                      if called.  use  rcond  in getConditionNumber for a reliable
    *                      indication of singularity.
    *
    *      linpack. this version dated 08/14/78 .
    *      cleve moler, university of new mexico, argonne national lab.
    *
    *      gaussian elimination with partial pivoting
    *
    */
    static int getFactorization(T* a, int lda, int n, RWPivotType* ipvt);


    /****************************************************************************
    *
    *  getConditionNumber
    *
    *      Factors a double precision matrix by gaussian elimination
    *      and estimates the condition of the matrix.
    *
    *      if  rcond  is not needed, getFactorization is slightly faster.
    *      to solve  a*x = b , follow getConditionNumber by dgesl.
    *      to compute  inverse(a)*c , follow getConditionNumber by dgesl.
    *      to compute  determinant(a) , follow getConditionNumber by dgedi.
    *      to compute  inverse(a) , follow getConditionNumber by dgedi.
    *
    *      on entry
    *
    *         a       double precision(lda, n)
    *                 the matrix to be factored.
    *
    *         lda     int
    *                 the leading dimension of the array  a .
    *
    *         n       int
    *                 the order of the matrix  a .
    *
    *      on return
    *
    *         a       an upper triangular matrix and the multipliers
    *                 which were used to obtain it.
    *                 the factorization can be written  a = l*u  where
    *                 l  is a product of permutation and unit lower
    *                 triangular matrices and  u  is upper triangular.
    *
    *         ipvt    int(n)
    *                 an int vector of pivot indices.
    *
    *         rcond   double precision
    *                 an estimate of the reciprocal condition of  a .
    *                 for the system  a*x = b , relative perturbations
    *                 in  a  and  b  of size  epsilon  may cause
    *                 relative perturbations in  x  of size  epsilon/rcond .
    *
    *                 if  rcond  is so small that the int expression
    *                            1.0 + rcond .eq. 1.0
    *                 is true, then  a  may be singular to working
    *                 precision.  in particular,  rcond  is zero  if
    *                 exact singularity is detected or the estimate
    *                 underflows.
    *
    *         z       double precision(n)
    *                 a work vector whose contents are usually unimportant.
    *
    *                 if  a  is close to a singular matrix, then  z  is
    *                 an approximate null vector in the sense that
    *                 norm(a*z) = rcond*norm(a)*norm(z) .
    *
    *      linpack. this version dated 08/14/78 .
    *      cleve moler, university of new mexico, argonne national lab.
    */

#if (defined(__SUNPRO_CC) && (__SUNPRO_CC == 0x520))
    static int getConditionNumber(T* a, int lda, int n, RWPivotType* ipvt,
                                  rw_numeric_traits<T>::norm_type& rcond,
                                  T* z);
#elif (defined(__SUNPRO_CC) && (__SUNPRO_CC >= 0x530))
    typedef typename rw_numeric_traits<T>::norm_type norm_type;
    static int getConditionNumber(T* a, int lda, int n, RWPivotType* ipvt,
                                  norm_type& rcond,
                                  T* z);
#else
    static int getConditionNumber(T* a, int lda, int n, RWPivotType* ipvt,
                                  typename rw_numeric_traits<T>::norm_type& rcond,
                                  T* z);
#endif

    /***************************************************************************
    *
    *  solveSystem
    *
    *      solves the system
    *      a * x = b  or  trans(a) * x = b
    *      using the factors computed by getConditionNumber or getFactorization.
    *
    *      on entry
    *
    *         a       double precision(lda, n)
    *                 the output from getConditionNumber or getFactorization.
    *
    *         lda     int
    *                 the leading dimension of the array  a .
    *
    *         n       int
    *                 the order of the matrix  a .
    *
    *         ipvt    int(n)
    *                 the pivot vector from getConditionNumber or getFactorization.
    *
    *         b       double precision(n)
    *                 the right hand side vector.
    *
    *         job     int
    *                 = 0         to solve  a*x = b ,
    *                 = nonzero   to solve  trans(a)*x = b  where
    *                             trans(a)  is the transpose.
    *
    *      on return
    *
    *         b       the solution vector  x .
    *
    *      error condition
    *
    *         a division by zero will occur if the input factor contains a
    *         zero on the diagonal.  technically this indicates singularity
    *
    *      linpack. this version dated 08/14/78 .
    *      cleve moler, university of new mexico, argonne national lab.
    *
    *      subroutines and functions
    *
    *      blas daxpy,ddot
    *
    */
    static void solveSystem(T* a, int lda, int n, RWPivotType* ipvt, T* b, int job);


    /**************************************************************************
    *
    *  determinateInverse
    *
    *      dgedi computes the determinant and inverse of a matrix
    *      using the factors computed by getConditionNumber or getFactorization.
    *
    *      on entry
    *
    *         a       double precision(lda, n)
    *                 the output from getConditionNumber or getFactorization.
    *
    *         lda     int
    *                 the leading dimension of the array  a .
    *
    *         n       int
    *                 the order of the matrix  a .
    *
    *         ipvt    int(n)
    *                 the pivot vector from getConditionNumber or getFactorization.
    *
    *         work    double precision(n)
    *                 work vector.  contents destroyed.
    *
    *         job     int
    *                 = 11   both determinant and inverse.
    *                 = 01   inverse only.
    *                 = 10   determinant only.
    *
    *      on return
    *
    *         a       inverse of original matrix if requested.
    *                 otherwise unchanged.
    *
    *         det
    *                 determinant of original matrix if requested.
    *                 otherwise not referenced.
    *                 determinant = det(1) * 10.0**det(2)
    *                 with  1.0 .le. dabs(det(1)) .lt. 10.0
    *                 or  det(1) .eq. 0.0 .
    *
    *      error condition
    *
    *         a division by zero will occur if the input factor contains
    *         a zero on the diagonal and the inverse is requested.
    *         it will not occur if the subroutines are called correctly
    *         and if getConditionNumber has set rcond .gt. 0.0 or getFactorization has set
    *         info .eq. 0 .
    *
    *      linpack. this version dated 08/14/78 .
    *      cleve moler, university of new mexico, argonne national lab.
    */
    static void determinantInverse(T* a, int lda, int n, RWPivotType* ipvt, T& det,
                                   T* work, int job);

};

//---------------------------------------------------------------------------
//
// RWLUDecompMethod specialized for type double
//
//----------------------------------------------------------------------------
RW_MATH_SPECIALIZE
class RW_MATH_GLOBAL RWLUDecompMethod<double>
{
public:
    static int getConditionNumber(double* a, int lda, int n, RWPivotType* ipvt,
                                  double& rcond, double* work);

    static int getFactorization(double* a, int lda, int n, RWPivotType* ipvt);

    static void solveSystem(double* a, int lda, int n, RWPivotType* ipvt, double* b, int job);

    static void determinantInverse(double* a, int lda, int n, RWPivotType* ipvt, double& det,
                                   double* work, int job);
};


//---------------------------------------------------------------------------
//
// RWLUDecompMethod specialized for type float
//
//----------------------------------------------------------------------------
RW_MATH_SPECIALIZE
class RW_MATH_GLOBAL RWLUDecompMethod<float>
{
public:
    static int getConditionNumber(float* a, int lda, int n, RWPivotType* ipvt,
                                  float& rcond, float* work);

    static int getFactorization(float* a, int lda, int n, RWPivotType* ipvt);

    static void solveSystem(float* a, int lda, int n, RWPivotType* ipvt, float* b, int job);

    static void determinantInverse(float* a, int lda, int n, RWPivotType* ipvt, float& det,
                                   float* work, int job);
};


//---------------------------------------------------------------------------
//
// RWLUDecompMethod specialized for type DComplex
//
//----------------------------------------------------------------------------
RW_MATH_SPECIALIZE
class RW_MATH_GLOBAL RWLUDecompMethod<DComplex>
{
public:
    static int getConditionNumber(DComplex* a, int lda, int n, RWPivotType* ipvt,
                                  double& rcond, DComplex* work);

    static int getFactorization(DComplex* a, int lda, int n, RWPivotType* ipvt);

    static void solveSystem(DComplex* a, int lda, int n, RWPivotType* ipvt, DComplex* b, int job);

    static void determinantInverse(DComplex* a, int lda, int n, RWPivotType* ipvt, DComplex& det,
                                   DComplex* work, int job);
};


#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/math/ludecomp.cc>
#endif

#endif // __RWLUDECOMP_H__
