#ifndef rw_tools_atomic_icc_h_
#define rw_tools_atomic_icc_h_
/**********************************************************************
 *
 * $Id: //tools/13/rw/tools/atomics/atomic_icc.h#1 $
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

#include <stdint.h>

template <typename T>
struct RWTAtomicImpTraits {
    typedef T user_type;
    typedef volatile T storage_type;
    typedef volatile T atomic_type;
    typedef T arithmetic_type;

    enum { arithmetic_offset = 1 };
};

template <typename T>
struct RWTAtomicImpTraits<T*> {
    typedef T* user_type;
    typedef T* volatile storage_type;
    typedef volatile uintptr_t atomic_type;
    typedef ptrdiff_t arithmetic_type;

    enum { arithmetic_offset = sizeof(T) };
};

#define RW_ATOMICIMP_INIT(val) { val }

template <typename T, size_t N = sizeof(T)>
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

template <typename T, size_t N>
inline typename RWTAtomicImp<T, N>::user_type
RWTAtomicImp<T, N>::swap(typename RWTAtomicImp<T, N>::user_type val,
                         RWAtomicMemoryOrder) volatile
{
    user_type prev = 0;
    user_type ret = 0;
    while ((ret = __sync_val_compare_and_swap(&value_, prev, val)) != prev) {
        prev = ret;
    }
    return ret;
}

template <typename T, size_t N>
inline void
RWTAtomicImp<T, N>::store(typename RWTAtomicImp<T, N>::user_type val,
                          RWAtomicMemoryOrder order) volatile
{
    swap(val, order);
}

template <typename T, size_t N>
inline typename RWTAtomicImp<T, N>::user_type
RWTAtomicImp<T, N>::load(RWAtomicMemoryOrder) const volatile
{
    volatile RWTAtomicImp<T, N>* self = const_cast<volatile RWTAtomicImp<T, N>*>(this);
    return __sync_val_compare_and_swap(&self->value_, 0, 0);
}

template <typename T, size_t N>
inline bool
RWTAtomicImp<T, N>::compareAndSwap(typename RWTAtomicImp<T, N>::user_type& expected,
                                   typename RWTAtomicImp<T, N>::user_type desired,
                                   RWAtomicMemoryOrder) volatile
{
    user_type prev = __sync_val_compare_and_swap(&value_, expected, desired);
    bool ret = (prev == expected);
    if (!ret) {
        expected = prev;
    }
    return ret;
}

template <typename T, size_t N>
inline typename RWTAtomicImp<T, N>::user_type
RWTAtomicImp<T, N>::fetchAndAdd(typename RWTAtomicImp<T, N>::arithmetic_type val,
                                RWAtomicMemoryOrder) volatile
{
    return __sync_fetch_and_add(&value_, val * traits_type::arithmetic_offset);
}

template <typename T, size_t N>
inline typename RWTAtomicImp<T, N>::user_type
RWTAtomicImp<T, N>::fetchAndSub(typename RWTAtomicImp<T, N>::arithmetic_type val,
                                RWAtomicMemoryOrder) volatile
{
    return __sync_fetch_and_sub(&value_, val * traits_type::arithmetic_offset);
}

template <typename T, size_t N>
inline typename RWTAtomicImp<T, N>::user_type
RWTAtomicImp<T, N>::fetchAndAnd(typename RWTAtomicImp<T, N>::user_type val,
                                RWAtomicMemoryOrder) volatile
{
    return __sync_fetch_and_and(&value_, val);
}

template <typename T, size_t N>
inline typename RWTAtomicImp<T, N>::user_type
RWTAtomicImp<T, N>::fetchAndOr(typename RWTAtomicImp<T, N>::user_type val,
                               RWAtomicMemoryOrder) volatile
{
    return __sync_fetch_and_or(&value_, val);
}

template <typename T, size_t N>
inline typename RWTAtomicImp<T, N>::user_type
RWTAtomicImp<T, N>::fetchAndXor(typename RWTAtomicImp<T, N>::user_type val,
                                RWAtomicMemoryOrder) volatile
{
    return __sync_fetch_and_xor(&value_, val);
}

template <typename T, size_t N>
inline typename RWTAtomicImp<T, N>::user_type
RWTAtomicImp<T, N>::operator++(int) volatile
{
    return fetchAndAdd(1, rw_mem_order_seq_cst);
}

template <typename T, size_t N>
inline typename RWTAtomicImp<T, N>::user_type
RWTAtomicImp<T, N>::operator++() volatile
{
    return __sync_add_and_fetch(&value_, 1 * traits_type::arithmetic_offset);
}

template <typename T, size_t N>
inline typename RWTAtomicImp<T, N>::user_type
RWTAtomicImp<T, N>::operator--(int) volatile
{
    return fetchAndSub(1, rw_mem_order_seq_cst);
}

template <typename T, size_t N>
inline typename RWTAtomicImp<T, N>::user_type
RWTAtomicImp<T, N>::operator--() volatile
{
    return __sync_sub_and_fetch(&value_, 1 * traits_type::arithmetic_offset);
}

template <typename T, size_t N>
inline typename RWTAtomicImp<T, N>::user_type
RWTAtomicImp<T, N>::operator+=(typename RWTAtomicImp<T, N>::arithmetic_type val) volatile
{
    return __sync_add_and_fetch(&value_, val * traits_type::arithmetic_offset);
}

template <typename T, size_t N>
inline typename RWTAtomicImp<T, N>::user_type
RWTAtomicImp<T, N>::operator-=(typename RWTAtomicImp<T, N>::arithmetic_type val) volatile
{
    return __sync_sub_and_fetch(&value_, val * traits_type::arithmetic_offset);
}

template <typename T, size_t N>
inline typename RWTAtomicImp<T, N>::user_type
RWTAtomicImp<T, N>::operator&=(typename RWTAtomicImp<T, N>::user_type val) volatile
{
    return __sync_and_and_fetch(&value_, val);
}

template <typename T, size_t N>
inline typename RWTAtomicImp<T, N>::user_type
RWTAtomicImp<T, N>::operator|=(typename RWTAtomicImp<T, N>::user_type val) volatile
{
    return __sync_or_and_fetch(&value_, val);
}

template <typename T, size_t N>
inline typename RWTAtomicImp<T, N>::user_type
RWTAtomicImp<T, N>::operator^=(typename RWTAtomicImp<T, N>::user_type val) volatile
{
    return __sync_xor_and_fetch(&value_, val);
}

#if defined(__i386__)

template <typename T>
class RWTAtomicImp<T, 8>
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
inline bool
RWTAtomicImp<T, 8>::compareAndSwap(typename RWTAtomicImp<T, 8>::user_type& expected,
                                   typename RWTAtomicImp<T, 8>::user_type desired,
                                   RWAtomicMemoryOrder) volatile
{
    uint32_t oldlo = expected;
    uint32_t oldhi = expected >> 32;
    uint32_t newlo = desired;
    uint32_t newhi = desired >> 32;
    char result = 0;

    __asm__(
        "lock; cmpxchg8b %0; setz %1"
        : "+m"(value_), "=q"(result), "=d"(oldhi), "=a"(oldlo)
        : "m"(value_), "d"(oldhi), "a"(oldlo), "c"(newhi), "b"(newlo)
        : "memory"
    );

    if (!result) {
        uint64_t ret = oldhi;
        ret <<= 32;
        ret |= oldlo;
        expected = ret;
    }

    return result;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::swap(typename RWTAtomicImp<T, 8>::user_type val,
                         RWAtomicMemoryOrder order) volatile
{
    user_type prev = 0;
    while (!compareAndSwap(prev, val, order)) {}
    return prev;
}

template <typename T>
inline void
RWTAtomicImp<T, 8>::store(typename RWTAtomicImp<T, 8>::user_type val,
                          RWAtomicMemoryOrder order) volatile
{
    swap(val, order);
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::load(RWAtomicMemoryOrder order) const volatile
{
    volatile RWTAtomicImp<T, 8>* self = const_cast<volatile RWTAtomicImp<T, 8>*>(this);
    user_type prev = 0;
    self->compareAndSwap(prev, 0, order);
    return prev;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::fetchAndAdd(typename RWTAtomicImp<T, 8>::arithmetic_type val,
                                RWAtomicMemoryOrder order) volatile
{
    user_type prev = 0;
    while (!compareAndSwap(prev, prev + val, order)) {}
    return prev;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::fetchAndSub(typename RWTAtomicImp<T, 8>::arithmetic_type val,
                                RWAtomicMemoryOrder order) volatile
{
    user_type prev = 0;
    while (!compareAndSwap(prev, prev - val, order)) {}
    return prev;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::fetchAndAnd(typename RWTAtomicImp<T, 8>::user_type val,
                                RWAtomicMemoryOrder order) volatile
{
    user_type prev = 0;
    while (!compareAndSwap(prev, prev & val, order)) {}
    return prev;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::fetchAndOr(typename RWTAtomicImp<T, 8>::user_type val,
                               RWAtomicMemoryOrder order) volatile
{
    user_type prev = 0;
    while (!compareAndSwap(prev, prev | val, order)) {}
    return prev;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::fetchAndXor(typename RWTAtomicImp<T, 8>::user_type val,
                                RWAtomicMemoryOrder order) volatile
{
    user_type prev = 0;
    while (!compareAndSwap(prev, prev ^ val, order)) {}
    return prev;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::operator++(int) volatile
{
    return fetchAndAdd(1, rw_mem_order_seq_cst);
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::operator++() volatile
{
    user_type prev = 0;
    user_type ret = prev + 1;
    while (!compareAndSwap(prev, ret, rw_mem_order_seq_cst)) {
        ret = prev + 1;
    }
    return ret;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::operator--(int) volatile
{
    return fetchAndSub(1, rw_mem_order_seq_cst);
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::operator--() volatile
{
    user_type prev = 0;
    user_type ret = prev - 1;
    while (!compareAndSwap(prev, ret, rw_mem_order_seq_cst)) {
        ret = prev - 1;
    }
    return ret;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::operator+=(typename RWTAtomicImp<T, 8>::arithmetic_type val) volatile
{
    user_type prev = 0;
    user_type ret = prev + val;
    while (!compareAndSwap(prev, ret, rw_mem_order_seq_cst)) {
        ret = prev + val;
    }
    return ret;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::operator-=(typename RWTAtomicImp<T, 8>::arithmetic_type val) volatile
{
    user_type prev = 0;
    user_type ret = prev - val;
    while (!compareAndSwap(prev, ret, rw_mem_order_seq_cst)) {
        ret = prev - val;
    }
    return ret;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::operator&=(typename RWTAtomicImp<T, 8>::user_type val) volatile
{
    user_type prev = 0;
    user_type ret = prev & val;
    while (!compareAndSwap(prev, ret, rw_mem_order_seq_cst)) {
        ret = prev & val;
    }
    return ret;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::operator|=(typename RWTAtomicImp<T, 8>::user_type val) volatile
{
    user_type prev = 0;
    user_type ret = prev | val;
    while (!compareAndSwap(prev, ret, rw_mem_order_seq_cst)) {
        ret = prev | val;
    }
    return ret;
}

template <typename T>
inline typename RWTAtomicImp<T, 8>::user_type
RWTAtomicImp<T, 8>::operator^=(typename RWTAtomicImp<T, 8>::user_type val) volatile
{
    user_type prev = 0;
    user_type ret = prev ^ val;
    while (!compareAndSwap(prev, ret, rw_mem_order_seq_cst)) {
        ret = prev ^ val;
    }
    return ret;
}

#endif

#endif
