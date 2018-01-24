#ifndef RW_TOOLS_COMPRESSED_STREAM_H
#define RW_TOOLS_COMPRESSED_STREAM_H

/**********************************************************************
 *
 * $Id: //tools/13/rw/tools/cmpstream.h#1 $
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
#include <rw/tools/cmpstreambuf.h>

#include <iostream>

// See SP-14202 for more information on why this is necessary
#if defined(_MSC_VER) && _MSC_VER >= 1700
#  pragma warning(push)
#  pragma warning(disable : 4250)
#endif

/**
 * @ingroup virtual_stream_classes
 *
 * @brief Compresses data and serializes it to an associated \b std::streambuf.
 *
 * Class RWCompressedOStream compresses data and serializes it to an associated
 * \b std::streambuf. The results can be restored by using its counterpart
 * RWCompressedIStream.
 *
 * @see RWCompressedStreamBuffer
 *
 * @note
 * RWCompressedOStream can be interrogated as to the stream state using member
 * functions good(), bad(), eof(), etc.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tools/cmpstream.h>
 * RWCompressedOStream<Compressor> cstr(std::cout);   // Construct an RWCompressedOStream using cout's streambuf
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * // for SourcePro Core Tools Module compressed streams
 * #include <rw/tools/cmpstream.h>
 *
 * // for SourcePro Core Tools Module endian streams
 * #include <rw/estream.h>
 *
 * // for SourcePro Core Tools Module strings
 * #include <rw/cstring.h>
 *
 * // for SourcePro Core Snappy compressor
 * #include <rw/tools/snappy/RWSnappyCompressor.h>
 *
 * // for C++ Standard Library file streams
 * #include <fstream>
 *
 * int main ()
 * {
 *   // open the file to write to
 *   std::ofstream output_file ("output.snap", std::ios_base::binary);
 *
 *   // layer a compressed stream on top of the file
 *   RWCompressedOStream<RWSnappyCompressor> compressed_stream (output_file);
 *
 *   // layer an endian stream on top of the compressed stream
 *   RWeostream endian_stream (compressed_stream);
 *
 *   //
 *   // Any data written to the endian stream is converted into
 *   // an endian safe format by the endian stream, compressed by the
 *   // compressed stream, and written to the file by the file stream.
 *   //
 *
 *   RWCString data ('a', 32);
 *
 *   endian_stream << data;
 *
 *   return 0;
 * }
 * @endcode
 */
template <class Compressor>
class RWCompressedOStream : public std::ostream
{
    RWCompressedStreamBuffer<Compressor> stream_buffer_;

public:

    /**
     * Constructor initializes and opens the underlying stream buffer with the
     * provided \a stream_buffer, \a mode, \a block_size, and \a compressor. If
     * the open fails, \b failbit is set on this stream.
     *
     * @note
     * The \a mode is always combined with the appropriate flags to allow the
     * underlying stream buffer to be used for output.
     *
     * @see RWCompressedStreamBuffer::open()
     */
    RWCompressedOStream(std::streambuf* stream_buffer,
                        std::ios_base::openmode mode = std::ios_base::out,
                        rwuint32 block_size = 2048U,
                        const Compressor& compressor = Compressor())
#if defined(_MSC_VER)
    // work around memory leak with Microsoft's C++ Standard Library
        : std::ostream(&stream_buffer_)
        , stream_buffer_(compressor) {
#else
    // normal code path avoids crash with STLport 4
        :
        std::ostream(0)
        , stream_buffer_(compressor) {
        init(&stream_buffer_);
#endif

        if (!stream_buffer_.open(stream_buffer,
                                 mode | std::ios_base::out,
                                 block_size)) {
            setstate(std::ios_base::failbit);
        }
    }

    /**
     * Constructor initializes and opens the underlying stream buffer with the
     * provided \a mode, \a block_size, \a compressor, and the stream buffer
     * associated with \a stream. If the open fails, \b failbit is set on
     * this stream.
     *
     * @note
     * The \a mode is always combined with the appropriate flags to allow the
     * underlying stream buffer to be used for output.
     *
     * @see RWCompressedStreamBuffer::open()
     */
    RWCompressedOStream(std::ostream& stream,
                        std::ios_base::openmode mode = std::ios_base::out,
                        rwuint32 block_size = 2048U,
                        const Compressor& compressor = Compressor())
#if defined(_MSC_VER)
        : std::ostream(&stream_buffer_)
        , stream_buffer_(compressor) {
#else
        :
        std::ostream(0)
        , stream_buffer_(compressor) {
        init(&stream_buffer_);
#endif

        if (!stream_buffer_.open(stream.rdbuf(),
                                 mode | std::ios_base::out,
                                 block_size)) {
            setstate(std::ios_base::failbit);
        }
    }

    /**
     * Opens the underlying stream buffer with the provided \a stream_buffer, \a mode,
     * and \a block_size. If the open fails, \b failbit is set on this stream.
     *
     * @note
     * The \a mode is always combined with the appropriate flags to allow the
     * underlying stream buffer to be used for output.
     *
     * @see RWCompressedStreamBuffer::open()
     */
    void
    open(std::streambuf* stream_buffer,
         std::ios_base::openmode mode = std::ios_base::out,
         rwuint32 block_size = 2048U) {
        if (!stream_buffer_.open(stream_buffer, mode | std::ios_base::out, block_size)) {
            setstate(std::ios_base::failbit);
        }
    }

    /**
     * Closes the underlying stream buffer. If the close fails, \b failbit is set
     * on this stream. After this method has been called, this stream cannot be used
     * for output until a successful call to open() has been made.
     */
    void
    close() {
        if (!stream_buffer_.close()) {
            setstate(std::ios_base::failbit);
        }
    }

    /**
     * Returns \c true if the underlying stream buffer is open and is ready for use,
     * otherwise returns \c false.
     */
    bool is_open() const {
        return stream_buffer_.is_open();
    }

    /**
     * Returns a reference to the underlying stream buffer's compressor.
     */
    const Compressor& get_compressor() const {
        return stream_buffer_.get_compressor();
    }

    /**
     * Returns a pointer to the underlying stream buffer.
     */
    RWCompressedStreamBuffer<Compressor>* rdbuf() const {
        return const_cast<RWCompressedStreamBuffer<Compressor>*>(&RW_EXPOSE(stream_buffer_));
    }

};

/**
 * @ingroup virtual_stream_classes
 *
 * @brief Deserializes and decompresses data from an associated \b std::streambuf.
 *
 * Class RWCompressedIStream deserializes and decompresses data from an associated
 * \b std::streambuf. The data on that streambuf must have been serialized with its
 * counterpart RWCompressedOStream.
 *
 * @see RWCompressedStreamBuffer
 *
 * @note
 * RWCompressedIStream can be interrogated as to the stream state using member
 * functions good(), bad(), eof(), etc.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tools/cmpstream.h>
 * RWCompressedIStream<Compressor> cstr(std::cin);   // Construct an RWCompressedIStream using cin's streambuf
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * // for SourcePro Core Tools Module compressed streams
 * #include <rw/tools/cmpstream.h>
 *
 * // for SourcePro Core Tools Module endian streams
 * #include <rw/estream.h>
 *
 * // for SourcePro Core Tools Module strings
 * #include <rw/cstring.h>
 *
 * // for SourcePro Core Snappy compressor
 * #include <rw/tools/snappy/RWSnappyCompressor.h>
 *
 * // for C++ Standard Library file streams
 * #include <fstream>
 *
 * int main ()
 * {
 *   // open the file to read from
 *   std::ifstream input_file ("output.snap", std::ios_base::binary);
 *
 *   // layer a compressed stream on top of the file
 *   RWCompressedIStream<RWSnappyCompressor> compressed_stream (input_file);
 *
 *   // layer an endian stream on top of the compressed stream
 *   RWeistream endian_stream (compressed_stream);
 *
 *   //
 *   // Any data read from the endian stream is read from the file
 *   // stream, decompressed by the compressed stream, and converted from
 *   // an endian safe format into binary data by the endian stream.
 *   //
 *
 *   RWCString data;
 *
 *   endian_stream >> data;
 *
 *   std::cout << data;
 *
 *   return 0;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 * aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
 * @endcode
 */
template <class Compressor>
class RWCompressedIStream : public std::istream
{
    RWCompressedStreamBuffer<Compressor> stream_buffer_;

public:

    /**
     * Constructor initializes and opens the underlying stream buffer with the
     * provided \a stream_buffer, \a mode, \a block_size, and \a compressor. If
     * the open fails, \b failbit is set on this stream.
     *
     * @note
     * The \a mode is always combined with the appropriate flags to allow the
     * underlying stream buffer to be used for input.
     *
     * @see RWCompressedStreamBuffer::open()
     */
    RWCompressedIStream(std::streambuf* stream_buffer,
                        std::ios_base::openmode mode = std::ios_base::in,
                        rwuint32 block_size = 2048U,
                        const Compressor& compressor = Compressor())
#if defined(_MSC_VER)
        : std::istream(&stream_buffer_)
        , stream_buffer_(compressor) {
#else
        :
        std::istream(0)
        , stream_buffer_(compressor) {
        init(&stream_buffer_);
#endif

        if (!stream_buffer_.open(stream_buffer,
                                 mode | std::ios_base::in,
                                 block_size)) {
            setstate(std::ios_base::failbit);
        }
    }

    /**
     * Constructor initializes and opens the underlying stream buffer with the
     * provided \a mode, \a block_size, \a compressor, and the stream buffer
     * associated with \a stream. If the open fails, \b failbit is set on
     * this stream.
     *
     * @note
     * The \a mode is always combined with the appropriate flags to allow the
     * underlying stream buffer to be used for input.
     *
     * @see RWCompressedStreamBuffer::open()
     */
    RWCompressedIStream(std::istream& stream,
                        std::ios_base::openmode mode = std::ios_base::in,
                        rwuint32 block_size = 2048U,
                        const Compressor& compressor = Compressor())
#if defined(_MSC_VER)
        : std::istream(&stream_buffer_)
        , stream_buffer_(compressor) {
#else
        :
        std::istream(0)
        , stream_buffer_(compressor) {
        init(&stream_buffer_);
#endif

        if (!stream_buffer_.open(stream.rdbuf(),
                                 mode | std::ios_base::in,
                                 block_size)) {
            setstate(std::ios_base::failbit);
        }
    }

    /**
     * Opens the underlying stream buffer with the provided \a stream_buffer, \a mode,
     * and \a block_size. If the open fails, \b failbit is set on this stream.
     *
     * @note
     * The \a mode is always combined with the appropriate flags to allow the
     * underlying stream buffer to be used for input.
     *
     * @see RWCompressedStreamBuffer::open()
     */
    void
    open(std::streambuf* stream_buffer,
         std::ios_base::openmode mode = std::ios_base::in,
         rwuint32 block_size = 2048U) {
        if (!stream_buffer_.open(stream_buffer, mode | std::ios_base::in, block_size)) {
            setstate(std::ios_base::failbit);
        }
    }

    /**
     * Closes the underlying stream buffer. If the close fails, \b failbit is set
     * on this stream. After this method has been called, this stream cannot be used
     * for input until a successful call to open() has been made.
     */
    void
    close() {
        if (!stream_buffer_.close()) {
            setstate(std::ios_base::failbit);
        }
    }

    /**
     * Returns \c true if the underlying stream buffer is open and is ready for use,
     * otherwise returns \c false.
     */
    bool is_open() const {
        return stream_buffer_.is_open();
    }

    /**
     * Returns a reference to the underlying stream buffer's compressor.
     */
    const Compressor& get_compressor() const {
        return stream_buffer_.get_compressor();
    }

    /**
     * Returns a pointer to the underlying stream buffer.
     */
    RWCompressedStreamBuffer<Compressor>* rdbuf() const {
        return const_cast<RWCompressedStreamBuffer<Compressor>*>(&RW_EXPOSE(stream_buffer_));
    }

};

/**
 * @ingroup virtual_stream_classes
 *
 * @brief Compresses and serializes data to or deserializes and decompresses data
 * from an associated \b std::streambuf.
 *
 * Class RWCompressedIOStream compresses and serializes data to or deserializes and
 * decompresses data from an associated \b std::streambuf. Data read from the
 * associated streambuf must have been serialized with RWCompressedOStream or
 * RWCompressedIOStream, and any data written to the associated streambuf can be
 * deserialized with RWCompressedIStream or RWCompressedIOStream.
 *
 * @see RWCompressedStreamBuffer
 *
 * @note
 * RWCompressedIOStream can be interrogated as to the stream state using member
 * functions good(), bad(), eof(), etc.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/tools/cmpstream.h>
 *
 * std::fstream f("output.fil");
 * RWCompressedIOStream<Compressor> cstr(f);   // Construct an RWCompressedIOStream using f's streambuf
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 *
 * @section example Example
 *
 * @code
 * // for SourcePro Core Tools Module compressed streams
 * #include <rw/tools/cmpstream.h>
 *
 * // for SourcePro Core Tools Module endian streams
 * #include <rw/estream.h>
 *
 * // for SourcePro Core Tools Module strings
 * #include <rw/cstring.h>
 *
 * // for SourcePro Core Snappy compressor
 * #include <rw/tools/snappy/RWSnappyCompressor.h>
 *
 * // for C++ Standard Library file streams
 * #include <fstream>
 *
 * int main ()
 * {
 *   // open the file to write to and read from
 *   std::fstream data_file ("output.snap", std::ios_base::binary);
 *
 *   // layer a compressed stream on top of the file
 *   RWCompressedIOStream<RWSnappyCompressor> compressed_stream (data_file);
 *
 *   {
 *     // layer an endian stream on top of the compressed stream
 *     RWeostream endian_stream (compressed_stream);
 *
 *     //
 *     // Any data written to the endian stream is converted into
 *     // an endian safe format by the endian stream, compressed by the
 *     // compressed stream, and written to the file by the file stream.
 *     //
 *
 *     RWCString data ('a', 32);
 *
 *     endian_stream << data;
 *   }
 *
 *   // Ensure data written to the associated stream buffer is written to
 *   // the underlying stream buffer.
 *   compressed_stream.flush();
 *
 *   {
 *     // layer an endian stream on top of the compressed stream
 *     RWeistream endian_stream (compressed_stream);
 *
 *     //
 *     // Any data read from the endian stream is read from the file
 *     // stream, decompressed by the compressed stream, and converted into
 *     // an endian safe format into binary data by the endian stream.
 *     //
 *
 *     RWCString data;
 *
 *     compressed_stream >> data;
 *
 *     std::cout << data;
 *   }
 *
 *   return 0;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 * aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
 * @endcode
 */
template <class Compressor>
class RWCompressedIOStream : public std::iostream
{
    RWCompressedStreamBuffer<Compressor> stream_buffer_;

public:

    /**
     * Constructor initializes and opens the underlying stream buffer with the
     * provided \a stream_buffer, \a mode, \a block_size, and \a compressor. If
     * the open fails, \b failbit is set on this stream.
     *
     * @note
     * The \a mode is always combined with the appropriate flags to allow the
     * underlying stream buffer to be used for input and output.
     *
     * @see RWCompressedStreamBuffer::open()
     */
    RWCompressedIOStream(std::streambuf* stream_buffer,
                         std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out,
                         rwuint32 block_size = 2048U,
                         const Compressor& compressor = Compressor())
#if defined(_MSC_VER)
        : std::iostream(&stream_buffer_)
        , stream_buffer_(compressor) {
#else
        :
        std::iostream(0)
        , stream_buffer_(compressor) {
        init(&stream_buffer_);
#endif

        if (!stream_buffer_.open(stream_buffer,
                                 mode | std::ios_base::in | std::ios_base::out,
                                 block_size)) {
            setstate(std::ios_base::failbit);
        }
    }

    /**
     * Constructor initializes and opens the underlying stream buffer with the
     * provided \a mode, \a block_size, \a compressor, and the stream buffer
     * associated with \a stream. If the open fails, \b failbit is set on
     * this stream.
     *
     * @note
     * The \a mode is always combined with the appropriate flags to allow the
     * underlying stream buffer to be used for input and output.
     *
     * @see RWCompressedStreamBuffer::open()
     */
    RWCompressedIOStream(std::iostream& stream,
                         std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out,
                         rwuint32 block_size = 2048U,
                         const Compressor& compressor = Compressor())
#if defined(_MSC_VER)
        : std::iostream(&stream_buffer_)
        , stream_buffer_(compressor) {
#else
        :
        std::iostream(0)
        , stream_buffer_(compressor) {
        init(&stream_buffer_);
#endif

        if (!stream_buffer_.open(stream.rdbuf(),
                                 mode | std::ios_base::in | std::ios_base::out,
                                 block_size)) {
            setstate(std::ios_base::failbit);
        }
    }

    /**
     * Opens the underlying stream buffer with the provided \a stream_buffer, \a mode,
     * and \a block_size. If the open fails, \b failbit is set on this stream.
     *
     * @note
     * The \a mode is always combined with the appropriate flags to allow the
     * underlying stream buffer to be used for input and output.
     *
     * @see RWCompressedStreamBuffer::open()
     */
    void
    open(std::streambuf* stream_buffer,
         std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out,
         rwuint32 block_size = 2048U) {
        if (!stream_buffer_.open(stream_buffer, mode | std::ios_base::in | std::ios_base::out, block_size)) {
            setstate(std::ios_base::failbit);
        }
    }

    /**
     * Closes the underlying stream buffer. If the close fails, \b failbit is set
     * on this stream. After this method has been called, this stream cannot be used
     * for input or output until a successful call to open() has been made.
     */
    void
    close() {
        if (!stream_buffer_.close()) {
            setstate(std::ios_base::failbit);
        }
    }

    /**
     * Returns \c true if the underlying stream buffer is open and is ready for use,
     * otherwise returns \c false.
     */
    bool is_open() const {
        return stream_buffer_.is_open();
    }

    /**
     * Returns a reference to the underlying stream buffer's compressor.
     */
    const Compressor& get_compressor() const {
        return stream_buffer_.get_compressor();
    }

    /**
     * Returns a pointer to the underlying stream buffer.
     */
    RWCompressedStreamBuffer<Compressor>* rdbuf() const {
        return const_cast<RWCompressedStreamBuffer<Compressor>*>(&RW_EXPOSE(stream_buffer_));
    }

};

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tools/cmpstream.cc"
#elif defined(_AIX)
#  pragma implementation ("cmpstream.cc")
#endif

#if defined(_MSC_VER)
#  if(_MSC_VER >= 1700)
#    pragma warning(pop)
#  endif
#endif

#endif /* RW_TOOLS_COMPRESSED_STREAM_H */
