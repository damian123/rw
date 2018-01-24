#ifndef __DATAVIEW_CC__
#define __DATAVIEW_CC__
/***************************************************************************
 *
 * dataview.cc
 *
 * $Id: //math/13/rw/dataview.cc#1 $
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

#include <rw/math/rwalloc.h>

//---------------------------------------------------------------------
//
// RWAllocBlock member functions.
//
//----------------------------------------------------------------------

template< class T, class Allocator >
RWAllocBlock<T, Allocator>::RWAllocBlock(size_t numElements)
{
    void* data = rwnil;
    if (numElements != 0) {
        data = (void*)the_allocator.allocate(numElements);
    }

    // use the base class method init() to set the data pointer and length
    init(data, numElements * sizeof(T));
}


template< class T, class Allocator >
RWAllocBlock<T, Allocator>::~RWAllocBlock()
{
    T* start = (T*)data();
    T* end = start + length();
    rw_math_destroy(start, end);
    the_allocator.deallocate((T*)data());
}

#endif //__DATAVIEW_CC__
