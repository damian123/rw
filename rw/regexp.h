#ifndef RW_TOOLS_REGEXP_H
#define RW_TOOLS_REGEXP_H



/**********************************************************************
 *
 * Declarations for class RWCRegexp --- Regular Expression
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/regexp.h#1 $
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
#include <rw/tools/pointer.h>

#if !defined(RW_DISABLE_DEPRECATED)

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

RW_SUPPRESS_DEPRECATED_WARNINGS

typedef unsigned short RWPatternType;

class RW_TOOLS_GLOBAL RWCString;


/**
 * @ingroup string_processing_classes
 * @deprecated As of SourcePro 4, use RWTRegex instead.
 *
 * @brief Deprecated. Represents a regular expression.
 *
 * Class RWCRegexp represents a regular expression.  The constructor
 * "compiles" the expression into a form that can be used more efficiently.
 * The results can then be used for string searches using class
 * RWCString.
 *
 * The regular expression (RE) is constructed as follows:
 *
 * The following rules determine one-character REs that match a
 * single character:
 *
 * -# Any character that is not a special character (to be defined)
 * -# A backslash (\c \\) followed by any special character matches
 *    the literal character itself. i.e., this "escapes" the special
 *    character.
 * @note
 * There is one exception to this rule. \c \^\c <i>char</i>
 * is interpreted as a control character: thus \c \^R is control-R.
 * To match the circumflex \c ^ itself, use \c \\x5e in US-ASCII environments.
 * -# The "special characters" are:
 * @code
 * +     *     ?     .     [     ]     ^     $
 * @endcode
 * -# The period (\c .) matches any character except the newline.
 * -# A set of characters enclosed in brackets (\c []) is a one-character
 *    RE that matches any of the characters in that set. <i>Example:</i>
 *    "[akm]" matches either an "a", "k", or "m". A range of characters
 *    can be indicated with a dash.  <i>Example:</i> "[a-z]" matches
 *    any lower-case letter. However, if the first character of the
 *    set is the caret (^), then the RE matches any character except
 *    those in the set. It does not match the empty string. <i>Example:</i>
 *    [^akm] matches any character except  "a", "k", or "m". The caret
 *    loses its special meaning if it is not the first character of
 *    the set.
 *
 * The following rules can be used to build a multi-character RE.
 *
 * -# A one-character RE followed by an asterisk (\c *) matches zero
 *    or more occurrences of the RE.  Hence, [a-z]* matches zero or
 *    more lower-case characters.
 * -# A one-character RE followed by a plus (\c +) matches one or more
 *    occurrences of the RE.  Hence, [a-z]+ matches one or more lower-case
 *    characters.
 * -# A question mark (\c ?) is an optional element. The preceding
 *    RE can occur zero or once in the string  --  no more. For example,
 *    xy?z matches either xyz or xz.
 * -# The concatenation of REs is an RE that matches the corresponding
 *    concatenation of strings. For example, [A-Z][a-z]* matches any
 *    capitalized word.
 *
 * Finally, the entire regular expression can be anchored to match
 * only the beginning or end of a line:
 *
 * -# If the caret (\c ^) is at the beginning of the RE, then the matched
 *    string must be at the beginning of the line.
 * -# If the dollar sign (\c $) is at the end of the RE, then the matched
 *    string must be at the end of the line.
 *
 * The following escape codes can be used to match control characters:
 *
 * <table>
 * <tr>
 * <td>\c \\b </td>
 * <td>backspace </td>
 * </tr>
 * <tr>
 * <td>\c \\e </td>
 * <td>\c ESC (escape) </td>
 * </tr>
 * <tr>
 * <td>\c \\f </td>
 * <td>formfeed </td>
 * </tr>
 * <tr>
 * <td>\c \\n </td>
 * <td>newline </td>
 * </tr>
 * <tr>
 * <td>\c \\r </td>
 * <td>carriage return </td>
 * </tr>
 * <tr>
 * <td>\c \\t </td>
 * <td>tab </td>
 * </tr>
 * <tr>
 * <td>\c \\xddd </td>
 * <td>the literal hex number \c 0xdd  </td>
 * </tr>
 * <tr>
 * <td>\c \\ddd </td>
 * <td>the literal octal number \c ddd  </td>
 * </tr>
 * <tr>
 * <td>\c \^C </td>
 * <td>Control code.  For example, \\c ^D is "control-D" </td>
 * </tr>
 * </table>
 *
 * The most frequent problem with using this class is
 * specifying a backslash character to be parsed. If you are
 * attempting to parse a regular expression that contains backslashes,
 * you must be aware that the C++ compiler and the regular expression
 * constructor both assume that any backslashes are
 * intended to escape the following character. Thus, to specify
 * a regular expression that exactly matches \c "a\a", you would
 * have to create the regular expression using four backslashes
 * as follows: the regular expression needs to see \c "a\\a", and
 * for that to happen, the compiler would have to see \c "a\\\\a".
 *
 * @code
 * RWCRegexp reg("a\\\\a");
 *                 ^|^|
 *                  1 2
 * @endcode
 *
 * The backslashes marked with a \c ^ are an escape for the compiler,
 * and the ones marked with \c | are seen by the regular
 * expression parser. At that point, the backslash marked \c 1 is
 * an escape, and the one marked \c 2 is actually put into
 * the regular expression.
 *
 * Similarly, if you really need to escape a character, such as
 * a '\c .' you have to pass two backslashes to the compiler:
 *
 * @code
 * RWCRegexp regDot("\\.")
 *                   ^|
 * @endcode
 *
 * Once again, the backslash marked \c ^ is an escape for the compiler,
 * and the one marked with \c | is seen by the regular expression
 * constructor as an escape for the following '\c .'.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/regexp.h>
 * RWCRegexp re(".*\\.doc");// Matches filename with suffix ".doc"
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
 *
 * #include <rw/regexp.h>
 * #include <rw/cstring.h>
 *
 * int main ()
 * {
 *     RWCString s("Hark! Hark! the lark");
 *
 *     std::cout << "Searching for expressions beginning with \"l\" in \""
 *               << s << "\".\n";
 *
 *     // A regular expression matching any lower-case word
 *     // starting with 'l':
 *     RWCRegexp reg("l[a-z]*");
 *     std::cout << "Found \"" << s(reg) << "\"." << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 * Searching for expressions beginning with "l" in "Hark! Hark! the lark".
 * Found "lark".
 * @endcode
 */
class RW_DEPRECATE_TYPE("Use RWTRegex<char> instead") RW_TOOLS_GLOBAL RWCRegexp
{
public:
    /**
     * This enumeration represents the status of the regular expression
     * encapsulated by a RWCRegexp instance.
     */
    enum statVal {

        /**
         *  No errors.
         */
        OK = 0,

        /**
         *  Pattern was illegal.
         */
        ILLEGAL,

        /**
         *  Memory could not be allocated.
         */
        NOMEM,

        /**
         *  Pattern exceeded maximum length. (To change the amount of
         *  space allocated for a pattern, edit \c regexp.cpp to change
         *  the value of \c RWCRegexp::maxpat_, then recompile and insert
         *  the changed object into the appropriate library.)
         */
        TOOLONG
    };

    /**
     * Constructs a regular expression from the pattern given by \a pat.
     * The status of the results can be found by using member function
     * status().
     */
    RWCRegexp(const char* pat);

    /**
     * @copydoc RWCRegexp(const char*)
     */
    RWCRegexp(const RWCString& pat);

    /**
     * Copy constructor. Uses value semantics  --  self is a copy
     * of \a r.
     */
    RWCRegexp(const RWCRegexp& r);

    /**
     * Destructor.  Releases any allocated memory.
     */
    ~RWCRegexp();

    /**
     * Uses value semantics  --  sets self to a copy of \a r.
     */
    RWCRegexp&    operator=(const RWCRegexp& r);

    /**
     * Recompiles self to the pattern given by \a pat. The status of
     * the results can be found by using member function status().
     */
    RWCRegexp&    operator=(const RWCString& pat);

    /**
     * @copydoc operator=(const RWCString&)
     */
    RWCRegexp&    operator=(const char*);

    /**
     * Returns the index of the first instance in the string \a str
     * that matches the regular expression compiled in self, or #RW_NPOS
     * if there is no such match. The search starts at index \a start.
     * The length of the matching pattern is returned in the variable
     * pointed to by \a len. Using an invalid regular expression
     * for the search throws an exception of type RWInternalErr.
     *
     *@note
     * This member function is relatively clumsy to use  --
     * class RWCString offers a better interface to regular expression
     * searches.
     */
    size_t        index(const RWCString& str, size_t* len, size_t start = 0) const;

    /**
     * Returns the status of the regular expression and resets status
     * to #OK
     */
    statVal       status();

private:
    void          copyPattern(const RWCRegexp&);
    void          genPattern(const char*);
    RWTScopedPointer<RWPatternType[]> thePattern_;   // Compiled pattern
    statVal       stat_;          // Status
    static const unsigned maxpat_;        // Max length of compiled pattern

};

typedef RWCRegexp RWRegexp;     // For backward compatibility

// Regular expressions stuff
int
rwMakePattern(const char*, RWPatternType*, int);
const char*
rwMatchString(const char*, size_t, const RWPatternType*, const char**);

RW_RESTORE_DEPRECATED_WARNINGS

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif /* RW_DISABLE_DEPRECATED */

#endif /* RW_TOOLS_REGEXP_H */
