#ifndef RW_TOOLS_MESSAGE_H
#define RW_TOOLS_MESSAGE_H



/**********************************************************************
 *
 * Message string facility
 *
 **********************************************************************
 *
 * $Id: //tools/13/rw/message.h#1 $
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
#include <rw/tools/pointer.h>

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

struct RWCatMsg {
    const char* domainName_;      // Called "Catalog Name" by catgets()
    int         msgNumber_;       // Message number

    // Default string (catgets()) or lookup key (gettext())
    const char* msg_;
};

typedef const RWCatMsg* RWMsgId;


class RW_TOOLS_SYMBOLIC RWMessage
{
public:
    RWMessage(const char*);
    RWMessage(RWMsgId ...);
    RWMessage(const RWMessage&);
    ~RWMessage();
    RWMessage& operator=(const RWMessage&);
    const char* str() const {
        return msg_.get();
    }
    size_t length() const {
        return length_;
    }
private:
    RWTScopedPointer<char[]> msg_;
    size_t length_;
};

RW_TOOLS_SYMBOLIC const char* rwMsgLookup(RWMsgId msgId);

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif /* RW_TOOLS_MESSAGE_H */
