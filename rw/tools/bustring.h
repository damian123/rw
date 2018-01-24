
/**********************************************************************
 *
 * This file contains the #RWBasicUString class declaration.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/bustring.h#1 $
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

#ifndef rw_tools_bustring_h
#define rw_tools_bustring_h

#include <rw/defs.h>
#include <rw/edefs.h> // for rw_swap

#include <rw/tools/uchar.h>
#include <rw/tools/hash.h>
#include <rw/tools/atomic.h>
#include <rw/tools/iterator.h> // for rw_iterator_traits

#include <rw/rwfile.h>
#include <rw/cstring.h>
#include <rw/rwerr.h>
#include <rw/wstring.h>
#include <rw/once.h>

#include <stdlib.h> // required to define free() with proper C++ or C linkage

#include <string> // required for RWClassicCString builds
#include <iterator>

/**
 * @ingroup string_processing_classes
 *
 * @brief Represents and manages an array of UTF-16 values.
 *
 * RWBasicUString is used to manage an array of UTF-16 values in the form
 * of an array of #RWUChar16 values. An instance of RWBasicUString may be
 * constructed to use one of three memory management models:
 *
 * -  The initial contents are owned and managed by the client, and the
 *    instance assumes no responsibility for deallocating the array.
 *    This model is primarily used to treat a static or other long
 *    storage-duration array as an RWBasicUString instance. The client
 *    array may be treated as read-only storage, or as a writable buffer.
 * -  The array is allocated by the client, but the RWBasicUString
 *    instance is given the responsibility for deallocating the array.
 * -  The source array is copied by the RWBasicUString instance. The
 *    client retains ownership of the source array. The instance owns
 *    and manages its own internal copy.
 *
 * When deallocation responsibility is passed from the client to
 * an RWBasicUString, the client must supply a function object,
 * or functor, that can be used to deallocate the storage. This
 * mechanism allows the client to choose to use
 * <tt>operator delete[]</tt>, \c free(), or custom memory-management
 * mechanisms. The use of an externally supplied deallocation method
 * can also be used to satisfy the heap management requirements of
 * Microsoft Windows dynamically linked libraries, which in some situations
 * may create their own heap in addition to that of the calling process.
 *
 * Given sufficient capacity, RWBasicUString adds a null terminator
 * to any non-static array passed to it. An added null is not considered
 * part of the contents, and is not included in the count returned
 * by length().
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tools/bustring.h>
 * RWBasicUString b;
 *
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWBasicUString
{

public:

    /*
     * The following typedefs are supplied to satisfy Standard C++ container
     * requirements but are not used in the definition of the API since these
     * names are inconsistent with the type names used in related classes.
     */

    /**
     * Declares a conventional C++ Standard Library alias for the type used
     * to represent non-negative values of the distance or difference
     * type. Used for sizes and indices.
     */
    typedef size_t size_type;

    /**
     * Declares a conventional C++ Standard Library alias for the type used
     * to represent the distance between two RWBasicUString iterators.
     */
    typedef ptrdiff_t difference_type;

    /*
     * Declares a conventional C++ Standard Library alias for the type of
     * values stored within an RWBasicUString instance.
     */
    typedef RWUChar16 value_type;

    /**
     * Declares a conventional C++ Standard Library alias for a pointer to
     * the RWBasicUString value type.
     */
    typedef RWUChar16* pointer;

    /**
     * Declares a conventional C++ Standard Library alias for a reference to
     * the RWBasicUString value type.
     */
    typedef RWUChar16& reference;

    /**
     * Declares a conventional C++ Standard Library alias for a \c const
     * reference to the RWBasicUString value type.
     */
    typedef const RWUChar16& const_reference;

    /**
     * A type that provides a random-access iterator over the elements
     * in the container.
     */
    typedef RWUChar16* iterator;

    /**
     * A type that provides a \c const random-access iterator over the
     * elements in the container.
     */
    typedef const RWUChar16* const_iterator;

    /**
     * A type that provides a random-access, reverse-order iterator over
     * the elements in the container.
     *
     * This iterator type is \c const to prohibit modification of the underlying
     * RWBasicUString data.
     */
#if !defined(RW_STDLIB_VER) || !defined(_RWSTD_NO_CLASS_PARTIAL_SPEC)
    typedef std::reverse_iterator<iterator> reverse_iterator;
#else
    typedef std::reverse_iterator < iterator,
            typename rw_iterator_traits<iterator>::iterator_category,
            typename rw_iterator_traits<iterator>::value_type,
            typename rw_iterator_traits<iterator>::reference,
            typename rw_iterator_traits<iterator>::pointer,
            typename rw_iterator_traits<iterator>::difference_type > reverse_iterator;
#endif

    /**
     * A type that provides a \c const random-access, reverse-order iterator
     * over the elements in the container.
     */
#if !defined(RW_STDLIB_VER) || !defined(_RWSTD_NO_CLASS_PARTIAL_SPEC)
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
#else
    typedef std::reverse_iterator < const_iterator,
            typename rw_iterator_traits<const_iterator>::iterator_category,
            typename rw_iterator_traits<const_iterator>::value_type,
            typename rw_iterator_traits<const_iterator>::reference,
            typename rw_iterator_traits<const_iterator>::pointer,
            typename rw_iterator_traits<const_iterator>::difference_type > const_reverse_iterator;
#endif

    /**
     * An enumeration whose values are used to indicate whether the
     * source array provided by a client should be copied or simply
     * referenced.
     */
    enum Duration {

        /**
         * Indicates that a source string or array may have a duration
         * less than that of the RWBasicUString and must therefore
         * be copied,
         */
        Transient,

        /**
         * Indicates that a source string or array will have a duration
         * greater than that of the RWBasicUString and may therefore be
         * simply referenced.
         */
        Persistent
    };

    // Defined out of line
    class RW_TOOLS_GLOBAL Pad;

    // Defined out-of-line.
    class RW_TOOLS_GLOBAL Deallocator;

    // Defined out-of-line.
    class RW_TOOLS_GLOBAL StaticDeallocator;

    /**
     * A static constant representing the Unicode space character.
     */
    RW_STATIC_CONST(RWUChar32 DefaultFillCharacter, = static_cast<RWUChar32>(0x0020));

    // Defined out-of-line.
    class RW_TOOLS_GLOBAL ByteSource;

    /**
     * Uses the C++ <tt>operator delete[]</tt> expression to deallocate an array
     * that was allocated using <tt>operator new</tt>.
     *
     * The method must be defined as an inline method to ensure that
     * the code generated to implement the delete resides in the client
     * library or executable, and not within the Essential Tools library.
     *
     * The code must be compiled this way to avoid the problems that
     * arise when an application contains some libraries that use debug
     * versions of heap management routines while others do not.
     */
    inline static void USE_DELETE(RWUChar16* p);

    /**
     * Calls the C Standard Library \c free() method to deallocate an array
     * that was allocated by a call to \c malloc().
     *
     * The method must be defined as an inline method to ensure that
     * the version of \c free() called by this method is the same version
     * that is used within the client code, and that it manipulates
     * the same heap as that of \c malloc().
     *
     * The code must be compiled this way to avoid the problems that
     * arise when an application contains some libraries that use debug
     * versions of heap management routines while others do not.
     */
    inline static void USE_FREE(RWUChar16* p);

    /**
     * When passed to the constructor of StaticDeallocator, results in
     * a StaticDeallocator instance that does nothing.
     *
     * Use this method when constructing an RWBasicUString instance
     * that has been given access to a client-owned and managed buffer.
     */
    inline static void USE_NONE(RWUChar16* p);

    /**
     * Retrieves a deallocator for use in constructing an RWBasicUString
     * instance that has been given access to a client-owned and managed buffer.
     *
     * @code
     * RWBasicUString buffer[32]; // Client owned array
     * RWBasicUString array(buffer,0,getNullDeallocator());
     * @endcode
     *
     * The Deallocator object is owned by this class and must not be deleted.
     */
    static Deallocator* getNullDeallocator();

    /**
     * Constructs an empty (null) instance.
     */
    inline RWBasicUString();

    /**
     * Constructs an RWBasicUString instance from \a source.
     *
     * The new instance either copies the data stored in \a source,
     * or refers to the same storage as \a source, depending on the
     * storage strategy used to construct \a source.
     */
    RWBasicUString(const RWBasicUString& source);

    /**
     * Constructs an RWBasicUString instance that copies or references
     * the contents of \a source, a null-terminated sequence of #RWUChar16
     * values. The new RWBasicUString instance assumes no responsibility
     * for deallocating the storage associated with \a source.
     *
     * If \a duration is #Transient, this method copies the contents
     * of \a source into an internally allocated and managed array.
     *
     * If \a duration is #Persistent, the client retains responsibility
     * for the storage used for \a source. This mode may be used when
     * \a source resides in static or otherwise durable storage. The
     * storage associated with \a source must not be deallocated while
     * the RWBasicUString instance still references it. The original
     * \a source array cannot be modified by any of the non-\c const
     * methods provided by this class. Any attempt to do so forces
     * RWBasicUString to create a copy of \a source.
     */
    RWBasicUString(const RWUChar16* source, Duration duration = Transient);

    /**
     * Constructs an RWBasicUString instance that copies or references
     * the contents of \a source, an array of #RWUChar16 values that
     * contains \a sourceLength elements and may contain embedded nulls. The
     * new RWBasicUString instance assumes no responsibility for deallocating
     * the storage associated with \a source.
     *
     * If \a duration is #Transient, this method copies the contents
     * of \a source into an internally allocated and managed array.
     *
     * If \a duration is #Persistent, the client retains responsibility
     * for the storage used for \a source. This mode may be used when
     * \a source resides in static or otherwise durable storage. The
     * storage associated with \a source must not be deallocated while
     * the RWBasicUString instance still references it. The original
     * \a source array cannot be modified by any of the non-\c const
     * methods provided by this class. Any attempt to do so forces
     * RWBasicUString to create a copy of \a source.
     */
    RWBasicUString(const RWUChar16* source, size_t sourceLength, Duration duration = Transient);

    /**
     * Constructs an RWBasicUString instance that copies \a sourceLength
     * characters from the array \a source into an internally managed buffer
     * with a minimum capacity of \a initialCapacity. If the original allocation
     * does not possess the capacity required by an append, insert, or replace
     * operation, a new buffer allocation is made to accommodate the change
     * in length.
     *
     * If the value passed for \a sourceLength is #RW_NPOS, the source is
     * assumed to be a null-terminated array and the entire array
     * is copied. Otherwise, \a sourceLength characters are copied.
     */
    RWBasicUString(const RWUChar16* source, size_t sourceLength, size_t initialCapacity);

    /**
     * Constructs an RWBasicUString instance that assumes ownership
     * of \a clientBuffer, a dynamically-allocated, null-terminated
     * sequence of #RWUChar16 values. The terminating null may appear
     * at any position within the storage allocated for the Unicode string.
     *
     * The \a deallocator parameter is used to supply a Deallocator object
     * that can be used to deallocate the storage referenced by \a clientBuffer.
     *
     * Copy construction or assignment produces an RWBasicUString that
     * refers to the same client-supplied buffer.
     *
     * RWBasicUString does not synchronize access to the client-supplied
     * buffer. External synchronization is required if multiple threads
     * will have access to the buffer through one or more RWBasicUString
     * instances.
     */
    inline RWBasicUString(RWUChar16* clientBuffer, Deallocator* deallocator);

    /**
     * Constructs an RWBasicUString instance that assumes ownership
     * of \a clientBuffer, a dynamically-allocated array of #RWUChar16
     * values that contains \a contentLength elements and may contain
     * embedded nulls.
     *
     * The storage required to hold \a contentLength elements may be
     * less than the storage that was allocated for \a clientBuffer.
     *
     * The \a deallocator parameter is used to supply a Deallocator object
     * that can be used to deallocate the storage referenced by \a clientBuffer.
     *
     * Copy construction or assignment produces an RWBasicUString that
     * refers to the same client-supplied buffer.
     *
     * RWBasicUString does not synchronize access to the client-supplied
     * buffer. External synchronization is required if multiple threads
     * will have access to the buffer through one or more RWBasicUString
     * instances.
     */
    inline RWBasicUString(RWUChar16* clientBuffer, size_t contentLength, Deallocator* deallocator);

    /**
     * Constructs an RWBasicUString instance that manipulates \a clientBuffer,
     * a writable, client-supplied array that initially contains \a contentLength
     * elements and whose total usable size is given by \a bufferCapacity.
     *
     * The \a deallocator parameter is used to supply a Deallocator object
     * that can be used to deallocate the storage referenced by \a clientBuffer.
     *
     * The storage associated with \a clientBuffer must not be deallocated
     * while the RWBasicUString instance still references it.
     *
     * If the original \a clientBuffer array does not possess the capacity
     * required by an append, insert, or replace operation, the buffer
     * is copied into an internally allocated buffer.
     *
     * Copy-construction or assignment produces an RWBasicUString that
     * refers to the same client-supplied buffer.
     *
     * RWBasicUString does not synchronize access to the client-supplied
     * buffer. External synchronization is required if multiple threads
     * will have access to the buffer through one or more RWBasicUString
     * instances.
     */
    inline RWBasicUString(RWUChar16* clientBuffer, size_t contentLength, size_t bufferCapacity, Deallocator* deallocator);

    /**
     * Constructs an RWBasicUString from the code units produced by
     * converting the null-terminated UCS-4 or UTF-32 encoded \a source
     * into its equivalent UTF-16 representation.
     */
    RWBasicUString(const RWUChar32* source);

    /**
     * Constructs an RWBasicUString from the code units produced by
     * converting the UCS-4 or UTF-32 encoded \a source into its
     * equivalent UTF-16 representation.
     *
     * The number of elements in \a source is specified using \a sourceLength.
     *
     * The source array may contain embedded nulls.
     */
    RWBasicUString(const RWUChar32* source, size_t sourceLength);

    /**
     * Constructs RWBasicUString containing a zero-length string and
     * a capacity of \a initialCapacity.
     *
     * An RWSize_T instance must be constructed to eliminate potential
     * ambiguity with the RWBasicUString(RWUChar32,size_t) constructor.
     *
     * @code
     * RWBasicUString array(RWSize_T(10));
     * @endcode
     */
    RWBasicUString(const RWSize_T& initialCapacity);

    /**
     * Constructs an RWBasicUString that contains \a repeat copies of
     * the \a codeUnit.
     */
    RWBasicUString(RWUChar16 codeUnit, size_t repeat = 1);

    /**
     * Constructs an RWBasicUString that contains \a repeat copies of
     * the code unit or surrogate pair of code units produced by converting
     * the UTF-32 \a codePoint into its equivalent UTF-16 representation.
     */
    RWBasicUString(RWUChar32 codePoint, size_t repeat = 1);

    /**
     * Constructs an RWBasicUString instance that contains the code
     * units produced by converting the UTF-8 encoded Unicode text in
     * \a utf8Source into a UTF-16 representation.
     *
     * The source string must be null-terminated.
     */
    RWBasicUString(const char* utf8Source);

    /**
     * Constructs an RWBasicUString instance that contains the code
     * units produced by converting the UTF-8 encoded Unicode text in
     * \a utf8Source into a UTF-16 representation.
     *
     * The source string length is given by \a length and may contain
     * embedded nulls.
     */
    RWBasicUString(const char* utf8Source, size_t length);

    /**
     * Constructs an RWBasicUString instance that contains the code
     * units produced by converting the UTF-8 encoded Unicode text in
     * \a utf8Source into a UTF-16 representation.
     */
    RWBasicUString(const RWCString& utf8Source);

#if !defined(RW_NO_RVALUE_REFERENCES)

    /**
     * Move constructor. The constructed instance takes ownership of the
     * data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWBasicUString(RWBasicUString && rhs);

    /**
     * Move assignment. Self takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWBasicUString& operator=(RWBasicUString && rhs);

#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Destroys self. Relinquishes ownership and frees any resources
     * used by self.
     */
    ~RWBasicUString();

    /**
     * Appends the contents of \a source to the contents of self.
     */
    inline RWBasicUString& append(const RWBasicUString& source);

    /**
     * Appends the contents of the specified range in \a source to the
     * contents of self.
     *
     * @throw RWBoundsErr Thrown if the sum of \a sourceOffset and
     * \a sourceLength is greater than \link length() source.length()\endlink.
     */
    inline RWBasicUString& append(const RWBasicUString& source, size_t sourceOffset, size_t sourceLength);

    /**
     * Appends the contents of the null-terminated array \a source to
     * the contents of self.
     */
    inline RWBasicUString& append(const RWUChar16* source);

    /**
     * Appends the contents of the array \a source to the contents of
     * self. The size of the source array is specified using \a sourceLength.
     * The source array may contain embedded nulls.
     */
    inline RWBasicUString& append(const RWUChar16* source, size_t sourceLength);

    /**
     * Converts the code points in the null-terminated array \a source
     * into UTF-16 code units and appends those code units to the contents
     * of self.
     */
    inline RWBasicUString& append(const RWUChar32* source);

    /**
     * Converts the code points in the array \a source into UTF-16 code
     * units and appends those code units to the contents of self. The size
     * of the source array is specified using \a sourceLength.
     */
    inline RWBasicUString& append(const RWUChar32* source, size_t sourceLength);

    /**
     * Appends \a repeat copies of \a codeUnit to the contents of self.
     * The code unit may be zero or null.
     */
    inline RWBasicUString& append(RWUChar16 codeUnit, size_t repeat = 1);

    /**
     * Converts the UTF-32 \a codePoint into its equivalent UTF-16 representation
     * and appends \a repeat copies of the resultant code unit or surrogate
     * pair of code units to the contents of self. The code point may
     * be zero or null.
     */
    inline RWBasicUString& append(RWUChar32 codePoint, size_t repeat = 1);

    /**
     * Returns \a offset if the value is in bounds.
     *
     * @throw RWBoundsErr Thrown with a message of \c RWTOOL_INDEXERR if
     * \a offset is greater than or equal to length().
     */
    inline size_t boundsCheckIndex(size_t offset) const;

    /**
     * Returns \a length if the value is in bounds.
     *
     * @throw RWBoundsErr Thrown with a message of \c RWTOOL_INDEXERR if
     * \a offset plus \a length is greater than or equal to length().
     */
    inline size_t boundsCheckLength(size_t offset, size_t length) const;

    /**
     * Returns the maximum number of UTF-16 code units that may be stored
     * in self without allocating new storage.
     */
    inline size_t capacity() const;

    /**
     * Allocates and copies the existing buffer contents as required
     * to provide the capacity specified by \a newCapacity. The new
     * capacity must be greater than or equal to the current value of
     * length(); otherwise no reallocation or copy occurs.  Returns
     * the resultant capacity of self, which in some cases may be larger
     * than the capacity requested.
     */
    size_t capacity(size_t newCapacity);

    /**
     * Returns the number of Unicode code points encoded by the code
     * units contained in self, including embedded nulls. This method
     * recalculates the result each time it is called. Use codeUnitLength()
     * to determine the number of UTF-16 code units contained in self.
     */
    inline size_t codePointLength() const;

    /**
     * Returns the number of Unicode code points encoded by the code
     * units in the null-terminated sequence \a array. The terminating
     * null is not included in the count.
     */
    inline static size_t codePointLength(const RWUChar16* array);

    /**
     * Returns the number of Unicode code points encoded by the code
     * units in the sequence \a array. The size of \a array is specified
     * using \a length.
     */
    inline static size_t codePointLength(const RWUChar16* array, size_t length);

    /**
     * Returns the size of the null-terminated sequence \a array. The
     * terminating null is not included in the count.
     */
    inline static size_t codePointLength(const RWUChar32* array);

    /**
     * Returns the number of code units contained in self, including
     * embedded nulls. This method uses a cached value and may be called
     * repeatedly without incurring a significant performance penalty.
     * This method returns the same value as length(). Use codePointLength()
     * to determine the number of Unicode code points contained in self.
     */
    inline size_t codeUnitLength() const;

    /**
     * Returns the size of the null-terminated sequence \a array. The
     * terminating null is not included in the count.
     */
    inline static size_t codeUnitLength(const RWUChar16* array);

    /**
     * Returns the number of UTF-16 code units required to encode the
     * null-terminated UTF-32 code point sequence contained in \a array.
     * The terminating null is not included in the count.
     */
    inline static size_t codeUnitLength(const RWUChar32* array);

    /**
     * Returns the number of UTF-16 code units required to encode the
     * UTF-32 code point sequence contained in \a array. The size of
     * \a array is specified using \a length.
     */
    inline static size_t codeUnitLength(const RWUChar32* array, size_t length);

    /**
     * Returns a value that describes the lexical ordering between self
     * and \a rhs. The return value should be interpreted as follows:
     *
     * - Self appears before \a rhs if the return value is less than zero.
     * - Self is identical to \a rhs if the return value is zero.
     * - Self appears after \a rhs if the return value is greater than zero.
     *
     * If the two strings have different lengths, the shorter string
     * appears before the longer string if both strings are identical
     * over the length of the shorter string.
     */
    inline int compareCodePoints(const RWBasicUString& rhs) const;

    /**
     * Returns a value that describes the lexical ordering between self
     * and the specified range in \a rhs. The return value should be
     * interpreted as follows:
     *
     * - Self appears before \a rhs range if the return value is less than zero.
     * - Self is identical to \a rhs range if the return value is zero.
     * - Self appears after \a rhs range if the return value is greater than zero.
     *
     * If the two strings have different lengths, the shorter string
     * appears before the longer string if both strings are identical
     * over the length of the shorter string.
     *
     * @throw RWBoundsErr Thrown if the sum of \a rhsOffset and \a rhsLength
     * is greater than \link length() \a rhs.length() \endlink.
     */
    inline int compareCodePoints(const RWBasicUString& rhs, size_t rhsOffset, size_t rhsLength) const;

    /**
     * Returns a value that describes the lexical ordering between self
     * and the null-terminated array \a rhs. The return value should
     * be interpreted as follows:
     *
     * - Self appears before \a rhs if the return value is less than zero.
     * - Self is identical to \a rhs if the return value is zero.
     * - Self appears after \a rhs if the return value is greater than zero.
     *
     * If the two strings have different lengths, the shorter string
     * appears before the longer string if both strings are identical
     * over the length of the shorter string.
     */
    inline int compareCodePoints(const RWUChar16* rhs) const;

    /**
     * Returns a value that describes the lexical ordering between self
     * and the array \a rhs whose length is given by \a rhsLength. The
     * return value should be interpreted as follows:
     *
     * - Self appears before \a rhs if the return value is less than zero.
     * - Self is identical to \a rhs if the return value is zero.
     * - Self appears after \a rhs if the return value is greater than zero.
     *
     * If the two strings have different lengths, the shorter string
     * appears before the longer string if both strings are identical
     * over the length of the shorter string.
     */
    inline int compareCodePoints(const RWUChar16* rhs, size_t rhsLength) const;

    /**
     * Returns a value that describes the lexical ordering between self
     * and the null-terminated array \a rhs. The return value should
     * be interpreted as follows:
     *
     * - Self appears before \a rhs if the return value is less than zero.
     * - Self is identical to \a rhs if the return value is zero.
     * - Self appears after \a rhs if the return value is greater than zero.
     *
     * If the two strings have different code unit lengths, the shorter
     * string appears before the longer string if both strings are identical
     * over the length of the shorter string.
     */
    inline int compareCodePoints(const RWUChar32* rhs) const;

    /**
     * Returns a value that describes the lexical ordering between self
     * and the array \a rhs whose length is given by \a rhsLength. The
     * return value should be interpreted as follows:
     *
     * - Self appears before \a rhs if the return value is less than zero.
     * - Self is identical to \a rhs if the return value is zero.
     * - Self appears after \a rhs if the return value is greater than zero.
     *
     * If the two string have different code unit lengths, the shorter
     * string appears before the longer string if both strings are identical
     * over the length of the shorter string.
     */
    inline int compareCodePoints(const RWUChar32* rhs, size_t rhsLength) const;

    /**
     * Returns a value that describes the lexical ordering between self
     * and \a rhs. The return value should be interpreted as follows:
     *
     * - Self appears before \a rhs if the return value is less than zero.
     * - Self is identical to \a rhs if the return value is zero.
     * - Self appears after \a rhs if the return value is greater than zero.
     *
     * If the two strings have different lengths, the shorter string
     * appears before the longer string if both strings are identical
     * over the length of the shorter string.
     *
     * This method compares code unit values, not code point values.
     * This may not produce the desired result if either string contains
     * surrogate pairs or code unit values above the surrogate region.
     * Use compareCodePoints() if code point ordering is required.
     */
    inline int compareCodeUnits(const RWBasicUString& rhs) const;

    /**
     * Returns a value that describes the lexical ordering between self
     * and the specified range in \a rhs. The return value should be
     * interpreted as follows:
     *
     * - Self appears before \a rhs range if the return value is less than zero.
     * - Self is identical to \a rhs range if the return value is zero.
     * - Self appears after \a rhs range if the return value is greater than zero.
     *
     * If the two strings have different lengths, the shorter string
     * appears before the longer string if both strings are identical
     * over the length of the shorter string.
     *
     * This method compares code unit values, not code point values.
     * This may not produce the desired result if either string contains
     * surrogate pairs or code unit values above the surrogate region.
     * Use compareCodePoints() if code point ordering is required.
     *
     * @throw RWBoundsErr Thrown if the sum of \a rhsOffset and \a rhsLength
     * is greater than \link length() \a rhs.length() \endlink.
     */
    inline int compareCodeUnits(const RWBasicUString& rhs, size_t rhsOffset, size_t rhsLength) const;

    /**
     * Returns a value that describes the lexical ordering between self
     * and the null-terminated array \a rhs. The return value should
     * be interpreted as follows:
     *
     * - Self appears before \a rhs if the return value is less than zero.
     * - Self is identical to \a rhs if the return value is zero.
     * - Self appears after \a rhs if the return value is greater than zero.
     *
     * If the two strings have different lengths, the shorter string
     * appears before the longer string if both strings are identical
     * over the length of the shorter string.
     *
     * This method compares code unit values, not code point values.
     * This may not produce the desired result if either string contains
     * surrogate pairs or code unit values above the surrogate region.
     * Use compareCodePoints() if code point ordering is required.
     */
    inline int compareCodeUnits(const RWUChar16* rhs) const;

    /**
     * Returns a value that describes the lexical ordering between self
     * and the array \a rhs whose length is given by \a rhsLength. The
     * return value should be interpreted as follows:
     *
     * - Self appears before \a rhs if the return value is less than zero.
     * - Self is identical to \a rhs if the return value is zero.
     * - Self appears after \a rhs if the return value is greater than zero.
     *
     * If the two strings have different lengths, the shorter string
     * appears before the longer string if both strings are identical
     * over the length of the shorter string.
     *
     * This method compares code unit values, not code point values.
     * This may not produce the desired result if either string contains
     * surrogate pairs or code unit values above the surrogate region.
     * Use compareCodePoints() if code point ordering is required.
     */
    inline int compareCodeUnits(const RWUChar16* rhs, size_t rhsLength) const;

    /**
     * Returns a value that describes the lexical ordering between self
     * and \a rhs. Equivalent to compareCodeUnits(const RWBasicUString&) const.
     */
    inline int compareTo(const RWBasicUString& rhs) const;

    /**
     * Returns a value that describes the lexical ordering between self
     * and the specified range in \a rhs. Equivalent to
     * compareCodeUnits(const RWBasicUString&,size_t,size_t) const.
     */
    inline int compareTo(const RWBasicUString& rhs, size_t rhsOffset, size_t rhsLength) const;

    /**
     * Returns a value that describes the lexical ordering between self
     * and the null-terminated array \a rhs. Equivalent to
     * compareCodeUnits(const RWUChar16*) const.
     */
    inline int compareTo(const RWUChar16* rhs) const;

    /**
     * Returns a value that describes the lexical ordering between self
     * and the array \a rhs whose length is give by \a rhsLength. Equivalent
     * to compareCodeUnits(const RWUChar16*,size_t) const.
     */
    inline int compareTo(const RWUChar16* rhs, size_t rhsLength) const;

    /**
     * Returns a value that describes the lexical ordering between self
     * and the null-terminated array \a rhs. Equivalent to
     * compareCodePoints(const RWUChar32*) const.
     */
    inline int compareTo(const RWUChar32* rhs) const;

    /**
     * Returns a value that describes the lexical ordering between self
     * and the array \a rhs whose length is give by \a rhsLength. Equivalent
     * to compareCodePoints(const RWUChar32*,size_t) const.
     */
    inline int compareTo(const RWUChar32* rhs, size_t rhsLength) const;

    /**
     * Returns the code point value encoded by the specified pair of
     * code units. The code units must describe a valid surrogate pair.
     * Any other values produce an undefined and erroneous result.
     */
    inline static RWUChar32 computeCodePointValue(RWUChar16 highSurrogate, RWUChar16 lowSurrogate);

    /**
     * Returns \c true if self contains \a pattern, otherwise returns \c false.
     * A zero-length pattern returns \c true.
     */
    inline bool contains(const RWBasicUString& pattern) const;

    /**
     * Returns \c true if self contains the specified range in \a pattern,
     * otherwise returns \c false. The pattern range starts at index
     * \a patternOffset and extends for \a patternLength code units.
     * A zero-length pattern returns \c true.
     *
     * @throw RWBoundsErr Thrown if the sum of \a patternOffset and
     * \a patternLength is greater than \link length() pattern.length()\endlink.
     */
    inline bool contains(const RWBasicUString& pattern, size_t patternOffset, size_t patternLength) const;

    /**
     * Returns \c true if self contains \a pattern, otherwise returns \c false.
     * The \a pattern array must be null-terminated. A zero-length pattern
     * returns \c true.
     */
    inline bool contains(const RWUChar16* pattern) const;

    /**
     * Returns \c true if self contains \a pattern, otherwise returns \c false.
     * The size of the \a pattern array is specified using \a patternLength.
     * A zero-length pattern returns \c true.
     */
    inline bool contains(const RWUChar16* pattern, size_t patternLength) const;

    /**
     * Returns \c true if self contains \a pattern, otherwise returns \c false.
     * The \a pattern array must be null-terminated. A zero-length pattern
     * returns \c true.
     */
    inline bool contains(const RWUChar32* pattern) const;

    /**
     * Returns \c true if self contains \a pattern, otherwise returns \c false.
     * The size of the \a pattern array is specified using \a patternLength.
     * A zero-length pattern returns \c true.
     */
    inline bool contains(const RWUChar32* pattern, size_t patternLength) const;

    /**
     * Returns \c true if self contains \a codeUnit, otherwise returns \c false.
     */
    inline bool contains(RWUChar16 codeUnit) const;

    /**
     * Returns \c true if self contains \a codePoint, otherwise returns \c false.
     */
    inline bool contains(RWUChar32 codePoint) const;

    /**
     * Returns \c true if the specified range in self contains \a pattern,
     * otherwise returns \c false. The search begins at index \a offset within
     * self. A zero-length pattern returns \c true.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline bool contains(size_t offset, const RWBasicUString& pattern) const;

    /**
     * Returns \c true if the specified range in self contains the specified
     * range in \a pattern, otherwise returns \c false. The search begins at
     * index \a offset within self. The pattern range starts at index
     * \a patternOffset and extends for \a patternLength code units.
     * A zero-length pattern returns \c true.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length(), or the sum of \a patternOffset and \a patternLength is greater
     * than \link length() pattern.length()\endlink.
     */
    inline bool contains(size_t offset, const RWBasicUString& pattern, size_t patternOffset, size_t patternLength) const;

    /**
     * Returns \c true if the specified range in self contains \a pattern,
     * otherwise returns \c false. The search begins at index \a offset within
     * self. The \a pattern array must be null-terminated. A zero-length
     * pattern returns \c true.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline bool contains(size_t offset, const RWUChar16* pattern) const;

    /**
     * Returns \c true if the specified range in self contains \a pattern,
     * otherwise returns \c false. The search begins at index \a offset within
     * self. The size of the \a pattern array is specified using \a
     * patternLength. A zero-length pattern returns \c true.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline bool contains(size_t offset, const RWUChar16* pattern, size_t patternLength) const;

    /**
     * Returns \c true if the specified range in self contains \a pattern,
     * otherwise returns \c false. The search begins at index \a offset within
     * self. The \a pattern array must be null-terminated. A zero-length
     * pattern returns \c true.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline bool contains(size_t offset, const RWUChar32* pattern) const;

    /**
     * Returns \c true if the specified range in self contains \a pattern,
     * otherwise returns \c false. The search begins at index \a offset within
     * self. The size of the \a pattern array is specified using \a patternLength.
     * A zero-length pattern returns \c true.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline bool contains(size_t offset, const RWUChar32* pattern, size_t patternLength) const;

    /**
     * Returns \c true if the specified range in self contains \a codeUnit,
     * otherwise returns \c false. The search begins at index \a offset within
     * self.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline bool contains(size_t offset, RWUChar16 codeUnit) const;

    /**
     * Returns \c true if the specified range in self contains \a codePoint,
     * otherwise returns \c false. The search begins at index \a offset within
     * self.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline bool contains(size_t offset, RWUChar32 codePoint) const;

    /**
     * Returns \c true if the specified range in self contains \a pattern,
     * otherwise returns \c false. The search range starts at index \a offset
     * within self, and extends for \a length code units. A zero-length
     * pattern returns \c true.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline bool contains(size_t offset, size_t length, const RWBasicUString& pattern) const;

    /**
     * Returns \c true if the specified range in self contains the specified
     * range in \a pattern, otherwise returns \c false. The search range starts
     * at index \a offset within self and extends for \a length code
     * units. The pattern range starts at index \a patternOffset and
     * extends for \a patternLength code units. A zero-length pattern
     * returns \c true.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length(), or the sum of \a patternOffset and \a patternLength
     * is greater than \link length() pattern.length()\endlink.
     */
    inline bool contains(size_t offset, size_t length, const RWBasicUString& pattern, size_t patternOffset, size_t patternLength) const;

    /**
     * Returns \c true if the specified range in self contains \a pattern,
     * otherwise returns \c false. The search range starts at index \a offset
     * within self and extends for \a length code units. The \a pattern
     * array must be null-terminated. A zero-length pattern returns \c true.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline bool contains(size_t offset, size_t length, const RWUChar16* pattern) const;

    /**
     * Returns \c true if the specified range in self contains \a pattern,
     * otherwise returns \c false. The search range starts at index \a offset
     * within self, and extends for \a length code units. The size of
     * the \a pattern array is specified using \a patternLength. A zero-length
     * pattern returns \c true.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline bool contains(size_t offset, size_t length, const RWUChar16* pattern, size_t patternLength) const;

    /**
     * Returns \c true if the specified range in self contains \a pattern,
     * otherwise returns \c false. The search range starts at index \a offset
     * within self, and extends for \a length code units. The \a pattern
     * array must be null-terminated. A zero-length pattern returns \c true.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline bool contains(size_t offset, size_t length, const RWUChar32* pattern) const;

    /**
     * Returns \c true if the specified range in self contains \a pattern,
     * otherwise returns \c false. The search range starts at index \a offset
     * within self and extends for \a length code units. The size of
     * the \a pattern array is specified using \a patternLength. A zero-length
     * pattern returns \c true.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline bool contains(size_t offset, size_t length, const RWUChar32* pattern, size_t patternLength) const;

    /**
     * Returns \c true if the specified range in self contains \a codeUnit,
     * otherwise returns \c false. The search range starts at index \a offset
     * within self and extends for \a length code units.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline bool contains(size_t offset, size_t length, RWUChar16 codeUnit) const;

    /**
     * Returns \c true if the specified range in self contains \a codePoint,
     * otherwise returns \c false. The search range starts at index \a offset
     * within self, and extends for \a length code units.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline bool contains(size_t offset, size_t length, RWUChar32 codePoint) const;

    /**
     * Returns a pointer to the contents of self. The storage referenced
     * by this pointer is owned by RWBasicUString and may not be deleted
     * or modified. The pointer becomes invalid if self is modified
     * or destroyed. The contents may not be null-terminated if the
     * client provided the underlying buffer and there was insufficient
     * space for a terminating null. In all other cases, the contents
     * will be null terminated. The contents may contain embedded nulls,
     * in which case the length of the code-unit sequence is determined
     * using length().
     */
    inline const RWUChar16* data() const;

    /**
     * Returns \c true if self has a length of zero, otherwise returns \c false.
     * This method requires constant time, while <tt>length() == 0</tt>
     * may require linear time to execute. This method is equivalent to isNull().
     */
    inline bool empty() const;

    /**
     * Returns the index of the first occurrence of any code unit in
     * \a codeUnitSet, or #RW_NPOS if none of the code units in the
     * set are found.
     */
    inline size_t first(const RWBasicUString& codeUnitSet) const;

    /**
     * Returns the index of the first occurrence of any code unit in
     * \a codeUnitSet, or #RW_NPOS if none of the code units in the
     * set are found. The \a codeUnitSet array must be null-terminated.
     */
    inline size_t first(const RWUChar16* codeUnitSet) const;

    /**
     * Returns the index of the first occurrence of any code unit in
     * \a codeUnitSet, or #RW_NPOS if none of the code units in the
     * set are found. The size of the \a codeUnitSet array is specified
     * using \a numCodeUnits.
     */
    inline size_t first(const RWUChar16* codeUnitSet, size_t numCodeUnits) const;

    /**
      * Returns the index of the first occurrence of any code point in
      * \a codePointSet, or #RW_NPOS if none of the code points in
      * the set are found. The \a codePointSet array must be null-terminated.
      */
    inline size_t first(const RWUChar32* codePointSet) const;

    /**
     * Returns the index of the first occurrence of any code point in
     * \a codePointSet, or #RW_NPOS if none of the code points in
     * the set are found. The size of the \a codePointSet is specified
     * using \a numCodePoints.
     */
    inline size_t first(const RWUChar32* codePointSet, size_t numCodePoints) const;

    /**
     * Returns the index of the first occurrence of \a codeUnit, or
     * #RW_NPOS if that code unit is not found. This method produces
     * the same result as index(RWUChar16) const.
     */
    inline size_t first(RWUChar16 codeUnit) const;

    /**
     * Returns the index of the first occurrence of \a codePoint, or
     * #RW_NPOS if the code point is not found. This method produces
     * the same result as index(RWUChar32) const.
     */
    inline size_t first(RWUChar32 codePoint) const;

    /**
     * Returns the index of the first occurrence of any code unit in
     * \a codeUnitSet, or #RW_NPOS if none of the code units in the
     * set are found. The search range starts at index \a offset within
     * self.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t first(size_t offset, const RWBasicUString& codeUnitSet) const;

    /**
     * Returns the index of the first occurrence of any code unit in
     * \a codeUnitSet within the specified range of self, or #RW_NPOS
     * is none of the code units in the set are found. The search range
     * starts at index \a offset within self. The \a codeUnitSet array must be
     * null-terminated.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t first(size_t offset, const RWUChar16* codeUnitSet) const;

    /**
     * Returns the index of the first occurrence of any code unit in
     * \a codeUnitSet, or #RW_NPOS if none of the code units in the
     * set are found. The search range starts at index \a offset within
     * self. The size of the \a codeUnitSet array is specified using
     * \a numCodeUnits.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t first(size_t offset, const RWUChar16* codeUnitSet, size_t numCodeUnits) const;

    /**
     * Returns the index of the first occurrence of any code point in
     * \a codePointSet within the specified range in self, or #RW_NPOS
     * if none of the code points in the set are found. The search begins
     * at index position \a offset within self. The \a codePointSet array
     * must be null-terminated.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t first(size_t offset, const RWUChar32* codePointSet) const;

    /**
     * Returns the index of the first occurrence of any code point in
     * \a codePointSet, or #RW_NPOS if none of the code points in
     * the set are found. The search begins at index position \a offset
     * within self. The size of the \a codePointSet array is specified
     * using \a numCodePoints.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t first(size_t offset, const RWUChar32* codePointSet, size_t numCodePoints) const;

    /**
     * Returns the index of the first occurrence of \a codeUnit, or
     * #RW_NPOS if the code unit is not found. The search range starts
     * at index \a offset within self.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t first(size_t offset, RWUChar16 codeUnit) const;

    /**
     * Returns the index of the first occurrence of \a codePoint within
     * the specified range in self, or #RW_NPOS if the code point
     * is not found. The search begins at index position \a offset within
     * self.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t first(size_t offset, RWUChar32 codePoint) const;

    /**
     * Returns the index of the first occurrence of any code unit in
     * \a codeUnitSet within the specified range of self, or #RW_NPOS
     * if none of the code units in the set are found. The search range
     * starts at index \a offset within self and extends for \a length
     * code units.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t first(size_t offset, size_t length, const RWBasicUString& codeUnitSet) const;

    /**
     * Returns the index of the first occurrence of any code unit in
     * \a codeUnitSet within the specified range of self, or #RW_NPOS
     * if none of the code units in the set are found. The search range
     * starts at index \a offset within self and extends for \a length
     * code units. The \a codeUnitSet array must be null-terminated.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    size_t first(size_t offset, size_t length, const RWUChar16* codeUnitSet) const;

    /**
     * Returns the index of the first occurrence of any code unit in
     * \a codeUnitSet within the specified range of self, or #RW_NPOS
     * if none of the code units in the set are found. The search range
     * starts at index \a offset within self, and extends for \a length
     * code units. The size of the \a codeUnitSet array is specified
     * using \a numCodeUnits.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    size_t first(size_t offset, size_t length, const RWUChar16* codeUnitSet, size_t numCodeUnits) const;

    /**
     * Returns the index of the first occurrence of any code point
     * in \a codePointSet within the specified range of self, or
     * #RW_NPOS if the none of the code points in the set are found.
     * The search range starts at index \a offset within self and extends
     * for \a length code units. The \a codePointSet array must be null-terminated.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    size_t first(size_t offset, size_t length, const RWUChar32* codePointSet) const;

    /**
     * Returns the index of the first occurrence of any code point
     * in \a codePointSet within the specified range of self, or
     * #RW_NPOS if the none of the code points in the set are found.
     * The search range starts at index \a offset within self and extends
     * for \a length code units. The size of the \a codePointSet
     * array is specified using \a numCodePoints.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    size_t first(size_t offset, size_t length, const RWUChar32* codePointSet, size_t numCodePoints) const;

    /**
     * Returns the index of the first occurrence of \a codeUnit within
     * the specified range of self, or #RW_NPOS if the code unit is
     * not found. The search range starts at index \a offset within
     * self and extends for \a length code units.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t first(size_t offset, size_t length, RWUChar16 codeUnit) const;

    /**
     * Returns the index of the first occurrence of \a codePoint within
     * the specified range of self, or #RW_NPOS if the code point
     * is not found. The search range starts at index \a offset within
     * self and extends for \a length code units.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t first(size_t offset, size_t length, RWUChar32 codePoint) const;

    /**
     * Returns the high, UTF-16 surrogate (\c 0xD800..\c 0xDBFF) for
     * a code point in the range \c 0x10000..\c 0x10FFFF.
     */
    inline static RWUChar16 highSurrogate(RWUChar32 codePoint);

    /**
     * Returns \c true if the contents and length of self are identical
     * to \a array, otherwise returns \c false. This method is more efficient
     * than compareCodeUnits() when testing for equality.
     */
    bool equals(const RWBasicUString& array) const;

    /**
     * Returns a hash value computed from the contents of self.
     */
    unsigned long hash() const;

    /**
     * Returns the index of the first occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. Returns an index value of
     * zero if the pattern length is zero.
     */
    inline size_t index(const RWBasicUString& pattern) const;

    /**
     * Returns the index of the first occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The pattern range starts
     * at index \a patternOffset and extends for \a patternLength code
     * units. Returns an index value of zero if the pattern length is
     * zero.
     *
     * @throw RWBoundsErr Thrown if the sum of \a patternOffset and
     * \a patternLength is greater than
     * \link length() \a pattern.length() \endlink.
     */
    inline size_t index(const RWBasicUString& pattern, size_t patternOffset, size_t patternLength) const;

    /**
     * Returns the index of the first occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The pattern must be null-terminated.
     * Returns an index value of zero if the pattern length is zero.
     */
    inline size_t index(const RWUChar16* pattern) const;

    /**
     * Returns the index of the first occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The size of the pattern
     * is specified using \a patternLength. Returns an index value of
     * zero if the pattern length is zero.
     */
    inline size_t index(const RWUChar16* pattern, size_t patternLength) const;

    /**
     * Returns the index of the first occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The pattern must be null-terminated.
     * Returns an index value of zero if the pattern length is zero.
     */
    inline size_t index(const RWUChar32* pattern) const;

    /**
     * Returns the index of the first occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The size of the pattern
     * is specified using \a patternLength. Returns an index value of
     * zero if the pattern length is zero.
     */
    inline size_t index(const RWUChar32* pattern, size_t patternLength) const;

    /**
     * Returns the index of the first occurrence of \a codeUnit, or
     * #RW_NPOS if the code unit is not found. This function produces
     * the same result as first(RWUChar16) const.
     */
    inline size_t index(RWUChar16 codeUnit) const;

    /**
     * Returns the index of the first occurrence of \a codePoint, or
     * #RW_NPOS if the code unit is not found. This function produces
     * the same result as first(RWUChar32) const.
     */
    inline size_t index(RWUChar32 codePoint) const;

    /**
     * Returns the index of the first occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * position \a offset within self. Returns an index value of zero if
     * the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t index(size_t offset, const RWBasicUString& pattern) const;

    /**
     * Returns the index of the first occurrence of the specified range
     * in \a pattern, or #RW_NPOS if the pattern is not found. The
     * search begins at index position \a offset within self. The pattern
     * range starts at index \a patternOffset and extends for \a patternLength
     * code units. Returns an index value of zero if the pattern length is
     * zero.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length(), or if the sum of \a patternOffset and \a patternLength is greater
     * than \link length() \a pattern.length() \endlink.
     */
    inline size_t index(size_t offset, const RWBasicUString& pattern, size_t patternOffset, size_t patternLength) const;

    /**
     * Returns the index of the first occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * position \a offset within self. The pattern must be null-terminated.
     * Returns an index value of zero if the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t index(size_t offset, const RWUChar16* pattern) const;

    /**
     * Returns the index of the first occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * position \a offset within self. The size of the pattern is specified
     * using \a patternLength. Returns an index value of zero if the pattern
     * length is zero.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t index(size_t offset, const RWUChar16* pattern, size_t patternLength) const;

    /**
     * Returns the index of the first occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * position \a offset within self. The pattern must be null-terminated.
     * Returns an index value of zero if the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t index(size_t offset, const RWUChar32* pattern) const;

    /**
     * Returns the index of the first occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * position \a offset within self. The size of the pattern is specified
     * using \a patternLength. Returns an index value of zero if the pattern
     * length is zero.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t index(size_t offset, const RWUChar32* pattern, size_t patternLength) const;

    /**
     * Returns the index of the first occurrence of \a codeUnit, or
     * #RW_NPOS if the code unit is not found. The search begins at
     * index position \a offset within self. This function produces
     * the same result as first(size_t,RWUChar16) const.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t index(size_t offset, RWUChar16 codeUnit) const;

    /**
     * Returns the index of the first occurrence of \a codePoint, or
     * #RW_NPOS if the code unit is not found. The search begins at
     * index position \a offset within self. This function produces
     * the same result as first(size_t,RWUChar32) const.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t index(size_t offset, RWUChar32 codePoint) const;

    /**
     * Returns the index of the first occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * \a offset within self and extends for \a length code units.
     * Returns an index value of zero if the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t index(size_t offset, size_t length, const RWBasicUString& pattern) const;

    /**
     * Returns the index of the first occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * \a offset within self and extends for \a length code units.
     * The pattern range starts at index \a patternOffset,
     * and extends for \a patternLength code units. Returns an index
     * value of zero if the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length() or if the sum of \a patternOffset and
     * \a patternLength is greater than \link length() pattern.length()\endlink.
     */
    inline size_t index(size_t offset, size_t length, const RWBasicUString& pattern, size_t patternOffset, size_t patternLength) const;

    /**
     * Returns the index of the first occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * \a offset within self and extends for \a length code units.
     * The pattern must be null-terminated. Returns an
     * index value of zero if the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t index(size_t offset, size_t length, const RWUChar16* pattern) const;

    /**
     * Returns the index of the first occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * \a offset within self and extends for \a length code units.
     * The size of the pattern is specified using \a patternLength.
     * Returns an index value of zero if the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t index(size_t offset, size_t length, const RWUChar16* pattern, size_t patternLength) const;

    /**
     * Returns the index of the first occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The pattern must be
     * null-terminated. The search begins at index \a offset within self
     * and extends for \a length code units. Returns an index value of zero
     * if the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t index(size_t offset, size_t length, const RWUChar32* pattern) const;

    /**
     * Returns the index of the first occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * \a offset within self and extends for \a length code units.
     * The size of the pattern is specified using \a patternLength.
     * Returns an index value of zero if the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t index(size_t offset, size_t length, const RWUChar32* pattern, size_t patternLength) const;

    /**
     * Returns the index of the first occurrence of \a codeUnit, or
     * #RW_NPOS if the code unit is not found. The search begins at
     * index \a offset within self and extends for \a length code units.
     * This function produces the same result as first(size_t,RWUChar16) const.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t index(size_t offset, size_t length, RWUChar16 codeUnit) const;

    /**
     * Returns the index of the first occurrence of \a codePoint, or
     * #RW_NPOS if the code unit is not found. The search begins at
     * index \a offset within self and extends for \a length code units.
     * This function produces the same result as first(size_t,RWUChar32) const.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t index(size_t offset, size_t length, RWUChar32 codePoint) const;

    /**
     * Inserts the contents of \a source before the code unit at index
     * \a offset within the contents of self.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline RWBasicUString& insert(size_t offset, const RWBasicUString& source);

    /**
     * Inserts the contents of the specified range in \a source before
     * the code unit at index \a offset within the contents of self.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length() or if the sum of \a sourceOffset and \a sourceLength is greater
     * than \link length() \a source.length() \endlink.
     */
    inline RWBasicUString& insert(size_t offset, const RWBasicUString& source, size_t sourceOffset, size_t sourceLength);

    /**
     * Inserts the contents of the null-terminated array \a source before
     * the code unit at index \a offset within the contents of self.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline RWBasicUString& insert(size_t offset, const RWUChar16* source);

    /**
     * Inserts the contents of the array \a source before the code unit
     * at index \a offset within the contents of self. The size of the source
     * array is specified using \a sourceLength. The source array may
     * contain embedded nulls.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline RWBasicUString& insert(size_t offset, const RWUChar16* source, size_t sourceLength);

    /**
     * Converts the code points in the null-terminated array \a source
     * into UTF-16 code units and inserts those code units before the
     * code unit at index \a offset within the contents of self.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline RWBasicUString& insert(size_t offset, const RWUChar32* source);

    /**
     * Converts the code points in the array \a source into UTF-16 code
     * units and inserts those code units before the code unit at index
     * \a offset within the contents of self. The size of the source array
     * is specified using \a sourceLength.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline RWBasicUString& insert(size_t offset, const RWUChar32* source, size_t sourceLength);

    /**
     * Inserts \a repeat copies of \a codeUnit before the code unit
     * at index \a offset within the contents of self. The code unit
     * may be zero or null.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline RWBasicUString& insert(size_t offset, RWUChar16 codeUnit, size_t repeat = 1);

    /**
     * Converts the UTF-32 \a codePoint into its equivalent UTF-16 representation
     * and inserts \a repeat copies of the resultant code unit or surrogate
     * pair of code units before the code unit at index \a offset within
     * the contents of self. The code point may be zero or null.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline RWBasicUString& insert(size_t offset, RWUChar32 codePoint, size_t repeat = 1);

    /**
     * Returns \c true if \a codeUnit encodes the high surrogate of
     * a surrogate pair, otherwise returns \c false.
     */
    inline static bool isHighSurrogate(RWUChar16 codeUnit);

    /**
     * Returns \c true if self has a length of zero, otherwise returns \c false.
     * This method requires constant time while <tt>length() == 0</tt>
     * may require linear time to execute. This method is equivalent to empty().
     */
    inline bool isNull() const;

    /**
     * Returns \c true if \a codeUnit encodes the low surrogate of a
     * surrogate pair, otherwise returns \c false.
     */
    inline static bool isLowSurrogate(RWUChar16 codeUnit);

    /**
     * Returns \c true if \a codePoint is a valid Unicode character,
     * otherwise returns \c false.
     *
     * Non-characters include:
     *
     * -  single surrogate code points (\c U+d800..\c U+dfff, 2048 code points)
     * -  the last two code points on each plane (\c U+__fffe and \c U+__ffff,
     *    34 code points)
     * -  \c U+fdd0..\c U+fdef (new with Unicode 3.1, 32 code points)
     * -  code point values above \c U+10ffff
     */
    inline static bool isUnicodeCharacter(RWUChar32 codePoint);

    /**
     * Returns the index of the last occurrence of any code unit in
     * \a codeUnitSet, or #RW_NPOS is none of the code units in the
     * set are found.
     */
    inline size_t last(const RWBasicUString& codeUnitSet) const;

    /**
     * Returns the index of the last occurrence of any code unit in
     * \a codeUnitSet, or #RW_NPOS is none of the code units in the
     * set are found. The \a codeUnitSet array must be null-terminated.
     */
    inline size_t last(const RWUChar16* codeUnitSet) const;

    /**
     * Returns the index of the last occurrence of any code unit in
     * \a codeUnitSet, or #RW_NPOS if none of the code units in the
     * set are found. The size of the \a codeUnitSet array is specified
     * using \a numCodeUnits.
     */
    inline size_t last(const RWUChar16* codeUnitSet, size_t numCodeUnits) const;

    /**
     * Returns the index of the last occurrence of any code point in
     * \a codePointSet, or #RW_NPOS if none of the code points in
     * the set are found. The \a codePointSet array must be null-terminated.
     */
    inline size_t last(const RWUChar32* codePointSet) const;

    /**
     * Returns the index of the last occurrence of any code point in
     * \a codePointSet, or #RW_NPOS if none of the code points in
     * the set are found. The size of the \a codePointSet is specified
     * using \a numCodePoints.
     */
    inline size_t last(const RWUChar32* codePointSet, size_t numCodePoints) const;

    /**
     * Returns the index of the last occurrence of \a codeUnit, or
     * #RW_NPOS if that code unit is not found.
     */
    inline size_t last(RWUChar16 codeUnit) const;

    /**
     * Returns the index of the last occurrence of \a codePoint, or
     * #RW_NPOS if that code point is not found.
     */
    inline size_t last(RWUChar32 codePoint) const;

    /**
     * Returns the index of the last occurrence of any code unit in
     * \a codeUnitSet within the specified range in self, or #RW_NPOS
     * is none of the code units in the set are found. The search begins
     * at index position \a offset within self.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t last(size_t offset, const RWBasicUString& codeUnitSet) const;

    /**
     * Returns the index of the last occurrence of any code unit in
     * \a codeUnitSet within the specified range in self, or #RW_NPOS
     * if none of the code units in the set are found. The search begins
     * at index position \a offset within self. The \a codeUnitSet array
     * must be null-terminated.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t last(size_t offset, const RWUChar16* codeUnitSet) const;

    /**
     * Returns the index of the last occurrence of any code unit in
     * \a codeUnitSet within the specified range in self, or #RW_NPOS
     * if none of the code units in the set are found. The search begins
     * at index position \a offset within self. The size of the \a codeUnitSet
     *  array is specified using \a numCodeUnits.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t last(size_t offset, const RWUChar16* codeUnitSet, size_t numCodeUnits) const;

    /**
     * Returns the index of the last occurrence of any code point in
     * \a codePointSet within the specified range of self, or #RW_NPOS
     * if none of the code points in the set are found. The search begins
     * at index position \a offset within self. The \a codePointSet array
     * must be null-terminated.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t last(size_t offset, const RWUChar32* codePointSet) const;

    /**
     * Returns the index of the last occurrence of any code point in
     * \a codePointSet within the specified range in self, or #RW_NPOS
     * if none of the code points in the set are found. The search begins
     * at index position \a offset within self. The size of the \a codePointSet
     * is specified using \a numCodePoints.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t last(size_t offset, const RWUChar32* codePointSet, size_t numCodePoints) const;

    /**
     * Returns the index of the last occurrence of \a codeUnit, within
     * the specified range in self, or #RW_NPOS if that code unit
     * is not found. The search begins at index position \a offset within
     * self.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t last(size_t offset, RWUChar16 codeUnit) const;

    /**
     * Returns the index of the last occurrence of \a codePoint within
     * the specified range in self, or #RW_NPOS if that code point
     * is not found. The search begins at index position \a offset within
     * self.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t last(size_t offset, RWUChar32 codePoint) const;

    /**
     * Returns the index of the last occurrence of any code unit in
     * \a codeUnitSet within the specified range in self, or #RW_NPOS
     * if none of the code units in the set are found. The search range
     * starts at index \a offset within self and extends for \a length
     * code units.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t last(size_t offset, size_t length, const RWBasicUString& codeUnitSet) const;

    /**
     * Returns the index of the last occurrence of any code unit in
     * \a codeUnitSet within the specified range in self, or #RW_NPOS
     * if none of the code units in the set are found. The search range
     * starts at index \a offset within self and extends for \a length
     * code units. The \a codeUnitSet array must be null-terminated.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t last(size_t offset, size_t length, const RWUChar16* codeUnitSet) const;

    /**
     * Returns the index of the last occurrence of any code unit in
     * \a codeUnitSet within the specified range in self, or #RW_NPOS
     * if none of the code units in the set are found. The search range
     * starts at index \a offset within self, and extends for \a length
     * code units. The size of the \a codeUnitSet array is specified
     * using \a numCodeUnits.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t last(size_t offset, size_t length, const RWUChar16* codeUnitSet, size_t numCodeUnits) const;

    /**
     * Returns the index of the last occurrence of any code point in
     * \a codePointSet within the specified range of self, or #RW_NPOS
     * if none of the code points in the set are found. The search range
     * starts at index \a offset within self and extends for \a length
     * code units. The \a codePointSet array must be null-terminated.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t last(size_t offset, size_t length, const RWUChar32* codePointSet) const;

    /**
     * Returns the index of the last occurrence of any code point in
     * \a codePointSet within the specified range in self, or #RW_NPOS
     * if none of the code points in the set are found. The search range
     * starts at index \a offset within self, and extends for \a length
     * code units. The size of the \a codePointSet is specified
     * using \a numCodePoints.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t last(size_t offset, size_t length, const RWUChar32* codePointSet, size_t numCodePoints) const;

    /**
      * Returns the index of the last occurrence of \a codeUnit within
      * the specified range in self, or #RW_NPOS if that code unit
      * is not found. The search range starts at index \a offset within
      * self and extends for \a length code units.
      *
      * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
      * greater than length().
      */
    inline size_t last(size_t offset, size_t length, RWUChar16 codeUnit) const;

    /**
     * Returns the index of the last occurrence of \a codePoint within
     * the specified range in self, or #RW_NPOS if that code point
     * is not found. The search range starts at index \a offset within
     * self and extends for \a length code units.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t last(size_t offset, size_t length, RWUChar32 codePoint) const;

    /**
     * Returns the number of UTF-16 code units contained in self, including
     * embedded nulls. This method returns the same value as codeUnitLength().
     * Use codePointLength() to determine the number of Unicode code
     * points contained in self.
     */
    inline size_t length() const;

    /**
     * Returns the maximum number of code units that may be stored in
     * an RWBasicUString instance.
     */
    inline size_t max_size() const;

    /**
     * Replaces the contents of self with the contents of \a source
     * and returns a reference to self.
     */
    RWBasicUString& operator=(const RWBasicUString& source);

    /**
     * Replaces the contents of self with the null-terminated contents
     * of \a source and returns a reference to self.
     */
    RWBasicUString& operator=(const RWUChar16* source);

    /**
     * Replaces the contents of self with the code unit sequence required
     * to represent the null-terminated, UTF-32 encoded contents of
     * \a source in the UTF-16 encoding form and returns a reference
     * to self.
     */
    RWBasicUString& operator=(const RWUChar32* source);

    /**
     * Replaces the contents of self with the single code unit \a codeUnit
     * and returns a reference to self.
     */
    RWBasicUString& operator=(RWUChar16 codeUnit);

    /**
     * Replaces the contents of self with the code unit sequence required
     * to represent \a codePoint in the UTF-16 encoding form and returns
     * a reference to self.
     */
    RWBasicUString& operator=(RWUChar32 codePoint);

    /**
     * Appends the contents of \a source to the contents of self
     * and returns a reference to self.
     */
    inline RWBasicUString& operator+=(const RWBasicUString& source);

    /**
     * Appends the contents of the null-terminated array \a source to
     * the contents of self and returns a reference to self.
     */
    inline RWBasicUString& operator+=(const RWUChar16* source);

    /**
     * Appends the contents of the null-terminated array \a source to
     * the contents of self and returns a reference to self.
     */
    inline RWBasicUString& operator+=(const RWUChar32* source);

    /**
     * Appends \a codeUnit to the contents of self and returns
     * a reference to self.
     */
    inline RWBasicUString& operator+=(RWUChar16 codeUnit);

    /**
     * Appends \a codePoint to the contents of self and returns
     * a reference to self.
     */
    inline RWBasicUString& operator+=(RWUChar32 codePoint);

    // Here all integer types are provided with an exact match
    // to avoid ambiguities with the built in char[] operator
    // All operator[] functions are with bounds checking

    // The doxygen comment is copied because @copydoc fails to work

    /**
     * Returns a reference to the code unit at the position specified
     * by \a offset. This non-const variant can be used as an l-value
     * in an assignment expression.
     *
     * @note
     * An individual code unit may not represent a complete code point.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or
     * equal to length().
     */
    inline RWUChar16& operator[](short offset);

    /**
     * Returns a reference to the code unit at the position specified
     * by \a offset. This non-const variant can be used as an l-value
     * in an assignment expression.
     *
     * @note
     * An individual code unit may not represent a complete code point.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or
     * equal to length().
     */
    inline RWUChar16& operator[](int offset);

    /**
     * Returns a reference to the code unit at the position specified
     * by \a offset. This non-const variant can be used as an l-value
     * in an assignment expression.
     *
     * @note
     * An individual code unit may not represent a complete code point.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or
     * equal to length().
     */
    inline RWUChar16& operator[](long offset);

    /**
     * Returns a reference to the code unit at the position specified
     * by \a offset. This non-const variant can be used as an l-value
     * in an assignment expression.
     *
     * @note
     * An individual code unit may not represent a complete code point.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or
     * equal to length().
     */
    inline RWUChar16& operator[](unsigned short offset);

    /**
     * Returns a reference to the code unit at the position specified
     * by \a offset. This non-const variant can be used as an l-value
     * in an assignment expression.
     *
     * @note
     * An individual code unit may not represent a complete code point.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or
     * equal to length().
     */
    inline RWUChar16& operator[](unsigned int offset);

    /**
     * Returns a reference to the code unit at the position specified
     * by \a offset. This non-const variant can be used as an l-value
     * in an assignment expression.
     *
     * @note
     * An individual code unit may not represent a complete code point.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or
     * equal to length().
     */
    inline RWUChar16& operator[](unsigned long offset);

#if !defined(RW_NO_LONG_LONG) || defined(DOXYGEN)
    /**
     * Returns a reference to the code unit at the position specified
     * by \a offset. This non-const variant can be used as an l-value
     * in an assignment expression.
     *
     * @note
     * An individual code unit may not represent a complete code point.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or
     * equal to length().
     */
    inline RWUChar16& operator[](long long offset);

    /**
     * Returns a reference to the code unit at the position specified
     * by \a offset. This non-const variant can be used as an l-value
     * in an assignment expression.
     *
     * @note
     * An individual code unit may not represent a complete code point.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or
     * equal to length().
     */
    inline RWUChar16& operator[](unsigned long long offset);
#endif

    /**
     * Returns the code unit value at the position specified by \a offset.
     *
     * @note
     * An individual code unit may not represent a complete code point.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or
     * equal to length().
     */
    inline RWUChar16 operator[](short offset) const;

    /**
     * Returns the code unit value at the position specified by \a offset.
     *
     * @note
     * An individual code unit may not represent a complete code point.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or
     * equal to length().
     */
    inline RWUChar16 operator[](int offset) const;

    /**
     * Returns the code unit value at the position specified by \a offset.
     *
     * @note
     * An individual code unit may not represent a complete code point.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or
     * equal to length().
     */
    inline RWUChar16 operator[](long offset) const;

    /**
     * Returns the code unit value at the position specified by \a offset.
     *
     * @note
     * An individual code unit may not represent a complete code point.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or
     * equal to length().
     */
    inline RWUChar16 operator[](unsigned short offset) const;

    /**
     * Returns the code unit value at the position specified by \a offset.
     *
     * @note
     * An individual code unit may not represent a complete code point.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or
     * equal to length().
     */
    inline RWUChar16 operator[](unsigned int offset) const;

    /**
     * Returns the code unit value at the position specified by \a offset.
     *
     * @note
     * An individual code unit may not represent a complete code point.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or
     * equal to length().
     */
    inline RWUChar16 operator[](unsigned long offset) const;

#if !defined(RW_NO_LONG_LONG) || defined(DOXYGEN)
    /**
     * Returns the code unit value at the position specified by \a offset.
     *
     * @note
     * An individual code unit may not represent a complete code point.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or
     * equal to length().
     */
    inline RWUChar16 operator[](long long offset) const;

    /**
     * Returns the code unit value at the position specified by \a offset.
     *
     * @note An individual code unit may not represent a complete code point.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or
     * equal to length().
     */
    inline RWUChar16 operator[](unsigned long long offset) const;
#endif


    /**
     * Returns a reference to the code unit at the position specified
     * by \a offset. This non-const variant can be used as an l-value
     * in an assignment expression.
     *
     * @note
     * An individual code unit may not represent a complete code point.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or
     * equal to length().
     */
    inline RWUChar16& operator()(size_t offset);

    /**
     * Returns the code unit value at the position specified by \a offset.
     *
     * @note
     * An individual code unit may not represent a complete code point.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or
     * equal to length().
     */
    inline RWUChar16 operator()(size_t offset) const;

    /**
     * Returns a pointer to the contents of self. This array is owned
     * by RWBasicUString and may not be deleted or modified. The pointer
     * becomes invalid if self is modified or destroyed.
     *
     * The contents may not be null-terminated if the client provided
     * the underlying buffer and there was insufficient space for a
     * terminating null. In all other cases, the contents will be
     * null terminated.
     *
     * The array may also contain embedded nulls, in which case the length
     * of the code-unit sequence can be determined using length().
     */
    inline operator const RWUChar16* () const;

    /**
     * Prepends the contents of \a source to the contents of self.
     */
    inline RWBasicUString& prepend(const RWBasicUString& source);

    /**
     * Prepends the contents of the specified range in \a source to the
     * contents of self. The range starts at index \a sourceOffset within
     * \a source and extends for \a sourceLength code units.
     *
     * @throw RWBoundsErr Thrown if the sum of \a sourceOffset and
     * \a sourceLength is greater than \link length() source.length()\endlink.
     */
    inline RWBasicUString& prepend(const RWBasicUString& source, size_t sourceOffset, size_t sourceLength);

    /**
     * Prepends the contents of the null-terminated array \a source
     * to the contents of self.
     */
    inline RWBasicUString& prepend(const RWUChar16* source);

    /**
     * Prepends the contents of the array \a source to the contents
     * of self. The size of the source array is specified using \a sourceLength.
     * The source array may contain embedded nulls.
     */
    inline RWBasicUString& prepend(const RWUChar16* source, size_t sourceLength);

    /**
     * Converts the code points in the null-terminated array \a source
     * into UTF-16 code units and prepends those code units to the contents
     * of self.
     */
    inline RWBasicUString& prepend(const RWUChar32* source);

    /**
     * Converts the code points in the array \a source into UTF-16 code
     * units and prepends those code units to the contents of self. The
     * size of the source array is specified using \a sourceLength.
     */
    inline RWBasicUString& prepend(const RWUChar32* source, size_t sourceLength);

    /**
     * Prepends \a repeat copies of \a codeUnit to the contents of self.
     * The code unit may be zero or null.
     */
    inline RWBasicUString& prepend(RWUChar16 codeUnit, size_t repeat = 1);

    /**
     * Converts the UTF-32 \a codePoint into its equivalent UTF-16 representation
     * and prepends \a repeat copies of the resultant code unit or surrogate
     * pair of code units to the contents of self. The code point may
     * be zero or null.
     */
    inline RWBasicUString& prepend(RWUChar32 codePoint, size_t repeat = 1);

    /**
     * Removes the range of code units that start at \a offset and extend
     * through the end of self.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline RWBasicUString& remove(size_t offset = 0);

    /**
     * Removes the specified range of code units from the contents of
     * self, and collapses the contents as necessary to produce a contiguous
     * result.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline RWBasicUString& remove(size_t offset, size_t length);

    /**
     * Replaces the specified range of code units in self with the contents
     * of \a source. The sum of \a offset and \a length must be less
     * than or equal to length().
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline RWBasicUString& replace(size_t offset, size_t length, const RWBasicUString& source);

    /**
     * Replaces the specified range of code units in self with the specified
     * range of code units in \a source. The range in self starts at index
     * \a offset and extends for \a length code units. The range in \a source
     * starts at index \a sourceOffset and extends for \a sourceLength code units.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length() or the sum of \a sourceOffset and \a sourceLength
     * is greater than \link length() \a source.length() \endlink.
     */
    inline RWBasicUString& replace(size_t offset, size_t length, const RWBasicUString& source, size_t sourceOffset, size_t sourceLength);

    /**
     * Replaces the specified range of code units in self with the contents
     * of the null-terminated array \a source. The range in self starts at index
     * \a offset and extends for \a length code units.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline RWBasicUString& replace(size_t offset, size_t length, const RWUChar16* source);

    /**
     * Replaces the specified range of code units in self with the contents
     * of the array \a source. The range in self starts at index \a offset
     * and extends for \a length code units. The size of the source array
     * is specified using \a sourceLength. The source array may contain
     * embedded nulls.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline RWBasicUString& replace(size_t offset, size_t length, const RWUChar16* source, size_t sourceLength);

    /**
     * Converts the code points in the null-terminated array \a source
     * into UTF-16 code units and replaces the specified range of code
     * units in self with those code units. The range in self starts
     * at index \a offset and extends for \a length code units.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline RWBasicUString& replace(size_t offset, size_t length, const RWUChar32* source);

    /**
     * Converts the code points in the array \a source into UTF-16 code
     * units and replaces the specified range of code units in self
     * with those code units. The range in self starts at index \a offset
     * and extends for \a length code units. The size of the source array
     * is specified using \a sourceLength. The source array may contain
     * embedded nulls.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline RWBasicUString& replace(size_t offset, size_t length, const RWUChar32* source, size_t sourceLength);

    /**
     * Replaces the specified range of code units in self with \a repeat
     * copies of \a codeUnit. The range in self starts at index \a offset
     * and extends for \a length code units. The code unit may be zero
     * or null.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline RWBasicUString& replace(size_t offset, size_t length, RWUChar16 codeUnit, size_t repeat = 1);

    /**
     * Converts the UTF-32 \a codePoint into its equivalent UTF-16 representation
     * and replaces the specified range of code units in self with \a repeat
     * copies of the resultant code unit or surrogate pair of code units.
     * The range in self starts at index \a offset and extends for \a length
     * code units. The code point may be zero or null.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline RWBasicUString& replace(size_t offset, size_t length, RWUChar32 codePoint, size_t repeat = 1);

    /**
     * Returns \c true if the UTF-16 representation of \a codeUnit requires
     * the use of a surrogate pair, otherwise returns \c false.
     */
    inline static bool requiresSurrogatePair(RWUChar32 codeUnit);

    /**
     * Pads or truncates the contents of self as required to produce
     * a length() of \a newCodeUnitLength. If the new length is greater
     * than the old length, \a codeUnit is used to pad the contents.
     */
    inline void resize(size_t newCodeUnitLength, RWUChar16 codeUnit);

    /**
     * Pads or truncates the contents of self as required to produce
     * a length() of \a newCodePointLength. If the new length is greater
     * than the old length, \a codePoint is used to pad the contents.
     */
    void resize(size_t newCodePointLength, RWUChar32 codePoint);

    /**
     * Replaces the contents of self with the code unit sequence read
     * from \a vis.
     */
    void restoreFrom(RWvistream& vis);

    /**
     * Replaces the contents of self with the code unit sequence read
     * from \a file.
     */
    void restoreFrom(RWFile& file);

    /**
     * Returns the index of the last occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. Returns an index value equal
     * to length() if the pattern length is zero.
     */
    inline size_t rindex(const RWBasicUString& pattern) const;

    /**
     * Returns the index of the last occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The pattern range starts
     * at index \a patternOffset and extends for \a patternLength code
     * units. Returns an index value equal to length() if the pattern
     * length is zero.
     *
     * @throw RWBoundsErr Thrown if the sum of \a patternOffset and
     * \a patternLength is greater than
     * \link length() \a pattern.length() \endlink.
     */
    inline size_t rindex(const RWBasicUString& pattern, size_t patternOffset, size_t patternLength) const;

    /**
     * Returns the index of the last occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The pattern must be null-terminated.
     * Returns an index value equal to length() if the pattern length is zero.
     */
    inline size_t rindex(const RWUChar16* pattern) const;

    /**
     * Returns the index of the last occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The size of the pattern
     * is specified using \a patternLength. Returns an index value equal
     * to length() if the pattern length is zero.
     */
    inline size_t rindex(const RWUChar16* pattern, size_t patternLength) const;

    /**
     * Returns the index of the last occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The pattern must be null-terminated.
     * Returns an index value equal to length() if the pattern length is zero.
     */
    inline size_t rindex(const RWUChar32* pattern) const;

    /**
     * Returns the index of the last occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The size of the pattern
     * is specified using \a patternLength. Returns an index value equal
     * to length() if the pattern length is zero.
     */
    inline size_t rindex(const RWUChar32* pattern, size_t patternLength) const;

    /**
     * Returns the index of the last occurrence of \a codeUnit, or
     * #RW_NPOS if the code unit is not found. This method is equivalent
     * to last(RWUChar16) const.
     */
    inline size_t rindex(RWUChar16 codeUnit) const;

    /**
     * Returns the index of the last occurrence of \a codePoint, or
     * #RW_NPOS if the code unit was not found. This method is equivalent
     * to last(RWUChar32) const.
     */
    inline size_t rindex(RWUChar32 codePoint) const;

    /**
     * Returns the index of the last occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * \a offset within self and extends through the end of self. Returns
     * an index value equal to length() if the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t rindex(size_t offset, const RWBasicUString& pattern) const;

    /**
     * Returns the index of the last occurrence of the specified range
     * in \a pattern, or #RW_NPOS if the pattern is not found. The
     * search begins at index \a offset within self and extends through
     * the end of self.
     *
     * The pattern begins at index \a patternOffset within the supplied
     * \a pattern and extends for \a patternLength code units. Returns an
     * index value equal to length() if the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length(), or the sum of \a patternOffset and \a patternLength is greater
     * than length().
     */
    inline size_t rindex(size_t offset, const RWBasicUString& pattern, size_t patternOffset, size_t patternLength) const;

    /**
     * Returns the index of the last occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * \a offset within self and extends through the end of self. The pattern
     * must be null-terminated. Returns an index value equal to length()
     * if the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t rindex(size_t offset, const RWUChar16* pattern) const;

    /**
     * Returns the index of the last occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * \a offset within self and extends through the end of self. The size
     * of the pattern is specified using \a patternLength. Returns an
     * index value equal to length() if the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t rindex(size_t offset, const RWUChar16* pattern, size_t patternLength) const;

    /**
      * Returns the index of the last occurrence of \a pattern, or
      * #RW_NPOS if the pattern is not found. The search begins at index
      * \a offset within self and extends through the end of self. The pattern
      * must be null-terminated. Returns an index value equal to length()
      * if the pattern length is zero.
      *
      * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
      * length().
      */
    inline size_t rindex(size_t offset, const RWUChar32* pattern) const;

    /**
     * Returns the index of the last occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * \a offset within self and extends through the end of self. The size
     * of the pattern is specified using \a patternLength. Returns an
     * index value equal to length() if the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t rindex(size_t offset, const RWUChar32* pattern, size_t patternLength) const;

    /**
     * Returns the index of the last occurrence of \a codeUnit, or
     * #RW_NPOS if the code unit is not found. The search begins at index
     * \a offset within self and extends through the end of self. This function
     * produces the same result as last(size_t,RWUChar16) const.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t rindex(size_t offset, RWUChar16 codeUnit) const;

    /**
     * Returns the index of the last occurrence of \a codePoint, or
     * #RW_NPOS if the code unit is not found. The search begins at
     * index \a offset within self and extends through the end of self.
     * This function produces the same result as last(size_t,RWUChar32) const.
     *
     * @throw RWBoundsErr Thrown if \a offset is greater than or equal to
     * length().
     */
    inline size_t rindex(size_t offset, RWUChar32 codePoint) const;

    /**
     * Returns the index of the last occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * \a offset within self and extends for \a length code units. Returns
     * an index value equal to the sum of \a offset and \a length if
     * the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t rindex(size_t offset, size_t length, const RWBasicUString& pattern) const;

    /**
     * Returns the index of the last occurrence of the specified range
     * in \a pattern, or #RW_NPOS if the pattern is not found. The
     * search begins at index \a offset within self and extends for
     * \a length code units.
     *
     * The pattern range begins at index \a patternOffset within \a pattern and
     * extends for \a patternLength code units. Returns an index value equal to
     * the sum of \a offset and \a length if the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length(), or the sum of \a patternOffset and \a patternLength
     * is greater than \link length() \a pattern.length() \endlink.
     */
    inline size_t rindex(size_t offset, size_t length, const RWBasicUString& pattern, size_t patternOffset, size_t patternLength) const;

    /**
     * Returns the index of the last occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * \a offset within self and extends for \a length code units. The pattern
     * must be null-terminated. Returns an index value equal to the sum of
     * \a offset and \a length if the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t rindex(size_t offset, size_t length, const RWUChar16* pattern) const;

    /**
     * Returns the index of the last occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * \a offset within self and extends for \a length code units. The size of
     * the pattern is specified using \a patternLength. Returns an index value
     * equal to the sum of \a offset and \a length if the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t rindex(size_t offset, size_t length, const RWUChar16* pattern, size_t patternLength) const;

    /**
     * Returns the index of the last occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * \a offset within self and extends for \a length code units. The pattern
     * must be null-terminated. Returns an index value equal to the sum of
     * \a offset and \a length if the pattern length is zero.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t rindex(size_t offset, size_t length, const RWUChar32* pattern) const;

    /**
     * Returns the index of the last occurrence of \a pattern, or
     * #RW_NPOS if the pattern is not found. The search begins at index
     * \a offset within self and extends for \a length code units. The size
     * of the pattern is specified using \a patternLength. Returns an index
     * value equal to the sum of \a offset and \a length if the pattern length
     * is zero.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t rindex(size_t offset, size_t length, const RWUChar32* pattern, size_t patternLength) const;

    /**
     * Returns the index of the last occurrence of \a codeUnit, or
     * #RW_NPOS if the code unit is not found. The search begins at index
     * \a offset within self and extends for \a length code units.
     * This method is equivalent to last(size_t,RWUChar16) const.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t rindex(size_t offset, size_t length, RWUChar16 codeUnit) const;

    /**
     * Returns the index of the last occurrence of \a codeUnit, or
     * #RW_NPOS if the code unit is not found. The search begins at index
     * \a offset within self and extends for \a length code units.
     * This method is equivalent to last(size_t,RWUChar32) const.
     *
     * @throw RWBoundsErr Thrown if the sum of \a offset and \a length is
     * greater than length().
     */
    inline size_t rindex(size_t offset, size_t length, RWUChar32 codeUnit) const;

    /**
     * Returns the number of bytes necessary to serialize the object.
     */
    RWspace binaryStoreSize() const;

    /**
     * Writes the binary contents of self to \a vos as an array of 16-bit
     * values.
     */
    void saveOn(RWvostream& vos) const;

    /**
     * Writes the binary contents of self to \a file as an array of
     * 16-bit values.
     */
    void saveOn(RWFile& file) const;

    /**
     * Returns the second or low UTF-16 surrogate (\c 0xDC00..\c 0xDFFF) for
     * a code point in the range \c 0x10000..\c 0x10FFFF.
     */
    inline static RWUChar16 lowSurrogate(RWUChar32 codePoint);

    /**
     * Returns the offset of the code unit in self that immediately
     * follows a prefix consisting of a repeated set of code units that
     * correspond to \a codePoint. If no such prefix exists, a value
     * of 0 is returned. If the contents of self consists entirely of
     * a repeated sequence of \a codePoint, a value equal to length()
     * is returned. The skip code point defaults to \c U+0020, the US-ASCII
     * space character.
     */
    size_t skipLeading(RWUChar32 codePoint = static_cast<RWUChar32>(0x0020)) const;

    /**
     * Returns the offset of the code unit in self that immediately
     * precedes a suffix consisting of a repeated set of code units
     * that correspond to \a codePoint. If no such suffix exists, a
     * value length() is returned. If the contents of self consists
     * entirely of a repeated sequence of \a codePoint, a value of #RW_NPOS
     * is returned. The skip code point defaults to \c U+0020, the US-ASCII
     * space character.
     */
    size_t skipTrailing(RWUChar32 codePoint = static_cast<RWUChar32>(0x0020)) const;

    /**
     * Returns the size of the code unit sequence contained in self.
     * This method is an alias for length().
     */
    inline size_t size() const;

    /**
     * Returns an #iterator that refers to the position of the first
     * code unit in self.
     */
    inline iterator begin();

    /**
     * Returns an #iterator that refers to the position after the last
     * code unit in self.
     */
    inline iterator end();

    /**
     * @copydoc cbegin()
     */
    inline const_iterator begin() const;

    /**
     * @copydoc cend()
     */
    inline const_iterator end() const;

    /**
     * Returns a #const_iterator that refers to the position of the
     * first code unit in self.
     */
    inline const_iterator cbegin() const;

    /**
     * Returns a #const_iterator that refers to the position after
     * the last code unit in self.
     */
    inline const_iterator cend() const;

    /**
     * Returns a #reverse_iterator that refers to the position of the
     * last code unit in self.
     */
    inline reverse_iterator rbegin();

    /**
     * Returns a #reverse_iterator that refers to the position before
     * the first code unit in self.
     */
    inline reverse_iterator rend();

    /**
     * @copydoc crbegin()
     */
    inline const_reverse_iterator rbegin() const;

    /**
     * @copydoc crend()
     */
    inline const_reverse_iterator rend() const;

    /**
     * Returns a #const_reverse_iterator that refers to the position
     * before the first code unit in self.
     */
    inline const_reverse_iterator crbegin() const;

    /**
     * Returns a #const_reverse_iterator that refers to the position
     * before the first code unit in self.
     */
    inline const_reverse_iterator crend() const;

    /**
     * Swaps the contents of self with that of \a ustr. This method
     * is provided to satisfy the C++ Standard Library requirements
     * for a container.
     */
    inline void swap(RWBasicUString& ustr);

    /**
     * Returns a UTF-32 encoded representation of the contents of self.
     *
     * @throw RWConversionErr Thrown if an illegal or truncated surrogate pair
     * is encountered during the conversion.
     */
    std::basic_string<RWUChar32> toUtf32() const;

    /**
     * Returns a UTF-8 encoded representation of the contents of self.
     */
    RWCString toUtf8(size_t numCodePoints = RW_NPOS) const;

    /**
     * Returns an RWWString containing a UTF-16 or UTF-32 representation
     * of the contents of self, depending on the size of \c wchar_t.
     * If \c sizeof(wchar_t) is \c 2, the RWWString is encoded in UTF-16.
     * If \c sizeof(wchar_t) is \c 4, the RWWString is encoded in UTF-32.
     *
     * This method produces Unicode-encoded wide-character strings.
     * This encoding may not match that of the execution environment.
     * The "native" encoding for \c wchar_t strings may be determined
     * by the current ANSI/POSIX LC_CTYPE locale and the execution environment.
     * Many environments use Unicode (ISO-10646) as the native encoding
     * for \c wchar_t. For those that do, the choice of encoding is
     * typically made based on the size of \c wchar_t.
     *
     * @throw RWConversionErr Thrown if an illegal or truncated surrogate pair
     * is encountered during the conversion to UTF-32. Such errors are
     * ignored if the result is encoded as UTF-16.
     */
    RWWString toWide() const;

    /**
     * Returns \a codePoint if it is valid. Invalid characters include:
     *
     * -  single surrogate code points (\c U+d800..\c U+dfff, 2048 code points)
     * -  the last two code points on each plane (\c U+__fffe and \c U+__ffff,
     *    34 code points)
     * -  \c U+fdd0..\c U+fdef (new with Unicode 3.1, 32 code points)
     * -  values greater than the last Unicode code point value, \c U+10ffff
     *
     * This method may be used anywhere an individual code point value
     * is passed to a method:
     *
     * @code
     * RWUChar32 indentationCharacter = ...
     * RWBasicUString indent(validateCodePoint(indentationCharacter),8);
     * @endcode
     *
     * @throw RWConversionErr Thrown if \a codePoint is not a valid Unicode
     * character.
     */
    static RWUChar32 validateCodePoint(RWUChar32 codePoint);

private:

    /**
     * @internal
     *
     * Returns a value that describes the lexical code point ordering between
     * the code unit sequence beginning at \a thisSpanStart in self at running
     * for \a thisSpanLength code units, and the code unit sequence identified by
     * \a that and running for \a thatSpanLength code units. If \a thatSpanLength
     * is RW_NPOS, the string \a that is assumed to be null-terminated and the
     * entire string is used for the comparison.
     *
     * The return value should be interpreted as follows:
     * - self appears before \a that if the return value is less than zero.
     * - self is identical to \a that if the return value is zero.
     * - self appears after \a that if the return value is greater than zero.
     *
     * If the two arrays have different lengths, the shorter array
     * appears before the longer array if both arrays are identical
     * over the length of the shorter array.
     */
    int doCodePointCompare(size_t thisSpanStart,
                           size_t thisSpanLength,
                           const RWUChar16* that,
                           size_t thatSpanLength) const;

    /**
     * @internal
     *
     * Returns a value that describes the lexical code point ordering between
     * the code unit sequence beginning at \a thisSpanStart in self at running
     * for \a thisSpanLength code units, and the code unit sequence identified by
     * \a that and running for \a thatSpanLength code units. If \a thatSpanLength
     * is RW_NPOS, the string \a that is assumed to be null-terminated and the
     * entire string is used for the comparison. The comparison occurs only after
     * \a that is converted to its equivalent UTF-16 array.
     *
     * The return value should be interpreted as follows:
     * - self appears before \a that if the return value is less than zero.
     * - self is identical to \a that if the return value is zero.
     * - self appears after \a that if the return value is greater than zero.
     *
     * If the two arrays have different lengths, the shorter array
     * appears before the longer array if both arrays are identical
     * over the length of the shorter array.
     */
    int doCodePointCompare(size_t thisSpanStart,
                           size_t thisSpanLength,
                           const RWUChar32* that,
                           size_t thatSpanLength) const;

    /**
     * @internal
     *
     * Returns a value that describes the lexical code point ordering between
     * the code unit sequence beginning at \a thisSpanStart in self at running
     * for \a thisSpanLength code units, and the code unit sequence identified by
     * \a that and running for \a thatSpanLength code units. If \a thatSpanLength
     * is RW_NPOS, the string \a that is assumed to be null-terminated and the
     * entire string is used for the comparison.
     *
     * The return value should be interpreted as follows:
     * - self appears before \a that if the return value is less than zero.
     * - self is identical to \a  that if the return value is zero.
     * - self appears after \a that if the return value is greater than zero.
     *
     * If the two arrays have different lengths, the shorter array
     * appears before the longer array if both arrays are identical
     * over the length of the shorter array.
     *
     * This method compares code unit values, not code point values.
     * This may not produce the desired result if either array contains
     * surrogate pairs or code unit values above the surrogate region
     * Use doCodePointCompare() if code point ordering is required.
     */
    int doCodeUnitCompare(size_t thisSpanStart,
                          size_t thisSpanLength,
                          const RWUChar16* that,
                          size_t thatSpanLength) const;

    /**
     * @internal
     *
     * Returns the index position of the first occurrence of the code point
     * pattern defined by \a pattern and \a patternLength, within the substring
     * in self defined by \a offset and \a length. The comparison occurs only after
     * \a pattern is converted to its equivalent UTF-16 array. Returns #RW_NPOS
     * if the search pattern was not found within the specified substring of self.
     *
     * If \a patternLength is #RW_NPOS this method determines
     * the length of \a pattern by looking for a terminating null.
     */
    size_t doIndexOfCodePointSequence(size_t offset,
                                      size_t length,
                                      const RWUChar32* pattern,
                                      size_t patternLength) const;

    /**
     * @internal
     *
     * Returns the index position of the first occurrence of the code unit
     * pattern defined by \a pattern and \a patternLength, within the substring
     * in self defined by \a offset and \a length. Returns #RW_NPOS if the
     * search pattern was not found within the specified substring of self.
     *
     * If \a patternLength is #RW_NPOS this method determines
     * the length of \a pattern by looking for a terminating null.
     */
    size_t doIndexOfCodeUnitSequence(size_t offset,
                                     size_t length,
                                     const RWUChar16* pattern,
                                     size_t patternLength) const;

    /**
     * @internal
     *
     * Returns the index position of the first occurrence of any code point in
     * \a codePointSet within the substring in self defined by \a offset and
     * \a length. Returns #RW_NPOS if none of the code points were
     * found within the specified substring of self.
     *
     * If \a numCodePoints is #RW_NPOS this method determines the length of
     * \a codePointSet by looking for a terminating null.
     */
    size_t doIndexOfCodePointSet(size_t offset,
                                 size_t length,
                                 const RWUChar32* codePointSet,
                                 size_t numCodePoints) const;

    /**
     * @internal
     *
     * Returns the index position of the first occurrence of any code unit in
     * \a codeUnitSet within the substring in self defined by \a offset and
     * \a length. Returns #RW_NPOS if none of the code units were
     * found within the specified substring of self.
     *
     * If \a numCodeUnits is #RW_NPOS this method determines the length of
     * \a codeUnitSet by looking for a terminating null.
     */
    size_t doIndexOfCodeUnitSet(size_t offset,
                                size_t length,
                                const RWUChar16* codeUnitSet,
                                size_t numCodeUnits) const;

    /**
     * @internal
     *
     * Creates space within self for use in replace operations. The code units
     * in the range specified by \a targetOffset and \a targetLength are
     * replaced, and that range is expanded or contracted as necessary to
     * provide an empty region of \a spaceRequired elements in size.
     */
    RWUChar16* doMakeSpace(size_t targetOffset,
                           size_t targetLength,
                           size_t spaceRequired);

    /**
     * @internal
     *
     * Returns the index position of the last occurrence of the code point
     * pattern defined by \a pattern and \a patternLength, within the substring
     * in self defined by \a offset and \a length. The comparison occurs only after
     * \a that is converted to its equivalent UTF-16 array. Returns #RW_NPOS if
     * the search pattern was not found within the specified substring of self.
     *
     * If \a patternLength is #RW_NPOS this method determines
     * the length of \a pattern by looking for a terminating null.
     */
    size_t doReverseIndexOfCodePointSequence(size_t offset,
            size_t length,
            const RWUChar32* pattern,
            size_t patternLength) const;

    /**
     * @internal
     *
     * Returns the index position of the last occurrence of the code unit
     * pattern defined by \a pattern and \a patternLength, within the substring
     * in self defined by \a offset and \a length. Returns #RW_NPOS if the
     * search pattern was not found within the specified substring of self.
     *
     * If \a patternLength is #RW_NPOS this method determines
     * the length of \a pattern by looking for a terminating null.
     */
    size_t doReverseIndexOfCodeUnitSequence(size_t offset,
                                            size_t length,
                                            const RWUChar16* pattern,
                                            size_t patternLength) const;

    /**
     * @internal
     *
     * Returns the relative position of the last occurrence of any code point in
     * \a codePointSet within the substring in self defined by \a offset and
     * \a length. Returns #RW_NPOS if none of the code points were
     * found within the specified substring of self.
     *
     * If \a numCodePoints is #RW_NPOS this method determines the length of
     * \a codePointSet by looking for a terminating null.
     */
    size_t doReverseIndexOfCodePointSet(size_t offset,
                                        size_t length,
                                        const RWUChar32* codePointSet,
                                        size_t numCodePoints) const;

    /**
     * @internal
     *
     * Returns the relative position of the last occurrence of any code unit in
     * \a codeUnitSet within the substring in self defined by \a offset and
     * \a length. Returns #RW_NPOS if none of the code units were
     * found within the specified substring of self.
     *
     * If \a numCodeUnits is #RW_NPOS this method determines the length of
     * \a codeUnitSet by looking for a terminating null.
     */
    size_t doReverseIndexOfCodeUnitSet(size_t offset,
                                       size_t length,
                                       const RWUChar16* codeUnitSet,
                                       size_t numCodeUnits) const;

    /**
     * @internal
     *
     * Replaces the range of code units within self starting at \a targetOffset
     * and extending \a targetLength units with an array of code units located at
     * \a sourceBegin and extending \a sourceLength units. If \a sourceLength
     * is #RW_NPOS this method determines the length of source by looking for
     * a terminating null.
     */
    RWBasicUString& doReplace(size_t targetOffset,
                              size_t targetLength,
                              const RWUChar16* sourceBegin,
                              size_t sourceLength);

    /**
     * @internal
     *
     * Replaces the range of code units within self starting at \a targetOffset
     * and extending \a targetLength units with an array of code units located at
     * \a sourceBegin and extending \a sourceLength units. The replacement occurs
     * only after the array \a sourceBegin is converted to its equivalent UTF-16
     * array. If \a sourceLength is #RW_NPOS this method determines the length of
     * source by looking for a terminating null.
     */
    RWBasicUString& doReplace(size_t targetOffset,
                              size_t targetLength,
                              const RWUChar32* sourceBegin,
                              size_t sourceLength);
    /**
     * @internal
     *
     * Replaces the range of code units within self defined by \a targetOffset
     * and \a targetLength with the value \a codeUnit repeated \a repeat times.
     */
    RWBasicUString& doReplace(size_t targetOffset,
                              size_t targetLength,
                              RWUChar16 codeUnit,
                              size_t repeat);

    /**
     * @internal
     *
     * Replaces the range of code units within self defined by \a targetOffset
     * and \a targetLength with the code units produced by converting the UCS-4
     * or UTF-32 code point \a codePoint into a UTF-16 representation and
     * repeating that result \a repeat times.
     */
    RWBasicUString& doReplace(size_t targetOffset,
                              size_t targetLength,
                              RWUChar32 codePoint,
                              size_t repeat);

    // Give operator access to StreamByteSource;
    friend RW_TOOLS_SYMBOLIC std::istream& operator>>(std::istream& is, RWBasicUString& ustr);

    /**
     * @internal
     *
     * Retrieves a single code point from a UTF-8 encoded sequence of bytes.
     * The bytes are retrieved from \a source and the resulting code point is
     * stored in \a codePoint. The number of bytes retrieved is added to
     * \a byteCount. A return value of \c true indicates that a code point
     * was stored in \a codePoint. A return value of \c false indicates that
     * \a source could provide no more bytes.
     *
     * @throw RWConversionErr Thrown to report an illegal,
     * irregular, or truncated UTF-8 sequence.
     */
    static bool getCodePointFromUtf8Source(ByteSource& source, RWUChar32& codePoint, size_t& byteCount);

    /**
     * @internal
     *
     * Initializes self from the UTF-8 encoded string \a source whose
     * length is given by \a length.
     */
    void initFromUtf8(const char* source, size_t length);

    /**
     * @internal
     *
     * Returns \c true if the contents of self reside in reference counted
     * storage and \c false if not.
     */
    inline bool isCounted() const;

    /**
     * @internal
     *
     * Returns \c true if the contents of self reside in the local buffer
     * and \c false if not.
     */
    inline bool isShort() const;

    /**
     * @internal
     *
     * Returns \c true if the content associated with self must be
     * copied before it can be modified, otherwise returns \c false.
     */
    inline bool requiresCopyOnWrite() const;

    /**
     * @internal
     *
     * Fills the supplied buffer with a UTF-32 representation of the
     * contents of self. The caller is responsible for allocating
     * and freeing the buffer.
     *
     * @throw RWConversionErr if an illegal or truncated
     * surrogate pair is encountered during the conversion.
     */
    RWUChar32* toUtf32(RWUChar32* buffer) const;

    /**
     * @internal
     *
     * Checks whether \a offset is greater than or equal to \a length.
     * If so, throws an RWBoundsErr exception.
     */
    static void reportBoundsCheckFailure(size_t offset, size_t length);

    // Defined out-of-line.
    class RW_TOOLS_GLOBAL Counter;

    friend class Counter; // workaround for compiler silliness

    /**
     * @internal
     *
     * @name Reference-Counted Buffer Management Methods
     *
     * These private methods are used to manage ownership and duration
     * of #RWUChar16 arrays. The life span of an array is governed by
     * a reference count. An array is deleted when it is no longer
     * referenced by any RWBasicUString instances. The Counter class
     * provides the necessary reference counting and deallocation functions.
     */

    //@{

    /**
     * @internal
     *
     * Increment the reference count of, and link \a counter.
     */
    inline void attach(Counter* counter);

    /**
     * @internal
     *
     * Decrement the reference count of, and unlink counter_ and
     * increment the reference count of, and link \a counter.
     */
    inline void transfer(Counter* counter);

    /**
     * @internal
     *
     * Unlink counter_ without decrementing its reference count.
     */
    inline Counter* orphan();

    /**
     * @internal
     *
     * Decrement the reference count of, and unlink counter_.
     */
    inline void detach();

    //@}

    /**
     * @internal
     *
     * The address of the first element in the current #RWUChar16 buffer.
     * This member always references the array data regardless of
     * the storage strategy. This member may reference:
     * - a client-owned read-only or writable array
     * - a short sequence stored in localArray_
     * - a reference counted, internally allocated array
     * - a reference counted, externally allocated array
     */
    RWUChar16* array_;

    /**
     * @internal
     *
     * Contains a pointer to the Counter instance, if any, currently
     * referenced by self, or zero if array_ references the local array
     * or client-owned storage.
     */
    Counter* counter_;

    /**
     * @internal
     *
     * The number of code units that may be stored without resorting to a
     * heap allocation.
     *
     * This value must be divisible by 4 and 8 to ensure that the overall size
     * of an RWBasicUString is evenly divisible by the word size of the
     * underlying architecture. An RWBasicUString requires 32 bytes on
     * a 32-bit machine and 40 bytes on a 64-bit machine.
     */
    enum { LocalArraySize = 8 };

    /**
     * @internal
     *
     * Local array used for short strings. Copies of short strings are stored
     * within an instance to avoid costly heap allocations.
     */
    RWUChar16 localArray_[LocalArraySize];

    /**
     * @internal
     *
     * The length of the code unit sequence referenced by array_.
     * This member is not used when the sequence is reference-counted.
     */
    size_t length_;

    /**
     * @internal
     *
     * Contains the total size of a client-owned writable buffer, excluding
     * any terminating null, or zero if the client array is static.
     * This member is not used when the current contents are stored
     * in the local array or in referenced-counted storage.
     */
    size_t capacity_;

};

/********************************************************************************/

/**
 * @brief Base class for subclasses that release client-allocated storage.
 *
 * Deallocator acts as an abstract base class for
 * subclasses that release client-allocated storage.
 *
 * Deallocator objects are required because RWBasicUString
 * cannot assume that local invocation of methods such as
 * <tt>operator delete[]</tt> or \c free() manipulate the same heap
 * as that of the client. Such a case might arise if, for instance,
 * an application contains some libraries that use debug versions of
 * heap management routines while others do not.
 *
 * A Deallocator subclass must override both doDeallocation()
 * methods to avoid the ambiguities produced should the subclass
 * declaration "hide" one of the superclass declarations of these
 * functions.
 */
class RW_TOOLS_GLOBAL RWBasicUString::Deallocator
{
public:

    /**
     * Forwards a deallocation request to a subclass-defined
     * non-\c const deallocation method.
     */
    inline void deallocate(RWUChar16* p);

    /**
     * Forwards a deallocation request to a subclass-defined \c const
     * deallocation method.
     */
    inline void deallocate(RWUChar16* p) const;

    /**
     * Required for virtual destruction.
     */
    virtual ~Deallocator();

protected:

    /**
     * Defined only to prevent direct instantiation of this class.
     */
    inline Deallocator();

    /**
     * A virtual function that must be implemented in each subclass.
     * A subclass implementation should take whatever actions are required
     * to deallocate the storage identified by \a p. The default implementation
     * of this function simply forwards to the \c const version.
     */
    virtual void doDeallocate(RWUChar16* p);

    /**
     * A pure virtual function that must be implemented in each subclass.
     * A subclass implementation should take whatever actions are required
     * to deallocate the storage identified by \a p.
     */
    virtual void doDeallocate(RWUChar16* p) const = 0;
};

/******************************************************************************/

/**
 * @brief Forwards deallocation requests to a static callback method.
 *
 * StaticDeallocator forwards deallocation requests to a
 * simple, client-defined, static callback method of the form:
 *
 * @code
 * void (*deallocator)(RWUChar16*)
 * @endcode
 */
class RW_TOOLS_GLOBAL RWBasicUString::StaticDeallocator : public RWBasicUString::Deallocator
{
public:

    /**
     * Constructs and initializes an instance with a pointer to a client-supplied
     * deallocation function. The \a destroyAfterDeallocate is used
     * to control the life-span of a StaticDeallocator instance. A value
     * of \c true causes the StaticDeallocator to delete itself
     * immediately after calling the deallocator function.
     */
    StaticDeallocator(void (*deallocator)(RWUChar16*), bool destroyAfterDeallocate = false);

    /**
     * Required for virtual destruction.
     */
    virtual ~StaticDeallocator();

protected:

    /**
     * Forwards deallocation requests to the client-supplied deallocator
     * method.
     */
    virtual void doDeallocate(RWUChar16* p) const;

#if !defined(RW_AMBIGUOUS_ON_CONST)
    /**
     * @copydoc doDeallocate(RWUChar16*)const
     */
    virtual void doDeallocate(RWUChar16* p) {
        RWBasicUString::Deallocator::doDeallocate(p);
    }
#endif

private:

    /**
     * The client-supplied deallocator callback function.
     */
    void (*deallocator_)(RWUChar16*);

    /**
     * Controls destruction of self.
     */
    bool destroyAfterDeallocate_;
};

/******************************************************************************/

/**
 * @internal
 *
 * This class:
 * - encapsulates the array of RWUChar16 instances that store
 *   the UTF-16 data
 * - is reference-counted by RWBasicUString
 * - deletes the data once the reference count becomes zero
 */
class RW_TOOLS_GLOBAL RWBasicUString::Counter
{
public:

    static Counter* make(const RWUChar16* source, size_t size);

    /**
     * Creates a new Counter instance with a capacity to store \a capacity
     * RWUChar16 elements. Copies \a size elements from \a source in self.
     */
    static Counter* make(const RWUChar16* source, size_t size, size_t capacity);

    /**
     * Creates a new Counter instance referencing an external RWUChar16 array
     * \a source of length \a size and capacity \a capacity. Uses \a deallocator
     * to delete the array pointed to by \a source once the reference count
     * reaches zero.
     */
    static Counter* make(RWUChar16* source, size_t size, size_t capacity, RWBasicUString::Deallocator* deallocator);

protected:

    /**
     * Constructs a Counter instance that assumes ownership of \a source, a buffer
     * of #RWUChar16 values that begins with a null-terminated string of \a length
     * elements and a capacity of \a capacity.
     *
     * The \a deallocator parameter is used to supply the Counter instance with
     * a Deallocator object that can be used to deallocate the storage referenced
     * by \a source.
     */
    Counter(RWUChar16* source, size_t length, size_t capacity, RWBasicUString::Deallocator* deallocator);

    /**
     * Destroys this instance and deallocates the storage used
     * for the #RWUChar16 array.
     */
    ~Counter();

public:

    /**
     * Returns the current reference count.
     * This method should only be used to test for sole-ownership,
     * i.e., a reference count of 1.
     */
    inline long references() const;

    /**
     * Increments the reference count.
     */
    inline void addReference();

    /**
     * Decrements the reference count and deletes this instance
     * if the result is zero.
     */
    void removeReference();

    /**
     * Returns \c true if the length of the string managed by self is zero,
     * otherwise returns \c false.
     *
     * This method requires constant time, while <tt>length() == 0</tt>
     * may require linear time.
     */
    inline bool empty() const;

    /**
     * Returns the total number of UTF-16 code units contained in self,
     * including embedded nulls.
     */
    size_t length() const;

    /**
     * Updates the total number of UTF-16 code units contained in self.
     */
    void length(size_t length);

    /**
     * Returns the total number of UTF-16 code units that may be stored in self.
     * This is the value specified during construction.
     */
    size_t capacity() const;

    /**
     * Returns a pointer to the #RWUChar16 array.
     */
    inline RWUChar16* data() const;

private:

    /**
     * The reference count.
     */
    RWTAtomic<long> counter_;

    /**
     * The array of #RWUChar16 values.
     */
    RWUChar16* array_;

    /**
     * The cached length of the #RWUChar16 sequence.
     */
    size_t length_;

    /**
     * The capacity of the storage referenced by array_.
     */
    size_t capacity_;

    /**
     * The Deallocator instance, if any, that will be used to free the
     * storage referenced by array_.
     */
    Deallocator* deallocator_;

};

/******************************************************************************/

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically less than \a rhs, otherwise returns
 * \c false. If the two arrays are identical over the length of
 * the shortest array, then the shorter array is considered less
 * than the longer array. The \a lhs array must be null-terminated.
 * This method cannot be used to compare arrays containing embedded
 * nulls. Use RWBasicUString::compareCodeUnits() in that case.
 */
inline bool
operator<(const RWUChar16* lhs, const RWBasicUString& rhs)
{
    RW_PRECONDITION(lhs != 0);
    return rhs.compareCodeUnits(lhs) > 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically less than or equal to
 * \a rhs, otherwise returns \c false. If the two arrays are identical
 * over the length of the shortest array, then the shorter array
 * is considered less than the longer array. The \a lhs array must
 * be null-terminated. This method cannot be used to compare arrays
 * containing embedded nulls. Use RWBasicUString::compareCodeUnits()
 * in that case.
 */
inline bool
operator<=(const RWUChar16* lhs, const RWBasicUString& rhs)
{
    RW_PRECONDITION(lhs != 0);
    return rhs.compareCodeUnits(lhs) >= 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs has the same length and is lexically
 * identical to \a rhs, otherwise returns \c false. The \a lhs array must
 * be null-terminated. This method cannot be used to compare arrays
 * containing embedded nulls. Use RWBasicUString::compareCodeUnits()
 * in that case.
 */
inline bool
operator==(const RWUChar16* lhs, const RWBasicUString& rhs)
{
    RW_PRECONDITION(lhs != 0);
    return rhs.compareCodeUnits(lhs) == 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs does not have the same length as, or
 * lexically differs from \a rhs, otherwise returns \c false. The \a lhs
 * array must be null-terminated. This method cannot be used to compare
 * arrays containing embedded nulls. Use RWBasicUString::compareCodeUnits()
 * in that case. This operator represents the logical negation of
 * operator==(const RWUChar16*,const RWBasicUString&).
 */
inline bool
operator!=(const RWUChar16* lhs, const RWBasicUString& rhs)
{
    RW_PRECONDITION(lhs != 0);
    return rhs.compareCodeUnits(lhs) != 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically greater than or equal
 * to \a rhs, otherwise returns \c false. If the two arrays are identical
 * over the length of the shortest array, then the longer array
 * is considered greater than the shorter array. The \a lhs array
 * must be null-terminated. This method cannot be used to compare
 * arrays containing embedded nulls. Use RWBasicUString::compareCodeUnits()
 * in that case.
 */
inline bool
operator>=(const RWUChar16* lhs, const RWBasicUString& rhs)
{
    RW_PRECONDITION(lhs != 0);
    return rhs.compareCodeUnits(lhs) <= 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically greater than \a rhs, otherwise
 * returns \c false. If the two arrays are identical over the length of
 * the shortest array, then the longer array is considered greater
 * than the shorter array. The \a lhs array must be null-terminated.
 * This method cannot be used to compare arrays containing embedded
 * nulls. Use RWBasicUString::compareCodeUnits() in that case.
 */
inline bool
operator>(const RWUChar16* lhs, const RWBasicUString& rhs)
{
    RW_PRECONDITION(lhs != 0);
    return rhs.compareCodeUnits(lhs) < 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically less than \a rhs, otherwise
 * returns \c false. If the two arrays are identical over the length of
 * the shortest array, then the shorter array is considered less
 * than the longer array. The \a rhs array must be null-terminated.
 * This method cannot be used to compare arrays containing embedded
 * nulls. Use RWBasicUString::compareCodeUnits() in that case.
 */
inline bool
operator<(const RWBasicUString& lhs, const RWUChar16* rhs)
{
    RW_PRECONDITION(rhs != 0);
    return lhs.compareCodeUnits(rhs) < 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically less than or equal to
 * \a rhs, otherwise returns \c false. If the two arrays are identical
 * over the length of the shortest array, then the shorter array
 * is considered less than the longer array. The \a rhs array must
 * be null-terminated. This method cannot be used to compare arrays
 * containing embedded nulls. Use RWBasicUString::compareCodeUnits()
 * in that case.
 */
inline bool
operator<=(const RWBasicUString& lhs, const RWUChar16* rhs)
{
    RW_PRECONDITION(rhs != 0);
    return lhs.compareCodeUnits(rhs) <= 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs has the same length and is lexically
 * identical to \a rhs, otherwise returns \c false. The \a rhs array must
 * be null-terminated. This method cannot be used to compare arrays
 * containing embedded nulls. Use RWBasicUString::compareCodeUnits()
 * in that case.
 */
inline bool
operator==(const RWBasicUString& lhs, const RWUChar16* rhs)
{
    RW_PRECONDITION(rhs != 0);
    return lhs.compareCodeUnits(rhs) == 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs does not have the same length as, or
 * lexically differs from \a rhs, otherwise returns \c false. The \a rhs
 * array must be null-terminated. This method cannot be used to compare
 * arrays containing embedded nulls. Use RWBasicUString::compareCodeUnits()
 * in that case. This operator represents the logical negation of
 * operator==(const RWBasicUString&, const RWUChar16*).
 */
inline bool
operator!=(const RWBasicUString& lhs, const RWUChar16* rhs)
{
    RW_PRECONDITION(rhs != 0);
    return lhs.compareCodeUnits(rhs) != 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically greater than or equal
 * to \a rhs, otherwise returns \c false. If the two arrays are identical
 * over the length of the shortest array, then the longer array
 * is considered greater than the shorter array. The \a rhs array
 * must be null-terminated. This method cannot be used to compare
 * arrays containing embedded nulls. Use RWBasicUString::compareCodeUnits()
 * in that case.
 */
inline bool
operator>=(const RWBasicUString& lhs, const RWUChar16* rhs)
{
    RW_PRECONDITION(rhs != 0);
    return lhs.compareCodeUnits(rhs) >= 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically greater than \a rhs, otherwise
 * returns \c false. If the two arrays are identical over the length of
 * the shortest array, then the longer array is considered greater
 * than the shorter array. The \a rhs array must be null-terminated.
 * This method cannot be used to compare arrays containing embedded
 * nulls. Use RWBasicUString::compareCodeUnits() in that case.
 */
inline bool
operator>(const RWBasicUString& lhs, const RWUChar16* rhs)
{
    RW_PRECONDITION(rhs != 0);
    return lhs.compareCodeUnits(rhs) > 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically less than \a rhs, otherwise
 * returns \c false. If the two arrays are identical over the length of
 * the shortest array, then the shorter array is considered less
 * than the longer array.
 */
inline bool
operator<(const RWBasicUString& lhs, const RWBasicUString& rhs)
{
    return lhs.compareCodeUnits(rhs) < 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically less than or equal to
 * \a rhs, otherwise returns \c false. If the two arrays are identical
 * over the length of the shortest array, then the shorter array
 * is considered less than the longer array.
 */
inline bool
operator<=(const RWBasicUString& lhs, const RWBasicUString& rhs)
{
    return lhs.compareCodeUnits(rhs) <= 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs has the same length and is lexically
 * identical to \a rhs, otherwise returns \c false.
 */
inline bool
operator==(const RWBasicUString& lhs, const RWBasicUString& rhs)
{
    return lhs.compareCodeUnits(rhs) == 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs does not have the same length as, or lexically
 * differs from \a rhs, otherwise returns \c false. This operator represents
 * the logical negation of operator==(const RWBasicUString&,const RWBasicUString&).
 */
inline bool
operator!=(const RWBasicUString& lhs, const RWBasicUString& rhs)
{
    return lhs.compareCodeUnits(rhs) != 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically greater than or equal
 * to \a rhs, otherwise returns \c false. If the two arrays are identical
 * over the length of the shortest array, then the longer array
 * is considered greater than the shorter array.
 */
inline bool
operator>=(const RWBasicUString& lhs, const RWBasicUString& rhs)
{
    return lhs.compareCodeUnits(rhs) >= 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically greater than \a rhs, otherwise
 * returns \c false. If the two arrays are identical over the length of
 * the shortest array, then the longer array is considered greater
 * than the shorter array.
 */
inline bool
operator>(const RWBasicUString& lhs, const RWBasicUString& rhs)
{
    return lhs.compareCodeUnits(rhs) > 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically less than \a rhs;  otherwise,
 * returns \c false. If the two arrays are identical over the length of
 * the shortest array, then the shorter array is considered less
 * than the longer array. The \a lhs array must be null-terminated.
 * This method cannot be used to compare arrays containing embedded
 * nulls. Use RWBasicUString::compareCodePoints() in that case.
 */
inline bool
operator<(const RWUChar32* lhs, const RWBasicUString& rhs)
{
    RW_PRECONDITION(lhs != 0);
    return rhs.compareCodePoints(lhs) > 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically less than or equal to
 * \a rhs, otherwise returns \c false. If the two arrays are identical
 * over the length of the shortest array, then the shorter array
 * is considered less than the longer array. The \a lhs array must
 * be null-terminated. This method cannot be used to compare arrays
 * containing embedded nulls. Use RWBasicUString::compareCodePoints()
 * in that case.
 */
inline bool
operator<=(const RWUChar32* lhs, const RWBasicUString& rhs)
{
    RW_PRECONDITION(lhs != 0);
    return rhs.compareCodePoints(lhs) >= 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs has the same length and is lexically
 * identical to \a rhs, otherwise returns \c false. The \a lhs array must
 * be null-terminated. This method cannot be used to compare arrays
 * containing embedded nulls. Use RWBasicUString::compareCodePoints()
 * in that case.
 */
inline bool
operator==(const RWUChar32* lhs, const RWBasicUString& rhs)
{
    RW_PRECONDITION(lhs != 0);
    return rhs.compareCodePoints(lhs) == 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs does not have the same length as, or
 * lexically differs from \a rhs, otherwise returns \c false. The \a lhs
 * array must be null-terminated. This method cannot be used to compare
 * arrays containing embedded nulls. Use RWBasicUString::compareCodePoints()
 * in that case. This operator represents the logical negation of
 * operator==(const RWUChar32*,const RWBasicUString&).
 */
inline bool
operator!=(const RWUChar32* lhs, const RWBasicUString& rhs)
{
    RW_PRECONDITION(lhs != 0);
    return rhs.compareCodePoints(lhs) != 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically greater than or equal
 * to \a rhs, otherwise returns \c false. If the two arrays are identical
 * over the length of the shortest array, then the longer array
 * is considered greater than the shorter array. The \a lhs array
 * must be null-terminated. This method cannot be used to compare
 * arrays containing embedded nulls. Use RWBasicUString::compareCodePoints()
 * in that case.
 */
inline bool
operator>=(const RWUChar32* lhs, const RWBasicUString& rhs)
{
    RW_PRECONDITION(lhs != 0);
    return rhs.compareCodePoints(lhs) <= 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically greater than \a rhs, otherwise returns
 * \c false. If the two arrays are identical over the length of
 * the shortest array, then the longer array is considered greater
 * than the shorter array. The \a lhs array must be null-terminated.
 * This method cannot be used to compare arrays containing embedded
 * nulls. Use RWBasicUString::compareCodePoints() in that case.
 */
inline bool
operator>(const RWUChar32* lhs, const RWBasicUString& rhs)
{
    RW_PRECONDITION(lhs != 0);
    return rhs.compareCodePoints(lhs) < 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically less than \a rhs, otherwise returns
 * \c false. If the two arrays are identical over the length of
 * the shortest array, then the shorter array is considered less
 * than the longer array. The \a rhs array must be null-terminated.
 * This method cannot be used to compare arrays containing embedded
 * nulls. Use RWBasicUString::compareCodePoints() in that case.
 */
inline bool
operator<(const RWBasicUString& lhs, const RWUChar32* rhs)
{
    RW_PRECONDITION(rhs != 0);
    return lhs.compareCodePoints(rhs) < 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically less than or equal to
 * \a rhs, otherwise returns \c false. If the two arrays are identical
 * over the length of the shortest array, then the shorter array
 * is considered less than the longer array. The \a rhs array must
 * be null-terminated. This method cannot be used to compare arrays
 * containing embedded nulls. Use RWBasicUString::compareCodePoints()
 * in that case.
 */
inline bool
operator<=(const RWBasicUString& lhs, const RWUChar32* rhs)
{
    RW_PRECONDITION(rhs != 0);
    return lhs.compareCodePoints(rhs) <= 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs has the same length and is lexically
 * identical to \a rhs, otherwise returns \c false. The \a rhs array must
 * be null-terminated. This method cannot be used to compare arrays
 * containing embedded nulls. Use RWBasicUString::compareCodePoints()
 * in that case.
 */
inline bool
operator==(const RWBasicUString& lhs, const RWUChar32* rhs)
{
    RW_PRECONDITION(rhs != 0);
    return lhs.compareCodePoints(rhs) == 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically greater than or equal
 * to \a rhs, otherwise returns \c false. If the two arrays are identical
 * over the length of the shortest array, then the longer array
 * is considered greater than the shorter array. The \a rhs array
 * must be null-terminated. This method cannot be used to compare
 * arrays containing embedded nulls. Use RWBasicUString::compareCodePoints()
 * in that case. This operator represents the logical negation of
 * operator==(const RWBasicUString&,const RWUChar32*).
 */
inline bool
operator!=(const RWBasicUString& lhs, const RWUChar32* rhs)
{
    RW_PRECONDITION(rhs != 0);
    return lhs.compareCodePoints(rhs) != 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs does not have the same length as, or
 * lexically differs from \a rhs, otherwise returns \c false. The \a rhs
 * array must be null-terminated. This method cannot be used to
 * compare arrays containing embedded nulls. Use RWBasicUString::compareCodePoints()
 * in that case.
 */
inline bool
operator>=(const RWBasicUString& lhs, const RWUChar32* rhs)
{
    RW_PRECONDITION(rhs != 0);
    return lhs.compareCodePoints(rhs) >= 0;
}

/**
 * @relates RWBasicUString
 *
 * Returns \c true if \a lhs is lexically greater than \a rhs, otherwise
 * returns \c false. If the two arrays are identical over the length of
 * the shortest array, then the longer array is considered greater
 * than the shorter array. The \a rhs array must be null-terminated.
 * This method cannot be used to compare arrays containing embedded
 * nulls. Use RWBasicUString::compareCodePoints() in that case.
 */
inline bool
operator>(const RWBasicUString& lhs, const RWUChar32* rhs)
{
    RW_PRECONDITION(rhs != 0);
    return lhs.compareCodePoints(rhs) > 0;
}

/******************************************************************************/

inline RWBasicUString
operator+(const RWBasicUString& lhs, const RWBasicUString& rhs)
{
    RWBasicUString result(lhs);
    return result.append(rhs);
}

inline RWBasicUString
operator+(const RWBasicUString& lhs, const RWUChar16* rhs)
{
    RWBasicUString result(lhs);
    return result.append(rhs);
}

inline RWBasicUString
operator+(const RWUChar16* lhs, const RWBasicUString& rhs)
{
    RWBasicUString result(lhs);
    return result.append(rhs);
}

inline RWBasicUString
operator+(const RWBasicUString& lhs, const RWUChar32* rhs)
{
    RWBasicUString result(lhs);
    return result.append(rhs);
}

inline RWBasicUString
operator+(const RWUChar32* lhs, const RWBasicUString& rhs)
{
    RWBasicUString result(lhs);
    return result.append(rhs);
}

inline RWBasicUString
operator+(const RWBasicUString& lhs, RWUChar16 rhs)
{
    RWBasicUString result(lhs);
    return result.append(rhs);
}

inline RWBasicUString
operator+(RWUChar16 lhs, const RWBasicUString& rhs)
{
    RWBasicUString result(lhs);
    return result.append(rhs);
}

inline RWBasicUString
operator+(const RWBasicUString& lhs, RWUChar32 rhs)
{
    RWBasicUString result(lhs);
    return result.append(rhs);
}

inline RWBasicUString
operator+(RWUChar32 lhs, const RWBasicUString& rhs)
{
    RWBasicUString result(lhs);
    return result.append(rhs);
}

/******************************************************************************/

/**
 * @brief Inserts the contents of a Unicode string into an output stream,
 * padding it with a designated fill character.
 *
 * Defines an iostream manipulator that can be used to insert the contents of
 * a Unicode string into an output stream, padding the Unicode string with a
 * specified fill character until the entire \c width() of the output stream
 * has been filled with code points.
 *
 * If the length of the Unicode string is greater than the \c width() of the
 * output stream, or if the \c width() of the output stream is zero, the entire
 * contents of the Unicode string is inserted into the stream and no padding occurs.
 *
 * A Pad instance is only valid as long as the source string remains unchanged.
 * Do not create persistent instances of this class. It should only be instantiated
 * as a temporary in an insertion expression.
 *
 * @code
 * RWBasicUString ustr = ...;
 * std::cout << RWBasicUString::Pad(ustr,static_cast<RWUChar32>('.')) << std::endl;
 * @endcode
 */
class RW_TOOLS_GLOBAL RWBasicUString::Pad
{
public:

    /**
     * Constructs a copy of \a source.
     */
    inline Pad(const Pad& source);

    /**
     * Constructs a manipulator instance. The default fill character, specified
     * in the second parameter, is the Unicode space character.
     */
    inline Pad(const RWBasicUString& ustr, RWUChar32 codePoint = RWBasicUString::DefaultFillCharacter);

    /**
     * The string to pad.
     */
    const RWUChar16* data_;

    /**
     * The code unit length of the array referenced by data_.
     */
    size_t length_;

    /**
     * The code point used to pad the string. The default code point is the Unicode
     * space character.
     */
    RWUChar32 codePoint_;

protected:

    /**
     * @internal
     *
     * Provides the means for constructing a pad object from a substring.
     */
    inline Pad(const RWUChar16* data, size_t length, RWUChar32 codePoint);

private:

    /* Declared private and left unimplemented to prevent unsupported operations */
    Pad();

    /* Declared private and left unimplemented to prevent unsupported operations */
    Pad& operator=(const Pad&);

};

inline
RWBasicUString::Pad::Pad(const Pad& source)
    : data_(source.data_),
      length_(source.length_),
      codePoint_(source.codePoint_)
{
}

inline
RWBasicUString::Pad::Pad(const RWBasicUString& ustr, RWUChar32 codePoint)
    : data_(ustr.data()),
      length_(ustr.length()),
      codePoint_(codePoint)
{
}

inline
RWBasicUString::Pad::Pad(const RWUChar16* data, size_t length, RWUChar32 codePoint)
    : data_(data),
      length_(length),
      codePoint_(codePoint)
{
}

/**
 * @relates RWBasicUString::Pad
 *
 * Writes the sequence of bytes that are produced when the contents of
 * the RWBasicUString used to construct \a pad are converted into a UTF-8
 * representation.
 *
 * If \c os.width() is greater than the number of code points contained
 * in the source string, the output is padded using the code point specified
 * as a parameter to \a pad. If \c os.width() is less than the number of code
 * points contained the source string, the  entire contents of the string
 * are inserted into the output stream.
 *
 * Any padding is inserted after the string if the \c ios_base::left format
 * flag is set, or before if \c ios_base::right is set or neither flag is set.
 *
 * @throw RWConversionErr Thrown to report conversion errors.
 * @throw std::ios_base::failure Thrown to report any errors
 * detected while performing stream operations.
 *
 * @sa RWBasicUString::Pad
 */
RW_TOOLS_SYMBOLIC std::ostream&
operator<<(std::ostream& os, const RWBasicUString::Pad& pad);

/******************************************************************************/

/**
 * @relates RWBasicUString
 *
 * Reads a UTF-8 encoded string from the input stream and stores it in the
 * provided RWBasicUString instance. Leading white space is always skipped
 * before storing any code points, regardless of the \c ios::skipws format
 * flag setting. Code points are then extracted until:
 *
 * - if the stream \c width() is greater than zero, until width() code points
 *   have been read
 * - a whitespace code point has been read
 * - the end of the input sequence has been reached
 *
 * The following Unicode characters are treated as whitespace delimiters:
 *
 * - U+0009 HORIZONTAL TABULATION
 * - U+000A LINE FEED
 * - U+000B VERTICAL TABULATION
 * - U+000C FORM FEED
 * - U+000D CARRIAGE RETURN
 * - U+001C FILE SEPARATOR
 * - U+001D GROUP SEPARATOR
 * - U+001E RECORD SEPARATOR
 * - U+001F UNIT SEPARATOR
 * - U+0020 SPACE
 * - U+1680 OGHAM SPACE MARK
 * - U+2000 EN QUAD
 * - U+2001 EM QUAD
 * - U+2002 EN SPACE
 * - U+2003 EM SPACE
 * - U+2004 THREE-PER-EM SPACE
 * - U+2005 FOUR-PER-EM SPACE
 * - U+2006 SIX-PER-EM SPACE
 * - U+2007 FIGURE SPACE
 * - U+2008 PUNCTUATION SPACE
 * - U+2009 THIN SPACE
 * - U+200A HAIR SPACE
 * - U+2028 LINE SEPARATOR
 * - U+2029 PARAGRAPH SEPARATOR
 * - U+205F MEDIUM MATHEMATICAL SPACE
 * - U+3000 IDEOGRAPHIC SPACE
 *
 * Unlike standard extractors, this extractor must consume and discard
 * the trailing whitespace delimiter. This behavior is due to the
 * nature of the  UTF-8 encoding scheme, which requires multiple
 * bytes to encode some whitespace characters.
 *
 * For the same reason, this extractor cannot support the
 * \c noskipws condition where the \c ios_base::skipws format flag
 * has been cleared because each extraction would consume one whitespace
 * character. A whitespace manipulator is not provided either, since
 * such a manipulator would consume the first non-whitespace character
 * following a sequence of whitespace characters.
 *
 * @throw RWConversionErr Thrown to report conversion errors.
 * @throw std::ios_base::failure Thrown to report any errors
 * detected while performing stream operations.
 *
 * If the virtual stream or file experience an error while extracting
 * the string, the operator returns the original string contents
 * unmodified. Check the virtual stream or file to determine if
 * an error occurred on extraction.
 */
//@{
RW_TOOLS_SYMBOLIC std::istream&
operator>>(std::istream& os, RWBasicUString& ustr);

RW_TOOLS_SYMBOLIC RWvistream&
operator>>(RWvistream& vis, RWBasicUString& ustr);

RW_TOOLS_SYMBOLIC RWFile&
operator>>(RWFile& file, RWBasicUString& ustr);

RW_TOOLS_SYMBOLIC RWvistream&
operator>>(RWvistream& vis, RWBasicUString*& ustrp);

RW_TOOLS_SYMBOLIC RWFile&
operator>>(RWFile& file, RWBasicUString*& ustrp);
//@}

/**
 * @relates RWBasicUString
 *
 * Converts the contents of \a ustr into a UTF-8 sequence and writes
 * that sequence to the output stream. The output sequence may contain
 * embedded nulls.
 *
 * If the \c width() of the output stream is greater than the number
 * of code points contained in the source string, the output is padded
 * using a single space fill character, \c [U+0020]. If the \c width()
 * is less than the number of code points contained in the source string,
 * the  entire contents of the string is inserted into the output stream.
 * Any padding is inserted after the string if the  \c ios_base::left format
 * flag is set, or before if \c ios_base::right is set or neither
 * flag is set.
 *
 * @throw RWConversionErr Thrown to report conversion errors.
 * @throw std::ios_base::failure Thrown to report any errors detected while
 * performing stream operations.
 */
//@{
RW_TOOLS_SYMBOLIC std::ostream&
operator<<(std::ostream& os, const RWBasicUString& ustr);

RW_TOOLS_SYMBOLIC RWvostream&
operator<<(RWvostream& vos, const RWBasicUString& ustr);

RW_TOOLS_SYMBOLIC RWFile&
operator<<(RWFile& file, const RWBasicUString& ustr);
//@}

/******************************************************************************/

inline void
RWBasicUString::Counter::addReference()
{
    ++counter_;
}

inline RWUChar16*
RWBasicUString::Counter::data() const
{
    return array_;
}

inline bool
RWBasicUString::Counter::empty() const
{
    if (length_ == RW_NPOS) {
        return array_[0] == 0;
    }
    else {
        return length_ == 0;
    }
}

inline long
RWBasicUString::Counter::references() const
{
    return counter_.load(rw_mem_order_acquire);
}

/******************************************************************************/

inline
RWBasicUString::RWBasicUString()
    : array_(localArray_), counter_(0), length_(0), capacity_(0)
{
    // Create a zero length, null-terminated string in the local buffer
    array_[0] = 0;
}

inline
RWBasicUString::RWBasicUString(RWUChar16* source, RWBasicUString::Deallocator* deallocator)
{
    RW_PRECONDITION(source != 0);
    RW_PRECONDITION(deallocator != 0);
    // Take ownership of source - create a reference counter to manage it
    attach(Counter::make(source, RW_NPOS, RW_NPOS, deallocator));
}

inline
RWBasicUString::RWBasicUString(RWUChar16* source, size_t n, RWBasicUString::Deallocator* deallocator)
{
    RW_PRECONDITION(source != 0);
    RW_PRECONDITION(deallocator != 0);
    // Take ownership of source - create a reference counter to manage it
    attach(Counter::make(source, n, n, deallocator));
}

inline
RWBasicUString::RWBasicUString(RWUChar16* source, size_t n, size_t cap, RWBasicUString::Deallocator* deallocator)
{
    RW_PRECONDITION(source != 0);
    RW_PRECONDITION(n == RW_NPOS || cap >= n);
    // Take ownership of source - create a reference counter to manage it
    attach(Counter::make(source, n, cap, deallocator));
}

#if !defined(RW_NO_RVALUE_REFERENCES)

inline
RWBasicUString::RWBasicUString(RWBasicUString && rhs)
    : array_(localArray_), counter_(0), length_(0)
{
    // Create a zero length, null-terminated string in the local buffer
    array_[0] = 0;

    rhs.swap(*this);
}

inline RWBasicUString&
RWBasicUString::operator=(RWBasicUString && rhs)
{
    if (&rhs != this) {
        rhs.swap(*this);
    }

    return *this;
}

#endif // !RW_NO_RVALUE_REFERENCES

inline void
RWBasicUString::attach(Counter* counter)
{
    counter->addReference();
    counter_ = counter;
    array_ = counter_->data();
    length_ = 0;
    capacity_ = 0;
}

inline RWBasicUString&
RWBasicUString::append(const RWBasicUString& source)
{
    return doReplace(length(), 0, source.data(), source.length());
}

inline RWBasicUString&
RWBasicUString::append(const RWBasicUString& source, size_t sourceOffset, size_t sourceLength)
{
    source.boundsCheckLength(sourceOffset, sourceLength);
    return doReplace(length(), 0, source.data() + sourceOffset, sourceLength);
}

inline RWBasicUString&
RWBasicUString::append(const RWUChar16* source)
{
    return doReplace(length(), 0, source, RW_NPOS);
}

inline RWBasicUString&
RWBasicUString::append(const RWUChar16* source, size_t sourceLength)
{
    return doReplace(length(), 0, source, sourceLength);
}

inline RWBasicUString&
RWBasicUString::append(const RWUChar32* source)
{
    return doReplace(length(), 0, source, RW_NPOS);
}

inline RWBasicUString&
RWBasicUString::append(const RWUChar32* source, size_t sourceLength)
{
    return doReplace(length(), 0, source, sourceLength);
}

inline RWBasicUString&
RWBasicUString::append(RWUChar16 codeUnit, size_t repeat)
{
    return doReplace(length(), 0, codeUnit, repeat);
}

inline RWBasicUString&
RWBasicUString::append(RWUChar32 codePoint, size_t repeat)
{
    return doReplace(length(), 0, codePoint, repeat);
}

inline size_t
RWBasicUString::boundsCheckIndex(size_t offset) const
{
    if (offset >= length()) {
        reportBoundsCheckFailure(offset, length());
    }
    return offset;
}

inline size_t
RWBasicUString::boundsCheckLength(size_t offset, size_t n) const
{
    if (offset + n > this->length()) {
        reportBoundsCheckFailure(offset + n, this->length());
    }
    return n;
}

inline size_t
RWBasicUString::capacity() const
{
    if (array_ == localArray_) {
        return LocalArraySize;
    }
    else if (counter_ != 0) {
        return counter_->capacity();
    }
    else {
        return capacity_;
    }
}

inline size_t
RWBasicUString::codeUnitLength() const
{
    if (counter_ != 0) {
        return counter_->length();
    }
    else {
        if (length_ == RW_NPOS) {
            // length has not been calculated yet
            // use capacity to limit length
            RWUChar16* start = array_;
            while (*start != 0) {
                start++;
            }
            const_cast<RWBasicUString*>(this)->length_ = static_cast<size_t>(start - array_);
        }
        return length_;
    }
}

inline size_t
RWBasicUString::codePointLength() const
{
    return codePointLength(array_, length());
}

inline /*static*/ size_t
RWBasicUString::codePointLength(const RWUChar16* array)
{
    RW_PRECONDITION(array != 0);
    size_t numCodePoints = 0;
    while (*array != 0) {
        numCodePoints++;
        // Note: array+1 may be terminating null
        // If so, the first surrogate must be treated as a code point
        if (isHighSurrogate(*array) && isLowSurrogate(*(array + 1))) {
            array += 2;
        }
        else {
            array++;
        }
    }
    return numCodePoints;
}

inline /*static*/ size_t
RWBasicUString::codePointLength(const RWUChar16* array, size_t length)
{
    RW_PRECONDITION(array != 0);
    size_t numCodePoints = 0;
    while (length > 0) {
        numCodePoints++;
        // Don't bother checking low surrogate if there wasn't room for it
        if (isHighSurrogate(*array) && length >= 2 && isLowSurrogate(*(array + 1))) {
            array += 2;
            length -= 2;
        }
        else {
            array++;
            length--;
        }
    }
    return numCodePoints;
}

inline /*static*/ size_t
RWBasicUString::codePointLength(const RWUChar32* array)
{
    RW_PRECONDITION(array != 0);
    const RWUChar32* start = array;
    while (*start != 0) {
        start++;
    }
    return static_cast<size_t>(start - array);
}

inline /*static*/ size_t
RWBasicUString::codeUnitLength(const RWUChar16* array)
{
    RW_PRECONDITION(array != 0);
    const RWUChar16* start = array;
    while (*start != 0) {
        start++;
    }
    return static_cast<size_t>(start - array);
}

inline /*static*/ size_t
RWBasicUString::codeUnitLength(const RWUChar32* array)
{
    RW_PRECONDITION(array != 0);
    const RWUChar32* start = array;
    size_t numCodeUnits = 0;
    while (*start != 0) {
        if (requiresSurrogatePair(*start)) {
            numCodeUnits += 2;
        }
        else {
            numCodeUnits++;
        }
        start++;
    }
    return numCodeUnits;
}

inline /*static*/ size_t
RWBasicUString::codeUnitLength(const RWUChar32* array, size_t length)
{
    RW_PRECONDITION(array != 0);
    size_t numCodeUnits = 0;
    while (length-- > 0) {
        if (requiresSurrogatePair(*array++)) {
            numCodeUnits += 2;
        }
        else {
            numCodeUnits++;
        }
    }
    return numCodeUnits;
}

inline int
RWBasicUString::compareCodePoints(const RWBasicUString& rhs) const
{
    return doCodePointCompare(0, length(), rhs.data(), rhs.length());
}

inline int
RWBasicUString::compareCodePoints(const RWBasicUString& rhs, size_t rhsOffset, size_t rhsLength) const
{
    rhs.boundsCheckLength(rhsOffset, rhsLength);
    return doCodePointCompare(0, length(), rhs.data() + rhsOffset, rhsLength);
}

inline int
RWBasicUString::compareCodePoints(const RWUChar16* rhs) const
{
    RW_PRECONDITION(rhs != 0);
    return doCodePointCompare(0, length(), rhs, RW_NPOS);
}

inline int
RWBasicUString::compareCodePoints(const RWUChar16* rhs, size_t rhsLength) const
{
    RW_PRECONDITION(rhs != 0);
    return doCodePointCompare(0, length(), rhs, rhsLength);
}

inline int
RWBasicUString::compareCodePoints(const RWUChar32* rhs) const
{
    RW_PRECONDITION(rhs != 0);
    return doCodePointCompare(0, length(), rhs, RW_NPOS);
}

inline int
RWBasicUString::compareCodePoints(const RWUChar32* rhs, size_t rhsLength) const
{
    RW_PRECONDITION(rhs != 0);
    return doCodePointCompare(0, length(), rhs, rhsLength);
}

inline int
RWBasicUString::compareCodeUnits(const RWBasicUString& rhs) const
{
    return doCodeUnitCompare(0, length(), rhs.data(), rhs.length());
}

inline int
RWBasicUString::compareCodeUnits(const RWBasicUString& rhs, size_t rhsOffset, size_t rhsLength) const
{
    rhs.boundsCheckLength(rhsOffset, rhsLength);
    return doCodeUnitCompare(0, length(), rhs.data() + rhsOffset, rhsLength);
}

inline int
RWBasicUString::compareCodeUnits(const RWUChar16* rhs) const
{
    RW_PRECONDITION(rhs != 0);
    return doCodeUnitCompare(0, length(), rhs, RW_NPOS);
}

inline int
RWBasicUString::compareCodeUnits(const RWUChar16* rhs, size_t rhsLength) const
{
    RW_PRECONDITION(rhs != 0);
    return doCodeUnitCompare(0, length(), rhs, rhsLength);
}

inline int
RWBasicUString::compareTo(const RWBasicUString& rhs) const
{
    return compareCodeUnits(rhs);
}

inline int
RWBasicUString::compareTo(const RWBasicUString& rhs, size_t rhsOffset, size_t rhsLength) const
{
    return compareCodeUnits(rhs, rhsOffset, rhsLength);
}

inline int
RWBasicUString::compareTo(const RWUChar16* rhs) const
{
    RW_PRECONDITION(rhs != 0);
    return compareCodeUnits(rhs);
}

inline int
RWBasicUString::compareTo(const RWUChar16* rhs, size_t rhsLength) const
{
    RW_PRECONDITION(rhs != 0);
    return compareCodeUnits(rhs, rhsLength);
}

inline int
RWBasicUString::compareTo(const RWUChar32* rhs) const
{
    RW_PRECONDITION(rhs != 0);
    return compareCodePoints(rhs);
}

inline int
RWBasicUString::compareTo(const RWUChar32* rhs, size_t rhsLength) const
{
    RW_PRECONDITION(rhs != 0);
    return compareCodePoints(rhs, rhsLength);
}

inline /*static*/ RWUChar32
RWBasicUString::computeCodePointValue(RWUChar16 highSurrogate, RWUChar16 lowSurrogate)
{
    const unsigned long SURROGATE_OFFSET = ((0xD800 << 10) + 0xDC00 - 0x10000);
    return
        (highSurrogate << 10UL) +
        RWUChar32(lowSurrogate) -
        RWUChar32(SURROGATE_OFFSET);
}

inline bool
RWBasicUString::contains(const RWBasicUString& pattern) const
{
    return doIndexOfCodeUnitSequence(0, length(), pattern.data(), pattern.length()) != RW_NPOS;
}

inline bool
RWBasicUString::contains(const RWBasicUString& pattern, size_t patternOffset, size_t patternLength) const
{
    pattern.boundsCheckLength(patternOffset, patternLength);
    return doIndexOfCodeUnitSequence(0, length(), pattern.data() + patternOffset, patternLength) != RW_NPOS;
}

inline bool
RWBasicUString::contains(const RWUChar16* pattern) const
{
    return doIndexOfCodeUnitSequence(0, length(), pattern, RW_NPOS) != RW_NPOS;
}

inline bool
RWBasicUString::contains(const RWUChar16* pattern, size_t patternLength) const
{
    return doIndexOfCodeUnitSequence(0, this->length(), pattern, patternLength) != RW_NPOS;
}

inline bool
RWBasicUString::contains(const RWUChar32* pattern) const
{
    return doIndexOfCodePointSequence(0, length(), pattern, RW_NPOS) != RW_NPOS;
}

inline bool
RWBasicUString::contains(const RWUChar32* pattern, size_t patternLength) const
{
    return doIndexOfCodePointSequence(0, length(), pattern, patternLength) != RW_NPOS;
}

inline bool
RWBasicUString::contains(RWUChar16 codeUnit) const
{
    return doIndexOfCodeUnitSequence(0, length(), &codeUnit, 1) != RW_NPOS;
}

inline bool
RWBasicUString::contains(RWUChar32 codePoint) const
{
    return doIndexOfCodePointSequence(0, length(), &codePoint, 1) != RW_NPOS;
}

inline bool
RWBasicUString::contains(size_t offset, const RWBasicUString& pattern) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodeUnitSequence(offset, length() - offset, pattern.data(), pattern.length()) != RW_NPOS;
}

inline bool
RWBasicUString::contains(size_t offset, const RWBasicUString& pattern, size_t patternOffset, size_t patternLength) const
{
    boundsCheckIndex(offset);
    pattern.boundsCheckLength(patternOffset, patternLength);
    return doIndexOfCodeUnitSequence(offset, length() - offset, pattern.data() + patternOffset, patternLength) != RW_NPOS;
}

inline bool
RWBasicUString::contains(size_t offset, const RWUChar16* pattern) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodeUnitSequence(offset, length() - offset, pattern, RW_NPOS) != RW_NPOS;
}

inline bool
RWBasicUString::contains(size_t offset, const RWUChar16* pattern, size_t patternLength) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodeUnitSequence(offset, length() - offset, pattern, patternLength) != RW_NPOS;
}

inline bool
RWBasicUString::contains(size_t offset, const RWUChar32* pattern) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodePointSequence(offset, length() - offset, pattern, RW_NPOS) != RW_NPOS;
}

inline bool
RWBasicUString::contains(size_t offset, const RWUChar32* pattern, size_t patternLength) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodePointSequence(offset, length() - offset, pattern, patternLength) != RW_NPOS;
}

inline bool
RWBasicUString::contains(size_t offset, RWUChar16 codeUnit) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodeUnitSequence(offset, length() - offset, &codeUnit, 1) != RW_NPOS;
}

inline bool
RWBasicUString::contains(size_t offset, RWUChar32 codePoint) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodePointSequence(offset, length() - offset, &codePoint, 1) != RW_NPOS;
}

inline bool
RWBasicUString::contains(size_t offset, size_t n, const RWBasicUString& pattern) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodeUnitSequence(offset, n, pattern.data(), pattern.length()) != RW_NPOS;
}

inline bool
RWBasicUString::contains(size_t offset, size_t n, const RWBasicUString& pattern, size_t patternOffset, size_t patternLength) const
{
    boundsCheckLength(offset, n);
    pattern.boundsCheckLength(patternOffset, patternLength);
    return doIndexOfCodeUnitSequence(offset, n, pattern.data() + patternOffset, patternLength) != RW_NPOS;
}

inline bool
RWBasicUString::contains(size_t offset, size_t n, const RWUChar16* pattern) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodeUnitSequence(offset, n, pattern, RW_NPOS) != RW_NPOS;
}

inline bool
RWBasicUString::contains(size_t offset, size_t n, const RWUChar16* pattern, size_t patternLength) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodeUnitSequence(offset, n, pattern, patternLength) != RW_NPOS;
}

inline bool
RWBasicUString::contains(size_t offset, size_t n, const RWUChar32* pattern) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodePointSequence(offset, n, pattern, RW_NPOS) != RW_NPOS;
}

inline bool
RWBasicUString::contains(size_t offset, size_t n, const RWUChar32* pattern, size_t patternLength) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodePointSequence(offset, n, pattern, patternLength) != RW_NPOS;
}

inline bool
RWBasicUString::contains(size_t offset, size_t n, RWUChar16 codeUnit) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodeUnitSequence(offset, n, &codeUnit, 1) != RW_NPOS;
}

inline bool
RWBasicUString::contains(size_t offset, size_t n, RWUChar32 codePoint) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodePointSequence(offset, n, &codePoint, 1) != RW_NPOS;
}

inline const RWUChar16*
RWBasicUString::data() const
{
    return array_;
}

inline void
RWBasicUString::detach()
{
    if (isCounted()) {
        counter_->removeReference();
        counter_ = 0;
    }
}

inline bool
RWBasicUString::empty() const
{
    if (counter_ != 0) {
        return counter_->empty();
    }
    else if (length_ == RW_NPOS) {
        return array_[0] == 0;
    }
    else {
        return length_ == 0;
    }
}

inline size_t
RWBasicUString::first(const RWBasicUString& codeUnitSet) const
{
    return doIndexOfCodeUnitSet(0, length(), codeUnitSet.data(), codeUnitSet.length());
}

inline size_t
RWBasicUString::first(const RWUChar16* codeUnitSet) const
{
    return doIndexOfCodeUnitSet(0, length(), codeUnitSet, RW_NPOS);
}

inline size_t
RWBasicUString::first(const RWUChar16* codeUnitSet, size_t numCodeUnits) const
{
    return doIndexOfCodeUnitSet(0, length(), codeUnitSet, numCodeUnits);
}

inline size_t
RWBasicUString::first(const RWUChar32* codePointSet) const
{
    return doIndexOfCodePointSet(0, length(), codePointSet, RW_NPOS);
}

inline size_t
RWBasicUString::first(const RWUChar32* codePointSet, size_t numCodePoints) const
{
    return doIndexOfCodePointSet(0, length(), codePointSet, numCodePoints);
}

inline size_t
RWBasicUString::first(RWUChar16 codeUnit) const
{
    return index(codeUnit);
}

inline size_t
RWBasicUString::first(RWUChar32 codePoint) const
{
    return doIndexOfCodePointSet(0, length(), &codePoint, 1);
}

inline size_t
RWBasicUString::first(size_t offset, const RWBasicUString& codeUnitSet) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodeUnitSet(offset, length() - offset, codeUnitSet.data(), codeUnitSet.length());
}

inline size_t
RWBasicUString::first(size_t offset, const RWUChar16* codeUnitSet) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodeUnitSet(offset, length() - offset, codeUnitSet, RW_NPOS);
}

inline size_t
RWBasicUString::first(size_t offset, const RWUChar16* codeUnitSet, size_t numCodeUnits) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodeUnitSet(offset, length() - offset, codeUnitSet, numCodeUnits);
}

inline size_t
RWBasicUString::first(size_t offset, const RWUChar32* codePointSet) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodePointSet(offset, length() - offset, codePointSet, RW_NPOS);
}

inline size_t
RWBasicUString::first(size_t offset, const RWUChar32* codePointSet, size_t numCodePoints) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodePointSet(offset, length() - offset, codePointSet, numCodePoints);
}

inline size_t
RWBasicUString::first(size_t offset, RWUChar16 codeUnit) const
{
    boundsCheckIndex(offset);
    return index(offset, codeUnit);
}

inline size_t
RWBasicUString::first(size_t offset, RWUChar32 codePoint) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodePointSet(offset, length() - offset, &codePoint, 1);
}

inline size_t
RWBasicUString::first(size_t offset, size_t n, const RWBasicUString& codeUnitSet) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodeUnitSet(offset, n, codeUnitSet.data(), codeUnitSet.length());
}

inline size_t
RWBasicUString::first(size_t offset, size_t n, const RWUChar16* codeUnitSet) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodeUnitSet(offset, n, codeUnitSet, RW_NPOS);
}

inline size_t
RWBasicUString::first(size_t offset, size_t n, const RWUChar16* codeUnitSet, size_t numCodeUnits) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodeUnitSet(offset, n, codeUnitSet, numCodeUnits);
}

inline size_t
RWBasicUString::first(size_t offset, size_t n, const RWUChar32* codePointSet) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodePointSet(offset, n, codePointSet, RW_NPOS);
}

inline size_t
RWBasicUString::first(size_t offset, size_t n, const RWUChar32* codePointSet, size_t numCodePoints) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodePointSet(offset, n, codePointSet, numCodePoints);
}

inline size_t
RWBasicUString::first(size_t offset, size_t n, RWUChar16 codeUnit) const
{
    boundsCheckLength(offset, n);
    return index(offset, n, codeUnit);
}

inline size_t
RWBasicUString::first(size_t offset, size_t n, RWUChar32 codePoint) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodePointSet(offset, n, &codePoint, 1);
}

inline /*static*/ RWUChar16
RWBasicUString::highSurrogate(RWUChar32 codePoint)
{
    return static_cast<RWUChar16>((codePoint >> 10) + 0xD7C0);
}

inline size_t
RWBasicUString::index(const RWBasicUString& pattern) const
{
    return doIndexOfCodeUnitSequence(0, length(), pattern.data(), pattern.length());
}

inline size_t
RWBasicUString::index(const RWBasicUString& pattern, size_t patternOffset, size_t patternLength) const
{
    pattern.boundsCheckLength(patternOffset, patternLength);
    return doIndexOfCodeUnitSequence(0, length(), pattern.data() + patternOffset, patternLength);
}

inline size_t
RWBasicUString::index(const RWUChar16* pattern) const
{
    return doIndexOfCodeUnitSequence(0, length(), pattern, RW_NPOS);
}

inline size_t
RWBasicUString::index(const RWUChar16* pattern, size_t patternLength) const
{
    return doIndexOfCodeUnitSequence(0, length(), pattern, patternLength);
}

inline size_t
RWBasicUString::index(const RWUChar32* pattern) const
{
    return doIndexOfCodePointSequence(0, length(), pattern, RW_NPOS);
}

inline size_t
RWBasicUString::index(const RWUChar32* pattern, size_t patternLength) const
{
    return doIndexOfCodePointSequence(0, length(), pattern, patternLength);
}

inline size_t
RWBasicUString::index(RWUChar16 codeUnit) const
{
    const RWUChar16* iter = array_;
    const RWUChar16* enditer = iter + length();
    while (iter < enditer && *iter != codeUnit) {
        iter++;
    }
    if (iter == enditer) {
        return RW_NPOS;
    }
    else {
        return static_cast<size_t>(iter - array_);
    }
}

inline size_t
RWBasicUString::index(RWUChar32 codePoint) const
{
    return doIndexOfCodePointSequence(0, length(), &codePoint, 1);
}

inline size_t
RWBasicUString::index(size_t offset, const RWBasicUString& pattern) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodeUnitSequence(offset, length() - offset, pattern.data(), pattern.length());
}

inline size_t
RWBasicUString::index(size_t offset, const RWBasicUString& pattern, size_t patternOffset, size_t patternLength) const
{
    boundsCheckIndex(offset);
    pattern.boundsCheckLength(patternOffset, patternLength);
    return doIndexOfCodeUnitSequence(offset, length() - offset, pattern.data() + patternOffset, patternLength);
}

inline size_t
RWBasicUString::index(size_t offset, const RWUChar16* pattern) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodeUnitSequence(offset, length() - offset, pattern, RW_NPOS);
}

inline size_t
RWBasicUString::index(size_t offset, const RWUChar16* pattern, size_t patternLength) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodeUnitSequence(offset, length() - offset, pattern, patternLength);
}

inline size_t
RWBasicUString::index(size_t offset, const RWUChar32* pattern) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodePointSequence(offset, length() - offset, pattern, RW_NPOS);
}

inline size_t
RWBasicUString::index(size_t offset, const RWUChar32* pattern, size_t patternLength) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodePointSequence(offset, length() - offset, pattern, patternLength);
}

inline size_t
RWBasicUString::index(size_t offset, RWUChar16 codeUnit) const
{
    boundsCheckIndex(offset);
    const RWUChar16* iter = array_ + offset;
    const RWUChar16* enditer = iter + length() - offset;
    while (iter < enditer && *iter != codeUnit) {
        iter++;
    }
    if (iter == enditer) {
        return RW_NPOS;
    }
    else {
        return static_cast<size_t>(iter - array_);
    }
}

inline size_t
RWBasicUString::index(size_t offset, RWUChar32 codePoint) const
{
    boundsCheckIndex(offset);
    return doIndexOfCodePointSequence(offset, length() - offset, &codePoint, 1);
}

inline size_t
RWBasicUString::index(size_t offset, size_t n, const RWBasicUString& pattern) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodeUnitSequence(offset, n, pattern.data(), pattern.length());
}

inline size_t
RWBasicUString::index(size_t offset, size_t n, const RWBasicUString& pattern, size_t patternOffset, size_t patternLength) const
{
    boundsCheckLength(offset, n);
    pattern.boundsCheckLength(patternOffset, patternLength);
    return doIndexOfCodeUnitSequence(offset, n, pattern.data() + patternOffset, patternLength);
}

inline size_t
RWBasicUString::index(size_t offset, size_t n, const RWUChar16* pattern) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodeUnitSequence(offset, n, pattern, RW_NPOS);
}

inline size_t
RWBasicUString::index(size_t offset, size_t n, const RWUChar16* pattern, size_t patternLength) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodeUnitSequence(offset, n, pattern, patternLength);
}

inline size_t
RWBasicUString::index(size_t offset, size_t n, const RWUChar32* pattern) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodePointSequence(offset, n, pattern, RW_NPOS);
}

inline size_t
RWBasicUString::index(size_t offset, size_t n, const RWUChar32* pattern, size_t patternLength) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodePointSequence(offset, n, pattern, patternLength);
}

inline size_t
RWBasicUString::index(size_t offset, size_t n, RWUChar16 codeUnit) const
{
    boundsCheckLength(offset, n);
    const RWUChar16* iter = array_ + offset;
    const RWUChar16* enditer = iter + n;
    while (iter < enditer && *iter != codeUnit) {
        iter++;
    }
    if (iter == enditer) {
        return RW_NPOS;
    }
    else {
        return static_cast<size_t>(iter - array_);
    }
}

inline size_t
RWBasicUString::index(size_t offset, size_t n, RWUChar32 codePoint) const
{
    boundsCheckLength(offset, n);
    return doIndexOfCodePointSequence(offset, n, &codePoint, 1);
}

inline RWBasicUString&
RWBasicUString::insert(size_t offset, const RWBasicUString& source)
{
    boundsCheckLength(offset, 0);
    return doReplace(offset, 0, source.data(), source.length());
}

inline RWBasicUString&
RWBasicUString::insert(size_t offset, const RWBasicUString& source, size_t sourceOffset, size_t sourceLength)
{
    boundsCheckLength(offset, 0);
    source.boundsCheckLength(sourceOffset, sourceLength);
    return doReplace(offset, 0, source.data() + sourceOffset, sourceLength);
}

inline RWBasicUString&
RWBasicUString::insert(size_t offset, const RWUChar16* source)
{
    boundsCheckLength(offset, 0);
    return doReplace(offset, 0, source, RW_NPOS);
}

inline RWBasicUString&
RWBasicUString::insert(size_t offset, const RWUChar16* source, size_t sourceLength)
{
    boundsCheckLength(offset, 0);
    return doReplace(offset, 0, source, sourceLength);
}

inline RWBasicUString&
RWBasicUString::insert(size_t offset, const RWUChar32* source)
{
    boundsCheckLength(offset, 0);
    return doReplace(offset, 0, source, RW_NPOS);
}

inline RWBasicUString&
RWBasicUString::insert(size_t offset, const RWUChar32* source, size_t sourceLength)
{
    boundsCheckLength(offset, 0);
    return doReplace(offset, 0, source, sourceLength);
}

inline RWBasicUString&
RWBasicUString::insert(size_t offset, RWUChar16 codeUnit, size_t repeat)
{
    boundsCheckLength(offset, 0);
    return doReplace(offset, 0, codeUnit, repeat);
}

inline RWBasicUString&
RWBasicUString::insert(size_t offset, RWUChar32 codePoint, size_t repeat)
{
    boundsCheckLength(offset, 0);
    return doReplace(offset, 0, codePoint, repeat);
}

inline bool
RWBasicUString::isCounted() const
{
    return !isShort() && counter_ != 0;
}

inline /*static*/ bool
RWBasicUString::isHighSurrogate(RWUChar16 ch)
{
    return (ch & 0xFFFFFC00) == 0xD800;
}

inline bool
RWBasicUString::isNull() const
{
    return empty();
}

inline /*static*/ bool
RWBasicUString::isLowSurrogate(RWUChar16 ch)
{
    return (ch & 0xFFFFFC00) == 0xDC00;
}

inline bool
RWBasicUString::isShort() const
{
    return array_ == localArray_;
}

inline /*static*/ bool
RWBasicUString::isUnicodeCharacter(RWUChar32 codePoint)
{
    // All code points below U+d800 are character code points,
    // so that boundary is tested first for efficiency.
    return (
               codePoint < 0xD800 ||
               (
                   codePoint > 0xDFFF &&
                   codePoint <= 0x10FFFF &&
                   (codePoint & 0xFFFE) != 0xFFFE &&
                   !(0xFDD0 <= codePoint && codePoint <= 0xFDEF)
               )
           );
}

inline size_t
RWBasicUString::length() const
{
    return codeUnitLength();
}

inline size_t
RWBasicUString::last(const RWBasicUString& codeUnitSet) const
{
    return doReverseIndexOfCodeUnitSet(0, length(), codeUnitSet.data(), codeUnitSet.length());
}

inline size_t
RWBasicUString::last(const RWUChar16* codeUnitSet) const
{
    return doReverseIndexOfCodeUnitSet(0, length(), codeUnitSet, RW_NPOS);
}

inline size_t
RWBasicUString::last(const RWUChar16* codeUnitSet, size_t numCodeUnits) const
{
    return doReverseIndexOfCodeUnitSet(0, length(), codeUnitSet, numCodeUnits);
}

inline size_t
RWBasicUString::last(const RWUChar32* codePointSet) const
{
    return doReverseIndexOfCodePointSet(0, length(), codePointSet, RW_NPOS);
}

inline size_t
RWBasicUString::last(const RWUChar32* codePointSet, size_t numCodePoints) const
{
    return doReverseIndexOfCodePointSet(0, length(), codePointSet, numCodePoints);
}

inline size_t
RWBasicUString::last(RWUChar16 codeUnit) const
{
    return doReverseIndexOfCodeUnitSet(0, length(), &codeUnit, 1);
}

inline size_t
RWBasicUString::last(RWUChar32 codePoint) const
{
    return doReverseIndexOfCodePointSet(0, length(), &codePoint, 1);
}

inline size_t
RWBasicUString::last(size_t offset, const RWBasicUString& codeUnitSet) const
{
    boundsCheckIndex(offset);
    return doReverseIndexOfCodeUnitSet(offset, length() - offset, codeUnitSet.data(), codeUnitSet.length());
}

inline size_t
RWBasicUString::last(size_t offset, const RWUChar16* codeUnitSet) const
{
    boundsCheckIndex(offset);
    return doReverseIndexOfCodeUnitSet(offset, length() - offset, codeUnitSet, RW_NPOS);
}

inline size_t
RWBasicUString::last(size_t offset, const RWUChar16* codeUnitSet, size_t numCodeUnits) const
{
    boundsCheckIndex(offset);
    return doReverseIndexOfCodeUnitSet(offset, length() - offset, codeUnitSet, numCodeUnits);
}

inline size_t
RWBasicUString::last(size_t offset, const RWUChar32* codePointSet) const
{
    boundsCheckIndex(offset);
    return doReverseIndexOfCodePointSet(offset, length() - offset, codePointSet, RW_NPOS);
}

inline size_t
RWBasicUString::last(size_t offset, const RWUChar32* codePointSet, size_t numCodePoints) const
{
    boundsCheckIndex(offset);
    return doReverseIndexOfCodePointSet(offset, length() - offset, codePointSet, numCodePoints);
}

inline size_t
RWBasicUString::last(size_t offset, RWUChar16 codeUnit) const
{
    boundsCheckIndex(offset);
    return doReverseIndexOfCodeUnitSet(offset, length() - offset, &codeUnit, 1);
}

inline size_t
RWBasicUString::last(size_t offset, RWUChar32 codePoint) const
{
    boundsCheckIndex(offset);
    return doReverseIndexOfCodePointSet(offset, length() - offset, &codePoint, 1);
}

inline size_t
RWBasicUString::last(size_t offset, size_t n, const RWBasicUString& codeUnitSet) const
{
    boundsCheckLength(offset, n);
    return doReverseIndexOfCodeUnitSet(offset, n, codeUnitSet.data(), codeUnitSet.length());
}

inline size_t
RWBasicUString::last(size_t offset, size_t n, const RWUChar16* codeUnitSet) const
{
    boundsCheckLength(offset, n);
    return doReverseIndexOfCodeUnitSet(offset, n, codeUnitSet, RW_NPOS);
}

inline size_t
RWBasicUString::last(size_t offset, size_t n, const RWUChar16* codeUnitSet, size_t numCodeUnits) const
{
    boundsCheckLength(offset, n);
    return doReverseIndexOfCodeUnitSet(offset, n, codeUnitSet, numCodeUnits);
}

inline size_t
RWBasicUString::last(size_t offset, size_t n, const RWUChar32* codePointSet) const
{
    boundsCheckLength(offset, n);
    return doReverseIndexOfCodePointSet(offset, n, codePointSet, RW_NPOS);
}

inline size_t
RWBasicUString::last(size_t offset, size_t n, const RWUChar32* codePointSet, size_t numCodePoints) const
{
    boundsCheckLength(offset, n);
    return doReverseIndexOfCodePointSet(offset, n, codePointSet, numCodePoints);
}

inline size_t
RWBasicUString::last(size_t offset, size_t n, RWUChar16 codeUnit) const
{
    boundsCheckLength(offset, n);
    return doReverseIndexOfCodeUnitSet(offset, n, &codeUnit, 1);
}

inline size_t
RWBasicUString::last(size_t offset, size_t n, RWUChar32 codePoint) const
{
    boundsCheckLength(offset, n);
    return doReverseIndexOfCodePointSet(offset, n, &codePoint, 1);
}

inline size_t
RWBasicUString::max_size() const
{
    return RW_NPOS - sizeof(Counter) - sizeof(RWBasicUString);
}

inline
RWBasicUString::operator const RWUChar16* () const
{
    return array_;
}

inline RWBasicUString&
RWBasicUString::operator+=(const RWBasicUString& rhs)
{
    return append(rhs);
}

inline RWBasicUString&
RWBasicUString::operator+=(const RWUChar16* rhs)
{
    return append(rhs);
}

inline RWBasicUString&
RWBasicUString::operator+=(const RWUChar32* rhs)
{
    return append(rhs);
}

inline RWBasicUString&
RWBasicUString::operator+=(RWUChar16 rhs)
{
    return append(rhs);
}

inline RWBasicUString&
RWBasicUString::operator+=(RWUChar32 rhs)
{
    return append(rhs);
}

inline RWUChar16&
RWBasicUString::operator[](short offset)
{
    boundsCheckIndex(size_t (offset));
    return *(array_ + offset);
}

inline RWUChar16&
RWBasicUString::operator[](int offset)
{
    boundsCheckIndex(size_t (offset));
    return *(array_ + offset);
}

inline RWUChar16&
RWBasicUString::operator[](long offset)
{
    boundsCheckIndex(size_t (offset));
    return *(array_ + offset);
}

inline RWUChar16&
RWBasicUString::operator[](unsigned short offset)
{
    boundsCheckIndex(size_t (offset));
    return *(array_ + offset);
}

inline RWUChar16&
RWBasicUString::operator[](unsigned int offset)
{
    boundsCheckIndex(size_t (offset));
    return *(array_ + offset);
}

inline RWUChar16&
RWBasicUString::operator[](unsigned long offset)
{
    boundsCheckIndex(size_t (offset));
    return *(array_ + offset);
}

#if !defined(RW_NO_LONG_LONG)
inline RWUChar16&
RWBasicUString::operator[](long long offset)
{
    boundsCheckIndex(static_cast<size_t>(offset));
    return *(array_ + offset);
}

inline RWUChar16&
RWBasicUString::operator[](unsigned long long offset)
{
    boundsCheckIndex(static_cast<size_t>(offset));
    return *(array_ + offset);
}
#endif

inline RWUChar16
RWBasicUString::operator[](short offset) const
{
    boundsCheckIndex(size_t (offset));
    return *(array_ + offset);
}

inline RWUChar16
RWBasicUString::operator[](int offset) const
{
    boundsCheckIndex(size_t (offset));
    return *(array_ + offset);
}

inline RWUChar16
RWBasicUString::operator[](long offset) const
{
    boundsCheckIndex(size_t (offset));
    return *(array_ + offset);
}

inline RWUChar16
RWBasicUString::operator[](unsigned short offset) const
{
    boundsCheckIndex(size_t (offset));
    return *(array_ + offset);
}

inline RWUChar16
RWBasicUString::operator[](unsigned int offset) const
{
    boundsCheckIndex(size_t (offset));
    return *(array_ + offset);
}

inline RWUChar16
RWBasicUString::operator[](unsigned long offset) const
{
    boundsCheckIndex(size_t (offset));
    return *(array_ + offset);
}

#if !defined(RW_NO_LONG_LONG)
inline RWUChar16
RWBasicUString::operator[](long long offset) const
{
    boundsCheckIndex(static_cast<size_t>(offset));
    return *(array_ + offset);
}

inline RWUChar16
RWBasicUString::operator[](unsigned long long offset) const
{
    boundsCheckIndex(static_cast<size_t>(offset));
    return *(array_ + offset);
}
#endif

inline RWUChar16&
RWBasicUString::operator()(size_t offset)
{
    boundsCheckIndex(offset);
    return *(array_ + offset);
}

inline RWUChar16
RWBasicUString::operator()(size_t offset) const
{
    boundsCheckIndex(offset);
    return *(array_ + offset);
}

inline RWBasicUString&
RWBasicUString::prepend(const RWBasicUString& source)
{
    return doReplace(0, 0, source.data(), source.length());
}

inline RWBasicUString&
RWBasicUString::prepend(const RWBasicUString& source, size_t sourceOffset, size_t sourceLength)
{
    source.boundsCheckLength(sourceOffset, sourceLength);
    return doReplace(0, 0, source.data() + sourceOffset, sourceLength);
}

inline RWBasicUString&
RWBasicUString::prepend(const RWUChar16* source)
{
    return doReplace(0, 0, source, RW_NPOS);
}

inline RWBasicUString&
RWBasicUString::prepend(const RWUChar16* source, size_t sourceLength)
{
    return doReplace(0, 0, source, sourceLength);
}

inline RWBasicUString&
RWBasicUString::prepend(const RWUChar32* source)
{
    return doReplace(0, 0, source, RW_NPOS);
}

inline RWBasicUString&
RWBasicUString::prepend(const RWUChar32* source, size_t sourceLength)
{
    return doReplace(0, 0, source, sourceLength);
}

inline RWBasicUString&
RWBasicUString::prepend(RWUChar16 codeUnit, size_t repeat)
{
    return doReplace(0, 0, codeUnit, repeat);
}

inline RWBasicUString&
RWBasicUString::prepend(RWUChar32 codePoint, size_t repeat)
{
    return doReplace(0, 0, codePoint, repeat);
}

inline /*static*/ bool
RWBasicUString::requiresSurrogatePair(RWUChar32 codeUnit)
{
    return codeUnit > 0xFFFF;
}

inline void
RWBasicUString::resize(size_t newCodeUnitLength, RWUChar16 codeUnit)
{
    if (newCodeUnitLength < length()) {
        doReplace(newCodeUnitLength, length() - newCodeUnitLength, codeUnit, 0);
    }
    else {
        doReplace(length(), 0, codeUnit, newCodeUnitLength - length());
    }
}

inline RWBasicUString&
RWBasicUString::remove(size_t offset)
{
    if (offset > 0) {
        boundsCheckIndex(offset);
    }
    RWUChar16 dummy;
    doReplace(offset, length() - offset, &dummy, 0);
    return *this;
}

inline RWBasicUString&
RWBasicUString::remove(size_t offset, size_t n)
{
    boundsCheckLength(offset, n);
    RWUChar16 dummy;
    doReplace(offset, n, &dummy, 0);
    return *this;
}

inline RWBasicUString&
RWBasicUString::replace(size_t offset, size_t n, const RWBasicUString& source)
{
    boundsCheckLength(offset, n);
    return doReplace(offset, n, source.data(), source.length());
}

inline RWBasicUString&
RWBasicUString::replace(size_t offset, size_t n, const RWBasicUString& source, size_t sourceOffset, size_t sourceLength)
{
    boundsCheckLength(offset, n);
    return doReplace(offset, n, source.data() + sourceOffset, sourceLength);
}

inline RWBasicUString&
RWBasicUString::replace(size_t offset, size_t n, const RWUChar16* source)
{
    boundsCheckLength(offset, n);
    return doReplace(offset, n, source, RW_NPOS);
}

inline RWBasicUString&
RWBasicUString::replace(size_t offset, size_t n, const RWUChar16* source, size_t sourceLength)
{
    boundsCheckLength(offset, n);
    return doReplace(offset, n, source, sourceLength);
}

inline RWBasicUString&
RWBasicUString::replace(size_t offset, size_t n, const RWUChar32* source)
{
    boundsCheckLength(offset, n);
    return doReplace(offset, n, source, RW_NPOS);
}

inline RWBasicUString&
RWBasicUString::replace(size_t offset, size_t n, const RWUChar32* source, size_t sourceLength)
{
    boundsCheckLength(offset, n);
    return doReplace(offset, n, source, sourceLength);
}

inline RWBasicUString&
RWBasicUString::replace(size_t offset, size_t n, RWUChar16 codeUnit, size_t repeat)
{
    boundsCheckLength(offset, n);
    return doReplace(offset, n, codeUnit, repeat);
}

inline RWBasicUString&
RWBasicUString::replace(size_t offset, size_t n, RWUChar32 codePoint, size_t repeat)
{
    boundsCheckLength(offset, n);
    return doReplace(offset, n, codePoint, repeat);
}

inline bool
RWBasicUString::requiresCopyOnWrite() const
{
    return !isShort() && ((counter_ != 0 && counter_->references() > 1) || (counter_ == 0 && capacity_ == 0));
}

inline size_t
RWBasicUString::rindex(const RWBasicUString& pattern) const
{
    return doReverseIndexOfCodeUnitSequence(0, length(), pattern.data(), pattern.length());
}

inline size_t
RWBasicUString::rindex(const RWBasicUString& pattern, size_t patternOffset, size_t patternLength) const
{
    pattern.boundsCheckLength(patternOffset, patternLength);
    return doReverseIndexOfCodeUnitSequence(0, length(), pattern.data() + patternOffset, patternLength);
}

inline size_t
RWBasicUString::rindex(const RWUChar16* pattern) const
{
    return doReverseIndexOfCodeUnitSequence(0, length(), pattern, RW_NPOS);
}

inline size_t
RWBasicUString::rindex(const RWUChar16* pattern, size_t patternLength) const
{
    return doReverseIndexOfCodeUnitSequence(0, length(), pattern, patternLength);
}

inline size_t
RWBasicUString::rindex(const RWUChar32* pattern) const
{
    return doReverseIndexOfCodePointSequence(0, length(), pattern, RW_NPOS);
}

inline size_t
RWBasicUString::rindex(const RWUChar32* pattern, size_t patternLength) const
{
    return doReverseIndexOfCodePointSequence(0, length(), pattern, patternLength);
}

inline size_t
RWBasicUString::rindex(RWUChar16 codeUnit) const
{
    const RWUChar16* enditer = array_;
    const RWUChar16* iter = enditer + length() - 1;
    while (iter >= enditer) {
        if (*iter == codeUnit) {
            return static_cast<size_t>(iter - array_);
        }
        iter--;
    }
    return RW_NPOS;
}

inline size_t
RWBasicUString::rindex(RWUChar32 codePoint) const
{
    return doReverseIndexOfCodePointSequence(0, length(), &codePoint, 1);
}

inline size_t
RWBasicUString::rindex(size_t offset, const RWBasicUString& pattern) const
{
    boundsCheckIndex(offset);
    return doReverseIndexOfCodeUnitSequence(offset, length() - offset, pattern.data(), pattern.length());
}

inline size_t
RWBasicUString::rindex(size_t offset, const RWBasicUString& pattern, size_t patternOffset, size_t patternLength) const
{
    boundsCheckIndex(offset);
    pattern.boundsCheckLength(patternOffset, patternLength);
    return doReverseIndexOfCodeUnitSequence(offset, length() - offset, pattern.data() + patternOffset, patternLength);
}

inline size_t
RWBasicUString::rindex(size_t offset, const RWUChar16* pattern) const
{
    boundsCheckIndex(offset);
    return doReverseIndexOfCodeUnitSequence(offset, length() - offset, pattern, RW_NPOS);
}

inline size_t
RWBasicUString::rindex(size_t offset, const RWUChar16* pattern, size_t patternLength) const
{
    boundsCheckIndex(offset);
    return doReverseIndexOfCodeUnitSequence(offset, length() - offset, pattern, patternLength);
}

inline size_t
RWBasicUString::rindex(size_t offset, const RWUChar32* pattern) const
{
    boundsCheckIndex(offset);
    return doReverseIndexOfCodePointSequence(offset, length() - offset, pattern, RW_NPOS);
}

inline size_t
RWBasicUString::rindex(size_t offset, const RWUChar32* pattern, size_t patternLength) const
{
    boundsCheckIndex(offset);
    return doReverseIndexOfCodePointSequence(offset, length() - offset, pattern, patternLength);
}

inline size_t
RWBasicUString::rindex(size_t offset, RWUChar16 codeUnit) const
{
    boundsCheckIndex(offset);
    const RWUChar16* enditer = array_ + offset;
    const RWUChar16* iter = array_ + length()  - 1;
    while (iter >= enditer) {
        if (*iter == codeUnit) {
            return static_cast<size_t>(iter - array_);
        }
        iter--;
    }
    return RW_NPOS;
}

inline size_t
RWBasicUString::rindex(size_t offset, RWUChar32 codePoint) const
{
    boundsCheckIndex(offset);
    return doReverseIndexOfCodePointSequence(offset, length() - offset, &codePoint, 1);
}

inline size_t
RWBasicUString::rindex(size_t offset, size_t n, const RWBasicUString& pattern) const
{
    boundsCheckLength(offset, n);
    return doReverseIndexOfCodeUnitSequence(offset, n, pattern.data(), pattern.length());
}

inline size_t
RWBasicUString::rindex(size_t offset, size_t n, const RWBasicUString& pattern, size_t patternOffset, size_t patternLength) const
{
    boundsCheckLength(offset, n);
    pattern.boundsCheckLength(patternOffset, patternLength);
    return doReverseIndexOfCodeUnitSequence(offset, n, pattern.data() + patternOffset, patternLength);
}

inline size_t
RWBasicUString::rindex(size_t offset, size_t n, const RWUChar16* pattern) const
{
    boundsCheckLength(offset, n);
    return doReverseIndexOfCodeUnitSequence(offset, n, pattern, RW_NPOS);
}

inline size_t
RWBasicUString::rindex(size_t offset, size_t n, const RWUChar16* pattern, size_t patternLength) const
{
    boundsCheckLength(offset, n);
    return doReverseIndexOfCodeUnitSequence(offset, n, pattern, patternLength);
}

inline size_t
RWBasicUString::rindex(size_t offset, size_t n, const RWUChar32* pattern) const
{
    boundsCheckLength(offset, n);
    return doReverseIndexOfCodePointSequence(offset, n, pattern, RW_NPOS);
}

inline size_t
RWBasicUString::rindex(size_t offset, size_t n, const RWUChar32* pattern, size_t patternLength) const
{
    boundsCheckLength(offset, n);
    return doReverseIndexOfCodePointSequence(offset, n, pattern, patternLength);
}

inline size_t
RWBasicUString::rindex(size_t offset, size_t n, RWUChar16 codeUnit) const
{
    boundsCheckLength(offset, n);
    const RWUChar16* enditer = array_ + offset;
    const RWUChar16* iter = enditer + n - 1;
    while (iter >= enditer) {
        if (*iter == codeUnit) {
            return static_cast<size_t>(iter - array_);
        }
        iter--;
    }
    return RW_NPOS;
}

inline size_t
RWBasicUString::rindex(size_t offset, size_t n, RWUChar32 codePoint) const
{
    boundsCheckLength(offset, n);
    return doReverseIndexOfCodePointSequence(offset, n, &codePoint, 1);
}

inline /*static*/ RWUChar16
RWBasicUString::lowSurrogate(RWUChar32 codePoint)
{
    return static_cast<RWUChar16>((codePoint & 0x03FF) | 0xDC00);
}

inline size_t
RWBasicUString::size() const
{
    return length();
}

inline RWBasicUString::iterator
RWBasicUString::begin()
{
    return array_;
}

inline RWBasicUString::iterator
RWBasicUString::end()
{
    return array_ + length();
}

inline RWBasicUString::const_iterator
RWBasicUString::begin() const
{
    return cbegin();
}

inline RWBasicUString::const_iterator
RWBasicUString::end() const
{
    return cend();
}

inline RWBasicUString::const_iterator
RWBasicUString::cbegin() const
{
    return data();
}

inline RWBasicUString::const_iterator
RWBasicUString::cend() const
{
    return data() + length();
}

inline RWBasicUString::reverse_iterator
RWBasicUString::rbegin()
{
    return reverse_iterator(end());
}

inline RWBasicUString::reverse_iterator
RWBasicUString::rend()
{
    return reverse_iterator(begin());
}

inline RWBasicUString::const_reverse_iterator
RWBasicUString::rbegin() const
{
    return crbegin();
}

inline RWBasicUString::const_reverse_iterator
RWBasicUString::rend() const
{
    return crend();
}

inline RWBasicUString::const_reverse_iterator
RWBasicUString::crbegin() const
{
    return const_reverse_iterator(cend());
}

inline RWBasicUString::const_reverse_iterator
RWBasicUString::crend() const
{
    return const_reverse_iterator(cbegin());
}

inline void
RWBasicUString::swap(RWBasicUString& rhs)
{
    rw_swap(rhs.counter_, counter_);
    rw_swap(rhs.length_, length_);

    const bool lhs_uses_local_array = (array_ ==     localArray_);
    const bool rhs_uses_local_array = (rhs.array_ == rhs.localArray_);

    if (lhs_uses_local_array || rhs_uses_local_array) {
        // swap the local array
        for (size_t i = 0; i < LocalArraySize; ++i) {
            rw_swap(rhs.localArray_[i], localArray_[i]);
        }

        // then the pointers
        rw_swap(rhs.array_, array_);

        if (rhs_uses_local_array) {
            array_ = localArray_;
        }

        if (lhs_uses_local_array) {
            rhs.array_ = rhs.localArray_;
        }
    }
    else {
        rw_swap(rhs.array_, array_);
    }
}

inline void
RWBasicUString::transfer(Counter* counter)
{
    if (counter_ != counter) {
        counter->addReference();
        if (isCounted()) {
            counter_->removeReference();
        }
        counter_ = counter;
        array_ = counter_->data();
    }
}

/******************************************************************************/

inline
RWBasicUString::Deallocator::Deallocator()
{
}

inline void
RWBasicUString::Deallocator::deallocate(RWUChar16* p)
{
    doDeallocate(p);
}

inline void
RWBasicUString::Deallocator::deallocate(RWUChar16* p) const
{
    doDeallocate(p);
}

/******************************************************************************/

inline void  /* static */
RWBasicUString::USE_DELETE(RWUChar16* p)
{
    delete[] p;
}

inline void  /* static */
RWBasicUString::USE_FREE(RWUChar16* p)
{
    free(static_cast<void*>(p));
}

inline void  /* static */
RWBasicUString::USE_NONE(RWUChar16*)
{
}

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an RWBasicUString.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<RWBasicUString> : public std::unary_function<RWBasicUString, size_t> {
    /**
     * Returns a hash of \a obj. This function is the equivalent of calling
     *
     * @code
     * obj.hash();
     * @endcode
     */
    size_t operator()(const RWBasicUString& obj) const {
        return obj.hash();
    }
};

#endif // rw_tools_uarray_h
