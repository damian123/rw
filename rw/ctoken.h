#ifndef RW_TOOLS_CTOKEN_H
#define RW_TOOLS_CTOKEN_H



/**********************************************************************
 *
 * RWCTokenizer --- converts strings into sequences of tokens
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/ctoken.h#1 $
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
#include <rw/edefs.h> // for rw_swap
#include <rw/cstring.h>

// Forward declare regular expression class template
template <class E> class RWTRegex;

/**
 * @ingroup string_processing_classes
 * @brief Breaks a string into separate tokens delimited by an arbitrary
 * whitespace. Can be used as an alternative to the ANSI C function
 * strtok().
 *
 * Class RWCTokenizer is designed to break a string into separate
 * tokens, delimited by an arbitrary whitespace.  Think of it as
 * an iterator for strings and as an alternative to the ANSI C function
 * strtok() that has the unfortunate side effect of changing the
 * tokenized string.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/ctoken.h>
 * RWCString str("a string of tokens");
 * RWCTokenizer(str);  // Lex the above string
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/ctoken.h>
 *
 * int main ()
 * {
 *     RWCString a ("Something is rotten in the state of "
 *                  "Denmark and Hamlet is taking out the trash.");
 *
 *     RWCTokenizer next(a); // Tokenize the string a
 *     RWCString token;      // Will receive each token
 *
 *     // Advance until the null string is returned:
 *     while (!(token=next()).isNull())
 *         std::cout << token << "\n";
 * }
 * @endcode
 */
class RW_TOOLS_GLOBAL RWCTokenizer
{
private:
    RWCString          theString_;
    size_t             place_;

public:

    /**
     * Constructs a tokenizer to lex the string \a s.
     */
    RWCTokenizer(const RWCString& s);     // Construct to lex a string

    /**
     * Copy constructor. The created tokenizer copies the data from \a rhs.
     */
    RWCTokenizer(const RWCTokenizer& rhs)
        : theString_(rhs.theString_), place_(rhs.place_) {
    }

    /**
     * Assignment operator. The tokenizer copies the data from \a rhs.
     * Returns a reference to self.
     */
    RWCTokenizer& operator=(const RWCTokenizer& rhs) {
        if (&rhs != this) {
            RWCTokenizer(rhs).swap(*this);
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
    RWCTokenizer(RWCTokenizer && rhs)
        : place_(0) {
        rhs.swap(*this);
    }

    /**
     * Move assignment. Self takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWCTokenizer& operator=(RWCTokenizer && rhs) {
        if (&rhs != this) {
            rhs.swap(*this);
        }
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    virtual ~RWCTokenizer();

    /**
     * Returns \c true if the last token from the search string has been
     * extracted, otherwise \c false. When using the function call
     * operator interface, this is the same as the last non-empty token
     * having been returned.
     */
    bool done() const;

    /**
     * Advances to the next token and returns it as a substring.  The
     * tokens are  delimited by any character in \a s, or any embedded
     * null.
     */
    RWCSubString          operator()(const char* s);

    /**
     * Advances to the next token and returns it as a substring.  The
     * tokens are delimited by any of the first \a n characters in
     * \a s. Buffer \a s may contain nulls, and must contain at least
     * \a n characters.  Tokens will not be delimited by nulls unless
     * \a s contains nulls.
     */
    RWCSubString          operator()(const char* s, size_t n);
    // { return operator()(" \t\n\000",4); }

    /**
     * Advances to the next token and returns it as a substring.  The
     * tokens are delimited by any of the four characters in <tt>" \t\n\0"</tt>
     * (space, tab, newline and null).
     */
    RWCSubString          operator()();

    /**
     * Returns the next token using a delimiter pattern represented
     * by the regular expression pattern \a regex.
     *
     * This method, unlike the other operator() overloads, allows a
     * single occurrence of a delimiter to span multiple characters.
     *
     * For example, consider the RWCTokenizer instance \c tok. The statement
     * <tt>tok(RWCString("ab"))</tt> treats either \c a or \c b as a delimiter
     * character. On the other hand, <tt>tok(RWTRegex<char>("ab"))</tt> treats
     * the two-character pattern, \c ab, as a single delimiter.
     *
     * This method consumes consecutive occurrences of delimiters and
     * skips over any empty fields present in the string.
     * To obtain empty fields as well as non-empty fields, use the nextToken()
     * method.
     */
    RWCSubString          operator()(RWTRegex<char>& regex);

    // Advance to next token, delimited by s:
    RWCSubString          nextToken(const char* ws);
    // allow nulls
    RWCSubString          nextToken(const char* ws, size_t n);
    // { return operator()(" \t\n\000",4); }

    /**
     * Returns the next token using a specified string of delimiter
     * characters.
     *
     * This method may return an empty token if there are consecutive
     * occurrences of any delimiter character in the search string.
     */
    RWCSubString          nextToken();

    /**
     * Returns the next token using a delimiter pattern represented
     * by a regular expression pattern.
     *
     * Unlike the other nextToken() overloads, this method allows a
     * single occurrence of a delimiter to span multiple characters.
     *
     * For example, <tt>nextToken(RWCString("ab"))</tt> treats either \c a or
     * \c b as a delimiter character. Conversely,
     * <tt>nextToken(RWTRegex<char>("ab"))</tt> treats the two-character
     * pattern \c ab as a single delimiter.
     *
     * This method may return an empty token if there are consecutive
     * occurrences of any delimiter character in the search string.
     */
    RWCSubString          nextToken(RWTRegex<char>& regex);

    RWCConstSubString     operator()(const char* s) const {
        return RWCConstSubString(
                   (const_cast<RWCTokenizer*>(this))->operator()(s));
    }

    RWCConstSubString     operator()(const char* s, size_t t) const {
        return RWCConstSubString(
                   (const_cast<RWCTokenizer*>(this))->operator()(s, t));
    }

    RWCConstSubString     operator()() const {
        return RWCConstSubString(
                   (const_cast<RWCTokenizer*>(this))->operator()());
    }

    RWCConstSubString     operator()(RWTRegex<char>& regex) const {
        return RWCConstSubString(
                   (const_cast<RWCTokenizer*>(this))->operator()(regex));
    }

    RWCConstSubString     nextToken(const char* ws) const {
        return RWCConstSubString(
                   (const_cast<RWCTokenizer*>(this))->nextToken(ws));
    }

    RWCConstSubString     nextToken(const char* ws, size_t n) const {
        return RWCConstSubString(
                   (const_cast<RWCTokenizer*>(this))->nextToken(ws, n));
    }

    RWCConstSubString     nextToken() const {
        return RWCConstSubString(
                   (const_cast<RWCTokenizer*>(this))->nextToken());
    }

    RWCConstSubString     nextToken(RWTRegex<char>& regex) const {
        return RWCConstSubString(
                   (const_cast<RWCTokenizer*>(this))->nextToken(regex));
    }

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWCTokenizer& rhs) {
        rw_swap(rhs.place_, place_);
        theString_.swap(rhs.theString_);
    }

private:
    size_t skipDelimiters(RWTRegex<char>& regex);
};

typedef RWCTokenizer RWTokenizer;  // backward compatibility


#endif /* RW_TOOLS_CTOKEN_H */
