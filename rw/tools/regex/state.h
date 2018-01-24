#ifndef RW_REGEX_STATE_H
#define RW_REGEX_STATE_H

/**********************************************************************
 *
 * rw_regex_state.h - Contains the class definitions for NFA regex states
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regex/state.h#1 $
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

// This struct defines a "namespace" for the weights applied to
// regular expression states, and are used by pass2() to set the
// primary, failed, and lookahead state transitions.

struct RWRegexStateWeights {
    enum Weights {
        Optional      = 16,
        SubExpression = 8,
        Repeated      = 4,
        EndSub        = 2,
        AlphaSub      = 1,
        Unset         = 0
    };
};

/**************************************************************************/
/*                                                                        */
/* Class Name                                                             */
/* ==========                                                             */
/*                                                                        */
/* RWRegexState                                                           */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This class serves as a base class for all RWTRegex states.  As such,    */
/* this class defines a required interface for all RWTRegex states,        */
/*   * match() - defines how a particular state determines if it          */
/*               matches against the current character location in the    */
/*               input string.                                            */
/*   * clone() - defines how a state can be replicated during RWTRegex     */
/*               copy/assignment.                                         */
/*                                                                        */
/* For the purposes of optimization (mainly, the removal of all virtual  */
/* functions that did not need to be virtual) and of simplicity of the    */
/* match algorithm code, all of the state data for any state is included  */
/* here.  As such, the layout in memory of any RWTRegex state will be      */
/* identical except for the vtable pointer.                               */
/*                                                                        */
/**************************************************************************/
template <class E> class RWRegexImp;
template <class E> class RWRegexLexicon;
template <class E>
class RWRegexState
{
    /////////////////////////////////////////////////////////////////////
    // typedef the character type for some compilers
    /////////////////////////////////////////////////////////////////////
private:
    typedef typename RWTRegexTraits<E>::Char RChar;

public:
    /////////////////////////////////////////////////////////////////////
    // Construction / Destruction
    /////////////////////////////////////////////////////////////////////
    RWRegexState();
    RWRegexState(size_t               newMyID,
                 size_t               newMyNext,
                 size_t               newMyClose,
                 RWRegexImp<E>*       newRegex,
                 unsigned char        newAlpha,
                 RWRegexLexicon<E>*   newLex,
                 RWRegexState<E>*     newMyAlpha);
    ~RWRegexState();

    /////////////////////////////////////////////////////////////////////
    // Initialize a state (while pattern matching)
    /////////////////////////////////////////////////////////////////////
    void             init(bool topLevel = false);

    /////////////////////////////////////////////////////////////////////
    // - Do the match for any type of state
    // - Clone a state (makes a new, heap-allocated copy)
    /////////////////////////////////////////////////////////////////////
    RWRegexState<E>* doMatch(const RChar*& c);
    RWRegexState<E>* clone();

    /////////////////////////////////////////////////////////////////////
    // - match matches a state against character(s), eats the matching
    //   character(s) if there is a match
    // - peek just looks to see if the character(s) would match, without
    //   eating the characters
    /////////////////////////////////////////////////////////////////////
    bool match(const RChar*& c) {
        if (anchorBegin_) {
            return c == regex_->bos_ + regex_->off_;
        }

        if (anchorEnd_) {
            return c == regex_->eos_;
        }

        return alpha_ ? primary_->peek(c) : lex_->match(c, regex_->eos_);
    }

    bool peek(const RChar* c) {
        if (anchorBegin_) {
            return c == regex_->bos_ + regex_->off_;
        }

        if (anchorEnd_) {
            return c == regex_->eos_;
        }

        return alpha_ ? primary_->peek(c) : lex_->match(c, regex_->eos_);
    }

    /////////////////////////////////////////////////////////////////////
    // Compilation goodies: these represent the indexes into the
    // regex state vector of this state, the next state, and the
    // closing state (only relevant for alpha states).
    /////////////////////////////////////////////////////////////////////
    size_t                     myID_;
    size_t                     myNext_;
    size_t                     myClose_;

    /////////////////////////////////////////////////////////////////////
    // State weights for calculating state transitions:
    // opt   - is the state's data optional (* or ?)
    // sub   - is the state part of a subexpression
    // rep   - is the state repeated (* or +)
    // end   - is the state at the end of a subexpression
    // alpha - is the state an alpha state
    /////////////////////////////////////////////////////////////////////
    unsigned char              opt_;
    unsigned char              sub_;
    unsigned char              rep_;
    unsigned char              end_;
    unsigned char              alpha_;

    // Owning expression
    RWRegexImp<E>*             regex_;

    // Alpha ID; used only by alpha states for storing subexp matches
    size_t                     aID_;

    // Lexicon decorator
    RWRegexLexicon<E>*         lex_;

    /////////////////////////////////////////////////////////////////////
    // Cardinality requirements {m,n}, with special cases:
    // ? ==> {0,1}
    // * ==> {0,-1}
    // + ==> {1,-1}
    /////////////////////////////////////////////////////////////////////
    size_t                     minVisits_;
    size_t                     maxVisits_;

    /////////////////////////////////////////////////////////////////////
    // Matching flags:
    // - Count number of visits to state during current pass through subexp
    // - Matched here flag
    // - Failed subexp match (applies to alpha states, set by states inside
    //   the subexp
    /////////////////////////////////////////////////////////////////////
    size_t                     visited_;
    bool                  matched_;
    bool                  fail_;

    /////////////////////////////////////////////////////////////////////
    // Miscellaneous flags:
    // lastGood_    - character position of last "good" status of the
    //                match
    // firstMatch_  - character position of first match of a state
    // betaID_      - beta ID - O..N-1 where N is the number of beta
    //                states in the expression
    // betaIndex    - beta setting for a beta state when alternating
    // fib_         - indicates that the state will report erroneous
    //                results, effectively telling a "fib" :-)  Actually
    //                indicates first state in first alternate sequence
    //                in an alternation sequence.
    // anchorBegin_ - true iff state is a begin anchor
    // anchorEnd_   - true iff state is anchored at end of string
    /////////////////////////////////////////////////////////////////////
    const RChar*               lastGood_;
    const RChar*               firstMatch_;
    size_t                     betaID_;
    size_t                     betaIndex_;
    bool                  fib_;
    bool                  anchorBegin_;
    bool                  anchorEnd_;

    /////////////////////////////////////////////////////////////////////
    // State transitions:
    // myAlpha   - This states alpha (valid only for states in subexps)
    // primary   - Primary transition (usually next for cardinalities
    //             required and question, self for + and *)
    // failed    - Where to go when matching on a state must stop
    //             (max card reached, or actual match failure)
    // lookahead - Where to look next for saving retries
    /////////////////////////////////////////////////////////////////////
    RWRegexState<E>*           myAlpha_;
    RWRegexState<E>*           primary_;
    RWRegexState<E>*           failed_;
    RWRegexState<E>*           lookahead_;

private:
    RWRegexState<E>*           handleAlphaState(const RChar*& c);
    bool                  alphasVisitedOnce();
};



#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("state.cc")
#endif

#if defined(RW_NO_IMPLICIT_INCLUSION)
#  include <rw/tools/regex/state.cc>
#endif

#endif
