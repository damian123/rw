#ifndef __MATOP_CC__
#define __MATOP_CC__

/***************************************************************************
 *
 * matop.cc
 *
 * $Id: //math/13/rw/math/matop.cc#1 $
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
 **************************************************************************/

#include <rw/math/genmat.h>
#include <rw/math/rwbla.h>

/************************************************
 *                                              *
 *              UNARY OPERATORS                 *
 *                                              *
 ************************************************/

template <class T>
RWGenMat<T> operator-(const RWGenMat<T>& s)   // Unary minus
{
    RWGenMat<T> temp(s.rows(), s.cols(), rwUninitialized);
    typename RWGenMat<T>::const_iterator it_s = s.begin();
    typename RWGenMat<T>::iterator it_temp = temp.begin();
    typename RWGenMat<T>::const_iterator last = s.end();
    RW_ASSERT((s.data() != 0 && temp.data() != 0) || it_s == last);
    for (; it_s < last; ++it_s, ++it_temp) {
        *it_temp = static_cast<T>(-(*it_s));
    }
    return temp;
}

// Unary prefix increment on a RWGenMat<T>; (i.e. ++a)
template <class T>
RWGenMat<T>& RWGenMat<T>::operator++()
{
    typename RWGenMat<T>::iterator it = begin();
    typename RWGenMat<T>::iterator last = end();
    RW_ASSERT(data() != 0 || it == last);
    for (; it < last; ++it) {
        *it += T(1);
    }
    return *this;
}

// Unary prefix decrement on a RWGenMat<T> (i.e., --a)
template <class T>
RWGenMat<T>&
RWGenMat<T>::operator--()
{
    typename RWGenMat<T>::iterator it = begin();
    typename RWGenMat<T>::iterator last = end();
    RW_ASSERT(data() != 0 || it == last);
    for (; it < last; ++it) {
        *it -= T(1);
    }
    return *this;
}

/************************************************
 *                                              *
 *              BINARY OPERATORS                *
 *               matrix - matrix                *
 *                                              *
 ************************************************/

template <class T>
RWGenMat<T> operator+(const RWGenMat<T>& u, const RWGenMat<T>& v)
{
    size_t m = v.rows();
    size_t n = v.cols();
    u.lengthCheck(m, n);
    RWGenMat<T> temp(m, n, rwUninitialized);
    typename RWGenMat<T>::iterator it_temp;
    typename RWGenMat<T>::const_iterator it_u, it_v;
    typename RWGenMat<T>::const_iterator last = u.end();
    for (it_u = u.begin(), it_v = v.begin(), it_temp = temp.begin(); it_u < last; INC_ROW(it_u), INC_ROW(it_v), INC_ROW(it_temp)) {
        RWBlas_util<T>::plvv(it_u.row_length(), &*it_temp, &*it_u, it_u.stride(), &*it_v, it_v.stride());
    }
    return temp;
}

template <class T>
RWGenMat<T> operator-(const RWGenMat<T>& u, const RWGenMat<T>& v)
{
    size_t m = v.rows();
    size_t n = v.cols();
    u.lengthCheck(m, n);
    RWGenMat<T> temp(m, n, rwUninitialized);
    typename RWGenMat<T>::iterator it_temp = temp.begin();
    typename RWGenMat<T>::const_iterator it_u = u.begin(), it_v = v.begin();
    typename RWGenMat<T>::const_iterator last = u.end();
    for (; it_u < last; INC_ROW(it_u), INC_ROW(it_v), INC_ROW(it_temp)) {
        RWBlas_util<T>::mivv(it_u.row_length(), &*it_temp, &*it_u, it_u.stride(), &*it_v, it_v.stride());
    }
    return temp;
}

template <class T>
RWGenMat<T> operator*(const RWGenMat<T>& u, const RWGenMat<T>& v)
{
    size_t m = v.rows();
    size_t n = v.cols();
    u.lengthCheck(m, n);
    RWGenMat<T> temp(m, n, rwUninitialized);
    typename RWGenMat<T>::iterator it_temp;
    typename RWGenMat<T>::const_iterator it_u, it_v;
    typename RWGenMat<T>::const_iterator last = u.end();
    for (it_u = u.begin(), it_v = v.begin(), it_temp = temp.begin(); it_u < last; INC_ROW(it_u), INC_ROW(it_v), INC_ROW(it_temp)) {
        RWBlas_util<T>::muvv(it_u.row_length(), &*it_temp, &*it_u, it_u.stride(), &*it_v, it_v.stride());
    }
    return temp;
}


template <class T>
RWGenMat<T> operator/(const RWGenMat<T>& u, const RWGenMat<T>& v)
{
    size_t m = v.rows();
    size_t n = v.cols();
    u.lengthCheck(m, n);
    RWGenMat<T> temp(m, n, rwUninitialized);
    typename RWGenMat<T>::iterator it_temp;
    typename RWGenMat<T>::const_iterator it_u, it_v;
    typename RWGenMat<T>::const_iterator last = u.end();
    for (it_u = u.begin(), it_v = v.begin(), it_temp = temp.begin(); it_u < last; INC_ROW(it_u), INC_ROW(it_v), INC_ROW(it_temp)) {
        RWBlas_util<T>::dvvv(it_u.row_length(), &*it_temp, &*it_u, it_u.stride(), &*it_v, it_v.stride());
    }
    return temp;
}

/************************************************
 *                                              *
 *              BINARY OPERATORS                *
 *               matrix - scalar                *
 *                                              *
 ************************************************/

template <class T>
RWGenMat<T> operator+(const RWGenMat<T>& s, const T& scalar)
{
    size_t m = s.rows();
    size_t n = s.cols();
    RWGenMat<T> temp(m, n, rwUninitialized);
    typename RWGenMat<T>::const_iterator it_s;
    typename RWGenMat<T>::iterator it_temp;
    typename RWGenMat<T>::const_iterator last = s.end();
    for (it_s = s.begin(), it_temp = temp.begin(); it_s < last; INC_ROW(it_s), INC_ROW(it_temp)) {
        RWBlas_util<T>::plvs(it_s.row_length(), &*it_temp, &*it_s, it_s.stride(), &scalar);
    }
    return temp;
}

template <class T>
RWGenMat<T> operator-(const T& scalar, const RWGenMat<T>& s)
{
    size_t m = s.rows();
    size_t n = s.cols();
    RWGenMat<T> temp(m, n, rwUninitialized);
    typename RWGenMat<T>::const_iterator it_s;
    typename RWGenMat<T>::iterator it_temp;
    typename RWGenMat<T>::const_iterator last = s.end();
    for (it_s = s.begin(), it_temp = temp.begin(); it_s < last; INC_ROW(it_s), INC_ROW(it_temp)) {
        RWBlas_util<T>::misv(it_s.row_length(), &*it_temp, &scalar, &*it_s, it_s.stride());
    }
    return temp;
}

template <class T>
RWGenMat<T> operator-(const RWGenMat<T>& s, const T& scalar)
{
    size_t m = s.rows();
    size_t n = s.cols();
    RWGenMat<T> temp(m, n, rwUninitialized);
    typename RWGenMat<T>::const_iterator it_s;
    typename RWGenMat<T>::iterator it_temp;
    typename RWGenMat<T>::const_iterator last = s.end();
    for (it_s = s.begin(), it_temp = temp.begin(); it_s < last; INC_ROW(it_s), INC_ROW(it_temp)) {
        RWBlas_util<T>::mivs(it_s.row_length(), &*it_temp, &scalar, &*it_s, it_s.stride());
    }
    return temp;
}

template <class T>
RWGenMat<T> operator*(const RWGenMat<T>& s, const T& scalar)
{
    size_t m = s.rows();
    size_t n = s.cols();
    RWGenMat<T> temp(m, n, rwUninitialized);
    typename RWGenMat<T>::const_iterator it_s;
    typename RWGenMat<T>::iterator it_temp;
    typename RWGenMat<T>::const_iterator last = s.end();
    for (it_s = s.begin(), it_temp = temp.begin(); it_s < last; INC_ROW(it_s), INC_ROW(it_temp)) {
        RWBlas_util<T>::muvs(it_s.row_length(), &*it_temp, &*it_s, it_s.stride(), &scalar);
    }
    return temp;
}

template <class T>
RWGenMat<T> operator/(const T& scalar, const RWGenMat<T>& s)
{
    size_t m = s.rows();
    size_t n = s.cols();
    RWGenMat<T> temp(m, n, rwUninitialized);
    typename RWGenMat<T>::const_iterator it_s;
    typename RWGenMat<T>::iterator it_temp;
    typename RWGenMat<T>::const_iterator last = s.end();
    for (it_s = s.begin(), it_temp = temp.begin(); it_s < last; INC_ROW(it_s), INC_ROW(it_temp)) {
        RWBlas_util<T>::dvsv(it_s.row_length(), &*it_temp, &scalar, &*it_s, it_s.stride());
    }
    return temp;
}

template <class T>
RWGenMat<T> operator/(const RWGenMat<T>& s, const T& scalar)
{
    size_t m = s.rows();
    size_t n = s.cols();
    RWGenMat<T> temp(m, n, rwUninitialized);
    typename RWGenMat<T>::const_iterator it_s;
    typename RWGenMat<T>::iterator it_temp;
    typename RWGenMat<T>::const_iterator last = s.end();
    for (it_s = s.begin(), it_temp = temp.begin(); it_s < last; INC_ROW(it_s), INC_ROW(it_temp)) {
        RWBlas_util<T>::dvvs(it_s.row_length(), &*it_temp, &*it_s, it_s.stride(), &scalar);
    }
    return temp;
}

template <class T>
RWGenMat<T> operator*(const T& scalar, const RWGenMat<T>& s)
{
    return s * scalar;
}

template <class T>
RWGenMat<T> operator+(const T& scalar, const RWGenMat<T>& s)
{
    return s + scalar;
}


/************************************************
 *                                              *
 *      ARITHMETIC ASSIGNMENT OPERATORS         *
 *        with other matrices                   *
 *                                              *
 ************************************************/

template <class T>
RWGenMat<T>& RWGenMat<T>::operator+=(const RWGenMat<T>& u)
{
    if (sameDataBlock(u)) {
        return operator+=(u.copy());    // Avoid aliasing
    }
    u.lengthCheck(rows(), cols());
    typename RWGenMat<T>::const_iterator it_u = u.begin();
    typename RWGenMat<T>::iterator it = begin();
    typename RWGenMat<T>::iterator last = end();
    for (; it < last; INC_ROW(it), INC_ROW(it_u)) {
        RWBlas_util<T>::aplvv(it.row_length(), &*it, it.stride(), &*it_u, it_u.stride());
    }
    return *this;
}

template <class T>
RWGenMat<T>& RWGenMat<T>::operator-=(const RWGenMat<T>& u)
{
    if (sameDataBlock(u)) {
        return operator-=(u.copy());    // Avoid aliasing
    }
    u.lengthCheck(rows(), cols());
    typename RWGenMat<T>::const_iterator it_u;
    typename RWGenMat<T>::iterator it;
    typename RWGenMat<T>::iterator last = end();
    for (it = begin(), it_u = u.begin(); it < last; INC_ROW(it), INC_ROW(it_u)) {
        RWBlas_util<T>::amivv(it.row_length(), &*it, it.stride(), &*it_u, it_u.stride());
    }
    return *this;
}

template <class T>
RWGenMat<T>& RWGenMat<T>::operator*=(const RWGenMat<T>& u)
{
    if (sameDataBlock(u)) {
        return operator*=(u.copy());    // Avoid aliasing
    }
    u.lengthCheck(rows(), cols());
    typename RWGenMat<T>::const_iterator it_u;
    typename RWGenMat<T>::iterator it;
    typename RWGenMat<T>::iterator last = end();
    for (it = begin(), it_u = u.begin(); it < last; INC_ROW(it), INC_ROW(it_u)) {
        RWBlas_util<T>::amuvv(it.row_length(), &*it, it.stride(), &*it_u, it_u.stride());
    }
    return *this;
}

template <class T>
RWGenMat<T>& RWGenMat<T>::operator/=(const RWGenMat<T>& u)
{
    if (sameDataBlock(u)) {
        return operator/=(u.copy());    // Avoid aliasing
    }
    u.lengthCheck(rows(), cols());
    typename RWGenMat<T>::const_iterator it_u;
    typename RWGenMat<T>::iterator it;
    typename RWGenMat<T>::iterator last = end();
    for (it = begin(), it_u = u.begin(); it < last; INC_ROW(it), INC_ROW(it_u)) {
        RWBlas_util<T>::advvv(it.row_length(), &*it, it.stride(), &*it_u, it_u.stride());
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
RWGenMat<T>& RWGenMat<T>::operator+=(const T& scalar)
{
    typename RWGenMat<T>::iterator it;
    typename RWGenMat<T>::iterator last = end();
    for (it = begin(); it < last; INC_ROW(it)) {
        RWBlas_util<T>::aplvs(it.row_length(), &*it, it.stride(), &scalar);
    }
    return *this;
}

template <class T>
RWGenMat<T>& RWGenMat<T>::operator-=(const T& scalar)
{
    typename RWGenMat<T>::iterator it;
    typename RWGenMat<T>::iterator last = end();
    for (it = begin(); it < last; INC_ROW(it)) {
        RWBlas_util<T>::amlvs(it.row_length(), &*it, it.stride(), &scalar);
    }
    return *this;
}

template <class T>
RWGenMat<T>& RWGenMat<T>::operator*=(const T& scalar)
{
    typename RWGenMat<T>::iterator it;
    typename RWGenMat<T>::iterator last = end();
    for (it = begin(); it < last; INC_ROW(it)) {
        RWBlas_util<T>::amuvs(it.row_length(), &*it, it.stride(), &scalar);
    }
    return *this;
}

template <class T>
RWGenMat<T>& RWGenMat<T>::operator/=(const T& scalar)
{
    typename RWGenMat<T>::iterator it;
    typename RWGenMat<T>::iterator last = end();
    for (it = begin(); it < last; INC_ROW(it)) {
        RWBlas_util<T>::advvs(it.row_length(), &*it, it.stride(), &scalar);
    }
    return *this;
}


/************************************************
 *                                              *
 *              LOGICAL OPERATORS               *
 *                                              *
 ************************************************/

template <class T>
bool RWGenMat<T>::operator==(const RWGenMat<T>& u) const
{
    if (rows() != u.rows() || cols() != u.cols()) {
        return false;    // They can't be equal if they don't have the same length.
    }
    typename RWGenMat<T>::const_iterator it_u;
    typename RWGenMat<T>::const_iterator it;
    typename RWGenMat<T>::const_iterator last = end();
    for (it = begin(), it_u = u.begin(); it < last; INC_ROW(it), INC_ROW(it_u))
        if (!RWBlas_util<T>::same(it.row_length(), (T*)(&*it), it.stride(), &*it_u, it_u.stride())) {
            return false;
        }
    return true;
}

template <class T>
bool RWGenMat<T>::operator!=(const RWGenMat<T>& u) const
{
    return !(*this == u);
}

#endif // __MATOP_CC__
