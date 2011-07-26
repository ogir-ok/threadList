/*
 * thread_List.h
 *
 *  Created on: Jul 22, 2011
 *      Author: Igor&Nastya
 */

#ifndef THREAD_LIST_H_
#define THREAD_LIST_H_

#include "thread_List_Element.h"

class thread_List
{
   thread_List_Element *pHead;		// pointer to the first element of list
   thread_List_Element *pTail;		// pointer to the last element of list
   pthread_rwlock_t rw_lock; 			//lock class
   pthread_mutex_t countElem_lock;	//mutex for variable countelement
   int countElem;// number elements in list
public:

   thread_List();
   ~thread_List();

   void addElToHead(int value);
   void addElToTail(int value);
   void addElLeft(int value, thread_List_Element *currentElement);
   void addElRight(int value, thread_List_Element *currentElement);
   void delEl(thread_List_Element *currentElement);
   int  showValue(thread_List_Element *currentElement);
   thread_List_Element* goLeft(thread_List_Element *currentElement);
   thread_List_Element* goRight(thread_List_Element *currentElement);
   void delAll();
   int getCount();
   thread_List_Element *getHead();
   thread_List_Element *getTail();
};

#endif /* THREAD_LIST_H_ */
