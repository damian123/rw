#ifndef RW_REGEX_ALT_H
#define RW_REGEX_ALT_H



/**********************************************************************
 *
 * alt.h - contains the class definition for the alternation
 *         management classes (NFA)
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regex/alt.h#1 $
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
#include <rw/tools/regex/vector.h>

/**************************************************************************/
/*                                                                        */
/* Class Name                                                             */
/* ==========                                                             */
/*                                                                        */
/* RWRegexAltSetting                                                      */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This class is used to represent one potential setting for the          */
/* regex alternator (see below) during pattern matching.  An alternation  */
/* setting represents a permutation of alternation bar settings.          */
/* For example, in a(b|c)d, we can traverse from a to b and on to d, or   */
/* we can go from a to c, and then on to d.  Hence the two permutations   */
/* of the pattern, with respect to the beta state after 'a' are:          */
/* abd and acd.  The valid alternation settings are determined at regex   */
/* compile time.                                                          */
/*                                                                        */
/**************************************************************************/
class RWRegexAltSetting
{
public:
    // Default constructor
    RWRegexAltSetting() {}

    // Create, based on input size
    RWRegexAltSetting(size_t newSize)
        : vsize_(newSize) {
        size_t i = 0;
        for (; i < vsize_; setting_.push_back(size_t(-1)), i++) { }
    }

    // Copy constructor
    RWRegexAltSetting(const RWRegexAltSetting& source)
        : vsize_(source.vsize_) {
        size_t i = 0;
        for (; i < vsize_; setting_.push_back(source.setting_[i++])) { }
    }

    // Destructor
    ~RWRegexAltSetting() {}

    // Compare two settings for equality, member by member
    bool operator==(const RWRegexAltSetting& rhs) const {
        bool rc = vsize_ == rhs.vsize_;
        for (size_t i = 0; rc && i < vsize_; i++) {
            rc = rc && (setting_[i] == rhs.setting_[i]);
        }
        return rc;
    }

    // Access an individual setting
    size_t& operator[](size_t i) {
        return setting_[i];
    }

    // How many elements are in a setting?
    size_t size() const {
        return vsize_;
    }

    // Translate all elements by changing all size_t(-1) elements
    // to zero elements.
    void translate() {
        for (size_t i = 0; i < vsize_; i++) {
            setting_[i] = setting_[i] == size_t(-1) ? 0 : setting_[i];
        }
    }

private:
    size_t           vsize_;
    RWRegVec<size_t> setting_;
};

/**************************************************************************/
/*                                                                        */
/* Class Name                                                             */
/* ==========                                                             */
/*                                                                        */
/* RWRegexAlternator                                                      */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This class is used to manipulate the contents of the Beta states in    */
/* the compiled regular expression.  An instance of this class is used    */
/* by the regular expression to manipulate these settings during match    */
/* operations.  The alternator stores, for each beta state, the set of    */
/* starting points following that Beta state.  For example, in the        */
/* following pattern, "ab|cd", a Beta state is inserted before the        */
/* state for "ab".  The possible starting points following the Beta       */
/* state are "ab", and "cd".  Each invocation of "advance()" will change  */
/* the settings of all of the Beta states to another of the valid         */
/* permutations of starting points in the regular expression. "advance()" */
/* returns true until all valid permutations have been exhausted,         */
/* at which point false is returned.  This class allows the regular       */
/* expression matcher to try all valid paths through the pattern as       */
/* defined by the set of alternation characters in the pattern.           */
/*                                                                        */
/**************************************************************************/
template <class E> class RWRegexState;
template <class E>
class RWRegexAlternator
{
private:

    typedef typename RWTRegexTraits<E>::Char RChar;

public:
    RWRegexAlternator();
    virtual ~RWRegexAlternator();

    // Copy from a given alternator and state vector; used
    // during copy construction of regular expressions
    void             copyFrom(const RWRegexAlternator<E>& source, RWRegexState<E>** states);

    // Methods to build the alternator
    void             addBetaState(RWRegexState<E>* state, RWRegexAltSetting& setting);
    void             addBetaTransition(RWRegexState<E>* state);
    void             addOr(RWRegexAltSetting& setting);
    void             addGamma(RWRegexAltSetting& setting);

    // Method to change Beta state configuration
    void             init();
    bool        advance();
    void             clear();

    // Methods for changing the state of individual Beta states
    // (during matching and alternation "advancing")
    size_t           curIndexFor(size_t beta);
    void             setBetaIndex(size_t beta, size_t index);

    // Methods for dynamic alternation
    size_t           transitionsAt(size_t beta);
    RWRegexState<E>* transitionAt(size_t beta, size_t index);

private:
    // Internal data structure for the management of beta transitions.
    // It contains a pointer to the beta state, and the current index
    // into the beta transitions.  The transitions are all first alpha
    // or literal or bracket set states that follow a beta state for
    // all valid settings.
    struct AltData {
        AltData() :
            beta_(0),
            curIndex_(0) {}
        AltData(RWRegexState<E>* newBeta) :
            beta_(newBeta),
            curIndex_(0) {}

        // Beta state manipulated by this alternation item
        RWRegexState<E>*           beta_;

        // Current index in the set of transitions for this beta state,
        // and the transitions collection
        size_t                     curIndex_;
        RWRegVec<RWRegexState<E>*> transitions_;
    };

    // Methods for alternation setup
    void   addSetting(const RWRegexAltSetting& newSetting);

    // Current beta ID during building of the gamma vector
    size_t                       curBeta_;
    RWRegVec<size_t>             betaStack_;

    // Vector of alternation data structures
    RWRegVec<AltData>            data_;

    // Valid alternator settings
    RWRegVec<RWRegexAltSetting>  settings_;
    size_t                       curSetting_;
};

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("alt.cc")
#endif

#if defined(RW_NO_IMPLICIT_INCLUSION)
#  include <rw/tools/regex/alt.cc>
#endif

#endif
