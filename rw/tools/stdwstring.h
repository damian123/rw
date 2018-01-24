#ifndef RW_TOOLS_STDWSTRING_H
#define RW_TOOLS_STDWSTRING_H



/**********************************************************************
 *
 * wstring.h -  Header file describing class RWWString
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/stdwstring.h#1 $
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
#  define RWStandardWString RWWString
#  define RWStandardWSubString RWWSubString
#  define RWStandardWConstSubString RWWConstSubString
#endif

#include <string>

class RW_TOOLS_GLOBAL RWStandardWString;
class RW_TOOLS_GLOBAL RWStandardCString;
class RW_TOOLS_SYMBOLIC RWStandardWSubString;
class RW_TOOLS_SYMBOLIC RWStandardWConstSubString;

#include <rw/ref.h> // for backwards source compatibility

#include <rw/tools/stdcstring.h>
#include <rw/locale.h>

#include <rw/tools/hash.h>
#include <rw/tools/pointer.h>
#include <rw/tools/ristream.h>
#include <rw/tools/rostream.h>

#include <rw/rwfile.h>
#include <rw/regexp.h>
#include <rw/rwwchar.h>

#include <rw/vstream.h>

#if !defined(RW_DISABLE_DEPRECATED) && defined(RW_WIDE_CHAR_REG_EXPRESSIONS) // not implemented
template<class charT> class RW_TOOLS_GLOBAL RWTRegularExpression;

/**
 * @ingroup string_processing_classes
 * @deprecated As of SourcePro 4, use \link RWTRegex RWTRegex<wchar_t>\endlink instead.
 *
 * @brief Deprecated. This class is a typedef for
 * \link RWTRegularExpression RWTRegularExpression<wchar_t>\endlink.
 */
typedef RWTRegularExpression<wchar_t> RWWRExpr;
#endif

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

// Document RWStandardWString and related classes.
/**
 * @class RWStandardWSubString rw/tools/stdwstring.h
 * @ingroup string_processing_classes
 * @brief Alternate implementation of RWWSubString when
 * \c RW_COPY_ON_WRITE_STRING is not defined.
 *
 * RWStandardWSubString should be directly used only with instances
 * of class RWStandardWString. RWWSubString should be used with instances
 * of class RWWString.
 *
 * @see RWWSubString for additional documentation.
 */

/**
 * @class RWStandardWConstSubString rw/tools/stdwstring.h
 * @ingroup string_processing_classes
 * @brief Alternate implementation of RWWConstSubString when
 * \c RW_COPY_ON_WRITE_STRING is not defined.
 *
 * RWStandardWConstSubString should be directly used only with instances
 * of class RWStandardWString. RWWConstSubString should be used with instances
 * of class RWWString.
 *
 * @see RWWConstSubString for additional documentation.
 */

/**
 * @class RWStandardWString rw/tools/stdwstring.h
 * @ingroup string_processing_classes
 * @brief Alternate implementation of RWWString when
 * \c RW_COPY_ON_WRITE_STRING is not defined.
 *
 * Unless the specific performance characteristics of RWStandardWString
 * are required, applications should use RWWString instead.  See the
 * <em>Essential Tools Module User's Guide</em> for additional information.
 *
 * @see RWWString for additional documentation.
 */


/**
 * @class RWWSubString rw/wstring.h
 * @ingroup string_processing_classes
 *
 * @brief Allows some subsection of an RWWString to be addressed
 * by defining a <i>starting position</i> and an \e extent.
 *
 * The class RWWSubString allows some subsection of an RWWString
 * to be addressed by defining a starting position and an extent.
 * For example the 7th through the 11th elements, inclusive, would
 * have a starting position of 7 and an extent of 5. The specification
 * of a starting position and extent can also be done in your behalf
 * by such functions as RWWString::strip() or the overloaded function
 * call operator taking a regular expression as an argument. There
 * are no public constructors; RWWSubString objects are constructed
 * by various functions of the RWWString class and then destroyed
 * immediately.
 *
 * A zero length substring is one with a defined starting position
 * and an extent of zero. It can be thought of as starting just
 * before the indicated character, but not including it. It can
 * be used as an lvalue. A null substring is also legal and is
 * frequently used to indicate that a requested substring, perhaps
 * through a search, does not exist. A null substring can be detected
 * with member function isNull(). However, it cannot be used as
 * an lvalue.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/wstring.h>
 * RWWString s(L"test string");
 * s(6,3);   // "tri"
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
 * #include <rw/wstring.h>
 *
 * int main()
 * {
 *     RWWString s (L"What I tell you is true.");
 *
 *     std::cout << "Take the string: [" << s << "]\n";
 *
 *     // Create a substring and use it as an lvalue:
 *     s(16, 0) = L"three times ";
 *
 *     std::cout << "After assigning text to a substring, the full string becomes: ["
 *               << s << "]" << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program output (assuming your platform displays wide characters as US-ASCII
 * if they are in the US-ASCII character set):
 *
 * @code
 * Take the string: [What I tell you is true.]
 * After assigning text to a substring, the full string becomes: [What I tell you three times is true.]
 * @endcode
 */
class RW_TOOLS_SYMBOLIC RWStandardWSubString
{
public:

    RWStandardWSubString(const RWStandardWSubString& sp)
        : str_(sp.str_), begin_(sp.begin_), extent_(sp.extent_) {
        ;
    }

    /**
     * Assignment from a wide character string. Example:
     *
     * @code
     * RWWString wstr(L"Mary had a little lamb");
     * wchar_t dat[] = L"Perrier";
     * wstr(11,4) = dat;   // "Mary had a Perrier"
     * @endcode
     *
     * The number of characters selected need not match; if
     * they differ, \c str is resized appropriately. If self
     * is the null substring, then the statement has no effect.
     *
     * Returns a reference to self.
     */
    RWStandardWSubString& operator=(const wchar_t* str);

    /**
     * Assignment from an RWWString. Example:
     *
     * @code
     * std::wstring a;
     * RWWString b;
     * ...
     * b(2, 3) = a;
     * @endcode
     *
     * Copies <tt>a</tt>'s data into the substring <tt>b(2,3)</tt>. The number
     * of elements need not match; if they differ, \c b is resized
     * appropriately. If self is the null substring, then the statement
     * has no effect. Returns a reference to self.
     */
    RWStandardWSubString& operator=(const std::wstring& str);

    /**
     * Assignment from an RWWString. Example:
     *
     * @code
     * RWWString a;
     * RWWString b;
     * ...
     * b(2, 3) = a;
     * @endcode
     *
     * Copies <tt>a</tt>'s data into the substring <tt>b(2,3)</tt>. The number
     * of elements need not match; if they differ, \c b is resized
     * appropriately. If self is the null substring, then the statement
     * has no effect. Returns a reference to self.
     */
    RWStandardWSubString& operator=(const RWStandardWString& str);

    /**
     * Assignment from an RWWSubString. Example:
     *
     * @code
     * RWWString a(10, L'a');
     * RWWString b;
     * ...
     * b(2, 3) = a(5,5);
     * @endcode
     *
     * Copies 5 characters of <tt>a</tt>'s data into the substring <tt>b(2,3)</tt>.
     * The number of elements need not match; if they differ, \c b is resized
     * appropriately. If self is the null substring, then the statement has no
     * effect. Returns a reference to self.
     */
    RWStandardWSubString& operator=(const RWStandardWSubString& str);

    /**
     * Assignment from an RWWConstSubString. Example:
     *
     * @code
     * const RWWString a(10, L'a');
     * RWWString b;
     * ...
     * b(2, 3) = a(5,5);
     * @endcode
     *
     * Copies 5 characters of <tt>a</tt>'s data into the substring <tt>b(2,3)</tt>.
     * The number of elements need not match; if they differ, \c b is resized
     * appropriately. If self is the null substring, then the statement has no
     * effect. Returns a reference to self.
     */
    RWStandardWSubString& operator=(const RWStandardWConstSubString& str);

    /**
     * Returns the \a i <sup>th</sup> character of the substring. The
     * index \a i must be between zero and the length of the substring
     * less one. Bounds checking is enabled by defining the preprocessor
     * macro \c RWBOUNDS_CHECK before including \c <rw/wstring.h>.
     *
     * @throw RWBoundsErr if RWBOUNDS_CHECK is defined and the index is out
     * of range.
     */
    wchar_t& operator()(size_t i);

    /**
      * Returns the \a i <sup>th</sup> character of the substring. The
      * index \a i must be between zero and the length of the substring
      * less one. Bounds checking is enabled by defining the preprocessor
      * macro \c RWBOUNDS_CHECK before including \c <rw/wstring.h>.
      *
      * @throw RWBoundsErr if RWBOUNDS_CHECK is defined and the index is out
      * of range.
      */
    wchar_t operator()(size_t i) const;

    /**
     * Returns the \a i <sup>th</sup> character of the substring. The
     * index \a i must be between zero and the length of the substring
     * less one. \a i is converted to a \c size_t and bounds checking
     * is performed.
     *
     * @throw RWBoundsErr if the index is out of range.
     */
    wchar_t& operator[](short i);

    /**
     * @copydoc operator[](short)
     */
    wchar_t& operator[](int i);

    /**
     * @copydoc operator[](short)
     */
    wchar_t& operator[](long i);

    /**
     * @copydoc operator[](short)
     */
    wchar_t& operator[](unsigned short i);

    /**
     * @copydoc operator[](short)
     */
    wchar_t& operator[](unsigned int i);

    /**
     * @copydoc operator[](short)
     */
    wchar_t& operator[](unsigned long i);

    /**
     * @copydoc operator[](short)
     */
    wchar_t operator[](short i) const;

    /**
     * @copydoc operator[](short)
     */
    wchar_t operator[](int i) const;

    /**
     * @copydoc operator[](short)
     */
    wchar_t operator[](long i) const;

    /**
     * @copydoc operator[](short)
     */
    wchar_t operator[](unsigned short i) const;

    /**
     * @copydoc operator[](short)
     */
    wchar_t operator[](unsigned int i) const;

    /**
     * @copydoc operator[](short)
     */
    wchar_t operator[](unsigned long i) const;

#if !defined(RW_NO_LONG_LONG)

    /**
     * @copydoc operator[](short)
     */
    wchar_t& operator[](long long i);

    /**
     * @copydoc operator[](short)
     */
    wchar_t& operator[](unsigned long long i);

    /**
     * @copydoc operator[](short)
     */
    wchar_t operator[](long long i) const;

    /**
     * @copydoc operator[](short)
     */
    wchar_t operator[](unsigned long long i) const;
#endif

    /**
     * @internal
     * Returns a pointer to the start of the substring.
     *
     * @note
     * Unlike RWWString::data(), the <tt>const wchar_t*</tt> returned by
     * this function is not null-terminated at its extent.
     */
    inline const wchar_t* startData() const;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12, use startData() instead.
     */
    RW_DEPRECATE_FUNC("Use RWStandardWSubString::startData() instead")
    const wchar_t* data() const {
        return startData();
    }
#endif

    /**
     * Returns the extent (i.e., length) of the substring.
     */
    size_t length() const {
        return extent_;
    }

    /**
     * Returns the index of the starting element of the substring.
     */
    size_t start() const {
        return begin_;
    }

    /**
     * Changes all upper-case letters in self to lower-case. Uses the
     * C library function \c towlower().
     */
    void toLower();

    /**
     * Changes all lower-case letters in self to upper-case. Uses the
     * C library function \c towupper().
     */
    void toUpper();

    /**
     * Returns \c true if \c this is a null substring.
     */
    bool isNull() const {
        return begin_ == RW_NPOS;
    }

    /**
     * Returns \c true if \c this is a null substring.
     */
    int               operator!() const {
        return begin_ == RW_NPOS;
    }

protected:

    /**
     * @internal
     * Throws an RWBoundsErr exception indicating the position \a pos or
     * extent \a len is outside the string length \a strLen.
     */
    void subStringError(size_t strLen, size_t pos, size_t len) const;

    /**
     * @internal
     * Verifies that the index \a i is within the bounds of the substring.
     *
     * @throw RWBoundsErr if the index is out of range.
     */
    void assertElement(std::wstring::size_type i) const;

private:

    /**
     * @internal
     * Constructs a substring instance from the string \a str with starting
     * position \a start and extent \a len.
     */
    inline RWStandardWSubString(const RWStandardWString& str, size_t start, size_t len);

    RWStandardWString*          str_;     // Referenced string
    size_t begin_;   // Index of starting character
    size_t extent_;  // Length of RWStandardWSubString

    friend class RWStandardWString;
    friend class RWStandardWConstSubString;

};

/**
 * @class RWWConstSubString rw/wstring.h
 * @ingroup string_processing_classes
 *
 * @brief Allows some subsection of an RWWString to be addressed
 * by defining a <i>starting position</i> and an \e extent.
 *
 * The class RWWConstSubString allows some subsection of an RWWString
 * to be addressed by defining a <i>starting position</i> and an
 * \e extent. For example, the 7th through the 11th elements,
 * inclusive, would have a starting position of 7 and an extent
 * of 5. The specification of a starting position and extent can
 * also be done on your behalf by such functions as RWWString::strip().
 * There are no public constructors other than two
 * that allow you to create an RWWConstSubString from an existing
 * RWWSubString or RWWConstSubString. In general, RWWConstSubString
 * objects are constructed by various functions of the RWWString class and
 * then destroyed immediately.
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
 * #include <rw/wstring.h>
 * RWWString s("test string");
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
 * #include <rw/wstring.h>
 *
 * int main()
 * {
 * const RWWString s (L"What I tell you is true.");
 *
 *     std::cout << "Take the string: [" << s << "]\n";
 *
 *     // Create a string from substrings:
 *     const RWWString s2 (s(0, 16) + L"three times " + s(16, 8));
 *
 *     std::cout << "After creating a string from substrings, you have: ["
 *               << s2 << "]" << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program output (assuming your platform displays wide characters as US-ASCII
 * if they are in the US-ASCII character set):
 *
 * @code
 * Take the string: [What I tell you is true.]
 * After creating a string from substrings, you have: [What I tell you three times is true.]
 * @endcode
 */
class RW_TOOLS_SYMBOLIC RWStandardWConstSubString
{
public:

    /**
     * Constructs an RWWConstSubString from an existing RWWSubString. The
     * resulting substring references the same start and extent of the
     * RWWString associated with \a str.
     *
     * @note
     * The resulting substring instance should not be used if the
     * RWWString associated with \a str is modified after the substring
     * is instantiated.
     */
    RWStandardWConstSubString(const RWStandardWSubString& str)
        : str_(str.str_), begin_(str.begin_), extent_(str.extent_) {
        ;
    }

    /**
     * Copy constructor. The resulting substring references the same start
     * and extent of the RWWString associated with \a str.
     *
     * @note
     * The resulting substring instance should not be used if the
     * RWWString associated with \a str is modified after the substring
     * is instantiated.
     */
    RWStandardWConstSubString(const RWStandardWConstSubString& str)
        : str_(str.str_), begin_(str.begin_), extent_(str.extent_) {
        ;
    }

    /**
     * Returns the \a i <sup>th</sup> character of the substring. The
     * index \a i must be between zero and the length of the substring
     * less one. Bounds checking is enabled by defining the preprocessor
     * macro \c RWBOUNDS_CHECK before including \c <rw/wstring.h>.
     *
     * @throw RWBoundsErr if RWBOUNDS_CHECK is defined and the index is out
     * of range.
     */
    wchar_t operator()(size_t i) const;

    /**
     * Returns the \a i <sup>th</sup> character of the substring. The
     * index \a i must be between zero and the length of the substring
     * less one. \a i is converted to a \c size_t and bounds checking
     * is performed.
     *
     * @throw RWBoundsErr if the index is out of range.
     */
    wchar_t operator[](short i) const;

    /**
     * @copydoc RWWSubString::operator[](int)const
     */
    wchar_t operator[](int i) const;

    /**
     * @copydoc RWWSubString::operator[](long)const
     */
    wchar_t operator[](long i) const;

    /**
     * @copydoc RWWSubString::operator[](unsigned short)const
     */
    wchar_t operator[](unsigned short i) const;

    /**
     * @copydoc RWWSubString::operator[](unsigned int)const
     */
    wchar_t operator[](unsigned int i) const;

    /**
     * @copydoc RWWSubString::operator[](unsigned long)const
     */
    wchar_t operator[](unsigned long i) const;

#if !defined(RW_NO_LONG_LONG)
    /**
     * @copydoc RWWSubString::operator[](long long)const
     */
    wchar_t operator[](long long i) const;

    /**
     * @copydoc RWWSubString::operator[](unsigned long long)const
     */
    wchar_t operator[](unsigned long long i) const;
#endif


    /**
     * @internal
     * Returns a pointer to the start of the substring.
     *
     * @note
     * Unlike RWWString::data(), the <tt>const wchar_t*</tt> returned by
     * this function is not null-terminated at its extent.
     */
    inline const wchar_t* startData() const;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12, use startData() instead.
     */
    RW_DEPRECATE_FUNC("Use RWStandardWConstSubString::startData() instead")
    const wchar_t* data() const {
        return startData();
    }
#endif

    /**
     * Returns the extent (i.e., length) of the substring.
     */
    size_t length() const {
        return extent_;
    }

    /**
     * Returns the starting element of the substring.
     */
    size_t start() const {
        return begin_;
    }

    /**
     * Returns \c true if \c this is a null substring.
     */
    bool isNull() const {
        return begin_ == RW_NPOS;
    }

    /**
     * Returns \c true if \c this is a null substring.
     */
    int            operator!() const {
        return begin_ == RW_NPOS;
    }

protected:

    /**
     * @internal
     * Throws an RWBoundsErr exception indicating the position \a pos or
     * extent \a len is outside the string length \a strLen.
     */
    void subStringError(size_t strLen, size_t pos, size_t len) const;

    /**
     * @internal
     * Verifies that the index \a i is within the bounds of the substring.
     *
     * @throw RWBoundsErr if the index is out of range.
     */
    void assertElement(std::wstring::size_type i) const;

private:

    /**
     * @internal
     * Constructs a substring instance from the string \a str with starting
     * position \a start and extent \a len.
     */
    inline RWStandardWConstSubString(const RWStandardWString& str, size_t start, size_t len);

    const RWStandardWString*      str_;     // Referenced string
    size_t begin_;   // Index of starting character
    size_t extent_;  // Length of RWStandardWConstSubString

    friend class RWStandardWString;
    friend class RWStandardWSubString;

};

/**
 * @class RWWString rw/wstring.h
 * @ingroup string_processing_classes
 *
 * @brief Offers powerful and convenient facilities for
 * manipulating wide character strings.
 *
 * Class RWWString offers very powerful and convenient facilities
 * for manipulating wide character strings.
 *
 * @note
 * RWWString is designed for use with wide character strings. To
 * manipulate multibyte character sequences, use RWCString.
 *
 * This string class manipulates wide characters of the fundamental
 * type \c wchar_t. These characters are generally two or four bytes,
 * and can be used to encode richer code sets than the classic
 * \c char type. Because \c wchar_t characters are all the same size,
 * indexing is fast.
 *
 * Conversion to and from multibyte and US-ASCII forms are provided
 * by the RWWString constructors, and by the RWWString member functions
 * isAscii(), toAscii(), and toMultiByte().
 *
 * Stream operations implicitly translate to and from the multibyte
 * stream representation. That is, on output, wide character strings
 * are converted into multibyte strings, while on input they are
 * converted back into wide character strings. Hence, the external
 * representation of wide character strings is usually as multibyte
 * character strings, saving storage space, and making interfaces
 * with devices (which usually expect multibyte strings) easier.
 *
 * RWWString tolerates embedded nulls.
 *
 * Parameters of type <tt>const wchar_t*</tt> must not be passed
 * a value of zero. This is detected in the debug version of the
 * library.
 *
 * A separate RWWSubString class supports substring extraction and
 * modification operations. RWWConstSubString supports substring
 * extractions on \c const RWWString instances.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/wstring.h>
 * RWWString a;
 * @endcode
 *
 * @section persistence Persistence
 * Simple
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/wstring.h>
 *
 * int main ()
 * {
 *     RWWString a(L"There is no joy in Beantown");
 *
 *     std::cout << a << std::endl << "becomes......" << std::endl;
 *
 *     a.subString(L"Beantown") = L"Redmond";
 *
 *     std::cout << a << std::endl;
 *
 *     return 0;
 *
 * }
 * @endcode
 *
 * Program output (assuming your platform displays wide characters as US-ASCII
 * if they are in the US-ASCII character set):
 *
 * @code
 * "There is no joy in Beantown" becomes "There is no joy in Redmond."
 * @endcode
 */
class RW_TOOLS_GLOBAL RWStandardWString
{
public:
    /**
     * Specifies how to strip characters from a string. Used by strip().
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
     * @internal
     * @deprecated As of SourcePro 12, provided for compatibility with code that
     * relies on the enumeration RWStandardWString::scopeType.
     *
     * Specifies whether regular expression functions replace
     * the first #one substring matched by the regular expression
     * or replaces #all substrings matched by the regular expression.
     */
    enum RW_DEPRECATE_TYPE("") scopeType {
        one,
        all
    };
#endif

    /**
     * Specifies that a function should assume US-ASCII content
     * when converting from a <tt>const char*</tt> string.
     */
    enum ascii_ {

        /**
         * Convert from US-ASCII
         */
        ascii

#if !defined(RW_DISABLE_DEPRECATED)
        /**
         * @internal
         * @deprecated As of SourcePro 12, use #ascii instead.
         */
        , RW_DEPRECATE_ENUMERATOR(singleByte, "Use RWStandardWString::ascii instead")
#endif
    };

    /**
     * Specifies that a function should assume multibyte content
     * when converting from a <tt>const char*</tt> string.
     */
    enum multiByte_ {

        /**
         * Convert from multibyte
         */
        multiByte
    };

    /**
     * The mutable substring class associated with RWWString.
     */
    typedef RWStandardWSubString SubString;

    /**
     * The immutable substring class associated with RWWString.
     */
    typedef RWStandardWConstSubString ConstSubString;

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12, use SubString instead.
     *
     * Retained for backward compatibility with SourcePro 11.1
     */
    typedef RW_DEPRECATE_TYPE("Use RWStandardWString::SubString instead") SubString SubStringType;

    /**
     * @internal
     * @deprecated As of SourcePro 12, use ConstSubString instead.
     *
     * Retained for backward compatibility with SourcePro 11.1
     */
    typedef RW_DEPRECATE_TYPE("Use RWStandardWString::ConstSubString instead") ConstSubString ConstSubStringType;
#endif

    /**
     * Creates a string of length zero (the null string).
     */
    RWStandardWString() {}

    /**
     * Constructs a string with data copied from \a str.
     */
    RWStandardWString(const std::wstring& str)   : data_(str) {}

    /**
     * Copy constructor. The created string copies the data from \a str.
     */
    RWStandardWString(const RWStandardWString& str)             : data_(str.data_) {}

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The created string takes ownership of the
     * data owned by \a str.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWStandardWString(RWStandardWString && str);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Creates a string of length zero (the null string). The string's
     * \e capacity (that is, the size it can grow to without resizing)
     * is given by the parameter \a cap. We recommend creating an RWSize_T
     * value from a numerical constant to pass into this constructor.
     * While RWSize_T knows how to convert \c size_t values to itself,
     * conforming compilers choose the conversion to \c char instead.
     */
    RWStandardWString(RWSize_T cap) {
        capacity(cap.value());
    }

    /**
     * Constructs a string containing the single character \a c.
     */
    RWStandardWString(wchar_t c)                     : data_(1, c) {}

    /**
     * Constructs a string containing the character \a c repeated
     * \a count times.
     */
    RWStandardWString(wchar_t c, size_t count)           : data_(count, c) {}

    /**
     * Converts from the null-terminated wide character string \a str. The
     * created string copies the data pointed to by \a str, up to the first
     * terminating null.
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
    RWStandardWString(const wchar_t* str)              : data_(str, wcslen(str)) {}

    /**
     * Constructs a string from the character string \a str. The created
     * string copies the data pointed to by \a str. Exactly \a len
     * characters are copied, including any embedded nulls. Hence,
     * the buffer pointed to by \a str must be at least \a len
     * <tt>* sizeof(wchar_t)</tt> bytes or \a len wide characters long.
     */
    RWStandardWString(const wchar_t* str, size_t len)    : data_(str, len) {}

    /**
     * Constructs a string containing the single character \a c. The
     * character \a c is widened to a \c wchar_t.
     */
    RWStandardWString(char c) : data_(1, wchar_t (c)) {}

    /**
     * Constructs a string containing the character \a c repeated
     * \a count times. The character \a c is widened to a \c wchar_t.
     */
    RWStandardWString(char c, size_t count) : data_(count, wchar_t (c)) {}

    /**
     * Constructs a string containing the single signed character \a c. The
     * character \a c is widened to a \c wchar_t.
     */
    RWStandardWString(signed char c) : data_(1, wchar_t (char(c))) {}

    /**
     * Constructs a string containing the single unsigned character \a c. The
     * character \a c is widened to a \c wchar_t.
     */
    RWStandardWString(unsigned char c) : data_(1, wchar_t (char(c))) {}

    /**
     * Constructs a wide character string from the US-ASCII character string
     * contained in \a str.
     *
     * For this conversion to be successful, the string \e must contain only
     * US-ASCII characters. This can be confirmed (if necessary) using
     * RWCString::isAscii(). If \a str contains non-US-ASCII characters, the
     * RWWString(const char*,multiByte_) constructor should be used instead.
     *
     * This constructor can be used as follows:
     *
     * @code
     * RWWString a("An US-ASCII character string", RWWString::ascii);
     * @endcode
     */
    RWStandardWString(const char* str, ascii_) {
        initChars(str, strlen(str));
    }

    /**
     * @copydoc RWWString(const char*, ascii_)
     */
    RWStandardWString(const RWStandardCString& str, ascii_);

    /**
     * Constructs a wide character string from the multibyte character
     * string contained in \a str. The conversion is done using the
     * Standard C Library function \c mbstowcs().
     *
     * This constructor can be used as follows:
     *
     * @code
     * RWWString a("\306\374\315\313\306\374", multiByte);
     * @endcode
     */
    RWStandardWString(const char* str, multiByte_) {
        size_t bytes = RWStandardCString::mbLength(str);
        initMB(str, (bytes != RW_NPOS) ? bytes : 0);
    }
    /**
     * @copydoc RWWString(const char*, multiByte_)
     */
    RWStandardWString(const RWStandardCString& str, multiByte_);

    /**
     * Constructs a wide character string from the US-ASCII character
     * string contained in \a str, copying exactly \a len characters including
     * any embedded nulls. Hence, the buffer pointed to by \a str must
     * be long enough to hold \a len characters.
     */
    RWStandardWString(const char* str, size_t len, ascii_) {
        initChars(str, len);
    }

    /**
     * Constructs a wide character string from the multibyte character
     * string contained in \a str, copying exactly \a len characters including
     * any embedded nulls. Hence, the buffer pointed to by \a str must
     * be long enough to hold \a len characters.
     */
    RWStandardWString(const char* str, size_t len, multiByte_) {
        initMB(str, len);
    }

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12, use RWWString(const RWWString&) instead.
     *
     * The equivalent of calling:
     *
     * @code
     * RWWString(str);
     * @endcode
     */
    RW_DEPRECATE_FUNC("Use RWStandardWString::RWStandardWString(const RWStandardWString&) instead")
    RWStandardWString(const RWStandardWString& str, multiByte_) : data_(str.data_) {}
#endif

    /**
     * Converts from substring. The created string copies the substring
     * represented by \a str.
     */
    inline RWStandardWString(const SubString& str);

    /**
     * Converts from substring. The created wide string copies the
     * substring represented by \a str.
     */
    inline RWStandardWString(const ConstSubString& str);

#if !defined(RW_DISABLE_DEPRECATED)
    /**
     * @internal
     * @deprecated As of SourcePro 12, use
     * RWWString(const RWCString&, RWWString::ascii) instead.
     *
     * This is the equivalent of calling:
     *
     * @code
     * RWWString(str, ascii)
     * @endcode
     */
    RW_DEPRECATE_FUNC("Use RWStandardWString::RWStandardWString(const RWCString&, RWWStandardWString::scopeType) instead")
    RWStandardWString(const RWStandardCString& str);
#endif


    ~RWStandardWString() {
#if defined(RW_DEBUG) && defined(RW_NO_BASIC_STRING_COW)
        // could make invalid pointers more obvious some of the time
        data_.replace(0, length(), length(), RW_STATIC_CAST(wchar_t, '0'));
#endif
    }

    /**
     * The equivalent of calling RWWString::data() on self.
     */
    operator const wchar_t* () const {
        return data_.c_str();
    }

    /**
     * Assignment operator. Copies the character \a c into self.
     * Returns a reference to self.
     */
    RWStandardWString& operator=(wchar_t c);

    /**
     * Assignment operator. Copies the null-terminated character string
     * pointed to by \a str into self. Returns a reference to self.
     *
     * @note
     * This function is incompatible with \a str strings with
     * embedded nulls.
     */
    RWStandardWString& operator=(const wchar_t* str);

    /**
     * @copydoc operator=(const RWWString&)
     */
    RWStandardWString& operator=(const std::wstring& str);

    /**
     * Assignment operator. The string copies the data from \a str.
     * Returns a reference to self.
     */
    RWStandardWString& operator=(const RWStandardWString& str);

    /**
     * @copydoc operator=(const RWWString&)
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardWString& operator=(const SubString& str);

    /**
     * @copydoc operator=(const RWWString&)
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardWString& operator=(const ConstSubString& str);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a str.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWStandardWString& operator=(RWStandardWString && str);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Appends the character \a c to self. Returns a reference to self.
     */
    RWStandardWString& operator+=(wchar_t c) {
        return append(c);
    }

    /**
     * Appends the null-terminated character string pointed to by
     * \a str to self. Returns a reference to self.
     *
     * @note
     * This function is incompatible with \a str strings with
     * embedded nulls.
     */
    RWStandardWString& operator+=(const wchar_t* str) {
        return append(str);
    }

    /**
     * Appends the string \a str to self. Returns a reference to self.
     */
    RWStandardWString& operator+=(const std::wstring& str) {
        return append(str);
    }

    /**
     * @copydoc operator+=(const std::wstring&)
     */
    RWStandardWString& operator+=(const RWStandardWString& str) {
        return append(str);
    }

    /**
     * Returns the \a i <sup>th</sup> character in self. The index \a i must be
     * between 0 and the length of the string less one. Bounds checking
     * is performed if the preprocessor macro \c RWBOUNDS_CHECK has
     * been defined before including \c <rw/wstring.h>.
     *
     * @throw RWBoundsErr if RWBOUNDS_CHECK is defined and the index is out
     * of range.
     */
    wchar_t& operator()(size_t i);

    /**
     * Returns the \a i <sup>th</sup> character in self. The index \a i must be
     * between 0 and the length of the string less one. Bounds checking
     * is performed if the preprocessor macro \c RWBOUNDS_CHECK has
     * been defined before including \c <rw/wstring.h>.
     *
     * @throw RWBoundsErr if RWBOUNDS_CHECK is defined and the index is out
     * of range.
     */
    wchar_t operator()(size_t i) const;

    /**
     * Returns the \a i <sup>th</sup> character in self. The index \a i must be
     * between 0 and the length of the string less one. \a i is converted to a
     * \c size_t and bounds checking is performed.
     *
     * @throw RWBoundsErr if the index is out of range.
     */
    wchar_t& operator[](short i);

    /**
     * @copydoc operator[](short)
     */
    wchar_t& operator[](int i);

    /**
     * @copydoc operator[](short)
     */
    wchar_t& operator[](long i);

    /**
     * @copydoc operator[](short)
     */
    wchar_t& operator[](unsigned short i);

    /**
     * @copydoc operator[](short)
     */
    wchar_t& operator[](unsigned int i);

    /**
     * @copydoc operator[](short)
     */
    wchar_t& operator[](unsigned long i);

    /**
     * @copydoc operator[](short)
     */
    wchar_t operator[](short i) const;

    /**
     * @copydoc operator[](short)
     */
    wchar_t operator[](int i) const;

    /**
     * @copydoc operator[](short)
     */
    wchar_t operator[](long i) const;

    /**
     * @copydoc operator[](short)
     */
    wchar_t operator[](unsigned short i) const;

    /**
     * @copydoc operator[](short)
     */
    wchar_t operator[](unsigned int i) const;

    /**
     * @copydoc operator[](short)
     */
    wchar_t operator[](unsigned long i) const;

#if !defined(RW_NO_LONG_LONG)
    /**
     * @copydoc operator[](short)
     */
    wchar_t& operator[](long long i);

    /**
     * @copydoc operator[](short)
     */
    wchar_t& operator[](unsigned long long i);

    /**
     * @copydoc operator[](short)
     */
    wchar_t operator[](long long i) const;

    /**
     * @copydoc operator[](short)
     */
    wchar_t operator[](unsigned long long i) const;
#endif


    /**
     * Substring operator. Returns a substring of self with length
     * \a len, starting at index \a start. The sum of \a start plus
     * \a len must be less than or equal to the string length.
     *
     * @throws RWBoundsErr if the library was built using the
     * \c RW_DEBUG flag, and \a start and \a len are out of range.
     */
    SubString         operator()(size_t start, size_t len);

    /**
     * Substring operator. Returns a substring of self with length
     * \a len, starting at index \a start.The sum of \a start plus
     * \a len must be less than or equal to the string length.
     *
     * @throws RWBoundsErr if the library was built using the
     * \c RW_DEBUG flag, and \a start and \a len are out of range.
     */
    ConstSubString    operator()(size_t start, size_t len) const;


#if !defined(RW_DISABLE_DEPRECATED) && defined(RW_WIDE_CHAR_REG_EXPRESSIONS) // not implemented
    RW_SUPPRESS_DEPRECATED_WARNINGS

    /**
     * @internal
     * @deprecated As of SourcePro 12, use RWTRegex::search instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::search instead")
    SubString match(const RWWRExpr& re);

    /**
     * @internal
     * @deprecated As of SourcePro 12, use RWTRegex::search instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::search instead")
    SubString match(const RWWRExpr& re, size_t start);

    RW_RESTORE_DEPRECATED_WARNINGS
#endif

    /**
     * Starting at \a start, returns a substring representing the first
     * occurrence of the null-terminated string \a pat in self. If \a pat
     * is not found, a null substring instance is returned.
     *
     * @note
     * This function is incompatible with \a pat strings with embedded nulls.
     */
    SubString         subString(const wchar_t* pat, size_t start = 0, caseCompare cmp = exact);

    /**
     * @copydoc subString(const wchar_t*, size_t, caseCompare)
     */
    ConstSubString    subString(const wchar_t* pat, size_t start = 0, caseCompare cmp = exact) const;


    /**
     * Returns a substring of self where the character \a c has been
     * stripped off the beginning, end, or both ends of the string,
     * depending on \a st.
     */
    SubString         strip(stripType st = trailing, wchar_t c = L' ');

    /**
     * @copydoc strip(stripType, wchar_t)
     */
    ConstSubString    strip(stripType st = trailing, wchar_t c = L' ') const;

#if !defined(RW_DISABLE_DEPRECATED) && defined(RW_WIDE_CHAR_REG_EXPRESSIONS) // not implemented
    RW_SUPPRESS_DEPRECATED_WARNINGS

    /**
     * @internal
     * @deprecated As of SourcePro 12, use RWTRegex::search instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::search instead")
    ConstSubString match(const RWWRExpr& re) const;

    /**
     * @internal
     * @deprecated As of SourcePro 12, use RWTRegex::search instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::search instead")
    ConstSubString match(const RWWRExpr& re, size_t start) const;

    RW_RESTORE_DEPRECATED_WARNINGS
#endif

    /**
     * Appends \a count copies of the character \a c to self. Returns a
     * reference to self.
     */
    inline RWStandardWString& append(wchar_t c, size_t count = 1);

    /**
     * Appends a copy of the null-terminated wide character string pointed
     * to by \a str to self. Returns a reference to self.
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
    inline RWStandardWString& append(const wchar_t* str);

    /**
     * Appends a copy of the wide character string \a str to self. Exactly
     * \a len characters are copied, <i>including any embedded nulls</i>. Hence,
     * the buffer pointed to by \a str must be at least \a len characters long.
     * Returns a reference to self.
     */
    inline RWStandardWString& append(const wchar_t* str, size_t len);

    /**
     * Appends a copy of the string \a str to self. Returns a reference
     * to self.
     */
    inline RWStandardWString& append(const std::wstring& str);

    /**
     * @copydoc append(const std::wstring&)
     */
    inline RWStandardWString& append(const RWStandardWString& str);

    /**
     * Appends the first \a len characters of \a str to self. If \a len is
     * greater than the length of \a str, the entire string is appended.
     * Returns a reference to self.
     */
    inline RWStandardWString& append(const std::wstring& str, size_t len);

    /**
     * @copydoc append(const std::wstring&, size_t)
     */
    inline RWStandardWString& append(const RWStandardWString& str, size_t len);

    /**
     * Assigns a copy of the wide character string \a str to self, replacing
     * existing content. Returns a reference to self.
     */
    RWStandardWString& assign(const RWStandardWString& str);

    /**
     * @copydoc assign(const RWWString&)
     */
    RWStandardWString& assign(const std::wstring& str);

    /**
     * @copydoc assign(const RWWString&)
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardWString& assign(const SubString& str);

    /**
     * @copydoc assign(const RWWString&)
     *
     * @note
     * Behavior is undefined if \a str is a null substring.
     */
    RWStandardWString& assign(const ConstSubString& str);

    /**
     * Assigns the null-terminated wide character string pointed to by \a str
     * to self, replacing existing content. Returns a reference to self.
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
    RWStandardWString& assign(const wchar_t* str);

    /**
     * Assigns a copy of the wide character string \a str to self, replacing
     * existing content. Exactly \a len bytes are copied, <i>including
     * any embedded nulls</i>. Hence, the buffer pointed to by \a str must
     * be at least \a len characters long. Returns a reference to self.
     */
    RWStandardWString& assign(const wchar_t* str, size_t len);

    /**
     * Assigns \a count copies of the wide character \a c to self, replacing
     * existing content. Returns a reference to self.
     */
    RWStandardWString& assign(const wchar_t c, size_t count = 1);

    /**
     * Returns the number of bytes necessary to store the object using
     * the global function:
     *
     * @code
     * RWFile& operator<<(RWFile&, const RWWString&);
     * RWvostream& operator<<(RWvostream&, const RWWString&);
     * @endcode
     */
    RWspace           binaryStoreSize() const {
        return length() * sizeof(wchar_t) + sizeof(size_t);
    }

    /**
     * Returns the current capacity of self. This is the number of
     * characters the string can hold without resizing.
     */
    size_t capacity() const;

    /**
     * A non-binding request to alter the capacity of self to \c cap.
     * The capacity is never reduced below the current number of characters.
     * Returns the actual capacity.
     */
    inline size_t capacity(size_t cap);

    /**
     * Returns an \c int less than, greater than, or equal to zero,
     * according to the result of calling the POSIX function \c wscoll()
     * on self and the argument \a str. This method supports locale-dependent
     * collation.
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
    int               collate(const wchar_t* str) const;

    /**
     * @copydoc collate(const wchar_t*) const
     */
    int               collate(const std::wstring& str) const;

    /**
     * @copydoc collate(const wchar_t*) const
     */
    int               collate(const RWStandardWString& str) const;

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
     */
    inline int        compareTo(const RWStandardWString& str, caseCompare cmp = exact) const;

    /**
     * @copydoc compareTo(const RWWString&, caseCompare) const
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
    inline int        compareTo(const wchar_t* str, caseCompare cmp = exact) const;

    /**
     * @copydoc compareTo(const RWWString&, caseCompare) const
     */
    inline int        compareTo(const std::wstring& str, caseCompare cmp = exact) const;

    /**
     * @copydoc compareTo(const RWWString*, caseCompare) const
     */
    int               compareTo(const std::wstring* str, caseCompare cmp = exact) const {
        return compareTo(*str, cmp);
    }

    /**
     * This function is the equivalent of calling:
     *
     * @code
     * RWWString::compareTo(*str, cmp);
     * @endcode
     */
    int               compareTo(const RWStandardWString* str, caseCompare cmp = exact) const {
        return compareTo(*str, cmp);
    }


    /**
     * Returns \c false if RWWString::index(\a str, \a cmp) returns
     * #RW_NPOS, otherwise returns \c true.
     */
    bool contains(const wchar_t* str, caseCompare cmp = exact) const;

    /**
     * @copydoc contains(const wchar_t*, caseCompare) const
     */
    bool contains(const std::wstring& str, caseCompare cmp = exact) const;

    /**
     * @copydoc contains(const wchar_t*, caseCompare) const
     */
    bool contains(const RWStandardWString& str, caseCompare cmp = exact) const;

#if !defined(RW_DISABLE_DEPRECATED)

    /**
     * @internal
     * @deprecated As of SourcePro 12, use operator=(const RWStandardWString&)
     * and modify the copy instead.
     *
     * Creates and returns a copy of self.
     */
    RW_DEPRECATE_FUNC("Use RWStandardWString::operator=(const RWStandardWString&) instead")
    RWStandardWString copy() const;

#endif

    /**
     * Provides access to the RWWString instances's data as a null-terminated
     * string. This datum is owned by the RWWString and may not be deleted or
     * changed. If the RWWString object itself changes or goes out of scope,
     * the pointer value previously returned becomes invalid. While the string
     * is null-terminated, note that its length is still given by the member
     * function length(). That is, it may contain embedded nulls.
     */
    const wchar_t* data() const {
        return data_.c_str();
    }

    /**
     * Returns the underlying \b std::wstring representation of self.
     *
     * @note
     * - This member function is unavailable on RWWString if
     *   \c RW_COPY_ON_WRITE_STRING is defined.
     * - This member function is unavailable on RWCopyOnWriteWString
     */
    std::wstring& std();

    /**
     * Returns the underlying \b std::wstring representation of self.
     *
     * @note
     * - This member function is unavailable on RWWString if
     *   \c RW_COPY_ON_WRITE_STRING is defined.
     * - This member function is unavailable on RWCopyOnWriteWString
     */
    const std::wstring& std() const {
        return data_;
    }

    /**
     * Swaps the contents of \a str with self.
     */
    void swap(RWStandardWString& str);

    /**
     * Returns the index of the first occurrence of the character \a c
     * in self. Returns #RW_NPOS if there is no match.
     */
    size_t first(wchar_t c) const;

#if !defined(RW_DISABLE_DEPRECATED)

    /**
     * @deprecated As of SourcePro 12, use RWWString::first(wchar_t) instead.
     */
    RW_DEPRECATE_FUNC("Use RWStandardWString::first(wchar_t) instead")
    size_t first(wchar_t c, size_t) const;

#endif

    /**
     * Returns the index of the first occurrence in self of any character
     * in \a str. Returns #RW_NPOS if there is no match.
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
    size_t first(const wchar_t* str) const;

    /**
     * Returns the index of the first occurrence in self of any character
     * in \a str. Exactly \a len characters in \a str are checked <i>including
     * any embedded nulls</i> so \a str must point to a buffer containing
     * at least \a len characters. Returns #RW_NPOS if there is no match.
     */
    size_t first(const wchar_t* str, size_t len) const;

    /**
     * Returns a suitable hash value.
     */
    unsigned long hash(caseCompare cmp = exact) const;

    /**
     * Pattern matching. Starting with index \a start, searches for the
     * first occurrence of the null-terminated string \a pat in self and
     * returns the index of the start of the match. Returns #RW_NPOS if
     * there is no such pattern. Case sensitivity is according to \a cmp.
     *
     * @note
     * This function is incompatible with \a pat strings with embedded nulls.
     */
    size_t index(const wchar_t* pat, size_t start = 0, caseCompare cmp = exact) const;

    /**
     * @copydoc index(const RWWString&,size_t,caseCompare)const
     */
    size_t index(const std::wstring& pat, size_t start = 0, caseCompare cmp = exact) const;

    /**
     * Pattern matching. Starting with index \a start, searches for the
     * first occurrence of \a pat in self and returns the index of the
     * start of the match. Returns #RW_NPOS if there is no such pattern.
     * Case sensitivity is according to \a cmp.
     */
    size_t index(const RWStandardWString& pat, size_t start = 0, caseCompare cmp = exact) const;

    /**
     * @copydoc index(const RWWString&,size_t,size_t,caseCompare)const
     */
    inline size_t index(const wchar_t* pat, size_t patlen, size_t start, caseCompare cmp) const;

    /**
     * @copydoc index(const RWWString&,size_t,size_t,caseCompare)const
     */
    inline size_t index(const std::wstring& pat, size_t patlen, size_t start, caseCompare cmp) const;

    /**
     * Pattern matching. Starting with index \a start, searches for the
     * first occurrence of the first \a patlen characters from \a pat in
     * self and returns the index of the start of the match. Returns
     * #RW_NPOS if there is no such pattern. Case sensitivity is according
     * to \a cmp.
     */
    size_t index(const RWStandardWString& pat, size_t patlen, size_t start, caseCompare cmp) const;

#if !defined(RW_DISABLE_DEPRECATED) && defined(RW_WIDE_CHAR_REG_EXPRESSIONS) // not implemented
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
    size_t index(const RWWRExpr& re, size_t i = 0) const;

    /**
     * @deprecated As of SourcePro 12, use RWTRegex::index instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::index instead")
    size_t index(const RWWRExpr& re, size_t* ext, size_t i = 0) const;

    /**
     * @deprecated As of SourcePro 12, use RWTRegex::index instead.
     */
    RW_DEPRECATE_FUNC("Use RWTRegex::index instead")
    size_t index(const wchar_t* re, size_t* ext, size_t i = 0) const;

    RW_RESTORE_DEPRECATED_WARNINGS
#endif

    /**
     * Inserts \a count instances of the character \a c into self at index
     * \a pos. Returns a reference to self.
     */
    RWStandardWString& insert(size_t pos, wchar_t c, size_t count);

    /**
     * Inserts a copy of the null-terminated string \a str into self at
     * index \a pos. Returns a reference to self.
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
    RWStandardWString& insert(size_t pos, const wchar_t* str);

    /**
     * Inserts a copy of the first \a len characters of \a str into self at
     * index \a pos. Returns a reference to self.
     */
    RWStandardWString& insert(size_t pos, const wchar_t* str, size_t len);

    /**
     * @copydoc insert(size_t,const RWWString&)
     */
    RWStandardWString& insert(size_t pos, const std::wstring& str);

    /**
     * Inserts a copy of the string \a str into self at index
     * \a pos. Returns a reference to self.
     */
    RWStandardWString& insert(size_t pos, const RWStandardWString& str);

    /**
     * @copydoc insert(size_t,const RWWString&,size_t)
     */
    RWStandardWString& insert(size_t pos, const std::wstring& str, size_t len);

    /**
     * Inserts the first \a len characters of \a str into self at index \a pos.
     * If \a len is greater than the length of \a str, the entire string
     * is inserted. Returns a reference to self.
     */
    RWStandardWString& insert(size_t pos, const RWStandardWString& str, size_t len);

    /**
     * Returns \c true if it is safe to perform the conversion toAscii()
     * (that is, if all characters of self are US-ASCII characters).
     */
    bool isAscii() const;

    /**
     * Returns \c true if this string has zero length (i.e., the null
     * string).
     */
    bool isNull() const {
        return 0 == data_.length();
    }

    /**
     * Returns the index of the last occurrence of the character \a c
     * in self. Returns #RW_NPOS if there is no match.
     */
    size_t last(wchar_t c) const;

#if !defined(RW_DISABLE_DEPRECATED)

    /**
     * @deprecated As of SourcePro 12, use RWWString::last(wchar_t) instead.
     */
    RW_DEPRECATE_FUNC("Use RWStandardWString::last(wchar_t) instead")
    size_t last(wchar_t c, size_t) const;

#endif

    /**
     * Returns the index of the last occurrence in self of any character
     * in \a str. Returns #RW_NPOS if there is no match.
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
    size_t last(const wchar_t* str) const;

    /**
     * Returns the index of the last occurrence in self of any character
     * in \a str. Exactly \a len characters in \a str are checked <i>including
     * any embedded nulls</i> so \a str must point to a buffer containing
     * at least \a len characters. Returns #RW_NPOS if there is no match.
     */
    size_t last(const wchar_t* str, size_t len) const;

    /**
     * Returns the number of characters in self.
     */
    size_t length() const {
        return data_.length();
    }

    /**
     * @internal
     * Returns the number of characters in self.
     */
    size_t mbLength() const {
        return data_.length();
    }

    /**
     * Prepends \a count copies of character \a c to self. Returns a reference
     * to self.
     */
    RWStandardWString& prepend(wchar_t c, size_t count = 1);

    /**
     * Prepends a copy of the null-terminated character string pointed
     * to by \a str to self. Returns a reference to self.
     *
     * @note
     * This function is incompatible with \a str strings with embedded nulls.
     */
    RWStandardWString& prepend(const wchar_t* str);

    /**
     * Prepends a copy of the character string \a str to self. Exactly
     * \a len characters are copied, <i>including any embedded nulls</i>. Hence,
     * the buffer pointed to by \a str must be at least \a len characters long.
     * Returns a reference to self.
     */
    RWStandardWString& prepend(const wchar_t* str, size_t len);

    /**
     * @copydoc prepend(const RWWString&)
     */
    RWStandardWString& prepend(const std::wstring& str);

    /**
     * Prepends a copy of the string \a str to self. Returns a reference
     * to self.
     */
    RWStandardWString& prepend(const RWStandardWString& str);

    /**
     * @copydoc prepend(const RWWString&,size_t)
     */
    RWStandardWString& prepend(const std::wstring& str, size_t len);

    /**
     * Prepends the first \a len characters of \a str to self. If \a len is
     * greater than the length of \a str, the entire string is prepended.
     * Returns a reference to self.
     */
    RWStandardWString& prepend(const RWStandardWString& str, size_t len);

    // istream input
    /**
     * Reads characters from the input stream \a strm, replacing the previous
     * contents of self, until \c EOF is reached. The input stream is
     * treated as a sequence of wide characters, each of which is read
     * and stored in the RWWString object. Null characters are treated
     * the same as other characters.
     *
     * @note
     * RWWString is designed for use with wide character strings;
     * to manipulate multibyte character sequences, use RWCString.
     */
    std::istream& readFile(std::istream& strm);

    /**
     * Reads characters from the input stream \a strm, replacing the previous
     * contents of self, until a newline (or an \c EOF) is encountered.
     * The newline is removed from the input stream but is not stored.
     * The input stream is treated as a sequence of wide characters,
     * each of which is read and stored in the RWWString object. Null
     * characters are treated the same as other characters. If the
     * \a skipWhite argument is \c true, then whitespace is skipped
     * (using the iostream library manipulator \c ws) before saving
     * characters.
     *
     * @note
     * RWWString is designed for use with wide character strings;
     * to manipulate multibyte character sequences, use RWCString.
     */
    std::istream& readLine(std::istream& strm, bool skipWhite = true);

    /**
     * Reads characters from the input stream \a strm, replacing the previous
     * contents of self, until an \c EOF or null terminator is encountered.
     * The input stream is treated as a sequence of wide characters,
     * each of which is read and stored in the RWWString object.
     *
     * @note
     * RWWString is designed for use with wide character strings;
     * to manipulate multibyte character sequences, use RWCString.
     */
    std::istream& readString(std::istream& strm);

    /**
     * The equivalent of calling:
     *
     * @code
     * RWWString::readToDelim(strm, L'\n');
     * @endcode
     */
    std::istream& readToDelim(std::istream& strm) {
        return readToDelim(strm, L'\n');
    }

    /**
     * Reads characters from the input stream \a strm, replacing the previous
     * contents of self, until an \c EOF or the delimiting character
     * \a delim is encountered. The delimiter is removed from the input
     * stream but is not stored. The input stream is treated as a sequence
     * of wide characters, each of which is read and stored in the RWWString
     * object. Null characters are treated the same as other characters.
     *
     * @note
     * RWWString is designed for use with wide character strings;
     * to manipulate multibyte character sequences, use RWCString.
     */
    std::istream& readToDelim(std::istream& strm, wchar_t delim) {
        return readToDelim(strm, delim, false);
    }

    /**
     * @internal
     * Reads characters from the input stream \a strm, replacing the previous
     * contents of self, until an \c EOF or the delimiting character
     * \a delim is encountered. The delimiter is removed from the input
     * stream but is not stored. The input stream is treated as a sequence
     * of wide characters, each of which is read and stored in the RWWString
     * object. Null characters are treated the same as other characters. If
     * the \a skipWhite argument is \c true, then leading white space is
     * skipped (using the iostream library manipulator \c ws).
     *
     * @note
     * RWWString is designed for use with wide character strings;
     * to manipulate multibyte character sequences, use RWCString.
     */
    std::istream& readToDelim(std::istream& strm, wchar_t delim, bool skipWhite);

    /**
     * Whitespace is skipped before storing characters into a wide string.
     * Characters are then read from the input stream \a strm, replacing
     * previous contents of self, until trailing whitespace or an
     * \c EOF is encountered. The trailing whitespace is left on the input
     * stream. Only US-ASCII whitespace characters are recognized, as defined
     * by the Standard C Library function \c isspace(). The input stream
     * is treated as a sequence of wide characters, each of which is
     * read and stored in the RWWString object.
     *
     * @note
     * RWWString is designed for use with wide character strings;
     * to manipulate multibyte character sequences, use RWCString.
     */
    std::istream& readToken(std::istream& strm);

    /**
     * @internal
     * Reads characters from the input stream \a strm, replacing the previous
     * contents of self, until \c EOF is reached. Null characters are
     * treated the same as other characters.
     */
    std::wistream& readFile(std::wistream& strm);

    /**
     * @internal
     * Reads characters from the input stream \a strm, replacing the previous
     * contents of self, until a newline (or \c EOF) is encountered.
     * The newline is removed from the input stream but is not stored.
     * Null characters are treated the same as other characters. If
     * the \a skipWhite argument is \c true, then leading white space is
     * skipped (using the iostream library manipulator \c ws).
     */
    std::wistream& readLine(std::wistream& strm, bool skipWhite = true);

    /**
     * @internal
     * Reads characters from the input stream \a strm, replacing the previous
     * contents of self, until a null terminator (or \c EOF) is encountered.
     */
    std::wistream& readString(std::wistream& strm);

    /**
     * @internal
     * The equivalent of calling:
     *
     * @code
     * RWWString::readToDelim(strm, L'\n');
     * @endcode
     */
    std::wistream& readToDelim(std::wistream& strm) {
        return readToDelim(strm, strm.widen('\n'));
    }

    /**
     * @internal
     * Reads characters from the input stream \a strm, replacing the previous
     * contents of self, until the delimiting character \a delim (or \c EOF)
     * is encountered. The delimiter is removed from the input
     * stream but is not stored.
     */
    std::wistream& readToDelim(std::wistream& strm, wchar_t delim) {
        return readToDelim(strm, delim, false);
    }

    /**
     * @internal
     * Reads characters from the input stream \a strm, replacing the previous
     * contents of self, until an \c EOF or the delimiting character
     * \a delim is encountered. The delimiter is removed from the input
     * stream but is not stored. The input stream is treated as a sequence
     * of wide characters, each of which is read and stored in the RWWString
     * object. Null characters are treated the same as other characters. If
     * the \a skipWhite argument is \c true, then leading white space is
     * skipped (using the iostream library manipulator \c ws).
     *
     * @note
     * RWWString is designed for use with wide character strings;
     * to manipulate multibyte character sequences, use RWCString.
     */
    std::wistream& readToDelim(std::wistream& strm, wchar_t delim, bool skipWhite);

    /**
     * White space is skipped before reading from \a strm. Characters are
     * then read from the input stream \a strm, replacing previous contents
     * of self, until trailing white space or \c EOF is encountered.
     * The white space is left on the input stream. Null characters
     * are treated the same as other characters. White space is identified
     * by the Standard C Library function \c isspace().
     */
    std::wistream& readToken(std::wistream& strm);

    /**
     * Removes the characters to the end of string, starting at the
     * position \a pos, which must be no greater than length().
     * Returns a reference to self.
     */
    RWStandardWString& remove(size_t pos);

    /**
     * Removes \a len wide characters or to the end of string (whichever
     * comes first) starting at the position \a pos, which must be no
     * greater than length(). Returns a reference to self.
     */
    RWStandardWString& remove(size_t pos, size_t len);

    /**
     * Replaces the substring beginning at \a pos and of length \a len
     * with the null-terminated string \a str.  If the sum of \a pos and
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
    RWStandardWString& replace(size_t pos, size_t len, const wchar_t* str);

    /**
     * @copydoc replace(size_t,size_t,const RWWString&)
     */
    RWStandardWString& replace(size_t pos, size_t len, const std::wstring& str);

    /**
     * Replaces the substring beginning at \a pos and of length \a len
     * with the string \a str.  If the sum of \a pos and \a len is greater
     * than the length of self, the substring replaced is from \a pos to
     * end of string.
     *
     * Returns a reference to self.
     *
     * @throws RWBoundsErr If \a pos is greater than the length of self.
     */
    RWStandardWString& replace(size_t pos, size_t len, const RWStandardWString& str);

    /**
     * Replaces the substring beginning at \a pos and of length \a len
     * with \a strLen characters from \a str.  If the sum of \a pos and \a len
     * is greater than the length of self, the substring replaced is from
     * \a pos to end of string.
     *
     * Returns a reference to self.
     *
     * @throws RWBoundsErr If \a pos is greater than the length of self.
     */
    RWStandardWString& replace(size_t pos, size_t len, const wchar_t* str, size_t strLen);

    /**
     * @copydoc replace(size_t,size_t,const RWWString&,size_t)
     */
    RWStandardWString& replace(size_t pos, size_t len, const std::wstring& str, size_t strLen);

    /**
     * Replaces the substring beginning at \a pos and of length \a len
     * with the first \a strLen characters of string \a str.  If the sum of
     * \a pos and \a len is greater than the length of self, the substring
     * replaced is from \a pos to end of string.  If \a strLen is greater
     * than the length of \a str, all of \a str is used.
     *
     * Returns a reference to self.
     *
     * @throws RWBoundsErr If \a pos is greater than the length of self.
     */
    RWStandardWString& replace(size_t pos, size_t len, const RWStandardWString& str, size_t strLen);

#if !defined(RW_DISABLE_DEPRECATED) && defined(RW_WIDE_CHAR_REG_EXPRESSIONS) // not implemented
    RW_SUPPRESS_DEPRECATED_WARNINGS

    /**
     * @internal
     * @deprecated As of SourcePro 12, use RWTRegex::replace instead.
     */
    RW_DEPRECATED_FUNC("Use RWTRegex::replace instead")
    RWStandardWString& replace(const RWWRExpr& pattern, const wchar_t* replacement  , scopeType scope = one);

    /**
     * @internal
     * @deprecated As of SourcePro 12, use RWTRegex::replace instead.
     */
    RW_DEPRECATED_FUNC("Use RWTRegex::replace instead")
    RWStandardWString&  replace(const RWWRExpr& pattern, const RWStandardWString& replacement, scopeType scope = one);

    RW_RESTORE_DEPRECATED_WARNINGS
#endif

    /**
     * Changes the length of self to \a len characters, adding blanks
     * (i.e., <tt>L' '</tt>) or truncating as necessary.
     */
    void resize(size_t len);

    /**
     * @internal
     * Restores an instance of RWWString from \a vis into which an instance had
     * been persisted using saveOn(RWvostream&).
     */
    void restoreFrom(RWvistream& vis);

    /**
     * @internal
     * Restores an instance of RWWString from \a fil into which an instance had
     * been persisted using saveOn(RWFile&).
     */
    void restoreFrom(RWFile& fil);

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
     * - This member function is unavailable on RWWString if
     *   \c RW_COPY_ON_WRITE_STRING is defined.
     * - This member function is unavailable on RWCopyOnWriteWString
     */
    size_t rindex(const wchar_t* pat, caseCompare cmp) const;

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
     * - This member function is unavailable on RWWString if
     *   \c RW_COPY_ON_WRITE_STRING is defined.
     * - This member function is unavailable on RWCopyOnWriteWString
     */
    size_t rindex(const wchar_t* pat, size_t end = RW_NPOS, caseCompare cmp = exact) const;

    /**
     * @copydoc rindex(const RWWString&,size_t,caseCompare)const
     */
    size_t rindex(const std::wstring& pat, size_t end = RW_NPOS, caseCompare cmp = exact) const;

    /**
     * Pattern matching. Searches for the last occurrence of \a pat in self
     * that occurs before index \a end and returns the index of the
     * start of the match. Returns #RW_NPOS if there is no such pattern.
     * Case sensitivity is according to \a cmp.
     *
     * @note
     * - This member function is unavailable on RWWString if
     *   \c RW_COPY_ON_WRITE_STRING is defined.
     * - This member function is unavailable on RWCopyOnWriteWString
     */
    size_t rindex(const RWStandardWString& pat, size_t end = RW_NPOS, caseCompare cmp = exact) const;

    /**
     * @copydoc rindex(const RWWString&,size_t,size_t,caseCompare)const
     */
    size_t rindex(const wchar_t* pat, size_t patlen, size_t end, caseCompare cmp) const;

    /**
     * @copydoc rindex(const RWWString&,size_t,size_t,caseCompare)const
     */
    size_t rindex(const std::wstring& pat, size_t patlen, size_t end, caseCompare cmp) const;

    /**
     * Pattern matching. Searches for the last occurrence of the first
     * \a patlen characters from \a pat in self that occurs before index \a end
     * and returns the index of the start of the match. Returns #RW_NPOS
     * if there is no such pattern. Case sensitivity is according to \a cmp.
     *
     * @note
     * - This member function is unavailable on RWWString if
     *   \c RW_COPY_ON_WRITE_STRING is defined.
     * - This member function is unavailable on RWCopyOnWriteWString
     */
    size_t rindex(const RWStandardWString& pat, size_t patlen, size_t end, caseCompare cmp) const;

    /**
     * @internal
     * Saves an instance of RWWString to \a vos. The instance can be restored
     * using the member function restoreFrom(RWvistream&).
     */
    void saveOn(RWvostream& vos) const;

    /**
     * @internal
     * Saves an instance of RWWString to \a fil. The instance can be restored
     * using the member function restoreFrom(RWFile&).
     */
    void saveOn(RWFile& fil) const;

    /**
     * Returns an ASCII representation of self. Any non-US-ASCII characters in
     * self simply have the high bits stripped off. Use isAscii() to determine
     * whether this function is safe to use.
     */
    RWStandardCString toAscii() const;

    /**
     * Returns a multibyte representation of self.
     *
     * @note
     * This method does not handle embedded nulls on systems where
     * \c wcstombs() doesn't handle them. Returns an empty string if
     * the wide string cannot be translated (for instance, if the correct
     * locale is not set to convert a specific character).
     */
    RWStandardCString toMultiByte() const;

    /**
     * Returns the result of the constructor
     * RWWString::RWWString(const RWCString&, RWWString::ascii).
     */
    static RWStandardWString fromAscii(const RWStandardCString& str) {
        return RWStandardWString(str, ascii);
    }

    /**
     * Returns the result of the constructor
     * RWWString::RWWString(const RWCString&, RWWString::multiByte).
     */
    static RWStandardWString fromMultiByte(const RWStandardCString& str) {
        return RWStandardWString(str, multiByte);
    }

#if !defined(RW_DISABLE_DEPRECATED)

    /**
     * @internal
     * @deprecated As of SourcePro 12, use toMultiByte() instead.
     */
    RW_DEPRECATE_FUNC("Use RWStandardWString::toMultiByte() instead")
    RWStandardCString toMultiByteReadNulls() const;

#endif

    /**
     * Changes all upper-case letters in self to lower-case. Uses the
     * C library function \c towlower().
     */
    void toLower() {
        toLower(0, length());
    }

    /**
     * Changes all lower-case letters in self to upper-case. Uses the
     * C library function \c towupper().
     */
    void toUpper() {
        toUpper(0, length());
    }

    /**
     * Returns \c true if \c this is lexicographically equal to \a rhs.
     * Otherwise returns \c false. Use member RWWString::collate()
     * or RWWString::strXForm() for locale-sensitive comparisons.
     */
    bool operator==(const RWStandardWString& rhs) const {
        return std() == rhs.std();
    }

    /**
     * The equivalent of calling:
     *
     * @code
     * !(*this == rhs)
     * @endcode
     */
    bool operator!=(const RWStandardWString& rhs) const {
        return !(*this == rhs);
    }

    /**
     * Returns \c true if \c this lexicographically precedes \a rhs.
     * Otherwise returns \c false. Use member RWWString::collate() or
     * RWWString::strXForm() for locale-sensitive comparisons.
     */
    bool operator< (const RWStandardWString& rhs) const {
        return std() < rhs.std();
    }

    /**
     * The equivalent of calling:
     *
     * @code
     * !(rhs < *this)
     * @endcode
     */
    bool operator> (const RWStandardWString& rhs) const {
        return (rhs < *this);
    }

    /**
     * @relates RWWString
     * The equivalent of calling:
     *
     * @code
     * !(rhs < *this)
     * @endcode
     */
    bool operator<=(const RWStandardWString& rhs) const {
        return !(rhs < *this);
    }

    /**
     * @relates RWWString
     * The equivalent of calling:
     *
     * @code
     * !(*this < rhs)
     * @endcode
     */
    bool operator>=(const RWStandardWString& rhs) const {
        return !(*this < rhs);
    }

    /**
     * Returns the hash value of \a str as returned by
     * hash(caseCompare) const, where #caseCompare is RWWString::exact.
     */
    static unsigned
    hash(const RWStandardWString& wstr) {
        return unsigned(wstr.hash());
    }

#if !defined(RW_DISABLE_DEPRECATED)

    /**
     * @deprecated As of SourcePro 12.
     *
     * It has no effect on an RWWString instance and
     * may be removed from a future release.
     */
    RW_DEPRECATE_FUNC("")
    static size_t initialCapacity(size_t capacity = 15);

    /**
     * @deprecated As of SourcePro 12.
     *
     * It has no effect on an RWWString instance and
     * may be removed from a future release.
     */
    RW_DEPRECATE_FUNC("")
    static size_t maxWaste(size_t maxwaste = 16);

    /**
     * @deprecated As of SourcePro 12.
     *
     * It has no effect on an RWWString instance and
     * may be removed from a future release.
     */
    RW_DEPRECATE_FUNC("")
    static size_t resizeIncrement(size_t increment = 16);

    /**
     * @internal
     * @deprecated As of SourcePro 12.
     *
     * It has no effect on an RWWString instance and
     * may be removed from a future release.
     */
    RW_DEPRECATE_FUNC("")
    static size_t getInitialCapacity();

    /**
     * @internal
     * @deprecated As of SourcePro 12.
     *
     * It has no effect on an RWWString instance and
     * may be removed from a future release.
     */
    RW_DEPRECATE_FUNC("")
    static size_t getResizeIncrement();

    /**
     * @internal
     * @deprecated As of SourcePro 12.
     *
     * It has no effect on an RWWString instance and
     * may be removed from a future release.
     */
    RW_DEPRECATE_FUNC("")
    static size_t getMaxWaste();

    /**
     * @internal
     * @deprecated As of SourcePro 12.
     *
     * It has no effect on an RWWString instance and
     * may be removed from a future release.
     */
    RW_DEPRECATE_FUNC("")
    static size_t adjustCapacity(size_t nc);

#endif

protected:
    /**
     * @internal
     * Top level method for outlined comparisons #4884
     */
    int compareToSpecial(const wchar_t* s, size_t slen, caseCompare cmp) const;

    /**
     * @internal
     * Top level outlined searching method #4884
     */
    size_t indexSpecial(const wchar_t* pat, size_t patlen, size_t i, caseCompare cmp) const;

#if !defined(RW_DISABLE_DEPRECATED)

    /**
     * @internal
     * @deprecated As of SourcePro 12.
     * This function is no longer used.
     */
    RW_DEPRECATE_FUNC("")
    size_t indexIgnoreCase(const wchar_t* pat, size_t patlen, size_t i) const;

#endif

    /**
     * @internal
     * Top level outlined reverse searching method #4884
     */
    size_t rindexSpecial(const wchar_t* pat, size_t patlen, size_t i, caseCompare cmp) const;

#if !defined(RW_DISABLE_DEPRECATED)

    /**
     * @internal
     * @deprecated As of SourcePro 12.
     * This function is no longer used.
     */
    RW_DEPRECATE_FUNC("")
    size_t rindexIgnoreCase(const wchar_t* pat, size_t patlen, size_t i) const;

#endif

    /**
     * @internal
     * Used by US-ASCII conversion ctors.
     */
    void initChars(const char* s, size_t N);

    /**
     * @internal
     * Used by multibyte conversion ctors.
     */
    inline void initMB(const char* s, size_t N);

#if !defined(RW_DISABLE_DEPRECATED)

    /**
     * @internal
     * @deprecated As of SourcePro 12.
     * This function is no longer used.
     */
    RW_DEPRECATE_FUNC("")
    inline void initMBReadNulls(const char* s, size_t N);

#endif

    /**
     * @internal
     * Changes all upper-case in the substring beginning at \a pos with
     * length \a len in self to lower-case, using the
     * Standard C library function \c towlower().
     */
    void toLower(size_t pos, size_t len);

    /**
     * @internal
     * Changes all lower-case in the substring beginning at \a pos with
     * length \a len in self to upper-case, using the
     * Standard C library function \c towupper().
     */
    void toUpper(size_t pos, size_t len);

    /**
     * @internal
     * Verifies that the index \a i is within the bounds of the substring.
     *
     * @throw RWBoundsErr if the index is out of range.
     */
    void assertElement(std::wstring::size_type i) const;  // Verifies i is valid index

    friend class RW_TOOLS_SYMBOLIC RWStandardWSubString;
    friend class RW_TOOLS_SYMBOLIC RWStandardWConstSubString;
    friend class RW_TOOLS_GLOBAL RWWTokenizer;

    /**
     * Concatenates \a lhs and \a rhs and returns the resulting
     * string.
     */
    friend RWStandardWString
    operator+(const RWStandardWString& lhs, const RWStandardWString& rhs);

    /**
     * Concatenates \a lhs and the null-terminated string \a rhs and returns
     * the resulting string.
     *
     * @note
     * This function is incompatible with \a rhs strings with embedded nulls.
     */
    friend RWStandardWString
    operator+(const RWStandardWString& lhs, const wchar_t* rhs);

    /**
     * Concatenates the null-terminated string \a lhs and \a rhs and returns
     * the resulting string.
     *
     * @note
     * This function is incompatible with \a lhs strings with embedded nulls.
     */
    friend RWStandardWString
    operator+(const wchar_t* lhs, const RWStandardWString& rhs);

    /**
     * @copydoc operator+(const RWWString&, const RWWString&)
     */
    friend RWStandardWString
    operator+(const std::wstring& lhs, const RWStandardWString& rhs);

    /**
     * @copydoc operator+(const RWWString&, const RWWString&)
     */
    friend RWStandardWString
    operator+(const RWStandardWString& lhs, const std::wstring& rhs);

private:

    /**
     * @internal
     * Converts the multibyte string \a str into a wide character string
     * and returns it. The length of the resulting wide character string
     * is returned via the out parameter \a wideLen.
     */
    wchar_t* multiByteToWide(const char* str, size_t len, size_t& wideLen) const;

    /**
     * @internal
     * Converts the wide character string \a str into a multibyte character
     * string and returns it. The length (in bytes) of the resulting multibyte
     * character string is returned via the out parameter \a mbLen.
     */
    char*    wideToMultiByte(const wchar_t* str, size_t len, size_t& mbLen) const;

    std::wstring data_;
};



//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                               Inlines                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#if !defined(__GNUG__) || __GNUG__ < 3 || \
    (defined(RW_STDLIB_VER) && RW_STDLIB_VER >= 0x02000000)

inline RWStandardWString::RWStandardWString(const RWStandardWSubString& s)
{
    if (!s.isNull()) {
        data_.assign(s.str_->data_, s.begin_, s.extent_);
    }
}

inline RWStandardWString::RWStandardWString(const RWStandardWConstSubString& s)
{
    if (!s.isNull()) {
        data_.assign(s.str_->data_, s.begin_, s.extent_);
    }
}

#else // __GNUG__ >= 3 || !defined(RW_RWV[2/3]X_STDLIB)

inline RWStandardWString::RWStandardWString(const RWStandardWSubString& s)
{
    if (!s.isNull()) {
        data_ = std::wstring(s.str_->data_, s.begin_, s.extent_);
    }
}

inline RWStandardWString::RWStandardWString(const RWStandardWConstSubString& s)
{
    if (!s.isNull()) {
        data_ = std::wstring(s.str_->data_, s.begin_, s.extent_);
    }
}

#endif  // !defined(__GNUG__) || __GNUG__ < 3 || defined(RW_RWV[2/3]X_STDLIB)

//Assignment
inline RWStandardWString&
RWStandardWString::assign(const RWStandardWString& str)
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

inline RWStandardWString&
RWStandardWString::assign(const std::wstring& str)
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

inline RWStandardWString&
RWStandardWString::assign(const SubString& str)
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

inline RWStandardWString&
RWStandardWString::assign(const ConstSubString& str)
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

inline RWStandardWString&
RWStandardWString::assign(const wchar_t* str)
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

inline RWStandardWString&
RWStandardWString::assign(const wchar_t* str, size_t len)
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

inline RWStandardWString&
RWStandardWString::assign(wchar_t c, size_t count)
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

//Assignment

inline RWStandardWString&
RWStandardWString::operator=(wchar_t c)
{
    assign(c);
    return *this;
}

inline RWStandardWString&
RWStandardWString::operator=(const wchar_t* s)
{
    assign(s);
    return *this;
}

inline RWStandardWString&
RWStandardWString::operator=(const std::wstring& s)
{
    assign(s);
    return *this;
}

RW_SUPPRESS_OP_ASSIGN_SELF_CHECK_WARNING
inline RWStandardWString&
RWStandardWString::operator=(const RWStandardWString& s)
{
    assign(s);
    return *this;
}
RW_RESTORE_OP_ASSIGN_SELF_CHECK_WARNING

inline RWStandardWString&
RWStandardWString::operator=(const RWStandardWString::SubString& substr)
{
    assign(substr);
    return *this;
}

inline RWStandardWString&
RWStandardWString::operator=(const RWStandardWString::ConstSubString& substr)
{
    assign(substr);
    return *this;
}

#if !defined(RW_NO_RVALUE_REFERENCES)

inline
RWStandardWString::RWStandardWString(RWStandardWString && str)
    : data_()
{
    str.swap(*this);
}

inline RWStandardWString&
RWStandardWString::operator=(RWStandardWString && str)
{
    if (&str != this) {
        str.swap(*this);
    }

    return *this;
}

#endif // !RW_NO_RVALUE_REFERENCES

inline wchar_t&
RWStandardWString::operator()(size_t i)
{
#ifdef RWBOUNDS_CHECK
    assertElement(i);
#endif
    return data_[i];
}

inline wchar_t
RWStandardWString::operator()(size_t i) const
{
#ifdef RWBOUNDS_CHECK
    assertElement(i);
#endif
    return data()[i];
}

inline RWStandardWSubString
RWStandardWString::operator()(size_t start, size_t len)
{
    return RWStandardWSubString(*this, start, len);
}


// Here all integer types are provided with an exact match
// to avoid ambiguities with the built in wchar_t[] operator
// All operator[] functions are with bounds checking
inline wchar_t&
RWStandardWString::operator[](short i)
{
    assertElement(std::wstring::size_type(i));
    return data_[std::wstring::size_type(i)];
}

inline wchar_t
RWStandardWString::operator[](short i) const
{
    assertElement(std::wstring::size_type(i));
    return data_[std::wstring::size_type(i)];
}

inline wchar_t&
RWStandardWString::operator[](int i)
{
    assertElement(std::wstring::size_type(i));
    return data_[std::wstring::size_type(i)];
}

inline wchar_t
RWStandardWString::operator[](int i) const
{
    assertElement(std::wstring::size_type(i));
    return data_[std::wstring::size_type(i)];
}

inline wchar_t&
RWStandardWString::operator[](long i)
{
    assertElement(std::wstring::size_type(i));
    return data_[std::wstring::size_type(i)];
}

inline wchar_t
RWStandardWString::operator[](long i) const
{
    assertElement(std::wstring::size_type(i));
    return data_[std::wstring::size_type(i)];
}

inline wchar_t&
RWStandardWString::operator[](unsigned short i)
{
    assertElement(std::wstring::size_type(i));
    return data_[std::wstring::size_type(i)];
}

inline wchar_t
RWStandardWString::operator[](unsigned short i) const
{
    assertElement(std::wstring::size_type(i));
    return data_[std::wstring::size_type(i)];
}

inline wchar_t&
RWStandardWString::operator[](unsigned int i)
{
    assertElement(std::wstring::size_type(i));
    return data_[std::wstring::size_type(i)];
}

inline wchar_t
RWStandardWString::operator[](unsigned int i) const
{
    assertElement(std::wstring::size_type(i));
    return data_[std::wstring::size_type(i)];
}

inline wchar_t&
RWStandardWString::operator[](unsigned long i)
{
    assertElement(std::wstring::size_type(i));
    return data_[std::wstring::size_type(i)];
}

inline wchar_t
RWStandardWString::operator[](unsigned long i) const
{
    assertElement(std::wstring::size_type(i));
    return data_[std::wstring::size_type(i)];
}

#if !defined(RW_NO_LONG_LONG)
inline wchar_t&
RWStandardWString::operator[](unsigned long long i)
{
    assertElement(std::wstring::size_type(i));
    return data_[std::wstring::size_type(i)];
}

inline wchar_t
RWStandardWString::operator[](unsigned long long i) const
{
    assertElement(std::wstring::size_type(i));
    return data_[std::wstring::size_type(i)];
}

inline wchar_t&
RWStandardWString::operator[](long long i)
{
    assertElement(std::wstring::size_type(i));
    return data_[std::wstring::size_type(i)];
}

inline wchar_t
RWStandardWString::operator[](long long i) const
{
    assertElement(std::wstring::size_type(i));
    return data_[std::wstring::size_type(i)];
}
#endif

inline RWStandardWConstSubString
RWStandardWString::operator()(size_t start, size_t len) const
{
    return RWStandardWConstSubString(*this, start, len);
}


// IO functions (istream)
inline std::istream&
RWStandardWString::readLine(std::istream& is, bool skipWhite)
{
    if (skipWhite) {
        std::ws(is);
    }
    return readToDelim(is, RW_STATIC_CAST(wchar_t, '\n'));
}

inline std::istream&
RWStandardWString::readString(std::istream& is)
{
    return readToDelim(is, RW_STATIC_CAST(wchar_t, '\0'));
}

// IO functions (wistream)
inline std::wistream&
RWStandardWString::readLine(std::wistream& is, bool skipWhite)
{
    if (skipWhite) {
        std::ws(is);
    }
    return readToDelim(is, RW_STATIC_CAST(wchar_t, '\n'));
}

inline std::wistream&
RWStandardWString::readString(std::wistream& is)
{
    return readToDelim(is, RW_STATIC_CAST(wchar_t, '\0'));
}

inline std::wistream&
RWStandardWString::readToken(std::wistream& is)
{
    return is >> data_;
}

//Other Non-static member functions
inline RWStandardWString&
RWStandardWString::append(wchar_t c, size_t N)
{
    data_.append(N, c);
    return *this;
}

inline RWStandardWString&
RWStandardWString::append(const wchar_t* s)
{
    data_.append(s);
    return *this;
}

inline RWStandardWString&
RWStandardWString::append(const wchar_t* s, size_t N)
{
    data_.append(s, N);
    return *this;
}

inline RWStandardWString&
RWStandardWString::append(const std::wstring& str)
{
    data_.append(str);
    return *this;
}

inline RWStandardWString&
RWStandardWString::append(const std::wstring& str, size_t N)
{
    data_.append(str, 0, N);
    return *this;
}

inline RWStandardWString&
RWStandardWString::append(const RWStandardWString& str)
{
    data_.append(str.std());
    return *this;
}

inline RWStandardWString&
RWStandardWString::append(const RWStandardWString& str, size_t N)
{
    data_.append(str.std(), 0, N);
    return *this;
}

inline size_t
RWStandardWString::capacity() const
{
    return data_.capacity();
}

inline size_t
RWStandardWString::capacity(size_t i)
{
    data_.reserve(i);
    return data_.capacity();
}

inline int
RWStandardWString::collate(const wchar_t* s) const
{
    return wcscoll(data_.c_str(), s);
}

inline int
RWStandardWString::collate(const std::wstring& str) const
{
    return wcscoll(data_.c_str(), str.data());
}

inline int
RWStandardWString::collate(const RWStandardWString& str) const
{
    return wcscoll(data_.c_str(), str.data());
}

inline int
RWStandardWString::compareTo(const wchar_t* s, caseCompare cmp) const
{
    RW_PRECONDITION2(s != rwnil, "RWStandardWString::compareTo(const wchar_t* s, caseCompare cmp) const: nil pointer");

    if (cmp == exact) {
        return data_.compare(s);
    }
    else {
        return compareToSpecial(s, wcslen(s), cmp);
    }
}

inline int
RWStandardWString::compareTo(const std::wstring& str, caseCompare cmp) const
{
    if (cmp == exact) {
        return data_.compare(str);
    }
    else {
        return compareToSpecial(str.data(), str.length(), cmp);
    }
}

inline int
RWStandardWString::compareTo(const RWStandardWString& str, caseCompare cmp) const
{
    if (cmp == exact) {
        return data_.compare(str.std());
    }
    else {
        return compareToSpecial(str.data(), str.length(), cmp);
    }
}

inline bool
RWStandardWString::contains(const wchar_t* pat, caseCompare cmp) const
{
    return index(pat, wcslen(pat), size_t(0), cmp) != RW_NPOS;
}

inline bool
RWStandardWString::contains(const std::wstring& pat, caseCompare cmp) const
{
    return index(pat, pat.length(), size_t(0), cmp) != RW_NPOS;
}

inline bool
RWStandardWString::contains(const RWStandardWString& pat, caseCompare cmp) const
{
    return index(pat.std(), pat.length(), size_t(0), cmp) != RW_NPOS;
}

#if !defined(RW_DISABLE_DEPRECATED)

inline RWStandardWString
RWStandardWString::copy() const
{
    return RWStandardWString(data_);
}

// size_t a dummy argument, it flags to ignore embedded nulls
inline size_t
RWStandardWString::first(wchar_t c, size_t) const
{
    return first(c);
}

#endif

inline size_t
RWStandardWString::first(wchar_t c) const
{
    return data_.find_first_of(c);
}

// ignore embedded nulls
inline size_t
RWStandardWString::first(const wchar_t* s, size_t N) const
{
    return data_.find_first_of(std::wstring(s, N));
}

inline size_t
RWStandardWString::first(const wchar_t* s) const
{
    return first(s, wcslen(s));
}

inline size_t
RWStandardWString::index(const wchar_t* pat, size_t i, caseCompare cmp) const
{

    if (cmp == exact) {
        return data_.find(pat, i, wcslen(pat));
    }
    else {
        return indexSpecial(pat, wcslen(pat), i, cmp);
    }
}

inline size_t
RWStandardWString::index(const wchar_t* pat, size_t patlen, size_t i, caseCompare cmp) const
{
    if (cmp == exact) {
        return data_.find(pat, i, patlen);
    }
    else {
        return indexSpecial(pat, patlen, i, cmp);
    }
}

inline size_t
RWStandardWString::index(const std::wstring& pat, size_t i, caseCompare cmp) const
{
    if (cmp == exact) {
        return data_.find(pat, i);
    }
    else {
        return indexSpecial(pat.data(), pat.length(), i, cmp);
    }
}

inline size_t
RWStandardWString::index(const std::wstring& pat, size_t patlen, size_t i, caseCompare cmp) const
{
    if (cmp == exact) {
        return data_.find(std::wstring(pat, 0, patlen), i);
    }
    else {
        return indexSpecial(pat.data(), patlen, i, cmp);
    }
}

inline size_t
RWStandardWString::index(const RWStandardWString& pat, size_t i, caseCompare cmp) const
{
    if (cmp == exact) {
        return data_.find(pat.std(), i);
    }
    else {
        return indexSpecial(pat.data(), pat.length(), i, cmp);
    }
}

inline size_t
RWStandardWString::index(const RWStandardWString& pat, size_t patlen, size_t i, caseCompare cmp) const
{
    if (cmp == exact) {
        return data_.find(std::wstring(pat.std(), 0, patlen), i);
    }
    else {
        return indexSpecial(pat.data(), patlen, i, cmp);
    }
}

inline RWStandardWString&
RWStandardWString::insert(size_t pos, wchar_t c, size_t extent)
{
    data_.insert(pos, extent, c);
    return *this;
}
inline RWStandardWString&
RWStandardWString::insert(size_t pos, const wchar_t* s)
{
    data_.insert(pos, s);
    return *this;
}

inline RWStandardWString&
RWStandardWString::insert(size_t pos, const wchar_t* s, size_t extent)
{
    data_.insert(pos, s, extent);
    return *this;
}

inline RWStandardWString&
RWStandardWString::insert(size_t pos, const std::wstring& str)
{
    data_.insert(pos, str);
    return *this;
}
inline RWStandardWString&
RWStandardWString::insert(size_t pos, const std::wstring& str, size_t extent)
{
    data_.insert(pos, str, 0, extent);
    return *this;
}

inline RWStandardWString&
RWStandardWString::insert(size_t pos, const RWStandardWString& str)
{
    data_.insert(pos, str.std());
    return *this;
}
inline RWStandardWString&
RWStandardWString::insert(size_t pos, const RWStandardWString& str, size_t extent)
{
    data_.insert(pos, str.std(), 0, extent);
    return *this;
}

inline size_t
RWStandardWString::last(const wchar_t* s) const
{
    return data_.find_last_of(s);
}

#if !defined(RW_DISABLE_DEPRECATED)

// size_t a dummy argument, it flags to ignore embedded nulls
inline size_t
RWStandardWString::last(wchar_t c, size_t) const
{
    return last(c);
}

#endif

inline size_t
RWStandardWString::last(wchar_t c) const
{
    return data_.find_last_of(c);
}

inline size_t
RWStandardWString::last(const wchar_t* s, size_t N) const
{
    return data_.find_last_of(std::wstring(s, N));
}

inline RWStandardWString&
RWStandardWString::prepend(wchar_t c, size_t N)
{
    data_.insert(size_t(0), N, c);
    return *this;
}

inline RWStandardWString&
RWStandardWString::prepend(const wchar_t* s)
{
    data_.insert(0, s);
    return *this;
}

inline RWStandardWString&
RWStandardWString::prepend(const wchar_t* s, size_t N)
{
    data_.insert(0, s, N);
    return *this;
}

inline RWStandardWString&
RWStandardWString::prepend(const std::wstring& str)
{
    data_.insert(0, str);
    return *this;
}

inline RWStandardWString&
RWStandardWString::prepend(const std::wstring& str, size_t N)
{
    data_.insert(0, str, 0, N);
    return *this;
}

inline RWStandardWString&
RWStandardWString::prepend(const RWStandardWString& str)
{
    data_.insert(0, str.std());
    return *this;
}

inline RWStandardWString&
RWStandardWString::prepend(const RWStandardWString& str, size_t N)
{
    data_.insert(0, str.std(), 0, N);
    return *this;
}

inline RWStandardWString&
RWStandardWString::remove(size_t pos)
{
    data_.erase(pos, data_.length() - pos);
    return *this;
}

inline RWStandardWString&
RWStandardWString::remove(size_t pos, size_t N)
{
    data_.erase(pos, N);
    return *this;
}

inline RWStandardWString&
RWStandardWString::replace(size_t pos, size_t extent, const wchar_t* s, size_t N)
{
#if (_STLPORT_VERSION == 0x452)
    // STLport 4.5.2 does not properly handle replacement from
    // overlapping data regions. we make a copy of the source
    // string if the source data overlaps part of the destination.

    const wchar_t* const beg_dst = data() + pos;
    const wchar_t* const end_dst = data() + length();
    if (!extent || s + N < beg_dst || end_dst < s) {
        data_.replace(pos, extent, s, N);
    }
    else {
        const std::wstring str(s, N);
        data_.replace(pos, extent, str, 0, N);
    }
#else
    data_.replace(pos, extent, s, N);
#endif
    return *this;
}

inline RWStandardWString&
RWStandardWString::replace(size_t pos, size_t extent, const wchar_t* s)
{
    replace(pos, extent, s, wcslen(s));
    return *this;
}

inline RWStandardWString&
RWStandardWString::replace(size_t pos, size_t extent, const std::wstring& str)
{
    replace(pos, extent, str.c_str(), str.length());
    return *this;
}

inline RWStandardWString&
RWStandardWString::replace(size_t pos, size_t extent, const std::wstring& str, size_t N)
{
    replace(pos, extent, str.c_str(), N);
    return *this;
}

inline RWStandardWString&
RWStandardWString::replace(size_t pos, size_t extent, const RWStandardWString& str)
{
    replace(pos, extent, str.data(), str.length());
    return *this;
}

inline RWStandardWString&
RWStandardWString::replace(size_t pos, size_t extent, const RWStandardWString& str, size_t N)
{
    replace(pos, extent, str.data(), N);
    return *this;
}

inline void
RWStandardWString::resize(size_t N)
{
    data_.resize(N, RW_STATIC_CAST(wchar_t, ' '));
}

inline size_t
RWStandardWString::rindex(const wchar_t* pat, caseCompare cmp) const
{
    if (cmp == exact) {
        return data_.rfind(pat, RW_NPOS, wcslen(pat));
    }
    else {
        return rindexSpecial(pat, wcslen(pat), RW_NPOS, cmp);
    }
}

inline size_t
RWStandardWString::rindex(const wchar_t* pat, size_t i, caseCompare cmp) const
{
    if (cmp == exact) {
        return data_.rfind(pat, i, wcslen(pat));
    }
    else {
        return rindexSpecial(pat, wcslen(pat), i, cmp);
    }
}

inline size_t
RWStandardWString::rindex(const wchar_t* pat, size_t patlen, size_t i, caseCompare cmp) const
{
    if (cmp == exact) {
        return data_.rfind(pat, i, patlen);
    }
    else {
        return rindexSpecial(pat, patlen, i, cmp);
    }
}

inline size_t
RWStandardWString::rindex(const std::wstring& pat, size_t i, caseCompare cmp) const
{
    if (cmp == exact) {
        return data_.rfind(pat, i);
    }
    else {
        return rindexSpecial(pat.data(), pat.length(), i, cmp);
    }
}

inline size_t
RWStandardWString::rindex(const std::wstring& pat, size_t patlen, size_t i, caseCompare cmp) const
{
    if (cmp == exact) {
        return data_.rfind(pat.data(), i, patlen);
    }
    else {
        return rindexSpecial(pat.data(), patlen, i, cmp);
    }
}

inline size_t
RWStandardWString::rindex(const RWStandardWString& pat, size_t i, caseCompare cmp) const
{
    if (cmp == exact) {
        return data_.rfind(pat.std(), i);
    }
    else {
        return rindexSpecial(pat.data(), pat.length(), i, cmp);
    }
}

inline size_t
RWStandardWString::rindex(const RWStandardWString& pat, size_t patlen, size_t i, caseCompare cmp) const
{
    if (cmp == exact) {
        return data_.rfind(pat.std(), i);
    }
    else {
        return rindexSpecial(pat.data(), patlen, i, cmp);
    }
}

inline void
RWStandardWString::swap(RWStandardWString& str)
{
    data_.swap(str.data_);
}

inline void
RWStandardWString::initMB(const char* s, size_t N)
{
    size_t len   = 0;
    size_t bytes = RWStandardCString::byteCount(s, N);
    RWTScopedPointer<wchar_t[]> ws((bytes != RW_NPOS) ? multiByteToWide(s, bytes, len) : 0);

    // If no error occurred then create the string
    if (ws) {
        data_ = std::wstring(ws.get(), len);
    }
    // Otherwise, there was an error: create an empty string
    else {
        data_ = std::wstring();
    }
}

#if !defined(RW_DISABLE_DEPRECATED)

inline void
RWStandardWString::initMBReadNulls(const char* cs, size_t N)
{
    initMB(cs, N);
}

#endif


//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                       Related global functions                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#if !defined(RW_DISABLE_DEPRECATED)

/**
 * @relates RWWString
 * @deprecated As of SourcePro 12, use strXForm(const RWWString&) instead.
 */
RW_DEPRECATE_FUNC("Use strXForm(const RWWString&) instead")
RW_TOOLS_SYMBOLIC RWStandardWString strXForm(const std::wstring& str);

#endif

/**
 * @relates RWWString
 * Returns the result of applying \c wsxfrm() to \a str
 * to allow quicker collation than RWWString::collate(). Provided
 * only on platforms that provide \c wsxfrm().
 *
 * @note
 * This function is incompatible with \a str strings with
 * embedded nulls.
 */
RW_TOOLS_SYMBOLIC RWStandardWString strXForm(const RWStandardWString& str);

/**
 * @relates RWWString
 * Returns a copy of \a str with all upper-case characters replaced
 * with lower-case characters. This function is the equivalent of
 * calling:
 *
 * @code
 * RWWString tmp(str);
 * tmp.toLower();
 * @endcode
 */
inline RWStandardWString
toLower(const RWStandardWString& str)
{
    RWStandardWString temp(str);
    temp.toLower();
    return temp;
}

/**
 * @relates RWWString
 * Returns a copy of \a str with all lower-case characters replaced
 * with upper-case characters. This function is the equivalent of
 * calling:
 *
 * @code
 * RWWString tmp(str);
 * tmp.toUpper();
 * @endcode
 */
inline RWStandardWString
toUpper(const RWStandardWString& str)
{
    RWStandardWString temp(str);
    temp.toUpper();
    return temp;
}


//
//    GLOBAL operators
//

/**
 * @relates RWWString
 * Concatenates \a lhs and \a rhs and returns the resulting
 * string.
 */
RWStandardWString
operator+(const RWStandardWString& lhs, const RWStandardWString& rhs);

/**
 * @relates RWWString
 * @copydoc operator+(const RWWString&,const RWWString&)
 */
RWStandardWString
operator+(const RWStandardWString& lhs,  const RWStandardWSubString& rhs);

/**
 * @relates RWWString
 * @copydoc operator+(const RWWString&,const RWWString&)
 */
RWStandardWString
operator+(const RWStandardWSubString& lhs, const RWStandardWString& rhs);

/**
 * @relates RWWString
 * @copydoc operator+(const RWWString&,const RWWString&)
 */
RWStandardWString
operator+(const RWStandardWString& lhs,  const RWStandardWConstSubString& rhs);

/**
 * @relates RWWString
 * @copydoc operator+(const RWWString&,const RWWString&)
 */
RWStandardWString
operator+(const RWStandardWConstSubString& lhs, const RWStandardWString& rhs);

/**
 * @relates RWWString
 * @copydoc operator+(const RWWString&,const RWWString&)
 */
RWStandardWString
operator+(const RWStandardWString& lhs,  const std::wstring& rhs);

/**
 * @relates RWWString
 * @copydoc operator+(const RWWString&,const RWWString&)
 */
RWStandardWString
operator+(const std::wstring& lhs, const RWStandardWString& rhs);

/**
 * @relates RWWString
 * Concatenates \a lhs and the null-terminated string \a rhs and returns
 * the resulting string.
 *
 * @note
 * This function is incompatible with \a rhs strings with embedded nulls.
 */
RWStandardWString
operator+(const RWStandardWString& lhs,  const wchar_t* rhs);

/**
 * @relates RWWString
 * Concatenates the null-terminated string \a lhs and \a rhs and returns
 * the resulting string.
 *
 * @note
 * This function is incompatible with \a lhs strings with embedded nulls.
 */
RWStandardWString
operator+(const wchar_t* lhs, const RWStandardWString& rhs);

/**
 * @relates RWWString
 * @copydoc operator+(const RWWString&,const RWWString&)
 */
RWStandardWString
operator+(const RWStandardWSubString& lhs, const RWStandardWSubString& rhs);

/**
 * @relates RWWString
 * @copydoc operator+(const RWWString&,const RWWString&)
 */
RWStandardWString
operator+(const RWStandardWSubString& lhs,  const RWStandardWConstSubString& rhs);

/**
 * @relates RWWString
 * @copydoc operator+(const RWWString&,const RWWString&)
 */
RWStandardWString
operator+(const RWStandardWConstSubString& lhs, const RWStandardWSubString& rhs);

/**
 * @relates RWWString
 * @copydoc operator+(const RWWString&,const RWWString&)
 */
RWStandardWString
operator+(const RWStandardWSubString& lhs,  const std::wstring& rhs);

/**
 * @relates RWWString
 * @copydoc operator+(const RWWString&,const RWWString&)
 */
RWStandardWString
operator+(const std::wstring& lhs, const RWStandardWSubString& rhs);

/**
 * @relates RWWString
 * Concatenates \a lhs and the null-terminated string \a rhs and returns
 * the resulting string.
 *
 * @note
 * This function is incompatible with \a rhs strings with embedded nulls.
 */
RWStandardWString
operator+(const RWStandardWSubString& lhs,  const wchar_t* rhs);

/**
 * @relates RWWString
 * Concatenates the null-terminated string \a lhs and \a rhs and returns
 * the resulting string.
 *
 * @note
 * This function is incompatible with \a lhs strings with embedded nulls.
 */
RWStandardWString
operator+(const wchar_t* lhs, const RWStandardWSubString& rhs);

/**
 * @relates RWWString
 * @copydoc operator+(const RWWString&,const RWWString&)
 */
RWStandardWString
operator+(const RWStandardWConstSubString& lhs, const RWStandardWConstSubString& rhs);

/**
 * @relates RWWString
 * @copydoc operator+(const RWWString&,const RWWString&)
 */
RWStandardWString
operator+(const RWStandardWConstSubString& lhs,  const std::wstring& rhs);

/**
 * @relates RWWString
 * @copydoc operator+(const RWWString&,const RWWString&)
 */
RWStandardWString
operator+(const std::wstring& lhs, const RWStandardWConstSubString& rhs);

/**
 * @relates RWWString
 * Concatenates \a lhs and the null-terminated string \a rhs and returns
 * the resulting string.
 *
 * @note
 * This function is incompatible with \a rhs strings with embedded nulls.
 */
RWStandardWString
operator+(const RWStandardWConstSubString& lhs,  const wchar_t* rhs);

/**
 * @relates RWWString
 * Concatenates the null-terminated string \a lhs and \a rhs and returns
 * the resulting string.
 *
 * @note
 * This function is incompatible with \a lhs strings with embedded nulls.
 */
RWStandardWString
operator+(const wchar_t* lhs, const RWStandardWConstSubString& rhs);

/***********************************************************
 *       istream and wistream shift in operators           *
 ***********************************************************/

/**
 * @relates RWWString
 *
 * Outputs an RWWString on ostream \a os as a sequence of bytes.
 * This is done literally from the byte content of the characters.
 */
RW_TOOLS_SYMBOLIC std::ostream&
operator<<(std::ostream& os, const RWStandardWString& s);


/**
 * @relates RWWString
 *
 * Calls str.readToken(s). That is, a token is read from the input
 * stream \a s.
 */
inline std::istream&
operator>>(std::istream& s, RWStandardWString& str)
{
    return str.readToken(s);
}

inline std::ostream&
operator<<(std::ostream& os, const RWStandardWSubString& str)
{
    os << RWStandardWString(str);
    return os;
}

inline std::ostream&
operator<<(std::ostream& os, const RWStandardWConstSubString& str)
{
    os << RWStandardWString(str);
    return os;
}

inline std::wistream&
operator>>(std::wistream& is, RWStandardWString& wstr)
{
    return wstr.readToken(is);
}

inline std::wostream&
operator<<(std::wostream& os, const RWStandardWString& str)
{
    os << str.std();
    return os;
}

inline std::wostream&
operator<<(std::wostream& os, const RWStandardWSubString& str)
{
    os << RWStandardWString(str);
    return os;
}

inline std::wostream&
operator<<(std::wostream& os, const RWStandardWConstSubString& str)
{
    os << RWStandardWString(str);
    return os;
}

/**
 * @relates RWWString
 *
 * Restores a wide character string into \a cstr from a virtual stream,
 * replacing the previous contents of \a cstr.
 *
 * If an error occurs while extracting the string, the operator returns
 * the original string contents unmodified. Check the virtual stream to
 * determine if an error occurred on extraction.
 */
inline RWvistream&
operator>>(RWvistream& str, RWStandardWString& cstr)
{
    cstr.restoreFrom(str);
    return str;
}

/**
 * @relates RWWString
 *
 * Restores a wide character string into \a cstr from an RWFile, replacing
 * the previous contents of \a cstr.
 *
 * If an error occurs while extracting the string, the operator returns
 * the original string contents unmodified. Check the file to determine if
 * an error occurred on extraction.
 */
inline RWFile&
operator>>(RWFile& file, RWStandardWString& cstr)
{
    cstr.restoreFrom(file);
    return file;
}

inline RWvistream&
operator>>(RWvistream& str, RWStandardWString*& wstr)
{
    RWTScopedPointer<RWStandardWString> newWstr(new RWStandardWString);
    newWstr->restoreFrom(str);
    wstr = newWstr.release();
    return str;
}

inline RWFile&
operator>>(RWFile& file, RWStandardWString*& wstr)
{
    RWTScopedPointer<RWStandardWString> newWstr(new RWStandardWString);
    newWstr->restoreFrom(file);
    wstr = newWstr.release();
    return file;
}

/**
 * @relates RWWString
 *
 * Saves string \a str to a virtual stream.
 */
inline RWvostream&
operator<<(RWvostream& vos, const RWStandardWString& str)
{
    str.saveOn(vos);
    return vos;
}

/**
 * @relates RWWString
 *
 * Saves string \a str to an RWFile.
 */
inline RWFile&
operator<<(RWFile& file, const RWStandardWString& str)
{
    str.saveOn(file);
    return file;
}

inline RWStandardWString
operator+(const RWStandardWString& str1, const RWStandardWString& str2)
{
    size_t str1Len = str1.length();
    if (!str1Len) {
        return str2;
    }
    size_t str2Len = str2.length();
    if (!str2Len) {
        return str1;
    }
    RWStandardWString ret(RWSize_T(str1Len + str2Len));
    ret.append(str1);
    return ret.append(str2);
}

inline RWStandardWString
operator+(const RWStandardWString& lhs, const RWStandardWSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardWString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardWString
operator+(const RWStandardWSubString& lhs, const RWStandardWString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardWString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardWString
operator+(const RWStandardWString& lhs, const RWStandardWConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardWString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardWString
operator+(const RWStandardWConstSubString& lhs, const RWStandardWString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardWString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardWString
operator+(const RWStandardWString& str, const wchar_t* s)
{
    RW_PRECONDITION2(s != rwnil,
                     "operator+(const RWStandardWString& str, const wchar_t* s): null pointer");
    size_t sLen = wcslen(s);
    if (!sLen) {
        return str;
    }
    size_t strLen = str.length();
    if (!strLen) {
        return RWStandardWString(s, sLen);
    }
    RWStandardWString tmp(RWSize_T(strLen + sLen));
    tmp.append(str);
    return tmp.append(s, sLen);
}

inline RWStandardWString
operator+(const wchar_t* s, const RWStandardWString& str)
{
    RW_PRECONDITION2(s != rwnil,
                     "operator+(const wchar_t* s, const RWStandardWString& str): null pointer");
    size_t sLen = wcslen(s);
    if (!sLen) {
        return str;
    }
    size_t strLen = str.length();
    if (!strLen) {
        return RWStandardWString(s, sLen);
    }
    RWStandardWString tmp(RWSize_T(strLen + sLen));
    tmp.append(s, sLen);
    return tmp.append(str);
}

inline RWStandardWString
operator+(const std::wstring& s, const RWStandardWString& str)
{
    size_t sLen = s.length();
    if (!sLen) {
        return str;
    }
    size_t strLen = str.length();
    if (!strLen) {
        return RWStandardWString(s);
    }
    RWStandardWString ret(RWSize_T(sLen + strLen));
    ret.append(s);
    return ret.append(str);
}

inline RWStandardWString
operator+(const RWStandardWString& str, const std::wstring& s)
{
    size_t strLen = str.length();
    if (!strLen) {
        return RWStandardWString(s);
    }
    size_t sLen = s.length();
    if (!sLen) {
        return str;
    }
    RWStandardWString ret(RWSize_T(sLen + strLen));
    ret.append(str);
    return ret.append(s);
}

inline RWStandardWString
operator+(const RWStandardWSubString& lhs, const RWStandardWSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardWString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardWString
operator+(const RWStandardWSubString& lhs, const RWStandardWConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardWString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardWString
operator+(const RWStandardWConstSubString& lhs, const RWStandardWSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardWString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardWString
operator+(const RWStandardWSubString& lhs,  const std::wstring& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWStandardWString(rhs);
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardWString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardWString
operator+(const std::wstring& lhs, const RWStandardWSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWStandardWString(lhs);
    }
    RWStandardWString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardWString
operator+(const RWStandardWSubString& lhs,  const wchar_t* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator+(const RWStandardWSubString& lhs,  const wchar_t* rhs): null pointer");
    size_t rhsLen = wcslen(rhs);
    if (!rhsLen) {
        return lhs;
    }
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWStandardWString(rhs, rhsLen);
    }
    RWStandardWString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs, rhsLen);
}

inline RWStandardWString
operator+(const wchar_t* lhs, const RWStandardWSubString& rhs)
{
    RW_PRECONDITION2(lhs != rwnil,
                     "operator+(const wchar_t* lhs, const RWStandardWSubString& rhs): null pointer");
    size_t lhsLen = wcslen(lhs);
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWStandardWString(lhs, lhsLen);
    }
    RWStandardWString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs, lhsLen);
    return ret.append(rhs);
}

inline RWStandardWString
operator+(const RWStandardWConstSubString& lhs, const RWStandardWConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardWString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardWString
operator+(const RWStandardWConstSubString& lhs,  const std::wstring& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWStandardWString(rhs);
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    RWStandardWString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardWString
operator+(const std::wstring& lhs, const RWStandardWConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWStandardWString(lhs);
    }
    RWStandardWString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs);
}

inline RWStandardWString
operator+(const RWStandardWConstSubString& lhs,  const wchar_t* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator+(const RWStandardWConstSubString& lhs,  const wchar_t* rhs): null pointer");
    size_t rhsLen = wcslen(rhs);
    if (!rhsLen) {
        return lhs;
    }
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWStandardWString(rhs, rhsLen);
    }
    RWStandardWString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs);
    return ret.append(rhs, rhsLen);
}

inline RWStandardWString
operator+(const wchar_t* lhs, const RWStandardWConstSubString& rhs)
{
    RW_PRECONDITION2(lhs != rwnil,
                     "operator+(const wchar_t* lhs, const RWStandardWConstSubString& rhs): null pointer");
    size_t lhsLen = wcslen(lhs);
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWStandardWString(lhs, lhsLen);
    }
    RWStandardWString ret(RWSize_T(lhsLen + rhsLen));
    ret.append(lhs, lhsLen);
    return ret.append(rhs);
}

// exact matches required here to avoid ambiguities

/**
 * @relates RWWString
 * Returns \c true if \a lhs is lexicographically equal to \a rhs.
 * Otherwise returns \c false. Use member RWWString::collate()
 * or RWWString::strXForm() for locale-sensitive comparisons.
 */
inline bool
operator==(const RWStandardWString& lhs, const wchar_t* rhs)
{
    return lhs.std() == rhs;
}

/**
 * @relates RWWString
 * @copydoc operator==(const RWWString&, const wchar_t*)
 */
inline bool
operator==(const wchar_t* lhs, const RWStandardWString& rhs)
{
    return (rhs.std() == lhs);
}

/**
 * @relates RWWString
 * @copydoc operator==(const RWWString&, const wchar_t*)
 */
inline bool
operator==(const RWStandardWString& lhs, const std::wstring& rhs)
{
    return lhs.std() == rhs;
}

/**
 * @relates RWWString
 * @copydoc operator==(const RWWString&, const wchar_t*)
 */
inline bool
operator==(const std::wstring& lhs, const RWStandardWString& rhs)
{
    return (lhs == rhs.std());
}

/**
 * @relates RWWString
 * The equivalent of calling:
 *
 * @code
 * !(lhs == rhs)
 * @endcode
 */
inline bool
operator!=(const RWStandardWString& lhs, const wchar_t* rhs)
{
    return !(lhs.std() == rhs);
}

/**
 * @relates RWWString
 * @copydoc operator!=(const RWWString&, const wchar_t*)
 */
inline bool
operator!=(const wchar_t* lhs, const RWStandardWString& rhs)
{
    return !(rhs.std() == lhs);
}

/**
 * @relates RWWString
 * @copydoc operator!=(const RWWString&, const wchar_t*)
 */
inline bool
operator!=(const RWStandardWString& lhs, const std::wstring& rhs)
{
    return !(rhs == lhs.std());
}

/**
 * @relates RWWString
 * @copydoc operator!=(const RWWString&, const wchar_t*)
 */
inline bool
operator!=(const std::wstring& lhs, const RWStandardWString& rhs)
{
    return !(rhs.std() == lhs);
}

/**
 * @relates RWWString
 * Returns \c true if \a lhs lexicographically precedes \a rhs.
 * Otherwise returns \c false. Use member RWWString::collate() or
 * RWWString::strXForm() for locale-sensitive comparisons.
 */
inline bool
operator< (const RWStandardWString& lhs, const wchar_t* rhs)
{
    return lhs.std() < rhs;
}

/**
 * @relates RWWString
 * @copydoc operator<(const RWWString&, const wchar_t*)
 */
inline bool
operator< (const wchar_t* lhs, const RWStandardWString& rhs)
{
    return rhs.std() > lhs;
}

/**
 * @relates RWWString
 * @copydoc operator<(const RWWString&, const wchar_t*)
 */
inline bool
operator< (const RWStandardWString& lhs, const std::wstring& rhs)
{
    return lhs.std() < rhs;
}

/**
 * @relates RWWString
 * @copydoc operator<(const RWWString&, const wchar_t*)
 */
inline bool
operator< (const std::wstring& lhs, const RWStandardWString& rhs)
{
    return (lhs < rhs.std());
}

/**
 * @relates RWWString
 * The equivalent of calling:
 *
 * @code
 * !(rhs < lhs)
 * @endcode
 */
inline bool
operator> (const RWStandardWString& lhs, const wchar_t* rhs)
{
    return rhs < lhs.std();
}

/**
 * @relates RWWString
 * @copydoc operator>(const RWWString&, const wchar_t*)
 */
inline bool
operator> (const wchar_t* lhs, const RWStandardWString& rhs)
{
    return rhs.std() < lhs;
}

/**
 * @relates RWWString
 * @copydoc operator>(const RWWString&, const wchar_t*)
 */
inline bool
operator> (const RWStandardWString& lhs, const std::wstring& rhs)
{
    return rhs < lhs.std();
}

/**
 * @relates RWWString
 * @copydoc operator>(const RWWString&, const wchar_t*)
 */
inline bool
operator> (const std::wstring& lhs, const RWStandardWString& rhs)
{
    return rhs.std() < lhs;
}

/**
 * @relates RWWString
 * The equivalent of calling:
 *
 * @code
 * !(rhs < lhs)
 * @endcode
 */
inline bool
operator<=(const RWStandardWString& lhs, const wchar_t* rhs)
{
    return !(rhs < lhs.std());
}

/**
 * @relates RWWString
 * @copydoc operator<=(const RWWString&, const wchar_t*)
 */
inline bool
operator<=(const wchar_t* lhs, const RWStandardWString& rhs)
{
    return !(rhs.std() < lhs);
}

/**
 * @relates RWWString
 * @copydoc operator<=(const RWWString&, const wchar_t*)
 */
inline bool
operator<=(const RWStandardWString& lhs, const std::wstring& rhs)
{
    return !(rhs < lhs.std());
}

/**
 * @relates RWWString
 * @copydoc operator<=(const RWWString&, const wchar_t*)
 */
inline bool
operator<=(const std::wstring& lhs, const RWStandardWString& rhs)
{
    return !(rhs.std() < lhs);
}

/**
 * @relates RWWString
 * The equivalent of calling:
 *
 * @code
 * !(lhs < rhs)
 * @endcode
 */
inline bool
operator>=(const RWStandardWString& lhs, const wchar_t* rhs)
{
    return !(lhs.std() < rhs);
}

/**
 * @relates RWWString
 * @copydoc operator>=(const RWWString&, const wchar_t*)
 */
inline bool
operator>=(const wchar_t* lhs, const RWStandardWString& rhs)
{
    return !(lhs < rhs.std());
}

/**
 * @relates RWWString
 * @copydoc operator>=(const RWWString&, const wchar_t*)
 */
inline bool
operator>=(const RWStandardWString& lhs, const std::wstring& rhs)
{
    return !(lhs.std() < rhs);
}

/**
 * @relates RWWString
 * @copydoc operator>=(const RWWString&, const wchar_t*)
 */
inline bool
operator>=(const std::wstring& lhs, const RWStandardWString& rhs)
{
    return !(lhs < rhs.std());
}

#if !defined(RW_DISABLE_DEPRECATED)

/**
 * @internal
 * @deprecated As of SourcePro 12, use RWWString::hash(const RWWString&) instead.
 */
RW_DEPRECATE_FUNC("Use RWStandardWString::hash(const RWStandardWString&) instead")
inline unsigned
rwhash(const RWStandardWString& s)
{
    return unsigned(s.hash());
}

/**
 * @internal
 * @deprecated As of SourcePro 12, use RWWString::hash(const RWWString&) instead.
 */
RW_DEPRECATE_FUNC("Use RWStandardWString::hash(const RWStandardWString&) instead")
inline unsigned
rwhash(const RWStandardWString* s)
{
    return unsigned(s->hash());
}

#endif

//////////////////////////////////////////////////////////////////////////
//                         RWStandardWSubString                         //
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// RWStandardWSubString::startData()
//
// This member replaces data().
// startData() will remain undocumented. Please don't even ask.
// Use at your own risk. It may be deprecated in the future.
//
// Since RWStandardWSubString works by referencing the RWStandardWString's data,
// if you attempt to directly use the data() member of the RWStandardWString,
// you will very likely be surprised by the result, which is null
// terminated not at the extent of the substring,
// but at the end of the RWStandardWString.
//
///////////////////////////////////////////////////////////////////////////////

inline
RWStandardWSubString::RWStandardWSubString(const RWStandardWString& str, size_t pos, size_t n)
    : str_(RW_CONST_CAST(RWStandardWString*, &str)), begin_(pos), extent_(n)
{
#ifdef RW_DEBUG
    size_t len = str.length();

    // Allow zero lengthed and null substrings:
    if ((pos == RW_NPOS && n != RW_NPOS && n != 0) ||
            (pos != RW_NPOS && pos + n > len)) {
        subStringError(len, pos, n);
    }
#endif
}

inline const wchar_t*
RWStandardWSubString::startData() const
{
    return str_->data() + begin_;
}

inline RWStandardWSubString&
RWStandardWSubString::operator=(const wchar_t* s)
{
    if (!isNull()) {
        str_->replace(begin_, extent_, s);
    }
    return *this;
}

inline RWStandardWSubString&
RWStandardWSubString::operator=(const RWStandardWSubString& str)
{
    if (this == &str) {
        return *this;
    }

    if (!isNull()) {
        str_->replace(begin_, extent_, str.startData(), str.length());
    }
    return *this;
}

inline RWStandardWSubString&
RWStandardWSubString::operator=(const RWStandardWConstSubString& str)
{
    if (!isNull()) {
        str_->replace(begin_, extent_, str.startData(), str.length());
    }
    return *this;
}

inline RWStandardWSubString&
RWStandardWSubString::operator=(const std::wstring& str)
{
    if (!isNull()) {
        str_->replace(begin_, extent_, str);
    }
    return *this;
}

inline RWStandardWSubString&
RWStandardWSubString::operator=(const RWStandardWString& str)
{
    if (!isNull()) {
        str_->replace(begin_, extent_, str.std());
    }
    return *this;
}

inline wchar_t&
RWStandardWSubString::operator()(size_t i)
{
    return (*str_)(begin_ + i);
}

inline wchar_t
RWStandardWSubString::operator()(size_t i) const
{
    return (*str_)(begin_ + i);
}

// Here all integer types are provided with an exact match
// to avoid ambiguities with the built in wchar_t[] operator
inline wchar_t&
RWStandardWSubString::operator[](short i)
{
    return (*str_)[begin_ + i];
}

inline wchar_t
RWStandardWSubString::operator[](short i) const
{
    return (*str_)[begin_ + i];
}

inline wchar_t&
RWStandardWSubString::operator[](int i)
{
    return (*str_)[begin_ + i];
}

inline wchar_t
RWStandardWSubString::operator[](int i) const
{
    return (*str_)[begin_ + i];
}

inline wchar_t&
RWStandardWSubString::operator[](long i)
{
    return (*str_)[begin_ + i];
}

inline wchar_t
RWStandardWSubString::operator[](long i) const
{
    return (*str_)[begin_ + i];
}

inline wchar_t&
RWStandardWSubString::operator[](unsigned short i)
{
    return (*str_)[begin_ + i];
}

inline wchar_t
RWStandardWSubString::operator[](unsigned short i) const
{
    return (*str_)[begin_ + i];
}

inline wchar_t&
RWStandardWSubString::operator[](unsigned int i)
{
    return (*str_)[begin_ + i];
}

inline wchar_t
RWStandardWSubString::operator[](unsigned int i) const
{
    return (*str_)[begin_ + i];
}

inline wchar_t&
RWStandardWSubString::operator[](unsigned long i)
{
    return (*str_)[begin_ + i];
}

inline  wchar_t
RWStandardWSubString::operator[](unsigned long i) const
{
    return (*str_)[begin_ + i];
}

#if !defined(RW_NO_LONG_LONG)
inline wchar_t&
RWStandardWSubString::operator[](unsigned long long i)
{
    return (*str_)[begin_ + i];
}

inline wchar_t
RWStandardWSubString::operator[](unsigned long long i) const
{
    return (*str_)[begin_ + i];
}

inline wchar_t&
RWStandardWSubString::operator[](long long i)
{
    return (*str_)[begin_ + i];
}

inline wchar_t
RWStandardWSubString::operator[](long long i) const
{
    return (*str_)[begin_ + i];
}
#endif


////////////////////// RWStandardWSubString::operator==() ///////////////////////
/**
 * @relates RWWSubString
 * @copydoc RWWString::operator==(const RWWString&, const wchar_t*)
 */
RW_TOOLS_SYMBOLIC bool operator==(const RWStandardWSubString& lhs,
                                  const wchar_t* rhs);

/**
 * @relates RWWSubString
 * @copydoc RWWString::operator==(const RWWString&, const wchar_t*)
 */
inline bool operator==(const wchar_t* lhs,
                       const RWStandardWSubString& rhs)
{
    return (rhs == lhs);
}

/**
 * @relates RWWSubString
 * @copydoc RWWString::operator==(const RWWString&, const wchar_t*)
 */
RW_TOOLS_SYMBOLIC bool operator==(const RWStandardWSubString& lhs,
                                  const RWStandardWSubString& rhs);

/**
 * @relates RWWSubString
 * @copydoc RWWString::operator==(const RWWString&, const wchar_t*)
 */
RW_TOOLS_SYMBOLIC bool operator==(const RWStandardWSubString& lhs,
                                  const RWStandardWConstSubString& rhs);

/**
 * @relates RWWString
 * @copydoc operator==(const RWWString&, const wchar_t*)
 */
RW_TOOLS_SYMBOLIC bool operator==(const RWStandardWSubString& lhs,
                                  const RWStandardWString& rhs);

/**
 * @relates RWWString
 * @copydoc operator==(const RWWString&, const wchar_t*)
 */
inline bool operator==(const RWStandardWString& lhs,
                       const RWStandardWSubString& rhs)
{
    return (rhs == lhs);
}

////////////////////// RWStandardWSubString::operator!=() ///////////////////////
/**
 * @relates RWWSubString
 * @copydoc RWWString::operator!=(const RWWString&, const wchar_t*)
 */
inline bool operator!=(const RWStandardWSubString& lhs, const wchar_t* rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWWSubString
 * @copydoc RWWString::operator!=(const RWWString&, const wchar_t*)
 */
inline bool operator!=(const wchar_t* lhs, const RWStandardWSubString& rhs)
{
    return !(rhs == lhs);
}

/**
 * @relates RWWSubString
 * @copydoc RWWString::operator!=(const RWWString&, const wchar_t*)
 */
inline bool operator!=(const RWStandardWSubString& lhs,
                       const RWStandardWSubString& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWWString
 * @copydoc operator!=(const RWWString&, const wchar_t*)
 */
inline bool operator!=(const RWStandardWSubString& lhs, const RWStandardWString& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWWString
 * @copydoc operator!=(const RWWString&, const wchar_t*)
 */
inline bool operator!=(const RWStandardWString& lhs, const RWStandardWSubString& rhs)
{
    return !(rhs == lhs);
}

/**
 * @relates RWWSubString
 * @copydoc RWWString::operator!=(const RWWString&, const wchar_t*)
 */
inline bool operator!=(const RWStandardWSubString& lhs,
                       const RWStandardWConstSubString& rhs)
{
    return !(lhs == rhs);
}


// Convert self to lower-case
inline void
RWStandardWSubString::toLower()
{
    if (!isNull()) {
        str_->toLower(begin_, extent_);    // Ignore null substrings
    }
}

// Convert self to upper-case
inline void
RWStandardWSubString::toUpper()
{
    if (!isNull()) {
        str_->toUpper(begin_, extent_);    // Ignore null substrings
    }
}

//////////////////////////////////////////////////////////////////////////
//                         RWStandardWConstSubString                    //
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// RWStandardWConstSubString::startData()
//
// This member replaces data().
// startData() will remain undocumented. Please don't even ask.
// Use at your own risk. It may be deprecated in the future.
//
// Since RWStandardWSubString works by referencing the RWStandardWString's data,
// if you attempt to directly use the data() member of the RWStandardWString,
// you will very likely be surprised by the result, which is null
// terminated not at the extent of the substring,
// but at the end of the RWStandardWString.
//
///////////////////////////////////////////////////////////////////////////////

inline RWStandardWConstSubString::RWStandardWConstSubString
(const RWStandardWString& str, size_t pos, size_t n)
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

inline const wchar_t*
RWStandardWConstSubString::startData() const
{
    return str_->data() + begin_;
}

inline wchar_t
RWStandardWConstSubString::operator()(size_t i) const
{
    return (*str_)(begin_ + i);
}

// Here all integer types are provided with an exact match
// to avoid ambiguities with the built in wchar_t[] operator
inline wchar_t
RWStandardWConstSubString::operator[](short i) const
{
    return (*str_)[begin_ + i];
}

inline wchar_t
RWStandardWConstSubString::operator[](int i) const
{
    return (*str_)[begin_ + i];
}

inline wchar_t
RWStandardWConstSubString::operator[](long i) const
{
    return (*str_)[begin_ + i];
}

inline wchar_t
RWStandardWConstSubString::operator[](unsigned short i) const
{
    return (*str_)[begin_ + i];
}

inline wchar_t
RWStandardWConstSubString::operator[](unsigned int i) const
{
    return (*str_)[begin_ + i];
}

inline wchar_t
RWStandardWConstSubString::operator[](unsigned long i) const
{
    return (*str_)[begin_ + i];
}

////////////////////// RWStandardWConstSubString::operator==() ///////////////////////
/**
 * @relates RWWConstSubString
 * @copydoc RWWString::operator==(const RWWString&, const wchar_t*)
 */
RW_TOOLS_SYMBOLIC bool operator==(const RWStandardWConstSubString& lhs,
                                  const wchar_t* rhs);

/**
 * @relates RWWConstSubString
 * @copydoc RWWString::operator==(const RWWString&, const wchar_t*)
 */
inline bool operator==(const wchar_t* lhs,
                       const RWStandardWConstSubString& rhs)
{
    return (rhs == lhs);
}

/**
 * @relates RWWConstSubString
 * @copydoc RWWString::operator==(const RWWString&, const wchar_t*)
 */
RW_TOOLS_SYMBOLIC bool operator==(const RWStandardWConstSubString& lhs,
                                  const RWStandardWConstSubString& rhs);

/**
 * @relates RWWString
 * @copydoc operator==(const RWWString&, const wchar_t*)
 */
RW_TOOLS_SYMBOLIC bool operator==(const RWStandardWConstSubString& lhs,
                                  const RWStandardWString& rhs);

/**
 * @relates RWWString
 * @copydoc operator==(const RWWString&, const wchar_t*)
 */
inline bool operator==(const RWStandardWString& lhs,
                       const RWStandardWConstSubString& rhs)
{
    return (rhs == lhs);
}

/**
 * @relates RWWSubString
 * @copydoc RWWString::operator==(const RWWString&, const wchar_t*)
 */
inline bool operator==(const RWStandardWConstSubString& lhs,
                       const RWStandardWSubString& rhs)
{
    return (rhs == lhs);
}

////////////////////// RWStandardWConstSubString::operator!=() ///////////////////////
/**
 * @relates RWWConstSubString
 * @copydoc RWWString::operator!=(const RWWString&, const wchar_t*)
 */
inline bool operator!=(const RWStandardWConstSubString& lhs,
                       const wchar_t* rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWWConstSubString
 * @copydoc RWWString::operator!=(const RWWString&, const wchar_t*)
 */
inline bool operator!=(const wchar_t* lhs,
                       const RWStandardWConstSubString& rhs)
{
    return !(rhs == lhs);
}

/**
 * @relates RWWConstSubString
 * @copydoc RWWString::operator!=(const RWWString&, const wchar_t*)
 */
inline bool operator!=(const RWStandardWConstSubString& lhs,
                       const RWStandardWConstSubString& rhs)
{
    return !(lhs == rhs);
}

/**
 * @relates RWWString
 * @copydoc operator!=(const RWWString&, const wchar_t*)
 */
inline bool operator!=(const RWStandardWConstSubString& lhs,
                       const RWStandardWString& rhs)
{
    return !(lhs == rhs);
}


/**
 * @relates RWWString
 * @copydoc operator!=(const RWWString&, const wchar_t*)
 */
inline bool operator!=(const RWStandardWString& lhs,
                       const RWStandardWConstSubString& rhs)
{
    return !(rhs == lhs);
}

/**
 * @relates RWWSubString
 * @copydoc RWWString::operator!=(const RWWString&, const wchar_t*)
 */
inline bool operator!=(const RWStandardWConstSubString& lhs,
                       const RWStandardWSubString& rhs)
{
    return !(lhs == rhs);
}

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an RWWString.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<RWStandardWString> : std::unary_function<RWStandardWString, size_t> {
    /**
     * Constructs a hash object instance with the specified comparison
     * type.
     */
    RWTHash<RWStandardWString>(RWStandardWString::caseCompare cmp = RWStandardWString::exact)
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
    size_t operator()(const RWStandardWString& obj) const {
        return obj.hash(cmp_);
    }

private:
    RWStandardWString::caseCompare cmp_;
};


#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif // RW_TOOLS_STDWSTRING_H
