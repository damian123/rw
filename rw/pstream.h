#ifndef RW_TOOLS_PSTREAM_H
#define RW_TOOLS_PSTREAM_H



/**********************************************************************
 *
 * rwp[io]stream ---    Portable I/O streams (use escape sequences for
 *                      writing special characters.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/pstream.h#1 $
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
#include <rw/vstream.h>

/************************************************
 *                                              *
 *              class RWpistream                *
 *                                              *
 ************************************************/

/**
 * @ingroup virtual_stream_classes
 *
 * @brief Specializes the abstract base class RWvistream
 * to restore variables stored in a portable US-ASCII format by RWpostream
 *
 * Class RWpistream specializes the abstract base class RWvistream
 * to restore variables stored in a portable US-ASCII format by RWpostream.
 *
 * You can think of RWpistream and RWpostream as an US-ASCII veneer over
 * an associated \b std::streambuf which are responsible for formatting
 * variables and escaping characters such that the results can be
 * interchanged between any machines. As such, they are slower than their
 * binary counterparts RWbistream and RWbostream which are more machine
 * dependent. Because RWpistream and RWpostream retain no information
 * about the state of their associated \b std::streambuf, their use can
 * be freely exchanged with other users of the \b std::streambuf (such as
 * \b std::istream or \b std::ifstream).
 *
 * RWpistream can be interrogated as to the stream state using member
 * functions good(), bad(), eof(), and so forth.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/pstream.h>
 * RWpistream pstr(cin);   // Construct a RWpistream using cin's streambuf
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <fstream>
 *
 * #include <rw/pstream.h>
 *
 * int main()
 * {
 *     // Open an input file
 *     std::ifstream fstr("data.dat");
 *
 *     // Construct RWpistream from it
 *     RWpistream pstr(fstr);
 *
 *     int i;
 *     float f;
 *     double d;
 *
 *     // Restore an int that was stored in binary
 *     if(!(pstr >> i))
 *     {
 *         std::cout << "Run postream.exe first!\n";
 *     }
 *     else
 *     {
 *         // Restore a float & double
 *         pstr >> f >> d;
 *
 *         std::cout << "The integer: " << i << std::endl;
 *         std::cout << "The float: " << f << std::endl;
 *         std::cout << "The double: " << d << std::endl;
 *     }
 *
 *     return 0;
 * }
 * @endcode
 */
class RW_TOOLS_GLOBAL RWpistream : public RWvistream
{

public:

    /**
     * Initializes an RWpistream using the \b std::streambuf associated with
     * the \b std::istream \a str.
     */
    RWpistream(std::istream& str);

    /**
     * Initializes an RWpistream from the \b std::streambuf \a sb.
     */
    RWpistream(std::streambuf* sb);

    /**
     * Empty destructor.
     */
    virtual ~RWpistream();

    // Documented in base class.
    virtual int eof() {
        return istr_.eof();
    }

    // Documented in base class.
    virtual int fail() {
        return istr_.fail();
    }

    // Documented in base class.
    virtual int bad() {
        return istr_.bad();
    }

    // Documented in base class.
    virtual int good() {
        return istr_.good();
    }

    // Documented in base class.
    virtual int rdstate() {
        return istr_.rdstate();
    }

    // Documented in base class.
    virtual void clear(int v = 0) {
        istr_.clear(std::ios::iostate(v));
    }

    std::streambuf* rdbuf() {
        return istr_.rdbuf();
    }

    /**
     * Returns a reference to the underlying C++ Standard Library data
     * type. The reference can be used to access the functionality exposed
     * via \b std::istream interface.
     */
    std::istream& std();

    /**
     * @copydoc std()
     */
    const std::istream& std() const {
        return istr_;
    }

    // Documented in base class.
    virtual int get();

    // Documented in base class.
    virtual RWvistream& get(char& c);

    // Documented in base class.
    virtual RWvistream& getChar(char& c);

    // Documented in base class.
    virtual RWvistream& get(signed char& c);

    // Documented in base class.
    virtual RWvistream& getChar(signed char& c);

    // Documented in base class.
    virtual RWvistream& get(unsigned char& c);

    // Documented in base class.
    virtual RWvistream& getChar(unsigned char& c);

    // Documented in base class.
    virtual RWvistream& get(wchar_t& wc);

    // Documented in base class.
    virtual RWvistream& getChar(wchar_t& wc);

    // Documented in base class.
    virtual RWvistream& get(bool& b);

    // Documented in base class.
    virtual RWvistream& get(short& i);

    // Documented in base class.
    virtual RWvistream& get(unsigned short& i);

    // Documented in base class.
    virtual RWvistream& get(int& i);

    // Documented in base class.
    virtual RWvistream& get(unsigned int& i);

    // Documented in base class.
    virtual RWvistream& get(long& i);

    // Documented in base class.
    virtual RWvistream& get(unsigned long& i);

#if !defined(RW_NO_LONG_LONG)
    // Documented in base class.
    virtual RWvistream& get(long long& i);

    // Documented in base class.
    virtual RWvistream& get(unsigned long long& i);
#endif

    // Documented in base class.
    virtual RWvistream& get(float& f);

    // Documented in base class.
    virtual RWvistream& get(double& d);

#if !defined(RW_NO_LONG_DOUBLE)
    // Documented in base class.
    virtual RWvistream& get(long double& d);
#endif

    /**
     * Gets a vector of \c char and stores it in the array beginning
     * at \a v. If the restore operation stops prematurely because there
     * is no more data available on the stream, because an exception
     * is thrown, or for some other reason, get() stores what has already
     * been retrieved from the stream into \a v, and sets the failbit.
     *
     * @note
     * This method preserves US-ASCII numerical codes, not their
     * corresponding character values. If you wish to restore a
     * character string, use the function getString().
     */
    virtual RWvistream& get(char* v, size_t n);

    // Documented in base class.
    virtual RWvistream& get(signed char* v, size_t n);

    // Documented in base class.
    virtual RWvistream& get(unsigned char* v, size_t n);

    // Documented in base class.
    virtual RWvistream& get(wchar_t* v, size_t n);

    // Documented in base class.
    virtual RWvistream& get(bool*, size_t n);

    // Documented in base class.
    virtual RWvistream& get(short* v, size_t n);

    // Documented in base class.
    virtual RWvistream& get(unsigned short* v, size_t n);

    // Documented in base class.
    virtual RWvistream& get(int* v, size_t n);

    // Documented in base class.
    virtual RWvistream& get(unsigned int* v, size_t n);

    // Documented in base class.
    virtual RWvistream& get(long* v, size_t n);

    // Documented in base class.
    virtual RWvistream& get(unsigned long* v, size_t n);

#if !defined(RW_NO_LONG_LONG)
    // Documented in base class.
    virtual RWvistream& get(long long* v, size_t n);

    // Documented in base class.
    virtual RWvistream& get(unsigned long long* v, size_t n);
#endif

    // Documented in base class.
    virtual RWvistream& get(float* v, size_t n);

    // Documented in base class.
    virtual RWvistream& get(double* v, size_t n);

#if !defined(RW_NO_LONG_DOUBLE)
    // Documented in base class.
    virtual RWvistream& get(long double* v, size_t n);
#endif

    /**
     * Restores a character string from the input stream that was stored
     * to the output stream with RWpostream::putString and stores it in
     * the array beginning at \a s. The function stops reading at the end
     * of the string or after \a n <tt>- 1</tt> characters, whichever comes
     * first. If \a n <tt>- 1</tt> characters have been read and the \a
     * n <sup>th</sup> character is not the string terminator, then the
     * failbit of the stream is set. In either case, the string is
     * terminated with a null byte.
     */
    virtual RWvistream& getString(char* s, size_t n);

    /**
     * Restores \a n \c char from the input stream into the array beginning
     * at \a s. The function stops reading after \a n characters. The
     * resulting buffer is not null terminated.
     */
    virtual RWvistream& getChars(char* s, size_t n);

    // Documented in base class.
    virtual RWvistream& getSizeT(size_t& sz);

protected:
    void syntaxErr(const char* expect, char was);
    char getCChar();

    std::istream istr_;
private:
    // Disallow copying:
    RWpistream(const RWpistream&);
    RWpistream& operator=(const RWpistream&);
};

/************************************************
 *                                              *
 *              class RWpostream                *
 *                                              *
 ************************************************/

/**
 * @ingroup virtual_stream_classes
 *
 * @brief Specializes the abstract base class RWvostream
 * to store variables in a portable (printable) US-ASCII format
 *
 * Class RWpostream specializes the abstract base class RWvostream
 * to store variables in a portable (printable) US-ASCII format. The
 * results can be restored by using its counterpart RWpistream.
 *
 * You can think of RWpistream and RWpostream as a US-ASCII veneer over
 * an associated \b std::streambuf. They are responsible for formatting
 * variables and escaping characters such that the results can be
 * interchanged between any machines. As such, they are slower than their
 * binary counterparts RWbistream and RWbostream, which are more machine
 * dependent. Because RWpistream and RWpostream retain no information
 * about the state of their associated \b std::streambuf, their use can
 * be freely exchanged with other users of the \b std::streambuf (such as
 * \b std::istream or \b std::ifstream).
 *
 * The goal of class RWpostream and RWpistream is to store variables
 * using nothing but printable US-ASCII characters. Hence, nonprintable
 * characters must be converted into an external representation where
 * they can be recognized. Furthermore, other characters may be merely
 * bit values (a bit image, for example), having nothing to do with
 * characters as symbols. For example,
 *
 * @code
 * RWpostream pstrm(cout);
 * char c = '\n';
 *
 * pstr << c;     // Stores "newline"
 * pstr.put(c);     // Stores the number 10.
 * @endcode
 *
 * The expression <tt>pstr << c</tt> treats \c c as a symbol for
 * a newline, an unprintable character. The expression <tt>pstr.put(c)</tt>
 * treats \c c as the literal number "10".
 *
 * @note
 * Variables should not be separated with white space. Such
 * white space is interpreted literally and would be
 * read back in as a character string.
 *
 * RWpostream can be interrogated as to the stream state using member
 * functions good(), bad(), eof(), precision(), and so forth.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/pstream.h>
 * RWpostream pstr(cout);   // Construct an RWpostream using cout's streambuf
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <fstream>
 *
 * #include <rw/pstream.h>
 *
 * int main()
 * {
 *     // Open an output file
 *     std::ofstream fstr("data.dat");
 *
 *     // Construct a RWpostream from it
 *     RWpostream pstr(fstr);
 *
 *     int i = 5;
 *     float f = 22.1f;
 *     double d = -0.05;
 *
 *     std::cout << "An integer, a float and a double sent out on a RWpostream!\n";
 *
 *     // Store a float & double
 *     pstr << i << f << d;
 *
 *     return 0;
 * }
 * @endcode
 */
class RW_TOOLS_GLOBAL RWpostream : public RWvostream
{

public:

    /**
     * Initializes an RWpostream from the \b std::streambuf associated
     * with the \b std::ostream \a str.
     */
    RWpostream(std::ostream& str);

    /**
     * Initializes an RWpostream from the \b std::streambuf \a sb.
     */
    RWpostream(std::streambuf* sb);

    /**
     * Empty Destructor.
     */
    virtual ~RWpostream();

    // Documented in base class.
    virtual int eof() {
        return ostr_.eof();
    }

    // Documented in base class.
    virtual int fail() {
        return ostr_.fail();
    }

    // Documented in base class.
    virtual int bad() {
        return ostr_.bad();
    }

    // Documented in base class.
    virtual int good() {
        return ostr_.good();
    }

    // Documented in base class.
    virtual int rdstate() {
        return ostr_.rdstate();
    }

    // Documented in base class.
    virtual void clear(int v = 0) {
        ostr_.clear(std::ios::iostate(v));
    }

    // Documented in base class.
    operator void* () {
        return RWvostream::operator void * ();
    }

    /**
     * Returns a reference to the underlying C++ Standard Library data
     * type. The reference can be used to access the functionality exposed
     * via \b std::ostream interface.
     */
    std::ostream& std();

    /**
     * @copydoc std()
     */
    const std::ostream& std() const {
        return ostr_;
    }

    std::streambuf* rdbuf() {
        return ostr_.rdbuf();
    }

    /**
     * Changes the precision used for writing \c float and \c double
     * data.  Returns the previously set precision. At construction,
     * the precision is set to #RW_DEFAULT_PRECISION.
     */
    int precision(int p) {
        return int(ostr_.precision(p));
    }

    /**
     * Returns the currently set precision used for writing \c float
     * and \c double data. At construction, the precision is set to
     * #RW_DEFAULT_PRECISION.
     */
    int precision() const {
        return int(ostr_.precision());
    }


    /**
     * Stores the \c char \a c to the output stream, preserving its value
     * using a portable format. This member only preserves US-ASCII numerical
     * codes, not the corresponding character symbol.
     */
    virtual RWvostream& put(char c);

    // Documented in base class.
    virtual RWvostream& putChar(char c);

    // Documented in base class.
    virtual RWvostream& put(signed char c);

    // Documented in base class.
    virtual RWvostream& putChar(signed char c);

    // Documented in base class.
    virtual RWvostream& put(unsigned char c);

    // Documented in base class.
    virtual RWvostream& putChar(unsigned char c);

    // Documented in base class.
    virtual RWvostream& put(wchar_t wc);

    // Documented in base class.
    virtual RWvostream& putChar(wchar_t wc);

    // Documented in base class.
    virtual RWvostream& put(bool b);

    // Documented in base class.
    virtual RWvostream& put(short i);

    // Documented in base class.
    virtual RWvostream& put(unsigned short i);

    // Documented in base class.
    virtual RWvostream& put(int i);

    // Documented in base class.
    virtual RWvostream& put(unsigned int i);

    // Documented in base class.
    virtual RWvostream& put(long i);

    // Documented in base class.
    virtual RWvostream& put(unsigned long i);

#if !defined(RW_NO_LONG_LONG)
    // Documented in base class.
    virtual RWvostream& put(long long i);

    // Documented in base class.
    virtual RWvostream& put(unsigned long long i);
#endif

    // Documented in base class.
    virtual RWvostream& put(float f);

    // Documented in base class.
    virtual RWvostream& put(double d);

#if !defined(RW_NO_LONG_DOUBLE)
    // Documented in base class.
    virtual RWvostream& put(long double d);
#endif

    // Documented in base class.
    virtual RWvostream& put(const char* v, size_t n);

    // Documented in base class.
    virtual RWvostream& put(const signed char* v, size_t n);

    // Documented in base class.
    virtual RWvostream& put(const unsigned char* v, size_t n);

    // Documented in base class.
    virtual RWvostream& put(const wchar_t* v, size_t n);

    // Documented in base class.
    virtual RWvostream& put(const bool* v, size_t n);

    // Documented in base class.
    virtual RWvostream& put(const short* v, size_t n);

    // Documented in base class.
    virtual RWvostream& put(const unsigned short* v, size_t n);

    // Documented in base class.
    virtual RWvostream& put(const int* v, size_t n);

    // Documented in base class.
    virtual RWvostream& put(const unsigned int* v, size_t n);

    // Documented in base class.
    virtual RWvostream& put(const long* v, size_t n);

    // Documented in base class.
    virtual RWvostream& put(const unsigned long* v, size_t n);

#if !defined(RW_NO_LONG_LONG)
    // Documented in base class.
    virtual RWvostream& put(const long long* v, size_t n);

    // Documented in base class.
    virtual RWvostream& put(const unsigned long long* v, size_t n);
#endif

    // Documented in base class.
    virtual RWvostream& put(const float* v, size_t n);

    // Documented in base class.
    virtual RWvostream& put(const double* v, size_t n);

#if !defined(RW_NO_LONG_DOUBLE)
    // Documented in base class.
    virtual RWvostream& put(const long double* v, size_t n);
#endif

    // Documented in base class.
    virtual RWvostream& putString(const char* s, size_t n);

    /**
     * Stores \a n \c char starting at \a s to the output stream.
     */
    virtual RWvostream& putChars(const char* s, size_t n);

    // Documented in base class.
    virtual RWvostream& putSizeT(size_t sz);

    // Documented in base class.
    virtual RWvostream& flush();

#if defined(_MSC_VER)
    virtual RWvostream& vflush() {
        return this->flush();
    }
#endif

protected:

    static const unsigned MAXCOL;
    void putwrap(char);
    void putwrap(const char* s, size_t len);
    void putCChars(const char* s, size_t len);

    std::ostream ostr_;
    size_t column_;

private:

    // Disallow copying:
    RWpostream(const RWpostream&);
    RWpostream& operator=(const RWpostream&);

};

inline void
RWpostream::putwrap(char c)
{
    if (++column_ > MAXCOL) {
        column_ = 1;
        ostr_ << '\n';
    }

    ostr_ << c;
}

inline void
RWpostream::putwrap(const char* s, size_t len)
{
    column_ += len;
    if (column_ > MAXCOL) {
        ostr_ << '\n';
        column_ = len;
    }

    ostr_ << s;
}

#endif /* RW_TOOLS_PSTREAM_H */
