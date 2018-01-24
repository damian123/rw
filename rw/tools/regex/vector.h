#ifndef RW_REGEX_VECTOR_H
#define RW_REGEX_VECTOR_H



/**********************************************************************
 *
 * vector.h - Contains limited vector interface for use in regex.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regex/vector.h#1 $
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


/**************************************************************************/
/*                                                                        */
/* Class Name                                                             */
/* ==========                                                             */
/*                                                                        */
/* RWRegVec                                                               */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This provides a limited, lightweight vector interface for use in       */
/* RWTRegex.  Tested with ResizeIncrement = 2 for assurance that resizing  */
/* works.                                                                 */
/*                                                                        */
/* Uses stack-allocated buffer for sizes up to ResizeIncrement.           */
/*                                                                        */
/* I chose a small resize increment and a linear step resize algorithm    */
/* because most of the vectors used in RWTRegex will be small.             */
/*                                                                        */
/* Implemented inline and no virtual destructor for speed.                */
/*                                                                        */
/**************************************************************************/

#include <rw/defs.h>
#include <rw/tools/pointer.h>

template <class T>
class RWRegVec
{
public:
    RWRegVec() :
        vSize_(0),
        limit_(ResizeIncrement),
        hBuf_(0),
        buf_(sBuf_) {
    }

    RWRegVec(const RWRegVec& source) :
        vSize_(0),
        limit_(ResizeIncrement),
        hBuf_(0),
        buf_(sBuf_) {
        *this = source;
    }

    T& back() {
        return buf_[vSize_ - 1];
    }

    const T& back() const {
        return buf_[vSize_ - 1];
    }

    void clear() {
        vSize_ = 0;
    }

    RWRegVec& operator=(const RWRegVec& rhs) {
        if (this != &rhs) {
            if (rhs.vSize_ > limit_) {
                limit_ = rhs.limit_;
                hBuf_.reset(new T[limit_]);
                buf_ = hBuf_.get();
            }

            vSize_ = rhs.vSize_;

            for (size_t i = 0; i < vSize_; i++) {
                buf_[i] = rhs.buf_[i];
            }
        }

        return *this;
    }

    T& operator[](size_t i) {
        return buf_[i];
    }

    const T& operator[](size_t i) const {
        return buf_[i];
    }

    void pop_back() {
        vSize_--;
    }

    void push_back(const T& item) {
        if (vSize_ == limit_) {
            limit_ += ResizeIncrement;
            RWTScopedPointer<T[]> tmp(new T[limit_]);
            for (size_t i = 0; i < vSize_; i++) {
                tmp[i] = buf_[i];
            }
            rw_swap(hBuf_, tmp);
            buf_ = hBuf_.get();
        }

        buf_[vSize_] = item;
        vSize_++;
    }

    size_t size() const {
        return vSize_;
    }

private:
    enum {
        ResizeIncrement = 16
    };

    size_t vSize_;
    size_t limit_;
    T      sBuf_[ResizeIncrement];
    RWTScopedPointer<T[]> hBuf_;
    T*     buf_;
};

#endif
