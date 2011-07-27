/*
 * thread_List_Element.h
 *
 *  Created on: Jul 22, 2011
 *      Author: Igor&Nastya
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef THREAD_LIST_ELEMENT_H_
#define THREAD_LIST_ELEMENT_H_

class thread_List_Element
{
public:
   int value;//value of an elment
   thread_List_Element *pNext;// pointer to the next element of list
   thread_List_Element *pPrev;// pointer to the previous element of list
   thread_List_Element();
   ~thread_List_Element();
};

#endif /* THREAD_LIST_ELEMENT_H_ */
