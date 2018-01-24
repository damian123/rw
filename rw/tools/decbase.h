#ifndef __RW_DECBASE_TOOLS_H__
#define __RW_DECBASE_TOOLS_H__



/**********************************************************************
 *
 * decbase.h -- RWDecimalBase: provide a scope for the enums used in the
 *              different RWDecimal classes.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/decbase.h#1 $
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

/*
 * RWDecimalBase class
 *
 * The basic use of this class is to provide a scope for the enums used
 * in the different RWDecimal classes.
 *
 * The member state_ would, in a pure design sense, be better included with
 * each individual RWDecimal class so that RWDecimalBase existed purely for
 * scoping.  The trouble with a class purely for scoping is that the compiler
 * will put a dummy variable in it to give it non-zero size.  Obviously, this
 * just won't do, so state_ is included since it is used in all the decimal
 * classes anyway.
 */

/**
 * @ingroup miscellaneous_classes decimal
 *
 * @brief Base class for the family of Rogue Wave decimal classes.
 *
 * The base class for the different RWDecimal classes. This class
 * provides the definition of enums used in the derived classes and
 * in the related classes RWDecimalInexactErr and RWDecimalOverflowErr.
 *
 */
class RW_TOOLS_SYMBOLIC RWDecimalBase
{
public:
    RWDecimalBase() {}

    /**
     * Rounding methods used by RWDecimalPortable::round()
     *
     * The following table shows the results of the different rounding methods.
     *
     * <table>
     * <tr>
     * <td><b>Method</b></td>
     * <td><b>1.25</b></td>
     * <td><b>1.35</b></td>
     * <td><b>1.251</b></td>
     * </tr>
     * <tr>
     * <td>#PLAIN</td>
     * <td>1.2</td>
     * <td>1.3</td>
     * <td>1.2</td>
     * </tr>
     * <tr>
     * <td>#UP</td>
     * <td>1.3</td>
     * <td>1.4</td>
     * <td>1.3</td>
     * </tr>
     * <tr>
     * <td>#DOWN</td>
     * <td>1.2</td>
     * <td>1.3</td>
     * <td>1.2</td>
     * </tr>
     * <tr>
     * <td>#TRUNCATE</td>
     * <td>1.2</td>
     * <td>1.3</td>
     * <td>1.2</td>
     * </tr>
     * <tr>
     * <td>#BANKERS</td>
     * <td>1.2</td>
     * <td>1.4</td>
     * <td>1.3</td>
     * </tr>
     * </table>
     */
    enum RoundingMethod {

        /**
         * Round up on a tie.
         */
        PLAIN = 0,

        /**
         * Always round up.
         */
        UP = 1,

        /**
         * Always round down.
         */
        DOWN = 2,

        /**
         * On a tie, round so that last digit is even.
         */
        BANKERS = 3,

        /**
         * Same as #DOWN.
         */
        TRUNCATE = 2
    };

    /**
     * An enumeration defining the state of the object.
     *
     * Any operation on \c nullstate returns the other operand.
     *
     * Any operation on \e NaN provides a \e NaN result.
     *
     * Any operation on \c ctorError or \c missingstate returns a result with
     * the State \c ctorError or \c missingstate respectively.
     *
     * Operations on \c infinitystate are as follows:
     * <table>
     * <tr>
     * <td><b>Operation</b></td>
     * <td><b>Result</b></td>
     * </tr>
     * <tr>
     * <td>\f$n \div \pm Infinity\f$</td>
     * <td>0</td>
     * </tr>
     * <tr>
     * <td>\f$\pm Infinity \times \pm Infinity\f$</td>
     * <td>\f$\pm Infinity\f$</td>
     * </tr>
     * <tr>
     * <td>\f$Infinity + Infinity\f$</td>
     * <td>\f$Infinity\f$</td>
     * </tr>
     * <tr>
     * <td>\f$Infinity - Infinity\f$</td>
     * <td>\e NaN</td>
     * </tr>
     * <tr>
     * <td>\f$\pm Infinity \div \pm Infinity\f$</td>
     * <td>\e NaN</td>
     * </tr>
     * <tr>
     * <td>\f$\pm Infinity \times 0\f$</td>
     * <td>\e NaN</td>
     * </tr>
     * </table>
     *
     * @note
     * An operation on an object with \c SNaNstate does not generate signals,
     * unless converted to a double or long double and used in double
     * arithmetic.
     */
    enum State          { normal = 0, ///< Normal numeric value.
                          nullstate = 1, ///< Non-numeric value.
                          NaNstate = 2, ///< Quiet NaN.
                          ctorError = 4, ///< Error in construction.
                          missingstate = 8, ///< Non-numeric value.
                          SNaNstate = 16, ///< Signaling NaN.
                          infinitystate = 32 ///< Infinity.
                        };
    /**
     * An enumeration defining the operation that caused the error
     * handler to be called.
     * @see RWDecimalInexactErr and RWDecimalOverflowErr.
     */
    enum Op             { assign, ///< &nbsp;
                          add, ///< &nbsp;
                          sub, ///< &nbsp;
                          mult, ///< &nbsp;
                          div, ///< &nbsp;
                          powop, ///< &nbsp;
                          pow10op, ///< &nbsp;
                          conversion ///< &nbsp;
                        };

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWDecimalBase& rhs) {
        rw_swap(state_, rhs.state_);
    }

protected:
    RWDecimalBase(State s) : state_(s) {}

    State state_;
};

#endif // __RW_DECBASE_TOOLS_H__
