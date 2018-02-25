#ifndef __RWTVORDVEC_H__
#define __RWTVORDVEC_H__

/*
 * RWTValOrderedVector<T>: Ordered collection of objects of type T.
 *
 * $Id: tvordvec.h,v 6.3 1994/07/12 19:58:19 vriezen Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 *
 * (c) Copyright 1989, 1990, 1991, 1992, 1993, 1994 Rogue Wave Software, Inc.
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, Rogue Wave Software, Inc., which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 * Notwithstanding any other lease or license that may pertain to, or
 * accompany the delivery of, this computer software and information, the
 * rights of the Government regarding its use, reproduction and disclosure
 * are as set forth in Section 52.227-19 of the FARS Computer
 * Software-Restricted Rights clause.
 * 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 52.227-7013.
 * 
 * This computer software and information is distributed with "restricted
 * rights."  Use, duplication or disclosure is subject to restrictions as
 * set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial
 * Computer Software-Restricted Rights (April 1985)."  If the Clause at
 * 18-52.227-74 "Rights in Data General" is specified in the contract,
 * then the "Alternate III" clause applies.
 *
 ***************************************************************************
 *
 *
 ***************************************************************************
 *
 * Stores a *copy* of the inserted item into the collection.
 *
 * Assumes that T has:
 *   - well-defined copy semantics (T::T(const T&) or equiv.);
 *   - well-defined assignment semantics (T::operator=(const T&) or equiv.);
 *   - well-defined equality semantics (T::operator==(const T&)).
 *
 * Note that while these are automatically defined for builtin types
 * (such as "int", "double", or any pointer), you may need to provide
 * appropriate operators for your own classes, particularly those with
 * constructors and/or pointers to other objects.
 *
 ***************************************************************************
 *
 * $Log: tvordvec.h,v $
 * Revision 6.3  1994/07/12  19:58:19  vriezen
 * Update Copyright notice
 *
 * Revision 6.2  1994/06/17  17:21:42  vriezen
 * Added virtual dtor or change dtor to virtual to enhance derivability and quiet some compilers.
 *
 * Revision 6.1  1994/04/15  19:48:25  vriezen
 * Move all files to 6.1
 *
 * Revision 2.11  1993/12/31  00:56:30  jims
 * ObjectStore version: add get_os_typespec() static member function
 *
 * Revision 2.10  1993/12/10  19:44:18  jims
 * ObjectStore version: Add explicit copy constructors to avoid problem
 * with the ones generated by the OSCC compiler
 *
 * Revision 2.9  1993/11/16  03:19:22  jims
 * Add RWExport modifier
 *
 * Revision 2.8  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 2.7  1993/04/09  19:35:45  keffer
 * Indexing is now done using size_t
 *
 * Revision 2.6  1993/03/24  01:29:51  keffer
 * Indexing operations now used unsigned
 *
 * Revision 2.5  1993/02/17  18:32:03  keffer
 * Now passes T's by const reference, rather than by value
 *
 * Revision 2.4  1993/02/12  00:18:50  keffer
 * Ported to the IBM xlC compiler
 *
 * Revision 2.3  1993/01/29  03:07:03  keffer
 * Increased code reuse between ordered and sorted vectors
 *
 * Revision 2.2  1993/01/28  02:01:34  keffer
 * Put definitions in a separate tvordvec.cc
 *
 *    Rev 1.0   02 Mar 1992 16:10:54   KEFFER
 * Initial revision.
 */

#include "rw/tvvector.h"

/****************************************************************
 *								*
 *	Declarations for RWTValOrderedVector<T>			*
 *								*
 ****************************************************************/

template <class T> class RWExport RWTValOrderedVector : private RWTValVector<T>
{

public:

  //Constructors:
  RWTValOrderedVector(size_t capac=RWDEFAULT_CAPACITY)
    : RWTValVector<T>(capac),
      nitems_(0)
  {;}

  virtual ~RWTValOrderedVector() {;}

  //Operators:
  T&		operator[](size_t i)       {return RWTValVector<T>::operator[](i);}
  T		operator[](size_t i) const {return RWTValVector<T>::operator[](i);}
  T&		operator()(size_t i)       {return RWTValVector<T>::operator()(i);}
  T		operator()(size_t i) const {return RWTValVector<T>::operator()(i);}

  //Member functions:
  void			append(const T& val)	{insertAt(nitems_,val);}
  T&			at(size_t i)		{return (*this)[i];}
  T			at(size_t i) const	{return (*this)[i];}
  void			clear()
	{nitems_=0; reshape(RWDEFAULT_CAPACITY);}

  RWBoolean		contains(const T& a) const {return index(a) != RW_NPOS;}
  const T*		data() const
	{return RWTValVector<T>::data();}
  size_t		entries() const		{return nitems_;}
  RWBoolean		find(const T& p, T& ret) const;
  T			first() const		{return (*this)(0);}
  virtual size_t	index(const T& p) const;
  virtual void		insert(const T& val)	{insertAt(nitems_,val);}
  void			insertAt(size_t, const T&);
  RWBoolean		isEmpty() const		{return nitems_==0;}
  T			last() const		{return (*this)(nitems_-1);}
  size_t		length() const		{return nitems_;}
  virtual size_t	occurrencesOf(const T&) const;
  void			prepend(const T& val)	{insertAt(0, val);}
  virtual RWBoolean	remove(const T& p);
  virtual size_t	removeAll(const T&);
  T			removeAt(size_t);
  T			removeFirst()		{return removeAt(0);}
  T			removeLast()		{return (*this)(--nitems_);}
  void			resize(size_t N)	{if(N>=nitems_) reshape(N);}

protected:

  size_t		nitems_;	// Number of items in the collection

};

#ifdef RW_COMPILE_INSTANTIATE
# include "rw/tvordvec.cc"
#endif

#endif	/* __RWTVORDVEC_H__ */