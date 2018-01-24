#ifndef RW_TOOLS_TBITVEC_H
#define RW_TOOLS_TBITVEC_H



/**********************************************************************
 *
 * RWTBitVec<N>: Parameterized bit vector of length N
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tbitvec.h#1 $
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
#include <rw/rwerr.h>
#include <rw/toolerr.h>
#include <rw/bitref.h>
#include <rw/tools/string.h>

#include <string.h>

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable:4351)
#endif

/**
 * @ingroup traditional_collection_classes
 * @brief  Parameterized bit vector of fixed length.
 *
 * \link RWTBitVec RWTBitVec<Size>\endlink is a parameterized bit vector of
 * fixed length \c Size.  Unlike class RWBitVec, its length cannot be changed
 * at runtime.  The advantage of RWTBitVec is its smaller size,
 * and one less level of indirection, resulting in a slight speed
 * advantage.
 *
 * Bits are numbered from \c 0 through \c Size-1, inclusive.
 *
 * The copy constructor and assignment operator use <i>copy</i>
 * semantics.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tbitvec.h>
 * RWTBitVec<22>   // A 22 bit long vector
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 *
 * In this example, two 8-bit vectors are exercised:
 *
 * @code
 *
 * #include <rw/tbitvec.h>
 * #include <iostream>
 *
 * const size_t VECSIZE = 8;
 *
 * int main ()
 * {
 *     RWTBitVec<VECSIZE> a, b; // Allocate two vectors.
 *
 *     a(2) = true; // Set bit 2 (the third bit) of a on.
 *     b(3) = true; // Set bit 3 (the fourth bit) of b on.
 *
 *     RWTBitVec<VECSIZE> c = a ^ b; // Set c to the XOR of a and b.
 *
 *     std::cout << "V1" << "\t"
 *               << "V2" << "\t"
 *               << "V1 XOR V2"
 *               << std::endl;
 *
 *     for(size_t i = 0; i < VECSIZE; i++) {
 *         std::cout << a[i] << "\t"
 *                   << b[i] << "\t"
 *                   << c[i] << "\n";
 *     }
 *     return 0;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 *
 * V1  V2  V1 XOR V2
 * 0  0  0
 * 0  0  0
 * 1  0  1
 * 0  1  1
 * 0  0  0
 * 0  0  0
 * 0  0  0
 * 0  0  0
 * @endcode
 *
 */
template <size_t N>
class  RWTBitVec
{

public:
    // VS 2005 and beyond will warn when an array member is value
    // initialized (indicating a change in behavior from previous
    // versions)
    /**
     * Constructs an instance with all bits set to \c false.
     */
    RWTBitVec() : vec_() {
    }

    /**
     * Constructs an instance with all bits set to \a val.
     */
    RWTBitVec(bool val) {
        ::memset(vec_, val ? ~0 : 0, sizeof vec_);
    }

    /**
     * Constructs a copy of \a v.
     */
    RWTBitVec(const RWTBitVec<N>& v) {
        rw_memcpy_s(vec_, sizeof vec_, v.vec_, sizeof vec_);
    }

    /**
     * Sets self to a copy of \a v.
     */
    RWTBitVec<N>&         operator=(const RWTBitVec<N>& v) {
        if (this != &v) {
            rw_memcpy_s(vec_, sizeof vec_, v.vec_, sizeof vec_);
        }
        return *this;
    }

    /**
     * Sets all bits in self to the value \a val.
     */
    RWTBitVec<N>&         operator=(bool val) {
        ::memset(vec_, val ? ~0 : 0, sizeof vec_);
        return *this;
    }

    /**
     * Logical assignments.  Sets each bit of self to the logical
     * \c AND of self and the corresponding bit in \a v.
     */
    RWTBitVec<N>&         operator&=(const RWTBitVec<N>& v) {
        rwAND(vec_, v.vec_, sizeof vec_);
        return *this;
    }

    /**
     * Logical assignments.  Sets each bit of self to the logical
     * \c XOR of self and the corresponding bit in \a v.
     */
    RWTBitVec<N>&         operator^=(const RWTBitVec<N>& v) {
        rwXOR(vec_, v.vec_, sizeof vec_);
        return *this;
    }

    /**
     * Logical assignments.  Sets each bit of self to the logical
     * \c OR of self and the corresponding bit in \a v.
     */
    RWTBitVec<N>&         operator|=(const RWTBitVec<N>& v) {
        rwOR(vec_, v.vec_, sizeof vec_);
        return *this;
    }

    /**
     * Returns a reference to the \a i <sup>th</sup> bit of self.  This reference
     * can be used as an lvalue.
     * @throw RWBoundsErr thrown if index \a i is not between \c 0
     * and \c Size-1, inclusive.
     */
    RWBitRef              operator[](size_t i) {
        if (i >= N) {
            RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEX, i, N)));
        }
        return RWBitRef(vec_, i);
    }

    /**
     * Returns the value of the \a i <sup>th</sup> bit of self. Bounds checking is
     * performed.
     *
     * @throw RWBoundsErr is thrown if index \a i is not between \c 0 and \c Size-1,
     * inclusive.
     */
    bool                  operator[](size_t i) const {
        if (i >= N) {
            RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEX, i, N)));
        }
        return testBit(i);
    }

    /**
     * Returns a reference to the \a i<sup>th</sup> bit of self.  This
     * reference can be used as an lvalue.  The index \a i must be between
     * \c 0 and \c Size-1, inclusive.  No bounds checking is performed.
     */
    RWBitRef              operator()(size_t i) {
        return RWBitRef(vec_, i);
    }

    /**
     * Returns the value of the \a i <sup>th</sup> bit of self. The index
     * \a i must be between \c 0 and \c Size-1, inclusive.  No bounds
     * checking is performed.
     */
    bool                  operator()(size_t i) const {
        return testBit(i);
    }

    /**
     * Returns \c true if every bit of self is set to the value \a b.
     * Otherwise, returns \c false.
     */
    bool             operator==(bool b) const;

    /**
     * Returns \c true if any bit of self is not set to the value \a b.
     * Otherwise, returns \c false.
     */
    bool             operator!=(bool b) const {
        return !operator==(b);
    }

    /**
     * Returns \c true if each bit of self is set to the same value
     * as the corresponding bit in \a v.  Otherwise, returns \c false.
     */
    bool             operator==(const RWTBitVec<N>& v) const;

    /**
     * Returns \c true if self is logically less than \a v. Otherwise,
     * returns \c false.
     *
     * @code
     * RWTBitVec<2> v1, v2;
     * v1.setBit(0);     // the value 1 in binary
     * v2.setBit(1);     // the value 2 in binary
     * assert (v1 < v2); // 1 is less than 2
     * @endcode
     */
    bool             operator< (const RWTBitVec<N>& v) const;

    /**
     * Returns \c true if any bit of self is not set to the same value
     * as the corresponding bit in \a v.  Otherwise, returns \c false.
     */
    bool             operator!=(const RWTBitVec<N>& v) const {
        return !(operator==(v));
    }

    /**
     * Returns the logical \c AND of self and \a v2.
     */
    RWTBitVec<N> operator&(const RWTBitVec<N>& v2) const {
        RWTBitVec<N> temp(*this);
        temp &= v2;
        return temp;
    }

    /**
     * Returns the logical \c XOR of self and \a v2.
     */
    RWTBitVec<N> operator^(const RWTBitVec<N>& v2) const {
        RWTBitVec<N> temp(*this);
        temp ^= v2;
        return temp;
    }

    /**
     * Returns the logical \c OR of self and \a v2.
     */
    RWTBitVec<N> operator|(const RWTBitVec<N>& v2) const {
        RWTBitVec<N> temp(*this);
        temp |= v2;
        return temp;
    }

    /**
     * Clears (i.e., sets to \c false) the bit with index \a i.  The
     * index \a i must be between \c 0 and \c Size-1.  No bounds checking
     * is performed.  The following two lines are equivalent, although using
     * clearBit(size_t) is slightly smaller and faster than using operator()(size_t):
     *
     * @code
     * a(i) = false;
     * a.clearBit(i);
     * @endcode
     */
    void                  clearBit(size_t i) {
        vec_[i >> 3] &= ~(1 << (7 & i));
    }

    /**
     * Returns a \c const pointer to the raw data of self.
     */
    const RWByte*         data() const {
        return vec_;
    }

    /**
     * Sets (i.e., sets to \c true) the bit with index \a i.  The index
     * \a i must be between \c 0 and \c Size-1.  No bounds checking
     * is performed.  The following two lines are equivalent, although using
     * setBit(size_t) is slightly smaller and faster than using operator()(size_t).
     *
     * @code
     * a(i) = true;
     * a.setBit(i);
     * @endcode
     */
    void                  setBit(size_t i) {
        vec_[i >> 3] |= (1 << (7 & i));
    }

    /**
     * Tests the bit with index \a i.  The index \a i must be between
     * \c 0 and \c Size-1.  No bounds checking is performed.  The following
     * are equivalent, although using
     * \link testBit(size_t) const testBit(size_t)\endlink is slightly smaller
     * and faster than using operator()(size_t):
     *
     * @code
     * if( a(i) ) doSomething();
     * if( a.testBit(i) ) doSomething();
     * @endcode
     *
     */
    bool             testBit(size_t i) const {
        return (vec_[i >> 3] & (1 << (7 & i))) != 0;
    }

    /**
     * Returns the index of the first \c ON (\c true) bit in self.  Returns
     * #RW_NPOS if there is no \c ON bit.
     */
    size_t                firstTrue() const {
        return rwFindOne(vec_, (size_t)N);
    }

    /**
     * Returns the index of the first \c OFF (\c false) bit in self.  Returns
     * #RW_NPOS if there is no \c OFF bit.
     */
    size_t                firstFalse() const {
        return rwFindZero(vec_, (size_t)N);
    }

private:

    RWByte                vec_[(N + 7) >> 3 ];

};

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tbitvec.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include <rw/tbitvec.cc>
#endif


#endif /* RW_TOOLS_TBITVEC_H */
