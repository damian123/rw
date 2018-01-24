#ifndef MYALLOC_H
#define MYALLOC_H

/***************************************************************************
 *
 * myalloc.h
 *
 * $Id: //math/13/rw/math/rwalloc.h#3 $
 *
 * Copyright (c) 1991-2015 Rogue Wave Software, Inc.  All Rights Reserved.
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
 **************************************************************************/

#include <rw/mathdefs.h>
#include <rw/dcomplex.h>

#include <rw/edefs.h>
#include <rw/rstream.h>

#ifdef RW_MATH_NO_NEW_HEADER
#  include <stddef.h>
#  include <stdlib.h>
#  include <limits.h>
#else
#  include <cstddef>
#  include <cstdlib>
#  include <climits>
#  if defined(RW_USING_SUNPERF) && !defined(sparc)
// Solaris x86 uses memalign and it is only available in
// stdlib.h. On Solaris, stdlib.h includes cstdlib and brings
// its functions into global scope.
#    include <stdlib.h>
#  endif
#endif
#include <new>
#include <iostream>

#if defined(RW_USING_MKL)
#  include <mkl/mkl_service.h>
#endif

#if !defined(RW_MATH_NO_STD_NEW_HANDLER)
#  define RW_MATH_NEW_HANDLER std::set_new_handler
#else
#  define RW_MATH_NEW_HANDLER set_new_handler
#endif

template <class T>
inline T* rw_math_allocate(int size, T*)
{
    RW_MATH_NEW_HANDLER(0);
#if defined(RW_USING_MKL)
    T* tmp = (T*)(mkl_malloc((size * sizeof(T)), RW_MKL_ALIGNMENT_BOUNDARY));
#elif defined(RW_USING_SUNPERF) && !defined(sparc)
    T* tmp = (T*)(memalign(RW_SPL_ALIGNMENT_BOUNDARY, (size * sizeof(T))));
#else
    T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
#endif
    if (tmp == 0) {
        RW_MATH_EXIT(1);
    }
    return tmp;
}

template <class T>
inline T* rw_math_allocate(long size, T*)
{
    RW_MATH_NEW_HANDLER(0);
#if defined(RW_USING_MKL)
    T* tmp = (T*)(mkl_malloc((size * sizeof(T)), RW_MKL_ALIGNMENT_BOUNDARY));
#elif defined(RW_USING_SUNPERF) && !defined(sparc)
    T* tmp = (T*)(memalign(RW_SPL_ALIGNMENT_BOUNDARY, (size * sizeof(T))));
#else
    T* tmp = (T*)(::operator new((unsigned long)(size * sizeof(T))));
#endif
    if (tmp == 0) {
        RW_MATH_EXIT(1);
    }
    return tmp;
}

template <class T>
inline void rw_math_deallocate(T* buffer)
{
#if defined(RW_USING_MKL)
    mkl_free(buffer);
#else
    //free(buffer);
    ::operator delete(buffer);
#endif
}

template <class T>
inline void rw_math_destroy(T* pointer)
{
    pointer->~T();
}

template <class T>
#ifndef  RW_NO_INLINED_WHILES
inline void rw_math_destroy(T* start, T* end)
#else
void rw_math_destroy(T* start, T* end)
#endif
{
    T* pointer = start;
    while (pointer != end) {
        rw_math_destroy(pointer);
        pointer++;
    }
}

inline void rw_math_destroy(DComplex*) {}
inline void rw_math_destroy(char*) {}
inline void rw_math_destroy(UChar*) {}
#ifndef RW_NO_SCHAR
inline void rw_math_destroy(SChar*) {}
#endif
inline void rw_math_destroy(short*) {}
inline void rw_math_destroy(unsigned short*) {}
inline void rw_math_destroy(int*) {}
inline void rw_math_destroy(unsigned int*) {}
inline void rw_math_destroy(long*) {}
inline void rw_math_destroy(unsigned long*) {}
inline void rw_math_destroy(float*) {}
inline void rw_math_destroy(double*) {}
inline void rw_math_destroy(char**) {}
inline void rw_math_destroy(UChar**) {}
inline void rw_math_destroy(short**) {}
inline void rw_math_destroy(unsigned short**) {}
inline void rw_math_destroy(int**) {}
inline void rw_math_destroy(unsigned int**) {}
inline void rw_math_destroy(long**) {}
inline void rw_math_destroy(unsigned long**) {}
inline void rw_math_destroy(float**) {}
inline void rw_math_destroy(double**) {}

inline void rw_math_destroy(char*, char*) {}
inline void rw_math_destroy(UChar*, UChar*) {}
inline void rw_math_destroy(short*, short*) {}
inline void rw_math_destroy(unsigned short*, unsigned short*) {}
inline void rw_math_destroy(int*, int*) {}
inline void rw_math_destroy(unsigned int*, unsigned int*) {}
inline void rw_math_destroy(long*, long*) {}
inline void rw_math_destroy(unsigned long*, unsigned long*) {}
inline void rw_math_destroy(float*, float*) {}
inline void rw_math_destroy(double*, double*) {}
inline void rw_math_destroy(char**, char**) {}
inline void rw_math_destroy(UChar**, UChar**) {}
inline void rw_math_destroy(short**, short**) {}
inline void rw_math_destroy(unsigned short**, unsigned short**) {}
inline void rw_math_destroy(int**, int**) {}
inline void rw_math_destroy(unsigned int**, unsigned int**) {}
inline void rw_math_destroy(long**, long**) {}
inline void rw_math_destroy(unsigned long**, unsigned long**) {}
inline void rw_math_destroy(float**, float**) {}
inline void rw_math_destroy(double**, double**) {}


template <class T>
class RWDefaultAllocator
{
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    pointer allocate(size_type n) {
#if defined(RW_USING_MKL)
        T* tmp = (T*)(mkl_malloc((n * sizeof(T)), RW_MKL_ALIGNMENT_BOUNDARY));
#elif defined(RW_USING_SUNPERF) && !defined(sparc)
        T* tmp = (T*)(memalign(RW_SPL_ALIGNMENT_BOUNDARY, (n * sizeof(T))));
#else
        //T* tmp = (T*)(malloc(n * sizeof(T)));
        T* tmp = (T*)(::operator new((size_t)(n * sizeof(T))));
#endif
        if (tmp == 0) {
            RW_MATH_EXIT(1);
        }
        return tmp;
    }
    void deallocate(pointer p) {
        ::rw_math_deallocate(p);
    }
    pointer address(reference x) {
        return (pointer)&x;
    }
    const_pointer const_address(const_reference x) {
        return (const_pointer)&x;
    }
    size_type init_page_size() {
        return rwmax(size_type(1), size_type(2048 / sizeof(T)));
    }
    size_type max_size() const {
        return rwmax(size_type(1), size_type(UINT_MAX / sizeof(T)));
    }
};

RW_MATH_SPECIALIZE
class RWDefaultAllocator<void>
{
public:
    typedef void* pointer;
};

template <typename T, typename A = RWDefaultAllocator<T> >
class RWTWorkBuffer
{
public:
    RWTWorkBuffer(size_t size) {
        data_ = size ? allocator_.allocate(size) : 0;
    }

    ~RWTWorkBuffer() {
        allocator_.deallocate(data_);
    }

    T* data() {
        return data_;
    }

    const T* data() const {
        return data_;
    }

    T& operator[](size_t pos) {
        return data_[pos];
    }

    const T& operator[](size_t pos) const {
        return data_[pos];
    }

    void swap(RWTWorkBuffer& rhs) {
        rw_swap(allocator_, rhs.allocator_);
        rw_swap(data_, rhs.data_);
    }

private:
    // declared but not defined to prevent use.
    RWTWorkBuffer(const RWTWorkBuffer& rhs);
    RWTWorkBuffer& operator=(const RWTWorkBuffer& rhs);

    A allocator_;
    T* data_;
};


#endif
