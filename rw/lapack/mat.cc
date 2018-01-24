/*
 *  Global functions used by the matrix classes.
 *
 * $Id: //lapack/13/rw/lapack/mat.cc#1 $
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

#include <rw/rstream.h>
#include <rw/dcomplex.h>
#include <rw/rwerr.h>
#include <rw/lapkerr.h>

template < class TypeT >
TypeT RWRORef<TypeT>::error(TypeT x)
{
    RWTHROW(RWInternalErr(RWMessage(RWLAPK_CHANGEDCONSTANT, TypeT(*data), TypeT(x))));
    return *data;
}
// specialize for DComplex:
template <>
inline DComplex RWRORef<DComplex>::error(DComplex x)
{
    RWTHROW(RWInternalErr(RWMessage(RWLAPK_CHANGEDCOMPLEXCONSTANT, RW_MATH_SL_STD_GLOBAL(real)(*data), RW_MATH_SL_STD_GLOBAL(imag)(*data), RW_MATH_SL_STD_GLOBAL(real)(x), RW_MATH_SL_STD_GLOBAL(imag)(x))));
    return *data;
}

template < class TypeT >
inline RWRORef<TypeT>& RWRORef<TypeT>::operator=(TypeT x)
{
    if (readonly) {
        *data = error(x);
    }
    else {
        *data = x;
    }
    return *this;
}


template <class TypeT>  // this is for Complex classes only
TypeT RWROCJRef<TypeT>::error(TypeT x)
{
    RWTHROW(RWInternalErr(RWMessage(RWLAPK_CHANGEDCOMPLEXCONSTANT, real(*data), imag(*data), real(x), imag(x))));
    return *data;
}
