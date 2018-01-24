#ifndef RW_TOOLS_COWCSTRING_H
#define RW_TOOLS_COWCSTRING_H

/**********************************************************************
 *
 * Declarations for RWCopyOnWriteCString
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/cowcstring.h#1 $
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

#include <string.h>

#include <rw/defs.h>
#include <rw/ref.h>
#include <rw/rstream.h>
#include <rw/tools/cstrutil.h>
#include <rw/tools/hash.h>
#include <rw/tools/pointer.h>
#include <rw/tools/string.h>

#if defined(RW_COPY_ON_WRITE_STRING)
#  define RWCopyOnWriteCString RWCString
#  define RWCopyOnWriteCSubString RWCSubString
#  define RWCopyOnWriteCConstSubString RWCConstSubString
#endif

#if !defined(RW_DISABLE_DEPRECATED)
class RW_TOOLS_GLOBAL RWCRegexp;
#endif

template<class charT> class  RWTRegularExpression;
typedef RWTRegularExpression<char> RWCRExpr;

class RW_TOOLS_GLOBAL RWCopyOnWriteCString;
class RW_TOOLS_SYMBOLIC RWCopyOnWriteCSubString;
class RW_TOOLS_SYMBOLIC RWCopyOnWriteCConstSubString;

//////////////////////////////////////////////////////////////////////////
//
//                             RWCStringRef
//
//////////////////////////////////////////////////////////////////////////

/*
 * This is the dynamically allocated part of a RWCopyOnWriteCString.
 * It maintains a reference count.
 * There are no public member functions.
 */

class RW_TOOLS_GLOBAL RWCStringRef : public RWReference
{
    RWCStringRef(size_t capacity, size_t nchars);

    char& operator[](size_t i);

    char operator[](size_t i) const;

    size_t capacity() const;

    char* data() const;

    static RWCStringRef* getEmptyRep();

    static RWCStringRef* getRep(size_t capac, size_t nchar, void* where);

    static RWCStringRef* getRep(size_t capac, size_t nchar);

    size_t length() const;

    void link(); // connect to a stringref

    void unLink(); // disconnect from a stringref, maybe delete it

    // Max string length (excluding null)
    const size_t capacity_;

    // String length (excluding terminating null)
    size_t nchars_;

    friend class RW_TOOLS_GLOBAL RWCopyOnWriteCString;
    friend class RW_TOOLS_SYMBOLIC RWCopyOnWriteCSubString;
    friend class RW_TOOLS_SYMBOLIC RWCopyOnWriteCConstSubString;
};


/**
 * @ingroup string_processing_classes
 * @brief Alternate implementation of RWCSubString when
 * \c RW_COPY_ON_WRITE_STRING is defined.
 *
 * RWCopyOnWriteCSubString should be directly used only with instances
 * of class RWCopyOnWriteCString. RWCSubString should be used with instances
 * of class RWCString.
 *
 * @see RWCSubString for additional documentation.
 */
class RW_TOOLS_SYMBOLIC RWCopyOnWriteCSubString
{

public:

    RWCopyOnWriteCSubString(const RWCopyOnWriteCSubString& sp);

    RWCopyOnWriteCSubString& operator=(const RWCopyOnWriteCSubString& str);

    RWCopyOnWriteCSubString& operator=(const RWCopyOnWriteCConstSubString&);

    RWCopyOnWriteCSubString& operator=(const RWCopyOnWriteCString& str);

    RWCopyOnWriteCSubString& operator=(const std::string&);

    RWCopyOnWriteCSubString& operator=(const char* str);

    char& operator()(size_t i);

    char operator()(size_t i) const;

    char& operator[](char i);

    char operator[](char i) const;

    char& operator[](signed char i);

    char operator[](signed char i) const;

    char& operator[](short i);

    char operator[](short i) const;

    char& operator[](int i);

    char operator[](int i) const;

    char& operator[](long i);

    char operator[](long i) const;

#if !defined(RW_NO_LONG_LONG)
    char& operator[](long long i);

    char operator[](long long i) const;
#endif

    char& operator[](unsigned char i);

    char operator[](unsigned char i) const;

    char& operator[](unsigned short i);

    char operator[](unsigned short i) const;

    char& operator[](unsigned int i);

    char operator[](unsigned int i) const;

    char& operator[](unsigned long i);

    char operator[](unsigned long i) const;

#if !defined(RW_NO_LONG_LONG)
    char& operator[](unsigned long long i);

    char operator[](unsigned long long i) const;
#endif

    int operator!() const;

#if !defined(RW_DISABLE_DEPRECATED)
    RW_DEPRECATE_FUNC("Use RWCopyOnWriteCSubString::startData() instead")
    const char* data() const;
#endif

    bool isNull() const;

    size_t length() const;

    size_t start() const;

    const char* startData() const;

    void toLower();

    void toUpper();

protected:

    void assertElement(size_t i) const; // Verifies i is valid index
    void subStringError(size_t, size_t, size_t) const;

private:

    RWCopyOnWriteCSubString(const RWCopyOnWriteCString&, size_t, size_t);

    void throwBoundsErr(size_t i, size_t len) const;

    RWCopyOnWriteCString* str_;

    size_t begin_;
    size_t extent_;

    friend class RW_TOOLS_GLOBAL RWCopyOnWriteCString;
    friend class RWCopyOnWriteCConstSubString;
};

/**
 * @ingroup string_processing_classes
 * @brief Alternate implementation of RWCConstSubString when
 * \c RW_COPY_ON_WRITE_STRING is defined.
 *
 * RWCopyOnWriteCConstSubString should be directly used only with instances
 * of class RWCopyOnWriteCString. RWCConstSubString should be used with instances
 * of class RWCString.
 *
 * @see RWCConstSubString for additional documentation.
 */
class RW_TOOLS_SYMBOLIC RWCopyOnWriteCConstSubString
{
public:

    RWCopyOnWriteCConstSubString(const RWCopyOnWriteCConstSubString& sp);

    RWCopyOnWriteCConstSubString(const RWCopyOnWriteCSubString& sp);

    char operator()(size_t i) const;

    char operator[](char i) const;

    char operator[](signed char i) const;

    char operator[](short i) const;

    char operator[](int i) const;

    char operator[](long i) const;

#if !defined(RW_NO_LONG_LONG)
    char operator[](long long i) const;
#endif

    char operator[](unsigned char i) const;

    char operator[](unsigned short i) const;

    char operator[](unsigned int i) const;

    char operator[](unsigned long i) const;

#if !defined(RW_NO_LONG_LONG)
    char operator[](unsigned long long i) const;
#endif

    int operator!() const;

#if !defined(RW_DISABLE_DEPRECATED)
    RW_DEPRECATE_FUNC("Use RWCopyOnWriteCConstSubString::startData() instead")
    const char* data() const;
#endif

    bool isNull() const;

    size_t length() const;

    size_t start() const;

    const char* startData() const; // Replaces data()

protected:

    void assertElement(size_t i) const;
    void subStringError(size_t, size_t, size_t) const;

private:

    RWCopyOnWriteCConstSubString(const RWCopyOnWriteCString& s, size_t start, size_t len);

    void throwBoundsErr(size_t i, size_t len) const;

    const RWCopyOnWriteCString* str_; // Referenced string
    size_t begin_;         // Index of starting character
    size_t extent_;        // Length of RWCopyOnWriteCConstSubString

    friend class RWCopyOnWriteCString;
    friend class RWCopyOnWriteCSubString;
};


/**
 * @ingroup string_processing_classes
 * @brief Alternate implementation of RWCString when
 * \c RW_COPY_ON_WRITE_STRING is defined.
 *
 * Unless the specific performance characteristics of RWCopyOnWriteCString
 * are required, applications should use RWCString instead.  See the
 * <em>Essential Tools Module User's Guide</em> for additional information.
 *
 * @see RWCString for additional documentation.
 */
class RW_TOOLS_GLOBAL RWCopyOnWriteCString
{
public:
    typedef RWCopyOnWriteCSubString SubString;

    typedef RWCopyOnWriteCConstSubString ConstSubString;

    enum caseCompare {
        exact,
        ignoreCase,
        ignoreCaseStrict
    };

#if !defined(RW_DISABLE_DEPRECATED)

    enum RW_DEPRECATE_TYPE("") scopeType {
        one,
        all
    };

#endif // RW_DISABLE_DEPRECATED

    enum stripType {
        leading = 0x1,
        trailing = 0x2,
        both = 0x3
    };

    RWCopyOnWriteCString();

    RWCopyOnWriteCString(const RWCopyOnWriteCString& str);

#if !defined(RW_NO_RVALUE_REFERENCES)
    RWCopyOnWriteCString(RWCopyOnWriteCString && str);
#endif // !RW_NO_RVALUE_REFERENCES

    RWCopyOnWriteCString(const SubString& ss);

    RWCopyOnWriteCString(const ConstSubString& ss);

#ifdef RW_DISABLE_IMPLICIT_CONVERSION
    explicit
#endif
    RWCopyOnWriteCString(const std::string& s);

#ifdef RW_DISABLE_IMPLICIT_CONVERSION
    explicit
#endif
    RWCopyOnWriteCString(const char* a);

    RWCopyOnWriteCString(const char* a, size_t N);

#ifdef RW_DISABLE_IMPLICIT_CONVERSION
    explicit
#endif
    RWCopyOnWriteCString(char c);

#ifdef RW_DISABLE_IMPLICIT_CONVERSION
    explicit
#endif
    RWCopyOnWriteCString(unsigned char c);

#ifdef RW_DISABLE_IMPLICIT_CONVERSION
    explicit
#endif
    RWCopyOnWriteCString(signed char c);

    RWCopyOnWriteCString(char c, size_t N);

    RWCopyOnWriteCString(RWSize_T ic);

    ~RWCopyOnWriteCString();

#ifndef RW_DISABLE_IMPLICIT_CONVERSION
    operator const char* () const;
#endif

    RWCopyOnWriteCString& operator=(const RWCopyOnWriteCString& str);

#if !defined(RW_NO_RVALUE_REFERENCES)
    RWCopyOnWriteCString& operator=(RWCopyOnWriteCString && str);
#endif // !RW_NO_RVALUE_REFERENCES

    RWCopyOnWriteCString& operator=(const SubString& str);

    RWCopyOnWriteCString& operator=(const ConstSubString& str);

    RWCopyOnWriteCString& operator=(const std::string& data);

    RWCopyOnWriteCString& operator=(const char* cs);

    RWCopyOnWriteCString& operator=(char c);

    RWCopyOnWriteCString& operator+=(const RWCopyOnWriteCString& str);

    RWCopyOnWriteCString& operator+=(const SubString& str);

    RWCopyOnWriteCString& operator+=(const ConstSubString& str);

    RWCopyOnWriteCString& operator+=(const std::string& str);

    RWCopyOnWriteCString& operator+=(const char* cs);

    RWCopyOnWriteCString& operator+=(char c);

    char& operator()(size_t i);

    char operator()(size_t i) const;

    char& operator[](char i);

    char operator[](char i) const;

    char& operator[](signed char i);

    char operator[](signed char i) const;

    char& operator[](short i);

    char operator[](short i) const;

    char& operator[](int i);

    char operator[](int i) const;

    char& operator[](long i);

    char operator[](long i) const;

#if !defined(RW_NO_LONG_LONG)
    char& operator[](long long i);

    char operator[](long long i) const;
#endif

    char& operator[](unsigned char i);

    char operator[](unsigned char i) const;

    char& operator[](unsigned short i);

    char operator[](unsigned short i) const;

    char& operator[](unsigned int i);

    char operator[](unsigned int i) const;

    char& operator[](unsigned long i);

    char operator[](unsigned long i) const;

#if !defined(RW_NO_LONG_LONG)
    char& operator[](unsigned long long i);

    char operator[](unsigned long long i) const;
#endif

    SubString operator()(size_t start, size_t len);

    ConstSubString operator()(size_t start, size_t len) const;

#if !defined(RW_DISABLE_DEPRECATED)
    RW_SUPPRESS_DEPRECATED_WARNINGS

    SubString operator()(const RWCRegexp& re);

    ConstSubString operator()(const RWCRegexp& re) const;

    SubString operator()(const RWCRegexp& re, size_t start);

    ConstSubString operator()(const RWCRegexp& re, size_t start) const;

    RW_RESTORE_DEPRECATED_WARNINGS
#endif

    RWCopyOnWriteCString& append(const RWCopyOnWriteCString& cs);

    RWCopyOnWriteCString& append(const SubString& str);

    RWCopyOnWriteCString& append(const ConstSubString& str);

    RWCopyOnWriteCString& append(const RWCopyOnWriteCString& cs, size_t N);

    RWCopyOnWriteCString& append(const std::string& str);

    RWCopyOnWriteCString& append(const std::string& str, size_t N);

    RWCopyOnWriteCString& append(const char* cs);

    RWCopyOnWriteCString& append(const char* cs, size_t N);

    RWCopyOnWriteCString& append(char c, size_t rep = 1);

    RWCopyOnWriteCString& assign(const RWCopyOnWriteCString& str);

    RWCopyOnWriteCString& assign(const std::string& str);

    RWCopyOnWriteCString& assign(const SubString& str);

    RWCopyOnWriteCString& assign(const ConstSubString& str);

    RWCopyOnWriteCString& assign(const char* str);

    RWCopyOnWriteCString& assign(const char* str, size_t len);

    RWCopyOnWriteCString& assign(char c, size_t count = 1);

    RWspace binaryStoreSize() const;

    static size_t byteCount(const char* cstr, size_t chars = RW_NPOS);

    size_t capacity() const;

    size_t capacity(size_t N);

#ifndef RW_NO_LOCALE
    int collate(const RWCopyOnWriteCString& str) const;

    int collate(const std::string& str) const;

    int collate(const char* str) const;
#endif

    int compareTo(const RWCopyOnWriteCString& str, caseCompare cmp = exact) const;

    int compareTo(const SubString& str, caseCompare cmp = exact) const;

    int compareTo(const ConstSubString& str, caseCompare cmp = exact) const;

    int compareTo(const RWCopyOnWriteCString* st, caseCompare cmp = exact) const;

    int compareTo(const std::string& str, caseCompare cmp = exact) const;

    int compareTo(const std::string* str, caseCompare cmp = exact) const;

    int compareTo(const char* str, caseCompare cmp = exact) const;

    int compareTo(const char* str, size_t len, caseCompare cmp = exact) const;

    bool contains(const RWCopyOnWriteCString& str, caseCompare cmp = exact) const;

    bool contains(const SubString& str, caseCompare cmp = exact) const;

    bool contains(const ConstSubString& str, caseCompare cmp = exact) const;

    bool contains(const std::string& str, caseCompare cmp = exact) const;

    bool contains(const char* str, caseCompare cmp = exact) const;

    bool contains(const char* str, size_t len, caseCompare cmp = exact) const;

#if !defined(RW_DISABLE_DEPRECATED)

    RW_DEPRECATE_FUNC("Use RWCopyOnWriteCString::operator=(...) instead")
    RWCopyOnWriteCString copy() const;

#endif

    const char* data() const;

#if !defined(RW_DISABLE_DEPRECATED)

    RW_DEPRECATE_FUNC("Use RWCopyOnWriteCString::firstOf(...) instead")
    size_t first(char c) const;

    RW_DEPRECATE_FUNC("Use RWCopyOnWriteCString::firstOf(...) instead")
    size_t first(char c, size_t i) const;

    RW_DEPRECATE_FUNC("Use RWCopyOnWriteCString::firstOf(...) instead")
    size_t first(const char* str) const;

    RW_DEPRECATE_FUNC("Use RWCopyOnWriteCString::firstOf(...) instead")
    size_t first(const char* str, size_t N) const;

#endif

    size_t firstNotOf(char c, size_t pos = 0) const;

    size_t firstNotOf(const char* str, size_t pos = 0) const;

    size_t firstNotOf(const char* str, size_t pos, size_t len) const;

    size_t firstNotOf(const RWCopyOnWriteCString& str, size_t pos = 0) const;

    size_t firstNotOf(const SubString& str, size_t pos = 0) const;

    size_t firstNotOf(const ConstSubString& str, size_t pos = 0) const;

    size_t firstNotOf(const std::string& str, size_t pos = 0) const;

    size_t firstOf(char c, size_t pos = 0) const;

    size_t firstOf(const char* str, size_t pos = 0) const;

    size_t firstOf(const char* str, size_t pos, size_t len) const;

    size_t firstOf(const RWCopyOnWriteCString& str, size_t pos = 0) const;

    size_t firstOf(const SubString& str, size_t pos = 0) const;

    size_t firstOf(const ConstSubString& str, size_t pos = 0) const;

    size_t firstOf(const std::string& str, size_t pos = 0) const;

    static size_t getMaxWaste();
    static size_t getResizeIncrement();

    static unsigned hash(const RWCopyOnWriteCString& str);

    unsigned hash(caseCompare cmp = exact) const;

    size_t
    index(const RWCopyOnWriteCString& pat, size_t i = 0, caseCompare cmp = exact) const;

    size_t
    index(const SubString& pat, size_t start = 0, caseCompare cmp = exact) const;

    size_t
    index(const ConstSubString& pat, size_t start = 0, caseCompare cmp = exact) const;

    size_t
    index(const RWCopyOnWriteCString& pat, size_t patlen, size_t i, caseCompare cmp) const;

    size_t index(const std::string& pat, size_t i = 0, caseCompare cmp = exact) const;

    size_t index(const std::string& pat, size_t patlen, size_t i, caseCompare cmp) const;

    size_t
    index(const char* pat, size_t i = 0, caseCompare cmp = exact) const;

    size_t
    index(const char* pat, size_t patlen, size_t i, caseCompare cmp) const;

    size_t
    index(const char pat, size_t i = 0, caseCompare cmp = exact) const;

#if !defined(RW_DISABLE_DEPRECATED)
    RW_SUPPRESS_DEPRECATED_WARNINGS

    RW_DEPRECATE_FUNC("Use RWTRegex::index instead")
    size_t
    index(const RWCRegexp& re, size_t i = 0) const;

    RW_DEPRECATE_FUNC("Use RWTRegex::index instead")
    size_t
    index(const RWCRegexp& re, size_t* ext, size_t i = 0) const;

    RW_DEPRECATE_FUNC("Use RWTRegex::index instead")
    size_t
    index(const RWCRExpr& re, size_t i = 0) const;

    RW_DEPRECATE_FUNC("Use RWTRegex::index instead")
    size_t
    index(const RWCRExpr& re, size_t* ext, size_t i = 0) const;

    RW_DEPRECATE_FUNC("Use RWTRegex::index instead")
    size_t
    index(const char* pat, size_t* ext, size_t i = 0) const;

    RW_RESTORE_DEPRECATED_WARNINGS
#endif // RW_DISABLE_DEPRECATED

    RWCopyOnWriteCString& insert(size_t pos, const RWCopyOnWriteCString& str);

    RWCopyOnWriteCString& insert(size_t pos, const SubString& str);

    RWCopyOnWriteCString& insert(size_t pos, const ConstSubString& str);

    RWCopyOnWriteCString& insert(size_t pos, const RWCopyOnWriteCString& str, size_t N);

    RWCopyOnWriteCString& insert(size_t pos, const std::string& str);

    RWCopyOnWriteCString& insert(size_t pos, const std::string& str, size_t N);

    RWCopyOnWriteCString& insert(size_t pos, const char* cs);

    RWCopyOnWriteCString& insert(size_t pos, const char* cs, size_t N);

    bool isAscii() const;

    bool isNull() const;

#if !defined(RW_DISABLE_DEPRECATED)

    RW_DEPRECATE_FUNC("Use RWCopyOnWriteCString::lastOf(...) instead")
    size_t last(char c) const;

    RW_DEPRECATE_FUNC("Use RWCopyOnWriteCString::lastOf(...) instead")
    size_t last(char c, size_t N) const;

    RW_DEPRECATE_FUNC("Use RWCopyOnWriteCString::lastOf(...) instead")
    size_t last(const char* s) const;

    RW_DEPRECATE_FUNC("Use RWCopyOnWriteCString::lastOf(...) instead")
    size_t last(const char* s, size_t N) const;

#endif

    size_t lastNotOf(char c, size_t pos = RW_NPOS) const;

    size_t lastNotOf(const char* str, size_t pos = RW_NPOS) const;

    size_t lastNotOf(const char* str, size_t pos, size_t len) const;

    size_t lastNotOf(const RWCopyOnWriteCString& str, size_t pos = RW_NPOS) const;

    size_t lastNotOf(const SubString& str, size_t pos = RW_NPOS) const;

    size_t lastNotOf(const ConstSubString& str, size_t pos = RW_NPOS) const;

    size_t lastNotOf(const std::string& str, size_t pos = RW_NPOS) const;

    size_t lastOf(char c, size_t pos = RW_NPOS) const;

    size_t lastOf(const char* str, size_t pos = RW_NPOS) const;

    size_t lastOf(const char* str, size_t pos, size_t len) const;

    size_t lastOf(const RWCopyOnWriteCString& str, size_t pos = RW_NPOS) const;

    size_t lastOf(const SubString& str, size_t pos = RW_NPOS) const;

    size_t lastOf(const ConstSubString& str, size_t pos = RW_NPOS) const;

    size_t lastOf(const std::string& str, size_t pos = RW_NPOS) const;

    size_t length() const;

#if !defined(RW_DISABLE_DEPRECATED)
    RW_SUPPRESS_DEPRECATED_WARNINGS

    RW_DEPRECATE_FUNC("Use RWTRegex::search instead")
    SubString match(const RWCRExpr& re);

    RW_DEPRECATE_FUNC("Use RWTRegex::search instead")
    ConstSubString match(const RWCRExpr& pat) const;

    RW_DEPRECATE_FUNC("Use RWTRegex::search instead")
    SubString match(const RWCRExpr& re, size_t start);

    RW_DEPRECATE_FUNC("Use RWTRegex::search instead")
    ConstSubString match(const RWCRExpr& re, size_t start) const;

    RW_RESTORE_DEPRECATED_WARNINGS
#endif // RW_DISABLE_DEPRECATED

    size_t mbLength() const;

    static size_t mbLength(const char* cstr, size_t bytes = RW_NPOS);

    RWCopyOnWriteCString& prepend(const RWCopyOnWriteCString& str);

    RWCopyOnWriteCString& prepend(const SubString& str);

    RWCopyOnWriteCString& prepend(const ConstSubString& str);

    RWCopyOnWriteCString& prepend(const RWCopyOnWriteCString& cstr, size_t N);

    RWCopyOnWriteCString& prepend(const std::string& str);

    RWCopyOnWriteCString& prepend(const std::string& cstr, size_t N);

    RWCopyOnWriteCString& prepend(const char* cs);

    RWCopyOnWriteCString& prepend(const char* cs, size_t N);

    RWCopyOnWriteCString& prepend(char c, size_t rep = 1);

    std::istream&
    readFile(std::istream& s);

    std::istream&
    readLine(std::istream& s, bool skipWhite = true);

    std::istream&
    readString(std::istream& s);

    std::istream&
    readToDelim(std::istream& s, char delim = '\n');

    std::istream&
    readToken(std::istream& s);

    RWCopyOnWriteCString& remove(size_t pos);

    RWCopyOnWriteCString& remove(size_t pos, size_t N);

    RWCopyOnWriteCString&
    replace(size_t pos, size_t N, const RWCopyOnWriteCString& str);

    RWCopyOnWriteCString& replace(size_t pos, size_t len, const SubString& str);

    RWCopyOnWriteCString& replace(size_t pos, size_t len, const ConstSubString& str);

    RWCopyOnWriteCString&
    replace(size_t pos, size_t N1, const RWCopyOnWriteCString& str, size_t N2);

    RWCopyOnWriteCString& replace(size_t pos, size_t N, const std::string& str);

    RWCopyOnWriteCString& replace(size_t pos, size_t N1, const std::string& str, size_t N2);

    RWCopyOnWriteCString&
    replace(size_t pos, size_t N, const char* cs);

    RWCopyOnWriteCString&
    replace(size_t pos, size_t N1, const char* cs, size_t N2);

#if !defined(RW_DISABLE_DEPRECATED)
    RW_SUPPRESS_DEPRECATED_WARNINGS

    RW_DEPRECATE_FUNC("Use RWTRegex::replace instead")
    RWCopyOnWriteCString&
    replace(const RWCRExpr& pattern, const RWCopyOnWriteCString& replacement, scopeType scope = one);

    RW_DEPRECATE_FUNC("Use RWTRegex::replace instead")
    RWCopyOnWriteCString&
    replace(const RWCRExpr& pattern, const char* replacement, scopeType scope = one);

    RW_RESTORE_DEPRECATED_WARNINGS
#endif // RW_DISABLE_DEPRECATED

    void resize(size_t N);

    void restoreFrom(RWFile&);
    void restoreFrom(RWvistream&);

    size_t rindex(const RWCopyOnWriteCString& pat,
                  size_t i = RW_NPOS, caseCompare cmp = exact) const;

    size_t rindex(const SubString& pat,
                  size_t end = RW_NPOS, caseCompare cmp = exact) const;

    size_t rindex(const ConstSubString& pat,
                  size_t end = RW_NPOS, caseCompare cmp = exact) const;

    size_t rindex(const RWCopyOnWriteCString& pat, size_t patlen, size_t i, caseCompare cmp) const;

    size_t rindex(const std::string& pat,
                  size_t i = RW_NPOS, caseCompare cmp = exact) const;

    size_t rindex(const std::string& pat, size_t patlen, size_t i, caseCompare cmp) const;

    size_t rindex(const char* pat, caseCompare cmp) const;

    size_t rindex(const char* pat, size_t i = RW_NPOS, caseCompare cmp = exact) const;

    size_t rindex(const char* pat, size_t patlen, size_t i, caseCompare cmp) const;

    size_t rindex(const char pat, size_t i = RW_NPOS, caseCompare cmp = exact) const;

    void saveOn(RWFile& f) const;
    void saveOn(RWvostream& s) const;

    void shrink();

    SubString
    subString(const char* cs, size_t start = 0, caseCompare cmp = exact);

    ConstSubString
    subString(const char* cs, size_t start = 0, caseCompare cmp = exact) const;

    SubString strip(stripType s = trailing, char c = ' ');

    ConstSubString strip(stripType s = trailing, char c = ' ') const;

    void swap(RWCopyOnWriteCString& str);

    void toLower();

    void toUpper();

protected:

    RWCopyOnWriteCString(const char* a1, size_t N1, const char* a2, size_t N2);

    void assertElement(size_t) const;

    void clobber(size_t nc);

    int compareToSpecial(const char*, size_t, caseCompare) const;

    void cow();
    void cow(size_t nc);

    size_t indexSpecial(const char*, size_t, size_t, caseCompare) const;

    void initChar(char);

    void throwBoundsErr(size_t i, size_t len) const;

private:

    void clone();
    void clone(size_t nc);

    RWCStringRef* pref() const;

    char* data_;

    friend RWCopyOnWriteCString
    operator+(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCString& rhs);

    friend RWCopyOnWriteCString
    operator+(const RWCopyOnWriteCString& lhs,  const RWCopyOnWriteCSubString& rhs);

    friend RWCopyOnWriteCString
    operator+(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCString& rhs);

    friend RWCopyOnWriteCString
    operator+(const RWCopyOnWriteCString& lhs,  const RWCopyOnWriteCConstSubString& rhs);

    friend RWCopyOnWriteCString
    operator+(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCString& rhs);

    friend RWCopyOnWriteCString
    operator+(const RWCopyOnWriteCString& lhs,  const std::string& rhs);

    friend RWCopyOnWriteCString
    operator+(const std::string& lhs, const RWCopyOnWriteCString& rhs);

    friend RWCopyOnWriteCString
    operator+(const RWCopyOnWriteCString& lhs,  const char* rhs);

    friend RWCopyOnWriteCString
    operator+(const char* lhs, const RWCopyOnWriteCString& rhs);

    friend RWCopyOnWriteCString
    operator+(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCSubString& rhs);

    friend RWCopyOnWriteCString
    operator+(const RWCopyOnWriteCSubString& lhs,  const RWCopyOnWriteCConstSubString& rhs);

    friend RWCopyOnWriteCString
    operator+(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCSubString& rhs);

    friend RWCopyOnWriteCString
    operator+(const RWCopyOnWriteCSubString& lhs,  const std::string& rhs);

    friend RWCopyOnWriteCString
    operator+(const std::string& lhs, const RWCopyOnWriteCSubString& rhs);

    friend RWCopyOnWriteCString
    operator+(const RWCopyOnWriteCSubString& lhs,  const char* rhs);

    friend RWCopyOnWriteCString
    operator+(const char* lhs, const RWCopyOnWriteCSubString& rhs);

    friend RWCopyOnWriteCString
    operator+(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCConstSubString& rhs);

    friend RWCopyOnWriteCString
    operator+(const RWCopyOnWriteCConstSubString& lhs,  const std::string& rhs);

    friend RWCopyOnWriteCString
    operator+(const std::string& lhs, const RWCopyOnWriteCConstSubString& rhs);

    friend RWCopyOnWriteCString
    operator+(const RWCopyOnWriteCConstSubString& lhs,  const char* rhs);

    friend RWCopyOnWriteCString
    operator+(const char* lhs, const RWCopyOnWriteCConstSubString& rhs);

    friend class RW_TOOLS_SYMBOLIC RWCopyOnWriteCSubString;
    friend class RW_TOOLS_SYMBOLIC RWCopyOnWriteCConstSubString;
    friend class RW_TOOLS_GLOBAL RWCStringRef;
};

RW_TOOLS_SYMBOLIC RWCopyOnWriteCString
strXForm(const RWCopyOnWriteCString&);

RWCopyOnWriteCString
toLower(const RWCopyOnWriteCString& str);

RWCopyOnWriteCString
toUpper(const RWCopyOnWriteCString& str);

RWCopyOnWriteCString
operator+(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCString& rhs);

RWCopyOnWriteCString
operator+(const RWCopyOnWriteCString& lhs,  const RWCopyOnWriteCSubString& rhs);

RWCopyOnWriteCString
operator+(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCString& rhs);

RWCopyOnWriteCString
operator+(const RWCopyOnWriteCString& lhs,  const RWCopyOnWriteCConstSubString& rhs);

RWCopyOnWriteCString
operator+(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCString& rhs);

RWCopyOnWriteCString
operator+(const RWCopyOnWriteCString& lhs,  const std::string& rhs);

RWCopyOnWriteCString
operator+(const std::string& lhs, const RWCopyOnWriteCString& rhs);

RWCopyOnWriteCString
operator+(const RWCopyOnWriteCString& lhs,  const char* rhs);

RWCopyOnWriteCString
operator+(const char* lhs, const RWCopyOnWriteCString& rhs);

RWCopyOnWriteCString
operator+(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCSubString& rhs);

RWCopyOnWriteCString
operator+(const RWCopyOnWriteCSubString& lhs,  const RWCopyOnWriteCConstSubString& rhs);

RWCopyOnWriteCString
operator+(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCSubString& rhs);

RWCopyOnWriteCString
operator+(const RWCopyOnWriteCSubString& lhs,  const std::string& rhs);

RWCopyOnWriteCString
operator+(const std::string& lhs, const RWCopyOnWriteCSubString& rhs);

RWCopyOnWriteCString
operator+(const RWCopyOnWriteCSubString& lhs,  const char* rhs);

RWCopyOnWriteCString
operator+(const char* lhs, const RWCopyOnWriteCSubString& rhs);

RWCopyOnWriteCString
operator+(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCConstSubString& rhs);

RWCopyOnWriteCString
operator+(const RWCopyOnWriteCConstSubString& lhs,  const std::string& rhs);

RWCopyOnWriteCString
operator+(const std::string& lhs, const RWCopyOnWriteCConstSubString& rhs);

RWCopyOnWriteCString
operator+(const RWCopyOnWriteCConstSubString& lhs,  const char* rhs);

RWCopyOnWriteCString
operator+(const char* lhs, const RWCopyOnWriteCConstSubString& rhs);

RWFile& operator>>(RWFile& file, RWCopyOnWriteCString& cstr);

RW_TOOLS_SYMBOLIC std::istream&
operator>>(std::istream& s, RWCopyOnWriteCString& cstr);

RWFile& operator<<(RWFile& file, const RWCopyOnWriteCString& cstr);

RW_TOOLS_SYMBOLIC std::ostream&
operator<<(std::ostream& s, const RWCopyOnWriteCString& cstr);

RW_TOOLS_SYMBOLIC std::ostream&
operator<<(std::ostream& s, const RWCopyOnWriteCSubString& cstr);

RW_TOOLS_SYMBOLIC std::ostream&
operator<<(std::ostream& s, const RWCopyOnWriteCConstSubString& cstr);

bool
operator==(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCString& rhs);

bool
operator==(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator==(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCString& rhs);

bool
operator==(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator==(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCString& rhs);

bool
operator==(const RWCopyOnWriteCString& lhs, const std::string& rhs);

bool
operator==(const std::string& lhs, const RWCopyOnWriteCString& rhs);

bool
operator==(const RWCopyOnWriteCString& lhs, const char* rhs);

bool
operator==(const char* lhs, const RWCopyOnWriteCString& rhs);

bool
operator==(const RWCopyOnWriteCString& lhs, char rhs);

bool
operator==(char lhs, const RWCopyOnWriteCString& rhs);

bool
operator==(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator==(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator==(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator==(const RWCopyOnWriteCSubString& lhs, const std::string& rhs);

bool
operator==(const std::string& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator==(const RWCopyOnWriteCSubString& lhs, const char* rhs);

bool
operator==(const char* lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator==(const RWCopyOnWriteCSubString& lhs, char rhs);

bool
operator==(char lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator==(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator==(const RWCopyOnWriteCConstSubString& lhs, const std::string& rhs);

bool
operator==(const std::string& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator==(const RWCopyOnWriteCConstSubString& lhs, const char* rhs);

bool
operator==(const char* lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator==(const RWCopyOnWriteCConstSubString& lhs, char rhs);

bool
operator==(char lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator!=(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCString& rhs);

bool
operator!=(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator!=(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCString& rhs);

bool
operator!=(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator!=(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCString& rhs);

bool
operator!=(const RWCopyOnWriteCString& lhs, const std::string& rhs);

bool
operator!=(const std::string& lhs, const RWCopyOnWriteCString& rhs);

bool
operator!=(const RWCopyOnWriteCString& lhs, const char* rhs);

bool
operator!=(const char* lhs, const RWCopyOnWriteCString& rhs);

bool
operator!=(const RWCopyOnWriteCString& lhs, char rhs);

bool
operator!=(char lhs, const RWCopyOnWriteCString& rhs);

bool
operator!=(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator!=(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator!=(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator!=(const RWCopyOnWriteCSubString& lhs, const std::string& rhs);

bool
operator!=(const std::string& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator!=(const RWCopyOnWriteCSubString& lhs, const char* rhs);

bool
operator!=(const char* lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator!=(const RWCopyOnWriteCSubString& lhs, char rhs);

bool
operator!=(char lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator!=(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator!=(const RWCopyOnWriteCConstSubString& lhs, const std::string& rhs);

bool
operator!=(const std::string& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator!=(const RWCopyOnWriteCConstSubString& lhs, const char* rhs);

bool
operator!=(const char* lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator!=(const RWCopyOnWriteCConstSubString& lhs, char rhs);

bool
operator!=(char lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator< (const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCString& rhs);

bool
operator< (const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator< (const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCString& rhs);

bool
operator< (const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator< (const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCString& rhs);

bool
operator< (const RWCopyOnWriteCString& lhs, const std::string& rhs);

bool
operator< (const std::string& lhs, const RWCopyOnWriteCString& rhs);

bool
operator< (const RWCopyOnWriteCString& lhs, const char* rhs);

bool
operator< (const char* lhs, const RWCopyOnWriteCString& rhs);

bool
operator< (const RWCopyOnWriteCString& lhs, char rhs);

bool
operator< (char lhs, const RWCopyOnWriteCString& rhs);

bool
operator< (const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator< (const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator< (const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator< (const RWCopyOnWriteCSubString& lhs, const std::string& rhs);

bool
operator< (const std::string& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator< (const RWCopyOnWriteCSubString& lhs, const char* rhs);

bool
operator< (const char* lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator< (const RWCopyOnWriteCSubString& lhs, char rhs);

bool
operator< (char lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator< (const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator< (const RWCopyOnWriteCConstSubString& lhs, const std::string& rhs);

bool
operator< (const std::string& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator< (const RWCopyOnWriteCConstSubString& lhs, const char* rhs);

bool
operator< (const char* lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator< (const RWCopyOnWriteCConstSubString& lhs, char rhs);

bool
operator< (char lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator<=(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCString& rhs);

bool
operator<=(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator<=(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCString& rhs);

bool
operator<=(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator<=(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCString& rhs);

bool
operator<=(const RWCopyOnWriteCString& lhs, const std::string& rhs);

bool
operator<=(const std::string& lhs, const RWCopyOnWriteCString& rhs);

bool
operator<=(const RWCopyOnWriteCString& lhs, const char* rhs);

bool
operator<=(const char* lhs, const RWCopyOnWriteCString& rhs);

bool
operator<=(const RWCopyOnWriteCString& lhs, char rhs);

bool
operator<=(char lhs, const RWCopyOnWriteCString& rhs);

bool
operator<=(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator<=(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator<=(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator<=(const RWCopyOnWriteCSubString& lhs, const std::string& rhs);

bool
operator<=(const std::string& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator<=(const RWCopyOnWriteCSubString& lhs, const char* rhs);

bool
operator<=(const char* lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator<=(const RWCopyOnWriteCSubString& lhs, char rhs);

bool
operator<=(char lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator<=(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator<=(const RWCopyOnWriteCConstSubString& lhs, const std::string& rhs);

bool
operator<=(const std::string& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator<=(const RWCopyOnWriteCConstSubString& lhs, const char* rhs);

bool
operator<=(const char* lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator<=(const RWCopyOnWriteCConstSubString& lhs, char rhs);

bool
operator<=(char lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator>=(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCString& rhs);

bool
operator>=(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator>=(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCString& rhs);

bool
operator>=(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator>=(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCString& rhs);

bool
operator>=(const RWCopyOnWriteCString& lhs, const std::string& rhs);

bool
operator>=(const std::string& lhs, const RWCopyOnWriteCString& rhs);

bool
operator>=(const RWCopyOnWriteCString& lhs, const char* rhs);

bool
operator>=(const char* lhs, const RWCopyOnWriteCString& rhs);

bool
operator>=(const RWCopyOnWriteCString& lhs, char rhs);

bool
operator>=(char lhs, const RWCopyOnWriteCString& rhs);

bool
operator>=(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator>=(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator>=(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator>=(const RWCopyOnWriteCSubString& lhs, const std::string& rhs);

bool
operator>=(const std::string& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator>=(const RWCopyOnWriteCSubString& lhs, const char* rhs);

bool
operator>=(const char* lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator>=(const RWCopyOnWriteCSubString& lhs, char rhs);

bool
operator>=(char lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator>=(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator>=(const RWCopyOnWriteCConstSubString& lhs, const std::string& rhs);

bool
operator>=(const std::string& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator>=(const RWCopyOnWriteCConstSubString& lhs, const char* rhs);

bool
operator>=(const char* lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator>=(const RWCopyOnWriteCConstSubString& lhs, char rhs);

bool
operator>=(char lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator> (const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCString& rhs);

bool
operator> (const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator> (const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCString& rhs);

bool
operator> (const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator> (const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCString& rhs);

bool
operator> (const RWCopyOnWriteCString& lhs, const std::string& rhs);

bool
operator> (const std::string& lhs, const RWCopyOnWriteCString& rhs);

bool
operator> (const RWCopyOnWriteCString& lhs, const char* rhs);

bool
operator> (const char* lhs, const RWCopyOnWriteCString& rhs);

bool
operator> (const RWCopyOnWriteCString& lhs, char rhs);

bool
operator> (char lhs, const RWCopyOnWriteCString& rhs);

bool
operator> (const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator> (const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator> (const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator> (const RWCopyOnWriteCSubString& lhs, const std::string& rhs);

bool
operator> (const std::string& lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator> (const RWCopyOnWriteCSubString& lhs, const char* rhs);

bool
operator> (const char* lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator> (const RWCopyOnWriteCSubString& lhs, char rhs);

bool
operator> (char lhs, const RWCopyOnWriteCSubString& rhs);

bool
operator> (const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator> (const RWCopyOnWriteCConstSubString& lhs, const std::string& rhs);

bool
operator> (const std::string& lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator> (const RWCopyOnWriteCConstSubString& lhs, const char* rhs);

bool
operator> (const char* lhs, const RWCopyOnWriteCConstSubString& rhs);

bool
operator> (const RWCopyOnWriteCConstSubString& lhs, char rhs);

bool
operator> (char lhs, const RWCopyOnWriteCConstSubString& rhs);


//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                               Inlines                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

inline RWCopyOnWriteCString
operator+(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteCString(lhs.data(), lhsLen, rhs.data(), rhsLen);
}

inline RWCopyOnWriteCString
operator+(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteCString(lhs.data(), lhsLen, rhs.startData(), rhsLen);
}

inline RWCopyOnWriteCString
operator+(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteCString(lhs.startData(), lhsLen, rhs.data(), rhsLen);
}

inline RWCopyOnWriteCString
operator+(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteCString(lhs.data(), lhsLen, rhs.startData(), rhsLen);
}

inline RWCopyOnWriteCString
operator+(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteCString(lhs.startData(), lhsLen, rhs.data(), rhsLen);
}

inline RWCopyOnWriteCString
operator+(const RWCopyOnWriteCString& lhs,  const std::string& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWCopyOnWriteCString(rhs);
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteCString(lhs.data(), lhsLen, rhs.data(), rhsLen);
}

inline RWCopyOnWriteCString
operator+(const std::string& lhs, const RWCopyOnWriteCString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWCopyOnWriteCString(lhs);
    }
    return RWCopyOnWriteCString(lhs.data(), lhsLen, rhs.data(), rhsLen);
}

inline RWCopyOnWriteCString
operator+(const RWCopyOnWriteCString& lhs,  const char* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator+(const RWCopyOnWriteCString& lhs,  const char* rhs): null pointer");
    size_t rhsLen = strlen(rhs);
    if (!rhsLen) {
        return lhs;
    }
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWCopyOnWriteCString(rhs, rhsLen);
    }
    return RWCopyOnWriteCString(lhs.data(), lhsLen, rhs, rhsLen);
}

inline RWCopyOnWriteCString
operator+(const char* lhs, const RWCopyOnWriteCString& rhs)
{
    RW_PRECONDITION2(lhs != rwnil,
                     "operator+(const char* lhs, const RWCopyOnWriteCString& rhs): null pointer");
    size_t lhsLen = strlen(lhs);
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWCopyOnWriteCString(lhs, lhsLen);
    }
    return RWCopyOnWriteCString(lhs, lhsLen, rhs.data(), rhsLen);
}

inline RWCopyOnWriteCString
operator+(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteCString(lhs.startData(), lhsLen, rhs.startData(), rhsLen);
}

inline RWCopyOnWriteCString
operator+(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteCString(lhs.startData(), lhsLen, rhs.startData(), rhsLen);
}

inline RWCopyOnWriteCString
operator+(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteCString(lhs.startData(), lhsLen, rhs.startData(), rhsLen);
}

inline RWCopyOnWriteCString
operator+(const RWCopyOnWriteCSubString& lhs,  const std::string& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWCopyOnWriteCString(rhs);
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteCString(lhs.startData(), lhsLen, rhs.data(), rhsLen);
}

inline RWCopyOnWriteCString
operator+(const std::string& lhs, const RWCopyOnWriteCSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWCopyOnWriteCString(lhs);
    }
    return RWCopyOnWriteCString(lhs.data(), lhsLen, rhs.startData(), rhsLen);
}

inline RWCopyOnWriteCString
operator+(const RWCopyOnWriteCSubString& lhs,  const char* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator+(const RWCopyOnWriteCString& lhs,  const char* rhs): null pointer");
    size_t rhsLen = strlen(rhs);
    if (!rhsLen) {
        return lhs;
    }
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWCopyOnWriteCString(rhs, rhsLen);
    }
    return RWCopyOnWriteCString(lhs.startData(), lhsLen, rhs, rhsLen);
}

inline RWCopyOnWriteCString
operator+(const char* lhs, const RWCopyOnWriteCSubString& rhs)
{
    RW_PRECONDITION2(lhs != rwnil,
                     "operator+(const char* lhs, const RWCopyOnWriteCString& rhs): null pointer");
    size_t lhsLen = strlen(lhs);
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWCopyOnWriteCString(lhs, lhsLen);
    }
    return RWCopyOnWriteCString(lhs, lhsLen, rhs.startData(), rhsLen);
}

inline RWCopyOnWriteCString
operator+(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteCString(lhs.startData(), lhsLen, rhs.startData(), rhsLen);
}

inline RWCopyOnWriteCString
operator+(const RWCopyOnWriteCConstSubString& lhs,  const std::string& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWCopyOnWriteCString(rhs);
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteCString(lhs.startData(), lhsLen, rhs.data(), rhsLen);
}

inline RWCopyOnWriteCString
operator+(const std::string& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWCopyOnWriteCString(lhs);
    }
    return RWCopyOnWriteCString(lhs.data(), lhsLen, rhs.startData(), rhsLen);
}

inline RWCopyOnWriteCString
operator+(const RWCopyOnWriteCConstSubString& lhs,  const char* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator+(const RWCopyOnWriteCString& lhs,  const char* rhs): null pointer");
    size_t rhsLen = strlen(rhs);
    if (!rhsLen) {
        return lhs;
    }
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWCopyOnWriteCString(rhs, rhsLen);
    }
    return RWCopyOnWriteCString(lhs.startData(), lhsLen, rhs, rhsLen);
}

inline RWCopyOnWriteCString
operator+(const char* lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    RW_PRECONDITION2(lhs != rwnil,
                     "operator+(const char* lhs, const RWCopyOnWriteCString& rhs): null pointer");
    size_t lhsLen = strlen(lhs);
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWCopyOnWriteCString(lhs, lhsLen);
    }
    return RWCopyOnWriteCString(lhs, lhsLen, rhs.startData(), rhsLen);
}

inline RWvistream& operator>>(RWvistream& str, RWCopyOnWriteCString& cstr)
{
    cstr.restoreFrom(str);
    return str;
}

inline RWFile& operator>>(RWFile& file, RWCopyOnWriteCString& cstr)
{
    cstr.restoreFrom(file);
    return file;
}

inline RWvistream& operator>>(RWvistream& str, RWCopyOnWriteCString*& cstr)
{
    RWTScopedPointer<RWCopyOnWriteCString> newCstr(new RWCopyOnWriteCString);
    newCstr->restoreFrom(str);
    cstr = newCstr.release();
    return str;
}

inline RWFile& operator>>(RWFile& file, RWCopyOnWriteCString*& cstr)
{
    RWTScopedPointer<RWCopyOnWriteCString> newCstr(new RWCopyOnWriteCString);
    newCstr->restoreFrom(file);
    cstr = newCstr.release();
    return file;
}

inline RWvostream& operator<<(RWvostream& str, const RWCopyOnWriteCString& cstr)
{
    cstr.saveOn(str);
    return str;
}

inline RWFile& operator<<(RWFile& file, const RWCopyOnWriteCString& cstr)
{
    cstr.saveOn(file);
    return file;
}

inline RWCopyOnWriteCString
toLower(const RWCopyOnWriteCString& str)
{
    register size_t len = str.length();
    RWCopyOnWriteCString temp((char)0, len);

    register const char* uc = str.data();
    register char* lc = const_cast<char*>(temp.data());

    // Guard against tolower() being a macro:
    while (len--) {
        *lc++ = char(tolower(RW_STATIC_CAST(unsigned char, *uc)));
        uc++;
    }

    return temp;
}

inline RWCopyOnWriteCString
toUpper(const RWCopyOnWriteCString& str)
{
    register size_t len = str.length();
    RWCopyOnWriteCString temp((char)0, len);
    register const char* uc = str.data();
    register char* lc = const_cast <char*>(temp.data());
    // Guard against toupper() being a macro:
    while (len--) {
        *lc++ = char(toupper(RW_STATIC_CAST(unsigned char, *uc)));
        uc++;
    }
    return temp;
}

inline unsigned rwhash(const RWCopyOnWriteCString& s)
{
    return s.hash();
}
inline unsigned rwhash(const RWCopyOnWriteCString* s)
{
    return s->hash();
}

inline unsigned
RWCopyOnWriteCString::hash(const RWCopyOnWriteCString& str)
{
    return str.hash();
}


//////////////////////////////////////////////////////////////////////////
//                              Inlines
//////////////////////////////////////////////////////////////////////////

inline
RWCStringRef::RWCStringRef(size_t capacity, size_t nchars)
    : RWReference(1), capacity_(capacity), nchars_(nchars)
{
    data()[nchars_] = 0;
}

inline size_t
RWCStringRef::length() const
{
    return nchars_;
}

inline size_t
RWCStringRef::capacity() const
{
    return capacity_;
}

inline char*
RWCStringRef::data() const
{
    return const_cast<char*>(
               reinterpret_cast<const char*>(this) + (sizeof(*this) / sizeof(char)));
}

inline char&
RWCStringRef::operator[](size_t i)
{
    return data() [i];
}

inline char
RWCStringRef::operator[](size_t i) const
{
    return data() [i];
}

inline RWCopyOnWriteCSubString::RWCopyOnWriteCSubString(
    const RWCopyOnWriteCString& str, size_t pos, size_t n)
    : str_(const_cast<RWCopyOnWriteCString*>(&str)),
      begin_(pos),
      extent_(n)
{
#ifdef RW_DEBUG
    size_t N = str.length();

    // Allow zero lengthed and null substrings:
    if ((pos == RW_NPOS && n != 0) ||
            (pos != RW_NPOS && (pos > N || n > N || pos + n > N))) {
        subStringError(N, pos, n);
    }
#endif
}



inline RWCopyOnWriteCSubString::RWCopyOnWriteCSubString(const RWCopyOnWriteCSubString& sp)
    : str_(sp.str_), begin_(sp.begin_), extent_(sp.extent_)
{
}

inline size_t
RWCopyOnWriteCSubString::length() const
{
    return extent_;
}

inline size_t
RWCopyOnWriteCSubString::start() const
{
    return begin_;
}

inline bool
RWCopyOnWriteCSubString::isNull() const
{
    return begin_ == RW_NPOS;
}

inline int
RWCopyOnWriteCSubString::operator!() const
{
    return begin_ == RW_NPOS;
}

inline
RWCopyOnWriteCConstSubString::RWCopyOnWriteCConstSubString(const RWCopyOnWriteCSubString& sp)
    : str_(sp.str_), begin_(sp.begin_), extent_(sp.extent_)
{
}

inline
RWCopyOnWriteCConstSubString::RWCopyOnWriteCConstSubString(const RWCopyOnWriteCConstSubString& sp)
    : str_(sp.str_), begin_(sp.begin_), extent_(sp.extent_)
{
}

#if !defined(RW_DISABLE_DEPRECATED)

inline const char*
RWCopyOnWriteCConstSubString::data() const
{
    return startData();
}

#endif // RW_DISABLE_DEPRECATED

inline size_t
RWCopyOnWriteCConstSubString::length() const
{
    return extent_;
}

inline size_t
RWCopyOnWriteCConstSubString::start() const
{
    return begin_;
}

inline bool
RWCopyOnWriteCConstSubString::isNull() const
{
    return begin_ == RW_NPOS;
}

inline int
RWCopyOnWriteCConstSubString::operator!() const
{
    return begin_ == RW_NPOS;
}

inline RWCopyOnWriteCString::RWCopyOnWriteCString()
{

    data_ = RWCStringRef::getEmptyRep()->data();
}

inline RWCopyOnWriteCString::RWCopyOnWriteCString(RWSize_T ic)
{
    data_ = RWCStringRef::getRep(ic.value(), 0, this)->data();
}

inline RWCopyOnWriteCString::RWCopyOnWriteCString(const char* cs)
{
    RW_PRECONDITION(cs != rwnil);
    size_t N = strlen(cs);
    data_ = RWCStringRef::getRep(N, N, this)->data();
    if (N) {
        rw_memcpy_s(data_, capacity(), cs, N);
    }
}

inline RWCopyOnWriteCString::RWCopyOnWriteCString(const char* cs, size_t N)
{
    RW_PRECONDITION(cs != rwnil);
    data_ = RWCStringRef::getRep(N, N, this)->data();
    if (N) {
        rw_memcpy_s(data_, capacity(), cs, N);
    }
}

inline RWCopyOnWriteCString::RWCopyOnWriteCString(const std::string& str)
{
    size_t N = str.length();
    data_ = RWCStringRef::getRep(N, N, this)->data();
    if (N) {
        rw_memcpy_s(data_, capacity(), str.data(), N);
    }
}

inline void RWCopyOnWriteCString::initChar(char c)
{
    data_ = RWCStringRef::getRep(16, 1, this)->data();
    data_[0] = c;

}

inline
RWCopyOnWriteCString::RWCopyOnWriteCString(char c)
{
    initChar(c);
}

inline
RWCopyOnWriteCString::RWCopyOnWriteCString(unsigned char c)
{
    initChar(char(c));
}

inline
RWCopyOnWriteCString::RWCopyOnWriteCString(signed char c)
{
    initChar(char(c));
}

inline RWCopyOnWriteCString::RWCopyOnWriteCString(char c, size_t N)
{
    data_ = RWCStringRef::getRep(N, N, this)->data();
    if (N) {
        memset(data_, c, N);
    }
}

inline RWCopyOnWriteCString::RWCopyOnWriteCString(const RWCopyOnWriteCSubString& substr)
{
    size_t len = substr.isNull() ? 0 : substr.length();
    data_ = RWCStringRef::getRep(len, len, this)->data();
    if (len) {
        rw_memcpy_s(data_, capacity(), substr.startData(), len);
    }
}

inline RWCopyOnWriteCString::RWCopyOnWriteCString(const RWCopyOnWriteCConstSubString& substr)
{
    size_t len = substr.isNull() ? 0 : substr.length();
    data_ = RWCStringRef::getRep(len, len, this)->data();
    if (len) {
        rw_memcpy_s(data_, capacity(), substr.startData(), len);
    }
}

inline RWCStringRef*
RWCopyOnWriteCString::pref() const
{
    union {
        char* ign;
        RWCStringRef* ptr;
    } tmp;

    tmp.ign = data_;
    return tmp.ptr - 1;
}

inline RWCopyOnWriteCString::RWCopyOnWriteCString(const RWCopyOnWriteCString& S)
{
    RWCStringRef* refp = S.pref();
    refp->link();
    data_ = S.data_;
}

#if !defined(RW_NO_RVALUE_REFERENCES)

inline RWCopyOnWriteCString::RWCopyOnWriteCString(RWCopyOnWriteCString && str)
    : data_(RWCStringRef::getEmptyRep()->data())
{
    str.swap(*this);
}

#endif // !RW_NO_RVALUE_REFERENCES

inline RWCopyOnWriteCString::~RWCopyOnWriteCString()
{
    pref()->unLink();
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::assign(const RWCopyOnWriteCString& str)
{
    if (data() == str.data()) {
        return *this;
    }

    RWCStringRef* refp = str.pref();
    refp->link();
    pref()->unLink();
    data_ = str.data_;
    return *this;
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::assign(const std::string& str)
{
    if (str.empty()) {
        if (pref()->unsafeReferences() == 1) {
            pref()->nchars_ = 0;
            *data_ = '\0';
        }
        else {
            pref()->unLink();
            data_ = RWCStringRef::getEmptyRep()->data();
        }
        return *this;
    }

    return replace(0, length(), str.data(), str.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::assign(const SubString& str)
{
    if (!str.length()) {
        if (pref()->unsafeReferences() == 1) {
            pref()->nchars_ = 0;
            *data_ = '\0';
        }
        else {
            pref()->unLink();
            data_ = RWCStringRef::getEmptyRep()->data();
        }
        return *this;
    }

    return replace(0, length(), str.startData(), str.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::assign(const ConstSubString& str)
{
    if (!str.length()) {
        if (pref()->unsafeReferences() == 1) {
            pref()->nchars_ = 0;
            *data_ = '\0';
        }
        else {
            pref()->unLink();
            data_ = RWCStringRef::getEmptyRep()->data();
        }
        return *this;
    }

    return replace(0, length(), str.startData(), str.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::assign(const char* str, size_t len)
{
    RW_PRECONDITION(str != rwnil);
    if (!len) {
        if (pref()->unsafeReferences() == 1) {
            pref()->nchars_ = 0;
            *data_ = '\0';
        }
        else {
            pref()->unLink();
            data_ = RWCStringRef::getEmptyRep()->data();
        }
        return *this;
    }

    return replace(0, length(), str, len);
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::assign(const char* str)
{
    RW_PRECONDITION(str != rwnil);
    if (!*str) {
        if (pref()->unsafeReferences() == 1) {
            pref()->nchars_ = 0;
            *data_ = '\0';
        }
        else {
            pref()->unLink();
            data_ = RWCStringRef::getEmptyRep()->data();
        }
        return *this;
    }

    return replace(0, length(), str, strlen(str));
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::assign(char c, size_t count)
{
    if (!count) {
        if (pref()->unsafeReferences() == 1) {
            pref()->nchars_ = 0;
            *data_ = '\0';
        }
        else {
            pref()->unLink();
            data_ = RWCStringRef::getEmptyRep()->data();
        }
        return *this;
    }
    clobber(count);
    memset(data_, c, count);
    data_[pref()->nchars_ = count] = '\0';
    return *this;
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::operator=(char c)
{
    assign(c);
    return *this;
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::operator=(const char* cs)
{
    assign(cs);
    return *this;
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::operator=(const std::string& str)
{
    assign(str);
    return *this;
}

RW_SUPPRESS_OP_ASSIGN_SELF_CHECK_WARNING
inline RWCopyOnWriteCString&
RWCopyOnWriteCString::operator=(const RWCopyOnWriteCString& str)
{
    assign(str);
    return *this;
}
RW_RESTORE_OP_ASSIGN_SELF_CHECK_WARNING

#if !defined(RW_NO_RVALUE_REFERENCES)

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::operator=(RWCopyOnWriteCString && str)
{
    if (&str != this) {
        str.swap(*this);
    }

    return *this;
}

#endif // !RW_NO_RVALUE_REFERENCES


inline RWCopyOnWriteCString&
RWCopyOnWriteCString::operator=(const RWCopyOnWriteCString::SubString& str)
{
    assign(str);
    return *this;
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::operator=(const RWCopyOnWriteCString::ConstSubString& str)
{
    assign(str);
    return *this;
}

inline
RWCopyOnWriteCString::operator const char* () const
{
    return data_;
}

inline size_t
RWCopyOnWriteCString::length() const
{
    return pref()->nchars_;
}

inline RWspace
RWCopyOnWriteCString::binaryStoreSize() const
{
    return length() + sizeof(size_t);
}

inline size_t
RWCopyOnWriteCString::capacity() const
{
    return pref()->capacity();
}

inline void
RWCopyOnWriteCString::clone()
{
    RWCStringRef* temp = RWCStringRef::getRep(length(), length(), this);
    rw_memcpy_s(temp->data(), temp->capacity(), data(), length());
    pref()->unLink();
    data_ = temp->data();

    RW_POSTCONDITION(pref()->unsafeReferences() == 1 || temp == RWCStringRef::getEmptyRep());
}

inline void
RWCopyOnWriteCString::clone(size_t nc)
{
    size_t len = length();
    if (len > nc) {
        len = nc;
    }
    RWCStringRef* temp = RWCStringRef::getRep(nc, len, this);
    rw_memcpy_s(temp->data(), temp->capacity(), data(), len);
    pref()->unLink();
    data_ = temp->data();
    RW_POSTCONDITION(pref()->unsafeReferences() == 1 || temp == RWCStringRef::getEmptyRep());
}

inline size_t
RWCopyOnWriteCString::capacity(size_t nc)
{
    if (nc > capacity()) {
        clone(nc);
    }

    RW_POSTCONDITION(capacity() >= length());
    return capacity();
}

inline int
RWCopyOnWriteCString::collate(const char* str) const
{
    RW_PRECONDITION2(str != rwnil,
                     "RWCopyOnWriteCString::collate(const char* str) const: null pointer");
    return strcoll(data(), str);
}

inline int
RWCopyOnWriteCString::compareTo(const std::string& str, caseCompare cmp) const
{
    if (cmp == exact) {
        return rwCStrCompareExact(data(), length(), str.data(), str.length());
    }
    else if (cmp == ignoreCase) {
        return rwCStrCompareIgnoreCase(data(), length(), str.data(), str.length());
    }
    else {
        return rwCStrCompareIgnoreCaseStrict(data(), length(), str.data(), str.length());
    }
}

inline int
RWCopyOnWriteCString::compareTo(const RWCopyOnWriteCString& str, caseCompare cmp) const
{
    if (cmp == exact) {
        return rwCStrCompareExact(data(), length(), str.data(), str.length());
    }
    else if (cmp == ignoreCase) {
        return rwCStrCompareIgnoreCase(data(), length(), str.data(), str.length());
    }
    else {
        return rwCStrCompareIgnoreCaseStrict(data(), length(), str.data(), str.length());
    }
}

inline int
RWCopyOnWriteCString::compareTo(const RWCopyOnWriteCString::SubString& str, caseCompare cmp) const
{
    if (cmp == exact) {
        return rwCStrCompareExact(data(), length(), str.startData(), str.length());
    }
    else if (cmp == ignoreCase) {
        return rwCStrCompareIgnoreCase(data(), length(), str.startData(), str.length());
    }
    else {
        return rwCStrCompareIgnoreCaseStrict(data(), length(), str.startData(), str.length());
    }
}

inline int
RWCopyOnWriteCString::compareTo(const RWCopyOnWriteCString::ConstSubString& str, caseCompare cmp) const
{
    if (cmp == exact) {
        return rwCStrCompareExact(data(), length(), str.startData(), str.length());
    }
    else if (cmp == ignoreCase) {
        return rwCStrCompareIgnoreCase(data(), length(), str.startData(), str.length());
    }
    else {
        return rwCStrCompareIgnoreCaseStrict(data(), length(), str.startData(), str.length());
    }
}

inline int
RWCopyOnWriteCString::compareTo(const char* str, size_t len, caseCompare cmp) const
{
    if (cmp == exact) {
        return rwCStrCompareExact(data(), length(), str, len);
    }
    else if (cmp == ignoreCase) {
        return rwCStrCompareIgnoreCase(data(), length(), str, len);
    }
    else {
        return rwCStrCompareIgnoreCaseStrict(data(), length(), str, len);
    }
}

inline int
RWCopyOnWriteCString::compareTo(const char* str, caseCompare cmp) const
{
    RW_PRECONDITION2(str != rwnil,
                     "RWCopyOnWriteCString::compareTo(const char* str, caseCompare cmp) const: "
                     "null pointer");
    return compareTo(str, strlen(str), cmp);
}

inline int
RWCopyOnWriteCString::compareTo(const std::string* st, caseCompare cmp) const
{
    RW_PRECONDITION2(st != rwnil,
                     "RWCopyOnWriteCString::compareTo(const std::string* str, caseCompare cmp) "
                     "const: null pointer");
    return compareTo(*st, cmp);
}

inline int
RWCopyOnWriteCString::compareTo(const RWCopyOnWriteCString* st, caseCompare cmp) const
{
    RW_PRECONDITION2(st != rwnil,
                     "RWCopyOnWriteCString::compareTo(const RWCopyOnWriteCString* str, caseCompare cmp) "
                     "const: null pointer");
    return compareTo(*st, cmp);
}

inline const char*
RWCopyOnWriteCString::data() const
{
    return data_;
}

inline size_t
RWCopyOnWriteCString::firstNotOf(char c, size_t pos) const
{
    return rwCStrFindFirstNotOf(data(), length(), c, pos);
}

inline size_t
RWCopyOnWriteCString::firstNotOf(const char* str, size_t pos, size_t len) const
{
    return rwCStrFindFirstNotOf(data(), length(), str, len, pos);
}

inline size_t
RWCopyOnWriteCString::firstNotOf(const char* str, size_t pos) const
{
    RW_PRECONDITION2(str != rwnil,
                     "RWCopyOnWriteCString::firstNotOf(const char* str, size_t pos) const: null pointer");
    return firstNotOf(str, pos, strlen(str));
}

inline size_t
RWCopyOnWriteCString::firstNotOf(const RWCopyOnWriteCString& str, size_t pos) const
{
    return firstNotOf(str.data(), pos, str.length());
}

inline size_t
RWCopyOnWriteCString::firstNotOf(const RWCopyOnWriteCString::SubString& str, size_t pos) const
{
    return firstNotOf(str.startData(), pos, str.length());
}

inline size_t
RWCopyOnWriteCString::firstNotOf(const RWCopyOnWriteCString::ConstSubString& str, size_t pos) const
{
    return firstNotOf(str.startData(), pos, str.length());
}

inline size_t
RWCopyOnWriteCString::firstNotOf(const std::string& str, size_t pos) const
{
    return firstNotOf(str.data(), pos, str.length());
}

inline size_t
RWCopyOnWriteCString::firstOf(char c, size_t pos) const
{
    return rwCStrFindFirstOf(data(), length(), c, pos);
}

inline size_t
RWCopyOnWriteCString::firstOf(const char* str, size_t pos, size_t len) const
{
    return rwCStrFindFirstOf(data(), length(), str, len, pos);
}

inline size_t
RWCopyOnWriteCString::firstOf(const char* str, size_t pos) const
{
    RW_PRECONDITION2(str != rwnil,
                     "RWCopyOnWriteCString::firstOf(const char* str, size_t pos) const: null pointer");
    return firstOf(str, pos, strlen(str));
}

inline size_t
RWCopyOnWriteCString::firstOf(const RWCopyOnWriteCString& str, size_t pos) const
{
    return firstOf(str.data(), pos, str.length());
}

inline size_t
RWCopyOnWriteCString::firstOf(const RWCopyOnWriteCString::SubString& str, size_t pos) const
{
    return firstOf(str.startData(), pos, str.length());
}

inline size_t
RWCopyOnWriteCString::firstOf(const RWCopyOnWriteCString::ConstSubString& str, size_t pos) const
{
    return firstOf(str.startData(), pos, str.length());
}

inline size_t
RWCopyOnWriteCString::firstOf(const std::string& str, size_t pos) const
{
    return firstOf(str.data(), pos, str.length());
}

#if !defined(RW_DISABLE_DEPRECATED)

inline size_t
RWCopyOnWriteCString::first(char c) const
{
    return firstOf(c);
}

inline size_t
RWCopyOnWriteCString::first(char c, size_t) const
{
    return firstOf(c);
}

inline size_t
RWCopyOnWriteCString::first(const char* s, size_t N) const
{
    return firstOf(s, 0, N);
}

inline size_t
RWCopyOnWriteCString::first(const char* s) const
{
    return firstOf(s);
}

#endif // RW_DISABLE_DEPRECATED

inline bool
RWCopyOnWriteCString::isNull() const
{
    return pref()->nchars_ == 0;
}

inline size_t
RWCopyOnWriteCString::lastNotOf(char c, size_t pos) const
{
    return rwCStrFindLastNotOf(data(), length(), c, pos);
}

inline size_t
RWCopyOnWriteCString::lastNotOf(const char* str, size_t pos, size_t len) const
{
    return rwCStrFindLastNotOf(data(), length(), str, len, pos);
}

inline size_t
RWCopyOnWriteCString::lastNotOf(const char* str, size_t pos) const
{
    RW_PRECONDITION2(str != rwnil,
                     "RWCopyOnWriteCString::lastNotOf(const char* str, size_t pos) const: null pointer");
    return lastNotOf(str, pos, strlen(str));
}

inline size_t
RWCopyOnWriteCString::lastNotOf(const RWCopyOnWriteCString& str, size_t pos) const
{
    return lastNotOf(str.data(), pos, str.length());
}

inline size_t
RWCopyOnWriteCString::lastNotOf(const RWCopyOnWriteCString::SubString& str, size_t pos) const
{
    return lastNotOf(str.startData(), pos, str.length());
}

inline size_t
RWCopyOnWriteCString::lastNotOf(const RWCopyOnWriteCString::ConstSubString& str, size_t pos) const
{
    return lastNotOf(str.startData(), pos, str.length());
}

inline size_t
RWCopyOnWriteCString::lastNotOf(const std::string& str, size_t pos) const
{
    return lastNotOf(str.data(), pos, str.length());
}

inline size_t
RWCopyOnWriteCString::lastOf(char c, size_t pos) const
{
    return rwCStrFindLastOf(data(), length(), c, pos);
}

inline size_t
RWCopyOnWriteCString::lastOf(const char* str, size_t pos, size_t len) const
{
    return rwCStrFindLastOf(data(), length(), str, len, pos);
}

inline size_t
RWCopyOnWriteCString::lastOf(const char* str, size_t pos) const
{
    RW_PRECONDITION2(str != rwnil,
                     "RWCopyOnWriteCString::lastOf(const char* str, size_t pos) const: null pointer");
    return lastOf(str, pos, strlen(str));
}

inline size_t
RWCopyOnWriteCString::lastOf(const RWCopyOnWriteCString& str, size_t pos) const
{
    return lastOf(str.data(), pos, str.length());
}

inline size_t
RWCopyOnWriteCString::lastOf(const RWCopyOnWriteCString::SubString& str, size_t pos) const
{
    return lastOf(str.startData(), pos, str.length());
}

inline size_t
RWCopyOnWriteCString::lastOf(const RWCopyOnWriteCString::ConstSubString& str, size_t pos) const
{
    return lastOf(str.startData(), pos, str.length());
}

inline size_t
RWCopyOnWriteCString::lastOf(const std::string& str, size_t pos) const
{
    return lastOf(str.data(), pos, str.length());
}

#if !defined(RW_DISABLE_DEPRECATED)

inline size_t
RWCopyOnWriteCString::last(char c) const
{
    return lastOf(c);
}

inline size_t
RWCopyOnWriteCString::last(char c, size_t) const
{
    return lastOf(c);
}

inline size_t
RWCopyOnWriteCString::last(const char* s, size_t N) const
{
    return lastOf(s, RW_NPOS, N);
}

inline size_t
RWCopyOnWriteCString::last(const char* s) const
{
    return lastOf(s);
}

#endif // RW_DISABLE_DEPRECATED

inline void RWCopyOnWriteCString::cow()
{
    if (pref()->unsafeReferences() > 1) {
        clone();
    }
}

inline void
RWCopyOnWriteCString::cow(size_t nc)
{
    if (pref()->unsafeReferences() > 1 || capacity() < nc) {
        clone(nc);
    }
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::append(char c, size_t rep)
{
    size_t tot;
    cow(tot = length() + rep);
    memset(data_ + length(), c, rep);

    data_[pref()->nchars_ = tot] = '\0';

    return *this;
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::append(const char* cs, size_t N)
{
    return replace(length(), 0, cs, N);
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::append(const char* cs)
{
    return append(cs, strlen(cs));
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::append(const std::string& s)
{
    return append(s.data(), s.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::append(const RWCopyOnWriteCString& s)
{
    return append(s.data(), s.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::append(const RWCopyOnWriteCString::SubString& s)
{
    return append(s.startData(), s.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::append(const RWCopyOnWriteCString::ConstSubString& s)
{
    return append(s.startData(), s.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::append(const std::string& s, size_t N)
{
    return append(s.data(), rwmin(N, s.length()));
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::append(const RWCopyOnWriteCString& s, size_t N)
{
    return append(s.data(), rwmin(N, s.length()));
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::operator+=(char c)
{
    return append(c);
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::operator+=(const char* cs)
{
    return append(cs);
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::operator+=(const RWCopyOnWriteCString& s)
{
    return append(s);
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::operator+=(const RWCopyOnWriteCString::SubString& str)
{
    return append(str);
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::operator+=(const RWCopyOnWriteCString::ConstSubString& str)
{
    return append(str);
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::operator+=(const std::string& s)
{
    return append(s);
}

#ifndef RW_NO_LOCALE
inline int
RWCopyOnWriteCString::collate(const std::string& st) const
{
    return strcoll(data(), st.data());
}

inline int
RWCopyOnWriteCString::collate(const RWCopyOnWriteCString& st) const
{
    return strcoll(data(), st.data());
}
#endif

inline size_t
RWCopyOnWriteCString::index(const char* pattern,
                            size_t plen,
                            size_t startIndex,
                            caseCompare cmp) const
{
    if (cmp == exact) {
        return rwCStrFindExact(data(), length(), pattern, plen, startIndex);
    }
    else {
        return rwCStrFindIgnoreCase(data(), length(), pattern, plen, startIndex);
    }
}

inline size_t
RWCopyOnWriteCString::index(const std::string& pat,
                            size_t patlen, size_t i, caseCompare cmp) const
{
    return index(pat.data(), patlen, i, cmp);
}

inline size_t
RWCopyOnWriteCString::index(const RWCopyOnWriteCString& pat,
                            size_t patlen, size_t i, caseCompare cmp) const
{
    return index(pat.data(), patlen, i, cmp);
}

inline size_t
RWCopyOnWriteCString::index(const char s, size_t i, caseCompare cmp) const
{
    return index(&s, 1, i, cmp);
}

inline size_t
RWCopyOnWriteCString::index(const char* s, size_t i, caseCompare cmp) const
{
    return index(s, strlen(s), i, cmp);
}

inline size_t
RWCopyOnWriteCString::index(const std::string& s,
                            size_t i, caseCompare cmp) const
{
    return index(s.data(), s.length(), i, cmp);
}

inline size_t
RWCopyOnWriteCString::index(const RWCopyOnWriteCString& s,
                            size_t i, caseCompare cmp) const
{
    return index(s.data(), s.length(), i, cmp);
}

inline size_t
RWCopyOnWriteCString::index(const RWCopyOnWriteCString::SubString& s,
                            size_t i, caseCompare cmp) const
{
    return index(s.startData(), s.length(), i, cmp);
}

inline size_t
RWCopyOnWriteCString::index(const RWCopyOnWriteCString::ConstSubString& s,
                            size_t i, caseCompare cmp) const
{
    return index(s.startData(), s.length(), i, cmp);
}

inline bool
RWCopyOnWriteCString::contains(const std::string& pat, caseCompare cmp) const
{
    return index(pat.data(), pat.length(), (size_t)0, cmp) != RW_NPOS;
}

inline bool
RWCopyOnWriteCString::contains(const RWCopyOnWriteCString& pat, caseCompare cmp) const
{
    return index(pat.data(), pat.length(), (size_t)0, cmp) != RW_NPOS;
}

inline bool
RWCopyOnWriteCString::contains(const RWCopyOnWriteCString::SubString& pat, caseCompare cmp) const
{
    return index(pat.startData(), pat.length(), (size_t)0, cmp) != RW_NPOS;
}

inline bool
RWCopyOnWriteCString::contains(const RWCopyOnWriteCString::ConstSubString& pat, caseCompare cmp) const
{
    return index(pat.startData(), pat.length(), (size_t)0, cmp) != RW_NPOS;
}

inline bool
RWCopyOnWriteCString::contains(const char* s, size_t len, caseCompare cmp) const
{
    return index(s, len, (size_t)0, cmp) != RW_NPOS;
}

inline bool
RWCopyOnWriteCString::contains(const char* s, caseCompare cmp) const
{
    return index(s, strlen(s), (size_t)0, cmp) != RW_NPOS;
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::insert(size_t pos, const char* cs, size_t N)
{
    return replace(pos, 0, cs, N);
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::insert(size_t pos, const char* cs)
{
    return insert(pos, cs, strlen(cs));
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::insert(size_t pos, const std::string& cstr)
{
    return insert(pos, cstr.data(), cstr.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::insert(size_t pos, const RWCopyOnWriteCString& cstr)
{
    return insert(pos, cstr.data(), cstr.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::insert(size_t pos, const RWCopyOnWriteCString::SubString& cstr)
{
    return insert(pos, cstr.startData(), cstr.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::insert(size_t pos, const RWCopyOnWriteCString::ConstSubString& cstr)
{
    return insert(pos, cstr.startData(), cstr.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::insert(size_t pos, const std::string& cstr, size_t N)
{
    return insert(pos, cstr.data(), rwmin(N, cstr.length()));
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::insert(size_t pos, const RWCopyOnWriteCString& cstr, size_t N)
{
    return insert(pos, cstr.data(), rwmin(N, cstr.length()));
}

inline size_t
RWCopyOnWriteCString::mbLength() const
{
    return
        RWCopyOnWriteCString::mbLength(data(), length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::prepend(const char* cs, size_t N)
{
    return replace(0, 0, cs, N);
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::prepend(const char* cs)
{
    return prepend(cs, strlen(cs));
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::prepend(const std::string& s)
{
    return prepend(s.data(), s.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::prepend(const RWCopyOnWriteCString& s)
{
    return prepend(s.data(), s.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::prepend(const RWCopyOnWriteCString::SubString& s)
{
    return prepend(s.startData(), s.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::prepend(const RWCopyOnWriteCString::ConstSubString& s)
{
    return prepend(s.startData(), s.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::prepend(const std::string& s, size_t N)
{
    return prepend(s.data(), rwmin(N, s.length()));
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::prepend(const RWCopyOnWriteCString& s, size_t N)
{
    return prepend(s.data(), rwmin(N, s.length()));
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::remove(size_t pos)
{
    return replace(pos, length() - pos, rwnil, 0);
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::remove(size_t pos, size_t n)
{
    return replace(pos, n, rwnil, 0);
}

inline void
RWCopyOnWriteCString::toLower()
{
    cow();
    register size_t N = length();
    register char* p = data_;
    while (N--) {
        *p = char(tolower(RW_STATIC_CAST(unsigned char, *p)));
        p++;
    }
}

inline void
RWCopyOnWriteCString::toUpper()
{
    cow();
    register size_t N = length();
    register char* p = data_;
    while (N--) {
        *p = char(toupper(RW_STATIC_CAST(unsigned char, *p)));
        p++;
    }
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::replace(size_t pos, size_t n, const char* cs)
{
    return replace(pos, n, cs, strlen(cs));
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::replace(size_t pos, size_t n, const std::string& cstr)
{
    return replace(pos, n, cstr.data(), cstr.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::replace(size_t pos, size_t n, const RWCopyOnWriteCString& cstr)
{
    return replace(pos, n, cstr.data(), cstr.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::replace(size_t pos, size_t n, const RWCopyOnWriteCString::SubString& cstr)
{
    return replace(pos, n, cstr.startData(), cstr.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::replace(size_t pos, size_t n, const RWCopyOnWriteCString::ConstSubString& cstr)
{
    return replace(pos, n, cstr.startData(), cstr.length());
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::replace(size_t pos, size_t n1,
                              const std::string& cstr, size_t n2)
{
    return replace(pos, n1, cstr.data(), rwmin(cstr.length(), n2));
}

inline RWCopyOnWriteCString&
RWCopyOnWriteCString::replace(size_t pos, size_t n1,
                              const RWCopyOnWriteCString& cstr, size_t n2)
{
    return replace(pos, n1, cstr.data(), rwmin(cstr.length(), n2));
}

inline void
RWCopyOnWriteCString::resize(size_t N)
{
    size_t len = length();
    if (N < len) {
        remove(N); // truncate
    }
    else if (N > len) {
        append(' ', N - len); // add spaces
    }
}

inline size_t
RWCopyOnWriteCString::rindex(const char* pat, size_t patlen,
                             size_t i, caseCompare cmp) const
{
    if (cmp == exact) {
        return rwCStrRFindExact(data(), length(), pat, patlen, i);
    }
    else {
        return rwCStrRFindIgnoreCase(data(), length(), pat, patlen, i);
    }
}

inline size_t
RWCopyOnWriteCString::rindex(const char* pat, caseCompare cmp) const
{
    RW_PRECONDITION2(pat != rwnil,
                     "RWCopyOnWriteCString::rindex(const char* pat, caseCompare cmp) const: "
                     "null pointer");
    return rindex(pat, strlen(pat), RW_NPOS, cmp);
}

inline size_t
RWCopyOnWriteCString::rindex(const RWCopyOnWriteCString& pat, size_t patlen,
                             size_t i, caseCompare cmp) const
{
    RW_ASSERT(patlen <= pat.length());
    return rindex(pat.data(), patlen, i, cmp);
}

inline size_t
RWCopyOnWriteCString::rindex(const char* pat, size_t i, caseCompare cmp) const
{
    RW_PRECONDITION2(pat != rwnil,
                     "RWCopyOnWriteCString::rindex(const char* pat, size_t i, "
                     "caseCompare cmp) const: null pointer");
    return rindex(pat, strlen(pat), i, cmp);
}

inline size_t
RWCopyOnWriteCString::rindex(const char pat, size_t i, caseCompare cmp) const
{
    return rindex(&pat, 1, i, cmp);
}

inline size_t
RWCopyOnWriteCString::rindex(const std::string& pat, size_t i, caseCompare cmp) const
{
    return rindex(pat.data(), pat.length(), i, cmp);
}

inline size_t
RWCopyOnWriteCString::rindex(const std::string& pat, size_t patlen,
                             size_t i, caseCompare cmp) const
{
    RW_ASSERT(patlen <= pat.length());
    return rindex(pat.data(), patlen, i, cmp);
}

inline size_t
RWCopyOnWriteCString::rindex(const RWCopyOnWriteCString& pat, size_t i, caseCompare cmp) const
{
    return rindex(pat.data(), pat.length(), i, cmp);
}

inline size_t
RWCopyOnWriteCString::rindex(const RWCopyOnWriteCString::SubString& pat, size_t i, caseCompare cmp) const
{
    return rindex(pat.startData(), pat.length(), i, cmp);
}

inline size_t
RWCopyOnWriteCString::rindex(const RWCopyOnWriteCString::ConstSubString& pat, size_t i, caseCompare cmp) const
{
    return rindex(pat.startData(), pat.length(), i, cmp);
}

inline void
RWCopyOnWriteCString::shrink()
{
    if (pref()->unsafeReferences() == 1) {
        if (length() < capacity()) {
            clone();
        }
    }
}

inline void
RWCopyOnWriteCString::swap(RWCopyOnWriteCString& str)
{
    char* tmp = data_;
    data_ = str.data_;
    str.data_ = tmp;
}

inline void
RWCopyOnWriteCString::assertElement(size_t i) const
{
    size_t len = length();
    if (i >= len) {
        throwBoundsErr(i, len);
    }
}

inline char&
RWCopyOnWriteCString::operator[](char i)
{
    assertElement(size_t (i));
    cow();
    return data_[size_t (i)];
}

inline char&
RWCopyOnWriteCString::operator[](signed char i)
{
    assertElement(size_t (i));
    cow();
    return data_[size_t (i)];
}

inline char&
RWCopyOnWriteCString::operator[](short i)
{
    assertElement(size_t (i));
    cow();
    return data_[size_t (i)];
}

inline char&
RWCopyOnWriteCString::operator[](int i)
{
    assertElement(size_t (i));
    cow();
    return data_[size_t (i)];
}

inline char&
RWCopyOnWriteCString::operator[](long i)
{
    assertElement(size_t (i));
    cow();
    return data_[size_t (i)];
}

inline char&
RWCopyOnWriteCString::operator[](unsigned char i)
{
    assertElement(size_t (i));
    cow();
    return data_[size_t (i)];
}

inline char&
RWCopyOnWriteCString::operator[](unsigned short i)
{
    assertElement(size_t (i));
    cow();
    return data_[size_t (i)];
}

inline char&
RWCopyOnWriteCString::operator[](unsigned int i)
{
    assertElement(size_t (i));
    cow();
    return data_[size_t (i)];
}

inline char&
RWCopyOnWriteCString::operator[](unsigned long i)
{
    assertElement(size_t (i));
    cow();
    return data_[size_t (i)];
}

#if !defined(RW_NO_LONG_LONG)
inline char&
RWCopyOnWriteCString::operator[](long long i)
{
    assertElement(size_t (i));
    cow();
    return data_[size_t (i)];
}

inline char&
RWCopyOnWriteCString::operator[](unsigned long long i)
{
    assertElement(size_t (i));
    cow();
    return data_[size_t (i)];
}
#endif

inline char&
RWCopyOnWriteCString::operator()(size_t i)
{
#ifdef RWBOUNDS_CHECK
    assertElement(i);
#endif
    cow();
    return data_[i];
}

inline char
RWCopyOnWriteCString::operator[](char i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWCopyOnWriteCString::operator[](signed char i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWCopyOnWriteCString::operator[](short i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWCopyOnWriteCString::operator[](int i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWCopyOnWriteCString::operator[](long i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWCopyOnWriteCString::operator[](unsigned char i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWCopyOnWriteCString::operator[](unsigned short i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWCopyOnWriteCString::operator[](unsigned int i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWCopyOnWriteCString::operator[](unsigned long i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

#if !defined(RW_NO_LONG_LONG)
inline char
RWCopyOnWriteCString::operator[](long long i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWCopyOnWriteCString::operator[](unsigned long long i) const
{
    assertElement(size_t (i));
    return data_[size_t (i)];
}

inline char
RWCopyOnWriteCString::operator()(size_t i) const
{

#  ifdef RWBOUNDS_CHECK
    assertElement(i);
#  endif
    return data_[i];

}
#endif

///////////////////////////////////////////////////////////////////////////////
//
// RWCopyOnWriteCSubString::startData()
//
// This member replaces data().
// startData() will remain undocumented. Please don't even ask.
// Use at your own risk. It may be deprecated in the future.
//
// Since RWCopyOnWriteCSubString works by referencing the RWCopyOnWriteCString's data,
// if you attempt to directly use the data() member of the RWCopyOnWriteCString,
// you will very likely be surprised by the result, which will be null
// terminated not at the extent of the substring,
// but at the end of the RWCopyOnWriteCString.
//
///////////////////////////////////////////////////////////////////////////////

inline const char* RWCopyOnWriteCSubString::startData() const
{
    return str_->data() + begin_;
}

#if !defined(RW_DISABLE_DEPRECATED)

inline const char* RWCopyOnWriteCSubString::data() const
{
    return startData();
}

#endif // RW_DISABLE_DEPRECATED

//------------------------------------------------------------------------------

inline void
RWCopyOnWriteCSubString::assertElement(size_t i) const
{
    size_t len = length();
    if (i >= len) {
        throwBoundsErr(i, len);
    }
}

inline char& RWCopyOnWriteCSubString::operator[](char i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char& RWCopyOnWriteCSubString::operator[](signed char i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char& RWCopyOnWriteCSubString::operator[](short i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char& RWCopyOnWriteCSubString::operator[](int i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char& RWCopyOnWriteCSubString::operator[](long i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char& RWCopyOnWriteCSubString::operator[](unsigned char i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char& RWCopyOnWriteCSubString::operator[](unsigned short i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char& RWCopyOnWriteCSubString::operator[](unsigned int i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char& RWCopyOnWriteCSubString::operator[](unsigned long i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

#if !defined(RW_NO_LONG_LONG)
inline char& RWCopyOnWriteCSubString::operator[](long long i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + (size_t)i);
}

inline char& RWCopyOnWriteCSubString::operator[](unsigned long long i)
{
    assertElement((size_t)i);
    return (*str_)(begin_ + (size_t)i);
}
#endif

inline char& RWCopyOnWriteCSubString::operator()(size_t i)
{
    assertElement(i);
    return (*str_)(begin_ + i);
}

// Access to elements of sub-string with bounds checking
inline char RWCopyOnWriteCSubString::operator[](char i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char RWCopyOnWriteCSubString::operator[](signed char i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char RWCopyOnWriteCSubString::operator[](short i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char RWCopyOnWriteCSubString::operator[](int i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char RWCopyOnWriteCSubString::operator[](long i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char RWCopyOnWriteCSubString::operator[](unsigned char i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char RWCopyOnWriteCSubString::operator[](unsigned short i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char RWCopyOnWriteCSubString::operator[](unsigned int i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char RWCopyOnWriteCSubString::operator[](unsigned long i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

#if !defined(RW_NO_LONG_LONG)
inline char RWCopyOnWriteCSubString::operator[](long long i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + (size_t)i);
}

inline char RWCopyOnWriteCSubString::operator[](unsigned long long i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + (size_t)i);
}
#endif

inline char RWCopyOnWriteCSubString::operator()(size_t i) const
{
    assertElement(i);
    return (*str_)(begin_ + i);
}

/*
 * A zero lengthed substring is legal. It can start
 * at any character. It is considered to be "pointing"
 * to just before the character.
 *
 * A "null" substring is a zero lengthed substring that
 * starts with the nonsense index RW_NPOS. It can
 * be detected with the member function isNull().
 */

// Sub-string operator
inline RWCopyOnWriteCSubString
RWCopyOnWriteCString::operator()(size_t pos, size_t len)
{
    return RWCopyOnWriteCSubString(*this, pos, len);
}

/*
 * Returns a substring matching "pattern", or the null substring
 * if there is no such match. It would be nice if this could be yet another
 * overloaded version of operator(), but this would result in a type
 * conversion ambiguity with operator(size_t, size_t).
 */
inline RWCopyOnWriteCSubString
RWCopyOnWriteCString::subString(
    const char* pattern, size_t startIndex, caseCompare cmp)
{
    RW_PRECONDITION2(
        pattern != rwnil,
        "RWCopyOnWriteCString::subString(const char*, "
        "size_t, caseCompare): null pointer");

    size_t len = strlen(pattern);
    size_t i = index(pattern, len, startIndex, cmp);
    return RWCopyOnWriteCSubString(*this, i, i == RW_NPOS ? 0 : len);
}

inline RWCopyOnWriteCConstSubString
RWCopyOnWriteCString::operator()(size_t start, size_t len) const
{
    return RWCopyOnWriteCSubString(*this, start, len);
}

inline RWCopyOnWriteCConstSubString
RWCopyOnWriteCString::subString(
    const char* pattern, size_t startIndex, caseCompare cmp) const
{
    RW_PRECONDITION2(
        pattern != rwnil,
        "RWCopyOnWriteCString::subString(const char*, "
        "size_t, caseCompare) const: null pointer");

    size_t len = strlen(pattern);
    size_t i = index(pattern, len, startIndex, cmp);
    return RWCopyOnWriteCSubString(*this, i, i == RW_NPOS ? 0 : len);
}

inline RWCopyOnWriteCSubString&
RWCopyOnWriteCSubString::operator=(const std::string& str)
{
    if (!isNull()) {
        size_t len = str.length();
        str_->replace(begin_, extent_, str.data(), len);
        extent_ = len;
    }
    return *this;
}

inline RWCopyOnWriteCSubString&
RWCopyOnWriteCSubString::operator=(const RWCopyOnWriteCString& str)
{
    if (!isNull()) {
        size_t len = str.length();
        str_->replace(begin_, extent_, str.data(), len);
        extent_ = len;
    }
    return *this;
}

inline RWCopyOnWriteCSubString&
RWCopyOnWriteCSubString::operator=(const RWCopyOnWriteCSubString& s)
{
    if (this == &s) {
        return *this;
    }

    if (!isNull()) {
        str_->replace(begin_, extent_, s.startData(), s.length());
        extent_ = s.length();
    }
    return *this;
}

inline RWCopyOnWriteCSubString&
RWCopyOnWriteCSubString::operator=(const RWCopyOnWriteCConstSubString& s)
{
    if (!isNull()) {
        str_->replace(begin_, extent_, s.startData(), s.length());
        extent_ = s.length();
    }
    return *this;
}

inline RWCopyOnWriteCSubString&
RWCopyOnWriteCSubString::operator=(const char* cs)
{
    if (!isNull()) {
        size_t len = strlen(cs);
        str_->replace(begin_, extent_, cs, len);
        extent_ = len;
    }
    return *this;
}

// Convert self to lower-case
inline void
RWCopyOnWriteCSubString::toLower()
{
    if (!isNull()) {
        str_->cow();
        register char* p = const_cast<char*>(str_->data() + begin_);
        size_t N = extent_;
        while (N--) {
            *p = char(tolower(RW_STATIC_CAST(unsigned char, *p)));
            p++;
        }
    }
}

// Convert self to upper-case
inline void
RWCopyOnWriteCSubString::toUpper()
{
    if (!isNull()) {
        str_->cow();
        register char* p = const_cast<char*>(str_->data() + begin_);
        size_t N = extent_;
        while (N--) {
            *p = char(toupper(RW_STATIC_CAST(unsigned char, *p)));
            p++;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
//                         RWCopyOnWriteCConstSubString                            //
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// RWCopyOnWriteCConstSubString::startData()
//
// This member replaces data().
// startData() will remain undocumented.  Please don't even ask.
// Use at your own risk. It may be deprecated in the future.
//
// Since RWCopyOnWriteCSubString works by referencing the RWCopyOnWriteCtring's data,
// if you attempt to directly use the data() member of the RWCopyOnWriteCString,
// you will very likely be surprised by the result, which will be null
// terminated not at the extent of the substring,
// but at the end of the RWCopyOnWriteCString.
//
///////////////////////////////////////////////////////////////////////////////

inline void
RWCopyOnWriteCConstSubString::assertElement(size_t i) const
{
    size_t len = length();
    if (i >= len) {
        throwBoundsErr(i, len);
    }
}

/*
  inline const char*
  RWCopyOnWriteCConstSubString::startData() const { return str_->data() + begin_; }
*/
inline char
RWCopyOnWriteCConstSubString::operator()(size_t i) const
{
    assertElement(i);
    return (*str_)(begin_ + i);
}

// Here all integer types are provided with an exact match
// to avoid ambiguities with the built in char[] operator
inline char
RWCopyOnWriteCConstSubString::operator[](char i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWCopyOnWriteCConstSubString::operator[](signed char i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWCopyOnWriteCConstSubString::operator[](short i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWCopyOnWriteCConstSubString::operator[](int i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWCopyOnWriteCConstSubString::operator[](long i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWCopyOnWriteCConstSubString::operator[](unsigned char i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWCopyOnWriteCConstSubString::operator[](unsigned short i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWCopyOnWriteCConstSubString::operator[](unsigned int i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

inline char
RWCopyOnWriteCConstSubString::operator[](unsigned long i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + i);
}

#if !defined(RW_NO_LONG_LONG)
inline char
RWCopyOnWriteCConstSubString::operator[](long long i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + (size_t)i);
}

inline char
RWCopyOnWriteCConstSubString::operator[](unsigned long long i) const
{
    assertElement((size_t)i);
    return (*str_)(begin_ + (size_t)i);
}
#endif

inline RWCopyOnWriteCConstSubString::RWCopyOnWriteCConstSubString
(const RWCopyOnWriteCString& str, size_t pos, size_t n)
    : str_(&str), begin_(pos), extent_(n)
{
#ifdef RW_DEBUG
    size_t len = str.length();

    // Allow zero lengthed and null substrings:
    if ((pos == RW_NPOS && n != 0) || (pos != RW_NPOS && pos + n > len)) {
        subStringError(len, pos, n);
    }
#endif
}

inline const char*
RWCopyOnWriteCConstSubString::startData() const
{
    return str_->data() + begin_;
}

inline bool
operator==(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCString& rhs)
{
    size_t lhsLen = lhs.length();
    size_t rhsLen = rhs.length();
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.data(), lhsLen, rhs.data(), rhsLen);
}

inline bool
operator==(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    size_t lhsLen = lhs.length();
    size_t rhsLen = rhs.length();
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.data(), lhsLen, rhs.startData(), rhsLen);
}

inline bool
operator==(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    size_t rhsLen = rhs.length();
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.data(), lhsLen, rhs.startData(), rhsLen);
}

inline bool
operator==(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWCopyOnWriteCString& lhs, const std::string& rhs)
{
    size_t lhsLen = lhs.length();
    size_t rhsLen = rhs.length();
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.data(), lhsLen, rhs.data(), rhsLen);
}

inline bool
operator==(const std::string& lhs, const RWCopyOnWriteCString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWCopyOnWriteCString& lhs, const char* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator==(const RWCopyOnWriteCString& lhs, const char* rhs): null pointer");
    size_t lhsLen = lhs.length();
    size_t rhsLen = strlen(rhs);
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.data(), lhsLen, rhs, rhsLen);
}

inline bool
operator==(const char* lhs, const RWCopyOnWriteCString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWCopyOnWriteCString& lhs, char rhs)
{
    return 1 == lhs.length() && lhs(0) == rhs;
}

inline bool
operator==(char lhs, const RWCopyOnWriteCString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    size_t lhsLen = lhs.length();
    size_t rhsLen = rhs.length();
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.startData(), lhsLen, rhs.startData(), rhsLen);
}

inline bool
operator==(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    size_t rhsLen = rhs.length();
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.startData(), lhsLen, rhs.startData(), rhsLen);
}

inline bool
operator==(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWCopyOnWriteCSubString& lhs, const std::string& rhs)
{
    size_t lhsLen = lhs.length();
    size_t rhsLen = rhs.length();
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.startData(), lhsLen, rhs.data(), rhsLen);
}

inline bool
operator==(const std::string& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWCopyOnWriteCSubString& lhs, const char* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator==(const RWCopyOnWriteCSubString& lhs, const char* rhs): null pointer");
    size_t lhsLen = lhs.length();
    size_t rhsLen = strlen(rhs);
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.startData(), lhsLen, rhs, rhsLen);
}

inline bool
operator==(const char* lhs, const RWCopyOnWriteCSubString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWCopyOnWriteCSubString& lhs, char rhs)
{
    return (1 == lhs.length() && lhs(0) == rhs);
}

inline bool
operator==(char lhs, const RWCopyOnWriteCSubString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    size_t rhsLen = rhs.length();
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.startData(), lhsLen, rhs.startData(), rhsLen);
}

inline bool
operator==(const RWCopyOnWriteCConstSubString& lhs, const std::string& rhs)
{
    size_t lhsLen = lhs.length();
    size_t rhsLen = rhs.length();
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.startData(), lhsLen, rhs.data(), rhsLen);
}

inline bool
operator==(const std::string& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWCopyOnWriteCConstSubString& lhs, const char* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator==(const RWCopyOnWriteCConstSubString& lhs, const char* rhs): "
                     "null pointer");
    size_t lhsLen = lhs.length();
    size_t rhsLen = strlen(rhs);
    if (lhsLen != rhsLen) {
        return false;
    }
    return 0 == rwCStrCompareExact(lhs.startData(), lhsLen, rhs, rhsLen);
}

inline bool
operator==(const char* lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator==(const RWCopyOnWriteCConstSubString& lhs, char rhs)
{
    return (1 == lhs.length() && lhs(0) == rhs);
}

inline bool
operator==(char lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return (rhs == lhs);
}

inline bool
operator!=(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWCopyOnWriteCString& lhs, const std::string& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const std::string& lhs, const RWCopyOnWriteCString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWCopyOnWriteCString& lhs,    const char* rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const char* lhs, const RWCopyOnWriteCString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWCopyOnWriteCString& lhs, char rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(char lhs, const RWCopyOnWriteCString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWCopyOnWriteCSubString& lhs, const std::string& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const std::string& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWCopyOnWriteCSubString& lhs, const char* rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const char* lhs, const RWCopyOnWriteCSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWCopyOnWriteCSubString& lhs, char rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(char lhs, const RWCopyOnWriteCSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWCopyOnWriteCConstSubString& lhs, const std::string& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const std::string& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWCopyOnWriteCConstSubString& lhs, const char* rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const char* lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(const RWCopyOnWriteCConstSubString& lhs, char rhs)
{
    return !(lhs == rhs);
}

inline bool
operator!=(char lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator< (const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.data(), lhs.length(), rhs.data(), rhs.length());
}

inline bool
operator< (const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.data(), lhs.length(), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs.data(), rhs.length());
}

inline bool
operator< (const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.data(), lhs.length(), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs.data(), rhs.length());
}

inline bool
operator< (const RWCopyOnWriteCString& lhs, const std::string& rhs)
{
    return 0 > rwCStrCompareExact(lhs.data(), lhs.length(), rhs.data(), rhs.length());
}

inline bool
operator< (const std::string& lhs, const RWCopyOnWriteCString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.data(), lhs.length(), rhs.data(), rhs.length());
}

inline bool
operator< (const RWCopyOnWriteCString& lhs, const char* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator< (const RWCopyOnWriteCString& lhs, const char* rhs): null pointer");
    return 0 > rwCStrCompareExact(lhs.data(), lhs.length(), rhs, strlen(rhs));
}

inline bool
operator< (const char* lhs, const RWCopyOnWriteCString& rhs)
{
    RW_PRECONDITION2(lhs != rwnil,
                     "operator< (const char* lhs, const RWCopyOnWriteCString& rhs): null pointer");
    return 0 > rwCStrCompareExact(lhs, strlen(lhs), rhs.data(), rhs.length());
}

inline bool
operator< (const RWCopyOnWriteCString& lhs, char rhs)
{
    return 0 == lhs.length() || lhs(0) < rhs;
}

inline bool
operator< (char lhs, const RWCopyOnWriteCString& rhs)
{
    const size_t len = rhs.length();
    return len > 0 && (lhs < rhs(0) || (lhs == rhs(0) && len > 1));
}

inline bool
operator< (const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWCopyOnWriteCSubString& lhs, const std::string& rhs)
{
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs.data(), rhs.length());
}

inline bool
operator< (const std::string& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.data(), lhs.length(), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWCopyOnWriteCSubString& lhs, const char* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator< (const RWCopyOnWriteCSubString& lhs, const char* rhs): null pointer");
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs, strlen(rhs));
}

inline bool
operator< (const char* lhs, const RWCopyOnWriteCSubString& rhs)
{
    RW_PRECONDITION2(lhs != rwnil,
                     "operator< (const char* lhs, const RWCopyOnWriteCSubString& rhs): null pointer");
    return 0 > rwCStrCompareExact(lhs, strlen(lhs), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWCopyOnWriteCSubString& lhs, char rhs)
{
    return 0 == lhs.length() || lhs(0) < rhs;
}

inline bool
operator< (char lhs, const RWCopyOnWriteCSubString& rhs)
{
    const size_t len = rhs.length();
    return len > 0 && (lhs < rhs(0) || (lhs == rhs(0) && len > 1));
}

inline bool
operator< (const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWCopyOnWriteCConstSubString& lhs, const std::string& rhs)
{
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs.data(), rhs.length());
}

inline bool
operator< (const std::string& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return 0 > rwCStrCompareExact(lhs.data(), lhs.length(), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWCopyOnWriteCConstSubString& lhs, const char* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator< (const RWCopyOnWriteCConstSubString& lhs, const char* rhs): null pointer");
    return 0 > rwCStrCompareExact(lhs.startData(), lhs.length(), rhs, strlen(rhs));
}

inline bool
operator< (const char* lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    RW_PRECONDITION2(lhs != rwnil,
                     "operator< (const char* lhs, const RWCopyOnWriteCConstSubString& rhs): null pointer");
    return 0 > rwCStrCompareExact(lhs, strlen(lhs), rhs.startData(), rhs.length());
}

inline bool
operator< (const RWCopyOnWriteCConstSubString& lhs, char rhs)
{
    return 0 == lhs.length() || lhs(0) < rhs;
}

inline bool
operator< (char lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    const size_t len = rhs.length();
    return len > 0 && (lhs < rhs(0) || (lhs == rhs(0) && len > 1));
}

inline bool
operator> (const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWCopyOnWriteCString& lhs, const std::string& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const std::string& lhs, const RWCopyOnWriteCString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWCopyOnWriteCString& lhs, const char* rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const char* lhs, const RWCopyOnWriteCString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWCopyOnWriteCString& lhs, char rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (char lhs, const RWCopyOnWriteCString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWCopyOnWriteCSubString& lhs, const std::string& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const std::string& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWCopyOnWriteCSubString& lhs, const char* rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const char* lhs, const RWCopyOnWriteCSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWCopyOnWriteCSubString& lhs, char rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (char lhs, const RWCopyOnWriteCSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWCopyOnWriteCConstSubString& lhs, const std::string& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const std::string& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWCopyOnWriteCConstSubString& lhs, const char* rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const char* lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (const RWCopyOnWriteCConstSubString& lhs, char rhs)
{
    return (rhs < lhs);
}

inline bool
operator> (char lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return (rhs < lhs);
}

inline bool
operator<=(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWCopyOnWriteCString& lhs, const std::string& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const std::string& lhs, const RWCopyOnWriteCString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWCopyOnWriteCString& lhs, const char* rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const char* lhs, const RWCopyOnWriteCString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWCopyOnWriteCString& lhs, char rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(char lhs, const RWCopyOnWriteCString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWCopyOnWriteCSubString& lhs, const std::string& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const std::string& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWCopyOnWriteCSubString& lhs, const char* rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const char* lhs, const RWCopyOnWriteCSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWCopyOnWriteCSubString& lhs, char rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(char lhs, const RWCopyOnWriteCSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWCopyOnWriteCConstSubString& lhs, const std::string& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const std::string& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWCopyOnWriteCConstSubString& lhs, const char* rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const char* lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(const RWCopyOnWriteCConstSubString& lhs, char rhs)
{
    return !(lhs > rhs);
}

inline bool
operator<=(char lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return !(lhs > rhs);
}

inline bool
operator>=(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWCopyOnWriteCString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWCopyOnWriteCString& lhs, const std::string& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const std::string& lhs, const RWCopyOnWriteCString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWCopyOnWriteCString& lhs, const char* rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const char* lhs, const RWCopyOnWriteCString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWCopyOnWriteCString& lhs, char rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(char lhs, const RWCopyOnWriteCString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWCopyOnWriteCSubString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWCopyOnWriteCSubString& lhs, const std::string& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const std::string& lhs, const RWCopyOnWriteCSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWCopyOnWriteCSubString& lhs, const char* rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const char* lhs, const RWCopyOnWriteCSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWCopyOnWriteCSubString& lhs, char rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(char lhs, const RWCopyOnWriteCSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWCopyOnWriteCConstSubString& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWCopyOnWriteCConstSubString& lhs, const std::string& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const std::string& lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWCopyOnWriteCConstSubString& lhs, const char* rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const char* lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(const RWCopyOnWriteCConstSubString& lhs, char rhs)
{
    return !(lhs < rhs);
}

inline bool
operator>=(char lhs, const RWCopyOnWriteCConstSubString& rhs)
{
    return !(lhs < rhs);
}

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an RWCopyOnWriteCString.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<RWCopyOnWriteCString> : public std::unary_function<RWCopyOnWriteCString, size_t> {
    /**
     * Constructs a hash object instance with the specified comparison
     * type.
     */
    RWTHash<RWCopyOnWriteCString>(RWCopyOnWriteCString::caseCompare cmp = RWCopyOnWriteCString::exact)
        : cmp_(cmp) {}

    /**
     * Returns a hash of \a obj. This function is the equivalent of calling
     *
     * @code
     * obj.hash(cmp);
     * @endcode
     *
     * Where \a cmp is the comparison type specified at construction.
     */
    size_t operator()(const RWCopyOnWriteCString& obj) const {
        return obj.hash(cmp_);
    }

private:
    RWCopyOnWriteCString::caseCompare cmp_;
};


#endif // #define RW_TOOLS_COWCSTRING_H
