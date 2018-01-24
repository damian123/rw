

/**********************************************************************
 *
 * rw_regex_state.cc - contains the implementation for NFA regex states
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regex/state.cc#1 $
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


#include <rw/tools/regex.h>
#include <rw/tools/regex/lexicon.h>

/////////////////////////////////////////////////////////////////////
//
// Default construction
//
/////////////////////////////////////////////////////////////////////
template <class E>
RWRegexState<E>::RWRegexState() :
    myID_(0),
    myNext_(0),
    myClose_(0),
    opt_(0),
    sub_(0),
    rep_(0),
    end_(0),
    alpha_(0),
    regex_(0),
    aID_(0),
    lex_(0),
    minVisits_(0),
    maxVisits_(0),
    visited_(0),
    matched_(false),
    fail_(false),
    lastGood_(0),
    firstMatch_(0),
    betaID_(size_t(-1)),
    betaIndex_(size_t(-1)),
    fib_(false),
    anchorBegin_(false),
    anchorEnd_(false),
    myAlpha_(0),
    primary_(0),
    failed_(0),
    lookahead_(0)
{
}

/////////////////////////////////////////////////////////////////////
//
// Construct with specific values for members
//
/////////////////////////////////////////////////////////////////////
template <class E>
RWRegexState<E>::RWRegexState(size_t               newMyID,
                              size_t               newMyNext,
                              size_t               newMyClose,
                              RWRegexImp<E>*       newRegex,
                              unsigned char        newAlpha,
                              RWRegexLexicon<E>*   newLex,
                              RWRegexState<E>*     newMyAlpha
                             ) :
    myID_(newMyID),
    myNext_(newMyNext),
    myClose_(newMyClose),
    opt_(0),
    sub_(0),
    rep_(0),
    end_(0),
    alpha_(newAlpha),
    regex_(newRegex),
    aID_(0),
    lex_(newLex),
    minVisits_(0),
    maxVisits_(0),
    visited_(0),
    matched_(false),
    fail_(false),
    lastGood_(0),
    firstMatch_(0),
    betaID_(size_t(-1)),
    betaIndex_(size_t(-1)),
    fib_(false),
    anchorBegin_(false),
    anchorEnd_(false),
    myAlpha_(newMyAlpha),
    primary_(0),
    failed_(0),
    lookahead_(0)
{
}

/////////////////////////////////////////////////////////////////////
//
// Destructor
//
/////////////////////////////////////////////////////////////////////
template <class E>
RWRegexState<E>::~RWRegexState()
{
}

/////////////////////////////////////////////////////////////////////
//
// While matching, reset visited count and lastGood marker.  Also,
// the matched is set according to optional setting - optional
// items are considered already matched.  The fail setting is reset,
// and if we are resetting from the "top level" (from before a match
// starts, as opposed to when we loop back to an alpha state or
// retry), we erase the firstmatch marker.
//
/////////////////////////////////////////////////////////////////////
template <class E>
void RWRegexState<E>::init(bool topLevel)
{
    visited_ = 0;
    lastGood_ = 0;

    matched_ = opt_ ? true : false;
    fail_ = false;

    if (topLevel) {
        firstMatch_ = 0;
    }
}

/////////////////////////////////////////////////////////////////////
//
// This method contains the top-level logic for matching in a
// particular state.
//
/////////////////////////////////////////////////////////////////////
template <class E>
RWRegexState<E>* RWRegexState<E>::doMatch(const RChar*& c)
{
    RWRegexState<E>* rc;

    // Handle alpha state processing
    if (alpha_) {
        rc = handleAlphaState(c);
    }
    // OR if this visit would exceed max visit count, then move on
    else if (visited_ >= maxVisits_) {
        rc = failed_;
    }
    else if (match(c)) {
        // OR if a non-alpha state matches...

        // Save this as the last good point for easy recovery
        // in subexpressions

        lastGood_ = c;
        visited_++;
        matched_ =
            (visited_ >= minVisits_ && (!anchorEnd_ || c == regex_->eos_));

        rc = visited_ > maxVisits_ ? failed_ : primary_;

        if ((anchorEnd_ && c != regex_->eos_) &&
                ((rc->myID_ > myID_) ||
                 (rc == myAlpha_ && !myAlpha_->rep_))) {
            rc = regex_->states_() [regex_->numStates_ - 1];
        }

        // Allow for zero-length matches from other types of lexicons
        if (end_ && matched_ && myAlpha_->visited_ >= myAlpha_->minVisits_) {
            myAlpha_->matched_ = (!myAlpha_->anchorEnd_ || c == regex_->eos_);
        }
    }
    else if (end_ && matched_) {
        // OR if we have an end state that is going to
        // transition to an alpha

        myAlpha_->matched_ = (!myAlpha_->anchorEnd_ || c == regex_->eos_);
        if (!anchorEnd_ || c == regex_->eos_) {
            rc = myAlpha_;
        }
        else {
            rc = regex_->states_() [regex_->numStates_ - 1];
        }
    }
    else if (visited_ >= minVisits_) {
        // OR if we've visited a non-alpha-state enough times already,
        // just move on; intended to handle cases like: a?b ==> if enough
        // a's, go to b

        if ((anchorEnd_ && c != regex_->eos_) &&
                ((failed_->myID_ > myID_) ||
                 (failed_ == myAlpha_ && !myAlpha_->rep_))) {
            rc = regex_->states_()[regex_->numStates_ - 1];
        }
        else {
            rc = failed_;
        }

    }
    else {
        // Otherwise, we've really got trouble - fail any subexp,
        // and goto alpha state (which is failed end state for non
        // subexp states).
        myAlpha_->fail_ = true;
        rc = myAlpha_;
    }

    return rc;
}

/////////////////////////////////////////////////////////////////////
//
// Create a copy of this regex state (used in regular expression
// copy construction).
//
/////////////////////////////////////////////////////////////////////
template <class E>
RWRegexState<E>* RWRegexState<E>::clone()
{
    return new RWRegexState<E>(*this);
}

/////////////////////////////////////////////////////////////////////
//
// This method handles matching logic for alpha states
//
/////////////////////////////////////////////////////////////////////
template <class E>
inline RWRegexState<E>* RWRegexState<E>::handleAlphaState(const RChar*& c)
{
    RWRegexState<E>* rc;

    if (fail_) {
        if (regex_->retries_.size() && regex_->retries_.back().alpha_ == this) {
            // Exercise any retries where the retry state is part
            // of this subexp (optimization)

            // Extract retry information from retries stack

            fail_ = false;

            rc = regex_->retries_.back().alpha_;
            c = regex_->retries_.back().start_;

            regex_->alphaRetry_ = (rc == regex_->retries_.back().state_);

            // Recursively update parent alpha states if there are any
            if (regex_->retries_.back().state_->myAlpha_ != rc) {
                size_t idx;

                // First, restore visited_ and lastGood_ attributes
                for (idx = 0; rc->aID_; idx++) {
                    // Restore all state machine data from the retry...
                    rc->visited_ =
                        regex_->retries_.back().aData_[idx].visited_;
                    rc->lastGood_ =
                        regex_->retries_.back().aData_[idx].lastGood_;

                    // Now, if the parent subexp has the same closing
                    // state as the current state { ==> (a(b(c)))d a, b,
                    // and c all have same closing state } then keep going;
                    // otherwise stop loop.
                    rc =
                        (rc->myAlpha_->myClose_ == rc->myClose_)
                        ? rc->myAlpha_
                        : regex_->states_()[regex_->numStates_ - 1];
                }

                // Now update matching information
                idx = 0;
                rc = regex_->retries_.back().alpha_;

                for (; rc->aID_; idx++) {
                    if (rc->visited_ == 1 && rc->alphasVisitedOnce())
                        (*regex_->matches_)()[rc->aID_] =
                            size_t (rc->lastGood_ - regex_->bos_);

                    else if (regex_->retries_.back().aData_[idx].match_ !=
                             size_t(-1))
                        (*regex_->matches_)()[rc->aID_] =
                            regex_->retries_.back().aData_[idx].match_;

                    if (regex_->retries_.back().aData_[idx].firstMatch_)
                        rc->firstMatch_ =
                            regex_->retries_.back().aData_[idx].firstMatch_;

                    else if (rc->opt_ || rc->visited_ == 1) {
                        rc->firstMatch_ = c;
                    }

                    // Now, if the parent subexp has the same closing state
                    // as the current state { ==> (a(b(c)))d a, b, and c all
                    // have same closing state } then keep going; otherwise
                    // stop loop.

                    if (rc->myAlpha_->myClose_ == rc->myClose_) {
                        rc =  rc->myAlpha_;
                    }
                    else {
                        rc = regex_->states_()[regex_->numStates_ - 1];
                    }
                }
            }

            // Now get the actual retry state
            rc = regex_->retries_.back().state_;
            regex_->retries_.pop_back();
        }
        else {
            // Otherwise - there are no retries so unmark this visit
            // and use visit count to determine next state

            visited_--;
            c = lastGood_;

            // Check for anchoring conditions
            if (!anchorEnd_ || c == regex_->eos_) {

                if (visited_ >= minVisits_) {

                    // If we've not already got a first match set one,
                    // because we just matched

                    if (!firstMatch_)
                        (*regex_->matches_)()[aID_] =
                            size_t (lastGood_ - regex_->bos_);

                    rc = failed_;
                    if (rc == myAlpha_ && rc->visited_ >= rc->minVisits_) {
                        rc->matched_ = true;
                    }

                }
                else {
                    // Otherwise subexp failed, so fail the parent (if there
                    // is one) and go there (if there is one).  If no parent,
                    // then alpha transition is bad end state.
                    myAlpha_->fail_ = true;
                    rc = myAlpha_;
                }
            }
            else {
                // If anchored, and not at end, then end match
                rc = regex_->states_()[regex_->numStates_ - 1];
            }
        }
    }
    else {
        // If subexpression states are not already initialized (not coming
        // in anew), then initialize them for a new subexpression match
        if (visited_ != 0) {
            for (size_t i = myNext_; i < myClose_; i++) {
                regex_->states_()[i]->init();
            }
        }

        // Back out any alternation attempts (beta retries)
        if (betaIndex_ != size_t(-1)) {
            regex_->alternator_.setBetaIndex(
                regex_->states_()[myNext_]->betaID_, betaIndex_);

            betaIndex_ = size_t(-1);
        }

        if (visited_ && lastGood_ == c) {
            // Save this as the last good point for easy recovery
            // in subexpressions

            // Save match information for 0-length matches

            if (!firstMatch_)
                (*regex_->matches_)()[aID_] =
                    size_t (lastGood_ - regex_->bos_);

            // Set rc based on whether we've matched and whether the failed
            // transition is the good end state - don't go to good end state
            // if there is no match

            if (matched_) {
                if (end_ &&
                        myAlpha_ != regex_->states_()[regex_->numStates_ - 1]) {
                    myAlpha_->matched_ = true;
                }

                rc = failed_;
            }
            else {
                if (failed_ != regex_->states_()[regex_->numStates_ - 2]) {
                    rc = failed_;
                }
                else {
                    rc = regex_->states_()[regex_->numStates_ - 1];
                }
            }
        }
        else {
            // Check for anchoring conditions
            if (!anchorEnd_ || rep_ || visited_ == 0 || c == regex_->eos_) {

                if (visited_ == 1 &&
                        (alphasVisitedOnce() || firstMatch_ == 0)) {

                    (*regex_->matches_)()[aID_] =
                        size_t (lastGood_ - regex_->bos_);

                    firstMatch_ = c;

                    RWRegexState<E>* cur = myAlpha_;
                    for (; cur->aID_ && myClose_ == cur->myClose_;
                            cur = cur->myAlpha_) {
                        if (cur->visited_ == 1) {
                            cur->firstMatch_ = c;
                        }
                    }
                }

                lastGood_ = c;
                matched_ = visited_ >= minVisits_;
                visited_++;

                if (visited_ > maxVisits_) {
                    if (!firstMatch_)
                        (*regex_->matches_)()[aID_] =
                            size_t (lastGood_ - regex_->bos_);

                    rc = failed_;
                }
                else {
                    rc = primary_;
                }

            }
            // Else bail out now - the non-anchoring has ended the match
            else {
                rc = regex_->states_()[regex_->numStates_ - 1];
            }

        }
    }

    return rc;
}

/////////////////////////////////////////////////////////////////////
//
// Test all alpha states to ensure that they've been visited only
// once.
//
/////////////////////////////////////////////////////////////////////
template <class E>
bool RWRegexState<E>::alphasVisitedOnce()
{
    bool        rc = true;
    RWRegexState<E>* s  = this;

    for (; rc && s->myAlpha_->aID_; s = s->myAlpha_) {
        // Say we're okay if we've been visited once, or if the alpha state precedes a beta state,
        // in which case all bets are off
        rc = s->myAlpha_->visited_ == 1 || regex_->states_()[s->myAlpha_->myID_ + 1]->betaID_ != size_t(-1);
    }

    return rc;
}
