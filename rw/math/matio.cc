#ifndef __GENMATIO_CC__
#define __GENMATIO_CC__

/***************************************************************************
 *
 * matio.cc
 *
 * $Id: //math/13/rw/math/matio.cc#1 $
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
#include <rw/math/genmat.h>
#include <rw/rwfile.h>
#include <rw/vstream.h>
#include <rw/rwerr.h>
#include <rw/matherr.h>
#include <rw/math/numtrait.h>
#include <rw/dcompio.h>
#ifdef RW_MATH_NO_NEW_HEADER
#  include <stdio.h>
#  include <ctype.h>
#else
#  include <cstdio>
#  include <cctype>
#endif
#include <iomanip>
#include <ios>


template <class T>
void RWGenMat<T>::printOn(std::ostream& s) const
{
    int w = static_cast<int>(s.width(0));

    s << rows() << "x" << cols() << " [\n";
    for (int i = 0; (size_t)i < rows(); i++) {
        for (int j = 0; (size_t)j < cols(); j++) {
            typename rw_numeric_traits<T>::numeric_type el = (*this)(i, j);
            s << std::setw(w) << el;
            if ((size_t)j < (cols() - 1)) {
                s << " ";
            }
        }
        s << "\n";
    }
    s << "]";
}

template <class T>
void RWGenMat<T>::scanFrom(std::istream& s, Storage storage)
{
    typename rw_numeric_traits<T>::numeric_type item;
    char c;

    int m, n;             // Read in array dimensions
    s >> std::dec >> m;
    do {
        s.get(c);
    }
    while ((RWMTH_STD::isspace)(c));   // Skip optional x character
    if ((RWMTH_STD::isdigit)(c)) {
        s.putback(c);
    }
    s >> std::dec >> n;

    do {
        s >> c;
    }
    while ((RWMTH_STD::isspace)(c));   // Eat the leading [, if it exists
    if (c != '[') {
        s.putback(c);
    }

    reshape(static_cast<unsigned>(m), static_cast<unsigned>(n), storage);
    for (int i = 0; s.good() && (size_t)i < rows(); i++) {
        for (int j = 0; s.good() && (size_t)j < cols(); j++) {
            s >> item;
            (*this)(i, j) = static_cast<T>(item);
        }
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
std::ostream& operator<<(std::ostream& s, const RWGenMat<T>& m)
{
    m.printOn(s);
    return s;
}

template <class T>
std::istream& operator>>(std::istream& s, RWGenMat<T>& m)
{
    m.scanFrom(s);
    return s;
}

template <class T>
void RWGenMat<T>::saveOn(RWFile& file) const
{
    file.Write(versionID);
    file.Write(nrows);
    file.Write(ncols);

    c_type item;
    const_iterator p = begin(), stop = end();
    while (p != stop) {
        item = (c_type)(*p);
        file << item;
        ++p;
    }
}

template <class T>
void RWGenMat<T>::saveOn(RWvostream& s) const
{
    s << versionID;       // Save the version number
    s.putSizeT(nrows);    // Save the vector length
    s.putSizeT(ncols);

    c_type item;
    const_iterator p = begin(), stop = end();
    while (p != stop) {
        item = (c_type)(*p);
        s << item;
        ++p;
    }
}

template <class T>
void RWGenMat<T>::restoreFrom(RWFile& file, Storage s)
{
    // Get and check the version number
    unsigned short ID;
    file.Read(ID);

    if (ID != versionID && ID != 340) {
        versionErr((int)versionID, (int)ID);
    }

    // Get the length and resize to it:
    size_t m, n;
    file.Read(m);
    file.Read(n);
    reshape(m, n, s);

    c_type item;
    iterator p = begin(), stop = end();
    while (p != stop) {
        file >> item;
        *p = T(item);
        ++p;
    }
}

template <class T>
void RWGenMat<T>::restoreFrom(RWvistream& s, Storage storage)
{
    // Get and check the version number
    unsigned short ID;
    s >> ID;

    if (ID != versionID && ID != 340) {
        versionErr((int)versionID, (int)ID);
    }

    // Get the length and resize to it:
    size_t m, n;
    s.getSizeT(m);
    s.getSizeT(n);
    reshape(m, n, storage);

    c_type item;
    iterator p = begin(), stop = end();
    while (p != stop) {
        s >> item;
        *p = T(item);
        ++p;
    }
}

template <class T>
size_t RWGenMat<T>::binaryStoreSize() const
{
    // Total storage requirements = Number of elements times their size,
    // plus space for the vector length and ID number:
    return rows() * cols() * sizeof(T) + 2 * sizeof(size_t) + sizeof(versionID);
}

#if defined(RW_MATH_BOOL_TOOLS_IO_WORKAROUND)
// defined in rw/mathdefs.h

template <>
void RWGenMat<bool>::restoreFrom(RWvistream& s, Storage storage)
{
    // Get and check the version number
    unsigned short ID;
    s >> ID;

    if (ID != versionID && ID != 340) {
        versionErr((int)versionID, (int)ID);
    }

    // Get the length and resize to it:
    size_t m, n;
    s >> m >> n;
    reshape(m, n, storage);

    //c_type item;
    char item;
    iterator p = begin(), stop = end();
    while (p != stop) {
        s >> item;
        *p = (item == '1' ? true : false);
        ++p;
    }
}

#endif

#endif // __GENMATIO_CC__
