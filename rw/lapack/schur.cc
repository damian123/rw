/*
 * defs for RWSchurDecomp
 *
 * $Id: //lapack/13/rw/lapack/schur.cc#1 $
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

#include <rw/lapack/hess.h>
#include <rw/lapack/lapkfunc.h>
#include <rw/lapkerr.h>
#include <rw/math/mathvec.h>
#include <rw/math/mthvecpk.h>
#include <rw/math/rwalloc.h>
#include <rw/rwerr.h>

template <class TypeT>
RWSchurDecomp<TypeT>::RWSchurDecomp()
{
}

template <class TypeT>
RWSchurDecomp<TypeT>::RWSchurDecomp(const RWGenMat<TypeT>& A, bool permute, bool scale)
{
    factor(A, permute, scale);
}

template <class TypeT>
RWSchurDecomp<TypeT>::RWSchurDecomp(const RWBalanceDecomp<TypeT>& A)
{
    factor(A);
}

template <class TypeT>
RWSchurDecomp<TypeT>::RWSchurDecomp(const RWHessenbergDecomp<TypeT>& A)
{
    factor(A);
}

template <class TypeT>
void RWSchurDecomp<TypeT>::factor(const RWGenMat<TypeT>& A, bool permute, bool scale)
{
    RWBalanceDecomp<TypeT> B(A, permute, scale);
    factor(B);
}

template <class TypeT>
void RWSchurDecomp<TypeT>::factor(const RWBalanceDecomp<TypeT>& A)
{
    RWHessenbergDecomp<TypeT> B(A);
    factor(B);
}

template <class TypeT>
bool RWSchurDecomp<TypeT>::good() const
{
    return (w_.length() == rows());
}

template <class TypeT>
void RWSchurDecomp<TypeT>::factor(const RWHessenbergDecomp<TypeT>& A)
{
    unsigned n = A.rows();
    B_ = A.balanceTransform();
    T_.reference(A.H());
    Z_.reference(A.Q());
    w_.reshape(n);
    unsigned numRows = rows();
    RWMathVec<double> wr(numRows, rwUninitialized);
    RWMathVec<double> wi(numRows, rwUninitialized);
    rw_lapack_int_t ilo = B_.lowIndex();
    rw_lapack_int_t ihi = B_.highIndex();
    RWTWorkBuffer<TypeT> work(n);
    rw_lapack_int_t info;
    hseqr('S', 'V', n, ilo, ihi, T_.data(), T_.colStride(), wr.data(), wi.data(), Z_.data(), Z_.colStride(), work.data(), (rw_lapack_int_t)n, info);
    RWPOSTCONDITION(info >= 0);
    if (info > 0) {
        int stride = (int)(n - info);
        size_t newSize = (size_t)ilo - 1 + stride;
        RWPOSTCONDITION(newSize < n);
        // The casts in the following prevent "long passed as int" warnings
        w_.reshape(newSize);
        wr(RWSlice((int)(ilo), stride)) = wr(RWToEnd((int)info));
        wr.resize(newSize);
        wi(RWSlice((int)(ilo), stride)) = wi(RWToEnd((int)info));
        wi.resize(newSize);
    }
    real(w_) = wr;
    imag(w_) = wi;
}
//specialize for DComplex
template <>
inline void RWSchurDecomp<DComplex>::factor(const RWHessenbergDecomp<DComplex>& A)
{
    factor_specialized(A);
}

template <class TypeT>
void RWSchurDecomp<TypeT>::factor_specialized(const RWHessenbergDecomp<TypeT>& A)
{
    unsigned n = A.rows();
    B_ = A.balanceTransform();
    T_.reference(A.H());
    Z_.reference(A.Q());
    w_.reshape(n);
    rw_lapack_int_t ilo = B_.lowIndex();
    rw_lapack_int_t ihi = B_.highIndex();
    RWTWorkBuffer<DComplex> work(n);
    rw_lapack_int_t info;
    hseqr('S', 'V', n, ilo, ihi, T_.data(), T_.colStride(), w_.data(), Z_.data(), Z_.colStride(), work.data(), (rw_lapack_int_t)n, info);
    RWPOSTCONDITION(info >= 0);
    if (info > 0) {
        RWPOSTCONDITION(ilo - 1 + n - info < n);
        w_(RWSlice((int)(ilo), (int)(n - info))) = w_(RWToEnd((int)info));
        w_.resize((size_t)(ilo - 1 + n - info));
    }
}
//end of specialization

template <class TypeT>
RWGenMat<TypeT> RWSchurDecomp<TypeT>::B() const
{
    RWGenMat<TypeT> I(rows(), cols(), (TypeT)0);
    I.diagonal() = (TypeT)1;
    return B_.transform(I);
}

template <class TypeT>
RWGenMat<TypeT> RWSchurDecomp<TypeT>::Z() const
{
    return Z_;
}
template <class TypeT>
RWGenMat<TypeT> RWSchurDecomp<TypeT>::T() const
{
    return T_;
}

template <class TypeT>
DComplex RWSchurDecomp<TypeT>::eigenvalue(int i) const
{
    if (i < 0 || i >= rows()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOEIG, i)));
    }
    return w_(i);
}

/*
template <class TypeT>
RWMathVec<DComplex> RWSchurDecomp<TypeT>::eigenvalues() const
{
  return w_;
}
*/

template <class TypeT>
RWMathVec<TypeT> RWSchurDecomp<TypeT>::Bx(const RWMathVec<TypeT>& x) const
{
    return B_.transform(x);
}

template <class TypeT>
RWMathVec<TypeT> RWSchurDecomp<TypeT>::BInvTx(const RWMathVec<TypeT>& x) const
{
    return B_.invTransform(x);
}

template <class TypeT>
RWMathVec<TypeT> RWSchurDecomp<TypeT>::Zx(const RWMathVec<TypeT>& x) const
{
    return product(Z_, x);
}

template <class TypeT>
RWMathVec<TypeT> RWSchurDecomp<TypeT>::ZTx(const RWMathVec<TypeT>& x) const
{
    return product(x, Z_);
}
//specialize for DComplex:
template <>
inline RWMathVec<DComplex> RWSchurDecomp<DComplex>::ZTx(const RWMathVec<DComplex>& x) const
{
    return conj(product(conj(x), Z_));
}
//end of specialization

template <class TypeT>
RWGenMat<TypeT> RWSchurDecomp<TypeT>::BX(const RWGenMat<TypeT>& X) const
{
    return B_.transform(X);
}

template <class TypeT>
RWGenMat<TypeT> RWSchurDecomp<TypeT>::BInvTX(const RWGenMat<TypeT>& X)const
{
    return B_.invTransform(X);
}

template <class TypeT>
RWGenMat<TypeT> RWSchurDecomp<TypeT>::ZX(const RWGenMat<TypeT>& X) const
{
    return product(Z_, X);
}

template <class TypeT>
RWGenMat<TypeT> RWSchurDecomp<TypeT>::ZTX(const RWGenMat<TypeT>& X) const
{
    return transposeProduct(Z_, X);
}

template <class TypeT>
int RWSchurDecomp<TypeT>::move(int from, int to)
{
    if (!good()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_SCHURREORDER)));
    }
    unsigned n = rows();
    rw_lapack_int_t ifst = from;
    rw_lapack_int_t ilst = to;
    rw_lapack_int_t info;
    RWTWorkBuffer<double> work(n);
    trexc('V', (rw_lapack_int_t)n, T_.data(), T_.colStride(), Z_.data(), Z_.colStride(), ifst, ilst, work.data(), info);
    RWPOSTCONDITION(info == 0 || info == 1);
    return (int) ilst;
}

template <class TypeT>
bool RWSchurDecomp<TypeT>::moveToFront(const RWMathVec<int>& indices)
{
    if (!good()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_SCHURREORDER)));
    }
    unsigned n = rows();
    RWSelectVec select(n, 0);
    select.pick(indices) = 1;
    RWMathVec<TypeT> wr(n, rwUninitialized);
    RWMathVec<TypeT> wi(n, rwUninitialized);
    rw_lapack_int_t m;
    double s, sep;

    // casting to unsigned long to prevent overflows
    unsigned long lwork = (unsigned long)n * (n + 1) / 2;
    RWTWorkBuffer<TypeT> work(n);
    rw_lapack_int_t ldt = T_.colStride();
    rw_lapack_int_t ldz = Z_.colStride();
    rw_lapack_int_t info;
    RWTWorkBuffer<rw_lapack_int_t> iwork(lwork);
    trsen('N', 'V', select.data(), (rw_lapack_int_t)n, T_.data(), ldt, Z_.data(), ldz,
          wr.data(), wi.data(), m, s, sep, work.data(), (rw_lapack_int_t)lwork, iwork.data(), (rw_lapack_int_t)lwork, info);
    RWPOSTCONDITION(info == 0 || info == 1);
#if !defined(RW_NO_CONVERSION_TO_SELF)
    real(w_) = wr;
    imag(w_) = wi;
#else
    real(w_) = RWConvertMathVec<TypeT, double>(wr);
    imag(w_) = RWConvertMathVec<TypeT, double>(wi);
#endif
    return info == 0;
}
//specialize for DComplex:
template <>
inline bool RWSchurDecomp<DComplex>::moveToFront(const RWMathVec<int>& indices)
{
    return moveToFront_specialized(indices);
}

template <class TypeT>
bool RWSchurDecomp<TypeT>::moveToFront_specialized(const RWMathVec<int>& indices)
{
    if (!good()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_SCHURREORDER)));
    }
    unsigned n = rows();
    RWSelectVec select(n, 0);
    select.pick(indices) = 1;
    rw_lapack_int_t m;
    double s, sep;

    // casting to unsigned long to prevent overflows
    unsigned long lwork = (unsigned long)n * (n + 1) / 2;
    RWTWorkBuffer<DComplex> work(lwork);
    rw_lapack_int_t ldt = T_.colStride();
    rw_lapack_int_t ldz = Z_.colStride();
    rw_lapack_int_t info;
    trsen('N', 'V', select.data(), (rw_lapack_int_t)n, T_.data(), ldt, Z_.data(), ldz,
          w_.data(), m, s, sep, work.data(), (rw_lapack_int_t)lwork, info);
    RWPOSTCONDITION(info == 0 || info == 1);
    return info == 0;
}

