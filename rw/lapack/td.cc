/*
 * Implementation of RWHermTriDiagDecomp
 *
 * $Id: //lapack/13/rw/lapack/td.cc#1 $
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
 */

#include <rw/lapkerr.h>
#include <rw/rwerr.h>


template <class TypeT>
void RWTriDiagDecomp<TypeT>::set(const RWMathVec<rt>& diag_, const RWMathVec<rt>& offdiag_)
{
    if (offdiag_.length() + 1 != diag_.length()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_OFFDIAG, offdiag_.length(), diag_.length() - 1)));
    }
    diag.reference(diag_);
    offdiag.reference(offdiag_);
}

template <class TypeT>
RWMathVec<TypeT> RWTriDiagDecomp<TypeT>::transform(const RWMathVec<rt>& x) const
{
    RWGenMat<rt> C(x, x.length(), 1);
    RWGenMat<TypeT> QC = transform(C);
    RWPOSTCONDITION(QC.cols() == 1);
    return QC.col(0);
}


#include <rw/lapack/bandtd.cc>
#include <rw/lapack/densetd.cc>
