#ifndef __RWCFFT_H__
#define __RWCFFT_H__

/***************************************************************************
 *
 * cfft.h - Double Precision Complex FFT server
 *
 * $Id: //math/13/rw/cfft.h#1 $
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

/*
 * This class does complex fourier transforms of a series of a specified
 * length.  It can serve as the nucleus by which to do a wide variety of
 * transforms and, indeed, is inherited by several other servers.  The
 * NCAR FFT routines are used to perform the actual calculations.  This
 * can easily be replaced by specialized hardware or another algorithm.
 *
 * The DFT calculated is:
 *
 *                N-1
 *         A(n) = sum X(j) exp(-2 * pi * n * j * I / N);      n=0,...,N-1
 *                j=0
 *
 * The IDFT calculated is similar:
 *
 *                N-1
 *         X(j) = sum A(n) exp( 2 * pi * n * j * I / N);      j=0,...,N-1
 *                n=0
 *
 * where A and X are complex.  Note that the sum is NOT normalized: a
 * call to fourier(), followed by a call to ifourier() will result in
 * the original series multiplied by N.
 *
 * It precalculates the weights necessary to do the transform.  If it is
 * handed a series of a different length, it will reconfigure, an
 * expensive calculation.  Hence, it is most efficient to call it
 * repeatedly for series of the same length.  Construct several servers
 * to handle a variety of lengths.
 *
 */

#include <rw/dcomplex.h>     /* Include here to reduce #include nesting depth */
#include <rw/cvec.h>         /* Include for backward compatibility */
#include <rw/math/mathvec.h>

/**
 * @ingroup fft_server_classes
 *
 * @brief Performs double precision complex fast Fourier transforms (FFTs)
 * of an \link RWMathVec RWMathVec<DComplex> \endlink vector.
 *
 * Class DComplexFFTServer performs double precision complex fast
 * Fourier transforms (FFTs) of an \link RWMathVec RWMathVec<DComplex>
 * \endlink vector. The transform of a complex vector is a complex
 * vector; therefore, the server returns a complex vector for either
 * a forward or backward (inverse) transform.
 *
 * At any given moment, the server is configured to transform a
 * vector of a specified length. This length can be set three ways:
 *
 * -  At construction time.
 * -  With the function setOrder().
 * -  By the server from a supplied vector length.
 *
 * The last method means that if the vector length changes, the
 * server automatically reconfigures to the new length. Since this
 * is an expensive calculation, it is most efficient to call a server
 * repeatedly for series of the same length. Different FFT servers
 * may be constructed to handle different lengths.
 *
 * There are no restrictions on the number of points; that is, the
 * number need not be a power of 2.
 *
 * The Fourier transform calculated is:
 *
 *
 * @f$
 * A(n) = \displaystyle\sum_{j=0}^{N-1}X(j)exp\left(\frac{-2i \pi nj}{N}\right);
 * \quad n=0, \quad .., \quad N-1
 * @f$
 *
 *
 * The inverse Fourier transform (IDFT) calculated is:
 *
 *
 * @f$
 * X(j) = \displaystyle\sum_{n=0}^{N-1}A(n)exp\left(\frac{2i \pi nj}{N}\right);
 * \quad j=0, \quad .., \quad N-1
 * @f$
 *
 *
 * where \e A and \e X are complex. Note that the sum
 * is not normalized; a call to fourier() followed by a call to
 * ifourier() results in the original series multiplied by \e N.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/cfft.h>
 * DComplexFFTServer dcffts;
 * @endcode
 */
class RW_MATH_SYMBOLIC DComplexFFTServer
{
    size_t                server_N;
    RWMathVec<DComplex>   the_weights;
    void                  fourTrans(DComplex*, size_t, DComplex*);
    size_t                radix(size_t);
    void                  split(DComplex*, size_t, size_t, DComplex*);
    void                  join(DComplex*, size_t, size_t, DComplex*);
public:
    /**
     * Constructs a complex FFT server to transform a vector of unspecified
     * length. This length is initialized when the server is used.
     */
    DComplexFFTServer();

    /**
     * Constructs a complex FFT server designed to transform a vector
     * of length \a order.
     */
    DComplexFFTServer(size_t order);

    /**
     * Copy constructor. The new server references the old server's
     * data.
     */
    DComplexFFTServer(const DComplexFFTServer&);

    /**
     * Sets self to the same order as \a v.
     */
    DComplexFFTServer& operator=(const DComplexFFTServer& v);

    /**
     * Returns the order of an FFT server, that is, the vector length
     * it is currently configured to transform.
     */
    size_t                order() const {
        return server_N;
    }

    /**
     * Resets the order of an FFT server to \a N.
     */
    void                  setOrder(size_t N); // Set new N

    /***********  TRANSFORMS ***********/

    /**
     * Returns the forward FFT, as defined above, of a complex vector
     * \a v. The expression:
     *
     * @code
     * DComplexFFTServer  f;
     * RWMathVec<DComplex>  v;
     * std::cout  <<  f.fourier(v);
     * @endcode
     *
     * outputs the Fourier transform of \a v as a complex vector with
     * \c N points. If the server is not currently configured to the
     * length of \a v, it reconfigures.
     */
    RWMathVec<DComplex>           fourier(const RWMathVec<DComplex>& v);
    void                          fourier(const RWMathVec<DComplex>&, RWMathVec<DComplex>&);

    /**
     * Returns the inverse FFT, as defined above, of a complex vector
     * \a v. The expression:
     *
     * @code
     * DComplexFFTServer  f;
     * RWMathVec<DComplex>  v;
     * std::cout  <<  f.fourier(v);
     * @endcode
     *
     * outputs the inverse Fourier transform of \a v as a complex vector
     * with \c N points.
     */
    RWMathVec<DComplex>           ifourier(const RWMathVec<DComplex>& v);
    void                          ifourier(const RWMathVec<DComplex>&, RWMathVec<DComplex>&);
};

#endif /* __RWCFFT_H__ */
