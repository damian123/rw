#ifndef RW_REGEX_IMP_H
#define RW_REGEX_IMP_H

/**********************************************************************
 *
 * imp.h - contains the class definition for the NFA regex impl
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regex/imp.h#1 $
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

#include <rw/rwerr.h>
#include <rw/tools/regex.h>
#include <rw/tools/regex/defs.h>
#include <rw/tools/regex/state.h>
#include <rw/tools/regex/compiler.h>
#include <rw/tools/regex/retry.h>
#include <rw/tools/regex/alt.h>
#include <rw/tools/regex/bufman.h>
#include <rw/tools/regex/vector.h>

/**************************************************************************/
/*                                                                        */
/* Class Name                                                             */
/* ==========                                                             */
/*                                                                        */
/* RWRegexImp                                                             */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This class implements regular expression functionality as an NFA       */
/* state machine.                                                         */
/*                                                                        */
/**************************************************************************/
template <class E>
class RWRegexImp
{
    friend class RWRegexCompiler<E>;
    friend class RWRegexState<E>;
    friend class RWRegexAlphaData<E>;

private:
    typedef typename RWTRegexTraits<E>::Char RChar;

public:
    // Constructors
    RWRegexImp();
    RWRegexImp(const RChar* str,
               size_t       length = size_t(-1));
    RWRegexImp(const RWRegexImp& source);

    // Destructor
    virtual ~RWRegexImp();

    // Operators
    RWRegexImp& operator= (const RWRegexImp& rhs);
    bool   operator< (const RWRegexImp& rhs) const;
    bool   operator==(const RWRegexImp& rhs) const;

    // Query number of parenthesized sub-expressions
    size_t subCount() const;

    // Pattern matching operations
    RWTRegexResult<E> search(const RChar* str, size_t start, size_t length);
    RWTRegexResult<E> matchAt(const RChar* str, size_t start, size_t length);

    // Regular expression status information
    const RWRegexErr& getStatus() const;

    // Extra utility method used by RWTRegex
    size_t strLen(const RChar* str) const;

protected:
    enum Constants {
        PatternLength = 128,
        NumMatches = 64
    };

    void                  doCopy(const RWRegexImp& source);
    void                  setStatus(const RWRegexErr& newStatus);
    void                  clear();
    void                  addState(RWRegexState<E>* s);
    void                  initMatchesFrom(size_t index = 0);
    bool             doMatch(const RChar* str, size_t length);
    const RChar*          matchOnce(RWRegexState<E>* s, const RChar*& c);
    void                  calcLengthsAndSave();
    bool             alphasMatched(RWRegexState<E>* cur);
    const RChar*          popRetry(RWRegexState<E>*& state);
    bool             findRetry(const RWRegexRetry<E>& retry);
    bool             canMatchAnother(RWRegexState<E>* cur);
    size_t                nextOffset(const RChar* str, size_t length);

    // #define RW_DEBUG_REGEX_RETRIES

#if defined(RW_DEBUG_REGEX_RETRIES)
    void saveAlphaRetries(RWRegexState<E>*&, const RChar*);
    void saveBetaRetries(RWRegexState<E>*&, const RChar*);
    void saveLookaheadRetries(RWRegexState<E>*&, const RChar*);
#endif


    RWRegexState<E>*  self(size_t i) {
        return states_()[i];
    }
    RWRegexState<E>*  next(size_t i) {
        return states_()[states_()[i]->myNext_];
    }
    RWRegexState<E>*  null_t(size_t) {
        return states_()[numStates_ - 1];
    }
    RWRegexState<E>*  alpha(size_t i) {
        return states_()[i]->myAlpha_;
    }
    RWRegexState<E>*  close(size_t i) {
        return states_()[states_()[i]->myClose_];
    }

    // Regular expression pattern length, and pattern buffer
    size_t                                          patternLength_;
    RWBufMan<RChar, PatternLength>                  pattern_;

    // Number of regular expression states, and a buffer for them
    size_t                                          numStates_;
    RWBufMan<RWRegexState<E>*, PatternLength>       states_;

    // Regular expression compiler, retries queue, and alternator
    RWRegexCompiler<E>                              compiler_;
    RWRegVec<RWRegexRetry<E> >                      retries_;
    RWRegVec<RWRegexRetry<E> >                      retriesRec_;
    RWRegexAlternator<E>                            alternator_;

    // Anchoring and validation flags
    bool                                       invalid_;

    // Saved matches information:
    // alphaCount   - count of number of alpha states, i.e. number
    //                of subexpression match starts and lengths
    // matches      - current match attempts match start positions
    // lengths      - subexpression match lengths
    // savedMatches - saved matches for saving start positions of
    //                the best match
    size_t                                          alphaCount_;
    RWBufMan<size_t, NumMatches>                    savedMatches_;
    RWBufMan<size_t, NumMatches>                    savedLengths_;
    RWBufMan<size_t, NumMatches>*                   matches_;
    RWBufMan<size_t, NumMatches>*                   lengths_;

    // Beginning of string (bos) and end of string (eos) for
    // current matching operation.
    const  RChar*                                   bos_;
    const  RChar*                                   eos_;

    // offset to begin of substring for searching
    size_t                                          off_;

    // Beta ID and character position of a beta retry (retry saved as a
    // result of finding that another alternation would give a match)
    const  RChar*                                   rBetaC_;
    size_t                                          rBeta_;

    // Set to true when we are restarting the engine on an alpha retry
    // (a retry performed because an inner, repeated element would match
    // the whole subexpression, but the greedy match forces us onward.
    bool                                       alphaRetry_;

    // Regular expression status
    RWTScopedPointer<RWRegexErr>                    status_;
};



template <class E>
class RWRegexMatchIteratorImp
{
private:
    typedef typename RWTRegexTraits<E>::Char RChar;

public:
    RWRegexMatchIteratorImp();
    RWRegexMatchIteratorImp(const RWRegexMatchIteratorImp& source);
    RWRegexMatchIteratorImp(RWTRegex<E>&   newRegex,
                            const RChar*   newString,
                            size_t         newStart,
                            size_t         newLength,
                            bool      newOverlap);

    virtual ~RWRegexMatchIteratorImp();

    RWRegexMatchIteratorImp&      operator=(const RWRegexMatchIteratorImp& rhs);
    RWRegexMatchIteratorImp&      operator++();
    const RWRegexMatchIteratorImp operator++(int);
    RWRegexMatchIteratorImp&      operator+=(size_t count);
    RWTRegexResult<E>             operator*() const;
    const RWTRegexResult<E>*      operator->() const;
    bool                     operator==(const RWRegexMatchIteratorImp& rhs) const;

private:
    void                          advance(size_t count = 1);

    RWTRegex<E>*                  regex_;
    const RChar*                  str_;
    size_t                        start_;
    size_t                        end_;
    bool                     overlap_;
    RWTRegexResult<E>             result_;
};

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("imp.cc")
#endif

#if defined(RW_NO_IMPLICIT_INCLUSION)
#  include "imp.cc"
#endif

#endif
