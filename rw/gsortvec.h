#ifndef RW_TOOLS_GSORTVEC_H
#define RW_TOOLS_GSORTVEC_H



/**********************************************************************
 *
 * <generic.h> style sorted vector --- uses insertion sort
 * Declares a generic sorted vector.  This means that an item is inserted
 * so as to be after items "less than" itself, and before items 
 * "greater than" itself.  
 * To use this class you must declare and implement the 
 * base class RWGVector(val), as well as the sorted vector itself.
 *   #include <rw/gsortvec.h>
 *   declare(RWGVector,int)             // Declare the base class
 *   declare(RWGSortedVector,int)       // Declares a sorted vector of ints
 *   // In one and only one .cpp file you must put the following:
 *   implement(RWGVector,int)           // Implement the base class
 *   implement(RWGSortedVector,int)     // Implement the sorted vector
 * An instance of this vector could be used as follows:
 *   int compFun(const int* a, const int* b)    // Define a comparison function
 *      { return *a - *b; }
 *   RWGSortedVector(compFun, int) avec;        // Create a sorted vector
 *   avec.insert(3);                            // Insert an item
 *
 **********************************************************************
 *
 * $Id: gsortvec.h,v 1.1.1.1 2009/10/14 14:55:20 manngar Exp $
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

#  define RWGSortedVector(val) name2(val,RWGSortedVector)

#define RWGSortedVectordeclare(val)                                           \
class RW_TOOLS_EXPORT_G RWGSortedVector(val) : public RWGVector(val) {                \
public:                                                                       \
  RWGSortedVector(val)(int(*f)(const val*,const val*))                        \
  : RWGVector(val)(RW_DEFAULT_CAPACITY), nitems_(0), cf(f) {;}                 \
  RWGSortedVector(val)(int(*f)(const val*,const val*),size_t capac)           \
  : RWGVector(val)(capac),              nitems_(0), cf(f) {;}                 \
  val           operator()(size_t i) const {return array_[i];}                \
  val           operator[](size_t i) const {boundsCheck(i); return array_[i];}\
  size_t        entries() const            {return nitems_;}                  \
  size_t        index(val);                                                   \
  bool     insert(val);                                                  \
  size_t        length() const {return nitems_;}                              \
  void          removeAt(size_t);                                             \
  void          resize(size_t newCapacity);                                   \
protected:                                                                    \
  void          boundsCheck(size_t) const;                                    \
  size_t        nitems_; /* Number of slots in use */                         \
  int           (*cf)(const val*, const val*); /* Comparison function */      \
};

#define RWGSortedVectorimplement(val)                                         \
size_t RWGSortedVector(val)::index(val item)                                  \
{                                                                             \
  size_t idx;                                                                 \
  if(!rwBSearch(&item, array_, nitems_, sizeof(val), (RWcompare)cf, idx))     \
    return RW_NPOS;                                                           \
  while (idx && (*cf)(array_+idx-1, &item)==0) --idx;                         \
  return idx;                                                                 \
}                                                                             \
                                                                              \
bool RWGSortedVector(val)::insert(val item)                              \
{                                                                             \
  size_t idx;                                                                 \
  if(rwBSearch(&item, array_, nitems_, sizeof(val), (RWcompare)cf, idx))      \
    while (idx<nitems_ && (*cf)(array_+idx, &item) == 0) ++idx;               \
  else                                                                        \
    while (idx<nitems_ && (*cf)(array_+idx, &item) <  0) ++idx;               \
                                                                              \
  if( nitems_==npts_ ) resize(npts_+RWDEFAULT_RESIZE);                        \
  for(size_t j=nitems_; j>idx; j--) array_[j] = array_[j-1]; /* Slide right */\
  array_[idx] = item;                                                         \
  nitems_++;                                                                  \
  return true;                                                                \
}                                                                             \
                                                                              \
void RWGSortedVector(val)::removeAt(size_t ipt)                               \
{                                                                             \
  /* Slide left (could be very expensive): */                                 \
  for(register size_t i=ipt; i<nitems_-1; i++) array_[i] = array_[i+1];       \
  nitems_--;                                                                  \
}                                                                             \
                                                                              \
void RWGSortedVector(val)::resize(size_t N)                                   \
{  if(N>nitems_) RWGVector(val)::reshape(N); }                                \
                                                                              \
void RWGSortedVector(val)::boundsCheck(size_t i) const {                      \
   if (i>=nitems_) {                                                          \
     if(RW_NPOS == i)                                                         \
       RWTHROW(RWBoundsErr(RWMessage(RWTOOL_NPOSINDEX)));                     \
     else                                                                     \
       RWTHROW(RWBoundsErr(RWMessage(RWTOOL_INDEXERR,                         \
                                   (unsigned)i, (unsigned)nitems_)) );        \
   }                                                                          \
}

#endif /*RW_TOOLS_GSORTVEC_H*/
