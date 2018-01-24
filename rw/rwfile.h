#ifndef RW_TOOLS_FILE_H
#define RW_TOOLS_FILE_H


/**********************************************************************
 *
 * Class RWFile encapsulates ANSI-C binary file operations.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/rwfile.h#1 $
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

//
// RWFile manipulates files in binary mode (see docs).
//

#include <stdio.h>

#include <rw/defs.h>
#include <rw/tools/pointer.h>

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

/**
 * @ingroup file_system_classes
 *
 * @brief Represents an abstraction of a filesystem regular file
 *
 * Class RWFile represents an abstraction of a filesystem regular
 * file. The file is manipulated in binary mode by default. This
 * class' interface is based on class \e PFile of the Interviews Class
 * Library (1987, Stanford University). The member function names
 * begin with upper case letters in order to maintain compatibility
 * with class PFile.
 *
 * If a function expects an RWFile to exist, your code should always
 * check isValid() prior to calling that function. Failure to do
 * so may cause a runtime error.  For example:
 *
 * @code
 * RWFile aFile("filename");
 * //...
 * if(aFile.isValid() && ! aFile.Error()){
 * //...
 * @endcode
 *
 * Note that RWFile cannot anticipate or define the behavior of
 * a program that calls any member function other than isValid()
 * on an invalid RWFile object.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/rwfile.h>
 * RWFile f("filename");
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWFile
{
    RWFile(const RWFile&);

    void operator=(const RWFile&);

public:

    /**
     * Constructs an RWFile to be used with the file name \a filename
     * and with mode \a mode. The mode string conforms to POSIX \c fopen
     * modes (see
     * http://www.opengroup.org/onlinepubs/009695399/functions/fopen.html).
     *
     * If \a mode is zero (the default), then the constructor attempts
     * to open an existing file with the given \a filename for update
     * (\c fopen mode \c "rb+"). If this is not possible, then it
     * attempts to create a new file with the given \a filename (\c fopen
     * mode \c "wb+"). The resulting object should be checked for validity
     * using the function isValid().
     *
     * The last parameter \a large_file is used to determine if the file
     * should be opened in large file mode. On systems that are large
     * file enabled, this parameter is ignored, and the file is always
     * opened in large file mode. The default is \c false for compatibility
     * with older versions of SourcePro, meaning that the file is not opened
     * in large file mode.  For newer versions and applications, we recommend
     * that you pass a value of \c true for the \a large_file parameter,
     * and that all file positioning calls be done with the large file safe
     * functions SeekToBegin(), SeekToEnd(), SeekTo64(), and CurOffset64().
     *
     * @note
     * Because this class is intended to encapsulate binary operations,
     * it is important that it open or create files in binary mode. If a file
     * mode is not specified, or is a \c NULL pointer, the file is opened in
     * binary mode. Using binary mode is particularly important on systems
     * where text mode file streams and binary streams are different.
     */
    RWFile(const char* filename,
           const char* mode = 0,
           bool large_file = false);

    /**
     * Performs any pending I/O operations and closes the file.
     */
    ~RWFile();

    /**
     * Returns a non-null \c FILE pointer for a valid file. Provided
     * for users who need to perform implementation-dependent operations
     * on \c FILE. Any changes in the state of the \c FILE object may
     * render the behavior of the corresponding RWFile object undefined.
     */
    FILE* GetStream() const;

    /**
     * Repositions the file pointer to \a offset bytes from the start
     * of the file. Can be used with both small and very large (more
     * than 2GB) files. Returns \c true if the operation is successful.
     * Returns \c -1 on error or if the requested seek offset is
     * outside the range supported by the underlying file.
     */
    bool SeekTo(RWoffset offset);
    /**
     * @copydoc SeekTo(RWoffset)
     */
    bool SeekTo64(RWoffset64 offset);

    /**
     * Returns the current position, in bytes from the start of the
     * file,  as a 32-bit integer. Returns \c -1 on error or if
     * the file position is beyond the representable range of #RWoffset.
     * May be used with both small and very large (more than 2GB) files.
     */
    RWoffset   CurOffset() const;

    /**
     * Returns the current offset, in bytes from the start of the file,
     * as a 64-bit integer. May be used with both small and very large
     * (more than 2GB) files.
     */
    RWoffset64 CurOffset64() const;

    /**
     * Repositions the file pointer to the end of the file.  Returns
     * \c true if the operation is successful.
     */
    bool SeekToEnd();

    /**
     * Repositions the file pointer to the start of the file.  Returns
     * \c true if the operation is successful.
     */
    bool SeekToBegin();

    /**
     * Erases the contents but does not close the file.  Returns \c
     * true if the operation was successful.
     */
    bool Erase();

    /**
     * Performs any pending I/O operations.  Returns \c true if successful.
     */
    bool Flush();

    /**
     * Resets error state so that neither Eof() nor Error() returns \c
     * true.
     */
    void ClearErr();

    /**********************************************************************/

    /**
     * Returns the file name.
     */
    const char* GetName() const {
        return name_.get();
    }

    /**
     * Returns the access mode with which the underlying \c FILE* was
     * opened.
     */
    const char* Access() const;

    /**
     * Returns \c true if the file was successfully opened, otherwise
     * \c false.
     */
    bool isValid() const;

    /**
     * Returns \c true if an end-of-file has been reached.
     */
    bool Eof() const;

    /**
     * Returns \c true if a file I/O error has occurred. Before calling
     * Error() you should call isValid() to determine if the file
     * was opened/created successfully. Failure to do so could result
     * in runtime errors.
     */
    bool Error() const;

    /**
     * Returns \c true if the file contains no data, otherwise
     * \c false, or on error.
     */
    bool IsEmpty() const;

    /**********************************************************************/

    /**
     * Returns the results of the call to Eof().
     */
    bool eof() const {
        return Eof();
    }

    /**
     * Returns \c true if the file was successfully opened, there are
     * no I/O errors, and the \c EOF has not been reached.
     */
    bool good() const {
        return (isValid() && !Error() && !Eof());
    }

    /***********************************************************************/

    /**
     * Returns \c true if the file exists.
     */
    bool Exists() const {
        return RWFile::Exists(name_.get());
    }

    /**
     * Returns \c true if a file with name \a filename exists.
     */
    static bool Exists(const char* filename);

    /**
     * Returns \c true if a file with name \a filename exists and may
     * be accessed according to the \a mode specified. The \a mode
     * may be <tt>OR</tt>ed together from one or more of:
     *
     * <table>
     * <tr><th>Mode</th><th>Meaning</th></tr>
     * <tr><td>\c F_OK</td><td>Exists (Implied by any of the others)</td></tr>
     * <tr><td>\c X_OK</td><td>Executable or searchable</td></tr>
     * <tr><td>\c W_OK</td><td>Writable</td></tr>
     * <tr><td>\c R_OK</td><td>Readable</td></tr>
     * </table>
     *
     * If your compiler or operating system does not support the POSIX
     * \c access() function, then mode \c X_OK always returns \c false.
     */
    static bool Exists(const char* filename, int mode);

    /***********************************************************************/

    /**
     * Reads the indicated built-in type.  Returns \c true if the read
     * is successful. If you are already at the end of a file, this
     * is interpreted as successfully reading a null string, and
     * returns \c true.
     */
    bool Read(bool& i);

    /**
     * @copydoc Read(bool&)
     */
    bool Read(char& c);
    /**
     * @copydoc Read(bool&)
     */
    bool Read(short& i);
    /**
     * @copydoc Read(bool&)
     */
    bool Read(int& i);
    /**
     * @copydoc Read(bool&)
     */
    bool Read(long& i);

    /**
     * @copydoc Read(bool&)
     */
#ifndef RW_NO_OVERLOAD_UCHAR
    bool Read(unsigned char& c);
#endif

    /**
     * @copydoc Read(bool&)
     */
    bool Read(wchar_t& w);

    /**
     * @copydoc Read(bool&)
     */
    bool Read(unsigned short& i);
    /**
     * @copydoc Read(bool&)
     */
    bool Read(unsigned int& i);
    /**
     * @copydoc Read(bool&)
     */
    bool Read(unsigned long& i);
    /**
     * @copydoc Read(bool&)
     */
    bool Read(float& f);
    /**
     * @copydoc Read(bool&)
     */
    bool Read(double& s);

#if !defined(RW_NO_LONG_LONG)
    /**
     * Reads the indicated built-in type.  Returns \c true if the read
     * is successful. If you are already at the end of a file, this
     * is interpreted as successfully reading a null string, and
     * returns \c true.
     *
     * @note
     * This function is available only if your compiler supports
     * the <tt>long long</tt> type.
     */
    bool Read(long long& i);
    /**
     * Reads the indicated built-in type.  Returns \c true if the read
     * is successful. If you are already at the end of a file, this
     * is interpreted as successfully reading a null string, and
     * returns \c true.
     *
     * @note
     * This function is available only if your compiler supports
     * the <tt>unsigned long long</tt> type.
     */
    bool Read(unsigned long long& i);
#endif

#ifndef RW_NO_LONG_DOUBLE
    /**
     * Reads the indicated built-in type.  Returns \c true if the read
     * is successful. If you are already at the end of a file, this
     * is interpreted as successfully reading a null string, and
     * returns \c true.
     *
     * @note
     * This function is available only if your compiler supports
     * the <tt>long double</tt> type.
     */
    bool Read(long double& d);
#endif

    RWFile& getSizeT(size_t& t) {
        Read(t);
        return *this;
    }

    /**
     * Extracts characters from the file, storing each in consecutive
     * elements of the array \a i until either \a n characters have
     * been extracted, or if <tt>(delim != EOF)</tt>, the character
     * specified by \a delim has been extracted.
     *
     * If the delimiter is reached before \a n characters are extracted,
     * the delimiter terminates the array \a i; if the delimiter is
     * \c EOF and is reached before \a n characters are extracted,
     * the method null terminates the array \a i.
     *
     * The parameter \a n is the maximum number of characters to
     * read, and must be less than or equal to the size of the array
     * pointed to by \a i. The \a delim parameter specifies a delimiter
     * to use as a termination point for extraction. The value of \a
     * delim must be a valid character converted to \c int, or \c EOF.
     *
     * The optional parameter \a count is a pointer to the actual number of
     * characters read by the function.
     *
     * This function returns \c true if \a n bytes were read from the
     * file, or the specified delimiter was reached.
     *
     * @note
     * The caller is responsible for declaring and allocating the necessary
     * storage for \a i and \a count (if used), before calling this function.
     */
    bool Read(char* i, size_t n, int delim, size_t* count = 0);

    /**
     * Reads \a n instances of the indicated built-in type into
     * a block pointed to by \a i.  Returns \c true if the read is
     * successful.
     *
     * The optional parameter \a count is a pointer to the actual number of
     * characters read by the function.
     *
     * @note
     * The caller is responsible for declaring and allocating the necessary
     * storage for \a i and \a count (if used), before calling this function.
     */
    bool Read(bool* i, size_t n, size_t* count = 0);

    /**
     * @copydoc Read(bool*,size_t,size_t*)
     *
     * @note
     * If the caller passes \c 0 for \a count, the program will compile without
     * any warnings and call Read(char* i, size_t n, int delim, size_t* count).
     */
    bool Read(char* i, size_t n, size_t* count = 0);
    /**
     * @copydoc Read(bool*,size_t,size_t*)
     */
    bool Read(short* i, size_t n, size_t* count = 0);
    /**
     * @copydoc Read(bool*,size_t,size_t*)
     */
    bool Read(int* i, size_t n, size_t* count = 0);
    /**
     * @copydoc Read(bool*,size_t,size_t*)
     */
    bool Read(long* i, size_t n, size_t* count = 0);

    /**
     * @copydoc Read(bool*,size_t,size_t*)
     */
    bool Read(wchar_t* i, size_t n, size_t* count = 0);

    /**
     * @copydoc Read(bool*,size_t,size_t*)
     */
    bool Read(unsigned char* c, size_t n, size_t* count = 0) {
        return Read((char*)c, n, count);
    }

    /**
     * @copydoc Read(bool*,size_t,size_t*)
     */
    bool Read(unsigned short* i, size_t n, size_t* count = 0) {
        return Read((short*)i, n, count);
    }

    /**
     * @copydoc Read(bool*,size_t,size_t*)
     */
    bool Read(unsigned int* i, size_t n, size_t* count = 0) {
        return Read((int*)i, n, count);
    }

    /**
     * @copydoc Read(bool*,size_t,size_t*)
     */
    bool Read(unsigned long* i, size_t n, size_t* count = 0) {
        return Read((long*)i, n, count);
    }

    /**
     * @copydoc Read(bool*,size_t,size_t*)
     */
    bool Read(float* i, size_t n, size_t* count = 0);
    /**
     * @copydoc Read(bool*,size_t,size_t*)
     */
    bool Read(double* i, size_t n, size_t* count = 0);

#if !defined(RW_NO_LONG_LONG)
    /**
      * Reads \a n instances of the indicated built-in type into
      * a block pointed to by \a i.  Returns \c true if the read is
      * successful.
      *
      * The optional parameter \a count is a pointer to the actual number of
      * characters read by the function.
      *
      * @note
      * The caller is responsible for declaring and allocating the necessary
      * storage for \a i and \a count (if used), before calling this function.
      *
      * @note
      * This function is available only if your compiler supports
      * the <tt>long long</tt> type.
      */
    bool Read(long long* i, size_t n, size_t* count = 0);
    /**
      * Reads \a n instances of the indicated built-in type into
      * a block pointed to by \a i.  Returns \c true if the read is
      * successful.
      *
      * The optional parameter \a count is a pointer to the actual number of
      * characters read by the function.
      *
      * @note
      * The caller is responsible for declaring and allocating the necessary
      * storage for \a i and \a count (if used), before calling this function.
      *
      * @note
      * This function is available only if your compiler supports
      * the <tt>unsigned long long</tt> type.
      */
    bool Read(unsigned long long* i, size_t n, size_t* count = 0);
#endif

#ifndef RW_NO_LONG_DOUBLE
    /**
     * Reads \a n instances of the indicated built-in type into
     * a block pointed to by \a i. Returns \c true if the read is
     * successful.
     *
     * The optional parameter \a count is a pointer to the actual number of
     * characters read by the function.
     *
     * @note
     * The caller is responsible for declaring and allocating the necessary
     * storage for \a i and \a count (if used), before calling this function.
     *
     * @note
     * This function is available only if your compiler supports
     * the <tt>long double</tt> type.
     */
    bool Read(long double* i, size_t n, size_t* count = 0);
#endif

#if !defined(RW_DISABLE_DEPRECATED)
    // Read to null terminator or \c EOF; no CR/LF translation done.
    /**
     * @deprecated As of SourcePro 11. Because of the risk of buffer overflow,
     * we strongly recommend against using this function. Use Read(char*, size_t)
     * or Read(char*, size_t, int) instead.
     *
     * Reads a character string, <i>including the terminating null
     * character</i>, into a block pointed to by \a string.  Returns \c true
     * if the read is successful.
     *
     * @note
     * The caller is responsible for declaring \a string and for
     * allocating the necessary storage before calling this function.
     *
     */
    RW_DEPRECATE_FUNC("Use Read(char*, size_t) instead")
    bool Read(char* string);
#endif

    /**********************************************************************/

    /**
     * Writes the appropriate built-in type. Returns \c true if the
     * write is successful.
     */
    bool Write(bool i);

    /**
     * @copydoc Write(bool)
     */
    bool Write(char i);
    /**
     * @copydoc Write(bool)
     */
    bool Write(short i);
    /**
     * @copydoc Write(bool)
     */
    bool Write(int i);
    /**
     * @copydoc Write(bool)
     */
    bool Write(long i);

    /**
     * @copydoc Write(bool)
     */
    bool Write(wchar_t i);

    /**
     * @copydoc Write(bool)
     */
    bool Write(unsigned char i);

    /**
     * @copydoc Write(bool)
     */
    bool Write(unsigned short i);
    /**
     * @copydoc Write(bool)
     */
    bool Write(unsigned int i);
    /**
     * @copydoc Write(bool)
     */
    bool Write(unsigned long i);

    /**
     * @copydoc Write(bool)
     */
    bool Write(float f);
    /**
     * @copydoc Write(bool)
     */
    bool Write(double d);

#if !defined(RW_NO_LONG_LONG)
    /**
      * Writes the appropriate built-in type.  Returns \c true if the
      * write is successful.
      *
      * @note
      * This function is available only if your compiler supports
      * the <tt>long long</tt> type.
      */
    bool Write(long long i);
    /**
      * Writes the appropriate built-in type.  Returns \c true if the
      * write is successful.
      *
      * @note
      * This function is available only if your compiler supports
      * the <tt>unsigned long long</tt> type.
      */
    bool Write(unsigned long long i);
#endif

#ifndef RW_NO_LONG_DOUBLE
    /**
     * Writes the appropriate built-in type.  Returns \c true if the
     * write is successful.
     *
     * @note
     * This function is available only if your compiler supports
     * the <tt>long double</tt> type.
     */
    bool Write(long double d);
#endif

    RWFile& putSizeT(size_t t) {
        Write(t);
        return *this;
    }
    /**
     * Writes a character string, <i>including the terminating null
     * character</i>, from a block pointed to by \a string.  Returns
     * \c true if the write is successful.  Beware of non-terminated
     * strings when using this function.
     */
    bool Write(const char* string);

    /**
     * Writes \a count instances of the indicated built-in type from
     * a block pointed to by \a i.  Returns \c true if the write is
     * successful.
     */
    bool Write(const bool* i, size_t count);

    /**
     * @copydoc Write(const bool*,size_t)
     */
    bool Write(const short* i, size_t count);
    /**
     * @copydoc Write(const bool*,size_t)
     */
    bool Write(const int* i, size_t count);
    /**
     * @copydoc Write(const bool*,size_t)
     */
    bool Write(const long* i, size_t count);

    /**
     * @copydoc Write(const bool*,size_t)
     */
    bool Write(const wchar_t* i, size_t count);

    /**
     * @copydoc Write(const bool*,size_t)
     */
    bool Write(const unsigned char* c, size_t N) {
        return Write((const char*)c, N);
    }

    /**
     * @copydoc Write(const bool*,size_t)
     */
    bool Write(const unsigned short* i, size_t N) {
        return Write((const short*)i, N);
    }

    /**
     * @copydoc Write(const bool*,size_t)
     */
    bool Write(const unsigned int* i, size_t N) {
        return Write((const int*)i, N);
    }

    /**
     * @copydoc Write(const bool*,size_t)
     */
    bool Write(const unsigned long* i, size_t N) {
        return Write((const long*)i, N);
    }

    /**
     * @copydoc Write(const bool*,size_t)
     */
    bool Write(const float* i, size_t count);
    /**
     * @copydoc Write(const bool*,size_t)
     */
    bool Write(const double* i, size_t count);
    /**
     * @copydoc Write(const bool*,size_t)
     */
    bool Write(const char* string, size_t);

#if !defined(RW_NO_LONG_LONG)
    /**
     * Writes \a count instances of the indicated built-in type from
     * a block pointed to by \a i.  Returns \c true if the write is
     * successful.
     *
     * @note
     * This function is available only if your compiler supports
     * the <tt>long long</tt> type.
     */
    bool Write(const long long* i, size_t count);
    /**
     * Writes \a count instances of the indicated built-in type from
     * a block pointed to by \a i.  Returns \c true if the write is
     * successful.
     *
     * @note
     * This function is available only if your compiler supports
     * the <tt>unsigned long long</tt> type.
     */
    bool Write(const unsigned long long* i, size_t count);
#endif

#ifndef RW_NO_LONG_DOUBLE
    /**
     * Writes \a count instances of the indicated built-in type from
     * a block pointed to by \a i.  Returns \c true if the write is
     * successful.
     *
     * @note
     * This function is available only if your compiler supports
     * the <tt>long double</tt> type.
     */
    bool Write(const long double* i, size_t count);
#endif

protected:

    RWTScopedPointer<char[]> name_;

    RWTScopedPointer<char[]> mode_;

    FILE* file_;

    bool large_;
};

/**************************************************************************/

/**
 * Saves a built-in type \a a to the RWFile \a f.
 */
inline RWFile& operator<<(RWFile& f, char a)
{
    f.Write(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char)
 */
inline RWFile& operator<<(RWFile& f, bool a)
{
    f.Write(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char)
 */
inline RWFile& operator<<(RWFile& f, short a)
{
    f.Write(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char)
 */
inline RWFile& operator<<(RWFile& f, int a)
{
    f.Write(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char)
 */
inline RWFile& operator<<(RWFile& f, long a)
{
    f.Write(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char)
 */
inline RWFile& operator<<(RWFile& f, wchar_t a)
{
    f.Write(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char)
 */
inline RWFile& operator<<(RWFile& f, unsigned char a)
{
    f.Write(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char)
 */
inline RWFile& operator<<(RWFile& f, unsigned short a)
{
    f.Write(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char)
 */
inline RWFile& operator<<(RWFile& f, unsigned int a)
{
    f.Write(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char)
 */
inline RWFile& operator<<(RWFile& f, unsigned long a)
{
    f.Write(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char)
 */
inline RWFile& operator<<(RWFile& f, float a)
{
    f.Write(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char)
 */
inline RWFile& operator<<(RWFile& f, double a)
{
    f.Write(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char)
 */
inline RWFile& operator<<(RWFile& f, const char* a)
{
    f.Write(a);
    return f;
}

#if !defined(RW_NO_LONG_LONG)
/**
 * Saves a built-in type \a a to the RWFile \a f.
 *
 * @note
 * This operator is available only if your compiler supports
 * the <tt>long long</tt> type.
 */
inline RWFile& operator<<(RWFile& f, long long a)
{
    f.Write(a);
    return f;
}
/**
 * @copydoc operator<<(RWFile&,long long)
 */
inline RWFile& operator<<(RWFile& f, unsigned long long a)
{
    f.Write(a);
    return f;
}
#endif

#ifndef RW_NO_LONG_DOUBLE
/**
 * Saves a built-in type \a a to the RWFile \a f.
 *
 * @note
 * This operator is available only if your compiler supports
 * the <tt>long double</tt> type.
 */
inline RWFile& operator<<(RWFile& f, long double a)
{
    f.Write(a);
    return f;
}
#endif


/**
 * Restores a builtin-type \a a from the RWFile \a f,
 * replacing the previous contents of \a a.
 */
inline RWFile& operator>>(RWFile& f, char& a)
{
    f.Read(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char&)
 */
inline RWFile& operator>>(RWFile& f, bool& a)
{
    f.Read(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char&)
 */
inline RWFile& operator>>(RWFile& f, short& a)
{
    f.Read(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char&)
 */
inline RWFile& operator>>(RWFile& f, int& a)
{
    f.Read(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char&)
 */
inline RWFile& operator>>(RWFile& f, long& a)
{
    f.Read(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char&)
 */
inline RWFile& operator>>(RWFile& f, wchar_t& a)
{
    f.Read(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char&)
 */
inline RWFile& operator>>(RWFile& f, unsigned char& a)
{
    f.Read(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char&)
 */
inline RWFile& operator>>(RWFile& f, unsigned short& a)
{
    f.Read(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char&)
 */
inline RWFile& operator>>(RWFile& f, unsigned int& a)
{
    f.Read(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char&)
 */
inline RWFile& operator>>(RWFile& f, unsigned long& a)
{
    f.Read(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char&)
 */
inline RWFile& operator>>(RWFile& f, float& a)
{
    f.Read(a);
    return f;
}

/**
 * @copydoc operator<<(RWFile&,char&)
 */
inline RWFile& operator>>(RWFile& f, double& a)
{
    f.Read(a);
    return f;
}

#if !defined(RW_NO_LONG_LONG)
/**
 * Restores a builtin-type \a a from the RWFile \a f,
 * replacing the previous contents of \a a.
 *
 * @note
 * This operator is available only if your compiler supports
 * the <tt>long long</tt> type.
 */
inline RWFile& operator>>(RWFile& f, long long& a)
{
    f.Read(a);
    return f;
}
/**
 * @copydoc operator>>(RWFile&,long long&)
 */
inline RWFile& operator>>(RWFile& f, unsigned long long& a)
{
    f.Read(a);
    return f;
}
#endif

#ifndef RW_NO_LONG_DOUBLE
/**
 * Restores a builtin-type \a a from the RWFile \a f,
 * replacing the previous contents of \a a.
 *
 * @note
 * This operator is available only if your compiler supports
 * the <tt>long double</tt> type.
 */
inline RWFile& operator>>(RWFile& f, long double& a)
{
    f.Read(a);
    return f;
}
#endif


// The following are used for simple External Template Persistence (ETP)
// for builtin types.

RWEXPORT_RETURN(RWFile&) operator>>(RWFile& s, char*& ip);

RWEXPORT_RETURN(RWFile&) operator>>(RWFile& s, wchar_t*& ip);

RWEXPORT_RETURN(RWFile&) operator>>(RWFile& s, unsigned char*& ip);

RWEXPORT_RETURN(RWFile&) operator>>(RWFile& s, double*& ip);
RWEXPORT_RETURN(RWFile&) operator>>(RWFile& s, float*& ip);
RWEXPORT_RETURN(RWFile&) operator>>(RWFile& s, int*& ip);
RWEXPORT_RETURN(RWFile&) operator>>(RWFile& s, long*& ip);
RWEXPORT_RETURN(RWFile&) operator>>(RWFile& s, short*& ip);
RWEXPORT_RETURN(RWFile&) operator>>(RWFile& s, unsigned int*& ip);
RWEXPORT_RETURN(RWFile&) operator>>(RWFile& s, unsigned long*& ip);
RWEXPORT_RETURN(RWFile&) operator>>(RWFile& s, unsigned short*& ip);

#if !defined(RW_NO_LONG_LONG)
RWEXPORT_RETURN(RWFile&) operator>>(RWFile& s, long long*& ip);
RWEXPORT_RETURN(RWFile&) operator>>(RWFile& s, unsigned long long*& ip);
#endif

#ifndef RW_NO_LONG_DOUBLE
RWEXPORT_RETURN(RWFile&) operator>>(RWFile& s, long double*& ip);
#endif

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif /* RW_TOOLS_FILE_H */
