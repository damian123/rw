#ifndef __RWBLA_CC
#define __RWBLA_CC__

// Borland does not have a const correct complex library.  e.g the +=
// operator is declared as complex::operator +=(complex &) instead
// of complex::operator += (const complex &).  Thus, when one of our
// functions that takes a const complex argument and passes that to
// a complex library function which doesn't want it to be const we get
// a "Temporary used for parameter" warning (the compiler makes a copy
// of the const parameter that is non-const and passes that to them
// complex routine.  We could, for the Borland case, just not have
// any complex parameters be declared const, but in the case of templates
// this would mean that no type of argument would be const.  This would
// make the library grossly const incorrect.  So, we have instead chosen
// to turn the warning off for selected files.


/***************************************************************************
 *
 * rwbla.cc - BLA-like extensions for type <T>
 *
 * $Id: //math/13/rw/math/rwbla.cc#1 $
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

#include <rw/math/rwbla.h>
#include <rw/math/funcobj.h>

/******************************** RWBlas_base_util ********************************/

template <class T>
int RWBlas_base_util<T>::lsame(const char& ca, const char& cb)
{
    return (RWMTH_STD::toupper(ca) == RWMTH_STD::toupper(cb));
}

template <class T>
void RWBlas_base_util<T>::xerbla(const char* srname, const long& info)
{
    rw_xerbla(srname, (rw_lapack_int_t)info);
}

template <class T>
void RWBlas_base_util<T>::set(size_t n, T* RW_RESTRICT_KEYWORD x, int incx,
                              const T* RW_RESTRICT_KEYWORD scalar)
{
    if (incx == 1)
        while (n--) {
            *x++ = *scalar;
        }
    else
        while (n--) {
            *x = *scalar;
            x += incx;
        }
}


template <class T>
int RWBlas_base_util<T>::same(size_t n, const T* RW_RESTRICT_KEYWORD x,
                              int incx, const T* RW_RESTRICT_KEYWORD y, int incy)
{
    while (n--) {
        if (*x != *y) {
            return 0;
        }
        x += incx;
        y += incy;
    }
    return 1;
}



template <class T>
void RWBlas_base_util<T>::aplvv(size_t n, T* RW_RESTRICT_KEYWORD x, int incx, const T* RW_RESTRICT_KEYWORD y,
                                int incy)
{
    while (n--) {
        *x += *y;
        x += incx;
        y += incy;
    }
}

template <class T>
void RWBlas_base_util<T>::amivv(size_t n, T* RW_RESTRICT_KEYWORD x, int incx, const T* RW_RESTRICT_KEYWORD y,
                                int incy)
{
    while (n--) {
        *x -= *y;
        x += incx;
        y += incy;
    }
}

template <class T>
void RWBlas_base_util<T>::amuvv(size_t n, T* RW_RESTRICT_KEYWORD x, int incx, const T* RW_RESTRICT_KEYWORD y,
                                int incy)
{
    while (n--) {
        *x = RW_STATIC_CAST(T, *x * *y);
        x += incx;
        y += incy;
    }
}

template <class T>
void RWBlas_base_util<T>::advvv(size_t n, T* RW_RESTRICT_KEYWORD x, int incx, const T* RW_RESTRICT_KEYWORD y,
                                int incy)
{
    while (n--) {
        *x = RW_STATIC_CAST(T, *x / *y);
        x += incx;
        y += incy;
    }
}



template <class T>
void RWBlas_base_util<T>::aplvs(size_t n, T* RW_RESTRICT_KEYWORD x, int incx,
                                const T* RW_RESTRICT_KEYWORD scalar)
{
    while (n--) {
        *x = RW_STATIC_CAST(T, *x + *scalar);
        x += incx;
    }
}

template <class T>
void RWBlas_base_util<T>::amlvs(size_t n, T* RW_RESTRICT_KEYWORD x, int incx,
                                const T* RW_RESTRICT_KEYWORD scalar)
{
    while (n--) {
        *x = RW_STATIC_CAST(T, *x - *scalar);
        x += incx;
    }
}

template <class T>
void RWBlas_base_util<T>::amuvs(size_t n, T* RW_RESTRICT_KEYWORD x, int incx,
                                const T* RW_RESTRICT_KEYWORD scalar)
{
    while (n--) {
        *x = RW_STATIC_CAST(T, *x * *scalar);
        x += incx;
    }
}

template <class T>
void RWBlas_base_util<T>::advvs(size_t n, T* RW_RESTRICT_KEYWORD x, int incx,
                                const T* RW_RESTRICT_KEYWORD scalar)
{
    while (n--) {
        *x = RW_STATIC_CAST(T, *x / *scalar);
        x += incx;
    }
}



template <class T>
void RWBlas_base_util<T>::plvv(size_t n, T* RW_RESTRICT_KEYWORD z, const T* RW_RESTRICT_KEYWORD x, int incx,
                               const T* RW_RESTRICT_KEYWORD y, int incy)
{
    while (n--) {
        *z++ = RW_STATIC_CAST(T, *x + *y);
        x += incx;
        y += incy;
    }
}

template <class T>
void RWBlas_base_util<T>::mivv(size_t n, T* RW_RESTRICT_KEYWORD z, const T* RW_RESTRICT_KEYWORD x, int incx,
                               const T* RW_RESTRICT_KEYWORD y, int incy)
{
    while (n--) {
        *z++ = RW_STATIC_CAST(T, *x - *y);
        x += incx;
        y += incy;
    }
}

template <class T>
void RWBlas_base_util<T>::muvv(size_t n, T* RW_RESTRICT_KEYWORD z, const T* RW_RESTRICT_KEYWORD x, int incx,
                               const T* RW_RESTRICT_KEYWORD y, int incy)
{
    while (n--) {
        *z++ = RW_STATIC_CAST(T, *x * *y);
        x += incx;
        y += incy;
    }
}

template <class T>
void RWBlas_base_util<T>::dvvv(size_t n, T* RW_RESTRICT_KEYWORD z, const T* RW_RESTRICT_KEYWORD x, int incx,
                               const T* RW_RESTRICT_KEYWORD y, int incy)
{
    while (n--) {
        *z++ = RW_STATIC_CAST(T, *x / *y);
        x += incx;
        y += incy;
    }
}



template <class T>
void RWBlas_base_util<T>::plvs(size_t n, T* RW_RESTRICT_KEYWORD z, const T* RW_RESTRICT_KEYWORD x, int incx,
                               const T* RW_RESTRICT_KEYWORD scalar)
{
    while (n--) {
        *z++ = RW_STATIC_CAST(T, *x + *scalar);
        x += incx;
    }
}

template <class T>
void RWBlas_base_util<T>::muvs(size_t n, T* RW_RESTRICT_KEYWORD z, const T* RW_RESTRICT_KEYWORD x, int incx,
                               const T* RW_RESTRICT_KEYWORD scalar)
{
    while (n--) {
        *z++ = RW_STATIC_CAST(T, *x * *scalar);
        x += incx;
    }
}

template <class T>
void RWBlas_base_util<T>::dvvs(size_t n, T* RW_RESTRICT_KEYWORD z, const T* RW_RESTRICT_KEYWORD x, int incx,
                               const T* RW_RESTRICT_KEYWORD scalar)
{
    while (n--) {
        *z++ = RW_STATIC_CAST(T, *x / *scalar);
        x += incx;
    }
}

template <class T>
void RWBlas_base_util<T>::misv(size_t n, T* RW_RESTRICT_KEYWORD z, const T* scalar,
                               const T* RW_RESTRICT_KEYWORD x, int incx)
{
    while (n--) {
        *z++ = RW_STATIC_CAST(T, *scalar - *x);
        x += incx;
    }
}

template <class T>
void RWBlas_base_util<T>::mivs(size_t n, T* RW_RESTRICT_KEYWORD z, const T* scalar,
                               const T* RW_RESTRICT_KEYWORD x, int incx)
{
    while (n--) {
        *z++ = RW_STATIC_CAST(T, *x - *scalar);
        x += incx;
    }
}

template <class T>
void RWBlas_base_util<T>::dvsv(size_t n, T* RW_RESTRICT_KEYWORD z, const T* scalar,
                               const T* RW_RESTRICT_KEYWORD x, int incx)
{
    while (n--) {
        *z++ = RW_STATIC_CAST(T, *scalar / *x);
        x += incx;
    }
}

/****************************** RWBlas_real_util *****************************/

template <class T>
long RWBlas_real_util<T>::imin(const long& n, const T* RW_RESTRICT_KEYWORD x, int incx)
{

    if (n < 1) {
        return -1;
    }
    if (n == 1) {
        return 0;
    }

    const T* ax = incx < 0 ? x - (n - 1) * incx : x;

    long ret_val;

    T themin;
    long i;
    for (ret_val = 0, themin = *ax, i = 0; i < n; ax += incx, ++i) {
        if (*ax < themin) {
            ret_val = i;
            themin = *ax;
        }
    }

    return ret_val;   /* Adjust for Fortran subscripting convention */

}

template <class T>
long RWBlas_real_util<T>::imax(const long& n, const T* RW_RESTRICT_KEYWORD x, int incx)
{

    if (n < 1) {
        return -1;
    }
    if (n == 1) {
        return 0;
    }

    const T* ax = incx < 0 ? x - (n - 1) * incx : x;

    long ret_val;
    T themax;
    long i;
    for (ret_val = 0, themax = *ax, i = 0; i < n; ax += incx, ++i) {
        if (*ax > themax) {
            ret_val = i;
            themax = *ax;
        }
    }

    return ret_val;   /* Adjust for Fortran subscripting convention */

}

template <class T>
T RWBlas_real_util<T>::sign(const T& a, const T& b)
{
    if (b >= 0) {
        return(a >= 0 ? a : -a);
    }
    else {
        return(a >= 0 ? -a : a);
    }

}

/**************************** RWBlas_complex_util ***************************/

template< class T >
T RWBlas_complex_util<T>::sign(const T& a, const T& b)
{
    return RW_abs<const T, T>()(a) * b / RW_abs<const T, T>()(b);
}


/********************************* RWBlas *********************************/

template <class T>
void RWBlas<T>::copy(const long& cn, const T* RW_RESTRICT_KEYWORD x, const long& incx,
                     T* RW_RESTRICT_KEYWORD y, const long& incy)
{
    long n = cn;

    if (n == 0) {
        return;
    }

    const T* ax = incx < 0 ? x - (n - 1) * incx : x;
    T* ay = incy < 0 ? y - (n - 1) * incy : y;

    while (n--) {
        *ay = *ax;
        ax += incx;
        ay += incy;
    }
}

template <class T>
T RWBlas<T>::dot(const long& cn, const T* RW_RESTRICT_KEYWORD x, const long& incx,
                 const T* RW_RESTRICT_KEYWORD y, const long& incy)
{
    T sum = (T)(0);

    T* ax = incx < 0 ? const_cast<T*>(x) - (cn - 1) * incx : const_cast<T*>(x);
    T* ay = incy < 0 ? const_cast<T*>(y) - (cn - 1) * incy : const_cast<T*>(y);

    long n = cn;
    while (n--) {
        sum += RW_STATIC_CAST(T, *ax * *ay);
        ax += incx;
        ay += incy;
    }
    return sum;
}

template <class T>
long RWBlas<T>::iamax(const long& n, const T* RW_RESTRICT_KEYWORD x, int incx)
{
    if (n < 1) {
        return -1;
    }
    if (n == 1) {
        return 0;
    }

    const T* ax = incx < 0 ? x - (n - 1) * incx : x;

    int ret_val;
    typename rw_numeric_traits<T>::norm_type themax;
    int i;
    for (ret_val = 0, themax = RW_abs<T, T>()(*ax), i = 0; i < n; ax += incx, ++i)
        if (RW_abs<T, T>()(*ax) > themax) {
            ret_val = i;
            themax = RW_abs<T, T>()(*ax);
        }

    return ret_val;   /* Adjust for Fortran subscripting convention */

}

template <class T>
void RWBlas<T>::scal(const long& n, const T& a, T* RW_RESTRICT_KEYWORD x,
                     const long& incx)
{
    T* ax = incx < 0 ? x - (n - 1) * incx : x;
    if (n > 0) {
        register int i;
        register T* ix;
        for (i = 0, ix = ax; i < n; ++i, ix += incx) {
            * ix *= a;
        }
    }
}

template <class T>
void RWBlas<T>::axpy(const long& n, const T& ca, T* RW_RESTRICT_KEYWORD x,
                     const long& incx, T* RW_RESTRICT_KEYWORD y, const long& incy)
{
    T* ax = incx < 0 ? x - (n - 1) * incx : x;
    T* ay = incy < 0 ? y - (n - 1) * incy : y;
    register int i;
    register T* ry;
    register T* rx;
    for (i = 0, ry = ay, rx = ax; i < n; ++i, ry += incy, rx += incx) {
        * ry += ca * *rx;
    }
}


template <class T>
typename RWBlas<T>::norm_type RWBlas<T>::asum(const long& n, T* RW_RESTRICT_KEYWORD x, const long& incx)
{
    T* ax = incx < 0 ? x - (n - 1) * incx : x;
    /*
     *  Local variables
     */

    int i, m;
    typename rw_numeric_traits<T>::norm_type dtemp;
    long nincx;

    /*
     *  Function Body
     */
    dtemp = 0;
    if (n <= 0) {
        return dtemp;
    }
    if (incx != 1) {

        /*
         *         CODE FOR INCREMENT NOT EQUAL TO 1
         */
        nincx = n * incx;
        for (i = 0; incx < 0 ? i > nincx : i < nincx; i += static_cast<int>(incx)) {
            dtemp += RW_abs<T, T>()(ax[i]);
        }
        return dtemp;
    }

    /*
     *         CODE FOR INCREMENT EQUAL TO 1
     *
     *
     *         CLEAN-UP LOOP
     */
    m = static_cast<int>(n % 6);

    for (i = 0; i < m; ++i) {
        dtemp += RW_abs<T, T>()(ax[i]);
    }
    for (i = m; i < n; i += 6)
        dtemp += RW_abs<T, T>()(ax[i]) + RW_abs<T, T>()(ax[i + 1])
                 + RW_abs<T, T>()(ax[i + 2]) + RW_abs<T, T>()(ax[i + 3])
                 + RW_abs<T, T>()(ax[i + 4]) + RW_abs<T, T>()(ax[i + 5]);
    return dtemp;
}


template <class T>
void RWBlas<T>::swap(const long& n, T* RW_RESTRICT_KEYWORD x, const long& incx,
                     T* RW_RESTRICT_KEYWORD y, const long& incy)
{
    T* ax = incx < 0 ? x - (n - 1) * incx : x;
    T* ay = incy < 0 ? y - (n - 1) * incy : y;
    int i, m;
    T dtemp;
    int ix = 0, iy = 0;

    if (n <= 0) {
        return;
    }
    if (incx == 1 && incy == 1) {
        /*
         *        CODE FOR BOTH INCREMENTS EQUAL TO 1
         */
        m = static_cast<int>(n % 3);
        for (i = 0; i < m; ++i) {   /* Clean up loop */
            dtemp = ax[i];
            ax[i] = ay[i];
            ay[i] = dtemp;
        }

        for (i = m; i < n; i += 3) {        /* Unrolled loop */
            dtemp     = ax[i];
            ax[i]     = ay[i];
            ay[i]     = dtemp;
            dtemp     = ax[i + 1];
            ax[i + 1] = ay[i + 1];
            ay[i + 1] = dtemp;
            dtemp     = ax[i + 2];
            ax[i + 2] = ay[i + 2];
            ay[i + 2] = dtemp;
        }
    }
    else {
        /*
         *        CODE FOR UNEQUAL INCREMENTS OR EQUAL INCREMENTS NOT EQUAL
         *          TO 1
         */
        for (i = 0; i < n; ++i) {
            dtemp = ax[ix];
            ax[ix] = ay[iy];
            ay[iy] = dtemp;
            ix += static_cast<int>(incx);
            iy += static_cast<int>(incy);
        }
    }
}


template <class T>
void RWBlas<T>::gemm(const char& transa, const char& transb, const long& m, const long& n,
                     const long& k, const T& alpha, T* a, const long& lda, T* b,
                     const long& ldb, const T& beta, T* c, const long& ldc)
{
#define A(I_,J_)  (*(a+(I_)*(lda)+(J_)))
#define B(I_,J_)  (*(b+(I_)*(ldb)+(J_)))
#define C(I_,J_)  (*(c+(I_)*(ldc)+(J_)))
    // PARAMETER translations
    const T ONE = T(1);
    const T ZERO = T(0);
    // end of PARAMETER translations

    int nota, notb;
    long _do0, _do1, _do10, _do11, _do12, _do13, _do14, _do15,
         _do16, _do17, _do18, _do19, _do2, _do3, _do4, _do5, _do6, _do7,
         _do8, _do9, i, i_, info, j, j_, l, l_, /*ncola,*/ nrowa, nrowb;
    T temp;

    //     .. Scalar Arguments ..
    //     .. Array Arguments ..
    //     ..

    //  Purpose
    //  =======

    //  GEMM  performs one of the matrix-matrix operations

    //     C := alpha*op( A )*op( B ) + beta*C,

    //  where  op( X ) is one of

    //     op( X ) = X   or   op( X ) = X',

    //  alpha and beta are scalars, and A, B and C are matrices, with op( A )
    //  an m by k matrix,  op( B )  a  k by n matrix and  C an m by n matrix.

    //  Parameters
    //  ==========

    //  TRANSA - CHARACTER*1.
    //           On entry, TRANSA specifies the form of op( A ) to be used in
    //           the matrix multiplication as follows:

    //              TRANSA = 'N' or 'n',  op( A ) = A.

    //              TRANSA = 'T' or 't',  op( A ) = A'.

    //              TRANSA = 'C' or 'c',  op( A ) = A'.

    //           Unchanged on exit.

    //  TRANSB - CHARACTER*1.
    //           On entry, TRANSB specifies the form of op( B ) to be used in
    //           the matrix multiplication as follows:

    //              TRANSB = 'N' or 'n',  op( B ) = B.

    //              TRANSB = 'T' or 't',  op( B ) = B'.

    //              TRANSB = 'C' or 'c',  op( B ) = B'.

    //           Unchanged on exit.

    //  M      - INTEGER.
    //           On entry,  M  specifies  the number  of rows  of the  matrix
    //           op( A )  and of the  matrix  C.  M  must  be at least  zero.
    //           Unchanged on exit.

    //  N      - INTEGER.
    //           On entry,  N  specifies the number  of columns of the matrix
    //           op( B ) and the number of columns of the matrix C. N must be
    //           at least zero.
    //           Unchanged on exit.

    //  K      - INTEGER.
    //           On entry,  K  specifies  the number of columns of the matrix
    //           op( A ) and the number of rows of the matrix op( B ). K must
    //           be at least  zero.
    //           Unchanged on exit.

    //  ALPHA  -
    //           On entry, ALPHA specifies the scalar alpha.
    //           Unchanged on exit.

    //  A      - array of DIMENSION ( LDA, ka ), where ka is
    //           k  when  TRANSA = 'N' or 'n',  and is  m  otherwise.
    //           Before entry with  TRANSA = 'N' or 'n',  the leading  m by k
    //           part of the array  A  must contain the matrix  A,  otherwise
    //           the leading  k by m  part of the array  A  must contain  the
    //           matrix A.
    //           Unchanged on exit.

    //  LDA    - INTEGER.
    //           On entry, LDA specifies the first dimension of A as declared
    //           in the calling (sub) program. When  TRANSA = 'N' or 'n' then
    //           LDA must be at least  max( 1, m ), otherwise  LDA must be at
    //           least  max( 1, k ).
    //           Unchanged on exit.

    //  B      - array of DIMENSION ( LDB, kb ), where kb is
    //           n  when  TRANSB = 'N' or 'n',  and is  k  otherwise.
    //           Before entry with  TRANSB = 'N' or 'n',  the leading  k by n
    //           part of the array  B  must contain the matrix  B,  otherwise
    //           the leading  n by k  part of the array  B  must contain  the
    //           matrix B.
    //           Unchanged on exit.

    //  LDB    - INTEGER.
    //           On entry, LDB specifies the first dimension of B as declared
    //           in the calling (sub) program. When  TRANSB = 'N' or 'n' then
    //           LDB must be at least  max( 1, k ), otherwise  LDB must be at
    //           least  max( 1, n ).
    //           Unchanged on exit.

    //  BETA   -
    //           On entry,  BETA  specifies the scalar  beta.  When  BETA  is
    //           supplied as zero then C need not be set on input.
    //           Unchanged on exit.

    //  C      - array of DIMENSION ( LDC, n ).
    //           Before entry, the leading  m by n  part of the array  C must
    //           contain the matrix  C,  except when  beta  is zero, in which
    //           case C need not be set on entry.
    //           On exit, the array  C  is overwritten by the  m by n  matrix
    //           ( alpha*op( A )*op( B ) + beta*C ).

    //  LDC    - INTEGER.
    //           On entry, LDC specifies the first dimension of C as declared
    //           in  the  calling  (sub)  program.   LDC  must  be  at  least
    //           max( 1, m ).
    //           Unchanged on exit.


    //  Level 3 Blas routine.

    //  -- Written on 8-February-1989.
    //     Jack Dongarra, Argonne National Laboratory.
    //     Iain Duff, AERE Harwell.
    //     Jeremy Du Croz, Numerical Algorithms Group Ltd.
    //     Sven Hammarling, Numerical Algorithms Group Ltd.


    //     .. External Functions ..
    //     .. External Subroutines ..
    //     .. Intrinsic Functions ..
    //     .. Local Scalars ..
    //     .. Parameters ..
    //     ..
    //     .. Executable Statements ..

    //     Set  NOTA  and  NOTB  as  true if  A  and  B  respectively are not
    //     transposed and set  NROWA, NCOLA and  NROWB  as the number of rows
    //     and  columns of  A  and the  number of  rows  of  B  respectively.

    nota = RWBlas_base_util<T>::lsame(transa, 'N');
    notb = RWBlas_base_util<T>::lsame(transb, 'N');
    if (nota) {
        nrowa = m;
        //ncola = k;
    }
    else {
        nrowa = k;
        //ncola = m;
    }
    if (notb) {
        nrowb = k;
    }
    else {
        nrowb = n;
    }

    //     Test the input parameters.

    info = 0;
    if (((!nota) && (!RWBlas_base_util<T>::lsame(transa, 'C'))) && (!RWBlas_base_util<T>::lsame(transa, 'T')
                                                                   )) {
        info = 1;
    }
    else if (((!notb) && (!RWBlas_base_util<T>::lsame(transb, 'C'))) && (!RWBlas_base_util<T>::lsame(transb,
             'T'))) {
        info = 2;
    }
    else if (m < 0) {
        info = 3;
    }
    else if (n < 0) {
        info = 4;
    }
    else if (k < 0) {
        info = 5;
    }
    else if (lda < (1 > nrowa ? 1 : nrowa)) {
        info = 8;
    }
    else if (ldb < (1 > nrowb ? 1 : nrowb)) {
        info = 10;
    }
    else if (ldc < (1 > m ? 1 : m)) {
        info = 13;
    }
    if (info != 0) {
        rw_xerbla("GEMM ", (rw_lapack_int_t)info);
        return;
    }

    //     Quick return if possible.

    if (((m == 0) || (n == 0)) || (((alpha == ZERO) || (k == 0)) &&
                                   (beta == ONE))) {
        return;
    }

    //     And if  alpha.eq.zero.

    if (alpha == ZERO) {
        if (beta == ZERO) {
            for (j = 1, j_ = j - 1, _do0 = n; j <= _do0; j++, j_++) {
                for (i = 1, i_ = i - 1, _do1 = m; i <= _do1; i++, i_++) {
                    C(j_, i_) = ZERO;
                }
            }
        }
        else {
            for (j = 1, j_ = j - 1, _do2 = n; j <= _do2; j++, j_++) {
                for (i = 1, i_ = i - 1, _do3 = m; i <= _do3; i++, i_++) {
                    C(j_, i_) = static_cast<T>(beta * C(j_, i_));
                }
            }
        }
        return;
    }

    //     Start the operations.

    if (notb) {
        if (nota) {

            //           Form  C := alpha*A*B + beta*C.

            for (j = 1, j_ = j - 1, _do4 = n; j <= _do4; j++, j_++) {
                if (beta == ZERO) {
                    for (i = 1, i_ = i - 1, _do5 = m; i <= _do5; i++, i_++) {
                        C(j_, i_) = ZERO;
                    }
                }
                else if (beta != ONE) {
                    for (i = 1, i_ = i - 1, _do6 = m; i <= _do6; i++, i_++) {
                        C(j_, i_) = static_cast<T>(beta * C(j_, i_));
                    }
                }
                for (l = 1, l_ = l - 1, _do7 = k; l <= _do7; l++, l_++) {
                    if (B(j_, l_) != ZERO) {
                        temp = static_cast<T>(alpha * B(j_, l_));
                        for (i = 1, i_ = i - 1, _do8 = m; i <= _do8; i++, i_++) {
                            C(j_, i_) = static_cast<T>(C(j_, i_) + temp * A(l_, i_));
                        }
                    }
                }
            }
        }
        else {

            //           Form  C := alpha*A'*B + beta*C

            for (j = 1, j_ = j - 1, _do9 = n; j <= _do9; j++, j_++) {
                for (i = 1, i_ = i - 1, _do10 = m; i <= _do10; i++, i_++) {
                    temp = ZERO;
                    for (l = 1, l_ = l - 1, _do11 = k; l <= _do11; l++, l_++) {
                        temp = static_cast<T>(temp + A(i_, l_) * B(j_, l_));
                    }
                    if (beta == ZERO) {
                        C(j_, i_) = static_cast<T>(alpha * temp);
                    }
                    else {
                        C(j_, i_) = static_cast<T>(alpha * temp + beta * C(j_, i_));
                    }
                }
            }
        }
    }
    else {
        if (nota) {

            //           Form  C := alpha*A*B' + beta*C

            for (j = 1, j_ = j - 1, _do12 = n; j <= _do12; j++, j_++) {
                if (beta == ZERO) {
                    for (i = 1, i_ = i - 1, _do13 = m; i <= _do13; i++, i_++) {
                        C(j_, i_) = ZERO;
                    }
                }
                else if (beta != ONE) {
                    for (i = 1, i_ = i - 1, _do14 = m; i <= _do14; i++, i_++) {
                        C(j_, i_) = static_cast<T>(beta * C(j_, i_));
                    }
                }
                for (l = 1, l_ = l - 1, _do15 = k; l <= _do15; l++, l_++) {
                    if (B(l_, j_) != ZERO) {
                        temp = static_cast<T>(alpha * B(l_, j_));
                        for (i = 1, i_ = i - 1, _do16 = m; i <= _do16; i++, i_++) {
                            C(j_, i_) = static_cast<T>(C(j_, i_) + temp * A(l_, i_));
                        }
                    }
                }
            }
        }
        else {

            //           Form  C := alpha*A'*B' + beta*C

            for (j = 1, j_ = j - 1, _do17 = n; j <= _do17; j++, j_++) {
                for (i = 1, i_ = i - 1, _do18 = m; i <= _do18; i++, i_++) {
                    temp = ZERO;
                    for (l = 1, l_ = l - 1, _do19 = k; l <= _do19; l++, l_++) {
                        temp = static_cast<T>(temp + A(i_, l_) * B(l_, j_));
                    }
                    if (beta == ZERO) {
                        C(j_, i_) = static_cast<T>(alpha * temp);
                    }
                    else {
                        C(j_, i_) = static_cast<T>(alpha * temp + beta * C(j_, i_));
                    }
                }
            }
        }
    }

    return;

    //     End of GEMM .

#undef  C
#undef  B
#undef  A
} // end of function


template <class T>
void RWBlas<T>::gemv(const char& trans, const long& m, const long& n, const T& alpha,
                     T* a, const long& lda, T x[], const long& incx, const T& beta,
                     T y[], const long& incy)
{
#define A(I_,J_)  (*(a+(I_)*(lda)+(J_)))
    // PARAMETER translations
    const T ONE(1);
    const T ZERO(0);
    // end of PARAMETER translations

    long _do0, _do1, _do10, _do11, _do2, _do3, _do4, _do5, _do6,
         _do7, _do8, _do9, i, i_, info, ix, iy, j, j_, jx, jy, kx, ky,
         lenx, leny;
    T temp;

    //     .. Scalar Arguments ..
    //     .. Array Arguments ..
    //     ..

    //  Purpose
    //  =======

    //  GEMV  performs one of the matrix-vector operations

    //     y := alpha*A*x + beta*y,   or   y := alpha*A'*x + beta*y,

    //  where alpha and beta are scalars, x and y are vectors and A is an
    //  m by n matrix.

    //  Parameters
    //  ==========

    //  TRANS  - CHARACTER*1.
    //           On entry, TRANS specifies the operation to be performed as
    //           follows:

    //              TRANS = 'N' or 'n'   y := alpha*A*x + beta*y.

    //              TRANS = 'T' or 't'   y := alpha*A'*x + beta*y.

    //              TRANS = 'C' or 'c'   y := alpha*A'*x + beta*y.

    //           Unchanged on exit.

    //  M      - INTEGER.
    //           On entry, M specifies the number of rows of the matrix A.
    //           M must be at least zero.
    //           Unchanged on exit.

    //  N      - INTEGER.
    //           On entry, N specifies the number of columns of the matrix A.
    //           N must be at least zero.
    //           Unchanged on exit.

    //  ALPHA  -
    //           On entry, ALPHA specifies the scalar alpha.
    //           Unchanged on exit.

    //  A      - array of DIMENSION ( LDA, n ).
    //           Before entry, the leading m by n part of the array A must
    //           contain the matrix of coefficients.
    //           Unchanged on exit.

    //  LDA    - INTEGER.
    //           On entry, LDA specifies the first dimension of A as declared
    //           in the calling (sub) program. LDA must be at least
    //           max( 1, m ).
    //           Unchanged on exit.

    //  X      - array of DIMENSION at least
    //           ( 1 + ( n - 1 )*abs( INCX ) ) when TRANS = 'N' or 'n'
    //           and at least
    //           ( 1 + ( m - 1 )*abs( INCX ) ) otherwise.
    //           Before entry, the incremented array X must contain the
    //           vector x.
    //           Unchanged on exit.

    //  INCX   - INTEGER.
    //           On entry, INCX specifies the increment for the elements of
    //           X. INCX must not be zero.
    //           Unchanged on exit.

    //  BETA   -
    //           On entry, BETA specifies the scalar beta. When BETA is
    //           supplied as zero then Y need not be set on input.
    //           Unchanged on exit.

    //  Y      - array of DIMENSION at least
    //           ( 1 + ( m - 1 )*abs( INCY ) ) when TRANS = 'N' or 'n'
    //           and at least
    //           ( 1 + ( n - 1 )*abs( INCY ) ) otherwise.
    //           Before entry with BETA non-zero, the incremented array Y
    //           must contain the vector y. On exit, Y is overwritten by the
    //           updated vector y.

    //  INCY   - INTEGER.
    //           On entry, INCY specifies the increment for the elements of
    //           Y. INCY must not be zero.
    //           Unchanged on exit.


    //  Level 2 Blas routine.

    //  -- Written on 22-October-1986.
    //     Jack Dongarra, Argonne National Lab.
    //     Jeremy Du Croz, Nag Central Office.
    //     Sven Hammarling, Nag Central Office.
    //     Richard Hanson, Sandia National Labs.


    //     .. Parameters ..
    //     .. Local Scalars ..
    //     .. External Functions ..
    //     .. External Subroutines ..
    //     .. Intrinsic Functions ..
    //     ..
    //     .. Executable Statements ..

    //     Test the input parameters.

    info = 0;
    if ((!RWBlas_base_util<T>::lsame(trans, 'N') && !RWBlas_base_util<T>::lsame(trans, 'T')) && !RWBlas_base_util<T>::lsame(trans,
            'C')) {
        info = 1;
    }
    else if (m < 0) {
        info = 2;
    }
    else if (n < 0) {
        info = 3;
    }
    else if (lda < (1 > m ? 1 : m)) {
        info = 6;
    }
    else if (incx == 0) {
        info = 8;
    }
    else if (incy == 0) {
        info = 11;
    }
    if (info != 0) {
        rw_xerbla("GEMV ", (rw_lapack_int_t)info);
        return;
    }

    //     Quick return if possible.

    if (((m == 0) || (n == 0)) || ((alpha == ZERO) && (beta == ONE))) {
        return;
    }

    //     Set  LENX  and  LENY, the lengths of the vectors x and y, and set
    //     up the start points in  X  and  Y.

    if (RWBlas_base_util<T>::lsame(trans, 'N')) {
        lenx = n;
        leny = m;
    }
    else {
        lenx = m;
        leny = n;
    }
    if (incx > 0) {
        kx = 1;
    }
    else {
        kx = 1 - (lenx - 1) * incx;
    }
    if (incy > 0) {
        ky = 1;
    }
    else {
        ky = 1 - (leny - 1) * incy;
    }

    //     Start the operations. In this version the elements of A are
    //     accessed sequentially with one pass through A.

    //     First form  y := beta*y.

    if (beta != ONE) {
        if (incy == 1) {
            if (beta == ZERO) {
                for (i = 1, i_ = i - 1, _do0 = leny; i <= _do0; i++, i_++) {
                    y[i_] = ZERO;
                }
            }
            else {
                for (i = 1, i_ = i - 1, _do1 = leny; i <= _do1; i++, i_++) {
                    y[i_] = static_cast<T>(beta * y[i_]);
                }
            }
        }
        else {
            iy = ky;
            if (beta == ZERO) {
                for (i = 1, i_ = i - 1, _do2 = leny; i <= _do2; i++, i_++) {
                    y[iy - 1] = ZERO;
                    iy = iy + incy;
                }
            }
            else {
                for (i = 1, i_ = i - 1, _do3 = leny; i <= _do3; i++, i_++) {
                    y[iy - 1] = static_cast<T>(beta * y[iy - 1]);
                    iy = iy + incy;
                }
            }
        }
    }
    if (alpha == ZERO) {
        return;
    }
    if (RWBlas_base_util<T>::lsame(trans, 'N')) {

        //        Form  y := alpha*A*x + y.

        jx = kx;
        if (incy == 1) {
            for (j = 1, j_ = j - 1, _do4 = n; j <= _do4; j++, j_++) {
                if (x[jx - 1] != ZERO) {
                    temp = static_cast<T>(alpha * x[jx - 1]);
                    for (i = 1, i_ = i - 1, _do5 = m; i <= _do5; i++, i_++) {
                        y[i_] = static_cast<T>(y[i_] + temp * A(j_, i_));
                    }
                }
                jx = jx + incx;
            }
        }
        else {
            for (j = 1, j_ = j - 1, _do6 = n; j <= _do6; j++, j_++) {
                if (x[jx - 1] != ZERO) {
                    temp = static_cast<T>(alpha * x[jx - 1]);
                    iy = ky;
                    for (i = 1, i_ = i - 1, _do7 = m; i <= _do7; i++, i_++) {
                        y[iy - 1] = static_cast<T>(y[iy - 1] + temp * A(j_, i_));
                        iy = iy + incy;
                    }
                }
                jx = jx + incx;
            }
        }
    }
    else {

        //        Form  y := alpha*A'*x + y.

        jy = ky;
        if (incx == 1) {
            for (j = 1, j_ = j - 1, _do8 = n; j <= _do8; j++, j_++) {
                temp = ZERO;
                for (i = 1, i_ = i - 1, _do9 = m; i <= _do9; i++, i_++) {
                    temp = static_cast<T>(temp + A(j_, i_) * x[i_]);
                }
                y[jy - 1] = static_cast<T>(y[jy - 1] + alpha * temp);
                jy = jy + incy;
            }
        }
        else {
            for (j = 1, j_ = j - 1, _do10 = n; j <= _do10; j++, j_++) {
                temp = ZERO;
                ix = kx;
                for (i = 1, i_ = i - 1, _do11 = m; i <= _do11; i++, i_++) {
                    temp = static_cast<T>(temp + A(j_, i_) * x[ix - 1]);
                    ix = ix + incx;
                }
                y[jy - 1] = static_cast<T>(y[jy - 1] + alpha * temp);
                jy = jy + incy;
            }
        }
    }

    return;

    //     End of DGEMV .

#undef  A
} // end of function

#endif
