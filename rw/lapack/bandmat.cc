
/*
 * The main code file for RWBandMat<DComplex>
 *
 *
 * $Id: //lapack/13/rw/lapack/bandmat.cc#1 $
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
#ifdef _WIN32
#  pragma warning(push)
#  pragma warning(disable:4018)
#endif

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
const char* RWBandMat<TypeT>::className = template_name<TypeT>("RWBandMat");
#else
const char* RWBandMat<TypeT>::className = template_name<TypeT>("RWBandMat", (TypeT)0);
#endif

template< class TypeT >
TypeT RWBandMat<TypeT>::zero_ = TypeT(0);

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


template < class TypeT >
RWBandMat<TypeT>::~RWBandMat()
{
}

template < class TypeT >
RWBandMat<TypeT>::RWBandMat()
    : vec()
{
    n = 0;
    band = 1;
    bandu = 0;
}

template < class TypeT >
RWBandMat<TypeT>::RWBandMat(const RWBandMat<TypeT>& A)
    : vec(A.vec)
{
    n = A.n;
    band = A.band;
    bandu = A.bandu;
}

template < class TypeT >
RWBandMat<TypeT>::RWBandMat(unsigned M, unsigned N, unsigned lowerWidth, unsigned upperWidth)
    : vec(BandMatSize(N, lowerWidth + upperWidth + 1), rwUninitialized)
{
    if (M != N) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOTSQUARE, int(M), int(N))));
    }
    n = N;
    band = lowerWidth + upperWidth + 1;
    bandu = upperWidth;
    zeroUnusedEntries();
}

template < class TypeT >
RWBandMat<TypeT>::RWBandMat(const RWMathVec<TypeT>& data, unsigned M, unsigned N, unsigned lowerWidth, unsigned upperWidth)
    : vec(data)
{
    if (M != N) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOTSQUARE, int(M), int(N))));
    }
    n = N;
    band = lowerWidth + upperWidth + 1;
    bandu = upperWidth;
    if (vec.length() != BandMatSize(N, band)) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NUMBERPOINTSBAND, vec.length(), n, n, className, band)));
    }
}

/*
 * Set entries in the data vector which are not used to zero.  This
 * avoids purify warnings and also ensures we won't have troubles
 * adding and subtracting these matrices
 */
template < class TypeT >
void RWBandMat<TypeT>::zeroUnusedEntries()
{
    int i = lowerBandwidth();  // Start i at min(n-1,upperBandwidth()-1)
    if (i > int(n - 1)) {
        i = n - 1;
    }
    for (; i >= 0; i--) {
        vec.slice(i * bandwidth(), upperBandwidth() - i) = TypeT(0);
    }
    i = lowerBandwidth() - 1;  // Start i at min(n-1,lowerBandwidth()-1)
    if (i > int(n - 1)) {
        i = n - 1;
    }
    for (; i >= 0; i--) {
        vec.slice((n - i)*bandwidth() - lowerBandwidth() + i, lowerBandwidth() - i) = TypeT(0);
    }
}

template < class TypeT >
TypeT RWBandMat<TypeT>::bcval(int i, int j) const
{
    if (i < 0 || size_t(i) >= rows() || j < 0 || size_t(j) >= cols()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_OUTOFBOUNDS, i, j, rows(), cols())));
    }
    return ((i <= j) ? (unsigned(j - i) > bandu) : (unsigned(i - j) >= (band - bandu)))
           ? TypeT(0) : vec(i - j + bandu + j * band);
}


template < class TypeT >
TypeT RWBandMat<TypeT>::bcset(int i, int j, TypeT x)
{
    if (i < 0 || size_t(i) >= rows() || j < 0 || size_t(j) >= cols()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_OUTOFBOUNDS, i, j, rows(), cols())));
    }
    if ((j >= i) ? (unsigned(j - i) > bandu) : (unsigned(i - j) >= (band - bandu))) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_NOTSETABLE, i, j, className)));
    }
    return vec(i - j + bandu + j * band) = x;
}

template < class TypeT >
RWRORef<TypeT> RWBandMat<TypeT>::bcref(int i, int j)
{
    if (i < 0 || (size_t)i >= rows() || j < 0 || (size_t)j >= cols()) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_OUTOFBOUNDS, i, j, rows(), cols())));
    };

    return ((i <= j) ? (unsigned(j - i) > bandu) : (unsigned(i - j) >= (band - bandu)))
           ? RWRORef<TypeT>(zero_, true)
           : RWRORef<TypeT>(vec(i - j + bandu + j * band), false);

}

template < class TypeT >
RWMathVec<TypeT> RWBandMat<TypeT>::bcdiagonal(int i) const
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

template < class TypeT >
RWBandMat<TypeT> RWBandMat<TypeT>::leadingSubmatrix(int k)
{
    if (k < 0 || (size_t)k > n) {
        RWTHROW(RWBoundsErr(RWMessage(RWLAPK_LEADINGSUBMATRIXORDER, k, n)));
    }
    return RWBandMat<TypeT>(vec.slice(0, BandMatSize((unsigned)k, bandwidth())), (unsigned)k, (unsigned)k, lowerBandwidth(), upperBandwidth());
}

template < class TypeT >
RWBandMat<TypeT>& RWBandMat<TypeT>::operator=(const RWBandMat<TypeT>& M)
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
        int l = lowerBandwidth();
        int Ml = M.lowerBandwidth();
        for (int i = -l; i <= u; i++) {
            if (i < -Ml || i > Mu) {
                diagonal(i) = TypeT(0);
            }
            else {
                diagonal(i) = M.diagonal(i);
            }
        }
    }
    return *this;
}

template < class TypeT >
RWBandMat<TypeT>& RWBandMat<TypeT>::reference(RWBandMat<TypeT>& A)
{
    vec.reference(A.vec);
    n = A.n;
    band = A.band;
    bandu = A.bandu;
    return *this;
}

template < class TypeT >
RWBandMat<TypeT> RWBandMat<TypeT>::copy() const
{
    RWBandMat<TypeT> A(*((RWBandMat<TypeT>*)this));   // cast this to non-const to make cfront happy
    A.deepenShallowCopy();
    return A;
}

template < class TypeT >
void RWBandMat<TypeT>::resize(unsigned M, unsigned N)
{
    if (M != N) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_NOTSQUARE, int(M), int(N))));
    }
    // Zero out elements past the bottom of the matrix if need be so
    // they don't enter in to the new matrix
    zeroUnusedEntries();
    n = N;
    vec.resize(BandMatSize(n, bandwidth()));

    // Finally, zero any unused entries just to keep things tidy
    zeroUnusedEntries();
}

template < class TypeT >
void RWBandMat<TypeT>::resize(unsigned M, unsigned N, unsigned lowWidth, unsigned upWidth)
{
    resize(M, N); // Set the dimension - all that's left is to change bandwidth
    int l = lowerBandwidth();  // Convert all unsigneds to ints to avoid nasty
    int u = upperBandwidth();  // surprises when trying stuff like i= -lowWidth
    int lw = lowWidth;
    int uw = upWidth;
    if (l != lw || u != uw) {
        RWBandMat<TypeT> newMat(M, N, lowWidth, upWidth);
        for (int i = -lw; i <= uw; i++) {
            RWMathVec<TypeT> d = newMat.diagonal(i);
            if (i < (-l) || i > u) {
                d = TypeT(0);
            }
            else {
                d = this->diagonal(i);
            }
        }
        this->reference(newMat);
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

template < class TypeT >
void RWBandMat<TypeT>::printOn(std::ostream& outs) const
{
    int w = (int)outs.width(0);
    size_t m = n;
    outs << className << ", ";
    outs << "lower and upper bandwidths: " << lowerBandwidth() << " ";
    outs << upperBandwidth() << " ";
    outs << rows() << "x" << cols() << " [\n";
    for (int i = 0; (size_t)i < m; i++) {
        for (int j = 0; (size_t)j < n; j++) {
            outs << std::setw(w) << val(i, j) << " ";
        }
        outs << "\n";
    }
    outs << "]";
}

template < class TypeT >
void RWBandMat<TypeT>::scanFrom(std::istream& s)
{
    char c;
    unsigned numRows, numCols;
    unsigned lwidth, uwidth;

    /* Skip through leading non-digits */
    do {
        s.get(c);
    }
    while (!s.fail() && !(RWMTH_STD::isdigit)(c));
    s.putback(c);

    s >> lwidth >> uwidth;

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
            resize(numRows, numCols, lwidth, uwidth);
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

template < class TypeT >
void RWBandMat<TypeT>::saveOn(RWFile& file) const
{
    /* First write the numbers which define the shape of the matrix */
    file.Write(n);
    file.Write(band);
    file.Write(bandu);
    vec.saveOn(file);
}

template < class TypeT >
void RWBandMat<TypeT>::saveOn(RWvostream& s) const
{
    /* First write the numbers which define the shape of the matrix */
    s << n;
    s << band;
    s << bandu;
    vec.saveOn(s);
}

template < class TypeT >
void RWBandMat<TypeT>::restoreFrom(RWFile& file)
{
    /* First read the numbers which define the shape of the matrix */
    file.Read(n);
    file.Read(band);
    file.Read(bandu);
    int size = BandMatSize(n, band);
    vec.restoreFrom(file);
    if (vec.length() != (size_t)size) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_RESTORESIZE, className, vec.length(), size)));
    }
}

template < class TypeT >
void RWBandMat<TypeT>::restoreFrom(RWvistream& s)
{
    /* First read the numbers which define the shape of the matrix */
    s >> n;
    s >> band;
    s >> bandu;
    int size = BandMatSize(n, band);
    vec.restoreFrom(s);
    if (vec.length() != (size_t)size) {
        RWTHROW(RWInternalErr(RWMessage(RWLAPK_RESTORESIZE, className, vec.length(), size)));
    }
}

template < class TypeT >
unsigned RWBandMat<TypeT>::binaryStoreSize() const
{
    /* First determine the size of the stuff
       which determines the matrix shape.    */
    unsigned size = 3 * sizeof(unsigned); // n and band and bandu
    return size + vec.binaryStoreSize();
}

#ifdef _WIN32
#  pragma warning(pop)
#endif

#include <rw/lapack/bandcg.cc>
#include <rw/lapack/bandmth.cc>
