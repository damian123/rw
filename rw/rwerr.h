#ifndef RW_TOOLS_ERR_H
#define RW_TOOLS_ERR_H


/**********************************************************************
 *
 * Error definitions
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/rwerr.h#1 $
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
#include <stdio.h>      /* Looking for FILE */


class RW_TOOLS_SYMBOLIC RWMessage;

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                               RWxmsg                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/**
 * @ingroup miscellaneous_classes
 *
 * @brief Base class for the hierarchy of exceptions
 * in the Essential Tools Module
 *
 * This class is the base class for the hierarchy of exceptions
 * in the Essential Tools Module. (See the section on error reporting and
 * handling in the <i>Essential Tools Module</i> User's Guide.) Although
 * the entire class is documented here, your code usually should not create or
 * directly throw RWxmsg objects. Instead, you should expect to
 * write code that may catch an RWxmsg or one of its derived classes.
 *
 * Essential Tools Module classes that need to report exceptional
 * circumstances call RWTHROW(). RWTHROW() throws
 * the RWxmsg if exceptions are enabled. Otherwise, RWTHROW()
 * calls an appropriate current Essential Tools Module
 * error handler function (see the section on error reporting and
 * handling in the <i>Essential Tools Module</i> User's Guide).
 *
 * RWxmsg provides a basic interface for exceptions, from which
 * an exception string is always available. This exception string
 * is obtained through the why() method, and is represented as
 * a <tt>const char*</tt>, single-byte character sequence.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/rwerr.h>
 * RWxmsg x("exception message");
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWxmsg
{
public:
    /**
     * Creates an exception that returns \a msg when its why() method
     * is called.
     */
    RWxmsg(const char* msg);

    /**
     * @internal
     * Creates an exception with a message from the localizable \c RWMessage
     * class instance \a msg. (See the section on messages
     * in the <i>Essential Tools Module User's Guide</i>).
     */
    RWxmsg(const RWMessage& msg);

    /**
     * Copy constructor.
     */
    RWxmsg(const RWxmsg& msg);

    virtual ~RWxmsg();

    /**
     * Returns the message string contained in self.
     */
    virtual const char* why() const;

    /**
     * Calls RWTHROW(), passing self as a parameter.
     */
    virtual void raise() const;

    /**
     * Assignment operator.
     */
    RWxmsg& operator= (const RWxmsg&);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed instance takes ownership of the
     * data owned by \a msg.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWxmsg(RWxmsg && msg);

    /**
     * Move assignment. Self takes ownership of the data owned by \a msg.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWxmsg& operator= (RWxmsg && msg);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWxmsg& rhs);

protected:

    /**
     * Creates an exception that returns \a msg when its why() method
     * is called. If \a doCopy is \c true, copies \a msg into a new buffer
     * on the heap, otherwise simply keeps a pointer to the given location
     * of \a msg.
     */
    RWxmsg(const char* msg, bool doCopy);

private:
    void makeCopy(const char* str);

    char*     msg_;
    bool owner_;
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                               RWxalloc                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

// No host-supplied xalloc; we must supply it:
/**
 * @ingroup miscellaneous_classes
 * @brief Exception class that reports buffer allocation errors.
 *
 * RWxalloc is used to report buffer allocation errors.
 */
class RW_TOOLS_GLOBAL RWxalloc : public RWxmsg
{
public:

    RWxalloc(size_t size);
    RWxalloc(const RWxalloc& msg);
    RWxalloc& operator= (const RWxalloc& msg);

#if !defined(RW_NO_RVALUE_REFERENCES)
    RWxalloc(RWxalloc && msg);
    RWxalloc& operator= (RWxalloc && msg);
#endif // !RW_NO_RVALUE_REFERENCES

    virtual ~RWxalloc();

    size_t requested() const {
        return size_;
    }

    void swap(RWxalloc& rhs);

private:
    size_t size_;
};


//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                            RWExternalErr                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

// For errors external to the library;
// these are errors the prudent programmer should expect,
// such as out-of-disk-space, file errors, and the like:
/**
 * @ingroup miscellaneous_classes
 * @brief Exception class that reports errors caused
 * by external sources over which the library has no control.
 *
 * RWExternalErr is derived from RWxmsg, and is used to report errors caused
 * by external sources over which the library has no control. Currently, this
 * class is used to report errors from RWFileManager and RWCollectable
 * during file processing when an invalid file format is encountered.
 */
class RW_TOOLS_GLOBAL RWExternalErr : public RWxmsg
{
public:

    RWExternalErr(const char* msg);
    RWExternalErr(const RWMessage& msg);
    RWExternalErr(const RWExternalErr& msg);
    RWExternalErr& operator=(const RWExternalErr& msg);

#if !defined(RW_NO_RVALUE_REFERENCES)
    RWExternalErr(RWExternalErr && msg);
    RWExternalErr& operator=(RWExternalErr && msg);
#endif // !RW_NO_RVALUE_REFERENCES

    virtual ~RWExternalErr();
    void swap(RWExternalErr& rhs);
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                              RWFileErr                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

// For errors involving a file descriptor
/**
 * @ingroup miscellaneous_classes
 * @brief Exception class that reports the failure of file IO operations.
 *
 * RWFileErr is a type of external error (i.e. non-SourcePro) reported when
 * file IO operations fail. For example, RWBTreeOnDisk makes extensive use
 * of this class to report file \c read, \c seek, and \c write errors.
 */
class RW_TOOLS_GLOBAL RWFileErr : public RWExternalErr
{
public:

    enum errType {unknown, seekErr, readErr, writeErr, openErr, closeErr};
    RWFileErr(const char*     , FILE*, errType type);
    RWFileErr(const RWMessage&, FILE*, errType type);
    RWFileErr(const RWFileErr& msg);
    RWFileErr& operator=(const RWFileErr& msg);

#if !defined(RW_NO_RVALUE_REFERENCES)
    RWFileErr(RWFileErr && msg);
    RWFileErr& operator=(RWFileErr && msg);
#endif // !RW_NO_RVALUE_REFERENCES
    virtual ~RWFileErr();

    FILE*   descriptor() const {
        return fd_;
    }
    errType errorType() const {
        return type_;
    }

    void swap(RWFileErr& rhs);

private:
    FILE* fd_;
    errType type_;
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                             RWStreamErr                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

// For errors involving streams
/**
 * @ingroup miscellaneous_classes
 * @brief Exception class that reports either no header or an invalid
 * header in an RWeostream or RWeistream.
 *
 * RWStreamErr is an external error reported when invalid stream data is
 * encountered. For example, this class is used to report errors from
 * RWeistream due to invalid stream headers.
 */
class RW_TOOLS_GLOBAL RWStreamErr : public RWExternalErr
{
private:

    // not defined
    RWStreamErr& operator=(const RWStreamErr& rhs);

#if !defined(RW_NO_RVALUE_REFERENCES)
    RWStreamErr& operator=(RWStreamErr && rhs);
#endif // !RW_NO_RVALUE_REFERENCES

public:

    RWStreamErr(const char* msg, std::ios& stream);
    RWStreamErr(const RWMessage& msg, std::ios& stream);
    RWStreamErr(const RWStreamErr& msg);

#if !defined(RW_NO_RVALUE_REFERENCES)
    RWStreamErr(RWStreamErr && msg);
#endif // !RW_NO_RVALUE_REFERENCES

    virtual ~RWStreamErr();

    std::ios& errStream();

    const std::ios& errStream() const;

private:
    std::ios&    stream_;
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                             RWConversionErr                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/**
 * @ingroup miscellaneous_classes
 * @brief Exception class that reports character encoding conversion errors.
 *
 * An RWConversionErr instance is used to report character encoding
 * conversion errors produced by classes such as RWBasicUString.
 * The begin() and end() methods may be used to extract and report
 * the offending code unit sequence within the conversion source.
 * The value returned by the end() method may be used as an offset for
 * restarting the conversion at the first code unit following the bad
 * sequence.
 */
class RW_TOOLS_GLOBAL RWConversionErr : public RWExternalErr
{
public:
    RWConversionErr(const char* msg, size_t begin, size_t end);
    RWConversionErr(const RWMessage& msg, size_t begin, size_t end);

    RWConversionErr(const RWConversionErr& msg);
    RWConversionErr& operator=(const RWConversionErr& msg);

#if !defined(RW_NO_RVALUE_REFERENCES)
    RWConversionErr(RWConversionErr && msg);
    RWConversionErr& operator=(RWConversionErr && msg);
#endif // !RW_NO_RVALUE_REFERENCES

    virtual ~RWConversionErr();

    /**
     * Returns the index of the first code unit in the invalid,
     * irregular, or truncated encoding sequence.
     */
    size_t begin() const;

    /**
     * Returns the index of the last code unit in the invalid,
     * irregular, or truncated encoding sequence.
     */
    size_t end() const;

    void swap(RWConversionErr& rhs);

private:
    size_t begin_;
    size_t end_;
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                            RWInternalErr                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

// Logic errors are violated invariants, bounds errors,
// and the like.
/**
 * @ingroup miscellaneous_classes
 * @brief Exception class that reports errors that
 * occur within the Essential Tools Module
 *
 * RWInternalErr is a type of error that is used to report errors that
 * occur within the Essential Tools Module. For example, RWDateTime uses
 * this class to report operations on invalid date/time objects.
 */
class RW_TOOLS_GLOBAL RWInternalErr : public RWxmsg
{
public:

    RWInternalErr(const char* msg);
    RWInternalErr(const RWMessage& msg);
    RWInternalErr(const RWInternalErr& msg);
    RWInternalErr& operator=(const RWInternalErr& msg);

#if !defined(RW_NO_RVALUE_REFERENCES)
    RWInternalErr(RWInternalErr && msg);
    RWInternalErr& operator=(RWInternalErr && msg);
#endif // !RW_NO_RVALUE_REFERENCES

    virtual ~RWInternalErr();
    void swap(RWInternalErr& rhs);
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                             RWBoundsErr                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/**
 * @ingroup miscellaneous_classes
 * @brief Exception class that reports invalid indexes into buffers
 *
 * RWBoundsErr is used to report invalid indexes into buffers. For example,
 * an \link RWTPtrOrderedVector RWTPtrOrderedVector<T, A> \endlink may report
 * this type of error when an invalid index is provided to the
 * RWTPtrOrderedVector<T>::at(size_type) method.
 */
class RW_TOOLS_GLOBAL RWBoundsErr : public RWInternalErr
{
public:

    RWBoundsErr(const char* msg);
    RWBoundsErr(const RWMessage& msg);
    RWBoundsErr(const RWBoundsErr& msg);
    RWBoundsErr& operator=(const RWBoundsErr& msg);

#if !defined(RW_NO_RVALUE_REFERENCES)
    RWBoundsErr(RWBoundsErr && msg);
    RWBoundsErr& operator=(RWBoundsErr && msg);
#endif // !RW_NO_RVALUE_REFERENCES

    virtual ~RWBoundsErr();
    void swap(RWBoundsErr& rhs);
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                             RWRegexErr                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/**
 * @ingroup string_processing_classes
 * @brief Exception class that reports errors from within RWTRegex.
 *
 * Class RWRegexErr is an exception class used to report errors
 * from within RWTRegex.
 *
 * RWRegexErr is derived from RWInternalErr, and is therefore part
 * of the RWxmsg class hierarchy. Classes in this hierarchy are
 * used to report errors from within the Essential Tools Module.
 *
 * RWRegexErr customizes RWInternalErr to provide not only an error
 * message (inherited from RWxmsg), but also an error code and an
 * error offset.
 *
 * The error code is an RWTRegex<T>::RWTRegexStatus enumeration constant
 * represented as a \c size_t, and is useful in generating error
 * messages in the wide characters required for many non-Latin
 * languages.
 *
 * When RWTRegex generates an error, it creates an RWRegexErr
 * object to represent the error. A copy of this object is saved
 * in the RWTRegex object, and is available through a call to
 * RWTRegex::getStatus().
 *
 * Then, RWTHROW() is used to report the error. If exceptions
 * are enabled, the function throws the RWRegexErr object as
 * an exception.  If not, then a user-defined callback function
 * is invoked.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/rwerr.h>
 * RWRegexErr msg("exception message");
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * #include <rw/cstring.h>
 * #include <rw/locale.h>
 * #include <rw/rwerr.h>
 * #include <rw/cstring.h>
 * #include <rw/tools/regex.h>
 *
 * #include <iostream>
 *
 * int main()
 * {
 *   const RWCString unmatchedParen("ab)$");
 *
 *   try
 *   {
 *     // Unmatched closing parenthesis generates exception
 *     RWTRegex<char> r(unmatchedParen);
 *
 *     // Use the regular expression to avoid warnings
 *     r.index(unmatchedParen);
 *   }
 *   catch (const RWRegexErr& err)
 *   {
 *     std::cout << "Caught exception: "                    << std::endl
 *               << "Description     : " << err.why()       << std::endl
 *               << "Code            : " << err.getCode()   << std::endl
 *               << "Offset          : " << err.getOffset() << std::endl
 *               << unmatchedParen                          << std::endl
 *               << RWCString('-', err.getOffset()) << '^'  << std::endl;
 *   }
 *
 *   return 0;
 * }
 * @endcode
 */
class RW_TOOLS_GLOBAL RWRegexErr : public RWInternalErr
{
public:
    /**
     * Initializes the error object to contain the specified error
     * message, \a msg. The error code, \a eCode, is stored as well
     * as the offset, \a eOffset, into the pattern of the error.
     */
    RWRegexErr(const char* msg, size_t eCode, size_t eOffset);

    /**
     * @internal
     * Creates an RWRegexErr from an \c RWMessage.
     */
    RWRegexErr(const RWMessage& msg);

    /**
     * Copy constructor.
     */
    RWRegexErr(const RWRegexErr& msg);

    /**
     * Assignment operator.
     */
    RWRegexErr& operator= (const RWRegexErr&);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed instance takes ownership of the
     * data owned by \a msg.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWRegexErr(RWRegexErr && msg);

    /**
     * Move assignment. Self takes ownership of the data owned by \a msg.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWRegexErr& operator= (RWRegexErr && msg);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Destructor
     */
    virtual ~RWRegexErr();

    /**
     * Returns the error code for the error. The error code is an
     * RWTRegex<T>::RWTRegexStatus enumerator represented as a \c size_t.
     */
    size_t getCode() const;

    /**
     * Returns the offset into the pattern at which the error occurred.
     */
    size_t getOffset() const;

    /**
     * Swaps the data owned by self with the data owned by \a rhs.
     */
    void swap(RWRegexErr& rhs);

private:
    size_t code;
    size_t offset;
};


// calls rwErrRoutine when exceptions are not supported,
// and otherwise throws
extern RW_TOOLS_SYMBOLIC void RWThrow(const RWxmsg&);
extern RW_TOOLS_SYMBOLIC void RWThrow(const RWxalloc&);
extern RW_TOOLS_SYMBOLIC void RWThrow(const RWExternalErr&);
extern RW_TOOLS_SYMBOLIC void RWThrow(const RWFileErr&);
extern RW_TOOLS_SYMBOLIC void RWThrow(const RWStreamErr&);
extern RW_TOOLS_SYMBOLIC void RWThrow(const RWInternalErr&);
extern RW_TOOLS_SYMBOLIC void RWThrow(const RWBoundsErr&);
extern RW_TOOLS_SYMBOLIC void RWThrow(const RWRegexErr&);
extern RW_TOOLS_SYMBOLIC void RWThrow(const RWConversionErr&);

typedef void (*rwErrHandler)(const RWxmsg&);

RW_TOOLS_SYMBOLIC rwErrHandler rwSetErrHandler(rwErrHandler);

template <class T>
void rwthrow(const T& err)
{
#if defined(RW_NO_EXCEPTIONS) || (defined(RW_NO_THROW_WITH_SHARED) && defined(RWBUILDSHARED))
    (*rwErrRoutine)(err);
#else
    throw(T&) err;
#endif //defined(RW_NO_EXCEPTIONS) || (defined(RW_NO_THROW_WITH_SHARED) && defined(RWBUILDSHARED))
}


// All exceptions are thrown by the library code itself so that
// if exceptions are changed from that of the library, both application
// code and library code throw exceptions identically.

// Use extra parens to skirt MS C7 bug as documented in MS Problem ID: C9301003

/**
 * @relates RWxmsg
 *
 * RWTHROW() throws the exception \a a if exceptions are enabled.
 * Otherwise, RWTHROW() invokes the error handler callback function
 * set by the last call to rwSetErrHandler() or the default callback if
 * one has not been explicitly set.
 */
#define RWTHROW(a) (RWThrow(a))


#endif /* RW_TOOLS_ERR_H */
