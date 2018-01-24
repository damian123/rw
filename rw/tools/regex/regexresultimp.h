#ifndef RW_REGEXRESULTIMP_H
#define RW_REGEXRESULTIMP_H



/**********************************************************************
 *
 * results.h - contains the class definition and implementation of the
 *             RWTRegexResultImp class.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regex/regexresultimp.h#1 $
 *
 **********************************************************************
 *
 * Copyright (c) 1989-2015 Rogue Wave Software, Inc.  All Rights Reserved.
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
 **********************************************************************/


#include <rw/tools/regex/bufman.h>

template <class E>
class RWTRegexResultImp
{
    // Friend declaration
    friend class RWRegexImp<E>;

public:
    RWTRegexResultImp();
    RWTRegexResultImp(const RWTRegexResultImp<E>& source);
    ~RWTRegexResultImp();
    RWTRegexResultImp<E>& operator=(const RWTRegexResultImp<E>& rhs);
    operator bool() const;
    size_t size() const;
    size_t getStart(size_t matchID) const;
    size_t getLength(size_t matchID) const;

protected:
    enum RConstants { // compromise workaround for aCC and sunpro
        RPatternLength = 128,
        RNumMatches = 64
    };

private:
    RWBufMan<size_t, RNumMatches> starts_;
    RWBufMan<size_t, RNumMatches> lengths_;
};

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("regexresultimp.cc")
#endif

#if defined(RW_NO_IMPLICIT_INCLUSION)
#  include <rw/tools/regex/regexresultimp.cc>
#endif

#endif
