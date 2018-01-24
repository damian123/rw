#ifndef rw_tools_atomic_sunpro_h_
#define rw_tools_atomic_sunpro_h_
/**********************************************************************
 *
 * $Id: //tools/13/rw/tools/atomics/atomic_sunpro.h#1 $
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
#include <rw/tools/atomicorder.h>

#include <sys/atomic.h>
#include <stdint.h>

template <typename T, size_t N = sizeof(T)>
struct RWTAtomicImpTraits;

template <typename T>
struct RWTAtomicImpTraits<T, 1> {
    typedef T user_type;
    typedef volatile T storage_type;
    typedef volatile uint8_t atomic_type;
    typedef T arithmetic_type;

    inline static uint8_t atomic_cas(volatile uint8_t* target, uint8_t cmp,
                                     uint8_t newval) {
        return atomic_cas_8(target, cmp, newval);
    }

    inline static uint8_t atomic_swap(volatile uint8_t* target,
                                      uint8_t newval) {
        return atomic_swap_8(target, newval);
    }

    inline static uint8_t atomic_add(volatile uint8_t* target, int8_t delta) {
        return atomic_add_8_nv(target, delta);
    }

    inline static uint8_t atomic_inc(volatile uint8_t* target) {
        return atomic_inc_8_nv(target);
    }

    inline static uint8_t atomic_dec(volatile uint8_t* target) {
        return atomic_dec_8_nv(target);
    }

    inline static uint8_t atomic_and(volatile uint8_t* target, uint8_t bits) {
        return atomic_and_8_nv(target, bits);
    }

    inline static uint8_t atomic_or(volatile uint8_t* target, uint8_t bits) {
        return atomic_or_8_nv(target, bits);
    }
};

template <typename T>
struct RWTAtomicImpTraits<T, 2> {
    typedef T user_type;
    typedef volatile T storage_type;
    typedef volatile uint16_t atomic_type;
    typedef T arithmetic_type;

    inline static uint16_t atomic_cas(volatile uint16_t* target, uint16_t cmp,
                                      uint16_t newval) {
        return atomic_cas_16(target, cmp, newval);
    }

    inline static uint16_t atomic_swap(volatile uint16_t* target,
                                       uint16_t newval) {
        return atomic_swap_16(target, newval);
    }

    inline static uint16_t atomic_add(volatile uint16_t* target,
                                      int16_t delta) {
        return atomic_add_16_nv(target, delta);
    }

    inline static uint16_t atomic_inc(volatile uint16_t* target) {
        return atomic_inc_16_nv(target);
    }

    inline static uint16_t atomic_dec(volatile uint16_t* target) {
        return atomic_dec_16_nv(target);
    }

    inline static uint16_t atomic_and(volatile uint16_t* target,
                                      uint16_t bits) {
        return atomic_and_16_nv(target, bits);
    }

    inline static uint16_t atomic_or(volatile uint16_t* target,
                                     uint16_t bits) {
        return atomic_or_16_nv(target, bits);
    }
};

template <typename T>
struct RWTAtomicImpTraits<T, 4> {
    typedef T user_type;
    typedef volatile T storage_type;
    typedef volatile uint32_t atomic_type;
    typedef T arithmetic_type;

    inline static uint32_t atomic_cas(volatile uint32_t* target, uint32_t cmp,
                                      uint32_t newval) {
        return atomic_cas_32(target, cmp, newval);
    }

    inline static uint32_t atomic_swap(volatile uint32_t* target,
                                       uint32_t newval) {
        return atomic_swap_32(target, newval);
    }

    inline static uint32_t atomic_add(volatile uint32_t* target,
                                      int32_t delta) {
        return atomic_add_32_nv(target, delta);
    }

    inline static uint32_t atomic_inc(volatile uint32_t* target) {
        return atomic_inc_32_nv(target);
    }

    inline static uint32_t atomic_dec(volatile uint32_t* target) {
        return atomic_dec_32_nv(target);
    }

    inline static uint32_t atomic_and(volatile uint32_t* target,
                                      uint32_t bits) {
        return atomic_and_32_nv(target, bits);
    }

    inline static uint32_t atomic_or(volatile uint32_t* target,
                                     uint32_t bits) {
        return atomic_or_32_nv(target, bits);
    }
};

template <typename T>
struct RWTAtomicImpTraits<T, 8> {
    typedef T user_type;
    typedef volatile T storage_type;
    typedef volatile uint64_t atomic_type;
    typedef T arithmetic_type;

    inline static uint64_t atomic_cas(volatile uint64_t* target, uint64_t cmp,
                                      uint64_t newval) {
        return atomic_cas_64(target, cmp, newval);
    }

    inline static uint64_t atomic_swap(volatile uint64_t* target,
                                       uint64_t newval) {
        return atomic_swap_64(target, newval);
    }

    inline static uint64_t atomic_add(volatile uint64_t* target,
                                      int64_t delta) {
        return atomic_add_64_nv(target, delta);
    }

    inline static uint64_t atomic_inc(volatile uint64_t* target) {
        return atomic_inc_64_nv(target);
    }

    inline static uint64_t atomic_dec(volatile uint64_t* target) {
        return atomic_dec_64_nv(target);
    }

    inline static uint64_t atomic_and(volatile uint64_t* target,
                                      uint64_t bits) {
        return atomic_and_64_nv(target, bits);
    }

    inline static uint64_t atomic_or(volatile uint64_t* target,
                                     uint64_t bits) {
        return atomic_or_64_nv(target, bits);
    }
};

template <typename T>
struct RWTAtomicImpTraits<T*, 4> {
    typedef T* user_type;
    typedef T* volatile storage_type;
    typedef T* volatile atomic_type;
    typedef ptrdiff_t arithmetic_type;

    inline static void* atomic_cas(volatile void* target, void* cmp,
                                   void* newval) {
        return atomic_cas_ptr(target, cmp, newval);
    }

    inline static void* atomic_swap(volatile void* target, void* newval) {
        return atomic_swap_ptr(target, newval);
    }

    inline static void* atomic_add(volatile void* target, ssize_t delta) {
        return atomic_add_ptr_nv(target, delta * sizeof(T));
    }

    inline static void* atomic_inc(volatile void* target) {
        return atomic_add(target, 1);
    }

    inline static void* atomic_dec(volatile void* target) {
        return atomic_add(target, -1);
    }
};

template <typename T>
struct RWTAtomicImpTraits<T*, 8> {
    typedef T* user_type;
    typedef T* volatile storage_type;
    typedef T* volatile atomic_type;
    typedef ptrdiff_t arithmetic_type;

    inline static void* atomic_cas(volatile void* target, void* cmp,
                                   void* newval) {
        return atomic_cas_ptr(target, cmp, newval);
    }

    inline static void* atomic_swap(volatile void* target, void* newval) {
        return atomic_swap_ptr(target, newval);
    }

    inline static void* atomic_add(volatile void* target, ssize_t delta) {
        return atomic_add_ptr_nv(target, delta * sizeof(T));
    }

    inline static void* atomic_inc(volatile void* target) {
        return atomic_add(target, 1);
    }

    inline static void* atomic_dec(volatile void* target) {
        return atomic_add(target, -1);
    }
};

#define RW_ATOMICIMP_INIT(val) { val }

template <typename T>
class RWTAtomicImp
{
public:
    typedef RWTAtomicImpTraits<T> traits_type;
    typedef typename traits_type::user_type user_type;
    typedef typename traits_type::storage_type storage_type;
    typedef typename traits_type::atomic_type atomic_type;
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

    storage_type value_;
};

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::swap(typename RWTAtomicImp<T>::user_type val,
                      RWAtomicMemoryOrder) volatile
{
    return (user_type)traits_type::atomic_swap((atomic_type*)&value_, val);
}

template <typename T>
inline void
RWTAtomicImp<T>::store(typename RWTAtomicImp<T>::user_type val,
                       RWAtomicMemoryOrder order) volatile
{
    swap(val, order);
}

template <typename T>
inline bool
RWTAtomicImp<T>::compareAndSwap(typename RWTAtomicImp<T>::user_type& expected,
                                typename RWTAtomicImp<T>::user_type desired,
                                RWAtomicMemoryOrder) volatile
{
    user_type prev = (user_type)traits_type::atomic_cas((atomic_type*)&value_,
                     expected, desired);
    bool ret = (prev == expected);
    if (!ret) {
        expected = prev;
    }
    return ret;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::load(RWAtomicMemoryOrder order) const volatile
{
    volatile RWTAtomicImp<T>* self = const_cast<volatile RWTAtomicImp<T>*>(this);
    user_type expected = 0;
    self->compareAndSwap(expected, 0, order);
    return expected;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndAdd(typename RWTAtomicImp<T>::arithmetic_type val,
                             RWAtomicMemoryOrder) volatile
{
    return (user_type)traits_type::atomic_add((atomic_type*)&value_, val) - val;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndSub(typename RWTAtomicImp<T>::arithmetic_type val,
                             RWAtomicMemoryOrder order) volatile
{
    return fetchAndAdd(-val, order);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndAnd(typename RWTAtomicImp<T>::user_type val,
                             RWAtomicMemoryOrder order) volatile
{
    user_type prev = 0;
    while (!compareAndSwap(prev, prev & val, order)) {}
    return prev;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndOr(typename RWTAtomicImp<T>::user_type val,
                            RWAtomicMemoryOrder order) volatile
{
    user_type prev = 0;
    while (!compareAndSwap(prev, prev | val, order)) {}
    return prev;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndXor(typename RWTAtomicImp<T>::user_type val,
                             RWAtomicMemoryOrder order) volatile
{
    user_type prev = 0;
    while (!compareAndSwap(prev, prev ^ val, order)) {}
    return prev;
}


template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator++(int) volatile
{
    return fetchAndAdd(1, rw_mem_order_seq_cst);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator++() volatile
{
    return (user_type)traits_type::atomic_inc((atomic_type*)&value_);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator--(int) volatile
{
    return fetchAndSub(1, rw_mem_order_seq_cst);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator--() volatile
{
    return (user_type)traits_type::atomic_dec((atomic_type*)&value_);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator+=(typename RWTAtomicImp<T>::arithmetic_type val) volatile
{
    return (user_type)traits_type::atomic_add((atomic_type*)&value_, val);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator-=(typename RWTAtomicImp<T>::arithmetic_type val) volatile
{
    return (user_type)traits_type::atomic_add((atomic_type*)&value_, -val);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator&=(typename RWTAtomicImp<T>::user_type val) volatile
{
    return traits_type::atomic_and((atomic_type*)&value_, val);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator|=(typename RWTAtomicImp<T>::user_type val) volatile
{
    return traits_type::atomic_or((atomic_type*)&value_, val);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator^=(typename RWTAtomicImp<T>::user_type val) volatile
{
    return fetchAndXor(val, rw_mem_order_seq_cst) ^ val;
}

#endif

