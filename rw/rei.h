#ifndef RW_TOOLS_REI_H
#define RW_TOOLS_REI_H 1



/**********************************************************************
 *
 * Declarations for class RWTRegularExpressionImp and related classes.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/rei.h#1 $
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
#include <rw/retrait.h>

#if !defined(RW_DISABLE_DEPRECATED)

#if !defined(RW_NO_EXCEPTIONS)
RW_SUPPRESS_DEPRECATED_WARNINGS

#  include <rw/tvordvec.h>

#  undef RWC
#  define RWC(name, dummy) \
    static charT name() \
      {return RWTRegularExpressionTraits<charT>::name();}

#  if !defined(RW_NO_POSIX_RE)

#    include <sys/types.h>

#    include <regex.h>
#    define RW_REGCOMP(preg, pattern, cflags) \
  ::regcomp(preg, pattern, cflags);
#    define RW_REGEXEC(preg, string, stringLength, nmatch, pmatch, eflags) \
  ::regexec(preg, string, nmatch, pmatch, eflags);

#  else
// The following #defines were done to minimize the passing of unused
// arguments on the stack.  These will have to be changed if the
// member functions are changed.
#    define RW_REGCOMP(preg, pattern, cflags) \
  rwRegcomp(pattern);
#    define RW_REGEXEC(preg, string, stringLength, nmatch, pmatch, eflags) \
  rwRegexec(string, stringLength, pmatch);
// stringLength is added to take care of embedded nulls.

#    include <rw/regexp.h>              // Fast RE class, RWCRegexp.

/////////////////////////////////////////////////////////////////////////////
//
// RWRERange
//
//  Class used to store range info.  This class isn't nested in
//  RWTRegularExpression because some compilers don't understand a nested
//  class name qualified with a templatized class name.
//
//  Used in RWTRegularExpressionImp during regular expression compilation and
//  during regular expression pattern matching.
//
//  RWRERange(const RWRERange& other) and operator=(const RWRERange& other)
//  are omitted because automatically-defined memberwise copy can safely and
//  perhaps more quickly copy present data members.
//
/////////////////////////////////////////////////////////////////////////////

class RW_TOOLS_SYMBOLIC RWRERange
{
public:
    size_t              start_;    // First state in range.
    size_t              end_;      // Last state in range.

    RWRERange(size_t first = RW_NPOS, size_t last = RW_NPOS)
        : start_(first), end_(last) {}

    bool   operator==(const RWRERange& other) const {
        return (start_ == other.start_ && end_ == other.end_);
    }

    bool   operator<(const RWRERange& other) const {
        return (start_ < other.start_ ||
                (start_ == other.start_ && end_ < other.end_));
    }

    bool   isValid() {
        return (start_ != RW_NPOS && end_ != RW_NPOS);
    }

private:
    RWRERange&  operator=(size_t) {
        return *this;
    }
    // Dummy to prevent assignment of size_t to RWRERange.

};

/////////////////////////////////////////////////////////////////////////////
//
// RWRESubexpression
//
//  Class stores both the starting and ending states of parenthesized
//  subexpressions in a given regular expression.
//
//  Used by RWTValSortedVector, which requires well-defined copy, assignment
//  and equality semantics as well as a default constructor.
//
//  Used in RWTRegularExpressionImp during regular expression compilation and
//  during regular expression pattern matching.
//
/////////////////////////////////////////////////////////////////////////////

class RW_TOOLS_SYMBOLIC RWRESubexpression : public RWRERange
{

public:
    RWRESubexpression(size_t first = 0, size_t last = 0)
        : RWRERange(first, last) {}

    ~RWRESubexpression() {}

    bool           operator==(const RWRESubexpression& other) const {
        return RWRERange::operator==(other);
    }

};

/////////////////////////////////////////////////////////////////////////////
//
// RWTREToken
//
//  Class supplies regular expression tokens for parsing.
//
//  Used in RWTRegularExpressionImp during regular expression compilation.
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> class  RWTREToken
{
    typedef typename RWTRegularExpressionTraits<charT>::tStringT    stringT;

public:
    RWTREToken(const stringT& s) : tIndex_(0), tString_(s) {
        Set();
    }
    RWTREToken() :
        tToken_(RWTRegularExpressionTraits<charT>::nullCharT()),
        tIndex_(0),
        tIsEmpty_(true)
    {}

    RWTREToken(const RWTREToken<charT>& token) :
        tToken_(token.tToken_), tIndex_(token.tIndex_),
        tString_(token.tString_), tIsEmpty_(token.tIsEmpty_) { }

    charT operator()() {
        return
            (
                tIsEmpty_ ?
                (tToken_ = RWTRegularExpressionTraits<charT>::nullCharT()) :
                tToken_
            );
    }
    charT operator*() {
        return
            (
                tIsEmpty_ ?
                (tToken_ = RWTRegularExpressionTraits<charT>::nullCharT()) :
                tToken_
            );
    }
    RWTREToken<charT>&   operator=(const stringT& s) {
        tString_ = s;
        tIndex_ = 0;
        Set();
        return *this;
    }
    RWTREToken<charT>&   operator=(const charT* cp) {
        tString_ = cp;
        tIndex_ = 0;
        Set();
        return *this;
    }
    void          operator++(int) {
        Set();
        tIndex_++;
    }
    void          operator--(int) {
        tIndex_--;
        Set();
    }

    RWTREToken<charT> next() {
        RWTREToken<charT> token(*this);
        token++;
        return token;
    }

    bool     isEmpty() {
        return tIsEmpty_;
    }
    size_t  position() {
        return tIndex_ - 1;
    }
    size_t  position(const size_t i) {
        tIndex_ = i;
        Set();
        tIndex_++;
        return tIndex_ - 1;
    }

private:
    // Used instead of tString_[tIndex_] to reduce time overhead of
    // operator().
    charT       tToken_;

    size_t      tIndex_;
    stringT     tString_;
    bool   tIsEmpty_;
    void        Set() {
        // tIndex now points to next token to get.

        if (tIndex_ < tString_.length()) {
            tToken_ = tString_[tIndex_];
            tIsEmpty_ = false;
        }
        else {
            tToken_ = RWTRegularExpressionTraits<charT>::nullCharT();
            tIsEmpty_ = true;
        }
    }

}; // RWTREToken

/////////////////////////////////////////////////////////////////////////////
//
// RWTRENode
//
//  Class is node of finite state machine contained in regex_t.
//
//  Used in RWTRegularExpressionImp during regular expression compilation
//  and during pattern matching.
//
//  RWRENode(const RWRERange& other) and operator=(const RWRENode& other)
//  are omitted because automatically-defined memberwise copy can safely and
//  perhaps more quickly copy present data members.
//
/////////////////////////////////////////////////////////////////////////////

template<class charT> class  RWRENode
{
public:

    charT     input_;           // Symbol that fires transition.
    size_t    next1_;           // First transition to next state.
    size_t    next2_;           // 2nd transition to next state.
    size_t    symbolSetIndex_;  // Pointer to character class.
    bool matchAny_;        // Match any character.
    int       anchor_;          // Flags anchoring match.


    RWRENode
    (
        charT     input,
        size_t    next1             = 0,
        size_t    next2             = 0,
        size_t    symbolSetIndex    = 0,
        bool matchAny          = false,
        int       anchor            = 0
    )
        : input_(input), next1_(next1), next2_(next2),
          symbolSetIndex_(symbolSetIndex), matchAny_(matchAny), anchor_(anchor)
    {}

    RWRENode()
        : input_(RWTRegularExpressionTraits<charT>::nullCharT()),
          next1_(0), next2_(0), symbolSetIndex_(0), matchAny_(false), anchor_(0)
    {}

    ~RWRENode() {}

    bool           operator==(const RWRENode<charT>& other) const {
        return
            input_ == other.input_                    &&
            next1_ == other.next1_                    &&
            next2_ == other.next2_                    &&
            symbolSetIndex_ == other.symbolSetIndex_  &&
            matchAny_ == other.matchAny_              &&
            anchor_ == other.anchor_
            ;
    }

    // Do not use. Given only to appease compilers that instantiate everything:
    bool           operator<(const RWRENode<charT>&) const {
        return false;
    }

};

/////////////////////////////////////////////////////////////////////////////
//
// RWRECompileError
//
// Exception for use within RWTRegularExpressionImp<charT>::rwRegcomp() only.
// Used to keep recursive descent parser clean.
//
/////////////////////////////////////////////////////////////////////////////

class RW_TOOLS_SYMBOLIC RWRECompileError
{
public:
    int no;
    RWRECompileError(int n) : no(n) {}
};

#  endif  // defined(RW_NO_POSIX_RE)


/////////////////////////////////////////////////////////////////////////////
//
// RWTRegularExpressionImp<charT>
//
//  Templatized Regular Expression implementation.
//
/////////////////////////////////////////////////////////////////////////////

#  include <rw/rebase.h>

// Work-around for Onyx PR#30893
#  if defined(REG_NOMATCH) && defined(RW_NO_POSIX_RE)
#    undef REG_NOMATCH
#    undef REG_BADPAT
#    undef REG_ECOLLATE
#    undef REG_ECTYPE
#    undef REG_EESCAPE
#    undef REG_ESUBREG
#    undef REG_EBRACK
#    undef REG_EPAREN
#    undef REG_EBRACE
#    undef REG_BADBR
#    undef REG_ERANGE
#    undef REG_ESPACE
#    undef REG_BADRPT

enum RWREErrorType  /* POSIX.2 error codes found in <regex.h> */
{
    REG_NOMATCH = 1,
    REG_BADPAT,
    REG_ECOLLATE,
    REG_ECTYPE,
    REG_EESCAPE,
    REG_ESUBREG,
    REG_EBRACK,
    REG_EPAREN,
    REG_EBRACE,
    REG_BADBR,
    REG_ERANGE,
    REG_ESPACE,
    REG_BADRPT
};
#  endif //defined(REG_NOMATCH) && defined(RW_NO_POSIX_RE)

template<class charT> class  RWTRegularExpressionImp
    : public RWREBaseClass
{
    friend class RWTRegularExpression<charT>;

public:

    virtual ~RWTRegularExpressionImp();

protected:  // All members accessed by RWTRegularExpression here are protected.

    typedef typename RWTRegularExpressionTraits<charT>::tStringT         stringT;

#  if  defined(RW_NO_POSIX_RE)

    typedef typename
    RWTRegularExpressionTraits<charT>::symbolSetT       symbolSetT;

    typedef typename
    RWTRegularExpressionTraits<charT>::symbolSetTVector symbolSetTVector;

#  endif // RW_NO_POSIX_RE

    // Compiles RE. This declaration needs to be here rather than in
    // Compilation section because otherwise the Solaris Cafe compilation
    // of RWTRegularExpressionImp(const RWTRegularExpressionImp<charT>&
    // other); won't be able to find compile().
    void    compile();

#  if defined(RW_NO_POSIX_RE)
    // These declarations need to be here rather than in Compilation section
    // because otherwise the Solaris Cafe compilation of this class can't find the
    // dependent member functions' source.  Essentially, for this to compile
    // under Cafe, Cafe must see declarations of first member function before the
    // declaration of a second member function that calls the first
    // member function.  #ifdef's aren't used to show the alternate positions
    // of the member function declarations because duplication of code can lead
    // to subtle errors if all duplicates aren't kept up to date.

private:
    // POSIX.2 RE finite automaton structure.  Frees space used by
    // compiled RE. This declaration needs to be here rather than in
    // Compilation and Matching Patterns section because otherwise the
    // Solaris Cafe compilation of ~RWTRegularExpressionImp()won't be
    // able to find regfree().
    struct  regex_t {
        // Only re_nsub is required by POSIX.2.
        size_t                re_nsub;          // # of parenthesized subexpressions.
        RWTValOrderedVector<RWRESubexpression>
        subexpression;    // ()'d subexpressions.
        RWTValOrderedVector<RWRENode<charT> >
        node;             // Node of RE finite automaton.
        regex_t() :
            re_nsub(0),
            subexpression(MAX_NO_OF_SUBEXPRESSIONS),
            node(16)
        {}
    };

    // POSIX.2 C-binding. Frees space used by compiled RE. This
    // definition needs to be here rather than in Compilation
    // section because otherwise the Solaris Cafe compilation of
    // ~RWTRegularExpressionImp() won't be able to find regfree().
    void              regfree(regex_t* preg) {
        delete preg;
    }

    enum anchorType {notAnchored = 0, anchorFront, anchorBack}; // Used in RWRENode.

    // Builds automaton nodes.  Used by rwRegcomp().
    void      build
    (
        const charT                symbol,       // Input that fires transition(s).
        const size_t               transition1     = 0,
        const size_t               transition2     = 0,
        const size_t    symbolSetIndex  = 0,            // Character range.
        const bool matchAny        = false,        // "."
        const int       anchor          = notAnchored   // "^" or "$"
    );

    // Sets values of automaton nodes.  Used by rwRegcomp().
    void      set
    (
        const size_t               stateIndex,   // Node of automaton.
        const charT                symbol,       // Input that fires transition(s).
        const size_t               transition1,
        const size_t               transition2,
        const size_t    symbolSetIndex  = 0,            // Character range
        const bool matchAny        = false,        // "."
        const int       anchor          = notAnchored   // "^" or "$".
    );

    // rwRegcomp() recursive descent compiler members.
    size_t          bracketExpression();
    bool       bracketList();
    bool       characterClass();
    bool       collatingElement();
    bool       collatingSymbol();
    bool       endRange();
    bool       equivalenceClass();
    RWRERange       ere(const RWRERange&);
    RWRERange       ereBranch(const RWRERange&);
    RWRERange       ereBranchRest();
    RWRERange       ereExpression(const RWRERange&);
    RWRERange       ereExpressionRest(const RWRERange&);
    RWRERange       ereExpressionToken(const RWRERange&);
    RWRERange       ereRepeat(const RWRERange&);
    RWRERange       ereRest(const RWRERange&);
    bool       expressionTerm();
    bool       followList();
    void            followListRest();
    RWRERange       makeStar(const RWRERange&);
    size_t          oneCharEre();
    size_t          ordinaryChar();
    size_t          quotedChar();
    bool       rangeExpression();
    bool       singleExpression();
    size_t          specialChar();
    bool       startRange();


    int       rwRegcomp(const stringT& pattern);  // RE compiler. Replaces regcomp()
#  endif

    // Clears internal state of regular expression. This declaration
    // needs to be here rather than in Compilation section because
    // otherwise the Solaris Cafe compilation of constructors won't be
    // able to find hexStringToChar().
    void              clear();

    // If you use the default constructor, then you must use operator=
    // before you call the execute() function.
    RWTRegularExpressionImp();


    RWTRegularExpressionImp(const charT* reStringPtr);

    RWTRegularExpressionImp(
        const typename RWTRegularExpressionTraits<charT>::tStringT&);

    RWTRegularExpressionImp(const RWTRegularExpressionImp<charT>&);

    RWTRegularExpressionImp<charT>& operator=(const charT*  other);
    RWTRegularExpressionImp<charT>& operator=
    (const RWTRegularExpressionImp<charT>&  other);
    RWTRegularExpressionImp<charT>& operator=(const stringT& string) {
        reString_ = string;
        compile();
        return *this;
    }

    statusType    status() const {
        return status_;
    }

private:  // All members used only by this class are private.

    //----- Both Regular Expression Compilation and Pattern Matching Members -----

#  if  defined(RW_NO_POSIX_RE)
    // Below are POSIX.2 C bindings for regular expressions.
    // All of these must be kept as members in case user wishes to use
    // Rogue Wave implementations of POSIX.2 C services rather than
    // operating system services.  For example, present POSIX.2 services do not
    // support wchar_t or embedded nulls.

public:  // errorType is public so that RWRECompileError can use it.
    enum errorType      /* POSIX.2 error codes found in <regex.h> */
    {
        REG_NOMATCH = 1, REG_BADPAT, REG_ECOLLATE, REG_ECTYPE, REG_EESCAPE,
        REG_ESUBREG, REG_EBRACK, REG_EPAREN, REG_EBRACE, REG_BADBR,
        REG_ERANGE, REG_ESPACE, REG_BADRPT
    };

private:
    // Members used by RWTRegularExpressionImp's POSIX.2 C-bindings but
    // not POSIX.2 C-bindings.

    const size_t      maxState_;          // The largest possible state in NFA.

    // fake state that tells rwRegexec() to scan in another symbol from
    // the search string.
    const size_t      getNextSymbol_;

    // fake state that tells rwRegexec() to search for a longer leftmost
    // substring.
    size_t            nonacceptingState_;

    // Used to match symbols in range and character class expressions.
    symbolSetTVector  symbolSetVector_;
    RWCRegexp*        fastRePtr_;         // RWCRegexp pointer to "fast" RE class

#  endif  // defined(RW_NO_POSIX_RE)

    // number of unmatched open parenthesis
    size_t            unmatchedOpenParen_;

    // the first char position in the bracket list of a bracket expression
    size_t            bracketListStartPos_;
    bool         compiled_;  // true if re_ is compiled_
    statusType        status_;
    regex_t*          rePtr_;     // POSIX.2-compatible finite automaton.

    // Copy of RE in text form. Need to keep this after
    // compile() is called in order to deep copy this class when
    // defined(RW_NO_POSIX_RE)
    stringT           reString_;

#  if defined(RW_NO_POSIX_RE)

    // Regular expression Nondeterministic Finite Automaton (regex_t) accessors.

    int               anchor(const size_t stateIndex) {
        return rePtr_->node[stateIndex].anchor_;
    }
    RWRENode<charT>   getNode(const size_t stateIndex) {
        return rePtr_->node[stateIndex];
    }
    charT             input(const size_t stateIndex) {
        return rePtr_->node[stateIndex].input_;
    }
    bool         matchAny(const size_t stateIndex) {
        return rePtr_->node[stateIndex].matchAny_;
    }
    size_t            next1(const size_t stateIndex) {
        return rePtr_->node[stateIndex].next1_;
    }
    size_t           next2(const size_t stateIndex) {
        return rePtr_->node[stateIndex].next2_;
    }
    size_t           symbolSetIndex(const size_t stateIndex) {
        if
        (
            rePtr_->node[stateIndex].symbolSetIndex_
            < symbolSetVector_.entries()
        ) {
            return rePtr_->node[stateIndex].symbolSetIndex_;
        }
        else {
            return 0;
        }
    }
#  endif

    //------------------ Regular Expression Compilation Members -----------------

protected:

#  if  defined(RW_NO_POSIX_RE)
    // Below are POSIX.2 C bindings for regular expressions.
    // All of these must be kept as members in case user wishes to use
    // Rogue Wave implementations of POSIX.2 C services rather than
    // operating system services.  For example, present POSIX.2 services do not
    // support wchar_t or embedded nulls.

private:
    enum compileFlags {     // POSIX.2 compile flags.
        REG_E_EXTENDED  = 0x0001,
        REG_E_ICASE     = 0x0002,
        REG_E_NOSUB     = 0x0004,
        REG_E_NEWLINE   = 0x0008
    };

    // Members used by RWTRegularExpressionImp's POSIX.2 C-bindings but
    // not POSIX.2 C-bindings.

    size_t           nextState_;         // The next state to build in the NFA.
    RWTREToken<charT> token_;            // Regex token for rwRegcomp

    // Used by rwRegcomp() to construct symbol sets that are stored in
    // symbolSetVector_ above.
    stringT           symbolSetString_;

#  endif  // defined(RW_NO_POSIX_RE)

    // RWC macros here and in retrait.h are used to retrieve either char
    // or wchar_t versions of metacharacters.
    RWC(a,              'a');
    RWC(b,              'b');
    RWC(bar,            '|');
    RWC(backslash,      '\\');
    RWC(backspace,      '\b');
    RWC(c,              'c');
    RWC(closeBrace,     '}');
    RWC(closeBracket,   ']');
    RWC(closeParen,     ')');
    RWC(colon,          ':');
    RWC(d,              'd');
    RWC(dash,           '-');
    RWC(dollar,         '$');
    RWC(dot,            '.');
    RWC(e,              'e');
    RWC(eight,          '8');
    RWC(equal,          '=');
    RWC(f,              'f');
    RWC(five,           '5');
    RWC(formfeed,       '\f');
    RWC(four,           '4');
    RWC(hat,            '^');
    RWC(n,              'n');
    RWC(newlineChar,    '\n');
    RWC(nine,           '9');
    RWC(one,            '1');
    RWC(openBrace,      '{');
    RWC(openBracket,    '[');
    RWC(openParen,      '(');
    RWC(plus,           '+');
    RWC(questionMark,   '?');
    RWC(r,              'r');
    RWC(seven,          '7');
    RWC(six,            '6');
    RWC(star,           '*');
    RWC(t,              't');
    RWC(tab,            '\t');
    RWC(three,          '3');
    RWC(two,            '2');
    RWC(x,              'x');
    RWC(zero,           '0');

#  if defined(RW_NO_POSIX_RE)

    // nullCharT returns a null character value for any symbol type.
    static charT  nullCharT() {
        return RWTRegularExpressionTraits<charT>::nullCharT();
    }

    // Adds a character class expression match map.  Returns index of
    // new symbol set.  Used by rwRegcomp().
    size_t    addSymbolSet() {
        return RWTRegularExpressionTraits<charT>::addSymbolSet
               (symbolSetVector_, symbolSetString_);
    }


    // Parenthesized subexpression manipulation functions.

    void
    addSubexpression(const size_t start, const size_t end) {
        rePtr_->re_nsub++;
        rePtr_->subexpression.insert(RWRESubexpression(start, end));
    }

    RWRESubexpression
    getSubexpression(const size_t i) {
        return rePtr_->subexpression.operator [](i);
    }

    void
    subexpression(const size_t i, const size_t start, const size_t end) {
        rePtr_->subexpression.operator [](i) = RWRESubexpression(start, end);
    }

    void subexpression(const size_t i, const RWRESubexpression sub) {
        rePtr_->subexpression.operator [](i) = sub;
    }

    size_t         lastSubexpression() {
        return numberOfSubexpressions();
    }
    size_t         numberOfSubexpressions() {
        return rePtr_->re_nsub - 1;
    }
    // RWRESubexpression 0 is whole string.


    //------------------ Pattern Matching Members -------------------------------

    // Below are POSIX.2 C bindings for regular expressions.
    // All of these must be kept as members in case user wishes to use
    // Rogue Wave implementations of POSIX.2 C services rather than
    // operating system services.  For example, present POSIX.2 services do not
    // support wchar_t or embedded nulls.

    enum executeFlags {     //  POSIX.2 execute flags.
        REG_E_NOTBOL  = 0x0010,
        REG_E_NOTEOL  = 0x0020
    };

public:
    // POSIX.2 RE byte offset.  public so can be accessed by regmatch_t.
    typedef size_t regoff_t;

private:

    struct regmatch_t {         // POSIX.2 RE match structure.
        // Byte offset from start of string to start of substring.
        regoff_t  rm_so;

        // Byte offset from start of string of first char after the end
        // of substring.
        regoff_t  rm_eo;
    };

    // Returns true if "symbol" has a match in symbol set pointed to by
    // "setIndex". Used by rwRegexec().
    bool isMatchedInSymbolSet(
        const size_t      setIndex,
        const unsigned char        symbol
    ) {
        if (setIndex < symbolSetVector_.entries())
            return RWTRegularExpressionTraits<charT>::isMatchedInSymbolSet
                   (symbolSetVector_[setIndex], symbol);

        return false;
    }

    // Pattern matching function. Replaces POSIX.2 regexec()
    int       rwRegexec(
        const charT*  searchStringPtr,    // String to be searched.
        size_t        searchStringLength, // Length to search including embedded nulls.
        regmatch_t    pmatch[]            // Array of matches found.
    );
#  endif  // RW_NO_POSIX_RE

public:
    // This member function is down here because some compilers can't
    // find it otherwise.
    size_t index(const typename
                 RWTRegularExpressionTraits<charT>::tStringT&,
                 size_t* = NULL,
                 size_t  = 0);
};

#  undef RWC

/*
 * class RWREState:
 *   Used to store states of NFA on deque.
 */

class RWREState
{
public:
    size_t          number;       // Number of NFA node (stateIndex).
    size_t          nextToMatch;  // Position of next symbol to be matched.

    RWREState(size_t inNumber = 0, size_t inNextToMatch = 0)
        : number(inNumber), nextToMatch(inNextToMatch) { }
    bool operator==(const RWREState& other)const {
        return(number == other.number);
    }
    bool operator!=(const RWREState& other) const {
        return(!operator==(other));
    }
    // some unused methods in stdlib need less-than:
    bool operator<(const RWREState& other) const {
        return number < other.number;
    }
    ~RWREState() { }
};

/*
 *   This is (possibly) the second include of the same ".cc" file. It
 *   may have been included from inside the class body above. Please see the
 *   comments in the class body above and at the beginning of the ".cc" file
 *   if you need to further understand what is accomplished with these macros.
 */

#  if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#    pragma implementation ("rei.cc")
#  endif

#  if defined(RW_NO_IMPLICIT_INCLUSION)
#    include "rw/rei.cc"
#    include "rw/reposix1.cc"
#    include "rw/reposix2.cc"
#  endif


RW_RESTORE_DEPRECATED_WARNINGS
#endif /* !defined(RW_NO_EXCEPTIONS) */

#endif /* RW_DISABLE_DEPRECATED */

#endif /* RW_TOOLS_REI_H */
