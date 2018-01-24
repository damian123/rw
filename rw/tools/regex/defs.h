#ifndef RW_REGEX_DEFS_H
#define RW_REGEX_DEFS_H



/**********************************************************************
 *
 * defs.h - contains the class definitions for the regex character
 *          traits classes
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regex/defs.h#1 $
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

/**
 * @ingroup string_processing_classes
 *
 * @brief Defines static, inline methods for returning specific
 * regular expression character values.
 *
 * Class \link RWTRegexTraits RWTRegexTraits<T>\endlink defines static, inline
 * methods for returning specific RE character values. These values
 * \e must be defined and include
 *
 * -  each of the regular expression special characters
 * -  numeric and hex digits
 * -  some escape sequences
 *
 * In addition, any class used as a traits class \e must
 * define some special methods for converting and comparing characters.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tools/regex.h>
 * #include <rw/cstring.h>
 * (Abstract base class. No instances are created.)
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template <class T>
class RWTRegexTraits
{
public:
    /**
     * Typedef for the character type.
     */
    typedef T         Char;

    /**
     * Returns \c '\\0'.
     */
    static Char       null() {
        return Char('\0');
    }

    /**
     * Returns <tt>'('</tt>.
     */
    static Char       oparen() {
        return Char('(');
    }

    /**
     * Returns <tt>')'</tt>.
     */
    static Char       cparen() {
        return Char(')');
    }

    /**
     * Returns <tt>'['</tt>.
     */
    static Char       obracket() {
        return Char('[');
    }

    /**
     * Returns <tt>']'</tt>.
     */
    static Char       cbracket() {
        return Char(']');
    }

    /**
     * Returns <tt>'{'</tt>.
     */
    static Char       ocurly() {
        return Char('{');
    }

    /**
     * Returns <tt>'}'</tt>.
     */
    static Char       ccurly() {
        return Char('}');
    }

    /**
     * Returns <tt>'.'</tt>.
     */
    static Char       dot() {
        return Char('.');
    }

    /**
     * Returns \c '*'.
     */
    static Char       star() {
        return Char('*');
    }

    /**
     * Returns \c '+'.
     */
    static Char       plus() {
        return Char('+');
    }

    /**
     * Returns \c '?'.
     */
    static Char       question() {
        return Char('?');
    }

    /**
     * Returns \c '|'.
     */
    static Char       vbar() {
        return Char('|');
    }

    /**
     * Returns \c '-'.
     */
    static Char       dash() {
        return Char('-');
    }

    /**
     * Returns <tt>','</tt> .
     */
    static Char       comma() {
        return Char(',');
    }

    /**
     * Returns \c '^'.
     */
    static Char       caret() {
        return Char('^');
    }

    /**
     * Returns \c '\$'.
     */
    static Char       dollar() {
        return Char('$');
    }

    /**
     * Returns \c '\\'.
     */
    static Char       escape() {
        return Char('\\');
    }

    /**
     * Returns \c '\\n'.
     */
    static Char       newline() {
        return Char('\n');
    }

    /**
     * Returns ' '.
     */
    static Char       blank() {
        return Char(' ');
    }

    /**
     * Returns \c '0'.
     */
    static Char       zero() {
        return Char('0');
    }

    /**
     * Returns \c '1'.
     */
    static Char       one() {
        return Char('1');
    }

    /**
     * Returns \c '2'.
     */
    static Char       two() {
        return Char('2');
    }

    /**
     * Returns \c '3'.
     */
    static Char       three() {
        return Char('3');
    }

    /**
     * Returns \c '4'.
     */
    static Char       four() {
        return Char('4');
    }

    /**
     * Returns \c '5'.
     */
    static Char       five() {
        return Char('5');
    }

    /**
     * Returns \c '6'.
     */
    static Char       six() {
        return Char('6');
    }

    /**
     * Returns \c '7'.
     */
    static Char       seven() {
        return Char('7');
    }

    /**
     * Returns \c '8'.
     */
    static Char       eight() {
        return Char('8');
    }

    /**
     * Returns \c '9'.
     */
    static Char       nine() {
        return Char('9');
    }

    /**
     * Returns \c 'a'.
     */
    static Char       a() {
        return Char('a');
    }

    /**
     * Returns \c 'A'.
     */
    static Char       A() {
        return Char('A');
    }

    /**
     * Returns \c 'b'.
     */
    static Char       b() {
        return Char('b');
    }

    /**
     * Returns \c 'B'.
     */
    static Char       B() {
        return Char('B');
    }

    /**
     * Returns \c 'c'.
     */
    static Char       c() {
        return Char('c');
    }

    /**
     * Returns \c 'C'.
     */
    static Char       C() {
        return Char('C');
    }

    /**
     * Returns \c 'd'.
     */
    static Char       d() {
        return Char('d');
    }

    /**
     * Returns \c 'D'.
     */
    static Char       D() {
        return Char('D');
    }

    /**
     * Returns \c 'e'.
     */
    static Char       e() {
        return Char('e');
    }

    /**
     * Returns \c 'E'.
     */
    static Char       E() {
        return Char('E');
    }

    /**
     * Returns \c 'f'.
     */
    static Char       f() {
        return Char('f');
    }

    /**
     * Returns \c 'F'.
     */
    static Char       F() {
        return Char('F');
    }

    /**
     * Returns \c 'x'.
     */
    static Char       x() {
        return Char('x');
    }

    /**
     * Returns \c 'u'.
     */
    static Char       u() {
        return Char('u');
    }

    /**
     * Returns \c 'n'.
     */
    static Char       n() {
        return Char('n');
    }

    /**
     * Returns \c 'r'.
     */
    static Char       r() {
        return Char('r');
    }

    /**
     * Returns \c 't'.
     */
    static Char       t() {
        return Char('t');
    }


private:
    RWTRegexTraits() {}
    ~RWTRegexTraits() {}
};

/**************************************************************************/
/*                                                                        */
/* Class Name                                                             */
/* ==========                                                             */
/*                                                                        */
/* RWTRegexTraits<char>                                                   */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This is a specialization of RWTRegexTraits on type char                */
/*                                                                        */
/**************************************************************************/
template <>
class RWTRegexTraits<char>
{
public:
    typedef char   Char;
    typedef int    IntType;

    static inline void assign(char& x, const char& y) {
        x = y;
    }

    static inline bool eq(const char& x, const char& y) {
        return (x == y);
    }

    static inline bool lt(const char& x, const char& y) {
        return (x < y);
    }

    static inline char to_char_type(const IntType& i) {
        return char(i);
    }

    static inline IntType to_int_type(const char& c) {
        return IntType(c);
    }

    static Char       null() {
        return '\0';
    }
    static Char       oparen() {
        return '(';
    }
    static Char       cparen() {
        return ')';
    }
    static Char       obracket() {
        return '[';
    }
    static Char       cbracket() {
        return ']';
    }
    static Char       ocurly() {
        return '{';
    }
    static Char       ccurly() {
        return '}';
    }
    static Char       dot() {
        return '.';
    }
    static Char       star() {
        return '*';
    }
    static Char       plus() {
        return '+';
    }
    static Char       question() {
        return '?';
    }
    static Char       vbar() {
        return '|';
    }
    static Char       dash() {
        return '-';
    }
    static Char       comma() {
        return ',';
    }
    static Char       caret() {
        return '^';
    }
    static Char       dollar() {
        return '$';
    }
    static Char       escape() {
        return '\\';
    }
    static Char       newline() {
        return '\n';
    }
    static Char       blank() {
        return ' ';
    }

    static Char       zero() {
        return '0';
    }
    static Char       one() {
        return '1';
    }
    static Char       two() {
        return '2';
    }
    static Char       three() {
        return '3';
    }
    static Char       four() {
        return '4';
    }
    static Char       five() {
        return '5';
    }
    static Char       six() {
        return '6';
    }
    static Char       seven() {
        return '7';
    }
    static Char       eight() {
        return '8';
    }
    static Char       nine() {
        return '9';
    }
    static Char       a() {
        return 'a';
    }
    static Char       A() {
        return 'A';
    }
    static Char       b() {
        return 'b';
    }
    static Char       B() {
        return 'B';
    }
    static Char       c() {
        return 'c';
    }
    static Char       C() {
        return 'C';
    }
    static Char       d() {
        return 'd';
    }
    static Char       D() {
        return 'D';
    }
    static Char       e() {
        return 'e';
    }
    static Char       E() {
        return 'E';
    }
    static Char       f() {
        return 'f';
    }
    static Char       F() {
        return 'F';
    }
    static Char       x() {
        return 'x';
    }
    static Char       u() {
        return 'u';
    }
    static Char       n() {
        return 'n';
    }
    static Char       r() {
        return 'r';
    }
    static Char       t() {
        return 't';
    }


private:
    RWTRegexTraits() {}
    ~RWTRegexTraits() {}
};

/**************************************************************************/
/*                                                                        */
/* Class Name                                                             */
/* ==========                                                             */
/*                                                                        */
/* RWTRegexTraits<wchar_t>                                                */
/*                                                                        */
/* Description                                                            */
/* ===========                                                            */
/*                                                                        */
/* This class is a specialization of RWTRegexTraits on wchar_t.           */
/*                                                                        */
/**************************************************************************/
template <>
class RWTRegexTraits<wchar_t>
{
public:
    typedef wchar_t   Char;
    typedef int       IntType;

    static inline void assign(wchar_t& x, const wchar_t& y) {
        x = y;
    }

    static inline bool eq(const wchar_t& x, const wchar_t& y) {
        return (x == y);
    }

    static inline bool lt(const wchar_t& x, const wchar_t& y) {
        return (x < y);
    }

    static inline wchar_t to_char_type(const IntType& i) {
        return wchar_t(i);
    }

    static inline IntType to_int_type(const wchar_t& c) {
        return IntType(c);
    }

    static Char       null() {
        return L'\0';
    }
    static Char       oparen() {
        return L'(';
    }
    static Char       cparen() {
        return L')';
    }
    static Char       obracket() {
        return L'[';
    }
    static Char       cbracket() {
        return L']';
    }
    static Char       ocurly() {
        return L'{';
    }
    static Char       ccurly() {
        return L'}';
    }
    static Char       dot() {
        return L'.';
    }
    static Char       star() {
        return L'*';
    }
    static Char       plus() {
        return L'+';
    }
    static Char       question() {
        return L'?';
    }
    static Char       vbar() {
        return L'|';
    }
    static Char       dash() {
        return L'-';
    }
    static Char       comma() {
        return L',';
    }
    static Char       caret() {
        return L'^';
    }
    static Char       dollar() {
        return L'$';
    }
    static Char       escape() {
        return L'\\';
    }
    static Char       newline() {
        return L'\n';
    }
    static Char       blank() {
        return L' ';
    }

    static Char       zero() {
        return L'0';
    }
    static Char       one() {
        return L'1';
    }
    static Char       two() {
        return L'2';
    }
    static Char       three() {
        return L'3';
    }
    static Char       four() {
        return L'4';
    }
    static Char       five() {
        return L'5';
    }
    static Char       six() {
        return L'6';
    }
    static Char       seven() {
        return L'7';
    }
    static Char       eight() {
        return L'8';
    }
    static Char       nine() {
        return L'9';
    }
    static Char       a() {
        return L'a';
    }
    static Char       A() {
        return L'A';
    }
    static Char       b() {
        return L'b';
    }
    static Char       B() {
        return L'B';
    }
    static Char       c() {
        return L'c';
    }
    static Char       C() {
        return L'C';
    }
    static Char       d() {
        return L'd';
    }
    static Char       D() {
        return L'D';
    }
    static Char       e() {
        return L'e';
    }
    static Char       E() {
        return L'E';
    }
    static Char       f() {
        return L'f';
    }
    static Char       F() {
        return L'F';
    }
    static Char       x() {
        return L'x';
    }
    static Char       u() {
        return L'u';
    }
    static Char       n() {
        return L'n';
    }
    static Char       r() {
        return L'r';
    }
    static Char       t() {
        return L't';
    }

private:
    RWTRegexTraits() {}
    ~RWTRegexTraits() {}
};


#endif
