#ifndef __RWDECIN_TOOLS_H__
#define __RWDECIN_TOOLS_H__


/**********************************************************************
 *
 *
 *
 *   decin.h -- RWDecimalParser: parse decimal fractions from an input stream
 *   Grammar
 *   -------
 *
 *   start      : LPAREN ( "null" | "missing" | "NaN" | "SNaN" ) RPAREN
 *              | LPAREN (PLUS | MINUS)? "infinity" RPAREN
 *              | ((LEAD)? signed) (TRAIL)?
 *
 * #if defined RW_NO_NEW_DECIMAL_PARSER
 *   signed     : LPAREN frac RPAREN
 *              | frac (PERCENT)?
 *   frac       : (PLUS | MINUS)? digits (DECIMAL_POINT (digits)?)?
 * #else
 *   signed     : LPAREN frac RPAREN
 *              | frac
 *   frac       : (PLUS | MINUS)? digits (DECIMAL_POINT (digits)?)? (PERCENT)?
 * #endif
 *
 *   digits     : (DIGIT | DIGIT_SEP) digits
 *
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/decin.h#1 $
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



#include <rw/decport.h>

#include <sstream>

// streambuf derived class
struct RWStringStreambuf
        : public std::streambuf {
    typedef std::streambuf base_class;

    RWStringStreambuf()
        : base_class() {
    }

    RWStringStreambuf(const char* s, size_t len)
        : base_class() {
        this->setg(
            RW_CONST_CAST(char*, s),
            RW_CONST_CAST(char*, s),
            RW_CONST_CAST(char*, s) + len);
    }

    void init(const char* s, size_t len) {
        this->setg(
            RW_CONST_CAST(char*, s),
            RW_CONST_CAST(char*, s),
            RW_CONST_CAST(char*, s) + len);
    }
};

class RW_TOOLS_GLOBAL RWDecimalParserBase
{
protected:

    RWDecimalParserBase()
        : number_(0),
          lead_('$') {
    }

    RWDecimalParserBase(RWDecimalPortable* p)
        : number_(p),
          lead_('$') {
    }

    virtual ~RWDecimalParserBase() {
    }

    typedef int int_type;

    // parser start point
    bool parse();

    // skip white spaces in the stream
    bool skipws();

    // character fetching
    virtual int_type(getc)() = 0;
    virtual int_type nextc() = 0;

    // consume various literals
    bool consume_right_paren();
    bool consume_frac_or_nonum();
    bool consume_frac();
    bool consume_nonum();
    bool consume_sign();
    bool consume_percent();

    // the number
    RWDecimalPortable* number_;

    // EOF
    int_type eof_;

    // adjustments
    char lead_;
};

class RW_TOOLS_GLOBAL RWDecimalParser : public RWDecimalParserBase
{
public:

    typedef RWDecimalParserBase::int_type int_type;

    RWDecimalParser(RWDecimalPortable* p);

    virtual ~RWDecimalParser() {
    }

    bool
    operator()(std::istream&);

protected:

    RWDecimalParser()
        : RWDecimalParserBase(0) { }

    // character fetching
    virtual int_type(getc)() {
        return input_->sgetc();
    }

    virtual int_type nextc() {
        if ((this->getc)() == eof_) {
            return eof_;
        }

        input_->sbumpc();
        return (this->getc)();
    }

    void restart(std::streambuf* sb) {
        input_ = sb;
    }

    // the stream buffer for input
    std::streambuf* input_;
};

class RW_TOOLS_GLOBAL RWDecimalStringParser : public RWDecimalParserBase
{
public:

    typedef RWDecimalParserBase::int_type int_type;

    RWDecimalStringParser(RWDecimalPortable* p);

    virtual ~RWDecimalStringParser() {
    }

    bool
    operator()(const char*);

protected:

    RWDecimalStringParser()
        : RWDecimalParserBase(0) { }

    // character fetching
    virtual int_type(getc)() {
        if (position_ < length_) {
            return (unsigned char)input_ [position_];
        }

        return eof_;
    }

    virtual int_type nextc() {
        if (position_ < length_) {
            position_++;
        }

        return (getc)();
    }

    void restart() {
    }

    // the char array
    const char* input_;
    size_t      length_;
    size_t      position_;
};

#endif // __RWDECIN_TOOLS_H__
