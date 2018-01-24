
/*
 * The main code file for RWSkewMat<TypeT>
 *
 * $Id: //lapack/13/rw/lapack/skewmat.cc#1 $
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
 */

#include <rw/rstream.h>
#include <rw/rwfile.h>
#include <rw/vstream.h>
#include <rw/lapkerr.h>
#include <rw/rwerr.h>

#include <iomanip>

#ifdef RW_MATH_NO_NEW_HEADER
#  include <stdio.h>
#  include <ctype.h>
#else
#  include <cstdio>
#  include <cctype>
#endif

template <class TypeT>
#if !defined(_MSC_VER)
const char* RWSkewMat<TypeT>::className = template_name<TypeT>("RWSkewMat");
#else
const char* RWSkewMat<TypeT>::className = template_name<TypeT>("RWSkewMat", (TypeT)0);
#endif

/*
 * These inline functions
 * give the number of elements that need be stored to represent
 * the different types of matrices
 */

#ifndef SIZEFUNCTIONS
#  define SIZEFUNCTIONS
inline unsigned GenMatSize(unsigned n, unsigned m)
{
    return m * n;
}
inline unsigned BandMatSize(unsigned n, unsigned b)
{
    return n * b;
}
inline unsigned SymBandMatSize(unsigned n, unsigned hb)
{
    return n * (hb + 1);
}
inline unsigned HermBandMatSize(unsigned n, unsigned hb)
{
    return n * (hb + 1);
}
inline unsigned SymMatSize(unsigned n)
{
    return n * (n + 1) / 2;
}
inline unsigned SkewMatSize(unsigned n)
{
    return n * (n + 1) / 2;
}
inline unsigned HermMatSize(unsigned n)
{
    return n * (n + 1) / 2;
}
inline unsigned UpperTriMatSize(unsigned n)
{
    return n * (n + 1) / 2;
}
inline unsigned LowerTriMatSize(unsigned n)
{
    return n * (n + 1) / 2;
}
inline unsigned TriDiagMatSize(unsigned n)
{
    return 3 * n;
}
#endif

template <class TypeT>
RWSkewMat<TypeT>::~RWSkewMat()
{
}

template <class TypeT>
RWSkewMat<TypeT>::RWSkewMat()
    : vec()
{
    n = 0;
}


template <class TypeT>
RWSkewMat<TypeT>::RWSkewMat(const RWSkewMat<TypeT>& A)
    : vec(A.vec)
{
    n = A.n;
}

template <class TypeT>
RWSkewMat<TypeT>::RWSkewMat(unsigned M, unsigned N)
    : vec(SkewMatSize(N), rwUninitialized)
{
    if (M != N) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOTSQUARE, int(M), int(N))));
    }
    n = N;
}

template <class TypeT>
RWSkewMat<TypeT>::RWSkewMat(const RWMathVec<TypeT>& data, unsigned M, unsigned N)
    : vec(data)
{
    if (M != N) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOTSQUARE, int(M), int(N))));
    }
    n = N;
    if (vec.length() != SkewMatSize(N)) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NUMBERPOINTS, vec.length(), n, n, className)));
    }
}

template <class TypeT>
TypeT RWSkewMat<TypeT>::bcval(int i, int j) const
{
    if (i < 0 || size_t(i) >= rows() || j < 0 || size_t(j) >= cols()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_OUTOFBOUNDS, i, j, rows(), cols())));
    }
    //if (i<=j) return vec(j*(j+1)/2+i); else return -vec(i*(i+1)/2+j);
    int index = (i <= j) ? (j * (j + 1) / 2 + i) : (i * (i + 1) / 2 + j);
    TypeT val = vec(index);
    return ((i <= j) || (val == TypeT(0))) ?  val : -val;
}

template <class TypeT>
TypeT RWSkewMat<TypeT>::bcset(int i, int j, TypeT x)
{
    if (i < 0 || size_t(i) >= rows() || j < 0 || size_t(j) >= cols()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_OUTOFBOUNDS, i, j, rows(), cols())));
    }
    if (i <= j) {
        vec(j * (j + 1) / 2 + i) = x;
        return x;
    }
    else {
        vec(i * (i + 1) / 2 + j) = (-x);
        return -x;
    }
    // return (i<=j) ? (vec(j*(j+1)/2+i)=x) : (vec(i*(i+1)/2+j)=(-x));
    // The above kills the cc optimizer on the DEC 5400 (complex case)
}

template <class TypeT>
RWNGRef<TypeT> RWSkewMat<TypeT>::bcref(int i, int j)
{
    if (i < 0 || (size_t)i >= rows() || j < 0 || (size_t)j >= cols()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_OUTOFBOUNDS, i, j, rows(), cols())));
    }
    if (i <= j) {
        return RWNGRef<TypeT>(vec(j * (j + 1) / 2 + i), false);
    }
    else {
        return RWNGRef<TypeT>(vec(i * (i + 1) / 2 + j), true);
    }
}


/*
 * Here are non-inline versions of the row(), col(), and diagonal()
 * routines.  Normally these would be inlined, but due to a bug
 * in the Borland C++ compiler it is best not to inline them with
 * Borland C++.
 *
 * These routines just call the bounds checking routines.
 */

/*
 * The leadingSubmatrix function
 */

template <class TypeT>
RWSkewMat<TypeT> RWSkewMat<TypeT>::leadingSubmatrix(int k)
{
    if (k < 0 || (size_t)k > n) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_LEADINGSUBMATRIXORDER, k, n)));
    }
    return RWSkewMat<TypeT>(vec.slice(0, SkewMatSize((unsigned)k)), (unsigned)k, (unsigned)k);
}

template <class TypeT>
RWSkewMat<TypeT>& RWSkewMat<TypeT>::operator=(const RWSkewMat<TypeT>& M)
{
    if (rows() != M.rows() || cols() != M.cols()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_MATSIZE, M.rows(), M.cols(), rows(), cols())));
    }
    vec = M.vec;
    return *this;
}

template <class TypeT>
RWSkewMat<TypeT>& RWSkewMat<TypeT>::reference(RWSkewMat<TypeT>& A)
{
    vec.reference(A.vec);
    n = A.n;
    return *this;
}

template <class TypeT>
RWSkewMat<TypeT> RWSkewMat<TypeT>::copy() const
{
    RWSkewMat<TypeT> A(*((RWSkewMat<TypeT>*)this));   // cast this to non-const to make cfront happy
    A.deepenShallowCopy();
    return A;
}

template <class TypeT>
void RWSkewMat<TypeT>::resize(unsigned M, unsigned N)
{
    if (M != N) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOTSQUARE, int(M), int(N))));
    }
    n = N;
    vec.resize(SkewMatSize(n));

}



/*
 * printOn,scanFrom:  printOn prints all non-numeric stuff first
 * then prints the numbers defining the shape and size of the matrix,
 * then prints the matrix itself.  The format used by printOn can be
 * used to read in a matrix with scanFrom.
 *
 * scanFrom first eats all the non-numeric characters it encounters.
 * This takes care of the stuff printOn spews before the data.  Next
 * it reads the numbers which define the matrix shape and then the
 * data is read in.
 */

template <class TypeT>
void RWSkewMat<TypeT>::printOn(std::ostream& outs) const
{
    int w = static_cast<int>(outs.width(0));
    size_t m = n;
    outs << className << ", ";
    outs << rows() << "x" << cols() << " [\n";
    for (int i = 0; (size_t)i < m; i++) {
        for (int j = 0; (size_t)j < n; j++) {
            outs << std::setw(w) << val(i, j) << " ";
        }
        outs << "\n";
    }
    outs << "]";
}

template <class TypeT>
void RWSkewMat<TypeT>::scanFrom(std::istream& s)
{
    char c;
    unsigned numRows, numCols;

    /* Skip through leading non-digits */
    do {
        s.get(c);
    }
    while (!s.fail() && !(RWMTH_STD::isdigit)(c));
    s.putback(c);


    s >> numRows;
    do {
        s.get(c);
    }
    while ((RWMTH_STD::isspace)(c));   // Skip optional x character
    if ((RWMTH_STD::isdigit)(c)) {
        s.putback(c);
    }
    s >> numCols;

    RWMathVec<TypeT> v;           // Read the data
    v.scanFrom(s);

    if (!s.fail()) {      // Now put the data into the matrix
        if (v.length() != (size_t)numRows * numCols) {
            RWTHROW(RWInternalErr(RWMessage(RWLAPK_WRONGNUMPOINTS, v.length(), className, numRows, numCols)));
        }
        else {
            resize(numRows, numCols);
            int index = 0;
            for (int i = 0; (size_t)i < numRows; i++) {
                for (int j = 0; (size_t)j < numCols; j++) {
                    if (val(i, j) != v[index]) {
                        bcset(i, j, v[index]);
                    }
                    index++;
                }
            }
        }
    }
}

template <class TypeT>
void RWSkewMat<TypeT>::saveOn(RWFile& file) const
{
    /* First write the numbers which define the shape of the matrix */
    file.Write(n);
    vec.saveOn(file);
}

template <class TypeT>
void RWSkewMat<TypeT>::saveOn(RWvostream& s) const
{
    /* First write the numbers which define the shape of the matrix */
    s << n;
    vec.saveOn(s);
}

template <class TypeT>
void RWSkewMat<TypeT>::restoreFrom(RWFile& file)
{
    /* First read the numbers which define the shape of the matrix */
    file.Read(n);
    int size = SkewMatSize(n);
    vec.restoreFrom(file);
    if (vec.length() != (size_t)size) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_RESTORESIZE, className, vec.length(), size)));
    }
}

template <class TypeT>
void RWSkewMat<TypeT>::restoreFrom(RWvistream& s)
{
    /* First read the numbers which define the shape of the matrix */
    s >> n;
    int size = SkewMatSize(n);
    vec.restoreFrom(s);
    if (vec.length() != (size_t)size) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_RESTORESIZE, className, vec.length(), size)));
    }
}

template <class TypeT>
unsigned RWSkewMat<TypeT>::binaryStoreSize() const
{
    /* First determine the size of the stuff
       which determines the matrix shape.    */
    unsigned size = sizeof(unsigned);    // n
    return size + vec.binaryStoreSize();
}


#include <rw/lapack/skewcg.cc>
#include <rw/lapack/skewmth.cc>
