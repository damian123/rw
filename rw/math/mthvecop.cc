#ifndef __RWMTHVECOP_CC__
#define __RWMTHVECOP_CC__

/***************************************************************************
 *
 * mthvecop.cc
 *
 * $Id: //math/13/rw/math/mthvecop.cc#1 $
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
 *
 **************************************************************************/

#include <rw/math/rwbla.h>


//************************************************
//
//      UNARY OPERATORS
//
//************************************************

// Unary minus on a RWMathVec<T>
template< class T>
RWMathVec<T> operator -(const RWMathVec<T>& s)
{
    size_t i = s.length();
    RWMathVec<T> temp(i, rwUninitialized);
    register T* sp = (T*)s.data();  // cast away constness to prevent Borland warning
    register T* dp = temp.data();
    register int j = s.stride();
    while (i--) {
        *dp++ = T(-(*sp));
        sp += j;
    }
    return temp;
}



// Unary prefix increment on a RWMathVec<T>; (i.e. ++a)
template< class T>
RWMathVec<T>& RWMathVec<T>::operator++()
{
    register size_t i = length();
    register T* sp = data();
    register int j = stride();
    while (i--) {
        //++(*sp);
        *sp = T(*sp + T(1));
        sp += j;
    }
    return *this;
}


// Unary prefix decrement on a RWMathVec<T> (i.e., --a)
template< class T>
RWMathVec<T>& RWMathVec<T>::operator--()
{
    register size_t i = length();
    register T* sp = data();
    register int j = stride();
    while (i--) {
        *sp -= T(1);
        sp += j;
    }
    return *this;
}



//************************************************
//
// BINARY OPERATORS
//       vector - vector
//
//************************************************

// Binary add element-by-element
template< class T> RWMathVec<T>
operator +(const RWMathVec<T>& u, const RWMathVec<T>& v)
{
    size_t i = v.length();
    u.lengthCheck(i);
    RWMathVec<T> temp(i, rwUninitialized);
    RWBlas_util<T>::plvv(i, temp.data(), u.data(), u.stride(), v.data(), v.stride());
    return temp;
}


// Binary subtract element-by-element
template< class T> RWMathVec<T>
operator -(const RWMathVec<T>& u, const RWMathVec<T>& v)
{
    size_t i = v.length();
    u.lengthCheck(i);
    RWMathVec<T> temp(i, rwUninitialized);
    RWBlas_util<T>::mivv(i, temp.data(), u.data(), u.stride(), v.data(), v.stride());
    return temp;
}


// Binary multiply element-by-element
template< class T> RWMathVec<T>
operator *(const RWMathVec<T>& u, const RWMathVec<T>& v)
{
    size_t i = v.length();
    u.lengthCheck(i);
    RWMathVec<T> temp(i, rwUninitialized);
    RWBlas_util<T>::muvv(i, temp.data(), u.data(), u.stride(), v.data(), v.stride());
    return temp;
}


// Binary divide element-by-element
template< class T> RWMathVec<T>
operator /(const RWMathVec<T>& u, const RWMathVec<T>& v)
{
    size_t i = v.length();
    u.lengthCheck(i);
    RWMathVec<T> temp(i, rwUninitialized);
    RWBlas_util<T>::dvvv(i, temp.data(), u.data(), u.stride(), v.data(), v.stride());
    return temp;
}

//************************************************
//
//      BINARY OPERATORS
//    vector - scalar
//
//************************************************

// Add a scalar
template< class T> RWMathVec<T>
operator +(const RWMathVec<T>& s, const T& scalar)
{
    size_t i = s.length();
    RWMathVec<T> temp(i, rwUninitialized);
    RWBlas_util<T>::plvs(i, temp.data(), s.data(), s.stride(), &scalar);
    return temp;
}


// Subtract from a scalar
template< class T> RWMathVec<T>
operator -(const T& scalar, const RWMathVec<T>& s)
{
    size_t i = s.length();
    RWMathVec<T> temp(i, rwUninitialized);
    RWBlas_util<T>::misv(i, temp.data(), &scalar, s.data(), s.stride());
    return temp;
}


// Subtract from a vector
template< class T> RWMathVec<T>
operator -(const RWMathVec<T>& s, const T& scalar)
{
    size_t i = s.length();
    RWMathVec<T> temp(i, rwUninitialized);
    RWBlas_util<T>::mivs(i, temp.data(), &scalar, s.data(), s.stride());
    return temp;
}


// Multiply by a scalar
template< class T> RWMathVec<T>
operator *(const RWMathVec<T>& s, const T& scalar)
{
    size_t i = s.length();
    RWMathVec<T> temp(s);
    temp.deepenShallowCopy();
    RW_ASSERT(i <= RW_LONG_MAX);
    RWBlas<T>::scal(RW_STATIC_CAST(long, i), scalar, temp.data(), temp.stride());
    return temp;
}


// Divide into a scalar
template< class T> RWMathVec<T>
operator /(const T& scalar, const RWMathVec<T>& s)
{
    size_t i = s.length();
    RWMathVec<T> temp(i, rwUninitialized);
    RWBlas_util<T>::dvsv(i, temp.data(), &scalar, s.data(), s.stride());
    return temp;
}

// Divide by a scalar
template< class T> RWMathVec<T>
operator /(const RWMathVec<T>& s, const T& scalar)
{
    size_t i = s.length();
    RWMathVec<T> temp(i, rwUninitialized);
    RWBlas_util<T>::dvvs(i, temp.data(), s.data(), s.stride(), &scalar);
    return temp;
}


//********************************************************
// Out-of-line versions for compilers which can't
// handle inlined temporaries:
//********************************************************

//#ifdef RW_NO_INLINED_TEMP_DESTRUCTORS
template< class T> RWMathVec<T>
operator *(const T& s, const RWMathVec<T>& V)
{
    return V * s;
}

template< class T> RWMathVec<T>
operator +(const T& s, const RWMathVec<T>& V)
{
    return V + s;
}

//#endif

//************************************************
//
//  ARITHMETIC ASSIGNMENT OPERATORS
//    with other vectors
//
//************************************************

template< class T> RWMathVec<T>&
RWMathVec<T>::operator +=(const RWMathVec<T>& u)
{
    if (sameDataBlock(u)) {
        return operator+=(u.deepCopy());   // Avoid aliasing
    }

    size_t i = u.length();
    lengthCheck(i);
    RWBlas_util<T>::aplvv(i, data(), stride(), u.data(), u.stride());
    return *this;
}


template< class T> RWMathVec<T>&
RWMathVec<T>::operator -=(const RWMathVec<T>& u)
{
    if (sameDataBlock(u)) {
        return operator-=(u.deepCopy());    // Avoid aliasing
    }

    size_t i = u.length();
    lengthCheck(i);
    RWBlas_util<T>::amivv(i, data(), stride(), u.data(), u.stride());
    return *this;
}


template< class T> RWMathVec<T>&
RWMathVec<T>::operator *=(const RWMathVec<T>& u)
{
    if (sameDataBlock(u)) {
        return operator*=(u.deepCopy());    // Avoid aliasing
    }

    size_t i = u.length();
    lengthCheck(i);
    RWBlas_util<T>::amuvv(i, data(), stride(), u.data(), u.stride());
    return *this;
}


template< class T> RWMathVec<T>&
RWMathVec<T>::operator /=(const RWMathVec<T>& u)
{
    if (sameDataBlock(u)) {
        return operator/=(u.deepCopy());    // Avoid aliasing
    }

    size_t i = u.length();
    lengthCheck(i);
    RWBlas_util<T>::advvv(i, data(), stride(), u.data(), u.stride());
    return *this;
}


//************************************************
//
//  ARITHMETIC ASSIGNMENT OPERATORS
//        with a scalar
//
//************************************************

template< class T> RWMathVec<T>&
RWMathVec<T>::operator +=(const T& scalar)
{
    RWBlas_util<T>::aplvs(length(), data(), stride(), &scalar);
    return *this;
}


template< class T> RWMathVec<T>&
RWMathVec<T>::operator -=(const T& scalar)
{
    RWBlas_util<T>::amlvs(length(), data(), stride(), &scalar);
    return *this;
}


template< class T> RWMathVec<T>&
RWMathVec<T>::operator *=(const T& scalar)
{
    RWBlas_util<T>::amuvs(length(), data(), stride(), &scalar);
    return *this;
}


template< class T> RWMathVec<T>&
RWMathVec<T>::operator /=(const T& scalar)
{
    RWBlas_util<T>::advvs(length(), data(), stride(), &scalar);
    return *this;
}


//************************************************
//
//      LOGICAL OPERATORS
//
//************************************************

template< class T> bool
RWMathVec<T>::operator ==(const RWMathVec<T>& u) const
{
    size_t i = length();
    if (i != u.length()) {
        return false;  // They can't be equal if they don't have the same length.
    }
    return RWBlas_util<T>::same(i, (T*)data(), stride(), u.data(), u.stride()) != 0;
}


template< class T> bool
RWMathVec<T>::operator !=(const RWMathVec<T>& u) const
{
    return !(*this == u);
}

#endif
