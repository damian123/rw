/*
 * $Id: match.cpp,v 6.3 1994/06/28 22:25:37 nevis Exp $
 *
 * Author:    Allen I. Holub 
 *
 * (c) C Gazette. May be used freely as long as author and publication are
 * acknowledged 
 *
 * Rogue Wave Software, Inc.
 * PO Box 2328
 * Corvallis, OR  97339 
 *
 * ---------------------------------------------------------------------- 
 *
 * $Log: match.cpp,v $
 * Revision 6.3  1994/06/28  22:25:37  nevis
 * Eliminated unused variable start in match.cpp
 *
 * Revision 6.2  1994/06/10  00:39:53  kevinj
 * Bug #482: ".*" matches newlines.
 *
 * Revision 6.1  1994/04/15  19:07:12  vriezen
 * Move all files to 6.1
 *
 * Revision 2.19  1994/04/05  01:49:13  myersn
 * simplify doccl(), fix boundary error on reverse sequences (e.g. "[b-a]").
 *
 * Revision 2.18  1994/04/01  14:10:20  vriezen
 * Eliminate trigraph from 2.9 rcs log entry.
 *
 * Revision 2.17  1994/02/02  23:58:38  myersn
 * remove special case test for match against empty string, to fix bug with
 * anchored closures.
 *
 * Revision 2.16  1993/09/14  00:41:55  myersn
 * prevent running off end of strings
 *
 * Revision 2.16  1993/09/14  00:05:32  myersn
 * prevent walking off the ends of strings
 *
 * Revision 2.15  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 2.14  1993/08/01  03:57:51  myersn
 * minor cleanup.
 *
 * Revision 2.13  1993/07/29  06:07:39  myersn
 * tolerate nulls & 8-bit chars better, also escape sequences.
 *
 * Revision 2.12  1993/06/10  00:39:48  myersn
 * remove skipping the first digit of octal constants.
 *
 * Revision 2.11  1993/05/13  23:53:28  myersn
 * fix some 8-bit uncleanliness and null intolerance.
 *
 * Revision 2.10  1993/05/06  02:35:08  myersn
 * change matchs() to rwmatchs(), makepat() to rwmakepat();
 * also, fix the closure-eats-at-least-one-char bug (finally!).
 *
 * Revision 2.9  1993/03/25  05:46:14  myersn
 * fix is????? () bug for 8 bit chars & Solaris.
 *
 * Revision 2.8  1993/03/17  20:35:28  keffer
 * patternType -> RWPatternType
 *
 * Revision 2.7  1993/02/11  03:15:50  keffer
 * Cast NULL to char* where appropriate to satisfy Glock.
 *
 * Revision 2.6  1993/02/06  03:49:51  keffer
 * Changed name _defs.h to defmisc.h to avoid collision with Borland header file
 *
 * Revision 2.5  1993/01/29  20:12:46  myersn
 * extend regexp to be 8-bit clean.
 *
 * Revision 2.4  1992/11/17  19:38:27  griswolf
 * fixed problem with dereference of NULL pointer
 *
 * Revision 2.3  1992/11/17  19:18:47  keffer
 * Now "const correct", resulting in correct name mangling.
 *
 * Revision 2.2  1992/11/11  02:34:55  myersn
 * changed from C to C++ for systems with no C compiler.
 *
 * Revision 2.1  1992/11/02  18:24:59  griswolf
 * Fixed problem with match of whole string in matchs()
 *
 * Revision 2.0  1992/10/23  03:36:08  keffer
 * RCS Baseline version
 *
 *    Rev 1.6   07 Jul 1992 11:08:14   KEFFER
 * Added function prototypes for hex2bin and oct2bin.
 * 
 *    Rev 1.5   28 May 1992 17:29:10   KEFFER
 * Suppressed unfounded use-before-set warning.
 * 
 *    Rev 1.4   26 May 1992 15:00:42   KEFFER
 * Suppressed unfounded Zortech warning.
 * 
 *    Rev 1.2   05 Nov 1991 14:07:54   keffer
 * Added ANSI style prototypes for Zortech and Borland.
 * 
 *    Rev 1.1   28 Oct 1991 09:24:20   keffer
 * Include file path now <rw/xxx.h>
 * 
 *    Rev 1.0   28 Jul 1991 09:27:54   keffer
 * Initial PVCS revision
 * 
 * Edited 1/27/91.  makepat changed to fill a pattern space provided
 * by the calling routine, rather than allocate one off the heap.
 *
 * Edited 12/11/90 Now works on System V.  Rogue Wave.
 *
 * Edited 12/10/90 to correct some assumptions that chars are unsigned by
 * default.  Now works with Zortech. 
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "defmisc.h"

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: match.cpp,v $ $Revision: 6.3 $ $Date: 1994/06/28 22:25:37 $");

/* Metacharacters in the input:         */
#define BOL     '^'		/* start-of-line anchor                 */
#define EOL     '$'		/* end-of-line anchor                   */
#define ANY     '.'		/* matches any character                */
#define CCL     '['		/* start a character class              */
#define CCLEND  ']'		/* end a character class                */
#define NCCL    '^'		/* negates character class if 1st char. */
#define CLOSURE '*'		/* Kleene closure (matches 0 or more)   */
#define PCLOSE  '+'		/* Positive closure (1 or more)         */
#define OPT     '?'		/* Optional closure (0 or 1)            */

typedef enum action {		/* These are put in the pattern string  */
  				/* to represent metacharacters.         */
  M_BOL =    (0x8000 | '^'), 
  M_EOL =    (0x8000 | '$'),
  M_ANY =    (0x8000 | '.'),
  M_CCL =    (0x8000 | '['),
  M_OPT =    (0x8000 | '?'),
  M_CLOSE =  (0x8000 | '*'),
  M_PCLOSE = (0x8000 | '+'),
  M_END =    (0x8000 | 0)     // end of pattern
} action;

#if 1
#define ISHEXDIGIT(x) isxdigit((unsigned char)x)
#else
#define ISHEXDIGIT(x) (isdigit(x)                       \
                            || ('a'<=(x) && (x)<='f')   \
                            || ('A'<=(x) && (x)<='F')   )
#endif

#define ISOCTDIGIT(x) ('0'<=(x) && (x)<='7')

/*----------------------------------------------------------------------*/
#define MAPSIZE 16		/* need this many RWPatternType elements for
				 * character class bit map */

/*
 * Advance a pointer into the pattern template 
 * to the next pattern element, this is a +1 for
 * all pattern elements but M_CCL, where you
 * to skip past both the M_CCL character and the
 * bitmap that follows that character
 */

inline void ADVANCE(const RWPatternType*& pat)
{
  if (*pat++ == (RWPatternType) M_CCL)
    pat += MAPSIZE;
}

/*
 * Bitmap functions. Set bit b in the map and
 * test bit b to see if it was set previously.
 */

static void SETBIT(unsigned char b, RWPatternType* map)
  { map[b >> 4] |= 1 << (b & 0x0f); }
static int TSTBIT(unsigned char b, const RWPatternType* map)
  { return map[b >> 4] & (1 << (b & 0x0f)); }

/*----------------------------------------------------------------------*/
#define E_NONE       0		/* Possible return values from pat_err. */
#define E_ILLEGAL    1		/* Set in makepat() to indicate prob-   */
#define E_NOMEM      2		/* lems that came up while making the   */
#define E_PAT        3		/* pattern template.                    */

/*----------------------------------------------------------------------*/

static const char*  doccl(RWPatternType*, const char*);
static int          hex2bin(int);
static int          oct2bin(int);
static int          omatch(const char**, size_t*,
		           const RWPatternType*, const char*);
static const char*  patcmp(const char*, size_t,
                           const RWPatternType*, const char*);
static int          esc(const char**);

/*----------------------------------------------------------------------*/
int
rwmakepat
(
 const char*     exp,	/* Regular expression */
 RWPatternType*  pat,	/* Assembled compiled pattern */
 int             maxpat	/* Length of pat */
)
{
  /*
    * Make a pattern template from the string pointed to by exp. Stop when
    * '\0' is found in exp.  The pattern template is assembled
    * in pat whose length is given by maxpat.
    *
    * Return:
    * E_ILLEGAL       Illegal input pattern.
    * E_NOMEM         out of memory.
    * E_PAT           pattern too long. 
    */
  
  RWPatternType* cur;		/* pointer to current pattern element  */
  RWPatternType* prev;		/* pointer to previous pattern element */
  int            Error = E_ILLEGAL;
  
  if (!*exp)
    goto exit;
  
  if (*exp == CLOSURE || *exp == PCLOSE || *exp == OPT)
    goto exit;
  
  Error = E_NOMEM;
  if (!pat) goto exit;		/* Check for bad pat */
  
  prev = cur = pat;
  Error = E_PAT;
  
  while (*exp) {
    
    if (cur >= &pat[maxpat - 1]) goto exit;
    
    switch (*exp) {
    case ANY:
      *cur = (RWPatternType)M_ANY;
      prev = cur++;
      ++exp;
      break;
      
    case BOL:
      *cur = (cur == pat) ? (RWPatternType)M_BOL : (unsigned char)*exp;
      prev = cur++;
      ++exp;
      break;
      
    case EOL:
      *cur = (!exp[1]) ? (RWPatternType)M_EOL : (unsigned char)*exp;
      prev = cur++;
      ++exp;
      break;
      
    case CCL:
      if (((cur - pat) + MAPSIZE) >= maxpat)
	goto exit;		/* not enough room for bit map */
      prev = cur;
      *cur++ = (RWPatternType)M_CCL;
      exp = doccl(cur, exp);
      if (*exp != CCLEND) goto exit;
      ++exp;
      cur += MAPSIZE;
      break;
      
    case OPT:
    case CLOSURE:
    case PCLOSE:
      switch (*prev) {
      case M_BOL:
      case M_EOL:
      case M_OPT:
      case M_PCLOSE:
      case M_CLOSE:
	goto exit;
      }
      
      memmove( prev+1, prev, (cur-prev)*sizeof(*cur));
      *prev = (RWPatternType) (*exp == OPT) ?    M_OPT :
                              (*exp == PCLOSE) ? M_PCLOSE : 
                                                 M_CLOSE;
      ++cur;
      ++exp;
      break;
      
    default:
      prev = cur;
      *cur++ = esc(&exp);
      break;
    }
  }

  *cur = (RWPatternType)M_END;
  Error = E_NONE;

 exit:
  return Error;
}

/*----------------------------------------------------------------------*/

const char *
rwmatchs
(
 const char*           str,
 size_t                slen,
 const RWPatternType*  pat,
 const char**          startpat
)
{
  if (!pat) return 0;
  const char* endp = 0;
  if (*pat == (RWPatternType)M_BOL) {
    // the rest has to match directly
    endp = patcmp(str, slen, pat+1, str);
  } else {
    // scoot along the string until it matches, or no more string
    const char* start = str;
    while ((endp = patcmp(str, slen, pat, start)) == 0 && slen != 0)
      ++str, --slen;
  }
  *startpat = str;
  return endp;
}

/*----------------------------------------------------------------------*/
static const char *
doccl
(
 RWPatternType*  map,
 const char*     src
)
{
  /*
   * Set bits in the map corresponding to characters specified in the src
   * character class.  
   */

  int negative;

  ++src;			/* skip past the [          */
  negative = (*src == NCCL);
  if (negative)			/* check for negative ccl   */
    ++src;
  memset(map, 0, MAPSIZE*sizeof(*map));	/* bitmap initially empty       */

  while (*src && *src != CCLEND) {
    unsigned char first = esc(&src);
    SETBIT(first, map);
    if (*src == '-' && src[1] && src[1] != CCLEND) {
      ++src;			/* skip to end-of-sequence char */
      unsigned char last = esc(&src);
      if (first <= last)  while (first < last ) SETBIT(++first, map);
      else                while (last  < first) SETBIT(last++,  map);
    }
  }

  if (negative) {
    for (int i = MAPSIZE; --i >= 0;)
      *map++ ^= ~0;		/* invert all bits */
  }

  return src;
}

/*----------------------------------------------------------------------*/
static const char *
patcmp
(
 const char*          str,
 size_t               slen,
 const RWPatternType* pat,
 const char*          start
)
{
  /*
   * Like strcmp, but compares str against pat. Each element of str is
   * compared with the template until either a mis-match is found or the end
   * of the template is reached. In the former case a 0 is returned; in the
   * latter, a pointer into str (pointing after the last character in the
   * matched pattern) is returned. start points at the first character in
   * the string, which might not be the same thing as str if the search
   * started in the middle of the string. 
   */

  if (!pat)			/* make sure pattern is valid   */
    return 0;

  while (*pat != (RWPatternType)M_END) {
    if (*pat == (RWPatternType)M_OPT) {

       // Zero or one matches. It doesn't matter if omatch fails---it will
       // advance str past the character on success, though. Always advance
       // the pattern past both the M_OPT and the operand. 
      omatch(&str, &slen, ++pat, start);
      ADVANCE(pat);

    } else if (*pat != (RWPatternType)M_CLOSE &&
               *pat != (RWPatternType)M_PCLOSE)    {

       // Do a simple match. Note that omatch() fails if there's still
       // something in pat but we're at end of string. 

      if (!omatch(&str, &slen, pat, start))
        return 0;
      ADVANCE(pat);

    } else {			/* Process a Kleene or positive closure */

      if (*pat++ == (RWPatternType)M_PCLOSE)	/* one match required */
        if (!omatch(&str, &slen, pat, start))
          return 0;

      /* Match as many as possible, zero is okay */

      const char* bocl = str;		/* beginning of closure string. */
      while (slen && omatch(&str, &slen, pat, start))
	;
      ADVANCE(pat);  // skip over the closure
      if (*pat == (RWPatternType)M_END)
	break;

      // 'str' now points to the character that made made us fail. Try to
      // process the rest of the string. If the character following the
      // closure could have been in the closure (as in the pattern "[a-z]*t")
      // the final 't' will be sucked up in the while loop. So, if the match
      // fails, back up a notch and try to match the rest of the string
      // again, repeating this process until we get back to the
      // beginning of the closure. The recursion goes as many levels
      // deep as there are closures in the pattern. 

      const char* end;
      while ((end = patcmp(str, slen, pat, start)) == 0) {
	++slen, --str;
	if (str < bocl) break;
      }
      return end; 

    }  // closure
  }  // while (*pat != M_END)

  /*
   * omatch() advances str to point at the next character to be matched. So
   * str points at the character following the last character matched when
   * you reach the end of the template. The exceptions are templates
   * containing only a BOLN or EOLN token. In these cases omatch doesn't
   * advance.
   */

  return str;
}

/*----------------------------------------------------------------------*/
static int 
omatch
(
 const char**          strp,
 size_t*               slenp,
 const RWPatternType*  pat,
 const char*           start
)
{
  /*
   * Match one pattern element, pointed at by pat, against the character at
   * **strp. Return 0 on a failure, 1 on success. *strp is advanced to skip
   * over the matched character on a successful match. Closure is handled one
   * level up by patcmp(). 
   *
   * "start" points at the character at the left edge of the line. This might
   * not be the same thing as *strp if the search is starting in the middle
   * of the string. An end-of- line anchor matches end of string only.
   */

  switch (*pat) {
  // Match beginning of line:
  case M_BOL:   return (*strp == start); 

  // Match end of line:
  case M_EOL:   return (*slenp == 0);

  // Notice: cases above don\'t advance.
  // Match any except newline
  case M_ANY: if (**strp == '\n') return 0; // Bug #482
    break;

  // Set match:
  case M_CCL: if (*slenp == 0 || !TSTBIT(**strp, pat + 1)) return 0;
    break;

  // Literal match:
  default:    if (*slenp == 0 || (unsigned char) **strp != *pat)  return 0;
    break;
  }

  ++*strp;
  --*slenp;
  return 2;
}

/*------------------------------------------------------------*/

static int hex2bin(int c)
{
  /* Convert the hex digit represented by 'c' to an int. 'c'
    * must be one of: 0123456789abcdefABCDEF
    */
  return (isdigit(c) ? (c)-'0': ((toupper((unsigned char)c))-'A')+10)  & 0xf;
}

/*------------------------------------------------------------*/

static int oct2bin(int c)
{
  /* Convert the hex digit represented by 'c' to an int. 'c'
    * must be a digit in the range '0'-'7'.
    */
  return ( ((c)-'0')  &  0x7 );
}

/*------------------------------------------------------------*/

static int
esc(const char** s)
{
     /* Map escape sequences into their equivalent symbols. Return
      * the equivalent ASCII character.* s is advanced past the
      * escape sequence. If no escape sequence is present, the
      * current character is returned and the string is advanced by
      * one. The following are recognized:
      *
      *  \b     backspace
      *  \f     formfeed
      *  \n     newline
      *  \r     carriage return
      *  \s     space
      *  \t     tab
      *  \e     ASCII ESC character ('\033')
      *  \DDD   number formed of 1-3 octal digits
      *  \xDD   number formed of 1-2 hex digits
      *  \^C    C = any letter. Control code
      */

     int rval;

     if( **s != '\\' )
          rval = *( (*s)++ );
     else {
          ++(*s);                                 /* Skip the \ */
          switch( toupper((unsigned char)**s) ) {
            case '\0':  rval = '\\';             break;
            case 'B':   rval = '\b' ;            break;
            case 'F':   rval = '\f' ;            break;
            case 'N':   rval = '\n' ;            break;
            case 'R':   rval = '\r' ;            break;
            case 'S':   rval = ' '  ;            break;
            case 'T':   rval = '\t' ;            break;
            case 'E':   rval = '\033';           break;

            case '^':   
              rval = *++(*s) ;
              rval = toupper((unsigned char)rval) - '@' ;
              break;

            case 'X':   
              rval = 0;
              ++(*s);
              if( ISHEXDIGIT(**s) ) {
                   rval  = hex2bin((unsigned char) *(*s)++ );
              }
              if( ISHEXDIGIT(**s) ) {
                   rval <<= 4;
                   rval  |= hex2bin((unsigned char) *(*s)++ );
              }
              --(*s);
              break;

            default:
              if( !ISOCTDIGIT(**s) )
                   rval = **s;
              else {
                   rval = oct2bin( *(*s)++ );
                   if( ISOCTDIGIT(**s) ) {
                        rval <<= 3;
                        rval  |= oct2bin( *(*s)++ );
                   }
                   if( ISOCTDIGIT(**s) ) {
                        rval <<= 3;
                        rval  |= oct2bin( *(*s)++ );
                   }
                   --(*s);
              }
              break;
          }
          ++(*s);
     }
     return (unsigned char)rval;
}
