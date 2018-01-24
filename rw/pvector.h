#ifndef RW_TOOLS_PVECTOR_H
#define RW_TOOLS_PVECTOR_H


/**********************************************************************
*
* RWPtrVector: Vector of pointers
*
**********************************************************************
*
* $Id: //tools/13/rw/pvector.h#1 $
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

#include <rw/tools/iterator.h>
#include <rw/tools/algorithm.h>

#include <rw/tools/traits/RWTEnableIf.h>
#include <rw/tools/traits/RWTIsIntegral.h>

class RW_TOOLS_SYMBOLIC RWPtrVector
{
public:

    RWPtrVector()
        : npts_(0), array_(0) {
    }

    RWPtrVector(size_t n)
        : npts_(n) {
        array_ = (0 == n) ? 0 : new RWvoid[n];
    }

    RWPtrVector(size_t n, void* ival);

    RWPtrVector(const RWPtrVector&);

#if !defined(RW_NO_RVALUE_REFERENCES)
    RWPtrVector(RWPtrVector &&);
#endif // !RW_NO_RVALUE_REFERENCES

    ~RWPtrVector() {
        delete [] array_;
    }

    RWPtrVector& operator=(const RWPtrVector&);

#if !defined(RW_NO_RVALUE_REFERENCES)
    RWPtrVector& operator=(RWPtrVector &&);
#endif // !RW_NO_RVALUE_REFERENCES

    template <typename InputIterator>
    RWPtrVector(InputIterator first, InputIterator last,
                typename RWTEnableIf < !RWTIsIntegral<InputIterator>::value >::type** = 0)
        : npts_(0), array_(0) {
        assign(first, last);
    }

    RWPtrVector& operator=(void* p);           // All elements initialized to p

    template <class InputIterator>
    void assign(InputIterator first, InputIterator last) {
        typedef typename rw_iterator_traits<InputIterator>::iterator_category cat;
        assign(first, last, cat());
    }

    void clear() {
        reshape(0);
    }

    void* const* data() const {
        return array_; // Cannot change vector elements
    }

    void** data() {
        return array_;
    }

    size_t entries() const {
        return npts_;
    }

    void reshape(size_t);

    void resize(size_t);

    void*& operator[](size_t i) {
        boundsCheck(i);
        return array_[i];
    }

    void* const& operator[](size_t i) const {
        boundsCheck(i);
        return array_[i];
    }

    void*& operator()(size_t i) {
#ifdef RWBOUNDS_CHECK
        boundsCheck(i);
#endif
        return array_[i];
    }

    void* const& operator()(size_t i) const {
#ifdef RWBOUNDS_CHECK
        boundsCheck(i);
#endif
        return array_[i];
    }

    void swap(RWPtrVector& rhs);

private:

    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last, const std::input_iterator_tag&) {
        size_t length = 0;
        size_t capacity = 16;

        RWPtrVector buf(capacity);
        for (/**/; first != last; ++first) {

            if (length == capacity) {
                buf.reshape(capacity *= 2);
            }

            buf.array_[length++] = *first;
        }

        assign(buf.array_, buf.array_ + length);
    }

    template <typename ForwardIterator>
    void assign(ForwardIterator first, ForwardIterator last, const std::forward_iterator_tag&) {
        const typename rw_iterator_traits<ForwardIterator>::difference_type n = rw_distance(first, last);
        RW_ASSERT(0 <= n);

        RWPtrVector buf(static_cast<size_t>(n));
        for (RWvoid* dst = buf.array_; first != last; ++first) {
            *dst++ = *first;
        }

        buf.swap(*this);
    }

protected:

    void boundsCheck(size_t i) const;

    size_t npts_;
    RWvoid* array_;
};


#endif /* RW_TOOLS_PVECTOR_H */
