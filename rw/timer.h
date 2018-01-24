#ifndef RW_TOOLS_TIMER_H
#define RW_TOOLS_TIMER_H


/**********************************************************************
 *
 * $Id: //tools/13/rw/timer.h#1 $
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

/**
 * @ingroup miscellaneous_classes
 *
 * @brief Measures two types of time, system and user time, both of which
 * can be reported as cumulative if desired.
 *
 * This class can measure two types of time: system time and user
 * time, both of which can be reported as cumulative, if desired.
 * The timer has two states: running and stopped. The timer measures
 * the total amount of time spent in the "running" state since it
 * was either constructed or reset.
 *
 * The timer is put into the "running" state by calling member function
 * start(). It is put into the "stopped" state by calling stop().
 *
 * RWTimer uses different APIs on Windows and UNIX; on Windows it
 * uses \c GetProcessTime, whereas on UNIX it uses the \c getrusage
 * system call.
 *
 * The time intervals are stored inside the RWTimer objects as 64-bit
 * integers. The resolution of the timer is microseconds. The wrap-up
 * interval for the counters is > 500,000 years.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/timer.h>
 * RWTimer timer;
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 *
 * #include <iostream>
 * #include <rw/timer.h>
 * #include <rw/tools/datetime.h>
 *
 * int main()
 * {
 *     std::cout << "Starting the timer ...\n";
 *     std::cout << "... and looping for 5 seconds using RWDateTime as clock ...\n";
 *
 *     RWTimer t;
 *     t.start(); // Start the timer
 *
 *     // Loop for 5 seconds:
 *
 *     RWDateTime tmp (RWDateTime::setCurrentTime);
 *     for (; (RWDateTime::now () - tmp) < 5000;) { }
 *
 *     t.stop(); // Stop the timer
 *
 *     std::cout << "The elapsed time was: \n";
 *
 *     std::cout << "System  : "
 *               << double (t.time (RWTimer::systemTime)) / 1000000.0F
 *               << " seconds.\n";
 *
 *     std::cout << "User    : "
 *               << double (t.time (RWTimer::userTime)) / 1000000.0F
 *               << " seconds.\n";
 *
 *     std::cout << "Total   : "
 *               << double (t.time (RWTimer::userTime | RWTimer::systemTime)) / 1000000.0F
 *               << " seconds.\n";
 *
 *     return 0;
 * }
 * @endcode
 */
struct RW_TOOLS_SYMBOLIC RWTimer {

    /**
     * The \c Type type indicates the type of time returned by the time()
     * member function.
     */
    enum Type {

#if !defined(RW_DISABLE_DEPRECATED)

        /**
         * @deprecated As of SourcePro 11, use RWTimer::userTime instead.
         */
        RW_DEPRECATE_ENUMERATOR(USER, "Use RWTimer::userTime instead") = 1,

        /**
         * @deprecated As of SourcePro 11, use RWTimer::systemTime instead.
         */
        RW_DEPRECATE_ENUMERATOR(SYSTEM, "Use RWTimer::systemTime instead") = 2,

#endif

        /**
         * Cumulated user time
         */
        userTime = 1,

        /**
         * Cumulated system time
         */
        systemTime = 2

    };

    /**
     * Constructs a new timer. The timer does not start running until
     * start() is called.
     */
    RWTimer()
        : suser_(0),
          ssys_(0),
          user_(0),
          sys_(0),
          running_(0) {
    }

    /**
     * Puts the timer in the "running" state. Time accumulates while
     * in this state.
     */
    void start();

    /**
     * Puts the timer in the "stopped" state. Time does not accumulate
     * while in this state.
     */
    void stop();

    /**
     * Resets (and stops) the timer.
     */
    void reset() {
        suser_ = user_ = ssys_ = sys_ = 0;
        running_ = 0;
    }

    /**
     * Returns the amount of time, in microseconds, measured by the
     * object. The parameter is an integer bitmask indicating the type
     * of time requested.
     */
    rwint64 time(int = userTime | systemTime) const;

    /**
     * Returns the amount of user and system time, in seconds, that
     * has accumulated while the timer was in the running state.
     */
    double elapsedTime() const;

    /**
     * Returns the number of seconds that have elapsed while the timer
     * was in the running state.
     */
    rwint64 elapsedSeconds() const;

    /**
     * Returns the number of milliseconds that have elapsed while the timer
     * was in the running state.
     */
    rwint64 elapsedMilliseconds() const;

    /**
     * Returns the number of  microseconds that have elapsed while the timer
     * was in the running state.
     */
    rwint64 elapsedMicroseconds() const;

private:

    // time snapshots
    rwint64 suser_;
    rwint64 ssys_;

    // time stores
    rwint64 user_;
    rwint64 sys_;

    char running_;
};

/**
 * @ingroup miscellaneous_classes
 *
 * @brief Measures elapsed wall clock time.
 *
 * This class measures elapsed wall clock time. The timer has two
 * states: running and stopped. The timer measures the total amount
 * of time spent in the "running" state since it was either constructed
 * or reset.
 *
 * The timer is put into the "running" state by calling member function
 * start(). It is put into the "stopped" state by calling stop().
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/timer.h>
 * RWClockTimer timer;
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 *
 * #include <iostream>
 *
 * #include <rw/timer.h>
 * #include <rw/tools/datetime.h>
 *
 * int main()
 * {
 *     std::cout << "Starting the timer ...\n";
 *     std::cout << "... and looping for 5 seconds using RWDateTime as clock ...\n";
 *
 *     RWClockTimer t;
 *     t.start(); // Start the timer
 *
 *     // Loop for 5 seconds:
 *
 *     RWDateTime tmp (RWDateTime::setCurrentTime);
 *     for (; (RWDateTime::now () - tmp) < 5000;) { }
 *
 *     t.stop(); // Stop the timer
 *
 *     std::cout << "The elapsed time was: \n";
 *
 *     std::cout << "Wall    : "
 *               << double (t.elapsedMicroseconds()) /  1000000.0F
 *               << " seconds.\n";
 *
 *     return 0;
 * }
 * @endcode
 */
struct RW_TOOLS_SYMBOLIC RWClockTimer {
    /**
     * Constructs a new timer. The timer does not start running until
     * start() is called.
     */
    RWClockTimer();

    /**
     * Puts the timer in the "running" state. Time accumulates while
     * in this state.
     */
    void start();

    /**
     * Puts the timer in the "stopped" state. Time does not accumulate
     * while in this state.
     */
    void stop();

    /**
     * Resets (and stops) the timer.
     */
    void reset();

    /**
     * Returns the amount of time in seconds that has accumulated while
     * the timer was in the running state.
     */
    double
    elapsedTime() const;

    /**
     * Returns the number of seconds that have elapsed while the timer
     * was in the running state.
     */
    rwint64 elapsedSeconds() const;

    /**
     * Returns the number of milliseconds that have elapsed while the timer
     * was in the running state.
     */
    rwint64 elapsedMilliseconds() const;

    /**
     * Returns the number of microseconds that have elapsed while the timer
     * was in the running state.
     */
    rwint64 elapsedMicroseconds() const;

private:

#if !defined(RW_NO_GETTIMEOFDAY)

    rwint64 snapshot_;
    rwint64 time_;

#elif !defined(RW_NO_QUERYPERFORMANCE)

    void init();

    static int init_;
    static rwint64 ticks_;

    rwint64 snapshot_;
    rwint64 time_;

#else  //   RW_NO_QUERYPERFORMANCE

    rwint64 snapshot_;
    rwint64 time_;

#endif //  !RW_NO_QUERYPERFORMANCE

    char running_;
};

typedef RWClockTimer RWUserTimer;

#endif  /* RW_TOOLS_TIMER_H */
