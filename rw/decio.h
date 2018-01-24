#ifndef RW_CURRENCY_DECIO_H
#define RW_CURRENCY_DECIO_H

/***************************************************************************
 *
 * Classes for fancy output of RWDecimal numbers
 *
 * $Id: //money/13/rw/decio.h#1 $
 *
 * Copyright (c) 1993-2015 Rogue Wave Software, Inc.  All Rights Reserved.
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
 ***************************************************************************
 *
 * See notes in decio.cpp for implementation details on how formatted
 * numbers are built and how pictures are parsed.
 *
 * There are two classes:
 *
 *   RWPosDecimalFormat formats a decimal which must be positive
 *   RWDecimalFormat contains two RWPosDecimalFormat objects: it uses one
 *     for positive numbers and the other for negative numbers.
 *
 * RWPosDecimalFormat is an implementation specific class: it may change.
 * Ideally, it would be declared somewhere outside the public eye, but this
 * is not possible since it must be visible to the RWDecimalFormat declaration
 * (since RWDecimalFormat uses instances of RWPosDecimalFormat).
 *
 ***************************************************************************/

/* for backward compatibility */
#ifndef RW_MONEY_DECIO_H
#  define RW_MONEY_DECIO_H
#endif

#include <iosfwd>
#include <rw/cstring.h>
#include <rw/currency/decdefs.h>
#include <rw/decbase.h>
#include <rw/decport.h>
#include <rw/locale.h>

/**
 * @ingroup formatting
 *
 * @brief Encapsulates decimal format specifiers.
 *
  * RWDecimalFormatScope encapsulates decimal format specifiers.
 */
class RW_DCML_SYMBOLIC RWDecimalFormatScope
{
public:
    /**
     * How to justify the number in the width provided
     */
    enum Justification {

        /**
         * Use left justification.
         */
        LEFT,

        /**
         * Use center justification.
         */
        CENTER,

        /**
         * Use right justification.
         */
        RIGHT
    };

    /**
     * The Sign enumeration is used to indicate which number formatting is
     * being accessed.
     */
    enum Sign {

        /**
         * attribute value for formatting positive numbers
         */
        POSITIVE,

        /**
         * attribute value for formatting negative numbers
         */
        NEGATIVE,

        /**
         * attribute values for formatting positive and negative values
         */
        BOTH
    };

    // Dead Code: not public methods cause warnings  (Sept 14, 2000)
    //private:
    //  static bool initialized_;
    //  static void init();

};



class RW_DCML_SYMBOLIC RWPosDecimalFormat : public RWDecimalFormatScope
{
    // skip defining access functions: just access members directly
    friend class RW_DCML_GLOBAL RWDecimalFormat;

public:
    RWPosDecimalFormat();
    RWPosDecimalFormat(const char* fmt);

    /* Functions to query and change attributes */
    RWCString     operator()(const RWDecimalPortable&) const;
    void printState(std::ostream&);

private:
    bool      fixedWidth_;
    unsigned       width_;
    Justification  justification_;
    bool      doRounding_;
    RWDecimalBase::RoundingMethod roundMethod_;
    int            roundPlaces_;
    bool      showDecimalPoint_;
    bool      allDecimals_;
    int            decimalPlaces_;
    RWCString      decimalSeparator_;
    RWCString      leftDigitSeparator_;
    unsigned       leftGroupSize_;
    RWCString      rightDigitSeparator_;
    unsigned       rightGroupSize_;
    RWCString      nearLeftText_;
    RWCString      nearRightText_;
    char           leftFillChar_;
    char           rightFillChar_;
    RWCString      farLeftText_;
    RWCString      farRightText_;

    // following functions helpers in printing
    void      createDigits(const RWDecimalPortable&, RWCString* before, RWCString* after) const;
    RWCString buildLeftDigitString(const RWCString&, unsigned, const RWCString&) const;
    RWCString buildRightDigitString(const RWCString&, unsigned, const RWCString&) const;
};


/**
 * @ingroup formatting
 *
 * @brief Encapsulates formatting information for converting a decimal number
 * to a string.
 *
 * The RWDecimalFormat class encapsulates formatting information
 * for converting a decimal number to a string. You can construct
 * and modify the attributes of a formatted object in two ways:
 *
 * -# Specify a picture string. The string gives a textual representation
 *    of the look of the format. A sample picture is \"<tt>$0___.__</tt>\".
 *    Using this sample picture, the number \c 3.14159 is formatted
 *    as \"<tt>$0003.14</tt>\".
 * -# Set attributes directly, using member functions. This allows
 *    maximum flexibility.
 *
 * You can also use a hybrid approach by initially constructing
 * an RWDecimalFormat object using a picture string and then "fine
 * tuning" the formatting parameters using the member functions
 * to change attributes directly.
 *
 * Once a formatted object is constructed, you use the function
 * call \link operator()(const RWDecimalPortable&) const operator() \endlink to convert from decimal numbers to strings.
 *
 * @section RWDecimalFormat_synopsis Synopsis
 *
 * @code
 * #include <rw/decio.h>
 * int main(){cout << RWDecimalFormat("$_____.__")("2.243") << endl;}
 * @endcode
 *
 * @section RWDecimalFormat_example Example
 *
 * @code
 * #include <iostream>
 * #include <rw/currency/decimal.h>
 * #include <rw/decio.h>
 *
 * int main()
 * {
 *   RWDecimal<RWMP2Int> USbucks = "232.455";   // an amount in US$
 *   RWDecimal<RWMP2Int> UStoCDN = "0.7921";    // today's exchange
 *                                              // rate
 *   RWDecimal<RWMP2Int> CDNbucks = USbucks*UStoCDN;
 *
 *   // set up the US dollars format. Use banker's rounding, turn
 *   // currency symbol on, and set currency symbol
 *   RWDecimalFormat USformat("________.__");
 *   USformat.setNearLeftText("US$");
 *   USformat.setRoundMethod(RWDecimalBase::BANKERS);
 *
 *   // the Canadian dollars format is the same as the US$ except
 *   // for the currency symbol.
 *   RWDecimalFormat CDNformat = USformat;
 *   CDNformat.setNearLeftText("CDN$");
 *   std::cout << "Initially, amount is: " << USformat(USbucks) << std::endl;
 *   std::cout << "After exchange" << CDNformat(CDNbucks) << std::endl;
 *   return 0;
 * }
 * @endcode
 *
 * Program output
 *
 * @code
 *
 * Initially, amount is:  US$232.46
 * After exchange  CDN$184.13
 * @endcode
 *
 * @section RWDecimalFormat_attributes Attributes
 * The attributes used by the formatting object are shown in the
 * following table. Two sets of attributes are maintained by the
 * RWDecimalFormat  class: one for formatting positive numbers and one
 * for formatting negative numbers. Member functions that set an
 * attribute have names like \c setAttribute; member functions that obtain
 * the current value of an attribute have names like \c attribute().
 * Each of these functions takes a parameter indicating whether the
 * attribute for formatting positive values, negative values, or both
 * is to be operated on:
 *
 * <table> <caption align="top">Table 3: Attributes</caption>
 * <tr><th valign="top">Attribute</th><th valign="top">Description</th><th valign="top">Default</th></tr>
 * <tr><td valign="top">\c allDecimals</td><td valign="top">If \c false, the number of decimal places printed is given by the \c decimalPlaces attribute, otherwise all digits are printed.</td><td valign="top">\c false</td></tr>
 * <tr><td valign="top">\c decimalPlaces</td><td valign="top">If \c allDecimals is \c false, this is the number of decimal places to print. Negative numbers are allowed.</td><td valign="top">0</td></tr>
 * <tr><td valign="top">\c decimalSeparator</td><td valign="top">The decimal point symbol.</td><td valign="top">"."</td></tr>
 * <tr><td valign="top">\c doRounding</td><td valign="top">If \c true, round the number before display.</td><td valign="top">\c false</td></tr>
 * <tr><td valign="top">\c farLeftText</td><td valign="top">Text on the extreme left.</td><td valign="top">""</td></tr>
 * <tr><td valign="top">\c farRightText</td><td valign="top">Text on the extreme right.</td><td valign="top">""</td></tr>
 * <tr><td valign="top">\c fixedWidth</td><td valign="top">If \c false, the natural width of the number is used; if \c true, the width is fixed to the value of the width attribute</td><td valign="top">\c false</td></tr>
 * <tr><td valign="top">\c justification</td><td valign="top">How to justify the number in the width provided (\c LEFT, \c RIGHT, or \c CENTER are allowed).</td><td valign="top">\c LEFT</td></tr>
 * <tr><td valign="top">\c leftFillChar</td><td valign="top">Fill space to left of number with this character.</td><td valign="top">' '</td></tr>
 * <tr><td valign="top">\c leftDigitSeparator</td><td valign="top">String that separates digits left of decimal point into groups.</td><td valign="top">""</td></tr>
 * <tr><td valign="top">\c leftGroupSize</td><td valign="top">Number of digits per group left of the decimal point</td><td valign="top">3</td></tr>
 * <tr><td valign="top">\c nearLeftText</td><td valign="top">String which goes just to the left of the number. Most often used for printing a currency symbol.</td><td valign="top">""</td></tr>
 * <tr><td valign="top">\c nearRightText</td><td valign="top">String which goes just to the right of the number. Most often used for printing a currency symbol.</td><td valign="top">""</td></tr>
 * <tr><td valign="top">\c rightDigitSeparator</td><td valign="top">String which separates digits right of decimal point into groups.</td><td valign="top">""</td></tr>
 * <tr><td valign="top">\c rightFillChar</td><td valign="top">Fill space to right of number with this character.</td><td valign="top">' '</td></tr>
 * <tr><td valign="top">\c rightGroupSize</td><td valign="top">Number of digits per group right of the decimal point.</td><td valign="top">3</td></tr>
 * <tr><td valign="top">\c roundMethod</td><td valign="top">The rounding mode used to truncate decimal places (\c PLAIN, \c LEFT, \c RIGHT, \c BANKERS, \c TRUNCATE are allowed).</td><td valign="top">\c PLAIN</td></tr>
 * <tr><td valign="top">\c roundPlaces</td><td valign="top">Number of decimal places to round if \c doRounding attribute is \c true. Negative numbers are allowed. If the \c allDecimals attribute is \c false, and the \c decimalPlaces attribute is less than \c roundPlaces, then the number is rounded to the number of digits indicated by \c decimalPlaces.</td><td valign="top">0</td></tr>
 * <tr><td valign="top">\c showDecimalPoint</td><td valign="top">If \c true, the decimal point is printed when exactly zero decimal places are displayed.</td><td valign="top">\c false</td></tr>
 * <tr><td valign="top">\c width</td><td valign="top">If the \c fixedWidth attribute is \c true, this determines the width of the formatted number.</td><td valign="top">0</td></tr>
 * </table>
 *
 * @section RWDecimalFormat_picture_strings Picture Strings
 *
 * A picture string uses text characters and their relative positions to
 * indicate how to set formatting attributes. The width attribute is set
 * to the length of the picture string. Other attributes are changed
 * depending on the characters in the picture. The table below lists valid
 * formatting characters. Most formatting characters can be placed in one
 * of four positions:
 *
 * - At the far left of the picture string. Attributes here have their effect at the left of the number.
 *
 * - Immediately to the left of the separator character. Attributes here have their effect just to the left of the number's digits.
 *
 * - Immediately to the right of the separator character. Attributes here have their effect just to the right of the number's digits.
 *
 * - At the far right of the picture string. Attributes here have their effect at the right edge of the number.
 *
 * <table> <caption align="top">Table 4: Valid formatting characters</caption>
 * <tr><th valign="top">Char</th><th valign="top">Affect</th><th valign="top">Illustration</th><th valign="top">Example number</th><th valign="top">Formatted number</th></tr>
 * <tr><td valign="top">\@</td><td valign="top">Indicates start of picture format string. Everything before the @ is taken as leading text.</td><td valign="top">Num=\@____</td><td valign="top">4.32</td><td valign="top">\c "Num=4.32"</td></tr>
 * <tr><td valign="top">_</td><td valign="top">Padding to make the width correct.</td><td valign="top"><tt>_________<br>123456789</tt></td><td valign="top">12.346</td><td valign="top">\c "  12.346"</td></tr>
 * <tr><td valign="top">.</td><td valign="top">Indicates where the decimal point goes, and separates left from right formatting.</td><td valign="top">______.__</td><td valign="top">12.346</td><td valign="top">\c "  12.35"</td></tr>
 * <tr><td valign="top">#</td><td valign="top">Separates left from right formatting.</td><td valign="top">_(_\#_)_</td><td valign="top">-12.3</td><td valign="top">\c " (12.3)"</td></tr>
 * <tr><td valign="top"> (</td><td valign="top">Use '()' as the negative sign.</td><td valign="top">(____.__)</td><td valign="top">-12.3</td><td valign="top">\c "(  12.3)"</td></tr>
 * <tr><td valign="top">)</td><td valign="top">Use '()' as the negative sign.</td><td valign="top">_(___._)_</td><td valign="top">-12.3</td><td valign="top">\c "  (12.3) "</td></tr>
 * <tr><td valign="top">-</td><td valign="top">Use '-' as the negative sign, with position as indicated.</td><td valign="top">-_____.__<br>_____.__-</td><td valign="top">-12.34<br>-12.34</td><td valign="top">\c "-   12.34"<br>\c "  12.34-"</td></tr>
 * <tr><td valign="top">+</td><td valign="top">Use '+' as the plus sign, with position as indicated.</td><td valign="top">_+____.__</td><td valign="top">12.34</td><td valign="top">\c "  +12.34"</td></tr>
 * <tr><td valign="top">0</td><td valign="top">Set padding character to '0', rather than blank. Affects either before or after the decimal point.</td><td valign="top">+0___.__0</td><td valign="top">12.34</td><td valign="top">\c "+0012.340"</td></tr>
 * <tr><td valign="top">L</td><td valign="top">Left justify the number.</td><td valign="top">L_____.__</td><td valign="top">12.34</td><td valign="top">\c " 12.34  "</td></tr>
 * <tr><td valign="top">C</td><td valign="top">Center justify the number.</td><td valign="top">C_____.__</td><td valign="top">12.34</td><td valign="top">\c "  12.34 "</td></tr>
 * <tr><td valign="top">,</td><td valign="top">Separate characters before/after the decimal point into groups of three.</td><td valign="top">,_____.__</td><td valign="top">1234.98</td><td valign="top">\c " 1,234.98"</td></tr>
 * <tr><td valign="top">$</td><td valign="top">Add the currency symbol '$' in the position indicated.</td><td valign="top">$_____.__</td><td valign="top">12.34</td><td valign="top">\c "$   12.34"</td></tr>
 * </table>
 *
 * @section RWDecimalFormat_formatting Formatting Wide Numbers
 *
 * If the width attribute is smaller than needed for the number to be output,
 * the output is marked with one or more width overflow characters
 * (currently &quot;<tt>*</tt>&quot; is used). If the number would have to be truncated to the
 * left of the decimal point in order to fit in the specified width, then the
 * entire field is filled with width overflow characters. If the number can be
 * made to fit by truncating at or to the right of the decimal place, then the
 * number is output with a single trailing overflow character to indicate
 * that truncation took place. For example, the following statements produce
 * the strings shown:
 * @code
 *     RWDecimalFormat("________")("1234.567") -> "  1234.567"
 *     RWDecimalFormat("______")("1234.567")   -> "  1234.*"
 *     RWDecimalFormat("___")("1234.567")      -> "***"
 * @endcode
 */
class RW_DCML_SYMBOLIC RWDecimalFormat : public RWDecimalFormatScope
{
public:
    /**
     * Constructs an RWDecimalFormat with default attributes.
     */
    RWDecimalFormat();

    /**
     * Constructs an RWDecimalFormat object using the picture string
     * provided.
     */
    RWDecimalFormat(const char* fmt);

    /**
     * @copydoc allDecimals(Sign) const
     */
    bool           fixedWidth(Sign) const;

    /**
     * @copydoc allDecimals(Sign) const
     */
    unsigned       width(Sign) const;

    /**
     * @copydoc allDecimals(Sign) const
     */
    Justification  justification(Sign) const;

    /**
     * @copydoc allDecimals(Sign) const
     */
    bool           doRounding(Sign) const;

    /**
     * @copydoc allDecimals(Sign) const
     */
    RWDecimalBase::RoundingMethod
    roundMethod(Sign) const;

    /**
     * @copydoc allDecimals(Sign) const
     */
    int            roundPlaces(Sign) const;

    /**
     * @copydoc allDecimals(Sign) const
     */
    bool           showDecimalPoint(Sign) const;

    /**
     * This function \e returns the current value of the indicated
     * attribute. If the \c Sign parameter is RWDecimalFormat::POSITIVE,
     * the attribute value for formatting positive numbers is returned.
     * If the \c Sign parameter is RWDecimalFormat::NEGATIVE, the
     * attribute value for formatting negative numbers is returned.
     * If the value of \c Sign is RWDecimalFormat::BOTH and the attribute
     * values for formatting positive and negative values are different,
     * an exception is thrown. Otherwise, the common value is returned.
     * See the \ref RWDecimalFormat_attributes section for details on how
     * each attribute affects formatting.
     */
    bool           allDecimals(Sign) const;

    /**
     * @copydoc allDecimals(Sign) const
     */
    int            decimalPlaces(Sign) const;

    /**
     * @copydoc allDecimals(Sign) const
     */
    RWCString      decimalSeparator(Sign) const;

    /**
     * @copydoc allDecimals(Sign) const
     */
    RWCString      leftDigitSeparator(Sign) const;

    RWCString      decimalSeperator(Sign s) const {
        return decimalSeparator(s);
    }
    RWCString      leftDigitSeperator(Sign s) const {
        return leftDigitSeparator(s);
    }

    /**
     * @copydoc allDecimals(Sign) const
     */
    unsigned       leftGroupSize(Sign) const;

    /**
     * @copydoc allDecimals(Sign) const
     */
    RWCString      rightDigitSeparator(Sign) const;

    RWCString      rightDigitSeperator(Sign s) const {
        return rightDigitSeparator(s);
    }

    /**
     * @copydoc allDecimals(Sign) const
     */
    unsigned       rightGroupSize(Sign) const;

    /**
     * @copydoc allDecimals(Sign) const
     */
    RWCString      nearLeftText(Sign) const;

    /**
     * @copydoc allDecimals(Sign) const
     */
    RWCString      nearRightText(Sign) const;

    /**
     * @copydoc allDecimals(Sign) const
     */
    char           leftFillChar(Sign) const;

    /**
     * @copydoc allDecimals(Sign) const
     */
    char           rightFillChar(Sign) const;

    /**
     * @copydoc allDecimals(Sign) const
     */
    RWCString      farLeftText(Sign) const;

    /**
     * @copydoc allDecimals(Sign) const
     */
    RWCString      farRightText(Sign) const;

    /* Functions to set attributes.  Be careful when changing the
       nearLeftText attribute that you don't accidentally eliminate the
       negative sign */

    /**
     * This function sets the attribute associated with this formatter
     * object. If the \a Sign parameter is RWDecimalFormat::POSITIVE,
     * the attribute value for formatting positive numbers is set. If
     * the \a Sign parameter is RWDecimalFormat::NEGATIVE, the attribute
     * value for formatting negative numbers is set. If the value of
     * \a Sign is RWDecimalFormat::BOTH, the attribute value for
     * formatting both positive and negative numbers is set. See the
     * \ref RWDecimalFormat_attributes section to learn how each attribute
     * affects formatting.
     */
    void           setFixedWidth(bool x, Sign = BOTH);

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setWidth(unsigned x, Sign = BOTH);

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setJustification(Justification x, Sign = BOTH);

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setDoRounding(bool x, Sign = BOTH);

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setRoundMethod(RWDecimalBase::RoundingMethod x, Sign = BOTH);

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setRoundPlaces(int x, Sign = BOTH);

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setShowDecimalPoint(bool x, Sign = BOTH);

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setAllDecimals(bool x, Sign = BOTH);

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setDecimalPlaces(int x, Sign = BOTH);

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setDecimalSeparator(const RWCString& x, Sign = BOTH);

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setLeftDigitSeparator(const RWCString& x, Sign = BOTH);

    void           setDecimalSeperator(const RWCString& x, Sign s = BOTH) {
        setDecimalSeparator(x, s);
    }

    void           setLeftDigitSeperator(const RWCString& x, Sign s = BOTH) {
        setLeftDigitSeparator(x, s);
    }

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setLeftGroupSize(unsigned x, Sign = BOTH);

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setRightDigitSeparator(const RWCString& x, Sign = BOTH);

    void           setRightDigitSeperator(const RWCString& x, Sign s = BOTH) {
        setRightDigitSeparator(x, s);
    }

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setRightGroupSize(unsigned x, Sign = BOTH);

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setLeftFillChar(char x, Sign = BOTH);

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setRightFillChar(char x, Sign = BOTH);

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setNearLeftText(const RWCString& x, Sign = BOTH);

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setNearRightText(const RWCString& x, Sign = BOTH);

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setFarLeftText(const RWCString& x, Sign = BOTH);

    /**
     * @copydoc setFixedWidth(bool,Sign)
     */
    void           setFarRightText(const RWCString& x, Sign = BOTH);

    void           appendFarRightText(const RWCString& x, Sign = BOTH);
    void           appendNearRightText(const RWCString& x, Sign = BOTH);
    void           appendFarLeftText(const RWCString& x, Sign = BOTH);
    void           appendNearLeftText(const RWCString& x, Sign = BOTH);

    /**
     * Changes the interpretation of commas and decimal points in the
     * picture strings of RWDecimalFormat. This method should only be
     * called if the current locale, initialized at the start of a program,
     * is not desired. This method does \e not affect the input/output
     * parsing of RWDecimal and RWDecimalPortable objects; it only affects
     * output for the \e current RWDecimalFormat object. For more
     * information, see the section on localization in the <i>Currency
     * Module User's Guide</i>.
     */
    void           setLocale(const RWLocaleSnapshot& newLocale);

    /**
     * Formats the number and returns the resulting string. The number
     * must consist of an optional plus or minus sign followed by a
     * sequence of digits. The digit sequence may contain a decimal
     * point.
     */
    RWCString     operator()(const char*) const;

    /**
     * Formats the number and returns the resulting string.
     */
    RWCString     operator()(const RWDecimalPortable&) const;

    void printState(std::ostream&);

    /**
     * Formats the number using the format indicated and returns a string.
     * Since there is an automatic type conversion from \c char* to
     * RWDecimalFormat you can use this operator in expressions such
     * as:
     * @code
     *    RWCString s = "$___.__" << x;
     * @endcode
     */
    friend RWCString RW_DCML_MEMBER_EXPORT operator <<(const RWDecimalFormat&, const RWDecimalPortable&);

private:
    RWPosDecimalFormat positiveFormat_;
    RWPosDecimalFormat negativeFormat_;
};

#endif  // RW_CURRENCY_DECIO_H
