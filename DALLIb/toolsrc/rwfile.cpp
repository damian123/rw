/*
 * Class RWFile encapsulates ANSI-C file operations.   
 *
 * $Id: rwfile.cpp,v 6.8 1994/07/18 20:51:00 jims Exp $
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
 ****************************************************************************
 *
 * $Log: rwfile.cpp,v $
 * Revision 6.8  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.7  1994/07/12  23:30:45  nevis
 * Turns unlink() into remove() on VMS and High C++ for OS/2
 *
 * Revision 6.6  1994/07/12  22:06:43  nevis
 * Changed location of sys/stat.h for MetaWare under OS/2.
 *
 * Revision 6.5  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.4  1994/06/17  17:29:08  vriezen
 * Use RWVECTOR_DELETE instead of delete []
 *
 * Revision 6.3  1994/06/13  19:49:13  vriezen
 * Bug #429.  Fixed dtor to use delete [] and declared copy ctor and op= as private.
 *
 * Revision 6.2  1994/06/02  18:52:07  foote
 * Port to Symantec 7.0 on Macintosh
 *
 * Revision 6.1  1994/04/15  19:07:37  vriezen
 * Move all files to 6.1
 *
 * Revision 2.17  1994/02/05  02:02:17  jims
 * Add huge pointer support for character reads and writes
 *
 * Revision 2.16  1993/09/16  22:53:09  keffer
 * Updated manifest macro for Zortech to Symantec's.
 *
 * Revision 2.15  1993/09/16  03:02:29  keffer
 * Added CSet++ to the list of compilers that hide access() in <io.h>.
 *
 * Revision 2.14  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.13  1993/08/31  23:20:19  myersn
 * eliminate RWThrow in case where open fails -- user should use isValid().
 *
 * Revision 2.12  1993/08/03  18:24:01  dealys
 * Ported to MPW C++ 3.3
 *
 * Revision 2.11  1993/05/31  21:46:32  keffer
 * New messaging architecture for localization
 *
 * Revision 2.10  1993/05/29  17:13:18  keffer
 * Added wchar_t* versions for Read() and Write().
 *
 * Revision 2.9  1993/05/26  00:25:48  keffer
 * Ported to Microsoft C++ under NT.
 *
 * Revision 2.8  1993/05/18  00:48:57  keffer
 * Introduced new exception handling classes
 *
 * Revision 2.7  1993/04/09  20:57:09  keffer
 * Indexing now done using size_t
 *
 * Revision 2.6  1993/03/25  01:51:39  keffer
 * Added optional "mode" argument to the constructor;
 * Added member function "isValid()".
 *
 * Revision 2.4  1993/03/15  02:54:56  keffer
 * Simplified header file inclusion.
 *
 * Revision 2.2  1992/11/26  23:06:26  myersn
 * add definitions for wchar_t functions
 *
 * Revision 2.1  1992/11/19  05:06:42  keffer
 * Introduced new <rw/compiler.h> macro directives
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.17   09 Sep 1992 12:43:24   KEFFER
 * Ported to TopSpeed C++.
 * 
 *    Rev 1.16   29 May 1992 09:46:56   KEFFER
 * Introduced CPP_ANSI_RECURSION macro
 * 
 *    Rev 1.15   28 May 1992 17:59:14   KEFFER
 * Ported to Metaware High-C/C++
 * 
 *    Rev 1.14   27 May 1992 18:09:10   KEFFER
 * RWMEMCK compatible.
 * 
 *    Rev 1.13   12 May 1992 08:32:14   KEFFER
 * Corrected Erase().
 * 
 *    Rev 1.12   04 Mar 1992 10:27:52   KEFFER
 * Oregon Software uses special case for IsEmpty()
 * 
 *    Rev 1.11   13 Nov 1991 13:29:00   keffer
 * 
 *    Rev 1.9   05 Nov 1991 14:06:34   keffer
 * Removed RWXXXXErr macros --- now done by error package
 * 
 *    Rev 1.8   29 Oct 1991 13:59:14   keffer
 * Late ZTC now has correct access() prototype.
 * 
 *    Rev 1.7   17 Oct 1991 09:15:36   keffer
 * Changed include path to <rw/xxx.h>
 * 
 *    Rev 1.5   31 Jul 1991 09:25:38   keffer
 * VMS now uses remove(const char*) instead of unlink().
 * 
 *    Rev 1.4   29 Jul 1991 14:33:50   keffer
 * Added check for VMS when #including <sys/file.h>
 * 
 *    Rev 1.3   29 Jul 1991 11:03:22   keffer
 * Made Exists(const char*) a static function.
 * 
 *    Rev 1.2   27 Jul 1991 21:28:52   keffer
 * Uses extern "C" wrappers around access() and unlink()
 * 
 */

/*
 * Under Unix, there there is no difference between text and binary 
 * files.  However, under MS-DOS, and some other operating systems, 
 * files opened for *text* output will use a CR/LF convention to end 
 * text lines, but do nothing for binary files.  Because RWFile is 
 * intended to model *binary* files, we must shut off this translation.
 * This requires opening the file in an explicit "binary" mode.  
 * This situation is detected by the macro "RW_CRLF_CONVENTION".
 */

#include "rw/defs.h"
#include "rw/rwfile.h"
#include "rw/rwerr.h"
#include "rw/coreerr.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: rwfile.cpp,v $ $Revision: 6.8 $ $Date: 1994/07/18 20:51:00 $");

STARTWRAP
#include <stdlib.h>
#include <string.h>
#include <stdio.h>	/* Looking for fileno() and, in the case of VMS, remove() */
#ifndef macintosh
# if defined(__HIGHC__) && defined(__OS2__)
#  include <types.h>
#  include <stat.h>
# else
#  include <sys/types.h>        /* Defines type used in <sys/stat.h> */
#  include <sys/stat.h>         /* Looking for statbuf */
# endif
#endif
#if defined(__SC__) || defined(__TURBOC__) || defined(RW_MSC_BACKEND) || defined(__HIGHC__) || defined(__JPI__) || defined(__IBMCPP__) && !defined(THINK_CPLUS)
#  include <io.h>	/* Looking for access() and unlink() in here. */
#else
   extern "C" {
     int access(const char*, int);
     int unlink(const char*); /* remove() may be more portable in the future */
   }
#endif
#ifdef unix
#  include <fcntl.h>
#endif
ENDWRAP

#ifdef THINK_CPLUS
#  include <unix.h>
#endif

// Microsoft C++ under NT does not have "access", instead calls 
// it "_access":
#ifdef THINK_CPLUS
#  define rwaccess stat
#else
#  ifdef RW_MSC_BACKEND
#    define rwaccess _access
#  else
#    define rwaccess access
#  endif
#endif

#ifdef RWMEMCK
   // If you're compiling w RWMEMCK, you'd better have Tools.h++,
   // or this will fail! 
# include "rw/tooldefs.h"	
# ifndef RW_NO_CPP_RECURSION
#   define new rwnew
# endif
#endif

#define READ(s)			fread((char*)&s, sizeof(s), 1, filep)
#define READVEC(s,count) 	fread((char*)s, sizeof(*s), count, filep)
#define WRITE(s)		fwrite((char*)&s, sizeof(s), 1, filep)
#define WRITEVEC(s,count)	fwrite((const char*) s, sizeof(*s), count, filep)

#ifdef RW_CRLF_CONVENTION
  static const char* updateMode = "rb+";
  static const char*    newMode = "wb+";
#else
  static const char* updateMode = "r+";
  static const char*    newMode = "w+";
#endif

RWFile::RWFile(const char* name, const char* mode)
 : filename(rwnil),
   filep(rwnil),
   access_mode(0)
{
  if (mode)
    filep = fopen(name, mode);
  else
  {
    // No mode specified.  Use default actions:
    filep = fopen(name, updateMode);	// Open existing file for update
    if (filep == NULL)			// Still not opened?
      filep = fopen(name, newMode);	// Open new file
  }
  filename = new char [strlen(name) + 1];
  strcpy(filename, name);
#ifdef unix  
  if(filep && (access_mode = fcntl(fileno(filep), F_GETFL, 0)) >= 0)
    access_mode = access_mode & O_ACCMODE;
#endif
}

RWFile::~RWFile()
{
  if (filep != NULL) fclose(filep);
  RWVECTOR_DELETE( strlen(filename)+1 ) filename;
}

RWBoolean RWFile::Exists() { return rwaccess(filename, 0) >= 0; }

RWBoolean RWFile::Exists(const char* name)
{
#if (defined(__ZTC__) && (__ZTC__ < 0x300)) || defined(__HIGHC__) || defined(THINK_CPLUS)
  // Metaware and early Zortech forget the "const" modifier for access():
  return rwaccess((char*)name, 0) >= 0;
#else
  return rwaccess(name, 0) >= 0;
#endif
}

RWBoolean RWFile::Read(char&    t)           { return READ(t)==1; }
RWBoolean RWFile::Read(short&   t)           { return READ(t)==1; }
RWBoolean RWFile::Read(int&     t)           { return READ(t)==1; }
RWBoolean RWFile::Read(long&    t)           { return READ(t)==1; }
#ifndef RW_NO_OVERLOAD_UCHAR
RWBoolean RWFile::Read(unsigned char&  t)    { return READ(t)==1; }
#endif
RWBoolean RWFile::Read(unsigned short& t)    { return READ(t)==1; }
RWBoolean RWFile::Read(unsigned int&   t)    { return READ(t)==1; }
RWBoolean RWFile::Read(unsigned long&  t)    { return READ(t)==1; }
RWBoolean RWFile::Read(float&          t)    { return READ(t)==1; }
RWBoolean RWFile::Read(double&         t)    { return READ(t)==1; }
RWBoolean RWFile::Read(char*   p, size_t N)  { return READVEC(p,N)==N; }
RWBoolean RWFile::Read(short*  p, size_t N)  { return READVEC(p,N)==N; }
RWBoolean RWFile::Read(int*    p, size_t N)  { return READVEC(p,N)==N; }
RWBoolean RWFile::Read(long*   p, size_t N)  { return READVEC(p,N)==N; }
RWBoolean RWFile::Read(float*  p, size_t N)  { return READVEC(p,N)==N; }
RWBoolean RWFile::Read(double* p, size_t N)  { return READVEC(p,N)==N; }
#ifndef RW_NO_OVERLOAD_WCHAR
RWBoolean RWFile::Read(wchar_t& t)           { return READ(t)==1; }
RWBoolean RWFile::Read(wchar_t* p, size_t N) { return READVEC(p,N)==N; }
#endif

#ifdef RW_SEGMENTED_ARCHITECTURE
RWBoolean RWFile::Read(char rwhuge* p, unsigned long N)  
{ 
  unsigned long        remaining = N;      // chars still to be read 
  unsigned long        roomLeftInSegment;  // max bytes before segment overflow
  const int            maxCount = 32766;   // max chars safe to read in one go
  unsigned             offset;
  int                  count;
  const unsigned long  segmentLength = 65536;

  while (remaining) {
    offset = *((unsigned*)&p);	// offset of p from start of segment
    roomLeftInSegment = segmentLength - offset;

    // count should be the least of remaining, maxCount, and roomLeftInSegment
    count = (maxCount < remaining)      ? maxCount : (int) remaining;
    count = (count < roomLeftInSegment) ? count    : (int) roomLeftInSegment;

    if (READVEC(p, count) != count)
      return FALSE;           // don't continue if not all bytes written

    remaining -= count;
    p += count;
  }
  return TRUE;
}
#endif

RWBoolean RWFile::Read(char* string)
{
  int c;
  while (1) {
    c = fgetc(filep);
    if( c==EOF || c=='\0') break;
    *string++ = (char)c;
  }
  *string = '\0';
  return TRUE;
}

RWBoolean RWFile::Write(char t)    { return WRITE(t)==1; }
RWBoolean RWFile::Write(short t)   { return WRITE(t)==1; }
RWBoolean RWFile::Write(int t)     { return WRITE(t)==1; }
RWBoolean RWFile::Write(long t)    { return WRITE(t)==1; }
#ifndef RW_NO_OVERLOAD_UCHAR
RWBoolean RWFile::Write(unsigned char  t) { return WRITE(t)==1; }
#endif
RWBoolean RWFile::Write(unsigned short t) { return WRITE(t)==1; }
RWBoolean RWFile::Write(unsigned int   t) { return WRITE(t)==1; }
RWBoolean RWFile::Write(unsigned long  t) { return WRITE(t)==1; }
RWBoolean RWFile::Write(float          t) { return WRITE(t)==1; }
RWBoolean RWFile::Write(double         t) { return WRITE(t)==1; }
RWBoolean RWFile::Write(const char*   p, size_t N)  { return WRITEVEC(p,N)==N; }
RWBoolean RWFile::Write(const short*  p, size_t N)  { return WRITEVEC(p,N)==N; }
RWBoolean RWFile::Write(const int*    p, size_t N)  { return WRITEVEC(p,N)==N; }
RWBoolean RWFile::Write(const long*   p, size_t N)  { return WRITEVEC(p,N)==N; }
RWBoolean RWFile::Write(const float*  p, size_t N)  { return WRITEVEC(p,N)==N; }
RWBoolean RWFile::Write(const double* p, size_t N)  { return WRITEVEC(p,N)==N; }
#ifndef RW_NO_OVERLOAD_WCHAR
RWBoolean RWFile::Write(wchar_t t)                  { return WRITE(t)==1; }
RWBoolean RWFile::Write(const wchar_t* p, size_t N) { return WRITEVEC(p,N)==N; }
#endif

#ifdef RW_SEGMENTED_ARCHITECTURE
RWBoolean RWFile::Write(const char rwhuge* p, unsigned long N)  
{ 
  unsigned long        remaining = N;      // chars still to be written
  unsigned long        roomLeftInSegment;  // max bytes before segment overflow
  const int            maxCount = 32766;   // max chars safe to write in one go
  unsigned             offset;
  int                  count;
  const unsigned long  segmentLength = 65536;

  while (remaining) {
    offset = *((unsigned*)&p);	// offset of p from start of segment
    roomLeftInSegment = segmentLength - offset;

    // count should be the least of remaining, maxCount, and roomLeftInSegment
    count = (maxCount < remaining)      ? maxCount : (int) remaining;
    count = (count < roomLeftInSegment) ? count    : (int) roomLeftInSegment;

    if (WRITEVEC(p, count) != count)
      return FALSE;	             // don't continue if not all bytes written

    remaining -= count;
    p += count;
  }
  return TRUE;
}
#endif


/*
 * Write up to and including the null terminator.
 * Be careful of non-terminated strings!
 */
RWBoolean RWFile::Write(const char* string) {
  return Write(string, strlen(string)+1);
}

/****************************************************************
 *								*
 *		Miscellaneous positioning & query functions	*
 *								*
 ****************************************************************/

long RWFile::CurOffset() { return ftell(filep); }

RWBoolean RWFile::Eof()   { return feof(filep); }

#if defined(VMS) || ( defined(__HIGHC__) && defined(__OS2__) )
#define unlink(file) remove(file)
#endif

RWBoolean RWFile::Erase() {
  return fclose(filep) != EOF && unlink(filename) == 0 && 
#ifdef RW_CRLF_CONVENTION
      (filep = fopen(filename, "wb+")) != NULL;
#else
      (filep = fopen(filename, "w+")) != NULL;
#endif
}

#if defined(VMS) || ( defined(__HIGHC__) && defined(__OS2__) )
#undef unlink
#endif

RWBoolean RWFile::Error()  { return ferror(filep); }

RWBoolean RWFile::Flush() { return fflush(filep) != EOF; }

RWBoolean RWFile::IsEmpty() {
#if defined(__OREGON__) || defined(applec)
  int dummy;
  int nb;

  nb = fread((char*)&dummy, sizeof(dummy), 1, filep);
  if( nb )   fseek(filep, -nb, 1);	// Undo what we did

  return !nb;
#else
  // Thanks to Michael Kent, Gainesville, FL for the following fix:
  struct stat statbuf;
  fstat(fileno(filep), &statbuf);
  return statbuf.st_size == 0;
#endif
}

RWBoolean RWFile::SeekTo(long offset) { return fseek(filep, offset, 0) >= 0; }	
RWBoolean RWFile::SeekToEnd()         { return fseek(filep, 0, 2) >= 0; }

RWBoolean RWFile::isReadOnly() {
#ifdef unix
  return access_mode == O_RDONLY;
#else
  return FALSE;
#endif
}

RWBoolean RWFile::isWriteOnly() {
#ifdef unix
  return access_mode == O_WRONLY;
#else
  return FALSE;
#endif
}

RWBoolean RWFile::isReadWrite() {
#ifdef unix
  return access_mode == O_RDWR;
#else
  return FALSE;
#endif
}

