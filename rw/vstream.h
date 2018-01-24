#ifndef RW_TOOLS_VSTREAM_H
#define RW_TOOLS_VSTREAM_H

/**********************************************************************
 *
 * rw[io]stream --- abstract base class for I/O
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/vstream.h#2 $
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
#include <rw/tools/ristream.h>
#include <rw/tools/rostream.h>

#include <string.h>

/*
 * vstreams maintain a version number to help with backward compatibility
 * issues.  See RWCString::saveOn for an example of how the version number
 * can be used internally.  Users of the library can set the version
 * number of a stream to force it to behave as in a previous incarnation.
 *
 *     Version 0: Baseline (virtually introduced with Tool600)
 *     Version 1: RWCStrings now save/restore themselves to/from virtual
 *                streams as sequence of characters, not numbers. (Introduced
 *                with Tool602).
 *     Version 2: RWCString size is only stored once on binary and endian
 *                streams.  Fixes issue with non-printable characters and
 *                portable streams.
 *     Version 3: size_t values are compressed for binary and endian streams.
 */
static const unsigned current_version = 3;

/************************************************
 *                                              *
 *              class RWvios                    *
 *                                              *
 ************************************************/

/**
 * @ingroup virtual_stream_classes
 * @brief Abstract base class defining an interface similar to
 * \b std::ios, but without a required association with \b std::streambuf.
 *
 * RWvios is an abstract base class that defines an interface similar
 * to the C++ Standard Library streams class \b std::ios.  However, unlike
 * \b std::ios, it does not require association
 * with a \b std::streambuf.
 *
 * This disassociation can be useful for classes that cannot use a
 * \b std::streambuf in their implementation, for example, RWXDRistream
 * in which the XDR model does not permit \b std::streambuf functionality.
 *
 * Specializing classes that do use \b std::streambuf in their implementation
 * (such as RWpistream) can usually just return the corresponding
 * \b std::ios function.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <vstream.h>
 * (abstract base class)
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWvios
{
public:

    virtual ~RWvios();

    /**
     * Returns a nonzero integer if an \c EOF is encountered.
     */
    virtual int eof() = 0;

    /**
     * Returns a nonzero integer if the failed or bad bit has been set.
     * Normally, this indicates that some storage or retrieval has
     * failed, but that the stream is still in a usable state.
     */
    virtual int fail() = 0;

    /**
     * Returns a nonzero integer if the bad bit has been set.  Normally
     * this indicates that a severe error has occurred from which recovery
     * is probably impossible.
     */
    virtual int bad() = 0;

    /**
     * Returns a nonzero integer if no error bits have been set.
     */
    virtual int good() = 0;

    /**
     * Returns the current error state.
     */
    virtual int rdstate() = 0;

    /**
     * Sets the current error state to \a v.  If \a v is zero, then
     * this clears the error state.
     */
    virtual void clear(int v = 0) = 0;

    /**
     * If fail(), returns \c 0, otherwise returns a pointer to self.
     */
    operator void* () {
        return fail() ? 0 : this;
    }

#if (__SUNPRO_CC_COMPAT == 5)
    // Work around sunpro bug that can cause the wrong virtual
    // function to be invoked when a class with no data members
    // is used in a hierarchy with both multiple inheritance and
    // virtual inheritance. See SP-14356 for details.
    int dummy_;
#endif
};


/************************************************
 *                                              *
 *              class RWios                     *
 *                                              *
 ************************************************/

// Convenience class for virtual streams that want ios type functionality,
// but cannot use streambufs in their implementation:

class RW_TOOLS_GLOBAL RWios : public virtual RWvios
{
public:

    virtual ~RWios();

    RWios()
        : status(0) {
    }

    virtual int eof() {
        return status & std::ios::eofbit;
    }

    virtual int fail() {
        return status & (std::ios::failbit | std::ios::badbit);
    }

    virtual int bad() {
        return status & std::ios::badbit;
    }

    virtual int good() {
        return status == 0;
    }

    virtual int rdstate() {
        return status;
    }

    virtual void clear(int v = 0) {
        status = v;
    }

protected:

    int status;

};


/************************************************
 *                                              *
 *              class RWvistream                *
 *                                              *
 ************************************************/

/**
 * @ingroup virtual_stream_classes
 * @brief Abstract base class providing an interface for format-independent
 * retrieval of fundamental types and fundamental-type arrays.
 *
 * Class RWvistream is an abstract base class that provides an interface
 * for format-independent retrieval of fundamental types, and arrays
 * of fundamental types.  Its counterpart, RWvostream, provides
 * a complementary interface for the storage of the fundamental
 * types.
 *
 * Because the interface of RWvistream and RWvostream is independent
 * of formatting, your application need not be concerned
 * with how variables are actually stored or restored -- functionality
 * that is the responsibility of whatever derived class you choose.
 * For instance, you could use an operating-system independent US-ASCII format
 * (classes RWpistream and RWpostream), a binary format (classes
 * RWbistream and RWbostream), or define your own format (such as an
 * interface to a network).
 *
 * @note
 * Because this is an abstract base class, these design goals cannot
 * be enforced; rather, the description here is merely the model of
 * how a class derived from RWvistream and RWvostream should act.
 *
 * See class RWvostream for additional explanations and examples
 * of format-independent stream storage.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/vstream.h>
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/vstream.h>
 * #include <rw/bstream.h>
 * #include <rw/pstream.h>
 * #include <fstream>
 * #include <iostream>
 *
 * void restoreStuff(RWvistream& str)
 * {
 *     int i;
 *     double d;
 *     char s [80];
 *
 *     str >> i; // Restore an int
 *     str >> d; // Restore a double
 *
 *     // Restore a character string, up to 80 characters long:
 *     str.getString(s, sizeof s);
 *
 *     if (str.fail()) {
 *         std::cerr << "Restoration failed.\n\n";
 *         return;
 *     }
 *
 *     std::cout << "Int        : "   << i << "\n";
 *     std::cout << "Double     : "   << d << "\n";
 *     std::cout << "Char array : \"" << s << "\"\n";
 * }
 *
 * int main()
 * {
 *     std::ifstream vf("vfile.dat");
 *
 *     RWpistream ps(vf);
 *     RWbistream bs(vf);
 *
 *     // Uncomment the call to storeStuff that matches your stream type
 *
 *     restoreStuff(ps);
 *     // restoreStuff(bs);
 *
 *     return 0;
 * }
 * @endcode
 */
class RW_TOOLS_GLOBAL RWvistream : public virtual RWvios
{
public:

    /**
     * Empty destructor.
     */
    virtual ~RWvistream();

    // Documented in base class.
    virtual int eof() = 0;

    // Documented in base class.
    virtual int fail() = 0;

    // Documented in base class.
    virtual int bad() = 0;

    // Documented in base class.
    virtual int good() = 0;

    // Documented in base class.
    virtual int rdstate() = 0;

    // Documented in base class.
    virtual void clear(int v = 0) = 0;

    // Documented in base class.
    operator void* () {
        return RWvios::operator void * ();
    }

    /**
     * Gets and returns the next byte from the input stream, returning
     * its value. Returns \c EOF if end of file is encountered.
     */
    virtual int get() = 0;     // Treat as number; EOF upon eof

    /**
     * Gets the next \c char from the input stream, returning its value
     * in \a c.
     */
    virtual RWvistream& get(char& c) = 0;

    /**
     * Gets the next \c char from the input stream, returning its value
     * in \a c. \a c is treated as a character.
     */
    virtual RWvistream& getChar(char& c) = 0;

    /**
     * Gets the next <tt>signed char</tt> from the input stream, returning
     * its value in \a c.
     */
    virtual RWvistream& get(signed char& c) = 0;

    /**
     * Gets the next <tt>signed char</tt> from the input stream, returning
     * its value in \a c. \a c is treated as a character.
     */
    virtual RWvistream& getChar(signed char& c) = 0;

    /**
     * Gets the next <tt>unsigned char</tt> from the input stream, returning
     * its value in \a c.
     */
    virtual RWvistream& get(unsigned char& c) = 0;

    /**
     * Gets the next <tt>unsigned char</tt> from the input stream, returning
     * its value in \a c. \a c is treated as a character.
     */
    virtual RWvistream& getChar(unsigned char& c) = 0;

    /**
     * Gets the next \c wchar_t from the input stream, returning its
     * value in \a wc.
     */
    virtual RWvistream& get(wchar_t& wc) = 0;

    /**
     * Gets the next \c wchar_t from the input stream, returning its
     * value in \a wc. \a wc is treated as a character.
     */
    virtual RWvistream& getChar(wchar_t& wc) = 0;

    /**
     * Gets the next \c bool from the input stream, returning its value
     * in \a b.
     */
    virtual RWvistream& get(bool& b) = 0;

    /**
     * Gets the next \c short from the input stream, returning its value
     * in \a i.
     */
    virtual RWvistream& get(short& i) = 0;

    /**
     * Gets the next <tt>unsigned short</tt> from the input stream, returning
     * its value in \a i.
     */
    virtual RWvistream& get(unsigned short& i) = 0;

    /**
     * Gets the next \c int from the input stream, returning its value in \a i.
     */
    virtual RWvistream& get(int& i) = 0;

    /**
     * Gets the next <tt>unsigned int</tt> from the input stream, returning
     * its value in \a i.
     */
    virtual RWvistream& get(unsigned int& i) = 0;

    /**
     * Gets the next \c long from the input stream, returning its value in \a i.
     */
    virtual RWvistream& get(long& i) = 0;

    /**
     * Gets the next <tt>unsigned long</tt> from the input stream, returning
     * its value in \a i.
     */
    virtual RWvistream& get(unsigned long& i) = 0;


#if !defined(RW_NO_LONG_LONG)
    /**
     * Gets the next <tt>long long</tt> from the input stream, returning its
     * value in \a i.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long long</tt> type.
     */
    virtual RWvistream& get(long long& i) = 0;

    /**
     * Gets the next <tt>unsigned long long</tt> from the input stream,
     * returning its value in \a i.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>unsigned long long</tt> type.
     */
    virtual RWvistream& get(unsigned long long& i) = 0;
#endif

    /**
     * Gets the next \c float from the input stream, returning its value in \a f.
     */
    virtual RWvistream& get(float& f) = 0;

    /**
     * Gets the next \c double from the input stream, returning its value in \a d.
     */
    virtual RWvistream& get(double& d) = 0;

#if !defined(RW_NO_LONG_DOUBLE)
    /**
     * Gets the next <tt>long double</tt> from the input stream, returning its
     * value in \a d.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long double</tt> type.
     */
    virtual RWvistream& get(long double& d) = 0;
#endif

    /**
     * Gets a vector of \a n \c char and stores them in the array beginning
     * at \a v. If the restore operation stops prematurely because there
     * is no more data available on the stream, because an exception
     * is thrown, or for some other reason, stores what has already
     * been retrieved from the stream into \a v, and sets the failbit.
     *
     * @note
     * This method retrieves raw characters and does not perform
     * any conversions on special characters such as <tt>'\\n'</tt>.
     */
    virtual RWvistream& get(char* v, size_t n) = 0;

    /**
     * Gets a vector of \a n <tt>signed char</tt> and stores them in the
     * array beginning at \a v. If the restore operation stops prematurely
     * because there is no more data available on the stream, because an
     * exception is thrown, or for some other reason, stores what has
     * already been retrieved from the stream into \a v, and sets the
     * failbit.
     */
    virtual RWvistream& get(signed char* v, size_t n) = 0;

    /**
     * Gets a vector of \a n <tt>unsigned char</tt> and stores them in the
     * array beginning at \a v. If the restore operation stops prematurely
     * because there is no more data available on the stream, because an
     * exception is thrown, or for some other reason, stores what has
     * already been retrieved from the stream into \a v, and sets the
     * failbit.
     */
    virtual RWvistream& get(unsigned char* v, size_t n) = 0;

    /**
     * Gets a vector of \a n \c wchar_t and stores them in the array
     * beginning at \a v. If the restore operation stops prematurely
     * because there is no more data available on the stream, because an
     * exception is thrown, or for some other reason, stores what has
     * already been retrieved from the stream into \a v, and sets the
     * failbit.
     *
     * @note
     * This method retrieves raw characters and does not perform
     * any conversions on special characters such as <tt>L'\\n'</tt>.
     */
    virtual RWvistream& get(wchar_t* v, size_t n) = 0;

    /**
     * Gets a vector of \a n \c bool and stores them in the array beginning
     * at \a v. If the restore operation stops prematurely because there
     * is no more data available on the stream, because an exception
     * is thrown, or for some other reason, stores what has already
     * been retrieved from the stream into \a v, and sets the failbit.
     */
    virtual RWvistream& get(bool* v, size_t n) = 0;

    /**
     * Gets a vector of \a n \c short and stores them in the array beginning
     * at \a v. If the restore operation stops prematurely because there is
     * no more data available on the stream, because an exception is thrown,
     * or for some other reason, stores what has already been retrieved from
     * the stream into \a v, and sets the failbit.
     */
    virtual RWvistream& get(short* v, size_t n) = 0;

    /**
     * Gets a vector of \a n <tt>unsigned short</tt> and stores them in the
     * array beginning at \a v. If the restore operation stops prematurely
     * because there is no more data available on the stream, because an
     * exception is thrown, or for some other reason, stores what has
     * already been retrieved from the stream into \a v, and sets the
     * failbit.
     */
    virtual RWvistream& get(unsigned short* v, size_t n) = 0;

    /**
     * Gets a vector of \a n \c int and stores them in the array beginning
     * at \a v. If the restore operation stops prematurely because there
     * is no more data available on the stream, because an exception is
     * thrown, or for some other reason, stores what has already been
     * retrieved from the stream into \a v, and sets the failbit.
     */
    virtual RWvistream& get(int* v, size_t n) = 0;

    /**
     * Gets a vector of \a n <tt>unsigned int</tt> and stores them in the
     * array beginning at \a v. If the restore operation stops prematurely
     * because there is no more data available on the stream, because an
     * exception is thrown, or for some other reason, stores what has
     * already been retrieved from the stream into \a v, and sets the
     * failbit.
     */
    virtual RWvistream& get(unsigned int* v, size_t n) = 0;

    /**
     * Gets a vector of \a n \c long and stores them in the array beginning
     * at \a v. If the restore operation stops prematurely because there is
     * no more data available on the stream, because an exception is thrown,
     * or for some other reason, stores what has already been retrieved from
     * the stream into \a v, and sets the failbit.
     */
    virtual RWvistream& get(long* v, size_t n) = 0;

    /**
     * Gets a vector of \a n <tt>unsigned long</tt> and stores them in the
     * array beginning at \a v. If the restore operation stops prematurely
     * because there is no more data available on the stream, because an
     * exception is thrown, or for some other reason, stores what has
     * already been retrieved from the stream into \a v, and sets the
     * failbit.
     */
    virtual RWvistream& get(unsigned long* v, size_t n) = 0;

#if !defined(RW_NO_LONG_LONG)
    /**
     * Gets a vector of \a n <tt>long long</tt> and stores them in the
     * array beginning at \a v. If the restore operation stops prematurely
     * because there is no more data available on the stream, because an
     * exception is thrown, or for some other reason, stores what has
     * already been retrieved from the stream into \a v, and sets the failbit.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long long</tt> type.
     */
    virtual RWvistream& get(long long* v, size_t n) = 0;

    /**
     * Gets a vector of \a n <tt>unsigned long long</tt> and stores them in
     * the array beginning at \a v. If the restore operation stops prematurely
     * because there is no more data available on the stream, because an
     * exception is thrown, or for some other reason, stores what has already
     * been retrieved from the stream into \a v, and sets the failbit.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>unsigned long long</tt> type.
     */
    virtual RWvistream& get(unsigned long long* v, size_t n) = 0;
#endif

    /**
     * Gets a vector of \a n \c float and stores them in the array beginning
     * at \a v. If the restore operation stops prematurely because there is
     * no more data available on the stream, because an exception is thrown,
     * or for some other reason, stores what has already been retrieved from
     * the stream into \a v, and sets the failbit.
     */
    virtual RWvistream& get(float* v, size_t n) = 0;

    /**
     * Gets a vector of \a n \c double and stores them in the array beginning
     * at \a v. If the restore operation stops prematurely because there is
     * no more data available on the stream, because an exception is thrown,
     * or for some other reason, stores what has already been retrieved from
     * the stream into \a v, and sets the failbit.
     */
    virtual RWvistream& get(double* v, size_t n) = 0;

#if !defined(RW_NO_LONG_DOUBLE)
    /**
     * Gets a vector of \a n <tt>long double</tt> and stores them in the
     * array beginning at \a v. If the restore operation stops prematurely
     * because there is no more data available on the stream, because an
     * exception is thrown, or for some other reason, stores what has
     * already been retrieved from the stream into \a v, and sets the
     * failbit.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long double</tt> type.
     */
    virtual RWvistream& get(long double* v, size_t n) = 0;
#endif

    /**
     * Restores a character string from the input stream that has been stored
     * to the output stream using putString(), then saves it in the array
     * beginning at \a s. The function stops reading at the end of the string
     * or after \a n <tt>- 1</tt> characters, whichever comes first. If
     * \a n <tt>- 1</tt> characters have been read and the \a n<sup>th</sup>
     * character is not the string terminator, then sets the failbit of the
     * stream.  In either case, the string is terminated with a null byte.
     */
    virtual RWvistream& getString(char* s, size_t n) = 0;

    /**
     * Restores \a n \c char from the input stream into the array beginning at
     * \a s. The function stops reading after \a n \c char. The resulting
     * buffer is not null terminated. \a s is treated as a character string.
     *
     * @note
     * This function delegates to get(char*, size_t) if not overridden.
     */
    virtual RWvistream& getChars(char* s, size_t n) {
        //
        // We provide a default implementation that calls get() to avoid forcing
        // the user to provide an implementation. This default breaks behavior for
        // user-provided stream classes when serializing RWCString.
        //
        return get(s, n);
    }

    /**
     * Gets the next \c size_t value from the input stream, returning
     * its value in \a sz.
     */
    virtual RWvistream& getSizeT(size_t& sz) = 0;

    // For backward compatibility
private:
    unsigned version_;
public:

    /**
     * Changes the value of the version number to \a v.
     *
     * @note
     * The version number can be used to determine how objects should
     * be deserialized from the underlying stream. Attempts to
     * deserialize data that was serialized using a different version
     * cannot be expected to work.
     */
    void version(unsigned v) {
        version_ = v;
    }

    /**
     * Returns the value of the version number.
     *
     * @note
     * The default version number is the maximum supported version that
     * this stream can read.
     */
    unsigned version() const {
        return version_;
    }

    // constructor
    RWvistream() : version_(current_version) { }
};

/************************************************
 *                                              *
 *              class RWvostream                *
 *                                              *
 ************************************************/

/**
 * @ingroup virtual_stream_classes
 * @brief Abstract base class that provides an interface for format-dependent
 * storage of fundamental types and fundamental-type arrays.
 *
 * Class RWvostream is an abstract base class that provides an interface
 * for format-independent storage of fundamental types, and arrays
 * of fundamental types.  Its counterpart, RWvistream, provides
 * a complementary interface for the retrieval of variables of the
 * fundamental types.
 *
 * Because the interface of RWvistream and RWvostream is independent
 * of formatting, your application need not be concerned
 * with how variables are actually stored or restored -- functionality
 * that is the responsibility of whatever derived class you choose.
 * For instance, you could use an operating-system independent US-ASCII format
 * (classes RWpistream and RWpostream), a binary format (classes
 * RWbistream and RWbostream), or define your own format (such as an
 * interface to a network).
 *
 * Note that, because this is an abstract base class, these design goals cannot
 * be enforced; rather, the description here is merely the model of
 * how a class derived from RWvistream and RWvostream should act.
 *
 * @note
 * There is no need to separate variables with whitespace. It
 * is the responsibility of the derived class to delineate variables
 * with whitespace, packet breaks, or whatever might be appropriate
 * for the final output sink.  The intended model is one where variables
 * are inserted into the output stream, either individually or as
 * homogeneous vectors, to be restored in the same order using RWvistream.
 *
 * <b>About the Storage and Retrieval of Characters</b>
 * Characters can be thought of as either representing some alpha-numeric
 * or control character, or as the literal number.  Generally, the
 * overloaded insertion (<<) and extraction (>>) operators seek
 * to store and restore characters while preserving their symbolic meaning.
 * That is, storage of a newline should be restored as a newline,
 * regardless of its representation on the target machine.  By contrast,
 * member functions \link RWvistream::get() get() \endlink and put() should
 * treat the character as a literal number whose value is to be preserved.

 * @see
 * RWpostream.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/vstream.h>
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
 * #include <rw/vstream.h>
 * #include <rw/bstream.h>
 * #include <rw/pstream.h>
 *
 * void storeStuff(RWvostream& str)
 * {
 *     int i = 5;
 *     double d = 22.5;
 *     char s [] = "A string with \t tabs and a newline\n";
 *
 *     str << i; // Store an int
 *     str << d; // Store a double
 *     str << s; // Store a string
 *
 *     if (str.fail()) {
 *         std::cerr << "Storage failed.\n";
 *     }
 * }
 *
 * int main()
 * {
 *     std::ofstream vf("vfile.dat");
 *
 *     RWpostream ps(vf);
 *     RWbostream bs(vf);
 *
 *
 *     // Uncomment the call to storeStuff that matches your stream type
 *
 *     storeStuff(ps);
 *     // storeStuff(bs);
 *
 *     return 0;
 * }
 * @endcode
 */
class RW_TOOLS_GLOBAL RWvostream : public virtual RWvios
{
public:

    /**
     * Empty destructor.
     */
    virtual ~RWvostream();

    // Documented in base class.
    virtual int eof() = 0;

    // Documented in base class.
    virtual int fail() = 0;

    // Documented in base class.
    virtual int bad() = 0;

    // Documented in base class.
    virtual int good() = 0;

    // Documented in base class.
    virtual int rdstate() = 0;

    // Documented in base class.
    virtual void clear(int v = 0) = 0;

    // Documented in base class.
    operator void* () {
        return RWvios::operator void * ();
    }

    /**
     * Stores the \c char \a c to the output stream, preserving its value.
     */
    virtual RWvostream& put(char c) = 0;

    /**
     * Stores the \c char \a c to the output stream, preserving its meaning. \a c is
     * treated as a character.
     */
    virtual RWvostream& putChar(char c) = 0;

    /**
     * Stores the <tt>signed char</tt> \a c to the output stream, preserving its value.
     */
    virtual RWvostream& put(signed char c) = 0;

    /**
     * Stores the <tt>signed char</tt> \a c to the output stream, preserving its meaning.
     * \a c is treated as a character.
     */
    virtual RWvostream& putChar(signed char c) = 0;

    /**
     * Stores the <tt>unsigned char</tt> \a c to the output stream, preserving its value.
     */
    virtual RWvostream& put(unsigned char c) = 0;

    /**
     * Stores the <tt>unsigned char</tt> \a c to the output stream, preserving its meaning.
     * \a c is treated as a character.
     */
    virtual RWvostream& putChar(unsigned char c) = 0;

    /**
     * Stores the \c wchar_t \a wc to the output stream, preserving its
     * value.
     */
    virtual RWvostream& put(wchar_t wc) = 0;

    /**
     * Stores the \c wchar_t \a wc to the output stream, preserving its
     * value. \a wc is treated as a character.
     */
    virtual RWvostream& putChar(wchar_t wc) = 0;

    /**
     * Stores the \c bool \a b to the output stream.
     */
    virtual RWvostream& put(bool b) = 0;

    /**
     * Stores the \c short \a i to the output stream.
     */
    virtual RWvostream& put(short i) = 0;

    /**
    * Stores the <tt>unsigned short</tt> \a i to the output stream.
    */
    virtual RWvostream& put(unsigned short i) = 0;

    /**
     * Stores the \c int \a i to the output stream.
     */
    virtual RWvostream& put(int i) = 0;

    /**
     * Stores the <tt>unsigned int</tt> \a i to the output stream.
     */
    virtual RWvostream& put(unsigned int i) = 0;

    /**
     * Stores the \c long \a i to the output stream.
     */
    virtual RWvostream& put(long i) = 0;

    /**
     * Stores the <tt>unsigned long</tt> \a i to the output stream.
     */
    virtual RWvostream& put(unsigned long i) = 0;

#if !defined(RW_NO_LONG_LONG)
    /**
     * Stores the <tt>long long</tt> \a i to the output stream.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long long</tt> type.
     */
    virtual RWvostream& put(long long i) = 0;

    /**
     * Stores the <tt>unsigned long long</tt> \a i to the output stream.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>unsigned long long</tt> type.
     */
    virtual RWvostream& put(unsigned long long i) = 0;
#endif

    /**
     * Stores the \c float \a f to the output stream.
     */
    virtual RWvostream& put(float f) = 0;

    /**
     * Stores the \c double \a d to the output stream.
     */
    virtual RWvostream& put(double d) = 0;

#if !defined(RW_NO_LONG_DOUBLE)
    /**
     * Stores the <tt>long double</tt> \a d to the output stream.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long double</tt> type.
     */
    virtual RWvostream& put(long double d) = 0;
#endif

    /**
     * Stores the vector of \a n \c char starting at \a v to the output
     * stream. The characters should be treated as literal numbers (i.e.,
     * not as a character string).
     */
    virtual RWvostream& put(const char* v, size_t n) = 0;

    /**
     * Stores the vector of \a n <tt>signed char</tt> starting at \a v to the
     * output stream. The characters should be treated as literal numbers (i.e.,
     * not as a character string).
     */
    virtual RWvostream& put(const signed char* v, size_t n) = 0;

    /**
     * Stores the vector of \a n <tt>unsigned char</tt> starting at \a v to
     * the output stream. The characters should be treated as literal numbers (i.e.,
     * not as a character string).
     */
    virtual RWvostream& put(const unsigned char* v, size_t n) = 0;

    /**
     * Stores the vector of \a n \c wchar_t starting at \a v to the output
     * stream. The characters should be treated as literal numbers (i.e.,
     * not as a character string).
     */
    virtual RWvostream& put(const wchar_t* v, size_t n) = 0;

    /**
     * Stores the vector of \a n \c bool starting at \a v to the output
     * stream.
     */
    virtual RWvostream& put(const bool* v, size_t n) = 0;

    /**
     * Stores the vector of \a n \c short starting at \a v to the output
     * stream.
     */
    virtual RWvostream& put(const short* v, size_t n) = 0;

    /**
     * Stores the vector of \a n <tt>unsigned short</tt> starting at
     * \a v to the output stream.
     */
    virtual RWvostream& put(const unsigned short* v, size_t n) = 0;

    /**
     * Stores the vector of \a n \c int starting at \a v to the output
     * stream.
     */
    virtual RWvostream& put(const int* v, size_t n) = 0;

    /**
     * Stores the vector of \a n <tt>unsigned int</tt> starting at \a v
     * to the output stream.
     */
    virtual RWvostream& put(const unsigned int* v, size_t n) = 0;

    /**
     * Stores the vector of \a n \c long starting at \a v to the output
     * stream.
     */
    virtual RWvostream& put(const long* v, size_t n) = 0;

    /**
     * Stores the vector of \a n <tt>unsigned long</tt> starting at \a v to
     * the output stream.
     */
    virtual RWvostream& put(const unsigned long* v, size_t n) = 0;

#if !defined(RW_NO_LONG_LONG)
    /**
     * Stores the vector of \a n <tt>long long</tt> starting at \a v to
     * the output stream.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long long</tt> type.
     */
    virtual RWvostream& put(const long long* v, size_t n) = 0;

    /**
     * Stores the vector of \a n <tt>unsigned long long</tt> starting at
     * \a v to the output stream.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>unsigned long long</tt> type.
     */
    virtual RWvostream& put(const unsigned long long* v, size_t n) = 0;
#endif

    /**
     * Stores the vector of \a n \c float starting at \a v to the output
     * stream.
     */
    virtual RWvostream& put(const float* v, size_t n) = 0;

    /**
     * Stores the vector of \a n \c double starting at \a v to the output
     * stream.
     */
    virtual RWvostream& put(const double* v, size_t n) = 0;

#if !defined(RW_NO_LONG_DOUBLE)
    /**
     * Stores the vector of \a n <tt>long double</tt> starting at \a v
     * to the output stream.
     *
     * @note
     * This operator function is available only if your compiler
     * supports the <tt>long double</tt> type.
     */
    virtual RWvostream& put(const long double* v, size_t n) = 0;
#endif

    /**
     * Stores the character string, <i>including embedded nulls</i>, starting
     * at \a s to the output stream.
     */
    virtual RWvostream& putString(const char* s, size_t n) = 0; // Treat as string

    /**
     * Stores \a n \c char starting at \a s to the output stream. \a s is
     * treated as a character string.
     *
     * @note
     * This function delegates to put(const char*, size_t) if not overridden.
     */
    virtual RWvostream& putChars(const char* s, size_t n) {
        //
        // We provide a default implementation that calls put() to avoid forcing
        // the user to provide an implementation. This default breaks behavior for
        // user-provided stream classes when serializing RWCString.
        //
        return put(s, n);
    }

    /**
     * Stores the \c size_t \a sz to the output stream.
     */
    virtual RWvostream& putSizeT(size_t sz) = 0;

    /**
     * Sends the contents of the stream buffer to output immediately.
     */
    virtual RWvostream& flush() = 0;

#if defined(_MSC_VER)
    virtual RWvostream& vflush() {
        return this->flush();
    }
#endif

    // For backward compatibility
private:
    unsigned version_;
public:

    /**
     * Changes the value of the version number to \a v.
     *
     * @note
     * The version number may be used to determine how objects should
     * be serialized to the underlying stream. This can be used to ensure
     * compatibility with existing code or reduce the amount of data put
     * on the underlying output stream.
     */
    void version(unsigned v) {
        version_ = v;
    }

    /**
     * Returns the value of the version number.
     *
     * @note
     * The default version number is the maximum supported version that
     * this stream can write.
     */
    unsigned version() const {
        return version_;
    }

    // constructor
    RWvostream() : version_(current_version) { }
};

/**
 * @ingroup virtual_stream_classes
 *
 * @brief Input iterator that reads successive elements from the input stream
 * for which it was constructed.
 *
 * Class RWvistreamIterator is an input iterator that reads (using \c operator>>)
 * successive elements from the input stream for which it was constructed. After
 * it is constructed, and every time operator++() or operator++(int) is used,
 * the iterator reads and stores a value of \c T.

 * The constructor with no arguments always constructs and end-of-stream
 * input iterator object, which is the only legitimate iterator to be used
 * for the end condition. The result of operator*() on an end-of-stream
 * iterator is undefined. For any other iterator value, a <tt>const T&</tt>
 * is returned. The result of operator->() on an end-of stream iterator is
 * not defined. For any other iterator value, a <tt>const T*</tt> is returned.
 * The behavior of a program that applies operator++() to an end-of-stream
 * iterator is undefined. It is impossible to store things into RWvistream
 * iterators.
 */
template <typename T>
class RWvistreamIterator :
    public std::iterator<std::input_iterator_tag, T, int, const T*, const T&>
{
public:

    /**
     * Constructs an end-of-stream iterator.
     */
    RWvistreamIterator()
        : in_stream(0) {
    }

    /**
     * Constructs an iterator that will use \a s as the source for all
     * extraction operations. Extracts a single value of type \c T from
     * the stream \p s. If the extraction fails, this iterator becomes
     * an end-of-stream iterator, otherwise this iterator becomes a
     * non-end-of-stream iterator.
     */
    RWvistreamIterator(RWvistream& s)
        : in_stream(&s) {
        RWvistreamIterator::operator++();
    }

    /**
     * Returns a reference to the last value read from the underlying stream.
     */
    const T& operator*() const {
        return value;
    }

    /**
     * Returns a pointer to the last value read from the underlying stream.
     */
    const T* operator->() const {
        return &value;
    }

    /**
     * Extracts an item from the underlying stream and returns a reference
     * to self. If the extraction operation fails, this iterator becomes
     * an end-of-stream iterator.
     */
    RWvistreamIterator& operator++() {
        RW_ASSERT(in_stream != 0);
        if ((*in_stream >> value).fail()) {
            in_stream = 0;
        }
        return *this;
    }

    RW_SUPPRESS_UNUSED_WARNING

    /**
     * Makes a copy of self, extracts an item from the underlying stream and,
     * returns the copy that was made before the extraction. If the extract
     * operation fails, this iterator becomes an end-of-stream iterator.
     */
    RWvistreamIterator operator++(int) {
        RWvistreamIterator tmp = *this;
        RWvistreamIterator::operator++();
        return (tmp);
    }

    RW_RESTORE_UNUSED_WARNING

    /**
     * @relates RWvistreamIterator
     *
     * Returns \c true if \a lhs is equal to \a rhs, otherwise \c false.
     *
     * Two end-of-stream iterators are always equal. An end-of-stream iterator
     * is not equal to a non-end-of-stream iterator. Two non-end-of-stream
     * iterators are equal when they are associated with the same stream.
     */
    friend bool operator==(const RWvistreamIterator<T>& lhs, const RWvistreamIterator<T>& rhs) {
        return lhs.in_stream == rhs.in_stream;
    }

    /**
     * @relates RWvistreamIterator
     *
     * Returns \c true if \a lhs is not equal to \a rhs, otherwise \c false.
     *
     * Two end-of-stream iterators are always equal. An end-of-stream iterator
     * is not equal to a non-end-of-stream iterator. Two non-end-of-stream
     * iterators are equal when they are associated with the same stream.
     */
    friend bool operator!=(const RWvistreamIterator<T>& lhs, const RWvistreamIterator<T>& rhs) {
        return lhs.in_stream != rhs.in_stream;
    }

private:
    RWvistream* in_stream;
    T value;
};

/**
 * @ingroup virtual_stream_classes
 *
 * @brief Writes successive elements onto the output stream from which
 * it was constructed.
 *
 * Class RWvostreamIterator writes (using \c operator<<) successive
 * elements onto the output stream from which it was constructed. It
 * is not possible to get a value out of the output iterator. Its only
 * use is as an output iterator in situations like the following.
 *
 * @code
 * RWvostreamIterator<int> result(vos);
 * while (first != last) {
 *   *result++ = *first++;
 * }
 * @endcode
 */
template <typename T>
class RWvostreamIterator :
    public std::iterator<std::output_iterator_tag, void, void, void, void>
{
public:

    /**
     * Constructs an iterator that will insert data to the stream \a s.
     */
    RWvostreamIterator(RWvostream& s)
        : out_stream(&s) {
    }

    /**
     * Inserts \a value onto the underlying stream and Returns a
     * reference to self.
     */
    RWvostreamIterator& operator=(const T& value) {
        *out_stream << value;
        return *this;
    }

    /**
     * Returns a reference to self.
     */
    RWvostreamIterator& operator*() {
        return *this;
    }

    /**
     * Returns a reference to self.
     */
    RWvostreamIterator& operator++() {
        return *this;
    }

    /**
     * Returns a reference to self.
     */
    RWvostreamIterator& operator++(int) {
        return *this;
    }

private:
    RWvostream* out_stream;
};

/**
 * Stores the null-terminated character string starting at \a s to the
 * output stream.
 *
 * @relates RWvostream
 */
inline RWvostream& operator<<(RWvostream& os, const char* s)
{
    RW_PRECONDITION2(s != 0,
                     "operator<<(RWvostream& os, const char *s): "
                     "const char* param is nil");

    return os.putString(s, strlen(s));
}

/**
 * Stores the \c char \a c to the output stream.
 *
 * @relates RWvostream
 */
inline RWvostream& operator<<(RWvostream& os, char c)
{
    return os.putChar(c);
}

/**
 * Stores the <tt>signed char</tt> \a c to the output stream.
 *
 * @relates RWvostream
 */
inline RWvostream& operator<<(RWvostream& os, signed char c)
{
    return os.putChar(c);
}

/**
 * Stores the <tt>unsigned char</tt> \a c to the output stream.
 *
 * @relates RWvostream
 */
inline RWvostream& operator<<(RWvostream& os, unsigned char c)
{
    return os.putChar(c);
}

/**
 * Stores the \c wchar_t \a wc to the output stream.
 *
 * @relates RWvostream
 */
inline RWvostream& operator<<(RWvostream& os, wchar_t wc)
{
    return os.putChar(wc);
}

#if defined(GDA_RW_STREAM_BOOL_AS_BOOL)
/**
 * Stores the \c bool \a b to the output stream.
 *
 * @relates RWvostream
 */
inline RWvostream& operator<<(RWvostream& os, bool b)
{
    return os.put(b);
}
#endif

#if !defined(RW_NO_LONG_LONG)
/**
 * Stores the <tt>long long</tt> \a i to the output stream.
 *
 * @relates RWvostream
 *
 * @note
 * This operator function is available only if your compiler
 * supports the <tt>long long</tt> type.
 */
inline RWvostream& operator<<(RWvostream& os, long long i)
{
    return os.put(i);
}

/**
 * Stores the <tt>unsigned long long</tt> \a i to the output stream.
 *
 * @relates RWvostream
 *
 * @note
 * This operator function is available only if your compiler
 * supports the <tt>unsigned long long</tt> type.
 */
inline RWvostream& operator<<(RWvostream& os, unsigned long long i)
{
    return os.put(i);
}
#endif

/**
 * Stores the \c float \a f to the output stream.
 *
 * @relates RWvostream
 */
inline RWvostream& operator<<(RWvostream& os, float f)
{
    return os.put(f);
}

/**
 * Stores the \c double \a d to the output stream.
 *
 * @relates RWvostream
 */
inline RWvostream& operator<<(RWvostream& os, double d)
{
    return os.put(d);
}

#if !defined(RW_NO_LONG_DOUBLE)
/**
 * Stores the <tt>long double</tt> \a d to the output stream.
 *
 * @relates RWvostream
 *
 * @note
 * This operator function is available only if your
 * compiler supports the <tt>long double</tt> type.
 */
inline RWvostream& operator<<(RWvostream& os, long double d)
{
    return os.put(d);
}
#endif

/**
 * Stores the \c short \a i to the output stream.
 *
 * @relates RWvostream
 */
inline RWvostream& operator<<(RWvostream& os, short i)
{
    return os.put(i);
}

/**
 * Stores the <tt>unsigned short</tt> \a i to the output stream.
 *
 * @relates RWvostream
 */
inline RWvostream& operator<<(RWvostream& os, unsigned short i)
{
    return os.put(i);
}

/**
 * Stores the \c int \a i to the output stream.
 *
 * @relates RWvostream
 */
inline RWvostream& operator<<(RWvostream& os, int i)
{
    return os.put(i);
}

/**
 * Stores the <tt>unsigned int</tt> \a i to the output stream.
 *
 * @relates RWvostream
 */
inline RWvostream& operator<<(RWvostream& os, unsigned int i)
{
    return os.put(i);
}

/**
 * Stores the \c long \a i to the output stream.
 *
 * @relates RWvostream
 */
inline RWvostream& operator<<(RWvostream& os, long i)
{
    return os.put(i);
}

/**
 * Stores the <tt>unsigned long</tt> \a i to the output stream.
 *
 * @relates RWvostream
 */
inline RWvostream& operator<<(RWvostream& os, unsigned long i)
{
    return os.put(i);
}

// If the following function were a member function of RWvostream, rather
// than a global function, classes derived from RWvostream would not
// be able to redefine other oper<< functions, without hiding this one, or
// redefining this one also.
inline RWvostream& operator<<(RWvostream& v, RWvostream & (*f)(RWvostream&))
{
    return (*f)(v);
}

RW_TOOLS_SYMBOLIC RWvostream& flush(RWvostream&);


/**
 * Gets the next \c char from the input stream and stores it in \a c.
 *
 * @relates RWvistream
 */
inline RWvistream& operator>>(RWvistream& is, char& c)
{
    return is.getChar(c);
}

/**
 * Gets the next <tt>signed char</tt> from the input stream and
 * stores it in \a c.
 *
 * @relates RWvistream
 */
inline RWvistream& operator>>(RWvistream& is, signed char& c)
{
    return is.getChar(c);
}

/**
 * Gets the next <tt>unsigned char</tt> from the input stream and
 * stores it in \a c.
 *
 * @relates RWvistream
 */
inline RWvistream& operator>>(RWvistream& is, unsigned char& c)
{
    return is.getChar(c);
}

/**
 * Gets the next \c wchar_t from the input stream and stores it in
 * \a wc.
 *
 * @relates RWvistream
 */
inline RWvistream& operator>>(RWvistream& is, wchar_t& wc)
{
    return is.getChar(wc);
}

/**
 * Gets the next \c bool from the input stream and stores it in
 * \a b.
 *
 * @relates RWvistream
 */
#if defined(GDA_RW_STREAM_BOOL_AS_BOOL)
inline RWvistream& operator>>(RWvistream& is, bool& b)
{
    return is.get(b);
}
#endif

/**
 * Gets the next \c short from the input stream and stores it in
 * \a i.
 *
 * @relates RWvistream
 */
inline RWvistream& operator>>(RWvistream& is, short& i)
{
    return is.get(i);
}

/**
 * Gets the next <tt>unsigned short</tt> from the input stream and
 * stores it in \a i.
 *
 * @relates RWvistream
 */
inline RWvistream& operator>>(RWvistream& is, unsigned short& i)
{
    return is.get(i);
}

/**
 * Gets the next \c int from the input stream and stores it in
 * \a i.
 *
 * @relates RWvistream
 */
inline RWvistream& operator>>(RWvistream& is, int& i)
{
    return is.get(i);
}

/**
 * Gets the next <tt>unsigned int</tt> from the input stream and
 * stores it in \a i.
 *
 * @relates RWvistream
 */
inline RWvistream& operator>>(RWvistream& is, unsigned int& i)
{
    return is.get(i);
}

/**
 * Gets the next \c long from the input stream and stores it in
 * \a i.
 *
 * @relates RWvistream
 */
inline RWvistream& operator>>(RWvistream& is, long& i)
{
    return is.get(i);
}

/**
 * Gets the next <tt>unsigned long</tt> from the input stream and
 * stores it in \a i.
 *
 * @relates RWvistream
 */
inline RWvistream& operator>>(RWvistream& is, unsigned long& i)
{
    return is.get(i);
}

#if !defined(RW_NO_LONG_LONG)
/**
 * Gets the next <tt>long long</tt> from the input stream and stores it
 * in \a i.
 *
 * @relates RWvistream
 *
 * @note
 * This operator function is available only if your compiler
 * supports the <tt>long long</tt> type.
 */
inline RWvistream& operator>>(RWvistream& is, long long& i)
{
    return is.get(i);
}

/**
 * Gets the next <tt>unsigned long long</tt> from the input stream
 * and stores it in \a i.
 *
 * @relates RWvistream
 *
 * @note
 * This operator function is available only if your compiler
 * supports the <tt>unsigned long long</tt> type.
 */
inline RWvistream& operator>>(RWvistream& is, unsigned long long& i)
{
    return is.get(i);
}
#endif

/**
 * Gets the next \c float from the input stream and stores it in
 * \a f.
 *
 * @relates RWvistream
 */
inline RWvistream& operator>>(RWvistream& is, float& f)
{
    return is.get(f);
}

/**
 * Gets the next \c double from the input stream and stores it in
 * \a d.
 *
 * @relates RWvistream
 */
inline RWvistream& operator>>(RWvistream& is, double& d)
{
    return is.get(d);
}

#if !defined(RW_NO_LONG_DOUBLE)
/**
 * Gets the next <tt>long double</tt> from the input stream and stores
 * it in \a d.
 *
 * @relates RWvistream
 *
 * @note
 * This operator function is available only if your compiler
 * supports the <tt>long double</tt> type.
 */
inline RWvistream& operator>>(RWvistream& is, long double& d)
{
    return is.get(d);
}
#endif

// The following are used for simple External Template Persistence (ETP)
// for builtin types.
// They are not virtual functions in RW{p,b,e}{i,o}stream in order to
// maintain link compatibility

RW_TOOLS_SYMBOLIC RWvistream& operator>>(RWvistream& s, char*& ip);
RW_TOOLS_SYMBOLIC RWvistream& operator>>(RWvistream& s, signed char*& ip);
RW_TOOLS_SYMBOLIC RWvistream& operator>>(RWvistream& s, unsigned char*& ip);
RW_TOOLS_SYMBOLIC RWvistream& operator>>(RWvistream& s, wchar_t*& ip);
RW_TOOLS_SYMBOLIC RWvistream& operator>>(RWvistream& s, bool*& ip);

#if !defined(RW_NO_LONG_LONG)
RW_TOOLS_SYMBOLIC RWvistream& operator>>(RWvistream& s, long long*& ip);
RW_TOOLS_SYMBOLIC RWvistream& operator>>(RWvistream& s, unsigned long long*& ip);
#endif

#if !defined(RW_NO_LONG_DOUBLE)
RW_TOOLS_SYMBOLIC RWvistream& operator>>(RWvistream& s, long double*& ip);
#endif

RW_TOOLS_SYMBOLIC RWvistream& operator>>(RWvistream& s, double*& ip);
RW_TOOLS_SYMBOLIC RWvistream& operator>>(RWvistream& s, float*& ip);
RW_TOOLS_SYMBOLIC RWvistream& operator>>(RWvistream& s, int*& ip);
RW_TOOLS_SYMBOLIC RWvistream& operator>>(RWvistream& s, long*& ip);
RW_TOOLS_SYMBOLIC RWvistream& operator>>(RWvistream& s, short*& ip);
RW_TOOLS_SYMBOLIC RWvistream& operator>>(RWvistream& s, unsigned int*& ip);
RW_TOOLS_SYMBOLIC RWvistream& operator>>(RWvistream& s, unsigned long*& ip);
RW_TOOLS_SYMBOLIC RWvistream& operator>>(RWvistream& s, unsigned short*& ip);

#endif /* RW_TOOLS_VSTREAM_H */
