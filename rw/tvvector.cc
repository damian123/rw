/**********************************************************************
 *
 * Template definitions for RWTValVector<T>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvvector.cc#1 $
 *
 **********************************************************************
 *
 * Copyright (c) 1989-2015 Rogue Wave Software, Inc.  All Rights Reserved.
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
 **********************************************************************/


#include <rw/defs.h>
#include <rw/edefs.h>
#include <rw/toolerr.h>
#include <rw/rwerr.h>

template<class T>
RWTValVector<T>::RWTValVector(size_type n, const_reference ival)
    : npts_(0), array_(0)
{
    RWTValVector<T> buf(n);

    pointer dst = buf.array_;
    while (n--) {
        *dst++ = ival;
    }

    buf.swap(*this);
}

template<class T>
RWTValVector<T>::RWTValVector(const RWTValVector<T>& rhs)
    : npts_(0), array_(0)
{
    RWTValVector<T> buf(rhs.npts_);

    pointer dst = buf.array_;
    pointer src = rhs.array_;

    size_type i = buf.npts_;
    while (i--) {
        *dst++ = *src++;
    }

    buf.swap(*this);
}

#if !defined(RW_NO_RVALUE_REFERENCES)

template<class T>
RWTValVector<T>::RWTValVector(RWTValVector<T> && rhs)
    : npts_(0),
      array_(0)
{
    rhs.swap(*this);
}

#endif // !RW_NO_RVALUE_REFERENCES

template<class T> RWTValVector<T>&
RWTValVector<T>::operator=(const RWTValVector<T>& rhs)
{
    if (&rhs != this) {
        RWTValVector<T>(rhs).swap(*this);
    }
    return *this;
}


#if !defined(RW_NO_RVALUE_REFERENCES)

template<class T>
RWTValVector<T>& RWTValVector<T>::operator=(RWTValVector<T> && rhs)
{
    if (&rhs != this) {
        rhs.swap(*this);
    }

    return *this;
}

#endif // !RW_NO_RVALUE_REFERENCES

template<class T> RWTValVector<T>&
RWTValVector<T>::operator=(const_reference val)
{
    for (size_type i = 0; i < npts_; i++) {
        array_[i] = val;
    }
    return *this;
}

template<class T> void
RWTValVector<T>::reshape(size_type N)
{
    if (N == npts_) {
        return;
    }

    RWTValVector<T> buf(N);

    pointer src = array_;
    pointer dst = buf.array_;

    size_type i = (buf.npts_ < npts_) ? buf.npts_ : npts_;
    while (i--) {
        *dst++ = *src++;
    }

    buf.swap(*this);
}

template <class T>
void RWTValVector<T>::swap(RWTValVector<T>& rhs)
{
    rw_swap(rhs.array_, array_);
    rw_swap(rhs.npts_, npts_);
}

template<class T> void
RWTValVector<T>::boundsCheck(size_type i) const
{
    if (i >= npts_) {
        RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEX, i, npts_)));
    }
}


#include <rw/epersist.h>

RW_DEFINE_PERSISTABLE_TEMPLATE(RWTValVector)

#ifdef RW_NO_TEMPLINST_ON_BASE
#  include <rw/pstream.h>
#  include <rw/bstream.h>
#  include <rw/estream.h>
RW_DEFINE_PERSIST_TEMPLATE_IO(RWTValVector, RWpistream, RWpostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWTValVector, RWbistream, RWbostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWTValVector, RWeistream, RWeostream)
#endif

template <class T, class S>
void rwSaveGuts(S& str, const RWTValVector<T>& c)
{
    str.putSizeT(c.entries());
    for (size_t i = 0; i < c.entries(); ++i) {
        str << c(i);
    }
}

template <class T, class S>
void rwRestoreGuts(S& str, RWTValVector<T>& c)
{
    size_t count;
    str.getSizeT(count);

    RWTValVector<T> buf(count);
    for (size_t i = 0; i < buf.entries(); ++i) {
        str >> buf(i);
    }

    buf.swap(c);
}
