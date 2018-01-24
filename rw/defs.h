#ifndef RW_TOOLS_DEFS_H
#define RW_TOOLS_DEFS_H

/**********************************************************************
 *
 * Common definitions
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/defs.h#7 $
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

// Address the behavior described in PR#26255
#if defined(__SunOS_5_8) || defined(__SunOS_5_9)
#  include <unistd.h>
#endif   // __SunOS_5_{8,9}

#include <stddef.h>
#include <assert.h>

// Compiler header - hardcoded, compiler-dependent macros
#include <rw/compiler.h>

// Typedefs header
#include <rw/typedefs.h>

// Tools-specific limits header
#include <rw/limits.h>

// Macros related to exporting symbols
// The following macros are not to be used
// directly only use within other def and
// pkgdefs files.
#if defined(_WIN32)
#  define RW_COMPILER_GLOBAL_EXPORT     __declspec(dllexport)
#  define RW_COMPILER_SYMBOLIC_EXPORT   __declspec(dllexport)
#  define RW_COMPILER_GLOBAL_IMPORT     __declspec(dllimport)
#elif defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x570
// __global: used to give global objects, polymorphic classes and classes with
// public static member variable to have global visibility and dynamic binding
// when building the library. Also used to bind to symbols declared __symbolic
// when using the library
// __symbolic: used when building the library to give non-polymorphic
// classes (classes without virtual methods) and functions global
// visibility and link-time binding
#  define RW_COMPILER_GLOBAL_EXPORT     __global
#  define RW_COMPILER_SYMBOLIC_EXPORT   __symbolic
#  define RW_COMPILER_GLOBAL_IMPORT     __global
#else
#  define RW_COMPILER_GLOBAL_EXPORT
#  define RW_COMPILER_SYMBOLIC_EXPORT
#  define RW_COMPILER_GLOBAL_IMPORT
#endif

#include <rw/rwwind.h>

// Define compatibility macro
#define RWTOOLS    RW_TOOLS_VERSION


////////////////////////////////////////////////////////////////////////////
//
// Types, forward declarations
//
////////////////////////////////////////////////////////////////////////////

// Type and value used in math (TODO - check move)
/**
 * @ingroup tools_classes
 *
 * #RWUninitialized is an enumeration type with one value,
 * #rwUninitialized. It is used in various places in SourcePro
 * products to indicate an uninitialized value.
 */
enum RWUninitialized {

    /**
     * uninitialized value
     */
    rwUninitialized
};

// Forward declarations (TODO - check move)
#include <iosfwd>

////////////////////////////////////////////////////////////////////////////
//
// Utilities
//
////////////////////////////////////////////////////////////////////////////

// Hardcoded macro traded for a hardcoded comp test
#if !defined(RW_NO_XMSG)
#  define RW_NO_XMSG
#endif // !RW_NO_XMSG

// Convenience macros to avoid unused warnings
#undef  RW_UNUSED
#define RW_UNUSED(arg) ((void)&arg)

#if !defined(RW_UNUSED_ATTR)
#  define RW_UNUSED_ATTR
#endif

// Various concatenation macros
#define RW_DO_PASTE2(a,b)       a ## b
#define RW_PASTE2(a,b)          RW_DO_PASTE2(a,b)

#define RW_DO_PASTE3(a,b,c)     a ## b ## c
#define RW_PASTE3(a,b,c)        RW_DO_PASTE3(a,b,c)

#define RW_DO_PASTE4(a,b,c,d)   a ## b ## c ## d
#define RW_PASTE4(a,b,c,d)      RW_DO_PASTE4(a,b,c,d)

#define RW_PASTE(a, b)          RW_PASTE2 (a, b)

#define RW_STRINGIFY(x)         #x

#define RW_DECLARE_GENERIC_CLASS(name,type)   RW_PASTE2(name, declare) (type)
#define RW_DEFINE_GENERIC_CLASS(name,type)    RW_PASTE2(name, implement) (type)

#if defined(RW_MULTI_THREAD)
// Synchronizes access by all objects holding the same mutex
#  define RW_MT_GUARD(mutex)                    \
          RWGuard RW_PASTE (__guard, __LINE__) (mutex)

#  define RW_MT_LOCK_GUARD(type, mutex)         \
          RWTMutexGuard<type>                   \
          RW_PASTE (__guard, __LINE__) (mutex)
#else
// Synchronizes access by all objects holding the same mutex
#  define RW_MT_GUARD(ignore)                 ((void)0)
#  define RW_MT_LOCK_GUARD(ignore0, ignore1)  ((void)0)
#endif

#undef RW_PRECONDITION
#undef RW_POSTCONDITION
#undef RW_PRECONDITION2
#undef RW_POSTCONDITION2
#undef RW_ASSERT

#if defined(RW_DEBUG)
#  define RW_PRECONDITION(a)     assert( !!(a) )
#  define RW_POSTCONDITION(a)    assert( !!(a) )

#  ifdef RW_NO_NESTED_QUOTES
#    define RW_PRECONDITION2(a,b)  assert( !!(a) )
#    define RW_POSTCONDITION2(a,b) assert( !!(a) )
#  else
#    define RW_PRECONDITION2(a,b)  assert(((void)b, !!(a)))
#    define RW_POSTCONDITION2(a,b) assert(((void)b, !!(a)))
#  endif

#  define RW_ASSERT(a)         assert( !!(a) )
#else
#  define RW_PRECONDITION(a)
#  define RW_POSTCONDITION(a)
#  define RW_PRECONDITION2(a,b)
#  define RW_POSTCONDITION2(a,b)
#  define RW_ASSERT(a)
#endif

#undef RW_BYTE_SWAP_16
#undef RW_BYTE_SWAP_32
#undef RW_BYTE_SWAP_64

#define RW_BYTE_SWAP_16(x) ((rwuint16(rwuint16(x) & 0xFF00U) >> 8) |     \
                            (rwuint16(rwuint16(x) & 0x00FFU) << 8))

#define RW_BYTE_SWAP_32(x) (((rwuint32(x) & 0xFF000000U) >> 24) |        \
                            ((rwuint32(x) & 0x00FF0000U) >>  8) |        \
                            ((rwuint32(x) & 0x0000FF00U) <<  8) |        \
                            ((rwuint32(x) & 0x000000FFU) << 24))

#define RW_BYTE_SWAP_64(x)                              \
    (((rwuint64(x) & 0xFF00000000000000ULL) >> 56) |    \
     ((rwuint64(x) & 0x00FF000000000000ULL) >> 40) |    \
     ((rwuint64(x) & 0x0000FF0000000000ULL) >> 24) |    \
     ((rwuint64(x) & 0x000000FF00000000ULL) >>  8) |    \
     ((rwuint64(x) & 0x00000000FF000000ULL) <<  8) |    \
     ((rwuint64(x) & 0x0000000000FF0000ULL) << 24) |    \
     ((rwuint64(x) & 0x000000000000FF00ULL) << 40) |    \
     ((rwuint64(x) & 0x00000000000000FFULL) << 56))

////////////////////////////////////////////////////////////////////////////
//
// Tools configuration
//
////////////////////////////////////////////////////////////////////////////

/*
 * RW_DECLARE_HOME is used at the start of some classes to declare a
 * non-inline virtual function where there would otherwise be none.  This
 * allows many compilers to establish a "home" for the virtual function
 * table.  Otherwise, a virtual function table is built in every
 * compilation unit which includes the class declaration.  RW_DEFINE_HOME
 * is used to generate the implementation of the function.
 *
 * The purpose of using macros is to hide the fact that these are
 * functions, since they are of no functional use.  Also, it
 * automatically includes the name of the class in the function name
 * which resolves name conflicts when a class inherits from two classes, both
 * with RW_DECLARE_HOME in them. (i.e RWXDRistream)
 */

#define RW_DECLARE_HOME(classname)  \
    virtual void RW_PASTE2(__RWhome,classname)();

#define RW_DEFINE_HOME(classname)   \
    void classname::RW_PASTE2(__RWhome,classname)() {}

////////////////////////////////////////////////////////////////////////////
//
// Language, typedefs
//
////////////////////////////////////////////////////////////////////////////

#if !defined(RW_NO_NAMESPACE)
#  define RW_NAMESPACE(x)    namespace x
#  define RW_ANON_NAMESPACE  namespace
#else
#  define RW_NAMESPACE(x)    extern "C++"
#  define RW_ANON_NAMESPACE  extern "C++"
#endif

#if !defined(RW_NO_STATIC_CONST_MEMBER_INIT)
#  define RW_STATIC_CONST(type, init)         static const type init
#  define RW_DEFINE_STATIC_CONST(type, init)  const type
#else
#  define RW_STATIC_CONST(type, init)         static const type
#  define RW_DEFINE_STATIC_CONST(type, init)  const type init
#endif   // _RWSTD_NO_STATIC_CONST_MEMBER_INIT

#if !defined(RW_NO_STATIC_CONST_MEMBER_INIT)
#  define RW_STATIC_CONST_INT_RANGE(type, init)  static const type init
#else
#  define RW_STATIC_CONST_INT_RANGE(type, init)  enum { init }
#endif

#ifndef RW_NO_ANSI_CASTS
#  define RW_REINTERPRET_CAST(type,pointer) reinterpret_cast< type >(pointer)
#  define RW_STATIC_CAST(type,pointer) static_cast< type >(pointer)
#  define RW_CONST_CAST(type,pointer) const_cast< type >(pointer)
#else
#  define RW_REINTERPRET_CAST(type,pointer) (type)pointer
#  define RW_STATIC_CAST(type,pointer) (type)pointer
#  define RW_CONST_CAST(type,pointer) (type)pointer
#endif /* RW_NO_ANSI_CASTS */

#if !defined(TRUE)
#  define  TRUE  true
#  define  FALSE false
#endif

////////////////////////////////////////////////////////////////////////////
//
// Library(s) configuration
//
////////////////////////////////////////////////////////////////////////////

#if !defined(RW_NO_LONGLONG_OVERLOAD)
#  if defined(_WIN32)
#    define RW_LONG_LONG_PRINTF_PREFIX "I64"
#  else
#    define RW_LONG_LONG_PRINTF_PREFIX "ll"
#  endif
#endif

// Use compatibility mode for decimal parser
#if !defined(RW_NO_NEW_DECIMAL_PARSER)
#  define RW_NO_NEW_DECIMAL_PARSER
#endif // RW_NO_NEW_DECIMAL_PARSER

#if defined(RW_NO_IMPLICIT_INCLUSION)
#  define RW_CC_INLINE inline
#else
#  define RW_CC_INLINE
#endif

////////////////////////////////////////////////////////////////////////////
//
// Constants
//
////////////////////////////////////////////////////////////////////////////

#ifndef   RW_INITIAL_BUFFER_SIZE
#  define RW_INITIAL_BUFFER_SIZE  512
#endif // RW_INITIAL_BUFFER_SIZE

#ifndef   RW_MAX_BUFFER_SIZE
#  define RW_MAX_BUFFER_SIZE  size_t(~0)
#endif // RW_MAX_BUFFER_SIZE

#ifndef RW_STORE_TABLE_DEFAULT_SIZE
#  define RW_STORE_TABLE_DEFAULT_SIZE      1024
#endif // RW_STORE_TABLE_DEFAULT_SIZE

#define RW_STRING_DEFAULT_INITIAL_CAPACITY 15
#define RW_STRING_DEFAULT_MAXIMUM_WASTE    15
#define RW_STRING_DEFAULT_RESIZE_INCREMENT 16

#define RW_MESSAGE_SET_NUMBER              1

/**
 * @ingroup tools_classes
 *
 * A sentinel value.
 */
#define rwnil    0

/**
 * @ingroup tools_classes
 *
 * A sentinel value.
 */
#define RWNIL   -1L

// Various numeric macros used in containers
#define RW_SUCCESS                    0
#define RW_DEFAULT_CAPACITY          64 /* Default collection class capacity */
#define RW_DEFAULT_RESIZE            64 /* Default collection class resize */
#define RW_BYTE_SIZE                  8 /* Number of bits per byte */

/**
 * @ingroup tools_classes
 *
 * A sentinel value.
 */
#define RW_NPOS            (~(size_t)0)

/*
 * Class ID definitions for Core.h++ classes:
 */
#define __GLOBAL                        0xf000
#define __RWBISTREAM                    0xf001
#define __RWBOSTREAM                    0xf002
#define __RWCLIPSTREAMBUF               0xf003
#define __RWDDESTREAMBUF                0xf004
#define __RWERROBJECT                   0xf005
#define __RWFIXEDMEMORY                 0xf006
#define __RWPISTREAM                    0xf007
#define __RWPOSTREAM                    0xf008
#define __RWVISTREAM                    0xf009
#define __RWVOSTREAM                    0xf00a

/* For historical reasons: */
#define __RWFILE                        0x8020

// Safety check on binary files.
const long MAGIC_CONSTANT = 0x8888;

/*
 * Class ID definitions:
 */

#define __RWUNKNOWN                     0x8000
#define __RWGDLIST                      0x8014
#define __RWGDLISTITERATOR              0x8015
#define __RWGQUEUE                      0x8016
#define __RWGSTACK                      0x8017
#define __RWGVECTOR                     0x8018
#define __RWBAG                         0x801b
#define __RWBAGITERATOR                 0x801c
#define __RWBINARYTREE                  0x8001
#define __RWBINARYTREEITERATOR          0x8002
#define __RWBTREE                       0x8003
#define __RWBTREEDICTIONARY             0x8004
#define __RWBTREENODE                   0x8005
#define __RWBTREEONDISK                 0x8006
#define __RWCOLLECTABLE                 0x8007
#define __RWCOLLECTABLEASSOCIATION      0x8008
#define __RWCOLLECTABLEDATE             0x8009
#define __RWCOLLECTABLEINT              0x800a
#define __RWCOLLECTABLESTRING           0x800b

#if !defined(RW_DISABLE_DEPRECATED)
#  define __RWCOLLECTABLETIME           0x800c
#endif

#define __RWCOLLECTION                  0x801d
#define __RWDATE                        0x801e
#define __RWDISKTREENODE                0x800d
#define __RWDLINK                       0x800e
#define __RWDLIST                       0x800f
#define __RWDLISTCOLLECTABLES           0x8010
#define __RWDLISTCOLLECTABLESITERATOR   0x8011
#define __RWDLISTITERATOR               0x8012
#define __RWFACTORY                     0x801f
#define __RWFILEMANAGER                 0x8013
#define __RWFREELISTDATA                0x8021
#define __RWHASHDICTIONARY              0x8022
#define __RWHASHDICTIONARYITERATOR      0x8023
#define __RWIDENTITYDICTIONARY          0x8024
#define __RWIDENTITYSET                 0x8025
#define __RWINTEGER                     0x8026
#define __RWITERATOR                    0x8027
#define __RWORDERED                     0x8028
#define __RWORDEREDITERATOR             0x8029
#define __RWSEQUENCEABLE                0x802a
#define __RWSET                         0x802b
#define __RWSETITERATOR                 0x802c
#define __RWSLINK                       0x8031
#define __RWSLIST                       0x8032
#define __RWSLISTCOLLECTABLES           0x8033
#define __RWSLISTCOLLECTABLESITERATOR   0x8034
#define __RWSLISTCOLLECTABLESQUEUE      0x8035
#define __RWSLISTCOLLECTABLESSTACK      0x8036
#define __RWSLISTITERATOR               0x8037
#define __RWSTRINGREF                   0x802e
#define __RWTIME                        0x8030
#define __RWTREENODE                    0x8038
//#define __RWSTRING                      0x802d
//#define __RWSUBSTRING                   0x802f

// --- V4.0 new objects ---
#define __GBITVEC                       0x8040
#define __GSLIST                        0x8041
#define __GSLISTITERATOR                0x8042
#define __GSORTEDVECTOR                 0x8043
#define __RWBITREF                      0x8045
#define __RWBITVEC                      0x8046
#define __RWCACHEMANAGER                0x8048
#define __RWREGEXP                      0x8049
#define __RWSORTEDVECTOR                0x804a
#define __RWTOKENIZER                   0x804b

// --- V4.1 new objects ---
#define __RWDLLDICTIONARY               0x804c
#define __RWDLLSET                      0x804d
#define __RWFACTORYMANAGER              0x804e
#define __RWINSTANCEDATA                0x804f
#define __RWINSTANCEDATAMANAGER         0x8050
#define __RWINSTANCEMANAGER             0x8051
#define __RWMODEL                       0x8052
#define __RWPROCESS                     0x8053
#define __RWPROCESSMANAGER              0x8054
#define __RWREADMANAGER                 0x8055
#define __RWREADTABLE                   0x8056
#define __RWSTOREMANAGER                0x8057
#define __RWSTORETABLE                  0x8058
#define __RWTHREAD                      0x8059

// --- V5.0 new objects ---
#define __RWCSTRING                     0x802d  /* Recycled from RWString */
#define __RWCSUBSTRING                  0x802f  /* Recycled from RWCSubString */
#define __RWBUFFER                      0x805a
#define __RWTEMPLATE                    0x805b
#define __RWVIRTUALREF                  0x805c
#define __RWDISKPAGEHEAP                0x805d
#define __RWHASHTABLE                   0x805e
#define __RWCOLLECTABLEIDASSOCIATION    0x805f

// --- V6.0 new objects ---
#define __RWCREGEXP                     0x8049  /* Recycled from RWRegexp */
#define __RWWSTRING                     0x8060
#define __RWWSUBSTRING                  0x8061
#define __RWCLASSIDASSOCIATION          0x8062
#define __RWSTRINGIDASSOCIATION         0x8063

// --- V7.0.x new IDs
#define __RWSTOREENTRY                  0x8064
#define __RWSTORESTRINGID               0x8065

// --- V7.5.3 new IDs
#define __RWCOLLECTABLEWSTRING          0x8066

// --- V7.6.1 new IDs
#define __RWCOLLECTABLEDATETIME         0x8067


////////////////////////////////////////////////////////////////////////////
//
// Miscellaneous declarations and typedefs
//
////////////////////////////////////////////////////////////////////////////

// Forward declarations (TODO - check move all)
class RW_TOOLS_GLOBAL RWvistream;
class RW_TOOLS_GLOBAL RWvostream;
class RW_TOOLS_GLOBAL RWFile;

struct RWMutex;

class RW_TOOLS_GLOBAL RWCollectable;

typedef RWCollectable*  RWCollectableP;

/**
 * @relates RWFactory
 *
 * A global function used to allocate a RWCollectable derived class for
 * RWFactory.  A creator function has prototype:
 *
 * @code
 * RWCollectable* aCreatorFunction();
 * @endcode
 */
typedef RWCollectable*  (*RWuserCreator)();

typedef void (*RWCollectableSaveToRWFile)(RWFile& file, const RWCollectable* col);
typedef void (*RWCollectableSaveToRWvostream)(RWvostream& vstrm, const RWCollectable* col);

typedef void (*RWCollectableRestoreFromRWFile)(RWFile& file, RWCollectable* col);
typedef void (*RWCollectableRestoreFromRWvistream)(RWvistream& vstrm, RWCollectable* col);

typedef int (*RWcompare)(const void*, const void*);
typedef bool (*RWtestGeneric)(const void*, const void*);

/**
 * @relates RWCollection
 *
 * A global function used by the select() method in RWCollection classes to test
 * an RWCollectable object for a user defined property.
 * A testCollectable function has prototype:
 *
 * @code
 * bool RWtestCollectable(const RWCollectable*, const void*);
 * @endcode
 */
typedef bool (*RWtestCollectable)(const RWCollectable*, const void*);

/**
 * @relates RWCollection
 *
 * A global function used by the select() method in RWCollection classes to test
 * a pair of RWCollectable objects for a user defined property.
 * A testCollectablePair function has prototype:
 *
 * @code
 * bool RWtestCollectablePair(const RWCollectable*, const RWCollectable*, const void*);
 * @endcode
 */
typedef bool (*RWtestCollectablePair)(const RWCollectable*, const RWCollectable*, const void*);

typedef void (*RWapplyGeneric)(void*, void*);

/**
 * @relates RWCollection
 *
 * A global function used by the apply() method in RWCollection classes to apply
 * a user defined action to an RWCollectable object.
 * A applyCollectable function has prototype:
 *
 * @code
 * void RWapplyCollectable(RWCollectable*, void*);
 * @endcode
 */
typedef void (*RWapplyCollectable)(RWCollectable*, void*);

// Compatibility header containing definitions of
// old/obsoleted macros - keep at the end of file.
#include <rw/compat.h>

// Include the utility header - various helpful declarations.
#include <rw/utility.h>

// define noexcept
#if !defined(RW_NO_NOEXCEPT)
#  define RW_NOEXCEPT noexcept
#else
#  define RW_NOEXCEPT
#endif

// define throw spec for functions that do not throw
#if !defined(RW_NO_NOEXCEPT)
#  define RW_NO_THROW noexcept
#else
#  define RW_NO_THROW throw()
#endif

// define throw spec to silence warnings
#if defined(_MSC_VER) && ((_MSC_VER == 1700) || (defined(__INTEL_COMPILER) && ((__INTEL_COMPILER >= 1400) && (__INTEL_COMPILER <= 1500))))
#  define RW_STREAMBUF_DTOR_THROWSPEC throw()
#else
#  define RW_STREAMBUF_DTOR_THROWSPEC
#endif

// Use secure variant of c functions if available
#if defined(RW_DEBUG) && !defined(RW_SECURE_CHECK)
#  define RW_SECURE_CHECK
#endif

#endif /* RW_TOOLS_DEFS_H */
