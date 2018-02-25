/*
 * Implementation of the RWBench class.
 *
 * $Id: bench.cpp,v 6.3 1994/07/18 20:40:21 jims Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 *
 * (c) Copyright 1989, 1990, 1991, 1992, 1993, 1994 Rogue Wave Software, Inc.
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, Rogue Wave Software, Inc., which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 * Notwithstanding any other lease or license that may pertain to, or
 * accompany the delivery of, this computer software and information, the
 * rights of the Government regarding its use, reproduction and disclosure
 * are as set forth in Section 52.227-19 of the FARS Computer
 * Software-Restricted Rights clause.
 * 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 52.227-7013.
 * 
 * This computer software and information is distributed with "restricted
 * rights."  Use, duplication or disclosure is subject to restrictions as
 * set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial
 * Computer Software-Restricted Rights (April 1985)."  If the Clause at
 * 18-52.227-74 "Rights in Data General" is specified in the contract,
 * then the "Alternate III" clause applies.
 *
 ***************************************************************************
 *
 *
 ***************************************************************************
 *
 * $Log: bench.cpp,v $
 * Revision 6.3  1994/07/18  20:40:21  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:07:35  vriezen
 * Updated Copyright.
 *
 * Revision 6.1  1994/04/15  19:04:52  vriezen
 * Move all files to 6.1
 *
 * Revision 2.8  1994/03/08  23:53:12  jims
 * Add strings for Sun C++ and DEC C++
 *
 * Revision 2.7  1994/03/04  07:43:23  jims
 * Extend possible versions of Borland to include 4.0
 *
 * Revision 2.6  1993/09/14  16:47:30  keffer
 * Added Symantec and WATCOM to the list of known compilers.
 *
 * Revision 2.5  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.4  1993/03/11  21:03:11  keffer
 * Added test for BC3.1.
 *
 * Revision 2.3  1993/02/07  17:48:43  keffer
 * Protects against divide by zero.
 *
 * Revision 2.1  1992/11/15  22:07:11  keffer
 * Removed use of macro NL
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.2   12 Mar 1992 19:04:18   KEFFER
 * Microsoft C/C++: test for _M_I86LM for large memory model, etc.
 * 
 *    Rev 1.1   12 Mar 1992 18:30:08   KEFFER
 * Ported to Microsoft C/C++
 * 
 *    Rev 1.0   11 Mar 1992 14:07:50   KEFFER
 * Initial revision.
 */

#include "rw/bench.h"
#include "rw/defs.h"
#include "rw/rstream.h"
STARTWRAP
#include <stdlib.h>
ENDWRAP

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: bench.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:40:21 $");

#ifdef __TURBOC__
#  ifdef __BORLANDC__
#    if __BORLANDC__ <= 0x200
       static const char* compiler = "Borland C++ V2.0 ";
#    else
#      if __BORLANDC__ >= 0x400 && __BORLANDC__ < 0x410
         static const char* compiler = "Borland C++ V3.0 ";
#      else
#        if __BORLANDC__ >= 0x410  && __BORLANDC__ < 0x452
           static const char* compiler = "Borland C++ V3.1 ";
#        else
#          if __BORLANDC__ >= 0x452
             static const char* compiler = "Borland C++ V4.0 ";
#          else
             static const char* compiler = "Borland C++ ";
#          endif
#        endif
#      endif
#    endif
#  else
     static const char* compiler = "Turbo C++ ";
#  endif
#else
#  ifdef __SC__
     static const char* compiler = "Symantec C++ ";
#  else
#    ifdef _MSC_VER
       static const char* compiler = "Microsoft C/C++ ";
#    else
#      ifdef __WATCOMC__
         static const char* compiler = "Watcom C/C++ ";
#      else
#        ifdef __SUNPRO_CC
           static const char* compiler = "Sun C++ ";
#        else	
#          ifdef __DECCXX
             static const char* compiler = "DEC C++ ";
#          else	
             static const char* compiler = "C++ ";
#          endif
#        endif
#      endif
#    endif
#  endif
#endif


/* Get the memory model (MS-DOS) */
#if defined(__LARGE__) || defined(_M_I86LM)
  static const char* memmodel = "Large";
#else
#  if defined(__COMPACT__) || defined(_M_I86CM)
    static const char* memmodel = "Compact";
#  else
#    if defined(__MEDIUM__) || defined(_M_I86MM)
      static const char* memmodel = "Medium";
#    else
#      if defined(__SMALL__) || defined(_M_I86SM)
        static const char* memmodel = "Small";
#      else
        static const char* memmodel = 0;
#      endif
#    endif
#  endif
#endif

RWBench::RWBench(double duration, unsigned long il, const char* mach) :
  machine_(mach),
  timeToTest_(duration),
  innerLoops_(il),
  delta_(0)
{
}

void RWBench::go()
{
  RWTimer timer;	// Construct a timer
  outerLoops_ = 0;	// Keep track of the number of times doLoop() was executed
  timer.start();	// Start the timer

  do {
    doLoop(innerLoops_);	// Execute the inner loops
    outerLoops_++;		// Increment the number of outer loops performed
  } while ( timer.elapsedTime() < timeToTest_ );

  timer.stop();

  // Now get the overhead:

  RWTimer overhead;
  unsigned long i = outerLoops_;
  overhead.start();
  while(i--){
    idleLoop(innerLoops_);
    if (overhead.elapsedTime() >= timeToTest_) break;
  }
  overhead.stop();

  delta_ = timer.elapsedTime() - overhead.elapsedTime();
}

void RWBench::parse(int argc, char* argv[])
{
  if (argc>1) timeToTest_ = atof(argv[1]);
  if (argc>2) innerLoops_ = (unsigned long)atol(argv[2]);
  if (argc>3) machine_    = argv[3];
}

void RWBench::idleLoop(unsigned long n)
{
  while(n--){ /* no-op, this is supposed to test overhead */ }
}

void RWBench::where(ostream& s) const
{
  s << compiler;
  if (memmodel) s << memmodel << " memory model.";
  s << endl;
  if (machine_) s << machine_ << endl;
  else          s << endl;
}

void RWBench::what(ostream& s) const
{
  s << "Nothing\n";
}

void RWBench::report(ostream& s) const
{
  where(s);
  what(s);
  
  s << endl;
  s << "Iterations:                 " << outerLoops_ << endl;
  s << "Inner loop operations:      " << innerLoops_ << endl;
  s << "Total operations:           " << ops() << endl;
  s << "Elapsed (user) time:        " << time() << endl;
  
  if (time())
  {
    if (opsRate()<1000)
    {
      s << "Operations per second:      " << opsRate();
    } else if (kiloOpsRate()<1000)
    {
      s << "Kilo-operations per second: " << kiloOpsRate();
    } else
    {
      s << "Mega-operations per second: " << megaOpsRate();
    }
  }
  else
  {
    s << "Operations per second:      ********";
  }
  s << "\n\n" << flush;
}

double
RWBench::setDuration(double d)
{
  double old = timeToTest_;
  timeToTest_ = d;
  return old;
}

unsigned long
RWBench::setInnerLoops(unsigned long l)
{
  unsigned long old = innerLoops_;
  innerLoops_ = l;
  return old;
}

double        RWBench::time() const        { return delta_; }

unsigned long RWBench::outerLoops() const  { return outerLoops_; }

double        RWBench::ops() const         { return (double)outerLoops_*(double)innerLoops_; }

double        RWBench::opsRate() const     { return ops()/time(); }

double        RWBench::kiloOpsRate() const { return opsRate()/1000; }

double        RWBench::megaOpsRate() const { return kiloOpsRate()/1000; }

