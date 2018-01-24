#ifndef RW_TOOLS_COMPRESSED_STREAMBUF_H
#define RW_TOOLS_COMPRESSED_STREAMBUF_H

/**********************************************************************
 *
 * $Id: //tools/13/rw/tools/cmpstreambuf.h#1 $
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
#include <rw/typedefs.h>  // for rwuint32
#include <rw/tvvector.h>  // for RWTValVector

#include <streambuf>

/**********************************************************************/

RW_TOOLS_GLOBAL RWssize_t rw_sputn(std::streambuf* stream_buffer, const void* buf, RWssize_t length);
RW_TOOLS_GLOBAL RWssize_t rw_sgetn(std::streambuf* stream_buffer, void* buf, RWssize_t length);

/**********************************************************************/

/**
 * @ingroup virtual_stream_classes
 *
 * @brief Derived stream buffer that reads and writes compressed data to
 * an associated \b std::streambuf.
 *
 * Derived stream buffer that reads and writes compressed data to an associated
 * \b std::streambuf using the provided compressor type.
 *
 * The compressor must be copy constructible, and if an instance isn't provided
 * to the constructor, it must be default constructible. It is also expected to
 * support the following interface methods.
 *
 * - int compress (void* dst, rwuint32& dst_len, const void* src, rwuint32 src_len)
 *
 * The parameter \e dst_len initially describes the length of the output buffer
 * \e dst, and \e src_len describes the length of the input buffer \e src. This
 * function compresses \e src_len bytes of data from \e src into \e dst and puts
 * the number of bytes written to \e dst into \e dst_len. Must return 0 on success,
 * or a negative value on error.

 * - int decompress (void* dst, rwuint32& dst_len, const void* src, rwuint32 src_len)
 *
 * The parameter \e dst_len initially describes the length of the output buffer
 * \e dst, and \e src_len describes the length of the input buffer \e src. This
 * function decompresses \e src_len bytes of data from \e src into \e dst and puts
 * the number of bytes written to \e dst into \e dst_len. Must return 0 on success,
 * or a negative value on error.
 *
 * - int max_compressed_length (rwuint32 src_len, rwuint32& dst_len)
 *
 * The parameter \e src_len is the length of an uncompressed data buffer. This function
 * writes the worst case compressed size of that data to \e dst_len. Must return 0
 * on success, or a negative value on error.
 *
 * @note This class is able to read only compressed data that was generated by
 * a similar stream buffer. It cannot read compressed data that was generated
 * with other tools or with data compressed using a different compressor type.
 */
template <class Compressor>
class RWCompressedStreamBuffer : public std::streambuf
{
    // this is the sink and source of compressed data
    std::streambuf* stream_buffer_;

    // this buffer contains the put area
    RWTValVector<char> put_area_;

    // this buffer contains the get area; might need to grow this if data is highly
    // compressed
    RWTValVector<char> get_area_;

    // data in put_area_ is compressed into this buffer on overflow and is written out
    // to stream_buffer_. data is read from stream_buffer_ into this buffer, and then
    // decompressed into the get area.
    RWTValVector<char> compress_buffer_;

    // the compressor used
    Compressor compressor_;

public:
    typedef std::streambuf::traits_type traits_type;
    typedef traits_type::char_type char_type;
    typedef traits_type::int_type  int_type;
    typedef traits_type::pos_type  pos_type;
    typedef traits_type::off_type  off_type;

    /**
     * Constructor initializes a stream buffer that uses \a compressor to compress
     * or decompress data. A successful call to open() is required before this
     * stream buffer can be used.
     */
    RWCompressedStreamBuffer(const Compressor& compressor = Compressor());

    /**
     * Constructor initializes a stream buffer that reads compressed data from or
     * writes compressed data to \a stream_buffer in buffers of up to \a block_size
     * bytes, and uses \a compressor to compress or decompress data as necessary.
     * The \a mode parameter controls how this stream buffer may be used.
     *
     * @note
     * If \a mode indicates that the underlying stream buffer is to be used for
     * output, \a block_size should be non-zero. Typically larger values result
     * in better compression levels.
     */
    RWCompressedStreamBuffer(std::streambuf* stream_buffer,
                             std::ios_base::openmode mode,
                             rwuint32 block_size,
                             const Compressor& compressor = Compressor());

    /**
     * Destructor.
     */
    virtual ~RWCompressedStreamBuffer() RW_STREAMBUF_DTOR_THROWSPEC;

    /**
     * Opens this stream buffer to read or write compressed data.
     *
     * If \a mode indicates that this stream buffer is to be used for output,
     * \a block_size specifies the number of bytes of uncompressed data to be buffered.
     * When the buffer is overfilled or flushed, it will be compressed and written to
     * \a stream_buffer.
     *
     * If \a mode indicates that this stream buffer is to be used for input, data
     * will be read from \a stream_buffer and decompressed into a local buffer. The
     * size of this local buffer is not associated with \a block_size in any way.
     *
     * Returns \c NULL when this stream buffer is already open or if there is some other
     * error, otherwise returns a pointer to self.
     *
     * @note
     * If \a mode indicates that the underlying stream buffer is to be used for
     * output, \a block_size should be non-zero. Typically larger values result
     * in better compression levels.
     */
    RWCompressedStreamBuffer*
    open(std::streambuf* stream_buffer, std::ios_base::openmode mode, rwuint32 block_size);

    /**
     * Closes this stream buffer, flushes all pending data, and disassociates it from
     * the underlying \b std::streambuf. After calling this method, this stream buffer
     * cannot be used for input or output until it has been reopened with a successful
     * call to open().
     *
     * Returns \c NULL on error, otherwise returns a pointer to self.
     */
    RWCompressedStreamBuffer*
    close();

    /**
     * Returns \c true if this stream buffer is open and is ready for use, otherwise
     * returns \c false.
     */
    bool is_open() const {
        return 0 != stream_buffer_;
    }

    /**
     * Returns a reference to the underlying compressor.
     */
    const Compressor& get_compressor() const {
        return compressor_;
    }

protected:

    virtual int_type overflow(int_type = traits_type::eof());

    virtual int_type underflow();

    virtual int sync();

private:

    void reset();

private:

    // not implemented
    RWCompressedStreamBuffer(const RWCompressedStreamBuffer&);

    // not implemented
    RWCompressedStreamBuffer& operator= (const RWCompressedStreamBuffer&);
};

#ifdef RW_NO_IMPLICIT_INCLUSION
#  include "rw/tools/cmpstreambuf.cc"
#elif defined(_AIX)
#  pragma implementation ("cmpstreambuf.cc")
#endif

#endif /* RW_TOOLS_COMPRESSED_STREAMBUF_H */