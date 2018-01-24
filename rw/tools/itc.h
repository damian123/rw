#ifndef RW_TOOLS_ITC_H
#define RW_TOOLS_ITC_H

/**********************************************************************
 *
 * Declarations for struct RWITCGuard
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/itc.h#2 $
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

#if !defined(RW_NO_ITC)

#  include <libittnotify.h>

struct RWITCGuard {
    RWITCGuard(__itt_thr_prop_t const& prop,
               __itt_thr_state_t const& state) RW_NO_THROW
:
    m_state(__itt_state_get()) {

        // Alter state
        RW_ASSERT(prop == __itt_thr_prop_quiet);
        __itt_thr_mode_set(prop, state);
    }

    RWITCGuard(__itt_obj_prop_t const& prop,
               __itt_obj_state_t const& state) RW_NO_THROW
:
    m_state(__itt_state_get()) {

        // Alter state
        RW_ASSERT(prop == __itt_obj_prop_ignore);
        __itt_obj_mode_set(prop, state);
    }

    ~RWITCGuard() RW_NO_THROW {
        // Restore state
        __itt_state_set(m_state);
    }

protected:

    __itt_state_t m_state;

};

#endif // RW_NO_ITC

/**************************************************************************/

#if !defined(RW_NO_ITC)

#  define RW_ITC_GUARD(x,y) RWITCGuard RW_PASTE(g_, __LINE__) (x, y);

#  define RW_ITC_SYNC_PREPARE(x)  __itt_notify_sync_prepare(x)
#  define RW_ITC_SYNC_ACQUIRED(x) __itt_notify_sync_acquired(x)
#  define RW_ITC_SYNC_CANCEL(x)   __itt_notify_sync_cancel(x)
#  define RW_ITC_SYNC_RELEASE(x)  __itt_notify_sync_releasing(x)

#else

#  define RW_ITC_GUARD(ignore,me)

#  define RW_ITC_SYNC_PREPARE(ignore)
#  define RW_ITC_SYNC_ACQUIRED(ignore)
#  define RW_ITC_SYNC_CANCEL(ignore)
#  define RW_ITC_SYNC_RELEASE(ignore)

#endif // RW_NO_ITC

#endif /* RW_TOOLS_ITC_H */
