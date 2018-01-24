#ifndef RW_TOOLS_BENCH_H
#define RW_TOOLS_BENCH_H



/**********************************************************************
 *
 * RWBench:  A class to help in the running of benchmarks
 * To use, derive a class from RWBench including a doLoop(unsigned long)
 * function, and a what() function (if you plan to use report()).
 * The doLoop(unsigned long N) function should perform N operations of
 * the type you are trying to benchmark.  RWBench will call this function
 * over and over again until "duration" time has elapsed.  Then it will sum
 * the total number of operations performed.
 * To run, construct an object then call go().  Then call report() to get
 * a summary.  You can call ops() outerLoops(), etc. for more detail.
 * If you wish to correct for overhead, then provide an idleLoop() function
 * which does non-benchmark related calculations.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/bench.h#1 $
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
#include <rw/timer.h>
#include <rw/tools/ristream.h>
#include <rw/tools/rostream.h>
#include <rw/tools/pointer.h>

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

/**
 * @ingroup miscellaneous_classes
 *
 * @brief Abstract base class to facilitate automatic benchmarking
 * of some piece of code.
 *
 * This is an abstract class that can automate the process of benchmarking
 * a piece of code. To use it, derive a class from RWBench, including
 * a definition for the virtual function doLoop(unsigned long N).
 * This function should perform \c N operations of the type
 * that you are trying to benchmark. RWBench will call doLoop()
 * over and over again until a preset amount of time has elapsed.
 * It will then sum the total number of operations performed.
 *
 * To run, construct an instance of your derived class and then
 * call go(). Then call report() to get a standard summary. For
 * many compilers, this summary will automatically include the compiler
 * type and memory model. You can call ops(), outerLoops(), etc.
 * for more detail.
 *
 * If you wish to correct for overhead, then provide an idleLoop()
 * function which should do all non-benchmark-related calculations.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/bench.h>
 * (Abstract base class)
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * This example compares container append operations by benchmarking
 * RWTValOrderedVector<T,A>::append() versus RWTValDlist<T,A>::append().
 *
 * @code
 * #include <cstdlib>
 * #include <iostream>
 *
 * #include <rw/bench.h>    //Benchmark software
 * #include <rw/tvdlist.h>  //RWTValDlist
 * #include <rw/tvordvec.h> //RWTValOrderedVector
 *
 * template <typename C>
 * class TimeAppend : public RWBench
 * {
 * public:
 *     TimeAppend(const RWCString& name) : name_(name) { }
 *
 *     virtual void doLoop(unsigned long);
 *
 *     virtual void idleLoop(unsigned long);
 *
 *     virtual void what(std::ostream& s) const {
 *         s << name_ << "::append() test: \n";
 *     }
 *
 * private:
 *     RWCString name_;
 * };
 *
 * template <typename C>
 * void TimeAppend<C>::doLoop(unsigned long n)
 * {
 *     C container;
 *     for (unsigned long i = 0; i < n; ++i) {
 *         container.append(i);
 *     }
 * }
 *
 * template <typename C>
 * void TimeAppend<C>::idleLoop(unsigned long n)
 * {
 *     C container;
 *     for (unsigned long i = 0; i < n; ++i) {
 *     }
 * }
 *
 *
 * int main(int argc, char** argv)
 * {
 *     std::cout << "Testing container append...\n";
 *
 *     TimeAppend<RWTValOrderedVector<unsigned long> > vec("RWTValOrderedVector<unsigned long>");
 *     vec.parse(argc, argv);
 *     vec.go();
 *     vec.report(std::cout);
 *
 *     TimeAppend<RWTValDlist<unsigned long> > list("RWTValDlist<unsigned long>");
 *     list.parse(argc, argv);
 *     list.go();
 *     list.report(std::cout);
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program Output:
 *
 * @code
 * Testing container append...
 * Microsoft C/C++
 *
 * RWTValOrderedVector<unsigned long>::append() test:
 *
 * Iterations:                 4071
 * Inner loop operations:      1000
 * Total operations:           4.071e+006
 * Elapsed (user) time:        5.00763
 * Kilo-operations per second: 812.959
 *
 * Microsoft C/C++
 *
 * RWTValDlist<unsigned long>::append() test:
 *
 * Iterations:                 4066
 * Inner loop operations:      1000
 * Total operations:           4.066e+006
 * Elapsed (user) time:        5.00763
 * Kilo-operations per second: 811.961
 * @endcode
 */
class RW_TOOLS_GLOBAL RWBench
{

public:

    /**
     * The parameter \a duration is the nominal amount of time that
     * the benchmark should take in seconds. The virtual function
     * doLoop(unsigned long) is called over and over again until at least
     * this amount of time has elapsed. The parameter \a ILO is the number of
     * inner loop operations that should be performed. This parameter is
     * passed in as parameter \c N to \link doLoop() doLoop(N) \endlink.
     * Parameter \a machine is an optional null terminated string that
     * should describe the test environment (perhaps the hardware the
     * benchmark is being run on).
     */
    RWBench(double duration = 5, unsigned long ILO = 1000, const char* machine = 0);

    // Deletes memory allocated for machine_
    virtual ~RWBench();

    /**
     * This is a non-virtual function that provides the same service
     * as \link parse(int, char*[]) parse(int argc, char * argv[])\endlink,
     * but is designed for Windows
     * users. It extracts tokens from the null-terminated command argument
     * provided by Windows, then calls the virtual
     * \link parse(int, char*[]) parse()\endlink for ANSI
     * C command arguments.
     */
    void           parse(const char* args);

    /**
     * This function allows an easy way to change the test duration,
     * number of inner loops and machine description from the command
     * line. \par
     *
     * <table>
     * <tr>
     * <td>\b Argument</td>
     * <td>\b Type</td>
     * <td>\b Description</td>
     * </tr>
     * <tr>
     * <td>\c argv[1]</td>
     * <td>\c double</td>
     * <td>Duration (sec.)</td>
     * </tr>
     * <tr>
     * <td>\c argv[2]</td>
     * <td><tt>unsigned long</tt></td>
     * <td>No. of inner loops</td>
     * </tr>
     * <tr>
     * <td>\c argv[3]</td>
     * <td><tt>const char*</tt></td>
     * <td>Machine</td>
     * </tr>
     * </table>
     */
    virtual void   parse(int argc, char* argv[]);

    // see doxygen bug #627884
    /**
     * This function allows an easy way to change the test duration,
     * number of inner loops and machine description from the command
     * line. \par
     *
     * <table>
     * <tr>
     * <td>\b Argument</td>
     * <td>\b Type</td>
     * <td>\b Description</td>
     * </tr>
     * <tr>
     * <td>\c argv[1]</td>
     * <td>\c double</td>
     * <td>Duration (sec.)</td>
     * </tr>
     * <tr>
     * <td>\c argv[2]</td>
     * <td><tt>unsigned long</tt></td>
     * <td>No. of inner loops</td>
     * </tr>
     * <tr>
     * <td>\c argv[3]</td>
     * <td><tt>const char*</tt></td>
     * <td>Machine</td>
     * </tr>
     * </table>
     */
    virtual void   parse(int argc, const char* argv[]);

    /**
     * Call this function to run the benchmark.
     */
    virtual void   go();

    /**
     * A pure virtual function whose actual definition should be supplied
     * by the specializing class. This function is repeatedly called
     * until a time duration has elapsed. It should perform the operation
     * to be benchmarked \a N times. See the example.
     */
    virtual void   doLoop(unsigned long n) = 0;

    /**
     * This function can help to correct the benchmark for overhead.
     * The default definition merely executes a "for()" loop \a n times.
     * See the example.
     */
    virtual void   idleLoop(unsigned long n);

    /**
     * You can supply a specializing version of this virtual function
     * that provides some detail of what is being benchmarked. It is
     * called by report() when generating a standard report.
     */
    virtual void   what(std::ostream&) const;

    /**
     * This function prints information to the stream about the
     * compiler and memory model that the code was compiled under.
     */
    void           where(std::ostream&) const;     // Print out the machine type and the compiler

    /**
     * Calling this function provides an easy and convenient way of
     * getting an overall summary of the results of a benchmark.
     */
    virtual void   report(std::ostream&) const;

    /**
     * Change the test duration to time \a t.
     */
    double         setDuration(double t);

    /**
     * Change the number of inner loop operations to \a N.
     */
    unsigned long  setInnerLoops(unsigned long N);

    /**
     * Return the current setting for the benchmark test duration. This
     * should not be confused with the function time(), which returns the
     * actual test time.
     */
    double         duration() const {
        return timeToTest_;
    }

    /**
     * This function accesses the name of the machine which is passed
     * into the benchmark object through parse().
     */
    const char*    machine() const {
        return machine_.get();
    }

    /**
     * Returns the current setting for the number of inner loop operations
     * that will be passed into function doLoop(unsigned long N) as
     * parameter \c N.
     */
    unsigned long  innerLoops() const {
        return innerLoops_;
    }

    /**
     * Returns the amount of time the benchmark took, corrected for
     * overhead.
     */
    double         time() const;

    /**
     * Returns the number of times the function doLoop() was called.
     */
    unsigned long  outerLoops() const;

    /**
     * Returns the total number of inner loop operations that were performed
     * (the product of the number of times outerLoops() was called times
     * the number of inner loop operations performed per call).
     */
    double         ops() const;

    /**
     * Returns the number of inner loop operations per second.
     */
    double         opsRate() const;

    double         kiloOpsRate() const;       // Number of thousands of ops per second
    double         megaOpsRate() const;       // Number of millions of ops per second

private:

    RWTScopedPointer<char[]> machine_;  // What machine we're running on
    double         timeToTest_;    // How long should the test take
    unsigned long  innerLoops_;    // Number of inner loop operations to be done
    unsigned long  outerLoops_;    // Number of outer loops actually executed
    double         delta_;         // Actual time (corrected for overhead)

};

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif  /* RW_TOOLS_BENCH_H */
