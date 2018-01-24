/*
 * Implementation of RWSchurEigServer
 *
 *
 * $Id: //lapack/13/rw/lapack/eigsch.cc#1 $
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


#include <rw/lapack/lapkfunc.h>
#include <rw/lapack/schur.h>
#include <rw/lapkerr.h>
#include <rw/math/rwalloc.h>
#include <rw/rwerr.h>

#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable:4018)
#endif

template <class TypeT>
RWSchurEigServer<TypeT>::RWSchurEigServer(bool left, bool right, bool sc, bool perm)
    : selectRange_((const RWSlice&)RWAll)   // Explicit cast prevents SUN error
{
    computeLeftVecs_  = left;
    computeRightVecs_ = right;
    scale_            = sc;
    permute_          = perm;
}

template <class TypeT>
bool RWSchurEigServer<TypeT>::computeLeftEigenVectors() const
{
    return computeLeftVecs_;
}
template <class TypeT>
void      RWSchurEigServer<TypeT>::computeLeftEigenVectors(bool x)
{
    computeLeftVecs_ = x;
}
template <class TypeT>
bool RWSchurEigServer<TypeT>::computeRightEigenVectors() const
{
    return computeRightVecs_;
}
template <class TypeT>
void      RWSchurEigServer<TypeT>::computeRightEigenVectors(bool x)
{
    computeRightVecs_ = x;
}
template <class TypeT>
bool RWSchurEigServer<TypeT>::scale() const
{
    return scale_;
}
template <class TypeT>
void      RWSchurEigServer<TypeT>::scale(bool x)
{
    scale_ = x;
}
template <class TypeT>
bool RWSchurEigServer<TypeT>::permute() const
{
    return permute_;
}
template <class TypeT>
void      RWSchurEigServer<TypeT>::permute(bool x)
{
    permute_ = x;
}
template <class TypeT>
void      RWSchurEigServer<TypeT>::balance(bool x)
{
    scale_ = permute_ = x;
}

template <class TypeT>
void RWSchurEigServer<TypeT>::selectEigenVectors(const RWMathVec<int>& s)
{
    selectVec_.reference(s.copy());
    selectRange_ = RWAll;
}

template <class TypeT>
void RWSchurEigServer<TypeT>::selectEigenVectors(const RWSlice& s)
{
    selectRange_ = s;
    selectVec_.reshape(0);
}

template <class TypeT>
bool RWSchurEigServer<TypeT>::computeAllEigenVectors() const
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
RWEigDecomp<TypeT> RWSchurEigServer<TypeT>::operator()(const RWGenMat<TypeT>& A)
{
    RWSchurDecomp<TypeT> x(A, permute_, scale_);
    return (*this)(x);
}

template <class TypeT>
RWEigDecomp<TypeT> RWSchurEigServer<TypeT>::operator()(const RWBalanceDecomp<TypeT>& A)
{
    RWSchurDecomp<TypeT> x(A);
    return (*this)(x);
}

template <class TypeT>
RWEigDecomp<TypeT> RWSchurEigServer<TypeT>::operator()(const RWHessenbergDecomp<TypeT>& A)
{
    RWSchurDecomp<TypeT> x(A);
    return (*this)(x);
}

template <class TypeT>
RWEigDecomp<TypeT> RWSchurEigServer<TypeT>::operator()(const RWSchurDecomp<TypeT>& A)
{
    // First build a container and set up the eigenvalues
    RWEigDecomp<TypeT> eig;
    this->n(eig) = A.rows();
    this->lambda(eig).reference(A.eigenvalues());

    if (computeAllEigenVectors()) {
        RWGenMat<TypeT> vl;
        RWGenMat<TypeT> vr;
        if (computeLeftVecs_) {
            vl.reshape(A.rows(), A.rows());
        }
        if (computeRightVecs_) {
            vr.reshape(A.rows(), A.rows());
        }
        if (computeLeftVecs_ || computeRightVecs_) {
            char job = (computeLeftVecs_ ? (computeRightVecs_ ? 'B' : 'L') : 'R');
            RWGenMat<TypeT> T = A.T();
            rw_lapack_int_t ldvl = rwmax(1, vl.colStride());  // max to cover the case of colStride zero
            rw_lapack_int_t ldvr = rwmax(1, vr.colStride());
            rw_lapack_int_t m;
            rw_lapack_int_t info;
            RWTWorkBuffer<TypeT> work(3 * A.rows());
            trevc(job, 'A', 0, A.rows(), T.data(), T.colStride(), vl.data(), ldvl,
                  vr.data(), ldvr, A.rows(), m, work.data(), info);
            RWPOSTCONDITION(info == 0);
            // Now transform and de-balance the eigenvectors
            if (computeLeftVecs_) {
                this->P(eig).reference(A.BInvTX(A.ZX(vl)));
            }
            if (computeRightVecs_) {
                this->Q(eig).reference(A.BX(A.ZX(vr)));
            }
        }
    }
    else {

        // Compute only selected eigenvectors
        //RWMathVec<int> select(n(eig),0);
        // the line below replaced the line above on 11/20/2001  --Omon
        RWSelectVec select(this->n(eig), 0);
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
            char job = (computeLeftVecs_ ? (computeRightVecs_ ? 'B' : 'L') : 'R');
            RWGenMat<TypeT> T = A.T();
            rw_lapack_int_t ldvl = rwmax(1, vl.colStride());  // max to cover the case of colStride zero
            rw_lapack_int_t ldvr = rwmax(1, vr.colStride());
            rw_lapack_int_t m;
            rw_lapack_int_t info;
            //RWMathVec<int> selectcopy = select.copy();     // Because dtrevc modifies select
            RWSelectVec selectcopy = select.copy();     // Because dtrevc modifies select   --this line replaced the line above on 11/30/2001
            RWTWorkBuffer<TypeT> work(3 * A.rows());
            trevc(job, 'S', selectcopy.data(), A.rows(), T.data(), T.colStride(), vl.data(), ldvl,
                  vr.data(), ldvr, mm, m, work.data(), info);
            RWPOSTCONDITION(info == 0);
            RWPOSTCONDITION(m == mm);
            // Now transform and de-balance the eigenvectors
            if (computeLeftVecs_) {
                this->P(eig).reference(A.BInvTX(A.ZX(vl)));
            }
            if (computeRightVecs_) {
                this->Q(eig).reference(A.BX(A.ZX(vr)));
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
    }
    return eig;
}

// specialize for DComplex:
template <>
inline RWEigDecomp<DComplex> RWSchurEigServer<DComplex>::operator()(const RWSchurDecomp<DComplex>& A)
{
    return op_specialized(A);
}

template <class TypeT>
RWEigDecomp<TypeT> RWSchurEigServer<TypeT>::op_specialized(const RWSchurDecomp<TypeT>& A)
{
    // First build a container and set up the eigenvalues
    RWEigDecomp<DComplex> eig;
    this->n(eig) = A.rows();
    (this->lambda(eig)).reference(A.eigenvalues());

    if (computeAllEigenVectors()) {
        RWGenMat<DComplex> vl;
        RWGenMat<DComplex> vr;
        if (computeLeftVecs_) {
            vl.reshape(A.rows(), A.rows());
        }
        if (computeRightVecs_) {
            vr.reshape(A.rows(), A.rows());
        }
        if (computeLeftVecs_ || computeRightVecs_) {
            char job = (computeLeftVecs_ ? (computeRightVecs_ ? 'B' : 'L') : 'R');
            RWGenMat<DComplex> T = A.T();
            rw_lapack_int_t ldvl = rwmax(1, vl.colStride());  // max to cover the case of colStride zero
            rw_lapack_int_t ldvr = rwmax(1, vr.colStride());
            rw_lapack_int_t m;
            rw_lapack_int_t info;
            RWTWorkBuffer<DComplex> work(3 * A.rows());
            RWTWorkBuffer<double> rwork(A.rows());
            trevc(job, 'A', 0, A.rows(), T.data(), T.colStride(), vl.data(), ldvl,
                  vr.data(), ldvr, A.rows(), m, work.data(), rwork.data(), info);
            RWPOSTCONDITION(info == 0);
            // Now transform and de-balance the eigenvectors
            if (computeLeftVecs_) {
                (this->P(eig)).reference(A.BInvTX(A.ZX(vl)));
            }
            if (computeRightVecs_) {
                (this->Q(eig)).reference(A.BX(A.ZX(vr)));
            }
        }
    }
    else {

        // Compute only selected eigenvectors
        //RWMathVec<int> select(this->n(eig),0);    // search for all the RWMathVec<int> and change them to Vec<int> ???????
        RWSelectVec select(this->n(eig), 0);  // this line replaced the line above on 11/20/2001  --Omon
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

        rw_lapack_int_t mm = 0;      // compute number of eigenvectors desired
        for (int i = int(select.length()); i--;) {
            if (select(i)) {
                mm++;
            }
        }
        RWGenMat<DComplex> vl;
        RWGenMat<DComplex> vr;
        if (computeLeftVecs_) {
            vl.reshape(A.rows(), (unsigned)mm);
        }
        if (computeRightVecs_) {
            vr.reshape(A.rows(), (unsigned)mm);
        }
        if (mm > 0 && (computeLeftVecs_ || computeRightVecs_)) {
            char job = (computeLeftVecs_ ? (computeRightVecs_ ? 'B' : 'L') : 'R');
            RWGenMat<DComplex> T = A.T();
            rw_lapack_int_t ldvl = rwmax(1, vl.colStride());  // max to cover the case of colStride zero
            rw_lapack_int_t ldvr = rwmax(1, vr.colStride());
            rw_lapack_int_t m;
            rw_lapack_int_t info;
            //RWMathVec<int> selectcopy = select.copy();     // Because dtrevc modifies select
            RWSelectVec selectcopy = select.copy();     // Because dtrevc modifies select   --this line replaced the line above on 11/30/2001
            RWTWorkBuffer<DComplex> work(3 * A.rows());
            RWTWorkBuffer<double> rwork(A.rows());
            trevc(job, 'S', selectcopy.data(), A.rows(), T.data(), T.colStride(), vl.data(), ldvl,
                  vr.data(), ldvr, mm, m, work.data(), rwork.data(), info);
            RWPOSTCONDITION(info == 0);
            RWPOSTCONDITION(m == mm);
            // Now transform and de-balance the eigenvectors
            if (computeLeftVecs_) {
                (this->P(eig)).reference(A.BInvTX(A.ZX(vl)));
            }
            if (computeRightVecs_) {
                (this->Q(eig)).reference(A.BX(A.ZX(vr)));
            }

            // Finally, move the eigenvalues corresponding the the chosen
            // eigenvectors to the front so the positions match up.  Do this
            // in two passes.
            RWMathVec<DComplex> temp = this->lambda(eig);
            (this->lambda(eig)).deepenShallowCopy();
            int index = 0;
            unsigned i;
            for (i = 0; i < temp.length(); i++) {
                if (select(i)) {
                    (this->lambda(eig))(index++) = temp(i);
                }
            }
            for (i = 0; i < temp.length(); i++) {
                if (!select(i)) {
                    (this->lambda(eig))(index++) = temp(i);
                }
            }
            RWPOSTCONDITION(static_cast<size_t>(index) == this->lambda(eig).length());
        }
    }
    return eig;
}
// end of specialization

#ifdef _WIN32
#  pragma warning(pop)
#endif
