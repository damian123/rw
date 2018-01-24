#ifndef RW_TOOLS_MUTEX_H
#define RW_TOOLS_MUTEX_H

/**********************************************************************
 *
 * Declarations for class RWMutex
 * This class is a portable implementation of a simple mutex lock
 * to be used for synchronizing multiple threads within a single process.
 * It is not suitable for use among threads of different processes.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/mutex.h#1 $
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

#include <rw/rwerr.h>
#include <rw/tools/atomic.h>

// Multithreaded macro safety checks
#if defined(RW_NO_THREADS) && defined(RW_MULTI_THREAD)
#  error Cannot have RW_MULTI_THREAD with RW_NO_THREADS defined
#endif


#if defined(RW_POSIX_D10_THREADS) && defined(RW_SOLARIS_THREADS)
#  undef RW_SOLARIS_THREADS
#endif


#if defined(RW_MULTI_THREAD)


#  if defined(RW_SOLARIS_THREADS)

#    include <synch.h>
#    include <thread.h>

#    define RW_MUTEX_T                mutex_t
#    define RW_THREADID_T             thread_t
#    define RW_THREADID()             thr_self()

#    define RW_MUTEX_INIT(mutex)      mutex_init    (&mutex, USYNC_THREAD, 0)
#    define RW_MUTEX_DESTROY(mutex)   mutex_destroy (&mutex)
#    define RW_MUTEX_LOCK(mutex)      mutex_lock    (&mutex)
#    define RW_MUTEX_UNLOCK(mutex)    mutex_unlock  (&mutex)

// Initializer used for objects that need constant initializers
#    define RW_MUTEX_INITIALIZER \
            { RW_ATOMIC_INIT(0), DEFAULTMUTEX }

#    define RW_MUTEX_RECURSIVE_INITIALIZER \
           { RW_ATOMIC_INIT(0), DEFAULTMUTEX, RW_ATOMIC_INIT(0), 0 }


#  elif defined(RW_POSIX_D10_THREADS)


#    include <pthread.h>

#    define RW_MUTEX_T                pthread_mutex_t
#    define RW_THREADID_T             pthread_t
#    define RW_THREADID()             pthread_self()

#    define RW_MUTEX_INIT(mutex)      pthread_mutex_init    (&mutex, 0)
#    define RW_MUTEX_DESTROY(mutex)   pthread_mutex_destroy (&mutex)
#    define RW_MUTEX_LOCK(mutex)      pthread_mutex_lock    (&mutex)
#    define RW_MUTEX_UNLOCK(mutex)    pthread_mutex_unlock  (&mutex)


#    define RW_PTHREAD_MUTEX_INITIALIZER      PTHREAD_MUTEX_INITIALIZER


#    define RW_MUTEX_INITIALIZER \
            { RW_ATOMIC_INIT(0), RW_PTHREAD_MUTEX_INITIALIZER }

#    define RW_MUTEX_RECURSIVE_INITIALIZER \
            { RW_ATOMIC_INIT(0), RW_PTHREAD_MUTEX_INITIALIZER, RW_ATOMIC_INIT(0), 0 }


#  elif defined(_WIN32)

// fake critical section type
union __rw_critical_section {
    long _C_pad;   // force alignment
    char _C_buf [RW_CRITICAL_SECTION_T];
};


#    define RW_MUTEX_T          __rw_critical_section
// use explicit unsigned long instead of DWORD
#    define RW_THREADID_T       unsigned long
#    define RW_THREADID()       GetCurrentThreadId()

extern "C" {

    struct _RTL_CRITICAL_SECTION;

    __declspec(dllimport) void __stdcall
    InitializeCriticalSection(_RTL_CRITICAL_SECTION*);

    __declspec(dllimport) void __stdcall
    EnterCriticalSection(_RTL_CRITICAL_SECTION*);

    __declspec(dllimport) void __stdcall
    LeaveCriticalSection(_RTL_CRITICAL_SECTION*);

    __declspec(dllimport) void __stdcall
    DeleteCriticalSection(_RTL_CRITICAL_SECTION*);

    __declspec(dllimport) unsigned long __stdcall
    GetCurrentThreadId(void);

}   // extern "C"

RW_ANON_NAMESPACE {

    inline int rwMutexInit(_RTL_CRITICAL_SECTION * mutex) {
        __try {
            InitializeCriticalSection(mutex);
        }
        __except (1) {
            return -1;
        }

        return 0;
    }

    inline int rwMutexDestroy(_RTL_CRITICAL_SECTION * mutex) {
        __try {
            DeleteCriticalSection(mutex);
        }
        __except (1) {
            return -1;
        }

        return 0;
    }

    inline int rwMutexLock(_RTL_CRITICAL_SECTION * mutex) {
        __try {
            EnterCriticalSection(mutex);
        }
        __except (1) {
            return -1;
        }

        return 0;
    }

    inline int rwMutexUnlock(_RTL_CRITICAL_SECTION * mutex) {
        __try {
            LeaveCriticalSection(mutex);
        }
        __except (1) {
            return -1;
        }

        return 0;
    }

} // anonymous namespace


#    define CRITICAL_SECTION_INITIALIZER   { 0 }

#    define RW_MUTEX_INITIALIZER \
          { RW_ATOMIC_INIT(0), CRITICAL_SECTION_INITIALIZER }

#    define RW_MUTEX_RECURSIVE_INITIALIZER \
          { RW_ATOMIC_INIT(0), CRITICAL_SECTION_INITIALIZER, RW_ATOMIC_INIT(0), 0 }


#    define RW_MUTEX_INIT(mutex) \
          rwMutexInit    (RW_REINTERPRET_CAST (_RTL_CRITICAL_SECTION*, &mutex))

#    define RW_MUTEX_DESTROY(mutex) \
          rwMutexDestroy (RW_REINTERPRET_CAST (_RTL_CRITICAL_SECTION*, &mutex))

#    define RW_MUTEX_LOCK(mutex) \
          rwMutexLock    (RW_REINTERPRET_CAST (_RTL_CRITICAL_SECTION*, &mutex))

#    define RW_MUTEX_UNLOCK(mutex) \
          rwMutexUnlock  (RW_REINTERPRET_CAST (_RTL_CRITICAL_SECTION*, &mutex))

#  else

#    error Class RWMutex is not yet supported in this environment

#  endif // RW_SOLARIS_THREADS 

// Legacy typedefs
typedef RW_MUTEX_T      RWMutexType;
typedef RW_THREADID_T   RWThreadIDType;

/**************************************************************************/

// Delegate mutex initialization and acquire/release. The init functions
// are identical in all respects now, since we are not actually using
// a recursive mutex but mimicking its behavior through a regular, fast
// mutex.
struct MutexFastPolicy {
    static int init(RWTAtomic<unsigned char>&, RW_MUTEX_T&);

    static int acquire(RW_MUTEX_T&);

    static int release(RW_MUTEX_T&);
};

struct MutexRecursivePolicy {
    static int init(RWTAtomic<unsigned char>&, RW_THREADID_T&, RWTAtomic<size_t>&, RW_MUTEX_T&);

    static int acquire(RW_MUTEX_T&, RW_THREADID_T&, RWTAtomic<size_t>&);

    static int release(RW_MUTEX_T&, RW_THREADID_T&, RWTAtomic<size_t>&);
};

struct StaticStoragePolicy {
    static int always_init();
};

struct NonStaticStoragePolicy {
    static int always_init();
};


// Mutex objects storages: static duration storage and auto/dynamic storage.
// For objects with static storage duration RWTMutex will be a suitable
// POD-struct, whereas for local (automatic) objects or dynamic storage
// objects that need resource release upon their destruction, will be a non-POD.

// Provide all specializations for compilers like MSVC 6.0 which do not
// understand partial specializations of class templates.

template<class T, class S>
struct RWTMutex {
    typedef   T   MutexPolicy;
    typedef   S   StoragePolicy;

    int acquire();
    int release();
};


// Specialization for static, fast mutexes
template<>
struct RW_TOOLS_SYMBOLIC
        RWTMutex<MutexFastPolicy, StaticStoragePolicy> {
    typedef   MutexFastPolicy        MutexPolicy;
    typedef   StaticStoragePolicy    StoragePolicy;

    int init();

    int acquire();

    int release();

    RWTAtomic<unsigned char> init_;
    RW_MUTEX_T mutex_;

};

// Specialization for static storage, recursive mutexes
template<>
struct RW_TOOLS_SYMBOLIC
        RWTMutex<MutexRecursivePolicy, StaticStoragePolicy> {
    typedef   MutexRecursivePolicy   MutexPolicy;
    typedef   StaticStoragePolicy    StoragePolicy;

    int init();

    int acquire();

    int release();

    RWTAtomic<unsigned char> init_;
    RW_MUTEX_T mutex_;

    RWTAtomic<size_t> count_;
    RW_THREADID_T thread_id_;

};

// Specialization for auto/dynamic storage, fast mutexes
template<>
struct RW_TOOLS_SYMBOLIC
        RWTMutex<MutexFastPolicy, NonStaticStoragePolicy> {
private:

    // not implemented
    RWTMutex(const RWTMutex&);
    RWTMutex& operator= (const RWTMutex&);

public:

    typedef   MutexFastPolicy          MutexPolicy;
    typedef   NonStaticStoragePolicy   StoragePolicy;

    // Provide the convenience of delaying initialization of RWTMutex objects
    enum DelayedInitialization {
        delayedInitialization = 0,
        noInitialization      = 0  // legacy value/name
    };


    RWTMutex();

    // Delayed initialization constructor
    RWTMutex(DelayedInitialization);

    ~RWTMutex();

    int init();

    int acquire();

    int release();

    RWTAtomic<unsigned char> init_;
    RW_MUTEX_T mutex_;

};


// Specialization for auto/dynamic storage, recursive mutexes
template<>
struct RW_TOOLS_SYMBOLIC
        RWTMutex<MutexRecursivePolicy, NonStaticStoragePolicy> {
private:

    // not implemented
    RWTMutex(const RWTMutex&);
    RWTMutex& operator= (const RWTMutex&);

public:

    typedef   MutexRecursivePolicy     MutexPolicy;
    typedef   NonStaticStoragePolicy   StoragePolicy;

    // Provide the convenience of delaying initialization of RWLock objects
    enum DelayedInitialization {
        delayedInitialization = 0,
        noInitialization      = 0  // legacy value/name
    };


    RWTMutex();

    // Delayed initialization constructor
    RWTMutex(DelayedInitialization);

    ~RWTMutex();

    int init();

    int acquire();

    int release();

    RWTAtomic<unsigned char> init_;
    RW_MUTEX_T mutex_;

    RWTAtomic<size_t> count_;
    RW_THREADID_T thread_id_;

};


// LOCK GUARD for RWLock
template<class T>
struct RWTMutexGuard {
private:

    // not defined
    RWTMutexGuard(const RWTMutexGuard&);
    RWTMutexGuard operator= (const RWTMutexGuard&);

public:

    typedef  T   MutexType;

    RWTMutexGuard(MutexType& lock)
        : lock_(lock) {
        lock_.acquire();
    }

    ~RWTMutexGuard() {
        lock_.release();
    }

    MutexType& lock_;
};

/**************************************************************************/

// Legacy RWMutex
struct  RW_TOOLS_SYMBOLIC RWMutex {
private:

    // not defined
    RWMutex(const RWMutex&);
    RWMutex& operator= (const RWMutex&);

public:

    enum StaticCtor {
        staticCtor
    };

    enum NoInitialization {
        noInitialization
    };

    RWMutex();

    RWMutex(StaticCtor);

    RWMutex(NoInitialization);

    ~RWMutex();

    int init();

    int acquire();

    int release();

private:

    RW_MUTEX_T mutex_;

    RWTAtomic<unsigned char> init_;
};

// RWGuard class
struct RW_TOOLS_SYMBOLIC RWGuard {
private:

    // not defined
    RWGuard(const RWGuard&);
    RWGuard operator= (const RWGuard&);

public:

    RWGuard(RWMutex&);

    ~RWGuard();

private:

    RWMutex& mutex_;

};


#else  /* MULTI_THREAD ****************************************************/


// Dummy types
typedef int      RWMutexType;
typedef unsigned RWThreadIDType;

#  define RW_MUTEX_T                RWMutexType
#  define RW_THREADID()             ((void)0)
#  define RW_THREADID_T             RWThreadIDType

struct StaticStoragePolicy    {};
struct NonStaticStoragePolicy {};

struct MutexFastPolicy       {};
struct MutexRecursivePolicy  {};


template<class T, class S>
struct RWTMutex {
    typedef   T   MutexPolicy;
    typedef   S   StoragePolicy;

    int acquire() {
        return 0;
    }

    int release() {
        return 0;
    }

    int ignore_;
};


// Dummy specializations
template<>
struct RWTMutex<MutexFastPolicy , NonStaticStoragePolicy> {
private:

    // not implemented
    RWTMutex(const RWTMutex&);
    RWTMutex& operator= (const RWTMutex&);

public:

    typedef   MutexFastPolicy          MutexPolicy;
    typedef   NonStaticStoragePolicy   StoragePolicy;


    enum DelayedInitialization {
        delayedInitialization = 0,
        noInitialization      = 0  // legacy value/name
    };

    RWTMutex() {
    }

    RWTMutex(DelayedInitialization) {
    }

    int acquire() {
        return 0;
    }

    int release() {
        return 0;
    }

    int ignore_;
};

template<>
struct RWTMutex<MutexRecursivePolicy , NonStaticStoragePolicy> {
private:

    // not implemented
    RWTMutex(const RWTMutex&);
    RWTMutex& operator= (const RWTMutex&);

public:

    typedef   MutexRecursivePolicy     MutexPolicy;
    typedef   NonStaticStoragePolicy   StoragePolicy;

    enum DelayedInitialization {
        delayedInitialization = 0,
        noInitialization      = 0  // legacy value/name
    };

    RWTMutex() {
    }

    RWTMutex(DelayedInitialization) {
    }

    int acquire() {
        return 0;
    }

    int release() {
        return 0;
    }

    int ignore_;
};


// Initializer used for objects that need constant initializers
#  define RW_MUTEX_INITIALIZER             { 0 }
#  define RW_MUTEX_RECURSIVE_INITIALIZER   { 0 }

template<class T>
struct RWTMutexGuard {
private:

    // not defined
    RWTMutexGuard(const RWTMutexGuard&);
    RWTMutexGuard& operator= (const RWTMutexGuard&);

public:

    typedef  T   MutexType;

    RWTMutexGuard(MutexType&) {
    }

    ~RWTMutexGuard() {
    }
};


struct RW_TOOLS_SYMBOLIC RWMutex {
private:

    // not defined
    RWMutex(const RWMutex&);
    RWMutex& operator= (const RWMutex&);

public:

    enum StaticCtor {
        staticCtor
    };

    enum NoInitialization {
        noInitialization
    };

    RWMutex() {
    }

    RWMutex(StaticCtor) {
    }

    RWMutex(NoInitialization) {
    }

    ~RWMutex() {
    }

    int init() {
        return 0;
    }

    int acquire() {
        return 0;
    }

    int release() {
        return 0;
    }
};

struct RW_TOOLS_SYMBOLIC RWGuard {
private:

    // not implemented
    RWGuard(const RWGuard&);
    RWGuard& operator= (const RWGuard&);

public:

    RWGuard(RWMutex& ref)
        : mutex_(ref) {
    }

    ~RWGuard() {
    }

private:

    RWMutex& mutex_;

};

#endif  // RW_MULTI_THREAD


// Convenience typedefs for user and internal usage;
// here static refers to storage duration.
typedef RWTMutex<MutexFastPolicy, StaticStoragePolicy>
RWStaticFastMutex;

typedef RWTMutex<MutexFastPolicy, NonStaticStoragePolicy>
RWLocalFastMutex;

typedef RWTMutex<MutexRecursivePolicy, StaticStoragePolicy>
RWStaticRecursiveMutex;

typedef RWTMutex<MutexRecursivePolicy, NonStaticStoragePolicy>
RWLocalRecursiveMutex;


#endif // RW_TOOLS_MUTEX_H
