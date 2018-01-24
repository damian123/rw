
#ifndef __RWREF_H__
#define __RWREF_H__
/*
 * Reference classes.
 *
 * Changed having a reference to "data" to using a pointer to "data"
 * because the bcc 3.0 compiler generates incorrect code sometimes for
 * classes containing a reference and no explicit copy constructor.
 * This is really too bad; the reference semantics make more sense here.
 *
 * The operator<<() output routines were added because the
 * Borland C++ compiler can't figure out enough to call the
 * conversion routine and then call the proper op<< routine.
 * For some bizarre reason it thinks there is a conflict with
 * converting the ref to a char*.  Hey, I don't pretend to understand.
 *
 * $Id: //lapack/13/rw/lapack/ref.h#1 $
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

#include <rw/lapkdefs.h>
#include <rw/rstream.h>

template <class TypeT>
class RWRORef
{
private:
    TypeT*        data;
    bool   readonly;
    TypeT         error(TypeT);   // Called when op=() invoked for a readonly
public:
    RWRORef(TypeT& x, bool ro = false) : data(&x) {
        readonly = ro;
    }
    RWRORef<TypeT>& operator=(TypeT x) {
        if (readonly) {
            *data = error(x);
        }
        else {
            *data = x;
        }
        return *this;
    }

    operator TypeT() {
        return *data;
    }
    friend std::ostream& operator<<(std::ostream& s, RWRORef<TypeT>& r) {
        return s << *r.data;
    }
};

template <class TypeT>
class RWNGRef
{
private:
    TypeT*        data;
    bool   negate;
public:
    RWNGRef(TypeT& x, bool ng = false) : data(&x) {
        negate = ng;
    }
    RWNGRef<TypeT>& operator=(TypeT x) {
        if (negate) {
            *data = -x;
        }
        else {
            *data = x;
        }
        return *this;
    }

    operator TypeT() {
        if (negate) {
            return -(*data);
        }
        return *data;
    }

    friend std::ostream& operator<<(std::ostream& s, RWNGRef<TypeT>& r) {
        return s << (r.negate ? -*r.data : *r.data);
    }
};

template <class TypeT>
class RWCJRef
{
private:
    TypeT*        data;
    bool   conjugate;
public:
    RWCJRef(TypeT& x, bool cj = false) : data(&x) {
        conjugate = cj;
    }
    RWCJRef& operator=(TypeT x) {
        if (conjugate) {
            *data = conj(x);
        }
        else {
            *data = x;
        }
        return *this;
    }

    operator TypeT() {
        if (conjugate) {
            return conj(*data);
        }
        return *data;
    }

    friend std::ostream& operator<<(std::ostream& s, RWCJRef<TypeT>& r) {
        return s << TypeT(r);
    }
};

template <class TypeT>
class RWROCJRef
{
private:
    TypeT*                data;
    unsigned char       rocj;         // 0:readable, 1:readonly, 2:conj
    TypeT                 error(TypeT);   // Called when op=() invoked for a readonly
public:
    RWROCJRef(TypeT& x, bool ro = false, bool cj = false) : data(&x) {
        rocj = (ro ? 1 : (cj ? 2 : 0));
    }
    RWROCJRef<TypeT>& operator=(TypeT x) {
        if (rocj == 0) {
            *data = x;
        }
        else if (rocj == 2) {
            *data = conj(x);
        }
        else {
            *data = error(x);
        }
        return *this;
    }

    operator TypeT() {
        if (rocj == 2) {
            return conj(*data);
        }
        return *data;
    }

    friend std::ostream& operator<<(std::ostream& s, RWROCJRef<TypeT>& r) {
        return s << TypeT(r);
    }
};

#endif
