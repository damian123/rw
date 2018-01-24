

/**********************************************************************
 *
 * rw_regex.cc - contains the implementation for the regex interface
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regex.cc#1 $
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


#include <rw/tools/regex/imp.h>

template <class E>
RWTRegex<E>::RWTRegex() :
    imp_(new RWRegexImp<RChar>)
{
}

template <class E>
RWTRegex<E>::RWTRegex(const RChar* str,
                      size_t       length) :
    imp_(new RWRegexImp<RChar>(str, length))
{
}

template <class E>
RWTRegex<E>::RWTRegex(const RString& str, size_t length) :
    imp_(new RWRegexImp<RChar>(str.data(), length == size_t(-1) ? str.length() : length))
{
}

template <class E>
RWTRegex<E>::RWTRegex(const RWTRegex<E>& source) :
    imp_(new RWRegexImp<RChar>(*source.imp_))
{
}

template <class E>
RWTRegex<E>::~RWTRegex() {}

template <class E>
RWTRegex<E>& RWTRegex<E>::operator= (const RWTRegex<E>& rhs)
{
    if (this == &rhs) {
        return *this;
    }

    RWTScopedPointer<RWRegexImp<RChar> > tmp(new RWRegexImp<RChar>(*rhs.imp_));
    rw_swap(imp_, tmp);

    return *this;
}

template <class E>
bool RWTRegex<E>::operator< (const RWTRegex<E>& rhs) const
{
    RW_ASSERT(imp_ && rhs.imp_);
    return (*imp_ < *rhs.imp_);
}

template <class E>
bool RWTRegex<E>::operator==(const RWTRegex<E>& rhs) const
{
    RW_ASSERT(imp_ && rhs.imp_);
    return (*imp_ == *rhs.imp_);
}

template <class E>
size_t RWTRegex<E>::subCount() const
{
    RW_ASSERT(imp_);
    return imp_->subCount();
}

template <class E>
RWTRegexResult<E> RWTRegex<E>::search(const RChar* str,
                                      size_t       start,
                                      size_t       length)
{
    RW_ASSERT(imp_);
    if (length == size_t(-1)) {
        length = imp_->strLen(str);
    }
    return imp_->search(str, start, length);
}

template <class E>
RWTRegexResult<E> RWTRegex<E>::search(const RString& str,
                                      size_t         start,
                                      size_t         length)
{
    RW_ASSERT(imp_);
    if (length == size_t(-1)) {
        length = str.length();
    }
    return imp_->search(str.data(), start, length);
}

template <class E>
RWTRegexResult<E> RWTRegex<E>::matchAt(const RChar* str,
                                       size_t       start,
                                       size_t       length)
{
    RW_ASSERT(imp_);
    if (length == size_t(-1)) {
        length = imp_->strLen(str);
    }
    return imp_->matchAt(str, start, length);
}

template <class E>
RWTRegexResult<E> RWTRegex<E>::matchAt(const RString& str,
                                       size_t         start,
                                       size_t         length)
{
    RW_ASSERT(imp_);
    if (length == size_t(-1)) {
        length = str.length();
    }
    return imp_->matchAt(str.data(), start, length);
}

template <class E>
size_t RWTRegex<E>::index(const RChar*   str,
                          size_t*        mLen,
                          size_t         start,
                          size_t         length)
{
    RW_ASSERT(imp_);
    size_t            rc = RW_NPOS;
    RWTRegexResult<E> result;

    if (mLen) {
        *mLen = 0;
    }
    if ((result = search(str, start, length))) {
        rc = result.getStart();
        if (mLen) {
            *mLen = result.getLength();
        }
    }

    return rc;
}

template <class E>
size_t RWTRegex<E>::index(const RString& str,
                          size_t*        mLen,
                          size_t         start,
                          size_t         length)
{
    RW_ASSERT(imp_);
    size_t            rc = RW_NPOS;
    RWTRegexResult<E> result;

    if (mLen) {
        *mLen = 0;
    }
    if ((result = search(str, start, length))) {
        rc = result.getStart();
        if (mLen) {
            *mLen = result.getLength();
        }
    }

    return rc;
}

template <class E>
size_t RWTRegex<E>::replace(RString&       str,
                            const RString& replacement,
                            size_t         count,
                            size_t         matchID,
                            size_t         start,
                            size_t         length,
                            bool      replaceEmptyMatches)
{
    RW_ASSERT(imp_);
    RString rc;
    size_t  curIndex = 0;
    size_t  rCount = 0;

    if (length == size_t(-1)) {
        length = str.length();
    }

    for (RWTRegexMatchIterator<RChar> i(*this, str, start, length);
            (count == 0 || rCount < count) && i != RWTRegexMatchIterator<RChar>();
            i++) {
        rc += RString(str, curIndex, i->getStart() - curIndex);

        if (i->getStart(matchID) != size_t(-1)) {
            size_t overallStart = i->getStart();
            size_t overallEnd   = i->getStart() + i->getLength();
            size_t subStart     = i->getStart(matchID);
            size_t subEnd       = i->getStart(matchID) + i->getLength(matchID);

            // Add the overall match up to requested match
            rc += RString(str.substr(overallStart, subStart - overallStart));

            // Add replacement
            if (i->getLength(matchID) || replaceEmptyMatches) {
                rc += replacement;
                rCount++;
            }

            // Add the remainder of the overall match
            rc += RString(str.substr(subEnd, overallEnd - subEnd));
        }

        curIndex = i->getStart() + i->getLength();
    }

    rc += RString(str, curIndex, str.length() - curIndex);
    str = rc;

    return rCount;
}

template <class E>
const RWRegexErr& RWTRegex<E>::getStatus() const
{
    RW_ASSERT(imp_);
    return imp_->getStatus();
}

template <class E>
void RWTRegex<E>::swap(RWTRegex<E>& rhs)
{
    rw_swap(rhs.imp_, imp_);
}

#if !defined(RW_NO_RVALUE_REFERENCES)

template <class E>
RWTRegex<E>::RWTRegex(RWTRegex<E> && rhs)
    : imp_(0)
{
    rhs.swap(*this);
}

template <class E>
RWTRegex<E>& RWTRegex<E>::operator= (RWTRegex<E> && rhs)
{
    if (&rhs != this) {
        rhs.swap(*this);
    }

    return *this;
}

#endif // !RW_NO_RVALUE_REFERENCES


template <class E> class RWRegexMatchIteratorImp;

template <class E>
RWTRegexMatchIterator<E>::RWTRegexMatchIterator() :
    imp_(new RWRegexMatchIteratorImp<E>)
{
}

template <class E>
RWTRegexMatchIterator<E>::RWTRegexMatchIterator(const RWTRegexMatchIterator<E>& source) :
    imp_(new RWRegexMatchIteratorImp<E>(*source.imp_))
{
}

template <class E>
RWTRegexMatchIterator<E>::RWTRegexMatchIterator(RWTRegex<E>&    regex,
        const RChar*    str,
        size_t          start,
        size_t          length,
        bool       overlap) :
    imp_(new RWRegexMatchIteratorImp<E>(regex, str, start, length, overlap))
{
}

template <class E>
RWTRegexMatchIterator<E>::RWTRegexMatchIterator(RWTRegex<E>&     regex,
        const RString&   str,
        size_t           start,
        size_t           length,
        bool        overlap) :
    imp_(new RWRegexMatchIteratorImp<E>(regex,
                                        str.data(),
                                        start,
                                        length == size_t(-1) ? str.length() : length,
                                        overlap))
{
}

template <class E>
RWTRegexMatchIterator<E>::~RWTRegexMatchIterator() { }

template <class E>
RWTRegexMatchIterator<E>& RWTRegexMatchIterator<E>::operator=(const RWTRegexMatchIterator& rhs)
{
    RW_ASSERT(imp_ && rhs.imp_);
    *imp_ = *rhs.imp_;
    return *this;
}

template <class E>
RWTRegexMatchIterator<E>& RWTRegexMatchIterator<E>::operator++()
{
    RW_ASSERT(imp_);
    imp_->operator++();
    return *this;
}

template <class E>
const RWTRegexMatchIterator<E> RWTRegexMatchIterator<E>::operator++(int)
{
    RW_ASSERT(imp_);
    RWTRegexMatchIterator<E> rc = *this;
    imp_->operator++(0);
    return rc;
}

template <class E>
RWTRegexMatchIterator<E>& RWTRegexMatchIterator<E>::operator+=(size_t count)
{
    RW_ASSERT(imp_);
    imp_->operator+=(count);
    return *this;
}

template <class E>
RWTRegexResult<E> RWTRegexMatchIterator<E>::operator*() const
{
    RW_ASSERT(imp_);
    return imp_->operator*();
}

template <class E>
const RWTRegexResult<E>* RWTRegexMatchIterator<E>::operator->() const
{
    RW_ASSERT(imp_);
    return imp_->operator->();
}

template <class E>
bool RWTRegexMatchIterator<E>::operator==(const RWTRegexMatchIterator& rhs) const
{
    RW_ASSERT(imp_ && rhs.imp_);
    return (*imp_ == *rhs.imp_);
}

template <class E>
bool RWTRegexMatchIterator<E>::operator!=(const RWTRegexMatchIterator<E>& rhs) const
{
    RW_ASSERT(imp_ && rhs.imp_);
    return (!(*imp_ == *rhs.imp_));
}

template <class E>
void RWTRegexMatchIterator<E>::swap(RWTRegexMatchIterator<E>& rhs)
{
    rw_swap(rhs.imp_, imp_);
}

#if !defined(RW_NO_RVALUE_REFERENCES)

template <class E>
RWTRegexMatchIterator<E>::RWTRegexMatchIterator(RWTRegexMatchIterator<E> && rhs)
    : imp_(0)
{
    rhs.swap(*this);
}

template <class E>
RWTRegexMatchIterator<E>& RWTRegexMatchIterator<E>::operator= (RWTRegexMatchIterator<E> && rhs)
{
    if (&rhs != this) {
        rhs.swap(*this);
    }

    return *this;
}

#endif // !RW_NO_RVALUE_REFERENCES
