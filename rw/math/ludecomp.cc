#ifndef __RWLUDECOMP_CC__
#define __RWLUDECOMP_CC__

/***************************************************************************
 *
 * ludecomp.cc
 *
 * $Id: //math/13/rw/math/ludecomp.cc#1 $
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
#include <rw/mathdefs.h>


template< class T >
int RWLUDecompMethod<T>::getFactorization(T* a, int lda, int n, RWPivotType* ipvt)
{
    /*
     *  Local variables
     */
    int j, k;
    RWPivotType l;
    T t;

    /*
     *  Function Body
     */
    int info = 0;
    if (n > 1) {
        for (k = 0; k < n - 1; ++k) {

            /*
             *         find l = pivot index
             */

            long temp = RWBlas<T>::iamax(n - k, &a[k + k * lda], 1) + k;
            RW_LAPACK_INT_MAX_ASSERT(temp);
            l = static_cast<RWPivotType>(temp);

            ipvt[k] = l;

            /*
             *         zero pivot implies this column already triangularized
             */
            if (a[l + k * lda] != T(0)) {
                /*
                 *            interchange if necessary
                 */
                if (l != k) {
                    t = a[l + k * lda];
                    a[l + k * lda] = a[k + k * lda];
                    a[k + k * lda] = t;
                };
                /*
                 *            compute multipliers
                 */
                t = -T(1) / a[k + k * lda];
                RWBlas<T>::scal(n - (k + 1), t, &a[k + 1 + k * lda], 1);

                /*
                 *            row elimination with column indexing
                 */
                for (j = k + 1; j < n; ++j) {
                    t = a[l + j * lda];
                    if (l != k) {
                        a[l + j * lda] = a[k + j * lda];
                        a[k + j * lda] = t;
                    }
                    RWBlas<T>::axpy(n - (k + 1), t, &a[k + 1 + k * lda], 1, &a[k + 1 + j * lda], 1);
                }
            }
            else {
                info = k;
            }
        }
    }
    ipvt[n - 1] = n - 1;
    if (a[n - 1 + (n - 1)*lda] == T(0)) {
        info = n - 1;
    }
    return info;
}


#if (defined(__SUNPRO_CC) && (__SUNPRO_CC == 0x520))
template< class T >
int RWLUDecompMethod<T>::getConditionNumber(T* a, int lda, int n, RWPivotType* ipvt,
        rw_numeric_traits<T>::norm_type& rcond,
        T* z)
#elif (defined(__SUNPRO_CC) && (__SUNPRO_CC >= 0x530))
template< class T >
int  RWLUDecompMethod<T>::getConditionNumber(T* a, int lda, int n, RWPivotType* ipvt,
        norm_type& rcond,
        T* z)
#else

template< class T >
int RWLUDecompMethod<T>::getConditionNumber(T* a, int lda, int n, RWPivotType* ipvt,
        typename rw_numeric_traits<T>::norm_type& rcond,
        T* z)
#endif  // defined(__SUNPRO_CC) && (__SUNPRO_CC == 0x520)
{
    /*
     *  Local variables
     */
    typedef typename rw_numeric_traits<T>::norm_type absType;
    RW_abs< T, absType > absValue;
    int info;
    int j, k;
    long l;
    typename rw_numeric_traits<T>::norm_type s, sm;
    T t;
    typename rw_numeric_traits<T>::norm_type anorm;
    typename rw_numeric_traits<T>::norm_type ynorm;
    int kb;
    T ek, wk;
    T wkm;


    /*
     *  Parameter adjustments
     */

    /*
     *  Function Body
     */
    for (j = 0, anorm = 0; j < n; ++j)
        /* MSVC does not like the typename keyword in a template parameter */
#if !defined(_MSC_VER)
        anorm = RWBlas_util< typename rw_numeric_traits<T>::norm_type >::max(anorm, RWBlas<T>::asum(n, &a[j * lda], 1));
#else
        anorm = (RWBlas_util< rw_numeric_traits<T>::norm_type >::max)(anorm, RWBlas<T>::asum(n, &a[j * lda], 1));
#endif

    /*
     *      factor
     */
    info = getFactorization(a, lda, n, ipvt);

    /*
     *      rcond = 1/(norm(a)*(estimate of norm(inverse(a)))) .
     *      estimate = norm(z)/norm(y) where  a*z = y  and  trans(a)*y = e .
     *
     *      trans(a)  is the transpose of a .  the components of  e  are
     *      chosen to cause maximum local growth in the elements of w  where
     *
     *      trans(u)*w = e .  the vectors are frequently rescaled to avoid
     *      overflow.
     *
     *      solve trans(u)*w = e
     */
    ek = T(1);
    for (j = 0; j < n; ++j) {
        z[j] = T(0);
    }
    for (k = 0; k < n; ++k) {
        if (z[k] != T(0)) {
            ek = RWBlas_util<T>::sign(ek, -z[k]);
        }
        if (absValue(ek - z[k]) > absValue(a[k + k * lda])) {
            s = absValue(a[k + k * lda]) / absValue(ek - z[k]);

            RWBlas<T>::scal(n, s, &z[0], 1);
            ek = s * ek;
        }
        wk = ek - z[k];
        wkm = -ek - z[k];
        s = absValue(wk);
        sm = absValue(wkm);
        if (a[k + k * lda] != T(0)) {
            wk /= a[k + k * lda];
            wkm /= a[k + k * lda];
        }
        else {
            wk = T(1);
            wkm = T(1);
        }
        if (k + 1 < n) {
            for (j = k + 1; j < n; ++j) {
                sm += absValue(z[j] + wkm * a[k + j * lda]);
                z[j] += wk * a[k + j * lda];
                s += absValue(z[j]);
            }
            if (s < sm) {
                t = wkm - wk;
                wk = wkm;
                for (j = k + 1; j < n; ++j) {
                    z[j] += t * a[k + j * lda];
                }
            }
        }
        z[k] = wk;
    }
    s = (typename rw_numeric_traits<T>::norm_type)1 / RWBlas<T>::asum(n, &z[0], 1);
    RWBlas<T>::scal(n, s, &z[0], 1);

    /*
     *      solve trans(l)*y = w
     */
    for (kb = 0; kb < n; ++kb) {
        k = n - kb;
        if (k < n) {
            z[k - 1] += RWBlas<T>::dot(n - k, &a[k + (k - 1) * lda], 1, &z[k], 1);
        }
        if (absValue(z[k - 1]) > (typename rw_numeric_traits<T>::norm_type)1) {
            s = (typename rw_numeric_traits<T>::norm_type)1 / absValue(z[k - 1]);
            RWBlas<T>::scal(n, s, &z[0], 1);
        }
        l = ipvt[k - 1];
        t = z[l];            // warning 4229 pragma'ed for hp-ipf (see start of the method)
        z[l] = z[k - 1];     // warning 4229 pragma'ed for hp-ipf (see start of the method)
        z[k - 1] = t;
    }
    s = (typename rw_numeric_traits<T>::norm_type)1 / RWBlas<T>::asum(n, &z[0], 1);
    RWBlas<T>::scal(n, s, &z[0], 1);

    ynorm = 1;

    /*
     *      solve l*v = y
     */
    for (k = 0; k < n; ++k) {
        l = ipvt[k];
        t = z[l];            // warning 4229 pragma'ed for hp-ipf (see start of the method)
        z[l] = z[k];         // warning 4229 pragma'ed for hp-ipf (see start of the method)
        z[k] = t;
        if (k + 1 < n) {
            RWBlas<T>::axpy(n - k - 1, t, &a[k + 1 + k * lda], 1, &z[k + 1], 1);
        }
        if (absValue(z[k]) > (typename rw_numeric_traits<T>::norm_type)1) {
            s = (typename rw_numeric_traits<T>::norm_type)1 / absValue(z[k]);
            RWBlas<T>::scal(n, s, &z[0], 1);
            ynorm = s * ynorm;
        }
    }
    s = (typename rw_numeric_traits<T>::norm_type)1 / RWBlas<T>::asum(n, &z[0], 1);
    RWBlas<T>::scal(n, s, &z[0], 1);
    ynorm = s * ynorm;

    /*
     *      solve  u*z = v
     */
    for (kb = 0; kb < n; ++kb) {
        k = n - kb;
        if (absValue(z[k - 1]) > absValue(a[k - 1 + (k - 1)*lda])) {
            s = absValue(a[k - 1 + (k - 1) * lda]) / absValue(z[k - 1]);
            RWBlas<T>::scal(n, s, &z[0], 1);
            ynorm = s * ynorm;
        }
        if (a[k - 1 + (k - 1)*lda] != T(0)) {
            z[k - 1] /= a[k - 1 + (k - 1) * lda];
        }
        if (a[k - 1 + (k - 1)*lda] == T(0)) {
            z[k - 1] = T(1);
        }
        t = -z[k - 1];
        RWBlas<T>::axpy(k - 1, t, &a[(k - 1)*lda], 1, &z[0], 1);
    }
    /*
     *      make znorm = 1.0
     */
    s = (typename rw_numeric_traits<T>::norm_type)1 / RWBlas<T>::asum(n, &z[0], 1);
    RWBlas<T>::scal(n, s, &z[0], 1);
    ynorm = s * ynorm;

    if (anorm != (typename rw_numeric_traits<T>::norm_type)0) {
        rcond = ynorm / anorm;
    }
    if (anorm == (typename rw_numeric_traits<T>::norm_type)0) {
        rcond = 0;
    }

    return info;
}

template< class T >
void RWLUDecompMethod<T>::solveSystem(T* a, int lda, int n, RWPivotType* ipvt,
                                      T* b, int job)
{
    /*
     *  Local variables
     */
    int k;
    long l;
    T t;
    int kb;


    /*
     *  Function Body
     */
    if (job == 0) {

        /*
         *         job = 0 , solve  a * x = b
         *         first solve  l*y = b
         */
        if (n >= 2) {
            for (k = 0; k < n - 1; ++k) {
                l = ipvt[k];
                t = b[l];        // warning 4229 pragma'ed for hp-ipf(see start of the method)
                if (l != k) {
                    b[l] = b[k];   // warning 4229 pragma'ed for hp-ipf(see start of the method)
                    b[k] = t;
                }
                RWBlas<T>::axpy(n - k - 1, t, &a[k + 1 + k * lda], 1, &b[k + 1], 1);
            }
        }

        /*
         *         now solve  u*x = y
         */
        for (kb = 0; kb < n; ++kb) {
            k = n - kb - 1;
            b[k] /= a[k + k * lda];
            t = -b[k];
            RWBlas<T>::axpy(k, t, &a[k * lda], 1, &b[0], 1);
        }
    }
    else {

        /*
         *         job = nonzero, solve  trans(a) * x = b
         *         first solve  trans(u)*y = b
         */
        for (k = 0; k < n; ++k) {
            t = RWBlas<T>::dot(k, &a[k * lda], 1, &b[0], 1);
            b[k] = (b[k] - t) / a[k + k * lda];
        }

        /*
         *         now solve trans(l)*x = y
         */
        if (n >= 2) {
            for (kb = 0; kb < n - 1; ++kb) {
                k = n - kb - 2;
                b[k] += RWBlas<T>::dot(n - k, &a[k + 1 + k * lda], 1, &b[k + 1], 1);
                l = ipvt[k];
                if (l != k) {
                    t = b[l];      // warning 4229 pragma'ed for hp-ipf (see start of the method)
                    b[l] = b[k];   // warning 4229 pragma'ed for hp-ipf (see start of the method)
                    b[k] = t;
                }
            }
        }
    }
}

template< class T >
void RWLUDecompMethod<T>::determinantInverse(T* a, int lda, int n, RWPivotType* ipvt,
        T& det, T* work, int job)
{
    /*
     *  Local variables
     */
    int i, j, k;
    long l;
    T t;
    int kb;

    /*
     *  Function Body
     */
    if (job / 10 != 0) {
        det = T(1);
        for (i = 0; i < n; ++i) {
            if (ipvt[i] != i) {
                det = -det;
            }
            det = a[i + i * lda] * det;
        }
    }

    /*
     *      compute inverse(u)
     */
    if (job % 10 != 0) {
        for (k = 0; k < n; ++k) {
            a[k + k * lda] = T(1) / a[k + k * lda];
            t = -a[k + k * lda];
            RWBlas<T>::scal(k, t, &a[k * lda], 1);
            if (n >= k + 2) {
                for (j = k + 1; j < n; ++j) {
                    t = a[k + j * lda];
                    a[k + j * lda] = T(0);
                    RWBlas<T>::axpy(k + 1, t, &a[k * lda], 1, &a[j * lda], 1);
                }
            }
        }

        /*
         *         form inverse(u)*inverse(l)
         */
        if (n >= 2) {
            for (kb = 0; kb < n - 1; ++kb) {
                k = n - kb - 2;
                for (i = k + 1; i < n; ++i) {
                    work[i] = a[i + k * lda];
                    a[i + k * lda] = T(0);
                }
                for (j = k + 1; j < n; ++j) {
                    t = work[j];
                    RWBlas<T>::axpy(n, t, &a[j * lda], 1, &a[k * lda], 1);
                }
                l = ipvt[k];
                if (l != k) {
                    RWBlas<T>::swap(n, &a[k * lda], 1, &a[l * lda], 1);
                }
            }
        }
    }
}

#endif // __RWLUDECOMP_CC__
