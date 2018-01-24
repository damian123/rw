#ifndef RW_TOOLS_AUDIT_STREAMBUFFER_H
#define RW_TOOLS_AUDIT_STREAMBUFFER_H



/**********************************************************************
 *
 * A streambuf that counts and allows you to "audit" each byte as it
 * passes through the buffer.
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/auditbuf.h#1 $
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
#include <rw/rstream.h>

#include <iosfwd>     // typedef for streamsize

/**
 * @relates RWAuditStreamBuffer
 * If you wish to do more than count each character handled by the
 * buffer, you may provide an \c RWauditFunction to the constructor.
 * The first parameter to this function is a byte provided by the
 * stream. The second parameter is the address of the counter to
 * be manipulated by RWAuditFunction.
 */
typedef void (*RWauditFunction)(unsigned char, void*);

/**
 * @ingroup virtual_stream_classes
 *
 * @brief Constructs a stream and audits the bytes that pass through it.
 *
 * Class RWAuditStreamBuffer is used to construct a stream, after
 * which the RWAuditStreamBuffer instance counts all the bytes
 * that pass through the stream. If constructed with a function
 * pointer, RWAuditStreamBuffer calls that function with each
 * byte that passes through the stream. The counting capacity provides
 * for streams the equivalent of the method RWCollectable::recursiveStoreSize(),
 * which is only available for RWFile.
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/auditbuf.h>
 * #include <iostream>
 * RWAuditStreamBuffer buf(arguments)
 * std::ostream os(&buf); // may be used for ostreams
 * std::istream is(&buf); // or istreams of any kind
 * @endcode
 *
 * @section example Example
 *
 * @code
 * #include <rw/bstream.h>
 * #include <rw/pstream.h>
 * #include <rw/auditbuf.h>
 *
 * #include <rw/collint.h>  // for RWCollectableInt
 * #include <rw/collstr.h>  // for RWCollectableString
 * #include <rw/dlistcol.h> // for RWDlistCollectables
 *
 * #include <iostream>
 *
 * int main ()
 * {
 *     RWCollectableInt i(7);
 *     RWCollectableString s("Hello!\n");
 *
 *     RWDlistCollectables ct;
 *
 *     // Construct a collection
 *     ct.append(&i);
 *     ct.append(&s);
 *     ct.prepend(&ct);
 *
 *     RWAuditStreamBuffer bcounter, pcounter;
 *
 *     RWbostream bcount(&bcounter); // ctor takes streambuf pointer
 *     RWpostream pcount(&pcounter);
 *
 *
 *     bcount << ct;
 *     pcount << ct;
 *
 *     std::cout << "We just counted " << bcounter
 *               << " bytes from an RWbostream.\n";
 *     std::cout << "We just counted " << pcounter
 *               << " bytes from an RWpostream." << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 *
 * Program output:
 *
 * @code
 * We just counted 43 bytes from an RWbostream.
 * We just counted 52 bytes from an RWpostream.
 * @endcode
 *
 * @section related Related Classes
 *
 * RWAuditStreamBuffer may be used as the \b std::streambuf for any stream,
 * including those derived from RWvostream or RWvistream, \b std::strstream,
 * \b std::ifstream, \b std::ofstream, etc.
 *
 * @section persistence Persistence
 *
 * None
 */
class RW_TOOLS_GLOBAL RWAuditStreamBuffer : public std::streambuf
{
    unsigned long     seenBytes_;

    RWauditFunction   funcP_;
    void*             funcV_;

    std::streambuf*  realBuf_;
    char myBuf[1]; // Required for underflow and sputbackc.

public:
    /**
     * Constructs a new RWAuditStreamBuffer that may be used only to
     * examine and count every byte that passes into an \b std::ostream
     * that has the RWAuditStreamBuffer instance as its \b std::streambuf.
     * It does not forward the bytes to any stream, nor accept bytes from
     * a stream. The second parameter to the constructor allows you to
     * supply storage for the byte count. It is optional.
     */
    RWAuditStreamBuffer(RWauditFunction = 0, void* = 0);

    /**
     * Constructs a new RWAuditStreamBuffer that passes bytes from the
     * \b std::istream on which it is constructed to the \b std::istream
     * that has the RWAuditStreamBuffer instance as its \b std::streambuf.
     * A typical use would be to count or examine the bytes being input
     * from a file through a stream derived from RWvistream. The third
     * parameter to the constructor allows you to supply storage for
     * the byte count. It is optional.
     */
    RWAuditStreamBuffer(std::istream&,
                        RWauditFunction = 0,
                        void* = 0);

    /**
     * Constructs a new RWAuditStreamBuffer that passes bytes to and
     * from the \b std::iostream on which it is constructed to and from the
     * \b std::istream that has the RWAuditStreamBuffer instance as its
     * \b std::streambuf. A typical use would be to count or examine the bytes
     * being transferred to and from a file used to store and retrieve
     * changing data. The third parameter to the constructor allows you
     * to supply storage for the byte count. It is optional.
     */
    RWAuditStreamBuffer(std::iostream&,
                        RWauditFunction = 0,
                        void* = 0);

    /**
     * Constructs a new RWAuditStreamBuffer that passes bytes into the
     * \b std::ostream on which it is constructed from the \b std::ostream
     * that has the RWAuditStreamBuffer instance as its \b std::streambuf. A
     * typical use would be to count or examine the bytes being output to a
     * file through a stream derived from RWvostream. The third parameter
     * to the constructor allows you to supply storage for the byte
     * count. It is optional.
     */
    RWAuditStreamBuffer(std::ostream&,
                        RWauditFunction = 0,
                        void* = 0);

    /**
     * Constructs a new RWAuditStreamBuffer that passes bytes into the
     * \b std::ostream on which it is constructed from the \b std::ostream
     * that has the RWAuditStreamBuffer instance as its \b std::streambuf. A
     * typical use would be to count or examine the bytes being output to a
     * file through a stream derived from RWvostream. The third parameter
     * to the constructor allows you to supply storage for the byte
     * count. It is optional.
     */
    RWAuditStreamBuffer(std::streambuf*,
                        RWauditFunction = 0,
                        void* = 0);

    /**
     * Provides the count of bytes seen so far.
     */
    operator unsigned long() {
        return seenBytes_;
    }

    /**
     * Resets the count of bytes seen so far. Returns the current count.
     */
    unsigned long reset(unsigned long val = 0ul) {
        unsigned long ret = seenBytes_;
        seenBytes_ = val;
        return ret;
    }

    std::streambuf* setbuf(char*, std::streamsize) {
        return 0;
    }

protected:

    virtual std::streambuf::int_type underflow();
    virtual std::streambuf::int_type overflow(int i);

    virtual std::streamsize xsgetn(char*,
                                   std::streamsize);

    std::streamsize xsputn(const char*,
                           std::streamsize);
};

#endif /* RW_TOOLS_AUDIT_STREAMBUFFER_H */
