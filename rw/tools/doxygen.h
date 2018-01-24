#ifndef rw_tools_doxygen_h__
#define rw_tools_doxygen_h__

//////////////////////////////////////////////////////////////////////////////
// Documentation related
//////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup spro_core_classes SourcePro Core
 *
 * SourcePro Core contains an extensive set of fundamental C++ components
 * capable of supporting a wide range of application development.
 */

/**
 * @defgroup tools_classes Essential Tools Module
 * @ingroup spro_core_classes
 *
 * The Essential Tools Module is an internationalized set of C++ classes
 * that provide the basic building blocks for most C++ applications.
 * The Essential Tools Module is the foundation for all Rogue Wave products.
 * Because all SourcePro C++ products depend on the Essential Tools Module,
 * SourcePro Core is included with each SourcePro C++ product.
 *
 * To help you quickly find the classes you are looking for, this section
 * organizes the Essential Tools Module classes into groups. Each section
 * provides a summary and a list of the classes in that group.
 */

/**
 * @defgroup atomics Atomics
 * @ingroup tools_classes
 *
 * Classes in this group provide abstractions for performing atomic
 * operations on integral types.
 */

/**
 * @defgroup date_and_time_classes Dates and Times
 * @ingroup tools_classes
 *
 * Classes in this group let you perform date and time tasks such as
 * calculating the number of days between two dates, calculating the day
 * of the week a date represents, or reading and writing days or times in
 * arbitrary formats.
 */

/**
 * @defgroup file_system_classes File System
 * @ingroup tools_classes
 *
 * Classes in this group let you work with things such as ordered collections
 * of associations of keys and values, fixed-length blocks to and from an
 * associated RWFile, page heaps, encapsulated binary file operations, and
 * disk allocation
 */

/**
 * @defgroup internationalization_classes Internationalization
 * @ingroup tools_classes
 *
 * Classes in this group let you internationalize your software with the
 * convenient and easy-to-use framework of class RWLocale, and to use class
 * RWZone to manipulate time zones and Daylight Saving Time. The entire
 * library is eight-bit clean, so you can use it with any eight-bit
 * character set.
 */

/**
 * @defgroup miscellaneous_classes Miscellaneous
 * @ingroup tools_classes
 *
 * Classes in this group let you work with things such as automatic
 * benchmarking, Model-View-Controller architecture, and elapsed time.
 */

/**
 * @defgroup rwcollectable_classes RWCollectable-derived
 * @ingroup tools_classes
 *
 * Classes in this group inherit from RWCollectable. These classes contain
 * virtual functions for identifying, hashing, comparing, storing, and
 * retrieving collectable objects.
 */

/**
 * @defgroup stl_based_collection_classes STL-based Collections
 * @ingroup tools_classes
 *
 * Classes in this group let you work with things such as pointer-based
 * collections of values, the iterator interface to the standard library-based
 * collections, pointer-based collections of associations of type, and so on.
 */

/**
 * @defgroup stl_extension_based_collection_classes STL Extension-based Collections
 * @ingroup tools_classes
 *
 * Classes in this group let you work with things such as collections of
 * mappings, variables, lists, associations of type implemented as hash
 * tables, and so on.
 */

/**
 * @defgroup string_processing_classes String Processing
 * @ingroup tools_classes
 *
 * Classes in this group let you perform string processing operations such as
 * manipulating single and multibyte strings with class RWCString, or choosing
 * class RWWString for wide character strings. These classes make it easy to
 * do concatenation, comparison, indexing (with optional bounds checking), I/O,
 * case changes, stripping, and many other functions.
 *
 * For Unicode character manipulation, please see the
 * \ref i18n_classes "Internationalization Classes".
 */

/**
 * @defgroup traditional_collection_classes Traditional Collections
 * @ingroup tools_classes
 *
 * Classes in this group let you work with items such as bit vectors,
 * singly and doubly linked lists, vectors of values or pointers,
 * parameterized queues, and stacks.
 */

/**
 * @defgroup scopeguard Scope Guard
 * @ingroup tools_classes
 *
 * Classes and functions in this group provide an implementation of the
 * Resource Acquisition Is Initialization (RAII) programming idiom. This
 * idiom is useful for writing exception-safe code. It relies on guarantees
 * provided by the C++ language to perform an action when control flow leaves
 * the current scope.
 */

/**
 * @defgroup scopeguard_functions rwtMakeScopeGuard Functions
 * @ingroup scopeguard
 *
 * The rwtMakeScopeGuard() functions are provided as a convenience mechanism
 * to construct scope guard instances given a function to invoke and parameters
 * to pass to that function.
 */

/**
 * @defgroup type_traits Type Traits
 * @ingroup tools_classes
 *
 * Classes and functions for accessing and manipulating type information.
 */

/**
 * @defgroup virtual_stream_classes Virtual Streams
 * @ingroup tools_classes
 *
 * Classes in this group let you perform streaming operations such as
 * transferring information between operating systems with the efficiency
 * of a binary stream. The endian streams mechanism, which keeps a record
 * of the operating environment where information originates, allows the
 * stream to be read on any system regardless of its native size or byte
 * order.
 */

/**
 * @internal
 * @defgroup cstdlib C Standard Library Extensions
 * @ingroup tools_classes
 *
 * Wrappers and extensions to functions defined in the C Standard Library.
 */

/**
 * @ingroup spro_core_classes
 * @deprecated As of SourcePro 13, use #RW_DISABLE_DEPRECATED instead.
 *
 * Deprecated. This macro may be defined as part of a build configuration
 * to disable support for deprecated constructs.
 */
#define RW_AVOID_PREPROCESSOR_PROBLEMS

/**
 * @ingroup spro_core_classes
 * This macro is defined automatically for build configurations that use
 * the classic copy-on-write string implementation.
 */
#define RW_COPY_ON_WRITE_STRING

/**
 * @ingroup spro_core_classes
 * This macro can be explicitly defined in a build configuration to disable
 * support for deprecated constructs.
 */
#define RW_DISABLE_DEPRECATED

/**
 * @ingroup spro_core_classes
 * This macro enables debugging construct such as assertions, preconditions,
 * postconditions, and conditional bounds checking. It is defined automatically
 * for build configurations that enable debugging.
 */
#define RW_DEBUG

/**
 * @ingroup spro_core_classes
 * This macro can be explicitly defined in a build configuration to enable
 * bounds checking for functions that use conditional bounds checking.
 */
#define RWBOUNDS_CHECK

/**
 * @ingroup date_and_time_classes
 * This macro enables checking for four-digit years when dealing with dates.
 * It is defined automatically for build configurations that enable the
 * appropriate advanced option.
 */
#define RW_CENTURY_REQD

/**
 * @ingroup spro_core_classes
 * This macro is defined automatically for build configurations that use a
 * threading library.
 */
#define RW_MULTI_THREAD

/**
 * @ingroup spro_core_classes
 * This macro can be explicitly defined to enable support for runtime-constraint
 * checking on platforms that support secure C library functions.
 *
 * @note
 * This macro is implicitly defined in RW_DEBUG builds.
 */
#define RW_SECURE_CHECK

#endif // rw_tools_doxygen_h__ 
