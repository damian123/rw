#ifndef RW_TOOLS_RWRECURSIVEMUTEX_H
#define RW_TOOLS_RWRECURSIVEMUTEX_H



/**********************************************************************
 *
 * Declarations for class RWRecursiveMutex
 * This class is a portable implementation of a recursive mutex lock
 * to be used for synchronizing multiple threads within a single process.
 * It is not suitable for use among threads of different processes.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/recursivemutex.h#1 $
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
#include <rw/mutex.h>

// Multithreaded Macro Safety checks
#if defined(RW_NO_THREADS) && defined(RW_MULTI_THREAD)
#  error cannot have RW_MULTI_THREAD with RW_NO_THREADS defined
#endif

#if defined(RW_POSIX_D10_THREADS) && defined(RW_SOLARIS_THREADS)
#  undef RW_SOLARIS_THREADS
#endif


struct RWtid {
    RWtid() : valid_(false) {}
    RWThreadIDType id_;
    bool valid_;
};

/*
  Include class definition of RWRecursiveMutex for all builds, with
  or without MT support.  All mutex operations for threadless builds
  will be no-ops.
*/

class RWRecursiveMutex // no RW_TOOLS_SYMBOLIC, since this is a header only class
{
private:

    // not defined
    RWRecursiveMutex(const RWRecursiveMutex&);
    RWRecursiveMutex& operator= (const RWRecursiveMutex&);

public:
    enum StaticCtor       {staticCtor};
    enum NoInitialization {noInitialization};

    RWRecursiveMutex();
    RWRecursiveMutex(StaticCtor);        // some statics need special handling
    RWRecursiveMutex(NoInitialization);

    int acquire();                       // block until mutex released, acquire and continue
    int release();                       // release the mutex

private:
    RWMutex   mutex_;
    RWtid     owner_;
    long      count_;

    static RWThreadIDType threadId() {
#if !defined(RW_MULTI_THREAD)
        return 0;
#elif defined(RW_POSIX_D10_THREADS)
        return pthread_self();
#elif defined(RW_SOLARIS_THREADS)
        return thr_self();
#elif defined(_WIN32)
        return GetCurrentThreadId();
#else
#  error Unsupported threading environment
#endif
    }
};

class RW_TOOLS_SYMBOLIC RWRecursiveGuard
{
private:
    // not defined
    RWRecursiveGuard(const RWRecursiveGuard&);
    RWRecursiveGuard& operator= (const RWRecursiveGuard&);

public:
    RWRecursiveGuard(RWRecursiveMutex& m);
    ~RWRecursiveGuard();

private:
    RWRecursiveMutex& recursiveMutex_;
};



/*
 * =================================
 * RWRecursiveMutex member functions
 * ---------------------------------
 */

#if defined(RW_MULTI_THREAD)

inline
RWRecursiveMutex::RWRecursiveMutex()
    : count_(0)
{}

inline
RWRecursiveMutex::RWRecursiveMutex(RWRecursiveMutex::StaticCtor) : mutex_(RWMutex::staticCtor)
{} // empty, because acquire() may already have been used.

inline RWRecursiveMutex::RWRecursiveMutex(RWRecursiveMutex::NoInitialization) : mutex_(RWMutex::noInitialization)
    , count_(0)
{}


inline int RWRecursiveMutex::acquire()
{
    int rc = 0;
    RWThreadIDType self = threadId();
    if (owner_.valid_ && self == owner_.id_) {
        ++count_;
    }
    else {
        rc = mutex_.acquire();
        owner_.id_ = self;
        count_ = 1;

        // valid MUST always be set last in order to ensure the above
        // read of this value is safe
        owner_.valid_ = true;
    }
    return rc;
}


inline int
RWRecursiveMutex::release()
{
    int rc = 0;
    RWThreadIDType self = threadId();
    if (owner_.valid_ && self == owner_.id_) {
        if (!--count_) {
            owner_.id_    = 0;

            // valid MUST always be set before the release (inside the
            // critical section)
            owner_.valid_ = false;
            rc = mutex_.release();
        }
    }
    else {
        // Usage Error: A mutex may not be released any other thread
        // than the owning thread which acquired it
        RW_ASSERT(0);
    }
    return rc;
}


/*
 * ========================================
 * RWRecursiveGuard inline member functions
 * ----------------------------------------
 */

inline RWRecursiveGuard::RWRecursiveGuard(RWRecursiveMutex& m) : recursiveMutex_(m)
{
    recursiveMutex_.acquire();
}

inline RWRecursiveGuard::~RWRecursiveGuard()
{
    recursiveMutex_.release();
}

#else /* if !defined(RW_MULTI_THREAD) */

inline      RWRecursiveMutex::RWRecursiveMutex() {}
inline      RWRecursiveMutex::RWRecursiveMutex(RWRecursiveMutex::StaticCtor) {}
inline      RWRecursiveMutex::RWRecursiveMutex(RWRecursiveMutex::NoInitialization) {}
inline int  RWRecursiveMutex::acquire()
{
    return 0;
}
inline int  RWRecursiveMutex::release()
{
    return 0;
}

inline      RWRecursiveGuard::RWRecursiveGuard(RWRecursiveMutex& m) : recursiveMutex_(m) {}
inline      RWRecursiveGuard::~RWRecursiveGuard() {}

#endif  /* RW_MULTI_THREAD */

#endif  /* RW_TOOLS_RWRECURSIVEMUTEX_H */
