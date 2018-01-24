#ifndef RW_REGEX_H
#define RW_REGEX_H

/**********************************************************************
 *
 * rw_regex.h = contains the class definition for the regular expression
 *              interface.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regex.h#1 $
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
#include <rw/tools/regexresult.h>

#include <string>

class RWRegexErr;
template <class T> class RWTRegexMatchIterator;
template <class T> class RWRegexImp;

/**
 * @ingroup string_processing_classes
 *
 * @brief Supports regular expression matching based on the POSIX.2 standard
 * and supports both narrow and wide characters.
 *
 * \link RWTRegex RWTRegex<T>\endlink is the primary template for the
 * regular expression interface. It provides most of the POSIX.2 standard for
 * regular expression pattern matching and may be used for both narrow (8-bit)
 * and wide (\c wchar_t) character strings.
 *
 * \link RWTRegex RWTRegex<T>\endlink can represent both a simple and an
 * \e extended regular expression such as those found in \c lex and \c awk.
 * The constructor "compiles" the expression into a form that can
 * be used more efficiently. The results can then be used for string
 * searches using class RWCString.   Regular expressions (REs) can
 * be of arbitrary size, limited by memory. The extended regular
 * expression features found here are a subset of those found in
 * the POSIX.2 standard (ANSI/IEEE Std. 1003.2, ISO/IEC 9945-2).
 *
 * \link RWTRegex RWTRegex<T>\endlink differs from the POSIX.2 standard in the
 * following ways:
 *
 * -  \link RWTRegex RWTRegex<T>\endlink treats all RE special characters as
 * special, unless escaped (prefixed with a \c \).  (The POSIX standard dictates
 * that some RE special characters are escaped when used to form a pattern.)
 * -  \link RWTRegex RWTRegex<T>\endlink does not currently support
 *    locale-based constructs, such as collating symbols, equivalence classes,
 *    or character classes.
 * -  \link RWTRegex RWTRegex<T>\endlink does not support backreferences.
 * Backreferencing is not supported in extended regular expressions (EREs)
 * but only in basic regular expressions (BREs).
 *
 *
 * <b>Constructing a regular expression</b>
 *
 * <i>To match a single character RE</i>
 *
 * Any character that is not a special character matches itself.
 *
 * -# A backslash (\c \) followed by any special character matches
 *    the literal character itself; that is, its use "escapes" the
 *    special character. For example, \c \* matches "*" without applying
 *    the syntax of the \c * special character.
 * -# The "special characters" are:
 * @code
 * +   *   ?   .   [   ]   ^   $   (   ) {   }   |   \
 * @endcode
 * -# The period (.) matches any character. For example, ".umpty" matches
 *    either "Humpty" or "Dumpty."
 * -# A set of characters enclosed in brackets (\c [ \c ]) is a one-character
 *    RE that matches <i>any</i> of the characters in that set. This
 *    means that \c [akm] matches either an "a", "k", or "m". A range
 *    of characters can be indicated with a dash, as in \c [a-z], which
 *    matches any lower-case letter. However, if the first character
 *    of the set is the caret (\c ^), then the RE matches any character
 *    <i>except</i> those in the set. It does not match the empty string.
 *    For example: \c [^akm] matches any character except  "a", "k",
 *    or "m". The caret loses its special meaning if it is not the
 *    first character of the set.
 *
 * <i>To match a multicharacter RE</i>
 *
 * -# Parentheses (<tt>( )</tt>) group parts of regular expressions together
 *    into subexpressions that can be treated as a single unit.
 *    For example, <tt>(ha)+</tt> matches one or more "ha"s.
 * -# An asterisk (\c *) following a one-character RE or a parenthesized
 *    subexpression matches zero or more occurrences of the RE. Hence,
 *    <tt>[a-z]*</tt> and <tt>(ha)*</tt> match zero or more lower-case
 *    characters.
 * -# A plus (\c +) following a one-character RE or a parenthesized
 *    subexpression matches one or more occurrences of the RE. Hence,
 *    <tt>[a-z]+</tt> and <tt>(ha)+</tt> match one or more lower-case
 *    characters.
 * -# A question mark (\c ?) is an optional element. The preceding
 *    RE can occur zero or once in the string  --  no more. For example,
 *    <tt>xy?z</tt> matches either xyz or xz.
 * -# The concatenation of REs is a RE that matches the corresponding
 *    concatenation of strings. For example, <tt>[A-Z][a-z]*</tt> matches
 *    matches any capitalized word.
 * -# The OR character ( \c | ) allows a choice between two regular
 *    expressions. For example, <tt>jell(y|ies)</tt> matches either "jelly"
 *    or "jellies".
 * -# Braces (\c { \c }) following a one-character RE matches the preceding
 *    element according to the number indicated. For example, <tt>a{2,3}</tt>
 *    matches either "aa" or "aaa."
 *
 * All or part of the regular expression can be "anchored" to either
 * the beginning or end of the string being searched.
 *
 * -# If the caret (\c ^) is at the beginning of the (sub)expression,
 *    then the matched string must be at the beginning of the string
 *    being searched. For example, you could use "t^hat" to return
 *    all occurrences of "hat" but avoid returning "that".
 * -# If the dollar sign (\c $) is at the end of the (sub)expression,
 *    then the matched string must be at the end of the string being
 *    searched. For example, "know$" would match "I know what I know"
 *    but not "He knows what he knows."
 *
 * <i>Overriding the backslash special character</i>
 *
 * A common pitfall with regular expression classes is overriding
 * the backslash special character (\c \). The C++ compiler and
 * the regular expression constructor will \e both assume that
 * any backslashes they see are intended to escape the following
 * character. Thus, to specify a regular expression that exactly
 * matches <tt>"a\a"</tt>, create the regular expression
 * using four backslashes as follows: the regular expression needs
 * to see <tt>"a\\a"</tt>, and for that to happen, the compiler would
 * have to see <tt>"a\\\\a"</tt>.
 *
 * @code
 *
 * RWTRegex<char> reg("a\\\\a");
 *                      ^|^|
 *                       1 2
 * @endcode
 *
 * The backslashes marked with a \c ^ are an escape for the compiler,
 * and the ones marked with \c | will thus be seen by the regular
 * expression parser. At that point, the backslash marked \c 1 is
 * an escape, and the one marked \c 2 will actually be put into
 * the regular expression.
 *
 * Similarly, if you really need to escape a character, such as
 * a '\c .', you will have to pass two backslashes to the compiler:
 *
 * @code
 *
 * RWTRegex<char> regDot("\\.");
 *                        ^|
 * @endcode
 *
 * Once again, the backslash marked \c ^ is an escape for the compiler,
 * and the one marked with \c | will be seen by the regular expression
 * constructor as an escape for the following '\c .' .
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tools/regex.h>
 * RWTRegex<char>       re0(".*\\.doc");
 *    // Matches filenames with suffix ".doc"
 * RWCRegularExpression re1("a+");
 *    // Matches one or more 'a'
 * RWWRegularExpression re2(L"b+");
 *    // Matches one or more wide-character, 'b'
 * @endcode
 *
 * @section related Related Classes
 *
 * Related classes include:
 *
 * -  \link RWTRegexMatchIterator RWTRegexMatchIterator<T>\endlink which
      iterates over matches of a pattern in a given string.
 * -  \link RWTRegexResult RWTRegexResult<T>\endlink which encapsulates the
 *    results of a pattern matching operation.
 * -  \link RWTRegexTraits RWTRegexTraits<T>\endlink which defines the
 *    character traits for a specific type of regular expression character and
 *    includes methods for returning these values.
 * -  RWRegexErr which reports errors from within
 *    \link RWTRegex RWTRegex<T>\endlink.
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/tools/regex.h>
 * #include <rw/cstring.h>
 * #include <iostream>
 * using std::cout;
 * using std::endl;
 *
 * int main()
 * {
 *   RWCString aString("Hark! Hark! The lark");
 *    // This regular expression matches any lowercase word
 *    // or end of a word starting with "l"
 *   RWTRegex<char> re("l[a-z]*");
 *   RWTRegexResult<char> result;
 *   if (result = re.search(aString))
 *     cout << result.subString(aString) << endl; //Prints "lark"
 *
 *   return 0;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 * lark
 * @endcode
 */
template <class T>
class RWTRegex
{
public:
    /**
     * Typedef for the character type.
     */
    typedef typename RWTRegexTraits<T>::Char  RChar;

    // Typedefs for regex iterators: we provide match_iterator,
    // and iterator.  RWTRegex::iterator will be a match iterator.
    // If new iterator types are added, they will need to be given
    // a descriptive prefix, as in match_iterator.

    /**
     * Typedef based on the character type used to instantiate
     * \link RWTRegex RWTRegex<T>\endlink. For example, for
     * RWTRegex<char>::iterator is a typedef
     * for \link RWTRegexMatchIterator RWTRegexMatchIterator<char>\endlink.
     * @note
     * RWTRegex::match_iterator and RWTRegex::iterator are provided.
     * RWTRegex::iterator is a match iterator.
     * If you need to add new iterator types, you must give them a descriptive
     * prefix, as in RWTRegex::match_iterator.
     */
    typedef RWTRegexMatchIterator<T>            iterator;

    /**
     * Typedef based on the character type used to instantiate
     * \link RWTRegex RWTRegex<T>\endlink.
     */
    typedef RWTRegexMatchIterator<T>            match_iterator;

    /**
     * Typedef for a string type to be used with RWTRegex.
     */
    typedef std::basic_string<RChar>            RString;

public:
    /**
     * Defines allowable status codes. These codes are accessed by RWRegexErr.
     */
    enum RWTRegexStatus {

        /**
         * &nbsp;
         */
        Ok,

        /**
         * &nbsp;
         */
        MissingEscapeSequence,

        /**
         * &nbsp;
         */
        InvalidHexNibble,

        /**
         * &nbsp;
         */
        InsufficientHex8Data,

        /**
         * &nbsp;
         */
        InsufficientHex16Data,

        /**
         * &nbsp;
         */
        MissingClosingBracket,

        /**
         * &nbsp;
         */
        MissingClosingCurlyBrace,

        /**
         * &nbsp;
         */
        MissingClosingParen,

        /**
         * &nbsp;
         */
        UnmatchedClosingParen,

        /**
         * &nbsp;
         */
        InvalidSubexpression,

        /**
         * &nbsp;
         */
        InvalidDataAfterOr,

        /**
         * &nbsp;
         */
        InvalidDataBeforeOr,

        /**
         * &nbsp;
         */
        ConsecutiveCardinalities,

        /**
         * &nbsp;
         */
        InvalidCardinalityRange,

        /**
         * &nbsp;
         */
        LeadingCardinality,

        /**
         * &nbsp;
         */
        InvalidDecimalDigit,

        /**
         * &nbsp;
         */
        UnmatchedClosingCurly,

        // Please leave this at the end of the list of enums
        /**
         * &nbsp;
         */
        NumberOfStatusCodes
    };

    /**
     * Default constructor. Objects initialized with this constructor
     * represent uninitialized patterns. These objects should be assigned
     * a valid pattern before use.
     */
    RWTRegex();

    /**
     * Initializes an \link RWTRegex RWTRegex<T>\endlink object to represent the
     * pattern specified in the \a str parameter.
     *
     * The parameter \a str specifies the pattern string for the regular
     * expression.
     *
     * The parameter \a length specifies the length, in characters,
     * of the pattern string. If the length is not specified, then it
     * is calculated as the number of characters preceding the first
     * occurrence of a \c NULL character, according to its character traits.
     * (The traits for each type of character are defined in
     * \link RWTRegexTraits RWTRegexTraits<T>\endlink.)
     *
     * @throw RWRegexErr Thrown if a pattern error is encountered.
     */
    RWTRegex(const RChar* str,
             size_t       length = size_t(-1));

    /**
     * Initializes an \link RWTRegex RWTRegex<T>\endlink object to represent the
     * pattern specified in \a str.

     * @param str      The pattern string for the RE.
     *
     * @param length   The length, in characters, of the pattern string.
     *                 If \a length is not specified, the length of \a str
     *                 is used.
     *
     * @throw RWRegexErr Thrown if a pattern error is encountered.
     */
    RWTRegex(const RString& str, size_t length = size_t(-1));

    /**
     * Copy constructor. The pattern represented by the \a source
     * RWTRegex object is copied to this RWTRegex object. This copying
     * operation is performed without recompiling the original pattern.
     */
    RWTRegex(const RWTRegex& source);

#if !defined(RW_NO_RVALUE_REFERENCES)

    /**
     * Move constructor. The constructed instance takes ownership of the
     * data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTRegex(RWTRegex && rhs);

#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Assignment operator. Copies the RWTRegex object specified
     * by \a rhs into this RWTRegex object. The copy
     * is performed without recompiling the original pattern. Returns
     * a reference to this newly assigned RWTRegex object.
     */
    RWTRegex& operator= (const RWTRegex& rhs);

#if !defined(RW_NO_RVALUE_REFERENCES)

    /**
     * Move assignment. Self takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTRegex& operator= (RWTRegex && rhs);

#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Destructor. Releases any allocated memory.
     */
    virtual ~RWTRegex();

    /**
     * Compares this RWTRegex object to the \a rhs RWTRegex
     * object by performing an element-by-element comparison of the
     * characters in each object's pattern string. Character comparisons
     * are performed as defined by the \c lt method on the
     * \link RWTRegexTraits RWTRegexTraits<T>\endlink
     * class implemented for the type of character in use.
     *
     * This object is considered less than \a rhs
     * if it contains the lesser of the first two unequal characters,
     * from left to right, or if there are no unequal characters, but
     * this pattern string is shorter than \a rhs, i.e. this pattern
     * string has fewer characters.
     *
     * Returns \c true if this RWTRegex is less \a rhs.
     */
    bool operator< (const RWTRegex& rhs) const;

    /**
     * Compares this RWTRegex object to the \a rhs RWTRegex
     * object by performing an element-by-element comparison of the
     * characters in each object's pattern string. Character comparisons
     * are performed as defined by the \c eq method on the
     * \link RWTRegexTraits RWTRegexTraits<T>\endlink
     * class implemented for the type of character in use.
     *
     * This object is considered equal to \a rhs if it contains
     * the same number of characters, and each corresponding pair of
     * characters in the patterns are equal to one another.
     *
     * Returns \c true if this RWTRegex is equal to \a rhs.
     */
    bool operator==(const RWTRegex& rhs) const;

    /**
     * Returns the number of parenthesized subexpressions in this regular
     * expression.
     */
    size_t subCount() const;

    /**
     * Searches an input string for the first occurrence of a match
     * for this RE pattern. The search begins with the
     * character at a specified \a start character position in the supplied
     * input string, and continues, one character at a time, until either
     * a match is found, or until the end of the string is reached.
     *
     * -  If a match is found, returns \c true, and the match
     *    information returned through RWTRegexResult<T>::getStart() and
     *    RWTRegexResult<T>::getLength() will represent the longest match
     *    starting from the first position.
     * -  If no match is found, returns \c false.
     *
     * @param str      The string to be searched for a match.
     *
     * @param start    The character position where the search for a
     *                 match will start.
     * @param length   The length, in characters, of the entire input string.
     *                 If the length is not specified, it is calculated as
     *                 the number of characters preceding the firs occurrence
     *                 of a \c NULL character, as defined by this character's
     *                 traits.
     */
    RWTRegexResult<T> search(const RChar* str,
                             size_t       start  = size_t(0),
                             size_t       length = size_t(-1));

    /**
     * Searches an input string
     * for the first occurrence of a match for this regular expression
     * pattern. The search begins with the character at a specified
     * \a start character position in the supplied input string, and
     * continues, one character at a time, until either a match is found,
     * or until the end of the string is reached.
     *
     * -  If a match is found, returns \c true, and the match
     *    information returned through RWTRegexResult<T>::getStart() and
     *    RWTRegexResult<T>::getLength() will represent the longest match
     *    starting from the first position at which a match is found.
     * -  If no match is found returns \c false.
     *
     * @param str      The string to be searched for a match.
     *
     * @param start    The character position where the search for a
     *                 match will start.
     * @param length   The length, in characters, of the entire input string.
     *                 If the length is not specified, then it is assigned
     *                 the length of the input string object.
     */
    RWTRegexResult<T> search(const RString& str,
                             size_t         start  = size_t(0),
                             size_t         length = size_t(-1));

    /**
     * Searches an input string for a match against the pattern string
     * represented by this RWTRegex object. The match \e must
     * start at the specified character in the input string. (This is
     * similar to anchoring the pattern at the beginning of the string
     * using the circumflex character \c ^.)
     *
     * -  If a match is found, returns \c true, and the match
     *    information returned through RWTRegexResult<T>::getStart() and
     *    RWTRegexResult<T>::getLength() represents the longest match
     *    starting from the first character in the string.
     * -  If no match is found, returns \c false.
     *
     * @param str      The string to be searched for a match.
     *
     * @param start    The character position where the search for a
     *                 match will start.
     * @param length   The length, in characters, of the entire input string.
     *                 If the length is not specified, then it is calculated
     *                 as the number of characters preceding the first
     *                 occurrence of a \c NULL character, as defined by the
     *                 traits specific to this type of character.
     */
    RWTRegexResult<T> matchAt(const RChar* str,
                              size_t       start  = size_t(0),
                              size_t       length = size_t(-1));

    /**
     * Searches an input string for a match against the pattern string
     * represented by this RWTRegex object.  The match \e must
     * start at the specified character in the input string. (This is
     * similar to anchoring the pattern at the beginning of the string
     * using the circumflex character \c ^.)
     *
     * -  If a match is found, returns \c true, and the match
     *    information returned through RWTRegexResult<T>::getStart() and
     *    RWTRegexResult<T>::getLength() represents the longest match
     *    starting from the first character in the string.
     * -  If no match is found, returns \c false.
     *
     * @param str      The string to be searched for a match.
     *
     * @param start    The character position where the search for a
     *                 match will start.
     * @param length   The length, in characters, of the entire input string.
     *                 If the length is not specified, it is assigned the
     *                 length of the input string object.
     */
    RWTRegexResult<T> matchAt(const RString& str,
                              size_t         start  = size_t(0),
                              size_t         length = size_t(-1));

    /**
     * Searches an input string for the first occurrence of a match
     * for this regular expression pattern. The search begins with the
     * character at a specified \a start character position in the supplied
     * input string. It continues, one character at a time, until
     * either a match is found, or the end of the string is reached. Use
     * \a length to specify the length of the input string.
     *
     * -  If a match is found, returns the index into the string
     *    at which the first match was found, starting from the beginning
     *    of the string. The length of the match is returned in the \a mLen
     *    argument.
     * -  If no match is found, returns #RW_NPOS.
     *
     * @param str      The string to be searched for a match.
     *
     * @param mLen     A return parameter representing the length of any
     *                 match found during this operation. If not supplied,
     *                (\c NULL), the length is not returned, but is available
     *                 through RWTRegexResult<T>::getLength().
     * @param start    The character position where the search for a
     *                 match will start.
     * @param length   The length, in characters, of the entire input string.
     *                 If the length is not specified, then it is calculated
     *                 as the number of characters preceding the first
     *                 occurrence of a \c NULL character, as defined by the
     *                 traits specific to this type of character.
     *
     * Returns the starting character position, from the beginning of
     * the string, of a match. If no match is found, #RW_NPOS is returned.
     */
    size_t index(const RChar*   str,
                 size_t*        mLen   = 0,
                 size_t         start  = size_t(0),
                 size_t         length = size_t(-1));

    /**
     * Searches an input string for the first occurrence of a match
     * for this regular expression pattern. The search begins with the
     * character at a specified \a start character position in the supplied
     * input string, and continues, one character at a time, until either
     * a match is found, or the end of the string is reached. Use \a length
     * to specify the length of the input string.
     *
     * -  If a match is found, the method returns the index into the string
     *    at which the first match was found, starting from the beginning
     *    of the string. The length of the match is returned in the \a mLen
     *    argument.
     * -  If no match is found, the method returns #RW_NPOS.
     *
     * @param str      The string to be searched for a match.
     * @param mLen     A return parameter representing the length of any
     *                 match found during this operation. If not supplied,
     *                (\c NULL), the length is not returned, but is available
     *                 through RWTRegexResult<T>::getLength().
     * @param start    The character position where the search for a
     *                 match will start.
     * @param length   The length, in characters, of the entire input string.
     *                 If the length is not specified, it is assigned the
     *                 length of the input string object.
     */
    size_t index(const RString& str,
                 size_t*        mLen   = 0,
                 size_t         start  = size_t(0),
                 size_t         length = size_t(-1));


    /**
     * Replaces occurrences of the regular expression pattern in
     * \a str with a replacement string, \a replacement. The number of
     * replacements is identified by \a count. The default value for
     * \a count is \c 1, which replaces only the first occurrence of
     * the pattern.
     *
     * Zero-length matches are replaced only if \a replaceEmptyMatches
     * is \c true. The search begins at the \a start character position.
     * The length, in characters, of the original string is identified
     * by \a length. The input \a str is updated as part of this operation.
     *
     * Returns the total number of occurrences replaced.
     *
     * @param str                  The string to be searched for a match.
     *
     * @param replacement          The string to replace all occurrences of
     *                             the pattern in \a str.
     *
     * @param count                The number of matches to replace. If \c 0
     *                             is specified, all matches are replaced.
     *
     * @param matchID              The match identifier of the sub-expression
     *                             to be replaced. The default value of \c 0
     *                             replaces the overall match with specified
     *                             replacement text.
     *
     * @param start                The character position where the search for
     *                             a match will start.
     *
     * @param length               The length, in characters, of the entire
     *                             input string. If the length is not
     *                             specified, it is assigned the
     *                             length of the input string object.
     *
     * @param replaceEmptyMatches  Boolean. If \c true, zero-length matches
     *                             are replaced, as well as all other matches.
     *                             Otherwise, only matches with \a length
     *                             greater than zero are replaced.
     *
     * Returns the starting character position, from the beginning of
     * the string, of a match. If no match is found, #RW_NPOS is
     * returned.
     */
    size_t replace(RString&                    str,
                   const RString&              replacement,
                   size_t                      count               = 1,
                   size_t                      matchID             = 0,
                   size_t                      start               = size_t(0),
                   size_t                      length              = size_t(-1),
                   bool                   replaceEmptyMatches = true);

    /**
     * Returns the regular expression status for the last-pattern
     * compilation status. This method is useful primarily in
     * exception-disabled environments in which the default error
     * handler for the Essential Tools Module error
     * framework has been replaced with a function that does not abort.
     * Otherwise, the regular expression object will not be available
     * for this query.
     */
    const RWRegexErr& getStatus() const;

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWTRegex<T>& rhs);

private:
    RWTScopedPointer<RWRegexImp<T> > imp_;
};



template <class T> class RWRegexMatchIteratorImp;

/**
 * @ingroup string_processing_classes
 *
 * @brief Iterates over matches found using \link RWTRegex RWTRegex<T>\endlink.
 *
 * Class \link RWTRegexMatchIterator RWTRegexMatchIterator<T>\endlink is an
 * iterator class provided with RWTRegex for iteration over matches of a
 * pattern in a given string.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tools/regex.h>
 * #include <rw/cstring.h>
 *
 * RWTRegex<char> pattern("a+");
 * RWCString      str("aabbccaadd");
 *
 * RWTRegexMatchIterator<char> i(pattern, str);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/tools/regex.h>
 * #include <rw/cstring.h>
 *
 * #include <iostream>
 *
 * int main()
 * {
 *   RWCString pattern("a+");
 *   RWCString str("aaabaacadeaaaafgaaa");
 *
 *   // Compile the pattern
 *   RWTRegex<char> r(pattern);
 *
 *   // Iterate over all occurrences of the "pattern" string in
 *   // "str"
 *   for (RWTRegex<char>::iterator i(r, str);
 *        i != RWTRegex<char>::iterator(); ++i)
 *   {
 *     std::cout << "Found match at index, "
 *               << i-> getStart() << ":"
 *               << i-> subString(str) << std::endl;
 *   }
 *
 *   return 0;
 * }
 * Program Output:
 *
 * Found match at index, 0 :aaa
 * Found match at index, 4 :aa
 * Found match at index, 7 :a
 * Found match at index, 10 :aaaa
 * Found match at index, 16 :aaa
 * @endcode
 */
template <class T>
class RWTRegexMatchIterator
{
private:
    typedef typename RWTRegex<T>::RChar   RChar;

    typedef typename RWTRegex<T>::RString RString;

public:

    /**
     * Default constructor
     */
    RWTRegexMatchIterator();

    /**
     * Copy constructor
     */
    RWTRegexMatchIterator(const RWTRegexMatchIterator<T>& source);

#if !defined(RW_NO_RVALUE_REFERENCES)

    /**
     * Move constructor. The constructed instance takes ownership of the
     * data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTRegexMatchIterator(RWTRegexMatchIterator<T> && rhs);

#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Constructor. Initializes a match iterator to the first match
     * in \a str, starting from the \a start character offset, of
     * the regular expression pattern given in \a regex. The parameter
     * \a length defines the length of \a str. If not specified,
     * the length is calculated as the number of characters up to the
     * first \c NULL character, according to the character traits.
     */
    RWTRegexMatchIterator(RWTRegex<T>&       regex,
                          const RChar*       str,
                          size_t             start   = size_t(0),
                          size_t             length  = size_t(-1),
                          bool          overlap = false);
    /**
     * Constructor. Initializes a match iterator to the first match
     * in \a str, starting from the \a start character offset, of
     * the regular expression pattern given in \a regex. The parameter
     * \a length identifies the length of \a str. If not
     * specified, the length is assigned the length of \a str.
     */
    RWTRegexMatchIterator(RWTRegex<T>&       regex,
                          const RString&     str,
                          size_t             start  = size_t(0),
                          size_t             length = size_t(-1),
                          bool          overlap = false);

    /**
     * Assignment operator
     */
    RWTRegexMatchIterator&      operator=(const RWTRegexMatchIterator& rhs);

#if !defined(RW_NO_RVALUE_REFERENCES)

    /**
     * Move assignment. Self takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTRegexMatchIterator& operator= (RWTRegexMatchIterator<T> && rhs);

#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Destructor
     */
    ~RWTRegexMatchIterator();

    /**
     * Pre-increment operator
     */
    RWTRegexMatchIterator&      operator++();

    /**
     * Post-increment operator
     */
    const RWTRegexMatchIterator operator++(int);

    /**
     * Advances through \a count matches.
     */
    RWTRegexMatchIterator&      operator+=(size_t count);

    /**
     * Dereferences the iterator, giving a reference to the regular
     * expression object represented in the iterator.
     */
    RWTRegexResult<T>           operator*() const;

    /**
     * Returns a pointer to the regular expression object represented
     * in the iterator.
     */
    const RWTRegexResult<T>*    operator->() const;

    /**
     * Compares two iterators for equality based on the
     * regular expression and the starting position in the string.
     */
    bool                   operator==(const RWTRegexMatchIterator& rhs) const;

    /**
     * Determines if two iterators are unequal. The return value of
     * this method is the same as !(*this == rhs).
     */
    bool                   operator!=(const RWTRegexMatchIterator& rhs) const;

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWTRegexMatchIterator<T>& rhs);

private:
    RWTScopedPointer<RWRegexMatchIteratorImp<T> > imp_;
};



#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("regex.cc")
#endif

#if defined(RW_NO_IMPLICIT_INCLUSION)
#  include <rw/tools/regex.cc>
#endif

// Include char and wchar_t specializations
#include <rw/tools/regex/cregex.h>
#include <rw/tools/regex/wregex.h>

typedef RWTRegex<char>          RWCRegularExpression;
typedef RWTRegex<wchar_t>       RWWRegularExpression;

#endif // RW_REGEX_H
