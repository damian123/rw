#ifndef RW_TOOLS_TVREF_H
#define RW_TOOLS_TVREF_H


/**********************************************************************
 *
 * RWTVirtualRef<T>: A typed interface to RWVirtualRef
 * Although this array could be used by the end user it is really intended
 * to be the reference part of a reference counted array.  It uses
 * pure copy semantics and hence is relatively slow.
 * It inherits from RWReference and hence contains a reference count.
 * This is used to implement copy-on-write reference counting as was
 * done with class RWTValVirtualArray<T>.
 * Assumes that T has:
 *   - well-defined copy constructor (T::T(const T&) or equiv.);
 *   - well-defined assignment operator (T::operator=(const T&) or equiv.).
 * Note that the location in physical memory of a particular index
 * of the array can change from invocation to invocation.  Hence,
 * the address of an element should never be taken.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvref.h#1 $
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
#include <rw/vref.h>

#if !defined(RW_DISABLE_DEPRECATED)
RW_SUPPRESS_DEPRECATED_WARNINGS

#include <rw/toolerr.h>


template <class T> class  RWTVirtualRef : public RWVirtualRef
{

public:

    RWTVirtualRef(long size, RWVirtualPageHeap* h);
    RWTVirtualRef(RWTVirtualRef<T>&);             // Takes deep copy.

    T                     val(long i);
    void                  set(long i, const T& v);
    void                  set(long start, long extent, const T& v);
    virtual void          conformalCopy(long start1, RWVirtualRef& v2, long start2, long N);

};


#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tvref.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tvref.cc"
#endif

RW_RESTORE_DEPRECATED_WARNINGS
#endif

#endif  /* RW_TOOLS_TVREF_H */
