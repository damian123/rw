
/**********************************************************************
 *
 * compiler.cc - contains the implementation for the NFA regex
 *               compiler.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regex/compiler.cc#2 $
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


#include <rw/tools/regex/lexicon.h>
#include <rw/tools/regex/imp.h>
#include <rw/tools/regex/alt.h>
#include <rw/tools/regex.h>
#include <rw/rwerr.h>

template <class E> class RWRegexLiteral;
template <class E> class RWRegexBracketSet;
template <class E> class RWRegexAlphaLex;
template <class E> class RWRegexEndNode;
template <class E> class RWRegexDot;
template <class E> class RWRegexLiteralString;
template <class E> class RWRegexRange;

template <class E>
bool RWRegexCompiler<E>::isSpecial(const RChar& c)
{
    return (RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::oparen()) ||
            RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::cparen()) ||
            RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::obracket()) ||
            RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::cbracket()) ||
            RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::ocurly()) ||
            RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::ccurly()) ||
            RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::dot()) ||
            RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::star()) ||
            RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::plus()) ||
            RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::question()) ||
            RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::vbar()) ||
            RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::caret()) ||
            RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::dollar()));
}

template <class E>
bool RWRegexCompiler<E>::isCard(RWRegexNodeType t)
{
    return (t == Star ||
            t == Plus ||
            t == Question ||
            t == ArbCard);
}

template <class E>
bool RWRegexCompiler<E>::isCard(const RChar& c)
{
    return (
               RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::star()) ||
               RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::plus()) ||
               RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::question()) ||
               RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::ocurly())
           );
}

template <class E>
bool RWRegexCompiler<E>::isRep(const RChar& c)
{
    return (
               RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::star()) ||
               RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::plus()) ||
               RWTRegexTraits<E>::eq(c, RWTRegexTraits<E>::ocurly())
           );
}

/**************************************************************************/

template <class E>
void RWRegexCompiler<E>::compile()
{
    preprocess();
    pass1();
    pass2();
}

/**************************************************************************/

template <class E>
void RWRegexCompiler<E>::preprocess()
{
    const RChar* buf = regex_->pattern_();
    size_t length = regex_->patternLength_;
    size_t i;
    size_t j;
    size_t tokenLength = 0;
    const RChar* cur = 0;
    RWRegVec<size_t> alphaStack;
    size_t alpha;
    size_t start;
    RWRegexNodeType last = Null;

    // Fill Beta map
    fillBGTable(buf, length);

    // Allocate space for nl if needed
    nl_.set(length + 2 * gammaCount_ + 2);
    nlSize_ = 0;

    for (i = 0; i < length; i += tokenLength) {
        cur = &buf[i];

        if (bgTable_()[i] == BGElement(Beta)) {
            last = validate(last, Beta, cur);
            const size_t nlSize = nlSize_++;
            (nl_()[nlSize]) = RWRegexNode(last, 0, nlSize_, size_t(-1));
        }
        else if (bgTable_()[i] == BGElement(Gamma)) {
            last = validate(last, Gamma, cur);
            const size_t nlSize = nlSize_++;
            (nl_()[nlSize]) = RWRegexNode(last, 0, nlSize_, size_t(-1));
        }

        // Look at cur to create regex nodes
        if (RWTRegexTraits<E>::eq(*cur, RWTRegexTraits<E>::oparen())) {
            last = validate(last, Alpha, cur);
            tokenLength = 1;
            const size_t nlSize = nlSize_++;
            (nl_()[nlSize]) = RWRegexNode(
                                  last, tokenLength, nlSize_,
                                  alphaStack.size() ? alphaStack.back() : size_t(-1));

            alphaStack.push_back(nlSize_ - 1);
        }
        else if (RWTRegexTraits<E>::eq(*cur, RWTRegexTraits<E>::cparen())) {
            if (alphaStack.size()) {
                last = validate(last, Omega, cur);
                tokenLength = 1;

                const size_t nlSize = nlSize_++;
                (nl_()[nlSize]) = RWRegexNode(
                                      last, tokenLength, nlSize_,
                                      alphaStack.size() ? alphaStack.back() : size_t(-1));

                alpha = alphaStack.back();
                alphaStack.pop_back();
                nl_()[alpha].myClose_ = nlSize_;
            }
            else {
                regexThrow(RWTRegex<E>::UnmatchedClosingParen, cur);
            }
        }
        else if (RWTRegexTraits<E>::eq(*cur, RWTRegexTraits<E>::obracket())) {
            last = validate(last, Bracket, cur);

            if (i + 1 < length &&
                    RWTRegexTraits<E>::eq(cur[1], RWTRegexTraits<E>::caret())) {
                start = 2;
            }
            else {
                start = 1;
            }

            j = endOfBracketSet(cur, i, start, length);
            if (RWTRegexTraits<E>::eq(cur[j], RWTRegexTraits<E>::cbracket())) {
                j++;
                tokenLength = j;

                const size_t nlSize = nlSize_++;
                (nl_()[nlSize]) = RWRegexNode(
                                      last, tokenLength, nlSize_,
                                      alphaStack.size() ? alphaStack.back() : size_t(-1));

            }
            else {
                regexThrow(RWTRegex<E>::MissingClosingBracket, cur);
            }
        }
        else if (RWTRegexTraits<E>::eq(*cur, RWTRegexTraits<E>::ocurly())) {
            last = validate(last, ArbCard, cur);
            for (j = 0;
                    i + j < length &&
                    !RWTRegexTraits<E>::eq(cur[j], RWTRegexTraits<E>::ccurly());
                    j++) { }

            if (RWTRegexTraits<E>::eq(cur[j], RWTRegexTraits<E>::ccurly())) {
                j++;
                tokenLength = j;
                const size_t nlSize = nlSize_++;
                (nl_()[nlSize]) = RWRegexNode(
                                      last, tokenLength, nlSize_,
                                      alphaStack.size() ? alphaStack.back() : size_t(-1));

            }
            else {
                regexThrow(RWTRegex<E>::MissingClosingCurlyBrace, cur);
            }
        }
        else if (RWTRegexTraits<E>::eq(*cur, RWTRegexTraits<E>::ccurly())) {
            regexThrow(RWTRegex<E>::UnmatchedClosingCurly, cur);
        }
        else if (RWTRegexTraits<E>::eq(*cur, RWTRegexTraits<E>::caret())) {
            last = validate(last, AnchorBegin, cur);
            tokenLength = 1;

            const size_t nlSize = nlSize_++;
            (nl_()[nlSize]) = RWRegexNode(
                                  last, tokenLength, nlSize_,
                                  alphaStack.size() ? alphaStack.back() : size_t(-1));
        }
        else if (RWTRegexTraits<E>::eq(*cur, RWTRegexTraits<E>::dollar())) {
            last = validate(last, AnchorEnd, cur);
            tokenLength = 1;
            const size_t nlSize = nlSize_++;
            (nl_()[nlSize]) = RWRegexNode(
                                  last, tokenLength, nlSize_,
                                  alphaStack.size() ? alphaStack.back() : size_t(-1));
        }
        else if (RWTRegexTraits<E>::eq(*cur, RWTRegexTraits<E>::plus())) {
            last = validate(last, Plus, cur);
            tokenLength = 1;
            const size_t nlSize = nlSize_++;
            (nl_()[nlSize]) = RWRegexNode(
                                  last, tokenLength, nlSize_,
                                  alphaStack.size() ? alphaStack.back() : size_t(-1));
        }
        else if (RWTRegexTraits<E>::eq(*cur, RWTRegexTraits<E>::star())) {
            last = validate(last, Star, cur);
            tokenLength = 1;
            const size_t nlSize = nlSize_++;
            (nl_()[nlSize]) = RWRegexNode(
                                  last, tokenLength, nlSize_,
                                  alphaStack.size() ? alphaStack.back() : size_t(-1));
        }
        else if (RWTRegexTraits<E>::eq(*cur, RWTRegexTraits<E>::question())) {
            last = validate(last, Question, cur);
            tokenLength = 1;
            const size_t nlSize = nlSize_++;
            (nl_()[nlSize]) = RWRegexNode(
                                  last, tokenLength, nlSize_,
                                  alphaStack.size() ? alphaStack.back() : size_t(-1));
        }
        else if (RWTRegexTraits<E>::eq(*cur, RWTRegexTraits<E>::dot())) {
            last = validate(last, Dot, cur);
            tokenLength = 1;
            const size_t nlSize = nlSize_++;
            (nl_()[nlSize]) = RWRegexNode(
                                  last, tokenLength, nlSize_,
                                  alphaStack.size() ? alphaStack.back() : size_t(-1));
        }
        else if (RWTRegexTraits<E>::eq(*cur, RWTRegexTraits<E>::vbar())) {
            last = validate(last, Or, cur);
            tokenLength = 1;
            const size_t nlSize = nlSize_++;
            (nl_()[nlSize]) = RWRegexNode(
                                  last, tokenLength, nlSize_,
                                  alphaStack.size() ? alphaStack.back() : size_t(-1));
        }
        else {
            size_t end = length - i;
            tokenLength = parseLiteral(cur, end, last);
            const size_t nlSize = nlSize_++;
            (nl_()[nlSize]) = RWRegexNode(
                                  last, tokenLength, nlSize_,
                                  alphaStack.size() ? alphaStack.back() : size_t(-1));
        }
    }

    if (alphaStack.size()) {
        regexThrow(RWTRegex<E>::MissingClosingParen, cur);
    }

    // Add an ending Gamma state if required.
    if (bgTable_()[i] == BGElement(Gamma)) {
        last = validate(last, Gamma, cur);
        const size_t nlSize = nlSize_++;
        (nl_()[nlSize]) = RWRegexNode(Gamma, 0, nlSize_, size_t(-1));
    }

    // Delete the bgTable_
    bgTable_.clear();

    // Optimize out all unnecessary Or states
    collapseBetaStates();

    // Build the gamma vector, which is used in the next two methods
    buildGammaVector();

    // Tie each last state before the Gamma/Or state to the proper next state
    setNextStates();

    // Tie each alpha/Or state after a Beta state to the proper gamma state
    setCloseStates();

    // Deallocate the gamma vector
    gammaVector_.clear();

    // Add two null states to make it easy to walk through, looking
    // for next, etc
    (nl_()[nlSize_++]) =
        RWRegexNode(Null, size_t(-1), size_t(-1), size_t(-1));
    (nl_()[nlSize_++]) =
        RWRegexNode(Null, size_t(-1), size_t(-1), size_t(-1));

    // Allocate the actual regex states
    allocateRegexStates();
}

template <class E>
inline bool RWRegexCompiler<E>::cParen(const RChar* buf, size_t i)
{
    return (RWTRegexTraits<E>::eq(buf[i], RWTRegexTraits<E>::cparen()) &&
            (i > 0 ?
             !RWTRegexTraits<E>::eq(buf[i - 1], RWTRegexTraits<E>::escape()) :
             true));
}

template <class E>
inline bool RWRegexCompiler<E>::oParen(const RChar* buf, size_t i)
{
    return (RWTRegexTraits<E>::eq(buf[i], RWTRegexTraits<E>::oparen()) &&
            (i > 0 ?
             !RWTRegexTraits<E>::eq(buf[i - 1], RWTRegexTraits<E>::escape()) :
             true));
}

template <class E>
inline bool RWRegexCompiler<E>::vBar(const RChar* buf, size_t i)
{
    return
        (RWTRegexTraits<E>::eq(buf[i], RWTRegexTraits<E>::vbar()) &&
         (i > 0
          ? !RWTRegexTraits<E>::eq(buf[i - 1], RWTRegexTraits<E>::escape())
          : true));
}

template <class E>
void RWRegexCompiler<E>::fillBGTable(const RChar*& buf, size_t length)
{
    size_t i;

    // Parenthesis count
    size_t parenCount;

    // True iff we've found an OR bar
    bool foundVBar = false;

    // Remembers sub-exps with alternation
    RWRegVec<size_t> orCounter;

    // Holds enums for oparen, cparen, and vbar
    RWBufMan<BGElement, BGTableSize> bgTemp;

    // Reset gamma count
    gammaCount_ = 0;

    // Allocate a bgTable_
    bgTable_.set(length + 1);
    memset(bgTable_(), 0, (length + 1) * sizeof(BGElement));

    // Assign the size of bgTemp, and fill it with the values we'll need
    bgTemp.set(length);

    // Look for alternation
    for (i = 0; i < length; i++) {
        if (vBar(buf, i)) {
            bgTemp()[i] = BGElement(Or);
            foundVBar = true;
        }
        else {
            bgTemp()[i] = BGElement(Null);
        }
    }

    // If we have alternation, then continue...
    if (foundVBar) {
        // Look for all opening and closing parentheses
        for (i = 0; i < length; i++) {
            if (oParen(buf, i)) {
                bgTemp()[i] = BGElement(Alpha);
            }
            else if (cParen(buf, i)) {
                bgTemp()[i] = BGElement(Omega);
            }
        }

        // Initially, make a slot for zero-level (not in subexpression)
        orCounter.push_back(0);

        // Pass backwards over the pattern, looking for places to put Beta states
        for (i = length - 1, parenCount = 0; i != size_t(-1); --i) {
            switch (bgTemp()[i]) {
                case BGElement(Omega):
                    parenCount++;
                    if (parenCount == orCounter.size()) {
                        orCounter.push_back(0);
                    }
                    else {
                        orCounter[parenCount] = 0;
                    }
                    break;

                case BGElement(Alpha):
                    if (orCounter[parenCount]) {
                        bgTable_()[i + 1] = BGElement(Beta);
                    }
                    if (parenCount) {
                        parenCount--;
                    }
                    else {
                        regexThrow(RWTRegex<E>::MissingClosingParen, buf + i);
                    }
                    break;

                case BGElement(Or):
                    orCounter[parenCount] = 1;
                    break;

                default:
                    break;
            }
        }

        if (orCounter[0]) {
            size_t offset;
            for (offset = 0;
                    offset < length &&
                    RWTRegexTraits<E>::eq(buf[offset], RWTRegexTraits<E>::caret());
                    ++offset) { }
            bgTable_()[offset] = BGElement(Beta);
        }

        // Now sweep forward over the pattern, looking for locations needing
        // Gamma states

        // Initially, make a slot for zero-level (not in subexpression)
        orCounter.clear();
        orCounter.push_back(0);

        for (i = 0, parenCount = 0; i < length; ++i) {
            switch (bgTemp()[i]) {
                case BGElement(Alpha):
                    parenCount++;
                    if (parenCount == orCounter.size()) {
                        orCounter.push_back(0);
                    }
                    else {
                        orCounter[parenCount] = 0;
                    }
                    break;

                case BGElement(Omega):
                    if (orCounter[parenCount]) {
                        bgTable_()[i] = BGElement(Gamma);
                        gammaCount_++;
                    }

                    if (parenCount) {
                        parenCount--;
                    }
                    else {
                        regexThrow(RWTRegex<E>::UnmatchedClosingParen, buf + i);
                    }

                    break;

                case BGElement(Or):
                    orCounter[parenCount] = 1;
                    break;

                default:
                    break;
            }
        }

        if (orCounter[0]) {
            bgTable_()[i] = BGElement(Gamma);
            gammaCount_++;
        }
    }
}

template <class E>
typename RWRegexCompiler<E>::RWRegexNodeType
RWRegexCompiler<E>::validate(RWRegexNodeType last,
                             RWRegexNodeType cur,
                             const RChar* c)
{
    switch (last) {
        case Null:
            if (isCard(cur)) {
                regexThrow(RWTRegex<E>::LeadingCardinality, c);
            }
            break;

        case Alpha:
            if (isCard(cur) || cur == Or || cur == Gamma) {
                regexThrow(RWTRegex<E>::InvalidSubexpression, c);
            }
            break;

        case Or:
            if (isCard(cur) || cur == Beta || cur == Or || cur == Gamma) {
                regexThrow(RWTRegex<E>::InvalidDataAfterOr, c);
            }
            break;

        case Beta:
            if (isCard(cur) || cur == Beta || cur == Or || cur == Gamma) {
                regexThrow(RWTRegex<E>::InvalidDataBeforeOr, c);
            }
            break;

        case Gamma:
            if (isCard(cur) || cur == Beta || cur == Or || cur == Gamma) {
                regexThrow(RWTRegex<E>::InvalidDataAfterOr, c);
            }
            break;

        case Plus:
            if (isCard(cur)) {
                regexThrow(RWTRegex<E>::ConsecutiveCardinalities, c);
            }
            break;

        case Star:
            if (isCard(cur)) {
                regexThrow(RWTRegex<E>::ConsecutiveCardinalities, c);
            }
            break;

        case Question:
            if (isCard(cur)) {
                regexThrow(RWTRegex<E>::ConsecutiveCardinalities, c);
            }
            break;

        case ArbCard:
            if (isCard(cur)) {
                regexThrow(RWTRegex<E>::ConsecutiveCardinalities, c);
            }
            break;

        default:
            break;
    }

    return cur;
}

template <class E>
bool RWRegexCompiler<E>::validateArbCard(const RChar* c, size_t len)
{
    bool validChar = true;
    bool sawDigit = false;

    size_t limit = len - 1;
    for (size_t i = 1; validChar && i < limit; i++) {
        if (RWTRegexTraits<E>::eq(c[i], RWTRegexTraits<E>::comma())) {
            if (!sawDigit) {
                validChar = false;
            }
        }
        else if (
            RWTRegexTraits<E>::eq(c[i], RWTRegexTraits<E>::zero()) ||
            RWTRegexTraits<E>::eq(c[i], RWTRegexTraits<E>::one()) ||
            RWTRegexTraits<E>::eq(c[i], RWTRegexTraits<E>::two()) ||
            RWTRegexTraits<E>::eq(c[i], RWTRegexTraits<E>::three()) ||
            RWTRegexTraits<E>::eq(c[i], RWTRegexTraits<E>::four()) ||
            RWTRegexTraits<E>::eq(c[i], RWTRegexTraits<E>::five()) ||
            RWTRegexTraits<E>::eq(c[i], RWTRegexTraits<E>::six()) ||
            RWTRegexTraits<E>::eq(c[i], RWTRegexTraits<E>::seven()) ||
            RWTRegexTraits<E>::eq(c[i], RWTRegexTraits<E>::eight()) ||
            RWTRegexTraits<E>::eq(c[i], RWTRegexTraits<E>::nine())
        ) {
            sawDigit = true;
        }
        else if (!RWTRegexTraits<E>::eq(c[i], RWTRegexTraits<E>::blank())) {
            validChar = false;
        }
    }

    return (validChar && sawDigit);
}

template <class E>
void RWRegexCompiler<E>::collapseBetaStates()
{
    size_t i;
    size_t j;
    size_t beta;
    size_t gamma;
    bool candidate;

    // Look for an Or state
    for (i = 0; i < nlSize_; i++) {
        if (nl_()[i].type_ == Or) {
            candidate = true;
            beta = gamma = size_t(-1);

            // Now that we have found an or state, see if we can collapse
            // it to a BracketSet

            // Check backward; ensuring we have no embedded subexpressions and
            // no special cardinalities
            for (j = i; candidate && nl_()[j].type_ != Beta; j--) {
                switch (nl_()[j].type_) {
                    case Bracket:

                        if (nl_()[j - 1].type_ != Or &&
                                nl_()[j - 1].type_ != Beta) {
                            candidate = false;
                        }

                        break;

                    case Literal:
                        if (nl_()[j].length_ != 1) {
                            candidate = false;
                        }

                        if (nl_()[j - 1].type_ != Or &&
                                nl_()[j - 1].type_ != Beta) {
                            candidate = false;
                        }

                        break;

                    case Beta:
                    case Or:
                        break;

                    default:
                        candidate = false;
                        break;
                }
            }
            if (candidate) {
                beta = j;
            }

            // Check forward; ensuring we have no embedded subexpressions and
            // no special cardinalities
            if (candidate) {
                for (j = i; candidate && nl_()[j].type_ != Gamma; j++) {
                    switch (nl_()[j].type_) {
                        case Bracket:
                            if (nl_()[j - 1].type_ != Or &&
                                    nl_()[j - 1].type_ != Beta) {
                                candidate = false;
                            }

                            break;

                        case Literal:
                            if (nl_()[j].length_ != 1) {
                                candidate = false;
                            }

                            if (nl_()[j - 1].type_ != Or &&
                                    nl_()[j - 1].type_ != Beta) {
                                candidate = false;
                            }

                            break;

                        case Gamma:
                        case Or:
                            break;

                        default:
                            candidate = false;
                            break;
                    }
                }
            }

            if (nl_()[j].type_ == Gamma) {
                gamma = j;
            }

            if (candidate) {
                nl_()[beta].type_ = SBeta;
                nl_()[gamma].type_ = SGamma;
                for (j = beta + 1; j < gamma; j++) {
                    if (nl_()[j].type_ == Or) {
                        nl_()[j].type_ = Collapsed;
                    }
                }
            }
        }
    }
}

template <class E>
void RWRegexCompiler<E>::buildGammaVector()
{
    size_t i;
    bool look = false;
    size_t lGammaCount = 0;

    // Allocate the gamma vector
    gammaVector_.set(gammaCount_);
    gammaCount_ = 0;

    // Save all first alpha or literal states following Gamma states
    for (i = 0; i < nlSize_; ++i) {
        switch (nl_()[i].type_) {
            case Gamma:
                look = true;
                lGammaCount++;
                continue;

            case Or:
                if (look) {
                    look = false;
                    continue;
                }

                break;

            default:
                break;
        }

        if (look &&
                (nl_()[i].type_ == Alpha || nl_()[i].type_ == Literal  ||
                 nl_()[i].type_ == Dot   || nl_()[i].type_ == Bracket  ||
                 nl_()[i].type_ == Beta)) {

            for (; lGammaCount; lGammaCount--) {
                gammaVector_()[gammaCount_++] = i;
            }

            look = false;
        }
    }

    if (look) {
        for (; lGammaCount; --lGammaCount) {
            gammaVector_()[gammaCount_++] = i;
        }

        look = false;
    }
}

// Set the next state; iterate backwards through the preliminary states
// and for each state preceding a gamma state or an or state, set the next
// state to be the corresponding entry in the gamma vector.
template <class E>
void RWRegexCompiler<E>::setNextStates()
{
    size_t i;
    bool look = false;
    size_t nextID;
    size_t betaCount;

    betaCount = 0;
    nextID = gammaCount_;
    for (i = nlSize_ - 1; i != size_t (-1); --i) {
        if (nl_()[i].type_ == Gamma) {
            look = true;
            betaCount++;
            nextID--;
            continue;
        }
        else if (nl_()[i].type_ == Or) {
            look = true;
            continue;
        }
        else if (nl_()[i].type_ == Beta) {
            look = false;
            betaCount--;
        }

        if (look) {
            nl_()[i].myNext_ = gammaVector_()[nextID];
            switch (nl_()[i].type_) {
                case AnchorBegin:
                case AnchorEnd:
                case Literal:
                case Dot:
                case Bracket:
                    look = false;
                    break;

                default:
                    break;
            }
        }
        else {
            nl_()[i].myNext_ = i + 1;
        }
    }
}

// Set the close states; iterate backwards through the preliminary states
// and for each omega state, set its corresponding alpha to close at the
// corresponding entry in the gamma vector.
template <class E>
void RWRegexCompiler<E>::setCloseStates()
{
    size_t i;
    bool look = false;
    size_t nextID;
    size_t betaCount;

    betaCount = 0;
    nextID = gammaCount_;

    for (i = nlSize_ - 1; i != size_t(-1); --i) {
        if (nl_()[i].type_ == Gamma) {
            look = true;
            betaCount++;
            nextID--;
            continue;
        }
        else if (nl_()[i].type_ == Or) {
            look = true;
            continue;
        }
        else if (nl_()[i].type_ == Beta) {
            look = false;
            betaCount--;
            if (betaCount) {
                nextID++;
            }
            continue;
        }
        else if (nl_()[i].type_ == AnchorBegin ||
                 nl_()[i].type_ == AnchorEnd   ||
                 nl_()[i].type_ == Literal     ||
                 nl_()[i].type_ == Dot         ||
                 nl_()[i].type_ == Bracket) {
            look = false;
            continue;
        }
        else if (look && nl_()[i].type_ == Omega) {
            nl_()[nl_()[i].myAlpha_].myClose_ = gammaVector_()[nextID];
        }
    }
}

template <class E>
void RWRegexCompiler<E>::allocateRegexStates()
{
    size_t i;
    size_t numStates = 0;
    bool sawLiteral = false;
    bool collapse = false;
    bool sawBeta = false;

    // Allocate the state ID map
    stateIDMap_.set(nlSize_);

    for (i = 0; i < nlSize_; i++) {
        stateIDMap_()[i] = numStates;

        switch (nl_()[i].type_) {
            case Alpha:
                ++numStates;
                break;

            case Beta:
                sawBeta = true;
                ++numStates;
                break;

            case SBeta:
                ++numStates;
                collapse = true;
                break;

            case SGamma:
                collapse = false;
                break;

            case Bracket:
            case Literal:
            case Dot:
                sawLiteral = true;
                if (!collapse) {
                    ++numStates;
                }
                break;

            case AnchorBegin:
                ++numStates;
                if (!sawBeta && sawLiteral) {
                    regex_->invalid_ = true;
                }
                break;

            case AnchorEnd:
                ++numStates;
                break;

            default:
                break;
        }
    }

    numStates += 2;

    // Allocate space for the states
    regex_->states_.set(numStates);
}

/**************************************************************************/

template <class E>
void RWRegexCompiler<E>::pass1()
{
    // Create all regular expression states, adding them to a vector container
    createStates();

    // Add sink states, and patch up alpha transitions
    addSinkStates();

    // Allocate match information buffers
    allocateMatchInfo();

    // Done with nl; deallocate it
    nl_.clear();

    // Done with stateIDMap_; deallocate it
    stateIDMap_.clear();
}

template <class E>
void RWRegexCompiler<E>::createStates()
{
    size_t i;
    size_t j;

    //  Number of nodes to consider
    size_t size = nlSize_ - 2;

    //  Current state
    RWRegexState<E>* s = 0;

    //  Pattern buffer
    const RChar* cur = regex_->pattern_();

    //  State stack
    RWRegVec<RWRegexState<E>*> sStack;

    //  Current alpha state
    RWRegexState<E>* curAlpha = 0;

    //  Flag for whether to alternate
    bool alternate = false;

    //  Alternation setting
    RWRegexAltSetting setting(gammaCount_);

    // Reset the number of states in the regex
    regex_->alphaCount_ = 0;
    regex_->numStates_ = 0;

    // Allocate the states
    for (i = 0; i < size; ++i, s = 0) {
        switch (nl_()[i].type_) {
            case Alpha:
                s = handleAlpha(i, curAlpha);
                sStack.push_back(s);
                break;

            case Omega:
                s = sStack.back();
                curAlpha = sStack.size() > 1 ? sStack[sStack.size() - 2] : 0;
                sStack.pop_back();
                break;

            case Beta:
                s = handleBeta(i, setting);
                alternate = true;
                break;

            case Gamma:
                regex_->alternator_.addGamma(setting);
                break;

            case Or:
                regex_->alternator_.addOr(setting);
                alternate = true;
                break;

            case Bracket:
            case Literal:
            case Dot:
                s = handleData(i, makeLexicon(cur, nl_()[i].length_), curAlpha);
                break;

            case SBeta:
                s = handleSBeta(i, cur, curAlpha);
                break;

            case AnchorBegin:
                // Handle anchor begin as a stand-alone state
                s = handleAnchor(i, curAlpha);
                s->anchorBegin_ = true;
                break;

            case AnchorEnd:
                // Handle anchor end as a stand-alone state
                s = handleAnchor(i, curAlpha);
                s->anchorEnd_ = true;
                break;

            default:
                break;
        }

        // advance current pointer
        cur += nl_()[i].length_;

        if (s == 0) {
            continue;
        }

        // Set cardinality for this state
        setCard(s, cur, i);

        // Advance to next state which is not a cardinality
        for (j = i + 1; j < size && isCard(nl_()[j].type_); ++j) { }

        // Set end of subexpression indicator on next state if that is
        // an omega, or, or gamma state.
        s->end_ =
            (nl_()[j].type_ == Omega) ||
            (sStack.size() && nl_()[j].type_ == Or) ||
            (sStack.size() && nl_()[j].type_ == Gamma) ?
            (unsigned char)RWRegexStateWeights::EndSub :
            (unsigned char)RWRegexStateWeights::Unset;

        // Set the subexpression indicator if the states stack is non-empty
        // (the stack is non-empty if between an alpha and omega state)
        s->sub_ =
            (sStack.size() > 0)
            ? (unsigned char)RWRegexStateWeights::SubExpression
            : (unsigned char)RWRegexStateWeights::Unset;

        if (alternate) {
            regex_->alternator_.addBetaTransition(s);
            alternate = false;
        }
    }
}

template <class E>
void RWRegexCompiler<E>::addSinkStates()
{
    RWTScopedPointer<RWRegexState<E> > ss(0);
    RWRegexState<E>* s = ss.get(); // Current state

    // Add final good state to the end of the state table that will not
    // match on lookaheads, and will fail to match when we transition to
    // it and invoke match. This state marks the end of a successful search
    RWTScopedPointer<RWRegexEndNode<E> > n(new RWRegexEndNode<E>);
    ss.reset(new RWRegexState<E>(
                 regex_->numStates_, regex_->numStates_ + 1, 0, regex_,
                 RWRegexStateWeights::Unset, n.get(), s));
    n.release();
    s = ss.get();

    s->lookahead_ = s->failed_ = s->primary_ = s;

    regex_->addState(s);
    ss.release();

    // Add another state to the very end of the table that will be the "bad"
    // transition state; if we ever transition here, then we'll be mystified
    // and report a bad match.
    n.reset(new RWRegexEndNode<E>);
    ss.reset(new RWRegexState<E>(
                 regex_->numStates_, regex_->numStates_, 0, regex_,
                 RWRegexStateWeights::Unset, n.get(), s));
    n.release();
    s = ss.get();

    s->lookahead_ = s->failed_ = s->primary_ = s;

    regex_->addState(s);
    ss.release();

    // Patch up all alpha references for safety - if not in a subexpression,
    // then the alpha reference should be to the error end state; also patch
    // up all close references for safety
    size_t size = regex_->numStates_;
    for (size_t i = 0; i < size; i++) {
        if (regex_->states_()[i]->sub_ != RWRegexStateWeights::SubExpression) {
            regex_->states_()[i]->myAlpha_ = s;
        }

        if (regex_->states_()[i]->myClose_ == size_t(-1)) {
            regex_->states_()[i]->myClose_ = size;
        }
    }
}

template <class E>
void RWRegexCompiler<E>::allocateMatchInfo()
{
    regex_->savedMatches_.set(regex_->alphaCount_ + 1);
    regex_->savedLengths_.set(regex_->alphaCount_ + 1);
}

template <class E>
RWRegexState<E>*
RWRegexCompiler<E>::handleAnchor(size_t n, RWRegexState<E>*& alpha)
{
    // Allocate the new state; for anchors, use an alpha lex
    // which returns true always; however, the execution of
    // anchors never reaches the lexer and it is handled in
    // the state itself.
    RWTScopedPointer<RWRegexAlphaLex<E> > lexer(new RWRegexAlphaLex<E>);
    RWTScopedPointer<RWRegexState<E> > state(
        new RWRegexState<E>(regex_->numStates_,
                            // pick up next state from temporary state map
                            stateIDMap_()[nl_()[n].myNext_],
                            nl_()[n].myClose_ != size_t (-1)
                            ? stateIDMap_()[nl_()[n].myClose_]
                            : size_t (-1),
                            regex_,
                            RWRegexStateWeights::Unset,
                            lexer.get(),
                            alpha));
    lexer.release();

    // Add state to state machine states vector
    regex_->addState(state.get());

    // Return the state
    return state.release();
}

template <class E>
RWRegexState<E>*
RWRegexCompiler<E>::handleAlpha(size_t i, RWRegexState<E>*& curAlpha)
{
    RWTScopedPointer<RWRegexAlphaLex<E> > lexer(new RWRegexAlphaLex<E>);
    RWTScopedPointer<RWRegexState<E> > rc(
        new RWRegexState<E>(
            regex_->numStates_,
            stateIDMap_()[nl_()[i].myNext_],
            nl_()[i].myClose_ != size_t(-1)
            ? stateIDMap_()[nl_()[i].myClose_]
            : size_t(-1),
            regex_,
            RWRegexStateWeights::AlphaSub,
            lexer.get(), curAlpha));
    lexer.release();

    curAlpha = rc.get();
    regex_->addState(rc.get());

    return rc.release();
}

template <class E>
RWRegexState<E>*
RWRegexCompiler<E>::handleBeta(size_t i, RWRegexAltSetting& setting)
{
    RWTScopedPointer<RWRegexAlphaLex<E> > lexer(new RWRegexAlphaLex<E>);
    RWTScopedPointer<RWRegexState<E> > rc(
        new RWRegexState<E>(regex_->numStates_,
                            stateIDMap_()[nl_()[i].myNext_],
                            nl_()[i].myClose_ != size_t(-1) ?
                            stateIDMap_()[nl_()[i].myClose_] : size_t(-1),
                            regex_,
                            RWRegexStateWeights::Unset,
                            lexer.get(),
                            0));
    lexer.release();

    regex_->addState(rc.get());
    regex_->alternator_.addBetaState(rc.release(), setting);

    return 0;
}

template <class E>
RWRegexState<E>*
RWRegexCompiler<E>::handleData(
    size_t i, RWRegexLexicon<E>* lex, RWRegexState<E>*& curAlpha)
{
    RWTScopedPointer<RWRegexState<E> > rc(
        new RWRegexState<E>(regex_->numStates_,
                            stateIDMap_()[nl_()[i].myNext_],
                            nl_()[i].myClose_ != size_t(-1) ?
                            stateIDMap_()[nl_()[i].myClose_] : size_t(-1),
                            regex_,
                            RWRegexStateWeights::Unset,
                            lex,
                            curAlpha));
    regex_->addState(rc.get());

    return rc.release();
}

template <class E>
RWRegexState<E>*
RWRegexCompiler<E>::handleSBeta(
    size_t& i, const RChar*& cur, RWRegexState<E>*& curAlpha)
{
    // Bracket set to optimize simple alternation
    RWTScopedPointer<RWRegexBracketSet<E> > obSet(new RWRegexBracketSet<E>);
    for (++i; nl_()[i].type_ != SGamma; i++) {
        switch (nl_()[i].type_) {
            case Bracket:
            case Literal:
            case Dot:
                obSet->addLexicon(makeLexicon(cur, nl_()[i].length_));
                break;

            default:
                break;
        }

        cur += nl_()[i].length_;
    }

    RWTScopedPointer<RWRegexState<E> > rc(
        new RWRegexState<E>(regex_->numStates_,
                            stateIDMap_()[nl_()[i].myNext_],
                            nl_()[i].myClose_ != size_t(-1) ?
                            stateIDMap_()[nl_()[i].myClose_] : size_t(-1),
                            regex_,
                            RWRegexStateWeights::Unset,
                            obSet.get(),
                            curAlpha));
    obSet.release();

    regex_->addState(rc.get());
    return rc.release();
}

/**************************************************************************/

// Pass 2 creates state transitions
template <class E>
void RWRegexCompiler<E>::pass2()
{
    size_t size = regex_->numStates_ - 2;

    for (size_t i = 0; i < size; i++) {

        RWRegexState<E>* cur = regex_->states_()[i];

        size_t weight = size_t (cur->opt_ +
                                cur->sub_ +
                                cur->rep_ +
                                cur->end_ +
                                cur->alpha_);

        cur->primary_   = primary(weight, i);
        cur->failed_    = failed(weight, i);
        cur->lookahead_ = lookahead(weight, i);
    }
}

template <class E>
RWRegexState<E>* RWRegexCompiler<E>::primary(size_t weight, size_t i)
{
    // Table:
    // 0 next, // 16 next,
    // 1 next, // 17 next,
    // 2 null_t, // 18 null_t,
    // 3 null_t, // 19 null_t,

    // 4 self, // 20 self,
    // 5 next, // 21 next,
    // 6 null_t, // 22 null_t,
    // 7 null_t, // 23 null_t,

    // 8 next, // 24 next,
    // 9 next, // 25 next,
    // 10 alpha, // 26 alpha,
    // 11 next, // 27 next,

    // 12 self, // 28 self,
    // 13 next, // 29 next,
    // 14 self, // 30 self,
    // 15 next // 31 next

    RWRegexState<E>* rc = 0;

    switch (weight) {
        case 2:
        case 3:
        case 6:
        case 7:
        case 18:
        case 19:
        case 22:
        case 23:
            rc = regex_->null_t(i);
            break;

        case 4:
        case 12:
        case 14:
        case 20:
        case 28:
        case 30:
            rc = regex_->self(i);
            break;

        case 10:
        case 26:
            rc = regex_->alpha(i);
            break;

        default:
            rc = regex_->next(i);
            break;
    }

    return rc;
}

template <class E>
RWRegexState<E>* RWRegexCompiler<E>::failed(size_t weight, size_t i)
{
    // Table:
    // 0 null_t, // 16 next,
    // 1 close, // 17 close,
    // 2 null_t, // 18 null_t,
    // 3 null_t, // 19 null_t,

    // 4 next, // 20 next,
    // 5 close, // 21 close,
    // 6 null_t, // 22 null_t,
    // 7 null_t, // 23 null_t,

    // 8 alpha, // 24 next,
    // 9 close, // 25 close,
    // 10 alpha, // 26 alpha,
    // 11 alpha, // 27 alpha,

    // 12 next, // 28 next,
    // 13 close, // 29 close,
    // 14 alpha, // 30 alpha,
    // 15 alpha // 31 alpha

    RWRegexState<E>* rc = 0;

    switch (weight) {
        case 0:
        case 2:
        case 3:
        case 6:
        case 7:
        case 18:
        case 19:
        case 22:
        case 23:
            rc = regex_->null_t(i);
            break;

        case 1:
        case 5:
        case 9:
        case 13:
        case 17:
        case 21:
        case 25:
        case 29:
            rc = regex_->close(i);
            break;

        case 4:
        case 12:
        case 16:
        case 20:
        case 24:
        case 28:
            rc = regex_->next(i);
            break;

        default:
            rc = regex_->alpha(i);
            break;
    }

    return rc;
}

template <class E>
RWRegexState<E>* RWRegexCompiler<E>::lookahead(size_t weight, size_t i)
{
    // Table:
    // 0 null_t, // 16 next,
    // 1 null_t, // 17 close,
    // 2 null_t, // 18 null_t,
    // 3 null_t, // 19 null_t,

    // 4 next, // 20 next,
    // 5 close, // 21 close,
    // 6 null_t, // 22 null_t,
    // 7 null_t, // 23 null_t,

    // 8 null_t, // 24 next,
    // 9 null_t, // 25 close,
    // 10 null_t, // 26 next,
    // 11 null_t, // 27 close,

    // 12 next, // 28 next,
    // 13 close, // 29 close,
    // 14 next, // 30 next,
    // 15 close, // 31 close

    RWRegexState<E>* rc = 0;

    switch (weight) {
        case 4:
        case 12:
        case 14:
        case 16:
        case 20:
        case 24:
        case 26:
        case 28:
        case 30:
            rc = regex_->next(i);
            break;

        case 5:
        case 13:
        case 15:
        case 17:
        case 21:
        case 25:
        case 27:
        case 29:
        case 31:
            rc = regex_->close(i);
            break;

        default:
            rc = regex_->null_t(i);
            break;
    }

    return rc;
}

/**************************************************************************/

template <class E>
void RWRegexCompiler<E>::setCard(RWRegexState<E>* s,
                                 const RChar* c,
                                 size_t i)
{
    // Look at next state's type
    RWRegexNodeType type = nl_()[i + 1].type_;

    if (type == Plus) {
        s->opt_ = RWRegexStateWeights::Unset;
        s->rep_ = RWRegexStateWeights::Repeated;
        s->minVisits_ = 1;
        s->maxVisits_ = size_t(-1);
    }
    else if (type == Star) {
        s->opt_ = RWRegexStateWeights::Optional;
        s->rep_ = RWRegexStateWeights::Repeated;
        s->minVisits_ = 0;
        s->maxVisits_ = size_t(-1);
    }
    else if (type == Question) {
        s->opt_ = RWRegexStateWeights::Optional;
        s->rep_ = RWRegexStateWeights::Unset;
        s->minVisits_ = 0;
        s->maxVisits_ = 1;
    }
    else if (type == ArbCard) {
        if (validateArbCard(c, nl_()[i + 1].length_)) {
            s->opt_ = RWRegexStateWeights::Unset;
            s->rep_ = RWRegexStateWeights::Repeated;

            // Parse first cardinality from the string
            size_t len = 0;
            c++;
            const RChar* temp = c;
            while (!RWTRegexTraits<E>::eq(*temp, RWTRegexTraits<E>::comma()) &&
                    !RWTRegexTraits<E>::eq(*temp, RWTRegexTraits<E>::ccurly())) {
                temp++, len++;
            }

            s->minVisits_ = size_t (parseDecNumber(c, len));

            // Parse second cardinality
            if (RWTRegexTraits<E>::eq(*temp, RWTRegexTraits<E>::comma())) {
                if (RWTRegexTraits<E>::eq(
                            *(temp + 1), RWTRegexTraits<E>::ccurly())) {
                    s->maxVisits_ = size_t(-1);
                }
                else {
                    len = 0;
                    temp++;
                    c = temp;
                    while (!RWTRegexTraits<E>::eq(
                                *temp, RWTRegexTraits<E>::ccurly())) {
                        temp++, len++;
                    }

                    s->maxVisits_ = size_t (parseDecNumber(c, len));
                }
            }
            else {
                s->maxVisits_ = s->minVisits_;
            }

            // Ensure proper cardinality range
            if (s->minVisits_ > s->maxVisits_) {
                regexThrow(RWTRegex<E>::InvalidCardinalityRange, c);
            }
        }
        else {
            regexThrow(RWTRegex<E>::InvalidCardinalityRange, c);
        }
    }
    else {
        s->opt_ = RWRegexStateWeights::Unset;
        s->rep_ = RWRegexStateWeights::Unset;
        s->minVisits_ = 1;
        s->maxVisits_ = 1;
    }
}

template <class E>
RWRegexLexicon<E>*
RWRegexCompiler<E>::makeLexicon(const RChar*& str,
                                size_t len)
{
    RWRegexLexicon<E>* rc = 0;

    switch (len) {
            // If length is 1, its either a dot or a literal
        case 1:
            if (RWTRegexTraits<E>::eq(str[0], RWTRegexTraits<E>::dot())) {
                rc = new RWRegexDot<E>;
            }
            else {
                rc = new RWRegexLiteral<E>(str);
            }
            break;

            // Otherwise, its a bracket set or a literal string
        default:
            if (RWTRegexTraits<E>::eq(str[0], RWTRegexTraits<E>::obracket())) {
                rc = parseBracketSet(str, len);
            }
            else {
                rc = new RWRegexLiteralString<E>(str, len, this);
            }
            break;
    }

    return rc;
}

template <class E>
RWRegexLexicon<E>*
RWRegexCompiler<E>::parseBracketSet(const RChar*& str,
                                    size_t len)
{
    RWTScopedPointer<RWRegexBracketSet<E> > rc(new RWRegexBracketSet<E>);

    const RChar* beg = str + 1; // Skip over open bracket
    const RChar* end = str + len - 1; // Don't look at closing bracket

    // Look for complemented bracket set
    if (RWTRegexTraits<E>::eq(*beg, RWTRegexTraits<E>::caret())) {
        rc->complement();
        beg++;
    }

    bool gCur = false;
    bool gNext1 = false;
    bool gNext2 = false;
    RChar cur = RWTRegexTraits<E>::to_char_type(0);
    RChar next1 = RWTRegexTraits<E>::to_char_type(0);
    RChar next2 = RWTRegexTraits<E>::to_char_type(0);

    if (beg < end) {
        gCur = true;
        cur = nextChar(beg, size_t (end - beg));
    }
    else {
        gCur = false;
    }

    if (beg < end) {
        gNext1 = true;
        next1 = nextChar(beg, size_t (end - beg));
    }
    else {
        gNext1 = false;
    }

    if (beg < end) {
        gNext2 = true;
        next2 = nextChar(beg, size_t (end - beg));
    }
    else {
        gNext2 = false;
    }

    while (gCur) {
        if (gNext1 &&
                RWTRegexTraits<E>::eq(next1, RWTRegexTraits<E>::dash()) &&
                gNext2) {
            RWTScopedPointer<RWRegexRange<E> > tmp(new RWRegexRange<E>(&cur, &next2));
            rc->addLexicon(tmp.get());
            tmp.release();
            if (beg < end) {
                gCur = true;
                cur = nextChar(beg, size_t (end - beg));
            }
            else {
                gCur = false;
            }

            if (beg < end) {
                gNext1 = true;
                next1 = nextChar(beg, size_t (end - beg));
            }
            else {
                gNext1 = false;
            }

            if (beg < end) {
                gNext2 = true;
                next2 = nextChar(beg, size_t (end - beg));
            }
            else {
                gNext2 = false;
            }
        }
        else {
            RWTScopedPointer<RWRegexLiteral<E> > tmp(new RWRegexLiteral<E>(&cur));
            rc->addLexicon(tmp.get());
            tmp.release();
            gCur = gNext1;
            cur = next1;
            gNext1 = gNext2;
            next1 = next2;
            if (beg < end) {
                gNext2 = true;
                next2 = nextChar(beg, size_t (end - beg));
            }
            else {
                gNext2 = false;
            }
        }
    }

    return rc.release();
}

template <class E>
int RWRegexCompiler<E>::parseDecNumber(const RChar* c,
                                       size_t len) const
{
    int rc = 0;
    int cur = 0;
    size_t i = 0;
    const RChar* temp = c + len - 1;

    // Trim blanks from end of string
    while (RWTRegexTraits<E>::eq(*temp, RWTRegexTraits<E>::blank())) {
        temp--, len--;
    }

    // Trim blanks at beginning of the number
    temp = c;
    while (RWTRegexTraits<E>::eq(*temp, RWTRegexTraits<E>::blank())) {
        temp++, len--;
    }

    for (; i < len; i++, temp++) {
        const RChar digit = *temp;

        if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::zero())) {
            cur = 0;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::one())) {
            cur = 1;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::two())) {
            cur = 2;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::three())) {
            cur = 3;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::four())) {
            cur = 4;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::five())) {
            cur = 5;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::six())) {
            cur = 6;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::seven())) {
            cur = 7;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::eight())) {
            cur = 8;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::nine())) {
            cur = 9;
        }
        else {
            rc = 0;
            cur = 0;
            i = len;
            regexThrow(RWTRegex<E>::InvalidDecimalDigit, temp);
        }

        rc = rc * 10 + cur;
    }

    return rc;
}

template <class E>
int RWRegexCompiler<E>::parseHexNumber(const RChar* c, size_t len) const
{
    int rc = 0;
    int cur = 0;
    size_t i = 0;
    const RChar* temp = c + len - 1;

    // Trim blanks from end of string
    while (RWTRegexTraits<E>::eq(*temp, RWTRegexTraits<E>::blank())) {
        temp--, len--;
    }

    // Trim blanks at beginning of the number
    temp = c;
    while (RWTRegexTraits<E>::eq(*temp, RWTRegexTraits<E>::blank())) {
        temp++, len--;
    }

    for (; i < len; i++, temp++) {
        const RChar digit = *temp;

        if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::zero())) {
            cur = 0;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::one())) {
            cur = 1;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::two())) {
            cur = 2;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::three())) {
            cur = 3;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::four())) {
            cur = 4;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::five())) {
            cur = 5;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::six())) {
            cur = 6;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::seven())) {
            cur = 7;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::eight())) {
            cur = 8;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::nine())) {
            cur = 9;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::a())) {
            cur = 10;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::A())) {
            cur = 10;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::b())) {
            cur = 11;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::B())) {
            cur = 11;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::c())) {
            cur = 12;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::C())) {
            cur = 12;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::d())) {
            cur = 13;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::D())) {
            cur = 13;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::e())) {
            cur = 14;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::E())) {
            cur = 14;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::f())) {
            cur = 15;
        }
        else if (RWTRegexTraits<E>::eq(digit, RWTRegexTraits<E>::F())) {
            cur = 15;
        }
        else {
            rc = 0;
            cur = 0;
            i = len;
            regexThrow(RWTRegex<E>::InvalidHexNibble, temp);
        }

        rc = rc * 16 + cur;
    }

    return rc;
}

template <class E>
typename RWRegexCompiler<E>::RChar
RWRegexCompiler<E>::nextChar(const RChar*& c, size_t len, void*) const
{
    RChar rc = RWTRegexTraits<E>::null();

    if (RWTRegexTraits<E>::eq(*c, RWTRegexTraits<E>::escape())) {
        c++;
        if (len > 1) {
            rc = *c;

            if (RWTRegexTraits<E>::eq(rc, RWTRegexTraits<E>::x())) {

                if (len >= 4)
                    rc = RWTRegexTraits<E>::to_char_type(
                             parseHexNumber(c + 1, 2));
                else {
                    regexThrow(RWTRegex<E>::InsufficientHex8Data, c);
                }

                c += 3;
            }
            else if (RWTRegexTraits<E>::eq(rc, RWTRegexTraits<E>::u())) {

                if (len >= 6)
                    rc = RWTRegexTraits<E>::to_char_type(
                             parseHexNumber(c + 1, 4));
                else {
                    regexThrow(RWTRegex<E>::InsufficientHex16Data, c);
                }

                c += 5;
            }
            else {

                if (RWTRegexTraits<E>::eq(rc, RWTRegexTraits<E>::n())) {
                    rc = RWTRegexTraits<E>::to_char_type('\n');
                }
                else if (RWTRegexTraits<E>::eq(rc, RWTRegexTraits<E>::r())) {
                    rc = RWTRegexTraits<E>::to_char_type('\r');
                }
                else if (RWTRegexTraits<E>::eq(rc, RWTRegexTraits<E>::t())) {
                    rc = RWTRegexTraits<E>::to_char_type('\t');
                }

                c++;
            }
        }
        else {
            regexThrow(RWTRegex<E>::MissingEscapeSequence, c);
        }
    }
    else {
        rc = *c;
        c++;
    }

    return rc;
}

template <class E>
void RWRegexCompiler<E>::regexThrow(
    size_t status, const RChar* offset) const
{
    static const char messages [][64] = {
        "No error",
        "Missing escape sequence",
        "Invalid hex nibble",
        "Insufficient data for 8-bit hex escape sequence",
        "Insufficient data for 16-bit hex escape sequence",
        "Missing closing bracket",
        "Missing closing curly brace",
        "Missing closing parenthesis",
        "Unmatched closing parenthesis",
        "Invalid sub-expression",
        "Invalid data after |",
        "Invalid data before |",
        "Consecutive cardinalities",
        "Invalid cardinality range",
        "Leading cardinality",
        "Invalid decimal digit",
        "Unmatched closing curly brace"
    };

    RW_ASSERT(status < sizeof messages / sizeof * messages);

    regex_->setStatus(RWRegexErr(messages [status], size_t(status),
                                 size_t (offset - regex_->pattern_())));
    RWTHROW(regex_->getStatus());
}

template <class E>
size_t RWRegexCompiler<E>::endOfBracketSet(
    const RChar* cur, size_t i, size_t start, size_t length)
{
    size_t rc = 0;
    for (rc = start;
            i + rc < length &&
            !(rc != start &&
              RWTRegexTraits<E>::eq(cur[rc], RWTRegexTraits<E>::cbracket()));
            rc++) { }

    return rc;
}

template <class E>
size_t RWRegexCompiler<E>::parseLiteral(
    const RChar* cur, size_t end, RWRegexNodeType& last)
{
    bool go = true;
    const RChar* c = cur;
    const RChar* next = cur;
    const RChar* limit = cur + end;

    // Validate state
    last = validate(last, Literal, cur);

    // Initialize c and next
    nextChar(c, size_t (limit - c));
    next = c;
    nextChar(next, size_t (limit - next));

    // Loop over characters
    while (c < limit && go) {
        if (isSpecial(*c) || (next < limit && isCard(*next))) {
            go = false;
        }
        else {
            c = next;
            nextChar(next, size_t (limit - next));
        }
    }

    return size_t (c - cur);
}

