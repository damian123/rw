#ifndef RW_TOOLS_STAT_H
#define RW_TOOLS_STAT_H

/**********************************************************************
 *
 * $Id: //tools/13/rw/stat.h#1 $
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

// C library
#include <sys/types.h>
#include <sys/stat.h>

#if !defined(_WIN32)
#  include <dirent.h>
#endif

/**
 * @ingroup file_system_classes
 *
 * @brief Provides file statistical information in a portable manner
 *
 * The  class RWFileStat  provides file statistical information
 * in a portable manner. RWFileStat is:
 *
 * -  not Assignable
 * -  not Copy Constructible
 * -  not Comparable
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/stat.h>
 * RWFileStat st;
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
struct RW_TOOLS_SYMBOLIC RWFileStat {
#if defined(_WIN32) || defined(DOXYGEN)
    /**
     * Type of the underlying data structure.
     *
     * @conditional
     * This type is used on Windows platforms.
     */
    typedef struct _stat stat_type;

    /**
     * Type for the number of hard links.
     *
     * @conditional
     * This type is used on Windows platforms.
     */
    typedef short nlink_type;

    /**
     * Type for the mode.
     *
     * @conditional
     * This type is used on Windows platforms.
     */
    typedef unsigned short mode_type;
#endif

#if !defined(_WIN32)
    /**
     * Type of the underlying data structure.
     *
     * @conditional
     * This type is used on non-Windows platforms.
     */
    typedef struct stat  stat_type;

    /**
     * Type for the number of hard links.
     *
     * @conditional
     * This type is used on non-Windows platforms.
     */
    typedef nlink_t nlink_type;

    /**
     * Type for the mode.
     *
     * @conditional
     * This type is used on non-Windows platforms.
     */
    typedef mode_t  mode_type;
#endif

    /**
     * Type for the \c device number (integer).
     */
    typedef dev_t device_type;

    /**
     * Type for the \c inode number (integer).
     */
    typedef ino_t inode_type;

    /**
     * Type for the offset (integer).
     */
    typedef off_t offset_type;

    // Ctors
    /**
     * Constructs the object by using a stat function on the filename
     * given in \a s. The call succeeds or throws an exception of type
     * <tt>(or derived of) RWInternalErr </tt>. The error can be inspected
     * by looking at \c errno. If the filename indicates a symbolic link,
     * it uses a stat function on the target of the symbolic link rather than
     * the symbolic link itself when the parameter \a follow_links is \c
     * true (default). Otherwise, it uses a stat function on the symbolic
     * link.
     */
    explicit RWFileStat(const char* s, bool follow_links = true);

    // Inode, devs, links, size, blocks, etc.
    /**
     * Returns the \c inode number for the file.
     */
    inode_type inode() const;

    /**
     * Returns the number or hard links for the file.
     */
    nlink_type nlink() const;

    /**
     * Returns the \c size of the file.
     */
    offset_type size() const;

    /**
     * Returns the \c device number for the file.
     */
    device_type dev() const;

    // Times
    /**
     * Returns the time of the last access.
     */
    time_t atime() const;

    /**
     * Returns the time of the last modification.
     */
    time_t mtime() const;

    /**
     * Returns the time of the last status change.
     */
    time_t ctime() const;

    /**
     * Returns \c true if the file is a \c symlink. This member function
     * always returns \c false for objects of type RWFileStat.
     */
    bool isSymlink() const;

    /**
     * Returns \c true if the file is a regular file.
     */
    bool isFile() const;

    /**
     * Returns \c true if the file is a block device.
     */
    bool isBlock() const;

    /**
     * Returns \c true if the file is a directory.
     */
    bool isDirectory() const;

    /**
     * Returns \c true if the file is a character device.
     */
    bool isCharacter() const;

    /**
     * Returns \c true if the file is a \c FIFO named \a pipe.
     */
    bool isFifo() const;

    // Accessors for underlying mode
    /**
     * Returns an integer which is a  bit mask value indicating the
     * type of the file and the permissions for it.
     */
    mode_type mode() const;

    // Accessors for underlying stat structure
    /**
     * Returns a pointer to the underlying type. The type is a struct
     * \c stat but it may change in the future. Using the returned type
     * directly is not supported.
     */
    const stat_type* stat() const;

protected:

    RWFileStat(const RWFileStat&);
    RWFileStat& operator= (const RWFileStat&);

    void init(const char*, bool);

protected:

    // underlying stat struct data
    stat_type stat_;
};

#endif // RW_TOOLS_STAT_H
