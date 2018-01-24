// Protect against multiple inclusions and mismatched configurations
#if defined _RW_mth_CONFIG
#  if _RW_mth_CONFIG != 0xbd0ecc57
#    error  Error, two different configurations given for mathcore-source
#  endif
#else
// Define configuration id
#define _RW_mth_CONFIG 0xbd0ecc57

// Characterization Macros
#include <rw/config/rwconfig_tls.h>

#define RW_MATH_VERSION 0x13010000
#define RW_C_M_PI /**/
#undef RW_C_M_PI
#define RW_M_PI 3.141592653589793
/* #define RW_MATH_NO_STD_NEW_HANDLER 0 */
#define RWDEBUG 1
#define RWSHARED 1
#define RWSTDLIBTYPE 2
/* #define RW_USING_SUNPERF 0 */
#define RW_USING_MKL 1
/* #define RW_USING_CLAPACK 0 */
/* #define RW_STREAM_EXTRACTION_COMPLEX_BUG 0 */


inline const char * rw_mth_library_id() { return "rw_mth_Edition_13_1_0__md__2015_8_28_17_57_10_210"; }
static const char *  rw_mth_Edition_13_1_0__md__2015_8_28_17_57_10_210 = rw_mth_library_id();

#if defined(__WIN32__) && defined(RWSHARED)
// map from new HAL build macro to old macro
#  define RWDLL 1
// Set the product specific build macro if appropriate.
#  if defined(_RWBUILD_mth)
#    define _RWBUILDDLL
#  endif
#endif

// macros defined by Essential Tools Module
// #define RW_MULTI_THREAD 1

#endif
