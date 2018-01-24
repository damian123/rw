/**********************************************************************
 *
 * $Id: //tools/13/rw/tools/cmpstreambuf.cc#1 $
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

#include <rw/tools/cmpstreambuf.h>
#include <rw/tools/scopeguard.h>  // used for cleanup automation

#include <rw/typedefs.h>          // for fixed-width types

#include <streambuf>

#if defined(RW_LITTLE_ENDIAN)
#  define RW_HE2LE_32(x) (x)
#  define RW_LE2HE_32(x) (x)
#else
#  define RW_HE2LE_32(x) RW_BYTE_SWAP_32(x)
#  define RW_LE2HE_32(x) RW_BYTE_SWAP_32(x)
#endif

/*******************************************************************************************************************************/

template <class Compressor>
RWCompressedStreamBuffer<Compressor>::RWCompressedStreamBuffer(const Compressor& compressor)
    : stream_buffer_(0)
    , compressor_(compressor)
{
}

/*******************************************************************************************************************************/

template <class Compressor>
RWCompressedStreamBuffer<Compressor>::RWCompressedStreamBuffer(std::streambuf* stream_buffer,
        std::ios_base::openmode mode,
        rwuint32 block_size,
        const Compressor& compressor)
    : stream_buffer_(0)
    , compressor_(compressor)
{
    open(stream_buffer, mode, block_size);
}

/*******************************************************************************************************************************/

template <class Compressor>
RWCompressedStreamBuffer<Compressor>::~RWCompressedStreamBuffer()
{
    close();
}

/*******************************************************************************************************************************/

template <class Compressor>
RWCompressedStreamBuffer<Compressor>*
RWCompressedStreamBuffer<Compressor>::open(std::streambuf* stream_buffer,
        std::ios_base::openmode mode, rwuint32 block_size)
{
    const bool is_input  = 0 != (mode & std::ios_base::in);
    const bool is_output = 0 != (mode & std::ios_base::out);

    // null stream buffer, zero block size for an output stream, or open for neither input nor output
    if (stream_buffer == 0 || (is_output && block_size == 0) || (!is_input && !is_output)) {
        return 0;
    }

    // stream is already open
    if (stream_buffer_ != 0) {
        return 0;
    }

    // in case any exception is thrown, be sure to call reset before leaving this scope
    RWScopeGuard sg = rwtMakeScopeGuardM(*this, &RWCompressedStreamBuffer<Compressor>::reset);

    if (mode & std::ios_base::out) {
        put_area_.reshape(block_size);

        char* put_beg = (char*)put_area_.data();
        char* put_end = put_beg + block_size;

        setp(put_beg, put_end);
    }

    stream_buffer_ = stream_buffer;

    // dismiss the scope guard so it won't clean anything up
    sg.dismiss();

    return this;
}

/*******************************************************************************************************************************/

template <class Compressor>
RWCompressedStreamBuffer<Compressor>*
RWCompressedStreamBuffer<Compressor>::close()
{
    if (stream_buffer_ == 0 || 0 != sync()) {
        return 0;
    }

    reset();

    return this;
}

/*******************************************************************************************************************************/

template <class Compressor>
typename RWCompressedStreamBuffer<Compressor>::int_type
RWCompressedStreamBuffer<Compressor>::overflow(typename RWCompressedStreamBuffer<Compressor>::int_type c)
{
    if (stream_buffer_ == 0) {
        return std::char_traits<char>::eof();
    }

    // we must have a valid stream buffer to put data on
    RW_PRECONDITION(0 != stream_buffer_);

    const bool is_eof = (std::char_traits<char>::eq_int_type(c, std::char_traits<char>::eof()));

    // number of bytes in the input that we need to flush out
    rwuint32 uncompressed_byte_count = rwuint32((pptr() - pbase()) * sizeof(char));

    // initial size of the output buffer in bytes
    rwuint32 compressed_byte_count = rwuint32(compress_buffer_.entries());

    if (0 != compressor_.max_compressed_length(uncompressed_byte_count, compressed_byte_count)) {
        return std::char_traits<char>::eof();
    }

    // grow our compress buffer if necessary
    if (compress_buffer_.entries() < compressed_byte_count) {
        compress_buffer_.reshape(compressed_byte_count);
    }

    // compress data from put area into compress_buffer_
    const int compress_result =
        compressor_.compress(&compress_buffer_[0], compressed_byte_count,
                             pbase(), uncompressed_byte_count);

    // check for compression error. we know that we have enough space, so it must be an
    // error with the data. we can either throw or return eof.
    if (compress_result < 0) {
        return std::char_traits<char>::eof();
    }

    // if compression actually inflated the data we set compressed_byte_count to
    // uncompressed_byte_count and prepare to send the uncompressed data.
    if (uncompressed_byte_count < compressed_byte_count) {
        compressed_byte_count = uncompressed_byte_count;
    }

    // write uncompressed_byte_count to stream (big endian number of bytes)
    const rwuint32 b_uncompressed_byte_count = RW_HE2LE_32(uncompressed_byte_count);
    if (rw_sputn(stream_buffer_, &b_uncompressed_byte_count, sizeof b_uncompressed_byte_count) != 0) {
        return std::char_traits<char>::eof();
    }

    // write compressed_byte_count to stream (big endian number of bytes)
    const rwuint32 b_compressed_byte_count = RW_HE2LE_32(compressed_byte_count);
    if (rw_sputn(stream_buffer_, &b_compressed_byte_count, sizeof b_compressed_byte_count) != 0) {
        return std::char_traits<char>::eof();
    }

    // if we actually compressed the data
    if (compressed_byte_count < uncompressed_byte_count) {
        // write compress_buffer_ to stream_buffer_
        if (rw_sputn(stream_buffer_, compress_buffer_.data(), compressed_byte_count) != 0) {
            return std::char_traits<char>::eof();
        }
    }
    else {
        // otherwise we send the uncompressed data
        if (rw_sputn(stream_buffer_, pbase(), uncompressed_byte_count) != 0) {
            return std::char_traits<char>::eof();
        }
    }

    // reset the put area
    setp(pbase(), epptr());

    if (is_eof) {
        return std::char_traits<char>::not_eof(c);
    }

    return sputc(char(c));
}

/*******************************************************************************************************************************/

template <class Compressor>
typename RWCompressedStreamBuffer<Compressor>::int_type
RWCompressedStreamBuffer<Compressor>::underflow()
{
    if (stream_buffer_ == 0) {
        return std::char_traits<char>::eof();
    }

    // we must have a valid stream buffer to get data from
    RW_PRECONDITION(0 != stream_buffer_);

    // read the uncompressed data from the stream (number of bytes)
    rwuint32 uncompressed_byte_count = 0;
    if (rw_sgetn(stream_buffer_, &uncompressed_byte_count, sizeof uncompressed_byte_count) != 0) {
        return std::char_traits<char>::eof();
    }

    uncompressed_byte_count = RW_LE2HE_32(uncompressed_byte_count);

    // read the compressed data size from the stream (number of bytes)
    rwuint32 compressed_byte_count = 0;
    if (rw_sgetn(stream_buffer_, &compressed_byte_count, sizeof compressed_byte_count) != 0) {
        return std::char_traits<char>::eof();
    }

    compressed_byte_count = RW_LE2HE_32(compressed_byte_count);

    // this should never trigger because overflow() will write uncompressed data if
    // the compression inflated the byte count
    RW_ASSERT(compressed_byte_count <= uncompressed_byte_count);

    // this divide is symmetric with the multiply in underflow(). this gets the number of
    // complete objects we will have in the get area when we are done.
    size_t num_objs = uncompressed_byte_count / sizeof(char);

    // ensure we have sufficient space in get area
    if (get_area_.entries() < num_objs) {
        get_area_.reshape(num_objs);
    }

    char* get_area = &get_area_[0];

    // if the data was compressed
    if (compressed_byte_count < uncompressed_byte_count) {
        // ensure we have sufficient space in our buffer
        if (compress_buffer_.entries() < compressed_byte_count) {
            compress_buffer_.reshape(compressed_byte_count);
        }

        // read the compressed data into buffer
        if (rw_sgetn(stream_buffer_, &compress_buffer_[0], compressed_byte_count) != 0) {
            return std::char_traits<char>::eof();
        }

        // decompress the data into get area
        const int decompress_result =
            compressor_.decompress(get_area, uncompressed_byte_count,
                                   &compress_buffer_[0], compressed_byte_count);

        if (decompress_result < 0) {
            return std::char_traits<char>::eof();
        }
    }
    else {
        // the data is not compressed. read uncompressed data right into the get area
        if (rw_sgetn(stream_buffer_, get_area, uncompressed_byte_count) != 0) {
            return std::char_traits<char>::eof();
        }
    }

    // reset the get area
    setg(get_area, get_area, get_area + num_objs);

    return std::char_traits<char>::to_int_type(*gptr());
}

/*******************************************************************************************************************************/

template <class Compressor>
int RWCompressedStreamBuffer<Compressor>::sync()
{
    if (pptr() != pbase()) {
        if (traits_type::eq_int_type(overflow(std::char_traits<char>::eof()), std::char_traits<char>::eof())) {
            return -1;
        }

        // not sure that this is appropriate, but it seems that flushing the compressed
        // buffer should flush the underlying stream also.
        return stream_buffer_->pubsync();
    }

    return 0;
}

/*******************************************************************************************************************************/

template <class Compressor>
void RWCompressedStreamBuffer<Compressor>::reset()
{
    setp(0, 0);

    setg(0, 0, 0);

    stream_buffer_ = 0;
}

/*******************************************************************************************************************************/
