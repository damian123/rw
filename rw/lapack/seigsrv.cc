/*
 * Implementation of RWSymEigServer
 *
 * $Id: //lapack/13/rw/lapack/seigsrv.cc#1 $
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

//#include <rw/lapack/seigsrv.h>
#include <rw/lapack/td.h>


template <class TypeT>
RWSymEigDecomp<TypeT> RWSymEigServer<TypeT>::operator()(const RWSymMat<TypeT>& A)
{
    RWDenseTriDiagDecomp<TypeT> td(A);         // Build a tri-diagonal decomposition
    RWSymEigDecomp<TypeT> tdeig = decompose(td);  // Get eigenvals/vecs for tri-diagonal problem
    RWSymEigDecomp<TypeT> eig = tdeig;            // Needs to be done this way for complex numbers
    P(eig) = td.transform(P(tdeig));
    return eig;
}

template <class TypeT>
RWSymEigDecomp<TypeT> RWSymEigServer<TypeT>::operator()(const RWSymBandMat<TypeT>& A)
{
    RWBandTriDiagDecomp<TypeT> td(A);          // Build a tri-diagonal decomposition
    RWSymEigDecomp<TypeT> tdeig = decompose(td);  // Get eigenvals/vecs for tri-diagonal problem
    RWSymEigDecomp<TypeT> eig = tdeig;            // Needs to be done this way for complex numbers
    P(eig) = td.transform(P(tdeig));
    return eig;
}




#include <rw/lapack/seigbis.cc>
#include <rw/lapack/seigrf.cc>
#include <rw/lapack/seigpd.cc>
#include <rw/lapack/seigqr.cc>
