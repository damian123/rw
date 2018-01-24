#ifndef RW_TOOLS_WTOKEN_H
#define RW_TOOLS_WTOKEN_H



/**********************************************************************
 *
 * RWTokenizer --- converts strings into sequences of tokens
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/wtoken.h#1 $
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
#include <rw/edefs.h>
#include <rw/wstring.h>

// Forward declare regular expression class template
template <class E> class RWTRegex;

/**
 * @ingroup string_processing_classes
 * @brief Breaks up a string into separate tokens, delimited by
 * arbitrary whitespace.
 *
 * Class RWWTokenizer is designed to break a string up into separate
 * tokens, delimited by arbitrary whitespace.  It can be thought
 * of as an iterator for strings and as an alternative to the C Standard
 * Library function \c wstok() which has the unfortunate side effect
 * of changing the string being tokenized.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/wtoken.h>
 * RWWString str("a string of tokens", RWWString::ascii);
 * RWWTokenizer(str);  // Lex the above string
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/wtoken.h>
 *
 * int main ()
 * {
 *     RWWString a(L"Something is rotten in the state of Denmark");
 *
 *     RWWTokenizer next(a); // Tokenize the string a
 *     RWWString token; // Will receive each token
 *
 *     // Advance until the null string is returned:
 *     while (!(token=next()).isNull())
 *         std::cout << token << "\n";
 *     return 0;
 * }
 * @endcode
 * *
 * Program output (assuming your platform displays wide characters
 * as US-ASCII if they are in the US-ASCII character set):
 *
 * @code
 *
 * Something
 * is
 * rotten
 * in
 * the
 * state
 * of
 * Denmark
 * @endcode
 *
 */
class RW_TOOLS_GLOBAL RWWTokenizer
{
private:
    RWWString        theString_;
    size_t           place_;

public:

    /**
     * Constructs a tokenizer to lex the string \a s.
     */
    RWWTokenizer(const RWWString& s);

    /**
     * Copy constructor. The created tokenizer copies the data from \a rhs.
     */
    RWWTokenizer(const RWWTokenizer& rhs)
        : theString_(rhs.theString_), place_(rhs.place_) {
    }

    /**
     * Assignment operator. The tokenizer copies the data from \a rhs.
     * Returns a reference to self.
     */
    RWWTokenizer& operator=(const RWWTokenizer& rhs) {
        if (&rhs != this) {
            RWWTokenizer(rhs).swap(*this);
        }
        return *this;
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed instance takes ownership of the
     * data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWWTokenizer(RWWTokenizer && rhs)
        : place_(0) {
        rhs.swap(*this);
    }

    /**
     * Move assignment. Self takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWWTokenizer& operator=(RWWTokenizer && rhs) {
        if (&rhs != this) {
            rhs.swap(*this);
        }
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    virtual ~RWWTokenizer();

    /**
     * Returns \c true if the last token from the search string has been
     * extracted, otherwise \c false. When using the function call operator
     * interface, this is the same as the last non-empty token having
     * been returned.
     */
    bool done() const;

    /**
     * Advances to the next token and returns it as a wide substring.
     * The tokens are delimited by any wide character in \a s, or
     * any embedded wide null.
     */
    RWWSubString          operator()(const wchar_t* s);

    /**
     * Advances to the next token and returns it as a substring.  The
     * tokens are delimited by any of the first \a num wide characters
     * in \a s.  Buffer \a s may contain embedded nulls, and must contain
     * at least \a num wide characters.  Tokens will not be delimited
     * by nulls unless \a s contains nulls.
     */
    RWWSubString          operator()(const wchar_t* s, size_t num);

    /**
     * Advances to the next token and returns it as a substring.  The
     * tokens are delimited by any of the four wide characters in
     * \c L, " \t\n\0" (space, tab, newline and null).
     */
    RWWSubString          operator()();

    /**
     * Returns the next token using a delimiter pattern represented
     * by the regular expression pattern \a regex.
     *
     * This method, unlike the other operator() overloads, allows a
     * single occurrence of a delimiter to span multiple characters.
     *
     * For example, consider the RWWTokenizer instance \c tok. The statement
     * <tt>tok(RWWString("ab"))</tt> treats either \c a or \c b as a delimiter
     * character. On the other hand, <tt>tok(RWTRegex<char>("ab"))</tt> treats
     * the two-character pattern, \c ab, as a single delimiter.
     *
     * This method consumes consecutive occurrences of delimiters and
     * skips over any empty fields that may be present in the string.
     * To obtain empty fields as well as non-empty fields, use the
     * \link nextToken(RWTRegex<wchar_t>&) nextToken() \endlink
     * method.
     */
    RWWSubString          operator()(RWTRegex<wchar_t>& regex);

    // Advance to next token, delimited by s:
    RWWSubString          nextToken(const wchar_t* ws);
    // allow nulls
    RWWSubString          nextToken(const wchar_t* ws, size_t n);
    // { return operator()(" \t\n\000",4); }
    RWWSubString          nextToken();

    /**
     * Returns the next token using a delimiter pattern represented
     * by a regular expression pattern.
     *
     * Unlike the other nextToken() overloads, this method allows a
     * single occurrence of a delimiter to span multiple characters.
     *
     * For example, <tt>nextToken(RWWString("ab"))</tt> treats either \c a or
     * \c b as a delimiter character. Conversely,
     * <tt>nextToken(RWTRegex<wchar_t>("ab"))</tt> treats the two-character
     * pattern \c ab as a single delimiter.
     *
     * This method may return an empty token if there are consecutive
     * occurrences of any delimiter character in the search string.
     */
    RWWSubString          nextToken(RWTRegex<wchar_t>& regex);

    RWWConstSubString     operator()(const wchar_t* s) const {
        return RWWConstSubString(
                   const_cast<RWWTokenizer*>(this)->operator()(s));
    }

    RWWConstSubString     operator()(const wchar_t* s, size_t t) const {
        return RWWConstSubString(
                   const_cast<RWWTokenizer*>(this)->operator()(s, t));
    }

    RWWConstSubString     operator()() const {
        return RWWConstSubString(
                   const_cast<RWWTokenizer*>(this)->operator()());
    }

    RWWConstSubString     operator()(RWTRegex<wchar_t>& regex) const {
        return RWWConstSubString(
                   const_cast<RWWTokenizer*>(this)->operator()(regex));
    }

    RWWConstSubString     nextToken(const wchar_t* ws) const {
        return RWWConstSubString(
                   const_cast<RWWTokenizer*>(this)->nextToken(ws));
    }

    RWWConstSubString     nextToken(const wchar_t* ws, size_t n) const {
        return RWWConstSubString(
                   const_cast<RWWTokenizer*>(this)->nextToken(ws, n));
    }

    RWWConstSubString     nextToken() const {
        return RWWConstSubString(
                   const_cast<RWWTokenizer*>(this)->nextToken());
    }

    RWWConstSubString     nextToken(RWTRegex<wchar_t>& regex) const {
        return RWWConstSubString(
                   const_cast <RWWTokenizer*>(this)->nextToken(regex));
    }

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWWTokenizer& rhs) {
        rw_swap(rhs.place_, place_);
        theString_.swap(rhs.theString_);
    }

private:
    size_t skipDelimiters(RWTRegex<wchar_t>& regex);
};


#endif /* RW_TOOLS_WTOKEN_H */
