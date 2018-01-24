#ifndef RW_TOOLS_STDIO_H
#define RW_TOOLS_STDIO_H

/**********************************************************************
 *
 * $Id: //tools/13/rw/tools/stdio.h#1 $
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
#include <stddef.h> // for size_t
#include <stdarg.h>

/**
 * @internal
 * @ingroup cstdlib
 *
 * Calls snprintf_s on platforms where it is available, otherwise calls snprintf.
 *
 * @note
 * In release builds, snprintf is preferred. snprintf_s will be invoked in
 * release builds if RW_SECURE_CHECK is defined.
 */
RW_TOOLS_SYMBOLIC int rw_snprintf_s(char* buf, size_t buf_size, const char* format, ...);

/**
 * @internal
 * @ingroup cstdlib
 *
 * Calls vsnprintf_s on platforms where it is available, otherwise calls vsnprintf.
 *
 * @note
 * In release builds, vsnprintf is preferred. vsnprintf_s will be invoked in
 * release builds if RW_SECURE_CHECK is defined.
 */
RW_TOOLS_SYMBOLIC int rw_vsnprintf_s(char* buf, size_t buf_size, const char* format, va_list args);

#endif
