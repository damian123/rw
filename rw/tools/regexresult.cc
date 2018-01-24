

/**********************************************************************
 *
 * regexresult.cc - contains the implementation of the RWTRegexResult
 *                  class.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regexresult.cc#1 $
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


#include <rw/edefs.h> // for rw_swap
#include <rw/tools/regex/regexresultimp.h>

template <class E>
RWTRegexResult<E>::RWTRegexResult() :
    imp_(new RWTRegexResultImp<E>)
{
}

template <class E>
RWTRegexResult<E>::RWTRegexResult(const RWTRegexResult<E>& source) :
    imp_(new RWTRegexResultImp<E>(*source.imp_))
{
}

template <class E>
RWTRegexResult<E>::~RWTRegexResult() {}

template <class E>
RWTRegexResult<E>& RWTRegexResult<E>::operator=(const RWTRegexResult<E>& rhs)
{
    *imp_ = *rhs.imp_;
    return *this;
}

template <class E>
RWTRegexResult<E>::operator bool() const
{
    return bool(*imp_);
}

template <class E>
size_t RWTRegexResult<E>::size() const
{
    return imp_->size();
}

template <class E>
size_t RWTRegexResult<E>::getStart(size_t matchID) const
{
    return imp_->getStart(matchID);
}

template <class E>
size_t RWTRegexResult<E>::getLength(size_t matchID) const
{
    return imp_->getLength(matchID);
}

template <class E>
typename RWTRegexResult<E>::RString
RWTRegexResult<E>::subString(const RString& str, size_t matchID) const
{
    RString rc;

    if (getLength(matchID)) {
        rc = str.substr(getStart(matchID), getLength(matchID));
    }

    return rc;
}

template <class E>
void RWTRegexResult<E>::swap(RWTRegexResult<E>& rhs)
{
    rw_swap(rhs.imp_, imp_);
}

#if !defined(RW_NO_RVALUE_REFERENCES)

template <class E>
RWTRegexResult<E>::RWTRegexResult(RWTRegexResult<E> && rhs)
    : imp_(0)
{
    rhs.swap(*this);
}

template <class E>
RWTRegexResult<E>& RWTRegexResult<E>::operator= (RWTRegexResult<E> && rhs)
{
    if (&rhs != this) {
        rhs.swap(*this);
    }

    return *this;
}

#endif // !RW_NO_RVALUE_REFERENCES
