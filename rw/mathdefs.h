#ifndef __MATHDEFS_H__
#define __MATHDEFS_H__

/***************************************************************************
 *
 * mathdefs.h - Math.h++ common definitions
 *
 * $Id: //math/13/rw/mathdefs.h#4 $
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
#include <rw/defs.h>
#include <rw/config/rwconfig_mth.h>

const unsigned short versionID = 2;  // Formatting version number

#if defined(_WIN32)
#  define RW_MTH_MEMBER_MAX (max)
#  define RW_MTH_MEMBER_MIN (min)
#else
#  define RW_MTH_MEMBER_MAX max
#  define RW_MTH_MEMBER_MIN min
#endif


#define RWMATH_NO_EXPLICIT_INSTANTIATION 1
#define RWMATH_NO_INSTANTIATE 1

#if defined(__SUNPRO_CC)
#  define RW_MATH_COMPILE_INSTANTIATE 1
#else
#  if !defined(_AIX)
#    ifdef RW_COMPILE_INSTANTIATE
#      define RW_MATH_COMPILE_INSTANTIATE 1
#    endif
#  else
#    ifdef _RWBUILDSHARED
#      ifndef RW_MATH_DEFINE_SYMBOLS
#        define RW_MATH_DEFINE_SYMBOLS
#      endif
#    endif
#  endif
#endif

#if defined(_AIX)
#  if defined(__IBMCPP__)
// Tools currently don't support bool streaming
// so we need to specialize functions that cause problems
// for bool.
#    define RW_MATH_BOOL_TOOLS_IO_WORKAROUND
// Visual Age throw internal compiler error on some
// complicated statements
#    if (__IBMCPP__ == 500)
#      define RW_AIX_BUG_WORKAROUND
#    endif
#  endif

// VisualAge 5.0 uses compile-time instantiation so implementation
// files should be included in header files
#  define RW_MATH_COMPILE_INSTANTIATE 1
#  define RW_NEEDS_SPECLS_IN_LINK_FILE
#  define RW_INLINE_TMPL_FUNC
#else
#  define RW_INLINE_TMPL_FUNC inline
#endif


// VC 7.0 requires explicit instantiation of
// RWMathVec<DComplex> and RWGenMat<DComplex> or
// we will get get unresolved symbols in the tests.
// Updated for VC 9.0 (_MSC_VER = 1500)

#if defined(_MSC_VER)
#  if defined(_WIN64)
#    define RW_INLINE_TRAITS_TEMPLATES 1
#    define RW_INLINE_FUNC_PTR_ARG 1
#    if !defined(_USE_MATH_DEFINES)
#      define _USE_MATH_DEFINES 1
#    endif
#  endif
#  if (_MSC_VER >= 1300)
#    if defined(_RWBUILD_mth)
#      pragma warning ( disable : 4661 )
#    endif
#    if (_MSC_VER <= 1800)   // VC 10.0, 11.0 and 12.0 also require explicit instantiation.
#      undef RWMATH_NO_EXPLICIT_INSTANTIATION
#      undef RWMATH_NO_INSTANTIATE
#    endif
#    if !defined(_USE_MATH_DEFINES)
#      define _USE_MATH_DEFINES 1
#    endif
#  endif
#  if (_MSC_VER >= 1310)
#    define RW_INLINE_TRAITS_TEMPLATES 1
#    define RW_INLINE_FUNC_PTR_ARG 1
#    define RW_MATH_SCOPED_ITERATOR 1
#  endif
#  if (_MSC_VER < 1310)
#    define RWMATH_NO_LONGLONG_STREAM_OPERATOR 1
#  endif
#  if (_MSC_VER >= 1020) && (_MSC_VER < 1100)
#    define RW_OVERLOADS_POW 1
#  endif
#endif

#if defined(__INTEL_COMPILER)
#  if !defined(_USE_MATH_DEFINES)
#    define _USE_MATH_DEFINES 1
#  endif
#  if defined(__GNUG__) && (__INTEL_COMPILER==810)
#    define _GLIBCPP_BUGGY_COMPLEX 1
#  endif
#endif

#if defined(__hppa)
#  define RW_INLINE_TRAITS_TEMPLATES 1
#endif

/*
 * RWPivotType is a switch between in and long.  Linpack uses int*
 * while lapack uses long* pivots.  Some subroutines also pass an int*
 * info variable which is long* for Lapack routines.  Use RWPivotType
 * to distinguish between them
 */
//typedef long RWPivotType;

// Define compatibility macro
#define RWMATH     RW_MATH_VERSION

/*
 *     W I N D O W S - S P E C I F I C   C O D E

 * Deal with DLL related issues. Note that rwwind.h has already been
 * included in rw/defs.h and defines issues related to the Tools.h++
 * components used in Math.h++
 */
#include <rw/mathwind.h>


/*********************************************************************

                U S E R   T U N A B L E   S E C T I O N

 *********************************************************************/

/*
 * This section has various parameters that you might
 * have to set depending on your compiler and operating system.
 * Be sure to take a look at the user-tunable section in compiler.h as well.
 */

/*
 * Uncomment this line to allow vector, matrix, and array constructors
 * which take only the dimensions and no initializer.  This is provided
 * for backward compatibility.  In the new version of math.h++, these
 * constructors have been replaced by allowing rwUninitialized as an
 * initializer.  This cuts down un ambiguous and potentially unwanted
 * type conversions.
 */

/* #define RW_ALLOW_OLD_ARRAY_CONSTRUCTORS */

/*
 * Uncomment the following line if your compiler has "restricted" pointers.
 * This is relatively rare.  If you do not know what restricted pointers are,
 * you probably should leave this commented.
 */

/* #define HAS_RESTRICT 1 */

/*
 * Uncomment the following if your -> C <- compiler has "native" complex;
 * that is, complex is a type built directly into the C compiler,
 * NOT implemented as a separate class by a C++ compiler.  (This is a NCEG extension)
 */

/* #define HAS_NATIVE_COMPLEX 1 */

/*
 * Uncomment this to use obsolete LAPACK error handling mechanism.
 * When this macro is defined on error LAPACK functions will abort
 * process instead of throwing an exception.
 *
 */
/* #define RW_XERBLA_COMPAT 1 */

#ifdef RW_USING_SUNPERF

struct rw_floatcomplex {
    float re;
    float im;

    // this empty constructor is vital for SPL-Fortran functions
    rw_floatcomplex() {
        ;
    }
};

struct rw_doublecomplex {
    double re;
    double im;

    // this empty constructor is vital for SPL-Fortran functions
    rw_doublecomplex() {
        ;
    }
};

#  define floatcomplex            FComplex
#  define doublecomplex           DComplex

#endif // RW_USING_SUNPERF

/*
 * Define true if sizeof(Complex)==2*sizeof(double);
 * This is true for all known compilers:
 */
#define COMPLEX_PACKS 1

/*
 * Uncomment the following if your complex data goes (imaginary, real),
 * rather than the more usual (real, imaginary).  This setting is ignored unless
 * COMPLEX_PACKS is also defined.
 */
/* #define IMAG_LEADS 1 */

/*
 * Uncomment the following to insist that the Rogue Wave(R) defined DComplex
 * class be used, rather than the native compiler complex class.
 */
/* #define RW_USE_RW_COMPLEX 1 */

/*
 * Uncomment the following if your operating system supports memory
 * mapped files.  This will allow you to set up arrays whose data
 * blocks are memory mapped files - useful when looking at very large
 * files when you only need to access some of the data in them.
 */
/* #define RW_HAS_MAPPED_FILES */


/*************************************************************************
**************************************************************************
**                                                                      **
**              From here on, it's pretty much boilerplate              **
**              and rarely requires any tuning.                         **
**                                                                      **
**************************************************************************
**************************************************************************/

/* The Cray has restricted pointers and native complex: */


/* Disable restricted pointers if the compiler doesn't support them: */
#ifndef HAS_RESTRICT
#  define RW_RESTRICT_KEYWORD
#else
#  define RW_RESTRICT_KEYWORD restrict
#endif

#if defined(__IBMCPP__) || (defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x500)
extern "C" {
    typedef double(*mathFunTy)(double);
}
#else
typedef double(*mathFunTy)(double);
#endif

#ifdef RW_NATIVE_EXTENDED
typedef extended(*XmathFunTy)(extended);
#endif

/*
 * sun and AIX have memory mapped files using mmap()
 */
#if defined(__sun) || defined(_AIX)
#  define RW_HAS_MAPPED_FILES
#endif

typedef unsigned char   UChar;
#if defined(RW_NO_SCHAR)
typedef char          SChar;
#else
typedef signed char   SChar;
#endif

/*
 * Template instantiation
 *
 * Explicit template instantiation can be used to force
 * the compiler to instantiate a (complete) template
 * class. RW_MATH_INSTANTIATE_TEMPLATES
 * is *off* by default, and the templates used within the
 * library are not instantiated. Note that forcing instantiation
 * could be redundant when using a shared library, and it can
 * lead to needless instantiation. On the other hand, it may
 * speed up compilation somewhat, because it removes some of the
 * need for implicit instantiation. See Stroustrup, p866.
 *
 */

// By default, explicit instantiation is disabled.
#ifndef RW_MATH_INSTANTIATE_TEMPLATES
// User did not choose to force instantiation
#  ifndef RW_MATH_NO_INSTANTIATE_TEMPLATES
#    define RW_MATH_NO_INSTANTIATE_TEMPLATES
#  endif
#else
// User did choose to force instantiation
#  ifdef RW_MATH_NO_INSTANTIATE_TEMPLATES
#    undef RW_MATH_INSTANTIATE_TEMPLATES
#  endif
#endif


// Define what we need to prepend specializations
// with. (AIX is nonstandard.)
// Note that, for many platforms, this is optional;
// but according to the standard, is necessary.
//

// VC 7.0 requires explicit instantiation of
// RWMathVec<DComplex> and RWGenMat<DComplex> or
// we will get get unresolved symbols in the tests.
// I have left the above code in, but it appears to be
// obsolete and not used anywhere.  I have stolen the
// code below from stdlib 2.x.

#if  defined(RW_MATH_INSTANTIATE_TEMPLATES)     \
    && !defined (RWMATH_NO_EXPLICIT_INSTANTIATION)   \
    && !defined (RWMATH_NO_INSTANTIATE)
#  define RWMATH_INSTANTIATE(exp,arg)          template class exp arg
#else
#  if    defined(_MSC_VER) && (_MSC_VER >=1300)    \
      && !defined (RWMATH_NO_EXPLICIT_INSTANTIATION) \
      && !defined (RWMATH_NO_INSTANTIATE)
#    if defined(_RWBUILD_mth)
#      define RWMATH_INSTANTIATE(exp,arg)        extern template class arg
#    else
#      pragma warning (disable: 4231)
#      define RWMATH_INSTANTIATE(exp,arg)        extern template class exp arg
#    endif  // _RWBUILD_mth
#  else
#    define RWMATH_INSTANTIATE(ignore1,ignore2)           \
            typedef void __rw_unused_typedef
#  endif
#endif   // RWMATH_INSTANTIATE_TEMPLATES

#if !defined(RW_NO_NEW_TEMPLATE_SYNTAX)
#  if 1
#    define RW_MATH_SPECIALIZE template<>
#  else
#    define RW_MATH_SPECIALIZE
#  endif
#else
#  define RW_MATH_SPECIALIZE
#endif
#if defined(__GNUG__) || (defined(__SUNPRO_CC) && (__SUNPRO_CC>=0x500) && !defined(__SunOS_5_6)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500)) || (defined(_MSC_VER) && (_MSC_VER >= 1310)) || (defined(__hpux) && defined(__ia64) && (__HP_aCC >= 61600))
#  define RWMATH_FRIEND_TEMPLATE_SPECIFIER <>
#else
#  define RWMATH_FRIEND_TEMPLATE_SPECIFIER
#endif
/* Partial specialization */
// This is a new feature of the standard; most
// compilers don't support it yet. For now,
// just assume all compilers don't until
// proven otherwise.
#ifndef RW_MATH_PARTIAL_SPEC
#  define RW_MATH_NO_PARTIAL_SPEC
#endif

#ifdef RW_MATH_NO_PARTIAL_SPEC
#  undef RW_MATH_PARTIAL_SPEC
#endif

// Sunpro 5.0 does not have the complex.h header file.  It does have the
// standard library class complex<T>, but when you try to use it with
// the old std::iostream classes it's a mess.  Best to just use our own
// complex class with non-standard library builds on Sunpro 5.0
#if (defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x500) && !defined(RWSTDLIBTYPE)
#  define RW_USE_RW_COMPLEX 1
#endif

// Sunpro 5.0/5.1 also needs to have some mathfunctions in std namespace
#if (defined(__SUNPRO_CC) && ((RWSTDLIBTYPE != 0) || defined(_STLPORT_VERSION)) && (__SUNPRO_CC >= 0x500))
#  define MATH_FUNCTIONS_IN_STD  1
#endif
// For some reason, SunPro 5.8 complains about inaccessible copy constructors in the
// base class for the iterators.
#if defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x580
#  define RW_MATH_REQUIRES_BASE_COPY_CONSTRUCTOR 1
#endif

#if defined(_MSC_VER) && (RWSTDLIBTYPE != 0)
#  define MATH_FUNCTIONS_IN_STD  1
#  define RW_MATH_EXIT_NOT_IN_STD 1
#endif

#if (RWSTDLIBTYPE != 0)

#  define RW_MATH_SL_STD_GLOBAL(RW_NAME) std::RW_NAME
#else
#  define RW_MATH_SL_STD_GLOBAL(RW_NAME) ::RW_NAME
#endif

/*
 *  Class ID definitions
 */

#define __CGEMATRIX                     0x9001
#define __DCOMPLEXGENMATPICK            0x9002
#define __CLUDECOMP                     0x9003
#define __DCOMPLEX                      0x9004
#define __DCOMPLEXFFT2DSERVER           0x9004
#define __DCOMPLEXFFTSERVER             0x9005
#define __DCOMPLEXVECPICK               0x9006
#define __DCOMPLEXVEC                   0x9007
#define __DGEMATRIX                     0x9008
#define __DOUBLEGENMATPICK              0x9009
#define __DLUDECOMP                     0x900a
#define __DOUBLECOSINESERVER            0x900b
#define __DOUBLEFFTSERVER               0x900c
#define __DOUBLEVECPICK                 0x900d
#define __DOUBLEVEC                     0x900e
#define __FGEMATRIX                     0x900f
#define __FLOATGENMATPICK               0x9010
#define __FLUDECOMP                     0x9011
#define __FLOATVECPICK                  0x9012
#define __FLOATVEC                      0x9013
#define __HISTOGRAM                     0x9014
#define __IGEMATRIX                     0x9015
#define __INTGENMATPICK                 0x9016
#define __INTVECPICK                    0x9017
#define __INTVEC                        0x9018
#define __LEASTSQFIT                    0x9019
#define __RWBLOCK                       0x901a
#define __RANDBINOMIAL                  0x901b
#define __RANDEXP                       0x901c
#define __RANDGAMMA                     0x901d
#define __RANDNORMAL                    0x901e
#define __RANDPOISSON                   0x901f
#define __RANDUNIFORM                   0x9020
#define __SCGEMATRIX                    0x9021
#define __SCHARGENMATPICK               0x9022
#define __SCHARVECPICK                  0x9023
#define __SCHARVEC                      0x9024
#define __UCGEMATRIX                    0x9025
#define __UCHARGENMATPICK               0x9026
#define __UCHARVECPICK                  0x9027
#define __UCHARVEC                      0x9028

#define __DCOMPLEXARRAY                 0x9029
#define __DOUBLEARRAY                   0x902a
#define __FLOATARRAY                    0x902b
#define __INTARRAY                      0x902c
#define __SCHARARRAY                    0x902d
#define __UCHARARRAY                    0x902e

/*
 * xGEMATRIX has been renamed
 */

#define __DCOMPLEXGENMAT __CGEMATRIX
#define __DOUBLEGENMAT   __DGEMATRIX
#define __FLOATGENMAT    __FGEMATRIX
#define __INTGENMAT      __IGEMATRIX
#define __SCHARGENMAT    __SCGEMATRIX
#define __UCHARGENMAT    __UCGEMATRIX

/*
 * abs - related stuff goes here. Use macros _RWMATH_OVERLOAD_ABS_FLOAT and _RWMATH_OVERLOAD_ABS_DOUBLE
 * to force mathdefs.h declare and implement abs(float) and abs(double) here as inlines
 */

//#if defined(_MSC_VER) && !defined(RWSTDLIBTYPE)
#if defined(_MSC_VER) && (_MSC_VER < 1310) && (RWSTDLIBTYPE == 2)
#  define _RWMATH_OVERLOAD_ABS_FLOAT
#  define _RWMATH_OVERLOAD_ABS_DOUBLE
#endif

#ifdef _RWMATH_OVERLOAD_ABS_FLOAT
inline float abs(float x)
{
    return x < 0 ? -x : x;
}
#endif

#ifdef _RWMATH_OVERLOAD_ABS_DOUBLE
inline double abs(double x)
{
    return x < 0 ? -x : x;
}
#endif

// The iterator classes in RWMathVec, RWGenMat, and RWMathArray
// are derived from the standard library's iterator class.

// In older standard library implementations they must be derived from
// the random_access_iterator class.  Modern implementations use the
// random_access_iterator_tag.  This macro indicates that a particular
// OS/compiler combination uses the older random_access_iterator class
// rather than the modern random_access_iterator_tag.
#if ( (defined(_AIX) || defined(__sun) || defined(__linux__)) && defined(__GNUG__) && (__GNUG__ <= 2) && (RWSTDLIBTYPE == 2) )
#  define RW_MATH_HAS_RANDOM_ACCESS_ITER
#endif

#if defined(MATH_FUNCTIONS_IN_STD)
#  define RW_USING_STD_DECLARATION(a)  using std::a;
#  ifndef _WIN32
#    define RWMTH_STD std
#  else
#    define RWMTH_STD
#  endif
#  if defined(RW_MATH_EXIT_NOT_IN_STD)
#    define RW_MATH_EXIT exit
#  else
#    define RW_MATH_EXIT std::exit
#  endif
#else
#  define RW_USING_STD_DECLARATION(a)
#  define RWMTH_STD
#  define RW_MATH_EXIT exit
#endif

#if defined(RW_NO_NEW_HEADER)
#  define RW_MATH_NO_NEW_HEADER 1
#endif

#if defined(RW_USING_MKL)
#  define RW_SIZEOF_LAPACK_INT RW_SIZEOF_INT
#  define RW_LAPACK_INT_MAX RW_INT_MAX
#  define RW_MKL_ALIGNMENT_BOUNDARY 16
typedef int rw_lapack_int_t;
typedef int rw_lapack_select_t;
typedef int RWPivotType;
#elif defined(RW_USING_SUNPERF)
// memory used for x86 SSE2 instructions should be 16 byte aligned
#  if !defined(sparc)
#    define RW_SPL_ALIGNMENT_BOUNDARY 16
#  endif
// could be long or int depending on which functions we call
#  if defined(__sparcv9) || defined(__x86_64)
#    define RW_SIZEOF_LAPACK_INT RW_SIZEOF_LONG_INT
#    define RW_LAPACK_INT_MAX RW_LONG_MAX
typedef long rw_lapack_int_t;
typedef long rw_lapack_select_t;
typedef long RWPivotType;
#  else
#    define RW_SIZEOF_LAPACK_INT RW_SIZEOF_INT
#    define RW_LAPACK_INT_MAX RW_INT_MAX
typedef int rw_lapack_int_t;
typedef int rw_lapack_select_t;
typedef int RWPivotType;
#  endif //defined(__sparcv9) || defined (__x86_64) 
#else
#  define RW_SIZEOF_LAPACK_INT RW_SIZEOF_LONG_INT
#  define RW_LAPACK_INT_MAX RW_LONG_MAX
typedef long rw_lapack_int_t;
typedef long rw_lapack_select_t;
typedef long RWPivotType;
#endif

#if RW_LAPACK_INT_MAX <= RW_INT_MAX
#  define RW_LAPACK_INT_MAX_ASSERT(x) RW_ASSERT(x <= RW_LAPACK_INT_MAX)
#else
#  define RW_LAPACK_INT_MAX_ASSERT(x)
#endif

// Define a dummy MAIN__ (Fortran main function) implementation
// on platforms that will warn of undefined symbols.
#if defined(RW_USING_CLAPACK) && defined(RW_SHARED) && defined(__HP_aCC) && (__HP_aCC <= 62801)
#  define RW_UNDEFINED_MAIN__
#endif

#endif /* __MATHDEFS_H__ */
