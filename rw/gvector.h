#ifndef RW_TOOLS_GVECTOR_H
#define RW_TOOLS_GVECTOR_H



/**********************************************************************
 *
 * <generic.h> style macro for a generic vector.
 *
 **********************************************************************
 *
 * $Id: gvector.h,v 1.1.1.1 2009/10/14 14:55:20 manngar Exp $
 *
 **********************************************************************
 *
 * Copyright (c) 1989-2009 Rogue Wave Software, Inc.  All Rights Reserved.
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
#include <rw/rwerr.h>
#include <rw/toolerr.h>

#  define RWGVector(val) name2(val,RWGVector)

/************************************************
 ************************************************
 *                                              *
 *              RWGVector declaration           *
 *                                              *
 ************************************************
 ************************************************/

#ifdef RW_NO_CONST_OVERLOAD

#define RWGVectordeclare(val)                                           \
class RW_TOOLS_EXPORT_G RWGVector(val) {                                        \
public:                                                                 \
  RWGVector(val)()         : npts_(0), array_(0)          {;}           \
  RWGVector(val)(size_t n) : npts_(n)                                   \
   { array_ = (0==n) ? 0 : new val[n]; }                                \
  RWGVector(val)(size_t n, val v);                                      \
  ~RWGVector(val)()     { if(array_) RWVECTOR_DELETE(npts_) array_; }   \
  RWGVector(val)(const RWGVector(val)&);                                \
  RWGVector(val)&               operator=(const RWGVector(val)&);       \
  RWGVector(val)&               operator=(val);                         \
  const val*    data() const            {return array_;}                \
  size_t        length() const          {return npts_;}                 \
  void          resize(size_t N)        {reshape(N);}                   \
  void          reshape(size_t);                                        \
  val&  operator()(size_t i)       {return array_[i];}                  \
  val   operator()(size_t i) const {return array_[i];}                  \
  val&  operator[](size_t i)       {boundsCheck(i); return array_[i];}  \
  val   operator[](size_t i) const {boundsCheck(i); return array_[i];}  \
protected:                                                              \
  void          boundsCheck(size_t i) const;                            \
  size_t        npts_;                                                  \
  val*          array_;                                                 \
};

#else /* !RW_NO_CONST_OVERLOAD */

#define RWGVectordeclare(val)                                           \
class RW_TOOLS_EXPORT_G RWGVector(val) {                                        \
public:                                                                 \
  RWGVector(val)()         : npts_(0), array_(0)   {;}                  \
  RWGVector(val)(size_t n) : npts_(n)                                   \
   { array_ = (0==n) ? 0 : new val[n]; }                                \
  RWGVector(val)(size_t n, val v);                                      \
  ~RWGVector(val)()     { if(array_) RWVECTOR_DELETE(npts_) array_; }   \
  RWGVector(val)(const RWGVector(val)&);                                \
  RWGVector(val)&               operator=(const RWGVector(val)&);       \
  RWGVector(val)&               operator=(val);                         \
  val&  operator()(size_t i)       {return array_[i];}                  \
  val   operator()(size_t i) const {return array_[i];}                  \
  val&  operator[](size_t i)       {boundsCheck(i); return array_[i];}  \
  val   operator[](size_t i) const {boundsCheck(i); return array_[i];}  \
  const val*    data() const               {return array_;}             \
  size_t        length() const             {return npts_; }             \
  void          resize(size_t N)           {reshape(N);   }             \
  void          reshape(size_t);                                        \
protected:                                                              \
  void          boundsCheck(size_t i) const;                            \
  size_t        npts_;                                                  \
  val*          array_;                                                 \
};

#endif /* RW_NO_CONST_OVERLOAD */


/************************************************
 ************************************************
 *                                              *
 *      RWGVector implementation                *
 *                                              *
 ************************************************
 ************************************************/

#define RWGVectorimplement(val)                                         \
RWGVector(val)::RWGVector(val)(size_t n, val ival)                      \
{                                                                       \
  register size_t i = npts_ = n;                                        \
  register val* dst = array_ = (0==n) ? 0 :                             \
    new val[i];                                                         \
  while(i--) *dst++ = ival;                                             \
}                                                                       \
                                                                        \
RWGVector(val)::RWGVector(val)(const RWGVector(val)& a)                 \
{                                                                       \
  register size_t i= npts_ = a.npts_;                                   \
  register val* dst = array_ = (0==i) ? 0 : new val[i];                 \
  register val* src = a.array_;                                         \
  while (i--) *dst++ = *src++;                                          \
}                                                                       \
                                                                        \
RWGVector(val)& RWGVector(val)::operator=(const RWGVector(val)& a)      \
{                                                                       \
  if(this != &a){                                                       \
    if(array_)                                                          \
      RWVECTOR_DELETE(npts_) array_;    /* Disconnect from old array */ \
    register size_t i = npts_ = a.npts_;                                \
    register val* dst = array_ = (0==i) ? 0 : new val[i];               \
    register val* src = a.array_;                                       \
    while (i--) *dst++ = *src++;                                        \
  }                                                                     \
  return *this;                                                         \
}                                                                       \
                                                                        \
RWGVector(val)& RWGVector(val)::operator=(val v)                        \
{                                                                       \
  register size_t i = npts_;                                            \
  while (i--) array_[i] = v;                                            \
  return *this;                                                         \
}                                                                       \
                                                                        \
void RWGVector(val)::reshape(size_t N)                                  \
{                                                                       \
  if(N==npts_)return;                                                   \
  val* newArray = (0==N) ? 0 : new val[N];                              \
  register size_t i = (N<=npts_) ? N:npts_;                             \
  register val* src = array_;                                           \
  register val* dst = newArray;                                         \
  RW_TRYSTART                                                           \
  while (i--) *dst++ = *src++;                                          \
  RW_TRYEND(RWVECTOR_DELETE(N) newArray) /*N never 0 here */            \
  if(array_) RWVECTOR_DELETE(npts_) array_;                             \
  array_ = newArray;                                                    \
  npts_ = N;                                                            \
}                                                                       \
                                                                        \
void RWGVector(val)::boundsCheck(size_t i) const {                      \
  if (i>=npts_) {                                                       \
    if(RW_NPOS == i)                                                    \
      RWTHROW(RWBoundsErr(RWMessage(RWTOOL_NPOSINDEX)));                \
    else                                                                \
      RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEXERR,                    \
                          (unsigned)i, (unsigned)npts_)) );             \
  }                                                                     \
}

#endif /* RW_TOOLS_GVECTOR_H */
