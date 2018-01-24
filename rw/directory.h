#ifndef RW_TOOLS_DIRECTORY_H
#define RW_TOOLS_DIRECTORY_H

/**********************************************************************
 *
 * $Id: //tools/13/rw/directory.h#1 $
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

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning (disable: 4275 4786)
#endif

#include <rw/defs.h>
#include <rw/cstring.h>
#include <rw/tools/handlebody.h>

#if defined(_WIN32)
#  define RW_PATH_SEPARATOR '\\'
#else
#  define RW_PATH_SEPARATOR '/'
#endif

//
// Directory iterator primary template
//

template<typename T>
struct RWDirectoryIterator;


/**
 * @ingroup file_system_classes
 * @brief Provides a specialization of the class template for
 * <tt>const char*</tt>, with no iteration of subdirectories.
 *
 * The Essential Tools library provides two specializations of
 * the class template: this class, RWDirectoryIterator<const char*>,
 * for <tt>const char*</tt> which acts like
 * a flat iterator , i.e. no iteration of subdirectories; and an
 * RWCString specialization, RWDirectoryIterator<RWCString>,
 * to provide a recursive iteration of subdirectories. Neither follow
 * symbolic links.
 *
 * Both iterators are refinements of the Input Iterator concept.
 * One limitation is that they are usable only in single-pass
 * algorithms.
 *
 * The nature of the API for manipulating files and directories
 * requires special attention to synchronization issues. These classes
 * are:
 *
 * -  Assignable
 * -  Copy Constructible
 * -  Equality Comparable
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/directory.h>
 * RWDirectoryIterator<const char*> it;
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template<>
struct RW_TOOLS_SYMBOLIC RWDirectoryIterator<const char*>
        : public std::iterator <std::input_iterator_tag, char*>,
      public RWHandle {
    /**
     * Typedef for the type on which self is specialized.
     */
    typedef const char* value_type;

    /**
     * Constructs an iterator that compares equal with any past-the-end
     * iterator.
     */
    RWDirectoryIterator();

    /**
     * Constructs an iterator over the content of the directory name
     * provided by the argument \a cs.
     * @throw RWInternalErr or one of its derived types, thrown if
     * \a cs is not a directory.
     */
    explicit RWDirectoryIterator(const char* cs);

    /**
     * Constructs an iterator that is a copy of the iterator passed
     * as the argument. Dereferencing each yields identical results,
     * yet they will not compare equal. See the equality comparison
     * operators below.
     */
    RWDirectoryIterator(const RWDirectoryIterator& it);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed iterator takes ownership of the
     * data owned by \a it.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWDirectoryIterator(RWDirectoryIterator && it);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Assignment operator. Performs the assignment of the value of
     * the argument. Note that the objects will not compare equal unless
     * they are both past-the-end iterators.
     */
    RWDirectoryIterator&
    operator= (const RWDirectoryIterator&);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a it.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWDirectoryIterator&
    operator= (RWDirectoryIterator && it);
#endif // !RW_NO_RVALUE_REFERENCES    

    /**
     * Prefix increment operator. Returns a reference to this object.
     */
    RWDirectoryIterator& operator++ ();

    /**
     * Postfix increment operator. Returns an iterator that points
     * to the entry previous to the increment operation.
     */
    RWDirectoryIterator  operator++ (int);

    /**
     * Dereferencing operator. Returns the file name or \c 0 if the
     * iterator is a past-the-end iterator.
     */
    value_type operator* () const;

    // Equality-inequality operators
    /**
     * Equality operator. Tests equality between the two
     * RWDirectoryIterator<const char*> objects passed as parameters.
     * Only past-the-end iterators compare equal.
     */
    friend RW_TOOLS_SYMBOLIC bool
    operator== (const RWDirectoryIterator<const char*>&,
                const RWDirectoryIterator<const char*>&);

    /**
     * Inequality operator. Equivalent to logical negation of
     * corresponding equality operator.
     */
    friend RW_TOOLS_SYMBOLIC bool
    operator!= (const RWDirectoryIterator<const char*>&,
                const RWDirectoryIterator<const char*>&);

    /**
     * Swaps the data owned by self with the data owned by \a it.
     */
    void swap(RWDirectoryIterator& it);
};

bool RW_TOOLS_SYMBOLIC
operator== (const RWDirectoryIterator<const char*>&,
            const RWDirectoryIterator<const char*>&);

bool RW_TOOLS_SYMBOLIC
operator!= (const RWDirectoryIterator<const char*>&,
            const RWDirectoryIterator<const char*>&);

/**************************************************************************/

//
// RWDirectoryIterator RWCString specialization
//

/**
 * @ingroup file_system_classes
 * @brief Provides a specialization of the class template
 * for RWCString, with recursive iteration of subdirectories.
 *
 * The Essential Tools library provides two specializations of
 * the class template: this class, RWDirectoryIterator<RWCString>, for
 * RWCString with recursive iteration of subdirectories; and
 * RWDirectoryIterator<const char*> for <tt>const char*</tt> which
 * acts like a flat iterator , i.e. no iteration of subdirectories.
 * Neither follow symbolic links.
 *
 * Both iterators are refinements of the Input Iterator concept.
 * One limitation is that they are usable only in single-pass
 * algorithms.
 *
 * The nature of the API for manipulating files and directories
 * requires special attention to synchronization issues. These classes
 * are:
 *
 * -  Assignable
 * -  Copy Constructible
 * -  Equality Comparable
 *
 * @section synopsis Synopsis
 *
 * @code
 * #include <rw/directory.h>
 * RWDirectoryIterator<RWCString> it;
 * @endcode
 *
 * @section persistence Persistence
 *
 * None
 */
template<>
struct RW_TOOLS_SYMBOLIC RWDirectoryIterator<RWCString>
        : public std::iterator <std::input_iterator_tag, RWCString>,
      public RWHandle {
public:
    /**
     * Typedef for the type on which self is specialized.
     */
    typedef RWCString   value_type;

    typedef RWCString*  pointer_type;

    /**
     * Constructs an iterator that compares equal with any past-the-end
     * iterator.
     */
    RWDirectoryIterator();

    /**
     * Constructs an iterator over the content of the directory name
     * passed as argument \a cs.  The \a depth argument controls
     * how deep to iterate within the directory hierarchy. The default
     * is to behave as a flat iterator. Passing an argument value of \c -1
     * iterates to the full depth of the hierarchy.
     * @throw RWInternalErr or one of its derived types, thrown if the
     * argument is not a directory.
     */
    explicit RWDirectoryIterator(const char* cs, int depth = 0);

    /**
     * Constructs an iterator that is a copy of the iterator passed
     * as the argument. Dereferencing each iterator yields identical
     * results, yet they will not compare equal. See the equality comparison
     * operators.
     */
    RWDirectoryIterator(const RWDirectoryIterator& it);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move constructor. The constructed iterator takes ownership of the
     * data owned by \a it.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWDirectoryIterator(RWDirectoryIterator && it);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Assignment operator. Performs the assignment of the value of the argument.
     * Note that the objects will not compare equal unless they are both
     * past-the-end iterators.
     */
    RWDirectoryIterator&
    operator= (const RWDirectoryIterator&);

#if !defined(RW_NO_RVALUE_REFERENCES)
    /**
     * Move assignment. Self takes ownership of the data owned by \a it.
     *
     * @conditional
     * This method is only available on platforms with rvalue reference support.
     */
    RWDirectoryIterator&
    operator= (RWDirectoryIterator && it);
#endif // !RW_NO_RVALUE_REFERENCES

    /**
     * Destroys the object.
     */
    ~RWDirectoryIterator();

    /**
     * Prefix increment operator. Returns a reference to this object.
     */
    RWDirectoryIterator& operator++ ();

    /**
     * Postfix increment operator. Returns an iterator that points
     * to the entry previous to the increment operation.
     */
    RWDirectoryIterator  operator++ (int);

    /**
     * Dereferencing operator. Returns the file name.
     */
    value_type& operator* () const;

    pointer_type operator-> () const;

    // Equality-inequality operators
    /**
     * Equality operator. Tests equality between the two
     * RWDirectoryIterator<RWCString> objects passed as parameters.
     * Only past-the-end iterators compare equal.
     */
    friend bool RW_TOOLS_SYMBOLIC
    operator== (const RWDirectoryIterator<RWCString>&,
                const RWDirectoryIterator<RWCString>&);

    /**
     * Inequality operator. Equivalent to logical negation of
     * corresponding equality operator.
     */
    friend bool RW_TOOLS_SYMBOLIC
    operator!= (const RWDirectoryIterator<RWCString>&,
                const RWDirectoryIterator<RWCString>&);

    /**
     * Swaps the data owned by self with the data owned by \a it.
     */
    void swap(RWDirectoryIterator& it);

private:

    RWCString cache_;

};


bool RW_TOOLS_SYMBOLIC
operator== (const RWDirectoryIterator<RWCString>&,
            const RWDirectoryIterator<RWCString>&);

bool RW_TOOLS_SYMBOLIC
operator!= (const RWDirectoryIterator<RWCString>&,
            const RWDirectoryIterator<RWCString>&);

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif // RW_TOOLS_DIRECTORY_H
