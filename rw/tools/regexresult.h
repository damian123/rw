#ifndef RW_REGEXRESULT_H
#define RW_REGEXRESULT_H



/**********************************************************************
 *
 * results.h - contains the class definition for the regular expression
 *             match results class.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/regexresult.h#1 $
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


// Get defs.h
#include <rw/defs.h>
#include <rw/cstring.h>
#include <rw/wstring.h>
#include <rw/tools/pointer.h>

#include <rw/tools/regex/defs.h>

#include <string>

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

template <class T> class RWRegexImp;
template <class T> class RWTRegexResultImp;

/**
 * @ingroup string_processing_classes
 *
 * @brief Encapsulates the results from a search using
 * \link RWTRegex RWTRegex<T>\endlink
 *
 * Class \link RWTRegexResult RWTRegexResult<T>\endlink is a class template
 * that encapsulates the result of a RWTRegex<T>::matchAt() or
 * RWTRegex<T>::search() operation executed on an instance of
 * \link RWTRegex RWTRegex<T>\endlink.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tools/regex.h>
 * #include <rw/cstring.h>
 *
 * RWTRegex<char> pattern("a+");
 * RWTRegexResult<char> result = pattern.search("aabbccaadd");
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/tools/regex.h>
 * #include <rw/cstring.h>
 *
 * #include <iostream>
 *
 * int main()
 * {
 *   RWTRegex<char>        pattern("h(t+)p");
 *   RWCString             str("http");
 *   RWTRegexResult<char>  result = pattern.search(str);
 *
 *   std::cout << "Overall match at offset " << result.getStart()
 *             << ", with length " << result.getLength()
 *             << " is " << result.subString(str)
 *             << std::endl;
 *
 *   std::cout << "First sub-expression match at offset "
 *             << result.getStart(1)
 *             << ", with length " << result.getLength(1)
 *             << " is " << result.subString(str, 1)
 *             << std::endl;
 *
 *   return 0;
 * }
 * @endcode
 */
template <class T>
class RWTRegexResult
{
    // Friend declaration
    friend class RWRegexImp<T>;

public:
    // Public Typedefs
    typedef typename RWTRegexTraits<T>::Char  RChar;
    typedef std::basic_string<RChar>      RString;

public:

    /**
     * Default constructor
     */
    RWTRegexResult();

    /**
     * Copy constructor
     */
    RWTRegexResult(const RWTRegexResult<T>& source);

#if !defined(RW_NO_RVALUE_REFERENCES)

    /**
     * Move constructor. The constructed instance takes ownership of the
     * data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTRegexResult(RWTRegexResult<T> && rhs);

#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Destructor
     */
    ~RWTRegexResult();

    /**
     * Assignment operator
     */
    RWTRegexResult<T>& operator=(const RWTRegexResult<T>& rhs);

#if !defined(RW_NO_RVALUE_REFERENCES)

    /**
     * Move assignment. Self takes ownership of the data owned by \a rhs.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWTRegexResult<T>& operator= (RWTRegexResult<T> && rhs);

#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Match operator
     */
    operator bool() const;

    /**
     * Returns the number of match identifiers (\c matchID) available for
     * request, including the \c 0 matchID. Hence, the largest \c matchID
     * that can be requested is size() -1.
     */
    size_t size() const;

    /**
     * Returns the offset of the start of the requested match.
     */
    size_t getStart(size_t matchID = 0) const;

    /**
     * Returns the length of the requested match.
     */
    size_t getLength(size_t matchID = 0) const;

    // Return a substring containing the requested match

    /**
     * Returns a substring containing the requested match. Available
     * only in C++ Standard Library builds.
     */
    RString subString(const RString& str, size_t matchID = 0) const;

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWTRegexResult<T>& rhs);

private:
    RWTScopedPointer<RWTRegexResultImp<T> > imp_;
};

/**************************************************************************/
/*                                                                        */
/* Specializations                                                        */
/*                                                                        */
/**************************************************************************/


// char specialization
template <>
class RW_TOOLS_GLOBAL RWTRegexResult<char>
{
    // Friend declaration
    friend class RWRegexImp<char>;

public:
    // Public Typedefs
    typedef RWTRegexTraits<char>::Char             RChar;
    typedef std::basic_string<RChar>         RString;

public:
    // Default constructor
    RWTRegexResult();

    // Copy constructor
    RWTRegexResult(const RWTRegexResult<char>& source);

    // Assignment operator
    RWTRegexResult<char>& operator=(const RWTRegexResult<char>& rhs);

#if !defined(RW_NO_RVALUE_REFERENCES)

    // Move constructor
    RWTRegexResult<char> (RWTRegexResult<char> && rhs);

    // Move assignment
    RWTRegexResult<char>& operator= (RWTRegexResult<char> && rhs);

#endif // !RW_NO_RVALUE_REFERENCES

    // Destructor
    ~RWTRegexResult();

    // Was there a match?
    operator bool() const;

    // Return the number of match ID's available for request,
    // including the 0 match ID.  Hence, the largest matchID
    // that can be requested is size() - 1.
    size_t size() const;

    // Return the offset of the start of the requested match
    size_t getStart(size_t matchID = 0) const;

    // Return the length of the requested match
    size_t getLength(size_t matchID = 0) const;

    // Return a substring containing the requested match
    RString           subString(const RString& str, size_t matchID = 0) const;
    RWCConstSubString subString(const RWCString& str, size_t matchID = 0) const;

    // Swap with rhs
    void swap(RWTRegexResult<char>& rhs);

private:
    RWTScopedPointer<RWTRegexResultImp<char> > imp_;
};

// wchar_t specialization
template <>
class RW_TOOLS_GLOBAL RWTRegexResult<wchar_t>
{
    // Friend declaration
    friend class RWRegexImp<wchar_t>;

public:
    // Public Typedefs
    typedef RWTRegexTraits<wchar_t>::Char          RChar;
    typedef std::basic_string<RChar>         RString;

public:
    // Default constructor
    RWTRegexResult();

    // Copy constructor
    RWTRegexResult(const RWTRegexResult<wchar_t>& source);

    // Assignment operator
    RWTRegexResult<wchar_t>& operator=(const RWTRegexResult<wchar_t>& rhs);

#if !defined(RW_NO_RVALUE_REFERENCES)

    // Move constructor
    RWTRegexResult<wchar_t> (RWTRegexResult<wchar_t> && rhs);

    // Move assignment
    RWTRegexResult<wchar_t>& operator= (RWTRegexResult<wchar_t> && rhs);

#endif // !RW_NO_RVALUE_REFERENCES

    // Destructor
    ~RWTRegexResult();

    // Was there a match?
    operator bool() const;

    // Return the number of match ID's available for request,
    // including the 0 match ID.  Hence, the largest matchID
    // that can be requested is size() - 1.
    size_t size() const;

    // Return the offset of the start of the requested match
    size_t getStart(size_t matchID = 0) const;

    // Return the length of the requested match
    size_t getLength(size_t matchID = 0) const;

    // Return a substring containing the requested match
    RString           subString(const RString& str, size_t matchID = 0) const;
    RWWConstSubString subString(const RWWString& str, size_t matchID = 0) const;

    // Swap with rhs
    void swap(RWTRegexResult<wchar_t>& rhs);

private:
    RWTScopedPointer<RWTRegexResultImp<wchar_t> > imp_;
};

#if defined(_AIX) && !defined(RW_NO_IMPLICIT_INCLUSION)
#  pragma implementation ("regexresult.cc")
#endif

#if defined(RW_NO_IMPLICIT_INCLUSION)
#  include <rw/tools/regexresult.cc>
#endif

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif
