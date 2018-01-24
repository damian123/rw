#ifndef RW_REGEX_LEXICON_H
#define RW_REGEX_LEXICON_H

/**********************************************************************
 *
 * rw_regex_lexicon.h - contains the class definitions for the regex lexicons
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regex/lexicon.h#1 $
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
#include <rw/tools/regex/bufman.h>
#include <rw/tools/regex/vector.h>
#include <rw/tools/regex/compiler.h>

/**************************************************************************/
/*                                                                        */
/* Class Name                                                             */
/* ==========                                                             */
/*                                                                        */
/* RWRegexLexicon                                                         */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This class serves as the interface for lexicons used in regular        */
/* expressions.  Lexicons represent lexical elements of the pattern       */
/* against which we can match part of the input string, including         */
/* literal characters, literal strings, dot wildcards, ranges, etc.       */
/*                                                                        */
/**************************************************************************/

template <class E>
class RWRegexLexicon
{

public:

    virtual ~RWRegexLexicon() { }

    virtual RWRegexLexicon<E>* clone(RWRegexImp<E>*) = 0;

    virtual bool match(
        const typename RWTRegexTraits<E>::Char*&,
        const typename RWTRegexTraits<E>::Char*&) = 0;
};



/**************************************************************************/
/*                                                                        */
/* Class Name                                                             */
/* ==========                                                             */
/*                                                                        */
/* RWRegexLiteral                                                         */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This type of lexicon represents a single literal character             */
/*                                                                        */
/**************************************************************************/

template <class E>
class RWRegexLiteral : public RWRegexLexicon<E>
{

private:

    // Literal character represented by this lexicon
    typename RWTRegexTraits<E>::Char character_;

public:

    RWRegexLiteral() : character_(0) { }

    RWRegexLiteral(const typename RWTRegexTraits<E>::Char* newC)
        : character_(*newC) { }

    virtual ~RWRegexLiteral() { }

    virtual RWRegexLexicon<E>* clone(RWRegexImp<E>*) {
        return new RWRegexLiteral<E>(&character_);
    }

    virtual bool match(
        const typename RWTRegexTraits<E>::Char*& c,
        const typename RWTRegexTraits<E>::Char*& eos) {
        if (!c) {
            return false;
        }
        bool rc = (c < eos && RWTRegexTraits<E>::eq(*c, character_));

        if (rc) {
            c++;
        }

        return rc;
    }
};



/**************************************************************************/
/*                                                                        */
/* Class Name                                                             */
/* ==========                                                             */
/*                                                                        */
/* RWRegexLiteralString                                                   */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This type of lexicon represents a string of literal characters that    */
/* can be matched without requiring a state transition.                   */
/*                                                                        */
/**************************************************************************/
template <class E>
class RWRegexLiteralString : public RWRegexLexicon<E>
{

private:

    typedef typename RWTRegexTraits<E>::Char RChar;

    // Default max length for a literal string, in characters
    enum { DefLength = 16 };

    // Actual length of literal string, in character, and the
    // literal string itself
    size_t length_;
    RWBufMan<typename RWTRegexTraits<E>::Char, DefLength> str_;

public:

    RWRegexLiteralString() :
        length_(0) {
        str_.set(0);
    }

    RWRegexLiteralString(const RChar* newString,
                         size_t newLength,
                         const RWRegexCompiler<E>* compiler) {
        size_t i = 0;
        const RChar* end = newString + newLength;
        const RChar* c = newString;

        str_.set(newLength);
        for (; c < end; i++) {
            str_()[i] = compiler->nextChar(c, size_t (end - c));
        }

        length_ = i;
    }

    virtual ~RWRegexLiteralString() { }

    // Clone the literal string: create a buffer, and copy character by
    // character
    virtual RWRegexLexicon<E>* clone(RWRegexImp<E>*) {

        RWTScopedPointer<RWRegexLiteralString<E> > rc(new RWRegexLiteralString<E>);

        rc->length_ = length_;
        rc->str_.set(length_);

        for (size_t i = 0; i < length_; i++) {
            rc->str_()[i] = str_()[i];
        }

        return rc.release();
    }

    // Match a literal string against the current input string position,
    // comparing character by character.
    virtual bool match(
        const typename RWTRegexTraits<E>::Char*& c,
        const typename RWTRegexTraits<E>::Char*& eos) {
        if (!c) {
            return false;
        }

        // *** Assume null-terminated c; no NULL will be in buf;
        // *** memcmp will stop on NULL; no seg fault will occur
        bool rc = c < eos && length_ <= size_t(eos - c);

        for (size_t i = 0; rc && i < length_; i++) {
            rc = RWTRegexTraits<E>::eq(str_()[i], c[i]);
        }

        if (rc) {
            c += length_;
        }

        return rc;
    }
};

/**************************************************************************/
/*                                                                        */
/* Class Name                                                             */
/* ==========                                                             */
/*                                                                        */
/* RWRegexDot                                                             */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This type of lexicon represents a dot wildcard                         */
/*                                                                        */
/**************************************************************************/

template <class E>
class RWRegexDot : public RWRegexLexicon<E>
{

public:

    RWRegexDot() { }

    virtual ~RWRegexDot() { }

    virtual RWRegexLexicon<E>* clone(RWRegexImp<E>*) {
        return new RWRegexDot<E>;
    }

    virtual bool match(
        const typename RWTRegexTraits<E>::Char*& c,
        const typename RWTRegexTraits<E>::Char*& eos) {
        if (!c) {
            return false;
        }

        bool rc =
            (c < eos &&
             !RWTRegexTraits<E>::eq(*c, RWTRegexTraits<E>::newline()));

        if (rc) {
            c++;
        }

        return rc;
    }
};

/**************************************************************************/
/*                                                                        */
/* Class Name                                                             */
/* ==========                                                             */
/*                                                                        */
/* RWRegexRange                                                           */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This type of lexicon represents a range of valid values, as indicated  */
/* in a bracket expression. Based on a backward compatibility             */
/* requirement, range expressions may be specified with the lesser of     */
/* the two range endpoints specified as the second endpoint (the range    */
/* endpoints may be specified out of order).                              */
/*                                                                        */
/**************************************************************************/
template <class E>
class RWRegexRange : public RWRegexLexicon<E>
{
private:
    // Min and max for the range expression
    typename RWTRegexTraits<E>::Char min_;
    typename RWTRegexTraits<E>::Char max_;

public:

    RWRegexRange() : min_(0), max_(0) { }

    RWRegexRange(const typename RWTRegexTraits<E>::Char* newMin,
                 const typename RWTRegexTraits<E>::Char* newMax) {

        // Swap min and max if specified "out of order"
        if (RWTRegexTraits<E>::lt(*newMin, *newMax)) {
            min_ = *newMin;
            max_ = *newMax;
        }
        else {
            min_ = *newMax;
            max_ = *newMin;
        }
    }

    virtual ~RWRegexRange() { }

    virtual RWRegexLexicon<E>* clone(RWRegexImp<E>*) {
        return new RWRegexRange<E>(&min_, &max_);
    }

    virtual bool match(
        const typename RWTRegexTraits<E>::Char*& c,
        const typename RWTRegexTraits<E>::Char*& eos) {
        if (!c) {
            return false;
        }

        bool rc =
            (c < eos &&
             (RWTRegexTraits<E>::lt(min_, *c)  ||
              RWTRegexTraits<E>::eq(min_, *c)) &&
             (RWTRegexTraits<E>::lt(*c, max_)  ||
              RWTRegexTraits<E>::eq(*c, max_)));

        if (rc) {
            c++;
        }

        return rc;
    }
};

/**************************************************************************/
/*                                                                        */
/* Class Name                                                             */
/* ==========                                                             */
/*                                                                        */
/* RWRegexBracketSet                                                      */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This lexicon represents a bracket expression                           */
/*                                                                        */
/**************************************************************************/
template <class E>
class RWRegexBracketSet : public RWRegexLexicon<E>
{

private:

    // Set of bracket set lexicons
    RWRegVec<RWRegexLexicon<E>*> data_;

    // Is the bracket set a complemented set (from circumflex (^) in 1st pos)
    bool complement_;

public:

    RWRegexBracketSet() : complement_(false) { }

    virtual ~RWRegexBracketSet() {
        while (data_.size()) {
            delete data_.back();
            data_.pop_back();
        }

        data_.clear();
    }

    virtual RWRegexLexicon<E>* clone(RWRegexImp<E>* regex) {
        RWTScopedPointer<RWRegexBracketSet<E> > rc(new RWRegexBracketSet<E>);
        rc->complement_ = complement_;
        for (size_t i = 0;
                i < data_.size();
                rc->data_.push_back(data_[i++]->clone(regex))) { }

        return rc.release();
    }

    virtual bool match(
        const typename RWTRegexTraits<E>::Char*& c,
        const typename RWTRegexTraits<E>::Char*& eos) {

        bool rc = false;

        const typename RWTRegexTraits<E>::Char* temp = c;

        size_t limit = data_.size();

        if (c < eos) {
            size_t i = 0;
            for (; !rc && i < limit; rc = data_[i++]->match(temp, eos)) { }

            rc = rc != complement_;
            if (rc) {
                c++;
            }
        }

        return rc;
    }

    void complement(bool set = true) {
        complement_ = set;
    }

    void addLexicon(RWRegexLexicon<E>* lex) {
        data_.push_back(lex);
    }
};

/**************************************************************************/
/*                                                                        */
/* Class Name                                                             */
/* ==========                                                             */
/*                                                                        */
/* RWAlphaLex                                                             */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This lexicon is used in alpha states, and is simply always reports     */
/* a match.  The match algorithm does not use the alpha lex, but          */
/* delegates the match to its primary state transition, so this return    */
/* value is not used; the alpha lex is used in preference to a NULL       */
/* pointer, and is a fringe application of the NULL object pattern.       */
/*                                                                        */
/**************************************************************************/
template <class E>
class RWRegexAlphaLex : public RWRegexLexicon<E>
{
public:
    RWRegexAlphaLex() { }
    virtual ~RWRegexAlphaLex() { }

    virtual RWRegexLexicon<E>* clone(RWRegexImp<E>*) {
        return new RWRegexAlphaLex<E>;
    }

    virtual bool match(const typename RWTRegexTraits<E>::Char*&,
                       const typename RWTRegexTraits<E>::Char*&) {
        return true;
    }
};

/**************************************************************************/
/*                                                                        */
/* Class Name                                                             */
/* ==========                                                             */
/*                                                                        */
/* RWRegexEndNode                                                         */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This type of lexicon is used in the end states for the set of regex    */
/* states.  End nodes can never match against any input string character  */
/* and therefore always return false.                                     */
/*                                                                        */
/**************************************************************************/
template <class E>
class RWRegexEndNode : public RWRegexLexicon<E>
{

public:

    RWRegexEndNode() { }

    virtual ~RWRegexEndNode() { }

    virtual RWRegexLexicon<E>* clone(RWRegexImp<E>*) {
        return new RWRegexEndNode<E>;
    }

    virtual bool match(const typename RWTRegexTraits<E>::Char*&,
                       const typename RWTRegexTraits<E>::Char*&) {
        return false;
    }
};

#endif
