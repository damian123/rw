#ifndef rw_tools_atomic_xlc_h_
#define rw_tools_atomic_xlc_h_
/**********************************************************************
 *
 * $Id: //tools/13/rw/tools/atomics/atomic_xlc.h#1 $
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

#include <sys/atomic_op.h>

template <typename T, size_t N = sizeof(T)>
struct RWTAtomicImpTraits;

template <typename T>
struct RWTAtomicImpTraits<T, 1> {
    typedef T user_type;
    typedef int storage_type;
    typedef int atomic_type;
    typedef T arithmetic_type;

    inline static boolean_t compare_and_swap(atomic_p addr, int* old_val_addr,
            int new_val) {
        return ::compare_and_swap(addr, old_val_addr, new_val);
    }

    inline static int fetch_and_add(atomic_p addr, int value) {
        return ::fetch_and_add(addr, value);
    }

    inline static uint fetch_and_and(atomic_p addr, uint value) {
        return ::fetch_and_and(addr, value);
    }

    inline static uint fetch_and_or(atomic_p addr, uint value) {
        return ::fetch_and_or(addr, value);
    }
};

template <typename T>
struct RWTAtomicImpTraits<T, 2> {
    typedef T user_type;
    typedef int storage_type;
    typedef int atomic_type;
    typedef T arithmetic_type;

    inline static boolean_t compare_and_swap(atomic_p addr, int* old_val_addr,
            int new_val) {
        return ::compare_and_swap(addr, old_val_addr, new_val);
    }

    inline static int fetch_and_add(atomic_p addr, int value) {
        return ::fetch_and_add(addr, value);
    }

    inline static uint fetch_and_and(atomic_p addr, uint value) {
        return ::fetch_and_and(addr, value);
    }

    inline static uint fetch_and_or(atomic_p addr, uint value) {
        return ::fetch_and_or(addr, value);
    }
};

template <typename T>
struct RWTAtomicImpTraits<T, 4> {
    typedef T user_type;
    typedef int storage_type;
    typedef int atomic_type;
    typedef T arithmetic_type;

    inline static boolean_t compare_and_swap(atomic_p addr, int* old_val_addr,
            int new_val) {
        return ::compare_and_swap(addr, old_val_addr, new_val);
    }

    inline static int fetch_and_add(atomic_p addr, int value) {
        return ::fetch_and_add(addr, value);
    }

    inline static uint fetch_and_and(atomic_p addr, uint value) {
        return ::fetch_and_and(addr, value);
    }

    inline static uint fetch_and_or(atomic_p addr, uint value) {
        return ::fetch_and_or(addr, value);
    }
};

template <typename T>
struct RWTAtomicImpTraits<T, 8> {
    typedef T user_type;
    typedef long storage_type;
    typedef long atomic_type;
    typedef T arithmetic_type;

    inline static boolean_t compare_and_swap(atomic_l addr, long* old_val_addr,
            long new_val) {
        return ::compare_and_swaplp(addr, old_val_addr, new_val);
    }

    inline static long fetch_and_add(atomic_l addr, long value) {
        return ::fetch_and_addlp(addr, value);
    }

    inline static ulong fetch_and_and(atomic_l addr, ulong value) {
        return ::fetch_and_andlp(addr, value);
    }

    inline static ulong fetch_and_or(atomic_l addr, ulong value) {
        return ::fetch_and_orlp(addr, value);
    }
};

template <typename T>
struct RWTAtomicImpTraits<T*, 4> {
    typedef T* user_type;
    typedef T* storage_type;
    typedef int atomic_type;
    typedef ptrdiff_t arithmetic_type;

    inline static boolean_t compare_and_swap(atomic_p addr, int* old_val_addr,
            int new_val) {
        return ::compare_and_swap(addr, old_val_addr, new_val);
    }

    inline static int fetch_and_add(atomic_p addr, int value) {
        return ::fetch_and_add(addr, value * sizeof(T));
    }
};

template <typename T>
struct RWTAtomicImpTraits<T*, 8> {
    typedef T* user_type;
    typedef T* storage_type;
    typedef long atomic_type;
    typedef ptrdiff_t arithmetic_type;

    inline static boolean_t compare_and_swap(atomic_l addr, long* old_val_addr,
            long new_val) {
        return ::compare_and_swaplp(addr, old_val_addr, new_val);
    }

    inline static long fetch_and_add(atomic_l addr, long value) {
        return ::fetch_and_addlp(addr, value * sizeof(T));
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
inline bool
RWTAtomicImp<T>::compareAndSwap(typename RWTAtomicImp<T>::user_type& expected,
                                typename RWTAtomicImp<T>::user_type desired,
                                RWAtomicMemoryOrder) volatile
{
    RWTAtomicImp<T>* self = const_cast<RWTAtomicImp<T>*>(this);
    atomic_type tmp = (atomic_type)expected;
    bool ret = (traits_type::compare_and_swap((atomic_type*)&self->value_,
                &tmp,
                (atomic_type)desired) == TRUE);
    if (!ret) {
        expected = (user_type)tmp;
    }
    return ret;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::swap(typename RWTAtomicImp<T>::user_type val,
                      RWAtomicMemoryOrder order) volatile
{
    RWTAtomicImp<T>* self = const_cast<RWTAtomicImp<T>*>(this);
    user_type prev = 0;
    while (!self->compareAndSwap(prev, val, order)) {}
    return prev;
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
RWTAtomicImp<T>::load(RWAtomicMemoryOrder order) const volatile
{
    RWTAtomicImp<T>* self = const_cast<RWTAtomicImp<T>*>(this);
    user_type ret = 0;
    self->compareAndSwap(ret, 0, order);
    return ret;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndAdd(typename RWTAtomicImp<T>::arithmetic_type val,
                             RWAtomicMemoryOrder) volatile
{
    RWTAtomicImp<T>* self = const_cast<RWTAtomicImp<T>*>(this);
    return (user_type)traits_type::fetch_and_add((atomic_type*)&self->value_, (atomic_type)val);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndSub(typename RWTAtomicImp<T>::arithmetic_type val,
                             RWAtomicMemoryOrder order) volatile
{
    RWTAtomicImp<T>* self = const_cast<RWTAtomicImp<T>*>(this);
    return self->fetchAndAdd(-val, order);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndAnd(typename RWTAtomicImp<T>::user_type val,
                             RWAtomicMemoryOrder) volatile
{
    RWTAtomicImp<T>* self = const_cast<RWTAtomicImp<T>*>(this);
    return (user_type)traits_type::fetch_and_and((atomic_type*)&self->value_, val);
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::fetchAndOr(typename RWTAtomicImp<T>::user_type val,
                            RWAtomicMemoryOrder) volatile
{
    RWTAtomicImp<T>* self = const_cast<RWTAtomicImp<T>*>(this);
    return traits_type::fetch_and_or(&self->value_, val);
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
    return fetchAndAdd(1, rw_mem_order_seq_cst) + 1;
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
    return fetchAndSub(1, rw_mem_order_seq_cst) - 1;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator+=(typename RWTAtomicImp<T>::arithmetic_type val) volatile
{
    return fetchAndAdd(val, rw_mem_order_seq_cst) + val;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator-=(typename RWTAtomicImp<T>::arithmetic_type val) volatile
{
    return fetchAndSub(val, rw_mem_order_seq_cst) - val;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator&=(typename RWTAtomicImp<T>::user_type val) volatile
{
    return fetchAndAnd(val, rw_mem_order_seq_cst) & val;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator|=(typename RWTAtomicImp<T>::user_type val) volatile
{
    return fetchAndOr(val, rw_mem_order_seq_cst) | val;
}

template <typename T>
inline typename RWTAtomicImp<T>::user_type
RWTAtomicImp<T>::operator^=(typename RWTAtomicImp<T>::user_type val) volatile
{
    return fetchAndXor(val, rw_mem_order_seq_cst) ^ val;
}

#endif

