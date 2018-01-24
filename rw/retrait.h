#ifndef RW_TOOLS_RETRAIT_H
#define RW_TOOLS_RETRAIT_H

/**********************************************************************
 *
 * Template trait declarations for class RWTRegularExpression<char>
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/retrait.h#1 $
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
#include <rw/cstring.h>

#if !defined(RW_NO_EXCEPTIONS)

#  include <rw/tvordvec.h>
#  include <rw/tbitvec.h>

/////////////////////////////////////////////////////////////////////////////
//
// RWTRegularExpressionTraits
//
//  Trait classes that enables RWTRegularExpression to specialize on
//  different types.
//
/////////////////////////////////////////////////////////////////////////////

template<class charT>
class RWTRegularExpressionTraits { };

#  undef RWC
#  define RWC(name, value) static char name() {return(value);}

#  ifndef RW_NO_NEW_TEMPLATE_SYNTAX
template<>
#  endif
class RW_TOOLS_GLOBAL RWTRegularExpressionTraits<char>
{
public:

    // Used in RWTRegularExpression<charT> to specialize on charT
    typedef RWCString                                     tStringT;

    // Extended regular expression metacharacters.
    RWC(a,              'a');
    RWC(b,              'b');
    RWC(bar,            '|');
    RWC(backslash,      '\\');
    RWC(backspace,      '\b');
    RWC(c,              'c');
    RWC(colon,          ':');
    RWC(closeBrace,     '}');
    RWC(closeBracket,   ']');
    RWC(closeParen,     ')');
    RWC(d,              'd');
    RWC(dash,           '-');
    RWC(dollar,         '$');
    RWC(dot,            '.');
    RWC(e,              'e');
    RWC(eight,          '8');
    RWC(equal,          '=');
    RWC(f,              'f');
    RWC(five,           '5');
    RWC(formfeed,       '\f');
    RWC(four,           '4');
    RWC(hat,            '^');
    RWC(n,              'n');
    RWC(newlineChar,    '\n');
    RWC(nine,           '9');
    RWC(one,            '1');
    RWC(openBrace,      '{');
    RWC(openBracket,    '[');
    RWC(openParen,      '(');
    RWC(plus,           '+');
    RWC(questionMark,   '?');
    RWC(r,              'r');
    RWC(seven,          '7');
    RWC(six,            '6');
    RWC(star,           '*');
    RWC(t,              't');
    RWC(tab,            '\t');
    RWC(three,          '3');
    RWC(two,            '2');
    RWC(x,              'x');
    RWC(zero,           '0');

#  if defined(RW_NO_POSIX_RE)

    typedef RWTBitVec<256>                    symbolSetT;

    typedef RWTValOrderedVector<symbolSetT>   symbolSetTVector;

    // Used in RWTRegularExpression<charT> to match charT input symbols
    // in range expressions and character class expressions.

    static bool  isChar() {
        return true;   // Used to see if can use "fast" RE.
    }

    // nullCharT returns a null character value for any symbol type.
    static char     nullCharT() {
        return 0;
    }

    // Adds character class expression match maps, and returns
    //   index of new symbol set entry:
    static size_t   addSymbolSet
    (
        symbolSetTVector&         symbolSetTVector, // Ref to symbol set array.
        const tStringT&           setString     // Character class string in RE.
    );

    // Returns true if "symbol" has a match in symbol set
    //   pointed to by "setIndex":
    static bool isMatchedInSymbolSet
    (
        // Symbol set of character class expression.  Can't be const
        // because RWTBitVec::operator[] isn't const.
        symbolSetT&       symbolSet,
        const unsigned char        symbol
    ) {
        size_t  i(symbol);

        if (symbolSet.testBit(i)) {
            return true;
        }

        return false;
    }
#  endif  // RW_NO_POSIX_RE
};  // RWTRegularExpressionTraits<char>

#  undef RWC


#endif /* !defined(RW_NO_EXCEPTIONS) */

#endif /* RW_TOOLS_RETRAIT_H */
