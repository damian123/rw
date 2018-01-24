#ifndef rw_tools_atomic_gcc_h_
#define rw_tools_atomic_gcc_h_
/**********************************************************************
 *
 * $Id: //tools/13/rw/tools/atomics/atomic_gcc.h#1 $
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
    user_type prev = 0;
    user_type ret = 0;
    while ((ret = __sync_val_compare_and_swap(&value_, prev, val)) != prev) {
        prev = ret;
    }
    return ret;
}

template <typename T>
inline void
RWTAtomicImp<T>::store(typename RWTAtomicImp<T>::user_type val,
                       RWAtomicMemoryOrder order) volatile
{
    swap(val, order);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::load(RWAtomicMemoryOrder) const volatile
{
    volatile RWTAtomicImp<T>* self = const_cast<volatile RWTAtomicImp<T>*>(this);
    return __sync_val_compare_and_swap(&self->value_, 0, 0);
}

template <typename T>
inline bool
RWTAtomicImp<T>::compareAndSwap(typename RWTAtomicImp<T>::user_type& expected,
                                RWTAtomicImp<T>::user_type desired,
                                RWAtomicMemoryOrder) volatile
{
    user_type prev = __sync_val_compare_and_swap(&value_, expected, desired);
    bool ret = (prev == expected);
    if (!ret) {
        expected = prev;
    }
    return ret;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndAdd(typename RWTAtomicImp<T>::arithmetic_type val,
                             RWAtomicMemoryOrder) volatile
{
    return __sync_fetch_and_add(&value_, (typename traits_type::storage_type)(val * traits_type::arithmetic_offset));
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndSub(typename RWTAtomicImp<T>::arithmetic_type val,
                             RWAtomicMemoryOrder) volatile
{
    return __sync_fetch_and_sub(&value_, (typename traits_type::storage_type)(val * traits_type::arithmetic_offset));
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndAnd(typename RWTAtomicImp<T>::user_type val,
                             RWAtomicMemoryOrder) volatile
{
    return __sync_fetch_and_and(&value_, val);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndOr(typename RWTAtomicImp<T>::user_type val,
                            RWAtomicMemoryOrder) volatile
{
    return __sync_fetch_and_or(&value_, val);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndXor(typename RWTAtomicImp<T>::user_type val,
                             RWAtomicMemoryOrder) volatile
{
    return __sync_fetch_and_xor(&value_, val);
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
    return __sync_add_and_fetch(&value_, (typename traits_type::storage_type)(1 * traits_type::arithmetic_offset));
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
    return __sync_sub_and_fetch(&value_, (typename traits_type::storage_type)(1 * traits_type::arithmetic_offset));
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator+=(typename RWTAtomicImp<T>::arithmetic_type val) volatile
{
    return __sync_add_and_fetch(&value_, (typename traits_type::storage_type)(val * traits_type::arithmetic_offset));
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator-=(typename RWTAtomicImp<T>::arithmetic_type val) volatile
{
    return __sync_sub_and_fetch(&value_, (typename traits_type::storage_type)(val * traits_type::arithmetic_offset));
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator&=(typename RWTAtomicImp<T>::user_type val) volatile
{
    return __sync_and_and_fetch(&value_, val);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator|=(typename RWTAtomicImp<T>::user_type val) volatile
{
    return __sync_or_and_fetch(&value_, val);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator^=(typename RWTAtomicImp<T>::user_type val) volatile
{
    return __sync_xor_and_fetch(&value_, val);
}

#endif
