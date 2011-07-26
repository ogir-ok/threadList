/*
 * thread_List.cpp
 *
 *  Created on: Jul 22, 2011
 *      Author: Igor&Nastya
 */
#include "thread_List.h"

//constructor of List class
thread_List::thread_List()
{
	this->pHead = NULL;
	this->pTail = NULL;
	this->countElem = 0;
	pthread_rwlock_init(&(this->rw_lock),NULL);
	pthread_mutex_init(&(this->countElem_lock),NULL);
}

//destructor of List class
thread_List::~thread_List()
{
	pthread_mutex_destroy(&(this->countElem_lock));
	pthread_rwlock_destroy(&(this->rw_lock));
	delete this->pHead;
	delete this->pTail;
}

//Add an element to the head of the list
void thread_List::addElToHead(int value)
{
	pthread_rwlock_wrlock(&(this->rw_lock));
	//Add of element
	thread_List_Element *temp= new thread_List_Element();
	temp->value=value;
	temp->pNext=this->pHead;
	temp->pPrev=NULL;
	//Changing of next and previous element
	if (NULL != this->pHead)
		this->pHead->pPrev=temp;
	this->pHead=temp;
	if(NULL == this->pTail)
	{
		this->pTail=temp;
	}
	pthread_rwlock_unlock(&(this->rw_lock));
	pthread_mutex_lock(&(this->countElem_lock));//lock countElem
	this->countElem++;
	pthread_mutex_unlock(&this->countElem_lock);//unlock countElem
}

//Add an element to the tail of the list
void thread_List::addElToTail(int value)
{
	pthread_rwlock_wrlock(&(this->rw_lock));
	//Add of element
	thread_List_Element *temp= new thread_List_Element();
	temp->value=value;
	temp->pNext=NULL;
	temp->pPrev=this->pTail;
	//Changing of next and previous element
	if (NULL != this->pTail)
		this->pTail->pPrev=temp;
	this->pTail=temp;
	if(NULL == this->pHead)
	{
		this->pHead=temp;
	}
	pthread_rwlock_unlock(&(this->rw_lock));
	pthread_mutex_lock(&(this->countElem_lock));//lock countElem
	this->countElem++;
	pthread_mutex_unlock(&this->countElem_lock);//unlock countElem
}

void thread_List::addElLeft(int value, thread_List_Element *currentElement)
{
	if(currentElement == this->pHead)
	{
		this->addElToHead(value);
		return;
	}
	pthread_rwlock_wrlock(&(this->rw_lock));
	if (NULL != currentElement)//if Null the function won`t be engine
	{
		//Add of element
		thread_List_Element *temp= new thread_List_Element();
		temp->value=value;
		temp->pNext=currentElement;
		temp->pPrev=currentElement->pPrev;
		//Changing of next and previous element
		currentElement->pPrev->pNext=temp;
		currentElement->pPrev=temp;
		pthread_mutex_lock(&(this->countElem_lock));//lock countElem
		this->countElem++;
		pthread_mutex_unlock(&this->countElem_lock);//unlock countElem
	}
	pthread_rwlock_unlock(&(this->rw_lock));
}

void thread_List::addElRight(int value, thread_List_Element *currentElement)
{
	if(currentElement == this->pTail)
	{
		this->addElToTail(value);
		return;
	}
	pthread_rwlock_wrlock(&(this->rw_lock));
	if (NULL != currentElement)//if Null the function won`t be engine
	{
		//Add of element
		thread_List_Element *temp= new thread_List_Element();
		temp->value=value;
		temp->pPrev=currentElement;
		temp->pNext=currentElement->pNext;
		//Changing of next and previous element
		currentElement->pNext->pPrev=temp;
		currentElement->pNext=temp;
		pthread_mutex_lock(&(this->countElem_lock));//lock countElem
		this->countElem++;
		pthread_mutex_unlock(&this->countElem_lock);//unlock countElem
	}
	pthread_rwlock_unlock(&(this->rw_lock));
}

void thread_List::delEl(thread_List_Element *currentElement)//удаление переданного элемента
{
	pthread_rwlock_wrlock(&this->rw_lock);
	if (NULL!=currentElement)
	{
		if (this->pHead == currentElement)
		{
			this->pHead = currentElement->pNext;
			delete currentElement;
		}
		else
		if (this->pTail == currentElement)
		{
			this->pTail = currentElement->pPrev;
			delete currentElement;
		}
		else
		{
			currentElement->pNext->pPrev=currentElement->pPrev;
			currentElement->pPrev->pNext=currentElement->pNext;
			pthread_mutex_lock(&(this->countElem_lock));//lock countElem
			this->countElem--;
			pthread_mutex_unlock(&this->countElem_lock);//unlock countElem
			delete currentElement;
		}
	}
	pthread_rwlock_unlock(&(this->rw_lock));
}
int thread_List::showValue(thread_List_Element *currentElement)//вывод значения элемента
{
	pthread_rwlock_rdlock(&(this->rw_lock));
	int temp = 0;
    if (NULL != currentElement )
    {
		temp = currentElement->value;
    }
    pthread_rwlock_unlock(&(this->rw_lock));
    return temp;
}

thread_List_Element* thread_List::goLeft(thread_List_Element *currentElement)//перемещение влево от переданного
{
   	thread_List_Element *temp=NULL;
 	pthread_rwlock_rdlock(&(this->rw_lock));
    if (NULL != currentElement )
    {
    	temp=currentElement->pPrev;
       	if (NULL==temp)
    	{
    		temp=this->pTail;
    	}
    }
    pthread_rwlock_unlock(&(this->rw_lock));
    return temp;

}
thread_List_Element* thread_List::goRight(thread_List_Element *currentElement)// перемещение вправо отпереданого
{
	thread_List_Element *temp=NULL;
	pthread_rwlock_rdlock(&(this->rw_lock));
    if (NULL != currentElement )
    {

    	temp=currentElement->pNext;
    	if (NULL==temp)
    	{
    		temp=this->pHead;
    	}
    }
    pthread_rwlock_unlock(&(this->rw_lock));
    return temp;
 }
void thread_List::delAll()//удаление всех элементов
{
	pthread_rwlock_wrlock(&(this->rw_lock));
	thread_List_Element *temp= this->pHead;
	if (countElem>0)
	while (temp!=NULL)
	{
		temp=temp->pNext;
		delete temp->pPrev;
	}
	delete pTail;
	this->pHead=NULL;
	this->pTail=NULL;
	pthread_mutex_lock(&(this->countElem_lock));//lock countElem
	countElem=0;
	pthread_mutex_unlock(&(this->countElem_lock));//lock countElem

	pthread_rwlock_unlock(&(this->rw_lock));
}
int thread_List::getCount()
{
	int res=0;
	pthread_mutex_lock(&(this->countElem_lock));
	res = countElem;
	pthread_mutex_unlock(&(this->countElem_lock));
	return res;
}

thread_List_Element* thread_List::getHead()
{
	thread_List_Element *temp=NULL;
	pthread_rwlock_rdlock(&(this->rw_lock));
       	temp=this->pHead;
    pthread_rwlock_unlock(&(this->rw_lock));
    return temp;
}
thread_List_Element* thread_List::getTail()
{
	thread_List_Element *temp=NULL;
	pthread_rwlock_rdlock(&(this->rw_lock));
		temp=this->pTail;
    pthread_rwlock_unlock(&(this->rw_lock));
    return temp;
}
