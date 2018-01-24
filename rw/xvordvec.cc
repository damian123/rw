

/**********************************************************************
 *
 * Template definitions for RWTValOrderedVector<T>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xvordvec.cc#1 $
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


template <class T> void
RWClassicTValOrderedVector<T>::clear()
{
    T defaultValue = T();     // avoid "used before set" warning
    register size_t i = 0;

    while (i < nitems_) {
        (*this)(i++) = defaultValue;
    }
    nitems_ = 0;
    if (RWTValVector<T>::entries() > RW_DEFAULT_CAPACITY) {
        this->reshape(RW_DEFAULT_CAPACITY);
    }
}


template <class T> bool
RWClassicTValOrderedVector<T>::find(const T& key, T& ret) const
{
    bool rc = false;
    size_t    idx = index(key);

    if (idx != RW_NPOS) {
        ret = this->array_[idx];
        rc = true;
    }

    return rc;
}

template <class T> size_t
RWClassicTValOrderedVector<T>::index(const T& val) const
{
    for (register size_t i = 0; i < nitems_; i++) {
        if ((*this)(i) == val) {
            return i;
        }
    }
    return RW_NPOS;
}

// Insert value at position "ipt"; value formerly at "ipt"
// gets moved to "ipt+1".
template <class T> void
RWClassicTValOrderedVector<T>::insertAt(size_t ipt, const T& val)
{
    if (ipt > 0) {
        boundsCheck(ipt - 1);
    }

    // Check for overflow:
    if (nitems_ >= RWTValVector<T>::entries()) {
        this->reshape(nitems_ + RWDEFAULT_RESIZE);
    }

    // Slide right (could be very expensive)
    // Need to increment nitems_ before indexing to avoid bounds error!
    for (register size_t i = nitems_++; i > ipt; i--) {
        (*this)(i) = (*this)(i - 1);
    }

    (*this)(ipt) = val;
}

template <class T> size_t
RWClassicTValOrderedVector<T>::occurrencesOf(const T& val) const
{
    size_t count = 0;
    for (register size_t i = 0; i < nitems_; i++)
        if ((*this)(i) == val) {
            ++count;
        }
    return count;
}

/*
 * Remove and return the first occurrence of an object which is equal to p
 */
template <class T> bool
RWClassicTValOrderedVector<T>::remove(const T& p)
{
    size_t idx = index(p);
    if (idx == RW_NPOS) {
        return false;
    }

    //else
    removeAt(idx);
    return true;
}


/*
 * Remove and return the last element of self
 */
template <class T> T
RWClassicTValOrderedVector<T>::removeLast()
{
    T ret((*this)(nitems_ - 1));   // construct ret from last item

    // reset now-unused cell to default value:
    (*this)(nitems_ - 1) = T();

    // now we can decrement nitems_ without causing a bounds error:
    --nitems_;
    return ret;
}


/*
 * Remove all items equal to val
 */
template <class T> size_t
RWClassicTValOrderedVector<T>::removeAll(const T& val)
{
    register size_t i;
    size_t j = 0;

    for (i = 0; i < nitems_; i++) {
        // Are they unequal?  If so, this value should be saved.
        if (!((*this)(i) == val)) {
            // Avoid the copy if possible:
            if (i != j) {
                (*this)(j) = (*this)(i);
            }
            ++j;
        }
    }

    size_t nremoved = nitems_ - j;

    // reset now-unused cells to default value:
    T defaultValue = T();           // avoid "used before set" warning
    while (j < i) {
        (*this)(j++) = defaultValue;
    }

    // now we can decrement nitems_ without a bounds error:
    nitems_ -= nremoved;

    return nremoved;
}

template <class T> T
RWClassicTValOrderedVector<T>::removeAt(size_t ipt)
{
    boundsCheck(ipt);

    T temp = (*this)(ipt);

    // Slide left (could be very expensive):
    for (register size_t i = ipt; i < nitems_ - 1; i++) {
        (*this)(i) = (*this)(i + 1);
    }

    // reset now-unused cell to default value:
    (*this)(nitems_ - 1) = T();

    // now we can decrement nitems_ without a bounds error:
    --nitems_;

    return temp;
}

template <class T> void
RWClassicTValOrderedVector<T>::boundsCheck(size_t i) const
{
    if (i >= nitems_) {
        RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEXERR, i, nitems_)));
    }
}


#include <rw/defs.h>
#define RW_USE_CLASSIC_TEMPLATES
#include <rw/epersist.h>

RW_DEFINE_PERSISTABLE_TEMPLATE(RWClassicTValOrderedVector)

#ifdef RW_NO_TEMPLINST_ON_BASE
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTValOrderedVector, RWpistream, RWpostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTValOrderedVector, RWbistream, RWbostream)
RW_DEFINE_PERSIST_TEMPLATE_IO(RWClassicTValOrderedVector, RWeistream, RWeostream)
#endif


template <class T, class S>
void rwSaveGuts(S& str, const RWClassicTValOrderedVector<T>& c)
{
    str.putSizeT(c.entries());
    for (size_t i = 0; i < c.entries(); ++i) {
        str << c[i];
    }
}

template <class T, class S>
void rwRestoreGuts(S& str, RWClassicTValOrderedVector<T>& c)
{
    size_t count;
    str.getSizeT(count);
    c.clear();
    for (size_t i = 0; i < count; ++i) {
        T t;
        str >> t;
        c.insert(t);
    }
}

template<class T>
bool rwCompareEqual(const RWClassicTValOrderedVector<T>& lhs, const RWClassicTValOrderedVector<T>& rhs)
{
    size_t l_len = lhs.length();
    size_t r_len = rhs.length();

    if (l_len != r_len) {
        return false;
    }

    size_t i;
    for (i = 0; i < l_len; ++i) {
        if (!(lhs[i] == rhs[i])) {
            return false;
        }
    }
    return true;
}
