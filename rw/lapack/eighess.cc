/*
 * Implementation of RWHessEigServer
 *
 *
 * $Id: //lapack/13/rw/lapack/eighess.cc#1 $
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
#include <rw/math/rwalloc.h>
#include <rw/rwerr.h>

#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable:4018)
#endif

template <class TypeT>
RWHessEigServer<TypeT>::RWHessEigServer(bool left, bool right, bool sc, bool perm)
    : selectRange_((const RWSlice&)RWAll)
{
    computeLeftVecs_  = left;
    computeRightVecs_ = right;
    scale_            = sc;
    permute_          = perm;
}

template <class TypeT>
bool RWHessEigServer<TypeT>::computeLeftEigenVectors() const
{
    return computeLeftVecs_;
}
template <class TypeT>
void      RWHessEigServer<TypeT>::computeLeftEigenVectors(bool x)
{
    computeLeftVecs_ = x;
}
template <class TypeT>
bool RWHessEigServer<TypeT>::computeRightEigenVectors() const
{
    return computeRightVecs_;
}
template <class TypeT>
void      RWHessEigServer<TypeT>::computeRightEigenVectors(bool x)
{
    computeRightVecs_ = x;
}
template <class TypeT>
bool RWHessEigServer<TypeT>::scale() const
{
    return scale_;
}
template <class TypeT>
void      RWHessEigServer<TypeT>::scale(bool x)
{
    scale_ = x;
}
template <class TypeT>
bool RWHessEigServer<TypeT>::permute() const
{
    return permute_;
}
template <class TypeT>
void      RWHessEigServer<TypeT>::permute(bool x)
{
    permute_ = x;
}
template <class TypeT>
void      RWHessEigServer<TypeT>::balance(bool x)
{
    scale_ = permute_ = x;
}

template <class TypeT>
void RWHessEigServer<TypeT>::selectEigenVectors(const RWMathVec<int>& s)
{
    selectVec_.reference(s.copy());
    selectRange_ = RWAll;
}

template <class TypeT>
void RWHessEigServer<TypeT>::selectEigenVectors(const RWSlice& s)
{
    selectRange_ = s;
    selectVec_.reshape(0);
}

template <class TypeT>
bool RWHessEigServer<TypeT>::computeAllEigenVectors() const
{
    // The next line needs to determine if the RWSlice object selectRange_
    // is a copy of RWAll.  This means it will have a begin of zero, a stride
    // of one, and a length which matches whatever length the argument vector
    // is.  Test for this by asking for two lengths, if the slice gives back
    // the two lengths, then it must be an RWAll.
    bool isRWAll = (selectRange_.begin() == 0 && selectRange_.stride() == 1 && selectRange_.len(5) == 5 && selectRange_.len(6) == 6);
    return (isRWAll && selectVec_.length() == 0);
}

template <class TypeT>
RWEigDecomp<TypeT> RWHessEigServer<TypeT>::operator()(const RWGenMat<TypeT>& A)
{
    RWHessenbergDecomp<TypeT> x(A, permute_, scale_);
    return (*this)(x);
}

template <class TypeT>
RWEigDecomp<TypeT> RWHessEigServer<TypeT>::operator()(const RWBalanceDecomp<TypeT>& A)
{
    RWHessenbergDecomp<TypeT> x(A);
    return (*this)(x);
}

template <class TypeT>
RWEigDecomp<TypeT> RWHessEigServer<TypeT>::setEigenVals(const RWHessenbergDecomp<TypeT>& A, RWMathVec<double>* wr, RWMathVec<double>* wi)
{
    RWEigDecomp<TypeT> eig;
    this->n(eig) = A.rows();
    rw_lapack_int_t n = A.rows();
    RWGenMat<TypeT> H = A.H();
    H.deepenShallowCopy();   // just in case the interface changes
    this->lambda(eig).reshape(A.rows());
    wr->reshape(A.rows());
    wi->reshape(A.rows());
    rw_lapack_int_t ilo = A.balanceTransform().lowIndex();
    rw_lapack_int_t ihi = A.balanceTransform().highIndex();
    RWPRECONDITION(n >= 0);
    RWTWorkBuffer<TypeT> work(static_cast<size_t>(n));
    rw_lapack_int_t info;
    hseqr('E', 'N', n, ilo, ihi, H.data(), H.colStride(), wr->data(), wi->data(), 0, 1, work.data(), n, info);
    RWPOSTCONDITION(info >= 0);
    if (info > 0) {
        RWPOSTCONDITION(ilo - 1 + n - info < n);
        this->lambda(eig).reshape((size_t)(ilo - 1 + n - info));
        (*wr)(RWSlice((int)ilo, (int)(n - info))) = (*wr)(RWToEnd((int)info));
        wr->reshape((size_t)(ilo - 1 + n - info));
        (*wi)(RWSlice((int)ilo, (int)(n - info))) = (*wi)(RWToEnd((int)info));
        wi->reshape((size_t)(ilo - 1 + n - info));
        this->computedAll(eig) = false;
    }
    real(this->lambda(eig)) = *wr;
    imag(this->lambda(eig)) = *wi;
    return eig;
}
//specialize for DComplex:
template <>
inline RWEigDecomp<DComplex> RWHessEigServer<DComplex>::setEigenVals(const RWHessenbergDecomp<DComplex>& A, RWMathVec<double>* wr, RWMathVec<double>* wi)
{
    return setEigenVals_specialized(A, wr, wi);
}

template <class TypeT>
RWEigDecomp<TypeT> RWHessEigServer<TypeT>::setEigenVals_specialized(const RWHessenbergDecomp<TypeT>& A, RWMathVec<double>*, RWMathVec<double>*)
{
    RWEigDecomp<DComplex> eig;
    this->n(eig) = A.rows();
    rw_lapack_int_t n = A.rows();
    RWGenMat<DComplex> H = A.H();
    H.deepenShallowCopy();   // just in case the interface changes
    (this->lambda(eig)).reshape(A.rows());
    rw_lapack_int_t ilo = A.balanceTransform().lowIndex();
    rw_lapack_int_t ihi = A.balanceTransform().highIndex();
    RWPRECONDITION(n >= 0);
    RWTWorkBuffer<DComplex> work(static_cast<size_t>(n));
    rw_lapack_int_t info;
    hseqr('E', 'N', n, ilo, ihi, H.data(), H.colStride(), this->lambda(eig).data(), 0, 1, work.data(), n, info);
    RWPOSTCONDITION(info >= 0);
    if (info > 0) {
        RWPOSTCONDITION(ilo - 1 + n - info < n);
        this->lambda(eig)(RWSlice((int)ilo, (int)(n - info))) = this->lambda(eig)(RWToEnd((int)info));
        (this->lambda(eig)).reshape((size_t)(ilo - 1 + n - info));
    }
    return eig;
}

//end of specialization

template <class TypeT>
RWEigDecomp<TypeT> RWHessEigServer<TypeT>::operator()(const RWHessenbergDecomp<TypeT>& A)
{
    // First build a container and set up the eigenvalues
    RWMathVec<TypeT> wr, wi;
    RWEigDecomp<TypeT> eig = setEigenVals(A, &wr, &wi);
    if (!eig.good()) {
        return eig;    // in case we didn't get the eigenvalues
    }


    // Now determine which eigenvectors we want
    // RWMathVec<int> select(n(eig),0);
    // the line below replaced the line above on 11/20/2001  --Omon
    RWSelectVec select(this->n(eig), 0);

    if (computeAllEigenVectors()) {
        select = 1;
    }
    else {
        if (selectVec_.length() != 0) {
            for (int i = int(selectVec_.length()); i--;) {
                int x = selectVec_(i);
                if (x < 0 || (unsigned)x >= this->n(eig)) {
                    RWTHROW(RWBoundsErr(RWMessage(RWLAPK_NOEIG, x)));
                }
                select(x) = 1;
            }
        }
        else {    // which to compute is indicated by the RWSlice
            selectRange_.boundsCheck(this->n(eig));
            select(selectRange_) = 1;
        }
        // Make sure that both halves of complex conjugate eigenvals are chosen
        for (int j = int(select.length()); j--;) {
            if (select(j)) {
                DComplex eval = eig.eigenValue(j);
                if (RW_MATH_SL_STD_GLOBAL(imag)(eval) != 0) {
                    if (j > 0 && RW_MATH_SL_STD_GLOBAL(real)(eval) == RW_MATH_SL_STD_GLOBAL(real)(eig.eigenValue(j - 1))) {
                        select(j - 1) = 1;
                    }
                    else {
                        RWPOSTCONDITION(j + 1 < (int)eig.rows() && RW_MATH_SL_STD_GLOBAL(real)(eval) == RW_MATH_SL_STD_GLOBAL(real)(eig.eigenValue(j + 1))); // complex must be in a pair
                        select(j + 1) = 1;
                    }
                }
            }
        }

    }

    rw_lapack_int_t mm = 0;      // compute number of eigenvectors desired
    for (int i = int(select.length()); i--;) {
        if (select(i)) {
            mm++;
        }
    }
    RWGenMat<TypeT> vl;
    RWGenMat<TypeT> vr;
    if (computeLeftVecs_) {
        vl.reshape(A.rows(), (unsigned)mm);
    }
    if (computeRightVecs_) {
        vr.reshape(A.rows(), (unsigned)mm);
    }
    if (mm > 0 && (computeLeftVecs_ || computeRightVecs_)) {
        rw_lapack_int_t n = A.rows();
        char job = (computeLeftVecs_ ? (computeRightVecs_ ? 'B' : 'L') : 'R');
        RWGenMat<TypeT> H = A.H();
        rw_lapack_int_t ldh = H.colStride();
        rw_lapack_int_t ldvl = rwmax(1, vl.colStride());  // max to cover the case of colStride zero
        rw_lapack_int_t ldvr = rwmax(1, vr.colStride());
        rw_lapack_int_t m;
        rw_lapack_int_t info;
        //RWMathVec<int> selectcopy = select.copy();     // Because dtrevc modifies select
        RWSelectVec selectcopy = select.copy();     // Because dtrevc modifies select   --this line replaced the line above on 11/30/2001
        RWPRECONDITION(n >= 0);
        RWPRECONDITION(mm >= 0);
        RWTWorkBuffer<TypeT> work(static_cast<size_t>((n + 2)*n));
        RWTWorkBuffer<rw_lapack_int_t> ifaill(static_cast<size_t>(mm));
        RWTWorkBuffer<rw_lapack_int_t> ifailr(static_cast<size_t>(mm));
        hsein(job, 'Q', 'N', selectcopy.data(), n, H.data(), ldh,
              wr.data(), wi.data(), vl.data(), ldvl,
              vr.data(), ldvr, mm, m, work.data(), ifaill.data(), ifailr.data(), info) ;
        RWPOSTCONDITION(info >= 0);
        if (info > 0) {
            this->accurate(eig) = false;
        }
        RWPOSTCONDITION(m == mm);
        // Now de-balance the eigenvectors
        if (computeLeftVecs_) {
            this->P(eig).reference(A.BInvTX(A.QX(vl)));
        }
        if (computeRightVecs_) {
            this->Q(eig).reference(A.BX(A.QX(vr)));
        }

        // Finally, move the eigenvalues corresponding the the chosen
        // eigenvectors to the front so the positions match up.  Do this
        // in two passes.
        RWMathVec<DComplex> temp = this->lambda(eig);
        this->lambda(eig).deepenShallowCopy();
        int index = 0;
        unsigned i;
        for (i = 0; i < temp.length(); i++) {
            if (select(i)) {
                this->lambda(eig)(index++) = temp(i);
            }
        }
        for (i = 0; i < temp.length(); i++) {
            if (!select(i)) {
                this->lambda(eig)(index++) = temp(i);
            }
        }
        RWPOSTCONDITION(static_cast<size_t>(index) == this->lambda(eig).length());
    }
    return eig;
}


// specialize for DComplex
template <>
inline RWEigDecomp<DComplex> RWHessEigServer<DComplex>::operator()(const RWHessenbergDecomp<DComplex>& A)
{
    return op_specialized(A);
}


template <class TypeT>
RWEigDecomp<TypeT> RWHessEigServer<TypeT>::op_specialized(const RWHessenbergDecomp<TypeT>& A)
{
    // First build a container and set up the eigenvalues
    RWEigDecomp<TypeT> eig = setEigenVals(A);
    if (!eig.good()) {
        return eig;    // in case we didn't get the eigenvalues
    }

    // Now determine which eigenvectors we want
    //RWMathVec<int> select(n(eig),0);
    // the line below replaced the line above on 11/20/2001  --Omon
    RWSelectVec select(this->n(eig), 0);
    if (computeAllEigenVectors()) {
        select = 1;
    }
    else {
        if (selectVec_.length() != 0) {
            for (int i = int(selectVec_.length()); i--;) {
                int x = selectVec_(i);
                if (x < 0 || (unsigned)x >= this->n(eig)) {
                    RWTHROW(RWBoundsErr(RWMessage(RWLAPK_NOEIG, x)));
                }
                select(x) = 1;
            }
        }
        else {    // which to compute is indicated by the RWSlice
            selectRange_.boundsCheck(this->n(eig));
            select(selectRange_) = 1;
        }

    }

    rw_lapack_int_t mm = 0;      // compute number of eigenvectors desired

    for (int i = int(select.length()); i--;) {
        if (select(i)) {
            mm++;
        }
    }

    RWGenMat<TypeT> vl;
    RWGenMat<TypeT> vr;
    if (computeLeftVecs_) {
        vl.reshape(A.rows(), (unsigned)mm);
    }
    if (computeRightVecs_) {
        vr.reshape(A.rows(), (unsigned)mm);
    }
    if (mm > 0 && (computeLeftVecs_ || computeRightVecs_)) {
        rw_lapack_int_t n = A.rows();
        char job = (computeLeftVecs_ ? (computeRightVecs_ ? 'B' : 'L') : 'R');
        RWGenMat<TypeT> H = A.H();
        rw_lapack_int_t ldh = H.colStride();
        rw_lapack_int_t ldvl = rwmax(1, vl.colStride());  // max to cover the case of colStride zero
        rw_lapack_int_t ldvr = rwmax(1, vr.colStride());
        rw_lapack_int_t m;
        rw_lapack_int_t info;
        //RWMathVec<int> selectcopy = select.copy();     // Because dtrevc modifies select
        RWSelectVec selectcopy = select.copy();     // Because dtrevc modifies select
        RWPRECONDITION(n >= 0);
        RWPRECONDITION(mm >= 0);
        RWTWorkBuffer<TypeT> work(static_cast<size_t>((n + 2)*n));
        RWTWorkBuffer<rw_lapack_int_t> ifaill(static_cast<size_t>(mm));
        RWTWorkBuffer<rw_lapack_int_t> ifailr(static_cast<size_t>(mm));
        RWTWorkBuffer<double> rwork(static_cast<size_t>(n));
        //RWMathVec<TypeT> mytmp = RWHessEigServer<TypeT>::lambda(eig);

        hsein(job, 'Q', 'N', selectcopy.data(), n, H.data(), ldh,
              this->lambda(eig).data(), vl.data(), ldvl,
              vr.data(), ldvr, mm, m, work.data(), rwork.data(), ifaill.data(), ifailr.data(), info) ;
        RWPOSTCONDITION(info >= 0);
        if (info > 0) {
            this->accurate(eig) = false;
        }
        RWPOSTCONDITION(m == mm);
        // Now de-balance the eigenvectors
        if (computeLeftVecs_) {
            this->P(eig).reference(A.BInvTX(A.QX(vl)));
        }
        if (computeRightVecs_) {
            this->Q(eig).reference(A.BX(A.QX(vr)));
        }

        // Finally, move the eigenvalues corresponding the the chosen
        // eigenvectors to the front so the positions match up.  Do this
        // in two passes.
        RWMathVec<TypeT> temp = this->lambda(eig);
        this->lambda(eig).deepenShallowCopy();
        unsigned index = 0;
        unsigned i;
        for (i = 0; i < temp.length(); i++) {
            if (select(i)) {
                this->lambda(eig)(index++) = temp(i);
            }
        }
        for (i = 0; i < temp.length(); i++) {
            if (!select(i)) {
                this->lambda(eig)(index++) = temp(i);
            }
        }
        RWPOSTCONDITION(index == this->lambda(eig).length());
    }
    return eig;
}
// end of specialization

#ifdef _WIN32
#  pragma warning(pop)
#endif
