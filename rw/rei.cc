
/**********************************************************************
 *
 * RWTRegularExpressionImp definitions for RE Implementation class.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/rei.cc#1 $
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

#include <rw/defs.h>

#if !defined(RW_NO_EXCEPTIONS)

#  include <rw/re.h>

#  if !defined(RW_NO_IMPLICIT_INCLUSION)
#    include "rw/reposix1.cc"
#    include "rw/reposix2.cc"
#  endif

template<class charT>
RWTRegularExpressionImp<charT>::RWTRegularExpressionImp(const charT* s)
    : maxState_(~(size_t)1),
      getNextSymbol_(~(size_t)0),
      nonacceptingState_(0),
#  if defined(RW_NO_POSIX_RE)
      symbolSetVector_(4),
      fastRePtr_(NULL),
#  endif
      compiled_(false),
      rePtr_(new regex_t),
      reString_(s),
      nextState_(0)
{
    compile();
}

template<class charT>
RWTRegularExpressionImp<charT>::RWTRegularExpressionImp(
    const typename RWTRegularExpressionTraits<charT>::tStringT& s)
    : maxState_(~(size_t)1),
      getNextSymbol_(~(size_t)0),
      nonacceptingState_(0),
#  if defined(RW_NO_POSIX_RE)
      symbolSetVector_(4),
      fastRePtr_(NULL),
#  endif
      compiled_(false),
      rePtr_(new regex_t),
      reString_(s),
      nextState_(0)
{
    compile();
}



template<class charT>
RWTRegularExpressionImp<charT>::RWTRegularExpressionImp(
    const RWTRegularExpressionImp<charT>& other)
    : maxState_(~(size_t)1),
      getNextSymbol_(~(size_t)0),
#  if defined(RW_NO_POSIX_RE)
      symbolSetVector_(4),
      fastRePtr_(NULL),
#  endif
      compiled_(false),
      rePtr_(new regex_t),
      reString_(other.reString_)
{
    compile();
}

/////////////////////////////////////////////////////////////////////////////
//
// Constructors and Destructors
//
/////////////////////////////////////////////////////////////////////////////

// this guard is used for normal (i.e., "out of class body") instantiations
#  ifndef __RWREI_CC__
#    define __RWREI_CC__ 1

template<class charT>
RWTRegularExpressionImp<charT>::RWTRegularExpressionImp()
    : maxState_(~(size_t)1),
      getNextSymbol_(~(size_t)0),
      nonacceptingState_(0),
#    if defined(RW_NO_POSIX_RE)
      symbolSetVector_(4),
      fastRePtr_(NULL),
#    endif
      compiled_(false),
      rePtr_(new regex_t),
      nextState_(0)
{
    clear();
}

template<class charT>
RWTRegularExpressionImp<charT>::~RWTRegularExpressionImp()
{
    delete fastRePtr_;
    fastRePtr_ = NULL;
    regfree(rePtr_);
    rePtr_ = NULL;
}

/////////////////////////////////////////////////////////////////////////////
//
// operator=
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> RWTRegularExpressionImp<charT>&
RWTRegularExpressionImp<charT>::operator=(
    const RWTRegularExpressionImp<charT>& other)
{
    reString_ = other.reString_;
    compile();
    return *this;
}

template<class charT> RWTRegularExpressionImp<charT>&
RWTRegularExpressionImp<charT>::operator=(
    const charT* reStringCharArray)
{
    reString_ = reStringCharArray;
    compile();
    return *this;
}

/////////////////////////////////////////////////////////////////////////////
//
// clear
//
//  Clears the entire nondeterministic finite automaton (NFA).
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> void
RWTRegularExpressionImp<charT>::clear()
{
    compiled_ = false;
    unmatchedOpenParen_ = 0;
    rePtr_->re_nsub = 0;

#    if defined(RW_NO_POSIX_RE)
    if (fastRePtr_ != NULL) {
        delete fastRePtr_;
        fastRePtr_ = NULL;
    }
    nextState_ = 0;
    nonacceptingState_ = 0;
    symbolSetVector_.clear();
    rePtr_->subexpression.clear();
    rePtr_->node.clear();
#    endif
}

/////////////////////////////////////////////////////////////////////////////
//
//  compile
//
//  Compiles regular expressions using regcomp().
//  Preprocesses RE's to replace escape sequences with literal characters.
//
/////////////////////////////////////////////////////////////////////////////

#    include <ctype.h>

template<class charT> void
RWTRegularExpressionImp<charT>::compile()
{
    clear();

#    if defined(RW_NO_POSIX_RE)
    // If regular expression can be handled by old "fast" RE class RWCRegexp,
    //  then don't use POSIX functions.

    compiled_ = false;
    bool charTrue = RWTRegularExpressionTraits<charT>::isChar();

    if (charTrue) {
        // RWTRegularExpression<char>, a.k.a. RWCRExpr
        const char*   cp = (const char*)reString_.data();
        const size_t  len(reString_.length());
        const char*   endCp = cp + len;              // Past end of string data.
        size_t        i = 0;                         // for() index.

        if (*cp != star() && *cp != plus() && *cp != questionMark()) { // BAD_REPEAT
            // Scan for symbols not in or treated differently in RWCRegexp;
            //  if found, then continue.
            for (; i < len; i++) {
                if    // Symbols treated differently in RWCRegexp.
                (
                    *cp == openBrace()      || *cp == closeBrace()      ||
                    *cp == openParen()      || *cp == closeParen()      ||
                    *cp == hat()            || *cp == dollar()          ||
                    *cp == bar()            || *cp == backslash()       ||
                    (
                        *cp == openBracket()  &&  cp < endCp              &&
                        (*(cp + 1) == colon() || *(cp + 1) == dot() || *(cp + 1) == equal())
                    )                                                   ||
                    (*cp && !isprint(*cp))
                ) {
                    break;
                }

                if (*cp == dash() && cp < endCp && *(cp + 1) == dash()) {
                    break;    // BAD_CHAR_RANGE syntax error.
                }

                if    // BAD_REPEAT syntax error.
                (
                    (*cp == star() || *cp == plus() || *cp == questionMark()) &&
                    cp < endCp                                                &&
                    (
                        *(cp + 1) == star() || *(cp + 1) == plus()              ||
                        *(cp + 1) == questionMark()
                    )
                ) {
                    break;
                }

                cp++;
            } // for
        } // if

        if (i >= len) {   // No extended RE symbols found; use RWCRExpr.
            compiled_ = true;
            fastRePtr_ = new RWCRegexp((const char*)reString_.data());

            // Warning: Only call fastRePtr_->status() once because
            // RWCRegexp::status() resets status to OK.

            switch (fastRePtr_->status()) {
                case RWCRegexp::OK:
                    status_ = RWTRegularExpression<charT>::OK;
                    return;
                case RWCRegexp::TOOLONG:
                    break;
                default:
                    status_ = RWTRegularExpression<charT>::BAD_PATTERN;
                    return;
            }

            // if error was just too long, try with new re class
            delete fastRePtr_;
            fastRePtr_ = NULL;
            compiled_ = false;
        }
    }

#    endif  // NO_RE_POSIX

    int error  = RW_REGCOMP(rePtr_, reString_, REG_E_EXTENDED);

    if (!error) {
        status_ = RWTRegularExpression<charT>::OK;
        compiled_ = true;
    }
    else { // Convert POSIX.2 error code to RWTRegularExpression<charT> error code.
        clear();

        if (error == REG_ECOLLATE) {
            status_ = RWTRegularExpression<charT>::BAD_COLLATING_ELEMENT;
        }
        else if (error == REG_ECOLLATE) {
            status_ = RWTRegularExpression<charT>::BAD_COLLATING_ELEMENT;
        }
        else if (error == REG_ECTYPE) {
            status_ = RWTRegularExpression<charT>::BAD_CHAR_CLASS_TYPE;
        }
        else if (error == REG_EESCAPE) {
            status_ = RWTRegularExpression<charT>::TRAILING_BACKSLASH;
        }
        else if (error == REG_EBRACK) {
            status_ = RWTRegularExpression<charT>::UNMATCHED_BRACKET;
        }
        else if (error == REG_EPAREN) {
            status_ = RWTRegularExpression<charT>::UNMATCHED_PARENTHESIS;
        }
        else if (error == REG_EBRACE) {
            status_ = RWTRegularExpression<charT>::UNMATCHED_BRACE;
        }
        else if (error == REG_BADBR) {
            status_ = RWTRegularExpression<charT>::BAD_BRACE;
        }
        else if (error == REG_ERANGE) {
            status_ = RWTRegularExpression<charT>::BAD_CHAR_RANGE;
        }
        else if (error == REG_ESPACE) {
            status_ = RWTRegularExpression<charT>::OUT_OF_MEMORY;
        }
        else if (error == REG_BADRPT) {
            status_ = RWTRegularExpression<charT>::BAD_REPEAT;
        }
        else if (error == RWTRegularExpression<charT>::NOT_SUPPORTED) {
            status_ = RWTRegularExpression<charT>::NOT_SUPPORTED;
        }
        else {
            status_ = RWTRegularExpression<charT>::BAD_PATTERN;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
//
// index
//
//  Returns the index of the leftmost longest instance in searchString that
//  matches the regular expression compiled in self or RW_NPOS if there is no
//  such match.
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> size_t
RWTRegularExpressionImp<charT>::index(
    const typename
    RWTRegularExpressionTraits<charT>::tStringT& searchString,
    size_t* len, size_t start)
{

#    if defined(RW_NO_POSIX_RE)
    // Use "fast" RE class if allowed.
    if (fastRePtr_) {
        size_t      tLen;
        size_t      result(fastRePtr_->index(searchString, &tLen, start));

        if (len) {
            *len = tLen;
        }

        return result;
    }
#    endif

    const charT*  searchStringPtr = searchString.data() + start;
    size_t        searchStringLength(searchString.length() - start);
    regmatch_t    match[MAX_NO_OF_SUBEXPRESSIONS];
    errorType     error;

    for (int i(0); i < MAX_NO_OF_SUBEXPRESSIONS; i++) {
        match[i].rm_so = (regoff_t) - 1;
        match[i].rm_eo = (regoff_t) - 1;
    }

    error =
        (errorType)RW_REGEXEC
        (
            rePtr_, searchStringPtr, searchStringLength,
            MAX_NO_OF_SUBEXPRESSIONS, match, 0x0
        );

    if (error) {
        if (error == REG_NOMATCH) {
            status_ = RWTRegularExpression<charT>::NO_MATCH;
        }
        //
        // dso: Cast quiets egcs 1.1.2 compiler warning about the comparison
        //      between an errorType and a statusType.
        //
        else if (error == (errorType) RWTRegularExpression<charT>::NOT_SUPPORTED) {
            status_ = RWTRegularExpression<charT>::NOT_SUPPORTED;
        }
        else {
            status_ = RWTRegularExpression<charT>::BAD_PATTERN;
        }

        if (len) {
            *len = 0;
        }

        return RW_NPOS;
    }
    else if (len) {
        *len = match[0].rm_eo - match[0].rm_so;
    }

    return match[0].rm_so + start;
}

#  endif // !defined(RW_NO_EXCEPTIONS)

#endif // __RWREI_CC__
