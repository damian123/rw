

/**********************************************************************
 *
 * alt.cc - contains the implementation of the NFA alternation
 *          policy.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regex/alt.cc#1 $
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




/////////////////////////////////////////////////////////////////////
//
// Default constructor
//
/////////////////////////////////////////////////////////////////////
template <class E>
RWRegexAlternator<E>::RWRegexAlternator() :
    curBeta_(0)
{
}

/////////////////////////////////////////////////////////////////////
//
// Destructor
//
/////////////////////////////////////////////////////////////////////
template <class E>
RWRegexAlternator<E>::~RWRegexAlternator()
{
}

/////////////////////////////////////////////////////////////////////
//
// Utility method for copying an alternator from another alternator
//
/////////////////////////////////////////////////////////////////////
template <class E>
void RWRegexAlternator<E>::copyFrom(const RWRegexAlternator<E>& source, RWRegexState<E>** states)
{
    size_t            i;
    size_t            j;
    size_t            sourceSize = source.data_.size();
    size_t            betaSize;
    RWRegexAltSetting dummy(sourceSize);

    // Copy the beta state configuration from an existing alternator
    for (i = 0; i < sourceSize; i++) {
        addBetaState(states[source.data_[i].beta_->myID_], dummy);

        betaSize = source.data_[i].transitions_.size();
        for (j = 0; j < betaSize; j++) {
            addBetaTransition(states[source.data_[i].transitions_[j]->myID_]);
        }
    }

    // Copy the alternation settings
    sourceSize = source.settings_.size();
    for (i = 0; i < sourceSize; i++) {
        settings_.push_back(source.settings_[i]);
    }
}

/////////////////////////////////////////////////////////////////////
//
// Adds a beta state to the alternator using an input alternator
// setting.  The setting is created in the compiler, and is of the
// correct size (number of beta/gamma states).  The setting for
// the new beta state is incremented.  This gives a valid beta
// setting for the beta state.  The index into the setting is the
// betaID, which counts each beta state from 1 to N, where N is
// the number of beta states.  Recall that all setting indices
// are initialized to -1, so the first increment sets the setting
// for the beta state to 0 index.
//
/////////////////////////////////////////////////////////////////////
template <class E>
void RWRegexAlternator<E>::addBetaState(RWRegexState<E>* state, RWRegexAltSetting& setting)
{
    betaStack_.push_back(curBeta_);
    curBeta_ = state->betaID_ = data_.size();
    data_.push_back(AltData(state));
    setting[curBeta_] = setting[curBeta_] + 1;
}

/////////////////////////////////////////////////////////////////////
//
// Adds a beta transition to the alternator.  This method is
// invoked when the first state off a beta state transition is
// encountered during pattern compilation.  Take a(b|c)d.  The
// beta state is added after the alpha state for '(' with
// "addBetaState".  Then, a transition is added for 'b', and for
// 'c'.  The 'b' transition corresponds to setting[0] set to 0.
// The 'c' transition corresponds to setting[0] set to 1, for two
// alt settings: the first with setting[0] = 0, and the second
// with setting[0] = 1.
//
/////////////////////////////////////////////////////////////////////
template <class E>
void RWRegexAlternator<E>::addBetaTransition(RWRegexState<E>* state)
{
    if (data_.size()) {
        data_[curBeta_].transitions_.push_back(state);
    }
    state->fib_ = true;
}

/////////////////////////////////////////////////////////////////////
//
// Records a new alternation setting that will have to be accounted
// for during matching.  Recall that when the beta state is added,
// we update the setting for the beta state to 0.  When we encounter
// the '|', we add the setting to record the setting of zero.  Then
// we update the setting by one for the next OR that we may come
// across.
//
/////////////////////////////////////////////////////////////////////
template <class E>
void RWRegexAlternator<E>::addOr(RWRegexAltSetting& setting)
{
    addSetting(setting);
    setting[curBeta_] = setting[curBeta_] + 1;
}

/////////////////////////////////////////////////////////////////////
//
// Records the gamma state (end of an alternation sequence).  The
// setting is added.  Then the setting for the beta is reset to
// -1, and we pop back to the beta state that we were working on
// before the current alternation expression was added (for nested
// alternation expressions).
//
/////////////////////////////////////////////////////////////////////
template <class E>
void RWRegexAlternator<E>::addGamma(RWRegexAltSetting& setting)
{
    addSetting(setting);
    setting[curBeta_] = size_t(-1);
    curBeta_ = betaStack_.back();
    betaStack_.pop_back();
}

/////////////////////////////////////////////////////////////////////
//
// Configures alternator with first stored alternation setting
//
/////////////////////////////////////////////////////////////////////
template <class E>
void RWRegexAlternator<E>::init()
{
    size_t i;
    size_t size = data_.size();

    // Initialize the current setting of the alternator
    curSetting_ = 0;

    // Set all beta transitions to those named in first setting
    for (i = 0; i < size; i++) {
        setBetaIndex(i, 0);
    }
}

/////////////////////////////////////////////////////////////////////
//
// Advances alternator to use settings named in next alternation
// setting, or if there are no more settings, then the method
// returns false.
//
/////////////////////////////////////////////////////////////////////
template <class E>
bool RWRegexAlternator<E>::advance()
{
    bool rc;

    curSetting_++;
    rc = curSetting_ < settings_.size();

    if (rc) {
        size_t limit = data_.size();
        for (size_t i = 0; i < limit; i++) {
            setBetaIndex(i, settings_[curSetting_][i]);
        }
    }

    return rc;
}

/////////////////////////////////////////////////////////////////////
//
// Empties the alternator
//
/////////////////////////////////////////////////////////////////////
template <class E>
void RWRegexAlternator<E>::clear()
{
    curBeta_ = 0;
    data_.clear();
    betaStack_.clear();
}

/////////////////////////////////////////////////////////////////////
//
// Returns the current index (or current branch off value) for the
// given beta state.  For example, in the pattern "a(b|c)d", if
// we are currently alternated to the setting "acd", then
// curIndexFor(0) will return 1.
//
/////////////////////////////////////////////////////////////////////
template <class E>
size_t RWRegexAlternator<E>::curIndexFor(size_t beta)
{
    return data_[beta].curIndex_;
}

/////////////////////////////////////////////////////////////////////
//
// Sets the beta index for the given indicated beta state to the
// indicated index.  For example, in the pattern "a(b|c)d", to set
// the first beta state to branch off from a( to c and then to d
// (setting acd), use setBetaIndex(0, 1);
//
/////////////////////////////////////////////////////////////////////
template <class E>
void RWRegexAlternator<E>::setBetaIndex(size_t beta, size_t index)
{
    data_[beta].curIndex_ = index;

    data_[beta].beta_->primary_ =
        data_[beta].beta_->failed_ =
            data_[beta].beta_->lookahead_ =
                data_[beta].transitions_[index];
}

/////////////////////////////////////////////////////////////////////
//
// Returns the number of state transitions available from a given
// beta state.  In "a(b|c)d", we'd get 2 transitions from beta
// state 0.  In "a(b|c|d)e", we'd get 3 transitions from beta 0.
//
/////////////////////////////////////////////////////////////////////
template <class E>
size_t RWRegexAlternator<E>::transitionsAt(size_t beta)
{
    return data_[beta].transitions_.size();
}

/////////////////////////////////////////////////////////////////////
//
// Returns a pointer to the state that is the index'th transition
// off of the indicated beta state.
//
/////////////////////////////////////////////////////////////////////
template <class E>
RWRegexState<E>* RWRegexAlternator<E>::transitionAt(size_t beta, size_t index)
{
    return data_[beta].transitions_[index];
}

/////////////////////////////////////////////////////////////////////
//
// Adds a beta setting to the alternator only if the indicated
// setting is not found in the alternator.  The linear search
// pays off, as even for a page-long expression with tens of
// alternations, only about 12 valid settings exist.  It is
// far cheaper to do the liner search than to re-save already
// saved settings.
//
/////////////////////////////////////////////////////////////////////
template <class E>
void RWRegexAlternator<E>::addSetting(const RWRegexAltSetting& newSetting)
{
    bool         alreadyThere = false;
    RWRegexAltSetting local(newSetting);

    local.translate();
    for (size_t i = 0; !alreadyThere && i < settings_.size(); i++) {
        alreadyThere = alreadyThere || (settings_[i] == local);
    }

    if (!alreadyThere) {
        settings_.push_back(local);
    }
}
