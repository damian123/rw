#ifndef RW_CURRENCY_DECPIC_H
#define RW_CURRENCY_DECPIC_H

/***************************************************************************
 *
 * RWPictureParser: parse decimal fractions from an input stream
 * RWPictureLexer:  used by above to lexically analyze input stream
 *
 * See .cpp file for a description of the grammar.
 *
 * This is an implementation class for reading in and analyzing output
 * format pictures.
 *
 * $Id: //money/13/rw/decpic.h#1 $
 *
 * Copyright (c) 1993-2015 Rogue Wave Software, Inc.  All Rights Reserved.
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
 ***************************************************************************/

/* for backward compatibility */
#ifndef RW_MONEY_DECPIC_H
#  define RW_MONEY_DECPIC_H
#endif

#include <rw/rstream.h>
#include <rw/cstring.h>
#include <rw/decio.h>
#include <rw/decport.h>

/*
 * Lexical analysis of format strings.  The lexical analysis is pretty
 * simple, since each token is a single character.  The *STR tokens are
 * sort of special, they are used to consume a selected bunch of tokens
 * out of the input string.
 */

class RWPictureLexer
{
public:
    enum Token { tok_at, tok_dec, tok_sep, tok_pad, tok_code, tok_other, tok_none, tok_endoffile };
    enum TokenString { toks_notatstr, toks_notcodestr, toks_reststr };
    RWPictureLexer();
    void      reset(const RWCString&);
    Token     token();       // token at current position
    void      consume(Token);// consumes the indicated token, or returns false if wrong token
    RWCString consumeString(TokenString);
    char      code();        // if token() is tok_code, this is the character code read in
    bool contains(char);// back door: does the input contain c
private:
    RWCString input_;        // string being lexed
    int       index_;        // last char read in input_
    Token     token_;        // current token
    void      nextToken();   // advance to next token
};

/*
 * Parsing of format strings.  Each of the private member functions
 * consumes its associated non-terminal, or returns false if no
 * production matches the input stream.
 */

class RWPictureParser
{
public:
    RWPictureParser();
    RWDecimalFormat operator()(const char*);  // parse a picture from a string
private:
    RWDecimalFormat format_;         // here's what we're building
    bool       setNegativeSign_;// was a negative sign explicitly set?
    unsigned        rightLen_;       // number of characters in right part
    RWPictureLexer  lexer_;          // here's where tokens come from
    void start();
    void pic();
    void left();
    void right();
    void padding();
    void farLeftCodes();
    void farRightCodes();
    void nearLeftCodes();
    void nearRightCodes();

    // helper functions.  Used in the implementation.
    typedef RWCString(RWDecimalFormat::*RWDecimalFormatGetFtn)(RWDecimalFormat::Sign);
    bool nearRightCodesLeavesPad(RWPictureLexer lex);
    void      increaseFormatWidth(unsigned amount);
    void      makeTextLengthsSame();  // ensure text attributes same length for +ve, -ve
    void      pad(RWDecimalFormatGetFtn,
                  void (RWDecimalFormat::*set)(const RWCString&, RWDecimalFormat::Sign),
                  int, RWDecimalFormat::Sign, char);
};

#endif // RW_CURRENCY_DECPIC_H
