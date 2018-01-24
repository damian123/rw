#ifndef RW_TOOLS_COWWSTRING_H
#define RW_TOOLS_COWWSTRING_H

/**********************************************************************
 *
 * Declarations for RWCopyOnWriteWString --- wide character strings.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/cowwstring.h#1 $
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
#include <rw/ref.h>

#include <rw/tools/cowcstring.h>
#include <rw/tools/hash.h>
#include <rw/tools/ristream.h>
#include <rw/tools/rostream.h>
#include <rw/tools/string.h>

#include <rw/rwwchar.h>

#include <rw/vstream.h>

#if defined(RW_COPY_ON_WRITE_STRING)
#  define RWCopyOnWriteWString RWWString
#  define RWCopyOnWriteWSubString RWWSubString
#  define RWCopyOnWriteWConstSubString RWWConstSubString
#endif


class RW_TOOLS_GLOBAL RWCopyOnWriteCString;
class RW_TOOLS_GLOBAL RWCopyOnWriteWString;
class RW_TOOLS_SYMBOLIC  RWCopyOnWriteWSubString;
class RW_TOOLS_SYMBOLIC  RWCopyOnWriteWConstSubString;

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                             RWWStringRef                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


/*
 * This is the dynamically allocated part of a RWCopyOnWriteWString.
 * It maintains a reference count.
 * There are no public member functions.
 */

class RW_TOOLS_SYMBOLIC RWWStringRef : public RWReference
{
    RWWStringRef(size_t capacity, size_t nchars);

    static RWWStringRef*  getEmptyRep();
    static RWWStringRef*  getRep(size_t capac, size_t nchar);
    static RWWStringRef*  getRep(size_t capac, size_t nchar, void* where);

    void          link();         // connect to a stringref
    void          unLink();       // Disconnect from a stringref, maybe delete it

    size_t        length() const {
        return nchars_;
    }
    size_t        capacity() const {
        return capacity_;
    }

    wchar_t* data() const {
        return const_cast<wchar_t*>(
                   reinterpret_cast<const wchar_t*>(this) +
                   (sizeof(*this) / sizeof(wchar_t)));
    }

    wchar_t& operator[](size_t i) {
        return data() [i];
    }

    wchar_t  operator[](size_t i) const {
        return data() [i];
    }

    int           collate(const wchar_t*) const;
    size_t        first(wchar_t) const;
    size_t        first(const wchar_t*) const;
    size_t        first(const wchar_t*, size_t) const;
    unsigned      hash() const;
    unsigned      hashFoldCase() const;
    size_t        last(wchar_t) const;

    const size_t  capacity_;      // Max string length (excluding null)
    size_t        nchars_;        // String length (excluding terminating null)

    friend class RW_TOOLS_GLOBAL RWCopyOnWriteWString;
    friend class RW_TOOLS_SYMBOLIC RWCopyOnWriteWSubString;

};


/**
 * @ingroup string_processing_classes
 * @brief Alternate implementation of RWWSubString when
 * \c RW_COPY_ON_WRITE_STRING is defined.
 *
 * RWCopyOnWriteWSubString should be directly used only with instances
 * of class RWCopyOnWriteWString. RWWSubString should be used with instances
 * of class RWWString.
 *
 * @see RWWSubString for additional documentation.
 */
class RW_TOOLS_SYMBOLIC RWCopyOnWriteWSubString
{
public:
    RWCopyOnWriteWSubString(const RWCopyOnWriteWSubString& sp)
        : str_(sp.str_), begin_(sp.begin_), extent_(sp.extent_) {
        ;
    }

    RWCopyOnWriteWSubString& operator=(const wchar_t* wstr);

    RWCopyOnWriteWSubString& operator=(const std::wstring&);

    RWCopyOnWriteWSubString& operator=(const RWCopyOnWriteWString& wstr);

    RWCopyOnWriteWSubString& operator=(const RWCopyOnWriteWSubString& wstr);

    RWCopyOnWriteWSubString& operator=(const RWCopyOnWriteWConstSubString& wstr);

    wchar_t&      operator()(size_t i);

    wchar_t       operator()(size_t i) const;

    wchar_t&      operator[](size_t i);

    wchar_t       operator[](size_t i) const;

    const wchar_t*        startData() const;
    // Replaces data(). See definition below.
    // startData() will remain undocumented.  Please don't even ask.
    // Use at your own risk. It may be deprecated in the future.

    // DON'T USE THE FUNCTION BELOW!
    const wchar_t*        data() const;         // Deprecated.
    // This member is deprecated and will be removed in a future version.
    // It remains public only to maintain source compatibility.
    // Since RWCopyOnWriteWSubString works by referencing the RWCopyOnWriteWString's data,
    // if you attempt to directly use the data() member of the RWCopyOnWriteWString,
    // you will very likely be surprised by the result, which will be null
    // terminated not at the extent of the substring, but at the end of the
    // RWCopyOnWriteWString.

    size_t        length() const {
        return extent_;
    }

    size_t        start() const {
        return begin_;
    }

    void          toLower();

    void          toUpper();

    bool     isNull() const {
        return begin_ == RW_NPOS;
    }

    int           operator!() const {
        return begin_ == RW_NPOS;
    }

protected:

    void          subStringError(size_t, size_t, size_t) const;
    void          assertElement(size_t i) const;  // Verifies i is valid index

private:

    // NB: the only constructor is private:
    RWCopyOnWriteWSubString(const RWCopyOnWriteWString& s, size_t start, size_t len);

    RWCopyOnWriteWString*    str_;           // Referenced string
    size_t        begin_;         // Index of starting wchar_tacter
    size_t        extent_;        // Length of RWCopyOnWriteWSubString

    friend RW_TOOLS_SYMBOLIC
    bool operator==(const RWCopyOnWriteWSubString& s1, const RWCopyOnWriteWSubString& s2);

    friend RW_TOOLS_SYMBOLIC
    bool operator==(const RWCopyOnWriteWSubString& s1, const RWCopyOnWriteWConstSubString& s2);

    friend RW_TOOLS_SYMBOLIC
    bool operator==(const RWCopyOnWriteWSubString& s1, const RWCopyOnWriteWString& s2);

    friend RW_TOOLS_SYMBOLIC
    bool operator==(const RWCopyOnWriteWSubString& s1, const wchar_t* s2);

    friend class RW_TOOLS_GLOBAL RWCopyOnWriteWString;
    friend class RW_TOOLS_SYMBOLIC RWCopyOnWriteWConstSubString;

};

/**
 * @ingroup string_processing_classes
 * @brief Alternate implementation of RWWConstSubString when
 * \c RW_COPY_ON_WRITE_STRING is defined.
 *
 * RWCopyOnWriteWConstSubString should be directly used only with instances
 * of class RWCopyOnWriteWString. RWWConstSubString should be used with instances
 * of class RWWString.
 *
 * @see RWWConstSubString for additional documentation.
 */
class RW_TOOLS_SYMBOLIC RWCopyOnWriteWConstSubString
{
public:

    RWCopyOnWriteWConstSubString(const RWCopyOnWriteWSubString& sp)
        : str_(sp.str_), begin_(sp.begin_), extent_(sp.extent_) {
        ;
    }

    RWCopyOnWriteWConstSubString(const RWCopyOnWriteWConstSubString& sp)
        : str_(sp.str_), begin_(sp.begin_), extent_(sp.extent_) {
        ;
    }

    wchar_t operator()(size_t i) const;

    wchar_t operator[](short i) const;

    wchar_t operator[](int i) const;

    wchar_t operator[](long i) const;

    wchar_t operator[](unsigned short i) const;

    wchar_t operator[](unsigned int i) const;

    wchar_t operator[](unsigned long i) const;

#if !defined(RW_NO_LONG_LONG)
    wchar_t operator[](long long i) const;

    wchar_t operator[](unsigned long long i) const;
#endif


    inline const wchar_t* startData() const;                // Replaces data()

#if !defined(RW_DISABLE_DEPRECATED)

    RW_DEPRECATE_FUNC("Use RWCopyOnWriteWSubString::startData() instead")
    const wchar_t*   data() const {
        return startData();
    }

#endif

    size_t length() const {
        return extent_;
    }

    size_t start() const {
        return begin_;
    }

    bool isNull() const {
        return begin_ == RW_NPOS;
    }

    int            operator!() const {
        return begin_ == RW_NPOS;
    }

protected:

    void subStringError(size_t, size_t, size_t) const;
    void assertElement(std::wstring::size_type) const;  // Verifies i is valid index

private:

    inline RWCopyOnWriteWConstSubString(const RWCopyOnWriteWString& s, size_t start, size_t len);

    const RWCopyOnWriteWString*      str_;     // Referenced string
    size_t begin_;   // Index of starting character
    size_t extent_;  // Length of RWCopyOnWriteWConstSubString

    friend RW_TOOLS_SYMBOLIC
    bool operator==(const RWCopyOnWriteWConstSubString& s1, const RWCopyOnWriteWConstSubString& s2);

    friend RW_TOOLS_SYMBOLIC
    bool operator==(const RWCopyOnWriteWSubString& s1, const RWCopyOnWriteWConstSubString& s2);

    friend RW_TOOLS_SYMBOLIC
    bool operator==(const RWCopyOnWriteWConstSubString& s1, const RWCopyOnWriteWString& s2);

    friend RW_TOOLS_SYMBOLIC
    bool operator==(const RWCopyOnWriteWConstSubString& s1, const wchar_t* s2);

    friend class RWCopyOnWriteWString;
    friend class RWCopyOnWriteWSubString;
};


/**
 * @ingroup string_processing_classes
 * @brief Alternate implementation of RWWString when
 * \c RW_COPY_ON_WRITE_STRING is defined.
 *
 * Unless the specific performance characteristics of RWCopyOnWriteWString
 * are required, applications should use RWWString instead.  See the
 * <em>Essential Tools Module User's Guide</em> for additional information.
 *
 * @see RWWString for additional documentation.
 */
class RW_TOOLS_GLOBAL RWCopyOnWriteWString
{

public:
    typedef RWCopyOnWriteWSubString SubString;

    typedef RWCopyOnWriteWConstSubString ConstSubString;


    enum stripType {
        leading = 0x1,
        trailing = 0x2,
        both = 0x3
    };

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

#endif

    enum multiByte_ {
        multiByte
    };

    enum ascii_ {
        ascii
    };

    RWCopyOnWriteWString();

    RWCopyOnWriteWString(const std::wstring& a);

    RWCopyOnWriteWString(RWSize_T ic);

    RWCopyOnWriteWString(const RWCopyOnWriteWString& str);

#if !defined(RW_NO_RVALUE_REFERENCES)
    RWCopyOnWriteWString(RWCopyOnWriteWString && str);
#endif // !RW_NO_RVALUE_REFERENCES

    RWCopyOnWriteWString(const wchar_t* cs);

    RWCopyOnWriteWString(const wchar_t* cs, size_t N);

    RWCopyOnWriteWString(wchar_t cs);

    RWCopyOnWriteWString(char c) {
        initChar(c);
    }

    RWCopyOnWriteWString(unsigned char c) {
        initChar(char(c));   // widen a char
    }
    RWCopyOnWriteWString(signed char c) {
        initChar(char(c));   // widen a char
    }
    RWCopyOnWriteWString(wchar_t, size_t N);

    RWCopyOnWriteWString(char c, size_t N);

    RWCopyOnWriteWString(const SubString& ss);

    RWCopyOnWriteWString(const ConstSubString& ss);

    RWCopyOnWriteWString(const char* mbcs, multiByte_) {
        size_t bytes = RWCopyOnWriteCString::mbLength(mbcs);
        initMB(mbcs, (bytes != RW_NPOS) ? bytes : 0);
    }

    RWCopyOnWriteWString(const RWCopyOnWriteCString& mbcs, multiByte_);

    RWCopyOnWriteWString(const char* mbcs, size_t N, multiByte_) { // Convert N characters from MB
        initMB(mbcs, N);
    }

    // out of line since it uses RWCopyOnWriteWString
    RWCopyOnWriteWString(const char* acs, ascii_ asc);

    RWCopyOnWriteWString(const RWCopyOnWriteCString& acs, ascii_ asc);

    // Convert N characters from US-ASCII
    RWCopyOnWriteWString(const char* cs, size_t N, ascii_ asc);

    ~RWCopyOnWriteWString();

    // Type conversion:
    operator const wchar_t* () const {
        return data_;
    }

    RWCopyOnWriteWString& operator=(wchar_t c);

    RWCopyOnWriteWString&   operator=(const wchar_t* wcs);

    RWCopyOnWriteWString& operator=(const std::wstring& str);

    RWCopyOnWriteWString&   operator=(const RWCopyOnWriteWString& wcs);

#if !defined(RW_NO_RVALUE_REFERENCES)
    RWCopyOnWriteWString& operator=(RWCopyOnWriteWString && str);
#endif // !RW_NO_RVALUE_REFERENCES

    RWCopyOnWriteWString& operator=(const SubString& wcs);

    RWCopyOnWriteWString& operator=(const ConstSubString& wcs);

    RWCopyOnWriteWString&   operator+=(const wchar_t* wcs);

    RWCopyOnWriteWString& operator+=(const std::wstring& str) {
        return append(str);
    }

    RWCopyOnWriteWString&   operator+=(const RWCopyOnWriteWString& s);

    wchar_t&            operator[](short i);

    wchar_t&            operator[](int i);

    wchar_t&            operator[](long i);

    wchar_t&            operator[](unsigned short i);

    wchar_t&            operator[](unsigned int i);

    wchar_t&            operator[](unsigned long i);

#if !defined(RW_NO_LONG_LONG)
    wchar_t&            operator[](long long);

    wchar_t&            operator[](unsigned long long);
#endif

    wchar_t             operator[](int i) const;

    wchar_t             operator[](short i) const;

    wchar_t             operator[](long i) const;

    wchar_t             operator[](unsigned short i) const;

    wchar_t             operator[](unsigned int i) const;

    wchar_t             operator[](unsigned long i) const;
#if !defined(RW_NO_LONG_LONG)

    wchar_t             operator[](long long) const;

    wchar_t             operator[](unsigned long long) const;
#endif

    wchar_t&            operator()(size_t);
    wchar_t              operator()(size_t) const;

    SubString operator()(size_t start, size_t len);
    ConstSubString  operator()(size_t start, size_t len) const;


    SubString  subString(const wchar_t* cs, size_t start = 0, caseCompare cmp = exact);
    ConstSubString  subString(const wchar_t* cs, size_t start = 0, caseCompare cmp = exact) const;

    // Non-static member functions:
    RWCopyOnWriteWString& assign(const RWCopyOnWriteWString& wstr);

    RWCopyOnWriteWString& assign(const std::wstring& wstr);

    RWCopyOnWriteWString& assign(const SubString& wstr);

    RWCopyOnWriteWString& assign(const ConstSubString& wstr);

    RWCopyOnWriteWString& assign(const wchar_t* wstr);

    RWCopyOnWriteWString& assign(const wchar_t* wstr, size_t len);

    RWCopyOnWriteWString& assign(wchar_t wc, size_t count = 1);

    RWCopyOnWriteWString& append(const wchar_t* cs);

    RWCopyOnWriteWString& append(const wchar_t* cs, size_t N);

    RWCopyOnWriteWString& append(const std::wstring& str);

    RWCopyOnWriteWString& append(const RWCopyOnWriteWString& s);

    RWCopyOnWriteWString& append(const std::wstring& str, size_t N);

    RWCopyOnWriteWString& append(const RWCopyOnWriteWString& s, size_t N);

    RWCopyOnWriteWString& append(wchar_t c, size_t rep = 1);

    RWspace       binaryStoreSize() const {
        return length() * sizeof(wchar_t) + sizeof(size_t);
    }

    size_t        capacity() const {
        return pref()->capacity();
    }

    size_t        capacity(size_t N);

    int           collate(const wchar_t* cs) const {
        return pref()->collate(cs);
    }

    int           collate(const std::wstring& cs) const;

    int           collate(const RWCopyOnWriteWString& cs) const;

    // some inlines appear here to avoid inlined after called warnings
    int  compareTo(const wchar_t* st,   caseCompare cmp = exact) const {
        return compareToSpecial(st, wcslen(st), cmp);
    }

    int  compareTo(const std::wstring& st, caseCompare cmp = exact) const {
        return compareToSpecial(st.data(), st.length(), cmp);
    }

    int  compareTo(const std::wstring* st, caseCompare cmp = exact) const {
        return compareTo(*st, cmp);
    }

    int  compareTo(const RWCopyOnWriteWString& st, caseCompare cmp = exact) const {
        return compareToSpecial(st.data(), st.length(), cmp);
    }
    int  compareTo(const RWCopyOnWriteWString* st, caseCompare cmp = exact) const {
        return compareTo(*st, cmp);
    }

    bool     contains(const wchar_t* str,   caseCompare cmp = exact) const;

    bool contains(const std::wstring& cs, caseCompare cmp = exact) const;

    bool     contains(const RWCopyOnWriteWString& str, caseCompare cmp = exact) const;

#if !defined(RW_DISABLE_DEPRECATED)

    RW_DEPRECATE_FUNC("Use RWCopyOnWriteWString::operator=(const RWCopyOnWriteWString&) instead")
    RWCopyOnWriteWString     copy() const;

#endif

    const wchar_t* data() const {
        return data_;
    }

    size_t        first(wchar_t c) const {
        return pref()->first(c);
    }

#if !defined(RW_DISABLE_DEPRECATED)

    RW_DEPRECATE_FUNC("Use RWCopyOnWriteWString::first(wchar_t) instead")
    size_t        first(wchar_t c, size_t) const {
        return pref()->first(c);
    }

#endif

    size_t        first(const wchar_t* cs) const {
        return pref()->first(cs);
    }

    size_t        first(const wchar_t* cs, size_t N) const {
        return pref()->first(cs, N);
    }

    unsigned      hash(caseCompare cmp = exact) const;

    size_t        index(const wchar_t* pat, size_t i = 0, caseCompare cmp = exact) const;

    size_t index(const std::wstring& pat, size_t i = 0, caseCompare cmp = exact) const;

    size_t        index(const RWCopyOnWriteWString& s, size_t i = 0, caseCompare cmp = exact) const;

    size_t        index(const wchar_t* pat, size_t patlen, size_t i,
                        caseCompare cmp) const;

    size_t index(const std::wstring& pat, size_t patlen, size_t i, caseCompare cmp) const;

    size_t        index(const RWCopyOnWriteWString& pat, size_t patlen, size_t i,
                        caseCompare cmp) const;


    RWCopyOnWriteWString&    insert(size_t pos, const wchar_t* cs);

    RWCopyOnWriteWString&    insert(size_t pos, const wchar_t* cs, size_t N);

    RWCopyOnWriteWString& insert(size_t pos, const std::wstring& str);

    RWCopyOnWriteWString&    insert(size_t pos, const RWCopyOnWriteWString& str);

    RWCopyOnWriteWString& insert(size_t pos, const std::wstring& str, size_t N);

    RWCopyOnWriteWString&    insert(size_t pos, const RWCopyOnWriteWString& str, size_t N);

    bool     isAscii() const;

    bool     isNull() const {
        return pref()->nchars_ == 0;
    }

    size_t        last(wchar_t c) const {
        return pref()->last(c);
    }

#if !defined(RW_DISABLE_DEPRECATED)

    RW_DEPRECATE_FUNC("Use RWCopyOnWriteWString::last(wchar_t) instead")
    size_t        last(wchar_t c, size_t) const {
        return pref()->last(c);
    }

#endif

    size_t last(const wchar_t* s) const;

    size_t last(const wchar_t* s, size_t N) const;

    size_t        length() const {
        return pref()->nchars_;
    }

    RWCopyOnWriteWString&    prepend(const wchar_t* cs);

    RWCopyOnWriteWString&    prepend(const wchar_t* cs, size_t N);

    RWCopyOnWriteWString& prepend(const std::wstring& str);

    RWCopyOnWriteWString&    prepend(const RWCopyOnWriteWString& str);

    RWCopyOnWriteWString& prepend(const std::wstring& str, size_t N);

    RWCopyOnWriteWString&    prepend(const RWCopyOnWriteWString& cstr, size_t N);

    RWCopyOnWriteWString&    prepend(wchar_t c, size_t rep = 1);

    std::istream&      readFile(std::istream& s);

    std::istream&      readLine(std::istream& s,
                                bool skipWhite = true);

    std::istream&      readString(std::istream& s);

    std::istream&      readToDelim(std::istream& s,
                                   wchar_t delim = (wchar_t)'\n');

    std::istream&      readToDelim(std::istream& is,
                                   wchar_t delim,
                                   bool skipWhite);

    std::istream&      readToken(std::istream& s);

    std::wistream&     readFile(std::wistream& s);

    std::wistream&     readLine(std::wistream& s,
                                bool skipWhite = true);

    std::wistream&     readString(std::wistream& s);

    std::wistream&     readToDelim(std::wistream& s,
                                   wchar_t delim = (wchar_t)'\n');

    std::wistream&     readToDelim(std::wistream& s,
                                   wchar_t delim,                                          bool skipWhite);

    std::wistream&     readToken(std::wistream& s);

    RWCopyOnWriteWString&    remove(size_t pos);

    RWCopyOnWriteWString&    remove(size_t pos, size_t N);

    RWCopyOnWriteWString&    replace(size_t pos, size_t N, const wchar_t* cs);

    RWCopyOnWriteWString&    replace(size_t pos, size_t N1, const wchar_t* cs, size_t N2);

    RWCopyOnWriteWString&    replace(size_t pos, size_t N, const std::wstring& str);

    RWCopyOnWriteWString&    replace(size_t pos, size_t N, const RWCopyOnWriteWString& str);

    RWCopyOnWriteWString&    replace(size_t pos, size_t N1, const std::wstring&, size_t N2);

    RWCopyOnWriteWString&    replace(size_t pos, size_t N1, const RWCopyOnWriteWString&, size_t N2);


    void          resize(size_t n);

    void          restoreFrom(RWvistream&);      // Restore from US-ASCII store
    void          restoreFrom(RWFile&);          // Restore string
    void          saveOn(RWvostream& s) const;
    void          saveOn(RWFile& f) const;

    SubString  strip(stripType s = trailing, wchar_t c = (wchar_t)' ');
    ConstSubString  strip(stripType s = trailing, wchar_t c = (wchar_t)' ') const;

    RWCopyOnWriteCString     toAscii() const;

    RWCopyOnWriteCString     toMultiByte() const;

#if !defined(RW_DISABLE_DEPRECATED)

    RW_DEPRECATE_FUNC("Use RWCopyOnWriteWString::toMultiByte() instead")
    RWCopyOnWriteCString     toMultiByteReadNulls() const;

#endif

    static RWCopyOnWriteWString  fromAscii(const RWCopyOnWriteCString& str) {
        return RWCopyOnWriteWString(str, ascii);
    }

    static RWCopyOnWriteWString  fromMultiByte(const RWCopyOnWriteCString& str) {
        return RWCopyOnWriteWString(str, multiByte);
    }

    void swap(RWCopyOnWriteWString& str);

    void toLower();

    void toUpper();

    // Static member functions:

#if !defined(RW_DISABLE_DEPRECATED)

    RW_DEPRECATE_FUNC("")
    static size_t initialCapacity(size_t capacity = 15);

    RW_DEPRECATE_FUNC("")
    static size_t maxWaste(size_t maxwaste = 16);

    RW_DEPRECATE_FUNC("")
    static size_t resizeIncrement(size_t increment = 16);

    RW_DEPRECATE_FUNC("")
    static size_t getInitialCapacity();

    RW_DEPRECATE_FUNC("")
    static size_t getResizeIncrement();

    RW_DEPRECATE_FUNC("")
    static size_t getMaxWaste();

    RW_DEPRECATE_FUNC("")
    static size_t adjustCapacity(size_t nc);

#endif

    static unsigned       hash(const RWCopyOnWriteWString& wstr);

protected:

    // Special concatenation constructor:
    RWCopyOnWriteWString(const wchar_t* a1, size_t N1, const wchar_t* a2, size_t N2);
    void                  assertElement(size_t) const;    // Index in range
    void                  clobber(size_t nc);             // Remove old contents
    void                  cow();                          // Do copy on write as needed
    void                  cow(size_t nc);                 // Do copy on write as needed
    void                  initMB(const char*, size_t); // Initialize from multibyte

#if !defined(RW_DISABLE_DEPRECATED)

    RW_DEPRECATE_FUNC("")
    void                  initMBReadNulls(const char*, size_t); // Initialize from multibyte to include embedded Nulls

#endif

    void                  initChar(char);              // Init from char

    int                   compareToSpecial(const wchar_t* s, size_t slen, caseCompare cmp) const;
    size_t                indexSpecial(const wchar_t* pat, size_t patlen, size_t i, caseCompare cmp) const;

private:
    void                  clone();          // Make self a distinct copy
    void                  clone(size_t nc); // Make self a distinct copy w. capacity nc

    wchar_t*              multiByteToWide(const char* mbs, size_t len, size_t& wideLen) const;
    char*                 wideToMultiByte(const wchar_t* wcs, size_t len, size_t& rcLen) const;

    RWWStringRef* pref() const {
        union {
            wchar_t* wptr;
            RWWStringRef* refptr;
        } ptr;
        ptr.wptr = data_;

        return ptr.refptr - 1;
    }

    wchar_t*      data_;          // ref. counted data (RWWStringRef is in front)

    friend RWCopyOnWriteWString
    operator+(const RWCopyOnWriteWString& lhs, const RWCopyOnWriteWString& rhs);

    friend RWCopyOnWriteWString
    operator+(const RWCopyOnWriteWString& lhs,  const RWCopyOnWriteWSubString& rhs);

    friend RWCopyOnWriteWString
    operator+(const RWCopyOnWriteWSubString& lhs, const RWCopyOnWriteWString& rhs);

    friend RWCopyOnWriteWString
    operator+(const RWCopyOnWriteWString& lhs,  const RWCopyOnWriteWConstSubString& rhs);

    friend RWCopyOnWriteWString
    operator+(const RWCopyOnWriteWConstSubString& lhs, const RWCopyOnWriteWString& rhs);

    friend RWCopyOnWriteWString
    operator+(const RWCopyOnWriteWString& lhs,  const std::wstring& rhs);

    friend RWCopyOnWriteWString
    operator+(const std::wstring& lhs, const RWCopyOnWriteWString& rhs);

    friend RWCopyOnWriteWString
    operator+(const RWCopyOnWriteWString& lhs,  const wchar_t* rhs);

    friend RWCopyOnWriteWString
    operator+(const wchar_t* lhs, const RWCopyOnWriteWString& rhs);

    friend RWCopyOnWriteWString
    operator+(const RWCopyOnWriteWSubString& lhs, const RWCopyOnWriteWSubString& rhs);

    friend RWCopyOnWriteWString
    operator+(const RWCopyOnWriteWSubString& lhs,  const RWCopyOnWriteWConstSubString& rhs);

    friend RWCopyOnWriteWString
    operator+(const RWCopyOnWriteWConstSubString& lhs, const RWCopyOnWriteWSubString& rhs);

    friend RWCopyOnWriteWString
    operator+(const RWCopyOnWriteWSubString& lhs,  const std::wstring& rhs);

    friend RWCopyOnWriteWString
    operator+(const std::wstring& lhs, const RWCopyOnWriteWSubString& rhs);

    friend RWCopyOnWriteWString
    operator+(const RWCopyOnWriteWSubString& lhs,  const wchar_t* rhs);

    friend RWCopyOnWriteWString
    operator+(const wchar_t* lhs, const RWCopyOnWriteWSubString& rhs);

    friend RWCopyOnWriteWString
    operator+(const RWCopyOnWriteWConstSubString& lhs, const RWCopyOnWriteWConstSubString& rhs);

    friend RWCopyOnWriteWString
    operator+(const RWCopyOnWriteWConstSubString& lhs,  const std::wstring& rhs);

    friend RWCopyOnWriteWString
    operator+(const std::wstring& lhs, const RWCopyOnWriteWConstSubString& rhs);

    friend RWCopyOnWriteWString
    operator+(const RWCopyOnWriteWConstSubString& lhs,  const wchar_t* rhs);

    friend RWCopyOnWriteWString
    operator+(const wchar_t* lhs, const RWCopyOnWriteWConstSubString& rhs);

    RW_INLINE_FRIEND bool operator==(const RWCopyOnWriteWString& s1, const RWCopyOnWriteWString& s2);
    RW_INLINE_FRIEND bool operator==(const RWCopyOnWriteWString& s1, const std::wstring& s2);
    RW_INLINE_FRIEND bool operator==(const std::wstring& s1, const RWCopyOnWriteWString& s2);
    friend RW_TOOLS_SYMBOLIC bool operator==(const RWCopyOnWriteWString& s1, const wchar_t* s2);

    friend class RW_TOOLS_SYMBOLIC RWCopyOnWriteWSubString;
    friend class RW_TOOLS_SYMBOLIC RWCopyOnWriteWConstSubString;
    friend class RW_TOOLS_SYMBOLIC RWWStringRef;

};

// Related global functions:


RWCopyOnWriteWString
operator+(const RWCopyOnWriteWString& lhs, const RWCopyOnWriteWString& rhs);

RWCopyOnWriteWString
operator+(const RWCopyOnWriteWString& lhs,  const RWCopyOnWriteWSubString& rhs);

RWCopyOnWriteWString
operator+(const RWCopyOnWriteWSubString& lhs, const RWCopyOnWriteWString& rhs);

RWCopyOnWriteWString
operator+(const RWCopyOnWriteWString& lhs,  const RWCopyOnWriteWConstSubString& rhs);

RWCopyOnWriteWString
operator+(const RWCopyOnWriteWConstSubString& lhs, const RWCopyOnWriteWString& rhs);

RWCopyOnWriteWString
operator+(const RWCopyOnWriteWString& lhs,  const std::wstring& rhs);

RWCopyOnWriteWString
operator+(const std::wstring& lhs, const RWCopyOnWriteWString& rhs);

RWCopyOnWriteWString
operator+(const RWCopyOnWriteWString& lhs,  const wchar_t* rhs);

RWCopyOnWriteWString
operator+(const wchar_t* lhs, const RWCopyOnWriteWString& rhs);

RWCopyOnWriteWString
operator+(const RWCopyOnWriteWSubString& lhs, const RWCopyOnWriteWSubString& rhs);

RWCopyOnWriteWString
operator+(const RWCopyOnWriteWSubString& lhs,  const RWCopyOnWriteWConstSubString& rhs);

RWCopyOnWriteWString
operator+(const RWCopyOnWriteWConstSubString& lhs, const RWCopyOnWriteWSubString& rhs);

RWCopyOnWriteWString
operator+(const RWCopyOnWriteWSubString& lhs,  const std::wstring& rhs);

RWCopyOnWriteWString
operator+(const std::wstring& lhs, const RWCopyOnWriteWSubString& rhs);

RWCopyOnWriteWString
operator+(const RWCopyOnWriteWSubString& lhs,  const wchar_t* rhs);

RWCopyOnWriteWString
operator+(const wchar_t* lhs, const RWCopyOnWriteWSubString& rhs);

RWCopyOnWriteWString
operator+(const RWCopyOnWriteWConstSubString& lhs, const RWCopyOnWriteWConstSubString& rhs);

RWCopyOnWriteWString
operator+(const RWCopyOnWriteWConstSubString& lhs,  const std::wstring& rhs);

RWCopyOnWriteWString
operator+(const std::wstring& lhs, const RWCopyOnWriteWConstSubString& rhs);

RWCopyOnWriteWString
operator+(const RWCopyOnWriteWConstSubString& lhs,  const wchar_t* rhs);

RWCopyOnWriteWString
operator+(const wchar_t* lhs, const RWCopyOnWriteWConstSubString& rhs);

RW_TOOLS_SYMBOLIC std::istream&
operator>>(std::istream& s, RWCopyOnWriteWString& wcstr);

RW_TOOLS_SYMBOLIC std::ostream&
operator<<(std::ostream& s, const RWCopyOnWriteWString& str);

RW_TOOLS_SYMBOLIC std::ostream&
operator<<(std::ostream& s, const RWCopyOnWriteWSubString& str);

RW_TOOLS_SYMBOLIC std::ostream&
operator<<(std::ostream& s, const RWCopyOnWriteWConstSubString& str);

#if !defined(RW_NO_WIDE_STREAMS)
RW_TOOLS_SYMBOLIC std::wistream&
operator>>(std::wistream& str, RWCopyOnWriteWString& wcstr);

RW_TOOLS_SYMBOLIC std::wostream&
operator<<(std::wostream& str, const RWCopyOnWriteWString& wcstr);

RW_TOOLS_SYMBOLIC std::wostream&
operator<<(std::wostream& str, const RWCopyOnWriteWSubString& wcstr);

RW_TOOLS_SYMBOLIC std::wostream&
operator<<(std::wostream& str, const RWCopyOnWriteWConstSubString& wcstr);
#endif


// inlines

inline RWCopyOnWriteWString::RWCopyOnWriteWString()
{
    data_ = RWWStringRef::getEmptyRep()->data();
    RW_POSTCONDITION(length() == 0);
}

inline RWCopyOnWriteWString::RWCopyOnWriteWString(RWSize_T ic)
{
    data_ = RWWStringRef::getRep(ic.value(), 0, this)->data();
    RW_POSTCONDITION(length() == 0);
}

inline RWCopyOnWriteWString::RWCopyOnWriteWString(const wchar_t* cs)
{
    RW_PRECONDITION(cs != rwnil);
    size_t N = wcslen(cs);
    data_ = RWWStringRef::getRep(N, N, this)->data();
    rw_memcpy_s(data_, capacity() * sizeof(wchar_t), cs, N * sizeof(wchar_t));
}

inline RWCopyOnWriteWString::RWCopyOnWriteWString(const wchar_t* cs, size_t N)
{
    RW_PRECONDITION(cs != rwnil);
    data_ = RWWStringRef::getRep(N, N, this)->data();
    rw_memcpy_s(data_, capacity() * sizeof(wchar_t), cs, N * sizeof(wchar_t));
    RW_POSTCONDITION(length() == N);
}

inline RWCopyOnWriteWString::RWCopyOnWriteWString(const std::wstring& str)
{
    size_t N = str.length();
    data_ = RWWStringRef::getRep(N, N, this)->data();
    rw_memcpy_s(data_, capacity() * sizeof(wchar_t), str.data(), N * sizeof(wchar_t));
    RW_POSTCONDITION(length() == N);
}

inline RWCopyOnWriteWString::RWCopyOnWriteWString(wchar_t c)
{
    data_ = RWWStringRef::getRep(16, 1, this)->data();
    data_[0] = c;
    RW_POSTCONDITION(length() == 1);
}

inline RWCopyOnWriteWString::RWCopyOnWriteWString(char c, size_t N)
{
    data_ = RWWStringRef::getRep(N, N, this)->data();
    while (N--) {
        data_[N] = wchar_t(c);
    }
}

inline RWCopyOnWriteWString::RWCopyOnWriteWString(wchar_t c, size_t N)
{
    data_ = RWWStringRef::getRep(N, N, this)->data();
    while (N--) {
        data_[N] = c;
    }
}

// Widen from a (known) ASCII character string
inline RWCopyOnWriteWString::RWCopyOnWriteWString(const char* cs, ascii_)
{
    RW_PRECONDITION(cs != rwnil);
    size_t N = strlen(cs);
    data_ = RWWStringRef::getRep(N, N, this)->data();
    for (size_t i = 0; i < N; ++i) {
        data_[i] = (wchar_t)(unsigned char) cs[i];    // prevent sign-extend
    }
}

// Widen from a (known) ASCII character string of length N
inline RWCopyOnWriteWString::RWCopyOnWriteWString(const char* cs, size_t N, ascii_)
{
    RW_PRECONDITION(cs != rwnil);
    data_ = RWWStringRef::getRep(N, N, this)->data();
    for (size_t i = 0; i < N; ++i) {
        data_[i] = (wchar_t)(unsigned char) cs[i];    // prevent sign-extend
    }
    RW_POSTCONDITION(length() == N);
}

inline RWCopyOnWriteWString::RWCopyOnWriteWString(const RWCopyOnWriteWSubString& substr)
{
    const size_t len = substr.isNull() ? 0 : substr.length();
    data_ = RWWStringRef::getRep(len, len, this)->data();
    if (len) {
        rw_memcpy_s(data_, capacity() * sizeof(wchar_t), substr.startData(), len * sizeof(wchar_t));
    }
}

inline RWCopyOnWriteWString::RWCopyOnWriteWString(const RWCopyOnWriteWConstSubString& substr)
{
    const size_t len = substr.isNull() ? 0 : substr.length();
    data_ = RWWStringRef::getRep(len, len, this)->data();
    if (len) {
        rw_memcpy_s(data_, capacity() * sizeof(wchar_t), substr.startData(), len * sizeof(wchar_t));
    }
}

inline RWCopyOnWriteWString::RWCopyOnWriteWString(const RWCopyOnWriteWString& S)
{
    RWWStringRef* refp = S.pref();
    refp->link();
    data_ = S.data_;
}

#if !defined(RW_NO_RVALUE_REFERENCES)

inline RWCopyOnWriteWString::RWCopyOnWriteWString(RWCopyOnWriteWString && str)
    : data_(RWWStringRef::getEmptyRep()->data())
{
    str.swap(*this);
}

#endif // !RW_NO_RVALUE_REFERENCES

inline RWCopyOnWriteWString::~RWCopyOnWriteWString()
{
    pref()->unLink();
}

inline void RWCopyOnWriteWString::initChar(char c)
{
    data_ = RWWStringRef::getRep(16, 1, this)->data();
    data_[0] = (wchar_t)(unsigned char) c;
    RW_POSTCONDITION(length() == 1);
}

// RWFile serializations
inline RWFile&
operator>>(RWFile& file, RWCopyOnWriteWString*& wcstr)
{
    RWTScopedPointer<RWCopyOnWriteWString> newWcstr(new RWCopyOnWriteWString);
    newWcstr->restoreFrom(file);
    wcstr = newWcstr.release();
    return file;
}

inline RWFile&
operator<<(RWFile& file, const RWCopyOnWriteWString& wcstr)
{
    wcstr.saveOn(file);
    return file;
}

inline RWvostream&
operator<<(RWvostream& s, const RWCopyOnWriteWString& wcstr)
{
    wcstr.saveOn(s);
    return s;
}

// RWv[i/o]stream serialization
inline RWvistream&
operator>>(RWvistream& str, RWCopyOnWriteWString& wcstr)
{
    wcstr.restoreFrom(str);
    return str;
}

inline RWFile&
operator>>(RWFile& file, RWCopyOnWriteWString& wcstr)
{
    wcstr.restoreFrom(file);
    return file;
}

inline RWvistream&
operator>>(RWvistream& str, RWCopyOnWriteWString*& wcstr)
{
    RWTScopedPointer<RWCopyOnWriteWString> newWcstr(new RWCopyOnWriteWString);
    newWcstr->restoreFrom(str);
    wcstr = newWcstr.release();
    return str;
}

RW_TOOLS_SYMBOLIC RWCopyOnWriteWString
toLower(const RWCopyOnWriteWString& str);  // Return lower-case version of arg

RW_TOOLS_SYMBOLIC RWCopyOnWriteWString
toUpper(const RWCopyOnWriteWString& str);  // Return upper-case version of arg

#if !defined(RW_DISABLE_DEPRECATED)

inline    unsigned rwhash(const RWCopyOnWriteWString& s)
{
    return s.hash();
}
inline    unsigned rwhash(const RWCopyOnWriteWString* s)
{
    return s->hash();
}

#endif

RW_TOOLS_SYMBOLIC RWCopyOnWriteWString
strXForm(const RWCopyOnWriteWString&);


//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                               Inlines                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


// Assignment:
inline RWCopyOnWriteWString&
RWCopyOnWriteWString::assign(const RWCopyOnWriteWString& wstr)
{
    if (data() == wstr.data()) {
        return *this;
    }

    RWWStringRef* refp = wstr.pref();
    refp->link();
    pref()->unLink();
    data_ = wstr.data_;
    return *this;
}

inline RWCopyOnWriteWString&
RWCopyOnWriteWString::assign(const std::wstring& wstr)
{
    if (wstr.empty()) {
        if (pref()->unsafeReferences() == 1) {
            pref()->nchars_ = 0;
            *data_ = wchar_t(0);
        }
        else {
            pref()->unLink();
            data_ = RWWStringRef::getEmptyRep()->data();
        }
        return *this;
    }

    return replace(0, length(), wstr.data(), wstr.length());
}

inline RWCopyOnWriteWString&
RWCopyOnWriteWString::assign(const SubString& wstr)
{
    if (!wstr.length()) {
        if (pref()->unsafeReferences() == 1) {
            pref()->nchars_ = 0;
            *data_ = wchar_t(0);
        }
        else {
            pref()->unLink();
            data_ = RWWStringRef::getEmptyRep()->data();
        }
        return *this;
    }

    return replace(0, length(), wstr.startData(), wstr.length());
}

inline RWCopyOnWriteWString&
RWCopyOnWriteWString::assign(const ConstSubString& wstr)
{
    if (!wstr.length()) {
        if (pref()->unsafeReferences() == 1) {
            pref()->nchars_ = 0;
            *data_ = wchar_t(0);
        }
        else {
            pref()->unLink();
            data_ = RWWStringRef::getEmptyRep()->data();
        }
        return *this;
    }

    return replace(0, length(), wstr.startData(), wstr.length());
}

inline RWCopyOnWriteWString&
RWCopyOnWriteWString::assign(const wchar_t* wstr, size_t len)
{
    RW_PRECONDITION(wstr != rwnil);
    if (!len) {
        if (pref()->unsafeReferences() == 1) {
            pref()->nchars_ = 0;
            *data_ = wchar_t(0);
        }
        else {
            pref()->unLink();
            data_ = RWWStringRef::getEmptyRep()->data();
        }
        return *this;
    }

    return replace(0, length(), wstr, len);
}

inline RWCopyOnWriteWString&
RWCopyOnWriteWString::assign(const wchar_t* wstr)
{
    RW_PRECONDITION(wstr != rwnil);
    if (!*wstr) {
        if (pref()->unsafeReferences() == 1) {
            pref()->nchars_ = 0;
            *data_ = wchar_t(0);
        }
        else {
            pref()->unLink();
            data_ = RWWStringRef::getEmptyRep()->data();
        }
        return *this;
    }

    return replace(0, length(), wstr, wcslen(wstr));
}

inline RWCopyOnWriteWString&
RWCopyOnWriteWString::assign(wchar_t c, size_t count)
{
    if (!count) {
        if (pref()->unsafeReferences() == 1) {
            pref()->nchars_ = 0;
            *data_ = wchar_t(0);
        }
        else {
            pref()->unLink();
            data_ = RWWStringRef::getEmptyRep()->data();
        }
        return *this;
    }
    clobber(count);
    wmemset(data_, c, count);
    data_[pref()->nchars_ = count] = wchar_t(0);
    return *this;
}

inline RWCopyOnWriteWString&
RWCopyOnWriteWString::operator=(wchar_t wc)
{
    assign(wc);
    return *this;
}

inline RWCopyOnWriteWString&
RWCopyOnWriteWString::operator=(const wchar_t* wcs)
{
    assign(wcs);
    return *this;
}

inline RWCopyOnWriteWString&
RWCopyOnWriteWString::operator=(const std::wstring& wstr)
{
    assign(wstr);
    return *this;
}

RW_SUPPRESS_OP_ASSIGN_SELF_CHECK_WARNING
inline RWCopyOnWriteWString&
RWCopyOnWriteWString::operator=(const RWCopyOnWriteWString& wstr)
{
    assign(wstr);
    return *this;
}
RW_RESTORE_OP_ASSIGN_SELF_CHECK_WARNING

#if !defined(RW_NO_RVALUE_REFERENCES)

inline RWCopyOnWriteWString&
RWCopyOnWriteWString::operator=(RWCopyOnWriteWString && wstr)
{
    if (&wstr != this) {
        wstr.swap(*this);
    }

    return *this;
}

#endif // !RW_NO_RVALUE_REFERENCES


inline RWCopyOnWriteWString&
RWCopyOnWriteWString::operator=(const RWCopyOnWriteWString::SubString& substr)
{
    assign(substr);
    return *this;
}

inline RWCopyOnWriteWString&
RWCopyOnWriteWString::operator=(const RWCopyOnWriteWString::ConstSubString& substr)
{
    assign(substr);
    return *this;
}

inline void RWCopyOnWriteWString::cow()
{
    if (pref()->references() > 1) {
        clone();
    }
}

inline void RWCopyOnWriteWString::cow(size_t nc)
{
    if (pref()->references() > 1  || capacity() < nc) {
        clone(nc);
    }
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::append(const wchar_t* cs, size_t N)
{
    return replace(length(), 0, cs, N);
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::append(const std::wstring& s)
{
    return replace(length(), 0, s.data(), s.length());
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::append(const RWCopyOnWriteWString& s)
{
    return replace(length(), 0, s.data(), s.length());
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::append(const std::wstring& s, size_t N)
{
    return replace(length(), 0, s.data(), rwmin(N, s.length()));
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::append(const RWCopyOnWriteWString& s, size_t N)
{
    return replace(length(), 0, s.data(), rwmin(N, s.length()));
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::operator+=(const RWCopyOnWriteWString& s)
{
    return append(s.data(), s.length());
}

inline int RWCopyOnWriteWString::collate(const RWCopyOnWriteWString& st) const
{
    return pref()->collate(st.data());
}

inline bool RWCopyOnWriteWString::contains(const std::wstring& pat, caseCompare cmp) const
{
    return index(pat.data(), pat.length(), (size_t)0, cmp) != RW_NPOS;
}

inline bool RWCopyOnWriteWString::contains(const RWCopyOnWriteWString& pat, caseCompare cmp) const
{
    return index(pat.data(), pat.length(), (size_t)0, cmp) != RW_NPOS;
}

inline size_t RWCopyOnWriteWString::index(const std::wstring& s, size_t i, caseCompare cmp) const
{
    return index(s.data(), s.length(), i, cmp);
}

inline size_t RWCopyOnWriteWString::index(const RWCopyOnWriteWString& s, size_t i, caseCompare cmp) const
{
    return index(s.data(), s.length(), i, cmp);
}

inline size_t RWCopyOnWriteWString::index(const std::wstring& pat, size_t plen, size_t i, caseCompare cmp) const
{
    return index(pat.data(), plen, i, cmp);
}

inline size_t RWCopyOnWriteWString::index(const RWCopyOnWriteWString& pat, size_t plen, size_t i, caseCompare cmp) const
{
    return index(pat.data(), plen, i, cmp);
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::insert(size_t pos, const wchar_t* cs, size_t N)
{
    return replace(pos, 0, cs, N);
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::insert(size_t pos, const std::wstring& wcstr)
{
    return replace(pos, 0, wcstr.data(), wcstr.length());
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::insert(size_t pos, const RWCopyOnWriteWString& wcstr)
{
    return replace(pos, 0, wcstr.data(), wcstr.length());
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::insert(size_t pos, const std::wstring& wcstr, size_t N)
{
    return replace(pos, 0, wcstr.data(), rwmin(N, wcstr.length()));
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::insert(size_t pos, const RWCopyOnWriteWString& wcstr, size_t N)
{
    return replace(pos, 0, wcstr.data(), rwmin(N, wcstr.length()));
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::prepend(const wchar_t* cs, size_t N)
{
    return replace(0, 0, cs, N);
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::prepend(const std::wstring& s)
{
    return replace(0, 0, s.data(), s.length());
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::prepend(const RWCopyOnWriteWString& s)
{
    return replace(0, 0, s.data(), s.length());
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::prepend(const std::wstring& s, size_t N)
{
    return replace(0, 0, s.data(), rwmin(N, s.length()));
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::prepend(const RWCopyOnWriteWString& s, size_t N)
{
    return replace(0, 0, s.data(), rwmin(N, s.length()));
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::remove(size_t pos)
{
    return replace(pos, length() - pos, rwnil, 0);
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::remove(size_t pos, size_t n)
{
    return replace(pos, n, rwnil, 0);
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::replace(size_t pos, size_t n, const std::wstring& wcstr)
{
    return replace(pos, n, wcstr.data(), wcstr.length());
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::replace(size_t pos, size_t n, const RWCopyOnWriteWString& wcstr)
{
    return replace(pos, n, wcstr.data(), wcstr.length());
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::replace(size_t pos, size_t n1, const std::wstring& wcstr, size_t n2)
{
    return replace(pos, n1, wcstr.data(), rwmin(wcstr.length(), n2));
}

inline RWCopyOnWriteWString& RWCopyOnWriteWString::replace(size_t pos, size_t n1, const RWCopyOnWriteWString& wcstr, size_t n2)
{
    return replace(pos, n1, wcstr.data(), rwmin(wcstr.length(), n2));
}

inline void
RWCopyOnWriteWString::swap(RWCopyOnWriteWString& str)
{
    wchar_t* tmp = data_;
    data_ = str.data_;
    str.data_ = tmp;
}

inline wchar_t& RWCopyOnWriteWString::operator()(size_t i)
{
#ifdef RWBOUNDS_CHECK
    assertElement(i);
#endif
    cow();
    return data_[i];
}

inline wchar_t RWCopyOnWriteWString::operator[](short i) const
{
    assertElement(size_t (i));
    return data_[i];
}

inline wchar_t RWCopyOnWriteWString::operator[](int i) const
{
    assertElement(size_t (i));
    return data_[i];
}

inline wchar_t RWCopyOnWriteWString::operator[](long i) const
{
    assertElement(size_t (i));
    return data_[i];
}

inline wchar_t RWCopyOnWriteWString::operator[](unsigned short i) const
{
    assertElement(size_t (i));
    return data_[i];
}

inline wchar_t RWCopyOnWriteWString::operator[](unsigned int i) const
{
    assertElement(size_t (i));
    return data_[i];
}

inline wchar_t RWCopyOnWriteWString::operator[](unsigned long i) const
{
    assertElement(size_t (i));
    return data_[i];
}

#if !defined(RW_NO_LONG_LONG)
inline wchar_t RWCopyOnWriteWString::operator[](long long i) const
{
    assertElement(RW_STATIC_CAST(size_t, i));
    return data_[RW_STATIC_CAST(size_t, i)];
}

inline wchar_t RWCopyOnWriteWString::operator[](unsigned long long i) const
{
    assertElement(RW_STATIC_CAST(size_t, i));
    return data_[RW_STATIC_CAST(size_t, i)];
}
#endif

inline wchar_t RWCopyOnWriteWString::operator()(size_t i) const
{
#ifdef RWBOUNDS_CHECK
    assertElement(i);
#endif
    return data_[i];
}

///////////////////////////////////////////////////////////////////////////////
//
// RWCopyOnWriteWSubString::startData()
//
// This member replaces data().
// startData() will remain undocumented.  Please don't even ask.
// Use at your own risk. It may be deprecated in the future.
//
// Since RWCopyOnWriteWSubString works by referencing the RWCopyOnWriteWString's data,
// if you attempt to directly use the data() member of the RWCopyOnWriteWString,
// you will very likely be surprised by the result, which will be null
// terminated not at the extent of the substring,
// but at the end of the RWCopyOnWriteWString.
//
///////////////////////////////////////////////////////////////////////////////

inline const wchar_t* RWCopyOnWriteWSubString::startData() const
{
    return str_->data() + begin_;
}

// DON'T USE THE FUNCTION BELOW!
// This member is deprecated and will be removed in a future version.
// It remains public only to maintain source compatibility.
inline const wchar_t* RWCopyOnWriteWSubString::data() const
{
    return str_->data() + begin_;
}

//------------------------------------------------------------------------------

// Access to elements of sub-string with bounds checking
inline wchar_t RWCopyOnWriteWSubString::operator[](size_t i) const
{
    assertElement(i);
    return str_->data_[begin_ + i];
}

inline wchar_t RWCopyOnWriteWSubString::operator()(size_t i) const
{
#ifdef RWBOUNDS_CHECK
    assertElement(i);
#endif
    return str_->data_[begin_ + i];
}

inline RWCopyOnWriteWString
operator+(const RWCopyOnWriteWString& lhs, const RWCopyOnWriteWString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteWString(lhs.data(), lhsLen, rhs.data(), rhsLen);
}

inline RWCopyOnWriteWString
operator+(const RWCopyOnWriteWString& lhs, const RWCopyOnWriteWSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteWString(lhs.data(), lhsLen, rhs.startData(), rhsLen);
}

inline RWCopyOnWriteWString
operator+(const RWCopyOnWriteWSubString& lhs, const RWCopyOnWriteWString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteWString(lhs.startData(), lhsLen, rhs.data(), rhsLen);
}

inline RWCopyOnWriteWString
operator+(const RWCopyOnWriteWString& lhs, const RWCopyOnWriteWConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteWString(lhs.data(), lhsLen, rhs.startData(), rhsLen);
}

inline RWCopyOnWriteWString
operator+(const RWCopyOnWriteWConstSubString& lhs, const RWCopyOnWriteWString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteWString(lhs.startData(), lhsLen, rhs.data(), rhsLen);
}

inline RWCopyOnWriteWString
operator+(const RWCopyOnWriteWString& lhs,  const std::wstring& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWCopyOnWriteWString(rhs);
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteWString(lhs.data(), lhsLen, rhs.data(), rhsLen);
}

inline RWCopyOnWriteWString
operator+(const std::wstring& lhs, const RWCopyOnWriteWString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWCopyOnWriteWString(lhs);
    }
    return RWCopyOnWriteWString(lhs.data(), lhsLen, rhs.data(), rhsLen);
}

inline RWCopyOnWriteWString
operator+(const RWCopyOnWriteWString& lhs,  const wchar_t* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator+(const RWCopyOnWriteWString& lhs,  const wchar_t* rhs): null pointer");
    size_t rhsLen = wcslen(rhs);
    if (!rhsLen) {
        return lhs;
    }
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWCopyOnWriteWString(rhs, rhsLen);
    }
    return RWCopyOnWriteWString(lhs.data(), lhsLen, rhs, rhsLen);
}

inline RWCopyOnWriteWString
operator+(const wchar_t* lhs, const RWCopyOnWriteWString& rhs)
{
    RW_PRECONDITION2(lhs != rwnil,
                     "operator+(const wchar_t* lhs, const RWCopyOnWriteWString& rhs): null pointer");
    size_t lhsLen = wcslen(lhs);
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWCopyOnWriteWString(lhs, lhsLen);
    }
    return RWCopyOnWriteWString(lhs, lhsLen, rhs.data(), rhsLen);
}

inline RWCopyOnWriteWString
operator+(const RWCopyOnWriteWSubString& lhs, const RWCopyOnWriteWSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteWString(lhs.startData(), lhsLen, rhs.startData(), rhsLen);
}

inline RWCopyOnWriteWString
operator+(const RWCopyOnWriteWSubString& lhs, const RWCopyOnWriteWConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteWString(lhs.startData(), lhsLen, rhs.startData(), rhsLen);
}

inline RWCopyOnWriteWString
operator+(const RWCopyOnWriteWConstSubString& lhs, const RWCopyOnWriteWSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteWString(lhs.startData(), lhsLen, rhs.startData(), rhsLen);
}

inline RWCopyOnWriteWString
operator+(const RWCopyOnWriteWSubString& lhs,  const std::wstring& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWCopyOnWriteWString(rhs);
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteWString(lhs.startData(), lhsLen, rhs.data(), rhsLen);
}

inline RWCopyOnWriteWString
operator+(const std::wstring& lhs, const RWCopyOnWriteWSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWCopyOnWriteWString(lhs);
    }
    return RWCopyOnWriteWString(lhs.data(), lhsLen, rhs.startData(), rhsLen);
}

inline RWCopyOnWriteWString
operator+(const RWCopyOnWriteWSubString& lhs,  const wchar_t* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator+(const RWCopyOnWriteWSubString& lhs,  const wchar_t* rhs): null pointer");
    size_t rhsLen = wcslen(rhs);
    if (!rhsLen) {
        return lhs;
    }
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWCopyOnWriteWString(rhs, rhsLen);
    }
    return RWCopyOnWriteWString(lhs.startData(), lhsLen, rhs, rhsLen);
}

inline RWCopyOnWriteWString
operator+(const wchar_t* lhs, const RWCopyOnWriteWSubString& rhs)
{
    RW_PRECONDITION2(lhs != rwnil,
                     "operator+(const wchar_t* lhs, const RWCopyOnWriteWSubString& rhs): null pointer");
    size_t lhsLen = wcslen(lhs);
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWCopyOnWriteWString(lhs, lhsLen);
    }
    return RWCopyOnWriteWString(lhs, lhsLen, rhs.startData(), rhsLen);
}

inline RWCopyOnWriteWString
operator+(const RWCopyOnWriteWConstSubString& lhs, const RWCopyOnWriteWConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteWString(lhs.startData(), lhsLen, rhs.startData(), rhsLen);
}

inline RWCopyOnWriteWString
operator+(const RWCopyOnWriteWConstSubString& lhs,  const std::wstring& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWCopyOnWriteWString(rhs);
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return lhs;
    }
    return RWCopyOnWriteWString(lhs.startData(), lhsLen, rhs.data(), rhsLen);
}

inline RWCopyOnWriteWString
operator+(const std::wstring& lhs, const RWCopyOnWriteWConstSubString& rhs)
{
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWCopyOnWriteWString(lhs);
    }
    return RWCopyOnWriteWString(lhs.data(), lhsLen, rhs.startData(), rhsLen);
}

inline RWCopyOnWriteWString
operator+(const RWCopyOnWriteWConstSubString& lhs,  const wchar_t* rhs)
{
    RW_PRECONDITION2(rhs != rwnil,
                     "operator+(const RWCopyOnWriteWConstSubString& lhs,  const wchar_t* rhs): null pointer");
    size_t rhsLen = wcslen(rhs);
    if (!rhsLen) {
        return lhs;
    }
    size_t lhsLen = lhs.length();
    if (!lhsLen) {
        return RWCopyOnWriteWString(rhs, rhsLen);
    }
    return RWCopyOnWriteWString(lhs.startData(), lhsLen, rhs, rhsLen);
}

inline RWCopyOnWriteWString
operator+(const wchar_t* lhs, const RWCopyOnWriteWConstSubString& rhs)
{
    RW_PRECONDITION2(lhs != rwnil,
                     "operator+(const wchar_t* lhs, const RWCopyOnWriteWConstSubString& rhs): null pointer");
    size_t lhsLen = wcslen(lhs);
    if (!lhsLen) {
        return rhs;
    }
    size_t rhsLen = rhs.length();
    if (!rhsLen) {
        return RWCopyOnWriteWString(lhs, lhsLen);
    }
    return RWCopyOnWriteWString(lhs, lhsLen, rhs.startData(), rhsLen);
}

// String Logical operators:
inline bool        operator==(const RWCopyOnWriteWString& s1, const RWCopyOnWriteWString& s2)
{
    return ((s1.length() == s2.length()) &&
            !memcmp(s1.data(), s2.data(), s1.length() * sizeof(wchar_t)));
}

inline bool        operator==(const RWCopyOnWriteWString& s1, const std::wstring& s2)
{
    return ((s1.length() == s2.length()) &&
            !memcmp(s1.data(), s2.data(), s1.length() * sizeof(wchar_t)));
}

inline bool        operator==(const std::wstring& s1, const RWCopyOnWriteWString& s2)
{
    return ((s1.length() == s2.length()) &&
            !memcmp(s1.data(), s2.data(), s1.length() * sizeof(wchar_t)));
}
inline bool        operator==(const wchar_t* s1, const RWCopyOnWriteWString& s2)
{
    return (s2 == s1);
}

inline bool        operator!=(const RWCopyOnWriteWString& s1, const wchar_t* s2)
{
    return !(s1 == s2);
}

inline bool        operator!=(const wchar_t* s1, const RWCopyOnWriteWString& s2)
{
    return !(s2 == s1);
}

inline bool        operator!=(const RWCopyOnWriteWString& s1, const RWCopyOnWriteWString& s2)
{
    return !(s1 == s2);
}

inline bool        operator!=(const RWCopyOnWriteWString& s1, const std::wstring& s2)
{
    return !(s1 == s2);
}

inline bool        operator!=(const std::wstring& s1, const RWCopyOnWriteWString& s2)
{
    return !(s1 == s2);
}

inline bool        operator< (const RWCopyOnWriteWString& s1, const RWCopyOnWriteWString& s2)
{
    return s1.compareTo(s2) < 0;
}

inline bool        operator< (const RWCopyOnWriteWString& s1, const std::wstring& s2)
{
    return s1.compareTo(s2) < 0;
}

inline bool        operator< (const std::wstring& s1, const RWCopyOnWriteWString& s2)
{
    return s2.compareTo(s1) > 0;
}

inline bool        operator> (const RWCopyOnWriteWString& s1, const RWCopyOnWriteWString& s2)
{
    return s1.compareTo(s2) > 0;
}

inline bool        operator> (const RWCopyOnWriteWString& s1, const std::wstring& s2)
{
    return s1.compareTo(s2) > 0;
}

inline bool        operator> (const std::wstring& s1, const RWCopyOnWriteWString& s2)
{
    return s2.compareTo(s1) < 0;
}

inline bool        operator<=(const RWCopyOnWriteWString& s1, const RWCopyOnWriteWString& s2)
{
    return s1.compareTo(s2) <= 0;
}

inline bool        operator<=(const RWCopyOnWriteWString& s1, const std::wstring& s2)
{
    return s1.compareTo(s2) <= 0;
}

inline bool        operator<=(const std::wstring& s1, const RWCopyOnWriteWString& s2)
{
    return s2.compareTo(s1) >= 0;
}

inline bool        operator>=(const RWCopyOnWriteWString& s1, const RWCopyOnWriteWString& s2)
{
    return s1.compareTo(s2) >= 0;
}

inline bool        operator>=(const RWCopyOnWriteWString& s1, const std::wstring& s2)
{
    return s1.compareTo(s2) >= 0;
}

inline bool        operator>=(const std::wstring& s1, const RWCopyOnWriteWString& s2)
{
    return s2.compareTo(s1) <= 0;
}

inline bool        operator< (const RWCopyOnWriteWString& s1, const wchar_t* s2)
{
    return s1.compareTo(s2) < 0;
}

inline bool        operator> (const RWCopyOnWriteWString& s1, const wchar_t* s2)
{
    return s1.compareTo(s2) > 0;
}

inline bool        operator<=(const RWCopyOnWriteWString& s1, const wchar_t* s2)
{
    return s1.compareTo(s2) <= 0;
}

inline bool        operator>=(const RWCopyOnWriteWString& s1, const wchar_t* s2)
{
    return s1.compareTo(s2) >= 0;
}

inline bool        operator< (const wchar_t* s1, const RWCopyOnWriteWString& s2)
{
    return s2.compareTo(s1) > 0;
}

inline bool        operator> (const wchar_t* s1, const RWCopyOnWriteWString& s2)
{
    return s2.compareTo(s1) < 0;
}

inline bool        operator<=(const wchar_t* s1, const RWCopyOnWriteWString& s2)
{
    return s2.compareTo(s1) >= 0;
}

inline bool        operator>=(const wchar_t* s1, const RWCopyOnWriteWString& s2)
{
    return s2.compareTo(s1) <= 0;
}

// SubString Logical operators:
//     bool operator==(const RWCopyOnWriteWSubString& s1, const RWCopyOnWriteWSubString& s2);
//     bool operator==(const RWCopyOnWriteWSubString& s1, const wchar_t* s2);
//     bool operator==(const RWCopyOnWriteWSubString& s1, const RWCopyOnWriteWString& s2);
inline bool operator==(const RWCopyOnWriteWString& s1,    const RWCopyOnWriteWSubString& s2)
{
    return (s2 == s1);
}

inline bool operator==(const wchar_t* s1,      const RWCopyOnWriteWSubString& s2)
{
    return (s2 == s1);
}

inline bool operator!=(const RWCopyOnWriteWSubString& s1, const wchar_t* s2)
{
    return !(s1 == s2);
}

inline bool operator!=(const RWCopyOnWriteWSubString& s1, const RWCopyOnWriteWString& s2)
{
    return !(s1 == s2);
}

inline bool operator!=(const RWCopyOnWriteWSubString& s1, const RWCopyOnWriteWSubString& s2)
{
    return !(s1 == s2);
}

inline bool operator!=(const RWCopyOnWriteWSubString& s1, const RWCopyOnWriteWConstSubString& s2)
{
    return !(s1 == s2);
}

inline bool operator!=(const RWCopyOnWriteWString& s1,    const RWCopyOnWriteWSubString& s2)
{
    return !(s2 == s1);
}

inline bool operator!=(const wchar_t* s1,      const RWCopyOnWriteWSubString& s2)
{
    return !(s2 == s1);
}

//////////////////////////////////////////////////////////////////////////
//                         RWCopyOnWriteWConstSubString                            //
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// RWCopyOnWriteWConstSubString::startData()
//
// This member replaces data().
// startData() will remain undocumented.  Please don't even ask.
// Use at your own risk. It may be deprecated in the future.
//
// Since RWCopyOnWriteWSubString works by referencing the RWCopyOnWriteWString's data,
// if you attempt to directly use the data() member of the RWCopyOnWriteWString,
// you will very likely be surprised by the result, which will be null
// terminated not at the extent of the substring,
// but at the end of the RWCopyOnWriteWString.
//
///////////////////////////////////////////////////////////////////////////////

inline RWCopyOnWriteWConstSubString::RWCopyOnWriteWConstSubString
(const RWCopyOnWriteWString& str, size_t pos, size_t n)
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

inline const wchar_t*
RWCopyOnWriteWConstSubString::startData() const
{
    return str_->data() + begin_;
}

inline wchar_t
RWCopyOnWriteWConstSubString::operator()(size_t i) const
{
    return (*str_)(begin_ + i);
}

// Here all integer types are provided with an exact match
// to avoid ambiguities with the built in wchar_t[] operator
inline wchar_t
RWCopyOnWriteWConstSubString::operator[](short i) const
{
    return (*str_)[begin_ + i];
}

inline wchar_t
RWCopyOnWriteWConstSubString::operator[](int i) const
{
    return (*str_)[begin_ + i];
}

inline wchar_t
RWCopyOnWriteWConstSubString::operator[](long i) const
{
    return (*str_)[begin_ + i];
}

inline wchar_t
RWCopyOnWriteWConstSubString::operator[](unsigned short i) const
{
    return (*str_)[begin_ + i];
}

inline wchar_t
RWCopyOnWriteWConstSubString::operator[](unsigned int i) const
{
    return (*str_)[begin_ + i];
}

inline wchar_t
RWCopyOnWriteWConstSubString::operator[](unsigned long i) const
{
    return (*str_)[begin_ + i];
}

////////////////////// RWCopyOnWriteWConstSubString::operator==() ///////////////////////
RW_TOOLS_SYMBOLIC bool operator==(const RWCopyOnWriteWConstSubString& s1,
                                  const wchar_t* s2);

inline bool operator==(const wchar_t* s1,
                       const RWCopyOnWriteWConstSubString& s2)
{
    return (s2 == s1);
}

RW_TOOLS_SYMBOLIC bool operator==(const RWCopyOnWriteWConstSubString& s1,
                                  const RWCopyOnWriteWConstSubString& s2);

RW_TOOLS_SYMBOLIC bool operator==(const RWCopyOnWriteWConstSubString& s1,
                                  const RWCopyOnWriteWString& s2);

inline bool operator==(const RWCopyOnWriteWString& s1,
                       const RWCopyOnWriteWConstSubString& s2)
{
    return (s2 == s1);
}

inline bool operator==(const RWCopyOnWriteWConstSubString& s1,
                       const RWCopyOnWriteWSubString& s2)
{
    return (s2 == s1);
}

////////////////////// RWCopyOnWriteWConstSubString::operator!=() ///////////////////////
inline bool operator!=(const RWCopyOnWriteWConstSubString& s1,
                       const wchar_t* s2)
{
    return !(s1 == s2);
}

inline bool operator!=(const wchar_t* s1,
                       const RWCopyOnWriteWConstSubString& s2)
{
    return !(s2 == s1);
}

inline bool operator!=(const RWCopyOnWriteWConstSubString& s1,
                       const RWCopyOnWriteWConstSubString& s2)
{
    return !(s1 == s2);
}

inline bool operator!=(const RWCopyOnWriteWConstSubString& s1,
                       const RWCopyOnWriteWString& s2)
{
    return !(s1 == s2);
}

inline bool operator!=(const RWCopyOnWriteWString& s1,
                       const RWCopyOnWriteWConstSubString& s2)
{
    return !(s2 == s1);
}

inline bool operator!=(const RWCopyOnWriteWConstSubString& s1,
                       const RWCopyOnWriteWSubString& s2)
{
    return !(s1 == s2);
}

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an RWCopyOnWriteWString.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<RWCopyOnWriteWString> : public std::unary_function<RWCopyOnWriteWString, size_t> {
    /**
     * Constructs a hash object instance with the specified comparison
     * type.
     */
    RWTHash<RWCopyOnWriteWString>(RWCopyOnWriteWString::caseCompare cmp = RWCopyOnWriteWString::exact)
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
    size_t operator()(const RWCopyOnWriteWString& obj) const {
        return obj.hash(cmp_);
    }

private:
    RWCopyOnWriteWString::caseCompare cmp_;
};

#endif // #define RW_TOOLS_COWWSTRING_H
