#ifndef __RWDCOSFFT_H__
#define __RWDCOSFFT_H__

/***************************************************************************
 *
 * dcosfft.h - Double Precision Fast Cosine server
 *  (NOTE: sine and cosine transforms are both included)
 *
 * $Id: //math/13/rw/dcosfft.h#1 $
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

#include <rw/dfft.h>

/**
 * @ingroup fft_server_classes
 *
 * @brief Performs double precision fast sine or cosine transforms of
 * a real vector.
 *
 * Class DoubleCosineServer performs double precision fast sine
 * or cosine transforms of a real vector. The transform of a real,
 * even sequence is a <i>cosine transform</i>. The transform of
 * a real, odd sequence is a <i>sine transform</i>.
 *
 * <b>Even Sequence:</b>
 *
 * A real \e even vector is symmetric, that is, <tt>V(j) == V(-j)</tt>
 * or <tt>V(j) == V(2N-j)</tt>, where \c 2N is the total
 * length of the vector. Of the \c 2N points, only the <tt>N+1</tt>
 * points <tt>V(j), j=0, ... , N</tt> need be given. The upper half
 * of \c V can be recovered from <tt>V(j) = V(2N-j), j = N+1,
 * ... , 2N-1</tt>. This means that of the total \c 2N sequence,
 * <tt>N-1</tt> values are repeated twice and two [<tt>V(0)</tt> and
 * <tt>V(N)</tt>] are unique. This produces a total of <tt>2N</tt> points,
 * of which only <tt>N+1</tt> are actually stored.
 *
 * The inverse Fourier transform (IDFT) of a real even sequence
 * is a cosine transform. The result is also a real even sequence.
 * The transform is defined as follows. As above, assume that <tt>V(j),
 * j = 0, 1, 2, ..., 2N-1</tt> is real, and that <tt>V(j) == V(2N-j)</tt>.
 * Then:
 *
 *
 * @f$
 * V(j) = \displaystyle\sum_{n=0}^{2N-1}C(n)exp\left(\frac{ijn\pi}{N}\right)
 * @f$
 *
 * or:
 *
 * @f$
 * V(j) = \frac{a(0)}{2}+\displaystyle\sum_{n=1}^{N-1}a(n)cos\left(\frac{\pi jn}{N}\right)
 * + \frac{(-1)^ja(N)}{2}
 * @f$
 *
 *
 * where the <tt>a(n)</tt>s are real and <tt>a(n) = 2C(n)</tt>.
 *
 * It also follows that the forward Fourier transform (DFT) of <tt>V(j)</tt>
 * can be expressed as a cosine series:
 *
 *
 * @f$
 * a(n) = \frac{V(0)}{2}+\displaystyle\sum_{j=1}^{N-1}V(j)cos\left(\frac{\pi jn}{N}\right)
 * + \frac{(-1)^jV(N)}{2}
 * @f$
 *
 *
 * Only the real part of <tt>C(n)</tt> is actually returned by cosine().
 *
 * <b>Odd Sequence:</b>
 *
 * A real <i>odd</i> vector is antisymmetric, that is, <tt>V(j) ==
 * -V(-j)</tt> or <tt>V(j) == -V(2N-j)</tt>. As above, the vector \c V
 * should be thought of as \c 2N points long, but here only
 * the points <tt>V(j),  j=1, ..., N-1</tt>, a total of <tt>N-1</tt>
 * points, need be given. This is true because we know that <tt>V(0)</tt>
 * and <tt>V(N)</tt> are always 0 if the sequence is odd. Consequently,
 * of the total \c 2N sequence, <tt>N-1</tt> values are repeated
 * twice and two are always 0. This produces a total of \c 2N
 * points, of which only <tt>N-1</tt> are actually stored.
 *
 * The sine transform is defined as:
 *
 *
 * @f$
 * b(n) = \frac{2}{N}\displaystyle\sum_{j=1}^{N-1}V(j)sin\left(\frac{\pi jn}{N}\right)
 * @f$
 *
 * and:
 *
 * @f$
 * V(j) = \displaystyle\sum_{j=1}^{N-1}b(n)sin\left(\frac{\pi jn}{N}\right)
 * @f$
 *
 *
 * Only the imaginary part of <tt>C(n)</tt> is actually returned by
 * sine().
 *
 * Due to an algorithmic limitation, the cosine and sine transforms
 * are limited to sequences with \c N even. In other words,
 * the \link RWVecView::length() length()\endlink of the vector \c V must be odd.
 *
 * <b><i>Speed Note:</i></b> In general, you have the choice of
 * using a cosine transform, or expanding to the full series and
 * using a regular \c complex FFT, saving only the lower half of the
 * real part. In reality, the cosine transform performs worst relative
 * to the regular \c complex FFT for short series that are a power
 * of 2 in length (empirically, the series that are 32, 16, 8, ...
 * points long). For this reason, you might want to consider using
 * the \c complex transform. For longer series, however, or for series
 * that are not a power of 2 in length, it is better to use the
 * cosine server. This also lets you use optimizations later.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/dcosfft.h>
 * DoubleCosineServer a;
 * @endcode
 */
class RW_MATH_SYMBOLIC DoubleCosineServer : private DoubleFFTServer
{
    size_t                server_N;
    RWMathVec<double>     sins;
protected:
    void                  checkOdd(int) const;
public:
    /**
     * Constructs a double precision cosine server for a vector of unspecified
     * length. This length is initialized when the server is used.
     */
    DoubleCosineServer();

    /**
     * Constructs a cosine server designed to transform a vector of
     * length \a N.
     */
    DoubleCosineServer(size_t N);

    /**
     * Copy constructor. The new server references the old server's
     * data.
     */
    DoubleCosineServer(const DoubleCosineServer&);

    /**
     * Sets self to the same order as \a v.
     */
    DoubleCosineServer& operator=(const DoubleCosineServer& v);

    /**
     * Returns the order of a cosine server, that is, the vector length
     * it is currently configured to transform.
     */
    size_t                order() const {
        return server_N;
    }

    /**
     * Resets the order of a cosine server to \a N.
     */
    void                  setOrder(size_t N); // Set new server_N

    /***********  TRANSFORMS ***********/

    // Returns DFT of a real even sequence, which is an even sequence:
    /**
     * Returns the cosine transform of a real vector.
     */
    RWMathVec<double>         cosine(const RWMathVec<double>& v);

    /**
     * Returns the inverse cosine transform of a real vector.
     */
    RWMathVec<double>         icosine(const RWMathVec<double>& v) {
        return cosine(v);
    }

    // Returns DFT of a real odd sequence, which is an odd sequence:
    /**
     * Returns the sine transform of a real vector.
     */
    RWMathVec<double>         sine(const RWMathVec<double>& v);

    /**
     * Returns the inverse sine transform of a real vector.
     */
    RWMathVec<double>         isine(const RWMathVec<double>& v) {
        return sine(v);
    }
};

#endif /*__RWDCOSFFT_H__*/
