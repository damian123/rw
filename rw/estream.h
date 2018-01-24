#ifndef RW_TOOLS_ESTREAM_H
#define RW_TOOLS_ESTREAM_H



/**********************************************************************
 *
 * rwe[io]stream ---    Use Binary reads and writes while allowing for
 *                      differences in big endian vs. little endian, as
 *                      well as differences in the sizes of integral types.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/estream.h#1 $
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
#include <rw/bstream.h>
#include <rw/rwerr.h>


/************************************************
 *                                              *
 *              class RWeostream                *
 *                                              *
 ************************************************/

/**
 * @ingroup virtual_stream_classes
 *
 * @brief Specializes the base class RWbostream to store
 * values in a portable binary format
 *
 * Class RWeostream specializes the base class RWbostream to store
 * values in a portable binary format. The results can be restored
 * via its counterpart, RWeistream.
 *
 * See the entry for RWeistream for a general description of the
 * endian stream classes.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/estream.h>
 * // Construct an RWeostream that uses cout's streambuf,
 * // and writes out values in little-endian format
 * RWeostream estr(std::cout, RWeostream::LittleEndian);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None.
 *
 * @section example Example
 *
 * @code
 * #include <iostream>
 * #include <fstream>
 *
 * #include <rw/estream.h>
 *
 * int main()
 * {
 *     // Open an output file
 *     std::ofstream fstr("data.dat");
 *
 *     // Construct an RWeostream from it
 *     RWeostream estr(fstr);
 *
 *     int i = 5;
 *     float f = 22.1f;
 *     double d = -0.05;
 *
 *     std::cout << "An integer, a float and a double sent out on a RWeostream!\n";
 *
 *     // Store a float & double
 *     estr << i << f << d;
 *
 *     return 0;
 * }
 * @endcode
 */
class RW_TOOLS_GLOBAL RWeostream : public RWbostream
{
public:

    /**
     * Specifies the format to use to represent numeric values in
     * the stream.
     */
    enum Endian {
        /**
         * Use little-endian format.
         */
        LittleEndian,

        /**
         * Use big-endian format.
         */
        BigEndian,

        /**
         * Use native format of the current environment.
         */
        HostEndian
    };

    /**
     * Constructs an RWeostream from the \b std::streambuf associated with
     * the \b std::ostream \a str. Values placed into the stream have
     * an endian format given by \a fmt. For DOS, the \a str must have
     * been opened in binary mode.
     *
     * @throw
     * RWStreamErr Thrown if the underlying \b std::streambuf is not empty.
     */
    RWeostream(std::ostream& str, Endian fmt = HostEndian);

    /**
     * Constructs an RWeostream from the \b std::streambuf \a s. Values
     * placed into the stream have an endian format given by \a
     * fmt. For DOS, the \b std::streambuf \a s must have been created
     * in binary mode.
     *
     * @throws
     * RWStreamErr Thrown if the \b std::streambuf \a sb is not
     * empty.
     */
    RWeostream(std::streambuf* sb, Endian fmt = HostEndian);

    // Documented in base class.
    virtual RWvostream& put(char c) {
        return RWbostream::put(c);
    }

    // Documented in base class.
    virtual RWvostream& putChar(char c) {
        return RWeostream::put(c);
    }

    // Documented in base class.
    virtual RWvostream& put(signed char c) {
        return RWbostream::put(c);
    }

    // Documented in base class.
    virtual RWvostream& putChar(signed char c) {
        return RWeostream::put(c);
    }

    // Documented in base class.
    virtual RWvostream& put(unsigned char c) {
        return RWbostream::put(c);
    }

    // Documented in base class.
    virtual RWvostream& putChar(unsigned char c) {
        return RWeostream::put(c);
    }

    // Documented in base class.
    virtual RWvostream& put(wchar_t wc);

    // Documented in base class.
    virtual RWvostream& putChar(wchar_t wc) {
        return RWeostream::put(wc);
    }

    // Documented in base class.
    virtual RWvostream& put(bool b) {
        return RWbostream::put(b);
    }

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
    /**
     * @throws
     * RWInternalErr This function is not supported with endian
     * streams.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long double</tt> type.
     */
    virtual RWvostream& put(long double) {
        RWTHROW(RWInternalErr("not supported"));
        return *this;
    }
#endif

    // Documented in base class.
    virtual RWvostream& put(const char* v, size_t n) {
        return RWbostream::put(v, n);
    }

    // Documented in base class.
    virtual RWvostream& put(const signed char* v, size_t n) {
        return RWbostream::put(v, n);
    }

    // Documented in base class.
    virtual RWvostream& put(const unsigned char* v, size_t n) {
        return RWbostream::put(v, n);
    }

    // Documented in base class.
    virtual RWvostream& put(const wchar_t* v, size_t n);

    // Documented in base class.
    virtual RWvostream& put(const bool* v, size_t n) {
        return RWbostream::put(v, n);
    }

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
    /**
     * @throws
     * RWInternalErr This function is not supported with endian
     * streams.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long double</tt> type.
     */
    virtual RWvostream& put(const long double*, size_t) {
        RWTHROW(RWInternalErr("not supported"));
        return *this;
    }
#endif

    // Documented in base class.
    virtual RWvostream& putString(const char* s, size_t n);

    // Documented in base class.
    virtual RWvostream& putChars(const char* s, size_t n);

    // Documented in base class.
    virtual RWvostream& putSizeT(size_t sz);

    // Documented in base class.
    virtual RWvostream& flush() {
        return RWbostream::flush();
    }

#if defined(_MSC_VER)
    virtual RWvostream& vflush() {
        return this->flush();
    }
#endif

protected:
    virtual void putHeader();

    Endian streamEndian_;
};


/************************************************
 *                                              *
 *              class RWeistream                *
 *                                              *
 ************************************************/

/**
* @ingroup virtual_stream_classes
*
* @brief Specializes the base class RWbistream to restore
* values previously stored by RWeostream
*
* Class RWeistream specializes the base class RWbistream to restore
* values previously stored by RWeostream. Please note, RWeostream
* must be opened before RWeistream, or the application does not
* function properly.
*
* The endian streams, RWeistream and RWeostream, offer an efficient
* compromise between the portable streams (RWpistream, RWpostream)
* and the binary streams (RWbistream, RWbostream). By compensating
* for differences in big-endian vs. little-endian formats, as well
* as sizes of the various integral types, the endian streams offer
* portability without incurring the stream-size overhead of translating
* values into a series of printable characters. For example, data
* stored in little-endian format by an RWeostream object in a DOS
* program can be retrieved by an RWeistream object on any of several
* machines, regardless of its native endian format or the sizes
* of its integral types. Endian streams work properly when
* shared among a group of platforms that:
*
* -  Share a common size and representation (apart from endian format)
*    for types \c float and \c double.
* -  Use two's complement format for negative integral values.
*
* As with the portable streams, care must be taken when storing
* or retrieving variables of type \c char. Endian stream methods
* treat \c char types as numbers except where the method description
* explicitly states that the \c char is being treated, instead, as a
* character. See the entry for RWpostream for an example of this distinction.
*
* Data stored in an integral type on one platform may be too large
* to fit into that type on a receiving platform. If so, the RWeistream
* class' failbit is set.
*
* Endian streams can be interrogated as to their stream state using
* member functions good(), bad(), eof(), etc.
*
* @section synopsis Synopsis
*
* @code
* #include <rw/estream.h>
* RWeistream estr(cin);   // Construct an RWeistream using cin's streambuf
* @endcode
*
* @section persistence Persistence
*
* None.
*
* @section example Example
*
* @code
* #include <iostream>
* #include <fstream>
*
* #include <rw/estream.h>
*
* int main()
* {
*     // Open an input file
*     std::ifstream fstr("data.dat");
*
*     // Construct RWeistream from it
*     RWeistream estr(fstr);
*
*     int i;
*     float f;
*     double d;
*
*     // Restore an int that was stored in binary
*     if(!(estr >> i))
*     {
*         std::cout << "Run eostream.exe first!" << std::endl;
*     }
*     else
*     {
*         // Restore a float & double
*         estr >> f >> d;
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
class RW_TOOLS_GLOBAL RWeistream : public RWbistream
{

public:

    /**
     * Constructs an RWeistream from the \b std::streambuf associated with
     * the \b std::istream \a str. For DOS, \a str must have been created
     * in binary mode.
     *
     * @throws
     * RWStreamErr Thrown if the data in \a str does not describe a
     * valid endian stream.
     */
    RWeistream(std::istream& str);

    /**
     * Constructs an RWeistream from the \b std::streambuf \a sb. For DOS,
     * this \b std::streambuf must have been created in binary mode.
     *
     * @throws
     * RWStreamErr Thrown if the data in \a sb does not describe a
     * valid endian stream.
     */
    RWeistream(std::streambuf* sb);

    // Documented in base class.
    virtual int get() {
        return RWbistream::get();
    }

    // Documented in base class.
    virtual RWvistream& get(char& c) {
        return RWbistream::get(c);
    }

    // Documented in base class.
    virtual RWvistream& getChar(char& c) {
        return RWeistream::get(c);
    }

    // Documented in base class.
    virtual RWvistream& get(signed char& c) {
        return RWbistream::get(c);
    }

    // Documented in base class.
    virtual RWvistream& getChar(signed char& c) {
        return RWeistream::get(c);
    }

    // Documented in base class.
    virtual RWvistream& get(unsigned char& c) {
        return RWbistream::get(c);
    }

    // Documented in base class.
    virtual RWvistream& getChar(unsigned char& c) {
        return RWeistream::get(c);
    }

    // Documented in base class.
    virtual RWvistream& get(wchar_t& wc);

    // Documented in base class.
    virtual RWvistream& getChar(wchar_t& wc) {
        return RWeistream::get(wc);
    }

    // Documented in base class.
    virtual RWvistream& get(bool& b) {
        return RWbistream::get(b);
    }

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
    /**
     * @throws
     * RWInternalErr This function is not supported with endian
     * streams.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long double</tt> type.
     */
    virtual RWvistream& get(long double&) {
        RWTHROW(RWInternalErr("not supported"));
        return *this;
    }
#endif


    // Documented in base class.
    virtual RWvistream& get(char* v, size_t n) {
        return RWbistream::get(v, n);
    }

    // Documented in base class.
    virtual RWvistream& get(signed char* v, size_t n) {
        return RWbistream::get(v, n);
    }

    // Documented in base class.
    virtual RWvistream& get(unsigned char* v, size_t n) {
        return RWbistream::get(v, n);
    }

    // Documented in base class.
    virtual RWvistream& get(wchar_t* v, size_t n);

    // Documented in base class.
    virtual RWvistream& get(bool* v, size_t n) {
        return RWbistream::get(v, n);
    }

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
    /**
     * @throws
     * RWInternalErr This function is not supported with endian
     * streams.
     *
     * @note
     * This method is available only if your compiler supports the
     * <tt>long double</tt> type.
     */
    virtual RWvistream& get(long double*, size_t) {
        RWTHROW(RWInternalErr("not supported"));
        return *this;
    }
#endif

    // Documented in base class.
    virtual RWvistream& getString(char* s, size_t n);

    // Documented in base class.
    virtual RWvistream& getChars(char* s, size_t n);

    // Documented in base class.
    virtual RWvistream& getSizeT(size_t& sz);


public:

    /**
     * Returns the size of \c short, as represented in the stream.
     */
    size_t streamSizeofShort() {
        return streamSizeofShort_;
    }

    /**
     * Returns the size of \c int, as represented in the stream.
     */
    size_t streamSizeofInt() {
        return streamSizeofInt_;
    }

    /**
     * Returns the size of \c long, as represented in the stream.
     */
    size_t streamSizeofLong() {
        return streamSizeofLong_;
    }

    /**
     * Returns the size of \c wchar_t, as represented in the stream.
     */
    size_t streamSizeofWchar() {
        return streamSizeofWchar_;
    }

    /**
     * Returns the size of \c size_t, as represented in the stream.
     */
    size_t streamSizeofSizeT() {
        return streamSizeofSizeT_;
    }

    /**
     * Returns the endian format (RWeostream::BigEndian or RWeostream::LittleEndian)
     * of numeric values, as represented in the stream.
     */
    RWeostream::Endian streamEndian() {
        return streamEndian_;
    }

protected:
    virtual void          getHeader();
    virtual void          headerErr();
    virtual void          getDiffSize(void*, size_t, size_t, bool);

    size_t                streamSizeofShort_;
    size_t                streamSizeofInt_;
    size_t                streamSizeofLong_;
    size_t                streamSizeofWchar_;
    size_t                streamSizeofSizeT_;
    RWeostream::Endian    streamEndian_;
};


#endif /* RW_TOOLS_ESTREAM_H */
