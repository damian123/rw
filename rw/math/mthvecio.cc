#ifndef __RWMTHVECIO_CC__
#define __RWMTHVECIO_CC__

/***************************************************************************
 *
 * mthvecio.cc
 *
 * $Id: //math/13/rw/math/mthvecio.cc#1 $
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

#include <rw/rwfile.h>
#include <rw/vstream.h>
#include <rw/rwerr.h>
#include <rw/matherr.h>
#include <rw/dcomplex.h>
#include <rw/dcompio.h>
#include <rw/mathdefs.h>
#ifdef RW_MATH_NO_NEW_HEADER
#  include <stdio.h>
#  include <ctype.h>
#else
#  include <cstdio>
#  include <cctype>
#endif


const int            default_resize        = 128;


/************************* I / O   R O U T I N E S ***********************/
template< class T >
void RWMathVec<T>::printOn(std::ostream& s, size_t numberPerLine) const
{
    s << "[\n";

    for (register unsigned n = 0; n < length(); n++) {
        if (n) {
            if (numberPerLine == 0 || n % numberPerLine) {
                s << " ";
            }
            else {
                s << "\n";
            }
        }

        typename rw_numeric_traits<T>::numeric_type item = (*this)(n);
        s << item;
    }
    s << "\n]";
}

template<class T>
struct rw_extract_item_imp {
    static bool do_it(std::istream& s, T& item) {
        if (s >> item) {
            return true;
        }
        else {
            return false;
        }
    }
};

#ifdef RW_STREAM_EXTRACTION_COMPLEX_BUG
// there is a bug in gcc where extracting a std::complex type from an istream
// would clear eof but not clear any other error states.
template<typename T>
struct rw_extract_item_imp<std::complex<T> > {
    static bool do_it(std::istream& s, std::complex<T>& item) {
        char c;
        if (!(s >> c)) {
            return false;
        }
        else if (s.putback(c)) {
            if (s >> item) {
                return true;
            }
        }
        return false;
    }
};

#endif

template<class T>
bool rw_extract_item(std::istream& s, T& item)
{
    return rw_extract_item_imp<T>::do_it(s, item);
}

template< class T >
void RWMathVec<T>::scanFrom(std::istream& s)
{
    typename rw_numeric_traits<T>::numeric_type item;
    register unsigned nextspace = 0;
    char c;

    if (!(s >> c)) {
        return;
    }

    if ('[' == c) {
        if (s >> c && ']' != c) {        // look for empty vector
            s.putback(c);
            while (rw_extract_item(s, item) && s >> c) {  // Read vector element
                if (nextspace >= length()) {
                    resize(length() + default_resize);
                }

                (*this)(nextspace++) = static_cast<T>(item);
                if (']' != c) {
                    s.putback(c);
                }
                else {
                    break;
                }
            }
        }
        if (']' != c && s) {
            s.setstate(s.failbit);
        }
    }
    else if (s.putback(c)) {
        while (rw_extract_item(s, item)) {
            if (nextspace >= length()) {
                resize(length() + default_resize);
            }
            (*this)(nextspace++) = static_cast<T>(item);
        }
    }
    if (nextspace != length()) {
        resize(nextspace);    // Trim to fit
    }
}


template< class T >
std::ostream& operator<<(std::ostream& s, const RWMathVec<T>& v)
{
    v.printOn(s);
    return s;
}


template< class T >
std::istream& operator>>(std::istream& s, RWMathVec<T>& v)
{
    v.scanFrom(s);
    return s;
}


//************************************************************************
//
// Save values to a file or stream
//
//************************************************************************
template< class T >
void RWMathVec<T>::saveOn(RWFile& file) const
{
    typedef typename rw_numeric_traits<T>::c_type c_type;
    size_type len = length();
    file.Write(versionID);
    file.Write(len);

    c_type item;
    const_iterator p = begin(), stop = end();
    while (p != stop) {
        item = (c_type)(*p);
        file << item;
        ++p;
    }
}


template< class T >
void RWMathVec<T>::saveOn(RWvostream& s) const
{
    typedef typename rw_numeric_traits<T>::c_type c_type;
    size_t len = length();
    s << versionID;       // Save the version number
    s.putSizeT(len);    // Save the vector length

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
template< class T >
void RWMathVec<T>::restoreFrom(RWFile& file)
{
    typedef typename rw_numeric_traits<T>::c_type c_type;
    // Get and check the version number
    unsigned short ID;
    file.Read(ID);

    if (ID != versionID && ID != 340) {
        versionErr((int)versionID, (int)ID);
    }

    // Get the length and resize to it:
    size_type len;
    file.Read(len);
    resize(len);

    c_type item;
    iterator p = begin(), stop = end();
    RW_ASSERT(data() != 0 || len == 0);
    while (p != stop) {
        file >> item;
        *p = T(item);
        ++p;
    }
}


template< class T >
void RWMathVec<T>::restoreFrom(RWvistream& s)
{
    typedef typename rw_numeric_traits<T>::c_type c_type;
    // Get and check the version number
    unsigned short ID;
    s >> ID;

    if (ID != versionID && ID != 340) {
        RWTHROW(RWExternalErr(RWMessage(RWMATH_BADVERNO, (int)versionID, (int)ID)));
    }

    // Get the length and resize to it:
    size_t len;
    s.getSizeT(len);
    resize(len);

    c_type item;
    iterator p = begin(), stop = end();
    RW_ASSERT(data() != 0 || len == 0);
    while (p != stop) {
        s >> item;
        *p = T(item);
        ++p;
    }
}


template< class T >
size_t RWMathVec<T>::binaryStoreSize() const
{
    // Total storage requirements = Number of elements times their size,
    // plus space for the vector length and ID number:
    return length() * sizeof(T) + sizeof(size_t) + sizeof(versionID);
}

// RW_MATH_BOOL_TOOLS_IO_WORKAROUND defined in rw/mathdefs.h
#if defined(RW_MATH_BOOL_TOOLS_IO_WORKAROUND)

template<>
void RWMathVec<bool>::restoreFrom(RWFile& file)
{
    //typedef typename rw_numeric_traits<bool>::c_type c_type;
    typedef char c_type;
    // Get and check the version number
    unsigned short ID;
    file.Read(ID);

    if (ID != versionID && ID != 340) {
        versionErr((int)versionID, (int)ID);
    }

    // Get the length and resize to it:
    size_type len;
    file.Read(len);
    resize(len);

    c_type item;
    iterator p = begin(), stop = end();
    while (p != stop) {
        file >> item;
        *p = (item == '1' ? true : false);
        ++p;
    }
}


template<>
void RWMathVec<bool>::restoreFrom(RWvistream& s)
{
    //typedef typename rw_numeric_traits<T>::c_type c_type;
    typedef char c_type;
    // Get and check the version number
    unsigned short ID;
    s >> ID;

    if (ID != versionID && ID != 340) {
        RWTHROW(RWExternalErr(RWMessage(RWMATH_BADVERNO, (int)versionID,
                                        (int)ID)));
    }

    // Get the length and resize to it:
    size_t len;
    s >> len;
    resize(len);

    c_type item;
    iterator p = begin(), stop = end();
    while (p != stop) {
        s >> item;
        *p = (item == '1' ? true : false);
        ++p;
    }
}

#endif //RW_MATH_BOOL_TOOLS_IO_WORKAROUND

#endif
