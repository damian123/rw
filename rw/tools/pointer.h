#ifndef RW_TOOLS_POINTER_H
#define RW_TOOLS_POINTER_H

/***************************************************************************
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
 * $Id: //tools/13/rw/tools/pointer.h#1 $
 *
 ***************************************************************************/

#include <rw/defs.h>
#include <rw/edefs.h>
#include <rw/tools/traits/RWTAddLvalueReference.h>
#include <rw/tools/traits/RWTConditional.h>
#include <rw/tools/traits/RWTIsClass.h>
#include <rw/tools/traits/RWTIsEmpty.h>
#include <rw/tools/traits/RWTIsReference.h>
#include <rw/tools/traits/RWTRemoveReference.h>

template <typename T>
class RWTDefaultDeleter;

template <typename T>
struct RWTHasPointerType {
private:
    typedef struct { } no;
    typedef struct {
        no field_[2];
    } yes;

    template <typename U> static yes check(typename U::pointer* = 0);
    template <typename U> static no check(...);

public:
    static const bool value = (sizeof(check<T>(0)) == sizeof(yes));
};

template <typename T, typename D, bool = RWTHasPointerType<D>::value>
struct RWTScopedPointerPointerType {
    typedef typename D::pointer type;
};

template <typename T, typename D>
struct RWTScopedPointerPointerType<T, D, false> {
    typedef T* type;
};


/**
 * @internal
 * Provides a storage implementation for the pointer and deleter
 * objects.
 */
template <typename T, typename D, bool = RWTIsEmpty<D>::value>
class RWTScopedPointerStorage
{
public:
    RWTScopedPointerStorage() : pointer_() {}

    RWTScopedPointerStorage(T pointer) : pointer_(pointer) {}

#if !defined(RW_NO_RVALUE_REFERENCES)
    RWTScopedPointerStorage(T pointer, D deleter)
        : pointer_(pointer), deleter_(rw_forward<D>(deleter)) {}
#else
    RWTScopedPointerStorage(T pointer, D deleter)
        : pointer_(pointer), deleter_(deleter) {}
#endif

    T& pointer() {
        return RW_EXPOSE(pointer_);
    }
    const T& pointer() const {
        return RW_EXPOSE(pointer_);
    }

    typename RWTRemoveReference<D>::type& deleter() {
        return RW_EXPOSE(deleter_);
    }
    const typename RWTRemoveReference<D>::type& deleter() const {
        return RW_EXPOSE(deleter_);
    }

    void swap(RWTScopedPointerStorage<T, D>& other) {
        rw_swap(pointer(), other.pointer());
        rw_swap(deleter(), other.deleter());
    }

private:
    T pointer_;
    D deleter_;
};

/**
 * @internal
 * Specialization of RWTScopedPointerStorage for situations where
 * the deleter type is empty. Leverages the empty base class
 * optimization.
 */
template <typename T, typename D>
class RWTScopedPointerStorage<T, D, true> : private D
{
public:
    RWTScopedPointerStorage() : pointer_(0) {}

    RWTScopedPointerStorage(T pointer) : pointer_(pointer) {}

    RWTScopedPointerStorage(T pointer, D deleter) : D(deleter), pointer_(pointer) {}

    T& pointer() {
        return RW_EXPOSE(pointer_);
    }
    const T& pointer() const {
        return RW_EXPOSE(pointer_);
    }

    D& deleter() {
        return *this;
    }
    const D& deleter() const {
        return *this;
    }

    void swap(RWTScopedPointerStorage<T, D>& other) {
        rw_swap(pointer(), other.pointer());
    }

private:
    T pointer_;
};

/**
 * @internal
 * @relates RWTScopedPointerStorage
 *
 * Specialization of rw_swap for RWTScopedPointerStorage. This is
 * equivalent to:
 *
 * @code
 * lhs.swap(rhs);
 * @endcode
 */
template <typename T, typename D>
void rw_swap(RWTScopedPointerStorage<T, D>& lhs, RWTScopedPointerStorage<T, D>& rhs)
{
    lhs.swap(rhs);
}

/**
 * @internal
 * @ingroup miscellaneous_classes
 * @brief Single owner smart pointer.
 *
 * RWTScopedPointer maintains sole ownership of a pointer to an object and
 * manages destruction of that object when the RWTScopedPointer goes out of
 * scope, or the RWTScopedPointer is assigned a pointer to a different
 * object.
 *
 * The destruction method can be configured through the \c D template
 * argument. When the RWTScopedPointer is destroyed, the call operator
 * will be invoked on an instance of \c D, passing the underlying pointer
 * as an argument. Unless a custom deletion object is specified, the
 * \c delete expression will be used.
 */
template <typename T, typename D = RWTDefaultDeleter<T> >
class RWTScopedPointer
{
public:
    typedef typename RWTScopedPointerPointerType<T, D>::type pointer;
    typedef T element_type;
    typedef D deleter_type;

    /**
     * Initializes self with null and with a default constructed deleter
     * object.
     */
    RWTScopedPointer() : storage_() {}

    /**
     * Initializes self with \a ptr and with a default constructed deleter
     * object.
     */
    explicit RWTScopedPointer(pointer ptr) : storage_(ptr) {}

    /**
     * Initializes self with \a ptr and with the deleter object \a del.
     */
    RWTScopedPointer(pointer ptr,
                     typename RWTConditional<RWTIsReference<deleter_type>::value, deleter_type, const typename RWTRemoveReference<deleter_type>::type&>::type del)
        : storage_(ptr, del) {}

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Initializes with with \a ptr and with the deleter object \a del.
     */
    RWTScopedPointer(pointer ptr, typename RWTRemoveReference<deleter_type>::type && del)
        : storage_(ptr, rw_move(del)) {}
#endif

    /**
     * Destructor. If the associated pointer is not null, calls associated
     * deleter object, passing in the associated pointer.
     */
    ~RWTScopedPointer() {
        if (get()) {
            getDeleter()(get());
        }
    }

    /**
     * Releases ownership of the associated pointer, if any, and returns it.
     * Calls to #get() return null after this call, until self has been
     * associated with a new pointer.
     */
    pointer release() {
        pointer ret = storage_.pointer();
        storage_.pointer() = 0;
        return ret;
    }

    /**
     * If the associated pointer is not null and is not equal to \a ptr, calls
     * the associated deleter object, passing in the associated pointer.
     * Associates self with the new pointer \a ptr.
     *
     * @note
     * This behavior differs from std::unique_ptr which is not required to
     * guard against self-reset.
     */
    void reset(pointer ptr = pointer()) {
        pointer old = get();
        storage_.pointer() = ptr;
        if (old && old != storage_.pointer()) {
            getDeleter()(old);
        }
    }

    /**
     * Swaps the associated pointer and associated deleter with \a ptr's
     * pointer and deleter.
     */
    void swap(RWTScopedPointer& ptr) {
        rw_swap(storage_, ptr.storage_);
    }

    /**
     * Returns the associated pointer.
     */
    pointer get() const {
        return storage_.pointer();
    }

    /**
     * Returns the associated deleter object.
     */
    typename RWTRemoveReference<deleter_type>::type& getDeleter() {
        return storage_.deleter();
    }

    /**
     * Returns the associated deleter object.
     */
    const typename RWTRemoveReference<deleter_type>::type& getDeleter() const {
        return storage_.deleter();
    }

#if !defined(RW_NO_EXPLICIT_CONVERSION_OPS)
    /**
     * Returns \c true if the associated pointer is not null.
     */
    explicit operator bool() const {
        return (get() != 0);
    }
#else
    typedef void (RWTScopedPointer::*unspecified_bool_type)() const;

    operator unspecified_bool_type() const {
        return (get() != 0) ? &RWTScopedPointer::unspecified_bool : 0;
    }
#endif

    /**
     * Dereferences and returns the underlying pointer.
     */
    typename RWTAddLvalueReference<T>::type operator*() const {
        return *get();
    }

    /**
     * Returns the underlying pointer.
     */
    pointer operator->() const {
        return get();
    }

private:
    RWTScopedPointer(const RWTScopedPointer&);
    RWTScopedPointer& operator=(const RWTScopedPointer&);

#if defined(RW_NO_EXPLICIT_CONVERSION_OPS)
    /**
     * @internal
     * Unused function whose address is used as a sentinel indicating the
     * pointer is non-null.
     */
    void unspecified_bool() const {}
#endif

    RWTScopedPointerStorage<pointer, deleter_type> storage_;
};


/**
 * @internal
 * @ingroup miscellaneous_classes
 * @brief Single owner smart pointer for array types.
 *
 * RWTScopedPointer maintains sole ownership of a pointer to an object and
 * manages destruction of that object when the RWTScopedPointer goes out of
 * scope, or the RWTScopedPointer is assigned a pointer to a different
 * object.
 *
 * The destruction method can be configured through the \c D template
 * argument. When the RWTScopedPointer is destroyed, the call operator
 * will be invoked on an instance of \c D, passing the underlying pointer
 * as an argument. Unless a custom deletion object is specified, the
 * \c delete[] expression will be used.
 *
 * This specialization is specific to pointers to array types, allowing
 * support for indexing into the array.
 */
template <typename T, typename D>
class RWTScopedPointer<T[], D>
{
public:
    typedef T* pointer; // see docs
    typedef T element_type;
    typedef D deleter_type;

    /**
     * Initializes self with null and with a default constructed deleter
     * object.
     */
    RWTScopedPointer() : storage_(0) {}

    /**
     * Initializes self with \a ptr and with a default constructed deleter
     * object.
     */
    explicit RWTScopedPointer(pointer ptr) : storage_(ptr) {}

    /**
     * Initializes self with \a ptr and with the deleter object \a del.
     */
    RWTScopedPointer(pointer ptr,
                     typename RWTConditional<RWTIsReference<deleter_type>::value, deleter_type, const typename RWTRemoveReference<deleter_type>::type&>::type del)
        : storage_(ptr, del) {}

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Initializes with with \a potr and with the deleter object \a del.
     */
    RWTScopedPointer(pointer ptr, typename RWTRemoveReference<deleter_type>::type && del)
        : storage_(ptr, rw_move(del)) {}
#endif

    /**
     * Destructor. If the associated pointer is not null, calls the associated
     * deleter object, passing in the associated pointer.
     */
    ~RWTScopedPointer() {
        if (get()) {
            getDeleter()(get());
        }
    }

    /**
     * Releases ownership of the associated pointer, if any, and returns it.
     * Calls to #get() return null after this call, until self has been
     * associated with a new pointer.
     */
    pointer release() {
        pointer ret = storage_.pointer();
        storage_.pointer() = 0;
        return ret;
    }

    /**
     * If the associated pointer is not null and is not equal to \a ptr, calls
     * the associated deleter object, passing in the associated pointer.
     * Associates self with the new pointer \a ptr.
     *
     * @note
     * This behavior differs from std::unique_ptr which is not required to
     * guard against self-reset.
     */
    void reset(pointer ptr = pointer()) {
        pointer old = storage_.pointer();
        storage_.pointer() = ptr;
        if (old && old != storage_.pointer()) {
            getDeleter()(old);
        }
    }

    /**
     * Swaps the associated pointer and associated deleter with \a ptr's
     * pointer and deleter.
     */
    void swap(RWTScopedPointer& ptr) {
        rw_swap(storage_, ptr.storage_);
    }

    /**
     * Returns the associated pointer.
     */
    pointer get() const {
        return storage_.pointer();
    }

    /**
     * Returns the associated deleter object.
     */
    typename RWTRemoveReference<deleter_type>::type& getDeleter() {
        return storage_.deleter();
    }

    /**
     * Returns the associated deleter object.
     */
    const typename RWTRemoveReference<deleter_type>::type& getDeleter() const {
        return storage_.deleter();
    }

#if !defined(RW_NO_EXPLICIT_CONVERSION_OPS)
    /**
     * Returns \c true if the associated pointer is not null.
     */
    explicit operator bool() const {
        return (get() != 0);
    }
#else
    typedef void (RWTScopedPointer::*unspecified_bool_type)() const;

    operator unspecified_bool_type() const {
        return (get() != 0) ? &RWTScopedPointer::unspecified_bool : 0;
    }
#endif

    /**
     * Returns the object at position \a i in the associated array pointer.
     */
    T& operator[](size_t i) const {
        return get()[i];
    }

private:
    RWTScopedPointer(const RWTScopedPointer&);
    RWTScopedPointer& operator=(const RWTScopedPointer&);

#if defined(RW_NO_EXPLICIT_CONVERSION_OPS)
    /**
     * @internal
     * Unused function whose address is used as a sentinel indicating the
     * pointer is non-null.
     */
    void unspecified_bool() const {}
#endif

    RWTScopedPointerStorage<pointer, deleter_type> storage_;
};

/**
 * @internal
 * @relates RWTScopedPointer
 *
 * The default deleter function object. Calls \c delete on the passed in
 * pointer.
 */
template <typename T>
class RWTDefaultDeleter
{
public:
    void operator()(T* ptr) const {
        delete ptr;
    }
};

/**
 * @internal
 * @relates RWTScopedPointer<T[]>
 *
 * The default deleter function object. Calls \c delete[] on the passed in
 * pointer.
 */
template <typename T>
class RWTDefaultDeleter<T[]>
{
public:
    void operator()(T* ptr) const {
        delete [] ptr;
    }
};

/**
 * @internal
 * @relates RWTScopedPointer
 *
 * Equivalent to calling \c operator== on the associated pointers
 * of \a lhs and \a rhs.
 */
template <typename T1, typename D1, typename T2, typename D2>
inline bool operator==(const RWTScopedPointer<T1, D1>& lhs,
                       const RWTScopedPointer<T2, D2>& rhs)
{
    return lhs.get() == rhs.get();
}

/**
 * @internal
 * @relates RWTScopedPointer
 *
 * Equivalent to calling \c operator!= on the associated pointers
 * of \a lhs and \a rhs.
 */
template <typename T1, typename D1, typename T2, typename D2>
inline bool operator!=(const RWTScopedPointer<T1, D1>& lhs,
                       const RWTScopedPointer<T2, D2>& rhs)
{
    return lhs.get() != rhs.get();
}

/**
 * @internal
 * @relates RWTScopedPointer
 *
 * Equivalent to calling \c operator< on the associated pointers
 * of \a lhs and \a rhs.
 */
template <typename T1, typename D1, typename T2, typename D2>
inline bool operator<(const RWTScopedPointer<T1, D1>& lhs,
                      const RWTScopedPointer<T2, D2>& rhs)
{
    return lhs.get() < rhs.get();
}

/**
 * @internal
 * @relates RWTScopedPointer
 *
 * Equivalent to calling \c operator<= on the associated pointers
 * of \a lhs and \a rhs.
 */
template <typename T1, typename D1, typename T2, typename D2>
inline bool operator<=(const RWTScopedPointer<T1, D1>& lhs,
                       const RWTScopedPointer<T2, D2>& rhs)
{
    return lhs.get() <= rhs.get();
}

/**
 * @internal
 * @relates RWTScopedPointer
 *
 * Equivalent to calling \c operator> on the associated pointers
 * of \a lhs and \a rhs.
 */
template <typename T1, typename D1, typename T2, typename D2>
inline bool operator>(const RWTScopedPointer<T1, D1>& lhs,
                      const RWTScopedPointer<T2, D2>& rhs)
{
    return lhs.get() > rhs.get();
}

/**
 * @internal
 * @relates RWTScopedPointer
 *
 * Equivalent to calling \c operator>= on the associated pointers
 * of \a lhs and \a rhs.
 */
template <typename T1, typename D1, typename T2, typename D2>
inline bool operator>=(const RWTScopedPointer<T1, D1>& lhs,
                       const RWTScopedPointer<T2, D2>& rhs)
{
    return lhs.get() >= rhs.get();
}

/**
 * @internal
 * @relates RWTScopedPointer
 *
 * Specialization of rw_swap for RWTScopedPointer. This is equivalent to:
 *
 * @code
 * lhs.swap(rhs);
 * @endcode
 */
template <typename T, typename D>
void rw_swap(RWTScopedPointer<T, D>& lhs, RWTScopedPointer<T, D>& rhs)
{
    lhs.swap(rhs);
}

#endif

