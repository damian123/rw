#ifndef RW_TOOLS_TVRTARRY_H
#define RW_TOOLS_TVRTARRY_H



/**********************************************************************
 *
 * RWTValVirtualArray<T>: A swapping virtual array of type T
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tvrtarry.h#1 $
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
#include <rw/tvref.h>

#if !defined(RW_DISABLE_DEPRECATED)
RW_SUPPRESS_DEPRECATED_WARNINGS

template <class T> class RWTVirtualElement;
template <class T> class RWTVirtualSlice;

/**
 * @ingroup traditional_collection_classes
 * @deprecated As of SourcePro 11.1.
 *
 * @brief Deprecated. A virtual array of templatized objects.
 *
 * This class represents a virtual array of elements of type
 * \c T of almost any length.  Individual elements are brought into
 * physical memory as needed.  If an element is updated, it
 * is automatically marked as "dirty" and will be rewritten to the
 * swapping medium.
 *
 * The swap space is provided by an abstract page heap
 * specified by the constructor.  Any number of virtual arrays can
 * use the same abstract page heap.
 * @note
 * Take care that the destructor of the abstract page heap is not
 * called before all virtual arrays built from it have been destroyed.
 *
 * The class supports reference counting using a copy-on-write technique,
 * so, for example, returning a virtual array by value is efficient.
 * Be aware, however, that if the copy-on-write machinery must make a
 * copy, large arrays can be time-consuming.
 *
 * For efficiency, more than one element can (and should) be put
 * on a page.  The actual number of elements is equal to the page
 * size divided by the element size, rounded downwards.  Example:
 * for a page size of 512 bytes, and an element size of 8, then
 * 64 elements would be put on a page.
 *
 * The indexing operator operator[](long) actually returns an
 * object of type \link RWTVirtualElement RWTVirtualElement<T>\endlink.
 * Consider this example:
 *
 * @code
 *
 * double d = vec[j];
 * vec[i] = 22.0;
 * @endcode
 *
 * Assume that \c vec is of type
 * \link RWTValVirtualArray RWTValVirtualArray<double>\endlink.  The
 * expression \c vec[j] returns an object of type
 * \link RWTVirtualElement RWTVirtualElement<double>\endlink, which
 * contains a reference to the element being addressed. In the first line, this
 * expression is being used to initialize a \c double.  The class
 * \link RWTVirtualElement RWTVirtualElement<T>\endlink
 * contains a type conversion operator to convert itself to a \c T, in
 * this case a \c double.  The compiler uses this to initialize \c d in the
 * first line.  In the second line, the expression \c vec[i] is being
 * used as an lvalue.  In this case, the compiler uses the assignment
 * operator for \link RWTVirtualElement RWTVirtualElement<T>\endlink.  This
 * assignment operator recognizes that the expression is being used as an
 * lvalue and automatically marks the appropriate page as "dirty," thus
 * guaranteeing that it will be written back out to the swapping medium.
 *
 * Slices, as well as individual elements, can also be addressed.
 * These should be used wherever possible as they are much more
 * efficient because they allow a page to be locked and used multiple
 * times before unlocking.
 *
 * The class \c T must have:
 *
 * -  well-defined copy semantics (<tt>T::T(const T&)</tt> or equivalent);
 * -  well-defined assignment semantics <tt>(T::operator=(const T&)</tt> or
 *    equivalent).
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tvrtarry.h>
 * RWVirtualPageHeap* heap;
 * RWTValVirtualArray<T> array(1000L, heap);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/tvrtarry.h>
 * #include <rw/diskpage.h>
 *
 * struct ErsatzInt
 * {
 * private:
 *
 *     char buf_ [16];
 *
 * public:
 *
 *     ErsatzInt (int i) {
 *         sprintf (buf_, "%d", i);
 *     }
 *
 *     const char* value () const {
 *         return buf_;
 *     }
 *
 *     friend std::ostream&
 *     operator<< (std::ostream& s, const ErsatzInt& ref) {
 *         s << atoi (ref.value ());
 *         return s;
 *     }
 * };
 *
 * int main()
 * {
 *     RWDiskPageHeap heap;
 *     RWTValVirtualArray<ErsatzInt> vec1(10000L, &heap);
 *
 *     for (int i = 0; i < 10000; i++)
 *         vec1 [i] = i; // Some compilers may need a cast here
 *
 *     std::cout << vec1[100] << std::endl; // Prints "100"
 *     std::cout << vec1[300] << std::endl; // Prints "300"
 *
 *     RWTValVirtualArray<ErsatzInt> vec2 = vec1.slice(5000L, 500L);
 *
 *     std::cout << vec2.length() << std::endl; // Prints "500"
 *     std::cout << vec2[0] << std::endl; // Prints "5000";
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program output:
 * @code
 *     100
 *     300
 *     500
 *     5000
 * @endcode
 */
template <class T> class RW_DEPRECATE_TYPE("") RWTValVirtualArray
{

public:
    /**
     * Returns a copy of the value at index \a i.
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0 and one
     * less than the number of entries in self.
     */
    T        val(long i) const {   // Readonly access
        return vref_->val(i);
    }

    /**
     * Sets the value at the index \a i to \a v.
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0 and one
     * less than the number of entries in self.
     */
    void  set(long i, const T& v) {       // Set a value
        cow();
        vref_->set(i, v);
    }

    /**
     * Constructs a vector of length \a size.  The pages for the vector
     * are allocated from the page heap given by \a heap which can
     * be of any type.
     */
    RWTValVirtualArray(long size, RWVirtualPageHeap* heap);

    /**
     * Constructs a vector as a copy of \a v.  The resultant vector
     * uses the same heap and has the same length as \a v.  The
     * actual copy is not made until a write, minimizing the amount
     * of heap allocations and copying that must be done.
     */
    RWTValVirtualArray(const RWTValVirtualArray<T>& v);

    /**
     * Constructs a vector from a slice of another vector.  The resultant
     * vector uses the same heap as the vector whose slice is being
     * taken.  Its length is given by the length of the slice.
     * The copy is made immediately.
     */
    RWTValVirtualArray(const RWTVirtualSlice<T>& sl);

    /**
     * Releases all pages allocated by the vector.
     */
    ~RWTValVirtualArray();

    /**
     * Sets self to a copy of \a v.  The resultant vector will use the
     * same heap and have the same length as \a v.  The actual copy
     * will not be made until a write, minimizing the amount of heap
     * allocations and copying that must be done.
     */
    RWTValVirtualArray<T>& operator=(const RWTValVirtualArray<T>& v);

    /**
     * Sets self equal to a slice of another vector.  The resultant
     * vector uses the same heap as the vector whose slice is being
     * taken.  Its length is given by the length of the slice.
     * The copy is made immediately.
     */
    void                  operator=(const RWTVirtualSlice<T>& sl);

    /**
     * Sets all elements in self equal to \a val, and returns a copy of \a val.
     * This operator is actually quite efficient because it can work with
     * many elements on a single page at once.
     */
    T                     operator=(const T& val);

    /**
     * Returns the length of the vector.
     */
    long                  length() const {                // Length of vector
        return vref_->length();
    }

    /**
     * Returns a copy of the value at index \a i.
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0 and one
     * less than the number of entries in self.
     */
    T                     operator[](long i) const {      // Readonly access
        return vref_->val(i);
    }

    /**
     * Returns a reference to the value at index \a i.  The results
     * can be used as an lvalue.
     * @throw RWBoundsErr Thrown if index \a i is not between \c 0 and one
     * less than the number of entries in self.
     */
    inline RWTVirtualElement<T>
    operator[](long i);       // Element as lvalue

    /**
     * Returns a reference to a slice of self.  The value
     * \a start is the starting index of the slice, the value \a length
     * its extent.  The results can be used as an lvalue.
     */
    inline RWTVirtualSlice<T>
    slice(long start, long length); // Slice as lvalue

    /**
     * Changes the length of the vector to \a newLength.  If the vector's
     * length is increased, then the value of the new elements
     * is undefined.
     */
    void                  reshape(long newLength) {
        cow();
        vref_->reshape(newLength);
    }

    /**
     * Returns a pointer to the heap from which the vector is getting
     * its pages.
     */
    RWVirtualPageHeap*    heap() const {
        return vref_->heap();
    }

private:

    RWTVirtualRef<T>*     vref_;

    void                  cow();          // Copy On Write
    friend class RWTVirtualElement<T>;
    friend class RWTVirtualSlice<T>;

};

/****************************************************************
 *                                                              *
 *              Declaration of RWTVirtualElement<T>             *
 *                                                              *
 ****************************************************************/

template <class T> class  RWTVirtualElement
{

public:

    operator      T() const {
        return varray_->val(start_);
    }
    T             operator=(const T& v) {
        varray_->set(start_, v);
        return v;
    }

protected:

    // Protected constructor:
    RWTVirtualElement
    (
        RWTValVirtualArray<T>* v,
        long s
    ) : varray_(v), start_(s) { }

    RWTValVirtualArray<T>*        varray_;
    long                          start_;

private:

    friend class RWTValVirtualArray<T>;

};


/****************************************************************
 *                                                              *
 *              Declaration of RWTVirtualSlice<T>               *
 *                                                              *
 ****************************************************************/

template <class T> class  RWTVirtualSlice : public RWTVirtualElement<T>
{

public:

    void                  operator=(const RWTVirtualSlice<T>&);
    void                  operator=(const RWTValVirtualArray<T>&);
    T                     operator=(const T& val);

protected:

    // Protected constructor:
    RWTVirtualSlice
    (
        RWTValVirtualArray<T>* v,
        long s,                     // Slice start
        long e                      // Slice extent
    ) : RWTVirtualElement<T>(v, s),
        extent_(e) {
        ;
    }

    long                  extent_;

private:

    friend class RWTValVirtualArray<T>;

};

/****************************************
 *                                      *
 *              INLINES                 *
 *                                      *
 ****************************************/

template <class T>
inline RWTVirtualElement<T> RWTValVirtualArray<T>::operator[](long i)
{
    RWTVirtualElement<T> value(this, i);
    return value;
}

template <class T>
inline RWTVirtualSlice<T> RWTValVirtualArray<T>::slice(long start, long length)
{
    RWTVirtualSlice<T> value(this, start, length);
    return value;
}

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("tvrtarry.cc")
#endif

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tvrtarry.cc"
#endif

RW_RESTORE_DEPRECATED_WARNINGS
#endif

#endif  /* RW_TOOLS_TVRTARRY_H */
