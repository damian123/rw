#ifndef __RWSVDCALC_H__
#define __RWSVDCALC_H__

/****************************************************************************
* Class RWSVDCalc
*
* Class for computing the Singular Value Decomposition of a general matrix
* A:
*
*                          [ s1                 ]
*                          [    .               ]
*            [           ] [      .             ] [   v1'   ]
*            [           ] [        .           ] [    .    ]
*        A = [ u1 ... um ] [          sr        ] [    .    ]
*            [           ] [             0      ] [    .    ]
*            [           ] [               .    ] [   vn'   ]
*                          [                 .  ]
*                          [                   .]
*
* where s1,...,sr are real positive numbers called the singular values
* of A; u1,...,um are the left singular vectors of A; and v1,...,vn are
* the right singular vectors of A.
* $Id: //lapack/13/rw/lapack/svdcalc.h#1 $
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
****************************************************************************/

#include <rw/math/numtrait.h>
#include <rw/mathwind.h>
#include <rw/lapkwind.h>

template< class TypeT >
class RWGenMat;

template< class TypeT >
class RWMathVec;

/**
 * @ingroup decomposition_classes
 * @brief Computes a singular value decomposition of a matrix based
 * on the LAPACK routine \c xgesvd.
 *
 * Computes a singular value decomposition of a matrix A. Based
 * on the LAPACK routine \c xgesvd. This class may be used as the
 * SVDCalc template parameter for the classes \link RWSVDecomp
 * RWSVDecomp<T,SVDCalc> \endlink, \link RWSVServer RWSVServer<T,SVDCalc>
 * \endlink and \link RWLeastSqSV RWLeastSqSV<T, SVDCalc> \endlink.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/lapack/svdcalc.h>
 * @endcode
 */
template < class TypeT >
class RWSVDCalc
{
private:
    /**
     * Typedef for the usual return type of numerical <tt>norm</tt>-like
     * functions. For more information, see rw_numeric_traits<T>::norm_type.
     */
    typedef typename rw_numeric_traits<TypeT>::norm_type norm_type;

public:
    /**
     * where #norm_type is a typedef for rw_numeric_traits<T>::norm_type.
     * The return value is \c true if the decomposition was successfully computed.
     *
     * @param A The input matrix for which the singular value decomposition
     * is being computed.
     *
     * @param U The output matrix of left singular victors (the columns of \a U
     * are the left singular vectors).
     *
     * @param VT The output matrix of right singular victors (the rows of \a VT
     * are the right singular victors).
     *
     * @param sigma The output vector of singular values in descending order.
     *
     * @param tolerance The input singular values with magnitude less than
     * \a tolerance will be set to zero.
     *
     * @param numLeftVectors The input number of left vectors to compute. If
     * the number is less than zero, the default number of vectors will be
     * computed (it is up to the developer to determine what the default is).
     *
     * @param numRightVectors The input number of right vectors to compute. If
     * the number is less than zero, the default number of vectors will be
     * computed (it is up to the developer to determine what the default is).
     */
    static bool computeSVD(const RWGenMat<TypeT>&    A,
                           RWGenMat<TypeT>&          U,
                           RWGenMat<TypeT>&          VT,
                           RWMathVec<norm_type>& sigma,
                           norm_type             tolerance = 0,
                           int numLeftVectors = -1,
                           int numRightVectors = -1);
};

#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/svdcalc.cc>
#endif

#endif // __RWSVDCALC_H__
