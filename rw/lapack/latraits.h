#ifndef __RWLATRAITS_H_
#define __RWLATRAITS_H_

/******************************************************************************
 *
 * Traits class for defining data types for linear algebra operations.
 *
 * $Id: //lapack/13/rw/lapack/latraits.h#1 $
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
 *****************************************************************************/

#include <rw/dcomplex.h>
#include <rw/lapkwind.h>

template <class TypeT>
class RWSymMat;

template <class TypeT>
class RWSymBandMat;

template <class TypeT>
class RWTriDiagMat;

template <class TypeT>
class RWHermMat;

template <class TypeT>
class RWSkewMat;

template <class TypeT>
class RWHermBandMat;

template <class TypeT>
class RWLowerTriMat;

template <class TypeT>
class RWUpperTriMat;


template <class TypeT>
class RWBandMat;

/**
 * @ingroup lapack_classes
 *
 * @brief A collection of traits typedefs.
 *
 * The rw_linear_algebra_traits class is a helper class that contains only
 * typedef information. The generic w_linear_algebra_traits<T> assumes that all
 * the typedefs are set to T. Since this is not desirable for any types, we
 * specialize for DComplex, double and float.
 *
 * The specializations used in the library are contained in the file
 * rw/math/latraits.h.
 *
 * There are twelve public typedefs in each specialized rw_linear_algebra_traits
 * class. Each typedef must be defined in a given specialization for proper
 * results.
 *
 * @section synopsis Synopsis
 *
 * @code
 * template <class T>
 * class rw_numeric_traits
 * @endcode
 *
 * @section specializations Specializations
 *
 * @code
 * rw_linear_algebra_traits<float>
 * rw_linear_algebra_traits<double>
 * rw_linear_algebra_traits<DComplex>
 * @endcode
 */
template <class TypeT>
class rw_linear_algebra_traits
{
public:
    typedef TypeT                real_type;

    /**
     * Type to represent a lower triangular matrix
     */
    typedef TypeT                narrow_lower_tri_mat;

    /**
     * Type to represent a upper triangular matrix
     */
    typedef TypeT                narrow_upper_tri_mat;

    /**
     * Type to represent a symmetric banded matrix
     */
    typedef TypeT                narrow_sym_band_mat;

    /**
     * Type to represent a tridiagonal matrix
     */
    typedef TypeT                generic_tri_diag_mat;

    /**
     * Type to represent a symmetric matrix
     */
    typedef TypeT                generic_sym_mat;

    /**
     * Type to represent a banded matrix
     */
    typedef TypeT                generic_band_mat;

    /**
     * Type to represent a skew matrix
     */
    typedef TypeT                generic_skew_mat;

    /**
     * Type to represent a symmetric hermitian matrix
     */
    typedef RWSymMat<TypeT>      hermitian_type;

    /**
     * Type to represent a symmetric banded hermitian matrix
     */
    typedef RWSymBandMat<TypeT>  banded_hermitian_type;

    typedef rw_lapack_int_t work_type;

    /**
     * Type to represent a Linear Algebra Package function
     */
    typedef mathFunTy lapkFunType;
};

/**
 * @ingroup lapack_classes
 *
 * @brief A collection of traits typedefs specialized on DComplex.
 *
 * @copydoc rw_linear_algebra_traits
 */
template <>
class rw_linear_algebra_traits <DComplex>
{
public:
    typedef double                  real_type;

    /**
     * @copydoc rw_linear_algebra_traits::narrow_lower_tri_mat
     */
    typedef RWLowerTriMat<double>   narrow_lower_tri_mat;

    /**
     * @copydoc rw_linear_algebra_traits::narrow_upper_tri_mat
     */
    typedef RWUpperTriMat<double>   narrow_upper_tri_mat;

    /**
     * @copydoc rw_linear_algebra_traits::narrow_sym_band_mat
     */
    typedef RWSymBandMat<double>    narrow_sym_band_mat;

    /**
     * @copydoc rw_linear_algebra_traits::generic_tri_diag_mat
     */
    typedef RWTriDiagMat<double>    generic_tri_diag_mat;

    /**
     * @copydoc rw_linear_algebra_traits::generic_sym_mat
     */
    typedef RWSymMat<double>        generic_sym_mat;

    /**
     * @copydoc rw_linear_algebra_traits::generic_band_mat
     */
    typedef RWBandMat<double>       generic_band_mat;

    /**
     * @copydoc rw_linear_algebra_traits::generic_skew_mat
     */
    typedef RWSkewMat<double>       generic_skew_mat;

    /**
     * @copydoc rw_linear_algebra_traits::hermitian_type
     */
    typedef RWHermMat<DComplex>     hermitian_type;

    /**
     * @copydoc rw_linear_algebra_traits::banded_hermitian_type
     */
    typedef RWHermBandMat<DComplex> banded_hermitian_type;

    typedef double work_type;

    /**
     * @copydoc rw_linear_algebra_traits::lapkFunType
     */
    typedef CmathFunTy lapkFunType;
};

/**
 * @ingroup lapack_classes
 *
 * @brief A collection of traits typedefs specialized on double.
 *
 * @copydoc rw_linear_algebra_traits
 */
template <>
class rw_linear_algebra_traits <double>
{
public:
    typedef double               real_type;

    /**
     * @copydoc rw_linear_algebra_traits::narrow_lower_tri_mat
     */
    typedef RWLowerTriMat<float> narrow_lower_tri_mat;

    /**
     * @copydoc rw_linear_algebra_traits::narrow_upper_tri_mat
     */
    typedef RWUpperTriMat<float> narrow_upper_tri_mat;

    /**
     * @copydoc rw_linear_algebra_traits::narrow_sym_band_mat
     */
    typedef RWSymBandMat<float>  narrow_sym_band_mat;

    /**
     * @copydoc rw_linear_algebra_traits::generic_tri_diag_mat
     */
    typedef RWTriDiagMat<float>  generic_tri_diag_mat;

    /**
     * @copydoc rw_linear_algebra_traits::generic_sym_mat
     */
    typedef RWSymMat<float>      generic_sym_mat;

    /**
     * @copydoc rw_linear_algebra_traits::generic_band_mat
     */
    typedef RWBandMat<float>     generic_band_mat;

    /**
     * @copydoc rw_linear_algebra_traits::generic_skew_mat
     */
    typedef RWSkewMat<float>     generic_skew_mat;

    /**
     * @copydoc rw_linear_algebra_traits::hermitian_type
     */
    typedef RWSymMat<double>     hermitian_type;

    /**
     * @copydoc rw_linear_algebra_traits::banded_hermitian_type
     */
    typedef RWSymBandMat<double> banded_hermitian_type;

    typedef rw_lapack_int_t work_type;


    /**
     * @copydoc rw_linear_algebra_traits::lapkFunType
     */
    typedef mathFunTy lapkFunType;
};

/**
 * @ingroup lapack_classes
 *
 * @brief A collection of traits typedefs specialized on float.
 *
 * @copydoc rw_linear_algebra_traits
 */
template <>
class rw_linear_algebra_traits <float>
{
public:
    typedef float               real_type;

    /**
     * @copydoc rw_linear_algebra_traits::narrow_lower_tri_mat
     */
    typedef RWLowerTriMat<int>  narrow_lower_tri_mat;

    /**
     * @copydoc rw_linear_algebra_traits::narrow_upper_tri_mat
     */
    typedef RWUpperTriMat<int>  narrow_upper_tri_mat;

    /**
     * @copydoc rw_linear_algebra_traits::narrow_sym_band_mat
     */
    typedef RWSymBandMat<int>   narrow_sym_band_mat;

    /**
     * @copydoc rw_linear_algebra_traits::generic_tri_diag_mat
     */
    typedef RWTriDiagMat<int>   generic_tri_diag_mat;

    /**
     * @copydoc rw_linear_algebra_traits::generic_sym_mat
     */
    typedef RWSymMat<int>       generic_sym_mat;

    /**
     * @copydoc rw_linear_algebra_traits::generic_band_mat
     */
    typedef RWBandMat<int>      generic_band_mat;

    /**
     * @copydoc rw_linear_algebra_traits::generic_skew_mat
     */
    typedef RWSkewMat<int>      generic_skew_mat;

    /**
     * @copydoc rw_linear_algebra_traits::hermitian_type
     */
    typedef RWSymMat<float>     hermitian_type;

    /**
     * @copydoc rw_linear_algebra_traits::banded_hermitian_type
     */
    typedef RWSymBandMat<float> banded_hermitian_type;

    typedef rw_lapack_int_t work_type;


    /**
     * @copydoc rw_linear_algebra_traits::lapkFunType
     */
    typedef mathFunTy lapkFunType;
};


//these are used in ????mat files
inline const char* join_str(const char* tname, const char* pname)
{
    size_t i = 0;
    size_t j = 0;
    for (i = 0; tname[ i ] != 0; i++) {};
    size_t n = 0;
    for (j = 0; pname[ j ] != 0; j++, n++) {};
    char* result = new char[ n + i + 1UL ];
    result[ n + i ] = '\0';
    for (j = 0; j < i; j++) {
        result[ j ] = tname[ j ];
    }
    for (j = 0; j < n; j++) {
        result[ i + j ] = pname[ j ];
    }

    return result;
}


//MSVC6.0 has trouble differentiating among different specializations of one template functions. Supplying additional
// parameter seems to help
// 10/17/2001 - Compaq compiler seems to have the same problem...
template <class TypeT>
const char* template_name(const char* tmplt
#if defined(_MSC_VER)
                          , TypeT dummy = 0
#endif
                         )
{
    const char* param = "<TypeT>";
    return join_str(tmplt, param);
}

template <>
inline const char* template_name<double>(const char* tmplt
#if defined(_MSC_VER)
        , double dummy
#endif
                                        )
{
    const char* param = "<double>";
    return join_str(tmplt, param);
}

template <>
inline const char* template_name<float>(const char* tmplt
#if defined(_MSC_VER)
                                        , float dummy
#endif
                                       )
{
    const char* param = "<float>";
    return join_str(tmplt, param);
}

template <>
inline const char* template_name<DComplex>(const char* tmplt
#if defined(_MSC_VER)
        , DComplex dummy
#endif
                                          )
{
    const char* param = "<DComplex>";
    return join_str(tmplt, param);
}

template <>
inline const char* template_name<int>(const char* tmplt
#if defined(_MSC_VER)
                                      , int dummy
#endif
                                     )
{
    const char* param = "<int>";
    return join_str(tmplt, param);
}

#endif // __RWLATRAITS_H_
