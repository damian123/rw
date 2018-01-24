#ifndef __RWDFFT_H__
#define __RWDFFT_H__

/***************************************************************************
 *
 * dfft.h - Double Precision FFT server
 *
 * $Id: //math/13/rw/dfft.h#1 $
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
 * This class does fourier transforms involving a real sequence.  It is
 * derived from class DComplexFFTServer.
 *
 * Note that the transform is not normalized: calling fourier(),
 * followed by ifourier() will leave the sequence multiplied by 2N.
 *
 * The server is set up to do a transform for a specified sequence
 * length (2N).  This length can be specified one of three ways:  Either
 * (1) at construction time; (2) by calling function setOrder(); or (3)
 * (the easiest!) let it figure it out for itself from the length of the
 * sequence handed to it.  If the sequence length changes, it will
 * automagically reconfigure (a relatively expensive calculation).
 */

#include <rw/cfft.h>

/**
 * @ingroup fft_server_classes
 *
 * @brief Performs double precision complex fast Fourier transforms (FFTs)
 * of a real vector.
 *
 * Class DoubleFFTServer performs double precision complex fast
 * Fourier transforms (FFTs) of a real vector.
 *
 * At any given moment, the server is configured to transform a
 * real vector of a specified length. This length can be set one
 * of three ways:
 *
 * -  At construction time
 * -  With the function setOrder()
 * -  By the server from a supplied vector length.
 *
 * If you use the last method, the server automatically reconfigures
 * to the new length if the vector length changes. This is an expensive
 * calculation. Hence, it is most efficient to call this method
 * repeatedly for series of the same length. Different FFT servers
 * may be constructed to handle different lengths.
 *
 * The real vector must have an even number of points.
 *
 * Let <tt>V(j), j=0, 1, ..., 2N-1</tt> be a \c 2N point long
 * real sequence. Its transform is a complex conjugate-even sequence,
 * that is:
 *
 * <tt>C(n) = conj[C(-n)]</tt>
 *
 * or:
 *
 * <tt>C(n) = conj[C(2N - n)]</tt>.
 *
 * The function fourier() returns the lower half of this complex
 * conjugate-even sequence, that is, <tt>C(n), n=0, ..., N</tt>.
 * The upper half can be recovered from the relationship <tt>C(n)
 * = conj[C(2N-n)]</tt>. Note that the returned complex conjugate-even
 * sequence <tt>C(n)</tt> has <tt>N+1</tt> complex points, or a total
 * of <tt>2N+2</tt> numbers. The two extra numbers are the imaginary
 * parts of <tt>C(0)</tt> and <tt>C(N)</tt>, which are always 0.
 *
 * The forward transform calculated is:
 *
 *
 * @f$
 * C(n) = \displaystyle\sum_{j=0}^{2N-1}V(j)exp\left(\frac{-i \pi nj}{2N}\right);
 * \quad n=0, \quad .., \quad 2N-1
 * @f$
 *
 *
 * The inverse Fourier transform calculated is:
 *
 *
 * @f$
 * V(j) = \displaystyle\sum_{n=0}^{2N-1}C(n)exp\left(\frac{i \pi nj}{2N}\right);
 * \quad j=0, \quad .., \quad 2N-1
 * @f$
 *
 *
 * where \c V is real and \c C is complex. Note that only
 * points <tt>C(n), n=0, ..., N</tt> should be given. Also note that
 * the sum is not normalized; a call to fourier() followed by a
 * call to ifourier() results in the original series multiplied
 * by \c 2N.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/dfft.h>
 * DoubleFFTServer dffts;
 * @endcode
 */
class RW_MATH_SYMBOLIC DoubleFFTServer : private DComplexFFTServer
{
    size_t                serverLength;
    RWMathVec<DComplex>           roots_of_1;
    RWMathVec<DComplex>           conjroots_of_1;
protected:
    void                  checkEven(int);
public:
    /**
     * Constructs a double precision FFT server for a vector of unspecified
     * length. This length is initialized when the server is used.
     */
    DoubleFFTServer();

    /**
     * Constructs a double precision FFT server designed to transform a vector
     * of length \a oforder.
     */
    DoubleFFTServer(size_t oforder);  // construct for sequence oforder points long

    /**
     * Copy constructor. The new server references the old server's
     * data.
     */
    DoubleFFTServer(const DoubleFFTServer&);

    /**
     * Sets self to the same order as \a v.
     */
    DoubleFFTServer& operator=(const DoubleFFTServer& v);

    /**
     * Returns the order of an FFT server, that is, the vector length
     * it is currently configured to transform.
     */
    size_t                order() const {
        return serverLength;
    }

    /**
     * Resets the order of an FFT server to \a N.
     */
    void                  setOrder(size_t N);   // Set up for sequence N point long

    /****************  TRANSFORMS ****************/

    /**
     * Returns the FFT of a real vector of \c 2N points. The expression:
     *
     * @code
     * DoubleFFTServer  f;
     * RWMathVec<DComplex>  v;
     * std::cout  <<  f.fourier(v);
     * @endcode
     *
     * prints the Fourier transform of \a v as a complex conjugate-even
     * sequence of <tt>N+1</tt> complex numbers.
     */
    RWMathVec<DComplex>           fourier(const RWMathVec<double>& v);
    void                          fourier(const RWMathVec<double>&, RWMathVec<DComplex>&);

    /**
     * Returns the inverse FFT of a complex conjugate-even sequence
     * of <tt>N+1</tt> complex numbers as a real sequence of \c 2N
     * points. The expression
     *
     * @code
     * DoubleFFTServer  f;
     * RWMathVec<DComplex>  v;
     * std::cout  <<  f.fourier(v);
     * @endcode
     *
     * prints the inverse Fourier transform of \a v as an \link RWMathVec
     * RWMathVec<double> \endlink vector.
     */
    RWMathVec<double>             ifourier(const RWMathVec<DComplex>& v);
    void                          ifourier(const RWMathVec<DComplex>& v, RWMathVec<double>&);

};

#endif /*__RWDFFT_H__*/
