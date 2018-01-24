#ifndef RW_TOOLS_ANY_H
#define RW_TOOLS_ANY_H

/**********************************************************************
 *
 * $Id: //tools/13/rw/any.h#1 $
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
#include <rw/edefs.h> // for rw_move, rw_swap
#include <rw/tools/pointer.h>

// C Standard Library
#include <assert.h>

// C++ Standard Library
#include <stdexcept>
#include <typeinfo>
#include <algorithm>

/**
 * @ingroup miscellaneous_classes
 *
 * @brief Encapsulates an object of an arbitrary type.
 *
 * RWAny encapsulates an object of an arbitrary type; it can:
 * - store objects of arbitrary types (by value or by pointer)
 * - retrieve the original object in a "type-safe" manner
 * - swap the object with another RWAny
 * - assign objects of type RWAny
 *
 * Requirements for types held by RWAny are:
 *
 * - the type must be complete
 * - the type must be copy constructible
 * - the type must be assignable
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/any.h>
 * RWAny a;
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
struct RWAny {
private:

    struct RWAnyBase {
        virtual ~RWAnyBase() { }

        virtual const std::type_info* type() const = 0;
        virtual RWAnyBase* clone() const = 0;
    };

    template <class T>
    struct RWAnyImp: RWAnyBase {
        RWAnyImp(const RWAnyImp& rhs);

        RWAnyImp& operator= (const RWAnyImp& rhs);

    public:

        RWAnyImp(const T& val) : value_(val) { }

        virtual ~RWAnyImp() { };

        // type and cloning
        virtual const std::type_info* type() const {
            return &typeid(T);
        }

        virtual RWAnyBase* clone() const {
            return new RWAnyImp(value_);
        }

        // value retrieval
        T& value() {
            return value_;
        }

        const T& value() const {
            return value_;
        }

    protected:

        T value_;

    };

public:

    /**
     * Default constructor. Creates an empty object. Post-condition:
     * for any \c T, <tt>(address\<T\>() == 0)</tt>.
     */
    RWAny()
        : imp_(0) {
    }

    /**
     * Template constructor for type \c any holding \c T. Creates RWAny
     * object holding a copy of argument \a ref.
     */
    template <class T>
    RWAny(const T& ref)
        : imp_(new RWAnyImp<T> (ref)) {
    }

    /**
     * Copy constructor. Creates a copy of \a ref.
     */
    RWAny(const RWAny& ref)
        : imp_(ref.imp_ ? ref.imp_->clone() : 0) {
    }

    /**
     * Assignment operator. Deep-copies \a ref. Returns a reference
     * to self.
     */
    RWAny& operator= (const RWAny& ref) {
        if (&ref != this) {
            RWAny(ref).swap(*this);
        }
        return *this;
    }

    /**
     * Template assignment operator for type \c any holding \c T. Assigns self
     * to hold copy of argument \a ref.
     */
    template<typename T>
    RWAny& operator= (const T& ref) {
        RWAny(ref).swap(*this);
        return *this;
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed instance takes ownership of the
     * data owned by \a ref.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWAny(RWAny && ref)
        : imp_(0) {
        ref.swap(*this);
    }

    /**
     * Move assignment. Self takes ownership of the data owned by \a ref.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWAny& operator= (RWAny && ref) {
        if (&ref != this) {
            ref.swap(*this);
        }
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    // Dtor
    ~RWAny() {
    }

    /**
     * Implements a swap operation. After the swap, \c this object
     * contains the old content of the argument and vice versa.
     */
    RWAny& swap(RWAny& ref) {
        rw_swap(imp_, ref.imp_);
        return *this;
    }

    /**
     * Implements type retrieval. Returns <tt>&typeid (T)</tt>
     * if \c any holds an object of type \c T, or \c 0 otherwise.
     */
    const std::type_info* type() const {
        return imp_.get() ? imp_->type() : 0;
    }

    /**
     * Implements value retrieval. Returns <tt>*address\<T\>()</tt>
     * if \c T is the type of the object stored in self, or throws an
     * exception of type (or a type derived from) \b std::bad_cast otherwise.
     */
    template<class T>
    T&  get() {
        // The following explicit qualification
        // is required for MS Visual C++ 7.1.
        T* p = RWAny::address <T> ();

        if (p == 0) {
            throw std::bad_cast();
        }

        return *p;
    }

    /**
     * @copydoc get()
     */
    template<class T>
    const T& get() const {
        return const_cast<RWAny*>(this)->get<T> ();
    }

    /**
     * Implements address retrieval. Returns the address of the object
     * held in \c any or \c 0 if the object is not of the type requested.
     */
    template<class T>
    T* address() {
        RWAny::RWAnyImp<T>* p =
            dynamic_cast<RWAny::RWAnyImp<T>*>(imp_.get());
        return p ? &p->value() : 0;
    }

    /**
     * @copydoc address()
     */
    template<class T>
    const T* address() const {
        return const_cast<RWAny*>(this)->address<T> ();
    }

private:

    RWTScopedPointer<RWAnyBase> imp_;

};

#endif // RW_TOOLS_ANY_H
