#ifndef RW_REGEX_BUFMAN_H
#define RW_REGEX_BUFMAN_H



/**********************************************************************
 *
 * bufman.h - contains the class definition for the class template
 *            providing buffer management facilities
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regex/bufman.h#1 $
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
/* RWBufMan                                                               */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This class template represents the management of buffers of items,     */
/* where a specified number of items are allocated off the stack for      */
/* use, and heap allocation is used only if there is not enough storage   */
/* in the stack-allocated buffer for all required items.                  */
/*                                                                        */
/* This policy can provide improved performance by avoiding the use of    */
/* the heap.                                                              */
/*                                                                        */
/* The T template argument specifies the type of element to be stored in  */
/* one element of the buffer.  The n template argument specifies the      */
/* size, in elements, of the stack-allocated buffer.                      */
/*                                                                        */
/**************************************************************************/

template <class T, size_t N>
class RWBufMan
{
private:
    typedef T* PT;

public:
    RWBufMan()
        : sBuf_(PT(ustorage_.storage_)),
          buf_(sBuf_),
          size_(0)
    { }

    RWBufMan(const RWBufMan<T, N>& source)
        : sBuf_(PT(ustorage_.storage_)),
          buf_(sBuf_),
          size_(0) {

        set(source.size_);
        for (size_t i = 0; i < size_; ++i) {
            buf_ [i] = source.buf_ [i];
        }
    }

    ~RWBufMan() {
        clear();
    }

    // Assignment
    RWBufMan<T, N>& operator= (const RWBufMan<T, N>& rhs) {
        if (&rhs != this) {
            clear();

            set(rhs.size_);
            for (size_t i = 0; i < size_; ++i) {
                buf_[i] = rhs.buf_[i];
            }
        }
        return *this;
    }

    // Use this operator to access the buffer
    T* operator()() {
        return buf_;
    }

    // Or this one for const objects
    const T* operator()() const {
        return buf_;
    }

    // Use set() when you know the buffer size (elements) needed
    // for the problem to be solved.  If the stack-allocated buffer
    // is too small, then the required space is allocated from the heap.
    void set(size_t n) {
        size_ = n;
        if (size_ > N) {
            buf_ = new T [size_];
        }
    }

    // Return the number of items stored
    size_t size() const {
        return size_;
    }

    // Use clear when finished with the buffer to return any heap-allocated
    // memory to the heap.
    void clear() {
        if (buf_ != sBuf_) {
            delete[] buf_;
            buf_ = sBuf_;
        }
    }

private:

    T*  sBuf_;
    T*  buf_;

    size_t size_;

    union {
        long double align_;
        char storage_[N* sizeof(T)];
    } ustorage_;
};

#endif
