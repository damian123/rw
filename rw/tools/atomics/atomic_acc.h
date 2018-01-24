#ifndef rw_tools_atomic_acc_h_
#define rw_tools_atomic_acc_h_
/**********************************************************************
 *
 * $Id: //tools/13/rw/tools/atomics/atomic_acc.h#1 $
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

#include <atomic.h>
#include <stdint.h>

template <typename T, size_t N = sizeof(T)>
struct RWTAtomicImpTraits;

template <typename T>
struct RWTAtomicImpTraits<T, 1> {
    typedef T user_type;
    typedef volatile T storage_type;
    typedef volatile uint8_t atomic_type;
    typedef T arithmetic_type;

    inline static uint8_t atomic_swap(volatile uint8_t* var,
                                      uint8_t newVal) {
        return atomic_swap_8(var, newVal);
    }

    inline static uint8_t atomic_cas(volatile uint8_t* var, uint8_t cmpVal,
                                     uint8_t newVal) {
        return atomic_cas_8(var, cmpVal, newVal);
    }

    inline static uint8_t atomic_inc(volatile uint8_t* var) {
        uint8_t prev = 0;
        uint8_t res = 0;
        while ((res = atomic_cas(var, prev, (uint8_t)(prev + 1))) != prev) {
            prev = res;
        }
        return prev;
    }

    inline static uint8_t atomic_dec(volatile uint8_t* var) {
        uint8_t prev = 0;
        uint8_t res = 0;
        while ((res = atomic_cas(var, prev, (uint8_t)(prev - 1))) != prev) {
            prev = res;
        }
        return prev;
    }
};

template <typename T>
struct RWTAtomicImpTraits<T, 2> {
    typedef T user_type;
    typedef volatile T storage_type;
    typedef volatile uint16_t atomic_type;
    typedef T arithmetic_type;

    inline static uint16_t atomic_swap(volatile uint16_t* var,
                                       uint16_t newVal) {
        return atomic_swap_16(var, newVal);
    }

    inline static uint16_t atomic_cas(volatile uint16_t* var, uint16_t cmpVal,
                                      uint16_t newVal) {
        return atomic_cas_16(var, cmpVal, newVal);
    }

    inline static uint16_t atomic_inc(volatile uint16_t* var) {
        uint16_t prev = 0;
        uint16_t res = 0;
        while ((res = atomic_cas(var, prev, (uint16_t)(prev + 1))) != prev) {
            prev = res;
        }
        return prev;
    }

    inline static uint16_t atomic_dec(volatile uint16_t* var) {
        uint16_t prev = 0;
        uint16_t res = 0;
        while ((res = atomic_cas(var, prev, (uint16_t)(prev - 1))) != prev) {
            prev = res;
        }
        return prev;
    }
};

template <typename T>
struct RWTAtomicImpTraits<T, 4> {
    typedef T user_type;
    typedef volatile T storage_type;
    typedef volatile uint32_t atomic_type;
    typedef T arithmetic_type;

    inline static uint32_t atomic_swap(volatile uint32_t* var,
                                       uint32_t newVal) {
        return atomic_swap_32(var, newVal);
    }

    inline static uint32_t atomic_cas(volatile uint32_t* var, uint32_t cmpVal,
                                      uint32_t newVal) {
        return atomic_cas_32(var, cmpVal, newVal);
    }

    inline static uint32_t atomic_inc(volatile uint32_t* var) {
        return atomic_inc_32(var);
    }

    inline static uint32_t atomic_dec(volatile uint32_t* var) {
        return atomic_dec_32(var);
    }
};

template <typename T>
struct RWTAtomicImpTraits<T, 8> {
    typedef T user_type;
    typedef volatile T storage_type;
    typedef volatile uint64_t atomic_type;
    typedef T arithmetic_type;

    inline static uint64_t atomic_swap(volatile uint64_t* var,
                                       uint64_t newVal) {
        return atomic_swap_64(var, newVal);
    }

    inline static uint64_t atomic_cas(volatile uint64_t* var, uint64_t cmpVal,
                                      uint64_t newVal) {
        return atomic_cas_64(var, cmpVal, newVal);
    }

    inline static uint64_t atomic_inc(volatile uint64_t* var) {
        return atomic_inc_64(var);
    }

    inline static uint64_t atomic_dec(volatile uint64_t* var) {
        return atomic_dec_64(var);
    }
};

template <typename T>
struct RWTAtomicImpTraits<T*, 4> {
    typedef T* user_type;
    typedef T* volatile storage_type;
    typedef volatile uint32_t atomic_type;
    typedef ptrdiff_t arithmetic_type;

    inline static uint32_t atomic_swap(volatile uint32_t* var,
                                       uint32_t newVal) {
        return atomic_swap_32(var, newVal);
    }

    inline static uint32_t atomic_cas(volatile uint32_t* var, uint32_t cmpVal,
                                      uint32_t newVal) {
        return atomic_cas_32(var, cmpVal, newVal);
    }

    inline static uint32_t atomic_inc(volatile uint32_t* var) {
        uint32_t prev = 0;
        uint32_t ret = 0;
        while ((ret = atomic_cas(var, prev, (uint32_t)(prev + sizeof(T)))) != prev) {
            prev = ret;
        }
        return prev;
    }

    inline static uint32_t atomic_dec(volatile uint32_t* var) {
        uint32_t prev = 0;
        uint32_t ret = 0;
        while ((ret = atomic_cas(var, prev, (uint32_t)(prev - sizeof(T)))) != prev) {
            prev = ret;
        }
        return prev;
    }
};

template <typename T>
struct RWTAtomicImpTraits<T*, 8> {
    typedef T* user_type;
    typedef T* volatile storage_type;
    typedef volatile uint64_t atomic_type;
    typedef ptrdiff_t arithmetic_type;

    inline static uint64_t atomic_swap(volatile uint64_t* var,
                                       uint64_t newVal) {
        return atomic_swap_64(var, newVal);
    }

    inline static uint64_t atomic_cas(volatile uint64_t* var, uint64_t cmpVal,
                                      uint64_t newVal) {
        return atomic_cas_64(var, cmpVal, newVal);
    }

    inline static uint64_t atomic_inc(volatile uint64_t* var) {
        uint64_t prev = 0;
        uint64_t ret = 0;
        while ((ret = atomic_cas(var, prev, (prev + sizeof(T)))) != prev) {
            prev = ret;
        }
        return prev;
    }

    inline static uint64_t atomic_dec(volatile uint64_t* var) {
        uint64_t prev = 0;
        uint64_t ret = 0;
        while ((ret = atomic_cas(var, prev, (prev - sizeof(T)))) != prev) {
            prev = ret;
        }
        return prev;
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
    // HP warns that the size of long may be different than the size of the
    // type we're casting to in 64-bit builds. In our case, a different
    // trait will be used, so these casts are safe.
    RW_SUPPRESS_64BIT_TARGET_OF_POINTER_SIZE_WARNING
    RW_SUPPRESS_64BIT_DIFFERENT_SIZE_TYPE_CONVERSION_WARNING
    return (user_type)traits_type::atomic_swap((atomic_type*)&value_, (atomic_type)val);
    RW_RESTORE_64BIT_DIFFERENT_SIZE_TYPE_CONVERSION_WARNING
    RW_RESTORE_64BIT_TARGET_OF_POINTER_SIZE_WARNING
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
    // HP warns that the size of long may be different than the size of the
    // type we're casting to in 64-bit builds. In our case, a different
    // trait will be used, so these casts are safe.
    RW_SUPPRESS_64BIT_TARGET_OF_POINTER_SIZE_WARNING
    RW_SUPPRESS_64BIT_DIFFERENT_SIZE_TYPE_CONVERSION_WARNING
    user_type prev = (user_type)traits_type::atomic_cas((atomic_type*)&value_,
                     (atomic_type)expected,
                     (atomic_type)desired);
    RW_RESTORE_64BIT_DIFFERENT_SIZE_TYPE_CONVERSION_WARNING
    RW_RESTORE_64BIT_TARGET_OF_POINTER_SIZE_WARNING
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
    user_type ret = 0;
    self->compareAndSwap(ret, 0, order);
    return ret;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndAdd(typename RWTAtomicImp<T>::arithmetic_type val,
                             RWAtomicMemoryOrder order) volatile
{
    user_type prev = 0;
    while (!compareAndSwap(prev, (user_type)(prev + val), order)) {}
    return prev;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndSub(typename RWTAtomicImp<T>::arithmetic_type val,
                             RWAtomicMemoryOrder order) volatile
{
    user_type prev = 0;
    while (!compareAndSwap(prev, (user_type)(prev - val), order)) {}
    return prev;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndAnd(typename RWTAtomicImp<T>::user_type val,
                             RWAtomicMemoryOrder order) volatile
{
    user_type prev = 0;
    while (!compareAndSwap(prev, (user_type)(prev & val), order)) {}
    return prev;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndOr(typename RWTAtomicImp<T>::user_type val,
                            RWAtomicMemoryOrder order) volatile
{
    user_type prev = 0;
    while (!compareAndSwap(prev, (user_type)(prev | val), order)) {}
    return prev;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndXor(typename RWTAtomicImp<T>::user_type val,
                             RWAtomicMemoryOrder order) volatile
{
    T prev = 0;
    while (!compareAndSwap(prev, (user_type)(prev ^ val), order)) {}
    return prev;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator++(int) volatile
{
    return (user_type)traits_type::atomic_inc((atomic_type*)&value_);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator++() volatile
{
    // HP warns that the size of long may be different than the size of the
    // type we're casting to in 64-bit builds. In our case, a different
    // trait will be used, so these casts are safe.
    RW_SUPPRESS_64BIT_TARGET_OF_POINTER_SIZE_WARNING
    return (user_type)((user_type)(traits_type::atomic_inc((atomic_type*)&value_)) + 1);
    RW_RESTORE_64BIT_TARGET_OF_POINTER_SIZE_WARNING
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator--(int) volatile
{
    // HP warns that the size of long may be different than the size of the
    // type we're casting to in 64-bit builds. In our case, a different
    // trait will be used, so these casts are safe.
    RW_SUPPRESS_64BIT_TARGET_OF_POINTER_SIZE_WARNING
    RW_SUPPRESS_64BIT_DIFFERENT_SIZE_TYPE_CONVERSION_WARNING
    return (user_type)traits_type::atomic_dec((atomic_type*)&value_);
    RW_RESTORE_64BIT_DIFFERENT_SIZE_TYPE_CONVERSION_WARNING
    RW_RESTORE_64BIT_TARGET_OF_POINTER_SIZE_WARNING
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator--() volatile
{
    // HP warns that the size of long may be different than the size of the
    // type we're casting to in 64-bit builds. In our case, a different
    // trait will be used, so these casts are safe.
    RW_SUPPRESS_64BIT_TARGET_OF_POINTER_SIZE_WARNING
    RW_SUPPRESS_64BIT_DIFFERENT_SIZE_TYPE_CONVERSION_WARNING
    return (user_type)((user_type)(traits_type::atomic_dec((atomic_type*)&value_)) - 1);
    RW_RESTORE_64BIT_DIFFERENT_SIZE_TYPE_CONVERSION_WARNING
    RW_RESTORE_64BIT_TARGET_OF_POINTER_SIZE_WARNING
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator+=(typename RWTAtomicImp<T>::arithmetic_type val) volatile
{
    return (user_type)(fetchAndAdd(val, rw_mem_order_seq_cst) + val);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator-=(typename RWTAtomicImp<T>::arithmetic_type val) volatile
{
    return (user_type)(fetchAndSub(val, rw_mem_order_seq_cst) - val);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator&=(typename RWTAtomicImp<T>::user_type val) volatile
{
    return (user_type)(fetchAndAnd(val, rw_mem_order_seq_cst) & val);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator|=(typename RWTAtomicImp<T>::user_type val) volatile
{
    return (user_type)(fetchAndOr(val, rw_mem_order_seq_cst) | val);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator^=(typename RWTAtomicImp<T>::user_type val) volatile
{
    return (user_type)(fetchAndXor(val, rw_mem_order_seq_cst) ^ val);
}

#endif

