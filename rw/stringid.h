#ifndef RW_TOOLS_STRINGID_H
#define RW_TOOLS_STRINGID_H



/**********************************************************************
 *
 * Implements a richer set of IDs than RWClassID
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/stringid.h#1 $
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
#include <rw/edefs.h> // for rw_move
#include <rw/rwfile.h>
#include <rw/cstring.h>

class RW_TOOLS_GLOBAL RWvostream;
class RW_TOOLS_GLOBAL RWvistream;

#include <rw/tools/hash.h>
#include <rw/tools/ristream.h>
#include <rw/tools/rostream.h>

#ifdef RW_DECLARE_GLOBAL_FRIENDS_FIRST
class RW_TOOLS_GLOBAL RWStringID;

inline
bool operator==(const RWStringID& lhs, const RWStringID& rhs);
#endif

/**
 * @ingroup rwcollectable_classes
 * @brief An identifier for RWCollectable instances.
 *
 * Class RWStringID is an identifier for RWCollectable instances. It is derived
 * from RWCString, and may be manipulated by any of the const RWCString methods.
 * The non-const methods have been hidden to prevent the disaster that could
 * occur if the RWStringID of a class changed at run time.
 *
 * You can associate an RWStringID with an RWCollectable class in one of two
 * ways: pick the RWStringID for the class, or allow the library to
 * automatically generate an RWStringID that is the same sequence of characters
 * as the name of the class; for example, class MyColl : public RWCollectable
 * would get the automatic RWStringID "MyColl".
 *
 * @section example Example
 *
 * You specify a class with a fixed #RWClassID and generated RWStringID by using
 * the macro RW_DEFINE_COLLECTABLE_CLASS_BY_ID as follows:
 *
 * @code
 * RW_DEFINE_COLLECTABLE_CLASS_BY_ID(USER_MODULE,ClassName, ClassID)
 * RW_DEFINE_COLLECTABLE_CLASS_BY_ID(USER_MODULE,MyCollectable1,0x1000)  // for example
 * @endcode
 *
 * You specify a class with a fixed RWStringID and a generated #RWClassID by
 * using the new macro RW_DEFINE_COLLECTABLE_CLASS_BY_NAME as follows:
 *
 * @code
 * RW_DEFINE_COLLECTABLE_CLASS_BY_NAME(USER_MODULE, ClassName, RWStringID)
 * RW_DEFINE_COLLECTABLE_CLASS_BY_NAME(USER_MODULE, MyCollectable2, "Second Collectable") // for example
 * @endcode
 *
 * @section example Example
 *
 * @code
 * #include <rw/collect.h>
 *
 * // See the section in the Essential Tools Module User's Guide
 * // about exporting symbols for more information about this macro.
 * #define EXPORT
 *
 * class MyCollectable1 : public RWCollectable {
 *   RW_DECLARE_COLLECTABLE_CLASS(EXPORT, MyCollectable1);
 * };
 *
 * RW_DEFINE_COLLECTABLE_CLASS_BY_ID(EXPORT,MyCollectable1,0x1000)
 *
 * class MyCollectable2 : public RWCollectable {
 *   RW_DECLARE_COLLECTABLE_CLASS(EXPORT, MyCollectable2);
 * };
 *
 * RW_DEFINE_COLLECTABLE_CLASS_BY_NAME(EXPORT, MyCollectable2, "Second Collectable")
 *
 * int main ()
 * {
 *    // First set up the experiment
 *    MyCollectable1 one;
 *    MyCollectable2 two;
 *
 *    // All running RWClassIDs are guaranteed distinct
 *    one.isA() != two.isA();
 *
 *    // Every RWCollectable has an RWStringID
 *    one.stringID() == "MyCollectable1";
 *
 *    // There are several ways to find ids
 *    two.isA();
 *    RWCollectable::classID("Second Collectable");
 *
 *    return 0;
 * }
 *
 * @endcode
 */
class RW_TOOLS_GLOBAL RWStringID : protected RWCString
{
public:
    /**
     * Default constructor.  Sets the value of the string ID to "NoID".
     */
    RWStringID()
        : RWCString("NoID", 4) {}

    /**
    * Copy constructor.  Sets the value of the string ID to the value
    * of \a sid.
    */
    RWStringID(const RWStringID& sid)
        : RWCString((const RWCString&)sid) {}

    /**
     * Sets the value of the string ID to the value of string \a s.
     */
    RWStringID(const RWCString& s)
        : RWCString(s) {}

    /**
     * Sets the value of the string ID to the value of char \a name.
     */
    RWStringID(const char* name)
        : RWCString(name) {}

#if !defined(RW_NO_RVALUE_REFERENCES)

    /**
     * Move Constructor. The created string ID takes ownership of the
     * data owned by \a s.
     *
     * @conditional
     * This method is available only on platforms with rvalue reference support.
     */
    RWStringID(RWCString && s)
        : RWCString(rw_move(s)) { }

    /**
     * Move constructor. The created string ID takes ownership of the
     * data owned by \a sid.
     *
     * @conditional
     * This method is available only on platforms with rvalue reference support.
     */
    RWStringID(RWStringID && sid)
        : RWCString(rw_move(sid)) {}

#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Destructor
     */
    ~RWStringID() {}

    /**
     * Assignment operator. Copies the data from string ID \a sid.
     * Returns a reference to self.
     */
    RWStringID& operator=(const RWStringID& sid) {
        return (RWStringID&)RWCString::operator=(sid);
    }

    /**
     * Copies the data from string \a s. Returns a reference to self.
     */
    RWStringID& operator=(const RWCString& s) {
        return (RWStringID&)RWCString::operator=(s);
    }

#if !defined(RW_NO_RVALUE_REFERENCES)

    /**
     * Move assignment. Self takes ownership of the data
     * owned by \a s.
     *
     * @conditional
     * This method is available only on platforms with rvalue reference support.
     */
    RWStringID& operator=(RWCString && s) {
        s.swap(*this);
        return *this;
    }

    /**
     * Move assignment. Self takes ownership of the data
     * owned by \a sid.
     *
     * @conditional
     * This method is available only on platforms with rvalue reference support.
     */
    RWStringID& operator=(RWStringID && sid) {
        if (&sid != this) {
            sid.swap(*this);
        }
        return *this;
    }

#endif // !RW_NO_RVALUE_REFERENCES

    // Documented in base class.
    operator const char* () const {
        return RWCString::data();
    }

    /**
    * Returns the \a t<sup>th</sup> byte in self. The index \a t must
    * be between 0 and the length of the string less one. Bounds
    * checking is performed.
    *
    * @throw RWBoundsErr if the index is out of range.
    */
    char  operator[](size_t t) const {
        return RWCString::operator[](t);
    }

    // Documented in base class.
    char  operator()(size_t t) const {
        return RWCString::operator()(t);
    }

    // Documented in base class.
    RWspace       binaryStoreSize() const {
        return RWCString::binaryStoreSize();
    }

#if !defined(RW_DISABLE_DEPRECATED)
    RW_SUPPRESS_DEPRECATED_WARNINGS

    // Declared internal in base class.
    RWCString     copy() const {
        return RWCString::copy();
    }

    RW_RESTORE_DEPRECATED_WARNINGS
#endif // RW_DISABLE_DEPRECATED

    // Documented in base class.
    const char*   data() const {
        return RWCString::data();
    }

    // Documented in base class.
    bool     isAscii() const {
        return RWCString::isAscii();
    }

    // Documented in base class.
    bool     isNull() const {
        return RWCString::isNull();
    }

    // Documented in base class.
    size_t        length() const {
        return RWCString::length();
    }

    /*
     * cfront does not implement access adjustment
     * of overloaded member functions:
     */
    // Documented in base class.
    size_t        capacity() const {
        return RWCString::capacity();
    }

    // Documented in base class.
    size_t        capacity(size_t N) {
        return RWCString::capacity(N);
    }

#ifndef RW_NO_LOCALE
    // Documented in base class.
    int           collate(const char* cs) const {
        return RWCString::collate(cs);
    }

    // Documented in base class.
    int           collate(const RWCString& st) const {
        return RWCString::collate(st);
    }
#endif

    // Documented in base class.
    int           compareTo(const char* cs,      caseCompare cmp = exact) const {
        return RWCString::compareTo(cs, cmp);
    }

    // Documented in base class.
    int           compareTo(const RWCString& st, caseCompare cmp = exact) const {
        return RWCString::compareTo(st, cmp);
    }

    // Documented in base class.
    bool     contains(const char* pat,      caseCompare cmp = exact) const {
        return RWCString::contains(pat, cmp);
    }

    // Documented in base class.
    bool     contains(const RWCString& pat, caseCompare cmp = exact) const {
        return RWCString::contains(pat, cmp);
    }

    // Documented in base class.
    size_t        first(char c) const {
        return RWCString::firstOf(c);
    }

    // Documented in base class.
    size_t        first(const char* cs) const {
        return RWCString::firstOf(cs);
    }

    // Documented in base class.
    size_t firstOf(char c, size_t pos = 0) const {
        return RWCString::firstOf(c, pos);
    }

    // Documented in base class.
    size_t firstOf(const char* str, size_t pos = 0) const {
        return RWCString::firstOf(str, pos);
    }

    /**
     * @copydoc RWCString::hash(caseCompare) const
     */
    unsigned      hash() const {
        return unsigned(RWCString::hash());
    }

    // Documented in base class.
    size_t index(const char* pat, size_t i = 0, caseCompare cmp = exact) const {
        return RWCString::index(pat, i, cmp);
    }

    // Documented in base class.
    size_t index(const RWCString& s, size_t i = 0, caseCompare cmp = exact) const {
        return RWCString::index(s, i, cmp);
    }

    // Documented in base class.
    size_t index(const char* pat, size_t patlen, size_t i, caseCompare cmp) const {
        return RWCString::index(pat, patlen, i, cmp);
    }

    // Documented in base class.
    size_t index(const RWCString& s, size_t patlen, size_t i, caseCompare cmp) const {
        return RWCString::index(s, patlen, i, cmp);
    }

#if !defined(RW_DISABLE_DEPRECATED)
    RW_SUPPRESS_DEPRECATED_WARNINGS

    // Documented in base class.
    RW_DEPRECATE_FUNC("")
    size_t        index(const RWCRExpr& pat, size_t i = 0) const {
        return RWCString::index(pat, i);
    }

    // Documented in base class.
    RW_DEPRECATE_FUNC("")
    size_t        index(const RWCRExpr& pat, size_t* ext, size_t i = 0) const {
        return RWCString::index(pat, ext, i);
    }

    // Documented in base class.
    RW_DEPRECATE_FUNC("")
    size_t        index(const char* pat, size_t* ext, size_t i = 0) const {
        return RWCString::index(pat, ext, i);
    }

    // Documented in base class.
    RW_DEPRECATE_FUNC("")
    size_t        index(const RWCRegexp& pat, size_t i = 0) const {
        return RWCString::index(pat, i);
    }

    // Documented in base class.
    RW_DEPRECATE_FUNC("")
    size_t        index(const RWCRegexp& pat, size_t* ext, size_t i = 0) const {
        return RWCString::index(pat, ext, i);
    }

    RW_RESTORE_DEPRECATED_WARNINGS
#endif // RW_DISABLE_DEPRECATED

    // Documented in base class.
    size_t        last(char c) const {
        return RWCString::lastOf(c);
    }

    // Documented in base class.
    size_t        last(char c, size_t) const {
        return RWCString::lastOf(c);
    }

    // Documented in base class.
    size_t lastOf(char c, size_t pos = RW_NPOS) const {
        return RWCString::lastOf(c, pos);
    }

    // Declared internal in base class.
    void          restoreFrom(RWvistream& s) {
        RWCString::restoreFrom(s);
    }

    // Declared internal in base class.
    void          restoreFrom(RWFile& f) {
        RWCString::restoreFrom(f);
    }

    // Declared internal in base class.
    void          saveOn(RWvostream& s) const {
        RWCString::saveOn(s);
    }

    // Declared internal in base class.
    void          saveOn(RWFile& f) const {
        RWCString::saveOn(f);
    }

    /**
     * Swaps the contents of \a rhs with self.
     */
    void          swap(RWStringID& rhs) {
        RWCString::swap(rhs);
    }

#ifdef RW_DECLARE_GLOBAL_FRIENDS_FIRST
    /**
     * @relates RWStringID
     *
     * Logical equality and inequality. Case sensitivity is <i>exact</i>.
     * This function is incompatible with \c const \c char* strings
     * with embedded nulls. This function may be incompatible with \c
     * const \c char* MBCS strings.
     */
    friend bool operator==(const RWStringID& lhs, const RWStringID& rhs);
#else
    /**
     * @relates RWStringID
     *
     * Logical equality and inequality. Case sensitivity is <i>exact</i>.
     * This function is incompatible with \c const \c char* strings
     * with embedded nulls. This function may be incompatible with \c
     * const \c char* MBCS strings.
     */
    friend  bool operator==(const RWStringID& lhs, const RWStringID& rhs) {
        return (const RWCString&)lhs == (const RWCString&)rhs;
    }
#endif

    /**
     * @relates RWStringID
     *
     * @copydoc operator==(const RWStringID&, const RWStringID&)
     */
    friend  bool operator==(const RWStringID& lhs, const char* rhs) {
        return (const RWCString&)lhs == rhs;
    }

    /**
     * @relates RWStringID
     *
     * @copydoc operator==(const RWStringID&, const RWStringID&)
     */
    friend  bool operator==(const char* lhs, const RWStringID& rhs) {
        return lhs == (const RWCString&)rhs;
    }

    /**
     * @relates RWStringID
     *
     * Logical equality and inequality. Case sensitivity is <i>exact</i>.
     * This function is incompatible with \c const \c char* strings
     * with embedded nulls. This function may be incompatible with \c
     * const \c char* MBCS strings.
     */
    friend  bool operator!=(const RWStringID& lhs, const char* rhs) {
        return (const RWCString&)lhs != rhs;
    }

    /**
     * @relates RWStringID
     *
     * @copydoc operator!=(const RWStringID&, const char*)
     */
    friend  bool operator!=(const char* lhs, const RWStringID&  rhs) {
        return lhs != (const RWCString&)rhs;
    }

    /**
     * @relates RWStringID
     *
     * @copydoc operator!=(const RWStringID&, const char*)
     */
    friend  bool operator<(const RWStringID& lhs, const RWStringID& rhs) {
        return (const RWCString&)lhs < (const RWCString&)rhs;
    }

#if !defined(RW_NO_RELOPS_NAMESPACE)
    /**
     * @relates RWStringID
     *
     * @copydoc operator!=(const RWStringID&, const char*)
     */
    friend  bool operator!=(const RWStringID& lhs, const RWStringID& rhs) {
        return (const RWCString&)lhs != (const RWCString&)rhs;
    }

    /**
     * @relates RWStringID
     *
     * Comparisons are done lexicographically, byte by byte. Case sensitivity
     * is <i>exact</i>. Use member RWStringID::collate()
     * for locale sensitivity. <i>This function is incompatible with</i> \c const
     * \c char*  <i>strings with embedded nulls. This function may be
     * incompatible with</i> \c const \c char* <i>MBCS strings.</i>
     */
    friend  bool operator>(const RWStringID& lhs, const RWStringID& rhs) {
        return (const RWCString&)lhs > (const RWCString&)rhs;
    }

    /**
     * @relates RWStringID
     *
     * @copydoc operator>(const RWStringID&, const RWStringID&)
     */
    friend  bool operator<=(const RWStringID& lhs, const RWStringID& rhs) {
        return (const RWCString&)lhs <= (const RWCString&)rhs;
    }

    /**
     * @relates RWStringID
     *
     * @copydoc operator>(const RWStringID&, const RWStringID&)
     */
    friend  bool operator>=(const RWStringID& lhs, const RWStringID& rhs) {
        return (const RWCString&)lhs >= (const RWCString&)rhs;
    }
#endif

    /**
     * @relates RWStringID
     *
     * Saves string \a sid to a virtual stream.
     */
    friend RWvostream& operator<<(RWvostream& vos, const RWStringID& sid) {
        sid.saveOn(vos);
        return vos;
    }

    /**
     * @relates RWStringID
     *
     * Restores a string into \a sid from a virtual stream,
     * replacing the previous contents of \a sid. If the
     * virtual stream produces an error on extraction, the operator
     * returns the original string contents unmodified.   Be sure to
     * check the virtual stream to determine if an extraction
     * error occurred.
     */
    friend RWvistream& operator>>(RWvistream& vis, RWStringID& sid) {
        sid.restoreFrom(vis);
        return vis;
    }

    /**
     * @relates RWStringID
     *
     * Saves string \a sid to an RWFile.
     */
    friend RWFile& operator<<(RWFile& file, const RWStringID& sid) {
        sid.saveOn(file);
        return file;
    }

    /**
     * @relates RWStringID
     *
     * Restores a string into \a sid from an RWFile,
     * replacing the previous contents of \a sid. If the
     * file produce an error on extraction, the operator
     * returns the original string contents unmodified.   Be sure to
     * check the file to determine if an extraction
     * error occurred.
     */
    friend RWFile& operator>>(RWFile& file, RWStringID& sid) {
        sid.restoreFrom(file);
        return file;
    }

    /**
     * @relates RWStringID
     *
     * Output an RWStringID on ostream \a os.
     */
    friend RW_TOOLS_SYMBOLIC std::ostream& operator<<(std::ostream& os,
            const RWStringID& sid) {
        return operator<<(os, (const RWCString&)sid);
    }

    /**
     * @relates RWStringID
     *
     * Calls RWCString::readToken(std::istream&). That is, a token is read from
     * the input stream \a is. <i>This function is incompatible with MBCS
     * strings.</i>
     */
    friend RW_TOOLS_SYMBOLIC std::istream& operator>>(std::istream& is,
            RWStringID& sid) {
        return operator>>(is, (RWCString&)sid);
    }

    // Documented in base class.
    static unsigned         hash(const RWStringID& sid) {
        return sid.hash();
    }

};

#ifdef RW_DECLARE_GLOBAL_FRIENDS_FIRST
inline
bool operator==(const RWStringID& lhs, const RWStringID& rhs)
{
    return (const RWCString&)lhs == (const RWCString&)rhs;
}
#endif

/**
 * @ingroup stl_extension_based_collection_classes
 * @brief Function object for hashing an RWStringID.
 *
 * Provides a C++ Standard Library compliant hash function object suitable for
 * use with hash or unordered containers.
 */
template <>
struct RWTHash<RWStringID> : public std::unary_function<RWStringID, size_t> {
    /**
     * Returns a hash of \a obj. This function is the equivalent of calling
     *
     * @code
     * obj.hash();
     * @endcode
     */
    size_t operator()(const RWStringID& obj) const {
        return obj.hash();
    }
};

#endif /*RW_TOOLS_STRINGID_H*/
