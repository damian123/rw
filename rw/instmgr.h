#ifndef RW_TOOLS_INSTMGR_H
#define RW_TOOLS_INSTMGR_H



/**********************************************************************
 *
 * Declarations for RWInstanceManager --- manages multithread and per-process
 * instance data.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/instmgr.h#1 $
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

#if defined(RW_MULTI_THREAD)

#  if defined(RW_POSIX_D10_THREADS)
#    include <pthread.h>
typedef pthread_key_t RWTSDKEY;

#  elif defined(RW_SOLARIS_THREADS)  // MT & Sun -- assuming Solaris 2.x/Cafe
#    include <thread.h>
typedef thread_key_t RWTSDKEY;

#  elif defined(_WIN32)
// using an explicit unsigned long instead of DWORD to avoid inclusion of
// a windows.h header
typedef unsigned long RWTSDKEY;
#  else
#    error RW_MULTI_THREAD not supported in this environment
#  endif


class RW_TOOLS_GLOBAL RWInstanceManager
{
public:
    void*   addValue();       // set instance specific data
    void*   currentValue();   // get instance specific data
    void    freeValue();      // release key and discard data

    // Values to be provided and discarded by the specializing class:
    virtual void*   newValue() = 0;
    virtual void    deleteValue(void*) = 0;

    RWInstanceManager();
    virtual ~RWInstanceManager();

private:
    RWTSDKEY              tsd_key;
};


#else   /* not Multithread */

//#error RWInstanceManager class not meant to be used outside DLL or MT situation
extern int rwDummyInstMgr;

#endif
#endif  /* RW_TOOLS_INSTMGR_H */
