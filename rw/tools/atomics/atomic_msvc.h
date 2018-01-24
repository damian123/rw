#ifndef rw_tools_atomic_msvc_h_
#define rw_tools_atomic_msvc_h_
/**********************************************************************
 *
 * $Id: //tools/13/rw/tools/atomics/atomic_msvc.h#1 $
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
#include <rw/tools/traits/RWTConditional.h>

// VC 9.0 has conflicting definitions for the "ceil" function between
// math.h and intrin.h in 64-bit builds. Including math.h before intrin.h
// addresses this issue.
#if (_MSC_VER == 1500) && defined(_WIN64)
#  include <math.h>
#endif
#include <intrin.h>

#pragma warning(push)
#pragma warning(disable : 4127)     // warning C4127: conditional expression is constant

#pragma intrinsic(_InterlockedAnd)
#pragma intrinsic(_InterlockedCompareExchange)
#pragma intrinsic(_InterlockedCompareExchange64)
#pragma intrinsic(_InterlockedDecrement)
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedExchangeAdd)
#pragma intrinsic(_InterlockedIncrement)
#pragma intrinsic(_InterlockedOr)
#pragma intrinsic(_InterlockedXor)

#if defined(_M_IX86)
inline __int64 rwInterlockedExchange64(volatile __int64* target, __int64 value)
{
    __int64 prev = 0;
    __int64 res;
    while ((res = _InterlockedCompareExchange64(target, value, prev)) != prev) {
        prev = res;
    }
    return prev;
}

inline __int64 rwInterlockedExchangeAdd64(volatile __int64* target, __int64 value)
{
    __int64 prev = 0;
    __int64 res;
    while ((res = _InterlockedCompareExchange64(target, prev + value, prev)) != prev) {
        prev = res;
    }
    return prev;
}

inline __int64 rwInterlockedAnd64(volatile __int64* target, __int64 value)
{
    __int64 prev = 0;
    __int64 res;
    while ((res = _InterlockedCompareExchange64(target, prev & value, prev)) != prev) {
        prev = res;
    }
    return prev;
}

inline __int64 rwInterlockedOr64(volatile __int64* target, __int64 value)
{
    __int64 prev = 0;
    __int64 res;
    while ((res = _InterlockedCompareExchange64(target, prev | value, prev)) != prev) {
        prev = res;
    }
    return prev;
}

inline __int64 rwInterlockedXor64(volatile __int64* target, __int64 value)
{
    __int64 prev = 0;
    __int64 res;
    while ((res = _InterlockedCompareExchange64(target, prev ^ value, prev)) != prev) {
        prev = res;
    }
    return prev;
}

inline __int64 rwInterlockedIncrement64(volatile __int64* target)
{
    __int64 prev = 0;
    __int64 res;
    __int64 newValue = prev + 1;
    while ((res = _InterlockedCompareExchange64(target, newValue, prev)) != prev) {
        prev = res;
        newValue = prev + 1;
    }
    return newValue;
}

inline __int64 rwInterlockedDecrement64(volatile __int64* target)
{
    __int64 prev = 0;
    __int64 res;
    __int64 newValue = prev - 1;
    while ((res = _InterlockedCompareExchange64(target, newValue, prev)) != prev) {
        prev = res;
        newValue = prev - 1;
    }
    return newValue;
}

inline void rwMemoryBarrier(void)
{
    // Derived from definition of MemoryBarrier. Redefined here to avoid
    // dependency on windows.h
    long barrier;
    __asm xchg barrier, eax;
}

#endif

#define RW_INTERLOCKED_AND _InterlockedAnd
#define RW_INTERLOCKED_COMPARE_EXCHANGE _InterlockedCompareExchange
#define RW_INTERLOCKED_COMPARE_EXCHANGE64 _InterlockedCompareExchange64
#define RW_INTERLOCKED_DECREMENT _InterlockedDecrement
#define RW_INTERLOCKED_EXCHANGE _InterlockedExchange
#define RW_INTERLOCKED_EXCHANGE_ADD _InterlockedExchangeAdd
#define RW_INTERLOCKED_INCREMENT _InterlockedIncrement
#define RW_INTERLOCKED_OR _InterlockedOr
#define RW_INTERLOCKED_XOR _InterlockedXor
#if defined(_M_IX86)
#  define RW_INTERLOCKED_AND64 rwInterlockedAnd64
#  define RW_INTERLOCKED_DECREMENT64 rwInterlockedDecrement64
#  define RW_INTERLOCKED_EXCHANGE64 rwInterlockedExchange64
#  define RW_INTERLOCKED_EXCHANGE_ADD64 rwInterlockedExchangeAdd64
#  define RW_INTERLOCKED_INCREMENT64 rwInterlockedIncrement64
#  define RW_INTERLOCKED_OR64 rwInterlockedOr64
#  define RW_INTERLOCKED_XOR64 rwInterlockedXor64
#  define RW_MEMORY_BARRIER rwMemoryBarrier
#else
#  define RW_INTERLOCKED_AND64 _InterlockedAnd64
#  define RW_INTERLOCKED_DECREMENT64 _InterlockedDecrement64
#  define RW_INTERLOCKED_EXCHANGE64 _InterlockedExchange64
#  define RW_INTERLOCKED_EXCHANGE_ADD64 _InterlockedExchangeAdd64
#  define RW_INTERLOCKED_INCREMENT64 _InterlockedIncrement64
#  define RW_INTERLOCKED_OR64 _InterlockedOr64
#  define RW_INTERLOCKED_XOR64 _InterlockedXor64
#  if defined(_M_X64)
#    define RW_MEMORY_BARRIER __faststorefence
#  else
#    define RW_MEMORY_BARRIER __mf
#  endif
#endif

#if defined(_M_IX86) || defined(_M_X64)
#  define RW_INTERLOCKED_AND64_RELEASE RW_INTERLOCKED_AND64
#  define RW_INTERLOCKED_AND_RELEASE RW_INTERLOCKED_AND
#  define RW_INTERLOCKED_COMPARE_EXCHANGE64_RELEASE RW_INTERLOCKED_COMPARE_EXCHANGE64
#  define RW_INTERLOCKED_COMPARE_EXCHANGE_RELEASE RW_INTERLOCKED_COMPARE_EXCHANGE
#  define RW_INTERLOCKED_DECREMENT64_RELEASE RW_INTERLOCKED_DECREMENT64
#  define RW_INTERLOCKED_DECREMENT_RELEASE RW_INTERLOCKED_DECREMENT
#  define RW_INTERLOCKED_EXCHANGE64_RELEASE RW_INTERLOCKED_EXCHANGE64
#  define RW_INTERLOCKED_EXCHANGE_ADD64_RELEASE RW_INTERLOCKED_EXCHANGE_ADD64
#  define RW_INTERLOCKED_EXCHANGE_ADD_RELEASE RW_INTERLOCKED_EXCHANGE_ADD
#  define RW_INTERLOCKED_EXCHANGE_RELEASE RW_INTERLOCKED_EXCHANGE
#  define RW_INTERLOCKED_INCREMENT64_RELEASE RW_INTERLOCKED_INCREMENT64
#  define RW_INTERLOCKED_INCREMENT_RELEASE RW_INTERLOCKED_INCREMENT
#  define RW_INTERLOCKED_OR64_RELEASE RW_INTERLOCKED_OR64
#  define RW_INTERLOCKED_OR_RELEASE RW_INTERLOCKED_OR
#  define RW_INTERLOCKED_XOR64_RELEASE RW_INTERLOCKED_XOR64
#  define RW_INTERLOCKED_XOR_RELEASE RW_INTERLOCKED_XOR
#else
#  define RW_INTERLOCKED_AND64_RELEASE _InterlockedAnd64_rel
#  define RW_INTERLOCKED_AND_RELEASE _InterlockedAnd_rel
#  define RW_INTERLOCKED_COMPARE_EXCHANGE64_RELEASE _InterlockedCompareExchange64_rel
#  define RW_INTERLOCKED_COMPARE_EXCHANGE_RELEASE _InterlockedCompareExchange_rel
#  define RW_INTERLOCKED_DECREMENT64_RELEASE _InterlockedDecrement64_rel
#  define RW_INTERLOCKED_DECREMENT_RELEASE _InterlockedDecrement_rel
#  define RW_INTERLOCKED_EXCHANGE64_RELEASE _InterlockedExchange64_rel
#  define RW_INTERLOCKED_EXCHANGE_ADD64_RELEASE _InterlockedExchangeAdd64_rel
#  define RW_INTERLOCKED_EXCHANGE_ADD_RELEASE _InterlockedExchangeAdd_rel
#  define RW_INTERLOCKED_EXCHANGE_RELEASE RW_INTERLOCKED_EXCHANGE
#  define RW_INTERLOCKED_INCREMENT64_RELEASE _InterlockedIncrement64_rel
#  define RW_INTERLOCKED_INCREMENT_RELEASE _InterlockedIncrement_rel
#  define RW_INTERLOCKED_OR64_RELEASE _InterlockedOr64_rel
#  define RW_INTERLOCKED_OR_RELEASE _InterlockedOr_rel
#  define RW_INTERLOCKED_XOR64_RELEASE _InterlockedXor64_rel
#  define RW_INTERLOCKED_XOR_RELEASE _InterlockedXor_rel
#endif

#if defined(_M_IX86) || defined(_M_X64)
#  define RW_INTERLOCKED_AND64_ACQUIRE RW_INTERLOCKED_AND64
#  define RW_INTERLOCKED_AND_ACQUIRE RW_INTERLOCKED_AND
#  define RW_INTERLOCKED_COMPARE_EXCHANGE64_ACQUIRE RW_INTERLOCKED_COMPARE_EXCHANGE64
#  define RW_INTERLOCKED_COMPARE_EXCHANGE_ACQUIRE RW_INTERLOCKED_COMPARE_EXCHANGE
#  define RW_INTERLOCKED_DECREMENT64_ACQUIRE RW_INTERLOCKED_DECREMENT64
#  define RW_INTERLOCKED_DECREMENT_ACQUIRE RW_INTERLOCKED_DECREMENT
#  define RW_INTERLOCKED_EXCHANGE64_ACQUIRE RW_INTERLOCKED_EXCHANGE64
#  define RW_INTERLOCKED_EXCHANGE_ACQUIRE RW_INTERLOCKED_EXCHANGE
#  define RW_INTERLOCKED_EXCHANGE_ADD64_ACQUIRE RW_INTERLOCKED_EXCHANGE_ADD64
#  define RW_INTERLOCKED_EXCHANGE_ADD_ACQUIRE RW_INTERLOCKED_EXCHANGE_ADD
#  define RW_INTERLOCKED_INCREMENT64_ACQUIRE RW_INTERLOCKED_INCREMENT64
#  define RW_INTERLOCKED_INCREMENT_ACQUIRE RW_INTERLOCKED_INCREMENT
#  define RW_INTERLOCKED_OR64_ACQUIRE RW_INTERLOCKED_OR64
#  define RW_INTERLOCKED_OR_ACQUIRE RW_INTERLOCKED_OR
#  define RW_INTERLOCKED_XOR64_ACQUIRE RW_INTERLOCKED_XOR64
#  define RW_INTERLOCKED_XOR_ACQUIRE RW_INTERLOCKED_XOR
#else
#  define RW_INTERLOCKED_AND64_ACQUIRE _InterlockedAnd64_acq
#  define RW_INTERLOCKED_AND_ACQUIRE _InterlockedAnd_acq
#  define RW_INTERLOCKED_COMPARE_EXCHANGE64_ACQUIRE _InterlockedCompareExchange64_acq
#  define RW_INTERLOCKED_COMPARE_EXCHANGE_ACQUIRE _InterlockedCompareExchange_acq
#  define RW_INTERLOCKED_DECREMENT64_ACQUIRE _InterlockedDecrement64_acq
#  define RW_INTERLOCKED_DECREMENT_ACQUIRE _InterlockedDecrement_acq
#  define RW_INTERLOCKED_EXCHANGE64_ACQUIRE _InterlockedExchange64_acq
#  define RW_INTERLOCKED_EXCHANGE_ACQUIRE _InterlockedExchange_acq
#  define RW_INTERLOCKED_EXCHANGE_ADD64_ACQUIRE _InterlockedExchangeAdd64_acq
#  define RW_INTERLOCKED_EXCHANGE_ADD_ACQUIRE _InterlockedExchangeAdd_acq
#  define RW_INTERLOCKED_INCREMENT64_ACQUIRE _InterlockedIncrement64_acq
#  define RW_INTERLOCKED_INCREMENT_ACQUIRE _InterlockedIncrement_acq
#  define RW_INTERLOCKED_OR64_ACQUIRE _InterlockedOr64_acq
#  define RW_INTERLOCKED_OR_ACQUIRE _InterlockedOr_acq
#  define RW_INTERLOCKED_XOR64_ACQUIRE _InterlockedXor64_acq
#  define RW_INTERLOCKED_XOR_ACQUIRE _InterlockedXor_acq
#endif

template <size_t N>
struct RWAtomicImpSize {
    enum { size = N };
};

template <>
struct RWAtomicImpSize<1> {
    enum { size = 4 };
};

template <>
struct RWAtomicImpSize<2> {
    enum { size = 4 };
};

template <typename T, size_t N>
struct RWTAtomicImpTraits;

template <typename T>
struct RWTAtomicImpTraits<T, 4> {
    typedef T user_type;
    typedef typename RWTConditional < (sizeof(T) < sizeof(long)), long, T >::type storage_type;
    typedef long atomic_type;
    typedef long atomic_cast_type;
    typedef T arithmetic_type;

    enum { arithmetic_offset = 1 };
};

template <typename T>
struct RWTAtomicImpTraits<T, 8> {
    typedef T user_type;
    typedef T storage_type;
    typedef __int64 atomic_type;
    typedef __int64 atomic_cast_type;
    typedef T arithmetic_type;

    enum { arithmetic_offset = 1 };
};


template <typename T>
struct RWTAtomicImpTraits<T*, 4> {
    typedef T* user_type;
    typedef T* storage_type;
    typedef long atomic_type;
    typedef intptr_t atomic_cast_type;
    typedef ptrdiff_t arithmetic_type;

    enum { arithmetic_offset = sizeof(T) };
};

template <typename T>
struct RWTAtomicImpTraits<T*, 8> {
    typedef T* user_type;
    typedef T* storage_type;
    typedef __int64 atomic_type;
    typedef intptr_t atomic_cast_type;
    typedef ptrdiff_t arithmetic_type;

    enum { arithmetic_offset = sizeof(T) };
};

#define RW_ATOMICIMP_INIT(val) { val }

template <typename T, size_t N = RWAtomicImpSize<sizeof(T)>::size>
class RWTAtomicImp;

template <typename T>
class RWTAtomicImp<T, 4>
{
public:
    typedef RWTAtomicImpTraits<T, 4> traits_type;
    typedef typename traits_type::user_type user_type;
    typedef typename traits_type::storage_type storage_type;
    typedef typename traits_type::atomic_type atomic_type;
    typedef typename traits_type::atomic_cast_type atomic_cast_type;
    typedef typename traits_type::arithmetic_type arithmetic_type;

    void store(user_type, RWAtomicMemoryOrder order) volatile;
    user_type load(RWAtomicMemoryOrder order) const volatile;
    user_type swap(user_type, RWAtomicMemoryOrder order) volatile;
    bool compareAndSwap(user_type&, user_type, RWAtomicMemoryOrder order) volatile;
    user_type fetchAndAdd(arithmetic_type, RWAtomicMemoryOrder order) volatile;
    user_type fetchAndSub(arithmetic_type, RWAtomicMemoryOrder order) volatile;
    user_type fetchAndAnd(user_type, RWAtomicMemoryOrder order) volatile;
    user_type fetchAndOr(user_type, RWAtomicMemoryOrder order) volatile;
    user_type fetchAndXor(user_type, RWAtomicMemoryOrder order) volatile;

    user_type operator++(int) volatile;
    user_type operator++() volatile;
    user_type operator--(int) volatile;
    user_type operator--() volatile;
    user_type operator+=(arithmetic_type) volatile;
    user_type operator-=(arithmetic_type) volatile;
    user_type operator&=(user_type) volatile;
    user_type operator|=(user_type) volatile;
    user_type operator^=(user_type) volatile;

    volatile storage_type value_;
};

template <typename T>
inline void
RWTAtomicImp<T, 4>::store(typename RWTAtomicImp<T, 4>::user_type val,
                          RWAtomicMemoryOrder order) volatile
{
    // Visual C++ 2005 and higher guarantee that reads from volatile
    // variables have read-acquire semantics and writes to volatile
    // variables have write-release semantics.
    value_ = (storage_type)val;
    switch (order) {
        case rw_mem_order_seq_cst:
            RW_MEMORY_BARRIER();
            break;
        case rw_mem_order_release:
        case rw_mem_order_relaxed:
            break;
            RW_DISABLE_BULLSEYE
        default:
            RW_ASSERT(0);
            RW_ENABLE_BULLSEYE
    };
}

template <typename T>
inline typename RWTAtomicImp<T, 4>::user_type
RWTAtomicImp<T, 4>::load(RWAtomicMemoryOrder order) const volatile
{
    // Visual C++ 2005 and higher guarantee that reads from volatile
    // variables have read-acquire semantics and writes to volatile
    // variables have write-release semantics.
    switch (order) {
        case rw_mem_order_seq_cst:
            RW_MEMORY_BARRIER();
            break;
        case rw_mem_order_acquire:
        case rw_mem_order_consume:
        case rw_mem_order_relaxed:
            break;
            RW_DISABLE_BULLSEYE
        default:
            RW_ASSERT(0);
            RW_ENABLE_BULLSEYE
    };
    return (user_type)value_;
}

template <typename T>
inline typename RWTAtomicImp<T, 4>::user_type
RWTAtomicImp<T, 4>::swap(typename RWTAtomicImp<T, 4>::user_type val,
                         RWAtomicMemoryOrder order) volatile
{
    atomic_type res;
    switch (order) {
        case rw_mem_order_seq_cst:
        case rw_mem_order_acq_rel:
            res = RW_INTERLOCKED_EXCHANGE((volatile atomic_type*)&value_,
                                          (atomic_cast_type)val);
            break;
        case rw_mem_order_release:
            res = RW_INTERLOCKED_EXCHANGE_RELEASE((volatile atomic_type*)&value_,
                                                  (atomic_cast_type)val);
            break;
        case rw_mem_order_acquire:
        case rw_mem_order_consume:
        case rw_mem_order_relaxed:
            res = RW_INTERLOCKED_EXCHANGE_ACQUIRE((volatile atomic_type*)&value_,
                                                  (atomic_cast_type)val);
            break;
            RW_DISABLE_BULLSEYE
        default:
            RW_ASSERT(0);
            RW_ENABLE_BULLSEYE
    }
    return (user_type)res;
}

template <typename T>
inline bool
RWTAtomicImp<T, 4>::compareAndSwap(typename RWTAtomicImp<T, 4>::user_type& expected,
                                   typename RWTAtomicImp<T, 4>::user_type desired,
                                   RWAtomicMemoryOrder order) volatile
{
    atomic_type res;
    switch (order) {
        case rw_mem_order_seq_cst:
        case rw_mem_order_acq_rel:
            res = RW_INTERLOCKED_COMPARE_EXCHANGE((volatile atomic_type*)&value_,
                                                  (atomic_cast_type)desired,
                                                  (atomic_cast_type)expected);
            break;
        case rw_mem_order_release:
            res = RW_INTERLOCKED_COMPARE_EXCHANGE_RELEASE((volatile atomic_type*)&value_,
                    (atomic_cast_type)desired,
                    (atomic_cast_type)expected);
            break;
        case rw_mem_order_acquire:
        case rw_mem_order_consume:
        case rw_mem_order_relaxed:
            res = RW_INTERLOCKED_COMPARE_EXCHANGE_ACQUIRE((volatile atomic_type*)&value_,
                    (atomic_cast_type)desired,
                    (atomic_cast_type)expected);
            break;
            RW_DISABLE_BULLSEYE
        default:
            RW_ASSERT(0);
            RW_ENABLE_BULLSEYE
    };
    bool ret = (res == (atomic_cast_type)expected);
    expected = (user_type)res;
    return ret;
}

template <typename T>
inline typename RWTAtomicImp<T, 4>::user_type
RWTAtomicImp<T, 4>::fetchAndAdd(typename RWTAtomicImp<T, 4>::arithmetic_type val,
                                RWAtomicMemoryOrder order) volatile
{
    atomic_type res;
    switch (order) {
        case rw_mem_order_seq_cst:
        case rw_mem_order_acq_rel:
            res = RW_INTERLOCKED_EXCHANGE_ADD((volatile atomic_type*)&value_,
                                              (atomic_cast_type)(val * traits_type::arithmetic_offset));
            break;
        case rw_mem_order_release:
            res = RW_INTERLOCKED_EXCHANGE_ADD_RELEASE((volatile atomic_type*)&value_,
                    (atomic_cast_type)(val * traits_type::arithmetic_offset));
            break;
        case rw_mem_order_acquire:
        case rw_mem_order_consume:
        case rw_mem_order_relaxed:
            res = RW_INTERLOCKED_EXCHANGE_ADD_ACQUIRE((volatile atomic_type*)&value_,
                    (atomic_cast_type)(val * traits_type::arithmetic_offset));
            break;
            RW_DISABLE_BULLSEYE
        default:
            RW_ASSERT(0);
            RW_ENABLE_BULLSEYE
    }
    return (user_type)res;
}

template <typename T>
inline typename RWTAtomicImp<T, 4>::user_type
RWTAtomicImp<T, 4>::fetchAndSub(typename RWTAtomicImp<T, 4>::arithmetic_type val,
                                RWAtomicMemoryOrder order) volatile
{
    atomic_type res;
    switch (order) {
        case rw_mem_order_seq_cst:
        case rw_mem_order_acq_rel:
            res = RW_INTERLOCKED_EXCHANGE_ADD((volatile atomic_type*)&value_,
                                              -(atomic_cast_type)(val * traits_type::arithmetic_offset));
            break;
        case rw_mem_order_release:
            res = RW_INTERLOCKED_EXCHANGE_ADD_RELEASE((volatile atomic_type*)&value_,
                    -(atomic_cast_type)(val * traits_type::arithmetic_offset));
            break;
        case rw_mem_order_acquire:
        case rw_mem_order_consume:
        case rw_mem_order_relaxed:
            res = RW_INTERLOCKED_EXCHANGE_ADD_ACQUIRE((volatile atomic_type*)&value_,
                    -(atomic_cast_type)(val * traits_type::arithmetic_offset));
            break;
            RW_DISABLE_BULLSEYE
        default:
            RW_ASSERT(0);
            RW_ENABLE_BULLSEYE
    }
    return (user_type)res;
}

template <typename T>
inline typename RWTAtomicImp<T, 4>::user_type
RWTAtomicImp<T, 4>::fetchAndAnd(typename RWTAtomicImp<T, 4>::user_type val,
                                RWAtomicMemoryOrder order) volatile
{
    atomic_type res;
    switch (order) {
        case rw_mem_order_seq_cst:
        case rw_mem_order_acq_rel:
            res = RW_INTERLOCKED_AND((volatile atomic_type*)&value_, val);
            break;
        case rw_mem_order_release:
            res = RW_INTERLOCKED_AND_RELEASE((volatile atomic_type*)&value_, val);
            break;
        case rw_mem_order_acquire:
        case rw_mem_order_consume:
        case rw_mem_order_relaxed:
            res = RW_INTERLOCKED_AND_ACQUIRE((volatile atomic_type*)&value_, val);
            break;
            RW_DISABLE_BULLSEYE
        default:
            RW_ASSERT(0);
            RW_ENABLE_BULLSEYE
    }
    return (user_type)res;
}

template <typename T>
inline typename RWTAtomicImp<T, 4>::user_type
RWTAtomicImp<T, 4>::fetchAndOr(typename RWTAtomicImp<T, 4>::user_type val,
                               RWAtomicMemoryOrder order) volatile
{
    atomic_type res;
    switch (order) {
        case rw_mem_order_seq_cst:
        case rw_mem_order_acq_rel:
            res = RW_INTERLOCKED_OR((volatile atomic_type*)&value_, val);
            break;
        case rw_mem_order_release:
            res = RW_INTERLOCKED_OR_RELEASE((volatile atomic_type*)&value_, val);
            break;
        case rw_mem_order_acquire:
        case rw_mem_order_consume:
        case rw_mem_order_relaxed:
            res = RW_INTERLOCKED_OR_ACQUIRE((volatile atomic_type*)&value_, val);
            break;
            RW_DISABLE_BULLSEYE
        default:
            RW_ASSERT(0);
            RW_ENABLE_BULLSEYE
    }
    return (user_type)res;
}

template <typename T>
inline typename RWTAtomicImp<T, 4>::user_type
RWTAtomicImp<T, 4>::fetchAndXor(typename RWTAtomicImp<T, 4>::user_type val,
                                RWAtomicMemoryOrder order) volatile
{
    atomic_type res;
    switch (order) {
        case rw_mem_order_seq_cst:
        case rw_mem_order_acq_rel:
            res = RW_INTERLOCKED_XOR((volatile atomic_type*)&value_, val);
            break;
        case rw_mem_order_release:
            res = RW_INTERLOCKED_XOR_RELEASE((volatile atomic_type*)&value_, val);
            break;
        case rw_mem_order_acquire:
        case rw_mem_order_consume:
        case rw_mem_order_relaxed:
            res = RW_INTERLOCKED_XOR_ACQUIRE((volatile atomic_type*)&value_, val);
            break;
            RW_DISABLE_BULLSEYE
        default:
            RW_ASSERT(0);
            RW_ENABLE_BULLSEYE
    }
    return (user_type)res;
}


template <typename T>
inline typename RWTAtomicImp<T, 4>::user_type
RWTAtomicImp<T, 4>::operator++(int) volatile
{
    if (traits_type::arithmetic_offset == 1) {
        return (user_type)(RW_INTERLOCKED_INCREMENT((volatile atomic_type*)&value_) - 1);
    }
    else {
        return fetchAndAdd(1, rw_mem_order_seq_cst);
    }
}

template <typename T>
inline typename RWTAtomicImp<T, 4>::user_type
RWTAtomicImp<T, 4>::operator++() volatile
{
    if (traits_type::arithmetic_offset == 1) {
        return (user_type)RW_INTERLOCKED_INCREMENT((volatile atomic_type*)&value_);
    }
    else {
        return (*this) += 1;
    }
}

template <typename T>
inline typename RWTAtomicImp<T, 4>::user_type
RWTAtomicImp<T, 4>::operator--(int) volatile
{
    if (traits_type::arithmetic_offset == 1) {
        return (user_type)(RW_INTERLOCKED_DECREMENT((volatile atomic_type*)&value_) + 1);
    }
    else {
        return fetchAndSub(1, rw_mem_order_seq_cst);
    }
}

template <typename T>
inline typename RWTAtomicImp<T, 4>::user_type
RWTAtomicImp<T, 4>::operator--() volatile
{
    if (traits_type::arithmetic_offset == 1) {
        return (user_type)RW_INTERLOCKED_DECREMENT((volatile atomic_type*)&value_);
    }
    else {
        return (*this) -= 1;
    }
}

template <typename T>
inline typename RWTAtomicImp<T, 4>::user_type
RWTAtomicImp<T, 4>::operator+=(typename RWTAtomicImp<T, 4>::arithmetic_type val) volatile
{
    return fetchAndAdd(val, rw_mem_order_seq_cst) + val;
}

template <typename T>
inline typename RWTAtomicImp<T, 4>::user_type
RWTAtomicImp<T, 4>::operator-=(typename RWTAtomicImp<T, 4>::arithmetic_type val) volatile
{
    return fetchAndSub(val, rw_mem_order_seq_cst) - val;
}

template <typename T>
inline typename RWTAtomicImp<T, 4>::user_type
RWTAtomicImp<T, 4>::operator&=(typename RWTAtomicImp<T, 4>::user_type val) volatile
{
    return fetchAndAnd(val, rw_mem_order_seq_cst) & val;
}

template <typename T>
inline typename RWTAtomicImp<T, 4>::user_type
RWTAtomicImp<T, 4>::operator|=(typename RWTAtomicImp<T, 4>::user_type val) volatile
{
    return fetchAndOr(val, rw_mem_order_seq_cst) | val;
}

template <typename T>
inline typename RWTAtomicImp<T, 4>::user_type
RWTAtomicImp<T, 4>::operator^=(typename RWTAtomicImp<T, 4>::user_type val) volatile
{
    return fetchAndXor(val, rw_mem_order_seq_cst) ^ val;
}


template <typename T>
class RWTAtomicImp<T, 8>
{
public:
    typedef RWTAtomicImpTraits<T, 8> traits_type;
    typedef typename traits_type::user_type user_type;
    typedef typename traits_type::storage_type storage_type;
    typedef typename traits_type::atomic_type atomic_type;
    typedef typename traits_type::atomic_cast_type atomic_cast_type;
    typedef typename traits_type::arithmetic_type arithmetic_type;

    void store(user_type, RWAtomicMemoryOrder order) volatile;
    user_type load(RWAtomicMemoryOrder order) const volatile;
    user_type swap(user_type, RWAtomicMemoryOrder order) volatile;
    bool compareAndSwap(user_type&, user_type, RWAtomicMemoryOrder order) volatile;
    user_type fetchAndAdd(arithmetic_type, RWAtomicMemoryOrder order) volatile;
    user_type fetchAndSub(arithmetic_type, RWAtomicMemoryOrder order) volatile;
    user_type fetchAndAnd(user_type, RWAtomicMemoryOrder order) volatile;
    user_type fetchAndOr(user_type, RWAtomicMemoryOrder order) volatile;
    user_type fetchAndXor(user_type, RWAtomicMemoryOrder order) volatile;

    user_type operator++(int) volatile;
    user_type operator++() volatile;
    user_type operator--(int) volatile;
    user_type operator--() volatile;
    user_type operator+=(arithmetic_type) volatile;
    user_type operator-=(arithmetic_type) volatile;
    user_type operator&=(user_type) volatile;
    user_type operator|=(user_type) volatile;
    user_type operator^=(user_type) volatile;

    volatile storage_type value_;
};

template <typename T>
inline void
RWTAtomicImp<T, 8>::store(typename RWTAtomicImp<T, 8>::user_type val,
                          RWAtomicMemoryOrder order) volatile
{
#if defined(_M_IX86)
    swap(val, order);
#else
    // Visual C++ 2005 and higher guarantee that reads from volatile
    // variables have read-acquire semantics and writes to volatile
    // variables have write-release semantics.
    value_ = val;
    switch (order) {
        case rw_mem_order_seq_cst:
        case rw_mem_order_acq_rel:
        case rw_mem_order_acquire:
        case rw_mem_order_consume:
            RW_MEMORY_BARRIER();
            break;
        case rw_mem_order_release:
        case rw_mem_order_relaxed:
            break;
            RW_DISABLE_BULLSEYE
        default:
            RW_ASSERT(0);
            RW_ENABLE_BULLSEYE
    };
#endif
}

template <typename T>
inline bool
RWTAtomicImp<T, 8>::compareAndSwap(typename RWTAtomicImp<T, 8>::user_type& expected,
                                   typename RWTAtomicImp<T, 8>::user_type desired,
                                   RWAtomicMemoryOrder order) volatile
{
    atomic_type res;
    switch (order) {
        case rw_mem_order_seq_cst:
        case rw_mem_order_acq_rel:
            res = RW_INTERLOCKED_COMPARE_EXCHANGE64((volatile atomic_type*)&value_,
                                                    (atomic_cast_type)desired,
                                                    (atomic_cast_type)expected);
            break;
        case rw_mem_order_release:
            res = RW_INTERLOCKED_COMPARE_EXCHANGE64_RELEASE((volatile atomic_type*)&value_,
                    (atomic_cast_type)desired,
                    (atomic_cast_type)expected);
            break;
        case rw_mem_order_acquire:
        case rw_mem_order_consume:
        case rw_mem_order_relaxed:
            res = RW_INTERLOCKED_COMPARE_EXCHANGE64_ACQUIRE((volatile atomic_type*)&value_,
                    (atomic_cast_type)desired,
                    (atomic_cast_type)expected);
            break;
            RW_DISABLE_BULLSEYE
        default:
            RW_ASSERT(0);
            RW_ENABLE_BULLSEYE
    };
    bool ret = (res == (atomic_cast_type)expected);
    expected = (user_type)res;
    return ret;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::load(RWAtomicMemoryOrder order) const volatile
{
#if defined(_M_IX86)
    user_type ret = 0;
    volatile RWTAtomicImp<T, 8>* self = const_cast<volatile RWTAtomicImp<T, 8>* >(this);
    self->compareAndSwap(ret, 0, rw_mem_order_seq_cst);
    return ret;
#else
    // Visual C++ 2005 and higher guarantee that reads from volatile
    // variables have read-acquire semantics and writes to volatile
    // variables have write-release semantics.
    switch (order) {
        case rw_mem_order_seq_cst:
        case rw_mem_order_acq_rel:
        case rw_mem_order_release:
            RW_MEMORY_BARRIER();
            break;
        case rw_mem_order_acquire:
        case rw_mem_order_consume:
        case rw_mem_order_relaxed:
            break;
            RW_DISABLE_BULLSEYE
        default:
            RW_ASSERT(0);
            RW_ENABLE_BULLSEYE
    };
    return value_;
#endif
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::swap(typename RWTAtomicImp<T, 8>::user_type val,
                         RWAtomicMemoryOrder order) volatile
{
    atomic_type res;
    switch (order) {
        case rw_mem_order_seq_cst:
        case rw_mem_order_acq_rel:
            res = RW_INTERLOCKED_EXCHANGE64((volatile atomic_type*)&value_,
                                            (atomic_cast_type)val);
            break;
        case rw_mem_order_release:
            res = RW_INTERLOCKED_EXCHANGE64_RELEASE((volatile atomic_type*)&value_,
                                                    (atomic_cast_type)val);
            break;
        case rw_mem_order_acquire:
        case rw_mem_order_consume:
        case rw_mem_order_relaxed:
            res = RW_INTERLOCKED_EXCHANGE64_ACQUIRE((volatile atomic_type*)&value_,
                                                    (atomic_cast_type)val);
            break;
            RW_DISABLE_BULLSEYE
        default:
            RW_ASSERT(0);
            RW_ENABLE_BULLSEYE
    }
    return (user_type)res;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::fetchAndAdd(typename RWTAtomicImp<T, 8>::arithmetic_type val,
                                RWAtomicMemoryOrder order) volatile
{
    atomic_type res;
    switch (order) {
        case rw_mem_order_seq_cst:
        case rw_mem_order_acq_rel:
            res = RW_INTERLOCKED_EXCHANGE_ADD64((volatile atomic_type*)&value_,
                                                (atomic_cast_type)(val * traits_type::arithmetic_offset));
            break;
        case rw_mem_order_release:
            res = RW_INTERLOCKED_EXCHANGE_ADD64_RELEASE((volatile atomic_type*)&value_,
                    (atomic_cast_type)(val * traits_type::arithmetic_offset));
            break;
        case rw_mem_order_acquire:
        case rw_mem_order_consume:
        case rw_mem_order_relaxed:
            res = RW_INTERLOCKED_EXCHANGE_ADD64_ACQUIRE((volatile atomic_type*)&value_,
                    (atomic_cast_type)(val * traits_type::arithmetic_offset));
            break;
            RW_DISABLE_BULLSEYE
        default:
            RW_ASSERT(0);
            RW_ENABLE_BULLSEYE
    }
    return (user_type)res;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::fetchAndSub(typename RWTAtomicImp<T, 8>::arithmetic_type val,
                                RWAtomicMemoryOrder order) volatile
{
    atomic_type res;
    switch (order) {
        case rw_mem_order_seq_cst:
        case rw_mem_order_acq_rel:
            res = RW_INTERLOCKED_EXCHANGE_ADD64((volatile atomic_type*)&value_,
                                                -(atomic_cast_type)(val * traits_type::arithmetic_offset));
            break;
        case rw_mem_order_release:
            res = RW_INTERLOCKED_EXCHANGE_ADD64_RELEASE((volatile atomic_type*)&value_,
                    -(atomic_cast_type)(val * traits_type::arithmetic_offset));
            break;
        case rw_mem_order_acquire:
        case rw_mem_order_consume:
        case rw_mem_order_relaxed:
            res = RW_INTERLOCKED_EXCHANGE_ADD64_ACQUIRE((volatile atomic_type*)&value_,
                    -(atomic_cast_type)(val * traits_type::arithmetic_offset));
            break;
            RW_DISABLE_BULLSEYE
        default:
            RW_ASSERT(0);
            RW_ENABLE_BULLSEYE
    }
    return (user_type)res;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::fetchAndAnd(typename RWTAtomicImp<T, 8>::user_type val,
                                RWAtomicMemoryOrder order) volatile
{
    atomic_type res;
    switch (order) {
        case rw_mem_order_seq_cst:
        case rw_mem_order_acq_rel:
            res = RW_INTERLOCKED_AND64((volatile atomic_type*)&value_, val);
            break;
        case rw_mem_order_release:
            res = RW_INTERLOCKED_AND64_RELEASE((volatile atomic_type*)&value_, val);
            break;
        case rw_mem_order_acquire:
        case rw_mem_order_consume:
        case rw_mem_order_relaxed:
            res = RW_INTERLOCKED_AND64_ACQUIRE((volatile atomic_type*)&value_, val);
            break;
            RW_DISABLE_BULLSEYE
        default:
            RW_ASSERT(0);
            RW_ENABLE_BULLSEYE
    }
    return (user_type)res;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::fetchAndOr(typename RWTAtomicImp<T, 8>::user_type val,
                               RWAtomicMemoryOrder order) volatile
{
    atomic_type res;
    switch (order) {
        case rw_mem_order_seq_cst:
        case rw_mem_order_acq_rel:
            res = RW_INTERLOCKED_OR64((volatile atomic_type*)&value_, val);
            break;
        case rw_mem_order_release:
            res = RW_INTERLOCKED_OR64_RELEASE((volatile atomic_type*)&value_, val);
            break;
        case rw_mem_order_acquire:
        case rw_mem_order_consume:
        case rw_mem_order_relaxed:
            res = RW_INTERLOCKED_OR64_ACQUIRE((volatile atomic_type*)&value_, val);
            break;
            RW_DISABLE_BULLSEYE
        default:
            RW_ASSERT(0);
            RW_ENABLE_BULLSEYE
    }
    return res;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::fetchAndXor(typename RWTAtomicImp<T, 8>::user_type val,
                                RWAtomicMemoryOrder order) volatile
{
    atomic_type res;
    switch (order) {
        case rw_mem_order_seq_cst:
        case rw_mem_order_acq_rel:
            res = RW_INTERLOCKED_XOR64((volatile atomic_type*)&value_, val);
            break;
        case rw_mem_order_release:
            res = RW_INTERLOCKED_XOR64_RELEASE((volatile atomic_type*)&value_, val);
            break;
        case rw_mem_order_acquire:
        case rw_mem_order_consume:
        case rw_mem_order_relaxed:
            res = RW_INTERLOCKED_XOR64_ACQUIRE((volatile atomic_type*)&value_, val);
            break;
            RW_DISABLE_BULLSEYE
        default:
            RW_ASSERT(0);
            RW_ENABLE_BULLSEYE
    }
    return res;
}


template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::operator++(int) volatile
{
    if (traits_type::arithmetic_offset == 1) {
        return (user_type)RW_INTERLOCKED_INCREMENT64((volatile atomic_type*)&value_) - 1;
    }
    else {
        return fetchAndAdd(1, rw_mem_order_seq_cst);
    }
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::operator++() volatile
{
    if (traits_type::arithmetic_offset == 1) {
        return (user_type)RW_INTERLOCKED_INCREMENT64((volatile atomic_type*)&value_);
    }
    else {
        return (*this) += 1;
    }
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::operator--(int) volatile
{
    if (traits_type::arithmetic_offset == 1) {
        return (user_type)RW_INTERLOCKED_DECREMENT64((volatile atomic_type*)&value_) + 1;
    }
    else {
        return fetchAndSub(1, rw_mem_order_seq_cst);
    }
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::operator--() volatile
{
    if (traits_type::arithmetic_offset == 1) {
        return (user_type)RW_INTERLOCKED_DECREMENT64((volatile atomic_type*)&value_);
    }
    else {
        return (*this) -= 1;
    }
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::operator+=(typename RWTAtomicImp<T, 8>::arithmetic_type val) volatile
{
    return fetchAndAdd(val, rw_mem_order_seq_cst) + val;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::operator-=(typename RWTAtomicImp<T, 8>::arithmetic_type val) volatile
{
    return fetchAndSub(val, rw_mem_order_seq_cst) - val;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::operator&=(typename RWTAtomicImp<T, 8>::user_type val) volatile
{
    return fetchAndAnd(val, rw_mem_order_seq_cst) & val;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::operator|=(typename RWTAtomicImp<T, 8>::user_type val) volatile
{
    return fetchAndOr(val, rw_mem_order_seq_cst) | val;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::operator^=(typename RWTAtomicImp<T, 8>::user_type val) volatile
{
    return fetchAndXor(val, rw_mem_order_seq_cst) ^ val;
}

#pragma warning(pop)

#endif
