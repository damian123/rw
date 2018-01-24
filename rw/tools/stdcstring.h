#ifndef RW_TOOLS_STDCSTRING_H
#define RW_TOOLS_STDCSTRING_H


/**********************************************************************
 *
 * Header file describing class RWStandardCString using basic_string
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/stdcstring.h#1 $
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

#if !defined(RW_COPY_ON_WRITE_STRING)
#  define RWStandardCString RWCString
#  define RWStandardCSubString RWCSubString
#  define RWStandardCConstSubString RWCConstSubString
#endif

class RW_TOOLS_GLOBAL RWStandardCString;
class RW_TOOLS_SYMBOLIC RWStandardCSubString;
class RW_TOOLS_SYMBOLIC RWStandardCConstSubString;

#include <rw/rwfile.h>
#include <rw/ref.h> // for backwards source compatibility
#include <rw/tools/hash.h>
#include <rw/tools/cstrutil.h>

#include <string>
#include <string.h>

template<class charT> class  RWTRegularExpression;

/**
 * @ingroup string_processing_classes
 * @deprecated As of SourcePro 4, use \link RWTRegex RWTRegex<char>\endlink instead.
 *
 * @brief Deprecated. This class is a typedef for
 * \link RWTRegularExpression RWTRegularExpression<char>\endlink.
 */
typedef RWTRegularExpression<char> RWCRExpr;

class  RWCRegexp;
class  RWCTokenizer;

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

// Document RWStandardCString and related classes.
/**
 * @class RWStandardCSubString rw/tools/stdcstring.h
 * @ingroup string_processing_classes
 * @brief Alternate implementation of RWCSubString when
 * \c RW_COPY_ON_WRITE_STRING is not defined.
 *
 * RWStandardCSubString should be directly used only with instances
 * of class RWStandardCString. RWCSubString should be used with instances
 * of class RWCString.
 *
 * @see RWCSubString for additional documentation.
 */

/**
 * @class RWStandardCConstSubString rw/tools/stdcstring.h
 * @ingroup string_processing_classes
 * @brief Alternate implementation of RWCConstSubString when
 * \c RW_COPY_ON_WRITE_STRING is not defined.
 *
 * RWStandardCConstSubString should be directly used only with instances
 * of class RWStandardCString. RWCConstSubString should be used with instances
 * of class RWCString.
 *
 * @see RWCConstSubString for additional documentation.
 */

/**
 * @class RWStandardCString rw/tools/stdcstring.h
 * @ingroup string_processing_classes
 * @brief Alternate implementation of RWCString when
 * \c RW_COPY_ON_WRITE_STRING is not defined.
 *
 * Unless the specific performance characteristics of RWStandardCString
 * are required, applications should use RWCString instead.  See the
 * <em>Essential Tools Module User's Guide</em> for additional information.
 *
 * @see RWCString for additional documentation.
 */


/**
 * @class RWCSubString rw/cstring.h
 * @ingroup string_processing_classes
 *
 * @brief Allows some subsection of an RWCString
 * to be addressed by defining a <i>starting position</i> and an
 * \e extent
 *
 * The class RWCSubString allows some subsection of an RWCString
 * to be addressed by defining a <i>starting position</i> and an
 * \e extent. For example the 7<sup>th</sup> through the
 * 11<sup>th</sup> elements, inclusive, would have a starting position of 7
 * and an extent of 5. The specification of a starting position and extent
 * can also be done on your behalf by such functions as RWCString::strip()
 * or the overloaded function call operator taking a regular expression
 * as an argument. There are no public constructors  --  RWCSubStrings
 * are constructed by various functions of the RWCString class and
 * then destroyed immediately.
 *
 * A <i>zero length</i> substring is one with a defined starting
 * position and an extent of zero. It can be thought of as starting
 * just before the indicated character, but not including it. It
 * can be used as an lvalue. A null substring is also legal and
 * is frequently used to indicate that a requested substring, perhaps
 * through a search, does not exist. A null substring can be detected
 * with member function isNull(). However, it cannot be used as
 * an lvalue.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/cstring.h>
 * RWCString s("test string");
 * s(6,3);     // "tri"
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
 * #include <rw/cstring.h>
 *
 * int main()
 * {
 *     RWCString s ("What I tell you is true.");
 *
 *     std::cout << "Take the string: [" << s << "]\n";
 *
 *     // Create a substring and use it as an lvalue:
 *     s(16, 0) = "three times ";
 *
 *     std::cout << "After assigning text to a substring, the full string becomes: ["
 *               << s << "]" << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 * Take the string: [What I tell you is true.]
 * After assigning text to a substring, the full string becomes: [What I tell you three times is true.]
 * @endcode
 */
class RW_TOOLS_SYMBOLIC RWStandardCSubString
{
public:

    RWStandardCSubString(const RWStandardCSubString& sp);

    /**
     * Assignment from an RWCSubString. Example:
     *
     * @code
     * RWCString a(10, 'a');
     * RWCString b;
     * ...
     * b(2, 3) = a(5,5);
     * @endcode
     *
     * Copies 5 characters of <tt>a</tt>'s data into the substring <tt>b(2,3)</tt>.
     * The number of elements need not match; if they differ, \c b is resized
     * appropriately. If self is the null substring, then the statement has no
     * effect. Returns a reference to self.
     */
    RWStandardCSubString& operator=(const RWStandardCSubString& str);

    /**
     * Assignment from an RWCConstSubString. Example:
     *
     * @code
     * const RWCString a(10, 'a');
     * RWCString b;
     * ...
     * b(2, 3) = a(5,5);
     * @endcode
     *
     * Copies 5 characters of <tt>a</tt>'s data into the substring <tt>b(2,3)</tt>.
     * The number of elements need not match; if they differ, \c b is resized
     * appropriately. If self is the null substring, then the statement has no
     * effect. Returns a reference to self.
     */
    RWStandardCSubString& operator=(const RWStandardCConstSubString& str);

    /**
     * Assignment from an RWCString. Example:
     *
     * @code
     * RWCString a;
     * RWCString b;
     * ...
     * b(2, 3) = a;
     * @endcode
     *
     * Copies <tt>a</tt>'s data into the substring <tt>b(2,3)</tt>. The number
     * of elements need not match; if they differ, \c b is resized
     * appropriately. If self is the null substring, then the statement
     * has no effect. Returns a reference to self.
     */
    RWStandardCSubString& operator=(const RWStandardCString& str);

    /**
     * Assignment from an \b std::string. Example:
     *
     * @code
     * std::string a;
     * RWCString b;
     * ...
     * b(2, 3) = a;
     * @endcode
     *
     * Copies <tt>a</tt>'s data into the substring <tt>b(2,3)</tt>. The number
     * of elements need not match; if they differ, \c b is resized
     * appropriately. If self is the null substring, then the statement
     * has no effect. Returns a reference to self.
     */
    RWStandardCSubString& operator=(const std::string& str);

    /**
     * Assignment from a character string. Example:
     *
     * @code
     * RWCString str("Mary had a lamb");
     * char dat[] = "Perrier";
     * str(11,4) = dat;  // "Mary had a Perrier"
     * @endcode
     *
     * The number of characters selected need not match; if they differ,
     * \c str is resized appropriately. If self is the null substring,
     * then the statement has no effect. Returns a reference to self.
     */
    RWStandardCSubString& operator=(const char* str);

    /**
     * Returns the \a i <sup>th</sup> character of the substring. The
     * index \a i must be between zero and the length of the substring
     * less one. Bounds checking is enabled by defining the preprocessor
     * macro \c RWBOUNDS_CHECK before including \c <rw/cstring.h>.
     *
     * @throw RWBoundsErr if RWBOUNDS_CHECK is defined and the index is out
     * of range.
     */
    char& operator()(size_t i);

    /**
     * Returns the \a i <sup>th</sup> character of the substring. The
     * index \a i must be between zero and the length of the substring
     * less one. Bounds checking is enabled by defining the preprocessor
     * macro \c RWBOUNDS_CHECK before including \c <rw/cstring.h>.
     *
     * @throw RWBoundsErr if RWBOUNDS_CHECK is defined and the index is out
     * of range.
     */
    char operator()(size_t i) const;


    // Here all integer types are provided with an exact match
    // to avoid ambiguities with the built in char[] operator
    // All operator[] functions are with bounds checking

    /**
     * Returns the \a i <sup>th</sup> character of the substring. The
     * index \a i must be between zero and the length of the substring
     * less one. \a i is converted to a \c size_t and bounds checking
     * is performed.
     *
     * @throw RWBoundsErr if the index is out of range.
     */
    char& operator[](char i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](char i) const;

    /**
     * @copydoc operator[](char)
     */
    char& operator[](signed char i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](signed char i) const;

    /**
     * @copydoc operator[](char)
     */
    char& operator[](short i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](short i) const;

    /**
     * @copydoc operator[](char)
     */
    char& operator[](int i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](int i) const;

    /**
     * @copydoc operator[](char)
     */
    char& operator[](long i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](long i) const;

#if !defined(RW_NO_LONG_LONG)
    /**
     * @copydoc operator[](char)
     */
    char& operator[](long long i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](long long i) const;
#endif

    /**
     * @copydoc operator[](char)
     */
    char& operator[](unsigned char i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](unsigned char i) const;

    /**
     * @copydoc operator[](char)
     */
    char& operator[](unsigned short i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](unsigned short i) const;

    /**
     * @copydoc operator[](char)
     */
    char& operator[](unsigned int i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](unsigned int i) const;

    /**
     * @copydoc operator[](char)
     */
    char& operator[](unsigned long i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](unsigned long i) const;

#if !defined(RW_NO_LONG_LONG)
    /**
     * @copydoc operator[](char)
     */
    char& operator[](unsigned long long i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](unsigned long long i) const;
#endif

    /**
     * Returns \c true if \c this is a null substring.
     */
    int operator!() const;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12, use startData() instead.
     */
    RW_DEPRECATE_FUNC("Use startData() instead")
    const char* data() const;
#endif

    /**
     * Returns \c true if \c this is a null substring.
     */
    bool isNull() const;

    /**
     * Returns the extent (i.e., length) of the substring.
     */
    size_t length() const;

    /**
     * Returns the index of the starting element of the substring.
     */
    size_t start() const;

    /**
     * @internal
     * Returns a pointer to the start of the substring.
     *
     * @note
     * Unlike RWCString::data(), the <tt>const char*</tt> returned by
     * this function is not null-terminated at its extent.
     */
    const char* startData() const;

    /**
     * Changes all upper-case letters in self to lower-case. Uses the
     * Standard C Library function \c tolower().
     */
    void toLower();

    /**
     * Changes all lower-case letters in self to upper-case. Uses the
     * Standard C Library function \c toupper().
     */
    void toUpper();

protected:

    /**
     * @internal
     * Verifies that the index \a i is within the bounds of the substring.
     *
     * @throw RWBoundsErr if the index is out of range.
     */
    void assertElement(size_t i) const;

    /**
     * @internal
     * Throws an RWBoundsErr exception indicating the position \a pos or
     * extent \a len is outside the string length \a strLen.
     */
    void subStringError(size_t strLen, size_t pos, size_t len) const;

private:

    /**
     * @internal
     * Constructs a substring instance from the string \a str with starting
     * position \a start and extent \a len.
     */
    RWStandardCSubString(const RWStandardCString& str, size_t start, size_t len);

    /**
     * @internal
     * Throws an exception indicating the position \a i is outside the
     * substring extent \a len.
     *
     * @throw RWBoundsErr
     */
    void throwBoundsErr(size_t i, size_t len) const;

    RWStandardCString* str_;
    size_t begin_;
    size_t extent_;

    friend class RWStandardCConstSubString;
    friend class RWStandardCString;
};


/**
 * @class RWCConstSubString rw/cstring.h
 * @ingroup string_processing_classes
 *
 * @brief Allows some subsection of an RWCString
 * to be addressed by defining a <i>starting position</i> and an
 * \e extent
 *
 * The class RWCConstSubString allows some subsection of an RWCString
 * to be addressed by defining a <i>starting position</i> and an
 * \e extent. For example, the 7<sup>th</sup> through the 11<sup>th</sup>
 * elements, inclusive, would have a starting position of 7 and an extent
 * of 5. The specification of a starting position and extent is
 * also done on your behalf by such functions as RWCString::strip().
 * There are no public constructors other than one that creates
 * an RWCConstSubString from an existing RWCSubString. In general,
 * RWCConstSubStrings are constructed by various functions of the
 * RWCString class and then destroyed immediately.
 *
 * A <i>zero length</i> substring is one with a defined starting
 * position and an extent of zero. It can be thought of as starting
 * just before the indicated character, but not including it. It
 * can be used as an lvalue. A null substring is also legal and
 * is frequently used to indicate that a requested substring, perhaps
 * through a search, does not exist. A null substring can be detected
 * with member function isNull(). However, it cannot be used
 * as an lvalue.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/cstring.h>
 * const RWCString s("test string");
 * s(6,3);     // "tri"
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
 * #include <rw/cstring.h>
 *
 * int main()
 * {
 *     const RWCString s ("What I tell you is true.");
 *
 *     std::cout << "Take the string: [" << s << "]\n";
 *
 *     // Create a string from substrings:
 *     const RWCString s2 (s(0, 16) + "three times " + s(16, 8));
 *
 *     std::cout << "After creating a string from substrings, you have: ["
 *               << s2 << "]" << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 * Take the string: [What I tell you is true.]
 * After creating a string from substrings, you have: [What I tell you three times is true.]
 * @endcode
 *
 */
class RW_TOOLS_SYMBOLIC RWStandardCConstSubString
{
public:

    /**
     * Copy constructor. The resulting substring references the same start
     * and extent of the RWCString associated with \a str.
     *
     * @note
     * The resulting substring instance should not be used if the
     * RWCString associated with \a str is modified after the substring
     * is instantiated.
     */
    RWStandardCConstSubString(const RWStandardCConstSubString& str);

    /**
     * Constructs an RWCConstSubString from an existing RWCSubString. The
     * resulting substring references the same start and extent of the
     * RWCString associated with \a str.
     *
     * @note
     * The resulting substring instance should not be used if the
     * RWCString associated with \a str is modified after the substring
     * is instantiated.
     */
    RWStandardCConstSubString(const RWStandardCSubString& str);

    /**
     * Returns the \a i <sup>th</sup> character of the substring. The
     * index \a i must be between zero and the length of the substring
     * less one. Bounds checking is enabled by defining the preprocessor
     * macro \c RWBOUNDS_CHECK before including \c <rw/cstring.h>.
     *
     * @throw RWBoundsErr if RWBOUNDS_CHECK is defined and the index is out
     * of range.
     */
    char operator()(size_t i) const;

    /**
     * @copydoc RWCSubString::operator[](char)const
     */
    char operator[](char i) const;

    /**
     * @copydoc RWCSubString::operator[](signed char)const
     */
    char operator[](signed char i) const;

    /**
     * @copydoc RWCSubString::operator[](short)const
     */
    char operator[](short i) const;

    /**
     * @copydoc RWCSubString::operator[](int)const
     */
    char operator[](int i) const;

    /**
     * @copydoc RWCSubString::operator[](long)const
     */
    char operator[](long i) const;

#if !defined(RW_NO_LONG_LONG)
    /**
     * @copydoc RWCSubString::operator[](long long)const
     */
    char operator[](long long i) const;
#endif

    /**
     * @copydoc RWCSubString::operator[](unsigned char)const
     */
    char operator[](unsigned char i) const;

    /**
     * @copydoc RWCSubString::operator[](unsigned short)const
     */
    char operator[](unsigned short i) const;

    /**
     * @copydoc RWCSubString::operator[](unsigned int)const
     */
    char operator[](unsigned int i) const;

    /**
     * @copydoc RWCSubString::operator[](unsigned long)const
     */
    char operator[](unsigned long i) const;

#if !defined(RW_NO_LONG_LONG)
    /**
     * @copydoc RWCSubString::operator[](unsigned long long)const
     */
    char operator[](unsigned long long i) const;
#endif

    /**
     * @copydoc RWCSubString::operator!()const
     */
    int operator!() const;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12, use startData() instead.
     */
    RW_DEPRECATE_FUNC("Use startData() instead")
    const char* data() const;
#endif

    /**
     * @copydoc RWCSubString::isNull()const
     */
    bool isNull() const;

    /**
     * @copydoc RWCSubString::length()const
     */
    size_t length() const;

    /**
     * Returns the index of the starting element of the substring.
     */
    size_t start() const;

    /**
     * @internal
     * Returns a pointer to the start of the substring.
     *
     * @note
     * Unlike RWCString::data(), the returned <tt>const char*</tt>
     * returned by this function is not null-terminated at its
     * extent.
     */
    const char* startData() const;

protected:

    /**
     * @internal
     * Verifies that the index \a i is within the bounds of the substring.
     *
     * @throw RWBoundsErr if the index is out of range.
     */
    void assertElement(size_t i) const;

    /**
     * @internal
     * Throws an RWBoundsErr exception indicating the position \a pos or
     * extent \a len is outside the string length \a strLen.
     */
    void subStringError(size_t strLen, size_t pos, size_t len) const;

private:

    /**
     * @internal
     * Constructs a substring instance from the string \a str with starting
     * position \a start and extent \a len.
     */
    RWStandardCConstSubString(const RWStandardCString& str, size_t start, size_t len);

    /**
     * @internal
     * Throws an exception indicating the position \a i is outside the
     * substring extent \a len.
     *
     * @throw RWBoundsErr
     */
    void throwBoundsErr(size_t i, size_t len) const;

    const RWStandardCString* str_; // Referenced string
    size_t begin_;         // Index of starting character
    size_t extent_;        // Length of RWStandardCConstSubString

    friend class RWStandardCString;
    friend class RWStandardCSubString;
};



/**
 * @class RWCString rw/cstring.h
 * @ingroup string_processing_classes
 *
 * @brief Offers powerful and convenient facilities for
 * manipulating strings.
 *
 * Class RWCString offers powerful and convenient facilities for
 * manipulating strings.
 *
 * @note
 * RWCString is designed for use with single or multibyte character
 * sequences. To manipulate wide character strings, use RWWString.
 *
 * Although the class is primarily intended to be used to handle
 * single-byte character sets (SBCS; such as US-ASCII or ISO Latin-1),
 * with care it can be used to handle multibyte character sets (MBCS).
 * There are two things that must be kept in mind when working with
 * MBCS:
 *
 * -  Because characters can be more than one byte long, the number
 *    of bytes in a string can, in general, be greater than the number
 *    of characters in the string. Use function RWCString::length()
 *    to get the number of bytes in a string, and function
 *    RWCString::mbLength() to get the number of characters. Note that the
 *    latter is much slower because it must determine the number of bytes
 *    in every character. Hence, if the string is known to use a SBCS,
 *    then RWCString::length() is preferred.
 * -  One or more bytes of a multibyte character can be zero. Hence,
 *    MBCS cannot be counted on being null-terminated. In practice,
 *    it is a rare MBCS that uses embedded nulls. Nevertheless, you
 *    should be aware of this and program defensively. In any case,
 *    class RWCString can handle embedded nulls.
 *
 * Parameters of type <tt>const char*</tt> must not be passed a value
 * of zero. This is detected in the debug version of the library.
 *
 * A separate class RWCSubString supports substring extraction and
 * modification operations. RWCConstSubString supports substring
 * extractions on \c const RWCString instances.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/cstring.h>
 * RWCString a;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Simple
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/cstring.h>
 *
 * int main()
 * {
 *     RWCString a("There is no joy in Beantown.");
 *
 *     std::cout << "\"" << a << "\"" << " becomes ";
 *
 *     a.subString("Beantown") = "Redmond";
 *
 *     std::cout << "\"" << a << "\"" << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 * "There is no joy in Beantown" becomes "There is no joy in Redmond."
 * @endcode
 */
class RW_TOOLS_GLOBAL RWStandardCString
{
public:
    /**
     * The mutable substring class associated with RWCString.
     */
    typedef RWStandardCSubString SubString;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12, use SubString instead.
     *
     * Retained for backward compatibility with SourcePro 11.1
     */
    typedef RW_DEPRECATE_TYPE("Use RWStandardCString::SubString instead") SubString SubStringType;
#endif

    /**
     * The immutable substring class associated with RWCString.
     */
    typedef RWStandardCConstSubString ConstSubString;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12, use ConstSubString instead.
     *
     * Retained for backward compatibility with SourcePro 11.1
     */
    typedef RW_DEPRECATE_TYPE("Use RWStandardCString::ConstSubString instead") ConstSubString ConstSubStringType;
#endif

    /**
     * Specifies semantics used by comparisons, searches, and hashing
     * functions
     */
    enum caseCompare {

        /**
         * Use exact case sensitive semantics
         */
        exact,

        /**
         * Ignore case sensitive semantics
         */
        ignoreCase,

        /**
         * Case-insensitive for alpha characters only.  #ignoreCaseStrict
         * only differs from #ignoreCase in comparison operations,
         * where changing case can affect the order of the strings.
         */
        ignoreCaseStrict
    };

#if !defined(RW_DISABLE_DEPRECATED)

    /**
     * @deprecated As of SourcePro 12, provided for compatibility with code that
     * relies on the enumeration RWStandardCString::scopeType.
     *
     * Specifies whether regular expression functions replace the first
     * #one substring matched by the regular expression or replaces #all
     * substrings matched by the regular expression.
     */
    enum RW_DEPRECATE_TYPE("") scopeType {

        /**
         * replaces first substring matched.
         */
        one,

        /**
         * replaces all substrings matched.
         */
        all
    };

#endif // RW_DISABLE_DEPRECATED

    /**
     * Specifies whether characters are stripped from the beginning
     * of the string, the end, or both.
     */
    enum stripType {

        /**
         * Remove characters at beginning
         */
        leading = 0x1,

        /**
         * Remove characters at end
         */
        trailing = 0x2,

        /**
         * Remove characters at both ends
         */
        both = 0x3
    };

    enum CharType {
        builtin
    };

public:
    /**
     * Creates a string of length zero (the null string).
     */
    RWStandardCString();

    /**
     * Copy constructor. The created string copies the data from \a str.
     */
    RWStandardCString(const RWStandardCString& str);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The created string takes ownership of the
     * data owned by \a str.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWStandardCString(RWStandardCString && str);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Converts from a substring. The created string copies
     * the substring represented by \a str.
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardCString(const SubString& str);

    /**
     * Converts from a const substring. The created string copies
     * the substring represented by \a str.
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardCString(const ConstSubString& str);

    /**
     * Constructs a string with data copied from \a str.
     */
#ifdef RW_DISABLE_IMPLICIT_CONVERSION
    explicit
#endif
    RWStandardCString(const std::string& str);

    /**
     * Converts from the null-terminated character string \a str. The
     * created string copies the data pointed to by \a str,
     * up to the first terminating null.
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
#ifdef RW_DISABLE_IMPLICIT_CONVERSION
    explicit
#endif
    RWStandardCString(const char* str);

    /**
     * Constructs a string from the character string \a str. The created
     * string copies the data pointed to by \a str. Exactly
     * \a len bytes are copied, <i>including any embedded nulls</i>. Hence,
     * the buffer pointed to by \a str must be at least \a len bytes long.
     */
    RWStandardCString(const char* str, size_t len);

    /**
     * Constructs a string containing the single character \a c.
     */
#ifdef RW_DISABLE_IMPLICIT_CONVERSION
    explicit
#endif
    RWStandardCString(char c);

    /**
     * Constructs a string containing the single signed character \a c.
     */
#ifdef RW_DISABLE_IMPLICIT_CONVERSION
    explicit
#endif
    RWStandardCString(signed char c);

    /**
     * Constructs a string containing the single unsigned character \a c.
     */
#ifdef RW_DISABLE_IMPLICIT_CONVERSION
    explicit
#endif
    RWStandardCString(unsigned char c);

    /**
     * Constructs a string containing the character \a c repeated
     * \a count times.
     */
    RWStandardCString(char c, size_t count);

    /**
     * Creates a string of length zero (the null string). The string's
     * \e capacity (that is, the size it can grow to without resizing)
     * is given by the parameter \a cap. We recommend creating an RWSize_T
     * value from a numerical constant to pass into this constructor.
     * While RWSize_T knows how to convert \c size_t values to itself,
     * conforming compilers choose the conversion to \c char instead.
     */
    RWStandardCString(RWSize_T cap);

    ~RWStandardCString();

    /**
     * The equivalent of calling RWCString::data() on self.
     */
#ifndef RW_DISABLE_IMPLICIT_CONVERSION
    operator const char* () const;
#endif

    /**
     * Assignment operator. The string copies the data from \a str.
     * Returns a reference to self.
     */
    RWStandardCString& operator=(const RWStandardCString& str);


#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a str.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWStandardCString& operator=(RWStandardCString && str);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * @copydoc operator=(const RWCString&)
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardCString& operator=(const SubString& str);

    /**
     * @copydoc operator=(const RWCString&)
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardCString& operator=(const ConstSubString& str);

    /**
     * @copydoc operator=(const RWCString&)
     */
    RWStandardCString& operator=(const std::string& str);

    /**
     * Assignment operator. Copies the null-terminated character string
     * pointed to by \a str into self. Returns a reference to self.
     *
     * @note
     * This function is incompatible with \a str strings with
     * embedded nulls.
     */
    RWStandardCString& operator=(const char* str);

    /**
     * Assignment operator. Copies the character \a c into self.
     */
    RWStandardCString& operator=(char c);

    /**
     * Appends the string \a str to self. Returns a reference to self.
     */
    RWStandardCString& operator+=(const RWStandardCString& str);

    /**
     * @copydoc operator+=(const RWCString&)
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardCString& operator+=(const SubString& str);

    /**
     * @copydoc operator+=(const RWCString&)
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardCString& operator+=(const ConstSubString& str);

    /**
     * @copydoc operator+=(const RWCString&)
     */
    RWStandardCString& operator+=(const std::string& str);

    /**
     * Appends the null-terminated character string pointed to by \a
     * str to self. Returns a reference to self.
     *
     * @note
     * This function is incompatible with \a str strings with
     * embedded nulls.
     */
    RWStandardCString& operator+=(const char* str);

    /**
     * Appends the character \a c to self. Returns a reference to self.
     */
    RWStandardCString& operator+=(char c);

    /**
     * Returns the \a i <sup>th</sup> byte in self. The index \a i must be
     * between 0 and the length of the string less one. Bounds checking
     * is performed if the preprocessor macro \c RWBOUNDS_CHECK has
     * been defined before including \c <rw/cstring.h>.
     *
     * @throw RWBoundsErr if RWBOUNDS_CHECK is defined and the index is out
     * of range.
     */
    char& operator()(size_t i);

    /**
     * Returns the \a i <sup>th</sup> byte in self. The index \a i must be
     * between 0 and the length of the string less one. Bounds checking
     * is performed if the preprocessor macro \c RWBOUNDS_CHECK has
     * been defined before including \c <rw/cstring.h>.
     *
     * @throw RWBoundsErr if RWBOUNDS_CHECK is defined and the index is out
     * of range.
     */
    char operator()(size_t i) const;

    // Here all integer types are provided with an exact match
    // to avoid ambiguities with the built in char[] operator
    // All operator[] functions are with bounds checking

    /**
     * Returns the \a i <sup>th</sup> byte in self. The index \a i must be
     * between 0 and the length of the string less one. \a i is converted
     * to a \c size_t and bounds checking is performed.
     *
     * @throw RWBoundsErr if the index is out of range.
     */
    char& operator[](char i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](char i) const;

    /**
     * @copydoc operator[](char)
     */
    char& operator[](signed char i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](signed char i) const;

    /**
     * @copydoc operator[](char)
     */
    char& operator[](short i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](short i) const;

    /**
     * @copydoc operator[](char)
     */
    char& operator[](int i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](int i) const;

    /**
     * @copydoc operator[](char)
     */
    char& operator[](long i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](long i) const;

#if !defined(RW_NO_LONG_LONG)
    /**
     * @copydoc operator[](char)
     */
    char& operator[](long long i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](long long i) const;
#endif

    /**
     * @copydoc operator[](char)
     */
    char& operator[](unsigned char i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](unsigned char i) const;

    /**
     * @copydoc operator[](char)
     */
    char& operator[](unsigned short i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](unsigned short i) const;

    /**
     * @copydoc operator[](char)
     */
    char& operator[](unsigned int i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](unsigned int i) const;

    /**
     * @copydoc operator[](char)
     */
    char& operator[](unsigned long i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](unsigned long i) const;

#if !defined(RW_NO_LONG_LONG)
    /**
     * @copydoc operator[](char)
     */
    char& operator[](unsigned long long i);

    /**
     * @copydoc operator[](char)
     */
    char  operator[](unsigned long long i) const;
#endif

    /**
     * Substring operator. Returns a substring of self with length
     * \a len, starting at index \a start. The sum of \a start plus
     * \a len must be less than or equal to the string length.
     *
     * @throws RWBoundsErr if the library was built using the
     * \c RW_DEBUG flag, and \a start or \a len are out of range.
     */
    SubString operator()(size_t start, size_t len);

    /**
     * Substring operator. Returns a substring of self with length
     * \a len, starting at index \a start. The sum of \a start plus
     * \a len must be less than or equal to the string length.
     *
     * @throws RWBoundsErr if the library was built using the
     * \c RW_DEBUG flag, and \a start or \a len are out of range.
     */
    ConstSubString operator()(size_t start, size_t len) const;

#if !defined(RW_DISABLE_DEPRECATED)
    RW_SUPPRESS_DEPRECATED_WARNINGS

    /**
     * @deprecated As of SourcePro 12, use RWTRegex::search instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::search instead")
    SubString operator()(const RWCRegexp& re);

    /**
     * @deprecated As of SourcePro 12, use RWTRegex::search instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::search instead")
    ConstSubString operator()(const RWCRegexp& re) const;

    /**
     * @deprecated As of SourcePro 12, use RWTRegex::search instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::search instead")
    SubString operator()(const RWCRegexp& re, size_t start);

    /**
     * @deprecated As of SourcePro 12, use RWTRegex::search instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::search instead")
    ConstSubString operator()(const RWCRegexp& re, size_t start) const;

    RW_RESTORE_DEPRECATED_WARNINGS
#endif

    /**
     * Appends a copy of the string \a str to self. Returns a reference
     * to self.
     */
    RWStandardCString& append(const RWStandardCString& str);

    /**
     * @copydoc append(const RWCString&)
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardCString& append(const SubString& str);

    /**
     * @copydoc append(const RWCString&)
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardCString& append(const ConstSubString& str);

    /**
     * Appends the first \a len bytes of \a str to self. If \a len is
     * greater than the length of \a str, the entire string is appended.
     * Returns a reference to self.
     */
    RWStandardCString& append(const RWStandardCString& str, size_t len);

    /**
     * @copydoc append(const RWCString&)
     */
    RWStandardCString& append(const std::string& str);

    /**
     * @copydoc append(const RWCString&,size_t)
     */
    RWStandardCString& append(const std::string& str, size_t len);

    /**
     * Appends a copy of the null-terminated character string pointed
     * to by \a str to self. Returns a reference to self.
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
    RWStandardCString& append(const char* str);

    /**
     * Appends a copy of the character string \a str to self. Exactly
     * \a len bytes are copied, <i>including any embedded nulls</i>. Hence,
     * the buffer pointed to by \a str must be at least \a len bytes long.
     * Returns a reference to self.
     */
    RWStandardCString& append(const char* str, size_t len);

    /**
     * Appends \a count copies of the character \a c to self. Returns a
     * reference to self.
     */
    RWStandardCString& append(char c, size_t count = 1);

    /**
     * Assigns a copy of the string \a str to self, replacing existing
     * content. Returns a reference to self.
     */
    RWStandardCString& assign(const RWStandardCString& str);

    /**
     * @copydoc assign(const RWCString&)
     */
    RWStandardCString& assign(const std::string& str);

    /**
     * @copydoc assign(const RWCString&)
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardCString& assign(const SubString& str);

    /**
     * @copydoc assign(const RWCString&)
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardCString& assign(const ConstSubString& str);

    /**
     * Assigns the null-terminated string pointed to by \a str to self,
     * replacing existing content. Returns a reference to self.
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
    RWStandardCString& assign(const char* str);

    /**
     * Assigns a copy of the character string \a str to self, replacing
     * existing content. Exactly \a len bytes are copied, <i>including
     * any embedded nulls</i>. Hence, the buffer pointed to by \a str must
     * be at least \a len bytes long. Returns a reference to self.
     */
    RWStandardCString& assign(const char* str, size_t len);

    /**
     * Assigns \a count copies of the character \a c to self, replacing
     * existing content. Returns a reference to self.
     */
    RWStandardCString& assign(char c, size_t count = 1);

    /**
     * Returns the number of bytes necessary to store the object using
     * the global functions:
     *
     * @code
     * RWFile& operator<<(RWFile&, const RWCString&);
     * RWvostream& operator<<(RWvostream&, const RWCString&);
     * @endcode
     */
    RWspace binaryStoreSize() const;

    /**
     * Given a multibyte sequence, \a str, and a number of multibyte
     * characters to consider, \a nChars, this method calculates the
     * number of bytes required to store the sequence. If \a nChars
     * is #RW_NPOS, then the method counts characters up to, but not
     * including, the first \c NULL character in the sequence. The method
     * returns the number of bytes required to represent the string.
     * If an error occurs during the operation, #RW_NPOS is returned.
     */
    static size_t byteCount(const char* str, size_t nChars = RW_NPOS);

    /**
     * Returns the current capacity of self. This is the number of bytes
     * the string can hold without resizing.
     */
    size_t capacity() const;

    /**
     * A non-binding request to alter the capacity of self to \c cap.
     * The capacity is never reduced below the current number of bytes.
     * Returns the actual capacity.
     */
    size_t capacity(size_t cap);

    /**
     * Returns an \c int less than, greater than, or equal to zero,
     * according to the result of calling the Standard C Library function
     * \c strcoll() on self and the argument \a str. This supports
     * locale-dependent collation.
     *
     * @note
     * This function is incompatible with strings with embedded nulls.
     */
    int collate(const RWStandardCString& str) const;

    /**
     * @copydoc collate(const RWCString&)const
     */
    int collate(const std::string& str) const;

    /**
     * @copydoc collate(const RWCString&)const
     */
    int collate(const char* str) const;

    /**
     * Lexicographically compares self to \a str.  Returns the following values
     * based on the results of the comparison:
     *
     * <table>
     * <tr><td align="right">&lt;0</td><td>self precedes \a str.</td></tr>
     * <tr><td align="right">0</td><td>self is equal to \a str.</td></tr>
     * <tr><td align="right">&gt;0</td><td>self follows \a str.</td></tr>
     * </table>
     *
     * If \a cmp is either RWCString::ignoreCase or
     * RWCString::ignoreCaseStrict, the strings are normalized before they are
     * compared.
     *
     * @note
     * If \a cmp is not RWCString::exact, this function is incompatible with
     * MBCS strings.
     */
    int compareTo(const RWStandardCString& str, caseCompare cmp = exact) const;

    /**
     * @copydoc compareTo(const RWCString&,caseCompare)const
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    int compareTo(const SubString& str, caseCompare cmp = exact) const;

    /**
     * @copydoc compareTo(const RWCString&,caseCompare)const
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    int compareTo(const ConstSubString& str, caseCompare cmp = exact) const;

    /**
     * This function is the equivalent of calling:
     *
     * @code
     * RWCString::compareTo(*str, cmp);
     * @endcode
     */
    int compareTo(const RWStandardCString* str, caseCompare cmp = exact) const;

    /**
     * @copydoc compareTo(const RWCString&,caseCompare)const
     */
    int compareTo(const std::string& str, caseCompare cmp = exact) const;

    /**
     * @copydoc compareTo(const RWCString*,caseCompare)const
     */
    int compareTo(const std::string* str, caseCompare cmp = exact) const;

    /**
     * @copydoc compareTo(const RWCString&,caseCompare)const
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
    int compareTo(const char* str, caseCompare cmp = exact) const;

    /**
     * Lexicographically compares self to \a str.  Returns the following values
     * based on the results of the comparison:
     *
     * <table>
     * <tr><td align="right">&lt;0</td><td>self precedes \a str.</td></tr>
     * <tr><td align="right">0</td><td>self is equal to \a str.</td></tr>
     * <tr><td align="right">&gt;0</td><td>self follows \a str.</td></tr>
     * </table>
     *
     * If \a cmp is either RWCString::ignoreCase or
     * RWCString::ignoreCaseStrict, the strings are normalized before they are
     * compared.
     *
     * @note
     * If \a cmp is not RWCString::exact, this function is incompatible with
     * MBCS strings.
     */
    int compareTo(const char* str, size_t len, caseCompare cmp = exact) const;

    /**
     * Returns \c false if RWCString::index(\a str, \a cmp) returns
     * #RW_NPOS, otherwise returns \c true.
     */
    bool contains(const RWStandardCString& str, caseCompare cmp = exact) const;

    /**
     * @copydoc contains(const RWCString&,caseCompare)const
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    bool contains(const SubString& str, caseCompare cmp = exact) const;

    /**
     * @copydoc contains(const RWCString&,caseCompare)const
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    bool contains(const ConstSubString& str, caseCompare cmp = exact) const;

    /**
     * @copydoc contains(const RWCString&,caseCompare)const
     */
    bool contains(const std::string& str, caseCompare cmp = exact) const;

    /**
     * @copydoc contains(const RWCString&,caseCompare)const
     */
    bool contains(const char* str, caseCompare cmp = exact) const;

    /**
     * Returns \c false if RWCString::index(\a str, \a len, \a cmp) returns
     * #RW_NPOS, otherwise returns \c true.
     */
    bool contains(const char* str, size_t len, caseCompare cmp = exact) const;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12, use operator=(const RWStandardCString&)
     * and modify the copy instead.
     */
    RW_DEPRECATE_FUNC("Use RWStandardCString::operator= instead")
    RWStandardCString copy() const;
#endif

    /**
     * Provides access to the RWCString instances's data as a null-terminated
     * string. This datum is owned by the RWCString and may not be deleted or
     * changed. If the RWCString object itself changes or goes out of scope,
     * the pointer value previously returned becomes invalid. While
     * the string is null-terminated, its \e length is still given by the member
     * function length(). That is, it may contain embedded nulls.
     */
    const char* data() const;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @deprecated As of SourcePro 12, use RWCString::firstOf(char, size_t) const instead.
     */
    RW_DEPRECATE_FUNC("Use RWCStandardCString::firstOf(...) instead")
    size_t first(char c) const;

    /**
     * @deprecated As of SourcePro 12, use RWCString::firstOf(char, size_t) const instead.
     */
    RW_DEPRECATE_FUNC("Use RWCStandardCString::firstOf(...) instead")
    size_t first(char c, size_t) const;

    /**
     * @deprecated As of SourcePro 12, use RWCString::firstOf(const char*, size_t) const instead.
     */
    RW_DEPRECATE_FUNC("Use RWCStandardCString::firstOf(...) instead")
    size_t first(const char* str) const;

    /**
     * @deprecated As of SourcePro 12, use RWCString::firstOf(const char*, size_t, size_t) const instead.
     */
    RW_DEPRECATE_FUNC("Use RWCStandardCString::firstOf(...) instead")
    size_t first(const char* str, size_t len) const;
#endif

    /**
     * Returns the index of the first occurrence of a character in self
     * that is not \a c, starting at position \a pos. Returns #RW_NPOS
     * if there is no match.
     */
    size_t firstNotOf(char c, size_t pos = 0) const;

    /**
     * Returns the index of the first occurrence in self, starting at
     * position \a pos, of any character not in the null-terminated string
     * \a str. Returns #RW_NPOS if there is no match.
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
    size_t firstNotOf(const char* str, size_t pos = 0) const;

    /**
     * Returns the index of the first occurrence in self, starting at
     * position \a pos, of any character not in the first \a len bytes of
     * \a str. Returns #RW_NPOS if there is no match.
     */
    size_t firstNotOf(const char* str, size_t pos, size_t len) const;

    /**
     * Returns the index of the first occurrence in self, starting at position
     * \a pos, of any character not in \a str. Returns #RW_NPOS if there is
     * no match.
     */
    size_t firstNotOf(const RWStandardCString& str, size_t pos = 0) const;

    /**
     * @copydoc firstNotOf(const RWCString&, size_t) const
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    size_t firstNotOf(const SubString& str, size_t pos = 0) const;

    /**
     * @copydoc firstNotOf(const RWCString&, size_t) const
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    size_t firstNotOf(const ConstSubString& str, size_t pos = 0) const;

    /**
     * @copydoc firstNotOf(const RWCString&, size_t) const
     */
    size_t firstNotOf(const std::string& str, size_t pos = 0) const;

    /**
     * Returns the index of the first occurrence of the character \a c
     * in self, starting at position \a pos. Returns #RW_NPOS if there is
     * no match.
     */
    size_t firstOf(char c, size_t pos = 0) const;

    /**
     * Returns the index of the first occurrence in self, starting at
     * position \a pos, of any character in the null-terminated string
     * \a str. Returns #RW_NPOS if there is no match.
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
    size_t firstOf(const char* str, size_t pos = 0) const;

    /**
     * Returns the index of the first occurrence in self, starting at
     * position \a pos, of any character in the first \a len bytes of
     * \a str. Returns #RW_NPOS if there is no match.
     */
    size_t firstOf(const char* str, size_t pos, size_t len) const;

    /**
     * Returns the index of the first occurrence in self, starting at position
     * \a pos, of any character in \a str. Returns #RW_NPOS if there is no
     * match.
     */
    size_t firstOf(const RWStandardCString& str, size_t pos = 0) const;

    /**
     * @copydoc firstOf(const RWCString&, size_t) const
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    size_t firstOf(const SubString& str, size_t pos = 0) const;

    /**
     * @copydoc firstOf(const RWCString&, size_t) const
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    size_t firstOf(const ConstSubString& str, size_t pos = 0) const;

    /**
     * @copydoc firstOf(const RWCString&, size_t) const
     */
    size_t firstOf(const std::string& str, size_t pos = 0) const;

    /**
     * Returns the hash value of \a str as returned by
     * hash(caseCompare) const, where #caseCompare is RWCString::exact.
     */
    static unsigned
    hash(const RWStandardCString& str);

    /**
     * Returns a suitable hash value.
     *
     * @note
     * If \a cmp is not RWCString::exact this function is incompatible
     * with MBCS strings.
     */
    unsigned hash(caseCompare cmp = exact) const;

    /**
     * Pattern matching. Starting with index \a start, searches for the
     * first occurrence of \a pat in self and returns the index of the
     * start of the match. Returns #RW_NPOS if there is no such pattern.
     * Case sensitivity is according to \a cmp.
     *
     * @note
     * If \a cmp is not RWCString::exact this function is incompatible
     * with MBCS strings.
     */
    size_t index(const RWStandardCString& pat, size_t start = 0 , caseCompare cmp = exact) const;

    /**
     * @copydoc index(const RWCString&,size_t,caseCompare)const
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    size_t index(const SubString& pat, size_t start = 0, caseCompare cmp = exact) const;

    /**
     * @copydoc index(const RWCString&,size_t,caseCompare)const
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    size_t index(const ConstSubString& pat, size_t start = 0, caseCompare cmp = exact) const;

    /**
     * Pattern matching. Starting with index \a start, searches for the
     * first occurrence of the first \a patlen bytes from \a pat in
     * self and returns the index of the start of the match. Returns
     * #RW_NPOS if there is no such pattern. Case sensitivity is according
     * to \a cmp.
     *
     * @note
     * If \a cmp is not RWCString::exact this function is incompatible
     * with MBCS strings.
     */
    size_t index(const RWStandardCString& pat, size_t patlen, size_t start, caseCompare cmp) const;

    /**
     * @copydoc index(const RWCString&,size_t,caseCompare)const
     */
    size_t index(const std::string& pat, size_t start = 0, caseCompare cmp = exact) const;

    /**
     * @copydoc index(const RWCString&,size_t,size_t,caseCompare)const
     */
    size_t index(const std::string& pat, size_t patlen, size_t start, caseCompare cmp) const;

    /**
     * Pattern matching. Starting with index \a start, searches for the
     * first occurrence of the null-terminated string \a pat in self and
     * returns the index of the start of the match. Returns #RW_NPOS if
     * there is no such pattern. Case sensitivity is according to \a cmp.
     *
     * @note
     * This function is incompatible with \a pat strings with embedded nulls.
     *
     * @note
     * If \a cmp is not RWCString::exact this function is incompatible
     * with MBCS strings.
     */
    size_t index(const char* pat, size_t start = 0 , caseCompare cmp = exact) const;

    /**
     * @copydoc index(const RWCString&,size_t,size_t,caseCompare)const
     */
    size_t index(const char* pat, size_t patlen, size_t start, caseCompare cmp) const;

    /**
     * @copydoc index(const RWCString&,size_t,caseCompare)const
     */
    size_t index(const char pat, size_t start = 0 , caseCompare cmp = exact) const;

#if !defined(RW_DISABLE_DEPRECATED)
    RW_SUPPRESS_DEPRECATED_WARNINGS

    /**
     * @deprecated As of SourcePro 12, use RWTRegex::index instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::index instead")
    size_t index(const RWCRegexp& re, size_t start = 0) const;

    /**
     * @deprecated As of SourcePro 12, use RWTRegex::index instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::index instead")
    size_t index(const RWCRegexp& re, size_t* ext, size_t start = 0) const;

    /**
     * @deprecated As of SourcePro 12, use RWTRegex::index instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::index instead")
    size_t index(const RWCRExpr& re, size_t start = 0) const;

    /**
     * @deprecated As of SourcePro 12, use RWTRegex::index instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::index instead")
    size_t index(const RWCRExpr& re, size_t* ext, size_t start = 0) const;

    /**
     * @deprecated As of SourcePro 12, use RWTRegex::index instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::index instead")
    size_t index(const char* re, size_t* ext, size_t start = 0) const;

    RW_RESTORE_DEPRECATED_WARNINGS
#endif // RW_DISABLE_DEPRECATED

    /**
     * Inserts a copy of the string \a str into self at index
     * \a pos. Returns a reference to self.
     */
    RWStandardCString& insert(size_t pos, const RWStandardCString& str);

    /**
     * @copydoc insert(size_t,const RWCString&)
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardCString& insert(size_t pos, const SubString& str);

    /**
     * @copydoc insert(size_t,const RWCString&)
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardCString& insert(size_t pos, const ConstSubString& str);

    /**
     * Inserts the first \a len bytes of \a str into self at index \a pos.
     * If \a len is greater than the length of \a str, the entire string
     * is inserted. Returns a reference to self.
     */
    RWStandardCString& insert(size_t pos, const RWStandardCString& str , size_t len);

    /**
     * @copydoc insert(size_t,const RWCString&)
     */
    RWStandardCString& insert(size_t pos, const std::string& str);

    /**
     * @copydoc insert(size_t,const RWCString&,size_t)
     */
    RWStandardCString& insert(size_t pos, const std::string& str, size_t len);

    /**
     * Inserts a copy of the null-terminated string \a str into self at
     * index \a pos. Returns a reference to self.
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
    RWStandardCString& insert(size_t pos, const char* str);

    /**
     * Inserts a copy of the first \a len bytes of \a str into self at byte
     * position \a pos. Returns a reference to self.
     */
    RWStandardCString& insert(size_t pos, const char* str , size_t len);

    /**
     * Inserts \a count instances of the character \a c into self at byte
     * position \a pos. Returns a reference to self.
     */
    RWStandardCString& insert(size_t pos, char c, size_t count);

    /**
     * Returns \c true if all bytes in self are between [0, 127].
     */
    bool isAscii() const;

    /**
     * Returns \c true if \c this is a zero length string (i.e., the null
     * string).
     */
    bool isNull() const;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @deprecated As of SourcePro 12, use RWCString::lastOf(char, size_t) const instead.
     */
    RW_DEPRECATE_FUNC("Use RWStandardCString::lastOf(...) instead")
    size_t last(char c) const;

    /**
     * @deprecated As of SourcePro 12, use RWCString::lastOf(char, size_t) const instead.
     */
    RW_DEPRECATE_FUNC("Use RWStandardCString::lastOf(...) instead")
    size_t last(char c, size_t) const;

    /**
     * @deprecated As of SourcePro 12, use RWCString::lastOf(const char*, size_t) const instead.
     */
    RW_DEPRECATE_FUNC("Use RWStandardCString::lastOf(...) instead")
    size_t last(const char* str) const;

    /**
     * @deprecated As of SourcePro 12, use RWCString::lastOf(const char*, size_t, size_t) const instead.
     */
    RW_DEPRECATE_FUNC("Use RWStandardCString::lastOf(...) instead")
    size_t last(const char* str, size_t len) const;
#endif

    /**
     * Returns the index of the last occurrence of a character in self
     * that is not \a c, ending at position \a pos. Returns #RW_NPOS
     * if there is no match.
     */
    size_t lastNotOf(char c, size_t pos = RW_NPOS) const;

    /**
     * Returns the index of the last occurrence in self, ending at
     * position \a pos, of any character not in the null-terminated string
     * \a str. Returns #RW_NPOS if there is no match.
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
    size_t lastNotOf(const char* str, size_t pos = RW_NPOS) const;

    /**
     * Returns the index of the last occurrence in self, ending at
     * position \a pos, of any character not in the last \a len bytes of
     * \a str. Returns #RW_NPOS if there is no match.
     */
    size_t lastNotOf(const char* str, size_t pos, size_t len) const;

    /**
     * Returns the index of the last occurrence in self, ending at position
     * \a pos, of any character not in \a str. Returns #RW_NPOS if there is
     * no match.
     */
    size_t lastNotOf(const RWStandardCString& str, size_t pos = RW_NPOS) const;

    /**
     * @copydoc lastNotOf(const RWCString&, size_t) const
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    size_t lastNotOf(const SubString& str, size_t pos = RW_NPOS) const;

    /**
     * @copydoc lastNotOf(const RWCString&, size_t) const
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    size_t lastNotOf(const ConstSubString& str, size_t pos = RW_NPOS) const;

    /**
     * @copydoc lastNotOf(const RWCString&, size_t) const
     */
    size_t lastNotOf(const std::string& str, size_t pos = RW_NPOS) const;

    /**
     * Returns the index of the last occurrence of the character \a c
     * in self, ending at position \a pos. Returns #RW_NPOS if there is
     * no match.
     */
    size_t lastOf(char c, size_t pos = RW_NPOS) const;

    /**
     * Returns the index of the last occurrence in self, ending at
     * position \a pos, of any character in the null-terminated string
     * \a str. Returns #RW_NPOS if there is no match.
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
    size_t lastOf(const char* str, size_t pos = RW_NPOS) const;

    /**
     * Returns the index of the last occurrence in self, ending at
     * position \a pos, of any character in the last \a len bytes of
     * \a str. Returns #RW_NPOS if there is no match.
     */
    size_t lastOf(const char* str, size_t pos, size_t len) const;

    /**
     * Returns the index of the last occurrence in self, ending at position
     * \a pos, of any character in \a str. Returns #RW_NPOS if there is
     * no match.
     */
    size_t lastOf(const RWStandardCString& str, size_t pos = RW_NPOS) const;

    /**
     * @copydoc lastOf(const RWCString&, size_t) const
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    size_t lastOf(const SubString& str, size_t pos = RW_NPOS) const;

    /**
     * @copydoc lastOf(const RWCString&, size_t) const
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    size_t lastOf(const ConstSubString& str, size_t pos = RW_NPOS) const;

    /**
     * @copydoc lastOf(const RWCString&, size_t) const
     */
    size_t lastOf(const std::string& str, size_t pos = RW_NPOS) const;

    /**
     * Returns the number of bytes in self.
     *
     * @note
     * If self contains any multibyte characters, RWCString::mbLength()
     * should be used to determine the number of characters in self.
     */
    size_t length() const;

#if !defined(RW_DISABLE_DEPRECATED)
    RW_SUPPRESS_DEPRECATED_WARNINGS

    /**
     * @deprecated As of SourcePro 12, use RWTRegex::search instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::search instead")
    SubString match(const RWCRExpr& re);

    /**
     * @deprecated As of SourcePro 12, use RWTRegex::search instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::search instead")
    ConstSubString match(const RWCRExpr&) const;

    /**
     * @deprecated As of SourcePro 12, use RWTRegex::search instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::search instead")
    SubString match(const RWCRExpr& re, size_t start);

    /**
     * @deprecated As of SourcePro 12, use RWTRegex::search instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::search instead")
    ConstSubString match(const RWCRExpr& re, size_t start) const;

    RW_RESTORE_DEPRECATED_WARNINGS
#endif // RW_DISABLE_DEPRECATED

    /**
     * Returns the number of multibyte characters in self, according
     * to the Standard C function \b mblen(). Returns #RW_NPOS if a
     * bad character is encountered. If self does not contain any
     * multibyte characters, then mbLength() == length().
     */
    size_t mbLength() const;

    /**
     * Given a multibyte character sequence, \a str, and a number of
     * bytes to consider, \a nBytes, this method calculates the number
     * of multibyte characters in the sequence. If \a nBytes is #RW_NPOS,
     * then the method counts the characters up to, but not including,
     * the first occurrence of a \c NULL character. The method returns
     * the length, in characters, of the input multibyte sequence if
     * successful. If an error occurs in the conversion, then #RW_NPOS
     * is returned.
     */
    static size_t mbLength(const char* str, size_t nBytes = RW_NPOS);

    /**
     * Prepends a copy of the string \a str to self. Returns a reference
     * to self.
     */
    RWStandardCString& prepend(const RWStandardCString& str);

    /**
     * @copydoc prepend(const RWCString&)
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardCString& prepend(const SubString& str);

    /**
     * @copydoc prepend(const RWCString&)
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardCString& prepend(const ConstSubString& str);

    /**
     * Prepends the first \a len bytes of \a str to self. If \a len is
     * greater than the length of \a str, the entire string is prepended.
     * Returns a reference to self.
     */
    RWStandardCString& prepend(const RWStandardCString& str , size_t len);

    /**
     * @copydoc prepend(const RWCString&)
     */
    RWStandardCString& prepend(const std::string& str);

    /**
     * @copydoc prepend(const RWCString&,size_t)
     */
    RWStandardCString& prepend(const std::string& str, size_t len);

    /**
     * Prepends a copy of the null-terminated character string pointed
     * to by \a str to self. Returns a reference to self.
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
    RWStandardCString& prepend(const char* str);

    /**
     * Prepends a copy of the character string \a str to self. Exactly
     * \a len bytes are copied, <i>including any embedded nulls</i>. Hence,
     * the buffer pointed to by \a str must be at least \a len bytes long.
     * Returns a reference to self.
     */
    RWStandardCString& prepend(const char* str , size_t len);

    /**
     * Prepends \a count copies of character \a c to self. Returns a reference
     * to self.
     */
    RWStandardCString& prepend(char c, size_t count = 1);

    /**
     * Reads bytes from the input stream \a strm, replacing the previous
     * contents of self, until \c EOF is reached. Null characters are
     * treated the same as other characters.
     */
    std::istream& readFile(std::istream& strm);

    /**
     * Reads bytes from the input stream \a strm, replacing the previous
     * contents of self, until a newline (or \c EOF) is encountered.
     * The newline is removed from the input stream but is not stored.
     * Null characters are treated the same as other characters. If
     * the \a skipWhite argument is \c true, then leading white space is
     * skipped (using the iostream library manipulator \c ws).
     */
    std::istream& readLine(std::istream& strm, bool skipWhite = true);

    /**
     * Reads bytes from the input stream \a strm, replacing the previous
     * contents of self, until a null terminator (or \c EOF) is encountered.
     */
    std::istream& readString(std::istream& strm);

    /**
     * Reads bytes from the input stream \a strm, replacing the previous
     * contents of self, until the delimiting character \a delim (or \c EOF)
     * is encountered. The delimiter is removed from the input
     * stream but is not stored.
     */
    std::istream& readToDelim(std::istream& strm, char delim = '\n');

    /**
     * White space is skipped before reading from \a strm. Bytes are
     * then read from the input stream \a strm, replacing previous contents
     * of self, until trailing white space or \c EOF is encountered.
     * The white space is left on the input stream. Null characters
     * are treated the same as other characters. White space is identified
     * by the Standard C Library function \c isspace().
     *
     * @note
     * This function is incompatible with streams containing MBCS.
     */
    std::istream& readToken(std::istream& strm);

    /**
     * Removes the bytes from index \a pos, which must be
     * no greater than length(), to the end of string. Returns a reference
     * to self.
     */
    RWStandardCString& remove(size_t pos);

    /**
     * Removes \a len bytes or to the end of string (whichever comes first)
     * starting at index \a pos, which must be no greater
     * than length(). Returns a reference to self.
     */
    RWStandardCString& remove(size_t pos, size_t len);

    /**
     * Replaces the substring beginning at \a pos and of length \a len
     * with the string \a str. If the sum of \a pos and \a len is greater
     * than the length of self, the substring replaced is from \a pos to
     * end of string.
     *
     * Returns a reference to self.
     *
     * @throws RWBoundsErr If \a pos is greater than the length of self.
     */
    RWStandardCString& replace(size_t pos, size_t len, const RWStandardCString& str);

    /**
     * @copydoc replace(size_t,size_t,const RWCString&)
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardCString& replace(size_t pos, size_t len, const SubString& str);

    /**
     * @copydoc replace(size_t,size_t,const RWCString&)
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardCString& replace(size_t pos, size_t len, const ConstSubString& str);

    /**
     * Replaces the substring beginning at \a pos and of length \a len
     * with the first \a strLen bytes of string \a str. If the sum of \a pos
     * and \a len is greater than the length of self, the substring replaced
     * is from \a pos to end of string. If \a strLen is greater than the
     * length of \a str, all of \a str is used.
     *
     * Returns a reference to self.
     *
     * @throws RWBoundsErr If \a pos is greater than the length of self.
     */
    RWStandardCString& replace(size_t pos, size_t len, const RWStandardCString& str, size_t strLen);

    /**
     * @copydoc replace(size_t,size_t,const RWCString&)
     */
    RWStandardCString& replace(size_t pos, size_t len, const std::string& str);

    /**
     * @copydoc replace(size_t,size_t,const RWCString&,size_t)
     */
    RWStandardCString& replace(size_t pos, size_t len, const std::string& str, size_t strLen);

    /**
     * Replaces the substring beginning at \a pos and of length \a len
     * with the null-terminated string \a str. If the sum of \a pos and
     * \a len is greater than the length of self, the substring replaced is
     * from \a pos to end of string.
     *
     * Returns a reference to self.
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     *
     * @throws RWBoundsErr If \a pos is greater than the length of self.
     */
    RWStandardCString& replace(size_t pos, size_t len, const char* str);

    /**
     * Replaces the substring beginning at \a pos and of length \a len
     * with \a strLen bytes from \a str. If the sum of \a pos and \a len is
     * greater than the length of self, the substring replaced is from \a pos
     * to end of string.
     *
     * Returns a reference to self.
     *
     * @throws RWBoundsErr If \a pos is greater than the length of self.
     */
    RWStandardCString& replace(size_t pos, size_t len, const char* str, size_t strLen);

#if !defined(RW_DISABLE_DEPRECATED)
    RW_SUPPRESS_DEPRECATED_WARNINGS

    /**
     * @deprecated As of SourcePro 12, use RWTRegex::replace instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::replace instead")
    RWStandardCString& replace(const RWCRExpr& re,
                               const RWStandardCString& str, scopeType scope = one);

    /**
     * @deprecated As of SourcePro 12, use RWTRegex::replace instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::replace instead")
    RWStandardCString& replace(const RWCRExpr& re,
                               const char* str, scopeType scope = one);

    RW_RESTORE_DEPRECATED_WARNINGS
#endif // RW_DISABLE_DEPRECATED

    /**
     * Changes the length of self to \a len bytes, adding blanks
     * (i.e., <tt>' '</tt>) or truncating as necessary.
     */
    void resize(size_t len);

    /**
     * @internal
     * Restores an instance of RWCString from \a fil into which an instance had
     * been persisted using saveOn(RWFile&).
     */
    void restoreFrom(RWFile& fil);

    /**
     * @internal
     * Restores an instance of RWCString from \a vis into which an instance had
     * been persisted using saveOn(RWvostream&).
     */
    void restoreFrom(RWvistream& vis);

    /**
     * Pattern matching. Searches for the last occurrence of \a pat in self
     * that occurs before index \a end and returns the index of the
     * start of the match. Returns #RW_NPOS if there is no such pattern.
     * Case sensitivity is according to \a cmp.
     *
     * @note
     * If \a cmp is not RWCString::exact this function is incompatible
     * with MBCS strings.
     */
    size_t rindex(const RWStandardCString& pat, size_t end = RW_NPOS, caseCompare cmp = exact) const;

    /**
     * @copydoc rindex(const RWCString&,size_t,caseCompare)const
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    size_t rindex(const SubString& pat,
                  size_t end = RW_NPOS, caseCompare cmp = exact) const;

    /**
     * @copydoc rindex(const RWCString&,size_t,caseCompare)const
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    size_t rindex(const ConstSubString& pat,
                  size_t end = RW_NPOS, caseCompare cmp = exact) const;

    /**
     * Pattern matching. Searches for the last occurrence of the first
     * \a patlen bytes from \a pat in self that occurs before index \a end
     * and returns the index of the start of the match. Returns #RW_NPOS
     * if there is no such pattern. Case sensitivity is according to \a cmp.
     *
     * @note
     * If \a cmp is not RWCString::exact this function is incompatible
     * with MBCS strings.
     */
    size_t rindex(const RWStandardCString& pat, size_t patlen, size_t end, caseCompare cmp) const;

    /**
     * @copydoc rindex(const RWCString&,size_t,caseCompare)const
     */
    size_t rindex(const std::string& pat,
                  size_t end = RW_NPOS, caseCompare cmp = exact) const;

    /**
     * @copydoc rindex(const RWCString&,size_t,size_t,caseCompare)const
     */
    size_t rindex(const std::string& pat, size_t patlen, size_t end, caseCompare cmp) const;

    /**
     * Pattern matching. Searches for the last occurrence of the
     * null-terminated string \a pat in self and returns the index of the
     * start of the match. Returns #RW_NPOS if there is no such pattern.
     * Case sensitivity is according to \a cmp.
     *
     * @note
     * This function is incompatible with \a pat strings with embedded nulls.
     *
     * @note
     * If \a cmp is not RWCString::exact this function is incompatible
     * with MBCS strings.
     */
    size_t rindex(const char* pat, caseCompare cmp) const;

    /**
     * Pattern matching. Searches for the last occurrence of the
     * null-terminated string \a pat in self that occurs before index \a end
     * and returns the index of the start of the match. Returns #RW_NPOS if
     * there is no such pattern. Case sensitivity is according to \a cmp.
     *
     * @note
     * This function is incompatible with \a pat strings with embedded nulls.
     *
     * @note
     * If \a cmp is not RWCString::exact this function is incompatible
     * with MBCS strings.
     */
    size_t rindex(const char* pat, size_t end = RW_NPOS, caseCompare cmp = exact) const;

    /**
     * @copydoc rindex(const RWCString&,size_t,size_t,caseCompare)const
     */
    size_t rindex(const char* pat, size_t patlen, size_t end, caseCompare cmp) const;

    /**
     * @copydoc rindex(const RWCString&,size_t,caseCompare)const
     */
    size_t rindex(const char pat, size_t end = RW_NPOS, caseCompare cmp = exact) const;

    /**
     * @internal
     * Saves an instance of RWCString to \a fil. The instance can be restored
     * using the member function restoreFrom(RWFile&).
     */
    void saveOn(RWFile& fil) const;

    /**
     * @internal
     * Saves an instance of RWCString to \a vos. The instance can be restored
     * using the member function restoreFrom(RWvistream&).
     */
    void saveOn(RWvostream& vos) const;

    /**
     * Attempts to minimize the memory used by self by reducing the capacity
     * to the minimum required to support the current string length. This
     * operation is non-binding, and may be ignored by the implementation.
     */
    void shrink();

    /**
     * Returns the underlying \b std::string representation of self.
     *
     * @note
     * - This member function is unavailable on RWCString if
     *   \c RW_COPY_ON_WRITE_STRING is defined.
     * - This member function is unavailable on RWCopyOnWriteCString
     */
    std::string& std();

    /**
     * Returns the underlying \b std::string representation of self.
     *
     * @note
     * - This member function is unavailable on RWCString if
     *   \c RW_COPY_ON_WRITE_STRING is defined.
     * - This member function is unavailable on RWCopyOnWriteCString
     */
    const std::string& std() const;

    /**
     * Swaps the contents of \a str with self.
     */
    void swap(RWStandardCString& str);

    /**
     * Starting at \a start, returns a substring representing the first
     * occurrence of the null-terminated string \a pat in self. If \a pat
     * is not found, a null substring instance is returned.
     *
     * @note
     * This function is incompatible with \a pat strings with embedded nulls.
     *
     * @note
     * If \a cmp is not RWCString::exact this function is incompatible
     * with MBCS strings.
     */
    SubString subString(const char* pat, size_t start = 0, caseCompare cmp = exact);

    /**
     * @copydoc subString(const char*,size_t,caseCompare)
     */
    ConstSubString subString(const char* pat, size_t start = 0, caseCompare cmp = exact) const;

    /**
     * Returns a substring of self where the character \a c has been
     * stripped off the beginning, end, or both ends of the string,
     * depending on \a st.
     */
    SubString strip(stripType st = trailing, char c = ' ');

    /**
     * @copydoc strip(stripType,char)
     */
    ConstSubString strip(stripType st = trailing, char c = ' ') const;

    /**
     * Changes all upper-case letters in self to lower-case, using the
     * Standard C Library facilities declared in \c <ctype.h>.
     *
     * @note
     * This function is incompatible with MBCS strings.
     */
    void toLower();

    /**
     * Changes all lower-case letters in self to upper-case, using the
     * Standard C Library facilities declared in \c <ctype.h>.
     *
     * @note
     * This function is incompatible with MBCS strings.
     */
    void toUpper();

protected:

    /**
     * @internal
     * Verifies that the index \a i is within the bounds of the substring.
     *
     * @throw RWBoundsErr if the index is out of range.
     */
    void assertElement(size_t i) const;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12.
     * This function is no longer used.
     */
    RW_DEPRECATE_FUNC("")
    int compareToSpecial(const char*, size_t, caseCompare) const;

    /**
     * @internal
     * @deprecated As of SourcePro 12.
     * This function is no longer used.
     */
    RW_DEPRECATE_FUNC("")
    size_t indexIgnoreCase(const char* pat, size_t patlen, size_t i) const;

    /**
     * @internal
     * @deprecated As of SourcePro 12.
     * This function is no longer used.
     */
    RW_DEPRECATE_FUNC("")
    size_t indexSpecial(const char*, size_t, size_t, caseCompare) const;

    /**
     * @internal
     * @deprecated As of SourcePro 12.
     * This function is no longer used.
     */
    RW_DEPRECATE_FUNC("")
    size_t rindexIgnoreCase(const char* pat, size_t patlen, size_t i) const;

    /**
     * @internal
     * @deprecated As of SourcePro 12.
     * This function is no longer used.
     */
    RW_DEPRECATE_FUNC("")
    size_t rindexSpecial(const char*, size_t, size_t, caseCompare) const;
#endif

    /**
     * @internal
     * Changes all upper-case in the substring beginning at \a pos with
     * length \a len in self to lower-case, using the Standard C Library
     * facilities declared in \c <ctype.h>.
     *
     * @note
     * This function is incompatible with MBCS strings.
     */
    void toLower(size_t pos, size_t len);

    /**
     * @internal
     * Changes all lower-case in the substring beginning at \a pos with
     * length \a len in self to upper-case, using the Standard C Library
     * facilities declared in \c <ctype.h>.
     *
     * @note
     * This function is incompatible with MBCS strings.
     */
    void toUpper(size_t pos, size_t len);

    friend class RWStandardCConstSubString;
    friend class RWStandardCSubString;
    friend class RWCTokenizer;

protected:

    std::string data_;

private:
    /**
     * @internal
     * Throws an exception indicating the position \a i is outside the
     * substring extent \a len.
     *
     * @throw RWBoundsErr
     */
    void throwBoundsErr(size_t i, size_t len) const;

};

/**
 * @relates RWCString
 * Returns the result of applying \c strxfrm() to \a str
 * to allow quicker collation than RWCString::collate(). Provided
 * only on platforms that provide \c strxfrm().
 *
 * @note
 * This function is incompatible with \a str strings with
 * embedded nulls.
 */
RW_TOOLS_SYMBOLIC RWStandardCString
strXForm(const RWStandardCString& str);

#if !defined(RW_DISABLE_DEPRECATED)
RW_SUPPRESS_DEPRECATED_WARNINGS
/**
 * @relates RWCString
 * @deprecated As of SourcePro 12, use strXForm(const RWCString&) instead.
 */
RW_DEPRECATE_FUNC("Use strXForm(const RWCString&) instead")
RW_TOOLS_SYMBOLIC RWStandardCString
strXForm(const std::string& str);
RW_RESTORE_DEPRECATED_WARNINGS
#endif

/**
 * @relates RWCString
 * Returns a copy of \a str with all upper-case characters replaced
 * with lower-case characters. This function is the equivalent of
 * calling:
 *
 * @code
 * RWCString tmp(str);
 * tmp.toLower();
 * @endcode
 */
RWStandardCString
toLower(const RWStandardCString& str);

/**
 * @relates RWCString
 * Returns a copy of \a str with all lower-case characters replaced
 * with upper-case characters. This function is the equivalent of
 * calling:
 *
 * @code
 * RWCString tmp(str);
 * tmp.toUpper();
 * @endcode
 */
RWStandardCString
toUpper(const RWStandardCString& str);

/**
 * @relates RWCString
 * Concatenates \a lhs and \a rhs and returns the resulting
 * string.
 */
RWStandardCString
operator+(const RWStandardCString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator+(const RWCString&,const RWCString&)
 */
RWStandardCString
operator+(const RWStandardCString& lhs,  const RWStandardCSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator+(const RWCString&,const RWCString&)
 */
RWStandardCString
operator+(const RWStandardCSubString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator+(const RWCString&,const RWCString&)
 */
RWStandardCString
operator+(const RWStandardCString& lhs,  const RWStandardCConstSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator+(const RWCString&,const RWCString&)
 */
RWStandardCString
operator+(const RWStandardCConstSubString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator+(const RWCString&,const RWCString&)
 */
RWStandardCString
operator+(const RWStandardCString& lhs,  const std::string& rhs);

/**
 * @relates RWCString
 * @copydoc operator+(const RWCString&,const RWCString&)
 */
RWStandardCString
operator+(const std::string& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * Concatenates \a lhs and the null-terminated string \a rhs and returns
 * the resulting string.
 *
 * @note
 * This function is incompatible with \a rhs strings with embedded nulls.
 */
RWStandardCString
operator+(const RWStandardCString& lhs,  const char* rhs);

/**
 * @relates RWCString
 * Concatenates the null-terminated string \a lhs and \a rhs and returns
 * the resulting string.
 *
 * @note
 * This function is incompatible with \a lhs strings with embedded nulls.
 */
RWStandardCString
operator+(const char* lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator+(const RWCString&,const RWCString&)
 */
RWStandardCString
operator+(const RWStandardCSubString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator+(const RWCString&,const RWCString&)
 */
RWStandardCString
operator+(const RWStandardCSubString& lhs,  const RWStandardCConstSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator+(const RWCString&,const RWCString&)
 */
RWStandardCString
operator+(const RWStandardCConstSubString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator+(const RWCString&,const RWCString&)
 */
RWStandardCString
operator+(const RWStandardCSubString& lhs,  const std::string& rhs);

/**
 * @relates RWCString
 * @copydoc operator+(const RWCString&,const RWCString&)
 */
RWStandardCString
operator+(const std::string& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCString
 * Concatenates \a lhs and the null-terminated string \a rhs and returns
 * the resulting string.
 *
 * @note
 * This function is incompatible with \a rhs strings with embedded nulls.
 */
RWStandardCString
operator+(const RWStandardCSubString& lhs,  const char* rhs);

/**
 * @relates RWCString
 * Concatenates the null-terminated string \a lhs and \a rhs and returns
 * the resulting string.
 *
 * @note
 * This function is incompatible with \a lhs strings with embedded nulls.
 */
RWStandardCString
operator+(const char* lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator+(const RWCString&,const RWCString&)
 */
RWStandardCString
operator+(const RWStandardCConstSubString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator+(const RWCString&,const RWCString&)
 */
RWStandardCString
operator+(const RWStandardCConstSubString& lhs,  const std::string& rhs);

/**
 * @relates RWCString
 * @copydoc operator+(const RWCString&,const RWCString&)
 */
RWStandardCString
operator+(const std::string& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCString
 * Concatenates \a lhs and the null-terminated string \a rhs and returns
 * the resulting string.
 *
 * @note
 * This function is incompatible with \a rhs strings with embedded nulls.
 */
RWStandardCString
operator+(const RWStandardCConstSubString& lhs,  const char* rhs);

/**
 * @relates RWCString
 * Concatenates the null-terminated string \a lhs and \a rhs and returns
 * the resulting string.
 *
 * @note
 * This function is incompatible with \a lhs strings with embedded nulls.
 */
RWStandardCString
operator+(const char* lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCString
 * Restores a string into \a str from an RWFile,
 * replacing the previous contents of \a str. If the
 * file produces an error on extraction, the operator
 * returns the original string contents unmodified. Be sure to
 * check the file to determine if an extraction
 * error occurred.
 */
RW_TOOLS_SYMBOLIC RWFile&
operator>>(RWFile& file,    RWStandardCString& str);

RW_TOOLS_SYMBOLIC RWFile&
operator>>(RWFile& file,    RWStandardCString*& cstr);

/**
 * @relates RWCString
 * Restores a string into \a str from a virtual stream,
 * replacing the previous contents of \a str. If the
 * virtual stream produces an error on extraction, the operator
 * returns the original string contents unmodified. Be sure to
 * check the virtual stream to determine if an extraction
 * error occurred.
 */
RW_TOOLS_SYMBOLIC RWvistream&
operator>>(RWvistream& vis, RWStandardCString& str);

RW_TOOLS_SYMBOLIC RWvistream&
operator>>(RWvistream& str, RWStandardCString*& cstr);

/**
 * @relates RWCString
 * Calls RWCString::readToken(std::istream&). That is, a token is read from
 * the input stream \a is.
 */
RW_TOOLS_SYMBOLIC std::istream&
operator>>(std::istream& is, RWStandardCString& str);

/**
 * @relates RWCString
 * Saves string \a str to an RWFile.
 */
RW_TOOLS_SYMBOLIC RWFile&
operator<<(RWFile& file,    const RWStandardCString& str);

/**
 * @relates RWCString
 * Saves string \a str to a virtual stream.
 */
RW_TOOLS_SYMBOLIC RWvostream&
operator<<(RWvostream& vos, const RWStandardCString& str);

/**
 * @relates RWCString
 * Output an RWCString on \b std::ostream \a os.
 */
RW_TOOLS_SYMBOLIC std::ostream&
operator<<(std::ostream& os, const RWStandardCString& str);

RW_TOOLS_SYMBOLIC std::ostream&
operator<<(std::ostream& os, const RWStandardCSubString& str);

RW_TOOLS_SYMBOLIC std::ostream&
operator<<(std::ostream& os, const RWStandardCConstSubString& str);

RW_TOOLS_SYMBOLIC std::wostream&
operator<<(std::wostream& os, const RWStandardCString& str);

RW_TOOLS_SYMBOLIC std::wostream&
operator<<(std::wostream& os, const RWStandardCSubString& str);

RW_TOOLS_SYMBOLIC std::wostream&
operator<<(std::wostream& os, const RWStandardCConstSubString& str);

/**
 * @relates RWCString
 * Returns \c true if \a lhs is lexicographically equal to \a rhs.
 * Otherwise returns \c false. Use member RWCString::collate()
 * or RWCString::strXForm() for locale-sensitive comparisons.
 */
bool
operator==(const RWStandardCString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator==(const RWCString&, const RWCString&)
 */
bool
operator==(const RWStandardCString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator==(const RWCString&, const RWCString&)
 */
bool
operator==(const RWStandardCSubString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator==(const RWCString&, const RWCString&)
 */
bool
operator==(const RWStandardCString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator==(const RWCString&, const RWCString&)
 */
bool
operator==(const RWStandardCConstSubString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator==(const RWCString&, const RWCString&)
 */
bool
operator==(const RWStandardCString& lhs, const std::string& rhs);

/**
 * @relates RWCString
 * @copydoc operator==(const RWCString&, const RWCString&)
 */
bool
operator==(const std::string& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator==(const RWCString&, const RWCString&)
 *
 * @note
 * This function is incompatible with \a rhs strings with
 * embedded nulls.
 */
bool
operator==(const RWStandardCString& lhs, const char* rhs);

/**
 * @relates RWCString
 * @copydoc operator==(const RWCString&, const RWCString&)
 *
 * @note
 * This function is incompatible with \a lhs strings with
 * embedded nulls.
 */
bool
operator==(const char* lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 *
 * Returns \c true if \a lhs has a length of one and the
 * first character \a lhs is identical to the char \a rhs.
 */
bool
operator==(const RWStandardCString& lhs, char rhs);

/**
 * @relates RWCString
 *
 * Returns \c true if \a rhs has a length of one and the
 * first character \a rhs is identical to the char \a lhs.
 */
bool
operator==(char lhs, const RWStandardCString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator==(const RWCString&, const RWCString&)
 */
bool
operator==(const RWStandardCSubString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator==(const RWCString&, const RWCString&)
 */
bool
operator==(const RWStandardCSubString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator==(const RWCString&, const RWCString&)
 */
bool
operator==(const RWStandardCConstSubString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator==(const RWCString&, const RWCString&)
 */
bool
operator==(const RWStandardCSubString& lhs, const std::string& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator==(const RWCString&, const RWCString&)
 */
bool
operator==(const std::string& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator==(const RWCString&, const char*)
 */
bool
operator==(const RWStandardCSubString& lhs, const char* rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator==(const char*, const RWCString&)
 */
bool
operator==(const char* lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator==(const RWCString&, char)
 */
bool
operator==(const RWStandardCSubString& lhs, char rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator==(char, const RWCString&)
 */
bool
operator==(char lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator==(const RWCString&, const RWCString&)
 */
bool
operator==(const RWStandardCConstSubString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator==(const RWCString&, const RWCString&)
 */
bool
operator==(const RWStandardCConstSubString& lhs, const std::string& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator==(const RWCString&, const RWCString&)
 */
bool
operator==(const std::string& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator==(const RWCString&, const char*)
 */
bool
operator==(const RWStandardCConstSubString& lhs, const char* rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator==(const char*, const RWCString&)
 */
bool
operator==(const char* lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator==(const RWCString&, char)
 */
bool
operator==(const RWStandardCConstSubString& lhs, char rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator==(char, const RWCString&)
 */
bool
operator==(char lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCString
 * The equivalent of calling:
 *
 * @code
 * !(lhs == rhs)
 * @endcode
 */
bool
operator!=(const RWStandardCString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const RWStandardCString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const RWStandardCSubString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const RWStandardCString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const RWStandardCConstSubString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const RWStandardCString& lhs, const std::string& rhs);

/**
 * @relates RWCString
 * @copydoc operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const std::string& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const RWStandardCString& lhs, const char* rhs);

/**
 * @relates RWCString
 * @copydoc operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const char* lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const RWStandardCString& lhs, char rhs);

/**
 * @relates RWCString
 * @copydoc operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(char lhs, const RWStandardCString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const RWStandardCSubString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const RWStandardCSubString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const RWStandardCConstSubString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const RWStandardCSubString& lhs, const std::string& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const std::string& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const RWStandardCSubString& lhs, const char* rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const char* lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const RWStandardCSubString& lhs, char rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(char lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const RWStandardCConstSubString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const RWStandardCConstSubString& lhs, const std::string& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const std::string& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const RWStandardCConstSubString& lhs, const char* rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const char* lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(const RWStandardCConstSubString& lhs, char rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator!=(const RWCString&, const RWCString&)
 */
bool
operator!=(char lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCString
 * Returns \c true if \a lhs lexicographically precedes \a rhs.
 * Otherwise returns \c false. Use member RWCString::collate() or
 * RWCString::strXForm() for locale-sensitive comparisons.
 */
bool
operator< (const RWStandardCString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator<(const RWCString&, const RWCString&)
 */
bool
operator< (const RWStandardCString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator<(const RWCString&, const RWCString&)
 */
bool
operator< (const RWStandardCSubString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator<(const RWCString&, const RWCString&)
 */
bool
operator< (const RWStandardCString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator<(const RWCString&, const RWCString&)
 */
bool
operator< (const RWStandardCConstSubString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator<(const RWCString&, const RWCString&)
 */
bool
operator< (const RWStandardCString& lhs, const std::string& rhs);

/**
 * @relates RWCString
 * @copydoc operator<(const RWCString&, const RWCString&)
 */
bool
operator< (const std::string& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator<(const RWCString&, const RWCString&)
 *
 * @note
 * This function is incompatible with \a rhs strings with
 * embedded nulls.
 */
bool
operator< (const RWStandardCString& lhs, const char* rhs);

/**
 * @relates RWCString
 * @copydoc operator<(const RWCString&, const RWCString&)
 *
 * @note
 * This function is incompatible with \a lhs strings with
 * embedded nulls.
 */
bool
operator< (const char* lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * Returns \c true if \a lhs is empty or the first character in
 * \a lhs is less than \a rhs. Otherwise returns false.
 */
bool
operator< (const RWStandardCString& lhs, char rhs);

/**
 * @relates RWCString
 * Returns true if the length of \a rhs is at least one and the
 * first character of \a rhs is greater than \a lhs.
 */
bool
operator< (char lhs, const RWStandardCString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator<(const RWCString&, const RWCString&)
 */
bool
operator< (const RWStandardCSubString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator<(const RWCString&, const RWCString&)
 */
bool
operator< (const RWStandardCSubString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator<(const RWCString&, const RWCString&)
 */
bool
operator< (const RWStandardCConstSubString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator<(const RWCString&, const RWCString&)
 */
bool
operator< (const RWStandardCSubString& lhs, const std::string& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator<(const RWCString&, const RWCString&)
 */
bool
operator< (const std::string& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator<(const RWCString&, const char*)
 */
bool
operator< (const RWStandardCSubString& lhs, const char* rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator<(const char*, const RWCString&)
 */
bool
operator< (const char* lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator<(const RWCString&, char)
 */
bool
operator< (const RWStandardCSubString& lhs, char rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator<(char, const RWCString&)
 */
bool
operator< (char lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator<(const RWCString&, const RWCString&)
 */
bool
operator< (const RWStandardCConstSubString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator<(const RWCString&, const RWCString&)
 */
bool
operator< (const RWStandardCConstSubString& lhs, const std::string& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator<(const RWCString&, const RWCString&)
 */
bool
operator< (const std::string& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator<(const RWCString&, const char*)
 */
bool
operator< (const RWStandardCConstSubString& lhs, const char* rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator<(const char*, const RWCString&)
 */
bool
operator< (const char* lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator<(const RWCString&, char)
 */
bool
operator< (const RWStandardCConstSubString& lhs, char rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator<(char, const RWCString&)
 */
bool
operator< (char lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCString
 * The equivalent of calling:
 *
 * @code
 * !(rhs < lhs)
 * @endcode
 */
bool
operator<=(const RWStandardCString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const RWStandardCString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const RWStandardCSubString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const RWStandardCString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const RWStandardCConstSubString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const RWStandardCString& lhs, const std::string& rhs);

/**
 * @relates RWCString
 * @copydoc operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const std::string& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const RWStandardCString& lhs, const char* rhs);

/**
 * @relates RWCString
 * @copydoc operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const char* lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const RWStandardCString& lhs, char rhs);

/**
 * @relates RWCString
 * @copydoc operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(char lhs, const RWStandardCString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const RWStandardCSubString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const RWStandardCSubString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const RWStandardCConstSubString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const RWStandardCSubString& lhs, const std::string& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const std::string& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const RWStandardCSubString& lhs, const char* rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const char* lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const RWStandardCSubString& lhs, char rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(char lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const RWStandardCConstSubString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const RWStandardCConstSubString& lhs, const std::string& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const std::string& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const RWStandardCConstSubString& lhs, const char* rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const char* lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(const RWStandardCConstSubString& lhs, char rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator<=(const RWCString&, const RWCString&)
 */
bool
operator<=(char lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCString
 * The equivalent of calling:
 *
 * @code
 * !(lhs < rhs)
 * @endcode
 */
bool
operator>=(const RWStandardCString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const RWStandardCString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const RWStandardCSubString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const RWStandardCString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const RWStandardCConstSubString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const RWStandardCString& lhs, const std::string& rhs);

/**
 * @relates RWCString
 * @copydoc operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const std::string& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const RWStandardCString& lhs, const char* rhs);

/**
 * @relates RWCString
 * @copydoc operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const char* lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const RWStandardCString& lhs, char rhs);

/**
 * @relates RWCString
 * @copydoc operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(char lhs, const RWStandardCString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const RWStandardCSubString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const RWStandardCSubString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const RWStandardCConstSubString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const RWStandardCSubString& lhs, const std::string& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const std::string& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const RWStandardCSubString& lhs, const char* rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const char* lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const RWStandardCSubString& lhs, char rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(char lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const RWStandardCConstSubString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const RWStandardCConstSubString& lhs, const std::string& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const std::string& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const RWStandardCConstSubString& lhs, const char* rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const char* lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(const RWStandardCConstSubString& lhs, char rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator>=(const RWCString&, const RWCString&)
 */
bool
operator>=(char lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCString
 * The equivalent of calling:
 *
 * @code
 * !(rhs < lhs)
 * @endcode
 */
bool
operator> (const RWStandardCString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const RWStandardCString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const RWStandardCSubString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const RWStandardCString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCString
 * @copydoc operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const RWStandardCConstSubString& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const RWStandardCString& lhs, const std::string& rhs);

/**
 * @relates RWCString
 * @copydoc operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const std::string& lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const RWStandardCString& lhs, const char* rhs);

/**
 * @relates RWCString
 * @copydoc operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const char* lhs, const RWStandardCString& rhs);

/**
 * @relates RWCString
 * @copydoc operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const RWStandardCString& lhs, char rhs);

/**
 * @relates RWCString
 * @copydoc operator>(const RWCString&, const RWCString&)
 */
bool
operator> (char lhs, const RWStandardCString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const RWStandardCSubString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const RWStandardCSubString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const RWStandardCConstSubString& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const RWStandardCSubString& lhs, const std::string& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const std::string& lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const RWStandardCSubString& lhs, const char* rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const char* lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const RWStandardCSubString& lhs, char rhs);

/**
 * @relates RWCSubString
 * @copydoc RWCString::operator>(const RWCString&, const RWCString&)
 */
bool
operator> (char lhs, const RWStandardCSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const RWStandardCConstSubString& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const RWStandardCConstSubString& lhs, const std::string& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const std::string& lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const RWStandardCConstSubString& lhs, const char* rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const char* lhs, const RWStandardCConstSubString& rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator>(const RWCString&, const RWCString&)
 */
bool
operator> (const RWStandardCConstSubString& lhs, char rhs);

/**
 * @relates RWCConstSubString
 * @copydoc RWCString::operator>(const RWCString&, const RWCString&)
 */
bool
operator> (char lhs, const RWStandardCConstSubString& rhs);


//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                               Inlines                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

inline
RWStandardCSubString::RWStandardCSubString(const RWStandardCSubString& sp)
    : str_(sp.str_), begin_(sp.begin_), extent_(sp.extent_)
{
}

inline size_t
RWStandardCSubString::length() const
{
    return extent_;
}

inline size_t
RWStandardCSubString::start() const
{
    return begin_;
}

inline bool
RWStandardCSubString::isNull() const
{
    return begin_ == RW_NPOS;
}

inline int
RWStandardCSubString::operator!() const
{
    return begin_ == RW_NPOS;
}

inline
RWStandardCConstSubString::RWStandardCConstSubString(const RWStandardCSubString& sp)
    : str_(sp.str_), begin_(sp.begin_), extent_(sp.extent_)
{
}

inline
RWStandardCConstSubString::RWStandardCConstSubString(const RWStandardCConstSubString& sp)
    : str_(sp.str_), begin_(sp.begin_), extent_(sp.extent_)
{
}

#if !defined(RW_DISABLE_DEPRECATED)

inline const char*
RWStandardCConstSubString::data() const
{
    return startData();
}

#endif // RW_DISABLE_DEPRECATED

inline size_t
RWStandardCConstSubString::length() const
{
    return extent_;
}

inline size_t
RWStandardCConstSubString::start() const
{
    return begin_;
}

inline bool
RWStandardCConstSubString::isNull() const
{
    return begin_ == RW_NPOS;
}

inline int
RWStandardCConstSubString::operator!() const
{
    return begin_ == RW_NPOS;
}

inline
RWStandardCString::RWStandardCString()
{
}

inline
RWStandardCString::RWStandardCString(const std::string& s)
    : data_(s)
{
}

inline RWStandardCString::RWStandardCString(const RWStandardCString& str)
    : data_(str.data_)
{
}

#if !defined(RW_NO_RVALUE_REFERENCES)

inline
RWStandardCString::RWStandardCString(RWStandardCString && str)
    : data_()
{
    str.swap(*this);
}

#endif // !RW_NO_RVALUE_REFERENCES

inline
RWStandardCString::RWStandardCString(char c, size_t N)
    : data_(N, c)
{
}

inline
RWStandardCString::RWStandardCString(const char* s)
    : data_(s)
{
}

inline
RWStandardCString::RWStandardCString(const char* s, size_t N)
    : data_(s, N)
{
}

inline
RWStandardCString::RWStandardCString(char c)
    : data_(1, c)
{
}

inline
RWStandardCString::RWStandardCString(signed char c)
    : data_(1, RW_STATIC_CAST(char, c))
{
}

inline
RWStandardCString::RWStandardCString(unsigned char c)
    : data_(1, RW_STATIC_CAST(char, c))
{
}

inline
RWStandardCString::~RWStandardCString()
{
}

#ifndef RW_DISABLE_IMPLICIT_CONVERSION
inline
RWStandardCString::operator const char* () const
{
    return data();
}
#endif

inline RWStandardCString&
RWStandardCString::append(char c, size_t N)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (N) {
        data_.append(N, c);
    }
#else
    data_.append(N, c);
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::append(const char* cs)
{
    RW_PRECONDITION2(cs != rwnil,
                     "RWStandardCString::append(const char* cs): null pointer");
    data_.append(cs);
    return *this;
}

inline RWStandardCString&
RWStandardCString::append(const RWStandardCString& str)
{
    data_.append(str.std());
    return *this;
}

inline RWStandardCString&
RWStandardCString::append(const RWStandardCString::SubString& str)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (!str.isNull()) {
        data_.append(str.startData(), str.length());
    }
#else
    data_.append(str.startData(), str.length());
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::append(const RWStandardCString::ConstSubString& str)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (!str.isNull()) {
        data_.append(str.startData(), str.length());
    }
#else
    data_.append(str.startData(), str.length());
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::append(const std::string& str)
{
    data_.append(str);
    return *this;
}

inline RWStandardCString&
RWStandardCString::operator+=(char c)
{
    return append(c);
}

inline RWStandardCString&
RWStandardCString::operator+=(const char* cs)
{
    return append(cs);
}

inline RWStandardCString&
RWStandardCString::operator+=(const std::string& str)
{
    return append(str);
}

inline RWStandardCString&
RWStandardCString::operator+=(const RWStandardCString& str)
{
    return append(str);
}

inline RWStandardCString&
RWStandardCString::operator+=(const RWStandardCString::SubString& str)
{
    return append(str);
}

inline RWStandardCString&
RWStandardCString::operator+=(const RWStandardCString::ConstSubString& str)
{
    return append(str);
}

inline RWspace
RWStandardCString::binaryStoreSize() const
{
    return length() + sizeof(size_t);
}

inline size_t
RWStandardCString::capacity(size_t capac)
{
    data_.reserve(capac);
    return data_.capacity();
}

inline
RWStandardCString::RWStandardCString(RWSize_T ic)
{
    capacity(ic.value());
}

inline int
RWStandardCString::compareTo(const std::string& str, caseCompare cmp) const
{
    if (cmp == exact) {
        return rwCStrCompareExact(data(), length(), str.data(), str.length());
    }
    else if (cmp == ignoreCase) {
        return rwCStrCompareIgnoreCase(data(), length(), str.data(), str.length());
    }
    else {
        return rwCStrCompareIgnoreCaseStrict(data(), length(), str.data(), str.length());
    }
}

inline int
RWStandardCString::compareTo(const std::string* str, caseCompare cmp) const
{
    RW_PRECONDITION2(str != rwnil,
                     "RWStandardCString::compareTo(const std::string* str, caseCompare cmp) "
                     "const: null pointer");
    return compareTo(*str, cmp);
}

inline int
RWStandardCString::compareTo(const RWStandardCString& str, caseCompare cmp) const
{
    if (cmp == exact) {
        return rwCStrCompareExact(data(), length(), str.data(), str.length());
    }
    else if (cmp == ignoreCase) {
        return rwCStrCompareIgnoreCase(data(), length(), str.data(), str.length());
    }
    else {
        return rwCStrCompareIgnoreCaseStrict(data(), length(), str.data(), str.length());
    }
}

inline int
RWStandardCString::compareTo(const RWStandardCString::SubString& str, caseCompare cmp) const
{
    if (cmp == exact) {
        return rwCStrCompareExact(data(), length(), str.startData(), str.length());
    }
    else if (cmp == ignoreCase) {
        return rwCStrCompareIgnoreCase(data(), length(), str.startData(), str.length());
    }
    else {
        return rwCStrCompareIgnoreCaseStrict(data(), length(), str.startData(), str.length());
    }
}

inline int
RWStandardCString::compareTo(const RWStandardCString::ConstSubString& str, caseCompare cmp) const
{
    if (cmp == exact) {
        return rwCStrCompareExact(data(), length(), str.startData(), str.length());
    }
    else if (cmp == ignoreCase) {
        return rwCStrCompareIgnoreCase(data(), length(), str.startData(), str.length());
    }
    else {
        return rwCStrCompareIgnoreCaseStrict(data(), length(), str.startData(), str.length());
    }
}

inline int
RWStandardCString::compareTo(const RWStandardCString* str, caseCompare cmp) const
{
    RW_PRECONDITION2(str != rwnil,
                     "RWStandardCString::compareTo(const RWStandardCString* str, caseCompare cmp) "
                     "const: null pointer");
    return compareTo(*str, cmp);
}

inline const char*
RWStandardCString::data() const
{
    return data_.c_str();
}

inline const std::string&
RWStandardCString::std() const
{
    return data_;
}

inline unsigned
RWStandardCString::hash(const RWStandardCString& str)
{
    return unsigned(str.hash());
}

inline bool
RWStandardCString::isNull() const
{
    return data_.empty();
}

inline size_t
RWStandardCString::length() const
{
    return data_.length();
}

inline void
RWStandardCString::toLower()
{
    toLower(0, length());
}

inline void
RWStandardCString::toUpper()
{
    toUpper(0, length());
}

////
//  This RWStandardCSubString ctor appears out of sequence here
//  to avoid inlined after called warnings
///
inline
RWStandardCSubString::RWStandardCSubString(const RWStandardCString& str, size_t pos, size_t n)
    : str_(const_cast<RWStandardCString*>(&str)),
      begin_(pos),
      extent_(n)
{
#ifdef RW_DEBUG
    size_t len = str.length();

    // Allow zero lengthed and null substrings:
    if ((pos == RW_NPOS && n != RW_NPOS && n != 0) ||
            (pos != RW_NPOS && (pos > len || n > len || pos + n > len))) {
        subStringError(len, pos, n);
    }
#endif
}

inline RWStandardCString::RWStandardCString(const RWStandardCSubString& s)
{
    if (!s.isNull()) {
        data_.assign(s.str_->data_, s.begin_, s.extent_);
    }
}

inline RWStandardCString::RWStandardCString(const RWStandardCConstSubString& s)
{
    if (!s.isNull()) {
        data_.assign(s.str_->data_, s.begin_, s.extent_);
    }
}

inline size_t
RWStandardCString::mbLength() const
{
    return RWStandardCString::mbLength(data(), length());
}

//Assignment
inline RWStandardCString&
RWStandardCString::assign(const RWStandardCString& str)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not attempt to use
    // existing capacity when assigning string with assign(),
    // however they did with operator=.
    data_ = str.std();
#else
    data_.assign(str.std());
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::assign(const std::string& str)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not attempt to use
    // existing capacity when assigning string with assign(),
    // however they did with operator=.
    data_ = str;
#else
    data_.assign(str);
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::assign(const SubString& str)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (!str.isNull()) {
        data_.assign(str.startData(), str.length());
    }
    else {
        remove(0);
    }
#else
    data_.assign(str.startData(), str.length());
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::assign(const ConstSubString& str)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (!str.isNull()) {
        data_.assign(str.startData(), str.length());
    }
    else {
        remove(0);
    }
#else
    data_.assign(str.startData(), str.length());
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::assign(const char* str)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not attempt to use
    // existing capacity when assigning string with assign(),
    // however they did with operator=.
    data_ = str;
#else
    data_.assign(str);
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::assign(const char* str, size_t len)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (len) {
        data_.assign(str, len);
    }
    else {
        remove(0);
    }
#else
    data_.assign(str, len);
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::assign(char c, size_t count)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (count) {
        data_.assign(count, c);
    }
    else {
        remove(0);
    }
#else
    data_.assign(count, c);
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::operator=(char c)
{
    assign(c);
    return *this;
}

inline RWStandardCString&
RWStandardCString::operator=(const char* s)
{
    assign(s);
    return *this;
}

inline RWStandardCString&
RWStandardCString::operator=(const std::string& s)
{
    assign(s);
    return *this;
}

RW_SUPPRESS_OP_ASSIGN_SELF_CHECK_WARNING
inline RWStandardCString&
RWStandardCString::operator=(const RWStandardCString& s)
{
    assign(s);
    return *this;
}
RW_RESTORE_OP_ASSIGN_SELF_CHECK_WARNING

#if !defined(RW_NO_RVALUE_REFERENCES)

inline RWStandardCString&
RWStandardCString::operator=(RWStandardCString && s)
{
    if (this != &s) {
        s.swap(*this);
    }

    return *this;
}

#endif // !RW_NO_RVALUE_REFERENCES

inline RWStandardCString&
RWStandardCString::operator=(const RWStandardCString::SubString& str)
{
    assign(str);
    return *this;
}

inline RWStandardCString&
RWStandardCString::operator=(const RWStandardCString::ConstSubString& str)
{
    assign(str);
    return *this;
}

inline void
RWStandardCString::assertElement(size_t i) const
{
    size_t len = length();
    if (i >= len) {
        throwBoundsErr(i, len);
    }
}

//Indexing - non-const
// Note: RWStandardCString::operator() is undefined for i>=length()
//       when RWBOUNDS_CHECK is not defined.
inline char&
RWStandardCString::operator()(size_t i)
{
#ifdef RWBOUNDS_CHECK
    assertElement(i);
#endif
    return data_[i];
}

inline char
RWStandardCString::operator()(size_t i) const
{
#ifdef RWBOUNDS_CHECK
    assertElement(i);
#endif
    return data()[i];
}

// Here all integer types are provided with an exact match
// to avoid ambiguities with the built in char[] operator
// All operator[] functions are with bounds checking
inline char&
RWStandardCString::operator[](char i)
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWStandardCString::operator[](char i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char&
RWStandardCString::operator[](signed char i)
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWStandardCString::operator[](signed char i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char&
RWStandardCString::operator[](short i)
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWStandardCString::operator[](short i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char&
RWStandardCString::operator[](int i)
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWStandardCString::operator[](int i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char&
RWStandardCString::operator[](long i)
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWStandardCString::operator[](long i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char&
RWStandardCString::operator[](unsigned char i)
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWStandardCString::operator[](unsigned char i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char&
RWStandardCString::operator[](unsigned short i)
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWStandardCString::operator[](unsigned short i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char&
RWStandardCString::operator[](unsigned int i)
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWStandardCString::operator[](unsigned int i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char&
RWStandardCString::operator[](unsigned long i)
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWStandardCString::operator[](unsigned long i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

#if !defined(RW_NO_LONG_LONG)
inline char&
RWStandardCString::operator[](unsigned long long i)
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWStandardCString::operator[](unsigned long long i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char&
RWStandardCString::operator[](long long i)
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWStandardCString::operator[](long long i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}
#endif

// Substring operators : non-const
inline RWStandardCSubString
RWStandardCString::operator()(size_t start, size_t len)
{
    return RWStandardCSubString(*this, start, len);
}

// Substring operators : const
inline RWStandardCConstSubString
RWStandardCString::operator()(size_t start, size_t len) const
{
    return RWStandardCConstSubString(*this, start, len);
}

// Other Non-static member functions

inline RWStandardCString&
RWStandardCString::append(const RWStandardCString& str, size_t N)
{
    size_t len = rwmin(N, str.length());
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (len) {
        data_.append(str.std(), 0, len);
    }
#else
    data_.append(str.std(), 0, len);
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::append(const char* s, size_t N)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (N) {
        data_.append(s, N);
    }
#else
    data_.append(s, N);
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::append(const std::string& str, size_t N)
{
    size_t len = rwmin(N, str.length());
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (len) {
        data_.append(str, 0, len);
    }
#else
    data_.append(str, 0, len);
#endif
    return *this;
}

inline size_t
RWStandardCString::capacity() const
{
    return data_.capacity();
}

inline int
RWStandardCString::compareTo(const char* s, size_t len, caseCompare cmp) const
{
    if (cmp == exact) {
        return rwCStrCompareExact(data(), length(), s, len);
    }
    else if (cmp == ignoreCase) {
        return rwCStrCompareIgnoreCase(data(), length(), s, len);
    }
    else {
        return rwCStrCompareIgnoreCaseStrict(data(), length(), s, len);
    }
}

inline int
RWStandardCString::compareTo(const char* s, caseCompare cmp) const
{
    RW_PRECONDITION2(s != rwnil,
                     "RWStandardCString::compareTo(const char* s, caseCompare cmp) const: "
                     "null pointer");
    return compareTo(s, strlen(s), cmp);
}

#if !defined(RW_DISABLE_DEPRECATED)

inline size_t
RWStandardCString::indexIgnoreCase(const char* pat, size_t patlen, size_t i) const
{
    return rwCStrFindIgnoreCase(data(), length(), pat, patlen, i);
}

#endif // RW_DISABLE_DEPRECATED

inline size_t
RWStandardCString::index(const char* pat, size_t patlen, size_t i, caseCompare cmp) const
{
    if (cmp == exact) {
        return rwCStrFindExact(data(), length(), pat, patlen, i);
    }
    else {
        return rwCStrFindIgnoreCase(data(), length(), pat, patlen, i);
    }
}

inline size_t
RWStandardCString::index(const std::string& pat, size_t i, caseCompare cmp) const
{
    return index(pat.data(), pat.length(), i, cmp);
}

inline size_t
RWStandardCString::index(const RWStandardCString& pat, size_t patlen,
                         size_t i, caseCompare cmp) const
{
    RW_ASSERT(patlen <= pat.length());
    return index(pat.data(), patlen, i, cmp);
}

inline bool
RWStandardCString::contains(const char* pat, size_t len, caseCompare cmp) const
{
    return index(pat, len, size_t(0), cmp) != RW_NPOS;
}

inline bool
RWStandardCString::contains(const char* pat, caseCompare cmp) const
{
    RW_PRECONDITION2(pat != rwnil,
                     "RWStandardCString::contains(const char* pat, caseCompare cmp) const: "
                     "null pointer");
    return index(pat, strlen(pat), size_t(0), cmp) != RW_NPOS;
}

inline bool
RWStandardCString::contains(const std::string& pat, caseCompare cmp) const
{
    return index(pat.data(), pat.length(), size_t(0), cmp) != RW_NPOS;
}

inline bool
RWStandardCString::contains(const RWStandardCString& pat, caseCompare cmp) const
{
    return index(pat.data(), pat.length(), size_t(0), cmp) != RW_NPOS;
}

inline bool
RWStandardCString::contains(const RWStandardCString::SubString& pat, caseCompare cmp) const
{
    return index(pat.startData(), pat.length(), size_t(0), cmp) != RW_NPOS;
}

inline bool
RWStandardCString::contains(const RWStandardCString::ConstSubString& pat, caseCompare cmp) const
{
    return index(pat.startData(), pat.length(), size_t(0), cmp) != RW_NPOS;
}

#if !defined(RW_DISABLE_DEPRECATED)

inline RWStandardCString
RWStandardCString::copy() const
{
    return RWStandardCString(data_);
}

#endif // RW_DISABLE_DEPRECATED

inline int
RWStandardCString::collate(const char* s) const
{
    RW_PRECONDITION2(s != rwnil,
                     "RWStandardCString::collate(const char* s) const: null pointer");
    return strcoll(data(), s);
}

inline int
RWStandardCString::collate(const std::string& str) const
{
    return strcoll(data(), str.c_str());
}

inline int
RWStandardCString::collate(const RWStandardCString& str) const
{
    return strcoll(data(), str.data());
}

inline size_t
RWStandardCString::firstNotOf(char c, size_t pos) const
{
    return rwCStrFindFirstNotOf(data(), length(), c, pos);
}

inline size_t
RWStandardCString::firstNotOf(const char* str, size_t pos, size_t len) const
{
    return rwCStrFindFirstNotOf(data(), length(), str, len, pos);
}

inline size_t
RWStandardCString::firstNotOf(const char* str, size_t pos) const
{
    RW_PRECONDITION2(str != rwnil,
                     "RWStandardCString::firstNotOf(const char* str, size_t pos) const: null pointer");
    return firstNotOf(str, pos, strlen(str));
}

inline size_t
RWStandardCString::firstNotOf(const RWStandardCString& str, size_t pos) const
{
    return firstNotOf(str.data(), pos, str.length());
}

inline size_t
RWStandardCString::firstNotOf(const RWStandardCString::SubString& str, size_t pos) const
{
    return firstNotOf(str.startData(), pos, str.length());
}

inline size_t
RWStandardCString::firstNotOf(const RWStandardCString::ConstSubString& str, size_t pos) const
{
    return firstNotOf(str.startData(), pos, str.length());
}

inline size_t
RWStandardCString::firstNotOf(const std::string& str, size_t pos) const
{
    return firstNotOf(str.data(), pos, str.length());
}

inline size_t
RWStandardCString::firstOf(char c, size_t pos) const
{
    return rwCStrFindFirstOf(data(), length(), c, pos);
}

inline size_t
RWStandardCString::firstOf(const char* str, size_t pos, size_t len) const
{
    return rwCStrFindFirstOf(data(), length(), str, len, pos);
}

inline size_t
RWStandardCString::firstOf(const char* str, size_t pos) const
{
    RW_PRECONDITION2(str != rwnil,
                     "RWStandardCString::firstOf(const char* str, size_t pos) const: null pointer");
    return firstOf(str, pos, strlen(str));
}

inline size_t
RWStandardCString::firstOf(const RWStandardCString& str, size_t pos) const
{
    return firstOf(str.data(), pos, str.length());
}

inline size_t
RWStandardCString::firstOf(const RWStandardCString::SubString& str, size_t pos) const
{
    return firstOf(str.startData(), pos, str.length());
}

inline size_t
RWStandardCString::firstOf(const RWStandardCString::ConstSubString& str, size_t pos) const
{
    return firstOf(str.startData(), pos, str.length());
}

inline size_t
RWStandardCString::firstOf(const std::string& str, size_t pos) const
{
    return firstOf(str.data(), pos, str.length());
}

#if !defined(RW_DISABLE_DEPRECATED)

inline size_t
RWStandardCString::first(char c) const
{
    return firstOf(c);
}

inline size_t
RWStandardCString::first(char c, size_t) const
{
    return firstOf(c);
}

inline size_t
RWStandardCString::first(const char* s, size_t N) const
{
    return firstOf(s, 0, N);
}

inline size_t
RWStandardCString::first(const char* s) const
{
    return firstOf(s);
}

#endif // RW_DISABLE_DEPRECATED

inline size_t
RWStandardCString::index(const char* pat, size_t i, caseCompare cmp) const
{
    RW_PRECONDITION2(pat != rwnil,
                     "RWStandardCString::index(const char* pat, size_t i, caseCompare cmp) "
                     "const: null pointer");
    return index(pat, strlen(pat), i, cmp);
}

inline size_t
RWStandardCString::index(const RWStandardCString& pat, size_t i, caseCompare cmp) const
{
    return index(pat.data(), pat.length(), i, cmp);
}

inline size_t
RWStandardCString::index(const RWStandardCString::SubString& pat, size_t i, caseCompare cmp) const
{
    return index(pat.startData(), pat.length(), i, cmp);
}

inline size_t
RWStandardCString::index(const RWStandardCString::ConstSubString& pat, size_t i, caseCompare cmp) const
{
    return index(pat.startData(), pat.length(), i, cmp);
}

inline size_t
RWStandardCString::index(const char pat, size_t i, caseCompare cmp) const
{
    return index(&pat, 1, i, cmp);
}

inline size_t
RWStandardCString::index(const std::string& pat,
                         size_t patlen, size_t i, caseCompare cmp) const
{
    RW_ASSERT(patlen <= pat.length());
    return index(pat.data(), patlen, i, cmp);
}

inline RWStandardCString&
RWStandardCString::insert(size_t pos, char c, size_t extent)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (extent) {
        data_.insert(pos, extent, c);
    }
#else
    data_.insert(pos, extent, c);
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::insert(size_t pos, const char* s)
{
    RW_PRECONDITION2(s != rwnil,
                     "RWStandardCString::insert(size_t pos, const char* s): null pointer");
    data_.insert(pos, s);
    return *this;
}

inline RWStandardCString&
RWStandardCString::insert(size_t pos, const std::string& str)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (!str.empty()) {
        data_.insert(pos, str);
    }
#else
    data_.insert(pos, str);
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::insert(size_t pos, const RWStandardCString& str)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (!str.isNull()) {
        data_.insert(pos, str.std());
    }
#else
    data_.insert(pos, str.std());
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::insert(size_t pos, const RWStandardCString::SubString& str)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (!str.isNull()) {
        data_.insert(pos, str.startData(), str.length());
    }
#else
    data_.insert(pos, str.startData(), str.length());
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::insert(size_t pos, const RWStandardCString::ConstSubString& str)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (!str.isNull()) {
        data_.insert(pos, str.startData(), str.length());
    }
#else
    data_.insert(pos, str.startData(), str.length());
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::insert(size_t pos, const char* s, size_t extent)
{
    RW_PRECONDITION2(s != rwnil,
                     "RWStandardCString::insert(size_t pos, const char* s, size_t extent): "
                     "null pointer");
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (extent) {
        data_.insert(pos, s, extent);
    }
#else
    data_.insert(pos, s, extent);
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::insert(size_t pos, const std::string& str, size_t extent)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (extent) {
        data_.insert(pos, str, 0, extent);
    }
#else
    data_.insert(pos, str, 0, extent);
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::insert(size_t pos, const RWStandardCString& str, size_t extent)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (extent) {
        data_.insert(pos, str.std(), 0, extent);
    }
#else
    data_.insert(pos, str.std(), 0, extent);
#endif
    return *this;
}

inline size_t
RWStandardCString::lastNotOf(char c, size_t pos) const
{
    return rwCStrFindLastNotOf(data(), length(), c, pos);
}

inline size_t
RWStandardCString::lastNotOf(const char* str, size_t pos, size_t len) const
{
    return rwCStrFindLastNotOf(data(), length(), str, len, pos);
}

inline size_t
RWStandardCString::lastNotOf(const char* str, size_t pos) const
{
    RW_PRECONDITION2(str != rwnil,
                     "RWStandardCString::lastNotOf(const char* str, size_t pos) const: null pointer");
    return lastNotOf(str, pos, strlen(str));
}

inline size_t
RWStandardCString::lastNotOf(const RWStandardCString& str, size_t pos) const
{
    return lastNotOf(str.data(), pos, str.length());
}

inline size_t
RWStandardCString::lastNotOf(const RWStandardCString::SubString& str, size_t pos) const
{
    return lastNotOf(str.startData(), pos, str.length());
}

inline size_t
RWStandardCString::lastNotOf(const RWStandardCString::ConstSubString& str, size_t pos) const
{
    return lastNotOf(str.startData(), pos, str.length());
}

inline size_t
RWStandardCString::lastNotOf(const std::string& str, size_t pos) const
{
    return lastNotOf(str.data(), pos, str.length());
}

inline size_t
RWStandardCString::lastOf(char c, size_t pos) const
{
    return rwCStrFindLastOf(data(), length(), c, pos);
}

inline size_t
RWStandardCString::lastOf(const char* str, size_t pos, size_t len) const
{
    return rwCStrFindLastOf(data(), length(), str, len, pos);
}

inline size_t
RWStandardCString::lastOf(const char* str, size_t pos) const
{
    RW_PRECONDITION2(str != rwnil,
                     "RWStandardCString::lastOf(const char* str, size_t pos) const: null pointer");
    return lastOf(str, pos, strlen(str));
}

inline size_t
RWStandardCString::lastOf(const RWStandardCString& str, size_t pos) const
{
    return lastOf(str.data(), pos, str.length());
}

inline size_t
RWStandardCString::lastOf(const RWStandardCString::SubString& str, size_t pos) const
{
    return lastOf(str.startData(), pos, str.length());
}

inline size_t
RWStandardCString::lastOf(const RWStandardCString::ConstSubString& str, size_t pos) const
{
    return lastOf(str.startData(), pos, str.length());
}

inline size_t
RWStandardCString::lastOf(const std::string& str, size_t pos) const
{
    return lastOf(str.data(), pos, str.length());
}

#if !defined(RW_DISABLE_DEPRECATED)

inline size_t
RWStandardCString::last(char c) const
{
    return lastOf(c);
}

inline size_t
RWStandardCString::last(char c, size_t) const
{
    return lastOf(c);
}

inline size_t
RWStandardCString::last(const char* s, size_t N) const
{
    return lastOf(s, RW_NPOS, N);
}

inline size_t
RWStandardCString::last(const char* s) const
{
    return lastOf(s);
}

#endif // RW_DISABLE_DEPRECATED

inline RWStandardCString&
RWStandardCString::prepend(char c, size_t N)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (N) {
        data_.insert(size_t(0), N, c);
    }
#else
    data_.insert(size_t(0), N, c);
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::prepend(const char* s)
{
    RW_PRECONDITION2(s != rwnil,
                     "RWStandardCString::prepend(const char* s): null pointer");
    data_.insert(0, s);
    return *this;
}

inline RWStandardCString&
RWStandardCString::prepend(const std::string& str)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (!str.empty()) {
        data_.insert(0, str);
    }
#else
    data_.insert(0, str);
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::prepend(const RWStandardCString& str)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (!str.isNull()) {
        data_.insert(0, str.std());
    }
#else
    data_.insert(0, str.std());
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::prepend(const RWStandardCString::SubString& str)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (!str.isNull()) {
        data_.insert(0, str.startData(), str.length());
    }
#else
    data_.insert(0, str.startData(), str.length());
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::prepend(const RWStandardCString::ConstSubString& str)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (!str.isNull()) {
        data_.insert(0, str.startData(), str.length());
    }
#else
    data_.insert(0, str.startData(), str.length());
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::prepend(const char* s, size_t N)
{
    RW_PRECONDITION2(s != rwnil,
                     "RWStandardCString::prepend(const char* s, size_t N): null pointer");
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (N) {
        data_.insert(0, s, N);
    }
#else
    data_.insert(0, s, N);
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::prepend(const std::string& str, size_t N)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (N) {
        data_.insert(0, str, 0, N);
    }
#else
    data_.insert(0, str, 0, N);
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::prepend(const RWStandardCString& str, size_t N)
{
#if defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (N) {
        data_.insert(0, str.std(), 0, N);
    }
#else
    data_.insert(0, str.std(), 0, N);
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::remove(size_t pos)
{
#ifdef RW_MSVC_STDLIB_BUG // see compiler.h
    if (data_.size()) {
        data_[0];
    }
#endif
    data_.erase(pos);
    return *this;
}

inline RWStandardCString&
RWStandardCString::remove(size_t pos, size_t N)
{
#ifdef RW_MSVC_STDLIB_BUG // see compiler.h
    if (data_.size()) {
        data_[0];
    }
#endif
    data_.erase(pos, N);
    return *this;
}

inline RWStandardCString&
RWStandardCString::replace(size_t pos, size_t extent, const char* s, size_t N)
{
    RW_PRECONDITION2(s != rwnil,
                     "RWStandardCString::replace(size_t pos, size_t extent, const char* s, "
                     "size_t N): null pointer");
#if (_STLPORT_VERSION == 0x452)
    // STLport 4.5.2 does not properly handle replacement from
    // overlapping data regions. we make a copy of the source
    // string if the source data overlaps part of the destination.

    const char* const beg_dst = data() + pos;
    const char* const end_dst = data() + length();
    if (!extent || (s + N < beg_dst) || (end_dst < s)) {
        data_.replace(pos, extent, s, N);
    }
    else {
        const std::string str(s, N);
        data_.replace(pos, extent, str, 0, N);
    }
#elif defined(_RWSTD_VER) && (_RWSTD_VER <= 0x02020400)
    // Older versions of the RW stdlib did not verify that the
    // original string would be modified before allocating a
    // new string instance for it. Avoid an unnecessary
    // allocation if the string won't be modified.
    if (extent || N) {
        data_.replace(pos, extent, s, N);
    }
#else
    data_.replace(pos, extent, s, N);
#endif
    return *this;
}

inline RWStandardCString&
RWStandardCString::replace(size_t pos, size_t extent, const char* s)
{
    RW_PRECONDITION2(s != rwnil,
                     "RWStandardCString::replace(size_t pos, size_t extent, const char* s): "
                     "null pointer");
    replace(pos, extent, s, strlen(s));
    return *this;
}

inline RWStandardCString&
RWStandardCString::replace(size_t pos, size_t extent, const std::string& str)
{
    replace(pos, extent, str.data(), str.length());
    return *this;
}

inline RWStandardCString&
RWStandardCString::replace(size_t pos, size_t extent, const RWStandardCString& str)
{
    replace(pos, extent, str.data(), str.length());
    return *this;
}

inline RWStandardCString&
RWStandardCString::replace(size_t pos, size_t extent, const RWStandardCString::SubString& str)
{
    replace(pos, extent, str.startData(), str.length());
    return *this;
}

inline RWStandardCString&
RWStandardCString::replace(size_t pos, size_t extent, const RWStandardCString::ConstSubString& str)
{
    replace(pos, extent, str.startData(), str.length());
    return *this;
}

inline RWStandardCString&
RWStandardCString::replace(size_t pos, size_t extent, const RWStandardCString& str, size_t N)
{
    replace(pos, extent, str.data(), N);
    return *this;
}

inline RWStandardCString&
RWStandardCString::replace(size_t pos, size_t extent,
                           const std::string& str, size_t N)
{
    replace(pos, extent, str.data(), N);
    return *this;
}

inline void
RWStandardCString::resize(size_t N)
{
    data_.resize(N, ' ');
}

#if !defined(RW_DISABLE_DEPRECATED)

inline size_t
RWStandardCString::rindexIgnoreCase(const char* pat, size_t patlen, size_t i) const
{
    return rwCStrRFindIgnoreCase(data(), length(), pat, patlen, i);
}

#endif // RW_DISABLE_DEPRECATED

inline size_t
RWStandardCString::rindex(const char* pat, size_t patlen,
                          size_t i, caseCompare cmp) const
{
    if (cmp == exact) {
        return rwCStrRFindExact(data(), length(), pat, patlen, i);
    }
    else {
        return rwCStrRFindIgnoreCase(data(), length(), pat, patlen, i);
    }
}

inline size_t
RWStandardCString::rindex(const char* pat, caseCompare cmp) const
{
    RW_PRECONDITION2(pat != rwnil,
                     "RWStandardCString::rindex(const char* pat, caseCompare cmp) const: "
                     "null pointer");
    return rindex(pat, strlen(pat), RW_NPOS, cmp);
}

inline size_t
RWStandardCString::rindex(const RWStandardCString& pat, size_t patlen,
                          size_t i, caseCompare cmp) const
{
    RW_ASSERT(patlen <= pat.length());
    return rindex(pat.data(), patlen, i, cmp);
}

inline size_t
RWStandardCString::rindex(const char* pat, size_t i, caseCompare cmp) const
{
    RW_PRECONDITION2(pat != rwnil,
                     "RWStandardCString::rindex(const char* pat, size_t i, "
                     "caseCompare cmp) const: null pointer");
    return rindex(pat, strlen(pat), i, cmp);
}

inline size_t
RWStandardCString::rindex(const char pat, size_t i, caseCompare cmp) const
{
    return rindex(&pat, 1, i, cmp);
}

inline size_t
RWStandardCString::rindex(const std::string& pat, size_t i, caseCompare cmp) const
{
    return rindex(pat.data(), pat.length(), i, cmp);
}

inline size_t
RWStandardCString::rindex(const std::string& pat, size_t patlen,
                          size_t i, caseCompare cmp) const
{
    RW_ASSERT(patlen <= pat.length());
    return rindex(pat.data(), patlen, i, cmp);
}

inline size_t
RWStandardCString::rindex(const RWStandardCString& pat, size_t i, caseCompare cmp) const
{
    return rindex(pat.data(), pat.length(), i, cmp);
}

inline size_t
RWStandardCString::rindex(const RWStandardCString::SubString& pat, size_t i, caseCompare cmp) const
{
    return rindex(pat.startData(), pat.length(), i, cmp);
}

inline size_t
RWStandardCString::rindex(const RWStandardCString::ConstSubString& pat, size_t i, caseCompare cmp) const
{
    return rindex(pat.startData(), pat.length(), i, cmp);
}

inline void
RWStandardCString::swap(RWStandardCString& str)
{
    data_.swap(str.data_);
}

inline void
RWStandardCString::shrink()
{
#ifndef RW_NO_BASIC_STRING_SHRINK_TO_FIT
    data_.shrink_to_fit();
#else
    data_.reserve(0);
#endif
}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                       Related global functions                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#if !defined(RW_DISABLE_DEPRECATED)
/**
 * @internal
 * @deprecated As of SourcePro 12, use RWStandardCString::hash(const RWStandardCString&) instead.
 */
RW_DEPRECATE_FUNC("Use RWStandardCString::hash(const RWStandardCString&) instead")
inline unsigned
rwhash(const RWStandardCString& s)
{
    return unsigned(s.hash());
}

/**
 * @internal
 * @deprecated As of SourcePro 12, use RWStandardCString::hash(const RWStandardCString&) instead.
 */
RW_DEPRECATE_FUNC("Use RWStandardCString::hash(const RWStandardCString&) instead")
inline unsigned
rwhash(const RWStandardCString* s)
{
    RW_PRECONDITION2(s != rwnil,
                     "rwhash(const RWStandardCString* s): null pointer");
    return unsigned(s->hash());
}
#endif

inline RWStandardCString
toLower(const RWStandardCString& str)
{
    RWStandardCString tmp(str);
    tmp.toLower();
    return tmp;
}

inline RWStandardCString
toUpper(const RWStandardCString& str)
{
    RWStandardCString temp(str);
    temp.toUpper();
    return temp;
}

inline RWStandardCString
operator+(const RWStandardCString& lhs, const RWStandardCString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardCString
operator+(const RWStandardCString& lhs, const RWStandardCSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardCString
operator+(const RWStandardCSubString& lhs, const RWStandardCString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardCString
operator+(const RWStandardCString& lhs, const RWStandardCConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardCString
operator+(const RWStandardCConstSubString& lhs, const RWStandardCString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardCString
operator+(const RWStandardCString& lhs,  const std::string& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWStandardCString(rhs);
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardCString
operator+(const std::string& lhs, const RWStandardCString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWStandardCString(lhs);
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardCString
operator+(const RWStandardCString& lhs,  const char* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator+(const RWStandardCString& lhs,  const char* rhs): null pointer");
    size_t rhsLen = strlen(rhs);
    if (!rhsLen) {
        return lhs;
    }
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWStandardCString(rhs, rhsLen);
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs, rhsLen);
}

inline RWStandardCString
operator+(const char* lhs, const RWStandardCString& rhs)
{
    RW_PRECONDITION2(lhs != rwnil,
                     "operator+(const char* lhs, const RWStandardCString& rhs): null pointer");
    size_t lhsLen = strlen(lhs);
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWStandardCString(lhs, lhsLen);
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs, lhsLen);
    return ret.append(rhs);
}

inline RWStandardCString
operator+(const RWStandardCSubString& lhs, const RWStandardCSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardCString
operator+(const RWStandardCSubString& lhs, const RWStandardCConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardCString
operator+(const RWStandardCConstSubString& lhs, const RWStandardCSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardCString
operator+(const RWStandardCSubString& lhs,  const std::string& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWStandardCString(rhs);
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardCString
operator+(const std::string& lhs, const RWStandardCSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWStandardCString(lhs);
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardCString
operator+(const RWStandardCSubString& lhs,  const char* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator+(const RWStandardCString& lhs,  const char* rhs): null pointer");
    size_t rhsLen = strlen(rhs);
    if (!rhsLen) {
        return lhs;
    }
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWStandardCString(rhs, rhsLen);
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs, rhsLen);
}

inline RWStandardCString
operator+(const char* lhs, const RWStandardCSubString& rhs)
{
    RW_PRECONDITION2(lhs != rwnil,
                     "operator+(const char* lhs, const RWStandardCString& rhs): null pointer");
    size_t lhsLen = strlen(lhs);
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWStandardCString(lhs, lhsLen);
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs, lhsLen);
    return ret.append(rhs);
}

inline RWStandardCString
operator+(const RWStandardCConstSubString& lhs, const RWStandardCConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardCString
operator+(const RWStandardCConstSubString& lhs,  const std::string& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWStandardCString(rhs);
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardCString
operator+(const std::string& lhs, const RWStandardCConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWStandardCString(lhs);
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardCString
operator+(const RWStandardCConstSubString& lhs,  const char* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator+(const RWStandardCString& lhs,  const char* rhs): null pointer");
    size_t rhsLen = strlen(rhs);
    if (!rhsLen) {
        return lhs;
    }
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWStandardCString(rhs, rhsLen);
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs, rhsLen);
}

inline RWStandardCString
operator+(const char* lhs, const RWStandardCConstSubString& rhs)
{
    RW_PRECONDITION2(lhs != rwnil,
                     "operator+(const char* lhs, const RWStandardCString& rhs): null pointer");
    size_t lhsLen = strlen(lhs);
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWStandardCString(lhs, lhsLen);
    }
    RWStandardCString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs, lhsLen);
    return ret.append(rhs);
}


//////////////////////////////////////////////////////////////////////////
//                         RWStandardCSubString                                 //
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// RWCSubString::startData()
//
// This member replaces data().
// startData() will remain undocumented.
// Use at your own risk. It may be deprecated in the future.
//
// Since RWCSubString works by referencing the RWStandardCString's data,
// if you attempt to directly use the data() member of the RWStandardCString,
// you will very likely be surprised by the result, which is null
// terminated not at the extent of the substring,
// but at the end of the RWStandardCString.
//
///////////////////////////////////////////////////////////////////////////////

////
// The RWStandardCSubString ctor appears out of sequence at the beginning of the
// RWStandardCString inlines section, in order to avoid inlined after called warnings.
//

inline const char*
RWStandardCSubString::startData() const
{
    return str_->data() + begin_;
}

#if !defined(RW_DISABLE_DEPRECATED)

inline const char*
RWStandardCSubString::data() const
{
    return startData();
}

#endif

inline RWStandardCSubString&
RWStandardCSubString::operator=(const char* s)
{
    if (!isNull()) {
        str_->replace(begin_, extent_, s);
    }
    return *this;
}

inline RWStandardCSubString&
RWStandardCSubString::operator=(const RWStandardCSubString& str)
{
    if (this == &str) {
        return *this;
    }

    if (!isNull()) {
        str_->replace(begin_, extent_, str.startData(), str.length());
    }
    return *this;
}

inline RWStandardCSubString&
RWStandardCSubString::operator=(const RWStandardCConstSubString& str)
{
    if (!isNull()) {
        str_->replace(begin_, extent_, str.startData(), str.length());
    }
    return *this;
}

inline RWStandardCSubString&
RWStandardCSubString::operator=(const std::string& str)
{
    if (!isNull()) {
        str_->replace(begin_, extent_, str);
    }
    return *this;
}

inline RWStandardCSubString&
RWStandardCSubString::operator=(const RWStandardCString& str)
{
    if (!isNull()) {
        str_->replace(begin_, extent_, str.std());
    }
    return *this;
}

inline void
RWStandardCSubString::assertElement(size_t i) const
{
    size_t len = length();
    if (i >= len) {
        throwBoundsErr(i, len);
    }
}

inline char&
RWStandardCSubString::operator()(size_t i)
{
    assertElement(i);
    return (*str_)(begin_ + i);
}

inline char
RWStandardCSubString::operator()(size_t i) const
{
    assertElement(i);
    return (*str_)(begin_ + i);
}

// Here all integer types are provided with an exact match
// to avoid ambiguities with the built in char[] operator
inline char&
RWStandardCSubString::operator[](char i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWStandardCSubString::operator[](char i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char&
RWStandardCSubString::operator[](signed char i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWStandardCSubString::operator[](signed char i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char&
RWStandardCSubString::operator[](short i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWStandardCSubString::operator[](short i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char&
RWStandardCSubString::operator[](int i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWStandardCSubString::operator[](int i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char&
RWStandardCSubString::operator[](long i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWStandardCSubString::operator[](long i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char&
RWStandardCSubString::operator[](unsigned char i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWStandardCSubString::operator[](unsigned char i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char&
RWStandardCSubString::operator[](unsigned short i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWStandardCSubString::operator[](unsigned short i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char&
RWStandardCSubString::operator[](unsigned int i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWStandardCSubString::operator[](unsigned int i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char&
RWStandardCSubString::operator[](unsigned long i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWStandardCSubString::operator[](unsigned long i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

#if !defined(RW_NO_LONG_LONG)
inline char&
RWStandardCSubString::operator[](unsigned long long i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + (size_t)i);
}

inline char
RWStandardCSubString::operator[](unsigned long long i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + (size_t)i);
}

inline char&
RWStandardCSubString::operator[](long long i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + (size_t)i);
}

inline char
RWStandardCSubString::operator[](long long i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + (size_t)i);
}
#endif

// Convert self to lower-case
inline void
RWStandardCSubString::toLower()
{
    if (!isNull()) {
        str_->toLower(begin_, extent_);
    }
}

// Convert self to upper-case
inline void
RWStandardCSubString::toUpper()
{
    if (!isNull()) {
        str_->toUpper(begin_, extent_);
    }
}


//////////////////////////////////////////////////////////////////////////
//                         RWStandardCConstSubString                            //
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// RWStandardCConstSubString::startData()
//
// This member replaces data().
// startData() will remain undocumented. Please don't even ask.
// Use at your own risk. It may be deprecated in the future.
//
// Since RWStandardCSubString works by referencing the RWStandardCString's data,
// if you attempt to directly use the data() member of the RWStandardCString,
// you will very likely be surprised by the result, which is null
// terminated not at the extent of the substring,
// but at the end of the RWStandardCString.
//
///////////////////////////////////////////////////////////////////////////////

inline void
RWStandardCConstSubString::assertElement(size_t i) const
{
    size_t len = length();
    if (i >= len) {
        throwBoundsErr(i, len);
    }
}

inline char
RWStandardCConstSubString::operator()(size_t i) const
{
    assertElement(i);
    return (*str_)(begin_ + i);
}

// Here all integer types are provided with an exact match
// to avoid ambiguities with the built in char[] operator
inline char
RWStandardCConstSubString::operator[](char i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWStandardCConstSubString::operator[](signed char i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWStandardCConstSubString::operator[](short i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWStandardCConstSubString::operator[](int i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWStandardCConstSubString::operator[](long i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWStandardCConstSubString::operator[](unsigned char i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWStandardCConstSubString::operator[](unsigned short i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWStandardCConstSubString::operator[](unsigned int i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWStandardCConstSubString::operator[](unsigned long i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

#if !defined(RW_NO_LONG_LONG)
inline char
RWStandardCConstSubString::operator[](long long i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + (size_t)i);
}

inline char
RWStandardCConstSubString::operator[](unsigned long long i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + (size_t)i);
}
#endif

/*
 * Returns a substring matching "pattern", or the null substring
 * if there is no such match. It would be nice if this could be yet another
 * overloaded version of operator(), but this would result in a type
 * conversion ambiguity with operator(size_t, size_t).
 */
inline RWStandardCString::SubString
RWStandardCString::subString(const char* pattern, size_t startIndex, caseCompare cmp)
{
    RW_PRECONDITION2(pattern != rwnil,
                     "RWStandardCString::subString(const char* pattern, size_t startIndex, "
                     "caseCompare cmp): null pointer");
    size_t len = strlen(pattern);
    size_t i = index(pattern, len, startIndex, cmp);
    return RWStandardCString::SubString(*this, i, i == RW_NPOS ? 0 : len);
}

inline RWStandardCConstSubString
RWStandardCString::subString(const char* pattern, size_t startIndex,
                             caseCompare cmp) const
{
    RW_PRECONDITION2(pattern != rwnil,
                     "RWStandardCString::subString(const char* pattern, size_t startIndex, "
                     "caseCompare cmp) const: null pointer");
    size_t len = strlen(pattern);
    size_t i = index(pattern, len, startIndex, cmp);
    return RWStandardCConstSubString(*this, i, i == RW_NPOS ? 0 : len);
}

inline std::string&
RWStandardCString::std()
{
    return RW_EXPOSE(data_);
}

inline void
RWStandardCString::toLower(size_t pos, size_t extent)
{
    size_t n = (pos + extent > length()) ? length() : pos + extent;
    for (; pos < n; ++pos) {
        char& c = data_[pos];
        c = char(tolower(RW_STATIC_CAST(unsigned char, c)));
    }
}

inline void
RWStandardCString::toUpper(size_t pos, size_t extent)
{
    size_t n = (pos + extent > length()) ? length() : pos + extent;
    for (; pos < n; ++pos) {
        char& c = data_[pos];
        c = char(toupper(RW_STATIC_CAST(unsigned char, c)));
    }
}

inline RWStandardCConstSubString::RWStandardCConstSubString
(const RWStandardCString& str, size_t pos, size_t n)
    : str_(&str), begin_(pos), extent_(n)
{
#ifdef RW_DEBUG
    size_t len = str.length();

    // Allow zero lengthed and null substrings:
    if ((pos == RW_NPOS && n != 0) || (pos != RW_NPOS && pos + n > len)) {
        subStringError(len, pos, n);
    }
#endif
}

inline const char*
RWStandardCConstSubString::startData() const
{
    return str_->data() + begin_;
}


inline bool
operator==(const RWStandardCString& lhs, const RWStandardCString& rhs)
{
    size_t lhsLen = lhs.length();
    size_t rhsLen = rhs.length();
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.data(), lhsLen, rhs.data(), rhsLen);
}

inline bool
operator==(const RWStandardCString& lhs, const RWStandardCSubString& rhs)
{
    size_t lhsLen = lhs.length();
    size_t rhsLen = rhs.length();
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.data(), lhsLen, rhs.startData(), rhsLen);
}

inline bool
operator==(const RWStandardCSubString& lhs, const RWStandardCString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWStandardCString& lhs, const RWStandardCConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    size_t rhsLen = rhs.length();
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.data(), lhsLen, rhs.startData(), rhsLen);
}

inline bool
operator==(const RWStandardCConstSubString& lhs, const RWStandardCString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWStandardCString& lhs, const std::string& rhs)
{
    size_t lhsLen = lhs.length();
    size_t rhsLen = rhs.length();
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.data(), lhsLen, rhs.data(), rhsLen);
}

inline bool
operator==(const std::string& lhs, const RWStandardCString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWStandardCString& lhs, const char* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator==(const RWStandardCString& lhs, const char* rhs): null pointer");
    size_t lhsLen = lhs.length();
    size_t rhsLen = strlen(rhs);
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.data(), lhsLen, rhs, rhsLen);
}

inline bool
operator==(const char* lhs, const RWStandardCString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWStandardCString& lhs, char rhs)
{
    return 1 == lhs.length() && lhs(0) == rhs;
}

inline bool
operator==(char lhs, const RWStandardCString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWStandardCSubString& lhs, const RWStandardCSubString& rhs)
{
    size_t lhsLen = lhs.length();
    size_t rhsLen = rhs.length();
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.startData(), lhsLen, rhs.startData(), rhsLen);
}

inline bool
operator==(const RWStandardCSubString& lhs, const RWStandardCConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    size_t rhsLen = rhs.length();
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.startData(), lhsLen, rhs.startData(), rhsLen);
}

inline bool
operator==(const RWStandardCConstSubString& lhs, const RWStandardCSubString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWStandardCSubString& lhs, const std::string& rhs)
{
    size_t lhsLen = lhs.length();
    size_t rhsLen = rhs.length();
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.startData(), lhsLen, rhs.data(), rhsLen);
}

inline bool
operator==(const std::string& lhs, const RWStandardCSubString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWStandardCSubString& lhs, const char* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator==(const RWStandardCSubString& lhs, const char* rhs): null pointer");
    size_t lhsLen = lhs.length();
    size_t rhsLen = strlen(rhs);
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.startData(), lhsLen, rhs, rhsLen);
}

inline bool
operator==(const char* lhs, const RWStandardCSubString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWStandardCSubString& lhs, char rhs)
{
    return (1 == lhs.length() && lhs(0) == rhs);
}

inline bool
operator==(char lhs, const RWStandardCSubString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWStandardCConstSubString& lhs, const RWStandardCConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    size_t rhsLen = rhs.length();
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.startData(), lhsLen, rhs.startData(), rhsLen);
}

inline bool
operator==(const RWStandardCConstSubString& lhs, const std::string& rhs)
{
    size_t lhsLen = lhs.length();
    size_t rhsLen = rhs.length();
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.startData(), lhsLen, rhs.data(), rhsLen);
}

inline bool
operator==(const std::string& lhs, const RWStandardCConstSubString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWStandardCConstSubString& lhs, const char* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator==(const RWStandardCConstSubString& lhs, const char* rhs): "
                     "null pointer");
    size_t lhsLen = lhs.length();
    size_t rhsLen = strlen(rhs);
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.startData(), lhsLen, rhs, rhsLen);
}

inline bool
operator==(const char* lhs, const RWStandardCConstSubString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWStandardCConstSubString& lhs, char rhs)
{
    return (1 == lhs.length() && lhs(0) == rhs);
}

inline bool
operator==(char lhs, const RWStandardCConstSubString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator!=(const RWStandardCString& lhs, const RWStandardCString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWStandardCString& lhs, const RWStandardCSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWStandardCSubString& lhs, const RWStandardCString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWStandardCString& lhs, const RWStandardCConstSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWStandardCConstSubString& lhs, const RWStandardCString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWStandardCString& lhs, const std::string& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const std::string& lhs, const RWStandardCString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWStandardCString& lhs,    const char* rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const char* lhs, const RWStandardCString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWStandardCString& lhs, char rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(char lhs, const RWStandardCString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWStandardCSubString& lhs, const RWStandardCSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWStandardCSubString& lhs, const RWStandardCConstSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWStandardCConstSubString& lhs, const RWStandardCSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWStandardCSubString& lhs, const std::string& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const std::string& lhs, const RWStandardCSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWStandardCSubString& lhs, const char* rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const char* lhs, const RWStandardCSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWStandardCSubString& lhs, char rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(char lhs, const RWStandardCSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWStandardCConstSubString& lhs, const RWStandardCConstSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWStandardCConstSubString& lhs, const std::string& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const std::string& lhs, const RWStandardCConstSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWStandardCConstSubString& lhs, const char* rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const char* lhs, const RWStandardCConstSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWStandardCConstSubString& lhs, char rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(char lhs, const RWStandardCConstSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator< (const RWStandardCString& lhs, const RWStandardCString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.data(), lhs.length(), rhs.data(), rhs.length());
}

inline bool
operator< (const RWStandardCString& lhs, const RWStandardCSubString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.data(), lhs.length(), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWStandardCSubString& lhs, const RWStandardCString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs.data(), rhs.length());
}

inline bool
operator< (const RWStandardCString& lhs, const RWStandardCConstSubString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.data(), lhs.length(), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWStandardCConstSubString& lhs, const RWStandardCString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs.data(), rhs.length());
}

inline bool
operator< (const RWStandardCString& lhs, const std::string& rhs)
{
    return 0 > rwCStrCompareExact(lhs.data(), lhs.length(), rhs.data(), rhs.length());
}

inline bool
operator< (const std::string& lhs, const RWStandardCString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.data(), lhs.length(), rhs.data(), rhs.length());
}

inline bool
operator< (const RWStandardCString& lhs, const char* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator< (const RWStandardCString& lhs, const char* rhs): null pointer");
    return 0 > rwCStrCompareExact(lhs.data(), lhs.length(), rhs, strlen(rhs));
}

inline bool
operator< (const char* lhs, const RWStandardCString& rhs)
{
    RW_PRECONDITION2(lhs != rwnil,
                     "operator< (const char* lhs, const RWStandardCString& rhs): null pointer");
    return 0 > rwCStrCompareExact(lhs, strlen(lhs), rhs.data(), rhs.length());
}

inline bool
operator< (const RWStandardCString& lhs, char rhs)
{
    return 0 == lhs.length() || lhs(0) < rhs;
}

inline bool
operator< (char lhs, const RWStandardCString& rhs)
{
    const size_t len = rhs.length();
    return len > 0 && (lhs < rhs(0) || (lhs == rhs(0) && len > 1));
}

inline bool
operator< (const RWStandardCSubString& lhs, const RWStandardCSubString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWStandardCSubString& lhs, const RWStandardCConstSubString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWStandardCConstSubString& lhs, const RWStandardCSubString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWStandardCSubString& lhs, const std::string& rhs)
{
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs.data(), rhs.length());
}

inline bool
operator< (const std::string& lhs, const RWStandardCSubString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.data(), lhs.length(), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWStandardCSubString& lhs, const char* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator< (const RWStandardCSubString& lhs, const char* rhs): null pointer");
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs, strlen(rhs));
}

inline bool
operator< (const char* lhs, const RWStandardCSubString& rhs)
{
    RW_PRECONDITION2(lhs != rwnil,
                     "operator< (const char* lhs, const RWStandardCSubString& rhs): null pointer");
    return 0 > rwCStrCompareExact(lhs, strlen(lhs), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWStandardCSubString& lhs, char rhs)
{
    return 0 == lhs.length() || lhs(0) < rhs;
}

inline bool
operator< (char lhs, const RWStandardCSubString& rhs)
{
    const size_t len = rhs.length();
    return len > 0 && (lhs < rhs(0) || (lhs == rhs(0) && len > 1));
}

inline bool
operator< (const RWStandardCConstSubString& lhs, const RWStandardCConstSubString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWStandardCConstSubString& lhs, const std::string& rhs)
{
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs.data(), rhs.length());
}

inline bool
operator< (const std::string& lhs, const RWStandardCConstSubString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.data(), lhs.length(), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWStandardCConstSubString& lhs, const char* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator< (const RWStandardCConstSubString& lhs, const char* rhs): null pointer");
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs, strlen(rhs));
}

inline bool
operator< (const char* lhs, const RWStandardCConstSubString& rhs)
{
    RW_PRECONDITION2(lhs != rwnil,
                     "operator< (const char* lhs, const RWStandardCConstSubString& rhs): null pointer");
    return 0 > rwCStrCompareExact(lhs, strlen(lhs), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWStandardCConstSubString& lhs, char rhs)
{
    return 0 == lhs.length() || lhs(0) < rhs;
}

inline bool
operator< (char lhs, const RWStandardCConstSubString& rhs)
{
    const size_t len = rhs.length();
    return len > 0 && (lhs < rhs(0) || (lhs == rhs(0) && len > 1));
}

inline bool
operator> (const RWStandardCString& lhs, const RWStandardCString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWStandardCString& lhs, const RWStandardCSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWStandardCSubString& lhs, const RWStandardCString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWStandardCString& lhs, const RWStandardCConstSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWStandardCConstSubString& lhs, const RWStandardCString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWStandardCString& lhs, const std::string& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const std::string& lhs, const RWStandardCString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWStandardCString& lhs, const char* rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const char* lhs, const RWStandardCString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWStandardCString& lhs, char rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (char lhs, const RWStandardCString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWStandardCSubString& lhs, const RWStandardCSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWStandardCSubString& lhs, const RWStandardCConstSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWStandardCConstSubString& lhs, const RWStandardCSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWStandardCSubString& lhs, const std::string& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const std::string& lhs, const RWStandardCSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWStandardCSubString& lhs, const char* rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const char* lhs, const RWStandardCSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWStandardCSubString& lhs, char rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (char lhs, const RWStandardCSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWStandardCConstSubString& lhs, const RWStandardCConstSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWStandardCConstSubString& lhs, const std::string& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const std::string& lhs, const RWStandardCConstSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWStandardCConstSubString& lhs, const char* rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const char* lhs, const RWStandardCConstSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWStandardCConstSubString& lhs, char rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (char lhs, const RWStandardCConstSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator<=(const RWStandardCString& lhs, const RWStandardCString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWStandardCString& lhs, const RWStandardCSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWStandardCSubString& lhs, const RWStandardCString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWStandardCString& lhs, const RWStandardCConstSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWStandardCConstSubString& lhs, const RWStandardCString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWStandardCString& lhs, const std::string& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const std::string& lhs, const RWStandardCString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWStandardCString& lhs, const char* rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const char* lhs, const RWStandardCString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWStandardCString& lhs, char rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(char lhs, const RWStandardCString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWStandardCSubString& lhs, const RWStandardCSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWStandardCSubString& lhs, const RWStandardCConstSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWStandardCConstSubString& lhs, const RWStandardCSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWStandardCSubString& lhs, const std::string& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const std::string& lhs, const RWStandardCSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWStandardCSubString& lhs, const char* rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const char* lhs, const RWStandardCSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWStandardCSubString& lhs, char rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(char lhs, const RWStandardCSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWStandardCConstSubString& lhs, const RWStandardCConstSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWStandardCConstSubString& lhs, const std::string& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const std::string& lhs, const RWStandardCConstSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWStandardCConstSubString& lhs, const char* rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const char* lhs, const RWStandardCConstSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWStandardCConstSubString& lhs, char rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(char lhs, const RWStandardCConstSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator>=(const RWStandardCString& lhs, const RWStandardCString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWStandardCString& lhs, const RWStandardCSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWStandardCSubString& lhs, const RWStandardCString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWStandardCString& lhs, const RWStandardCConstSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWStandardCConstSubString& lhs, const RWStandardCString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWStandardCString& lhs, const std::string& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const std::string& lhs, const RWStandardCString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWStandardCString& lhs, const char* rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const char* lhs, const RWStandardCString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWStandardCString& lhs, char rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(char lhs, const RWStandardCString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWStandardCSubString& lhs, const RWStandardCSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWStandardCSubString& lhs, const RWStandardCConstSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWStandardCConstSubString& lhs, const RWStandardCSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWStandardCSubString& lhs, const std::string& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const std::string& lhs, const RWStandardCSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWStandardCSubString& lhs, const char* rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const char* lhs, const RWStandardCSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWStandardCSubString& lhs, char rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(char lhs, const RWStandardCSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWStandardCConstSubString& lhs, const RWStandardCConstSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWStandardCConstSubString& lhs, const std::string& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const std::string& lhs, const RWStandardCConstSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWStandardCConstSubString& lhs, const char* rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const char* lhs, const RWStandardCConstSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWStandardCConstSubString& lhs, char rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(char lhs, const RWStandardCConstSubString& rhs)
{
    return !(lhs < rhs);
}

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an RWCString.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<RWStandardCString> : public std::unary_function<RWStandardCString, size_t> {
    /**
     * Constructs a hash object instance with the specified comparison
     * type.
     */
    RWTHash<RWStandardCString>(RWStandardCString::caseCompare cmp = RWStandardCString::exact)
        : cmp_(cmp) {}

    /**
     * Returns a hash of \a obj. This function is the equivalent of calling
     *
     * @code
     * obj.hash(cmp);
     * @endcode
     *
     * Where \a cmp is the comparison type specified at construction.
     */
    size_t operator()(const RWStandardCString& obj) const {
        return obj.hash(cmp_);
    }

private:
    RWStandardCString::caseCompare cmp_;
};

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif // RW_TOOLS_STDCSTRING_H
