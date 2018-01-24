#ifndef __MATPICK_H__
#define __MATPICK_H__

/***************************************************************************
 *
 * matpick.h
 *
 * $Id: //math/13/rw/math/matpick.h#1 $
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

#include <rw/math/mathvec.h>

/*
 * The RWGenMatPick class allows selected elements to be addressed.
 * There are no public constructors.
 */

template <class T>
class RWGenMatPick
{
    friend class RWGenMat<T>;
private:
    RWGenMatPick(RWGenMat<T>&, const RWIntVec&, const RWIntVec&);
    RWGenMat<T>&             V;
    const RWIntVec     rowpick;
    const RWIntVec     colpick;
protected:
    void                  assertElements() const;
    void                  lengthCheck(size_t, size_t) const;
public:
    RWGenMatPick<T>&      operator=(const RWGenMat<T>&);
    RWGenMatPick<T>&      operator=(const RWGenMatPick<T>&);
    RWGenMatPick<T>&      operator=(const T&);

    size_t              rows() const {
        return rowpick.length();
    }
    size_t              cols() const {
        return colpick.length();
    }
};

/************************************************
 *                                              *
 *              I N L I N E S                   *
 *                                              *
 ************************************************/

template <class T>
inline
RWGenMatPick<T>::RWGenMatPick(RWGenMat<T>& v, const RWIntVec& x, const RWIntVec& y) :
    V(v), rowpick(x), colpick(y)
{
#ifdef RWBOUNDS_CHECK
    assertElements();
#endif
}

#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/math/matpick.cc>
#endif

#endif //__MATPICK_H__
