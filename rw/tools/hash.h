#ifndef rw_tools_hash_h_
#define rw_tools_hash_h_
/**********************************************************************
 *
 * $Id: //tools/13/rw/tools/hash.h#1 $
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
#include <rw/typedefs.h>
#include <rw/tools/traits/RWTConditional.h>

#include <functional>

#if !defined(_MSC_VER)
#  include <stdint.h>
#endif

#if (RW_SIZEOF_SIZE_T < 8)

inline size_t rwHash(rwuint32 key)
{
    // Thomas Wang's 32-bit integer hash:
    //     http://www.concentric.net/~ttwang/tech/inthash.htm
    const rwuint32 c2 = 0x27d4eb2d; // a prime or an odd constant
    key = (key ^ 61) ^ (key >> 16);
    key = key + (key << 3);
    key = key ^ (key >> 4);
    key = key * c2;
    key = key ^ (key >> 15);
    return key;
}

inline size_t rwHash(rwuint64 key)
{
    // Thomas Wang's 64-bit to 32-bit integer hash:
    //     http://www.concentric.net/~ttwang/tech/inthash.htm
    key = (~key) + (key << 18); // key = (key << 18) - key - 1;
    key = key ^ (key >> 31);
    key = key * 21; // key = (key + (key << 2)) + (key << 4);
    key = key ^ (key >> 11);
    key = key + (key << 6);
    key = key ^ (key >> 22);
    return (rwuint32) key;
}

#else

inline size_t rwHash(rwuint64 key)
{
    // Thomas Wang's 64-bit integer hash:
    //     http://www.concentric.net/~ttwang/tech/inthash.htm
    key = (~key) + (key << 21); // key = (key << 21) - key - 1;
    key = key ^ (key >> 24);
    key = (key + (key << 3)) + (key << 8); // key * 265
    key = key ^ (key >> 14);
    key = (key + (key << 2)) + (key << 4); // key * 21
    key = key ^ (key >> 28);
    key = key + (key << 31);
    return (rwuint64)key;
}

#endif

#if defined(DOXYGEN)
/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Hash function object.
 *
 * Provides a C++ Standard Library compliant hash object suitable for use with
 * hash or unordered containers. The primary template definition is not
 * defined. Specializations of this class are expected to:
 *
 * - Conform to the interface of \c std::unary_function<T, size_t>.
 *   Specifically they must provide a typedef of \c T to \c argument_type
 *   and of \c size_t to \c result_type.
 * - Define <tt>operator() const</tt> such that it takes a single argument of
 *   type \c argument_type and returns \c result_type.
 * - Be copy constructible and destructible.
 */
template <typename T>
struct RWTHash {};

#else

template <typename T>
struct RWTHash;

#endif

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing a \c bool.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<bool> : public std::unary_function<bool, size_t> {
    /**
     * Returns a hashed value of \a obj.
     */
    size_t operator()(bool obj) const {
        typedef RWTConditional < (sizeof(bool) < 8), rwuint32, rwuint64 >::type type;
        return rwHash((type)obj);
    }
};

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing a \c char.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<char> : public std::unary_function<char, size_t> {
    /**
     * Returns a hashed value of \a obj.
     */
    size_t operator()(char obj) const {
        typedef RWTConditional < (sizeof(char) < 8), rwuint32, rwuint64 >::type type;
        return rwHash((type)obj);
    }
};

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing a <tt>signed char</tt>.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<signed char> : public std::unary_function<signed char, size_t> {
    /**
     * Returns a hashed value of \a obj.
     */
    size_t operator()(signed char obj) const {
        typedef RWTConditional < (sizeof(signed char) < 8), rwuint32, rwuint64 >::type type;
        return rwHash((type)obj);
    }
};

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an <tt>unsigned char</tt>.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<unsigned char> : public std::unary_function<unsigned char, size_t> {
    /**
     * Returns a hashed value of \a obj.
     */
    size_t operator()(unsigned char obj) const {
        typedef RWTConditional < (sizeof(unsigned char) < 8), rwuint32, rwuint64 >::type type;
        return rwHash((type)obj);
    }
};

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing a \c wchar_t.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<wchar_t> : public std::unary_function<wchar_t, size_t> {
    /**
     * Returns a hashed value of \a obj.
     */
    size_t operator()(wchar_t obj) const {
        typedef RWTConditional < (sizeof(wchar_t) < 8), rwuint32, rwuint64 >::type type;
        return rwHash((type)obj);
    }
};

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing a \c short.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<short> : public std::unary_function<short, size_t> {
    /**
     * Returns a hashed value of \a obj.
     */
    size_t operator()(short obj) const {
        typedef RWTConditional < (sizeof(short) < 8), rwuint32, rwuint64 >::type type;
        return rwHash((type)obj);
    }
};

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an <tt>unsigned short</tt>.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<unsigned short> : public std::unary_function <unsigned short, size_t> {
    /**
     * Returns a hashed value of \a obj.
     */
    size_t operator()(unsigned short obj) const {
        typedef RWTConditional < (sizeof(unsigned short) < 8), rwuint32, rwuint64 >::type type;
        return rwHash((type)obj);
    }
};

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an \c int.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<int> : public std::unary_function<int, size_t> {
    /**
     * Returns a hashed value of \a obj.
     */
    size_t operator()(int obj) const {
        typedef RWTConditional < (sizeof(int) < 8), rwuint32, rwuint64 >::type type;
        return rwHash((type)obj);
    }
};

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an <tt>unsigned int</tt>.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<unsigned int> : public std::unary_function<unsigned int, size_t> {
    /**
     * Returns a hashed value of \a obj.
     */
    size_t operator()(unsigned int obj) const {
        typedef RWTConditional < (sizeof(unsigned int) < 8), rwuint32, rwuint64 >::type type;
        return rwHash((type)obj);
    }
};

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing a \c long.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<long> : public std::unary_function<long, size_t> {
    /**
     * Returns a hashed value of \a obj.
     */
    size_t operator()(long obj) const {
        typedef RWTConditional < (sizeof(long) < 8), rwuint32, rwuint64 >::type type;
        return rwHash((type)obj);
    }
};

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an <tt>unsigned long</tt>.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<unsigned long> : public std::unary_function<unsigned long, size_t> {
    /**
     * Returns a hashed value of \a obj.
     */
    size_t operator()(unsigned long obj) const {
        typedef RWTConditional < (sizeof(unsigned long) < 8), rwuint32, rwuint64 >::type type;
        return rwHash((type)obj);
    }
};

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing a <tt>long long</tt>.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<long long> : public std::unary_function<long long, size_t> {
    /**
     * Returns a hashed value of \a obj.
     */
    size_t operator()(long long obj) const {
        typedef RWTConditional < (sizeof(long long) < 8), rwuint32, rwuint64 >::type type;
        return rwHash((type)obj);
    }
};

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an <tt>unsigned long long</tt>.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<unsigned long long> : public std::unary_function<unsigned long long, size_t> {
    /**
     * Returns a hashed value of \a obj.
     */
    size_t operator()(unsigned long long obj) const {
        typedef RWTConditional < (sizeof(unsigned long long) < 8), rwuint32, rwuint64 >::type type;
        return rwHash((type)obj);
    }
};

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing a \c float.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<float> : public std::unary_function<float, size_t> {
    /**
     * Returns a hashed value of \a obj.
     */
    size_t operator()(float obj) const {
        // Temporary required to avoid type-punning warnings with
        // GCC 4.1.2
        char* tmp = reinterpret_cast<char*>(&obj);
        return rwHash(*reinterpret_cast<rwuint32*>(tmp));
    }
};

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing a \c double.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<double> : public std::unary_function<double, size_t> {
    /**
     * Returns a hashed value of \a obj.
     */
    size_t operator()(double obj) const {
        // Temporary required to avoid type-punning warnings with
        // GCC 4.1.2
        char* tmp = reinterpret_cast<char*>(&obj);
        return rwHash(*reinterpret_cast<rwuint64*>(tmp));
    }
};

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing a <tt>long double</tt>.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<long double> : public std::unary_function<long double, size_t> {
    /**
     * Returns a hashed value of \a obj.
     */
    size_t operator()(long double obj) const {
        double d = (double)obj;
        // Temporary required to avoid type-punning warnings with
        // GCC 4.1.2
        char* tmp = reinterpret_cast<char*>(&d);
        return rwHash(*reinterpret_cast<rwuint64*>(tmp));
    }
};

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing a pointer type.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <typename T>
struct RWTHash<T*> : public std::unary_function<T*, size_t> {
    /**
     * Returns a hashed value of \a obj.
     */
    size_t operator()(T* obj) const {
        typedef typename RWTConditional < (sizeof(T*) < 8), rwuint32, rwuint64 >::type type;
        return rwHash((type)(uintptr_t)obj);
    }
};

#endif
