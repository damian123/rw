#ifndef __RWMTHARRIO_CC__
#define __RWMTHARRIO_CC__

/***************************************************************************
 *
 * mtharrio.cc - Definitions for RWMathArray<T> I/O
 *
 * $Id: //math/13/rw/math/mtharrio.cc#1 $
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
#include <rw/mathdefs.h>
#include <rw/rwfile.h>
#include <rw/vstream.h>
#include <rw/rwerr.h>
#include <rw/matherr.h>
#include <rw/dcompio.h>
#ifdef RW_MATH_NO_NEW_HEADER
#  include <stdio.h>
#  include <ctype.h>
#else
#  include <cstdio>
#  include <cctype>
#endif


// Print out the array in row major order
template <class T>
void RWMathArray<T>::printOn(std::ostream& s) const
{
    typedef typename rw_numeric_traits<T>::numeric_type numeric_type;

    for (unsigned int i = 0; i < dimension(); ++i) { // Print dimensions: eg, 5x3x6
        s << length(i);
        if (i < dimension() - 1) {
            s << "x";
        }
    }
    s << " [\n";
    if (length().length() > 0) {
        // Loop through the array printing a plane at a time.  The code
        // is much simpler if we don't have to worry about zero, one or
        // two dimension arrays.  To accommodate this, the looping index is
        // at least 3-D, but the index used to access the array in the 0,1,2D
        // cases is just a slice of the last entries of the index.
        unsigned p = static_cast<unsigned>(dimension());  // Save some typing
        RWIntVec index(((p > 3) ? p : 3), 0);
        RWIntVec accessIndex = index.slice(static_cast<unsigned>(index.length()) - p, p);
        bool isBeginningOfLine = true;
        while (index((int)0) == 0 || (p > 2 && index((int)0) < length((int)0))) {  // Loop over planes
            while (index(static_cast<unsigned>(index.length()) - 2) == 0 || (p > 1 && accessIndex(p - 2) < length(p - 2))) {  // Loop over lines
                while (index(static_cast<unsigned>(index.length()) - 1) == 0 || (p > 0 && accessIndex(p - 1) < length(p - 1))) {
                    if (!isBeginningOfLine) {
                        s << " ";
                    }
                    numeric_type el = (*this)(accessIndex);
                    s << el;
                    isBeginningOfLine = false;
                    ++(index(static_cast<unsigned>(index.length()) - 1)); // Increment last component of index
                }
                isBeginningOfLine = true;
                s << "\n";
                index(static_cast<unsigned>(index.length()) - 1) = 0;
                ++(index(static_cast<unsigned>(index.length()) - 2));
            }
            index(static_cast<unsigned>(index.length()) - 2) = 0;     // Advance to next plane
            if (p < 3) {
                ++(index((int)0));
            }
            else {        // The array is at least 3D
                int r = p - 3;
                int numBlankLinesToPrint = 1;    // Only print these if not done
                while (++(index(r)) >= npts(r) && --r >= 0) {
                    index(r + 1) = 0;
                    numBlankLinesToPrint++;
                }
                if (r >= 0) {
                    while (--numBlankLinesToPrint >= 0) {
                        s << "\n";
                    }
                }
            }
        }
    }
    s << "]";
}

template <class T>
void RWMathArray<T>::scanFrom(std::istream& s)
{
    typedef typename rw_numeric_traits<T>::numeric_type numeric_type;
    numeric_type item;
    char c;

    do {
        s.get(c);
    }
    while (s.good() && c != '[' && !(RWMTH_STD::isdigit)(c));
    s.putback(c);

    RWIntVec n;                     // Read in array dimensions
    if (c != '[') { // if c is a left bracket, this is a 0-D array
        do {
            n.resize(n.length() + 1);
            s >> n(static_cast<unsigned>(n.length() - 1));
            do {
                s.get(c);
            }
            while ((RWMTH_STD::isspace)(c) && s.good());   // Ignore whitespace
        }
        while (s.good() && c == 'x');
    }
    if (c != '[') {
        s.putback(c);
    }

    reshape(n);
    // Loop through all the entries and read them in.
    for (RWMultiIndex i(n); s.good() && i; ++i) {
        s >> item;
        (*this)(i) = static_cast<T>(item);
    }

    if (s.good()) {
        do {
            s.get(c);
        }
        while (s.good() && (RWMTH_STD::isspace)(c));   // Eat the trailing ]
    }
    if (c != ']') {
        s.putback(c);
    }

    return;
}

template <class T>
std::ostream& operator<<(std::ostream& s, const RWMathArray<T>& v)
{
    v.printOn(s);
    return s;
}

template <class T>
std::istream& operator>>(std::istream& s, RWMathArray<T>& v)
{
    v.scanFrom(s);
    return s;
}


//************************************************************************
//
// Save values to a file or stream
//
//************************************************************************

template <class T>
void RWMathArray<T>::saveOn(RWFile& file) const
{
    typedef typename rw_numeric_traits<T>::c_type c_type;

    file.Write(versionID);
    npts.saveOn(file);

    c_type item;
    const_iterator p = begin(), stop = end();
    while (p != stop) {
        item = (c_type)(*p);
        file << item;
        ++p;
    }
}


template <class T>
void RWMathArray<T>::saveOn(RWvostream& s) const
{
    typedef typename rw_numeric_traits<T>::c_type c_type;
    s << versionID;               // Save the version number
    npts.saveOn(s);               // Save the vector length

    c_type item;
    const_iterator p = begin(), stop = end();
    while (p != stop) {
        item = (c_type)(*p);
        s << item;
        ++p;
    }
}

//***********************************************************************
//
// Restore from a file or stream.
//
//***********************************************************************

template <class T>
void RWMathArray<T>::restoreFrom(RWFile& file)
{

    typedef typename rw_numeric_traits<T>::c_type c_type;
    unsigned short ID;
    file.Read(ID);

    if (ID != versionID && ID != 340) {
        versionErr((int)versionID, (int)ID);
    }

    // Get the length and resize to it:
    RWIntVec lenVec;
    lenVec.restoreFrom(file);
    resize(lenVec);

    c_type item;
    iterator p = begin(), stop = end();
    while (p != stop) {
        file >> item;
        *p = T(item);
        ++p;
    }
}

template <class T>
void RWMathArray<T>::restoreFrom(RWvistream& s)
{
    typedef typename rw_numeric_traits<T>::c_type c_type;
    // Get and check the version number
    unsigned short ID;
    s >> ID;

    if (ID != versionID && ID != 340) {
        versionErr((int)versionID, (int)ID);
    }

    // Get the length and resize to it:
    RWIntVec lenVec;
    lenVec.restoreFrom(s);
    resize(lenVec);

    c_type item;
    iterator p = begin(), stop = end();
    while (p != stop) {
        s >> item;
        *p = T(item);
        ++p;
    }
}


//***********************************************************************
//
// binaryStoreSize, for writing to an RWFile
//
//***********************************************************************

template <class T>
size_t RWMathArray<T>::binaryStoreSize() const
{
    // Total storage requirements = Number of elements times their size,
    // plus space for the vector length and ID number:
    return prod(length()) * sizeof(T) + npts.binaryStoreSize() + sizeof(versionID);
}

#endif
