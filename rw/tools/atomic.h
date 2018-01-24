#ifndef rw_tools_atomic_h_
#define rw_tools_atomic_h_
/**********************************************************************
 *
 * $Id: //tools/13/rw/tools/atomic.h#1 $
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

#if defined(DOXYGEN)
#elif defined(_MSC_VER)
#  include <rw/tools/atomics/atomic_msvc.h>
#elif defined(__INTEL_COMPILER) && (__INTEL_COMPILER < 1300)
#  include <rw/tools/atomics/atomic_icc.h>
#elif defined(__GNUG__) && __GNUG__ >= 4
#  include <rw/tools/atomics/atomic_gcc.h>
#elif defined(__SUNPRO_CC)
#  include <rw/tools/atomics/atomic_sunpro.h>
#elif defined(__xlC__)
#  include <rw/tools/atomics/atomic_xlc.h>
#elif defined(__HP_aCC)
#  include <rw/tools/atomics/atomic_acc.h>
#else
#  error "A lock-free implementation of RWTAtomic is not supported on this platform."
#endif

template <typename T>
struct RWAtomicType {
    enum { value = 0 };
};

template <typename T>
struct RWAtomicType<T*> {
    enum { value = 1 };
};

#define RW_ATOMIC_TYPE(type) \
template <>                  \
struct RWAtomicType<type> {  \
    enum { value = 1 };      \
}

RW_ATOMIC_TYPE(char);
RW_ATOMIC_TYPE(unsigned char);
RW_ATOMIC_TYPE(signed char);
RW_ATOMIC_TYPE(short);
RW_ATOMIC_TYPE(unsigned short);
RW_ATOMIC_TYPE(int);
RW_ATOMIC_TYPE(unsigned int);
RW_ATOMIC_TYPE(long);
RW_ATOMIC_TYPE(unsigned long);
RW_ATOMIC_TYPE(wchar_t);
#if !defined(__xlC__) || defined(__64BIT__)
RW_ATOMIC_TYPE(long long);
RW_ATOMIC_TYPE(unsigned long long);
#endif

#undef RW_ATOMIC_TYPE



template <typename T, bool E = RWAtomicType<T>::value>
class RWTAtomic;

/**
 * @relates RWTAtomic
 *
 * Initializes an RWTAtomic<T> to the value \a val.
 */
#define RW_ATOMIC_INIT(val) { RW_ATOMICIMP_INIT(val) }

/**
 * @ingroup atomics
 * @brief Atomically manipulates an integral type
 *
 * RWTAtomic provides atomic manipulation of an underlying integral type
 * \a T. All operations performed on an RWTAtomic instance are atomic.
 *
 * RWTAtomic is limited to the following integral types and their associated
 * typedefs:
 *
 * - char
 * - signed char
 * - unsigned char
 * - short
 * - unsigned short
 * - int
 * - unsigned int
 * - long
 * - unsigned long
 * - long long
 * - unsigned long long
 * - wchar_t
 *
 * @section example Example
 *
 * @code
 * RWTAtomic<int> a = RW_ATOMIC_INIT(0);
 * int prev = a.swap(5);
 * @endcode
 *
 * @note
 * While copy construction and assignment are not prohibited, these
 * operations are not performed in an atomic manner, and may result in
 * undefined behavior. These operations may be prohibited in a future
 * release and should be avoided.
 *
 * @note
 * 64-bit integral types are not supported in 32-bit builds on AIX.
 */
template <typename T>
#if defined(DOXYGEN)
class RWTAtomic
#else
class RWTAtomic <T, true>
#endif
{
public:
    /**
     * Compares \a expected to self. If \a expected is equal to self,
     * \a desired is assigned to self and \c true is returned. If
     * \a expected is not equal to self, self is assigned to \a expected and
     * \c false is returned.
     *
     * This function is equivalent to:
     *
     * @code
     * if (self == expected) {
     *     self = desired;
     *     return true;
     * }
     * else {
     *     expected = self;
     *     return false;
     * }
     * @endcode
     */
    bool compareAndSwap(T& expected, T desired,
                        RWAtomicMemoryOrder order = rw_mem_order_seq_cst) {
        return value_.compareAndSwap(expected, desired, order);
    }

    /**
     * @copydoc compareAndSwap(T&,T,RWAtomicMemoryOrder)
     */
    bool compareAndSwap(T& expected, T desired,
                        RWAtomicMemoryOrder order = rw_mem_order_seq_cst) volatile {
        return value_.compareAndSwap(expected, desired, order);
    }

    /**
     * Adds \a val to self. Returns the previous value of self.
     *
     * This function is equivalent to:
     *
     * @code
     * T prev = self;
     * self += val;
     * return prev;
     * @endcode
     */
    T fetchAndAdd(T val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) {
        return value_.fetchAndAdd(val, order);
    }

    /**
     * @copydoc fetchAndAdd(T,RWAtomicMemoryOrder)
     */
    T fetchAndAdd(T val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) volatile {
        return value_.fetchAndAdd(val, order);
    }

    /**
     * Subtracts \a val from self. Returns the previous value of self.
     *
     * This function is equivalent to:
     *
     * @code
     * T prev = self;
     * self -= val;
     * return prev;
     * @endcode
     */
    T fetchAndSub(T val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) {
        return value_.fetchAndSub(val, order);
    }

    /**
     * @copydoc fetchAndSub(T,RWAtomicMemoryOrder)
     */
    T fetchAndSub(T val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) volatile {
        return value_.fetchAndSub(val, order);
    }

    /**
     * Performs a bitwise AND between \a val and self, storing the result in self.
     * Returns the previous value of self.
     *
     * This function is equivalent to:
     *
     * @code
     * T prev = self;
     * self &= val;
     * return prev;
     * @endcode
     */
    T fetchAndAnd(T val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) {
        return value_.fetchAndAnd(val, order);
    }

    /**
     * @copydoc fetchAndAnd(T,RWAtomicMemoryOrder)
     */
    T fetchAndAnd(T val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) volatile {
        return value_.fetchAndAnd(val, order);
    }

    /**
     * Performs a bitwise OR between \a val and self, storing the result in self.
     * Returns the previous value of self.
     *
     * This function is equivalent to:
     *
     * @code
     * T prev = self;
     * self |= val;
     * return prev;
     * @endcode
     */
    T fetchAndOr(T val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) {
        return value_.fetchAndOr(val, order);
    }

    /**
     * @copydoc fetchAndOr(T,RWAtomicMemoryOrder)
     */
    T fetchAndOr(T val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) volatile {
        return value_.fetchAndOr(val, order);
    }

    /**
     * Performs a bitwise XOR between \a val and self, storing the result in self.
     * Returns the previous value of self.
     *
     * This function is equivalent to:
     *
     * @code
     * T prev = self;
     * self ^= val;
     * return prev;
     * @endcode
     */
    T fetchAndXor(T val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) {
        return value_.fetchAndXor(val, order);
    }

    /**
     * @copydoc fetchAndXor(T,RWAtomicMemoryOrder)
     */
    T fetchAndXor(T val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) volatile {
        return value_.fetchAndXor(val, order);
    }

    /**
     * Returns the current value of self.
     *
     * @note
     * \a order values of #rw_mem_order_release and
     * #rw_mem_order_acq_rel are not supported and will result in
     * undefined behavior.
     */
    T load(RWAtomicMemoryOrder order = rw_mem_order_seq_cst) const {
        RW_PRECONDITION(order != rw_mem_order_release);
        RW_PRECONDITION(order != rw_mem_order_acq_rel);
        return value_.load(order);
    }

    /**
     * @copydoc load(RWAtomicMemoryOrder)const
     */
    T load(RWAtomicMemoryOrder order = rw_mem_order_seq_cst) const volatile {
        RW_PRECONDITION(order != rw_mem_order_release);
        RW_PRECONDITION(order != rw_mem_order_acq_rel);
        return value_.load(order);
    }

    /**
     * Assigns \a val to self.
     *
     * @note
     * \a order values of #rw_mem_order_consume, #rw_mem_order_acquire
     * and #rw_mem_order_acq_rel are not supported and will result in
     * undefined behavior.
     */
    void store(T val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) {
        RW_PRECONDITION(order != rw_mem_order_consume);
        RW_PRECONDITION(order != rw_mem_order_acquire);
        RW_PRECONDITION(order != rw_mem_order_acq_rel);
        value_.store(val, order);
    }

    /**
     * @copydoc store(T,RWAtomicMemoryOrder)
     */
    void store(T val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) volatile {
        RW_PRECONDITION(order != rw_mem_order_consume);
        RW_PRECONDITION(order != rw_mem_order_acquire);
        RW_PRECONDITION(order != rw_mem_order_acq_rel);
        value_.store(val, order);
    }

    /**
     * Assigns \a val to self. Returns the previous value of self.
     *
     * This function is equivalent to:
     *
     * @code
     * T prev = self;
     * self = val;
     * return prev;
     * @endcode
     */
    T swap(T val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) {
        return value_.swap(val, order);
    }

    /**
     * @copydoc swap(T,RWAtomicMemoryOrder)
     */
    T swap(T val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) volatile {
        return value_.swap(val, order);
    }

#if !defined(RW_NO_VOLATILE_CONVERSION_OVERLOAD)
    /**
     * Returns the current value of self.
     *
     * This function is equivalent to:
     *
     * @code
     * self.load();
     * @endcode
     */
    operator T() const {
        return value_.load(rw_mem_order_seq_cst);
    }
#endif

    /**
     * Returns the current value of self.
     *
     * This function is equivalent to:
     *
     * @code
     * self.load();
     * @endcode
     */
    operator T() const volatile {
        return value_.load(rw_mem_order_seq_cst);
    }

    /**
     * Assigns \a val to self.
     *
     * This function is equivalent to:
     *
     * @code
     * self.store(val);
     * @endcode
     */
    T operator=(T val) {
        value_.store(val, rw_mem_order_seq_cst);
        return val;
    }

    /**
     * @copydoc operator=(T)
     */
    T operator=(T val) volatile {
        value_.store(val, rw_mem_order_seq_cst);
        return val;
    }

    /**
     * Increments self by 1. Returns the previous value of self.
     *
     * @note
     * This function assumes #rw_mem_order_seq_cst semantics.
     */
    T operator++(int) {
        return value_++;
    }

    /**
     * @copydoc operator++(int)
     */
    T operator++(int) volatile {
        return value_++;
    }

    /**
     * Increments self by 1. Returns the new value.
     *
     * @note
     * This function assumes #rw_mem_order_seq_cst semantics.
     */
    T operator++() {
        return ++value_;
    }

    /**
     * @copydoc operator++()
     */
    T operator++() volatile {
        return ++value_;
    }

    /**
     * Decrements self by 1. Returns the previous value of self.
     *
     * @note
     * This function assumes #rw_mem_order_seq_cst semantics.
     */
    T operator--(int) {
        return value_--;
    }

    /**
     * @copydoc operator--(int)
     */
    T operator--(int) volatile {
        return value_--;
    }

    /**
     * Decrements self by 1. Returns the new value.
     *
     * @note
     * This function assumes #rw_mem_order_seq_cst semantics.
     */
    T operator--() {
        return --value_;
    }

    /**
     * @copydoc operator--()
     */
    T operator--() volatile {
        return --value_;
    }

    /**
     * Adds \a val to self. Returns the new value.
     *
     * @note
     * This function assumes #rw_mem_order_seq_cst semantics.
     */
    T operator+=(T val) {
        return value_ += val;
    }

    /**
     * @copydoc operator+=(T)
     */
    T operator+=(T val) volatile {
        return value_ += val;
    }

    /**
     * Subtracts \a val from self. Returns the new value.
     *
     * @note
     * This function assumes #rw_mem_order_seq_cst semantics.
     */
    T operator-=(T val) {
        return value_ -= val;
    }

    /**
     * @copydoc operator-=(T)
     */
    T operator-=(T val) volatile {
        return value_ -= val;
    }

    /**
     * Performs a bitwise AND between \a val and self, storing the result in
     * self. Returns the new value.
     *
     * @note
     * This function assumes #rw_mem_order_seq_cst semantics.
     */
    T operator&=(T val) {
        return value_ &= val;
    }

    /**
     * @copydoc operator&=(T)
     */
    T operator&=(T val) volatile {
        return value_ &= val;
    }

    /**
     * Performs a bitwise OR between \a val and self, storing the result in
     * self. Returns the new value.
     *
     * @note
     * This function assumes #rw_mem_order_seq_cst semantics.
     */
    T operator|=(T val) {
        return value_ |= val;
    }

    /**
     * @copydoc operator|=(T)
     */
    T operator|=(T val) volatile {
        return value_ |= val;
    }

    /**
     * Performs a bitwise XOR between \a val and self, storing the result in
     * self. Returns the new value.
     *
     * @note
     * This function assumes #rw_mem_order_seq_cst semantics.
     */
    T operator^=(T val) {
        return value_ ^= val;
    }

    /**
     * @copydoc operator^=(T)
     */
    T operator^=(T val) volatile {
        return value_ ^= val;
    }

    RWTAtomicImp<T> value_;
};

/**
 * @ingroup atomics
 * @brief Atomically manipulates a pointer type
 *
 * RWTAtomic provides atomic manipulation of an underlying pointer type
 * \a T. All operations performed on an RWTAtomic instance are atomic.
 *
 * @section example Example
 *
 * @code
 * RWTAtomic<char*> a = RW_ATOMIC_INIT(0);
 * char* prev = a.swap(new char('a'));
 * @endcode
 *
 * @note
 * While copy construction and assignment are not prohibited, these
 * operations are not performed in an atomic manner, and may result in
 * undefined behavior. These operations may be prohibited in a future
 * release and should be avoided.
 */
template <typename T>
#if defined(DOXYGEN)
class RWTAtomic<T*>
#else
class RWTAtomic <T*, true>
#endif
{
public:
    /**
     * Compares \a expected to self. If \a expected is equal to self,
     * \a desired is assigned to self and \c true is returned. If
     * \a expected is not equal to self, self is assigned to \a expected and
     * \c false is returned.
     *
     * This function is equivalent to:
     *
     * @code
     * if (self == expected) {
     *     self = desired;
     *     return true;
     * }
     * else {
     *     expected = self;
     *     return false;
     * }
     * @endcode
     */
    bool compareAndSwap(T*& expected, T* desired,
                        RWAtomicMemoryOrder order = rw_mem_order_seq_cst) {
        return value_.compareAndSwap(expected, desired, order);
    }

    /**
     * @copydoc compareAndSwap(T*&,T*,RWAtomicMemoryOrder)
     */
    bool compareAndSwap(T*& expected, T* desired,
                        RWAtomicMemoryOrder order = rw_mem_order_seq_cst) volatile {
        return value_.compareAndSwap(expected, desired, order);
    }

    /**
     * Adds \a val to self. Returns the previous value of self.
     *
     * This function is equivalent to:
     *
     * @code
     * T prev = self;
     * self += val;
     * return prev;
     * @endcode
     */
    T* fetchAndAdd(ptrdiff_t val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) {
        return value_.fetchAndAdd(val, order);
    }

    /**
     * @copydoc fetchAndAdd(ptrdiff_t,RWAtomicMemoryOrder)
     */
    T* fetchAndAdd(ptrdiff_t val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) volatile {
        return value_.fetchAndAdd(val, order);
    }

    /**
     * Subtracts \a val from self. Returns the previous value of self.
     *
     * This function is equivalent to:
     *
     * @code
     * T prev = self;
     * self -= val;
     * return prev;
     * @endcode
     */
    T* fetchAndSub(ptrdiff_t val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) {
        return value_.fetchAndSub(val, order);
    }

    /**
     * @copydoc fetchAndSub(ptrdiff_t,RWAtomicMemoryOrder)
     */
    T* fetchAndSub(ptrdiff_t val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) volatile {
        return value_.fetchAndSub(val, order);
    }

    /**
     * Returns the current value of self.
     *
     * @note
     * \a order values of #rw_mem_order_release and
     * #rw_mem_order_acq_rel are not supported and will result in
     * undefined behavior.
     */
    T* load(RWAtomicMemoryOrder order = rw_mem_order_seq_cst) const {
        RW_PRECONDITION(order != rw_mem_order_release);
        RW_PRECONDITION(order != rw_mem_order_acq_rel);
        return value_.load(order);
    }

    /**
     * @copydoc load(RWAtomicMemoryOrder)const
     */
    T* load(RWAtomicMemoryOrder order = rw_mem_order_seq_cst) const volatile {
        RW_PRECONDITION(order != rw_mem_order_release);
        RW_PRECONDITION(order != rw_mem_order_acq_rel);
        return value_.load(order);
    }

    /**
     * Assigns \a val to self.
     *
     * @note
     * \a order values of #rw_mem_order_consume, #rw_mem_order_acquire
     * and #rw_mem_order_acq_rel are not supported and will result in
     * undefined behavior.
     */
    void store(T* val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) {
        RW_PRECONDITION(order != rw_mem_order_consume);
        RW_PRECONDITION(order != rw_mem_order_acquire);
        RW_PRECONDITION(order != rw_mem_order_acq_rel);
        value_.store(val, order);
    }

    /**
     * @copydoc store(T*,RWAtomicMemoryOrder)
     */
    void store(T* val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) volatile {
        RW_PRECONDITION(order != rw_mem_order_consume);
        RW_PRECONDITION(order != rw_mem_order_acquire);
        RW_PRECONDITION(order != rw_mem_order_acq_rel);
        value_.store(val, order);
    }

    /**
     * Assigns \a val to self. Returns the previous value of self.
     *
     * This function is equivalent to:
     *
     * @code
     * T prev = self;
     * self = val;
     * return prev;
     * @endcode
     */
    T* swap(T* val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) {
        return value_.swap(val, order);
    }

    /**
     * @copydoc swap(T*,RWAtomicMemoryOrder)
     */
    T* swap(T* val, RWAtomicMemoryOrder order = rw_mem_order_seq_cst) volatile {
        return value_.swap(val, order);
    }

#if !defined(RW_NO_VOLATILE_CONVERSION_OVERLOAD)
    /**
     * Returns the current value of self.
     *
     * This function is equivalent to:
     *
     * @code
     * self.load();
     * @endcode
     */
    operator T* () const {
        return value_.load(rw_mem_order_seq_cst);
    }
#endif

    /**
     * Returns the current value of self.
     *
     * This function is equivalent to:
     *
     * @code
     * self.load();
     * @endcode
     */
    operator T* () const volatile {
        return value_.load(rw_mem_order_seq_cst);
    }

    /**
     * Assigns \a val to self.
     *
     * This function is equivalent to:
     *
     * @code
     * self.store(val);
     * @endcode
     */
    T* operator=(T* val) {
        value_.store(val, rw_mem_order_seq_cst);
        return val;
    }

    /**
     * @copydoc operator=(T*)
     */
    T* operator=(T* val) volatile {
        value_.store(val, rw_mem_order_seq_cst);
        return val;
    }

    /**
     * Increments self by 1. Returns the previous value of self.
     *
     * @note
     * This function assumes #rw_mem_order_seq_cst semantics.
     */
    T* operator++(int) {
        return value_++;
    }

    /**
     * @copydoc operator++(int)
     */
    T* operator++(int) volatile {
        return value_++;
    }

    /**
     * Increments self by 1. Returns the new value.
     *
     * @note
     * This function assumes #rw_mem_order_seq_cst semantics.
     */
    T* operator++() {
        return ++value_;
    }

    /**
     * @copydoc operator++()
     */
    T* operator++() volatile {
        return ++value_;
    }

    /**
     * Decrements self by 1. Returns the previous value of self.
     *
     * @note
     * This function assumes #rw_mem_order_seq_cst semantics.
     */
    T* operator--(int) {
        return value_--;
    }

    /**
     * @copydoc operator--(int)
     */
    T* operator--(int) volatile {
        return value_--;
    }

    /**
     * Decrements self by 1. Returns the new value.
     *
     * @note
     * This function assumes #rw_mem_order_seq_cst semantics.
     */
    T* operator--() {
        return --value_;
    }

    /**
     * @copydoc operator--()
     */
    T* operator--() volatile {
        return --value_;
    }

    /**
     * Adds \a val to self. Returns the new value.
     *
     * @note
     * This function assumes #rw_mem_order_seq_cst semantics.
     */
    T* operator+=(ptrdiff_t val) {
        return (value_ += val);
    }

    /**
     * @copydoc operator+=(ptrdiff_t)
     */
    T* operator+=(ptrdiff_t val) volatile {
        return (value_ += val);
    }

    /**
     * Subtracts \a val from self. Returns the new value.
     *
     * @note
     * This function assumes #rw_mem_order_seq_cst semantics.
     */
    T* operator-=(ptrdiff_t val) {
        return (value_ -= val);
    }

    /**
     * @copydoc operator-=(ptrdiff_t)
     */
    T* operator-=(ptrdiff_t val) volatile {
        return (value_ -= val);
    }

    RWTAtomicImp<T*> value_;
};

#endif
