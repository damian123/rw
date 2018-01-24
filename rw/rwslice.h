#ifndef __RWRWSLICE_H__
#define __RWRWSLICE_H__

/***************************************************************************
 *
 * RWSlice:   Base subscripting class - subscript using a string or a slice
 *            RWRange:  Subscript a range of indices
 *            RWToEnd:  Subscript everything from a given index to the end
 *
 * $Id: //math/13/rw/rwslice.h#1 $
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
 ***************************************************************************
 *
 * It is required that the stride in an RWRange and an RWToEnd be positive.
 * Reasons:  The semantics of negative strides in an RWRange are unclear.
 * Constructing an RWToEnd with a negative stride, RWToEnd(b,-2) eg,
 * has the same semantics as the clearer RWRange(b,0,2).  Prohibiting
 * a negative stride in this case also makes the inline function
 * RWSlice::len() considerably simpler.
 *
 **************************************************************************/

#include <rw/mathdefs.h>

/*
 * The slice data is kept in a separate struct so that we can
 * initialize RWAll at compile time using an initializer.
 */

struct RWSliceData {
    int      theBegin;
    int      theLen;    // A negative value means as long as possible
    int      theStride; // Stride of zero means collapse this dimension
};

/**
 * @ingroup math_collection_classes
 * @brief Represents an index that can be used for subscripting
 * vectors, matrices, and arrays.
 *
 * An RWSlice object is an index which can be used for subscripting
 * vectors, matrices, and arrays. By subscripting with RWSlice objects,
 * you create views of selected elements. These new views are vectors,
 * matrices, or arrays in their own right, rather than simply helper
 * classes. This means that a view created with subscripting can
 * be used as an rvalue, an lvalue, or kept as an object for use
 * later.
 *
 * The classes RWRange and RWToEnd are derived from RWSlice. RWRange
 * and RWToEnd objects can be used wherever an RWSlice object can
 * be used.
 *
 * Although they are not declared, the C++ language automatically
 * defines a copy constructor and an assignment operator. These
 * can be useful when passing RWSlice objects as function arguments
 * or using RWSlice objects as class instance variables.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/math/mathvec.h>
 * RWMathVec<double> x(20,2);
 * RWMathVec<double> y = x("4:9"); // set y to view
 *                                 // elements 4 through 9
 * @endcode

 * @section example Example
 *
 * @code
 * #include <iostream.h>
 * #include <rw/math/genmat.h>
 *
 * int main()
 * {
 *    RWGenMat<int> A(6,6);
 *    RWSlice I("0::2");  // indices 0,2,4
 *    RWSlice J(1,3,2);   // indices 1,3,5
 *    A(I,I) = 1;
 *    A(I,J) = 2;
 *    A(J,I) = 3;
 *    A(J,J) = 4;
 *    cout << A << endl;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 * 6x6
 * [
 *      1     2     1     2     1     2
 *      3     4     3     4     3     4
 *      1     2     1     2     1     2
 *      3     4     3     4     3     4
 *      1     2     1     2     1     2
 *      3     4     3     4     3     4
 * ]
 * @endcode
 *
 *
 * @see RWRange, RWToEnd, RWMathVec::operator()(), RWGenMat::operator()(), RWMathArray::operator()()
 *
 */
class RW_MATH_SYMBOLIC RWSlice : protected RWSliceData
{
    friend class RW_MATH_SYMBOLIC RWRange;
    friend class RW_MATH_SYMBOLIC RWToEnd;

    // For use by the friend classes.  The argument is a dummy to
    // distinguish this constructor.
    RWSlice(double) {}

public:
    RWSlice();                    // Defaults to all entries

    RWSlice(const RWSlice&);      // Copy ctor needs to be non-inline to avoid bcc for OS/2 bug

    RWSlice(int i);               // Extracts one index entry

    /**
     * Constructs an object that indexes \a length elements starting
     * at element \a begin.
     */
    RWSlice(int begin, int length);

    /**
     * Constructs an object that indexes \a length elements starting
     * at element \a begin. The parameter \a stride is the increment
     * between successive selected elements. If the stride is negative,
     * the new view reverses the indices.
     */
    RWSlice(int begin, int length, int stride);

    /**
     * Constructs an index from the null terminated character string
     * \a s. The syntax of the character string is \c x:y:z, where \c
     * x, \c y, and \c z are integers. The resulting index selects elements
     * from \c x through \c y, inclusive, with an increment of \c z
     * between successive selected elements. Any or all of \c x, \c y, and
     * \c z may be left out, in which case they default to 0, the last
     * element, and 1, respectively. If \c z is omitted, the second
     * colon may also be omitted. The character string \c * is a synonym
     * for \c :, which selects all elements.
     */
    RWSlice(const char* s);        // Parse a string of form "b:e:s"

    static void boundsCheck(int l, int s);  // Check l>=0, s!=0
    // The int constructor is meant for automatic conversion type from
    // an integer.  This situation is marked by setting the stride to
    // zero, it is checked for using the collapse() function.

    void  boundsCheck(int vecLen) const; // Check that all indicated elements in vector
    int   collapse() const {
        return theStride == 0;
    }
    int   begin() const {
        return theBegin;
    }
    int   len(int vecLen) const {
        return (theLen >= 0) ? theLen : (vecLen - theBegin - 1) / theStride + 1;
    }
    int   stride() const {
        return theStride;
    }
    // boundsCheck() differs from the earlier overloaded boundsCheck() in
    // that this one is used by the vector at subscripting time, and the
    // other is used when constructing an RWSlice.
    // collapse() indicates that this RWSlice was constructed via type
    // conversion from an integer.  This implies the user is subscripting
    // with an integer, so this dimension should be collapsed.
    // len() requires the vector length to be passed in because some
    // calculation will occur if this is actually a RWToEnd object (flagged
    // by len<0).
};

/**
 * @ingroup math_collection_classes
 * @brief Represents an index that can be used for subscripting
 * vectors, matrices, and arrays.
 *
 * An RWRange object is an index which can be used for subscripting
 * vectors, matrices, and arrays. By subscripting with RWRange objects,
 * you create views of selected elements. These new views are vectors,
 * matrices, or arrays in their own right, rather than simply helper
 * classes. This means that a view created with subscripting can
 * be used as an rvalue, an lvalue, or kept as an object for use
 * later.
 *
 * The class RWRange is derived from RWSlice. It can be used to
 * initialize an RWSlice object as follows:
 *
 * \c RWSlice \c s \c = \c RWRange(b,e);
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/math/mathvec.h>
 * RWMathVec<double> x(20,2);
 * cout << x(RWRange(3,5));   // print elements 3,4,5
 * @endcode

 * @section example Example
 *
 * @code
 * #include <iostream.h>
 * #include <rw/math/genmat.h>
 *
 * int main()
 * {
 *    RWGenMat<int> A(6,6);
 *    RWRange I(0,2);
 *    RWRange J(3,5);
 *    A(I,I) = 1;
 *    A(I,J) = 2;
 *    A(J,I) = 3;
 *    A(J,J) = 4;
 *    cout << A << endl;
 * }
 *
 * @endcode
 *
 * Program output:
 *
 * @code
 * 6x6
 * [
 *      1     1     1     2     2     2
 *      1     1     1     2     2     2
 *      1     1     1     2     2     2
 *      3     3     3     4     4     4
 *      3     3     3     4     4     4
 *      3     3     3     4     4     4
 * ]
 * @endcode
 *
 *
 * @see RWSlice, RWToEnd, RWMathVec::operator()(), RWGenMat::operator()(), RWMathArray::operator()()
 *
 */
class RW_MATH_SYMBOLIC RWRange : public RWSlice
{
public:
    /**
     * Constructs an object which indexes starting at element \a begin
     * and ending with element \a end. If \a end is smaller than \a
     * begin, the elements are accessed in reverse order.
     */
    RWRange(int begin, int end);

    /**
     * Constructs an object which indexes starting at element \a begin
     * and ending with element \a end. If \a end is smaller than \a
     * begin, the elements are accessed in reverse order. The parameter
     * \a stride gives the increment between selected elements; it must
     * always be greater than 0.
     */
    RWRange(int begin, int end, int stride);

    static void boundsCheck(int s);// Ensure s>0
};

/**
 * @ingroup math_collection_classes
 * @brief Represents an index that can be used for subscripting
 * vectors, matrices, and arrays from a given element to the end.
 *
 * An RWToEnd object is an index which can be used for subscripting
 * vectors, matrices, and arrays from a given element to the end.
 * By subscripting with RWToEnd objects, you create views of selected
 * elements. These new views are vectors, matrices, or arrays in
 * their own right, rather than simply helper classes. This means
 * that a view created with subscripting can be used as an rvalue,
 * an lvalue, or kept as an object for use later.
 *
 * The class RWToEnd is derived from RWSlice. It can be used to
 * initialize an RWSlice object as follows:
 *
 * @code
 * RWSlice s = RWToEnd(3);
 * @endcode
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/math/mathvec.h>
 * RWMathVec<double> x(20);
 * cout << x(RWToEnd(4));  // output elements 4 to last
 * @endcode

 * @section example Example
 *
 * @code
 * #include <iostream.h>
 * #include <rw/math/genmat.h>
 *
 * int main()
 * {
 *    RWGenMat<int> A(6,6);
 *    A = 0;
 *    RWToEnd I(3);        // indices 3,4,5
 *    A(I,I) = 1;
 *    cout << A << endl;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 * 6x6
 * [
 *      0     0     0     0     0     0
 *      0     0     0     0     0     0
 *      0     0     0     0     0     0
 *      0     0     0     1     1     1
 *      0     0     0     1     1     1
 *      0     0     0     1     1     1
 * ]
 * @endcode
 *
 *
 * @see RWRange, RWSlice, RWMathVec::operator()(), RWGenMat::operator()(), RWMathArray::operator()()
 *
 */
class RW_MATH_SYMBOLIC RWToEnd : public RWSlice
{
public:
    /**
     * Constructs an object which indexes starting at element \a begin
     * and ending with the last element.
     */
    RWToEnd(int begin);

    /**
     * Constructs an object which indexes starting at element \a begin
     * and ending with the last element. The parameter \a stride is the
     * increment between successive selected elements.
     */
    RWToEnd(int begin, int stride);

    static void boundsCheck(int s);// Ensure s>0
};

#if defined(RW_TRAILING_RWEXPORT)
extern const RWSlice& RW_MATH_GLOBAL RWAll;  // Definition in rwslice.cpp
#else
/**
 * @relates RWSlice
 *
 * This special index selects all the elements.
 */
extern RW_MATH_GLOBAL const RWSlice& RWAll;  // Definition in rwslice.cpp
#endif

/*
 * Inline definitions for the constructors.
 */

inline RWSlice::RWSlice()
{
    theBegin = 0;
    theLen = -1;
    theStride = 1;
}

inline RWSlice::RWSlice(int i)
{
    theBegin = i;
    theLen = 0;
    theStride = 0;
#ifdef RWBOUNDS_CHECK
    boundsCheck(theLen, 1); // This is the one instance where stride==0 is ok
#endif
}

inline RWSlice::RWSlice(int b, int l)
{
    theBegin = b;
    theLen = l;
    theStride = 1;
#ifdef RWBOUNDS_CHECK
    boundsCheck(theLen, theStride);
#endif
}

inline RWSlice::RWSlice(int b, int l, int s)
{
    theBegin = b;
    theLen = l;
    theStride = s;
#ifdef RWBOUNDS_CHECK
    boundsCheck(theLen, theStride);
#endif
}

inline RWRange::RWRange(int b, int e) : RWSlice((double)0)
{
    theBegin = b;
    theLen = ((b > e) ? b - e : e - b) + 1;
    theStride = (b > e) ? (-1) : 1;
}

inline RWRange::RWRange(int b, int e, int s) : RWSlice((double)0)
{
#ifdef RWBOUNDS_CHECK
    boundsCheck(s);
#endif
    theBegin = b;
    theLen = ((b > e) ? b - e : e - b) / s + 1;
    theStride = (b > e) ? (-s) : s;
}

inline RWToEnd::RWToEnd(int b) : RWSlice((double)0)
{
    theBegin = b;
    theLen = -1;
    theStride = 1;
}

inline RWToEnd::RWToEnd(int b, int s) : RWSlice((double)0)
{
#ifdef RWBOUNDS_CHECK
    boundsCheck(s);
#endif
    theBegin = b;
    theLen = -1;
    theStride = s;
}

#endif /*__RWRWSLICE_H__*/
