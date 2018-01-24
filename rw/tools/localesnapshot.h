
/**********************************************************************
 *
 * Declarations of RWLocaleSnapshot class
 *
 * Class RWLocaleSnapshot encapsulates the "C" locales data.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/localesnapshot.h#1 $
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

#ifndef RW_TOOLS_LOCALESNAPSHOT_H
#define RW_TOOLS_LOCALESNAPSHOT_H

#include <rw/defs.h>

#if !defined(RW_NO_LOCALE)
#  include <locale.h>
#endif

#include <time.h>               /* for struct tm and time_t */

#include <rw/locale.h>          /* for RWLocale */
#include <rw/cstring.h>         /* for RWCString */
#include <rw/mutex.h>
#include <rw/zone.h>            /* for RWZone::local() */

// RWLocaleSnapshot is the real working locale object.  When constructed
// it copies all necessary data from the environment into itself, for use
// later use by its member functions.

// Forward declarations of implementation types
struct RWLocaleSnapshotImpl;

/**
 * @ingroup internationalization_classes
 *
 * @brief Creates an snapshot of a locale by querying
 * the program's environment to determine the formatting conventions in
 * effect at the moment of instantiation
 *
 * The class RWLocaleSnapshot implements the RWLocale interface
 * using Standard C Library facilities. To use it, the program creates
 * an RWLocaleSnapshot instance. The constructor of the instance
 * queries the program's environment (using Standard C Library functions
 * such as \c localeconv(), \c strftime(), and, if available, vendor-specific
 * library functions) to determine the formatting conventions in
 * effect at the moment of instantiation. When done, the locale
 * can then be switched and another instance of RWLocaleSnapshot
 * created.
 *
 * By creating multiple instances of RWLocaleSnapshot, your program
 * can have more than one locale active at the same time, something
 * that is difficult to do with the Standard C Library facilities.
 *
 * @note
 * RWLocaleSnapshot does not encapsulate character
 * set, collation, or message information.
 *
 * Class RWLocaleSnapshot contains a set of public data members
 * initialized by its constructor with information extracted from
 * its execution environment.
 *
 * For a description of the static members of the RWLocale interface
 * that operate on objects of type RWLocaleSnapshot, see the RWLocale
 * class description.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <locale.h>
 * #include <rw/locale.h>
 * RWLocaleSnapshot ourLocale("");  // encapsulate user's formats
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 *
 * @note
 * To ensure that the asString() methods in this example
 * correctly format the output, you must first install on your system
 * any locale you request before invoking RWLocaleSnapshot().
 *
 * @code
 *
 * #include <rw/tools/datetime.h>
 * #include <rw/locale.h>
 * #include <rw/cstring.h>
 *
 * #if defined(_WIN32)
 *    // french should be a valid French locale name on Windows
 *   #define FRENCH_LOCALE_NAME "french"
 * #elif defined(__HP_aCC)
 *   #define FRENCH_LOCALE_NAME "fr_FR.utf8"
 * #else
 *    // fr_FR should be a valid French locale name on most Unices
 *   #define FRENCH_LOCALE_NAME "fr_FR"
 * #endif
 *
 * int main()
 * {
 *     RWLocaleSnapshot us("C");
 *
 *     RWLocaleSnapshot french(FRENCH_LOCALE_NAME); // or vendor-specific
 *
 *     RWCString americanDate ("10 June 2025");
 *     RWCString frenchDate   ("10 Juin 2025");
 *
 *     RWDateTime d(frenchDate, RWDateTime::setDate, french); // OK
 *
 *     std::cout << frenchDate << ((d.isValid()) ? " IS " : " IS NOT ")
 *               << "a valid date (french locale)." << std::endl << std::endl;
 *
 *     RWDateTime bad = RWDateTime(frenchDate, RWDateTime::setDate);
 *     std::cout << frenchDate;
 *     std::cout << ((bad.isValid() && bad == d) ? " IS " : " IS NOT ")
 *               << "a valid date (default locale)." << std::endl << std::endl;
 *
 *     bad = RWDateTime(americanDate, RWDateTime::setDate, french);
 *     std::cout << americanDate;
 *     std::cout << ((bad.isValid() && bad == d) ? " IS " : " IS NOT ")
 *               << "a valid date (french locale)." << std::endl << std::endl;
 *
 *     std::cout << d << std::endl;
 *     std::cout << d.asString('x') << std::endl;
 *     std::cout << d.asString('x', french) << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 *
 * 10 Juin 2025 IS a valid date (french locale).
 *
 * 10 Juin 2025 IS NOT a valid date (default locale).
 *
 * 10 June 2025 IS a valid date (french locale).
 *
 * Tue Jun 10 00:00:00 2025
 * 06/10/25
 * 10.06.2025
 * @endcode
 *
 */
class RW_TOOLS_GLOBAL RWLocaleSnapshot : public RWLocale
{

public:

    // old, already exposed type and data
    enum RWDateOrder { DMY, MDY, YDM, YMD };
    RWDateOrder dateOrder_;

public:

    // Locale name
    RWCString locale_name_;

    // Data - lconv members
    /**
     * Defined identically as the correspondingly-named member of the
     * Standard C Library type \c lconv, from \c <locale.h>.
     */
    RWCString decimal_point_;
    /**
     * Defined identically as the correspondingly-named member of the
     * Standard C Library type \c lconv, from \c <locale.h>.
     */
    RWCString thousands_sep_;
    /**
     * Defined identically as the correspondingly-named member of the
     * Standard C Library type \c lconv, from \c <locale.h>.
     */
    RWCString grouping_;
    /**
     * Defined identically as the correspondingly-named member of the
     * Standard C Library type \c lconv, from \c <locale.h>.
     */
    RWCString int_curr_symbol_;
    /**
     * Defined identically as the correspondingly-named member of the
     * Standard C Library type \c lconv, from \c <locale.h>.
     */
    RWCString currency_symbol_;
    /**
     * Defined identically as the correspondingly-named member of the
     * Standard C Library type \c lconv, from \c <locale.h>.
     */
    RWCString mon_decimal_point_;
    /**
     * Defined identically as the correspondingly-named member of the
     * Standard C Library type \c lconv, from \c <locale.h>.
     */
    RWCString mon_thousands_sep_;
    /**
     * Defined identically as the correspondingly-named member of the
     * Standard C Library type \c lconv, from \c <locale.h>.
     */
    RWCString mon_grouping_;
    /**
     * Defined identically as the correspondingly-named member of the
     * Standard C Library type \c lconv, from \c <locale.h>.
     */
    RWCString positive_sign_;
    /**
     * Defined identically as the correspondingly-named member of the
     * Standard C Library type \c lconv, from \c <locale.h>.
     */
    RWCString negative_sign_;

    /**
     * Defined identically as the correspondingly-named member of the
     * Standard C Library type \c lconv, from \c <locale.h>.
     */
    char int_frac_digits_;
    /**
     * Defined identically as the correspondingly-named member of the
     * Standard C Library type \c lconv, from \c <locale.h>.
     */
    char frac_digits_;
    /**
     * Defined identically as the correspondingly-named member of the
     * Standard C Library type \c lconv, from \c <locale.h>.
     */
    char p_cs_precedes_;
    /**
     * Defined identically as the correspondingly-named member of the
     * Standard C Library type \c lconv, from \c <locale.h>.
     */
    char p_sep_by_space_;
    /**
     * Defined identically as the correspondingly-named member of the
     * Standard C Library type \c lconv, from \c <locale.h>.
     */
    char n_cs_precedes_;
    /**
     * Defined identically as the correspondingly-named member of the
     * Standard C Library type \c lconv, from \c <locale.h>.
     */
    char n_sep_by_space_;
    /**
     * Defined identically as the correspondingly-named member of the
     * Standard C Library type \c lconv, from \c <locale.h>.
     */
    char p_sign_posn_;
    /**
     * Defined identically as the correspondingly-named member of the
     * Standard C Library type \c lconv, from \c <locale.h>.
     */
    char n_sign_posn_;

    char mb_cur_max_;

    // Data accessors
    /**
     * Returns the name of the encapsulated \b std::locale.
     */
    virtual const RWCString& locale_name() const {
        return locale_name_;
    }

    /**
     * Returns a string to use as the numerical decimal separator.
     */
    virtual const RWCString& decimal_point() const {
        return decimal_point_;
    }

    /**
     * Returns a string to use as the numerical thousands separator.
     */
    virtual const RWCString& thousands_sep() const {
        return thousands_sep_;
    }

    /**
     * Returns a string identifying the number of digits to be included
     * in a numerical group. A group is simply the digits between adjacent
     * thousand's separators.
     *
     * Each character of the string is an integer that specifies the
     * number of digits in a group, starting with the right most group.
     *
     * -  If the value is equal to 0, the previous element is used repeatedly
     *    for the remainder of the digits.
     * -  If the value is equal to \c CHAR_MAX, no further grouping is
     *    to be performed.
     *
     * @section example Example
     *
     * @code
     * "\3"    // Specifies the United States grouping system.
     *         // For example, "1,000,000"
     * "\3\2"  // Specifies the Nepalese grouping system.
     *         // For example, "1,00,00,000"
     * @endcode
     */
    virtual const RWCString& grouping() const {
        return grouping_;
    }

    /**
     * Returns a string to use as the international currency symbol.
     */
    virtual const RWCString& int_curr_symbol() const {
        return int_curr_symbol_;
    }

    /**
     * Returns a string to use as the currency symbol for this locale.
     */
    virtual const RWCString& currency_symbol() const {
        return currency_symbol_;
    }

    /**
     * Returns a string to use as the monetary decimal separator.
     */
    virtual const RWCString& mon_decimal_point() const {
        return mon_decimal_point_;
    }

    /**
     * Returns a string to use as the monetary thousands separator.
     */
    virtual const RWCString& mon_thousands_sep() const {
        return mon_thousands_sep_;
    }

    /**
     * Returns a string identifying the number of digits to be included
     * in a numerical group. A group is simply the digits between adjacent
     * thousand's separators.
     *
     * Each character of the string is an integer that specifies the
     * number of digits in a group, starting with the right most group.
     *
     * -  If the value is equal to 0, the previous element is used repeatedly
     *    for the remainder of the digits.
     *
     * If the value is equal to \c CHAR_MAX, no further grouping is
     * to be performed.
     *
     * See \e Example under grouping().
     */
    virtual const RWCString& mon_grouping() const {
        return mon_grouping_;
    }

    /**
     * Returns a string to use as the positive sign.
     */
    virtual const RWCString& positive_sign() const {
        return positive_sign_;
    }

    /**
     * Returns a string to use as the negative sign.
     */
    virtual const RWCString& negative_sign() const {
        return negative_sign_;
    }


    virtual char int_frac_digits() const {
        return int_frac_digits_;
    }

    virtual char frac_digits() const {
        return frac_digits_;
    }

    virtual char p_cs_precedes() const {
        return p_cs_precedes_;
    }

    virtual char p_sep_by_space() const {
        return p_sep_by_space_;
    }

    virtual char n_cs_precedes() const {
        return n_cs_precedes_;
    }

    virtual char n_sep_by_space() const {
        return n_sep_by_space_;
    }

    virtual char p_sign_posn() const {
        return p_sign_posn_;
    }

    virtual char n_sign_posn() const {
        return n_sign_posn_;
    }


    // Constructor
    /**
     * Constructs an RWLocale object by extracting formats from the
     * global locale environment. It uses the Standard C Library function
     * \c setlocale() to set the named locale, and then restores the previous
     * global locale after formats have been extracted. If \a localeName
     * is 0, it uses the current locale. The most useful locale
     * name is the empty string, "", which is a synonym for the user's
     * chosen locale (usually specified by the environment variable
     * \c LANG).
     */
    RWLocaleSnapshot(const char* localeName = 0);

    // Destructor
    virtual ~RWLocaleSnapshot();

    // Time formatting
    /**
     * Redefined from RWLocale
     * @note
     * These virtual functions follow the interface described under
     * class RWLocale. They generally work by converting values to and from
     * strings using the rules specified by the <tt>struct lconv</tt> values
     * (see \c <locale.h>) encapsulated in self.
     */
    RWCString asString(const struct tm* t, const char* fmt,
                       const RWZone& z = RWZone::local()) const;

    // Date time formatting
    /**
     * @copydoc asString(const struct tm*,const char* fmt,const RWZone&)const
     */
    virtual
    RWCString asString(const struct tm*, char,
                       const RWZone& = RWZone::local()) const;


    // Number formatting
    /**
     * @copydoc asString(const struct tm*,const char* fmt,const RWZone&)const
     */
    virtual RWCString asString(long) const;
    /**
     * @copydoc asString(const struct tm*,const char* fmt,const RWZone&)const
     */
    virtual RWCString asString(unsigned long) const;
    /**
     * @copydoc asString(const struct tm*,const char* fmt,const RWZone&)const
     */
    virtual RWCString asString(double, int =  6, bool = 0) const;

#if !defined(RW_NO_LONG_DOUBLE)
    /**
     * @copydoc asString(const struct tm*,const char* fmt,const RWZone&)const
     */
    virtual RWCString asString(long double, int = 10, bool = 0) const;
#endif // RW_NO_LONG_DOUBLE

    // Monetary formatting
    /**
     * @copydoc asString(const struct tm*,const char* fmt,const RWZone&)const
     */
    virtual RWCString moneyAsString(double,
                                    RWLocaleSnapshot::CurrSymbol = LOCAL) const;

    // Data time formatting
    /**
     * @copydoc asString(const struct tm*,const char* fmt,const RWZone&)const
     */
    virtual bool stringToDate(const RWCString&, struct tm*) const;
    /**
     * @copydoc asString(const struct tm*,const char* fmt,const RWZone&)const
     */
    virtual bool stringToTime(const RWCString&, struct tm*) const;

    // Numeric parsing
    /**
     * @copydoc asString(const struct tm*,const char* fmt,const RWZone&)const
     */
    virtual bool stringToNum(const RWCString&, double*) const;
    /**
     * @copydoc asString(const struct tm*,const char* fmt,const RWZone&)const
     */
    virtual bool stringToNum(const RWCString&, long*) const;
    /**
     * @copydoc asString(const struct tm*,const char* fmt,const RWZone&)const
     */
    virtual bool stringToNum(const RWCString&, unsigned long*) const;

    // Monetary parsing
    /**
     * @copydoc asString(const struct tm*,const char* fmt,const RWZone&)const
     */
    virtual bool stringToMoney(const RWCString&, double*,
                               RWLocaleSnapshot::CurrSymbol = LOCAL) const;

    // returns [1..12], 0 for error
    virtual int monthIndex(const RWCString&) const;

    // returns 1 for Monday, 7 for Sunday, 0 for error.
    virtual int weekdayIndex(const RWCString&) const;

    void
    refresh();

    friend class RWLocaleStream;

private:

    // time, monetary and ctype data
    RWLocaleSnapshotImpl* impl_;

    // extracts all time, monetary and ctype formats
    int extractFormats();

private:

    // not CopyConstructible
    RWLocaleSnapshot(RWLocaleSnapshot&);

    // not Assignable
    void operator= (RWLocaleSnapshot&);

    // class synchronization object -
    // consistent usage of RWLocaleSnapshot is preferred to direct
    // manipulation of C locales; this synchronization object makes
    // possible the use of and construction of RWLocaleSnapshot
    // objects in a MT environ without corruption of class objects data.
    static RWStaticFastMutex lock_;

public:
};

#endif /*RW_TOOLS_LOCALESNAPSHOT_H*/
