#ifndef RW_CREGEX_H
#define RW_CREGEX_H



/**********************************************************************
 *
 * cregex.h   = contains the char specialization of RWTRegex
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regex/cregex.h#1 $
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

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

class RWRegexErr;
class RWCString;
template <class E> class RWRegexImp;
template <>
class RW_TOOLS_GLOBAL RWTRegex<char>
{
public:
    typedef char                          RChar;

    // Typedefs for regex iterators: we provide match_iterator,
    // and iterator.  RWTRegex::iterator will be a match iterator.
    // If new iterator types are added, they will need to be given
    // a descriptive prefix, as in match_iterator.
    typedef RWTRegexMatchIterator<char>   iterator;
    typedef RWTRegexMatchIterator<char>   match_iterator;
    typedef std::basic_string<RChar>      RString;

public:
    enum RWTRegexStatus {
        Ok,                         // 0
        MissingEscapeSequence,      // 1
        InvalidHexNibble,           // 2
        InsufficientHex8Data,       // 3
        InsufficientHex16Data,      // 4
        MissingClosingBracket,      // 5
        MissingClosingCurlyBrace,   // 6
        MissingClosingParen,        // 7
        UnmatchedClosingParen,      // 8
        InvalidSubexpression,       // 9
        InvalidDataAfterOr,         // 10
        InvalidDataBeforeOr,        // 11
        ConsecutiveCardinalities,   // 12
        InvalidCardinalityRange,    // 13
        LeadingCardinality,         // 14
        InvalidDecimalDigit,        // 15
        UnmatchedClosingCurly,      // 16

        // Please leave this at the end of the list of enums
        NumberOfStatusCodes
    };

    // Constructors and Destructors
    RWTRegex();
    RWTRegex(const RChar* str, size_t length = size_t(-1));
    RWTRegex(const RWCString& str, size_t length = size_t(-1));
    RWTRegex(const RString& str, size_t length = size_t(-1));

    RWTRegex(const RWTRegex& source);
    RWTRegex& operator= (const RWTRegex& rhs);

#if !defined(RW_NO_RVALUE_REFERENCES)
    RWTRegex(RWTRegex && rhs);
    RWTRegex& operator= (RWTRegex && rhs);
#endif // !RW_NO_RVALUE_REFERENCES

    virtual ~RWTRegex();

    // Member operators
    bool operator< (const RWTRegex& rhs) const;
    bool operator==(const RWTRegex& rhs) const;

    // Return number of sub-expressions
    size_t subCount() const;

    // Search and Match operations
    RWTRegexResult<char> search(const RChar*           str,
                                size_t                 start  = size_t(0),
                                size_t                 length = size_t(-1));

    RWTRegexResult<char> search(const RWCString&       str,
                                size_t                 start  = size_t(0),
                                size_t                 length = size_t(-1));

    RWTRegexResult<char> search(const RString&         str,
                                size_t                 start  = size_t(0),
                                size_t                 length = size_t(-1));

    RWTRegexResult<char> matchAt(const RChar*           str,
                                 size_t                 start  = size_t(0),
                                 size_t                 length = size_t(-1));

    RWTRegexResult<char> matchAt(const RWCString&       str,
                                 size_t                 start  = size_t(0),
                                 size_t                 length = size_t(-1));

    RWTRegexResult<char> matchAt(const RString&         str,
                                 size_t                 start  = size_t(0),
                                 size_t                 length = size_t(-1));

    size_t index(const RChar*           str,
                 size_t*                mLen   = 0,
                 size_t                 start  = size_t(0),
                 size_t                 length = size_t(-1));

    size_t index(const RWCString&       str,
                 size_t*                mLen   = 0,
                 size_t                 start  = size_t(0),
                 size_t                 length = size_t(-1));

    size_t index(const RString&         str,
                 size_t*                mLen   = 0,
                 size_t                 start  = size_t(0),
                 size_t                 length = size_t(-1));

    size_t replace(RWCString&                  str,
                   const RWCString&            replacement,
                   size_t                      count               = 1,
                   size_t                      matchID             = 0,
                   size_t                      start               = size_t(0),
                   size_t                      length              = size_t(-1),
                   bool                   replaceEmptyMatches = true);

    size_t replace(RString&                    str,
                   const RString&              replacement,
                   size_t                      count               = 1,
                   size_t                      matchID             = 0,
                   size_t                      start               = size_t(0),
                   size_t                      length              = size_t(-1),
                   bool                   replaceEmptyMatches = true);

    const RWRegexErr& getStatus() const;

    void swap(RWTRegex<char>& rhs);

private:
    RWTScopedPointer<RWRegexImp<RChar> > imp_;
};



template <class E> class RWRegexMatchIteratorImp;
template <>
class RW_TOOLS_GLOBAL RWTRegexMatchIterator<char>
{
private:
    typedef RWTRegex<char>::RChar   RChar;
    typedef RWTRegex<char>::RString RString;

public:
    RWTRegexMatchIterator();
    RWTRegexMatchIterator(const RWTRegexMatchIterator<char>& source);
    RWTRegexMatchIterator(RWTRegex<char>&    regex,
                          const RChar*       str,
                          size_t             start   = size_t(0),
                          size_t             length  = size_t(-1),
                          bool          overlap = false);
    RWTRegexMatchIterator(RWTRegex<char>&    regex,
                          const RWCString&   str,
                          size_t             start   = size_t(0),
                          size_t             length  = size_t(-1),
                          bool          overlap = false);
    RWTRegexMatchIterator(RWTRegex<char>&    regex,
                          const RString&     str,
                          size_t             start   = size_t(0),
                          size_t             length  = size_t(-1),
                          bool          overlap = false);

    RWTRegexMatchIterator& operator=(const RWTRegexMatchIterator& rhs);

#if !defined(RW_NO_RVALUE_REFERENCES)
    RWTRegexMatchIterator(RWTRegexMatchIterator && rhs);
    RWTRegexMatchIterator& operator= (RWTRegexMatchIterator && rhs);
#endif // !RW_NO_RVALUE_REFERENCES

    ~RWTRegexMatchIterator();

    RWTRegexMatchIterator&      operator++();
    const RWTRegexMatchIterator operator++(int);
    RWTRegexMatchIterator&      operator+=(size_t count);
    RWTRegexResult<char>        operator*() const;
    const RWTRegexResult<char>* operator->() const;
    bool                   operator==(const RWTRegexMatchIterator& rhs) const;
    bool                   operator!=(const RWTRegexMatchIterator& rhs) const;

    void swap(RWTRegexMatchIterator& rhs);

private:
    RWTScopedPointer<RWRegexMatchIteratorImp<char> > imp_;
};

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif
