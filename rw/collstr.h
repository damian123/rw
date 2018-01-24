#ifndef RW_TOOLS_COLLSTR_H
#define RW_TOOLS_COLLSTR_H

/**********************************************************************
 *
 * RWCollectableString --- collectable strings
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/collstr.h#1 $
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
#include <rw/cstring.h>
#include <rw/collect.h>
#include <rw/wstring.h>
#include <rw/edefs.h> // for rw_move
#include <rw/tools/hash.h>

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Provides a polymorphically persistable RWCString
 *
 * Collectable strings. This class is useful when strings are stored
 * and retrieved as RWCollectable instances, or when they are used as keys
 * in the "dictionary" collection classes. Class RWCollectableString
 * inherits from both class RWCString and class RWCollectable.
 *
 * The virtual functions of the base class RWCollectable have
 * been redefined.
 *
 * @section synopsis Synopsis
 *
 * @code
 * typedef RWCollectableString String;  // Smalltalk typedef
 * #include <rw/collstr.h>
 * RWCollectableString  c;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWCollectableString
    : public RWCollectable, public RWCString
{
private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWCollectableString);
    friend RWCollectable* rwCreateFN(RWCollectableString)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns the unique ID for RWCollectableString.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    /**
     * Returns the #RWClassID of this class.
     */
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWCollectableString*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWCollectableString*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWCollectableString& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWCollectableString& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWCollectableString& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWCollectableString& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Creates a string of length zero (the null string).
     */
    RWCollectableString();

    /**
     * Copy constructor. The created string copies the data from \a str.
     */
    RWCollectableString(const RWCollectableString& str) : RWCollectable(str), RWCString(str) { }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The created collectable string takes ownership of the
     * data owned by \a str.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWCollectableString(RWCollectableString && str) : RWCollectable(str), RWCString(rw_move(str)) { }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Converts from the null-terminated character string \a c. The
     * created string \e copies the data pointed to by \a c,
     * up to the first terminating null. <i>This function is incompatible
     * with \a c strings with embedded nulls. This function
     * may be incompatible with \a c MBCS strings.</i>
     */
    RWCollectableString(const char* c) : RWCString(c) { }

    /**
     * Constructs an RWCollectableString from the character string \a a.
     * The created string \e copies the data pointed to by \a a. Exactly
     * \a N bytes are copied, <i>including any embedded nulls</i>. Hence,
     * the buffer pointed to by \a a must be at least \a N bytes long.
     */
    RWCollectableString(const char* a, size_t N) : RWCString(a, N) { }

    /**
     * Constructs an RWCollectableString containing the character \a c
     * repeated \a N times.
     */
    RWCollectableString(char c, size_t N) : RWCString(c, N) { }

    /**
     * Constructs an RWCollectableString from the RWCString \a s
     * by \e copying data from \a s.
     */
    RWCollectableString(const RWCString& s) : RWCString(s) { }

    /**
     * Converts from a sub-string. The created string \e copies
     * the substring represented by \a s.
     */
    RWCollectableString(const RWCSubString& s) : RWCString(s) { }

    /**
     * Empty destructor.
     */
    virtual ~RWCollectableString();

    /**
     * Assignment operator. The string copies the data from \a str.
     * Returns a reference to self.
     */
    RWCollectableString& operator=(const RWCollectableString& str) {
        RWCString::operator=(str);
        return *this;
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a str.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWCollectableString& operator=(RWCollectableString && str) {
        RWCString::operator=(rw_move(str));
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    // Documented in base class.
    virtual RWspace binaryStoreSize() const {
        return RWCString::binaryStoreSize();
    }

    /**
     * Returns the result of RWCString::compareTo(*(const
     * String*)c, RWCString::exact). This compares strings lexicographically,
     * with case considered. It would be possible to define, for instance,
     * \c CaseFoldedString which did comparisons ignoring case. We have
     * deliberately left this as an exercise for two reasons: Because
     * it is both easy to do and not universally needed; and because
     * the presence of both RWCollectableStrings and such a \c CaseFoldedString
     * in any kind of sorted collection has the potential for very confusing
     * behavior, since the result of a comparison would depend on the
     * order in which the comparison was done.
     */
    virtual int compareTo(const RWCollectable* c) const;


    /**
     * @copydoc compareTo(const RWCollectable*) const
     */
    virtual int compareTo(const RWCollectableString& c) const {
        return compareTo(&c);
    }

    /**
     * Calls RWCString::hash() and returns the results.
     */
    virtual unsigned hash() const;

    /**
     * Calls RWCString::operator==()
     * (i.e., the equivalence operator) with \a c as the argument and
     * returns the results.
     */
    virtual bool isEqual(const RWCollectable* c) const;

    // Documented in base class.
    virtual void restoreGuts(RWvistream&);

    // Documented in base class.
    virtual void restoreGuts(RWFile&);

    // Documented in base class.
    virtual void saveGuts(RWvostream&) const;

    // Documented in base class.
    virtual void saveGuts(RWFile&) const;

    int compareTo(const char*, caseCompare cmp = exact) const;
    int compareTo(const RWCString&, caseCompare cmp = exact) const;
    unsigned hash(caseCompare) const;
    static unsigned hash(const RWCString&);

};

typedef RWCollectableString RWCollectableCString;

/**
 * @ingroup rwcollectable_classes
 *
 * @brief Provides a polymorphically persistable RWWString
 *
 * Collectable wide strings paralleling RWCollectableString. This
 * class is useful when wide strings are stored and retrieved as
 * RWCollectables, or when they are used as keys in the "dictionary"
 * collection classes. Class RWCollectableWString inherits from
 * both class RWWString and class RWCollectable.
 *
 * The virtual functions of the base class RWCollectable have
 * been redefined.
 *
 * @note
 * Encodings used by operating systems for \c wchar_t  strings
 * may vary. If your application is dependent on the presence of
 * encodings across different operating systems, you  need to
 * ensure that it supports the desired formats.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/collstr.h>
 * RWCollectableWString  c;
 * @endcode
 *
 * @section persistence Persistence
 *
 * Polymorphic
 */
class RW_TOOLS_GLOBAL RWCollectableWString
    : public RWCollectable, public RWWString
{
private:
    static RWClassID myAtom;

private:
    friend struct RWInit(RWCollectableWString);
    friend RWCollectable* rwCreateFN(RWCollectableWString)();

public:

    // Documented in base class.
    virtual RWCollectable* newSpecies() const;

    virtual RWCollectable* copy() const;

    /**
     * Returns the unique ID for RWCollectableWString.
     */
    virtual RWClassID isA() const;

    virtual void* self();
    virtual const void* self() const;

    /**
     * Returns the #RWClassID of this class.
     */
    static RWClassID classIsA();

    friend RW_TOOLS_SYMBOLIC RWvistream&
    operator>>(RWvistream& s, RWCollectableWString*& pCl);

    friend RW_TOOLS_SYMBOLIC RWFile&
    operator>>(RWFile& f, RWCollectableWString*& pCl);

    RW_INLINE_FRIEND
    RWvostream& operator<<(RWvostream& v, const RWCollectableWString& obj) {
        obj.recursiveSaveOn(v);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator<<(RWFile& f, const RWCollectableWString& obj) {
        obj.recursiveSaveOn(f);
        return f;
    }

    RW_INLINE_FRIEND
    RWvistream& operator>>(RWvistream& v, RWCollectableWString& obj) {
        RWCollectable::recursiveRestoreFrom(v, &obj);
        return v;
    }

    RW_INLINE_FRIEND
    RWFile& operator>>(RWFile& f, RWCollectableWString& obj) {
        RWCollectable::recursiveRestoreFrom(f, &obj);
        return f;
    }

public:

    /**
     * Creates a string of length zero (the null string).
     */
    RWCollectableWString();

    /**
     * Copy constructor. The created string copies the data from \a str.
     */
    RWCollectableWString(const RWCollectableWString& str) : RWCollectable(str), RWWString(str) { }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The created collectable string takes ownership of the
     * data owned by \a str.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWCollectableWString(RWCollectableWString && str) : RWCollectable(str), RWWString(rw_move(str)) { }
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Creates a string from the wide character string \a a.  The created
     * string copies the data pointed to by \a a, up to the first
     * terminating null.
     */
    RWCollectableWString(const wchar_t* a) : RWWString(a) { }

    /**
     * Constructs a string from the character string \a a.  The created
     * string copies the data pointed to by \a a.  Exactly \a N
     * characters are copied, including any embedded nulls.  Hence,
     * the buffer pointed to by \a a must be at least \a N
     * <tt>* sizeof(wchar_t)</tt> bytes or \a N wide characters long.
     */
    RWCollectableWString(const wchar_t* a, size_t N) : RWWString(a, N) { }

    /**
     * Constructs a string containing the character \a c repeated \a N times.
     */
    RWCollectableWString(wchar_t c, size_t N) : RWWString(c, N) { }

    /**
     * Constructs an RWCollectableWString from the RWWString \a s
     * by copying data from \a s.
     */
    RWCollectableWString(const RWWString& s) : RWWString(s) { }

    /**
     * Converts from sub-string.  The created string copies the substring
     * represented by \a s.
     */
    RWCollectableWString(const RWWSubString& s) : RWWString(s) { }

    /**
     * Empty destructor.
     */
    virtual ~RWCollectableWString();

    /**
     * Assignment operator. The string copies the data from \a str.
     * Returns a reference to self.
     */
    RWCollectableWString& operator=(const RWCollectableWString& str) {
        RWWString::operator=(str);
        return *this;
    }

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a str.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWCollectableWString& operator=(RWCollectableWString && str) {
        RWWString::operator=(rw_move(str));
        return *this;
    }
#endif // !RW_NO_RVALUE_REFERENCES

    // Documented in base class.
    virtual RWspace binaryStoreSize() const {
        return RWWString::binaryStoreSize();
    }

    /**
     * Redefined from class RWCollectable. Returns the result of
     * RWWString::compareTo(*(constString*)c, RWWString::exact).
     * This compares strings lexicographically, with case considered. It
     * would be possible to define, for instance, \c CaseFoldedString
     * which did comparisons ignoring case. We have deliberately left
     * this as an exercise for two reasons: Because it is both easy to do
     * and not universally needed; and because the presence of both
     * RWCollectableWStrings and \c CaseFoldedString
     * in any kind of sorted collection has the potential for very confusing
     * behavior, since the result of a comparison would depend on the
     * order in which the comparison was done.
     */
    virtual int compareTo(const RWCollectable* c) const;

    /**
     * @copydoc compareTo(const RWCollectable*) const
     */
    virtual int compareTo(const RWCollectableWString& c) const {
        return compareTo(&c);
    }

    /**
     * Calls RWWString::hash() and returns the results.
     */
    virtual unsigned hash() const;

    /**
     * Calls RWWString::operator==()
     * (i.e., the equivalence operator) with \a c as the argument and
     * returns the results.
     */
    virtual bool isEqual(const RWCollectable* c) const;

    // Documented in base class.
    virtual void restoreGuts(RWvistream&);

    // Documented in base class.
    virtual void restoreGuts(RWFile&);

    // Documented in base class.
    virtual void saveGuts(RWvostream&) const;

    // Documented in base class.
    virtual void saveGuts(RWFile&) const;

    int compareTo(const wchar_t*, caseCompare cmp = exact) const;
    int compareTo(const RWWString&, caseCompare cmp = exact) const;
    unsigned hash(caseCompare) const;
    static unsigned hash(const RWWString&);

};

/*
 * The following functions help compilers disambiguate in the face of
 * multiple derivation, casting operators, etc.
 */

RW_TOOLS_SYMBOLIC std::ostream&
operator<<(std::ostream& os, const RWCollectableString& cts);

RW_TOOLS_SYMBOLIC std::istream&
operator>>(std::istream& is, RWCollectableString& cts);

RW_TOOLS_SYMBOLIC std::ostream&
operator<<(std::ostream& os, const RWCollectableWString& cts);

RW_TOOLS_SYMBOLIC std::istream&
operator>>(std::istream& is, RWCollectableWString& cts);

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an RWCollectableString.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<RWCollectableString> : public std::unary_function<RWCollectableString, size_t> {
    /**
     * Returns a hash of \a obj. This function is the equivalent of calling
     *
     * @code
     * obj.hash();
     * @endcode
     */
    size_t operator()(const RWCollectableString& obj) const {
        return obj.hash();
    }
};

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an RWCollectableWString.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<RWCollectableWString> : public std::unary_function<RWCollectableWString, size_t> {
    /**
     * Returns a hash of \a obj. This function is the equivalent of calling
     *
     * @code
     * obj.hash();
     * @endcode
     */
    size_t operator()(const RWCollectableWString& obj) const {
        return obj.hash();
    }
};



#endif /* RW_TOOLS_COLLSTR_H */
