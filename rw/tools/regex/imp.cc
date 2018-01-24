/**********************************************************************
 *
 * imp.cc - contains the implementation for the NFA regex impl
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regex/imp.cc#1 $
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

#include <rw/tools/string.h>
#include <rw/tools/regex/state.h>
#include <rw/tools/regex/lexicon.h>
#include <rw/tools/regex/regexresultimp.h>

/*****************************************************************************/
/*                                                                           */
/* The following items are defined as macros except if one needs to debug    */
/* the retries information storage.  Although this is a bit grotesque,       */
/* the use of these macros does improve performance on some nameless         */
/* compilers, where not all template functions, or even all template         */
/* functions that are declared to be inline, are actually inlined.  This     */
/* guarantees that these are "inlined" by the preprocessor, and still        */
/* provides some visual separation of the logic.                             */
/*                                                                           */
/* This is done because these blocks are invoked from within a rather        */
/* tight loop in the execution of the engine where performance is critical.  */
/* Actual profiling data shows that this improved performance by roughly    */
/* 18% in release builds on one platform.                                    */
/*                                                                           */
/*****************************************************************************/

#if defined(RW_DEBUG_REGEX_RETRIES)

#  define RW_REGEX_SAVE_ALPHA_RETRIES(cur, c) saveAlphaRetries(cur, c);

template<class E>
void RWRegexImp<E>::saveAlphaRetries(RWRegexState<E>*& cur, const RChar* c)
{
    if (cur->rep_ && cur->sub_ &&
            cur->visited_ >= cur->minVisits_ &&
            !cur->fail_ && cur->lookahead_ == cur->myAlpha_->lookahead_ &&
            cur->myAlpha_->peek(c) && canMatchAnother(cur)) {

        bool go = true;

        RWRegexState<E>* lookahead = cur->lookahead_;

        RWRegVec<RWRegexState<E>* > rStack;

        for (RWRegexState<E>* si = cur;
                go && si->myAlpha_->aID_ && si->myAlpha_->lookahead_ == lookahead;
                si = si->myAlpha_) {
            if (si->visited_ >= si->minVisits_) {
                rStack.push_back(si->myAlpha_);
            }
            else {
                go = false;
            }
        }

        for (; rStack.size(); rStack.pop_back()) {
            RWRegexRetry<E> retry(rStack.back(), size_t(-1), false,
                                  rStack.back(), rStack.back()->lastGood_, c);

            RWRegexState<E>* curState = rStack.back();
            while (curState->aID_) {
                retry.aData_.push_back(RWRegexAlphaData<E>(curState));
                curState = curState->myAlpha_;
            }

            if (!findRetry(retry)) {
                retries_.push_back(retry);
                retriesRec_.push_back(retry);
            }
        }
    }
}

#else

#  define RW_REGEX_SAVE_ALPHA_RETRIES(cur, c)                             \
{                                                                       \
    if (cur->rep_ && cur->sub_ && cur->visited_ >= cur->minVisits_ &&   \
        !cur->fail_ && cur->lookahead_ == cur->myAlpha_->lookahead_ &&  \
        cur->myAlpha_->peek(c) &&                                       \
        canMatchAnother(cur)) {                                         \
        bool go = true;                                            \
        RWRegexState<E>* lookahead = cur->lookahead_;                   \
        RWRegVec<RWRegexState<E>* > rStack;                             \
                                                                        \
        for (RWRegexState<E>* si = cur;                                 \
             go && si->myAlpha_->aID_ &&                                \
             si->myAlpha_->lookahead_ == lookahead;                     \
             si = si->myAlpha_) {                                       \
            if (si->visited_ >= si->minVisits_) {                       \
                rStack.push_back(si->myAlpha_);                         \
            }                                                           \
            else {                                                      \
                go = false;                                             \
            }                                                           \
        }                                                               \
                                                                        \
        for (; rStack.size(); rStack.pop_back()) {                      \
            RWRegexRetry<E> retry(                                      \
                rStack.back(), size_t(-1), false,                       \
                rStack.back(), rStack.back()->lastGood_, c);            \
                                                                        \
            RWRegexState<E>* curState = rStack.back();                  \
            while (curState->aID_) {                                    \
                retry.aData_.push_back(RWRegexAlphaData<E>(curState));  \
                curState = curState->myAlpha_;                          \
            }                                                           \
                                                                        \
            if (!findRetry(retry)) {                                    \
                retries_.push_back(retry);                              \
                retriesRec_.push_back(retry);                           \
            }                                                           \
        }                                                               \
    }                                                                   \
}

#endif



#if defined(RW_DEBUG_REGEX_RETRIES)

#  define RW_REGEX_SAVE_BETA_RETRIES(cur, c) saveBetaRetries(cur, c);

template<class E>
void RWRegexImp<E>::saveBetaRetries(RWRegexState<E>*& cur, const RChar* c)
{
    size_t beta;
    bool bGood = false;

    /* If the current state is a beta state then */
    /* operate on this beta state */

    if (cur->betaID_ != size_t(-1)) {
        beta = cur->betaID_;
    }

    /* Or, if the current state is a repeated state */
    /* that is the first state in a beta transition, */
    /* and we are in a sub-expression */

    else if (cur->rep_ && cur->fib_ &&
             cur->myAlpha_->aID_ && cur->peek(c)) {
        beta = cur->myAlpha_->primary_->betaID_;
        bGood = cur->matched_;

        /* Be a pal, and save the would-be match as */
        /* part of the beta retry */

        if ((*matches_)()[cur->myAlpha_->aID_] == size_t(-1))
            (*matches_)()[cur->myAlpha_->aID_] =
                size_t (cur->myAlpha_->lastGood_ - bos_);
    }
    else {
        beta = size_t(-1);
    }

    /* If we have a beta state to operate upon, and we aren't */
    /* currently working on a saved retry for this beta and this */
    /* character position */

    if (beta != size_t(-1) && (beta != rBeta_ || c != rBetaC_)) {
        bool matchedOne = false;
        size_t transitions = alternator_.transitionsAt(beta);
        size_t curSetting;
        RWRegexState<E>* curTrans;

        /* If this is a retry for a repeated element in a */
        /* sub-expression, ensure cur remains the repeated */
        /* element */

        if (bGood) {
            matchedOne = true;
        }

        /* Save all potential retries*/
        for (size_t k = 0; k < transitions; k++) {
            curSetting = (alternator_.curIndexFor(beta) + k) % transitions;
            curTrans = alternator_.transitionAt(beta, curSetting);

            /* If the current transition is optional or if it */
            /* matches the current character position, then save */
            /* a retry for it */

            if (curTrans->opt_ || curTrans->peek(c)) {
                if (!matchedOne) {
                    cur = alternator_.transitionAt(beta, curSetting);
                    matchedOne = true;
                }
                else {
                    RWRegexState<E>* trans =
                        alternator_.transitionAt(beta, curSetting);
                    RWRegexRetry<E> retry(cur->myAlpha_, beta, bGood,
                                          trans, trans->lastGood_, c);

                    if (!findRetry(retry)) {
                        retries_.push_back(retry);
                        retriesRec_.push_back(retry);
                        RWRegexState<E>* cs = cur;
                        while (cs->myAlpha_->myID_ != numStates_ - 1) {
                            retries_.back().aData_.push_back(
                                RWRegexAlphaData<E>(cs->myAlpha_));
                            cs = cs->myAlpha_;
                        }
                    }
                }
            }
        }
    }
}

#else

#  define RW_REGEX_SAVE_BETA_RETRIES(cur, c)                              \
{                                                                       \
    size_t beta;                                                        \
    bool bGood = false;                                            \
                                                                        \
    /* If the current state is a beta state then */                     \
    /* operate on this beta state */                                    \
    if (cur->betaID_ != size_t(-1))                                     \
    {                                                                   \
        beta = cur->betaID_;                                            \
    }                                                                   \
    /* Or, if the current state is a repeated state */                  \
    /* that is the first state in a beta transition, */                 \
    /* and we are in a sub-expression */                                \
                                                                        \
    else if (cur->rep_           && cur->fib_ &&                        \
             cur->myAlpha_->aID_ && cur->peek(c))                       \
    {                                                                   \
        beta = cur->myAlpha_->primary_->betaID_;                        \
        bGood = cur->matched_;                                          \
                                                                        \
        /* Be a pal, and save the would-be match as */                  \
        /* part of the beta retry */                                    \
                                                                        \
        if ((*matches_)()[cur->myAlpha_->aID_] == size_t(-1))           \
            (*matches_)()[cur->myAlpha_->aID_] =                        \
                size_t (cur->myAlpha_->lastGood_ - bos_);               \
    }                                                                   \
    else                                                                \
    {                                                                   \
        beta = size_t(-1);                                              \
    }                                                                   \
                                                                        \
    /* If we have a beta state to operate upon, and we aren't */        \
    /* currently working on a saved retry for this beta and this */     \
    /* character position */                                            \
    if (beta != size_t(-1) && (beta != rBeta_ || c != rBetaC_))         \
    {                                                                   \
        bool matchedOne = false;                                   \
        size_t transitions = alternator_.transitionsAt(beta);           \
        size_t curSetting;                                              \
        RWRegexState<E>* curTrans;                                      \
                                                                        \
        /* If this is a retry for a repeated element in a */            \
        /* sub-expression, ensure cur remains the repeated */           \
        /* element */                                                   \
                                                                        \
        if (bGood) matchedOne = true;                                   \
                                                                        \
        /* Save all potential retries*/                                 \
        for (size_t k = 0; k < transitions; ++k)                        \
        {                                                               \
            curSetting =                                                \
                (alternator_.curIndexFor(beta) + k) % transitions;      \
            curTrans = alternator_.transitionAt(beta, curSetting);      \
                                                                        \
            /* If the current transition is optional or if it */        \
            /* matches the current character position, then save */     \
            /* a retry for it */                                        \
                                                                        \
            if (curTrans->opt_ || curTrans->peek(c))                    \
            {                                                           \
                if (!matchedOne)                                        \
                {                                                       \
                    cur = alternator_.transitionAt(beta, curSetting);   \
                    matchedOne = true;                                  \
                }                                                       \
                else                                                    \
                {                                                       \
                    RWRegexState<E>* trans =                            \
                        alternator_.transitionAt(beta, curSetting);     \
                    RWRegexRetry<E> retry(                              \
                        cur->myAlpha_, beta, bGood,                     \
                        trans, trans->lastGood_, c);                    \
                                                                        \
                    if (!findRetry(retry))                              \
                    {                                                   \
                        retries_.push_back(retry);                      \
                        retriesRec_.push_back(retry);                   \
                        RWRegexState<E>* cs = cur;                      \
                        while (cs->myAlpha_->myID_ != numStates_ - 1)   \
                        {                                               \
                            retries_.back().aData_.push_back(           \
                                RWRegexAlphaData<E>(cs->myAlpha_));     \
                            cs = cs->myAlpha_;                          \
                        }                                               \
                    }                                                   \
                }                                                       \
            }                                                           \
        }                                                               \
    }                                                                   \
}

#endif



#if defined(RW_DEBUG_REGEX_RETRIES)

#  define RW_REGEX_SAVE_LOOKAHEAD_RETRIES(cur, c) saveLookaheadRetries(cur, c);

template<class E>
void RWRegexImp<E>::saveLookaheadRetries(RWRegexState<E>*& cur, const RChar* c)
{
    if (cur->matched_ && !cur->fail_ &&
            ((cur->peek(c) && cur->lookahead_->peek(c)) ||
             cur->lookahead_->opt_) && alphasMatched(cur)) {

        RWRegexState<E>* ptr = cur->aID_ && cur->opt_ ? cur : cur->myAlpha_;

        RWRegexRetry<E> retry(ptr, size_t(-1), false,
                              cur->lookahead_, cur->lookahead_->lastGood_, c);

        if (!findRetry(retry)) {
            retries_.push_back(retry);
            retriesRec_.push_back(retry);

            RWRegexState<E>* curState = ptr;

            while (curState->aID_) {
                if (curState->opt_ && !curState->firstMatch_) {
                    (*matches_)()[curState->aID_] = size_t (c - bos_);
                }

                retries_.back().aData_.push_back(
                    RWRegexAlphaData<E>(curState));

                curState = curState->myAlpha_;
            }
        }
    }
}

#else

#  define RW_REGEX_SAVE_LOOKAHEAD_RETRIES(cur, c)                         \
{                                                                       \
    if (cur->matched_ && !cur->fail_ &&                                 \
        ((cur->peek (c) && cur->lookahead_->peek(c)) ||                 \
         cur->lookahead_->opt_) && alphasMatched(cur)) {                \
        RWRegexState<E>* ptr =                                          \
            cur->aID_ && cur->opt_ ? cur : cur->myAlpha_;               \
                                                                        \
        RWRegexRetry<E> retry(                                          \
            ptr, size_t(-1), false,                                     \
            cur->lookahead_, cur->lookahead_->lastGood_, c);            \
                                                                        \
        if (!findRetry(retry)) {                                        \
            retries_.push_back(retry);                                  \
            retriesRec_.push_back(retry);                               \
                                                                        \
            RWRegexState<E>* curState = ptr;                            \
                                                                        \
            while (curState->aID_) {                                    \
                if (curState->opt_ && !curState->firstMatch_)           \
                    (*matches_)()[curState->aID_] = size_t (c - bos_);  \
                                                                        \
                retries_.back().aData_.push_back(                       \
                    RWRegexAlphaData<E>(curState));                     \
                                                                        \
                curState = curState->myAlpha_;                          \
            }                                                           \
        }                                                               \
    }                                                                   \
}

#endif



template <class E>
RWRegexImp<E>::RWRegexImp()
    : patternLength_(0),
      numStates_(0),
      invalid_(true),
      alphaCount_(0),
      matches_(0),
      lengths_(0),
      bos_(0),
      eos_(0),
      off_(0),
      status_(new RWRegexErr("No Error", 0, 0))
{
}

template <class E>
RWRegexImp<E>::RWRegexImp(const RChar* str, size_t length)
    : patternLength_(length),
      numStates_(0),
      invalid_(false),
      alphaCount_(0),
      matches_(0),
      lengths_(0),
      bos_(0),
      eos_(0),
      off_(0),
      status_(new RWRegexErr("No Error", 0, 0))
{
    // Set the actual pattern length
    if (patternLength_ == size_t(-1)) {
        patternLength_ = strLen(str);
    }

    // Allocate and copy the pattern
    pattern_.set(patternLength_ + 1);

    for (size_t i = 0; i < patternLength_; i++) {
        pattern_()[i] = str[i];
    }

    pattern_()[patternLength_] = RWTRegexTraits<E>::null();

    // Set the regex pointer for the compiler_
    compiler_.setRegex(this);

    // Compile the expression
    try {
        compiler_.compile();
    }
    catch (RWRegexErr&) {
        clear();
        throw;
    }
}

template <class E>
RWRegexImp<E>::RWRegexImp(const RWRegexImp<E>& source) :
    numStates_(0),
    matches_(0),
    lengths_(0),
    bos_(0),
    eos_(0),
    off_(0),
    status_(0)
{
    doCopy(source);
}

template <class E>
RWRegexImp<E>::~RWRegexImp()
{
    clear();
}

template <class E>
RWRegexImp<E>& RWRegexImp<E>::operator= (const RWRegexImp<E>& rhs)
{
    doCopy(rhs);

    return *this;
}

template <class E>
bool RWRegexImp<E>::operator< (const RWRegexImp<E>& rhs) const
{
    bool rc = true;
    size_t i;

    for (i = 0; rc && i < patternLength_ && i < rhs.patternLength_; i++) {
        if (RWTRegexTraits<E>::lt(pattern_()[i], rhs.pattern_()[i])) {
            break;
        }
        else if (!RWTRegexTraits<E>::eq(pattern_()[i], rhs.pattern_()[i])) {
            rc = false;
        }
    }

    if (rc && i == rhs.patternLength_ && i <= patternLength_) {
        rc = false;
    }

    return rc;
}

template <class E>
bool RWRegexImp<E>::operator==(const RWRegexImp<E>& rhs) const
{
    bool rc = (patternLength_ == rhs.patternLength_);

    for (size_t i = 0; rc && i < patternLength_; i++) {
        rc = RWTRegexTraits<E>::eq(pattern_()[i], rhs.pattern_()[i]);
    }

    return rc;
}

template <class E>
size_t RWRegexImp<E>::subCount() const
{
    return alphaCount_;
}

template <class E>
RWTRegexResult<E> RWRegexImp<E>::search(
    const RChar* str, size_t start, size_t length)
{
    RWTRegexResult<E> result;
    bool rc = false;

    // Set matches and lengths to reference result buffers
    matches_ = &result.imp_->starts_;
    lengths_ = &result.imp_->lengths_;
    (*matches_).set(alphaCount_ + 1);
    (*lengths_).set(alphaCount_ + 1);

    // Initialize match info
    initMatchesFrom();

    if (start <= length) {
        size_t i;

        // Save beginning of string
        bos_ = str;
        off_ = start;

        // If regex is not invalid...
        if (!invalid_)
            // Repeatedly doMatch() until a match is found
            for (i = start; !rc && i <= length;
                    i += nextOffset(str + i, length - i)) {
                alphaRetry_ = false;
                rc = doMatch(str + i, length - i);
            }
    }

    return result;
}

template <class E>
RWTRegexResult<E> RWRegexImp<E>::matchAt(
    const RChar* str, size_t start, size_t length)
{
    RWTRegexResult<E> result;

    // Set matches and lengths to reference result buffers
    matches_ = &result.imp_->starts_;
    lengths_ = &result.imp_->lengths_;

    (*matches_).set(alphaCount_ + 1);
    (*lengths_).set(alphaCount_ + 1);

    // Initialize match info
    initMatchesFrom();

    if (start <= length) {
        // Save beginning of string
        bos_ = str;
        off_ = start;

        // Mark that we are not in the midst of an alpha retry
        alphaRetry_ = false;

        if (!invalid_) {
            doMatch(str + start, length - start);
        }

    }

    return result;
}

template <class E>
const RWRegexErr& RWRegexImp<E>::getStatus() const
{
    return *status_;
}

template<class E>
size_t RWRegexImp<E>::strLen(const RChar* str) const
{
    size_t rc;

    for (rc = 0;
            !RWTRegexTraits<E>::eq(str[rc], RWTRegexTraits<E>::null());
            rc++) { }

    return rc;
}



/*****************************************************************************/
/*                                                                           */
/* RWRegexImp private interface                                              */
/*                                                                           */
/*****************************************************************************/

template <class E>
void RWRegexImp<E>::doCopy(const RWRegexImp& source)
{
    // Guard against self-assignment
    if (this == &source) {
        return;
    }

    size_t i;

    // Clear currently allocated heap memory
    clear();

    // Copy over the easy stuff - lengths, flags, etc
    patternLength_ = source.patternLength_;
    numStates_ = source.numStates_;
    invalid_ = source.invalid_;
    alphaCount_ = source.alphaCount_;
    bos_ = source.bos_;
    eos_ = source.eos_;
    off_ = source.off_;
    rBetaC_ = source.rBetaC_;
    rBeta_ = source.rBeta_;
    alphaRetry_ = source.alphaRetry_;
    status_.reset(new RWRegexErr(*source.status_));

    // If we've got a pattern buf, assume good regex and continue
    if (source.pattern_()) {
        // Allocate and copy the pattern_
        pattern_.set(patternLength_ + 1);
        for (i = 0; i < patternLength_; i++) {
            pattern_()[i] = source.pattern_()[i];
        }

        pattern_()[patternLength_] = RWTRegexTraits<E>::null();

        // Allocate states buffer
        states_.set(numStates_);

        // Copy all states
        for (i = 0; i < numStates_; i++) {
            states_()[i] = source.states_()[i]->clone();
        }

        // Assign state transitions to point to local regex states_
        for (i = 0; i < numStates_; i++) {
            states_()[i]->regex_ = this;

            states_()[i]->lex_ =
                states_()[i]->lex_ ? states_()[i]->lex_->clone(this) : 0;

            states_()[i]->lastGood_ = 0;

            states_()[i]->myAlpha_ =
                states_()[source.states_()[i]->myAlpha_->myID_];

            states_()[i]->primary_ =
                states_()[source.states_()[i]->primary_->myID_];

            states_()[i]->failed_ =
                states_()[source.states_()[i]->failed_->myID_];

            states_()[i]->lookahead_ =
                states_()[source.states_()[i]->lookahead_->myID_];
        }

        // Set up the beta table/alternator
        alternator_.copyFrom(source.alternator_, states_());

        // Set this regex for the compiler_
        compiler_.setRegex(this);

        // Allocate match information buffers
        savedMatches_.set(alphaCount_ + 1);
        savedLengths_.set(alphaCount_ + 1);
    }
}

template <class E>
void RWRegexImp<E>::setStatus(const RWRegexErr& newStatus)
{
    status_.reset(new RWRegexErr(newStatus));
}

template <class E>
void RWRegexImp<E>::clear()
{
    pattern_.clear();

    for (size_t i = 0; i < numStates_; i++) {
        if (states_()[i]) {
            if (states_()[i]->lex_) {
                delete states_()[i]->lex_;
            }

            delete states_()[i];
        }
    }

    states_.clear();

    savedMatches_.clear();
    savedLengths_.clear();

    status_.reset(0);
}

template <class E>
void RWRegexImp<E>::addState(RWRegexState<E>* s)
{
    if (s->alpha_) {
        s->aID_ = ++alphaCount_;
    }

    states_()[numStates_++] = s;
}

template <class E>
void RWRegexImp<E>::initMatchesFrom(size_t index)
{
    size_t limit = alphaCount_ + 1;
    for (size_t i = index; i < limit; i++) {
        (*matches_)()[i] = size_t(-1);
        (*lengths_)()[i] = 0;
    }
}

template <class E>
bool RWRegexImp<E>::doMatch(const RChar* str, size_t length)
{
    bool rc = false;

    if (pattern_() != 0) {
        const RChar* temp;
        RWRegexState<E>* state;
        const RChar* start;
        const RChar* end = 0;
        size_t mLen = 0;

        // Set bos_ and eos_
        eos_ = str + length;

        // Initialize rBeta_
        rBeta_ = size_t(-1);
        rBetaC_ = 0;

        // Initialize alternator
        alternator_.init();

        // Try to match repeatedly, trying all alternation permutations
        do {
            temp = str;
            state = states_()[0];
            start = temp;

            // Match Once and if it matches, then save the length of
            // the match
            end = matchOnce(state, start);
            mLen = size_t (end - temp);

            // If we matched, and we're either not anchored at the end
            // OR we are and the match ends at the end of the string...
            if (end) {
                // Save the start of the match and, if length beats
                // current length, save length and all starting
                // positions
                (*matches_)()[0] = size_t (temp - bos_);

                if (mLen > (*lengths_)()[0]) {
                    (*lengths_)()[0] = mLen;
                    calcLengthsAndSave();
                    rc = true;
                }

                // If we haven't already saved starting positions, then
                // do so now
                if (!rc) {
                    calcLengthsAndSave();
                }

                // Ensure rc is true
                rc = true;
            }

            // Exercise all potential retries if needed
            while (!end && retries_.size()) {
                start = popRetry(state);

                end = matchOnce(state, start);
                mLen = size_t (end - temp);

                // If we have a newer, better match, save the starting
                // locations and overall length
                if (end && mLen > (*lengths_)()[0]) {
                    rc = true;

                    (*matches_)()[0] = size_t (temp - bos_);
                    (*lengths_)()[0] = mLen;

                    calcLengthsAndSave();
                }
            }

            retries_.clear();
            retriesRec_.clear();
            initMatchesFrom(1);

        }
        while ((*lengths_)()[0] < length && alternator_.advance());
    }

    // Copy back the best-match overall and sub-expression match information
    if (rc) {
        size_t size = (alphaCount_ + 1) * sizeof(size_t);
        rw_memcpy_s((*matches_)(), matches_->size() * sizeof(size_t), savedMatches_(), size);
        rw_memcpy_s((*lengths_)(), lengths_->size() * sizeof(size_t), savedLengths_(), size);
    }

    return rc;
}

template <class E>
const typename RWTRegexTraits<E>::Char*
RWRegexImp<E>::matchOnce(RWRegexState<E>* s, const RChar*& c)
{
    const RChar* rc = 0;
    RWRegexState<E>* cur = s;
    size_t i;

    // Initialize all states, and reset them
    size_t size = numStates_ - 2;
    for (i = alphaRetry_ ? cur->myID_ + 1 : cur->myID_; i < size; i++) {
        states_()[i]->init(true);
    }

    // Run the state machine
    while (cur->myID_ < size) {
        // Save any retries that may exist due to currently matched
        // that could result in a matched sub-expression
        RW_REGEX_SAVE_ALPHA_RETRIES(cur, c)

        // Save any retries that may exist due to alternation
        RW_REGEX_SAVE_BETA_RETRIES(cur, c)

        // Save any retries that may exist due to lookahead states_
        RW_REGEX_SAVE_LOOKAHEAD_RETRIES(cur, c)

        // Match against the current state
        cur = cur->doMatch(c);
    }

    // Second to last state is the good end state, size - 1 is the
    // bad transition state.
    if (cur == states_()[numStates_ - 2]) {
        rc = c;
    }

    return rc;
}

template<class E>
void RWRegexImp<E>::calcLengthsAndSave()
{
    for (size_t i = 0; i < numStates_ - 2; i++) {
        if (states_()[i]->firstMatch_ &&
                (*matches_)()[states_()[i]->aID_] != size_t(-1)) {
            (*lengths_)()[states_()[i]->aID_] =
                size_t (states_()[i]->firstMatch_ -
                        (bos_ + (*matches_)()[states_()[i]->aID_]));
        }
    }

    // Save match start positions and lengths
    size_t size = (alphaCount_ + 1) * sizeof(size_t);
    rw_memcpy_s(savedMatches_(), savedMatches_.size() * sizeof(size_t), (*matches_)(), size);
    rw_memcpy_s(savedLengths_(), savedLengths_.size() * sizeof(size_t), (*lengths_)(), size);
}

template <class E>
bool RWRegexImp<E>::alphasMatched(RWRegexState<E>* cur)
{
    bool rc = true;
    RWRegexState<E>* lookahead = cur->lookahead_;

    for (cur = cur->myAlpha_;
            rc && cur->aID_ && cur->lookahead_ == lookahead;
            cur = cur->myAlpha_) {
        if (cur->visited_ < cur->minVisits_) {
            rc = false;
        }
    }

    return rc;
}

template <class E>
const typename RWRegexImp<E>::RChar*
RWRegexImp<E>::popRetry(RWRegexState<E>*& state)
{
    const RChar* rc = 0;

    // Grab the retry state's alpha state
    RWRegexState<E>* alphaptr = state = retries_.back().alpha_;

    // Set the alphaRetry_ flag based on whether the alpha and
    // beginning state are the same
    alphaRetry_ = alphaptr == retries_.back().state_;

    // Save the beta ID of the retry state (also tells that this is a
    // beta retry if rBeta_ is not size_t(-1))
    rBeta_ = retries_.back().beta_;
    rc = rBetaC_ = retries_.back().start_;

    // Restore all alpha state information for all alphas of the retry's alpha
    if (retries_.back().state_->myAlpha_ == alphaptr) {
        for (size_t idx = 0; alphaptr->aID_; idx++) {
            alphaptr->visited_ = retries_.back().aData_[idx].visited_;
            alphaptr->lastGood_ = retries_.back().aData_[idx].lastGood_;

            if (retries_.back().aData_[idx].match_ != size_t(-1))
                (*matches_)()[alphaptr->aID_] =
                    retries_.back().aData_[idx].match_;

            if (retries_.back().aData_[idx].firstMatch_)
                alphaptr->firstMatch_ =
                    retries_.back().aData_[idx].firstMatch_;

            alphaptr = alphaptr->myAlpha_;
        }
    }

    // If this was a "Beta Good" retry, then update the visited flag and
    // update the first match for the sub-expression
    if (retries_.back().bGood_) {
        state->visited_++;
        state->firstMatch_ = rBetaC_;
    }

    // For as long as the state has an alpha state and the alpha state's
    // closing state is the same as the original alpha's closing state, update
    // the first match tag in those alpha states_.
    while (state->aID_) {
        if (state->firstMatch_ > rc) {
            state->firstMatch_ = rc;
        }
        state = state->myAlpha_->myClose_ ==
                state->myClose_
                ? state->myAlpha_
                : states_()[numStates_ - 1];
    }

    // Now get the actual state to begin retrying at
    state = retries_.back().state_;
    state->lastGood_ = retries_.back().lGood_;

    retries_.pop_back();

    return rc;
}

template<class E>
bool RWRegexImp<E>::findRetry(const RWRegexRetry<E>& retry)
{
    bool rc = false;
    size_t size = retriesRec_.size();

    // Search to see if a given retry is in the retries record
    for (size_t i = size - 1; !rc && i != size_t(-1); i--) {
        if (retriesRec_[i] == retry) {
            rc = true;
        }
    }

    return rc;
}

template<class E>
bool RWRegexImp<E>::canMatchAnother(RWRegexState<E>* cur)
{
    bool rc = cur->visited_ < cur->maxVisits_;
    RWRegexState<E>* lookahead = cur->lookahead_;

    // Verify that all parent sub-expression cardinalities permit
    // an additional match
    for (cur = cur->myAlpha_;
            rc && cur->aID_ && cur->lookahead_ == lookahead;
            cur = cur->myAlpha_) {
        if (cur->visited_ >= cur->maxVisits_) {
            rc = false;
        }
    }

    return rc;
}

template<class E>
size_t RWRegexImp<E>::nextOffset(const RChar*, size_t)
{
    return 1;
}



template <class E>
RWRegexMatchIteratorImp<E>::RWRegexMatchIteratorImp()
    : regex_(0)
    , str_(0)
    , start_(0)
    , end_(0)
    , overlap_(false)
{
}

template <class E>
RWRegexMatchIteratorImp<E>::RWRegexMatchIteratorImp(const RWRegexMatchIteratorImp<E>& source)
    : regex_(source.regex_)
    , str_(source.str_)
    , start_(source.start_)
    , end_(source.end_)
    , overlap_(source.overlap_)
    , result_(source.result_)
{
}

template <class E>
RWRegexMatchIteratorImp<E>::RWRegexMatchIteratorImp(RWTRegex<E>& newRegex,
        const RChar* newString,
        size_t newStart,
        size_t newLength,
        bool newOverlap)
    : regex_(&newRegex)
    , str_(newString)
    , start_(newStart)
    , end_(start_ + newLength)
    , overlap_(newOverlap)
{
    if (end_ == size_t (-1))
        for (end_ = start_;
                !RWTRegexTraits<E>::eq(str_[end_], RWTRegexTraits<E>::null());
                end_++) { }

    advance();
}

template <class E>
RWRegexMatchIteratorImp<E>::~RWRegexMatchIteratorImp()
{
}

template <class E>
RWRegexMatchIteratorImp<E>&
RWRegexMatchIteratorImp<E>::operator=(const RWRegexMatchIteratorImp<E>& source)
{
    if (&source != this) {
        regex_ = source.regex_;
        str_ = source.str_;
        start_ = source.start_;
        end_ = source.end_;
        overlap_ = source.overlap_;
        result_ = source.result_;
    }
    return *this;
}

template <class E>
RWRegexMatchIteratorImp<E>&
RWRegexMatchIteratorImp<E>::operator++()
{
    advance();
    return *this;
}

template <class E>
const RWRegexMatchIteratorImp<E>
RWRegexMatchIteratorImp<E>::operator++(int)
{
    RWRegexMatchIteratorImp<E> rc = *this;
    advance();
    return rc;
}

template <class E>
RWRegexMatchIteratorImp<E>&
RWRegexMatchIteratorImp<E>::operator+=(size_t count)
{
    advance(count);
    return *this;
}

template <class E>
RWTRegexResult<E>
RWRegexMatchIteratorImp<E>::operator*() const
{
    return result_;
}

template <class E>
const RWTRegexResult<E>*
RWRegexMatchIteratorImp<E>::operator->() const
{
    return &result_;
}

template <class E>
bool
RWRegexMatchIteratorImp<E>::operator==(
    const RWRegexMatchIteratorImp<E>& ref) const
{
    return (regex_ == ref.regex_ && start_ == ref.start_);
}

template <class E>
void RWRegexMatchIteratorImp<E>::advance(size_t count)
{
    for (size_t i = 0; regex_ && i < count; i++) {
        if ((start_ == 0 || start_ < end_) &&
                (result_ = regex_->search(str_, start_, end_))) {
            start_ = (result_.getLength() && !overlap_)
                     ? result_.getStart() + result_.getLength()
                     : result_.getStart() + 1;
        }
        else {
            regex_ = 0;
            str_ = 0;
            start_ = 0;
            end_ = 0;
            overlap_ = false;
        }
    }
}
