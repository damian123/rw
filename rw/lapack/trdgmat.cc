
/*
 * The main code file for RWTriDiagMat<TypeT>
 *
 * $Id: //lapack/13/rw/lapack/trdgmat.cc#1 $
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
const char* RWTriDiagMat<TypeT>::className = template_name<TypeT>("RWTriDiagMat");
#else
const char* RWTriDiagMat<TypeT>::className = template_name<TypeT>("RWTriDiagMat", (TypeT)0);
#endif

template< class TypeT >
TypeT RWTriDiagMat<TypeT>::zero_ = TypeT(0);


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
RWTriDiagMat<TypeT>::~RWTriDiagMat()
{
}

template <class TypeT>
RWTriDiagMat<TypeT>::RWTriDiagMat()
    : vec()
{
    n = 0;
}

template <class TypeT>
RWTriDiagMat<TypeT>::RWTriDiagMat(const RWTriDiagMat<TypeT>& A)
    : vec(A.vec)
{
    n = A.n;
}

template <class TypeT>
RWTriDiagMat<TypeT>::RWTriDiagMat(unsigned M, unsigned N)
    : vec(TriDiagMatSize(N), rwUninitialized)
{
    if (M != N) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOTSQUARE, int(M), int(N))));
    }
    n = N;
    zeroUnusedEntries();
}


template <class TypeT>
RWTriDiagMat<TypeT>::RWTriDiagMat(const RWMathVec<TypeT>& data, unsigned M, unsigned N)
    : vec(data)
{
    if (M != N) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOTSQUARE, int(M), int(N))));
    }
    n = N;
    if (vec.length() != TriDiagMatSize(N)) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NUMBERPOINTS, vec.length(), n, n, className)));
    }
}

/*
 * Set entries in the data vector which are not used to zero.  This
 * avoids purify warnings and also ensures we won't have troubles
 * adding and subtracting these matrices
 */
template <class TypeT>
void RWTriDiagMat<TypeT>::zeroUnusedEntries()
{
    if (n > 0) {
        vec(3 * n - 1) = TypeT(0);
        vec(0) = TypeT(0);
    }
}

template <class TypeT>
TypeT RWTriDiagMat<TypeT>::bcval(int i, int j) const
{
    if (i < 0 || size_t(i) >= rows() || j < 0 || size_t(j) >= cols()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_OUTOFBOUNDS, i, j, rows(), cols())));
    }

    return ((i - j > 1) || (i - j < (-1))) ? zero_ : vec(i + 1 + j * 2);
}

template <class TypeT>
TypeT RWTriDiagMat<TypeT>::bcset(int i, int j, TypeT x)
{
    if (i < 0 || size_t(i) >= rows() || j < 0 || size_t(j) >= cols()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_OUTOFBOUNDS, i, j, rows(), cols())));
    }
    if (i - j < (-1) || i - j > 1) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOTSETABLE, i, j, className)));
    }
    return vec(i + 1 + j * 2) = x;
}

template <class TypeT>
RWRORef<TypeT> RWTriDiagMat<TypeT>::bcref(int i, int j)
{
    if (i < 0 || (size_t)i >= rows() || j < 0 || (size_t)j >= cols()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_OUTOFBOUNDS, i, j, rows(), cols())));
    }
    return ((i - j > 1) || (i - j < (-1))) ? RWRORef<TypeT>(zero_, true)
           : RWRORef<TypeT>(vec(i + 1 + j * 2), false);
}

template <class TypeT>
RWMathVec<TypeT> RWTriDiagMat<TypeT>::bcdiagonal(int i) const
{
    if (i >= int(cols()) || (-i) >= int(rows()) ||
            i > int(upperBandwidth()) || (-i) > int(lowerBandwidth())) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_DIAGOUTOFBOUNDS, -i, className, rows(), cols())));
    }
    int iabs = (i >= 0) ? i : -i;
    return vec.slice(i > 0 ? upperBandwidth() + i * (bandwidth() - 1)
                     : upperBandwidth() + iabs
                     , n - iabs, bandwidth());
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
RWTriDiagMat<TypeT> RWTriDiagMat<TypeT>::leadingSubmatrix(int k)
{
    if (k < 0 || (size_t)k > n) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_LEADINGSUBMATRIXORDER, k, n)));
    }
    return RWTriDiagMat<TypeT>(vec.slice(0, TriDiagMatSize((unsigned)k)), (unsigned)k, (unsigned)k);
}

template <class TypeT>
RWTriDiagMat<TypeT>& RWTriDiagMat<TypeT>::operator=(const RWTriDiagMat<TypeT>& M)
{
    if (rows() != M.rows() || cols() != M.cols()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_MATSIZE, M.rows(), M.cols(), rows(), cols())));
    }
    vec = M.vec;
    return *this;
}

template <class TypeT>
RWTriDiagMat<TypeT>& RWTriDiagMat<TypeT>::reference(RWTriDiagMat<TypeT>& A)
{
    vec.reference(A.vec);
    n = A.n;
    return *this;
}

template <class TypeT>
RWTriDiagMat<TypeT> RWTriDiagMat<TypeT>::copy() const
{
    RWTriDiagMat<TypeT> A(*((RWTriDiagMat<TypeT>*)this));     // cast this to non-const to make cfront happy
    A.deepenShallowCopy();
    return A;
}

template <class TypeT>
void RWTriDiagMat<TypeT>::resize(unsigned M, unsigned N)
{
    if (M != N) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOTSQUARE, int(M), int(N))));
    }
    // Zero out elements past the bottom of the matrix if need be so
    // they don't enter in to the new matrix
    zeroUnusedEntries();
    n = N;
    vec.resize(TriDiagMatSize(n));

    // Finally, zero any unused entries just to keep things tidy
    zeroUnusedEntries();
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
void RWTriDiagMat<TypeT>::printOn(std::ostream& outs) const
{
    int w = (int)(outs.width(0));
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
void RWTriDiagMat<TypeT>::scanFrom(std::istream& s)
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
void RWTriDiagMat<TypeT>::saveOn(RWFile& file) const
{
    /* First write the numbers which define the shape of the matrix */
    file.Write(n);
    vec.saveOn(file);
}

template <class TypeT>
void RWTriDiagMat<TypeT>::saveOn(RWvostream& s) const
{
    /* First write the numbers which define the shape of the matrix */
    s << n;
    vec.saveOn(s);
}

template <class TypeT>
void RWTriDiagMat<TypeT>::restoreFrom(RWFile& file)
{
    /* First read the numbers which define the shape of the matrix */
    file.Read(n);
    int size = TriDiagMatSize(n);
    vec.restoreFrom(file);
    if (vec.length() != (size_t)size) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_RESTORESIZE, className, vec.length(), size)));
    }
}

template <class TypeT>
void RWTriDiagMat<TypeT>::restoreFrom(RWvistream& s)
{
    /* First read the numbers which define the shape of the matrix */
    s >> n;
    int size = TriDiagMatSize(n);
    vec.restoreFrom(s);
    if (vec.length() != (size_t)size) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_RESTORESIZE, className, vec.length(), size)));
    }
}

template <class TypeT>
unsigned RWTriDiagMat<TypeT>::binaryStoreSize() const
{
    /* First determine the size of the stuff
       which determines the matrix shape.    */
    unsigned size = sizeof(unsigned);    // n
    return size + vec.binaryStoreSize();
}


#include <rw/lapack/trdgcg.cc>
#include <rw/lapack/trdgcs.cc>
#include <rw/lapack/trdgmth.cc>
