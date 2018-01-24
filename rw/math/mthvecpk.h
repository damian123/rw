#ifndef __RWMTHVECPK_H__
#define __RWMTHVECPK_H__

/***************************************************************************
 *
 * mthvecpk.h
 *   The RWMathVecPick class allows selected elements to be addressed.
 *   There are no public constructors.
 *
 * $Id: //math/13/rw/math/mthvecpk.h#1 $
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
#include <rw/mathwind.h>
#include <rw/math/mathvec.h>

/**
 * @ingroup math_collection_classes
 *
 * @brief Allows selected elements to be addressed in a vector, without
 * creating a new view.
 *
 * The RWMathVecPick class allows selected elements to be addressed.
 * Unlike subscripting or slicing, picking does not create a new view.
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/math/mathvec.h>
 *
 * int main()
 * {
 *     // Set up picked elements:
 *     RWIntVec ipick("[3 4 8]");
 *
 *     RWMathVec<double> dv(10, 0, 2);     // 0 2 4 6 8 10 12 14 16 18
 *
 *     RWMathVecPick<double> dvPick = dv.pick(ipick);
 *
 *     std::cout << dvPick.length() << std::endl;    // 3
 *     std::cout << dvPick(0)       << std::endl;    // 6
 *     std::cout << dvPick(2)       << std::endl;    // 16
 *
 *     return 0;
 * }
 * @endcode
 *
 */
template< class T >
class RWMathVecPick
{
private:
    RWMathVec<T> V;
    const RWIntVec         pick;

    RWMathVecPick(RWMathVec<T>& v, const RWIntVec& x);

    friend class RWMathVec<T>;

protected:
    void          assertElements() const;
    void          lengthCheck(size_t) const;
public:
    RWMathVecPick<T>& operator=(const RWMathVec<T>& vec);

    /**
     * Assignment operator for RWMathVecPick. The picks must have the same
     * dimensions.  The expression: \par
     *
     * <tt>this = \a v;</tt> \par
     *
     * implies <tt>this<SUB>i</SUB> = \a v<SUB>i</SUB></tt>
     */
    RWMathVecPick<T>& operator=(const RWMathVecPick<T>&);

    RWMathVecPick<T>& operator=(const T&);

    /**
     * Returns a reference to the value in the associated RWMathVec stored at the
     * index indicated by the \a i <sup>th</sup> element of the RWMathVecPick.
     */
    inline    T&   operator()(int i);

    /**
     * Returns the value in the associated RWMathVec stored at the index
     * indicated by the \a i <sup>th</sup> element of the RWMathVecPick.
     */
    inline T   operator()(int i) const;

    /**
     * Return the number of elements in the pick
     */
    typename RWMathVec<T>::size_type length() const {
        return pick.length();
    }

};


//***********************************************
//
//      I N L I N E S
//
//***********************************************



template< class T >
inline RWMathVecPick<T>::RWMathVecPick(RWMathVec<T>& v, const RWIntVec& x) :
    V(v), pick(x)
{
#ifdef RWBOUNDS_CHECK
    assertElements();
#endif
}

template< class T > inline T&
RWMathVecPick<T>::operator()(int i)
{
    return V(pick(i));
}

template< class T > inline T
RWMathVecPick<T>::operator()(int i) const
{
    return V(pick(i));
}

#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/math/mthvecpk.cc>
#endif

#endif // __RWMTHVECPK_H__
