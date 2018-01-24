/***************************************************************************
 *
 * mtharrop.cc -
 *   Definitions for various arithmetic operations for RWMathArray<T>
 *
 * $Id: //math/13/rw/math/mtharrop.cc#1 $
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
 ***************************************************************************
 *
 * If a promotable scalar (such as a float) is passed by value, then
 * it will get promoted to, say, a double.  If the address of this is
 * then taken, we have a pointer to a double.  Unfortunately, if
 * this address is then passed to a routine that expects a pointer
 * to a float, cfront thinks it has done its job and quietly passes
 * the address of this promoted variable.
 *
 * This problem occurs when K&R compilers are used as a backend to
 * cfront.  The workaround is to create a temporary of known type
 * on the stack.  This is the "scalar2" variable found throughout
 * this file.
 *
 **************************************************************************/

#include <rw/math/mtharray.h>
#include <rw/math/rwbla.h>


/************************************************
 *                                              *
 *              UNARY OPERATORS                 *
 *                                              *
 ************************************************/

template <class T>
RWMathArray<T> operator-(const RWMathArray<T>& s)   // Unary minus
{
    RWMathArray<T> temp(s.length(), rwUninitialized);
    T* dp = temp.data();
    RW_ASSERT(dp != 0 || temp.length() == 0);
    for (ArrayLooper l(s.length(), s.stride()); l; ++l) {
        size_t n = l.length;
        T* sp = (T*)s.data() + l.start;
        while (n--) {
            *dp++ = T(-(*sp));
            sp += l.stride;
        }
    }
    return temp;
}

// Unary prefix increment on a RWMathArray<T>; (i.e. ++a)
template <class T>
RWMathArray<T>& RWMathArray<T>::operator++()
{
    for (ArrayLooper l(length(), stride()); l; ++l) {
        size_t n = l.length;
        T* sp = data() + l.start;
        while (n--) {
            *sp = T(*sp + T(1));
            sp += l.stride;
        }
    }
    return *this;
}

// Unary prefix decrement on a RWMathArray<T> (i.e., --a)
template <class T>
RWMathArray<T>& RWMathArray<T>::operator--()
{
    for (ArrayLooper l(length(), stride()); l; ++l) {
        size_t n = l.length;
        T* sp = data() + l.start;
        while (n--) {
            *sp = T(*sp - T(1));
            sp += l.stride;
        }
    }
    return *this;
}

/************************************************
 *                                              *
 *              BINARY OPERATORS                *
 *               array - array                  *
 *                                              *
 ************************************************/

template <class T>
RWMathArray<T> operator+(const RWMathArray<T>& u, const RWMathArray<T>& v)
{
    u.lengthCheck(v.length());
    RWMathArray<T> temp(u.length(), rwUninitialized, RWDataView::COLUMN_MAJOR);
    T* dp = temp.data();
    for (DoubleArrayLooper l(u.length(), u.stride(), v.stride()); l; ++l) {
        RWBlas_util<T>::plvv(l.length, dp, u.data() + l.start1, l.stride1, v.data() + l.start2, l.stride2);
        dp += l.length;
    }
    return temp;
}

template <class T>
RWMathArray<T> operator-(const RWMathArray<T>& u, const RWMathArray<T>& v)
{
    u.lengthCheck(v.length());
    RWMathArray<T> temp(u.length(), rwUninitialized, RWDataView::COLUMN_MAJOR);
    T* dp = temp.data();
    for (DoubleArrayLooper l(u.length(), u.stride(), v.stride()); l; ++l) {
        RWBlas_util<T>::mivv(l.length, dp, u.data() + l.start1, l.stride1, v.data() + l.start2, l.stride2);
        dp += l.length;
    }
    return temp;
}

template <class T>
RWMathArray<T> operator*(const RWMathArray<T>& u, const RWMathArray<T>& v)
{
    u.lengthCheck(v.length());
    RWMathArray<T> temp(u.length(), rwUninitialized, RWDataView::COLUMN_MAJOR);
    T* dp = temp.data();
    for (DoubleArrayLooper l(u.length(), u.stride(), v.stride()); l; ++l) {
        RWBlas_util<T>::muvv(l.length, dp, u.data() + l.start1, l.stride1, v.data() + l.start2, l.stride2);
        dp += l.length;
    }
    return temp;
}

template <class T>
RWMathArray<T> operator/(const RWMathArray<T>& u, const RWMathArray<T>& v)
{
    u.lengthCheck(v.length());
    RWMathArray<T> temp(u.length(), rwUninitialized, RWDataView::COLUMN_MAJOR);
    T* dp = temp.data();
    for (DoubleArrayLooper l(u.length(), u.stride(), v.stride()); l; ++l) {
        RWBlas_util<T>::dvvv(l.length, dp, u.data() + l.start1, l.stride1, v.data() + l.start2, l.stride2);
        dp += l.length;
    }
    return temp;
}

/************************************************
 *                                              *
 *              BINARY OPERATORS                *
 *               array  - scalar                *
 *                                              *
 ************************************************/

template <class T>
RWMathArray<T> operator+(const RWMathArray<T>& s, const T& scalar)
{
    RWMathArray<T> temp(s.length(), rwUninitialized);
    T* dp = temp.data();
    for (ArrayLooper l(s.length(), s.stride()); l; ++l) {
        //  Promotable types need a temporary for K&R compilers:
        T scalar2 = scalar;
        RWBlas_util<T>::plvs(l.length, dp, s.data() + l.start, l.stride, &scalar2);
        dp += l.length;
    }
    return temp;
}

template <class T>
RWMathArray<T> operator-(const T& scalar, const RWMathArray<T>& s)
{
    RWMathArray<T> temp(s.length(), rwUninitialized);
    T* dp = temp.data();
    for (ArrayLooper l(s.length(), s.stride()); l; ++l) {
        //  Promotable types need a temporary for K&R compilers:
        T scalar2 = scalar;
        RWBlas_util<T>::misv(l.length, dp, &scalar2, s.data() + l.start, l.stride);
        dp += l.length;
    }
    return temp;
}

template <class T>
RWMathArray<T> operator*(const RWMathArray<T>& s, const T& scalar)
{
    RWMathArray<T> temp(s.length(), rwUninitialized);
    T* dp = temp.data();
    for (ArrayLooper l(s.length(), s.stride()); l; ++l) {
        //  Promotable types need a temporary for K&R compilers:
        T scalar2 = scalar;
        RWBlas_util<T>::muvs(l.length, dp, s.data() + l.start, l.stride, &scalar2);
        dp += l.length;
    }
    return temp;
}

template <class T>
RWMathArray<T> operator/(const T& scalar, const RWMathArray<T>& s)
{
    RWMathArray<T> temp(s.length(), rwUninitialized);
    T* dp = temp.data();
    for (ArrayLooper l(s.length(), s.stride()); l; ++l) {
        //  Promotable types need a temporary for K&R compilers:
        T scalar2 = scalar;
        RWBlas_util<T>::dvsv(l.length, dp, &scalar2, s.data() + l.start, l.stride);
        dp += l.length;
    }
    return temp;
}

template <class T>
RWMathArray<T> operator/(const RWMathArray<T>& s, const T& scalar)
{
    RWMathArray<T> temp(s.length(), rwUninitialized);
    T* dp = temp.data();
    for (ArrayLooper l(s.length(), s.stride()); l; ++l) {
        //  Promotable types need a temporary for K&R compilers:
        T scalar2 = scalar;
        RWBlas_util<T>::dvvs(l.length, dp, s.data() + l.start, l.stride, &scalar2);
        dp += l.length;
    }
    return temp;
}

/*
 * Out-of-line versions for compilers which can't
 * handle inlined temporaries:
 */
#ifdef RW_NO_INLINED_TEMP_DESTRUCTORS
template <class T>
RWMathArray<T>     operator*(const T& s, const RWMathArray<T>& V)
{
    return V * s;
}
template <class T>
RWMathArray<T>     operator+(const T& s, const RWMathArray<T>& V)
{
    return V + s;
}
template <class T>
RWMathArray<T>     operator-(const RWMathArray<T>& V, const T& s)
{
    return V + (T)(-s);
}
#endif

/************************************************
 *                                              *
 *      ARITHMETIC ASSIGNMENT OPERATORS         *
 *        with other arrays                     *
 *                                              *
 ************************************************/

template <class T>
RWMathArray<T>& RWMathArray<T>::operator+=(const RWMathArray<T>& u)
{
    if (sameDataBlock(u)) {
        return operator+=(u.deepCopy());    // Avoid aliasing
    }
    u.lengthCheck(length());
    for (DoubleArrayLooper l(length(), stride(), u.stride()); l; ++l) {
        RWBlas_util<T>::aplvv(l.length, data() + l.start1, l.stride1, u.data() + l.start2, l.stride2);
    }
    return *this;
}

template <class T>
RWMathArray<T>& RWMathArray<T>::operator-=(const RWMathArray<T>& u)
{
    if (sameDataBlock(u)) {
        return operator-=(u.deepCopy());    // Avoid aliasing
    }
    u.lengthCheck(length());
    for (DoubleArrayLooper l(length(), stride(), u.stride()); l; ++l) {
        RWBlas_util<T>::amivv(l.length, data() + l.start1, l.stride1, u.data() + l.start2, l.stride2);
    }
    return *this;
}

template <class T>
RWMathArray<T>& RWMathArray<T>::operator*=(const RWMathArray<T>& u)
{
    if (sameDataBlock(u)) {
        return operator*=(u.deepCopy());    // Avoid aliasing
    }
    u.lengthCheck(length());
    for (DoubleArrayLooper l(length(), stride(), u.stride()); l; ++l) {
        RWBlas_util<T>::amuvv(l.length, data() + l.start1, l.stride1, u.data() + l.start2, l.stride2);
    }
    return *this;
}

template <class T>
RWMathArray<T>& RWMathArray<T>::operator/=(const RWMathArray<T>& u)
{
    if (sameDataBlock(u)) {
        return operator/=(u.deepCopy());    // Avoid aliasing
    }
    u.lengthCheck(length());
    for (DoubleArrayLooper l(length(), stride(), u.stride()); l; ++l) {
        RWBlas_util<T>::advvv(l.length, data() + l.start1, l.stride1, u.data() + l.start2, l.stride2);
    }
    return *this;
}

/************************************************
 *                                              *
 *      ARITHMETIC ASSIGNMENT OPERATORS         *
 *                with a scalar                 *
 *                                              *
 ************************************************/

template <class T>
RWMathArray<T>& RWMathArray<T>::operator+=(const T& scalar)
{
    for (ArrayLooper l(length(), stride()); l; ++l) {
        //  Promotable types need a temporary for K&R compilers:
        T scalar2 = scalar;
        RWBlas_util<T>::aplvs(l.length, data() + l.start, l.stride, &scalar2);
    }
    return *this;
}


template <class T>
RWMathArray<T>& RWMathArray<T>::operator*=(const T& scalar)
{
    for (ArrayLooper l(length(), stride()); l; ++l) {
        //  Promotable types need a temporary for K&R compilers:
        T scalar2 = scalar;
        RWBlas_util<T>::amuvs(l.length, data() + l.start, l.stride, &scalar2);
    }
    return *this;
}

template <class T>
RWMathArray<T>& RWMathArray<T>::operator/=(const T& scalar)
{
    for (ArrayLooper l(length(), stride()); l; ++l) {
        //  Promotable types need a temporary for K&R compilers:
        T scalar2 = scalar;
        RWBlas_util<T>::advvs(l.length, data() + l.start, l.stride, &scalar2);
    }
    return *this;
}

/************************************************
 *                                              *
 *              LOGICAL OPERATORS               *
 *                                              *
 ************************************************/

template <class T>
bool RWMathArray<T>::operator==(const RWMathArray<T>& u) const
{
    if (length() != u.length()) {
        return false;    // They can't be equal if they don't have the same length.
    }
    for (DoubleArrayLooper l(length(), stride(), u.stride()); l; ++l) {
        if (!RWBlas_util<T>::same(l.length, (T*)(data() + l.start1), l.stride1, u.data() + l.start2, l.stride2)) {
            return false;
        }
    }
    return true;
}

template <class T>
bool RWMathArray<T>::operator!=(const RWMathArray<T>& u) const
{
    return !(*this == u);
}
