

/**********************************************************************
 *
 * Template definitions for RWTValVirtualArray<T>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvrtarry.cc#1 $
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
#include <rw/vpage.h>

template <class T>
RWTValVirtualArray<T>::RWTValVirtualArray(long size, RWVirtualPageHeap* heap)
{
    vref_ = new RWTVirtualRef<T>(size, heap);
}

template <class T>
RWTValVirtualArray<T>::~RWTValVirtualArray()
{
    if (vref_->removeReference() == 0) {
        delete vref_;
    }
}

template <class T>
RWTValVirtualArray<T>::RWTValVirtualArray(const RWTValVirtualArray<T>& v)
{
    vref_ = v.vref_;
    vref_->addReference();
}

template <class T>
RWTValVirtualArray<T>::RWTValVirtualArray(const RWTVirtualSlice<T>& sl)
{
    vref_ = new
    RWTVirtualRef<T>(sl.extent_, sl.varray_->heap());
    RWTVirtualRef<T>* vr = (RWTVirtualRef<T>*)sl.varray_->vref_;
    vref_->conformalCopy(0, *vr, sl.start_, sl.extent_);
}

template <class T> RWTValVirtualArray<T>&
RWTValVirtualArray<T>::operator=(const RWTValVirtualArray<T>& v)
{
    v.vref_->addReference();
    if (vref_->removeReference() == 0) {
        delete vref_;
    }
    vref_ = v.vref_;
    return *this;
}

template <class T> void
RWTValVirtualArray<T>::operator=(const RWTVirtualSlice<T>& sl)
{
    RWTValVirtualArray<T>* v2 = (RWTValVirtualArray<T>*)sl.varray_;
    RWTVirtualRef<T>* newvref = new
    RWTVirtualRef<T>(sl.extent_, v2->heap());
    newvref->conformalCopy(0, *v2->vref_, sl.start_, sl.extent_);
    if (vref_->removeReference() == 0) {
        delete vref_;
    }
    vref_ = newvref;
}

template <class T> T
RWTValVirtualArray<T>::operator=(const T& val)
{
    slice(0, length()) = val;             // Take a slice of self
    return val;
}

template <class T> void
RWTValVirtualArray<T>::cow()
{
    if (vref_->references() > 1) {
        // Make a deep copy of it
        RWTVirtualRef<T>* tmp = new RWTVirtualRef<T> (*vref_);

        // Remove our reference to the body
        vref_->removeReference();

        // Assign new value
        vref_ = tmp;
    }
}

/****************************************************************
 ****************************************************************
 *                                                              *
 *                      RWTVirtualSlice<T>                      *
 *                      Definitions                             *
 *                                                              *
 ****************************************************************
 ****************************************************************/

template <class T> T
RWTVirtualSlice<T>::operator=(const T& newVal)
{
    this->varray_->cow();
    this->varray_->vref_->set(this->start_, extent_, newVal);
    return newVal;
}

template <class T> void
RWTVirtualSlice<T>::operator=(const RWTVirtualSlice<T>& sl)
{
    RWVirtualRef& vr = *((RWVirtualRef*)sl.varray_->vref_);

    this->varray_->cow();
    this->varray_->vref_->setSlice(this->start_, extent_, vr, sl.start_, sl.extent_);
}

template <class T> void
RWTVirtualSlice<T>::operator=(const RWTValVirtualArray<T>& v)
{
    RWTValVirtualArray<T>& va = (RWTValVirtualArray<T>&)v;
    this->varray_->cow();
    this->varray_->vref_->setSlice(this->start_, extent_, *(va.vref_), 0, v.length());
}
