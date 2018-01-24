#ifndef RW_TOOLS_GORDVEC_H
#define RW_TOOLS_GORDVEC_H



/**********************************************************************
 *
 * RWGOrderedVector(val): <generic.h> style Ordered Vector of type val
 * To use this class you must declare and implement the 
 * base class RWGVector(val), as well as the ordered collection itself.
 * Example: To declare and implement an ordered vector of doubles:
 *   #include <rw/gordvec.h>
 *   declare(RWGVector,double)          // Declare the base class
 *   declare(RWGOrderedVector,double)   // Declare the ordered vector
 *   // In one and only one .cpp file you must put the following:
 *   implement(RWGVector,double)        // Implement the base class
 *   implement(RWGOrderedVector,double) // Implement the ordered vector
 *   // To use:
 *   void func(){
 *     RWGOrderedVector(double) ord;
 *     ord.insert(22.1);
 *     ord.insert(5.0);         // etc.
 *   }
 *
 **********************************************************************
 *
 * $Id: gordvec.h,v 1.1.1.1 2009/10/14 14:55:20 manngar Exp $
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
#include <rw/gvector.h>

#  define RWGOrderedVector(val) name2(val,RWGOrderedVector)

#if defined(RW_NO_INLINE_CLASS_BY_VALUE)
#  define RWGORDVEC_PUSH_BODY ;
#else
#  define RWGORDVEC_PUSH_BODY {insert(item);}
#endif

#define RWGOrderedVectordeclare(val)                                          \
class RW_TOOLS_EXPORT_G RWGOrderedVector(val) : private RWGVector(val) {              \
public:                                                                       \
  RWGOrderedVector(val)(size_t capac=RW_DEFAULT_CAPACITY)                      \
    : RWGVector(val)(capac), nitems_(0) {;}                                   \
  val&          operator()(size_t i)       {return array_[i];}                \
  val           operator()(size_t i) const {return array_[i];}                \
  val&          operator[](size_t i)       {boundsCheck(i); return array_[i];}\
  val           operator[](size_t i) const {boundsCheck(i); return array_[i];}\
  void          clear()                    {nitems_=0;}                       \
  const val*    data() const               {return RWGVector(val)::data();}   \
  size_t        entries() const            {return nitems_;}                  \
  size_t        index(val) const;                                             \
  void          insert(val item);                                             \
  void          insertAt(size_t indx, val item);                              \
  bool     isEmpty() const            {return nitems_==0;}               \
  size_t        length() const             {return nitems_;}                  \
  val           pop()                      {return array_[--nitems_];}        \
  void          push(val item)             RWGORDVEC_PUSH_BODY                \
  void          removeAt(size_t);                                             \
  void          resize(size_t N);                                             \
protected:                                                                    \
  void          boundsCheck(size_t) const;                                    \
  size_t        nitems_;                                                      \
};

#if defined(RW_NO_INLINE_CLASS_BY_VALUE)
#  define RWGORDVEC_PUSH_IMPL(val) void RWGOrderedVector(val)::push(val item) \
     {insert(item);}
#else
#  define RWGORDVEC_PUSH_IMPL(val)
#endif


#define RWGOrderedVectorimplement(val)                                  \
size_t RWGOrderedVector(val)::index(val item) const                     \
{                                                                       \
  for(size_t i=0; i<nitems_; i++) if (array_[i]==item) return i;        \
  return RW_NPOS;                                                       \
}                                                                       \
                                                                        \
RWGORDVEC_PUSH_IMPL(val)                                                \
                                                                        \
void RWGOrderedVector(val)::insert(val item)                            \
{                                                                       \
  if(nitems_>=RWGVector(val)::length()) /* Check for overflow */        \
    resize(nitems_ + RWDEFAULT_RESIZE);                                 \
  array_[nitems_++] = item;                                             \
}                                                                       \
                                                                        \
void RWGOrderedVector(val)::insertAt(size_t ipt, val item)              \
{                                                                       \
  if(nitems_>=RWGVector(val)::length()) /* Check for overflow */        \
    resize(nitems_ + RWDEFAULT_RESIZE);                                 \
                                                                        \
  for(size_t i=nitems_; i>ipt; i--) /*Slide right(could be expensive) */\
    array_[i] = array_[i-1];                                            \
                                                                        \
  nitems_++;                                                            \
  array_[ipt] = item;                                                   \
}                                                                       \
                                                                        \
void RWGOrderedVector(val)::removeAt(size_t ipt)                        \
{                                                                       \
  /* Slide left (could be very expensive): */                           \
  for(size_t i=ipt; i<nitems_-1; i++) array_[i] = array_[i+1];          \
  nitems_--;                                                            \
}                                                                       \
                                                                        \
void RWGOrderedVector(val)::resize(size_t N)                            \
{  if(N>nitems_) RWGVector(val)::reshape(N); }                          \
                                                                        \
void RWGOrderedVector(val)::boundsCheck(size_t i) const {               \
   if (i>=nitems_) {                                                    \
     if(RW_NPOS == i)                                                   \
       RWTHROW(RWBoundsErr(RWMessage(RWTOOL_NPOSINDEX)));               \
     else                                                               \
       RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEXERR,                   \
                                   (unsigned)i, (unsigned)nitems_)) );  \
   }                                                                    \
}

#endif /* RW_TOOLS_GORDVEC_H */

