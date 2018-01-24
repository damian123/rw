#ifndef RW_REGEX_RETRY_H
#define RW_REGEX_RETRY_H



/**********************************************************************
 *
 * rw_regex_retry.h - contains the class definition for the retry struct
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regex/retry.h#1 $
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


#include <rw/tools/regex/defs.h>
#include <rw/tools/regex/state.h>
#include <rw/tools/regex/vector.h>

/**************************************************************************/
/*                                                                        */
/* Class Name                                                             */
/* ==========                                                             */
/*                                                                        */
/* RWRegexAlphaData                                                       */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This class is used to store information about the alpha states for a   */
/* retry state.  This information is needed to properly restore the       */
/* state machine for a retry operation.                                   */
/*                                                                        */
/**************************************************************************/
template <class E>
class RWRegexAlphaData
{
private:
    typedef typename RWTRegexTraits<E>::Char RChar;

public:
    RWRegexAlphaData() {}
    RWRegexAlphaData(RWRegexState<E>* alpha) :
        visited_(alpha->visited_),
        lastGood_(alpha->lastGood_),
        match_((*alpha->regex_->matches_)()[alpha->aID_]),
        firstMatch_(alpha->firstMatch_)
    {}
    RWRegexAlphaData(const RWRegexAlphaData& source) :
        visited_(source.visited_),
        lastGood_(source.lastGood_),
        match_(source.match_),
        firstMatch_(source.firstMatch_)
    {}

    RWRegexAlphaData& operator=(const RWRegexAlphaData& rhs) {
        visited_    = rhs.visited_;
        lastGood_   = rhs.lastGood_;
        match_      = rhs.match_;
        firstMatch_ = rhs.firstMatch_;
        return *this;
    }

    size_t          visited_;    // Alpha's visited setting
    const RChar*    lastGood_;   // Alpha's lastGood setting
    size_t          match_;      // Alpha's match setting
    const RChar*    firstMatch_; // Alpha's lastGood setting
};

/**************************************************************************/
/*                                                                        */
/* Class Name                                                             */
/* ==========                                                             */
/*                                                                        */
/* RWRegexRetry                                                           */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This class represents a queue of retry attempts for backtracking       */
/* through the NFA when greedy match attempts fail, and we need to        */
/* return to a known "good" position in the match.                        */
/*                                                                        */
/**************************************************************************/
template <class E> class RWRegexState;
template <class E>
struct RWRegexRetry {
    // Obligatory default constructor
    RWRegexRetry() : alpha_(0), beta_(size_t(-1)), state_(0), lGood_(0), start_(0) {}

    // Normal use constructor
    RWRegexRetry(RWRegexState<E>*                          newAlpha,
                 size_t                                    newBeta,
                 bool                                 newBGood,
                 RWRegexState<E>*                          newState,
                 const typename RWTRegexTraits<E>::Char* newLGood,
                 const typename RWTRegexTraits<E>::Char* newStart) :
        alpha_(newAlpha),
        beta_(newBeta),
        bGood_(newBGood),
        state_(newState),
        lGood_(newLGood),
        start_(newStart)
    {}

    // Copy constructor
    RWRegexRetry(const RWRegexRetry<E>& rhs) :
        alpha_(rhs.alpha_),
        beta_(rhs.beta_),
        bGood_(rhs.bGood_),
        state_(rhs.state_),
        lGood_(rhs.lGood_),
        start_(rhs.start_)
    {}

    // Destructor
    ~RWRegexRetry() {}

    // Equality comparator
    bool operator==(const RWRegexRetry& rhs) const {
        bool rc = true;

        // The two items are defined to be equal if the retry
        // states and starting positions are equal, AND, all
        // visited tags for alpha states compare equal
        if (start_ == rhs.start_ && state_ == rhs.state_) {
            size_t size = aData_.size() <= rhs.aData_.size() ? aData_.size() : rhs.aData_.size();
            for (size_t i = 0; i < size; i++) {
                if (aData_[i].visited_ != rhs.aData_[i].visited_) {
                    rc = false;
                }
            }
        }
        else {
            rc = false;
        }

        return rc;
    }

    // Attribute data
    RWRegexState<E>*                          alpha_;  // Alpha of retry state
    size_t                                    beta_;   // Beta ID of a beta retry
    bool                                 bGood_;  // True if visited should be updated
    RWRegexState<E>*                          state_;  // Actual Retry state
    const typename RWTRegexTraits<E>::Char* lGood_;  // State's lastGood setting
    const typename RWTRegexTraits<E>::Char* start_;  // Starting character position for retry

    // Alpha state data
    RWRegVec<RWRegexAlphaData<E> >            aData_;  // Recursive alpha data
};

#endif
