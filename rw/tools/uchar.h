
/**********************************************************************
 *
 * Definitions for ./tools/uchar.h
 *
 * This file contains the RWUChar16 type declaration.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/uchar.h#1 $
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

#ifndef rw_tools_uchar_h
#define rw_tools_uchar_h

#include <rw/compiler.h>

/*
 * This typedef declares a fundamental type-name for use in representing
 * a Unicode/ISO-10646 UCS-2 code point or a UTF-16 code unit.
 *
 * The Unicode Standard (now unified with ISO-10646) defines a
 * \ref coded_character_set. This character set assigns numeric values
 * from 0 to 0x10FFFF to a repertoire of \ref abstract_character
 * "abstract characters". This numerical values are called
 * \ref code_unit "code units".
 *
 * The ISO-10646 standard defines three \ref character_encoding_form
 * "character encoding forms" that may be used represent characters
 * within the combined character set. These froms are \ref UCS_2,
 * \ref UCS_4 and \ref UTF_16:
 * - The UCS-2 encoding form uses a 16-bit value to represent character
 * values in the range from 0 to 0xFFFF.
 * - The UCS-4 encoding uses a 32-bit value to represent all of the characters
 * in the range from 0 to 0x10FFFF. UCS-4 is equivalent to \ref UTF_32.
 * - The UTF-16 encoding form is used to encode a UCS-4 value as one or two
 * 16-bit \ref code_unit "code units". The complete set of valid UCS-2 values
 * are represented as single code units under UTF-16. Code points above the
 * the 16-bit boundary must be represented using a \ref surrogate_pair of
 * code units.
 *
 * The #RWUChar16 type may be used for UCS-2 code points or UTF-16 code units.
 * A 16-bit representation was chosen as the primary representation for
 * Unicode characters because nearly all of the abstract characters contained
 * int the character repertoires associated with common
 * \ref character_encoding_schemes map into the first 0xFFFF characters of
 * the Unicode character set -- the characters above the 16-bit boundary
 * are less commonly used.
 */
#if defined(DOXYGEN)
/**
 * @ingroup tools_classes
 *
 * Typedef for a UTF-16 code unit.
 *
 * @see #RWUChar32
 */
typedef unspecified_16bit_type RWUChar16;
#elif (RW_SIZEOF_WCHAR_T == 2)
typedef wchar_t RWUChar16;
#else
typedef unsigned short RWUChar16;
#endif

/*
 * This typedef declares a fundamental type-name for use in representing
 * a Unicode code point value.
 *
 * Unicode is a coded character set. It assigns numeric values
 * from 0 to 0x10FFFF known as \ref code_point "code points" to
 * \ref abstract_character "abstract characters".  An #RWUChar32
 * holds a single code point representing any Unicode character.
 */
#if defined(DOXYGEN)
/**
 * @ingroup tools_classes
 *
 * Typedef for a Unicode code point.
 *
 * @see #RWUChar16
 */
typedef unspecified_32bit_type RWUChar32;
#elif (RW_SIZEOF_WCHAR_T == 2)
typedef unsigned RWUChar32;
#else
typedef wchar_t RWUChar32;
#endif

#endif // rw_tools_uchar_h
