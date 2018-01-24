#ifndef RW_TOOLS_REFERENCE_WRAPPER_H
#define RW_TOOLS_REFERENCE_WRAPPER_H
/**************************************************************************
 *
 * $Id: //tools/13/rw/tools/refwrap.h#1 $
 *
 * Copyright (c) 1996-2015 Rogue Wave Software, Inc.  All Rights Reserved.
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
 *************************************************************************/

#include <rw/defs.h>

/**
 * @ingroup miscellaneous_classes
 * @brief Stores a reference to an object.
 *
 * RWTReferenceWrapper stores a reference to an object.
 */
template <typename T>
class RWTReferenceWrapper
{
public:
    /**
     * Constructs a reference wrapper for the object referenced by \a ref
     */
    RWTReferenceWrapper(T& ref) : ref_(&ref) {
    }

    /**
     * Constructs a reference wrapper for the object referenced by \a rhs.
     */
    RWTReferenceWrapper(const RWTReferenceWrapper& rhs) : ref_(rhs.ref_) {
    }

    /**
     * Assignment operator. Self will reference the same instance as
     * \a rhs.
     */
    RWTReferenceWrapper& operator=(const RWTReferenceWrapper& rhs) {
        ref_ = rhs.ref_;
        return *this;
    }

    /**
     * Conversion operator.
     */
    operator T& () const {
        return *ref_;
    }

private:

    T* ref_;
};


/**
 * @relates RWTReferenceWrapper
 *
 * Constructs an RWTReferenceWrapper for \a ref.
 */
template <typename T>
RWTReferenceWrapper<T> rwRef(T& ref)
{
    return RWTReferenceWrapper<T>(ref);
}

/**
 * @relates RWTReferenceWrapper
 *
 * Constructs an RWTReferenceWrapper for \a ref.
 */
template <typename T>
RWTReferenceWrapper<const T> rwCRef(const T& ref)
{
    return RWTReferenceWrapper<const T>(ref);
}

/**
 * @relates RWTReferenceWrapper
 * @internal
 *
 * Returns \a ref.
 */
template <typename T>
T& rwUnwrapReference(T& ref)
{
    return ref;
}

/**
 * @relates RWTReferenceWrapper
 * @internal
 *
 * Returns the underlying reference to \c T of \a ref.
 */
template <typename T>
T& rwUnwrapReference(RWTReferenceWrapper<T>& ref)
{
    return ref;
}

template <typename T>
struct RWTUnwrapType {
    typedef T type;
};

template <typename T>
struct RWTUnwrapType<RWTReferenceWrapper<T> > {
    typedef T type;
};

template <typename T>
struct RWTUnwrapType<const RWTReferenceWrapper<T> > {
    typedef T type;
};

template <typename T>
struct RWTUnwrapType<volatile RWTReferenceWrapper<T> > {
    typedef T type;
};

template <typename T>
struct RWTUnwrapType<const volatile RWTReferenceWrapper<T> > {
    typedef T type;
};

#endif
