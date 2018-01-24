#ifndef RW_TOOLS_SCOPE_GUARD_H
#define RW_TOOLS_SCOPE_GUARD_H

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
 * $Id: //tools/13/rw/tools/scopeguard.h#2 $
 *
 ***************************************************************************/

#include <rw/defs.h>
#include <rw/tools/refwrap.h>

/**
 * @ingroup scopeguard
 * @brief Base class for derived RWScopeGuardImp classes.
 *
 * Base class for derived RWScopeGuardImp classes. This class should not be
 * used directly; rather, use the typedef #RWScopeGuard and the
 * rwtMakeScopeGuardG() or rwtMakeScopeGuardM() convenience functions.
 *
 * @see RWScopeGuard
 */
class RWScopeGuardImp
{
public:
    /**
     * Dismisses the action associated with this RWScopeGuardImp instance, so
     * that the action does not execute at the end of the current scope.
     */
    void dismiss() const {
        dismissed_ = true;
    }

protected:
    /**
     * Default constructor
     */
    RWScopeGuardImp()
        : dismissed_(false) {
    }

    /**
     * Copy constructor. Transfers responsibility for invoking the associated
     * action to self, and dismisses \a other.
     */
    RWScopeGuardImp(const RWScopeGuardImp& other)
        : dismissed_(other.dismissed_) {
        other.dismiss();
    }

    /*
     * A flag indicating whether the current scope guard has been dismissed.
     * Used by derived implementations to determine whether the associated
     * action should be executed.
     */
    mutable bool dismissed_;

private:

    /**
     * Assignment operator. Assignment for this type is prohibited.
     */
    RWScopeGuardImp& operator=(const RWScopeGuardImp&);
};

/**
 * @ingroup scopeguard
 *
 * #RWScopeGuard encapsulates a function to invoke and a set of parameters to pass to that
 * function on exit from the current local scope. It can handle functions that take up to
 * five parameters, and those parameters can be of any type.
 *
 * @section example Examples
 *
 * @code
 * FILE* f = fopen("file.txt", "w+");
 *
 * // close the file automatically on exit from scope
 * RWScopeGuard sg = rwtMakeScopeGuardG(fclose, f);
 *
 * // some operation that might throw
 * potentially_dangerous_call();
 * @endcode
 *
 * If the action to be performed by the scope guard is no longer necessary, it
 * may be canceled.
 *
 * @code
 * // prevent the action from being performed
 * sg.dismiss();
 * @endcode
 */
#if !defined(RW_GCC_VERSION) && !defined(RW_CLANG_VERSION)
typedef const RWScopeGuardImp& RWScopeGuard;
#elif RW_GCC_VERSION < 40700
typedef const RWScopeGuardImp& __attribute__((__unused__)) RWScopeGuard;
#else
typedef const RWScopeGuardImp& RWScopeGuard __attribute__((__unused__));
#endif

/**
 * @ingroup scopeguard
 * @brief Derived RWScopeGuardImp for global or static member functions that
 * take 0 parameters.
 *
 * Derived RWScopeGuardImp for global or static member functions that take 0
 * parameters. This class should not be used directly; rather, use the typedef
 * #RWScopeGuard and the rwtMakeScopeGuardG() convenience functions.
 *
 * @see rwtMakeScopeGuardG, #RWScopeGuard
 */
template <class Fun>
class RWScopeGuard0GImp : public RWScopeGuardImp
{
public:
    RWScopeGuard0GImp(Fun func)
        : func_(func) {
    }

    ~RWScopeGuard0GImp() {
        try {
            if (!dismissed_) {
                func_();
            }
        }
        catch (...) {
            // consume all exceptions
        }
    }

private:
    Fun func_;
};

/**
 * @ingroup scopeguard
 * @brief Derived RWScopeGuardImp for global or static member functions that
 * take 1 parameter.
 *
 * Derived RWScopeGuardImp for global or static member functions that take 1
 * parameter. This class should not be used directly; rather, use the typedef
 * #RWScopeGuard and the rwtMakeScopeGuardG() convenience functions.
 *
 * @see rwtMakeScopeGuardG, #RWScopeGuard
 */
template <class Fun, class P1>
class RWScopeGuard1GImp : public RWScopeGuardImp
{
public:
    RWScopeGuard1GImp(Fun func, const P1& p1)
        : func_(func), p1_(p1) {
    }

    ~RWScopeGuard1GImp() {
        try {
            if (!dismissed_) {
                func_(p1_);
            }
        }
        catch (...) {
            // consume all exceptions
        }
    }

private:
    Fun func_;
    const P1 p1_;
};

/**
 * @ingroup scopeguard
 * @brief Derived RWScopeGuardImp for global or static member functions that
 * take 2 parameters.
 *
 * Derived RWScopeGuardImp for global or static member functions that take 2
 * parameters. This class should not be used directly; rather, use the typedef
 * #RWScopeGuard and the rwtMakeScopeGuardG() convenience functions.
 *
 * @see rwtMakeScopeGuardG, #RWScopeGuard
 */
template <class Fun, class P1, class P2>
class RWScopeGuard2GImp : public RWScopeGuardImp
{
public:
    RWScopeGuard2GImp(Fun func, const P1& p1, const P2& p2)
        : func_(func), p1_(p1), p2_(p2) {
    }

    ~RWScopeGuard2GImp() {
        try {
            if (!dismissed_) {
                func_(p1_, p2_);
            }
        }
        catch (...) {
            // consume all exceptions
        }
    }

private:
    Fun func_;
    const P1 p1_;
    const P2 p2_;
};

/**
 * @ingroup scopeguard
 * @brief Derived RWScopeGuardImp for global or static member functions that
 * take 3 parameters.
 *
 * Derived RWScopeGuardImp for global or static member functions that take 3
 * parameters. This class should not be used directly; rather, use the typedef
 * #RWScopeGuard and the rwtMakeScopeGuardG() convenience functions.
 *
 * @see rwtMakeScopeGuardG, #RWScopeGuard
 */
template <class Fun, class P1, class P2, class P3>
class RWScopeGuard3GImp : public RWScopeGuardImp
{
public:
    RWScopeGuard3GImp(Fun func, const P1& p1, const P2& p2, const P3& p3)
        : func_(func), p1_(p1), p2_(p2), p3_(p3) {
    }

    ~RWScopeGuard3GImp() {
        try {
            if (!dismissed_) {
                func_(p1_, p2_, p3_);
            }
        }
        catch (...) {
            // consume all exceptions
        }
    }

private:
    Fun func_;
    const P1 p1_;
    const P2 p2_;
    const P3 p3_;
};

/**
 * @ingroup scopeguard
 * @brief Derived RWScopeGuardImp for global or static member functions that
 * take 4 parameters.
 *
 * Derived RWScopeGuardImp for global or static member functions that take 4
 * parameters. This class should not be used directly; rather, use the typedef
 * #RWScopeGuard and the rwtMakeScopeGuardG() convenience functions.
 *
 * @see rwtMakeScopeGuardG, #RWScopeGuard
 */
template <class Fun, class P1, class P2, class P3, class P4>
class RWScopeGuard4GImp : public RWScopeGuardImp
{
public:
    RWScopeGuard4GImp(Fun func, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
        : func_(func), p1_(p1), p2_(p2), p3_(p3), p4_(p4) {
    }

    ~RWScopeGuard4GImp() {
        try {
            if (!dismissed_) {
                func_(p1_, p2_, p3_, p4_);
            }
        }
        catch (...) {
            // consume all exceptions
        }
    }

private:
    Fun func_;
    const P1 p1_;
    const P2 p2_;
    const P3 p3_;
    const P4 p4_;
};

/**
 * @ingroup scopeguard
 * @brief Derived RWScopeGuardImp for global or static member functions that
 * take 5 parameters.
 *
 * Derived RWScopeGuardImp for global or static member functions that take 5
 * parameters. This class should not be used directly; rather, use the typedef
 * #RWScopeGuard and the rwtMakeScopeGuardG() convenience functions.
 *
 * @see rwtMakeScopeGuardG, #RWScopeGuard
 */
template <class Fun, class P1, class P2, class P3, class P4, class P5>
class RWScopeGuard5GImp : public RWScopeGuardImp
{
public:
    RWScopeGuard5GImp(Fun func, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5)
        : func_(func), p1_(p1), p2_(p2), p3_(p3), p4_(p4), p5_(p5) {
    }

    ~RWScopeGuard5GImp() {
        try {
            if (!dismissed_) {
                func_(p1_, p2_, p3_, p4_, p5_);
            }
        }
        catch (...) {
            // consume all exceptions
        }
    }

private:
    Fun func_;
    const P1 p1_;
    const P2 p2_;
    const P3 p3_;
    const P4 p4_;
    const P5 p5_;
};

RW_SUPPRESS_OUT_OF_BOUNDS_ACCESS_WARNING
/**
 * @ingroup scopeguard
 * @brief Derived RWScopeGuardImp for member functions that take 0 parameters.
 *
 * Derived RWScopeGuardImp for member functions that take 0 parameters. This
 * class should not be used directly; rather, use the typedef #RWScopeGuard
 * and the rwtMakeScopeGuardM() convenience functions.
 *
 * @see rwtMakeScopeGuardM, #RWScopeGuard
 */
template <class Obj, class Fun>
class RWScopeGuard0MImp : public RWScopeGuardImp
{
public:
    RWScopeGuard0MImp(Obj& obj, Fun func)
        : obj_(obj), func_(func) {
    }

    ~RWScopeGuard0MImp() {
        try {
            if (!dismissed_) {
                (obj_.*func_)();
            }
        }
        catch (...) {
            // consume all exceptions
        }
    }

private:
    Obj& obj_;
    Fun func_;
};
RW_RESTORE_OUT_OF_BOUNDS_ACCESS_WARNING


/**
 * @ingroup scopeguard
 * @brief Derived RWScopeGuardImp for member functions that take 1 parameter.
 *
 * Derived RWScopeGuardImp for member functions that take 1 parameter. This
 * class should not be used directly; rather, use the typedef #RWScopeGuard
 * and the rwtMakeScopeGuardM() convenience functions.
 *
 * @see rwtMakeScopeGuardM, #RWScopeGuard
 */
template <class Obj, class Fun, class P1>
class RWScopeGuard1MImp : public RWScopeGuardImp
{
public:
    RWScopeGuard1MImp(Obj& obj, Fun func, const P1& p1)
        : obj_(obj), func_(func), p1_(p1) {
    }

    ~RWScopeGuard1MImp() {
        try {
            if (!dismissed_) {
                (obj_.*func_)(p1_);
            }
        }
        catch (...) {
            // consume all exceptions
        }
    }

private:
    Obj& obj_;
    Fun func_;
    const P1 p1_;
};

/**
 * @ingroup scopeguard
 * @brief Derived RWScopeGuardImp for member functions that take 2 parameters.
 *
 * Derived RWScopeGuardImp for member functions that take 2 parameters. This
 * class should not be used directly; rather, use the typedef #RWScopeGuard
 * and the rwtMakeScopeGuardM() convenience functions.
 *
 * @see rwtMakeScopeGuardM, #RWScopeGuard
 */
template <class Obj, class Fun, class P1, class P2>
class RWScopeGuard2MImp : public RWScopeGuardImp
{
public:
    RWScopeGuard2MImp(Obj& obj, Fun func, const P1& p1, const P2& p2)
        : obj_(obj), func_(func), p1_(p1), p2_(p2) {
    }

    ~RWScopeGuard2MImp() {
        try {
            if (!dismissed_) {
                (obj_.*func_)(p1_, p2_);
            }
        }
        catch (...) {
            // consume all exceptions
        }
    }

private:
    Obj& obj_;
    Fun func_;
    const P1 p1_;
    const P2 p2_;
};

/**
 * @ingroup scopeguard
 * @brief Derived RWScopeGuardImp for member functions that take 3 parameters.
 *
 * Derived RWScopeGuardImp for member functions that take 3 parameters. This
 * class should not be used directly; rather, use the typedef #RWScopeGuard
 * and the rwtMakeScopeGuardM() convenience functions.
 *
 * @see rwtMakeScopeGuardM, #RWScopeGuard
 */
template <class Obj, class Fun, class P1, class P2, class P3>
class RWScopeGuard3MImp : public RWScopeGuardImp
{
public:
    RWScopeGuard3MImp(Obj& obj, Fun func, const P1& p1, const P2& p2, const P3& p3)
        : obj_(obj), func_(func), p1_(p1), p2_(p2), p3_(p3) {
    }

    ~RWScopeGuard3MImp() {
        try {
            if (!dismissed_) {
                (obj_.*func_)(p1_, p2_, p3_);
            }
        }
        catch (...) {
            // consume all exceptions
        }
    }

private:
    Obj& obj_;
    Fun func_;
    const P1 p1_;
    const P2 p2_;
    const P3 p3_;
};

/**
 * @ingroup scopeguard
 * @brief Derived RWScopeGuardImp for member functions that take 4 parameters.
 *
 * Derived RWScopeGuardImp for member functions that take 4 parameters. This
 * class should not be used directly; rather, use the typedef #RWScopeGuard
 * and the rwtMakeScopeGuardM() convenience functions.
 *
 * @see rwtMakeScopeGuardM, #RWScopeGuard
 */
template <class Obj, class Fun, class P1, class P2, class P3, class P4>
class RWScopeGuard4MImp : public RWScopeGuardImp
{
public:
    RWScopeGuard4MImp(Obj& obj, Fun func, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
        : obj_(obj), func_(func), p1_(p1), p2_(p2), p3_(p3), p4_(p4) {
    }

    ~RWScopeGuard4MImp() {
        try {
            if (!dismissed_) {
                (obj_.*func_)(p1_, p2_, p3_, p4_);
            }
        }
        catch (...) {
            // consume all exceptions
        }
    }

private:
    Obj& obj_;
    Fun func_;
    const P1 p1_;
    const P2 p2_;
    const P3 p3_;
    const P4 p4_;
};

/**
 * @ingroup scopeguard
 * @brief Derived RWScopeGuardImp for member functions that take 5 parameters.
 *
 * Derived RWScopeGuardImp for member functions that take 5 parameters. This
 * class should not be used directly; rather, use the typedef #RWScopeGuard
 * and the rwtMakeScopeGuardM() convenience functions.
 *
 * @see rwtMakeScopeGuardM, #RWScopeGuard
 */
template <class Obj, class Fun, class P1, class P2, class P3, class P4, class P5>
class RWScopeGuard5MImp : public RWScopeGuardImp
{
public:
    RWScopeGuard5MImp(Obj& obj, Fun func, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5)
        : obj_(obj), func_(func), p1_(p1), p2_(p2), p3_(p3), p4_(p4), p5_(p5) {
    }

    ~RWScopeGuard5MImp() {
        try {
            if (!dismissed_) {
                (obj_.*func_)(p1_, p2_, p3_, p4_, p5_);
            }
        }
        catch (...) {
            // consume all exceptions
        }
    }

private:
    Obj& obj_;
    Fun func_;
    const P1 p1_;
    const P2 p2_;
    const P3 p3_;
    const P4 p4_;
    const P5 p5_;
};

#if !defined(RW_DISABLE_DEPRECATED)

// For compatibility with SourcePro 12's RWTRefHolder class.
#define RWTRefHolder RWTReferenceWrapper

/**
 * @ingroup scopeguard
 * @deprecated As of SourcePro 12.5, use rwRef() instead.
 *
 * Creates a wrapper around an object reference.
 */
template <class T>
RW_DEPRECATE_FUNC("Use rwRef() instead")
RWTReferenceWrapper<T>
rwtMakeRef(T& t)
{
    return rwRef<T>(t);
}
#endif

/**
 * @ingroup scopeguard_functions
 *
 * Template function to create an #RWScopeGuard set to call <tt>\a func()</tt> automatically
 * when control flow leaves the current scope.
 */
template <class Func>
RWScopeGuard0GImp<Func>
rwtMakeScopeGuardG(Func func)
{
    return RWScopeGuard0GImp<Func>(func);
}

/**
 * @ingroup scopeguard_functions
 *
 * Template function to create an #RWScopeGuard set to call <tt>\a func(a1)</tt> automatically
 * when control flow leaves the current scope.
 */
template <class Func, class A1>
RWScopeGuard1GImp<Func, A1>
rwtMakeScopeGuardG(Func func, A1 a1)
{
    return RWScopeGuard1GImp<Func, A1>(func, a1);
}

/**
 * @ingroup scopeguard_functions
 *
 * Template function to create an #RWScopeGuard set to call <tt>\a func(a1,a2)</tt> automatically
 * when control flow leaves the current scope.
 */
template <class Func, class A1, class A2>
RWScopeGuard2GImp<Func, A1, A2>
rwtMakeScopeGuardG(Func func, A1 a1, A2 a2)
{
    return RWScopeGuard2GImp<Func, A1, A2>(func, a1, a2);
}

/**
 * @ingroup scopeguard_functions
 *
 * Template function to create an #RWScopeGuard set to call <tt>\a func(a1,a2,a3)</tt> automatically
 * when control flow leaves the current scope.
 */
template <class Func, class A1, class A2, class A3>
RWScopeGuard3GImp<Func, A1, A2, A3>
rwtMakeScopeGuardG(Func func, A1 a1, A2 a2, A3 a3)
{
    return RWScopeGuard3GImp<Func, A1, A2, A3>(func, a1, a2, a3);
}

/**
 * @ingroup scopeguard_functions
 *
 * Template function to create an #RWScopeGuard set to call <tt>\a func(a1,a2,a3,a4)</tt> automatically
 * when control flow leaves the current scope.
 */
template <class Func, class A1, class A2, class A3, class A4>
RWScopeGuard4GImp<Func, A1, A2, A3, A4>
rwtMakeScopeGuardG(Func func, A1 a1, A2 a2, A3 a3, A4 a4)
{
    return RWScopeGuard4GImp<Func, A1, A2, A3, A4>(func, a1, a2, a3, a4);
}

/**
 * @ingroup scopeguard_functions
 *
 * Template function to create an #RWScopeGuard set to call <tt>\a func(a1,a2,a3,a4,a5)</tt> automatically
 * when control flow leaves the current scope.
 */
template <class Func, class A1, class A2, class A3, class A4, class A5>
RWScopeGuard5GImp<Func, A1, A2, A3, A4, A5>
rwtMakeScopeGuardG(Func func, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
    return RWScopeGuard5GImp<Func, A1, A2, A3, A4, A5>(func, a1, a2, a3, a4, a5);
}

/**
 * @ingroup scopeguard_functions
 *
 * Template function to create an #RWScopeGuard set to call <tt>\a obj.\a func()</tt> automatically
 * when control flow leaves the current scope.
 */
template <class Func, class Obj>
RWScopeGuard0MImp<Obj, Func>
rwtMakeScopeGuardM(Obj& obj, Func func)
{
    return RWScopeGuard0MImp<Obj, Func>(obj, func);
}

/**
 * @ingroup scopeguard_functions
 *
 * Template function to create an #RWScopeGuard set to call <tt>\a obj.\a func(a1)</tt> automatically
 * when control flow leaves the current scope.
 */
template <class Func, class Obj, class A1>
RWScopeGuard1MImp<Obj, Func, A1>
rwtMakeScopeGuardM(Obj& obj, Func func, A1 a1)
{
    return RWScopeGuard1MImp<Obj, Func, A1>(obj, func, a1);
}

/**
 * @ingroup scopeguard_functions
 *
 * Template function to create an #RWScopeGuard set to call <tt>\a obj.\a func(a1,a2)</tt> automatically
 * when control flow leaves the current scope.
 */
template <class Func, class Obj, class A1, class A2>
RWScopeGuard2MImp<Obj, Func, A1, A2>
rwtMakeScopeGuardM(Obj& obj, Func func, A1 a1, A2 a2)
{
    return RWScopeGuard2MImp<Obj, Func, A1, A2>(obj, func, a1, a2);
}

/**
 * @ingroup scopeguard_functions
 *
 * Template function to create an #RWScopeGuard set to call <tt>\a obj.\a func(a1,a2,a3)</tt> automatically
 * when control flow leaves the current scope.
 */
template <class Func, class Obj, class A1, class A2, class A3>
RWScopeGuard3MImp<Obj, Func, A1, A2, A3>
rwtMakeScopeGuardM(Obj& obj, Func func, A1 a1, A2 a2, A3 a3)
{
    return RWScopeGuard3MImp<Obj, Func, A1, A2, A3>(obj, func, a1, a2, a3);
}

/**
 * @ingroup scopeguard_functions
 *
 * Template function to create an #RWScopeGuard set to call <tt>\a obj.\a func(a1,a2,a3,a4)</tt> automatically
 * when control flow leaves the current scope.
 */
template <class Func, class Obj, class A1, class A2, class A3, class A4>
RWScopeGuard4MImp<Obj, Func, A1, A2, A3, A4>
rwtMakeScopeGuardM(Obj& obj, Func func, A1 a1, A2 a2, A3 a3, A4 a4)
{
    return RWScopeGuard4MImp<Obj, Func, A1, A2, A3, A4>(obj, func, a1, a2, a3, a4);
}

/**
 * @ingroup scopeguard_functions
 *
 * Template function to create an #RWScopeGuard set to call <tt>\a obj.\a func(a1,a2,a3,a4,a5)</tt> automatically
 * when control flow leaves the current scope.
 */
template <class Func, class Obj, class A1, class A2, class A3, class A4, class A5>
RWScopeGuard5MImp<Obj, Func, A1, A2, A3, A4, A5>
rwtMakeScopeGuardM(Obj& obj, Func func, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
    return RWScopeGuard5MImp<Obj, Func, A1, A2, A3, A4, A5>(obj, func, a1, a2, a3, a4, a5);
}

/*
 *
 */
#define rwtMakeScopeGuard rwtMakeScopeGuardG


#endif  // RW_TOOLS_SCOPE_GUARD_H
