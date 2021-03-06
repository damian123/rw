#ifndef RW_TOOLS_TRAITS_RWTISRVALUEREFERENCE_H
#define RW_TOOLS_TRAITS_RWTISRVALUEREFERENCE_H

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
 * $Id: //tools/13/rw/tools/traits/RWTIsRvalueReference.h#1 $
 *
 ***************************************************************************/

#include <rw/defs.h>

#if !defined(RW_NO_RVALUE_REFERENCES) || defined(DOXYGEN)

#include <rw/tools/traits/RWFalseType.h>
#include <rw/tools/traits/RWTrueType.h>

/**
 * @internal
 * @ingroup type_traits Type Traits
 * @brief Determines if a type is an rvalue reference
 *
 * If \c T is an rvalue reference, derives from RWTrueType; otherwise
 * derives from RWFalseType.
 *
 * @section example Example
 * @code
 * RWTIsRvalueReference<int>::value            // -> false
 * RWTIsRvalueReference<int&>::value           // -> false
 * RWTIsRvalueReference<int&&>::value          // -> true
 * RWTIsRvalueReference<int(&&)(void)>::value  // -> true
 * RWTIsRvalueReference<int(&&)[]>::value      // -> true
 * @endcode
 */
template <typename T>
struct RWTIsRvalueReference : public RWFalseType {
};

template <typename T>
struct RWTIsRvalueReference < T && > : public RWTrueType {
};

#endif

#endif
