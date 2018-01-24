#ifndef RW_TOOLS_WIND_H
#define RW_TOOLS_WIND_H


/**********************************************************************
 *
 * Microsoft Windows related directives.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/rwwind.h#1 $
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


// For  Windows DLL builds,  define macros  for proper __declspec(export)
// decorations  when   building  the    library,   and __declspec(import)
// decorations for library clients
#if defined(RW_SHARED)
#  if defined(RW_TOOLS_SOURCE)
// building Tools library as a shared library
// Used for exporting generic container classes
#    define RW_TOOLS_EXPORT_G       RW_COMPILER_GLOBAL_EXPORT
// General export macro for all other classes
#    define RW_TOOLS_GLOBAL         RW_COMPILER_GLOBAL_EXPORT
#    define RW_TOOLS_SYMBOLIC       RW_COMPILER_SYMBOLIC_EXPORT


// Macro to support legacy behavior of the legacy COLLECTABLE macros.
#    define RW_TOOLS_ONCE_EXPORT    RW_COMPILER_SYMBOLIC_EXPORT
#    define RW_TOOLS_DONT_EXPORT

#  else // ! RW_TOOLS_SOURCE 

// _using_ Tools library
#    define RW_TOOLS_EXPORT_G
#    define RW_TOOLS_GLOBAL           RW_COMPILER_GLOBAL_IMPORT
#    define RW_TOOLS_SYMBOLIC         RW_COMPILER_GLOBAL_IMPORT

#    define RW_TOOLS_ONCE_EXPORT
#    define RW_TOOLS_DONT_EXPORT

#  endif // RW_TOOLS_SOURCE

// Macro to support legacy COLLECTABLE_DLL macros
#  if defined(RWCOLLECTABLE_DLL)
#    define RW_TOOLS_ALWAYS_EXPORT  RW_COMPILER_SYMBOLIC_EXPORT
#  else
#    define RW_TOOLS_ALWAYS_EXPORT  RW_COMPILER_GLOBAL_IMPORT
#  endif // RWCOLLECTABLE_DLL

#else // not shared library or not on Windows

#  define RW_TOOLS_EXPORT_G
#  define RW_TOOLS_GLOBAL
#  define RW_TOOLS_SYMBOLIC

#  define RW_TOOLS_ONCE_EXPORT
#  define RW_TOOLS_ALWAYS_EXPORT
#  define RW_TOOLS_DONT_EXPORT

#endif // RW_SHARED

#endif /* RW_TOOLS_WIND_H */
