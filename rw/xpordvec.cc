

/**********************************************************************
 *
 * Template definitions for RWClassicTPtrOrderedVector<TP>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xpordvec.cc#1 $
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


template <class TP> void
RWClassicTPtrOrderedVector<TP>::clearAndDestroy()
{
    for (size_t i = 0; i < entries(); i++) {
        delete(*this)(i);
    }
    clear();
}

template <class TP> TP*
RWClassicTPtrOrderedVector<TP>::find(const TP* p) const
{
    size_t i = index(p);
    return i == RW_NPOS ? rwnil : (*this)(i);
}

template <class TP> size_t
RWClassicTPtrOrderedVector<TP>::index(const TP* val) const
{
    for (register size_t i = 0; i < nitems_; i++)
        if (*(*this)(i) == *val) {
            return i;
        }
    return RW_NPOS;
}

// Insert value at position "ipt"; value formerly at "ipt"
// gets moved to "ipt+1".
template <class TP> void
RWClassicTPtrOrderedVector<TP>::insertAt(size_t ipt, TP* val)
{
    if (ipt > 0) {
        boundsCheck(ipt - 1);
    }

    // Check for overflow:
    if (nitems_ >= RWTPtrVector<TP>::entries()) {
        this->reshape(nitems_ + RWDEFAULT_RESIZE);
    }

    // Slide right (could be very expensive)
    // Need to increment nitems_ before indexing to avoid bounds error!
    for (register size_t i = nitems_++; i > ipt; i--) {
        (*this)(i) = (*this)(i - 1);
    }

    (*this)(ipt) = val;
}

template <class TP> size_t
RWClassicTPtrOrderedVector<TP>::occurrencesOf(const TP* val) const
{
    size_t count = 0;
    for (register size_t i = 0; i < nitems_; i++)
        if (*(*this)(i) == *val) {
            ++count;
        }
    return count;
}

/*
 * Remove and return the first occurrence of an object with the
 * same value as the object pointed to by "p".
 */
template <class TP> TP*
RWClassicTPtrOrderedVector<TP>::remove(const TP* p)
{
    size_t idx = index(p);
    return idx == RW_NPOS ? rwnil : removeAt(idx);
}

/*
 * Remove all occurrences of the value
 */
template <class TP> size_t
RWClassicTPtrOrderedVector<TP>::removeAll(const TP* val)
{
    size_t j = 0;
    for (register size_t i = 0; i < nitems_; i++) {
        // Are they unequal?  If so, this value should be saved.
        if (!(*(*this)(i) == *val)) {
            // Avoid the copy if possible:
            if (i != j) {
                (*this)(j) = (*this)(i);
            }
            ++j;
        }
    }

    size_t nremoved = nitems_ - j;
    nitems_ = j;
    return nremoved;
}

template <class TP> TP*
RWClassicTPtrOrderedVector<TP>::removeAt(size_t ipt)
{
    boundsCheck(ipt);
    //  RW_PRECONDITION2(ipt<nitems_, "RWClassicTPtrOrderedVector::removeAt(size_t): index out of range");

    TP* temp = (*this)(ipt);

    // Slide left (could be very expensive):
    for (register size_t i = ipt; i < nitems_ - 1; i++) {
        (*this)(i) = (*this)(i + 1);
    }

    --nitems_;
    return temp;
}

template <class TP> void
RWClassicTPtrOrderedVector<TP>::boundsCheck(size_t i) const
{
    if (i >= nitems_) {
        RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEXERR, i, nitems_)));
    }
}


#include <rw/defs.h>
#define RW_USE_CLASSIC_TEMPLATES
#include <rw/epersist.h>

RW_DEFINE_PERSISTABLE_TEMPLATE(RWClassicTPtrOrderedVector)

#ifdef RW_NO_TEMPLINST_ON_BASE
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTPtrOrderedVector, RWpistream, RWpostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTPtrOrderedVector, RWbistream, RWbostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTPtrOrderedVector, RWeistream, RWeostream)
#endif


template <class T, class S>
void rwSaveGuts(S& str, const RWClassicTPtrOrderedVector<T>& c)
{
    str.putSizeT(c.entries());
    for (size_t i = 0; i < c.entries(); ++i) {
        str << *c[i];
    }
}

template <class T, class S>
void rwRestoreGuts(S& str, RWClassicTPtrOrderedVector<T>& c)
{
    size_t count;
    str.getSizeT(count);
    c.clear();
    for (size_t i = 0; i < count; ++i) {
        T* p;
        str >> p;
        c.insert(p);
    }
}

template<class T>
bool rwDerefCompareEqual(const RWClassicTPtrOrderedVector<T>& lhs, const RWClassicTPtrOrderedVector<T>& rhs)
{
    size_t l_len = lhs.length();
    size_t r_len = rhs.length();

    if (l_len != r_len) {
        return false;
    }

    size_t i;
    for (i = 0; i < l_len; ++i) {
        if (!(*lhs[i] == *rhs[i])) {
            return false;
        }
    }
    return true;
}
