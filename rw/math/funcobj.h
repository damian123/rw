#ifndef __RWFUNCOBJ_H__
#define __RWFUNCOBJ_H__

/***************************************************************************
 *
 * funcobj.h
 *
 * $Id: //math/13/rw/math/funcobj.h#1 $
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
 **************************************************************************
 *
 * $Log: funcobj.h,v $
 * Revision 1.10  1996/05/10 21:24:29  steves
 * Changed name of transform function to rw_math_transform and generate
 * to rw_math_generate to remove dependency on the standard library.
 *
 * Revision 1.9  1996/05/08  21:38:53  bass
 * add using std::transform and std::generate so the functions can be used
 * even though they are defined in the std:: namespace
 *
 * Revision 1.8  1996/05/07 21:18:25  bass
 * use std namespace for unary_function if STL is available
 *
 * Revision 1.7  1996/05/06 23:27:16  steves
 * added RW_NEEDS_INLINES_IN_HEADER macro to keep SGI happy.
 *
 * Revision 1.6  1996/05/03  20:14:05  steves
 * Added include of funcobh.cc if compile instantiate.
 *
 * Revision 1.5  1996/04/30 17:45:22  steves
 * Now uses the macro RW_INLINE_TMPL_FUNC to declare the global template
 * functions inline.  This macro expands to the keyword inline in most cases,
 * except the IBM where inline template functions get compiled into all
 * compilation units that include them (thereby giving multiple symbol
 * definition errors).
 * IBM Port
 *
 * Revision 1.4  1996/04/22 22:04:11  steves
 * Added STL algorithm generate for those who do not have the
 * STL.
 *
 **************************************************************************/

#include <rw/compiler.h>
#include <rw/mathdefs.h>
#include <rw/dcomplex.h>
#include <rw/rand.h>
#include <cstdlib>
#include <complex>

#ifndef _WIN32
RW_USING_STD_DECLARATION(acos)
RW_USING_STD_DECLARATION(asin)
RW_USING_STD_DECLARATION(atan)
RW_USING_STD_DECLARATION(atan2)
RW_USING_STD_DECLARATION(ceil)
RW_USING_STD_DECLARATION(fabs)
RW_USING_STD_DECLARATION(floor)
RW_USING_STD_DECLARATION(tan)
RW_USING_STD_DECLARATION(tanh)
#endif

RW_USING_STD_DECLARATION(abs)
RW_USING_STD_DECLARATION(cos)
RW_USING_STD_DECLARATION(cosh)
RW_USING_STD_DECLARATION(exp)
RW_USING_STD_DECLARATION(log)
RW_USING_STD_DECLARATION(log10)
RW_USING_STD_DECLARATION(pow)
RW_USING_STD_DECLARATION(sin)
RW_USING_STD_DECLARATION(sinh)
RW_USING_STD_DECLARATION(sqrt)

#if !defined(_STLPORT_VERSION) && !defined(_MSC_VER)
RW_USING_STD_DECLARATION(arg)
RW_USING_STD_DECLARATION(conj)
RW_USING_STD_DECLARATION(imag)
RW_USING_STD_DECLARATION(norm)
RW_USING_STD_DECLARATION(real)
#endif

#if (defined(_MSC_VER) && (_MSC_VER >= 1020)) || defined(__hpux)
// Microsoft 4.2 refuses to convert ints into floating point for any
// built in functions.  Put the explicit overloads here to make it
// happy.
inline double cos(int x)
{
    return cos(double(x));
}
inline double cosh(int x)
{
    return cosh(double(x));
}
inline double exp(int x)
{
    return exp(double(x));
}
inline double sin(int x)
{
    return sin(double(x));
}
inline double sinh(int x)
{
    return sinh(double(x));
}
inline double tan(int x)
{
    return tan(double(x));
}
inline double tanh(int x)
{
    return tanh(double(x));
}
inline double sqrt(int x)
{
    return sqrt(double(x));
}
inline double acos(int x)
{
    return acos(double(x));
}
inline double asin(int x)
{
    return asin(double(x));
}
inline double atan(int x)
{
    return atan(double(x));
}
inline double atan2(int x, int y)
{
    return atan2(double(x), double(y));
}
inline double log(int x)
{
    return log(double(x));
}
inline double log10(int x)
{
    return log10(double(x));
}
#endif

template <class InputIterator, class OutputIterator, class UnaryOperation>
RW_INLINE_TMPL_FUNC OutputIterator rw_math_transform(InputIterator first, InputIterator last,
        OutputIterator result, UnaryOperation op);

template < class InputIterator1, class InputIterator2, class OutputIterator,
         class BinaryOperation >
RW_INLINE_TMPL_FUNC OutputIterator rw_math_transform(InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2, OutputIterator result,
        BinaryOperation binary_op);

template <class ForwardIterator, class Generator>
RW_INLINE_TMPL_FUNC void rw_math_generate(ForwardIterator first, ForwardIterator last,
        Generator gen);

template <class ForwardIterator>
RW_INLINE_TMPL_FUNC void rw_math_generate2(ForwardIterator first, ForwardIterator last,
        RWRandInterface& gen);

template <class Domain, class Range>
struct RW_abs {
    inline Range operator()(const Domain& x) {
        return (Range)(x >= 0 ? x : -x);
    }
};

RW_MATH_SPECIALIZE
struct RW_abs<int, int> {
    inline int operator()(const int& x) {
        return abs(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_abs<double, double> {
    inline double operator()(const double& x) {
        return (double)fabs(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_abs<float, float> {
    inline float operator()(const float& x) {
        return (float)fabs(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_abs<long double, long double> {
    inline long double operator()(const long double& x) {
        return (long double)fabs(x);
    }
};


#ifndef RW_MATH_NO_PARTIAL_SPEC
template <Domain, class Range>
struct RW_abs<unsigned Domain, Range> {
    inline Range operator()(const unsigned Domain& x) {
        return (Range)x;
    }
};

#else
RW_MATH_SPECIALIZE
struct RW_abs<unsigned long, unsigned long> {
    inline unsigned long operator()(const unsigned long& x) {
        return x;
    }
};

RW_MATH_SPECIALIZE
struct RW_abs<unsigned int, unsigned int> {
    inline unsigned long operator()(const unsigned int& x) {
        return x;
    }
};
#endif

RW_MATH_SPECIALIZE
struct RW_abs<long, long> {
    inline long operator()(const long& x) {
        return std::labs(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_abs<DComplex, double> {
    inline double operator()(const DComplex& x) {
        return abs(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_abs< const std::complex<double>, std::complex<double> > {
    inline std::complex<double> operator()(const std::complex<double>& x) {
        return abs(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_abs< const std::complex<float>, std::complex<float> > {
    inline std::complex<float> operator()(const std::complex<float>& x) {
        return abs(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_abs< const std::complex<long double>, std::complex<long double> > {
    inline std::complex<long double> operator()(const std::complex<long double>& x) {
        return abs(x);
    }
};

template <class Domain, class Range>
struct RW_acos {
    inline Range operator()(const Domain& x) {
        return (Range)acos(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_acos<SChar, SChar> {
    inline SChar operator()(const SChar& x) {
        return SChar(acos(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_acos<UChar, UChar> {
    inline UChar operator()(const UChar& x) {
        return UChar(acos(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_acos<int, int> {
    inline int operator()(const int& x) {
        return int(acos(double(x)));
    }
};

template <class Domain, class Range>
struct RW_arg {
    inline Range operator()(const Domain& x) {
        return x == (Domain)(0) ? (Range)(0) : (Range)RW_MATH_SL_STD_GLOBAL(arg)(x);
    }
};

template <class Domain, class Range>
struct RW_asin {
    inline Range operator()(const Domain& x) {
        return (Range)asin(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_asin<SChar, SChar> {
    inline SChar operator()(const SChar& x) {
        return SChar(asin(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_asin<UChar, UChar> {
    inline UChar operator()(const UChar& x) {
        return UChar(asin(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_asin<int, int> {
    inline int operator()(const int& x) {
        return int(asin(double(x)));
    }
};

template <class Domain, class Range>
struct RW_atan {
    inline Range operator()(const Domain& x) {
        return (Range)atan(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_atan<SChar, SChar> {
    inline SChar operator()(const SChar& x) {
        return SChar(atan(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_atan<UChar, UChar> {
    inline UChar operator()(const UChar& x) {
        return UChar(atan(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_atan<int, int> {
    inline int operator()(const int& x) {
        return int(atan(double(x)));
    }
};

template <class Domain1, class Domain2, class Range>
struct RW_atan2 {
    inline Range operator()(const Domain1& x, const Domain2& y) {
        return (Range)atan2(x, y);
    }
};

// overload the atan2 function for float,float if fcomplex atan2 is declared
// there is an ambiguity with atan2(float,FComplex) and atan2(FComplex,float)
// if explicitly declared (not templates).
//
// don't do it for HP aCC 3.05 though because the library defines
// atan2(float,float) which has different precision than atan2(double,double)
//
// 2000-FEB-23 -- the commentary above still applies for HP aCC 3.13
// and is likely to apply for future versions as well.
//
#if defined(HP_ANSI) && (__cplusplus >= 199707 )
#else
RW_MATH_SPECIALIZE
struct RW_atan2<float, float, float> {
    inline float operator()(const float& x, const float& y) {
        return (float)atan2((double)x, (double)y);
    }
};
#endif

RW_MATH_SPECIALIZE
struct RW_atan2<SChar, SChar, SChar> {
    inline SChar operator()(const SChar& x, const SChar& y) {
        return SChar(atan2(double(x), double(y)));
    }
};

RW_MATH_SPECIALIZE
struct RW_atan2<UChar, UChar, UChar> {
    inline UChar operator()(const UChar& x, const UChar& y) {
        return UChar(atan2(double(x), double(y)));
    }
};

RW_MATH_SPECIALIZE
struct RW_atan2<int, int, int> {
    inline int operator()(const int& x, const int& y) {
        return int(atan2(double(x), double(y)));
    }
};

template <class Domain, class Range>
struct RW_ceil {
    inline Range operator()(const Domain& x) {
        return (Range)ceil(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_ceil<UChar, UChar> {
    inline UChar operator()(const UChar& x) {
        return x;
    }
};

RW_MATH_SPECIALIZE
struct RW_ceil<SChar, SChar> {
    inline SChar operator()(const SChar& x) {
        return x;
    }
};

RW_MATH_SPECIALIZE
struct RW_ceil<int, int> {
    inline int operator()(const int& x) {
        return x;
    }
};

template <class Domain, class Range>
struct RW_conj {
    inline Range operator()(const Domain& x) {
        return (Range)RW_MATH_SL_STD_GLOBAL(conj)(x);
    }
};

template <class Domain, class Range>
struct RW_cos {
    inline Range operator()(const Domain& x) {
        return (Range)cos(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_cos<UChar, UChar> {
    inline UChar operator()(const UChar& x) {
        return UChar(cos(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_cos<SChar, SChar> {
    inline SChar operator()(const SChar& x) {
        return SChar(cos(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_cos<int, int> {
    inline int operator()(const int& x) {
        return int(cos(double(x)));
    }
};

template <class Domain, class Range>
struct RW_cosh {
    inline Range operator()(const Domain& x) {
        return (Range)cosh(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_cosh<UChar, UChar> {
    inline UChar operator()(const UChar& x) {
        return UChar(cosh(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_cosh<SChar, SChar> {
    inline SChar operator()(const SChar& x) {
        return SChar(cosh(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_cosh<int, int> {
    inline int operator()(const int& x) {
        return int(cosh(double(x)));
    }
};

template <class Domain, class Range>
struct RW_exp {
    inline Range operator()(const Domain& x) {
        return (Range)exp(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_exp<UChar, UChar> {
    inline UChar operator()(const UChar& x) {
        return UChar(exp(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_exp<SChar, SChar> {
    inline SChar operator()(const SChar& x) {
        return SChar(exp(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_exp<int, int> {
    inline int operator()(const int& x) {
        return int(exp(double(x)));
    }
};

template <class Domain, class Range>
struct RW_floor {
    inline Range operator()(const Domain& x) {
        return (Range)floor(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_floor<UChar, UChar> {
    inline UChar operator()(const UChar& x) {
        return x;
    }
};

RW_MATH_SPECIALIZE
struct RW_floor<SChar, SChar> {
    inline SChar operator()(const SChar& x) {
        return x;
    }
};

RW_MATH_SPECIALIZE
struct RW_floor<int, int> {
    inline int operator()(const int& x) {
        return x;
    }
};

template <class Domain, class Range>
struct RW_imag {
    inline Range operator()(const Domain& x) {
        return (Range)RW_MATH_SL_STD_GLOBAL(imag)(x);
    }
};

template <class Domain, class Range>
struct RW_log {
    inline Range operator()(const Domain& x) {
        return (Range)log(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_log<UChar, UChar> {
    inline UChar operator()(const UChar& x) {
        return UChar(log(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_log<SChar, SChar> {
    inline SChar operator()(const SChar& x) {
        return SChar(log(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_log<int, int> {
    inline int operator()(const int& x) {
        return int(log(double(x)));
    }
};

template <class Domain, class Range>
struct RW_log10 {
    inline Range operator()(const Domain& x) {
        return (Range)log10(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_log10<UChar, UChar> {
    inline UChar operator()(const UChar& x) {
        return UChar(log10(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_log10<SChar, SChar> {
    inline SChar operator()(const SChar& x) {
        return SChar(log10(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_log10<int, int> {
    inline int operator()(const int& x) {
        return int(log10(double(x)));
    }
};

template< class Domain, class Range >
struct RW_norm {
    inline Range operator()(const Domain& x) {
        return (Range)RW_MATH_SL_STD_GLOBAL(norm)(x);
    }
};

template<>
struct RW_norm<double, double> {
    inline double operator()(const double& x) {
        return fabs(x);
    }
};

template<>
struct RW_norm<float, float> {
    inline float operator()(const float& x) {
        return (float)fabs(x);
    }
};

template<>
struct RW_norm<long double, long double> {
    inline long double operator()(const long double& x) {
        return fabs(x);
    }
};

template< class T >
struct RW_pow2 {
    T operator()(const T& x, int n);
};

template< class Domain1, class Domain2, class Range >
struct RW_pow {
    inline Range operator()(const Domain1& x, const Domain2& y) {
        return (Range)pow(x, y);
    }
};

RW_MATH_SPECIALIZE
struct RW_pow<DComplex, DComplex, DComplex> {
    inline DComplex operator()(const DComplex& x, const DComplex& y) {
        return pow(x, y);
    }
};

// overload the pow function for float,float in case fcomplex pow is declared
// there is an ambiguity with pow(float,FComplex) and pow(FComplex,float)
// if explicitly declared (not templates).
RW_MATH_SPECIALIZE
struct RW_pow<float, float, float> {
    inline float operator()(const float& x, const float& y) {
        return (float) pow((double)x, (double)y);
    }
};

//overload the pow function for aix5l requires explicit prototype for pow(int,int)
RW_MATH_SPECIALIZE
struct RW_pow<int, int, int> {
    inline int operator()(const int& x, const int& y) {
        return (int)pow((double)x, (double)y);
    }
};

RW_MATH_SPECIALIZE
struct RW_pow<int, int, double> {
    inline double operator()(const int& x, const int& y) {
        return (double)pow((double)x, (double)y);
    }
};

template <class Domain, class Range>
struct RW_real {
    inline Range operator()(const Domain& x) {
        return (Range)RW_MATH_SL_STD_GLOBAL(real)(x);
    }
};

template<>
struct RW_real<double, double> {
    inline double operator()(const double& x) {
        return x;
    }
};

template<>
struct RW_real<float, float> {
    inline float operator()(const float& x) {
        return x;
    }
};

template< class Domain, class Range >
struct RW_sin {
    inline Range operator()(const Domain& x) {
        return (Range)sin(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_sin<UChar, UChar> {
    inline UChar operator()(const UChar& x) {
        return UChar(sin(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_sin<SChar, SChar> {
    inline SChar operator()(const SChar& x) {
        return SChar(sin(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_sin<int, int> {
    inline int operator()(const int& x) {
        return int(sin(double(x)));
    }
};

template< class Domain, class Range >
struct RW_sinh {
    inline Range operator()(const Domain& x) {
        return (Range)sinh(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_sinh<UChar, UChar> {
    inline UChar operator()(const UChar& x) {
        return UChar(sinh(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_sinh<SChar, SChar> {
    inline SChar operator()(const SChar& x) {
        return SChar(sinh(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_sinh<int, int> {
    inline int operator()(const int& x) {
        return int(sinh(double(x)));
    }
};

template< class Domain, class Range >
struct RW_sqrt {
    inline Range operator()(const Domain& x) {
        return (Range)sqrt(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_sqrt<UChar, UChar> {
    inline UChar operator()(const UChar& x) {
        return UChar(sqrt(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_sqrt<SChar, SChar> {
    inline SChar operator()(const SChar& x) {
        return SChar(sqrt(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_sqrt<int, int> {
    inline int operator()(const int& x) {
        return int(sqrt(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_sqrt<long, long> {
    inline long operator()(const long& x) {
        return long(sqrt(double(x)));
    }
};

template< class Domain, class Range >
struct RW_tan {
    inline Range operator()(const Domain& x) {
        return (Range)tan(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_tan<UChar, UChar> {
    inline UChar operator()(const UChar& x) {
        return UChar(tan(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_tan<SChar, SChar> {
    inline SChar operator()(const SChar& x) {
        return SChar(tan(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_tan<int, int> {
    inline int operator()(const int& x) {
        return int(tan(double(x)));
    }
};

template< class Domain, class Range >
struct RW_tanh {
    inline Range operator()(const Domain& x) {
        return (Range)tanh(x);
    }
};

RW_MATH_SPECIALIZE
struct RW_tanh<UChar, UChar> {
    inline UChar operator()(const UChar& x) {
        return UChar(tanh(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_tanh<SChar, SChar> {
    inline SChar operator()(const SChar& x) {
        return SChar(tanh(double(x)));
    }
};

RW_MATH_SPECIALIZE
struct RW_tanh<int, int> {
    inline int operator()(const int& x) {
        return int(tanh(double(x)));
    }
};

template <class Domain, class Range>
struct RW_convert {
    inline Range operator()(const Domain& x) {
        return (Range)x;
    }
};


#if defined(RW_MATH_COMPILE_INSTANTIATE) || defined(RW_NEEDS_INLINES_IN_HEADER)
#  include <rw/math/funcobj.cc>
#endif

#endif // __RWFUNCOBJ_H__
