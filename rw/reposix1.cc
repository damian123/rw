

/**********************************************************************
 *
 * Definitions for POSIX services for the parameterized regular expression
 * class, Part 1: regcomp() and related functions.
 * NOTE:  Do NOT change the signature of any of the POSIX.2 member functions
 * in this file except in response to changes in the POSIX standard for RE's.
 * If you do, then you will be unable to use POSIX RE services in the future.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/reposix1.cc#2 $
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
#include <rw/rei.h>

#if !defined(RW_NO_EXCEPTIONS)

// Below enables users to use regular expression bindings
// included with compiler.
#  if defined(RW_NO_POSIX_RE)

/******************************
 *
 * build
 *
 * Builds a new node in the RE finite state machine.
 *****************************/


template<class charT> void
RWTRegularExpressionImp<charT>::build
(
    const charT     symbol,       // Input that fires transition.
    const size_t    transition1,  // 1st transition on input.
    const size_t    transition2,  // 2nd transition on input.
    const size_t    setIndex,     // Character set.
    const bool bmatch,       // Match any character (".")
    const int       nanchor       // Match anchored to front/back.
)
{
    RW_PRECONDITION(rePtr_);

    RWRENode<charT>
    node(symbol, transition1, transition2, setIndex, bmatch, nanchor);
    rePtr_->node.insert(node);
    nextState_++;
}

/**************************
 * set
 *  Sets a state in the RE finite state machine.
 *
 **************************/

template<class charT> void
RWTRegularExpressionImp<charT>::set
(
    const size_t     stateIndex,   // Index of state node.
    const charT      symbol,       // Input that fires transition.
    const size_t     transition1,  // First transition to next state.
    const size_t     transition2,  // 2nd transition to next state.
    const size_t     setIndex,     // Character set.
    const bool  bmatch,       // Match any character (".")
    const int        nanchor       // Match anchored to front/back.
)
{
    RW_PRECONDITION(rePtr_);
    RW_PRECONDITION(stateIndex < maxState_);

    RWRENode<charT>
    node(symbol, transition1, transition2, setIndex, bmatch, nanchor);
    rePtr_->node[stateIndex] = node;
}

/////////////////////////////////////////////////////////////////////////////
//
// regcomp
//
//  Compiles regular expression into a nondeterministic finite automaton.
//  Syntax is based on ISO/IEC 9945-2 (POSIX.2) with extensions.
//  Related functions follow this function in this section.
//
//  This function can be replaced by POSIX.2 system services.
//
//  Returns:
//    0           if OK
//    errorType   if not OK.
//
//      NFA is built with one symbol lookahead and each state transitions to a
//      state that has not yet been built.
//
//      NFA recursive-parser functions return first state of last construction or
//  zero if unsuccessful.
//
//  Extended Regular Expression (ERE) Syntax:
//
//  TOKENS:
//  bar               := '|'
//  className         := one of at least the following:
//                      "alnum", "alpha", "blank", "cntrl", "digit", "graph",
//                      "lower", "print", "punct", "space", "upper", "xdigit"
//  closeBrace        := '}'
//  closeBracket      := ']'
//  closeParen        := ')'
//  collatingElement  := single character that is not a metacharacter
//  colon             := ':'
//  comma             := ','
//  dash              := '-'
//  dollar            := '$'
//  dot               := '.'
//  epsilon           := no symbol needed for transformation.
//  equal             := '='
//  hat               := '^'
//  metacharacter     := ^, -, ] (See ISO/IEC 9945-2 Section 2.8.5.1)
//  openBrace         := '{'
//  openBracket       := '['
//  openParen         := '('
//  ordinaryChar      := not a specialChar
//  plus              := '+'
//  questionMark      := '?'
//  quotedChar        := one of: \\ \^ \. \[ \] \$ \( \) \| \* \+ \? \{ \}
//  repeatCount       := 0 <= repeatCount <= RE_DUP_MAX
//  specialChar       := one of: \ ^ . [ $ ( ) | * + ? {
//                      outside of bracketExpression.
//  star              := '*'
//
//  GRAMMAR:
//  bracketExpression := openBracket bracketList closeBracket
//                      | openBracket hat bracketList closeBracket
//  bracketList       := followList | followList dash
//  characterClass    := colon className colon
//  collatingSymbol   := dot collatingElement dot | dot metacharacter dot
//  endRange          := collatingElement | collatingSymbol
//  equivalenceClass  := equal collatingElement equal
//
//  ere               := ereBranch ereRest
//  ereRest           := bar ereBranch ereRest | epsilon
//              Above two rules eliminate left-recursion in:
//  ere               := ereBranch | ere bar ereBranch
//
//  ereBranch         := ereExpression ereBranchRest
//  ereBranchRest     := ereExpression ereBranchRest | epsilon
//              Above two rules eliminate left-recursion in:
//  ereBranch         := ereExpression | ereBranch ereExpression
//
//  ereExpression     := ereExpressionToken ereExpressionRest
//  ereExpressionRest := ereRepeat ereExpressionRest
//                      | ereExpressionToken ereExpressionRest | epsilon
//      ereExpressionToken:= oneCharEre | hat | dollar | openParen ere closeParen
//              Above three rules eliminate left-recursion in:
//              ereExpression     := oneCharEre | hat | dollar
//                                                                                                      | openParen ere closeParen
//                                      | ereExpression ereRepeat
//
//  ereRepeat         := star | plus | questionMark
//                      | openBrace repeatCount closeBrace
//                      | openBrace repeatCount comma closeBrace
//                      | openBrace repeatCount comma repeatCout closeBrace
//
//  expressionTerm    := rangeExpression | singleExpression
//    Above rule eliminates RE compile errors on rangeExpressions because
//    singleExpression matched first in:
//      expressionTerm    := singleExpression | rangeExpression
//
//      followList                              := expressionTerm followListRest
//      followListRest          := expressionTerm followListRest | epsilon
//              Above two rules eliminate left-recursion in:
//              followList        := expressionTerm | followList expressionTerm
//
//  oneCharEre        := ordinaryChar | quotedChar | dot | bracketExpression
//  rangeExpression   := startRange endRange | startRange dash
//  singleExpression  := endRange | characterClass | equivalenceClass
//  startRange        := endrange dash
/////////////////////////////////////////////////////////////////////////////

template<class charT> int
RWTRegularExpressionImp<charT>::rwRegcomp
(
    const stringT& pattern       // RE to be compiled.
)
{
    RW_PRECONDITION(pattern.length() != 0);
    token_ = pattern;

    try {
        token_++;

        if (token_.isEmpty()) {
            throw RWRECompileError(REG_BADPAT);
        }


        // Build initial state.
        size_t              firstState(0);
        addSubexpression(firstState, 0);
        build(nullCharT(), firstState + 1, firstState + 1);

        // Start compiling regular expression.
        nonacceptingState_ = 0;
        ere(RWRERange(next1(firstState)));

        // here all tokens should have been eaten
        if (!token_.isEmpty()) {
            throw RWRECompileError(REG_BADPAT);
        }

        // Build a state that pretends that it's a transition on an input symbol
        // that isn't in the input alphabet.  This is part of how you get the
        // longest leftmost substring in a nondeterministic finite automaton.
        // See rwRegexec() and ereRepeat() for more information.
        nonacceptingState_ = nextState_;
        build(nullCharT(), nextState_ + 1, nextState_ + 1);

        // Build final (accepting) state.
        subexpression(0, firstState, nextState_);
        build(nullCharT());
    }

    catch (const RWRECompileError& error) {
        return error.no;
    }

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
//                  REGULAR EXPRESSION GRAMMAR
//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
//  bracketExpression := openBracket bracketList closeBracket
//                      | openBracket hat bracketList closeBracket
//  (aka character set)
/////////////////////////////////////////////////////////////////////////////


template<class charT> size_t
RWTRegularExpressionImp<charT>::bracketExpression()
{
    size_t        result(0);

    if (token_() == openBracket()) {
        token_++;

        if (token_.isEmpty()) {
            throw RWRECompileError(REG_BADPAT);
        }

        if (!symbolSetString_.isNull()) {
            symbolSetString_.resize(0);
        }

        if (token_() == hat()) {
            symbolSetString_.append(hat());
            token_++;
        }

        bracketListStartPos_ = token_.position();
        // used in CollatingElement(). ] is not a meta char if found first

        if (!bracketList()) {
            throw RWRECompileError(REG_BADPAT);
        }

        if (token_() == closeBracket()) {
            result = nextState_;
            build(nullCharT(), nextState_ + 1, nextState_ + 1, addSymbolSet());
            // Must be nullCharT input for rwRegexec.
            token_++;

            if (!symbolSetString_.isNull()) {
                symbolSetString_.resize(0);
            }

            return result;
        }

        throw RWRECompileError(REG_EBRACK);
    } // if (token_() == openParen)

    return result;
}

/////////////////////////////////////////////////////////////////////////////
//
//  bracketList       := followList | followList dash
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> bool
RWTRegularExpressionImp<charT>::bracketList()
{
    if (followList()) {
        if (token_() == dash()) {
            symbolSetString_.append(dash());
            token_++;
        }

        return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////
//
//  characterClass    := colon className colon
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> bool
RWTRegularExpressionImp<charT>::characterClass()
{
    if (token_() == colon()) {
        while
        (
            token_() != colon() && token_() != closeBracket() &&
            !token_.isEmpty()
        ) {
            token_++;    // Character class not supported so consume tokens.
        }

        if (token_() == colon()) {
            throw RWRECompileError(RWTRegularExpression<charT>::NOT_SUPPORTED);
        }

        else if (token_() == closeBracket()) {
            return false;    // Not a character class.
        }

        throw RWRECompileError(REG_BADPAT);
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////
//
//  collatingElement  := single character that is not a metacharacter
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> bool
RWTRegularExpressionImp<charT>::collatingElement()
{
    // ^ is a meta char when found first in a bracket expression
    // ] is a meta char when found anywhere but first(after initial ^ if any)
    //    in a bracket expression .  see ISO/IEC 9945-2: 1993(E)

    // The initial ^ has been processed in bracketExpression, so all ^'s found
    // here are normal chars.

    if (token_.isEmpty()) {
        return false;
    }

    if (token_.position() == bracketListStartPos_ ||
            token_() != closeBracket()) {
        // Add character to symbol set to be matched.
        symbolSetString_.append(token_());

        token_++;

        return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////
//
//  collatingSymbol   := dot collatingElement dot | dot metacharacter dot
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> bool
RWTRegularExpressionImp<charT>::collatingSymbol()
{
    if (token_() == dot()) {
        while (token_() != dot() && !token_.isEmpty()) {
            token_++;    // Not supported so consume tokens.
        }

        if (token_() == dot()) {
            throw RWRECompileError(RWTRegularExpression<charT>::NOT_SUPPORTED);
        }
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////
//
//  endRange          := collatingElement | collatingSymbol
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> bool
RWTRegularExpressionImp<charT>::endRange()
{
    if (collatingElement()) {
        return true;
    }

    if (collatingSymbol()) {
        // Special handling will be put here sometime in future.
        // A collating symbol does NOT set the symbol set
        // directly but refers to the locale to see if and as what the collating
        // is defined.  For now, collating symbols are not supported.
        return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////
//
//  equivalenceClass  := equal collatingElement equal
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> bool
RWTRegularExpressionImp<charT>::equivalenceClass()
{
    if (token_() == equal()) {
        while
        (
            token_() != equal() && token_() != closeBracket() &&
            !token_.isEmpty()
        ) {
            token_++;    // Not supported so consume tokens.
        }

        if (token_() == equal()) {
            throw RWRECompileError(RWTRegularExpression<charT>::NOT_SUPPORTED);
        }

        else if (token_() == closeBracket()) {
            return false;
        }

        throw RWRECompileError(REG_BADPAT);
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////
//
//      ere     := ereBranch ereRest
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> RWRERange
RWTRegularExpressionImp<charT>::ere(const RWRERange& range)
{
    if (token_() == bar()) {
        throw(RWRECompileError((int)REG_BADPAT));
    }

    RWRERange  state2(ereBranch(range));          // See ereRest() for details.
    if (state2.isValid()) {
        return ereRest(state2);
    }
    return state2;
}

/////////////////////////////////////////////////////////////////////////////
//
//      ereBranch       := ereExpression ereBranchRest
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> RWRERange
RWTRegularExpressionImp<charT>::ereBranch(const RWRERange& range)
{
    RWRERange        result;
    RWRERange        result1(ereExpression(range));
    RWRERange        result2(ereBranchRest());

    result.start_ = result1.start_;
    if (result2.isValid()) {
        result.end_ = result2.end_;
    }
    else {
        result.end_ = result1.end_;
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////
//
//      ereBranchRest           := ereExpression ereBranchRest | epsilon
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> RWRERange
RWTRegularExpressionImp<charT>::ereBranchRest()
{
    RWRERange     result;
    RWRERange     tmp(ereExpression(result));

    while (tmp.isValid()) {
        result = tmp;
        tmp = ereExpression(result);
    }
    return result;
}

/////////////////////////////////////////////////////////////////////////////
//
//      ereExpression           := ereExpressionToken ereExpressionRest
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> RWRERange
RWTRegularExpressionImp<charT>::ereExpression(const RWRERange& range)
{
    RWRERange             result(range);

    if (token_.isEmpty()) {
        return result;
    }

    result = ereExpressionToken(result);

    if (!result.isValid()) {
        return result;
    }

    result = ereExpressionRest(result);
    return result;
}

/////////////////////////////////////////////////////////////////////////////
//
//  ereExpressionRest := ereRepeat ereExpressionRest
//                      | ereExpressionToken ereExpressionRest | epsilon
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> RWRERange
RWTRegularExpressionImp<charT>::ereExpressionRest(const RWRERange& range)
{
    RWRERange     result(range);
    RWRERange     tmp;

    do {                // do-while eliminates tail-recursion.
        tmp = ereRepeat(result);
        if (!tmp.isValid()) {
            tmp = ereExpressionToken(result);

            if (tmp.isValid()) {
                result = tmp;
            }
        }
        else {
            result = tmp;
        }

        result = (tmp.isValid() ? tmp : result);
    }
    while (tmp.isValid() && !token_.isEmpty());

    return result;
}

/////////////////////////////////////////////////////////////////////////////
//
// ereExpressionToken:= oneCharEre | hat | dollar | openParen ere closeParen
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> RWRERange
RWTRegularExpressionImp<charT>::ereExpressionToken(const RWRERange& range)
{
    RWRERange             result(range.start_);

    if
    (
        token_() == star() || token_() == plus() ||
        token_() == questionMark() || token_() == openBrace()
    ) {
        throw RWRECompileError(REG_BADRPT);    // Repeat metacharacter in wrong place.
    }

    size_t  endResult = oneCharEre();

    if (endResult) {
        result.end_ = endResult;
        return result;
    }

    if (token_() == hat()) {          // Anchor match to front of string or line.
        result.end_ = nextState_;
        build(nullCharT(), nextState_ + 1, nextState_ + 1, 0, false, anchorFront);
        token_++;

        if
        (
            token_() == star() || token_() == plus() ||
            token_() == questionMark() || token_() == openBrace()
        ) {
            throw RWRECompileError(REG_BADRPT);
        }

        return result;
    }

    if (token_() == dollar()) {     // Anchor match to end of string or line.
        result.end_ = nextState_;
        build(nullCharT(), nextState_ + 1, nextState_ + 1, 0, false, anchorBack);
        token_++;
        return result;
    }

    if (token_() == openParen()) {  // Parenthesized subexpression: (ere).
        size_t            startState = nextState_;
        build(nullCharT(), startState + 1, startState + 1);
        token_++;

        unmatchedOpenParen_++;   // number of unmatched open parentheses

        if
        (
            token_() == star() || token_() == plus() ||
            token_() == questionMark() || token_() == openBrace()
        ) {
            throw RWRECompileError(REG_BADRPT);
        }

        if (token_() == closeParen()) {
            throw RWRECompileError(REG_BADPAT);
        }

        RWRERange result1(startState, startState);
        result1 = ere(result1);

        if (token_() == closeParen()) {
            size_t            endState(nextState_);
            build(nullCharT(), endState + 1, endState + 1);
            addSubexpression(startState, endState);
            token_++;

            unmatchedOpenParen_--;

            result.end_ = endState;
            return result;
        }

        throw RWRECompileError(REG_EPAREN);
    } // if (token_() == openParen)

    return result;
}

/////////////////////////////////////////////////////////////////////////////
//
//  ereRepeat         := star | plus | questionMark
//                      | openBrace repeatCount closeBrace
//                      | openBrace repeatCount comma closeBrace
//                      | openBrace repeatCount comma repeatCout closeBrace
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> RWRERange
RWTRegularExpressionImp<charT>::ereRepeat(const RWRERange& range)
{
    RWRERange result(range);

    // suppress bogus warning for unused variable `result'
    RW_UNUSED(result);

    if (token_() == plus()) {
        // "a+" == "aa*".  (Sub)Expression is repeated 1 or more times.
        // Parenthesized subexpressions are treated as one unit.
        size_t            lastSub(lastSubexpression());
        RWRESubexpression sub(getSubexpression(lastSub));
        RWRERange         previousState(nextState_ - 1, nextState_ - 1);
        size_t            plusStateStart(previousState.start_);
        RWRENode<charT>   node;


        if (lastSub && previousState.start_ == sub.end_) {
            // If RWRESubexpression immediately before repeat,
            // then clone entire subexpression as "a".
            size_t          offset(sub.end_ - sub.start_ + 1);
            plusStateStart = sub.start_;

            for (size_t i(sub.start_); i <= sub.end_; i++) {
                node = getNode(i);
                build
                (
                    node.input_, node.next1_ + offset, node.next2_ + offset,
                    node.symbolSetIndex_, node.matchAny_, node.anchor_
                );
            }

            previousState.start_ = sub.end_ + 1;
            previousState.end_ = nextState_ - 1;
        }
        else {
            //Clone one character ERE "a".
            node = getNode(previousState.start_);
            build
            (
                node.input_, nextState_ + 1, nextState_ + 1,
                node.symbolSetIndex_, node.matchAny_, node.anchor_
            );
            previousState.start_ = previousState.end_ = nextState_ - 1;
        }

        makeStar(previousState);  // "a*"

        if (lastSub) {
            // Update subexpression so that last state points to _this_ last state.
            sub.end_ = nextState_ - 1;
            subexpression(lastSub, sub);
        }

        if          // Duplicate repeat metacharacter.
        (
            token_() == plus() || token_() == star() || token_() == questionMark() ||
            token_() == openBrace() || token_() == closeBrace()
        ) {
            throw RWRECompileError(REG_BADRPT);
        }

        // A nonaccepting state is appended to an RE that has a repeat in it.
        nonacceptingState_ = 1;

        return plusStateStart;
    }

    if (token_() == star()) {
        // (Sub)Expression is repeated 0 or more times.

        // Parenthesized subexpressions are treated as one unit.
        size_t            lastSub(lastSubexpression());
        RWRESubexpression sub(getSubexpression(lastSub));
        RWRERange         previousState(nextState_ - 1, nextState_ - 1);

        if (lastSub && previousState.start_ == sub.end_) {
            // Entire parenthesized subexpression is to be repeated.
            previousState.start_ = sub.start_;
            previousState.end_   = sub.end_;
        }

        makeStar(previousState);              // "a*"

        if (lastSub) {
            // Update subexpression so that last state points to _this_ last state.
            sub.end_ = nextState_ - 1;
            subexpression(lastSub, sub);
        }

        if         // Duplicate repeat metacharacter
        (
            token_() == plus() || token_() == star() || token_() == questionMark() ||
            token_() == openBrace() || token_() == closeBrace()
        ) {
            throw RWRECompileError(REG_BADRPT);
        }

        // A nonaccepting state is appended to an RE that has a repeat in it.
        nonacceptingState_ = 1;

        result.end_ = nextState_ - 1;
        return result;
        //return previousState;
    }

    if (token_() == questionMark()) {
        // Construction for a?:
        //           a
        // 0-->1-->2-->3
        //     |       |
        //     +-->-->-+
        //         e
        // (Sub)Expression is repeated 0 or 1 times.

        // Parenthesized subexpressions are treated as one unit.
        size_t            lastSub(lastSubexpression());
        RWRESubexpression sub(getSubexpression(lastSub));
        RWRERange         state2(nextState_ - 1, nextState_ - 1);

        if (lastSub && state2.start_ == sub.end_) {
            // Entire subexpression is to be repeated.
            state2.start_  = sub.start_;
            state2.end_    = sub.end_;
        }

        // Make a new state 2.
        // At this point we have state 2 and "a".

        size_t                newState2(nextState_);
        RWRENode<charT>   node(getNode(state2.start_));

        if (state2.start_ == state2.end_)   // One char ERE is repeated.
            build
            (
                node.input_, newState2 + 1, newState2 + 1,
                node.symbolSetIndex_, node.matchAny_, node.anchor_
            );
        else                              // Subexpression is repeated.
            build
            (
                node.input_, node.next1_, node.next2_,
                node.symbolSetIndex_, node.matchAny_, node.anchor_
            );

        size_t  state3(nextState_);
        build(nullCharT(), state3 + 1, state3 + 1);

        // Move old state 2 (or old state 2 first state) to state 1.
        set(state2.start_, nullCharT(), newState2, state3);

        if (state2.start_ != state2.end_) {
            set(state2.end_, nullCharT(), state3, state3);
        }

        token_++;

        if (lastSub) {
            // Update subexpression so that last state points to _this_ last state.
            sub.end_ = state3;
            subexpression(lastSub, sub);
        }

        if    // Duplicate repeat metacharacter(s).
        (
            token_() == plus() || token_() == star() || token_() == questionMark() ||
            token_() == openBrace() || token_() == closeBrace()
        ) {
            throw RWRECompileError(REG_BADRPT);
        }

        return state2.start_;     // State 1.
    }

    // Braces not supported.
    if (token_() == openBrace()) {
        while (token_() != closeBrace() && !token_.isEmpty()) {
            token_++;    // Not supported so consume tokens.
        }

        if (token_() == closeBrace()) {
            throw RWRECompileError(RWTRegularExpression<charT>::NOT_SUPPORTED);
        }
        else {
            throw RWRECompileError(REG_BADBR);
        }
    }

    return RWRERange();
}

/////////////////////////////////////////////////////////////////////////////
//
// makeStar
//
//  Makes "a*" subexpression in RE finite state machine.
//  Called by ereRepeat().
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> RWRERange
RWTRegularExpressionImp<charT>::makeStar
(
    const RWRERange&    state2
)
{
    // Thompson construction for a* (e == no input symbol):
    //         e
    //       +-<-+
    //       | a |
    //    0-->1-->2-->3-->4
    //    |           |
    //    +--->--->---+
    //                e
    // (Sub)Expression repeats 0 or more times.

    // Make a new state 2.
    // At this point we have state 2 and "a".

    size_t        newState2(nextState_);
    RWRENode<charT> node(getNode(state2.start_));

    if (state2.start_ == state2.end_)          // One character ERE.
        build
        (
            node.input_, nextState_ + 1, nextState_ + 1,
            node.symbolSetIndex_, node.matchAny_, node.anchor_
        );
    else                                      // Parenthesized subexpression.
        build
        (
            node.input_, node.next1_, node.next2_,
            node.symbolSetIndex_, node.matchAny_, node.anchor_
        );

    node = getNode(state2.end_);
    set
    (
        state2.end_, node.input_, newState2, nextState_ + 1,
        node.symbolSetIndex_, node.matchAny_, node.anchor_
    );
    build(nullCharT(), newState2, nextState_ + 1);      // State 3

    size_t                state4(nextState_);
    build(nullCharT(), state4 + 2, state4 + 2);         // State 4
    build(nullCharT(), newState2, state4);              // state 1

    // Make old state 2 into state 0.
    set(state2.start_, nullCharT(), nextState_ - 1, nextState_ - 1);

    token_++;
    return state2;      // State 0.
}

/////////////////////////////////////////////////////////////////////////////
//
//      ereRest := bar ereBranch ereRest | epsilon
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> RWRERange
RWTRegularExpressionImp<charT>::ereRest(const RWRERange& state)
// state 2 in Thompson construction for a|b.
{

    RWRERange result(state.start_);
    RWRERange state2(state);

    while (token_() == bar()) {
        // Thompson construction for a | b (e == no input symbol):
        //  e   a   e
        //  +-->2-->3--+
        //  |          |
        //   -->1      +-->6
        //  |          |
        //  +-->4-->5--+
        //  e   b   e

        // At this point we only have state 2 and "a|".
        // Concatenation has a higher precedence than alternation.

        size_t        state2Start(state2.start_);

        build(nullCharT(), nextState_ + 1, nextState_ + 1); // State 3
        RWRERange state6(nextState_, nextState_);
        build(nullCharT(), 0, 0);                           // State 6 start.
        token_++;

        if
        (
            token_() == star() || token_() == plus() ||
            token_() == questionMark() || token_() == openBrace()
        ) {
            throw RWRECompileError(REG_BADRPT);
        }

        // Get "b".
        RWRERange  state4(nextState_);
        state4 = ereBranch(state4);

        if (!state4.isValid()) {
            // Look at previous token and see if can get a different token.
            token_--;
            state4.start_ = nextState_;
            state4.end_ = nextState_;
            state4 = ereExpressionToken(state4);

            if (!state4.isValid()) {
                throw RWRECompileError(REG_BADPAT);
            }

            // Get "b*" if any.
            if (!token_.isEmpty()) {
                ereRepeat(result);
            }
        }

        build(nullCharT(), state6.start_, state6.start_);     // State 5.

        // Insert new state 2 and make old state 2 into state 1 so don't have
        // to update pointers to state 1.
        size_t              newState2(nextState_);
        RWRENode<charT> node(getNode(state2Start));
        build
        (
            node.input_, node.next1_, node.next2_,
            node.symbolSetIndex_, node.matchAny_, node.anchor_
        );
        set(state2Start, nullCharT(), newState2, state4.start_);
        // Old state 2 now state 1.

        set(state6.end_, nullCharT(), nextState_, nextState_);

        // set new state2
        state2.end_ = nextState_ - 1; // state2.begin_ not changed
    }

    result.end_ = nextState_ - 1;
    return result;
}

/////////////////////////////////////////////////////////////////////////////
//
//  expressionTerm    := rangeExpression | singleExpression
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> bool
RWTRegularExpressionImp<charT>::expressionTerm()
{
    // Prepare for backtracking.
    size_t  oldTokenPosition(token_.position());
    size_t  oldSymbolSetStringPosition(symbolSetString_.length());

    if (rangeExpression()) {
        return true;
    }

    // Backtrack
    token_.position(oldTokenPosition);
    symbolSetString_.remove(oldSymbolSetStringPosition);

    if (singleExpression()) {
        return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////
//
//      followList                      := expressionTerm followListRest
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> bool
RWTRegularExpressionImp<charT>::followList()
{
    if (expressionTerm()) {
        followListRest();
        return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////
//
//      followListRest          := expressionTerm followListRest | epsilon
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> void
RWTRegularExpressionImp<charT>::followListRest()
{
    if (token_.isEmpty()) {
        return;
    }
    while (expressionTerm())
        ;
}

/////////////////////////////////////////////////////////////////////////////
//
//  oneCharEre        := ordinaryChar | quotedChar | dot | bracketExpression
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> size_t
RWTRegularExpressionImp<charT>::oneCharEre()
{
    size_t        result;

    if ((result = ordinaryChar()) == 0) {  /* regular char? */
        if (token_() == dot()) {             /* no. dot?      */
            result = nextState_;
            build(nullCharT(), nextState_ + 1, nextState_ + 1, 0, true);
            token_++;
        }
#    if !defined(RW_THROW_OPTIMIZATION_BUG)
        else if ((result = quotedChar()) == 0) { /* no. Quoted? */
            result = bracketExpression();           /* no. Only bracket left */
        }
#    else
        else {
            result = quotedChar();
            if (result == size_t(-1)) {
                throw RWRECompileError((int)REG_BADPAT);
            }
            if (result == 0) {
                result = bracketExpression();    /* no. Only bracket left */
            }
        }
#    endif
    }
    return result;
}


/////////////////////////////////////////////////////////////////////////////
//
//  ordinaryChar      := not a specialChar
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> size_t
RWTRegularExpressionImp<charT>::ordinaryChar()
{
    size_t result = 0;
    if (token_() != hat() && token_() != dot() &&
            token_() != openBracket() &&
            token_() != openBrace() &&
            token_() != openParen() &&
            (token_() != closeParen() || unmatchedOpenParen_ == 0) &&
            token_() != dollar() && token_() != bar() && token_() != star() &&
            token_() != plus() && token_() != questionMark() &&
            token_() != backslash() && !token_.isEmpty()
       ) {
        result = nextState_;
        build(token_(), nextState_ + 1, nextState_ + 1);
        token_++;
    }
    return result;
}


/************************************
 *
 *  quotedChar        := one of: \^ \. \[  \$ \( \) \| \* \+ \? \{  \\
 *
 ************************************/

template<class charT> size_t
RWTRegularExpressionImp<charT>::quotedChar()
{
    size_t result = 0;

    // ^ and $ must be quoted if they are not anchors and not inside a
    // bracket expression.


#    if !defined(RW_THROW_OPTIMIZATION_BUG)
    if ((token_() == hat() && token_.position() != 0) ||
            (token_() == dollar() && !token_.next().isEmpty())) {
        throw RWRECompileError((int)REG_BADPAT);
    }
#    else
    if ((token_() == hat() && token_.position() != 0) ||
            (token_() == dollar() && !token_.next().isEmpty())) {
        return size_t(-1);
    }
#    endif

    if (token_() == backslash()) {
        token_++;
        if (token_.isEmpty()) {
            throw(RWRECompileError((int)REG_EESCAPE));
        }
        if (token_() == hat()         || token_() == dot()          ||
                token_() == openBracket() || token_() == dollar()       ||
                token_() == openParen()   || token_() == closeParen()   ||
                token_() == bar()         || token_() == star()         ||
                token_() == plus()        || token_() == questionMark() ||
                token_() == openBrace()   || token_() == backslash()    ||
                // Incident #22516: Added ability to escape closing braces and brackets
                token_() == closeBrace()  || token_() == closeBracket()
           ) {
            result = nextState_;
            build(token_(), nextState_ + 1, nextState_ + 1);
            token_++;
            return result;
        }
        else {
            throw(RWRECompileError((int)REG_BADPAT));
        }
    }
    return result;
}

/////////////////////////////////////////////////////////////////////////////
//
//  rangeExpression   := startRange endRange | startRange dash
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> bool
RWTRegularExpressionImp<charT>::rangeExpression()
{
    if (startRange()) {
        if (endRange()) { // Test to make sure that startRange < endRange.
            // symbolSetString_ holds string representation of range.
            size_t  lastIndex(symbolSetString_.length() - 1);

            if ((unsigned char)symbolSetString_[lastIndex] <
                    (unsigned char)symbolSetString_[lastIndex - 2])

            {
                throw RWRECompileError(REG_ERANGE);
            }

            return true;
        }

        else if (token_() == dash()) {
            symbolSetString_.append(token_());
            return true;
        }
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////
//
//  singleExpression  := endRange | characterClass | equivalenceClass
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> bool
RWTRegularExpressionImp<charT>::singleExpression()
{
    if (endRange()) {
        return true;
    }

    if (characterClass()) {
        // Special handling will be put here.
        // A collating symbol does NOT set the symbol set
        // directly but refers to the locale to see if and as what the character
        // class is defined.  For now, this feature is not supported.
        return true;
    }

    if (equivalenceClass()) {
        // Special handling will be put here.
        // A collating symbol does NOT set the symbol set
        // directly but refers to the locale to see if and as what the equivalence
        // class is defined.  For now, this feature is not supported.

        return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////
//
//  specialChar       := one of: \ ^ . [ $ ( ) | * + ? {
//                      outside of bracketExpression.
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> size_t
RWTRegularExpressionImp<charT>::specialChar()
{
    size_t        result(0);
    if
    (
        (token_() == hat() || token_() == dot() || token_() == openBracket()) ||
        (token_() == dollar() || token_() == openParen()) ||
        (token_() == closeParen() || token_() == bar() || token_() == star()) ||
        (token_() == plus() || token_() == questionMark()) ||
        (token_() == openBrace() || token_() == backslash())
    ) {
        result = nextState_;
        build(token_(), nextState_ + 1, nextState_ + 1);
        token_++;
        return result;
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////
//
//  startRange        := endRange dash
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> bool
RWTRegularExpressionImp<charT>::startRange()
{
    if (endRange() && token_() == dash()) {
        symbolSetString_.append(dash());
        token_++;
        return true;
    }

    return false;
}
#  endif  // defined(RW_NO_POSIX_RE)
#endif  // !defined(RW_NO_EXCEPTIONS)
