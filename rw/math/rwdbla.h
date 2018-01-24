#ifdef __RWBLA_H
#  ifndef __RWDBLA_H
#    define __RWDBLA_H

/***************************************************************************
 *
 * rwdbla.h - BLAS specializations for double
 *
 * $Id: //math/13/rw/math/rwdbla.h#1 $
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
class RW_MATH_GLOBAL RWBlas<double>
{
public:

    static double asum(const long& n,
                       double* RW_RESTRICT_KEYWORD x, const long& incx);

    static void axpy(const long& n, const double& ca, double* RW_RESTRICT_KEYWORD x,
                     const long& incx, double* RW_RESTRICT_KEYWORD y, const long& incy);

    static void copy(const long& n, const double* RW_RESTRICT_KEYWORD x, const long& incx,
                     double* RW_RESTRICT_KEYWORD y, const long& incy);

    static long iamax(const long& n, double* RW_RESTRICT_KEYWORD x, const long& incx);

    static double nrm2(const long& n,
                       double* RW_RESTRICT_KEYWORD x, const long& incx);

    static void rot(const long& n, double* RW_RESTRICT_KEYWORD x, const long& incx,
                    double* RW_RESTRICT_KEYWORD y, const long& incy,
                    const double& c,
                    const double& s);

    static void scal(const long& n, const double& a, double* RW_RESTRICT_KEYWORD x,
                     const long& incx);

    static void gbmv(const char& trans, const long& m, const long& n,
                     const long& kl, const long& ku, const double& alpha, double* a,
                     const long& lda, double* RW_RESTRICT_KEYWORD x, const long& incx,
                     const double& beta, double* RW_RESTRICT_KEYWORD y, const long& incy);

    static void tbmv(const char& uplo, const char& trans, const char& diag,
                     const long& n, const long& k, double* a, const long& lda,
                     double* RW_RESTRICT_KEYWORD x, const long& incx);

    static void gemm(const char& transa, const char& transb, const long& m,
                     const long& n, const long& k, const double& alpha, double* a,
                     const long& lda, double* b, const long& ldb, const double& beta,
                     double* c, const long& ldc);

    static void gemv(const char& trans, const long& m, const long& n,
                     const double& alpha, double* a, const long& lda, double* RW_RESTRICT_KEYWORD x,
                     const long& incx, const double& beta, double* RW_RESTRICT_KEYWORD y,
                     const long& incy);

    static void rotg(double& ca, double& cb,
                     double& c, double& s);

    static void swap(const long& n, double* RW_RESTRICT_KEYWORD x, const long& incx,
                     double* RW_RESTRICT_KEYWORD y, const long& incy);

    static void symm(const char& side, const char& uplo, const long& m,
                     const long& n, const double& alpha, double* a, const long& lda,
                     double* b, const long& ldb, const double& beta, double* c,
                     const long& ldc);

    static void syr2k(const char& uplo, const char& trans, const long& n,
                      const long& k, const double& alpha, double* a, const long& lda,
                      double* b, const long& ldb, const double& beta, double* c,
                      const long& ldc);

    static void syrk(const char& uplo, const char& trans, const long& n,
                     const long& k, const double& alpha, double* a, const long& lda,
                     const double& beta, double* c, const long& ldc);

    static void tbsv(const char& uplo, const char& trans, const char& diag,
                     const long& n, const long& k, double* a, const long& lda,
                     double* RW_RESTRICT_KEYWORD x, const long& incx);

    static void tpmv(const char& uplo, const char& trans, const char& diag,
                     const long& n, double* RW_RESTRICT_KEYWORD ap, double* RW_RESTRICT_KEYWORD x,
                     const long& incx);

    static void tpsv(const char& uplo, const char& trans, const char& diag,
                     const long& n, double* RW_RESTRICT_KEYWORD ap, double* RW_RESTRICT_KEYWORD x,
                     const long& incx);

    static void trmm(const char& side, const char& uplo, const char& transa,
                     const char& diag, const long& m, const long& n, const double& alpha,
                     double* a, const long& lda, double* b, const long& ldb);

    static void trmv(const char& uplo, const char& trans, const char& diag,
                     const long& n, double* a, const long& lda, double* RW_RESTRICT_KEYWORD x,
                     const long& incx);

    static void trsm(const char& side, const char& uplo, const char& transa,
                     const char& diag, const long& m, const long& n, const double& alpha,
                     double* a, const long& lda, double* b, const long& ldb);

    static void trsv(const char& uplo, const char& trans, const char& diag,
                     const long& n, double* a, const long& lda, double* RW_RESTRICT_KEYWORD x,
                     const long& incx);

    static double dot(const long& n, const double* RW_RESTRICT_KEYWORD x, const long& incx,
                      const double* RW_RESTRICT_KEYWORD y, const long& incy);

    static void sbmv(const char& uplo, const long& n, const long& k,
                     const double& alpha, double* a, const long& lda, double* RW_RESTRICT_KEYWORD x,
                     const long& incx, const double& beta, double* RW_RESTRICT_KEYWORD y,
                     const long& incy);

    static void ger(const long& m, const long& n, const double& alpha,
                    double* RW_RESTRICT_KEYWORD x, const long& incx, double* RW_RESTRICT_KEYWORD y,
                    const long& incy, double* a, const long& lda);

    static void spmv(const char& uplo, const long& n, const double& alpha,
                     double* RW_RESTRICT_KEYWORD ap, double* RW_RESTRICT_KEYWORD x, const long& incx,
                     const double& beta, double* RW_RESTRICT_KEYWORD y, const long& incy);

    static void spr(const char& uplo, const long& n, const double& alpha,
                    double* RW_RESTRICT_KEYWORD x, const long& incx, double* RW_RESTRICT_KEYWORD ap);

    static void spr2(const char& uplo, const long& n, const double& alpha,
                     double* RW_RESTRICT_KEYWORD x, const long& incx, double* RW_RESTRICT_KEYWORD y,
                     const long& incy, double* RW_RESTRICT_KEYWORD ap);

    static void symv(const char& uplo, const long& n, const double& alpha,
                     double* a, const long& lda, double* RW_RESTRICT_KEYWORD x, const long& incx,
                     const double& beta, double* RW_RESTRICT_KEYWORD y, const long& incy);

    static void syr(const char& uplo, const long& n, const double& alpha,
                    double* RW_RESTRICT_KEYWORD x, const long& incx, double* a, const long& lda);

    static void syr2(const char& uplo, const long& n, const double& alpha,
                     double* RW_RESTRICT_KEYWORD x, const long& incx, double* RW_RESTRICT_KEYWORD y,
                     const long& incy, double* a, const long& lda);


};

#  endif /* __RWDBLA_H */
#else
#  error You need to include <rw/math/rwbla.h> instead.
#endif /* __RWBLA_H */
