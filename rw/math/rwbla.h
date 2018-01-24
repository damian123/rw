#ifndef __RWBLA_H
#define __RWBLA_H

/***************************************************************************
 *
 * rwbla.h - Base classes for templatized bla functions
 *
 * $Id: //math/13/rw/math/rwbla.h#1 $
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
#include <rw/dcomplex.h>
#include <rw/math/numtrait.h>
#ifdef RW_MATH_NO_NEW_HEADER
#  include <stdio.h>
#  include <ctype.h>
#  include <math.h>
#  include <memory.h>
#  include <string.h>
#else
#  include <cstdio>
#  include <cctype>
#  include <cmath>
#  include <memory>
#  include <cstring>
#endif

#include <rw/bla.h>

template <class T>
class RWBlas_base_util
{
public:

    /****************************************************************
     *                                                              *
     *                           UTILITY                            *
     *                                                              *
     ****************************************************************/


    static  int lsame(const char& ca, const char& cb);

    static  void xerbla(const char* srname, const long& info);

    static  void set(size_t n, T* RW_RESTRICT_KEYWORD x, int incx,
                     const T* RW_RESTRICT_KEYWORD scalar);

    static  int same(size_t n, const T* RW_RESTRICT_KEYWORD x,
                     int incx, const T* RW_RESTRICT_KEYWORD y, int incy);

    /****************************************************************
     *                                                              *
     *              VECTOR --- VECTOR ASSIGNMENT OPERATIONS         *
     *                                                              *
     ****************************************************************/

    static  void aplvv(size_t n, T* RW_RESTRICT_KEYWORD x, int incx, const T* RW_RESTRICT_KEYWORD y, int incy);

    static  void amivv(size_t n, T* RW_RESTRICT_KEYWORD x, int incx, const T* RW_RESTRICT_KEYWORD y, int incy);

    static  void amuvv(size_t n, T* RW_RESTRICT_KEYWORD x, int incx, const T* RW_RESTRICT_KEYWORD y, int incy);

    static  void advvv(size_t n, T* RW_RESTRICT_KEYWORD x, int incx, const T* RW_RESTRICT_KEYWORD y, int incy);

    /****************************************************************
     *                                                              *
     *               VECTOR --- SCALAR ASSIGNMENT OPERATIONS        *
     *                                                              *
     ****************************************************************/

    static  void aplvs(size_t n, T* RW_RESTRICT_KEYWORD x, int incx,
                       const T* RW_RESTRICT_KEYWORD scalar);

    static  void amlvs(size_t n, T* RW_RESTRICT_KEYWORD x, int incx,
                       const T* RW_RESTRICT_KEYWORD scalar);

    static  void amuvs(size_t n, T* RW_RESTRICT_KEYWORD x, int incx,
                       const T* RW_RESTRICT_KEYWORD scalar);

    static  void advvs(size_t n, T* RW_RESTRICT_KEYWORD x, int incx,
                       const T* RW_RESTRICT_KEYWORD scalar);

    /****************************************************************
     *                                                              *
     *                  VECTOR --- VECTOR BINARY OPERATIONS         *
     *                                                              *
     ****************************************************************/

    static  void plvv(size_t n, T* RW_RESTRICT_KEYWORD z, const T* RW_RESTRICT_KEYWORD x, int incx, const T* RW_RESTRICT_KEYWORD y, int incy);

    static  void mivv(size_t n, T* RW_RESTRICT_KEYWORD z, const T* RW_RESTRICT_KEYWORD x, int incx, const T* RW_RESTRICT_KEYWORD y, int incy);

    static  void muvv(size_t n, T* RW_RESTRICT_KEYWORD z, const T* RW_RESTRICT_KEYWORD x, int incx, const T* RW_RESTRICT_KEYWORD y, int incy);

    static  void dvvv(size_t n, T* RW_RESTRICT_KEYWORD z, const T* RW_RESTRICT_KEYWORD x, int incx, const T* RW_RESTRICT_KEYWORD y, int incy);

    /****************************************************************
     *                                                              *
     *                 VECTOR --- SCALAR BINARY OPERATIONS          *
     *                                                              *
     ****************************************************************/

    static  void plvs(size_t n, T* RW_RESTRICT_KEYWORD z, const T* RW_RESTRICT_KEYWORD x, int incx, const T* RW_RESTRICT_KEYWORD scalar);

    static  void muvs(size_t n, T* RW_RESTRICT_KEYWORD z, const T* RW_RESTRICT_KEYWORD x, int incx, const T* RW_RESTRICT_KEYWORD scalar);

    static  void dvvs(size_t n, T* RW_RESTRICT_KEYWORD z, const T* RW_RESTRICT_KEYWORD x, int incx, const T* RW_RESTRICT_KEYWORD scalar);

    /****************************************************************
     *                                                              *
     *                    SCALAR --- VECTOR BINARY OPERATIONS       *
     *                                                              *
     ****************************************************************/

    static  void misv(size_t n, T* RW_RESTRICT_KEYWORD z, const T* scalar,
                      const T* RW_RESTRICT_KEYWORD x, int incx);

    static  void mivs(size_t n, T* RW_RESTRICT_KEYWORD z, const T* scalar,
                      const T* RW_RESTRICT_KEYWORD x, int incx);

    static  void dvsv(size_t n, T* RW_RESTRICT_KEYWORD z, const T* scalar,
                      const T* RW_RESTRICT_KEYWORD x, int incx);

}; // RWBlas_base_util<T>

template< class T >
class RWBlas_real_util : public RWBlas_base_util<T>
{
public:
    static  long imin(const long& n, const T* RW_RESTRICT_KEYWORD x, int incx);

    static  long imax(const long& n, const T* RW_RESTRICT_KEYWORD x, int incx);

    static  const T& (min)(const T& x, const T& y) {
        return(x < y ? x : y);
    }

    static  const T& (max)(const T& x, const T& y) {
        return(y < x ? x : y);
    }

    static  T sign(const T& a, const T& b);

}; // RWBlas_real_util<T>

template< class T >
class RWBlas_complex_util : public RWBlas_base_util<T>
{
public:
    static  T sign(const T& a, const T& b);

}; // RWBlas_complex_base<T>

template< class T >
class RWBlas_util : public RWBlas_real_util<T> {};

#if !defined(RW_MATH_NO_PARTIAL_SPEC)

template <class T>
class RWBlas_util<complex<T>>: public RWBlas_complex_util<complex<T>> {};

#else

RW_MATH_SPECIALIZE
class RWBlas_util<DComplex>: public RWBlas_complex_util<DComplex> {};

#  ifdef  RW_MATH_DEFINE_FCOMPLEX_BLAS
RW_MATH_SPECIALIZE
class RWBlas_util<FComplex>: public RWBlas_complex_util<FComplex> {};
#  endif

#endif

template< class T >
class RWBlas
{
public:
    typedef typename rw_numeric_traits<T>::norm_type             norm_type;

    static  void copy(const long& cn, const T* RW_RESTRICT_KEYWORD x,
                      const long& incx, T* RW_RESTRICT_KEYWORD y,
                      const long& incy);

    static  T dot(const long& cn, const T* RW_RESTRICT_KEYWORD x, const long& incx,
                  const T* RW_RESTRICT_KEYWORD y, const long& incy);

    static  long iamax(const long& n, const T* RW_RESTRICT_KEYWORD x, int incx);

    static  void scal(const long& n, const T& a, T* RW_RESTRICT_KEYWORD x,
                      const long& incx);

    static  void axpy(const long& n, const T& ca, T* RW_RESTRICT_KEYWORD x,
                      const long& incx, T* RW_RESTRICT_KEYWORD y, const long& incy);

    static void gemm(const char& transa, const char& transb, const long& m,
                     const long& n, const long& k, const T& alpha, T* a,
                     const long& lda, T* b, const long& ldb, const T& beta,
                     T* c, const long& ldc);

    static void gemv(const char& trans, const long& m, const long& n,
                     const T& alpha, T* a, const long& lda, T x[],
                     const long& incx, const T& beta,
                     T y[], const long& incy);

    static norm_type asum(const long& n, T* RW_RESTRICT_KEYWORD x,
                          const long& incx);

    static  void swap(const long& n, T* RW_RESTRICT_KEYWORD x, const long& incx,
                      T* RW_RESTRICT_KEYWORD y, const long& incy);

};

#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/math/rwbla.cc>
#endif

#include <rw/math/rwdbla.h>

#include <rw/math/rwsbla.h>

#include <rw/math/rwzbla.h>

#ifdef  RW_MATH_DEFINE_FCOMPLEX_BLAS
#  include <rw/math/rwcbla.h>
#endif

// Sunpro doesn't instantiate these by default, for some reason
//
#if defined(__SUNPRO_CC)

template class RWBlas_complex_util<DComplex>;
template class RWBlas_base_util<DComplex>;

#endif
#endif /* __RWBLAS_H */
