

/**********************************************************************
 *
 * regexresultimp.cc - contains the implementation for the RWTRegexResultImp
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regex/regexresultimp.cc#1 $
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


template <class E>
RWTRegexResultImp<E>::RWTRegexResultImp()
{
    memset(starts_(), 0, RNumMatches * sizeof(size_t));
    memset(lengths_(), 0, RNumMatches * sizeof(size_t));
}


template <class E>
RWTRegexResultImp<E>::RWTRegexResultImp(const RWTRegexResultImp<E>& source) :
    starts_(source.starts_),
    lengths_(source.lengths_)
{
}

template <class E>
RWTRegexResultImp<E>::~RWTRegexResultImp()
{
}

template <class E>
RWTRegexResultImp<E>& RWTRegexResultImp<E>::operator=(const RWTRegexResultImp<E>& rhs)
{
    starts_ = rhs.starts_;
    lengths_ = rhs.lengths_;

    return *this;
}

template <class E>
RWTRegexResultImp<E>::operator bool() const
{
    return starts_.size() ? starts_()[0] != size_t(-1) : false;
}

template <class E>
size_t RWTRegexResultImp<E>::size() const
{
    return starts_.size();
}

template <class E>
size_t RWTRegexResultImp<E>::getStart(size_t matchID) const
{
    return starts_()[matchID];
}

template <class E>
size_t RWTRegexResultImp<E>::getLength(size_t matchID) const
{
    return lengths_()[matchID];
}
