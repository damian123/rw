#ifndef RW_TOOLS_EPERSIST_H
#define RW_TOOLS_EPERSIST_H


/**********************************************************************
 *
 * epersist.h - declarations for external template persistence.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/epersist.h#1 $
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


#include <rw/vstream.h>
#include <rw/defs.h>
#include <rw/rwerr.h>
#include <rw/rwfile.h>
#include <rw/toolerr.h>
#include <rw/rwstore.h>

#define RW_DEFINE_PERSIST_IO(CLASS,ISTR,OSTR)                            \
                                                                        \
extern  void rwSaveGuts(OSTR&, const CLASS&);                           \
                                                                        \
RWEXPORT_RETURN(OSTR&) operator<<(OSTR& stream, const CLASS& item)      \
{                                                                       \
  size_t objectNum;                                                     \
                                                                        \
  RWUseStoreTable storeTable;                                           \
                                                                        \
  if(storeTable.add(&item, objectNum)){                                 \
    stream << ':';                                                      \
    rwSaveGuts(stream,item);                                            \
  }                                                                     \
  else                                                                  \
    stream << '@' << objectNum;                                         \
                                                                        \
  return stream;                                                        \
}                                                                       \
                                                                        \
extern void rwRestoreGuts(ISTR&, CLASS&);                               \
                                                                        \
RWEXPORT_RETURN(ISTR&) operator>>(ISTR& stream, CLASS& object)          \
{                                                                       \
  if (!stream.good()) return stream;                                    \
                                                                        \
  size_t         objectNum;                                             \
  char           refFlag;                                               \
  RWUseReadTable readTable;                                             \
                                                                        \
  stream >> refFlag;                                                    \
  if (!stream.good()) return stream;                                    \
                                                                        \
  if(refFlag == '@')                                                    \
  {                                                                     \
    stream >> objectNum;                                                \
    if (!stream.good()) return stream;                                  \
    if (  objectNum >= readTable.entries()                              \
        || (&object != readTable.getPtr(objectNum))                     \
       )                                                                \
      RWTHROW(RWInternalErr(RWMessage(RWTOOL_REF)));                    \
  }                                                                     \
  else if (refFlag == ':') {                                            \
    if (!stream.good()) return stream;                                  \
    readTable.append(&object);                                          \
    rwRestoreGuts(stream, object);                                      \
  }                                                                     \
  else {                                                                \
    /* Neither '@' nor ':' was found.  Bad input stream. */             \
      RWTHROW(RWInternalErr(RWMessage(RWTOOL_REF)));                    \
  }                                                                     \
  return stream;                                                        \
}                                                                       \
                                                                        \
extern  void rwRestoreGuts(ISTR&, CLASS&);                              \
                                                                        \
RWEXPORT_RETURN(ISTR&) operator>>(ISTR& stream, CLASS*& objectPtr)      \
{                                                                       \
  if (!stream.good()) return stream;                                    \
                                                                        \
  size_t         objectNum;                                             \
  char           refFlag;                                               \
  RWUseReadTable readTable;                                             \
                                                                        \
  stream >> refFlag;                                                    \
  if (!stream.good()) return stream;                                    \
                                                                        \
  if(refFlag == '@')                                                    \
  {                                                                     \
    stream >> objectNum;                                                \
    if (!stream.good()) return stream;                                  \
    if ( objectNum >= readTable.entries())                              \
      RWTHROW(RWInternalErr(RWMessage(RWTOOL_REF)));                    \
    else                                                                \
      objectPtr = (CLASS*) readTable.getPtr(objectNum);                 \
  }                                                                     \
  else if (refFlag == ':') {                                            \
    if (!stream.good()) return stream;                                  \
    objectPtr = new CLASS;                                              \
    readTable.append(objectPtr);                                        \
    rwRestoreGuts(stream,*objectPtr);                                   \
  }                                                                     \
  else {                                                                \
    /* Neither '@' nor ':' was found.  Bad input stream. */             \
      RWTHROW(RWInternalErr(RWMessage(RWTOOL_REF)));                    \
  }                                                                     \
  return stream;                                                        \
}

// Hack to help Borland 2.0 for OS/2, by casting away reference
// so Borland will instantiate global template function.
#ifdef RW_NO_TEMPLATE_REF_INSTANTIATED
#  define RW_CAST_TEMPLATE_IO(T__CLASS) (const T__CLASS<T>)
#  define RW_CAST_TEMPLATE_IO_2(T__CLASS) (const T__CLASS<T1,T2>)
#  define RW_CAST_TEMPLATE_IO_3(T__CLASS) (const T__CLASS<T1,T2,T3>)
#  define RW_CAST_TEMPLATE_IO_4(T__CLASS) (const T__CLASS<T1,T2,T3,T4>)
#  define RW_CAST_TEMPLATE_IO_5(T__CLASS) (const T__CLASS<T1,T2,T3,T4,T5>)
#else
#  define RW_CAST_TEMPLATE_IO(T__CLASS)
#  define RW_CAST_TEMPLATE_IO_2(T__CLASS)
#  define RW_CAST_TEMPLATE_IO_3(T__CLASS)
#  define RW_CAST_TEMPLATE_IO_4(T__CLASS)
#  define RW_CAST_TEMPLATE_IO_5(T__CLASS)
#endif


//////////////////////////////////////////////////////////////////////////
//
// Class template stream insertion and stream extraction implementation
// macros WITHOUT ALLOCATORS.
//
//////////////////////////////////////////////////////////////////////////
#define RW_DEFINE_PERSIST_INSERTION_IMPL(T__CLASS,ISTR,OSTR,TARGS)   \
  OSTR& operator<<(OSTR& stream, const T__CLASS<TARGS>& item)       \
  {                                                                 \
    size_t objectNum;                                               \
                                                                    \
    RWUseStoreTable storeTable;                                     \
                                                                    \
    if(storeTable.add(&item, objectNum)){                           \
      stream << ':';                                                \
      rwSaveGuts(stream,RW_CAST_TEMPLATE_IO(T__CLASS) item);        \
    }                                                               \
    else                                                            \
      stream << '@' << objectNum;                                   \
                                                                    \
    return stream;                                                  \
  }

#define RW_DEFINE_PERSIST_REF_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS) \
  ISTR& operator>>(ISTR& stream, T__CLASS<TARGS>& object)           \
  {                                                                 \
    if (!stream.good()) return stream;                              \
                                                                    \
    size_t         objectNum;                                       \
    char           refFlag;                                         \
    RWUseReadTable readTable;                                       \
                                                                    \
    stream >> refFlag;                                              \
    if (!stream.good()) return stream;                              \
                                                                    \
    if(refFlag == '@')                                              \
    {                                                               \
      stream >> objectNum;                                          \
      if (!stream.good()) return stream;                            \
      if (  objectNum >= readTable.entries()                        \
          || (&object != readTable.getPtr(objectNum))               \
         )                                                          \
        RWTHROW(RWInternalErr(RWMessage(RWTOOL_REF)));              \
    }                                                               \
    else if (refFlag == ':') {                                      \
      if (!stream.good()) return stream;                            \
      readTable.append(&object);                                    \
      rwRestoreGuts(stream, object);                                \
    }                                                               \
    else {                                                          \
      /* Neither '@' nor ':' was found.  Bad input stream. */       \
        RWTHROW(RWInternalErr(RWMessage(RWTOOL_REF)));              \
    }                                                               \
    return stream;                                                  \
  }

#define RW_DEFINE_PERSIST_PTR_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS) \
  ISTR& operator>>(ISTR& stream, T__CLASS<TARGS>*& objectPtr)       \
  {                                                                 \
    if (!stream.good()) return stream;                              \
                                                                    \
    size_t         objectNum;                                       \
    char           refFlag;                                         \
    RWUseReadTable readTable;                                       \
                                                                    \
    stream >> refFlag;                                              \
    if (!stream.good()) return stream;                              \
                                                                    \
    if(refFlag == '@')                                              \
    {                                                               \
      stream >> objectNum;                                          \
      if (!stream.good()) return stream;                            \
      if ( objectNum >= readTable.entries())                        \
        RWTHROW(RWInternalErr(RWMessage(RWTOOL_REF)));              \
      else                                                          \
        objectPtr =                                                 \
          (T__CLASS<TARGS>*)                                        \
          readTable.getPtr(objectNum);                              \
    }                                                               \
    else if (refFlag == ':') {                                      \
      if (!stream.good()) return stream;                            \
      objectPtr =                                                   \
        new T__CLASS<TARGS>;                                        \
      readTable.append(objectPtr);                                  \
      rwRestoreGuts(stream, *objectPtr);                            \
    }                                                               \
    else {                                                          \
      /* Neither '@' nor ':' was found.  Bad input stream. */       \
        RWTHROW(RWInternalErr(RWMessage(RWTOOL_REF)));              \
    }                                                               \
    return stream;                                                  \
  }
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
// Class template stream insertion and stream extraction implementation
// macros WITH ALLOCATORS.
//
//////////////////////////////////////////////////////////////////////////
#define RW_DEFINE_TLS_PERSIST_INSERTION_IMPL(T__CLASS,ISTR,OSTR,TARGS)   \
  OSTR& operator<<(OSTR& stream,                                        \
  const T__CLASS<TARGS, A>& item)                       \
  {                                                                     \
    size_t objectNum;                                                   \
                                                                        \
    RWUseStoreTable storeTable;                                         \
                                                                        \
    if(storeTable.add(&item, objectNum)){                               \
      stream << ':';                                                    \
      rwSaveGuts(stream,RW_CAST_TEMPLATE_IO(T__CLASS) item);            \
    }                                                                   \
    else                                                                \
      stream << '@' << objectNum;                                       \
                                                                        \
    return stream;                                                      \
  }

#define RW_DEFINE_TLS_PERSIST_REF_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS) \
  ISTR& operator>>(ISTR& stream,                                        \
  T__CLASS<TARGS, A>& object)                           \
  {                                                                     \
    if (!stream.good()) return stream;                                  \
                                                                        \
    size_t         objectNum;                                           \
    char           refFlag;                                             \
    RWUseReadTable readTable;                                           \
                                                                        \
    stream >> refFlag;                                                  \
    if (!stream.good()) return stream;                                  \
                                                                        \
    if(refFlag == '@')                                                  \
    {                                                                   \
      stream >> objectNum;                                              \
      if (!stream.good()) return stream;                                \
      if (  objectNum >= readTable.entries()                            \
          || (&object != readTable.getPtr(objectNum))                   \
         )                                                              \
        RWTHROW(RWInternalErr(RWMessage(RWTOOL_REF)));                  \
    }                                                                   \
    else if (refFlag == ':') {                                          \
      if (!stream.good()) return stream;                                \
      readTable.append(&object);                                        \
      rwRestoreGuts(stream, object);                                    \
    }                                                                   \
    else {                                                              \
      /* Neither '@' nor ':' was found.  Bad input stream. */           \
        RWTHROW(RWInternalErr(RWMessage(RWTOOL_REF)));                  \
    }                                                                   \
    return stream;                                                      \
  }

#define RW_DEFINE_TLS_PERSIST_PTR_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS) \
  ISTR& operator>>(ISTR& stream,                                        \
  T__CLASS<TARGS, A>*& objectPtr)                       \
  {                                                                     \
    if (!stream.good()) return stream;                                  \
                                                                        \
    size_t         objectNum;                                           \
    char           refFlag;                                             \
    RWUseReadTable readTable;                                           \
                                                                        \
    stream >> refFlag;                                                  \
    if (!stream.good()) return stream;                                  \
                                                                        \
    if(refFlag == '@')                                                  \
    {                                                                   \
      stream >> objectNum;                                              \
      if (!stream.good()) return stream;                                \
      if ( objectNum >= readTable.entries())                            \
        RWTHROW(RWInternalErr(RWMessage(RWTOOL_REF)));                  \
      else                                                              \
        objectPtr =                                                     \
          (T__CLASS<TARGS, A>*)                         \
          readTable.getPtr(objectNum);                                  \
    }                                                                   \
    else if (refFlag == ':') {                                          \
      if (!stream.good()) return stream;                                \
      objectPtr =                                                       \
        new T__CLASS<TARGS, A>;                         \
      readTable.append(objectPtr);                                      \
      rwRestoreGuts(stream, *objectPtr);                                \
    }                                                                   \
    else {                                                              \
      /* Neither '@' nor ':' was found.  Bad input stream. */           \
        RWTHROW(RWInternalErr(RWMessage(RWTOOL_REF)));                  \
    }                                                                   \
    return stream;                                                      \
  }


//////////////////////////////////////////////////////////////////////////
//
// Utility macros for multiple template arguments
//
//////////////////////////////////////////////////////////////////////////
#define TARGS1 T
#define TARGS2 T1,T2
#define TARGS3 T1,T2,T3
#define TARGS4 T1,T2,T3,T4
#define TARGS5 T1,T2,T3,T4,T5

//////////////////////////////////////////////////////////////////////////
//
// Class template IO stream definition macros for differing numbers of
// template arguments WITHOUT ALLOCATORS
//
//////////////////////////////////////////////////////////////////////////
#define RW_DEFINE_PERSIST_TEMPLATE_IO(T__CLASS,ISTR,OSTR)                \
template <class T>                                                      \
RW_DEFINE_PERSIST_INSERTION_IMPL(T__CLASS,ISTR,OSTR,TARGS1)              \
                                                                        \
template <class T>                                                      \
RW_DEFINE_PERSIST_REF_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS1)            \
                                                                        \
template <class T>                                                      \
RW_DEFINE_PERSIST_PTR_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS1)            \
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define RW_DEFINE_PERSIST_TEMPLATE_IO_2(T__CLASS,ISTR,OSTR)              \
template <class T1, class T2>                                           \
RW_DEFINE_PERSIST_INSERTION_IMPL(T__CLASS,ISTR,OSTR,TARGS2)              \
                                                                        \
template <class T1, class T2>                                           \
RW_DEFINE_PERSIST_REF_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS2)            \
                                                                        \
template <class T1, class T2>                                           \
RW_DEFINE_PERSIST_PTR_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS2)            \
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define RW_DEFINE_PERSIST_TEMPLATE_IO_3(T__CLASS,ISTR,OSTR)              \
template <class T1, class T2, class T3>                                 \
RW_DEFINE_PERSIST_INSERTION_IMPL(T__CLASS,ISTR,OSTR,TARGS3)              \
                                                                        \
template <class T1, class T2, class T3>                                 \
RW_DEFINE_PERSIST_REF_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS3)            \
                                                                        \
template <class T1, class T2, class T3>                                 \
RW_DEFINE_PERSIST_PTR_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS3)            \
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define RW_DEFINE_PERSIST_TEMPLATE_IO_4(T__CLASS,ISTR,OSTR)              \
template <class T1, class T2, class T3, class T4>                       \
RW_DEFINE_PERSIST_INSERTION_IMPL(T__CLASS,ISTR,OSTR,TARGS4)              \
                                                                        \
template <class T1, class T2, class T3, class T4>                       \
RW_DEFINE_PERSIST_REF_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS4)            \
                                                                        \
template <class T1, class T2, class T3, class T4>                       \
RW_DEFINE_PERSIST_PTR_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS4)            \
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define RW_DEFINE_PERSIST_TEMPLATE_IO_5(T__CLASS,ISTR,OSTR)              \
template <class T1, class T2, class T3, class T4, class T5>             \
RW_DEFINE_PERSIST_INSERTION_IMPL(T__CLASS,ISTR,OSTR,TARGS5)              \
                                                                        \
template <class T1, class T2, class T3, class T4, class T5>             \
RW_DEFINE_PERSIST_REF_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS5)            \
                                                                        \
template <class T1, class T2, class T3, class T4, class T5>             \
RW_DEFINE_PERSIST_PTR_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS5)            \
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
// Class template IO stream definition macros for differing numbers of
// template arguments WITH ALLOCATORS
//
//////////////////////////////////////////////////////////////////////////
#define RW_DEFINE_TLS_PERSIST_TEMPLATE_IO(T__CLASS,ISTR,OSTR)            \
template <class T, class A>                   \
RW_DEFINE_TLS_PERSIST_INSERTION_IMPL(T__CLASS,ISTR,OSTR,TARGS1)          \
                                                                        \
template <class T, class A>                   \
RW_DEFINE_TLS_PERSIST_REF_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS1)        \
                                                                        \
template <class T, class A>                   \
RW_DEFINE_TLS_PERSIST_PTR_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS1)
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define RW_DEFINE_TLS_PERSIST_TEMPLATE_IO_2(T__CLASS,ISTR,OSTR)          \
template <class T1, class T2, class A>         \
RW_DEFINE_TLS_PERSIST_INSERTION_IMPL(T__CLASS,ISTR,OSTR,TARGS2)          \
                                                                        \
template <class T1, class T2, class A>         \
RW_DEFINE_TLS_PERSIST_REF_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS2)        \
                                                                        \
template <class T1, class T2, class A>         \
RW_DEFINE_TLS_PERSIST_PTR_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS2)
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define RW_DEFINE_TLS_PERSIST_TEMPLATE_IO_3(T__CLASS,ISTR,OSTR)            \
template <class T1, class T2, class T3, class A> \
RW_DEFINE_TLS_PERSIST_INSERTION_IMPL(T__CLASS,ISTR,OSTR,TARGS3)            \
                                                                          \
template <class T1, class T2, class T3, class A> \
RW_DEFINE_TLS_PERSIST_REF_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS3)          \
                                                                          \
template <class T1, class T2, class T3, class A> \
RW_DEFINE_TLS_PERSIST_PTR_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS3)
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define RW_DEFINE_TLS_PERSIST_TEMPLATE_IO_4(T__CLASS,ISTR,OSTR)                      \
template <class T1, class T2, class T3, class T4, class A> \
RW_DEFINE_TLS_PERSIST_INSERTION_IMPL(T__CLASS,ISTR,OSTR,TARGS4)                      \
                                                                                    \
template <class T1, class T2, class T3, class T4, class A> \
RW_DEFINE_TLS_PERSIST_REF_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS4)                    \
                                                                                    \
template <class T1, class T2, class T3, class T4, class A> \
RW_DEFINE_TLS_PERSIST_PTR_EXTRACT_IMPL(T__CLASS,ISTR,OSTR,TARGS4)
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
// Use these macros for classes that are not instantiated with an
// allocator
//
//////////////////////////////////////////////////////////////////////////

/**
 * @ingroup spro_core_classes
 * Defines the functions necessary to provide isomorphic persistence to
 * user-defined classes that implement rwSaveGuts() and rwRestoreGuts().
 */
#define RW_DEFINE_PERSISTABLE(CLASS)                                     \
RW_DEFINE_PERSIST_IO(CLASS,RWvistream,RWvostream)                        \
RW_DEFINE_PERSIST_IO(CLASS,RWFile,RWFile)

/**
 * @ingroup spro_core_classes
 * Declares the functions necessary to provide isomorphic persistence to
 * user-defined class templates that take one template parameter and
 * implement rwSaveGuts() and rwRestoreGuts().
 */
#define RW_DEFINE_PERSISTABLE_TEMPLATE(T__CLASS)                         \
RW_DEFINE_PERSIST_TEMPLATE_IO(T__CLASS,RWvistream,RWvostream)            \
RW_DEFINE_PERSIST_TEMPLATE_IO(T__CLASS,RWFile,RWFile)

/**
 * @ingroup spro_core_classes
 * Declares the functions necessary to provide isomorphic persistence to
 * user-defined class templates that take two template parameters and
 * implement rwSaveGuts() and rwRestoreGuts().
 */
#define RW_DEFINE_PERSISTABLE_TEMPLATE_2(T__CLASS)                       \
RW_DEFINE_PERSIST_TEMPLATE_IO_2(T__CLASS,RWvistream,RWvostream)          \
RW_DEFINE_PERSIST_TEMPLATE_IO_2(T__CLASS,RWFile,RWFile)

/**
 * @ingroup spro_core_classes
 * Declares the functions necessary to provide isomorphic persistence to
 * user-defined class templates that take three template parameters and
 * implement rwSaveGuts() and rwRestoreGuts().
 */
#define RW_DEFINE_PERSISTABLE_TEMPLATE_3(T__CLASS)                       \
RW_DEFINE_PERSIST_TEMPLATE_IO_3(T__CLASS,RWvistream,RWvostream)          \
RW_DEFINE_PERSIST_TEMPLATE_IO_3(T__CLASS,RWFile,RWFile)

/**
 * @ingroup spro_core_classes
 * Declares the functions necessary to provide isomorphic persistence to
 * user-defined class templates that take four template parameters and
 * implement rwSaveGuts() and rwRestoreGuts().
 */
#define RW_DEFINE_PERSISTABLE_TEMPLATE_4(T__CLASS)                       \
RW_DEFINE_PERSIST_TEMPLATE_IO_4(T__CLASS,RWvistream,RWvostream)          \
RW_DEFINE_PERSIST_TEMPLATE_IO_4(T__CLASS,RWFile,RWFile)

/**
 * @ingroup spro_core_classes
 * Declares the functions necessary to provide isomorphic persistence to
 * user-defined class templates that take five template parameters and
 * implement rwSaveGuts() and rwRestoreGuts().
 */
#define RW_DEFINE_PERSISTABLE_TEMPLATE_5(T__CLASS)                       \
RW_DEFINE_PERSIST_TEMPLATE_IO_5(T__CLASS,RWvistream,RWvostream)          \
RW_DEFINE_PERSIST_TEMPLATE_IO_5(T__CLASS,RWFile,RWFile)
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
// These are used to define persistable class templates for classes
// that are instantiated with an allocator
//
//////////////////////////////////////////////////////////////////////////
#define RW_DEFINE_TLS_PERSISTABLE_TEMPLATE(T__CLASS)                     \
RW_DEFINE_TLS_PERSIST_TEMPLATE_IO(T__CLASS,RWvistream,RWvostream)        \
RW_DEFINE_TLS_PERSIST_TEMPLATE_IO(T__CLASS,RWFile,RWFile)

#define RW_DEFINE_TLS_PERSISTABLE_TEMPLATE_2(T__CLASS)                   \
RW_DEFINE_TLS_PERSIST_TEMPLATE_IO_2(T__CLASS,RWvistream,RWvostream)      \
RW_DEFINE_TLS_PERSIST_TEMPLATE_IO_2(T__CLASS,RWFile,RWFile)

#define RW_DEFINE_TLS_PERSISTABLE_TEMPLATE_3(T__CLASS)                   \
RW_DEFINE_TLS_PERSIST_TEMPLATE_IO_3(T__CLASS,RWvistream,RWvostream)      \
RW_DEFINE_TLS_PERSIST_TEMPLATE_IO_3(T__CLASS,RWFile,RWFile)

#define RW_DEFINE_TLS_PERSISTABLE_TEMPLATE_4(T__CLASS)                   \
RW_DEFINE_TLS_PERSIST_TEMPLATE_IO_4(T__CLASS,RWvistream,RWvostream)      \
RW_DEFINE_TLS_PERSIST_TEMPLATE_IO_4(T__CLASS,RWFile,RWFile)
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


#if defined(RW_USE_CLASSIC_TEMPLATES)
// provide functions so we can use apply() to stream data out

template<class T>
void rwInsertInStream(T& item, void* x);

template<class T>
void rwInsertInFile(T& item, void* x);

#endif /* RW_USE_CLASSIC_TEMPLATES */


#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("epersist.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include <rw/epersist.cc>
#endif

#endif /* RW_TOOLS_EPERSIST_H */
