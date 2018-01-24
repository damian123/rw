#ifndef RW_TOOLS_BITVEC_H
#define RW_TOOLS_BITVEC_H



/**********************************************************************
 *
 * Ar`bit'rarily long bit vector.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/bitvec.h#3 $
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


/*
 * Defining the preprocessor directive RWBOUNDS_CHECK will
 * cause bounds checking on the subscripting operator.
 */

#include <rw/defs.h>
#include <rw/bitref.h>
#include <rw/tools/hash.h>
#include <rw/tools/ristream.h>
#include <rw/tools/rostream.h>
#include <rw/tools/pointer.h>

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

/**
 * @ingroup traditional_collection_classes
 *
 * @brief Represents a bit vector whose length can be changed at
 * runtime.
 *
 * Class RWBitVec is a bit vector whose length can be changed at
 * runtime. Because this requires an extra level of indirection,
 * this makes it slightly less efficient than \link RWTBitVec
 * RWTBitVec<Size> \endlink, whose lengths are fixed at compile time.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/bitvec.h>
 * RWBitVec v;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Simple
 *
 * @section example Example
 *
 * @code
 * #include <rw/bitvec.h>
 * #include <iostream>
 *
 * int main(){
 *    // Allocate a vector with 20 bits, set to true:
 *    RWBitVec av(20, true);
 *
 *    av(0) = false;     // Turn bit 0 off
 *    av(2) = false;     // Turn bit 2 off
 *    av.clearBit(7);    // Turn bit 7 off
 *    av.setBit(2);      // Turn bit 2 back on
 *
 *    for(int i=11; i<=14; i++) av(i) = false;
 *
 *    std::cout << av << std::endl;    // Print the vector out
 *    return 0;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 * [
 *  0 1 1 1 1 1 1 0 1 1 1 0 0 0 0 1 1 1 1 1
 * ]
 * @endcode
 */
class RW_TOOLS_SYMBOLIC RWBitVec RWMemoryPool_OPTION
{

public:

    /**
     * Constructs a zero length (null) vector.
     */
    RWBitVec() : npts_(0) {
        ;
    }

    /**
     * Constructs a vector with \a N bits. The initial value of the bits
     * is undefined.
     */
    RWBitVec(size_t N);

    /**
     * Constructs a vector with \a N bits, each set to the Boolean value
     * \a initVal.
     */
    RWBitVec(size_t N, bool initVal);

    /**
     * Constructs a vector with \a N bits, initialized to the data in
     * the array of bytes pointed to by \a bp. This array must be at
     * least long enough to contain \a N bits. The identifier \c RWByte
     * is a typedef for an <tt>unsigned char</tt>.
     */
    RWBitVec(const RWByte* bp, size_t N);

    /**
     * Copy constructor. Uses value semantics. The constructed vector
     * is a copy of \a v.
     */
    RWBitVec(const RWBitVec& v);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed vector takes ownership of the
     * data owned by \a v.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWBitVec(RWBitVec && v);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * The destructor. Releases any allocated memory.
     */
    ~RWBitVec();

    // Assignment operators
    /**
     * Assignment operator. Uses value semantics. Self is a copy of \a v.
     */
    RWBitVec&             operator=(const RWBitVec& v);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a v.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWBitVec& operator=(RWBitVec && v);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Assignment operator. Sets every bit in self to the boolean value
     * \a b.
     */
    RWBitVec&             operator=(bool b);

    /**
     * Logical assignment. Sets each element of self to the logical
     * \c AND of self and the corresponding bit in \a v. Self and
     * \a v must have the same number of elements (i.e., be conformal)
     * or an exception of type RWInternalErr is thrown.
     */
    RWBitVec&             operator&=(const RWBitVec& v) {
        rwAND(vec_.get(), v.vec_.get(), nbytes());
        return *this;
    }
    /**
     * Logical assignment. Sets each element of self to the logical
     * \c XOR of self and the corresponding bit in \a v. Self and
     * \a v must have the same number of elements (i.e., be conformal)
     * or an exception of type RWInternalErr is thrown.
     */
    RWBitVec&             operator^=(const RWBitVec& v) {
        rwXOR(vec_.get(), v.vec_.get(), nbytes());
        return *this;
    }
    /**
     * Logical assignment. Sets each element of self to the logical
     * \c OR of self and the corresponding bit in \a v. Self and
     * \a v must have the same number of elements (i.e., be conformal)
     * or an exception of type RWInternalErr is thrown.
     */
    RWBitVec&             operator|=(const RWBitVec& v) {
        rwOR(vec_.get(), v.vec_.get(), nbytes());
        return *this;
    }

    // Indexing operators
    /**
     * Returns a reference to bit \a i of self. A helper class, RWBitRef,
     * is used. The result can be used as an lvalue. The index \a i
     * must be between \c 0 and the length of the vector less one. Bounds
     * checking is performed. If the index is out of range, then an
     * exception of type RWBoundsErr is thrown.
     */
    RWBitRef              operator[](size_t i);

    /**
     * Returns a reference to bit \a i of self. A helper class, RWBitRef,
     * is used. The result can be used as an lvalue. The index \a i
     * must be between \c 0 and the length of the vector less one. Bounds
     * checking is performed only if the preprocessor macro \c RWBOUNDS_CHECK
     * has been defined before including the header file \c <rw/bitvec.h>.
     * If so, and if the index is out of range, then an exception of
     * type RWBoundsErr is thrown.
     */
    RWBitRef              operator()(size_t i);

    /**
     * Returns the boolean value of bit \a i. The result cannot be used
     * as an lvalue. The index \a i must be between \c 0 and the length
     * of the vector less one. Bounds checking is performed. If the
     * index is out of range, then an exception of type RWBoundsErr
     * is thrown.
     */
    bool             operator[](size_t i) const;

    /**
     * Returns the boolean value of bit \a i. The result cannot be used
     * as an lvalue. The index \a i must be between \c 0 and the length
     * of the vector less one. Bounds checking is performed only if
     * the preprocessor macro \c RWBOUNDS_CHECK has been defined before
     * including the header file \c <rw/bitvec.h>. If so, and if the
     * index is out of range, then an exception of type RWBoundsErr
     * is thrown.
     */
    bool             operator()(size_t i) const;

    // Other operators ---
    /**
     * Returns \c true if self and \a v have the same length and if
     * each bit of self is set to the same value as the corresponding
     * bit in \a v. Otherwise, returns \c false.
     */
    bool             operator==(const RWBitVec& v) const {
        return  isEqual(v);
    }

    /**
     * Returns \c false if self and \a v have the same length and if
     * each bit of self is set to the same value as the corresponding
     * bit in \a v. Otherwise, returns \c true.
     */
    bool             operator!=(const RWBitVec& v) const {
        return !isEqual(v);
    }

    /**
     * Returns \c true if every bit of self is set to the boolean value
     * \a b. Otherwise, returns \c false.
     */
    bool             operator==(bool b) const;

    /**
     * Returns \c false if every bit of self is set to the boolean value
     * \a b. Otherwise, returns \c true.
     */
    bool             operator!=(bool b) const {
        return !operator==(b);
    }

    /**
     * Unary operator that returns the logical negation of vector \a v.
     */
    friend RW_TOOLS_SYMBOLIC RWBitVec      operator!(const RWBitVec& v);

    /**
     * Returns a vector that is the logical \c AND
     * of the vectors \a v1 and \a v2. The two vectors must have the
     * same length or an exception of type RWInternalErr is thrown.
     */
    friend RW_TOOLS_SYMBOLIC RWBitVec      operator&(const RWBitVec& v1, const RWBitVec& v2);
    /**
     * Returns a vector that is the logical \c XOR
     * of the vectors \a v1 and \a v2. The two vectors must have the
     * same length or an exception of type RWInternalErr is thrown.
     */
    friend RW_TOOLS_SYMBOLIC RWBitVec      operator^(const RWBitVec& v1, const RWBitVec& v2);
    /**
     * Returns a vector that is the logical \c OR
     * of the vectors \a v1 and \a v2. The two vectors must have the
     * same length or an exception of type RWInternalErr is thrown.
     */
    friend RW_TOOLS_SYMBOLIC RWBitVec      operator|(const RWBitVec& v1, const RWBitVec& v2);

    /**
     * Prints the bit vector \a v on the output stream \a s.
     */
    friend RW_TOOLS_SYMBOLIC std::ostream&      operator<<(std::ostream& s, const RWBitVec& v);

    /**
     * Reads the bit vector \a v from the input stream \a s.
     */
    friend RW_TOOLS_SYMBOLIC std::istream&      operator>>(std::istream& s, RWBitVec& v);

    // Member functions
    /**
     * Clears (i.e., sets to \c false) the bit with index \a i. The
     * index \a i must be between \c 0 and the length of the vector
     * less one. No bounds checking is performed. The following are
     * equivalent, although clearBit(size_t) is slightly smaller and
     * faster than using operator()(size_t):
     *
     * @code
     * a(i) = false;
     * a.clearBit(i);
     * @endcode
     */
    void                  clearBit(size_t i);

    /**
     * Returns a const pointer to the raw data of self. Should be
     * used with care.
     */
    const RWByte*         data() const {
        return vec_.get();
    }

    /**
     * Returns the index of the first \c false bit in self. Returns
     * #RW_NPOS if there is no \c false bit.
     */
    size_t                firstFalse() const;

    /**
     * Returns the index of the first \c true bit in self. Returns
     * #RW_NPOS if there is no \c true bit.
     */
    size_t                firstTrue() const;

    /**
     * Returns a value suitable for hashing.
     */
    unsigned              hash() const;

    /**
     * Returns \c true if self and \a v have the same length and if
     * each bit of self is set to the same value as the corresponding
     * bit in \a v. Otherwise, returns \c false.
     */
    bool                  isEqual(const RWBitVec& v) const;

    /**
     * Returns the number of bits in the vector.
     */
    size_t                length() const {
        return npts_;
    }

    /**
     * Prints the bit vector on the output stream \a s. See the example
     * above for a sample of the format.
     */
    std::ostream&         printOn(std::ostream& s) const;

    void                  restoreFrom(RWvistream&);
    void                  restoreFrom(RWFile&);

    /**
     * Resizes the vector to have length \a N. If this results in a
     * lengthening of the vector, the additional bits are set to \c false.
     */
    void                  resize(size_t N);

    void                  saveOn(RWvostream&) const;
    void                  saveOn(RWFile&) const;

    /**
     * Reads the bit vector from the input stream \a s. The vector is
     * dynamically resized as necessary. The vector should be in
     * the same format printed by member function printOn(std::ostream&).
     */
    std::istream&              scanFrom(std::istream& s);

    /**
     * Sets (i.e., sets to \c true) the bit with index \a i. The index
     * \a i must be between \c 0 and \c size-1. No bounds checking is
     * performed. The following are equivalent, although setBit(size_t)
     * is slightly smaller and faster than using operator()(size_t):
     *
     * @code
     * a(i) = true;
     * a.setBit(i);
     * @endcode
     */
    void             setBit(size_t i);                       // Set bit i

    /**
     * Tests the bit with index \a i. The index \a i must be between
     * \c 0 and \c size-1. No bounds checking is performed. The following
     * are equivalent, although testBit(size_t) is slightly smaller
     * and faster than using operator()(size_t):
     *
     * @code
     * if( a(i) )              doSomething();
     * if( a.testBit(i) )      doSomething();
     * @endcode
     */
    bool             testBit(size_t i) const;                // Return value of bit i

    /**
     * Swaps the data owned by self with the data owned by \a v.
     */
    void             swap(RWBitVec& v);

    // Other friend functions:
    /**
     * Returns the total number of bits set in the vector \a v.
     */
    friend size_t RW_TOOLS_SYMBOLIC        sum(const RWBitVec& v);           // Total # of bits set

private:

    void                  indexRangeErr(size_t) const;
    void                  lengthErr(size_t, size_t) const;
    size_t                nbytes() const {
        return (npts_ + 7) >> 3;
    }
    size_t                nfull() const {
        return npts_ >> 3;
    }

    RWTScopedPointer<RWByte[]> vec_; // The bit data, packed within a byte vector
    size_t npts_;                    // Length of vector in bits

};

/**
 * @relates RWBitVec
 *
 * Restores an RWBitVec into \a vec from a virtual stream,
 * replacing the previous contents of \a vec.
 */
inline RWvistream& operator>>(RWvistream& str,       RWBitVec& vec)
{
    vec.restoreFrom(str);
    return str;
}

/**
 * @relates RWBitVec
 *
 * Restores an RWBitVec into \a vec from an RWFile,
 * replacing the previous contents of \a vec.
 */
inline RWFile&     operator>>(RWFile& file,          RWBitVec& vec)
{
    vec.restoreFrom(file);
    return file;
}

inline RWvistream& operator>>(RWvistream& str,       RWBitVec*& vec)
{
    vec = new RWBitVec(1);
    vec->restoreFrom(str);
    return str;
}
inline RWFile&     operator>>(RWFile& file,          RWBitVec*& vec)
{
    vec = new RWBitVec(1);
    vec->restoreFrom(file);
    return file;
}

/**
 * @relates RWBitVec
 *
 * Saves the RWBitVec \a vec to a virtual stream.
 */
inline RWvostream& operator<<(RWvostream& str, const RWBitVec& vec)
{
    vec.saveOn(str);
    return str;
}

/**
 * @relates RWBitVec
 *
 * Saves the RWBitVec \a vec to an RWFile.
 */
inline RWFile&     operator<<(RWFile& file,    const RWBitVec& vec)
{
    vec.saveOn(file);
    return file;
}

inline size_t RWBitVec::firstFalse() const
{
    return rwFindZero(vec_.get(), npts_);
}
inline size_t RWBitVec::firstTrue() const
{
    return rwFindOne(vec_.get(), npts_);
}

inline RWBitRef
RWBitVec::operator[](size_t i)
{
    if (i >= npts_) {
        indexRangeErr(i);
    }
    return RWBitRef(vec_.get(), i);
}

inline RWBitRef
RWBitVec::operator()(size_t i)
{
#ifdef RWBOUNDS_CHECK
    if (i >= npts_) {
        indexRangeErr(i);
    }
#endif
    return RWBitRef(vec_.get(), i);
}

inline bool
RWBitVec::operator[](size_t i) const
{
    if (i >= npts_) {
        indexRangeErr(i);
    }
    return testBit(i);
}

inline bool
RWBitVec::operator()(size_t i) const
{
#  ifdef RWBOUNDS_CHECK
    if (i >= npts_) {
        indexRangeErr(i);
    }
#  endif
    return testBit(i);
}

inline void
RWBitVec::clearBit(size_t i)
{
    (*(vec_.get() + (i >> 3))) &= ~(1 << (7 & i));
}

inline void
RWBitVec::setBit(size_t i)
{
    (*(vec_.get() + (i >> 3))) |= (1 << (7 & i));
}

inline bool
RWBitVec::testBit(size_t i) const
{
    return (*(((i) >> 3) + (vec_.get())) & (1 << (7 & (i)))) != 0;
}

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an RWBitVec.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<RWBitVec> : public std::unary_function<RWBitVec, size_t> {
    /**
     * Returns a hash of \a obj. This function is the equivalent of calling
     *
     * @code
     * obj.hash();
     * @endcode
     */
    size_t operator()(const RWBitVec& obj) const {
        return obj.hash();
    }
};

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif /* RW_TOOLS_BITVEC_H */
