
#ifndef __RWTREF_H__
#define __RWTREF_H__
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
 * $Id: //lapack/13/rw/lapack/mat.h#1 $
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

/**
 * @ingroup sparse_matrix_classes
 * @brief Handles the case of potentially read-only access to data.
 *
 * \link RWRORef RWRORef<T>\endlink handles the case of potentially read-only
 * access to data. It is used by the \link RWBandMat RWBandMat<T>\endlink,
 * \link RWSymBandMat RWSymBandMat<T>\endlink,
 * \link RWUpperTriMat RWUpperTriMat<T>\endlink,
 * \link RWLowerTriMat RWLowerTriMat<T>\endlink, and
 * \link RWTriDiagMat RWTriDiagMat<T>\endlink classes, which all contain
 * elements that are explicitly defined as 0 and \e cannot be changed. If an
 * element defined to be 0 is accessed, the \link RWRORef RWRORef<T>\endlink is
 * constructed with a reference to a static variable initialized to 0, and
 * information that the reference is read-only.
 */
template < class TypeT >
class RWRORef
{
private:
    TypeT*        data;
    bool   readonly;
    TypeT         error(TypeT);   // Called when op=() invoked for a readonly
public:
    /**
     * Builds a reference to \a x.  The parameter \a ro indicates if the
     * reference should be considered read-only.
     */
    RWRORef(TypeT& x, bool ro = false) : data(&x) {
        readonly = ro;
    }

    /**
     * This function changes the value referenced by the class to the value
     * \a x, after modifying \a x if necessary (for example, by taking its
     * conjugate). If the reference is supposed to be read-only, a runtime
     * error occurs.
     */
    RWRORef<TypeT>& operator=(TypeT x)  ;
    /*
    {
      if ( readonly )
      {
          *data = error(x);
      }
      else
      {
          *data = x;
      }
      return *this;
    }
    */

    /**
     * This type conversion operator is invoked by the compiler as necessary to
     * return the value referenced by the class for use as a right-side value.
     * It may have to modify the value as necessary (for example, to take its
     * conjugate).
     */
    operator TypeT() {
        return *data;
    }
    friend std::ostream& operator<<(std::ostream& s, RWRORef<TypeT>& r) {
        return s << *r.data;
    }
};

/**
 * @ingroup sparse_matrix_classes
 * @brief Handles the case of a reference to an element that may have
 * to be negated.
 *
 * \link RWNGRef RWNGRef<T>\endlink handles the case of a reference to an
 * element that may have to be negated. It is used by the
 * \link RWSkewMat RWSkewMat<T>\endlink matrix class.
 */
template < class TypeT >
class RWNGRef
{
private:
    TypeT*        data;
    bool   negate;
public:
    /**
     * Builds a reference to \a x.  The parameter \a ng indicates if the
     * reference should be negated.
     */
    RWNGRef(TypeT& x, bool ng = false) : data(&x) {
        negate = ng;
    }

    /**
     * This function changes the value referenced by the class to the value
     * \a x, after modifying \a x if necessary (for example, by taking its
     * conjugate). If the reference is supposed to be read-only, a runtime
     * error occurs.
     */
    RWNGRef<TypeT>& operator=(TypeT x) {
        if (negate) {
            *data = -x;
        }
        else {
            *data = x;
        }
        return *this;
    }

    /**
     * This type conversion operator is invoked by the compiler as necessary to
     * return the value referenced by the class for use as a right-side value.
     * It may have to modify the value as necessary (for example, to take its
     * conjugate).
     */
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

/**
 * @ingroup sparse_matrix_classes
 * @brief Used by the \link RWHermMat RWHermMat<T>\endlink
 * classes to reference data that may need to be conjugated.
 *
 * \link RWCJRef RWCJRef<T>\endlink is used by the
 * \link RWHermMat RWHermMat<T>\endlink classes to reference data that may need
 * to be conjugated.
 */
template <class TypeT>
class RWCJRef
{
private:
    TypeT*        data;
    bool   conjugate;
public:
    /**
     * Builds a reference to \a x.  The parameter \a cj indicates if the
     * reference should be conjugated.
     */
    RWCJRef(TypeT& x, bool cj = false) : data(&x) {
        conjugate = cj;
    }

    /**
     * This function changes the value referenced by the class to the value
     * \a x, after modifying \a x if necessary (for example, by taking its
     * conjugate). If the reference is supposed to be read-only, a runtime
     * error occurs.
     */
    RWCJRef<TypeT>& operator=(TypeT x) {
        if (conjugate) {
            *data = RW_MATH_SL_STD_GLOBAL(conj)(x);
        }
        else {
            *data = x;
        }
        return *this;
    }

    /**
     * This type conversion operator is invoked by the compiler as necessary to
     * return the value referenced by the class for use as a right-side value.
     * It may have to modify the value as necessary (for example, to take its
     * conjugate).
     */
    operator TypeT() {
        if (conjugate) {
            return RW_MATH_SL_STD_GLOBAL(conj)(*data);
        }
        return *data;
    }

    friend std::ostream& operator<<(std::ostream& s, RWCJRef<TypeT>& r) {
        return s << TypeT(r);
    }
};

/**
 * @ingroup sparse_matrix_classes
 * @brief Handles a reference to a datum that may be read-only,
 * or may need to be conjugated.
 *
 * \link RWROCJRef RWROCJRef<T>\endlink handles a reference to a datum that may
 * be read-only, or may need to be conjugated. It is used by the
 * \link RWHermBandMat RWHermBandMat<T>\endlink classes. Elements outside the
 * band are defined as 0. Accessing these elements returns an
 * \link RWROCJRef RWROCJRef<T>\endlink initialized with a static variable
 * initialized to 0, and information that the reference is read-only.
 */
template <class TypeT>
class RWROCJRef
{
private:
    TypeT*                data;
    unsigned char       rocj;         // 0:readable, 1:readonly, 2:conj
    TypeT                 error(TypeT);   // Called when op=() invoked for a readonly
public:
    /**
     * Builds a reference to \a x.  The parameter \a ro indicates if the
     * reference should be considered read-only.  The parameter \a cj indicates
     * if the reference should be conjugated.
     */
    RWROCJRef(TypeT& x, bool ro = false, bool cj = false) : data(&x) {
        rocj = (unsigned char)(ro ? 1 : (cj ? 2 : 0));
    }

    /**
     * This function changes the value referenced by the class to the value
     * \a x, after modifying \a x if necessary (for example, by taking its
     * conjugate). If the reference is supposed to be read-only, a runtime
     * error occurs.
     */
    RWROCJRef<TypeT>& operator=(TypeT x) {
        if (rocj == 0) {
            *data = x;
        }
        else if (rocj == 2) {
            *data = RW_MATH_SL_STD_GLOBAL(conj)(x);
        }
        else {
            *data = error(x);
        }
        return *this;
    }

    /**
     * This type conversion operator is invoked by the compiler as necessary to
     * return the value referenced by the class for use as a right-side value.
     * It may have to modify the value as necessary (for example, to take its
     * conjugate).
     */
    operator TypeT() {
        if (rocj == 2) {
            return RW_MATH_SL_STD_GLOBAL(conj)(*data);
        }
        return *data;
    }

    friend std::ostream& operator<<(std::ostream& s, RWROCJRef<TypeT>& r) {
        return s << TypeT(r);
    }
};


#ifdef RW_MATH_COMPILE_INSTANTIATE
#  include <rw/lapack/mat.cc>
#endif

#endif
