#ifdef __RWBLA_H
#  ifndef __RWCBLA_H
#    define __RWCBLA_H

/***************************************************************************
 *
 * rwcbla.h - BLAS specializations for FComplex
 *
 * $Id: //math/13/rw/math/rwcbla.h#1 $
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


RW_MATH_SPECIALIZE
class RW_MATH_GLOBAL RWBlas<FComplex>
{
public:

    static float asum(const long& n,
                      FComplex* RW_RESTRICT_KEYWORD x, const long& incx);

    static void axpy(const long& n, const FComplex& ca, FComplex* RW_RESTRICT_KEYWORD x,
                     const long& incx, FComplex* RW_RESTRICT_KEYWORD y, const long& incy);

    static void copy(const long& n, const FComplex* RW_RESTRICT_KEYWORD x, const long& incx,
                     FComplex* RW_RESTRICT_KEYWORD y, const long& incy);

    static long iamax(const long& n, const FComplex* RW_RESTRICT_KEYWORD x, const long& incx);

#    ifdef RW_MATH_BLAS_SPECL_IAMIN
    long iamin(const long& n, const FComplex* RW_RESTRICT_KEYWORD x, const long& incx);
#    endif

    static float nrm2(const long& n,
                      FComplex* RW_RESTRICT_KEYWORD x, const long& incx);

    static void rot(const long& n, FComplex* RW_RESTRICT_KEYWORD x, const long& incx,
                    FComplex* RW_RESTRICT_KEYWORD y, const long& incy,
                    const float& c,
                    const float& s);

    static void scal(const long& n, const FComplex& a, FComplex* RW_RESTRICT_KEYWORD x,
                     const long& incx);

    static void gbmv(const char& trans, const long& m, const long& n,
                     const long& kl, const long& ku, const FComplex& alpha, FComplex* a,
                     const long& lda, FComplex* RW_RESTRICT_KEYWORD x, const long& incx,
                     const FComplex& beta, FComplex* RW_RESTRICT_KEYWORD y, const long& incy);

    static void tbmv(const char& uplo, const char& trans, const char& diag,
                     const long& n, const long& k, FComplex* a, const long& lda,
                     FComplex* RW_RESTRICT_KEYWORD x, const long& incx);

    static void gemm(const char& transa, const char& transb, const long& m,
                     const long& n, const long& k, const FComplex& alpha, FComplex* a,
                     const long& lda, FComplex* b, const long& ldb, const FComplex& beta,
                     FComplex* c, const long& ldc);

    static void gemv(const char& trans, const long& m, const long& n,
                     const FComplex& alpha, FComplex* a, const long& lda,
                     FComplex* RW_RESTRICT_KEYWORD x, const long& incx, const FComplex& beta,
                     FComplex* RW_RESTRICT_KEYWORD y, const long& incy);

    static void rotg(FComplex& ca, FComplex& cb,
                     float& c, FComplex& s);

    static void swap(const long& n, FComplex* RW_RESTRICT_KEYWORD x, const long& incx,
                     FComplex* RW_RESTRICT_KEYWORD y, const long& incy);

    static void symm(const char& side, const char& uplo, const long& m,
                     const long& n, const FComplex& alpha, FComplex* a, const long& lda,
                     FComplex* b, const long& ldb, const FComplex& beta, FComplex* c,
                     const long& ldc);

    static void syr2k(const char& uplo, const char& trans, const long& n,
                      const long& k, const FComplex& alpha, FComplex* a, const long& lda,
                      FComplex* b, const long& ldb, const FComplex& beta, FComplex* c,
                      const long& ldc);

    static void syrk(const char& uplo, const char& trans, const long& n,
                     const long& k, const FComplex& alpha, FComplex* a, const long& lda,
                     const FComplex& beta, FComplex* c, const long& ldc);

    static void tbsv(const char& uplo, const char& trans, const char& diag,
                     const long& n, const long& k, FComplex* a, const long& lda,
                     FComplex* RW_RESTRICT_KEYWORD x, const long& incx);

    static void tpmv(const char& uplo, const char& trans, const char& diag,
                     const long& n, FComplex* RW_RESTRICT_KEYWORD ap, FComplex* RW_RESTRICT_KEYWORD x,
                     const long& incx);

    static void tpsv(const char& uplo, const char& trans, const char& diag,
                     const long& n, FComplex* RW_RESTRICT_KEYWORD ap, FComplex* RW_RESTRICT_KEYWORD x,
                     const long& incx);

    static void trmm(const char& side, const char& uplo, const char& transa,
                     const char& diag, const long& m, const long& n, const FComplex& alpha,
                     FComplex* a, const long& lda, FComplex* b, const long& ldb);

    static void trmv(const char& uplo, const char& trans, const char& diag,
                     const long& n, FComplex* a, const long& lda, FComplex* RW_RESTRICT_KEYWORD x,
                     const long& incx);

    static void trsm(const char& side, const char& uplo, const char& transa,
                     const char& diag, const long& m, const long& n, const FComplex& alpha,
                     FComplex* a, const long& lda, FComplex* b, const long& ldb);

    static void trsv(const char& uplo, const char& trans, const char& diag,
                     const long& n, FComplex* a, const long& lda, FComplex* RW_RESTRICT_KEYWORD x,
                     const long& incx);

    static FComplex dotc(const long& n, const FComplex* RW_RESTRICT_KEYWORD x,
                         const long& incx, const FComplex* RW_RESTRICT_KEYWORD y, const long& incy);

    static FComplex dotu(const long& n, const FComplex* RW_RESTRICT_KEYWORD x,
                         const long& incx, const FComplex* RW_RESTRICT_KEYWORD y, const long& incy)
    ;

    static FComplex dot(const long& n, const FComplex* RW_RESTRICT_KEYWORD x,
                        const long& incx, const FComplex* RW_RESTRICT_KEYWORD y, const long& incy);

    static void scal(const long& n, const float& a, FComplex* RW_RESTRICT_KEYWORD x,
                     const long& incx);

    static void hbmv(const char& uplo, const long& n, const long& k,
                     const FComplex& alpha, FComplex* a, const long& lda,
                     FComplex* RW_RESTRICT_KEYWORD x, const long& incx, const FComplex& beta,
                     FComplex* RW_RESTRICT_KEYWORD y, const long& incy);

    static void gerc(const long& m, const long& n, const FComplex& alpha,
                     FComplex* RW_RESTRICT_KEYWORD x, const long& incx, FComplex* RW_RESTRICT_KEYWORD y,
                     const long& incy, FComplex* a, const long& lda);

    static void geru(const long& m, const long& n, const FComplex& alpha,
                     FComplex* RW_RESTRICT_KEYWORD x, const long& incx, FComplex* RW_RESTRICT_KEYWORD y,
                     const long& incy, FComplex* a, const long& lda);

    static void hemm(const char& side, const char& uplo, const long& m,
                     const long& n, const FComplex& alpha, FComplex* a, const long& lda,
                     FComplex* b, const long& ldb, const FComplex& beta, FComplex* c,
                     const long& ldc);

    static void hemv(const char& uplo, const long& n, const FComplex& alpha,
                     FComplex* a, const long& lda, FComplex* RW_RESTRICT_KEYWORD x, const long& incx,
                     const FComplex& beta, FComplex* RW_RESTRICT_KEYWORD y, const long& incy);

    static void hpmv(const char& uplo, const long& n, const FComplex& alpha,
                     FComplex* RW_RESTRICT_KEYWORD ap, FComplex* RW_RESTRICT_KEYWORD x, const long& incx,
                     const FComplex& beta, FComplex* RW_RESTRICT_KEYWORD y, const long& incy);

};

#  endif /* __RWCBLA_H */
#else
#  error You need to include <rw/math/rwbla.h> instead.
#endif /* __RWBLA_H */
