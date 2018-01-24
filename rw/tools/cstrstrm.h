#ifndef RW_TOOLS_RWCSTRINGSTREAM_H
#define RW_TOOLS_RWCSTRINGSTREAM_H

/**********************************************************************
 *
 * Declarations for class RWCStringStream
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/tools/cstrstrm.h#1 $
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

#include <iostream>
#include <sstream>

typedef std::basic_stringbuf<char> RWCStringStreambuf;

// See SP-14202 for more information on why this is necessary
#if defined(_MSC_VER) && _MSC_VER >= 1700
#  pragma warning(push)
#  pragma warning(disable : 4250)
#endif

// Workaround for broken stream initializers.

#if defined(RW_BROKEN_STREAM_INIT)
#  define RW_STREAM_INIT_VALUE &buf_
#  define RW_STREAM_INIT_FUNC
#else
#  define RW_STREAM_INIT_VALUE 0
#  define RW_STREAM_INIT_FUNC  this->init(&buf_);
#endif

class RWCStringIStream : public std::istream
{
public:

    RWCStringIStream(void);

    RWCStringIStream(const RWCString& str);

    void str(const RWCString& str);

    RWCString str(void);

private:

    RWCStringStreambuf buf_;
};


inline
RWCStringIStream::RWCStringIStream(void)
    : std::istream(RW_STREAM_INIT_VALUE)
{
    RW_STREAM_INIT_FUNC
}

inline
RWCStringIStream::RWCStringIStream(const RWCString& s)
    : std::istream(RW_STREAM_INIT_VALUE)
#if defined(RW_COPY_ON_WRITE_STRING)
    , buf_(std::string(s.data(), s.length()), std::ios_base::in)
#else
    , buf_(s.std(), std::ios_base::in)
#endif
{
    RW_STREAM_INIT_FUNC
}

inline void
RWCStringIStream::str(const RWCString& s)
{
#if defined(RW_COPY_ON_WRITE_STRING)
    buf_.str(std::string(s.data(), s.length()));
#else
    buf_.str(s.std());
#endif
}

inline RWCString
RWCStringIStream::str(void)
{
    return RWCString(buf_.str());
}


class RWCStringOStream : public std::ostream
{
public:

    RWCStringOStream(void);

    RWCStringOStream(const RWCString& str);

    void str(const RWCString& str);

    RWCString str(void);

private:

    RWCStringStreambuf buf_;
};

inline
RWCStringOStream::RWCStringOStream(void)
    : std::ostream(RW_STREAM_INIT_VALUE)
{
    RW_STREAM_INIT_FUNC
}

inline
RWCStringOStream::RWCStringOStream(const RWCString& s)
    : std::ostream(RW_STREAM_INIT_VALUE)
#if defined(RW_COPY_ON_WRITE_STRING)
    , buf_(std::string(s.data(), s.length()), std::ios_base::out | std::ios_base::app)
#else
    , buf_(s.std(), std::ios_base::out | std::ios_base::app)
#endif
{
    RW_STREAM_INIT_FUNC

#if defined(RW_NO_STRINGSTREAM_APPEND)
    // stringstream doesn't support the ios_base::app mode
    // modifier, so we will have to seek to the end of the
    // input buffer manually.
    buf_.pubseekpos(s.length(), std::ios_base::out);
#endif
}

inline void
RWCStringOStream::str(const RWCString& s)
{
#if defined(RW_COPY_ON_WRITE_STRING)
    buf_.str(std::string(s.data(), s.length()));
#else
    buf_.str(s.std());
#endif
}

inline RWCString
RWCStringOStream::str(void)
{
    return RWCString(buf_.str());
}


class RWCStringStream : public std::iostream
{
public:

    RWCStringStream(void);

    RWCStringStream(const RWCString& str);

    void str(const RWCString& str);

    RWCString str(void);

private:

    RWCStringStreambuf buf_;
};

inline
RWCStringStream::RWCStringStream(void)
    : std::iostream(RW_STREAM_INIT_VALUE)
{
    RW_STREAM_INIT_FUNC
}

inline
RWCStringStream::RWCStringStream(const RWCString& s)
    : std::iostream(RW_STREAM_INIT_VALUE)
#if defined(RW_COPY_ON_WRITE_STRING)
    , buf_(std::string(s.data(), s.length())
           , std::ios_base::in | std::ios_base::out | std::ios_base::app)
#else
    , buf_(s.std(), std::ios_base::in | std::ios_base::out | std::ios_base::app)
#endif
{
    RW_STREAM_INIT_FUNC

#if defined(RW_NO_STRINGSTREAM_APPEND)
    // stringstream doesn't support the ios_base::app mode
    // modifier, so we will have to seek to the end of the
    // input buffer manually.
    buf_.pubseekpos(s.length(), std::ios_base::out);
#endif
}

inline void
RWCStringStream::str(const RWCString& s)
{
#if defined(RW_COPY_ON_WRITE_STRING)
    buf_.str(std::string(s.data(), s.length()));
#else
    buf_.str(s.std());
#endif
}

inline RWCString
RWCStringStream::str(void)
{
    return RWCString(buf_.str());
}

#undef RW_STREAM_INIT_VALUE
#undef RW_STREAM_INIT_FUNC

#if defined(_MSC_VER)
#  if(_MSC_VER >= 1700)
#    pragma warning(pop)
#  endif
#endif

#endif // #ifndef RW_TOOLS_RWCSTRINGSTREAM_H
