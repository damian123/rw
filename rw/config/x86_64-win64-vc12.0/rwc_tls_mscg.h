// Protect against multiple inclusions and mismatched configurations
#if defined _RW_tls_CONFIG
#  if _RW_tls_CONFIG != 0x57cd8b44
#    error  Error, two different configurations given for tools-headers
#  endif
#else
// Define configuration id
#define _RW_tls_CONFIG 0x57cd8b44

// Characterization Macros

#define RW_TOOLS_VERSION 0x13010000
#define RW_DEBUG 1
/* #define RW_SHARED 0 */
#define RW_MULTI_THREAD 1
#define RW_NO_UNAME /**/
#undef RW_NO_UNAME
#define RW_OS_WINDOWS
#define RW_OS_SYSNAME "Generic Windows"
#define RW_OS_RELEASE "6.1.0"
#define RW_OS_MAJOR 6
#define RW_OS_MINOR 1
#define RW_OS_MICRO 0
#define RW_OS_VERSION "0"
#define RW_LITTLE_ENDIAN /**/
#undef RW_LITTLE_ENDIAN
#define RW_LITTLE_ENDIAN  1
/* #define RW_NO_THREADS 0 */
/* #define RW_SOLARIS_THREADS 0 */
/* #define RW_POSIX_D10_THREADS 0 */
#define RW_WIN32_THREADS 1
#define RW_CRLF_CONVENTION 1
#define RW_CRITICAL_SECTION_T 40
/* #define RW_NO_NAMESPACE 0 */
/* #define RW_NO_USING_DIRECTIVE 0 */
#define RW_NO_IMPLICIT_INCLUSION 1
/* #define RW_NO_LONG_LONG 0 */
/* #define RW_NO_LONG_DOUBLE 0 */
#define RW_FLOATS_IMPLEMENTATIONS_SIZES /**/
#undef RW_FLOATS_IMPLEMENTATIONS_SIZES
#define RW_SIZEOF_FLOAT_IMPLEMENTATION    4
#define RW_SIZEOF_DOUBLE_IMPLEMENTATION   8
#define RW_SIZEOF_LDOUBLE_IMPLEMENTATION  8
#define RW_FLOATS_EXPONENTS_SIZES /**/
#undef RW_FLOATS_EXPONENT_SIZES
#define RW_FLOAT_EXPONENT_SIZE    8
#define RW_DOUBLE_EXPONENT_SIZE   11
#define RW_LDOUBLE_EXPONENT_SIZE  11
#define RW_SIZEOF /**/
#undef RW_SIZEOF
#define RW_SIZEOF_CHAR 1
#define RW_SIZEOF_UCHAR 1
#define RW_SIZEOF_SHORT 2
#define RW_SIZEOF_USHORT 2
#define RW_SIZEOF_INT 4
#define RW_SIZEOF_UINT 4
#define RW_SIZEOF_LONG_INT 4
#define RW_SIZEOF_ULONG_INT 4
#define RW_SIZEOF_LONG_LONG_INT 8
#define RW_SIZEOF_ULONG_LONG_INT 8
#define RW_SIZEOF_FLOAT 4
#define RW_SIZEOF_DOUBLE 8
#define RW_SIZEOF_LONG_DOUBLE 8
#define RW_SIZEOF_PTR 8
#define RW_SIZEOF_SIZE_T 8
#define RW_SIZEOF_WCHAR_T 2
/* #define RW_NO_USING_DIRECTIVE 0 */
/* #define RW_NO_STATIC_CONST_MEMBER_INIT 0 */
/* #define RW_NO_NESTED_QUOTES 0 */
/* #define RW_NO_NEW_TEMPLATE_SYNTAX 0 */
/* #define RW_NO_RECURSIVE_MACRO_DEF 0 */
/* #define RW_NO_POSTFIX 0 */
/* #define RW_NO_TEMPLINST_ON_BASE 0 */
/* #define RW_NO_VOLATILE_CONVERSION_OVERLOAD 0 */
/* #define M_PI 0 */
/* #define RW_NO_ANSI_TIMEZONE_NAMES 0 */
/* #define RW_NO_POSIX_TIMEZONE_NAMES 0 */
#define RW_NO_GETTIMEOFDAY 1
/* #define RW_NO_QUERYPERFORMANCE 0 */
#define RW_NO_LOCALTIME_R 1
/* #define RW_NO_STRERROR 0 */
#define RW_NO_TM_ZONE 1
/* #define RW_NO_LARGE_FILE_SUPPORT 0 */
#define RW_LIMITS /**/
#undef RW_LIMITS
#define RW_LLP64
#define RW_CHAR_MAX '\x7f'
#define RW_CHAR_MIN (-RW_CHAR_MAX - 1)
#define RW_SCHAR_MAX 0x7f
#define RW_SCHAR_MIN (-RW_SCHAR_MAX - 1)
#define RW_UCHAR_MAX 0xffU
#define RW_UCHAR_MIN 0x0U
#define RW_SHORT_MAX 0x7fff
#define RW_SHORT_MIN (-RW_SHORT_MAX - 1)
#define RW_USHORT_MAX 0xffffU
#define RW_USHORT_MIN 0x0U
#define RW_INT_MAX 0x7fffffff
#define RW_INT_MIN (-RW_INT_MAX - 1)
#define RW_UINT_MAX 0xffffffffU
#define RW_UINT_MIN 0x0U
#define RW_LONG_MAX 0x7fffffffL
#define RW_LONG_MIN (-RW_LONG_MAX - 1L)
#define RW_ULONG_MAX 0xffffffffUL
#define RW_ULONG_MIN 0x0UL
#define RW_LLONG_MAX 0x7fffffffffffffffLL
#define RW_LLONG_MIN (-RW_LLONG_MAX - 1LL)
#define RW_ULLONG_MAX 0xffffffffffffffffULL
#define RW_ULLONG_MIN 0x0ULL
#define RW_WCHAR_T_MAX 0xffffU
#define RW_WCHAR_T_MIN 0x0U
#define RW_SIZE_MAX 0xffffffffffffffffULL
#define RW_SIZE_MIN 0x0ULL
#define RW_DBL_DIG 15
#define RW_DBL_MANT_DIG 53
#define RW_DBL_MAX_10_EXP 308
#define RW_DBL_MAX_EXP 1024
#define RW_DBL_MIN_10_EXP -307
#define RW_DBL_MIN_EXP -1021
#define RW_DECIMAL_DIG 10
#define RW_FLT_DIG 6
#define RW_FLT_MANT_DIG 24
#define RW_FLT_MAX_10_EXP 38
#define RW_FLT_MAX_EXP 128
#define RW_FLT_MIN_10_EXP -37
#define RW_FLT_MIN_EXP -125
#define RW_FLT_RADIX 2
#define RW_LDBL_DIG 15
#define RW_LDBL_MANT_DIG 53
#define RW_LDBL_MAX_10_EXP 308
#define RW_LDBL_MAX_EXP 1024
#define RW_LDBL_MIN_10_EXP -307
#define RW_LDBL_MIN_EXP -1021
#define RW_DBL_MAX 1.7976931348623158e+308
#define RW_FLT_MAX 3.402823466e+38F
#define RW_LDBL_MAX 1.7976931348623158e+308
#define RW_DBL_EPSILON 2.2204460492503131e-016
#define RW_DBL_MIN 2.2250738585072014e-308
#define RW_FLT_EPSILON 1.192092896e-07F
#define RW_FLT_MIN 1.175494351e-38F
#define RW_LDBL_EPSILON 2.2204460492503131e-016
#define RW_LDBL_MIN 2.2250738585072014e-308
#define RW_NO_XDR 1
/* #define RW_XDR_OPS_PROTOTYPES 0 */
/* #define RW_XDR_OPS_SCOPE 0 */
#define RW_NO_POSIX_RE 1
#define RW_NO_NL_LANGINFO 1
#define RW_PRIz "ll"
#define RW_PRIt "ll"
#define RW_STDLIB_VERSION_MACROS /* RW stdlib version related macros*/
#undef RW_STDLIB_VERSION_MACROS
// not using RW C++ Standard Library
#define RW_NO_BASIC_STRING_COW 1
/* #define RW_NO_BASIC_STRING_SHRINK_TO_FIT 0 */
/* #define RW_ALLOCATOR_NO_REBIND 0 */
/* #define RW_NO_STREAM_LONG_LONG 0 */
#define RW_IOS_XALLOC_BROKEN 1
/* #define RW_NO_EXCEPTIONS 0 */
/* #define RW_NO_IO_SENTRY 0 */
/* #define RW_NO_IOSTD 0 */
/* #define RW_NO_POD_BASIC_STRING 0 */
#define RW_USE_FACET /**/
#undef RW_USE_FACET
#define RW_USE_FACET(Locale,Facet) std::use_facet < Facet > (Locale)
/* #define RW_NO_STREAMBUF_INT_TYPE 0 */
/* #define RW_NO_IOS_HEADER 0 */
#define RW_NO_NEW_HEADER 1
/* #define RW_NO_LIBC_IN_STD 0 */
/* #define RW_NO_MAP_PAIR_ALLOC 0 */
#define RW_NO_STD_TR1_FUNCTIONAL_HEADER 1
/* #define RW_NO_STD_TR1_RESULT_OF 0 */
#define RW_BROKEN_RESULT_OF_TEMPLATE_RESULT 1
/* #define RW_BROKEN_RVALUE_OVERLOAD_RESOLUTION 0 */
/* #define RW_NO_DECLTYPE 0 */
/* #define RW_NO_DECLVAL 0 */
/* #define RW_NO_EXPLICIT_CONVERSION_OPS 0 */
/* #define RW_NO_LAMBDA_EXPRESSIONS 0 */
#define RW_RVALUE_CONTAINER_SUPPORT /**/
#undef RW_RVALUE_CONTAINER_SUPPORT
#define RW_NO_STDVECTOR_RVALUE_SUPPORT 1
#define RW_NO_STDLIST_RVALUE_SUPPORT 1
#define RW_NO_STDDEQUE_RVALUE_SUPPORT 1
#define RW_NO_STDMAP_RVALUE_SUPPORT 1
#define RW_NO_STDMMAP_RVALUE_SUPPORT 1
#define RW_NO_STDSET_RVALUE_SUPPORT 1
#define RW_NO_STDMSET_RVALUE_SUPPORT 1
/* #define RW_NO_RVALUE_REFERENCES 0 */
/* #define RW_NO_STD_MOVE 0 */
/* #define RW_NO_STD_RESULT_OF 0 */
/* #define RW_NO_STD_MAP_ERASE_RETURN 0 */
/* #define RW_NO_STD_MULTIMAP_ERASE_RETURN 0 */
/* #define RW_NO_STD_MULTISET_ERASE_RETURN 0 */
/* #define RW_NO_STD_SET_ERASE_RETURN 0 */
/* #define RW_NO_STD_VECTOR_DATA 0 */
#define RW_NO_NOEXCEPT 1
#define RW_NO_NEWDEL_NOEXCEPT_SPEC 1
/* #define RW_COPY_ON_WRITE_STRING 0 */
/* #define RW_CENTURY_REQD 0 */


inline const char * rw_tls_library_id() { return "rw_tls_Edition_13_1_0__msd__2015_8_28_15_46_53_842"; }
static const char *  rw_tls_Edition_13_1_0__msd__2015_8_28_15_46_53_842 = rw_tls_library_id();

#endif
