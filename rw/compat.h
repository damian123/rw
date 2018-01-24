#ifndef RW_TOOLS_COMPAT_H
#define RW_TOOLS_COMPAT_H


/**********************************************************************
 *
 * Macros declarations
 * The file contains compatibility definitions for macros that have
 * been replaced so that downstream and/or user products do not break
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/compat.h#1 $
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

// For floating-point precision macros
#include <float.h>

#if defined(RW_LITTLE_ENDIAN)
#  define RW_USE_LITTLE_ENDIAN 1
#endif // RW_USE_LITTLE_ENDIAN

// Only for backward compatibility
#define RW_SL_STD_USING using namespace std;
#define RW_NESTED_NAMESPACE(x,y) namespace x { namespace y {
#define RW_END_NESTED_NAMESPACE(x,y) }} // end namespace x, y

// Handles differences in support for the friend keyword
#ifdef RW_NO_FRIEND_INLINE_DECL
#  define RW_INLINE_FRIEND friend
#else
#  ifdef RW_FRIEND_THEN_INLINE
#    define RW_INLINE_FRIEND friend inline
#  else
#    define RW_INLINE_FRIEND inline friend
#  endif
#endif

#ifdef RW_NO_FRIEND_INLINE_TMPL_DECL
#  define RW_INLINE_TMPL_FRIEND friend
#else
#  ifdef RW_FRIEND_THEN_INLINE
#    define RW_INLINE_TMPL_FRIEND friend inline
#  else
#    define RW_INLINE_TMPL_FRIEND inline friend
#  endif
#endif

#if defined (RW_NO_USING_DIRECTIVE)
#  if !defined(RW_NAMESPACE_REQUIRED)
#    define RW_NAMESPACE_REQUIRED
#  endif // RW_NAMESPACE_REQUIRED
#endif

// TODO: replace the usage of RW_COMPILE_INSTANTIATE with the new one
#if defined(RW_NO_IMPLICIT_INCLUSION) && !defined(RW_COMPILE_INSTANTIATE)
#  define RW_COMPILE_INSTANTIATE
#endif // RW_NO_IMPLICIT_INCLUSION && !RW_COMPILE_INSTANTIATE

#if defined(RW_DEBUG) && !defined(RWDEBUG)
#  if    !defined(__HP_aCC) \
      || !defined (RW_STDLIB_VER) || RW_STDLIB_VER != 0x01020100
#    define RWDEBUG 1
#  endif // not HP aCC w/ native C++ Standard Library
#endif // RW_DEBUG && !RWDEBUG

#if defined(RW_SHARED) && !defined(RWSHARED)
#  define RWSHARED 1
#endif // RW_SHARED && !RWSHARED

#if defined(_WIN32) && defined(RW_SHARED)
#  define RW_DLL 1
#  if defined(_RWBUILD_tls)
#    define RWBUILDSHARED 1
#    define _RWBUILDDLL 1
#  endif // _RWBUILD_tls
#endif // _WIN32 && RW_SHARED

#if defined(RW_DLL) && !defined(RWDLL)
#  define RWDLL 1
#endif // RW_DLL && !RWDLL

// Sync legacy macro
#define RWGUARD(mutex)   RW_MT_GUARD(mutex)

// RWASSERT definition
#define RWASSERT(a)    RW_ASSERT(a)

// RWEXPLICIT definition
#define RW_EXPLICIT    explicit
#define RWEXPLICIT     RW_EXPLICIT

// RWTYPENAME definition
#define RWTYPENAME     RW_TYPENAME

// Tools version macro
#define RW_TOOLS_VER   RWTOOLS

// Historical macro support related to DLL building
#if defined(_MSC_VER)

#  if defined(_WINDLL) && !defined(__DLL__)
#    define __DLL__     1
#  endif

#  if !defined(_export)
#    define _export __export
#  endif

#  if defined(_RWCLASSDLL) && !defined(RWTOOLSDLL)
#    define RWTOOLSDLL  1
#  endif

#  if defined(RWTOOLSDLL) && !defined(__DLL__)
#    define __DLL__     1
#  endif

#endif // _MSC_VER

// __WIN32__ and __WIN64__ macros
#if defined(_WIN32) && !defined(__WIN32__)
#  define __WIN32__
#endif // __WIN32 && !__WIN32__

#if defined(_WIN64) && !defined(__WIN64__)
#  define __WIN64__
#endif // __WIN64 && !__WIN64__

#if !defined(RW_NO_TM_ZONE) && !defined(RW_STRUCT_TM_TZ)
#  define RW_STRUCT_TM_TZ
#endif // RW_STRUCT_TM_TZ

// Compatibility in function names; some of them may
// be used by users although they are not documented
#define RWbsearch       rwBSearch
#define RWhashAddress   rwHashAddress
#define RWor            rwOR
#define RWxor           rwXOR
#define RWand           rwAND
#define rwFirstTrue     rwFindOne
#define rwFirstFalse    rwFindZero

// Various numeric macros used in containers
#define RWSUCCESS                    RW_SUCCESS
#define RWDEFAULT_CAPACITY           RW_DEFAULT_CAPACITY
#define RWDEFAULT_RESIZE             RW_DEFAULT_RESIZE
#define RWBITSPERBYTE                RW_BYTE_SIZE

// Various legacy concatenation macros
#define name2(a, b)       RW_PASTE2(a, b)
#define name3(a, b, c)    RW_PASTE3(a, b, c)
#define name4(a, b, c, d) RW_PASTE4(a, b, c, d)

#define declare(name,type)     RW_DECLARE_GENERIC_CLASS (name,type)
#define implement(name,type)   RW_DEFINE_GENERIC_CLASS  (name, type)

#define RWNAME2(a, b)     RW_PASTE2(a, b)
#define RWNAME3(a, b, c)  RW_PASTE2(a, b, c)
#define RWSTRINGIFY(a)    RW_STRINGIFY(a)

/***************************************************************************/

#define RWClassicCString RWCopyOnWriteCString
#define RWClassicCSubString RWCopyOnWriteCSubString
#define RWClassicCConstSubString RWCopyOnWriteCConstSubString
#define RWClassicWString RWCopyOnWriteWString
#define RWClassicWSubString RWCopyOnWriteWSubString
#define RWClassicWConstSubString RWCopyOnWriteWConstSubString

#if defined(RW_CLASSIC_TEMPLATE_CONTAINERS)
#  ifndef RW_USE_CLASSIC_TEMPLATES
#    define RW_USE_CLASSIC_TEMPLATES
#  endif
#  define RWTPtrDlist                  RWClassicTPtrDlist
#  define RWTPtrDlistIterator          RWClassicTPtrDlistIterator
#  define RWTPtrHashTable              RWClassicTPtrHashTable
#  define RWTPtrHashTableIterator      RWClassicTPtrHashTableIterator
#  define RWTPtrHashDictionary         RWClassicTPtrHashDictionary
#  define RWTPtrHashDictionaryIterator RWClassicTPtrHashDictionaryIterator
#  define RWTPtrHashSet                RWClassicTPtrHashSet
#  define RWTPtrHashSetIterator        RWClassicTPtrHashSetIterator
#  define RWTPtrOrderedVector          RWClassicTPtrOrderedVector
#  define RWTPtrSlist                  RWClassicTPtrSlist
#  define RWTPtrSlistIterator          RWClassicTPtrSlistIterator
#  define RWTPtrSortedVector           RWClassicTPtrSortedVector
#  define RWTValDlist                  RWClassicTValDlist
#  define RWTValDlistIterator          RWClassicTValDlistIterator
#  define RWTValHashTable              RWClassicTValHashTable
#  define RWTValHashTableIterator      RWClassicTValHashTableIterator
#  define RWTValHashDictionary         RWClassicTValHashDictionary
#  define RWTValHashDictionaryIterator RWClassicTValHashDictionaryIterator
#  define RWTValHashSet                RWClassicTValHashSet
#  define RWTValHashSetIterator        RWClassicTValHashSetIterator
#  define RWTValOrderedVector          RWClassicTValOrderedVector
#  define RWTValSlist                  RWClassicTValSlist
#  define RWTValSlistIterator          RWClassicTValSlistIterator
#  define RWTValSortedVector           RWClassicTValSortedVector
#endif

/**************************************************************************/

// Define the old macros in terms of the new ones
#if defined(_WIN32)

// Make sure RWTOOLSDLL is defined
#  if defined(RWDLL) || defined(RWSHARED) || defined(_RWTOOLSDLL)
#    undef  RWTOOLSDLL
#    define RWTOOLSDLL   1
#  endif // DLL building

// Make sure RWBUILD is correctly defined
#  if defined(_RWBUILD) || defined(_RWBUILD_tls)
#    undef  RWBUILD
#    define RWBUILD      1
#  endif // RWBUILD - building the library
#endif

#define RWExport    RW_TOOLS_SYMBOLIC
#define RWGExport   RW_TOOLS_SYMBOLIC
#define RWTExport
#define RWTIExport  RW_TOOLS_SYMBOLIC
#define rwexport    RW_TOOLS_SYMBOLIC
#define rwtexport
#define RWSExport   RWExport

// The first macro is defined as such to
// temporarily accommodate the persistable macros.
#define RWEXPORT_RETURN(x)   RW_TOOLS_ONCE_EXPORT x
#define RWTEXPORT_RETURN(x)  x

// Compatibility macros
#define RWDECLARE_DLL_COLLECTABLE(x)                        \
    RW_DECLARE_COLLECTABLE_CLASS(RW_TOOLS_ALWAYS_EXPORT, x)

#define RWDECLARE_ABSTRACT_COLLECTABLE(x)                           \
    RW_DECLARE_ABSTRACT_COLLECTABLE_CLASS(RW_TOOLS_ONCE_EXPORT, x)

#define RWDEFINE_ABSTRACT_COLLECTABLE(x)                            \
    RW_DEFINE_ABSTRACT_COLLECTABLE_CLASS(RW_TOOLS_ONCE_EXPORT, x)

#define RWDECLARE_COLLECTABLE(x)                            \
    RW_DECLARE_COLLECTABLE_CLASS(RW_TOOLS_ONCE_EXPORT, x)

#define RWDEFINE_COLLECTABLE(x,y)                                   \
    RW_DEFINE_COLLECTABLE_CLASS_BY_ID(RW_TOOLS_ONCE_EXPORT, x, y)

#define RWDEFINE_NAMED_COLLECTABLE(x,s)                                 \
    RW_DEFINE_COLLECTABLE_CLASS_BY_NAME(RW_TOOLS_ONCE_EXPORT, x, s)

/**************************************************************************/

// Persistence macros
#define RWDECLARE_PERSISTABLE(x)  RW_DECLARE_PERSISTABLE(x)

#define RWDECLARE_PERSISTABLE_TEMPLATE(x)   RW_DECLARE_PERSISTABLE_TEMPLATE(x)
#define RWDECLARE_PERSISTABLE_TEMPLATE_2(x) RW_DECLARE_PERSISTABLE_TEMPLATE_2(x)
#define RWDECLARE_PERSISTABLE_TEMPLATE_3(x) RW_DECLARE_PERSISTABLE_TEMPLATE_3(x)
#define RWDECLARE_PERSISTABLE_TEMPLATE_4(x) RW_DECLARE_PERSISTABLE_TEMPLATE_4(x)
#define RWDECLARE_PERSISTABLE_TEMPLATE_5(x) RW_DECLARE_PERSISTABLE_TEMPLATE_5(x)

#define RWDECLARE_PERSIST_IO(x,in,out)            RW_DECLARE_PERSIST_IO(x,in,out)
#define RWDECLARE_PERSIST_TEMPLATE_IO(x,in,out)   RW_DECLARE_PERSIST_TEMPLATE_IO(x,in,out)
#define RWDECLARE_PERSIST_TEMPLATE_IO_2(x,in,out) RW_DECLARE_PERSIST_TEMPLATE_IO_2(x,in,out)
#define RWDECLARE_PERSIST_TEMPLATE_IO_3(x,in,out) RW_DECLARE_PERSIST_TEMPLATE_IO_3(x,in,out)
#define RWDECLARE_PERSIST_TEMPLATE_IO_4(x,in,out) RW_DECLARE_PERSIST_TEMPLATE_IO_4(x,in,out)
#define RWDECLARE_PERSIST_TEMPLATE_IO_5(x,in,out) RW_DECLARE_PERSIST_TEMPLATE_IO_5(x,in,out)

#define RWDECLARE_TLS_PERSISTABLE_TEMPLATE(x) RW_DECLARE_TLS_PERSISTABLE_TEMPLATE(x)
#define RWDECLARE_TLS_PERSISTABLE_TEMPLATE_2(x) RW_DECLARE_TLS_PERSISTABLE_TEMPLATE_2(x)
#define RWDECLARE_TLS_PERSISTABLE_TEMPLATE_3(x) RW_DECLARE_TLS_PERSISTABLE_TEMPLATE_3(x)
#define RWDECLARE_TLS_PERSISTABLE_TEMPLATE_4(x) RW_DECLARE_TLS_PERSISTABLE_TEMPLATE_4(x)

#define RWDECLARE_TLS_PERSIST_TEMPLATE_IO(x,in,out) RW_DECLARE_TLS_PERSIST_TEMPLATE_IO(x,in,out)
#define RWDECLARE_TLS_PERSIST_TEMPLATE_IO_2(x,in,out) RW_DECLARE_TLS_PERSIST_TEMPLATE_IO_2(x,in,out)
#define RWDECLARE_TLS_PERSIST_TEMPLATE_IO_3(x,in,out) RW_DECLARE_TLS_PERSIST_TEMPLATE_IO_3(x,in,out)
#define RWDECLARE_TLS_PERSIST_TEMPLATE_IO_4(x,in,out) RW_DECLARE_TLS_PERSIST_TEMPLATE_IO_4(x,in,out)

#define RWDEFINE_PERSISTABLE(x) RW_DEFINE_PERSISTABLE(x)

#define RWDEFINE_PERSISTABLE_TEMPLATE(x)   RW_DEFINE_PERSISTABLE_TEMPLATE(x)
#define RWDEFINE_PERSISTABLE_TEMPLATE_2(x) RW_DEFINE_PERSISTABLE_TEMPLATE_2(x)
#define RWDEFINE_PERSISTABLE_TEMPLATE_3(x) RW_DEFINE_PERSISTABLE_TEMPLATE_3(x)
#define RWDEFINE_PERSISTABLE_TEMPLATE_4(x) RW_DEFINE_PERSISTABLE_TEMPLATE_4(x)
#define RWDEFINE_PERSISTABLE_TEMPLATE_5(x) RW_DEFINE_PERSISTABLE_TEMPLATE_5(x)

#define RWDEFINE_PERSIST_INSERTION_IMPL(x,in,out,args) RW_DEFINE_PERSIST_INSERTION_IMPL(x,in,out,args)

#define RWDEFINE_PERSIST_IO(x,in,out) RW_DEFINE_PERSIST_IO(x,in,out)

#define RWDEFINE_PERSIST_PTR_EXTRACT_IMPL(x,in,out,args) RW_DEFINE_PERSIST_PTR_EXTRACT_IMPL(x,in,out,args)
#define RWDEFINE_PERSIST_REF_EXTRACT_IMPL(x,in,out,args) RW_DEFINE_PERSIST_REF_EXTRACT_IMPL(x,in,out,args)

#define RWDEFINE_PERSIST_TEMPLATE_IO(x,in,out)   RW_DEFINE_PERSIST_TEMPLATE_IO(x,in,out)
#define RWDEFINE_PERSIST_TEMPLATE_IO_2(x,in,out) RW_DEFINE_PERSIST_TEMPLATE_IO_2(x,in,out)
#define RWDEFINE_PERSIST_TEMPLATE_IO_3(x,in,out) RW_DEFINE_PERSIST_TEMPLATE_IO_3(x,in,out)
#define RWDEFINE_PERSIST_TEMPLATE_IO_4(x,in,out) RW_DEFINE_PERSIST_TEMPLATE_IO_4(x,in,out)
#define RWDEFINE_PERSIST_TEMPLATE_IO_5(x,in,out) RW_DEFINE_PERSIST_TEMPLATE_IO_5(x,in,out)

#define RWDEFINE_TLS_PERSISTABLE_TEMPLATE(x)   RW_DEFINE_TLS_PERSISTABLE_TEMPLATE(x)
#define RWDEFINE_TLS_PERSISTABLE_TEMPLATE_2(x) RW_DEFINE_TLS_PERSISTABLE_TEMPLATE_2(x)
#define RWDEFINE_TLS_PERSISTABLE_TEMPLATE_3(x) RW_DEFINE_TLS_PERSISTABLE_TEMPLATE_3(x)
#define RWDEFINE_TLS_PERSISTABLE_TEMPLATE_4(x) RW_DEFINE_TLS_PERSISTABLE_TEMPLATE_4(x)

#define RWDEFINE_TLS_PERSIST_INSERTION_IMPL(x,in,out,args) RW_DEFINE_TLS_PERSIST_INSERTION_IMPL(x,in,out,args)

#define RWDEFINE_TLS_PERSIST_PTR_EXTRACT_IMPL(x,in,out,args) RW_DEFINE_TLS_PERSIST_PTR_EXTRACT_IMPL(x,in,out,args)
#define RWDEFINE_TLS_PERSIST_REF_EXTRACT_IMPL(x,in,out,args) RW_DEFINE_TLS_PERSIST_REF_EXTRACT_IMPL(x,in,out,args)

#define RWDEFINE_TLS_PERSIST_TEMPLATE_IO(x,in,out)   RW_DEFINE_TLS_PERSIST_TEMPLATE_IO(x,in,out)
#define RWDEFINE_TLS_PERSIST_TEMPLATE_IO_2(x,in,out) RW_DEFINE_TLS_PERSIST_TEMPLATE_IO_2(x,in,out)
#define RWDEFINE_TLS_PERSIST_TEMPLATE_IO_3(x,in,out) RW_DEFINE_TLS_PERSIST_TEMPLATE_IO_3(x,in,out)
#define RWDEFINE_TLS_PERSIST_TEMPLATE_IO_4(x,in,out) RW_DEFINE_TLS_PERSIST_TEMPLATE_IO_4(x,in,out)

// Hash and sorted container helpers
#if !defined(RW_CLASSIC_TEMPLATE_CONTAINERS)
#  ifndef RWDefCArgs
#    define RWDefCArgs(T) ,std::less< T >
#  endif
#  ifndef RWDefHArgs
#    define RWDefHArgs(T) ,RWTHasher< T >,std::equal_to< T >
#  endif
#else
#  ifndef RWDefCArgs
#    define RWDefCArgs(T)
#  endif
#  ifndef RWDefHArgs
#    define RWDefHArgs(T)
#  endif
#endif

/**************************************************************************/

// Standard Smalltalk interface, activated by defining
// RW_STD_TYPEDEFS before including any Tools header.

#ifdef RW_STD_TYPEDEFS

class RW_TOOLS_GLOBAL RWBag;
class RW_TOOLS_GLOBAL RWBagIterator;
class RW_TOOLS_GLOBAL RWBinaryTree;
class RW_TOOLS_GLOBAL RWBinaryTreeIterator;
class RW_TOOLS_GLOBAL RWCollectableDate;
class RW_TOOLS_GLOBAL RWCollectableInt;
class RW_TOOLS_GLOBAL RWCollectableString;

#if !defined(RW_DISABLE_DEPRECATED)
class RW_TOOLS_GLOBAL RWCollectableTime;
#endif

class RW_TOOLS_GLOBAL RWCollection;
class RW_TOOLS_GLOBAL RWDlistCollectables;
class RW_TOOLS_GLOBAL RWDlistCollectablesIterator;
class RW_TOOLS_GLOBAL RWHashDictionary;
class RW_TOOLS_GLOBAL RWHashDictionaryIterator;
class RW_TOOLS_GLOBAL RWIdentityDictionary;
class RW_TOOLS_GLOBAL RWIdentitySet;
class RW_TOOLS_GLOBAL RWOrdered;
class RW_TOOLS_GLOBAL RWOrderedIterator;
class RW_TOOLS_GLOBAL RWSequenceable;
class RW_TOOLS_GLOBAL RWSet;
class RW_TOOLS_GLOBAL RWSetIterator;
class RW_TOOLS_GLOBAL RWSlistCollectables;
class RW_TOOLS_GLOBAL RWSlistCollectablesIterator;
class RW_TOOLS_GLOBAL RWSlistCollectablesQueue;
class RW_TOOLS_GLOBAL RWSlistCollectablesStack;

typedef RWBag                           Bag;
typedef RWBagIterator                   BagIterator;
typedef RWBinaryTree                    SortedCollection;
typedef RWBinaryTreeIterator            SortedCollectionIterator;
typedef RWCollectable                   Object;
typedef RWCollectableDate               Date;
typedef RWCollectableInt                Integer;
typedef RWCollectableString             String;

#if !defined(RW_DISABLE_DEPRECATED)
typedef RWCollectableTime               Time;
#endif

typedef RWCollection                    Collection;
typedef RWHashDictionary                Dictionary;
typedef RWHashDictionaryIterator        DictionaryIterator;
typedef RWIdentityDictionary            IdentityDictionary;
typedef RWIdentitySet                   IdentitySet;
typedef RWOrdered                       OrderedCollection;
typedef RWOrderedIterator               OrderedCollectionIterator;
typedef RWSequenceable                  SequenceableCollection;
typedef RWSet                           Set;
typedef RWSetIterator                   SetIterator;
typedef RWSlistCollectables             LinkedList;
typedef RWSlistCollectablesIterator     LinkedListIterator;
typedef RWSlistCollectablesQueue        Queue;
typedef RWSlistCollectablesStack        Stack;

#endif  /* RW_STD_TYPEDEFS */

/**************************************************************************/

#define DEFINE_COLLECTABLE(a,b) RWDEFINE_COLLECTABLE(a,b)

/*********************  Old generic.h names *******************/

#define GBitVec(sz)          RWGBitVec(sz)
#define GDlist(type)         RWGDlist(type)
#define GDlistIterator(type) RWGDlistIterator(type)
#define GOrderedVector(val)  RWGOrderedVector(val)
#define GQueue(type)         RWGQueue(type)
#define GSlist(type)         RWGSlist(type)
#define GSlistdeclare(type)  RWGSlistdeclare(type)
#define GSlistIterator(type) RWGSlistIterator(type)
#define GSortedVector(type)  RWGSortedVector(type)
#define GStack(type)         RWGStack(type)
#define GVector(val)         RWGVector(val)

#define __GDLIST                      0x8014
#define __GDLISTITERATOR              0x8015
#define __GQUEUE                      0x8016
#define __GSTACK                      0x8017
#define __GVECTOR                     0x8018

/*****************  Old <rw/compiler.h> macros ****************/

#define SCOPE_NESTED_ENUMS 1

#ifndef RW_KR_ONLY
#  define KR_ONLY 1
#endif

#ifndef RW_NO_CONST_OVERLOAD
#  define CONST_OVERLOADS 1
#endif

#ifndef RW_NO_MEMMOVE
#  define HAS_MEMMOVE 1
#endif

#ifndef RW_NO_POSTFIX
#  define HAS_POSTFIX 1
#endif

/******************** From <rw/defs.h> ***************************/

#define RWINT64_MAX    (RW_INT64_MAX)
#define RWINT64_MIN    (RW_INT64_MIN)
#define RWUINT64_MAX   (RW_UINT64_MAX)
#define RWUINT64_MIN   (RW_UINT64_MIN)

typedef int fileDescTy; /* Type of file descriptors */
typedef int ClassID;

typedef unsigned char  RWByte;    /* Bitflag atomics. */

/**
 * @ingroup rwcollectable_classes
 *
 * A unique identifier for each class.
 */
typedef unsigned short RWClassID; /* Class ID tag. */
typedef unsigned short RWErrNo;   /* Error number */

typedef void*          RWvoid;         // Useful for arrays of void*.
typedef size_t         RWspace;        // Size of file allocations
typedef unsigned long  RW4Byte;
typedef long           RW4ByteSigned;

typedef rwint64 rwlonglong;
typedef rwuint64 rwulonglong;

#if defined(DOXYGEN)

/**
 * @relates RWDecimalPortable
 *
 * An unspecified signed integer type.
 */
typedef unspecified_type RWssize_t;

/**
 * @relates RWBTreeOnDisk
 *
 * An unspecified signed integer type representing a file offset.
 */
typedef unspecified_type RWstoredValue;  // Type of value in disk dictionaries.

/**
 * @ingroup file_system_classes
 *
 * An unspecified signed integer type representing a file offset.
 */
typedef unspecified_type RWoffset;  /* Used for file offsets */

#  define RW_OFFSET_MIN RW_INT64_MIN
#  define RW_OFFSET_MAX RW_INT64_MAX

#elif defined(_WIN64)

typedef __int64        RWssize_t;
typedef __int64        RWstoredValue;  // Type of value in disk dictionaries.

typedef __int64        RWoffset;  /* Used for file offsets */
#  define RW_OFFSET_MIN RW_INT64_MIN
#  define RW_OFFSET_MAX RW_INT64_MAX

#else

typedef long           RWssize_t;
typedef long           RWstoredValue;  // Type of value in disk dictionaries.

typedef long           RWoffset;  /* Used for file offsets */
#  define RW_OFFSET_MIN RW_LONG_MIN
#  define RW_OFFSET_MAX RW_LONG_MAX

#endif

// Support for large file offsets
typedef rwint64        RWoffset64;
#define RW_OFFSET64_MIN RW_INT64_MIN
#define RW_OFFSET64_MAX RW_INT64_MAX

/********************* From <rw/rstream.h> *************************/

#define OPENOSTREAM(filename, streamname)  ofstream streamname(filename);
#define OPENISTREAM(filename, streamname)  ifstream streamname(filename);

/********************* From <rw/rwdate.h> *************************/

typedef unsigned dayTy;
typedef unsigned monthTy;
typedef unsigned yearTy;
typedef unsigned long julTy;

#define jul1901 rwJul1901

/********************* From <rw/rwtime.h> *************************/

typedef unsigned hourTy;
typedef unsigned minuteTy;
typedef unsigned secondTy;
typedef unsigned long clockTy;

#define secFrom_Jan_1_1901_to_Jan_1_1970 rwEpoch

/******************** From <rw/defs.h> ***************************/

// Forward declarations (TODO - check move)
#ifndef RW_NO_IOSTD
#  include <iosfwd>
#else
class istream;
class ostream;
class ios;
#endif

#if !defined(RWPRECONDITION)
#  define RWPRECONDITION(a)  RW_PRECONDITION(a)
#endif

#if !defined(RWPOSTCONDITION)
#  define RWPOSTCONDITION(a) RW_POSTCONDITION(a)
#endif

#if !defined(RWPRECONDITION2)
#  define RWPRECONDITION2(a,b)  RW_PRECONDITION2(a,b)
#endif

#if !defined(RWPOSTCONDITION2)
#  define RWPOSTCONDITION2(a,b) RW_POSTCONDITION2(a,b)
#endif

#ifndef PRECONDITION
// The following group of lines can be removed if they cause naming conflicts:
#  define PRECONDITION(a)  RW_PRECONDITION(a)
#  define POSTCONDITION(a) RW_POSTCONDITION(a)
#endif

#if defined(BOUNDS_CHECK) && !defined(RWBOUNDS_CHECK)
#  define RWBOUNDS_CHECK  1
#endif

#if defined(RW_AVOID_PREPROCESSOR_PROBLEMS) && !defined(RW_DISABLE_DEPRECATED)
#  define RW_DISABLE_DEPRECATED
#elif defined(RW_DISABLE_DEPRECATED) && !defined(RW_AVOID_PREPROCESSOR_PROBLEMS)
#  define RW_AVOID_PREPROCESSOR_PROBLEMS
#endif

// Check for hardcoding when building on top of RW SL2
#if defined(RW_STDLIB_VER) && RW_STDLIB_VER >= 0x02000000
#  include <iosfwd>
#  if defined(_RWSTD_NO_NEW_HEADER) && !defined(RW_NO_NEW_HEADER)
#    define RW_NO_NEW_HEADER 1
#  endif
#  if defined(_RWSTD_NO_LIBC_IN_STD) && !defined(RW_NO_LIBC_IN_STD)
#    define RW_NO_LIBC_IN_STD 1
#  endif
#endif // RW_STDLIB_VER && RW_STDLIB_VER >= 0x02000000

#if !defined(RW_NO_LONG_LONG)
// do not use this macro. it only exists for compatibility.
#  define RW_TLS_LONGLONG  1
#endif // !RW_NO_LONG_LONG

// Various defs for old iostreams
#if defined(RW_USE_OLD_IOSTREAMS)
#  undef RW_TLS_LONGLONG
#  define RW_NO_IOSTD                  1
#  define RW_CLASSIC_STRING_CLASSES    1
#  define RW_NO_IO_SENTRY              1
#  define RW_NO_LONG_DOUBLE            1
#endif

// Macros to handle the varying need for the std:: prefix
#if !defined(RW_NO_IOSTD)
#  define RW_SL_IOSTATE          std::ios
#  define RW_SL_IOS_FLAGS(x)    (std::ios::iostate(x))
#  define RW_SL_IO_STD(x)        std::x
#else
#  define RW_SL_IOSTATE          ios
#  define RW_SL_IOS_FLAGS(x)    (x)
#  define RW_SL_IO_STD(x)        x
#endif

#define RW_SL_STD(x)             std::x
#define RW_SL_STD_GLOBAL(x)      std::x

#if !defined(RW_NO_IOSTD)
#  define RW_STREAMBUF_INT_TYPE RW_SL_IO_STD(streambuf)::int_type
#  define RW_STREAMBUF_EOF      RW_SL_IO_STD(streambuf)::traits_type::eof()

#  define RW_STREAM_IN_SENTRY_CTOR(s) \
    RW_SL_STD(istream)::sentry ipfx(s); if (ipfx)
#  define RW_STREAM_IN_SENTRY_DTOR(ignore)
#else
#  define RW_STREAMBUF_INT_TYPE         int
#  define RW_STREAMBUF_EOF              EOF

#  define RW_STREAM_IN_SENTRY_CTOR(s)   if (s.ipfx())
#  define RW_STREAM_IN_SENTRY_DTOR(s)   s.isfx ()
#endif // RW_NO_STREAMBUF_INT_TYPE

// Configure headers names to replace the ifdef's scattered all
// over the place; the conditionals are used in approx. 45 tests.
#if defined(RW_NO_IOSTD)
#  define RW_IOSTREAM_H  <iostream.h>
#  define RW_FSTREAM_H   <fstream.h>
#else
#  define RW_IOSTREAM_H  <iostream>
#  define RW_FSTREAM_H   <fstream>
#endif // RW_NO_IOSTD

#define RW_TYPENAME              typename
#define RW_OUTSIDE_DECL_TYPENAME typename

// Macro for forming or omitting default template arguments in constructors
#define RW_DEFAULT_ARG(n) = n

// Macros for allocator support
#define RW_ALLOCATOR_TEMPLATE_DECLARATION        ,class A
#define RW_ALLOCATOR_FWD_TEMPLATE_DECL(type)     ,class A
#define RW_ALLOCATOR_PARAMETER(type)             ,class A = RW_SL_STD(allocator)< type >
#define RW_REQUIRED_ALLOCATOR_TYPE(type)         ,A
#define RW_ALLOCATOR_TYPE                        ,A

#define RW_DEFAULT_ALLOCATOR_ARGUMENT(X)       = RW_SL_STD(allocator)< X >
#define RW_STD_ALLOCATOR(X)                      RW_SL_STD(allocator)< X >

/**************************** From comp tests *****************************/

// Macros that guarded in the inclusion of C standard library header files.
// These guards are no longer required.
#define STARTWRAP
#define ENDWRAP

#if !defined(RW_DEFAULT_PRECISION) || defined(DOXYGEN)
/**
 * @relates RWpostream
 *
 * Default decimal digits of precision.
 */
#  define RW_DEFAULT_PRECISION  DBL_DIG
#endif // RW_DEFAULT_PRECISION

#endif // RW_TOOLS_COMPAT_H
