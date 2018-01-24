#ifndef __RWDCOMPIO_H__
#define __RWDCOMPIO_H__

/***************************************************************************
 *
 * dcompio.h - Standard header file for double precision complex
 * I/O functions
 *
 * $Id: //math/13/rw/dcompio.h#1 $
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
 ***************************************************************************/

#include <rw/dcomplex.h>
#include <rw/rwfile.h>
#include <rw/vstream.h>

inline RWFile& operator <<(RWFile& f, const DComplex& a)
{
    double tmpReal = RW_MATH_SL_STD_GLOBAL(real)(a);
    f.Write(tmpReal);
    tmpReal = RW_MATH_SL_STD_GLOBAL(imag)(a);
    f.Write(tmpReal);
    return f;
}

inline RWFile& operator >>(RWFile& f, DComplex& a)
{
    double tmpReal, tmpImag;
    f.Read(tmpReal);
    f.Read(tmpImag);
    a = DComplex(tmpReal, tmpImag);
    return f;
}

inline RWvostream& operator <<(RWvostream& s, const DComplex& a)
{
    double tmpReal = RW_MATH_SL_STD_GLOBAL(real)(a);
    s << tmpReal;
    tmpReal = RW_MATH_SL_STD_GLOBAL(imag)(a);
    s << tmpReal;
    return s;
}

inline RWvistream& operator >>(RWvistream& s, DComplex& a)
{
    double tmpReal, tmpImag;
    s >> tmpReal;
    s >> tmpImag;
    a = DComplex(tmpReal, tmpImag);
    return s;
}


#endif /* __RWDCOMPIO_H__ */
