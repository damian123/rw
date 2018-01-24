/*
 * Implementation of RWHermDenseTriDiagDecomp
 *
 *
 * $Id: //lapack/13/rw/lapack/densetd.cc#1 $
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

#include <rw/dcomplex.h>
#include <rw/lapack/hermmat.h>
#include <rw/lapack/lapkfunc.h>
#include <rw/lapack/latraits.h>
#include <rw/lapkerr.h>
#include <rw/math/rwalloc.h>
#include <rw/rwerr.h>

template <class TypeT>
RWDenseTriDiagDecomp<TypeT>::RWDenseTriDiagDecomp(const typename rw_linear_algebra_traits<TypeT>::hermitian_type& A)
{
    typedef typename rw_numeric_traits<TypeT>::norm_type rt;
    if (A.rows() > 0) {
        char uplo = 'U';   // Indicates to lapack that upper triangle is stored
        RW_LAPACK_INT_MAX_ASSERT(A.rows());
        rw_lapack_int_t n = RW_STATIC_CAST(rw_lapack_int_t, A.rows());
        Qdata.reference(A.dataVec());
        Qdata.deepenShallowCopy();
        //RWMathVec<typename rw_numeric_traits<TypeT>::norm_type> D(A.rows(),rwUninitialized);
        //RWMathVec<typename rw_numeric_traits<TypeT>::norm_type> E(A.rows()-1,rwUninitialized);
        RWMathVec<rt> D(A.rows(), rwUninitialized);
        RWMathVec<rt> E(A.rows() - 1, rwUninitialized);
        tau.reshape(A.rows());
        rw_lapack_int_t info;
        sptrd(uplo, n, (TypeT*)Qdata.data(), (typename rw_numeric_traits<TypeT>::norm_type*)D.data(), (typename rw_numeric_traits<TypeT>::norm_type*)E.data(), (TypeT*)tau.data(), info);
        RWPOSTCONDITION(info == 0);
        this->set(D, E);
    }
}

template <class TypeT>
RWMathVec<TypeT> RWDenseTriDiagDecomp<TypeT>::transform(const RWMathVec < rt/*typename rw_numeric_traits<TypeT>::norm_type*/ > & V) const
{
    return RWTriDiagDecomp<TypeT>::transform(V);
}

template <class TypeT>
RWGenMat<TypeT> RWDenseTriDiagDecomp<TypeT>::transform(const RWGenMat < rt/*typename rw_numeric_traits<TypeT>::norm_type*/ > & Cinput) const
{
    RWGenMat<TypeT> C = Cinput.copy();
    C.deepenShallowCopy();
    if (C.rows() > 0) {
        if (C.rows() != this->rows()) {
            RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, C.rows(), this->rows())));
        }
        char side = 'L';
        char uplo = 'U';
        char trans = 'N';
        RW_ASSERT(C.rows() <= RW_LAPACK_INT_MAX);
        RW_ASSERT(C.cols() <= RW_LAPACK_INT_MAX);
        rw_lapack_int_t m = RW_STATIC_CAST(rw_lapack_int_t, C.rows());
        rw_lapack_int_t n = RW_STATIC_CAST(rw_lapack_int_t, C.cols());
        RWPRECONDITION(n >= 0);
        RWTWorkBuffer<TypeT> work(static_cast<size_t>(n));
        rw_lapack_int_t info;
        opmtr(side, uplo, trans, m, n, (TypeT*)Qdata.data(), (TypeT*)tau.data(), (TypeT*)C.data(), m, work.data(), info);
        RWPOSTCONDITION(info == 0);
    }
    return C;
}
//specialize for DComplex:
template <>
inline RWGenMat<DComplex> RWDenseTriDiagDecomp<DComplex>::transform(const RWGenMat<rw_numeric_traits<DComplex>::norm_type>& Cinput) const
{
    return transformSpecialized(Cinput);
}

template <class TypeT>
RWGenMat<DComplex> RWDenseTriDiagDecomp<TypeT>::transformSpecialized(const RWGenMat<double>& Cinput) const
{
    RWGenMat<DComplex> C = RWConvertGenMat<double, DComplex>(Cinput);
    C.deepenShallowCopy();
    if (C.rows() > 0) {
        if (C.rows() != this->rows()) {
            RWTHROW(RWInternalErr(RWMessage(RWLAPK_VECLENGTH, C.rows(), this->rows())));
        }
        char side = 'L';
        char uplo = 'U';
        char trans = 'N';
        RW_ASSERT(C.rows() <= RW_LAPACK_INT_MAX);
        RW_ASSERT(C.cols() <= RW_LAPACK_INT_MAX);
        rw_lapack_int_t m = RW_STATIC_CAST(rw_lapack_int_t, C.rows());
        rw_lapack_int_t n = RW_STATIC_CAST(rw_lapack_int_t, C.cols());
        RWPRECONDITION(n >= 0);
        RWTWorkBuffer<DComplex> work(static_cast<size_t>(n));
        rw_lapack_int_t info;
        opmtr(side, uplo, trans, m, n, (DComplex*)Qdata.data(), (DComplex*)tau.data(), (DComplex*)C.data(), m, work.data(), info);
        RWPOSTCONDITION(info == 0);
    }
    return C;
}

//end of specialization
