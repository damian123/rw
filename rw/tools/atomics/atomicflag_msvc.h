#ifndef rw_tools_atomicflag_msvc_h_
#define rw_tools_atomicflag_msvc_h_
/**********************************************************************
 *
 * $Id: //tools/13/rw/tools/atomics/atomicflag_msvc.h#1 $
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

// http://msdn.microsoft.com/en-us/library/windows/desktop/ms684122(v=vs.85).aspx


#include <rw/defs.h>
#include <rw/tools/atomicorder.h>

// VC 9.0 has conflicting definitions for the "ceil" function between
// math.h and intrin.h in 64-bit builds. Including math.h before intrin.h
// addresses this issue.
#if (_MSC_VER == 1500) && defined(_WIN64)
#  include <math.h>
#endif
#include <intrin.h>

class RWAtomicFlagImp
{
public:
    inline bool testAndSet(RWAtomicMemoryOrder order) volatile;
    inline void clear(RWAtomicMemoryOrder order) volatile;

#if (_MSC_VER >= 1600)
    volatile long value_;
#else
    long value_;
#endif
};

inline bool
RWAtomicFlagImp::testAndSet(RWAtomicMemoryOrder) volatile
{
#if (_MSC_VER >= 1600)
    return _interlockedbittestandset(&value_, 0) != 0;
#else
    return _interlockedbittestandset(const_cast<long*>(&value_), 0) != 0;
#endif
}

inline void
RWAtomicFlagImp::clear(RWAtomicMemoryOrder) volatile
{
#if (_MSC_VER >= 1600)
    _interlockedbittestandreset(&value_, 0);
#else
    _interlockedbittestandreset(const_cast<long*>(&value_), 0);
#endif
}

#endif
