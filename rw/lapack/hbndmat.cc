
/*
 * The main code file for RWHermBandMat
 *
*
 * $Id: //lapack/13/rw/lapack/hbndmat.cc#1 $
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

//#include <rw/hbndmat.h>

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

#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable:4018)
#endif

template <class TypeT>
#if !defined(_MSC_VER)
const char* RWHermBandMat<TypeT>::className = template_name<TypeT>("RWHermBandMat");
#else
const char* RWHermBandMat<TypeT>::className = template_name<TypeT>("RWHermBandMat", (TypeT)0);
#endif

template< class TypeT >
TypeT RWHermBandMat<TypeT>::zero_ = TypeT(0);

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
RWHermBandMat<TypeT>::~RWHermBandMat()
{
}

template <class TypeT>
RWHermBandMat<TypeT>::RWHermBandMat()
    : vec()
{
    n = 0;
    bandu = 0;
}

template <class TypeT>
RWHermBandMat<TypeT>::RWHermBandMat(const RWHermBandMat<TypeT>& A)
    : vec(A.vec)
{
    n = A.n;
    bandu = A.bandu;
}

template <class TypeT>
RWHermBandMat<TypeT>::RWHermBandMat(unsigned M, unsigned N, unsigned halfWidth)
    : vec(HermBandMatSize(N, halfWidth), rwUninitialized)
{
    if (M != N) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOTSQUARE, int(M), int(N))));
    }
    n = N;
    bandu = halfWidth;
    zeroUnusedEntries();
}

template <class TypeT>
RWHermBandMat<TypeT>::RWHermBandMat(const RWMathVec<TypeT>& data, unsigned M, unsigned N, unsigned halfWidth)
    : vec(data)
{
    if (M != N) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOTSQUARE, int(M), int(N))));
    }
    n = N;
    bandu = halfWidth;
    if (vec.length() != HermBandMatSize(N, bandu)) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NUMBERPOINTSBAND, vec.length(), n, n, className, bandwidth())));
    }
}

/*
 * Set entries in the data vector which are not used to zero.  This
 * avoids purify warnings and also ensures we won't have troubles
 * adding and subtracting these matrices
 */
template <class TypeT>
void RWHermBandMat<TypeT>::zeroUnusedEntries()
{
    int i = halfBandwidth() - 1;   // Start i at min(n-1,halfBandwidth()-1)
    if (i > int(n - 1)) {
        i = n - 1;
    }
    for (; i >= 0; i--) {
        vec.slice(i * (halfBandwidth() + 1), halfBandwidth() - i) = TypeT(0, 0);
    }
}

template <class TypeT>
TypeT RWHermBandMat<TypeT>::bcval(int i, int j) const
{
    if (i < 0 || size_t(i) >= rows() || j < 0 || size_t(j) >= cols()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_OUTOFBOUNDS, i, j, rows(), cols())));
    }
    return ((i <= j) ? ((unsigned(j - i) > bandu) ? TypeT(0, 0) : vec(i + bandu + j * bandu))
                : ((unsigned(i - j) > bandu) ? TypeT(0, 0) : RW_MATH_SL_STD_GLOBAL(conj)(vec(j + bandu + i * bandu))));
}

template <class TypeT>
TypeT RWHermBandMat<TypeT>::bcset(int i, int j, TypeT x)
{
    if (i < 0 || size_t(i) >= rows() || j < 0 || size_t(j) >= cols()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_OUTOFBOUNDS, i, j, rows(), cols())));
    }
    if ((j >= i) ? (unsigned(j - i) > bandu) : (unsigned(i - j) > bandu)) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOTSETABLE, i, j, className)));
    }
    if (i <= j) {
        vec(i + bandu + j * bandu) = x;
        return x;
    }
    else {
        vec(j + bandu + i * bandu) = RW_MATH_SL_STD_GLOBAL(conj)(x);
        return RW_MATH_SL_STD_GLOBAL(conj)(x);
    }
}

template <class TypeT>
RWROCJRef<TypeT> RWHermBandMat<TypeT>::bcref(int i, int j)
{
    if (i < 0 || (size_t)i >= rows() || j < 0 || (size_t)j >= cols()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_OUTOFBOUNDS, i, j, rows(), cols())));
    }
    return (i <= j) ? ((unsigned(j - i) > bandu) ? RWROCJRef<TypeT>(zero_, true)
                       : RWROCJRef<TypeT>(vec(i + bandu + j * bandu), false))
               : ((unsigned(i - j) > bandu) ? RWROCJRef<TypeT>(zero_, true)
                  : RWROCJRef<TypeT>(vec(j + bandu + i * bandu), false, true));
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
RWHermBandMat<TypeT> RWHermBandMat<TypeT>::leadingSubmatrix(int k)
{
    if (k < 0 || (size_t)k > n) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_LEADINGSUBMATRIXORDER, k, n)));
    }
    return RWHermBandMat<TypeT>(vec.slice(0, HermBandMatSize((unsigned)k, halfBandwidth())), (unsigned)k, (unsigned)k, halfBandwidth());
}

template <class TypeT>
RWHermBandMat<TypeT>& RWHermBandMat<TypeT>::operator=(const RWHermBandMat<TypeT>& M)
{
    if (rows() != M.rows() || cols() != M.cols()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_MATSIZE, M.rows(), M.cols(), rows(), cols())));
    }
    if (lowerBandwidth() < M.lowerBandwidth() ||
            upperBandwidth() < M.upperBandwidth()) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_BANDSIZE, M.lowerBandwidth(), lowerBandwidth(),
                                        upperBandwidth(), M.upperBandwidth())));
    }
    if (bandwidth() == M.bandwidth()) {
        vec = M.vec;
    }
    else {
        int u = upperBandwidth();
        int Mu = M.upperBandwidth();
        for (int i = 0; i <= u; i++) {
            RWMathVec<TypeT> d = vec.slice((i + 1) * bandu, n - i, bandu + 1);
            if (i > Mu) {
                d = TypeT(0, 0);
            }
            else {
                d = M.vec.slice((i + 1) * M.bandu, n - i, M.bandu + 1);
            }
        }
    }
    return *this;
}

template <class TypeT>
RWHermBandMat<TypeT>& RWHermBandMat<TypeT>::reference(RWHermBandMat<TypeT>& A)
{
    vec.reference(A.vec);
    n = A.n;
    bandu = A.bandu;
    return *this;
}

template <class TypeT>
RWHermBandMat<TypeT> RWHermBandMat<TypeT>::copy() const
{
    RWHermBandMat<TypeT> A(*((RWHermBandMat<TypeT>*)this));   // cast this to non-const to make cfront happy
    A.deepenShallowCopy();
    return A;
}

template <class TypeT>
void RWHermBandMat<TypeT>::resize(unsigned M, unsigned N)
{
    if (M != N) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOTSQUARE, int(M), int(N))));
    }
    n = N;
    vec.resize(HermBandMatSize(n, upperBandwidth()));

    // Finally, zero any unused entries just to keep things tidy
    zeroUnusedEntries();
}

template <class TypeT>
void RWHermBandMat<TypeT>::resize(unsigned M, unsigned N, unsigned halfWidth)
{
    resize(M, N); // Set the dimension - all that's left is to change bandwidth
    int h = halfBandwidth();
    int hw = halfWidth;
    if (h != hw) {
        RWHermBandMat<TypeT> newMat(M, N, halfWidth);
        for (int i = 0; (unsigned)i <= halfWidth; i++) {
            RWMathVec<TypeT> d = newMat.vec.slice((i + 1) * newMat.bandu, newMat.n - i, newMat.bandu + 1);
            if (i < (-h) || i > h) {
                d = TypeT(0, 0);
            }
            else {
                d = vec.slice((i + 1) * bandu, n - i, bandu + 1);
            }
        }
        this->reference(newMat);
    }
}

template <class TypeT>
void RWHermBandMat<TypeT>::makeDiagonalReal()
{
    for (int i = rows(); i--;) {
        TypeT entry = val(i, i);
        set(i, i, TypeT(RW_MATH_SL_STD_GLOBAL(real)(entry), 0));
    }
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
void RWHermBandMat<TypeT>::printOn(std::ostream& outs) const
{
    int w = (int)outs.width(0);
    size_t m = n;
    outs << className << ", ";
    outs << "half bandwidth: " << halfBandwidth() << " ";
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
void RWHermBandMat<TypeT>::scanFrom(std::istream& s)
{
    char c;
    unsigned numRows, numCols;
    unsigned hwidth;

    /* Skip through leading non-digits */
    do {
        s.get(c);
    }
    while (!s.fail() && !(RWMTH_STD::isdigit)(c));
    s.putback(c);

    s >> hwidth;

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
            resize(numRows, numCols, hwidth);
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
void RWHermBandMat<TypeT>::saveOn(RWFile& file) const
{
    /* First write the numbers which define the shape of the matrix */
    file.Write(n);
    file.Write(bandu);
    vec.saveOn(file);
}

template <class TypeT>
void RWHermBandMat<TypeT>::saveOn(RWvostream& s) const
{
    /* First write the numbers which define the shape of the matrix */
    s << n;
    s << bandu;
    vec.saveOn(s);
}

template <class TypeT>
void RWHermBandMat<TypeT>::restoreFrom(RWFile& file)
{
    /* First read the numbers which define the shape of the matrix */
    file.Read(n);
    file.Read(bandu);
    int size = HermBandMatSize(n, bandu);
    vec.restoreFrom(file);
    if (vec.length() != (size_t)size) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_RESTORESIZE, className, vec.length(), size)));
    }
}

template <class TypeT>
void RWHermBandMat<TypeT>::restoreFrom(RWvistream& s)
{
    /* First read the numbers which define the shape of the matrix */
    s >> n;
    s >> bandu;
    int size = HermBandMatSize(n, bandu);
    vec.restoreFrom(s);
    if (vec.length() != (size_t)size) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_RESTORESIZE, className, vec.length(), size)));
    }
}

template <class TypeT>
unsigned RWHermBandMat<TypeT>::binaryStoreSize() const
{
    /* First determine the size of the stuff
       which determines the matrix shape.    */
    unsigned size = 3 * sizeof(unsigned); // n and bandu
    return size + vec.binaryStoreSize();
}

#ifdef _WIN32
#  pragma warning(pop)
#endif

#include <rw/lapack/hbndmth.cc>
#include <rw/lapack/hbndct.cc>
#include <rw/lapack/hbndcs.cc>
#include <rw/lapack/hbndcg.cc>
