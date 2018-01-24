#ifndef __RWVECTOR_H__
#define __RWVECTOR_H__

/***************************************************************************
 *
 * vector.h - Base class for vector views.
 *
 * $Id: //math/13/rw/vector.h#1 $
 *
 * Copyright (c) 1991-2015 Rogue Wave Software, Inc.  All Rights Reserved.
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
 **************************************************************************/

#include <rw/dataview.h>

/*
 * This is the part of a vector common to all types of vectors.
 */

/**
 * @ingroup math_collection_classes
 *
 * @brief A base class for mathematical vector classes.
 *
 * A base class for vector classes.
 */
class RW_MATH_SYMBOLIC RWVecView : public RWDataView
{
protected:
    size_t   npts;
    int        step;

    RWVecView();
    RWVecView(size_t n, size_t s);
    RWVecView(RWBlock* block, size_t n);

protected:
    RWVecView(const RWDataView& v, void* b, size_t l, int s) : RWDataView(v, b) {
        npts = l;
        step = s;
    }

    void      reference(const RWVecView&);

public:  // These functions need to be public so that they can be called by global functions

    /**
     * @internal
     * Ensure 0<=i<npts
     */
    void boundsCheck(int i) const;

    /**
     * @internal
     * Check that length of self is i
     */
    void lengthCheck(size_t  i) const;

    /**
     * @internal
     * Error: vector not empty
     */
    void emptyErr(const char* fname) const;

public:  // These functions really are for public consumption

    /**
     * Returns the number of elements in the vector.
     */
    size_t   length() const {
        return npts;
    }

    static int setFormatting(int);              // Change # items per line

    /**
     * Returns the distance between successive elements of the vector.
     * The stride can be computed using \c &v[i+1]-&v[i].
     */
    int        stride() const {
        return step;
    }
};

#endif /* RW_VECTOR_H */
