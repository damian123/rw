#ifndef RW_TOOLS_RE_H
#define RW_TOOLS_RE_H



/**********************************************************************
 *
 * Declarations for class RWTRegularExpression --- Extended Regular Expression
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/re.h#1 $
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

#ifdef RW_MAXINT_CONFLICT
#  define _SYS_PARAM_INCLUDED
#endif

#if !defined(RW_NO_EXCEPTIONS)

#  include <rw/rei.h>         // Regular expression implementation class.

#if !defined(RW_DISABLE_DEPRECATED)
RW_SUPPRESS_DEPRECATED_WARNINGS

/**
 * @ingroup string_processing_classes
 * @deprecated As of SourcePro 4, use \link RWTRegex RWTRegex<T>\endlink instead.
 *
 * @brief Deprecated. Provides extended regular expression matching similar to that
 * found in lex and awk.
 *
 * Class \link RWTRegularExpression RWTRegularExpression<T>\endlink represents
 * an <i>extended</i> regular expression
 * such as those found in \c lex and \c awk. The constructor "compiles"
 * the expression into a form that can be used more efficiently.
 * The results can then be used for string searches using class
 * RWCString.   Regular expressions can be of arbitrary size, limited
 * by memory. The extended regular expression features found here
 * are a subset of those found in the POSIX.2 standard (<i>ANSI/IEEE
 * Std 1003.2, ISO/IEC 9945-2)</i>.
 *
 * Note: \link RWTRegularExpression RWTRegularExpression<T>\endlink is
 * available only if your compiler supports exception
 * handling and the C++ Standard Library.
 *
 * The regular expression (RE) is constructed as follows:
 *
 * The following rules determine one-character REs that match a
 * single character:
 *
 * Any character that is not a special character (to be defined)
 * matches itself.
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
 *    either "Humpty" or "Dumpty".
 * -# A set of characters enclosed in brackets (\c [ \c ]) is a one-character
 *    RE that matches any of the characters in that set. This means
 *    that \c [akm] matches either an "a", "k", or "m". A range of
 *    characters can be indicated with a dash, as in \c [a-z], which
 *    matches any lower-case letter. However, if the first character
 *    of the set is the caret (\c ^), then the RE matches any character
 *    except those in the set. It does not match the empty string.
 *    For example: \c [^akm] matches any character except  "a", "k",
 *    or "m". The caret loses its special meaning if it is not the
 *    first character of the set.
 *
 * The following rules can be used to build a multicharacter RE:
 *
 * -# Parentheses (( )) group parts of regular expressions together
 *    into subexpressions that can be treated as a single unit. For
 * -# A one-character RE followed by an asterisk (\c *) matches zero
 *    or more occurrences of the RE.  Hence, \c [a-z]* matches zero
 * -# A one-character RE followed by a plus (\c +) matches one or more
 *    occurrences of the RE.  Hence, \c [a-z]+ matches one or more
 * -# A question mark (\c ?) is an optional element. The preceding
 *    RE can occur zero or once in the string  --  no more. For example,
 * -# The concatenation of REs is a RE that matches the corresponding
 *    concatenation of strings. For example, \c [A-Z][a-z]* matches
 * -# The OR character ( \c | ) allows a choice between two regular
 *    expressions. For example, jell(y|ies) matches either "jelly"
 * -# Braces (\c { \c }) are reserved for future use.
 *
 * All or part of the regular expression can be "anchored" to either
 *  the beginning or end of the string being searched:
 *
 * -# If the caret (\c ^) is at the beginning of the (sub)expression,
 *    then the matched string must be at the beginning of the string
 * -# If the dollar sign (\c $) is at the end of the (sub)expression,
 *    then the matched string must be at the end of the string being
 *    searched.
 *
 * The most frequent problem with use of this class is in being
 * able to specify a backslash character to be parsed. If you are
 * attempting to parse a regular expression that contains backslashes,
 * you must be aware that the C++ compiler and the regular expression
 * constructor will both assume that any backslashes they see are
 * intended to escape the following character. Thus, to specify
 * a regular expression that exactly matches \c "a\a", you would
 * have to create the regular expression using four backslashes
 * as follows: the regular expression needs to see \c "a\\a", and
 * for that to happen, the compiler would have to see \c "a\\\\a".
 *
 * @code
 * RWTRegularExpression<char> reg("a\\\\a");
 *                                  ^|^|
 *                                   1 2
 * @endcode
 *
 * The backslashes marked with a \c ^ are an escape for the compiler,
 * and the ones marked with \c | will thus be seen by the regular
 * expression parser. At that point, the backslash marked \c 1 is
 * an escape, and the one marked \c 2 will actually be put into
 * the regular expression.
 *
 * Similarly, if you really need to escape a character, such as
 * a \c '.', you will have to pass two backslashes to the compiler:
 *
 * @code
 *
 * RWTRegularExpression<char> regDot("\\.")
 *                                    ^|
 * @endcode
 *
 * Once again, the backslash marked \c ^ is an escape for the compiler,
 * and the one marked with \c | will be seen by the regular expression
 * constructor as an escape for the following \c '.'.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/re.h>
 * RWTRegularExpression<char> re(".*\\.doc$");  // Matches filename with suffix ".doc"
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
 * #include <rw/re.h>
 * #include <rw/cstring.h>
 *
 * int main ()
 * {
 *     RWCString s ("Hark! Hark! the lark");
 *
 *
 *     std::cout << "Searching for an expression beginning with \"l\" in \""
 *               << s << "\".\n";
 *
 *     // A regular expression matching any lower-case word
 *     // starting with 'l':
 *     RWTRegularExpression<char> reg("l[a-z]*");
 *
 *     // Prints 'lark'
 *     std::cout << "Found \"" << s.match(reg) << "\"." << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 */
template<class charT>
class RW_DEPRECATE_TYPE("Use RWTRegex instead") RWTRegularExpression : public RWREBaseClass
{
public:
    typedef typename RWTRegularExpressionTraits<charT>::tStringT stringT;

    /**
     * Default constructor.  You must assign a pattern to the regular
     * expression before you use it.
     */
    RWTRegularExpression() :
        impl_(new RWTRegularExpressionImp<charT>) {
    }

    /**
     * Construct a regular expression from the pattern given by \a pat.
     *  The status of the results can be found by using member function
     * status().
     */
    //@{
    RWTRegularExpression(const charT* pat)  :
        impl_(new RWTRegularExpressionImp<charT>(pat)) {
    }

    RWTRegularExpression(const stringT& pat) :
        impl_(new RWTRegularExpressionImp<charT>(pat)) {
    }
    //@}

    /**
     * Copy constructor. Uses value semantics  --  self will be a copy
     * of \a other.
     */
    RWTRegularExpression(const RWTRegularExpression<charT>& other) :
        impl_(new RWTRegularExpressionImp<charT>(*other.impl_)) {
    }

    /**
     * Destructor.  Releases any allocated memory.
     */
    virtual ~RWTRegularExpression() {}

    /**
     * Recompiles self to pattern found in \a other.
     */
    RWTRegularExpression<charT>&
    operator=(const RWTRegularExpression<charT>& other) {
        if (&other != this) {
            RWTRegularExpression<charT> dup(other);
            rw_swap(impl_, dup.impl_);
        }
        return *this;
    }

    /**
     * Recompiles self to the pattern given by \a pat. The status of
     * the results can be found by using member function status().
     */
    //@{
    RWTRegularExpression<charT>&
    operator=(const charT* pat) {
        RWTRegularExpression<charT> dup(pat);
        rw_swap(impl_, dup.impl_);

        return *this;
    }

    RWTRegularExpression<charT>&
    operator=(const stringT& pat) {
        RWTRegularExpression<charT> dup(pat);
        rw_swap(impl_, dup.impl_);

        return *this;
    }
    //@}

    /**
     * Returns the index of the first instance in the string \a str
     * that matches the regular expression compiled in self, or #RW_NPOS
     * if there is no such match. The search starts at index \a start.
     * The length of the matching pattern is returned in the variable
     * pointed to by \a len. If an invalid regular expression is used
     * for the search, an exception of type RWInternalErr will be thrown.
     * Note that this member function is relatively clumsy to use  --
     *  class RWCString offers a better interface to regular expression
     * searches.
     */
    size_t index(const stringT& str, size_t* len = NULL, size_t  start = 0) {
        size_t rc = impl_.get() ? impl_->index(str, len, start) : RW_NPOS;
        return rc;
    }

    /**
     * Returns the status of the regular expression:
     *
     * <table>
     * <tr>
     * <td><B>statusType </B> </td>
     * <td><B>Meaning </B> </td>
     * </tr>
     * <tr>
     * <td>\c RWTRegularExpression&lt;charT&gt;::%OK  </td>
     * <td>No errors  </td>
     * </tr>
     * <tr>
     * <td>\c RWTRegularExpression&lt;charT&gt;::%NOT_SUPPORTED  </td>
     * <td>POSIX.2 feature not yet supported.  </td>
     * </tr>
     * <tr>
     * <td>\c RWTRegularExpression&lt;charT&gt;::%NO_MATCH  </td>
     * <td>Tried to find a match but failed   </td>
     * </tr>
     * <tr>
     * <td>\c RWTRegularExpression&lt;charT&gt;::%BAD_PATTERN  </td>
     * <td>Pattern was illegal   </td>
     * </tr>
     * <tr>
     * <td>\c RWTRegularExpression&lt;charT&gt;::%BAD_COLLATING_ELEMENT  </td>
     * <td>Invalid collating element referenced  </td>
     * </tr>
     * <tr>
     * <td>\c RWTRegularExpression&lt;charT&gt;::%BAD_CHAR_CLASS_TYPE  </td>
     * <td>Invalid character class type referenced  </td>
     * </tr>
     * <tr>
     * <td>\c RWTRegularExpression&lt;charT&gt;::%TRAILING_BACKSLASH  </td>
     * <td>Trailing \ in pattern  </td>
     * </tr>
     * <tr>
     * <td>\c RWTRegularExpression&lt;charT&gt;::%UNMATCHED_BRACKET  </td>
     * <td>[] imbalance  </td>
     * </tr>
     * <tr>
     * <td>\c RWTRegularExpression&lt;charT&gt;::%UNMATCHED_PARENTHESIS  </td>
     * <td>() imbalance  </td>
     * </tr>
     * <tr>
     * <td>\c RWTRegularExpression&lt;charT&gt;::%UNMATCHED_BRACE  </td>
     * <td>{} imbalance  </td>
     * </tr>
     * <tr>
     * <td>\c RWTRegularExpression&lt;charT&gt;::%BAD_BRACE  </td>
     * <td>Content of {} invalid.  </td>
     * </tr>
     * <tr>
     * <td>\c RWTRegularExpression&lt;charT&gt;::%BAD_CHAR_RANGE  </td>
     * <td>Invalid endpoint in [a-z] expression  </td>
     * </tr>
     * <tr>
     * <td>\c RWTRegularExpression&lt;charT&gt;::%OUT_OF_MEMORY  </td>
     * <td>Out of memory  </td>
     * </tr>
     * <tr>
     * <td>\c RWTRegularExpression&lt;charT&gt;::%BAD_REPEAT  </td>
     * <td>?,* or + not preceded by valid regular expression  </td>
     * </tr>
     * </table>
     */
    statusType status() const {
        statusType rc = impl_.get() ? impl_->status() : RWREBaseClass::BAD_PATTERN;
        return rc;
    }

    RWCString  statusText() const {
        switch (status()) {
            case RWREBaseClass::OK:
                return "No errors";
            case RWREBaseClass::NOT_SUPPORTED:
                return "Not supported";
            case RWREBaseClass::NO_MATCH:
                return "Tried to find a match but failed";
            case RWREBaseClass::BAD_PATTERN:
                return "Pattern was illegal";
            case RWREBaseClass::BAD_COLLATING_ELEMENT:
                return "Invalid collating element referenced.";
            case RWREBaseClass::BAD_CHAR_CLASS_TYPE:
                return "Invalid character class type referenced";
            case RWREBaseClass::TRAILING_BACKSLASH:
                return "Trailing \\ in pattern.";
            case RWREBaseClass::UNMATCHED_BRACKET:
                return "[] imbalance";
            case RWREBaseClass::UNMATCHED_PARENTHESIS:
                return "() imbalance";
            case RWREBaseClass::UNMATCHED_BRACE:
                return "{} imbalance";
            case RWREBaseClass::BAD_BRACE:
                return "Content of {} invalid";
            case RWREBaseClass::BAD_CHAR_RANGE:
                return "Invalid endpoint in [a-z] expression";
            case RWREBaseClass::OUT_OF_MEMORY:
                return "Out of memory";
            case RWREBaseClass::BAD_REPEAT:
                return "?, * or + not preceded by valid regular expression";
            default:
                return "Unknown state.";
        }
    }

private:
    RWTScopedPointer<RWTRegularExpressionImp<charT> > impl_; // Regular expression implementation.

}; // RWTRegularExpression


#  undef RWC
/**
 * @ingroup string_processing_classes
 * @deprecated As of SourcePro 4, use \link RWTRegex RWTRegex<T>\endlink instead.
 * @relates RWTRegularExpression&lt;char&gt;
 *
 * Class RWCRExpr represents an <i>extended</i> regular expression
 * such as those found in \c lex and \c awk. The constructor "compiles"
 * the expression into a form that can be used more efficiently.
 * The results can then be used for string searches using class
 * RWCString.   Regular expressions can be of arbitrary size, limited
 * by memory. The extended regular expression features found here
 * are a subset of those found in the POSIX.2 standard (<i>ANSI/IEEE
 * Std 1003.2, ISO/IEC 9945-2)</i>.
 *
 * Note: RWCRExpr is available only if your compiler supports exception
 * handling and the C++ Standard Library.
 *
 * The regular expression (RE) is constructed as follows:
 *
 * The following rules determine one-character REs that match a
 * single character:
 *
 * Any character that is not a special character (to be defined)
 * matches itself.
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
 *    either "Humpty" or "Dumpty".
 * -# A set of characters enclosed in brackets (\c [ \c ]) is a one-character
 *    RE that matches any of the characters in that set. This means
 *    that \c [akm] matches either an "a", "k", or "m". A range of
 *    characters can be indicated with a dash, as in \c [a-z], which
 *    matches any lower-case letter. However, if the first character
 *    of the set is the caret (\c ^), then the RE matches any character
 *    except those in the set. It does not match the empty string.
 *    For example: \c [^akm] matches any character except  "a", "k",
 *    or "m". The caret loses its special meaning if it is not the
 *    first character of the set.
 *
 * The following rules can be used to build a multicharacter RE:
 *
 * -# Parentheses (( )) group parts of regular expressions together
 *    into subexpressions that can be treated as a single unit. For
 * -# A one-character RE followed by an asterisk (\c *) matches zero
 *    or more occurrences of the RE.  Hence, \c [a-z]* matches zero
 * -# A one-character RE followed by a plus (\c +) matches one or more
 *    occurrences of the RE.  Hence, \c [a-z]+ matches one or more
 * -# A question mark (\c ?) is an optional element. The preceding
 *    RE can occur zero or once in the string  --  no more. For example,
 * -# The concatenation of REs is a RE that matches the corresponding
 *    concatenation of strings. For example, \c [A-Z][a-z]* matches
 * -# The OR character ( \c | ) allows a choice between two regular
 *    expressions. For example, jell(y|ies) matches either "jelly"
 * -# Braces (\c { \c }) are reserved for future use.
 *
 * All or part of the regular expression can be "anchored" to either
 *  the beginning or end of the string being searched:
 *
 * -# If the caret (\c ^) is at the beginning of the (sub)expression,
 *    then the matched string must be at the beginning of the string
 * -# If the dollar sign (\c $) is at the end of the (sub)expression,
 *    then the matched string must be at the end of the string being
 *    searched.
 *
 * The most frequent problem with use of this class is in being
 * able to specify a backslash character to be parsed. If you are
 * attempting to parse a regular expression that contains backslashes,
 * you must be aware that the C++ compiler and the regular expression
 * constructor will both assume that any backslashes they see are
 * intended to escape the following character. Thus, to specify
 * a regular expression that exactly matches \c "a\a", you would
 * have to create the regular expression using four backslashes
 * as follows: the regular expression needs to see \c "a\\a", and
 * for that to happen, the compiler would have to see \c "a\\\\a".
 *
 * @code
 * RWCRExpr reg("a\\\\a");
 *                ^|^|
 *                 1 2
 * @endcode
 *
 * The backslashes marked with a \c ^ are an escape for the compiler,
 * and the ones marked with \c | will thus be seen by the regular
 * expression parser. At that point, the backslash marked \c 1 is
 * an escape, and the one marked \c 2 will actually be put into
 * the regular expression.
 *
 * Similarly, if you really need to escape a character, such as
 * a \c '.', you will have to pass two backslashes to the compiler:
 *
 * @code
 * RWCRExpr regDot("\\.")
 *                  ^|
 * @endcode
 *
 * Once again, the backslash marked \c ^ is an escape for the compiler,
 * and the one marked with \c | will be seen by the regular expression
 * constructor as an escape for the following \c '.'.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/re.h>
 * RWCRExpr re(".*\\.doc$");  // Matches filename with suffix ".doc"
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
 * #include <rw/re.h>
 * #include <rw/cstring.h>
 *
 * int main ()
 * {
 *     RWCString s ("Hark! Hark! the lark");
 *
 *
 *     std::cout << "Searching for an expression beginning with \"l\" in \""
 *               << s << "\".\n";
 *
 *     // A regular expression matching any lower-case word
 *     // starting with 'l':
 *     RWCRExpr reg("l[a-z]*");
 *
 *     // Prints 'lark'
 *     std::cout << "Found \"" << s.match(reg) << "\"." << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 */
typedef RW_DEPRECATE_TYPE("Use RWTRegex<char> instead") RWTRegularExpression<char> RWCRExpr;

RW_RESTORE_DEPRECATED_WARNINGS
#endif // RW_DISABLE_DEPRECATED

#else
#  error You must have exceptions to use this class.
#endif // !defined(RW_NO_EXCEPTIONS)

#endif /* RW_TOOLS_RE_H */
