#ifndef RW_TOOLS_EDEFS_H
#define RW_TOOLS_EDEFS_H



/**********************************************************************
 *
 * edefs.h - definitions for STL-based containers
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/edefs.h#1 $
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
#include <rw/tools/traits/RWTIdentity.h>
#include <rw/tools/traits/RWTIsSame.h>
#include <rw/tools/traits/RWTRemoveReference.h>

#define RW_DECLARE_PERSIST_IO(CLASS,ISTR,OSTR)                          \
    RWEXPORT_RETURN(OSTR&) operator<<(OSTR& strm, const CLASS& item);   \
    RWEXPORT_RETURN(ISTR&) operator>>(ISTR& strm, CLASS& obj);          \
    RWEXPORT_RETURN(ISTR&) operator>>(ISTR& strm, CLASS*& pObj);

//////////////////////////////////////////////////////////////////////////
//
// Class template stream insertion and stream extraction
// function prototypes WITHOUT ALLOCATORS.
//
//////////////////////////////////////////////////////////////////////////
#define RW_DECLARE_PERSIST_TEMPLATE_IO(T__CLASS, ISTR, OSTR) \
    template <class T>                                       \
    OSTR&  operator<<(OSTR&, const T__CLASS < T >&);         \
                                                             \
    template <class T>                                       \
    ISTR&  operator>>(ISTR&, T__CLASS < T >&);               \
                                                             \
    template <class T>                                       \
    ISTR&  operator>>(ISTR&, T__CLASS < T >*&);


#define RW_DECLARE_PERSIST_TEMPLATE_IO_2(T__CLASS, ISTR, OSTR)  \
    template <class T1, class T2>                               \
    OSTR&  operator<<(OSTR&, const T__CLASS<T1,T2>&);           \
                                                                \
    template <class T1, class T2>                               \
    ISTR&  operator>>(ISTR&, T__CLASS<T1,T2>&);                 \
                                                                \
    template <class T1, class T2>                               \
    ISTR&  operator>>(ISTR&, T__CLASS<T1,T2>*&);


#define RW_DECLARE_PERSIST_TEMPLATE_IO_3(T__CLASS, ISTR, OSTR)  \
    template <class T1, class T2, class T3>                     \
    OSTR&  operator<<(OSTR&, const T__CLASS<T1,T2,T3>&);        \
                                                                \
    template <class T1, class T2, class T3>                     \
    ISTR&  operator>>(ISTR&, T__CLASS<T1,T2,T3>&);              \
                                                                \
    template <class T1, class T2, class T3>                     \
    ISTR&  operator>>(ISTR&, T__CLASS<T1,T2,T3>*&);

#define RW_DECLARE_PERSIST_TEMPLATE_IO_4(T__CLASS, ISTR, OSTR)  \
    template <class T1, class T2, class T3, class T4>           \
    OSTR&  operator<<(OSTR&, const T__CLASS<T1,T2,T3,T4>&);     \
                                                                \
    template <class T1, class T2, class T3, class T4>           \
    ISTR&  operator>>(ISTR&, T__CLASS<T1,T2,T3,T4>&);           \
                                                                \
    template <class T1, class T2, class T3, class T4>           \
    ISTR&  operator>>(ISTR&, T__CLASS<T1,T2,T3,T4>*&);

#define RW_DECLARE_PERSIST_TEMPLATE_IO_5(T__CLASS, ISTR, OSTR)  \
    template <class T1, class T2, class T3, class T4, class T5> \
    OSTR&  operator<<(OSTR&, const T__CLASS<T1,T2,T3,T4,T5>&);  \
                                                                \
    template <class T1, class T2, class T3, class T4, class T5> \
    ISTR&  operator>>(ISTR&, T__CLASS<T1,T2,T3,T4,T5>&);        \
                                                                \
    template <class T1, class T2, class T3, class T4, class T5> \
    ISTR&  operator>>(ISTR&, T__CLASS<T1,T2,T3,T4,T5>*&);


//////////////////////////////////////////////////////////////////////////
//
// Class template stream insertion and stream extraction
// function prototypes WITH ALLOCATORS.
//
//////////////////////////////////////////////////////////////////////////
#define RW_DECLARE_TLS_PERSIST_TEMPLATE_IO(T__CLASS, ISTR, OSTR)    \
    template <class T, class A>                                     \
    OSTR&  operator<<(OSTR&, const T__CLASS<T,A>&);                 \
                                                                    \
    template <class T, class A>                                     \
    ISTR&  operator>>(ISTR&, T__CLASS<T,A>&);                       \
                                                                    \
    template <class T, class A>                                     \
    ISTR&  operator>>(ISTR&, T__CLASS<T,A>*&);

#define RW_DECLARE_TLS_PERSIST_TEMPLATE_IO_2(T__CLASS, ISTR, OSTR)  \
    template <class T1, class T2, class A>                          \
    OSTR&  operator<<(OSTR&, const T__CLASS<T1,T2,A>&);             \
                                                                    \
    template <class T1, class T2, class A>                          \
    ISTR&  operator>>(ISTR&, T__CLASS<T1,T2,A>&);                   \
                                                                    \
    template <class T1, class T2, class A>                          \
    ISTR&  operator>>(ISTR&, T__CLASS<T1,T2,A>*&);

#define RW_DECLARE_TLS_PERSIST_TEMPLATE_IO_3(T__CLASS, ISTR, OSTR)  \
    template <class T1, class T2, class T3, class A>                \
    OSTR&  operator<<(OSTR&, const T__CLASS<T1,T2,T3,A>&);          \
                                                                    \
    template <class T1, class T2, class T3, class A>                \
    ISTR&  operator>>(ISTR&, T__CLASS<T1,T2,T3,A>&);                \
                                                                    \
    template <class T1, class T2, class T3, class A>                \
    ISTR&  operator>>(ISTR&, T__CLASS<T1,T2,T3,A>*&);


#define RW_DECLARE_TLS_PERSIST_TEMPLATE_IO_4(T__CLASS, ISTR, OSTR)  \
    template <class T1, class T2, class T3, class T4, class A>      \
    OSTR&  operator<<(OSTR&, const T__CLASS<T1,T2,T3,T4,A>&);       \
                                                                    \
    template <class T1, class T2, class T3, class T4, class A>      \
    ISTR&  operator>>(ISTR&, T__CLASS<T1,T2,T3,T4,A>&);             \
                                                                    \
    template <class T1, class T2, class T3, class T4, class A>      \
    ISTR&  operator>>(ISTR&, T__CLASS<T1,T2,T3,T4,A>*&);


//////////////////////////////////////////////////////////////////////////
//
// Generate prototypes for streaming operators for containers
// WITHOUT ALLOCATORS.
//
//////////////////////////////////////////////////////////////////////////

/**
 * @ingroup spro_core_classes
 * Declares the functions necessary to provide isomorphic persistence to
 * user-defined classes that implement rwSaveGuts() and rwRestoreGuts().
 */
#define RW_DECLARE_PERSISTABLE(CLASS)                                    \
RW_DECLARE_PERSIST_IO(CLASS,RWvistream,RWvostream)                       \
RW_DECLARE_PERSIST_IO(CLASS,RWFile,RWFile)

/**
 * @ingroup spro_core_classes
 * Declares the functions necessary to provide isomorphic persistence to
 * user-defined class templates that take one template parameter and
 * implement rwSaveGuts() and rwRestoreGuts().
 */
#define RW_DECLARE_PERSISTABLE_TEMPLATE(T__CLASS)                        \
RW_DECLARE_PERSIST_TEMPLATE_IO(T__CLASS,RWvistream,RWvostream)           \
RW_DECLARE_PERSIST_TEMPLATE_IO(T__CLASS,RWFile,RWFile)

/**
 * @ingroup spro_core_classes
 * Declares the functions necessary to provide isomorphic persistence to
 * user-defined class templates that take two template parameters and
 * implement rwSaveGuts() and rwRestoreGuts().
 */
#define RW_DECLARE_PERSISTABLE_TEMPLATE_2(T__CLASS)                      \
RW_DECLARE_PERSIST_TEMPLATE_IO_2(T__CLASS,RWvistream,RWvostream)         \
RW_DECLARE_PERSIST_TEMPLATE_IO_2(T__CLASS,RWFile,RWFile)

/**
 * @ingroup spro_core_classes
 * Declares the functions necessary to provide isomorphic persistence to
 * user-defined class templates that take three template parameters and
 * implement rwSaveGuts() and rwRestoreGuts().
 */
#define RW_DECLARE_PERSISTABLE_TEMPLATE_3(T__CLASS)                      \
RW_DECLARE_PERSIST_TEMPLATE_IO_3(T__CLASS,RWvistream,RWvostream)         \
RW_DECLARE_PERSIST_TEMPLATE_IO_3(T__CLASS,RWFile,RWFile)

/**
 * @ingroup spro_core_classes
 * Declares the functions necessary to provide isomorphic persistence to
 * user-defined class templates that take four template parameters and
 * implement rwSaveGuts() and rwRestoreGuts().
 */
#define RW_DECLARE_PERSISTABLE_TEMPLATE_4(T__CLASS)                      \
RW_DECLARE_PERSIST_TEMPLATE_IO_4(T__CLASS,RWvistream,RWvostream)         \
RW_DECLARE_PERSIST_TEMPLATE_IO_4(T__CLASS,RWFile,RWFile)

/**
 * @ingroup spro_core_classes
 * Declares the functions necessary to provide isomorphic persistence to
 * user-defined class templates that take five template parameters and
 * implement rwSaveGuts() and rwRestoreGuts().
 */
#define RW_DECLARE_PERSISTABLE_TEMPLATE_5(T__CLASS)                      \
RW_DECLARE_PERSIST_TEMPLATE_IO_5(T__CLASS,RWvistream,RWvostream)         \
RW_DECLARE_PERSIST_TEMPLATE_IO_5(T__CLASS,RWFile,RWFile)
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
// Generate prototypes for streaming operators for containers
// WITH ALLOCATORS.
//
//////////////////////////////////////////////////////////////////////////
#define RW_DECLARE_TLS_PERSISTABLE_TEMPLATE(T__CLASS)                    \
RW_DECLARE_TLS_PERSIST_TEMPLATE_IO(T__CLASS,RWvistream,RWvostream)       \
RW_DECLARE_TLS_PERSIST_TEMPLATE_IO(T__CLASS,RWFile,RWFile)

#define RW_DECLARE_TLS_PERSISTABLE_TEMPLATE_2(T__CLASS)                  \
RW_DECLARE_TLS_PERSIST_TEMPLATE_IO_2(T__CLASS,RWvistream,RWvostream)     \
RW_DECLARE_TLS_PERSIST_TEMPLATE_IO_2(T__CLASS,RWFile,RWFile)

#define RW_DECLARE_TLS_PERSISTABLE_TEMPLATE_3(T__CLASS)                  \
RW_DECLARE_TLS_PERSIST_TEMPLATE_IO_3(T__CLASS,RWvistream,RWvostream)     \
RW_DECLARE_TLS_PERSIST_TEMPLATE_IO_3(T__CLASS,RWFile,RWFile)

#define RW_DECLARE_TLS_PERSISTABLE_TEMPLATE_4(T__CLASS)                  \
RW_DECLARE_TLS_PERSIST_TEMPLATE_IO_4(T__CLASS,RWvistream,RWvostream)     \
RW_DECLARE_TLS_PERSIST_TEMPLATE_IO_4(T__CLASS,RWFile,RWFile)
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


/*
 * Here's a useful default hasher that gives the same semantics
 * as the NO_STL hash tables.  That is, it allows you to construct
 * an stl-based hash collection with a function pointer.
 */

template <class T>
struct RWTHasher {
    // should not be necessary, do not use:
    RWTHasher() : hfun_(NULL) { }

    typedef unsigned(*hashfun)(const T&);

    RWTHasher(hashfun f) : hfun_(f) { }

    unsigned operator()(const T& t) const {
        RW_PRECONDITION(hfun_ != NULL);
        return hfun_(t);
    }

private:

    hashfun hfun_;

};


#if !defined(RW_NO_RVALUE_REFERENCES)

template <class T>
inline typename RWTRemoveReference<T>::type && rw_move(T && t)
{
#  if defined(RW_GCC_VERSION) && (RW_GCC_VERSION >= 40300 && RW_GCC_VERSION < 40500)
    return t;
#  else
    return static_cast < typename RWTRemoveReference<T>::type && >(t);
#  endif // !RW_GCC_VERSION
}

template <class T>
inline T && rw_forward(typename RWTIdentity<T>::type& t)
{
#  if defined(RW_GCC_VERSION) && (RW_GCC_VERSION >= 40300 && RW_GCC_VERSION < 40500)
    return t;
#  else
    return static_cast < T && >(t);
#  endif // !RW_GCC_VERSION
}

#endif // !RW_NO_RVALUE_REFERENCES

// vanilla swap, uses move semantics when available
template<typename T>
void rw_swap(T& lhs, T& rhs)
{
#if !defined(RW_NO_RVALUE_REFERENCES)
    T tmp(rw_move(lhs));
    lhs = rw_move(rhs);
    rhs = rw_move(tmp);
#else
    T tmp(lhs);
    lhs = rhs;
    rhs = tmp;
#endif
}

// swap for arrays, uses move semantics when available
template <typename T, size_t N>
void rw_swap(T(&lhs)[N], T(&rhs)[N])
{
    for (size_t i = 0; i < N; ++i) {
        rw_swap(lhs[i], rhs[i]);
    }
}

#endif /* RW_TOOLS_EDEFS_H */
