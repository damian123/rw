/*
 * RWInstanceManager
 *
 * $Id: instmgr.cpp,v 6.3 1994/07/18 20:51:00 jims Exp $
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
 * $Log: instmgr.cpp,v $
 * Revision 6.3  1994/07/18  20:51:00  jims
 * Fix typo
 *
 * Revision 6.2  1994/07/12  21:08:46  vriezen
 * Updated Copyright
 *
 * Revision 6.1  1994/04/15  19:06:50  vriezen
 * Move all files to 6.1
 *
 * Revision 1.10  1994/03/21  22:12:12  jims
 * Add support for POSIX threads
 *
 * Revision 1.9  1993/11/29  18:17:27  jims
 * Remove extra underbar from __OS2___
 *
 * Revision 1.8  1993/11/29  18:11:49  jims
 * Remove #include rwtsd.h from OS/2 section
 *
 * Revision 1.7  1993/11/18  01:20:44  jims
 * Move support for OS/2 MT from rwtsd to instmgr
 *
 * Revision 1.6  1993/11/17  02:12:50  myersn
 * add OS/2 support
 *
 * Revision 1.5  1993/09/10  03:59:57  keffer
 * Added RW_RCSID macro.
 *
 * Revision 1.4  1993/08/05  11:40:24  jims
 * Remove destructor; instmgr for WIN16 DLLs only
 *
 * Revision 1.3  1993/07/29  06:42:17  jims
 * Change RW_WIN32_API to __WIN32__
 *
 * Revision 1.2  1993/07/09  08:04:10  jims
 * Change FAR to rwfar; Port to Windows NT
 *
 * Revision 1.1  1993/04/12  12:03:21  jims
 * Initial revision
 *
 * Revision 2.3  1993/02/05  07:36:14  jims
 * Now uses rwtsd.dll (Rogue Wave Task Specific Data) library
 *
 * Revision 2.2  1992/11/20  18:51:56  keffer
 * DEBUG -> RWDEBUG
 *
 * Revision 2.1  1992/11/16  04:23:53  keffer
 * Reflects changes to RWModel
 *
 * Revision 2.0  1992/10/23  03:36:49  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.4   07 Jul 1992 16:22:22   KEFFER
 * Added additional preconditions.
 * 
 *    Rev 1.3   22 Jun 1992 17:54:24   KEFFER
 * Instance mgr now uses fallback position if no process has been registered
 * 
 *    Rev 1.2   18 Mar 1992 11:50:22   KEFFER
 * 
 *    Rev 1.1   18 Mar 1992 11:44:42   KEFFER
 * Changed nil to rwnil.
 * 
 *    Rev 1.0   12 Nov 1991 13:53:22   keffer
 * Initial revision.
 */

#include "rw/instmgr.h"
#ifdef __OS2__ 
#  include <stdlib.h>     // looking for size_t
#  include <stddef.h>     // looking for _threadid
#  include <limits.h>     // looking for ULONG_MAX
#endif

RW_RCSID("Copyright (C) Rogue Wave Software --- $RCSfile: instmgr.cpp,v $ $Revision: 6.3 $ $Date: 1994/07/18 20:51:00 $");

/****************************************
 *					*
 *   Win16-DLL / Multi-thread CODE	*
 *					*
 ****************************************/

#if (defined(__DLL__) && defined(__WIN16__)) || defined(RW_MULTI_THREAD)

void rwfar*
RWInstanceManager::addValue()
{
#ifdef RWDEBUG
  assert(currentValue()==rwnil);
#endif

  void rwfar* value = newValue();	 // init value provided by specializing class

#if defined(__DLL__) && defined(__WIN16__) || defined(__OS2__)
  RWSetTaskSpecificData(tsd_key, value);

#elif defined(sun)
  thr_setspecific(tsd_key, value);

#elif defined(RW_POSIX_THREADS)
  pthread_setspecific(tsd_key, value);

#elif defined(__WIN32__)
  TlsSetValue(tsd_key, value);

#endif 
  return value;
}

void rwfar*
RWInstanceManager::currentValue()
{
#if defined(__DLL__) && defined(__WIN16__) || defined(__OS2__)
  return RWGetTaskSpecificData(tsd_key); 

#elif defined(sun)
  void *value;
  thr_getspecific(tsd_key, &value);
  return value;

#elif defined(RW_POSIX_THREADS)
  void *value;
  pthread_getspecific(tsd_key, &value);
  return value;

#elif defined(__WIN32__)
  return TlsGetValue(tsd_key);

#endif
}

void
RWInstanceManager::freeValue()
{
  deleteValue(currentValue());	  // give value to specializing class to delete

// Now make sure the next call to currentValue() returns NULL:
#if defined(__DLL__) && defined(__WIN16__) || defined(__OS2__)
  RWReleaseTaskSpecificData(tsd_key);

#elif defined(sun)
  thr_setspecific(tsd_key, NULL); 

#elif defined(RW_POSIX_THREADS)
  pthread_setspecific(tsd_key, NULL); 

#elif defined(__WIN32__)
  TlsSetValue(tsd_key, NULL);
#endif
}

RWInstanceManager::RWInstanceManager()
{
// Get a key -- Thread lib should init each thread to have NULL associated
//              with this key.

#if defined(__DLL__) && defined(__WIN16__) || defined(__OS2__)
  tsd_key = RWGetTaskSpecificKey();

#elif defined(sun)
  thr_keycreate(&tsd_key, NULL);

#elif defined(RW_POSIX_THREADS)
  pthread_keycreate(&tsd_key, NULL);

#elif defined(__WIN32__)
  tsd_key = TlsAlloc();
#endif
}

// =========================================================================
//  For OS/2 Only:
// -------------------------------------------------------------------------
#ifdef __OS2__ 

#include <stdlib.h>     // looking for size_t
#include <stddef.h>	// looking for _threadid
#include <limits.h>	// looking for ULONG_MAX

// Note: this value must match the typedef for RWTSDKEY in instmgr.h
RWTSDKEY const MAX_KEY = ULONG_MAX;

/*
 * Threads obtain Keys through which they request that this facility
 * maintain thread-specific data. For each Key, we may store and
 * retrieve a single pointer---often to a structure holding several items
 * of interest---for each thread. As several threads may each hold
 * one or more keys, each piece of thread specific data is defined by
 * the combiniation of a thread ID (TID) and a Key. 
 * 
 * The global taskList (defined below) is a linked list with one
 * TaskEntry for each thread.  Each TaskEntry contains a linked-list
 * of KeyEntrys, where the actual task-specific data is held. 
 *
 * (As soon as enough compilers support templates, KeyEntry and
 *  TaskEntry should be instances of the same class template)
 */

/*
 * for some reason, the CSet compiler confuses unsigned, int and
 * unsigned long for TIDs. However the type TID is unsigned long, so
 * we'll try to use that...
 */
	 
struct KeyEntry {
  RWTSDKEY		hKey;
  void  *		taskSpecificData;
  KeyEntry		*next;

  KeyEntry		*find(RWTSDKEY h);
  void			add(KeyEntry *entry);
  KeyEntry		*remove(RWTSDKEY h, KeyEntry** victim);

  KeyEntry(RWTSDKEY h, void  *tsd)
    : hKey(h), taskSpecificData(tsd), next(0) { }  
};

struct TaskEntry  {
  TID 	hTask;
  KeyEntry	*keyList;
  TaskEntry	*next;              //   for chaining

  TaskEntry     *find(TID h);
  void		add(TaskEntry *entry);
  TaskEntry     *remove(TID h, TaskEntry** victim);

  TaskEntry(TID);
  ~TaskEntry();
};   	  

/*
 * Build a new task entry
 */
TaskEntry::TaskEntry(TID h) : hTask(h), keyList(0), next(0)
{
}
  
// before deleting a TaskEntry, delete all of its keys:
TaskEntry::~TaskEntry()
{
  KeyEntry *key = keyList;
  while (key) {
    KeyEntry *temp = key->next;
    delete key;
    key = temp;
  }
}

// ================================
//  TaskEntry member functions:
// --------------------------------

TaskEntry *TaskEntry::find(TID h)
{
  TaskEntry *entry = this;
  while (entry) {
    if (entry->hTask == h)
      return entry;
    else
      entry = entry->next;
  }
  return 0;
}

void TaskEntry::add(TaskEntry *newEntry)
{
  TaskEntry *entry = this;
  while (entry->next) entry = entry->next;
  entry->next = newEntry;
}	  

/*
 * Returns the list with the entry for h removed, sets *victim to
 * the entry which was removed, or 0 if no entry removed:
 */
TaskEntry *TaskEntry::remove(TID h, TaskEntry** victim)
{
  if (hTask == h) {
    *victim = this;
    return next;
  }

  TaskEntry *prev  = this;
  TaskEntry *entry = this->next;
  while (entry) {
    if (entry->hTask == h) {
      *victim = entry;
      prev->next = entry->next;
      return this;
    }
    else {
      prev = entry;
      entry = entry->next;
    }
  }	    	    	  
  *victim = 0;
  return this;  	  	  
}

// ================================
//  KeyEntry member functions:
// --------------------------------

KeyEntry *KeyEntry::find(RWTSDKEY h)
{
  KeyEntry *entry = this;
  while (entry) {
    if (entry->hKey == h)
      return entry;
    else
      entry = entry->next;
  }
  return 0;
}

void KeyEntry::add(KeyEntry *newEntry)
{
  KeyEntry *entry = this;
  while (entry->next) entry = entry->next;
  entry->next = newEntry;
}	  

/*
 * Returns the list with the entry for h removed, sets *victim to
 * the entry which was removed, or 0 if no entry removed:
 */
KeyEntry *KeyEntry::remove(RWTSDKEY h, KeyEntry** victim)
{
  if (hKey == h) {
    *victim = this;
    return next;
  }

  KeyEntry *prev  = this;
  KeyEntry *entry = this->next;

  while (entry) {
    if (entry->hKey == h) {
      *victim = entry;
      prev->next = entry->next;
      return this;
    }
    else {
      prev = entry;
      entry = entry->next;
    }
  }	    	    	  
  *victim = 0;
  return this;  	  	  
}

// =====================
//  Internal functions
// ---------------------

TaskEntry *taskList = 0;

// delete the record for a particular task & key:
static void releaseTaskSpecificData(RWTSDKEY hKey, TID hTask)
{
  TaskEntry *task;
  KeyEntry  *removedEntry;
    	
  if (!taskList || 0 == (task = taskList->find(hTask)) || !task->keyList)
    return;
  else {
    task->keyList = task->keyList->remove(hKey, &removedEntry);
    delete removedEntry;
  }
}

//========================================================================
// Public Functions
// -----------------------------------------------------------------------

/*
 * Return the next key in sequence, 0 if out of keys.  Key numbers
 * are not reused, but at one key/second it would take 136 years
 * to run out.
 */
RWTSDKEY
RWGetTaskSpecificKey()
{
  static RWTSDKEY key = 0;
  if (key == MAX_KEY)
    return 0;
  else
    return ++key;
}
      	      	
/*
 * Keep task specific data on behalf of the passed key
 * for the current task.  Call exitProc when the task exits.
 * Return 1 if successful, 0 if key already has TSD for this task.
 */
int
RWSetTaskSpecificData(RWTSDKEY hKey, void * tsd)
{
  TID hCurTask = RWTHREADID;
  TaskEntry *task = 0;
  KeyEntry *key = 0;

  if (taskList) { 
    task = taskList->find(hCurTask);
    if (task) {
      if (task->keyList) {
        key = task->keyList->find(hKey);
        if (key) // ERROR: key already has tsd for this task
          return 0;
        else {
          task->keyList->add(new KeyEntry(hKey, tsd));
        }
      }
      else { // task has no keyList, make new entry:
	task->keyList = new KeyEntry(hKey, tsd);
      }
    }
    else { // Current task not in taskList, make new entry:
      task = new TaskEntry(hCurTask);
      task->keyList = new KeyEntry(hKey, tsd);
      taskList->add(task);
    }	    
  }
  else { // No taskList yet, make new entry:
    taskList = new TaskEntry(hCurTask);
    taskList->keyList = new KeyEntry(hKey, tsd);
  }
  return 1;
}


// Return the data being kept under this key for the current task:
void *
RWGetTaskSpecificData(RWTSDKEY hKey)
{
  TaskEntry *task;
  KeyEntry *key;
  
  if (!taskList || 0 == (task = taskList->find( RWTHREADID ))) 
    return 0;    // Task not found
  else if (!task->keyList || 0 == (key = task->keyList->find(hKey))) 
    return 0;    // Key not found
  else
    return key->taskSpecificData;
}

/*
 * Stop keeping data under this key for the current task,
 * and return the data that was being kept (presumably so
 * it can be deleted or otherwise cleaned-up).
 */
void *
RWReleaseTaskSpecificData(RWTSDKEY hKey)
{
  void * tsd = RWGetTaskSpecificData(hKey);
  releaseTaskSpecificData(hKey, RWTHREADID);
  return tsd;
}

#endif /* __OS2__ */

#else	/* Neither Win16-DLL nor Multi-threaded */

#  error RWInstanceManager only to be used in Win16-DLL or MT Envirnonments

#endif	/* (__DLL__ &&__WIN16) || RW_MULTI_THREAD */
