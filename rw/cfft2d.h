#ifndef __RWCFFT2D_H__
#define __RWCFFT2D_H__

/***************************************************************************
 *
 * cfft2d.h - Double Precision 2-Dimensional Complex FFT server
 *
 * $Id: //math/13/rw/cfft2d.h#1 $
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
 ****************************************************************************/

#include <rw/dcomplex.h>
#include <rw/cgenmat.h>      /* Include for backward compatibility */
#include <rw/math/genmat.h>
#include <rw/cfft.h>

/**
 * @ingroup fft_server_classes
 *
 * @brief Performs two-dimensional (2-D) double precision complex fast
 * Fourier transforms (FFTs) of an RWGenMat<DComplex> matrix.
 *
 * Class DComplexFFT2DServer performs two-dimensional (2-D) double
 * precision complex fast Fourier transforms (FFTs) of an \link
 * RWGenMat RWGenMat<DComplex> \endlink matrix. The transform
 * of a complex matrix is a complex matrix; therefore, the server
 * returns a complex matrix for either a forward or backward transform.
 *
 * At any given moment, the server is configured to transform a
 * matrix with specified dimensions. These dimensions can be set
 * one of three ways:
 *
 * -  At construction time
 * -  With the functions setRows() or setCols()
 * -  By the server from a supplied matrix.
 *
 * The last method means that if the matrix size changes, the server
 * automatically reconfigures to the new size. Since this is an
 * expensive calculation, it is most efficient to call the server
 * repeatedly for matrices of the same size. Different FFT servers
 * may be constructed to handle different sizes.
 *
 * The forward transform calculated is:
 *
 *
 * @f$
 * A(m, n) = \displaystyle\sum_{j=0}^{M-1} \displaystyle\sum_{k=0}^{N-1} X(j, k)
 * exp\left(-2\pi i\left[\frac{jm}{M}+\frac{kn}{n}\right]\right);
 * @f$
 *
 * @f$
 * m=0, \quad .., \quad M-1; \quad k=0, \quad .., \quad N-1
 * @f$
 *
 *
 * The inverse Fourier transform (IDFT) calculated is:
 *
 *
 * @f$
 * X(j, k) = \displaystyle\sum_{m=0}^{M-1} \displaystyle\sum_{n=0}^{N-1} A(m, n)
 * exp\left(2\pi i\left[\frac{jm}{M}+\frac{kn}{n}\right]\right);
 * @f$
 *
 * @f$
 * j=0, \quad .., \quad M-1; \quad k=0, \quad .., \quad N-1
 * @f$
 *
 *
 * As with the 1-D FFT, a call to fourier() followed by a call to
 * ifourier() results in the original matrix multiplied by the total
 * number of elements, that is, the number of rows times the number
 * of columns.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/cfft2d.h>
 * DComplexFFT2DServer dcfft2s;
 * @endcode
 */
class RW_MATH_SYMBOLIC DComplexFFT2DServer
{
    DComplexFFTServer     FFTrow;
    DComplexFFTServer     FFTcol;
    size_t                Nrows;
    size_t                Ncols;
public:
    // constructors

    /**
     * Constructs a double precision FFT server for a matrix of unspecified
     * dimensions. The dimensions are initialized when the server is
     * used.
     */
    DComplexFFT2DServer();

    /**
     * Constructs a complex FFT server designed to transform a matrix
     * with \a nrows rows and \a ncols columns.
     */
    DComplexFFT2DServer(size_t nrows, size_t ncols);

    /**
     * Constructs a complex FFT server designed to transform a square
     * matrix with \a n rows and columns.
     */
    DComplexFFT2DServer(size_t n);            // Assumes square matrix

    /**
     * Copy constructor. The new server references the old server's
     * data.
     */
    DComplexFFT2DServer(const DComplexFFT2DServer&);

    /**
     * Sets self to the same order as \a v.
     */
    DComplexFFT2DServer& operator=(const DComplexFFT2DServer& v);

    /**
     * Returns the number of rows the 2-D FFT server is currently configured
     * to transform.
     */
    size_t                rows() const {
        return Nrows;
    }

    /**
     * Returns the number of columns that the 2-D FFT server is currently
     * configured to transform.
     */
    size_t                cols() const {
        return Ncols;
    }

    /**
     * Resets the number of rows of a 2-D FFT server to \a N.
     */
    void                    setRows(size_t N);

    /**
     * Resets the number of columns of a 2-D FFT server to \a N.
     */
    void                    setCols(size_t N);  // set new Ncols

    /**
     * Returns the 2-D FFT of a complex matrix.
     */
    RWGenMat<DComplex>            fourier(const RWGenMat<DComplex>&);
    void                          fourier(const RWGenMat<DComplex>&, RWGenMat<DComplex>&);

    /**
     * Returns the inverse 2-D FFT of a complex matrix.
     */
    RWGenMat<DComplex>            ifourier(const RWGenMat<DComplex>&);
    void                          ifourier(const RWGenMat<DComplex>&, RWGenMat<DComplex>&);
};

#endif /* __RWCFFT2D_H__ */
