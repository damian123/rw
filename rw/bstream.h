#ifndef RW_TOOLS_BSTREAM_H
#define RW_TOOLS_BSTREAM_H



/**********************************************************************
 *
 * rwb[io]stream ---    Use Binary reads and writes
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/bstream.h#1 $
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
 *              class RWbistream                *
 *                                              *
 ************************************************/

/**
 * @ingroup virtual_stream_classes
 *
 * @brief Restores variables stored in binary format by RWbostream.
 *
 * Class RWbistream specializes the abstract base class RWvistream
 * to restore variables stored in binary format by RWbostream.
 *
 * You can think of it as a binary veneer over an associated \b std::streambuf.
 * Because the RWbistream retains no information about the state
 * of its associated \b std::streambuf, its use can be freely exchanged
 * with other users of the \b std::streambuf (such as an \b std::istream or
 * \b std::ifstream).
 *
 * RWbistream can be interrogated as to the stream state using member
 * functions good(), bad(), eof(), and so on.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/bstream.h>
 * RWbistream bstr(std::cin);  // Construct an RWbistream using cin's streambuf
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
 * #include <rw/bstream.h>
 *
 * int main()
 * {
 *     // Open an input file
 *     std::ifstream fstr("data.dat");
 *
 *     // Construct RWbistream from it
 *     RWbistream bstr(fstr);
 *
 *     int i;
 *     float f;
 *     double d;
 *
 *     // Restore an int that was stored in binary
 *     if(!(bstr >> i))
 *     {
 *         std::cout << "Run bostream.exe first!\n";
 *     }
 *     else
 *     {
 *         // Restore a float & double
 *         bstr >> f >> d;
 *
 *         std::cout << "The integer: " << i << std::endl;
 *         std::cout << "The float: " << f << std::endl;
 *         std::cout << "The double: " << d << std::endl;
 *     }
 *
 *     return 0;
 * }
 *
 * @endcode
 *
 * Program output:
 *
 * @code
 * An integer, a float and a double sent out on an RWbostream!
 * @endcode
 */
class RW_TOOLS_GLOBAL RWbistream
    : public virtual std::ios, public RWvistream
{
public:

    /**
     * Constructs an RWbistream using the \b std::streambuf associated with
     * the \b std::istream \a str. For DOS, the \b std::streambuf must have
     * been opened in binary mode.
     */
    RWbistream(std::istream& str);

    /**
     * Constructs an RWbistream from the \b std::streambuf \a sb. For DOS,
     * this \b std::streambuf must have been opened in binary mode.
     */
    RWbistream(std::streambuf* sb);

    // Documented in base class.
    virtual int eof() {
        return std::ios::eof();
    }

    // Documented in base class.
    virtual int fail() {
        return std::ios::fail();
    }

    // Documented in base class.
    virtual int bad() {
        return std::ios::bad();
    }

    // Documented in base class.
    virtual int good() {
        return std::ios::good();
    }

    // Documented in base class.
    virtual int rdstate() {
        return std::ios::rdstate();
    }

    // Documented in base class.
    virtual void clear(int v = 0) {
        std::ios::clear(std::ios::iostate(v));
    }

    // Documented in base class.
    operator void* () {
        return RWvistream::operator void * ();
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

    // Documented in base class.
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
    virtual RWvistream& get(long double*, size_t n);
#endif

    // Documented in base class.
    virtual RWvistream& getString(char* s, size_t n);

    /**
     * Restores \a n \c char from the input stream into the array beginning at
     * \a s. The function stops reading after \a n \c char. The resulting
     * buffer is not null terminated.
     */
    virtual RWvistream& getChars(char* s, size_t n);

    // Documented in base class.
    virtual RWvistream& getSizeT(size_t& sz);

};

/************************************************
 *                                              *
 *              class RWbostream                *
 *                                              *
 ************************************************/

/**
 * @ingroup virtual_stream_classes
 *
 * @brief Stores variables in binary format.
 *
 * Class RWbostream specializes the abstract base class RWvostream
 * to store variables in binary format. The results can be restored
 * by using its counterpart RWbistream.
 *
 * You can think of it as a binary veneer over an associated \b std::streambuf.
 * Because the RWbostream retains no information about the state
 * of its associated \b std::streambuf, its use can be freely exchanged
 * with other users of the \b std::streambuf (such as \b std::ostream or
 * \b std::ofstream).
 *
 * @note
 * Variables should not be separated with white space. Such
 * white space would be interpreted literally and would have to be
 * read back in as a character string.
 *
 * RWbostream can be interrogated as to the stream state using member
 * functions good(), bad(), eof(), etc.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/bstream.h>
 * RWbostream bstr(std::cout);   // Construct an RWbostream using cout's streambuf
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
 * #include <rw/bstream.h>
 *
 * int main()
 * {
 *     // Open an output file
 *     std::ofstream fstr("data.dat");
 *
 *     // Construct a RWbostream from it
 *     RWbostream bstr(fstr);
 *
 *     int i = 5;
 *     float f = 22.1f;
 *     double d = -0.05;
 *
 *     std::cout << "An integer, a float and a double sent out on a RWbostream!\n";
 *
 *     // Store a float & double
 *     bstr << i << f << d;
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 * An integer, a float and a double sent out on an RWbostream!
 * @endcode
 */
class RW_TOOLS_GLOBAL RWbostream
    : public virtual std::ios, public RWvostream
{

public:

    /**
     * Constructs an RWbostream from the \b std::streambuf associated with
     * the \b std::ostream \a str. For DOS, the \b std::streambuf must have
     * been opened in binary mode.
     */
    RWbostream(std::ostream& str);

    /**
     * Constructs an RWbostream from the \b std::streambuf \a sb. For DOS,
     * the \b std::streambuf must have been opened in binary mode.
     */
    RWbostream(std::streambuf* sb);

    // Documented in base class.
    virtual int eof() {
        return std::ios::eof();
    }

    // Documented in base class.
    virtual int fail() {
        return std::ios::fail();
    }

    // Documented in base class.
    virtual int bad() {
        return std::ios::bad();
    }

    // Documented in base class.
    virtual int good() {
        return std::ios::good();
    }

    // Documented in base class.
    virtual int rdstate() {
        return std::ios::rdstate();
    }

    // Documented in base class.
    virtual void clear(int v = 0) {
        std::ios::clear(std::ios::iostate(v));
    }

    // Documented in base class.
    operator void* () {
        return RWvostream::operator void * ();
    }


    // Documented in base class.
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
    virtual RWvostream& put(bool);

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

};

#endif /* RW_TOOLS_BSTREAM_H */
