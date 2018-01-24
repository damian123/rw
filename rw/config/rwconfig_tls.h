//This header file is updated automatically by the
//configuration generation system.  Unauthorized changes
//to this file may cause the update to fail resulting in
//unusable libraries.

#include "rwconfig.h"

#if defined(_MSC_VER)
#	if (_MSC_VER==1800)
#		if defined(_M_IX86)
#			if defined(_RWCONFIG_mo)
#				include "i686-win32-vc12.0/rwc_tls_mo.h"
#			elif defined(_RWCONFIG_mcg)
#				include "i686-win32-vc12.0/rwc_tls_mcg.h"
#			elif defined(_RWCONFIG_mso)
#				include "i686-win32-vc12.0/rwc_tls_mso.h"
#			elif defined(_RWCONFIG_mscg)
#				include "i686-win32-vc12.0/rwc_tls_mscg.h"
#			else
#				error "No _RWCONFIG_<tag> found"
#			endif
#		elif defined(_M_X64)
#			if defined(_RWCONFIG_mo)
#				include "x86_64-win64-vc12.0/rwc_tls_mo.h"
#			elif defined(_RWCONFIG_mcg)
#				include "x86_64-win64-vc12.0/rwc_tls_mcg.h"
#			elif defined(_RWCONFIG_mso)
#				include "x86_64-win64-vc12.0/rwc_tls_mso.h"
#			elif defined(_RWCONFIG_mscg)
#				include "x86_64-win64-vc12.0/rwc_tls_mscg.h"
#			else
#				error "No _RWCONFIG_<tag> found"
#			endif
#		else
#			error "Unsupported CPU architecture"
#		endif
#	elif (_MSC_VER==1900)
#		if defined(_M_IX86)
#			if defined(_RWCONFIG_mo)
#				include "i686-win32-vc14.0/rwc_tls_mo.h"
#			elif defined(_RWCONFIG_mcg)
#				include "i686-win32-vc14.0/rwc_tls_mcg.h"
#			elif defined(_RWCONFIG_mso)
#				include "i686-win32-vc14.0/rwc_tls_mso.h"
#			elif defined(_RWCONFIG_mscg)
#				include "i686-win32-vc14.0/rwc_tls_mscg.h"
#			else
#				error "No _RWCONFIG_<tag> found"
#			endif
#		elif defined(_M_X64)
#			if defined(_RWCONFIG_mo)
#				include "x86_64-win64-vc14.0/rwc_tls_mo.h"
#			elif defined(_RWCONFIG_mcg)
#				include "x86_64-win64-vc14.0/rwc_tls_mcg.h"
#			elif defined(_RWCONFIG_mso)
#				include "x86_64-win64-vc14.0/rwc_tls_mso.h"
#			elif defined(_RWCONFIG_mscg)
#				include "x86_64-win64-vc14.0/rwc_tls_mscg.h"
#			else
#				error "No _RWCONFIG_<tag> found"
#			endif
#		else
#			error "Unsupported CPU architecture"
#		endif
#	else
#		error "Unsupported Microsoft compiler version"
#	endif
#elif defined(__GNUC__)
#	if (__GNUC__==4 && __GNUC_MINOR__==8)
#		if defined(__i386__)
#			if defined(_RWCONFIG_mo)
#				include "i686-rhel5.0-gcc4.8/rwc_tls_mo.h"
#			elif defined(_RWCONFIG_mcg)
#				include "i686-rhel5.0-gcc4.8/rwc_tls_mcg.h"
#			elif defined(_RWCONFIG_mso)
#				include "i686-rhel5.0-gcc4.8/rwc_tls_mso.h"
#			elif defined(_RWCONFIG_mscg)
#				include "i686-rhel5.0-gcc4.8/rwc_tls_mscg.h"
#			else
#				error "No _RWCONFIG_<tag> found"
#			endif
#		elif defined(__x86_64__)
#			if defined(_RWCONFIG_mo)
#				include "x86_64-rhel5.0-gcc4.8/rwc_tls_mo.h"
#			elif defined(_RWCONFIG_mcg)
#				include "x86_64-rhel5.0-gcc4.8/rwc_tls_mcg.h"
#			elif defined(_RWCONFIG_mso)
#				include "x86_64-rhel5.0-gcc4.8/rwc_tls_mso.h"
#			elif defined(_RWCONFIG_mscg)
#				include "x86_64-rhel5.0-gcc4.8/rwc_tls_mscg.h"
#			else
#				error "No _RWCONFIG_<tag> found"
#			endif
#		else
#			error "Unsupported CPU architecture"
#		endif
#	else
#		error "Unsupported GCC compiler version"
#	endif
#else
#	error "Unsupported compiler"
#endif
