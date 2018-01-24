#ifndef RW_TOOLS_TRAITS_RWTISFUNCTION_H
#define RW_TOOLS_TRAITS_RWTISFUNCTION_H

/***************************************************************************
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
 * $Id: //tools/13/rw/tools/traits/RWTIsFunction.h#1 $
 *
 ***************************************************************************/

#include <rw/defs.h>
#include <rw/tools/traits/RWFalseType.h>
#include <rw/tools/traits/RWTrueType.h>

/**
 * @internal
 * @ingroup type_traits Type Traits
 * @brief Determines if a type is a function
 *
 * If \c T is a function type, derives from RWTrueType; otherwise derives
 * from RWFalseType.
 *
 * @note
 * This trait is only implemented for functions of up to 10 arguments.
 *
 * @section example Example
 * @code
 * RWTIsFunction<void()>::value  // -> true
 * RWTIsFunction<void>::value    // -> false
 * @endcode
 */
template <typename T>
struct RWTIsFunction : public RWFalseType {
};

template <typename R>
struct RWTIsFunction<R()> : public RWTrueType {
};

template <typename R, typename A1>
struct RWTIsFunction<R(A1)> : public RWTrueType {
};

template <typename R, typename A1, typename A2>
struct RWTIsFunction<R(A1, A2)> : public RWTrueType {
};

template <typename R, typename A1, typename A2, typename A3>
struct RWTIsFunction<R(A1, A2, A3)> : public RWTrueType {
};

template <typename R, typename A1, typename A2, typename A3, typename A4>
struct RWTIsFunction<R(A1, A2, A3, A4)> : public RWTrueType {
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
struct RWTIsFunction<R(A1, A2, A3, A4, A5)> : public RWTrueType {
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
struct RWTIsFunction<R(A1, A2, A3, A4, A5, A6)> : public RWTrueType {
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
struct RWTIsFunction<R(A1, A2, A3, A4, A5, A6, A7)> : public RWTrueType {
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
struct RWTIsFunction<R(A1, A2, A3, A4, A5, A6, A7, A8)> : public RWTrueType {
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
struct RWTIsFunction<R(A1, A2, A3, A4, A5, A6, A7, A8, A9)> : public RWTrueType {
};

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
struct RWTIsFunction<R(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10)> : public RWTrueType {
};

#endif
