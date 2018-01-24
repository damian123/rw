#ifndef RW_TOOLS_XDRSTREA_H
#define RW_TOOLS_XDRSTREA_H



/**********************************************************************
 *
 * rwxdr[io]stream ---  Portable I/O streams based on XDR format.
 *      Copyright (C) 1992 Sun Microsystems, Inc.  All Rights Reserved.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/xdrstrea.h#1 $
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

#ifndef RW_NO_XDR

#  include <rw/rwerr.h>

#  include <stddef.h>     /* Required for IBM xlC */
#  include <stdio.h>      /* Required for IBM xlC */

#  include <rpc/types.h>  /* Due to bug in some rpc/xdr.h. */
#  include <rpc/xdr.h>

#  include <rw/vstream.h>

// Forward declaration of RWCollectable class
class RW_TOOLS_GLOBAL RWCollectable;

// Forward declaration of xdr
int xdr(XDR*, RWCollectable*&);



#if !defined(DOXYGEN)
// The hyper operation will not be available if the xdr_ops structure
// does not have a member for it.
#  if !defined(RW_X_PUTHYPER_DECL) || !defined(RW_X_GETHYPER_DECL)
#    define RW_NO_XDR_HYPER_OP
#  endif // !RW_X_PUTHYPER_DECL || !RW_X_GETHYPER_DECL
#endif // !DOXYGEN


/**
 * @ingroup virtual_stream_classes
 * @brief A portable input stream based on XDR routines.
 *
 * Class RWXDRistream is a portable input stream based on XDR routines.
 * Class RWXDRistream encapsulates a portion of the XDR library
 * routines that are used for external data representation.  XDR
 * routines allow programmers to describe arbitrary data structures
 * in a machine-independent fashion.  Data for remote procedure
 * calls (RPC) are transmitted using XDR routines.
 *
 * Class RWXDRistream enables decoding an XDR structure to
 * a machine representation, providing the ability to decode all
 * the standard data types and vectors of those data types.
 *
 * An XDR stream must first be created by calling the appropriate
 * creation routine. XDR streams currently exist for encoding/decoding
 * of data to or from standard iostreams and file streams, TCP/IP
 * connections and Unix files, and memory. These creation routines
 * take arguments that are tailored to the specific properties of
 * the stream.  After the XDR stream has been created, it can then
 * be used as the argument to the constructor for a RWXDRistream
 * object.
 *
 * RWXDRistream can be interrogated as to the status of the stream
 * using member functions bad(), clear(), eof(), fail(), good(),
 * and rdstate().
 *
 * @conditional
 * This class is not supported on Windows.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/xdrstrea.h>
 * XDR xdr;
 * xdrstdio_create(&xdr, stdin, XDR_DECODE);
 * RWXDRistream rw_xdr(&xdr);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 *
 * #include <iostream>
 * #include <rw/xdrstrea.h>
 *
 * int main()
 * {
 *     XDR xdr;
 *     FILE* fp = fopen("test", "r+");
 *     if (fp == 0) {
 *         std::cout << "Run xdrostrm first.\n";
 *         return 1;
 *     }
 *
 *     xdrstdio_create(&xdr, fp, XDR_DECODE);
 *
 *     RWXDRistream rw_xdr(&xdr);
 *     int data;
 *
 *     for(int i = 0; i < 10; ++i) {
 *       rw_xdr >> data; // decode integer data
 *
 *       if(data == i)
 *         std::cout << data << std::endl;
 *       else
 *         std::cout << "Bad input value" << std::endl;
 *     }
 *
 *     fclose(fp);
 *
 *     return 0;
 * }
 * @endcode
 */
class RW_TOOLS_GLOBAL RWXDRistream : public RWvistream, public RWios
{

public:

    /**
     * Initializes an RWXDRistream from the XDR structure \a xp.
     */
    RWXDRistream(XDR* xp);

    /**
     * Initializes an RWXDRistream from the \b std::streambuf \a sb.
     * \a sb must already be allocated.
     */
    RWXDRistream(std::streambuf* sb);

    /**
     * Initializes an RWXDRistream from the \b std::streambuf associated
     * with the \b std::istream \a str.
     */
    RWXDRistream(std::istream& str);

    /**
     * Deallocates previously allocated resources.
     */
    virtual ~RWXDRistream();

    // Documented in base class.
    virtual int eof() {
        return RWios::eof();
    }

    // Documented in base class.
    virtual int fail() {
        return RWios::fail();
    }

    // Documented in base class.
    virtual int bad() {
        return RWios::bad();
    }

    // Documented in base class.
    virtual int good() {
        return RWios::good();
    }

    // Documented in base class.
    virtual int rdstate() {
        return RWios::rdstate();
    }

    // Documented in base class.
    virtual void clear(int v = 0) {
        RWios::clear(v);
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
    virtual RWvistream& getChar(char& c) {
        return RWXDRistream::get(c);
    }

    // Documented in base class.
    virtual RWvistream& get(signed char& c);

    // Documented in base class.
    virtual RWvistream& getChar(signed char& c) {
        return RWXDRistream::get(c);
    }

    // Documented in base class.
    virtual RWvistream& get(unsigned char& c);

    // Documented in base class.
    virtual RWvistream& getChar(unsigned char& c) {
        return RWXDRistream::get(c);
    }

    // Documented in base class.
    virtual RWvistream& get(wchar_t& wc);

    // Documented in base class.
    virtual RWvistream& getChar(wchar_t& wc) {
        return RWXDRistream::get(wc);
    }

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

#  if !defined(RW_NO_LONG_LONG)
#    if !defined(RW_NO_XDR_HYPER_OP)
    /**
     * Gets the next <tt>long long</tt> from the input stream and
     * stores it in \a i.
     *
     * @throws
     * RWInternalErr Thrown if the necessary support for reading a
     * <tt>long long</tt> from an XDR is not available.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long long</tt> type.
     */
    virtual RWvistream& get(long long& i);

    /**
     * Gets the next <tt>unsigned long long</tt> from the input stream
     * and stores it in \a i.
     *
     * @throws
     * RWInternalErr Thrown if the necessary support for reading an
     * <tt>unsigned long long</tt> from an XDR is not available.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>unsigned long long</tt> type.
     */
    virtual RWvistream& get(unsigned long long& i);
#    else
    virtual RWvistream& get(long long&) {
        RWTHROW(RWInternalErr("not available on platform."));
        return *this;
    }
    virtual RWvistream& get(unsigned long long&) {
        RWTHROW(RWInternalErr("not available on platform."));
        return *this;
    }
#    endif
#  endif

    // Documented in base class.
    virtual RWvistream& get(float& f);

    // Documented in base class.
    virtual RWvistream& get(double& d);

#  if !defined(RW_NO_LONG_DOUBLE)
    /**
     * @throws
     * RWInternalErr This function is not supported with XDR
     * streams.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long double</tt> type.
     */
    virtual RWvistream& get(long double&) {
        RWTHROW(RWInternalErr("not available on platform."));
        return *this;
    }
#  endif

    // Documented in base class.
    virtual RWvistream& get(char* v, size_t n);

    // Documented in base class.
    virtual RWvistream& get(signed char* v, size_t n);

    // Documented in base class.
    virtual RWvistream& get(unsigned char* v, size_t n);

    // Documented in base class.
    virtual RWvistream& get(wchar_t*, size_t n);

    // Documented in base class.
    virtual RWvistream& get(bool*, size_t n);

    // Documented in base class.
    virtual RWvistream& get(float* v, size_t n);

    // Documented in base class.
    virtual RWvistream& get(double* v, size_t n);

#  if !defined(RW_NO_LONG_DOUBLE)
    /**
     * @throws
     * RWInternalErr This function is not supported with XDR
     * streams.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long double</tt> type.
     */
    virtual RWvistream& get(long double*, size_t) {
        RWTHROW(RWInternalErr("not available on platform."));
        return *this;
    }
#  endif


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

    // Long long streaming only if long long type exists and hyper op is
    // available
#  if !defined(RW_NO_LONG_LONG)
#    if !defined(RW_NO_XDR_HYPER_OP)
    /**
     * Gets a vector of \a n <tt>long long</tt> and stores them
     * in the array beginning at \a v.
     *
     * @throws
     * RWInternalErr Thrown if the necessary support for reading a
     * <tt>long long</tt> from an XDR is not available.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long long</tt> type.
     */
    virtual RWvistream& get(long long* v, size_t n);

    /**
     * Gets a vector of \a n <tt>unsigned long long</tt> and stores
     * them in the array beginning at \a v.
     *
     * @throws
     * RWInternalErr Thrown if the necessary support for reading an
     * <tt>unsigned long long</tt> from an XDR is not available.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>unsigned long long</tt> type.
     */
    virtual RWvistream& get(unsigned long long* v, size_t n);
#    else
    virtual RWvistream& get(long long*, size_t) {
        RWTHROW(RWInternalErr("not available on platform."));
        return *this;
    }
    virtual RWvistream& get(unsigned long long*, size_t) {
        RWTHROW(RWInternalErr("not available on platform."));
        return *this;
    }
#    endif
#endif

    // Documented in base class.
    virtual RWvistream& getString(char* s, size_t n);

    // Documented in base class.
    virtual RWvistream& getChars(char* s, size_t n);

    // Documented in base class.
    virtual RWvistream& getSizeT(size_t& sz);

private:

    XDR*  xdrp;
    int streambufXDR;
};


/**
 * @ingroup virtual_stream_classes
 * @brief A portable output stream based on XDR routines.
 *
 * Class RWXDRostream is a portable output stream based on XDR routines.
 * Class RWXDRostream encapsulates a portion of the XDR library
 * routines that are used for external data representation.  XDR
 * routines allow programmers to describe arbitrary data structures
 * in a machine-independent fashion.  Data for remote procedure
 * calls (RPC) are transmitted using XDR routines.
 *
 * Class RWXDRostream enables outputting from a stream and encoding
 * an XDR structure from a machine representation.  RWXDRostream
 * provides the ability to encode the standard data types and
 * vectors of those data types.
 *
 * An XDR stream must first be created by calling the appropriate
 * creation routine. XDR streams currently exist for encoding/decoding
 * of data to or from standard iostreams and file streams, TCP/IP
 * connections and Unix files, and memory. These creation routines
 * take arguments that are tailored to the specific properties of
 * the stream.  After the XDR stream has been created, it can then
 * be used as an argument to the constructor for a RWXDRostream
 * object.
 *
 * RWXDRostream can be interrogated as to the status of the stream
 * using member functions bad(), clear(), eof(), fail(), good(),
 * and rdstate().
 *
 * @conditional
 * This class is not supported on Windows.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/xdrstrea.h>
 * XDR xdr;
 * xdrstdio_create(&xdr, stdout, XDR_ENCODE) ;
 * RWXDRostream rw_xdr(&xdr);
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 *
 * #include <iostream>
 * #include <rw/xdrstrea.h>
 *
 * int main()
 * {
 *     XDR xdr;
 *
 *     FILE* fp = fopen("test","w+");
 *     if (fp == 0) {
 *         std::cerr << "failed to open file.\n";
 *         return 1;
 *     }
 *
 *     xdrstdio_create(&xdr, fp, XDR_ENCODE);
 *
 *     RWXDRostream rw_xdr(&xdr);
 *
 *     for(int i = 0; i < 10; ++i)
 *         rw_xdr << i; // encode integer data
 *
 *     fclose(fp);
 *
 *     return 0;
 * }
 * @endcode
 */
class RW_TOOLS_GLOBAL RWXDRostream : public RWvostream, public RWios
{

public:

    /**
     * Initializes an RWXDRostream from the XDR structure \a xp.
     */
    RWXDRostream(XDR* xp);

    /**
     * Initializes an RWXDRostream from the \b std::streambuf \a sb.
     * \a sb must already be allocated.
     */
    RWXDRostream(std::streambuf* sb);

    /**
     * Initializes an RWXDRistream from the \b std::streambuf associated
     * with the \b std::istream \a str.
     */
    RWXDRostream(std::ostream& str);

    /**
     * Deallocates previously allocated resources.
     */
    virtual ~RWXDRostream();

    // Documented in base class.
    virtual int eof() {
        return RWios::eof();
    }

    // Documented in base class.
    virtual int fail() {
        return RWios::fail();
    }

    // Documented in base class.
    virtual int bad() {
        return RWios::bad();
    }

    // Documented in base class.
    virtual int good() {
        return RWios::good();
    }

    // Documented in base class.
    virtual int rdstate() {
        return RWios::rdstate();
    }

    // Documented in base class.
    virtual void clear(int v = 0) {
        RWios::clear(v);
    }

    // Documented in base class.
    operator void* () {
        return RWvostream::operator void * ();
    }

    // Documented in base class.
    virtual RWvostream& put(char c);

    // Documented in base class.
    virtual RWvostream& putChar(char c) {
        return RWXDRostream::put(c);
    }

    // Documented in base class.
    virtual RWvostream& put(signed char c);

    // Documented in base class.
    virtual RWvostream& putChar(signed char c) {
        return RWXDRostream::put(c);
    }

    // Documented in base class.
    virtual RWvostream& put(unsigned char c);

    // Documented in base class.
    virtual RWvostream& putChar(unsigned char c) {
        return RWXDRostream::put(c);
    }

    // Documented in base class.
    virtual RWvostream& put(wchar_t wc);

    // Documented in base class.
    virtual RWvostream& putChar(wchar_t wc) {
        return RWXDRostream::put(wc);
    }

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

    /**
     * Stores the \c long \a i to the output stream.
     *
     * @note
     * In a 64-bit environment, this method fails if the value of
     * \a i is outside the range of a 32-bit signed integer.
     */
    virtual RWvostream& put(long i);

    /**
     * Stores the <tt>unsigned long</tt> \a i to the output stream.
     *
     * @note
     * In a 64-bit environment, this method fails if the value of
     * \a i is outside the range of a 32-bit unsigned integer.
     */
    virtual RWvostream& put(unsigned long i);

#  if !defined(RW_NO_LONG_LONG)
#    if !defined(RW_NO_XDR_HYPER_OP)
    /**
     * Stores the <tt>long long</tt> \a i to the output stream.
     *
     * @throws
     * RWInternalErr Thrown if the necessary support for storing a
     * <tt>long long</tt> on an XDR is not available.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long long</tt> type.
     */
    virtual RWvostream& put(long long i);

    /**
     * Stores the <tt>unsigned long long</tt> \a i to the output stream.
     *
     * @throws
     * RWInternalErr Thrown if the necessary support for storing an
     * <tt>unsigned long long</tt> on an XDR is not available.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>unsigned long long</tt> type.
     */
    virtual RWvostream& put(unsigned long long i);
#    else
    virtual RWvostream& put(long long) {
        RWTHROW(RWInternalErr("not available on platform."));
        return *this;
    }
    virtual RWvostream& put(unsigned long long) {
        RWTHROW(RWInternalErr("not available on platform."));
        return *this;
    }
#    endif
#  endif

    // Documented in base class.
    virtual RWvostream& put(float f);

    // Documented in base class.
    virtual RWvostream& put(double d);

#  if !defined(RW_NO_LONG_DOUBLE)
    /**
     * @throws
     * RWInternalErr This function is not supported with XDR
     * streams.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long double</tt> type.
     */
    virtual RWvostream& put(long double) {
        RWTHROW(RWInternalErr("not available on platform."));
        return *this;
    }
#  endif

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

    /**
     * Stores the vector of \a n \c long starting at \a v to the output
     * stream.
     *
     * @note
     * In a 64-bit environment, this method fails if the value of
     * one of the stored elements in \a v is outside the range of
     * a 32-bit signed integer.
     */
    virtual RWvostream& put(const long* v, size_t n);

    /**
     * Stores the vector of \a n <tt>unsigned long</tt> starting at \a v
     * to the output stream.
     *
     * @note
     * In a 64-bit environment, this method fails if the value of
     * one of the stored elements in \a v is outside the range of
     * a 32-bit unsigned integer.
     */
    virtual RWvostream& put(const unsigned long* v, size_t n);

#  if !defined(RW_NO_LONG_LONG)
#    if !defined(RW_NO_XDR_HYPER_OP)
    /**
     * Stores the vector of \a n <tt>long long</tt> starting at \a v
     * to the output stream.
     *
     * @throws
     * RWInternalErr Thrown if the necessary support for storing a
     * <tt>long long</tt> on an XDR is not available.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long long</tt> type.
     */
    virtual RWvostream& put(const long long* v, size_t n);

    /**
     * Stores the vector of \a n <tt>unsigned long long</tt> starting
     * at \a v to the output stream.
     *
     * @throws
     * RWInternalErr Thrown if the necessary support for storing an
     * <tt>unsigned long long</tt> on an XDR is not available.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>unsigned long long</tt> type.
     */
    virtual RWvostream& put(const unsigned long long* v, size_t n);
#    else
    virtual RWvostream& put(const long long*, size_t) {
        RWTHROW(RWInternalErr("not available on platform."));
        return *this;
    }
    virtual RWvostream& put(const unsigned long long*, size_t) {
        RWTHROW(RWInternalErr("not available on platform."));
        return *this;
    }
#    endif
#  endif


    // Documented in base class.
    virtual RWvostream& put(const float* v, size_t n);

    // Documented in base class.
    virtual RWvostream& put(const double* v, size_t n);

#  if !defined(RW_NO_LONG_DOUBLE)
    /**
     * @throws
     * RWInternalErr This function is not supported with XDR
     * streams.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long double</tt> type.
     */
    virtual RWvostream& put(const long double*, size_t) {
        RWTHROW(RWInternalErr("not available on platform."));
        return *this;
    }
#  endif

    // Documented in base class.
    virtual RWvostream& putString(const char* s, size_t n);

    // Documented in base class.
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

private:

    XDR*  xdrp;
    int streambufXDR;
};

#endif // RW_NO_XDR

#endif /* RW_TOOLS_XDRSTREA_H */
