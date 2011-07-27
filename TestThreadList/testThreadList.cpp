//============================================================================
// Name        : testThreadList.cpp
// Author      : Garagatyy Igor
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <pthread.h>
#include <dlfcn.h>
#include "thread_List.h"
#include "thread_List_Element.h"




#define EXEC_COUNT 10
#define INPUT_FILE "/home/user/test.in"
#define OUTPUT_FILE "/home/user/test.out"

thread_List *list = new thread_List();
FILE* output;
pthread_t  *addtohead;
pthread_t  *addtotail;
pthread_t  *addelleft;
pthread_t  *addelright;
pthread_t  *delel;
pthread_t  *showvalue;
pthread_t  *delall;
pthread_mutex_t sync;
int randel()
{
	pthread_mutex_lock(&sync);
	srand(time(NULL));
	int a=rand()%100;
    pthread_mutex_unlock(&sync);
    return a;
}

void* faddtohead(void* arg)
{
    for (int iter=0;iter<EXEC_COUNT;iter++)
    {
	    int a=randel();
	    pthread_mutex_lock(&sync);
        fprintf(output,"try addToHead:%d\n",a);
        pthread_mutex_unlock(&sync);
        list->addElToHead(a);
        pthread_mutex_lock(&sync);
        fprintf(output,"addToHead: %d \n",a);
        pthread_mutex_unlock(&sync);
    }
    return (void*)0;
}

void* faddtotail(void* arg)
{
    for (int iter=0;iter<EXEC_COUNT;iter++)
    {
        int a=randel();
    	pthread_mutex_lock(&sync);
        fprintf(output,"try addToTail:%d\n",a);
        pthread_mutex_unlock(&sync);
        list->addElToTail(a);
        pthread_mutex_lock(&sync);
        fprintf(output,"addToTail: %d \n",a);
        pthread_mutex_unlock(&sync);
    }
    return (void*)0;
}

void* faddelleft(void* arg)
{
	int iter=0;
	for (iter=0;iter<EXEC_COUNT;iter++)
	{
		int a=randel();
		int count =list->getCount();
		if (count>0)
		{
			int pos=a%(count-1)+1;
			thread_List_Element* el;
			el=list->getHead();
			for (int i=0;i<pos;i++)
			el=list->goRight(el);

			pthread_mutex_lock(&sync);
			fprintf(output,"try addElLeft: %d on position: %d  \n",a,pos);
			pthread_mutex_unlock(&sync);
			list->addElLeft(a,el);

			pthread_mutex_lock(&sync);
			fprintf(output,"addElLeft: %d on position: %d  \n",a,pos);
			pthread_mutex_unlock(&sync);
		}
		else
		{
			pthread_mutex_lock(&sync);
			fprintf(output,"addElLeft: could not do it because there is no elements in list\n");
			pthread_mutex_unlock(&sync);
		}
	}
	return (void*)0;
}

void* faddelright(void* arg)
{
	int iter=0;
	for (iter=0;iter<EXEC_COUNT;iter++)
	{
		int count =list->getCount();
		if (count>0)
		{
			int a=randel();
			int pos=a%(count-1)+1;
			thread_List_Element* el;
			el=list->getHead();
			for (int i=0;i<pos;i++)
			el=list->goRight(el);
			pthread_mutex_lock(&sync);
			fprintf(output,"try addElRight: %d on position: %d  \n",a,pos);
			pthread_mutex_unlock(&sync);
			list->addElRight(a,el);
			pthread_mutex_lock(&sync);
			fprintf(output,"addElRight: %d on position: %d  \n",a,pos);
			pthread_mutex_unlock(&sync);
		}
		else
		{
			pthread_mutex_lock(&sync);
			fprintf(output,"addElRight: could not do it because there is no elements in list\n");
			pthread_mutex_unlock(&sync);
		}
	}
	return (void*)0;
}

void* fdelel(void* arg)
{
	int iter;
	for (iter=0;iter<EXEC_COUNT;iter++)
	{
		int count =list->getCount();
		if (count>0)
		{
			int pos = 1;
			pthread_mutex_lock(&sync);
			fprintf(output,"try delEl:%d\n",pos);
			pthread_mutex_unlock(&sync);

			thread_List_Element* el=list->getHead();
			list->delEl(el);

			pthread_mutex_lock(&sync);
			fprintf(output,"delEl:%d\n",pos);
			pthread_mutex_unlock(&sync);
			}
		else
		{
			pthread_mutex_lock(&sync);
			fprintf(output,"delEl: could not do it because there is no elements in list\n");
			pthread_mutex_unlock(&sync);
		}
	}
	return (void*)0;
}
void* fshowvalue(void* arg)
{
	int iter;
	for (iter=0;iter<EXEC_COUNT;iter++)
	{
		int count = list->getCount();
		if (count>0)
		{
			int a=randel();
			int pos=a%(count-1)+1;
			thread_List_Element *el;
			el=list->getHead();
			for (int i=0;i<pos;i++)
			list->goRight(el);
			pthread_mutex_lock(&sync);
			fprintf(output,"try showValue on position:%d\n",pos);
			pthread_mutex_unlock(&sync);
			int res =list->showValue(el);
			pthread_mutex_lock(&sync);
			fprintf(output,"showValue:%d on position:%d\n",res,pos);
			pthread_mutex_unlock(&sync);
		}
		else
		{
			pthread_mutex_lock(&sync);
			fprintf(output,"showValue: could not do it because there is no elements in list\n");
			pthread_mutex_unlock(&sync);
		}
	}
	return (void*)0;
}

void* fdelall(void* arg)
{
	int iter;
	for (iter=0;iter<EXEC_COUNT;iter++)
	{
		fprintf(output,"try delAll\n");
		list->delAll();
		fprintf(output,"delAll:Ok\n");
	}
	return (void*)0;
}
int main()
{
    printf("Start\n");
    pthread_mutex_init(&sync,NULL);
    //thread_List* list = new thread_List();
    int i = 0;
    int listCount;
    int list_elem;
    void* res = NULL;

    FILE* input= fopen(INPUT_FILE,"r");
    output = fopen(OUTPUT_FILE,"w");
    printf("Creating start list\n");
    fscanf(input,"%d",&listCount);
    for (i=0;i<listCount;i++)
    {
        fscanf(input,"%d",&list_elem);
        list->addElToTail(list_elem);
    }
    printf("Start list created\n");

    printf("Creating AddElToHead threads\n");
    int addtohead_count;
    fscanf(input,"%d",&addtohead_count);
    addtohead = new pthread_t[addtohead_count];
    for (i=0;i<addtohead_count;i++)
    {
            pthread_create(&addtohead[i],NULL,faddtohead,NULL);
    }


    printf("Creating AddElToTail threads\n");
    int addtotail_count;
    fscanf(input,"%d",&addtotail_count);
    addtotail = new pthread_t[addtohead_count];
    for (i=0;i<addtotail_count;i++)
    {
            pthread_create(&addtotail[i],NULL,faddtotail,NULL);
    }


    printf("Creating AddElLeft threads\n");
    int addelleft_count;
    fscanf(input,"%d",&addelleft_count);
    addelleft = new pthread_t[addelleft_count];
    for (i=0;i<addelleft_count;i++)
    {
            pthread_create(&addelleft[i],NULL,faddelleft,NULL);
    }

    printf("Creating AddElRight threads\n");
    int addelright_count;
    fscanf(input,"%d",&addelright_count);
    addelright = new pthread_t[addelright_count];
    for (i=0;i<addelright_count;i++)
    {
            pthread_create(&addelright[i],NULL,faddelright,NULL);
    }


    int delel_count;
    fscanf(input,"%d",&delel_count);
    printf("Creating DelEl threads\n");
    delel = new pthread_t[delel_count];
    for (i=0;i<delel_count;i++)
    {
            pthread_create(&delel[i],NULL,fdelel,NULL);
    }

    printf("Creating ShowValue threads\n");
    int showvalue_count;
    fscanf(input,"%d",&showvalue_count);
    showvalue = new pthread_t[showvalue_count];
    for (i=0;i<showvalue_count;i++)
    {
            pthread_create(&showvalue[i],NULL,fshowvalue,NULL);
    }
    int delall_count=0;
        for (i=0;i<addtohead_count;i++)
    {
                pthread_join(addtohead[i],&res);
    }
        for (i=0;i<addtotail_count;i++)
    {
                pthread_join(addtotail[i],&res);
    }
        for (i=0;i<addelleft_count;i++)
    {
                pthread_join(addelleft[i],&res);
    }
        for (i=0;i<addelright_count;i++)
    {
                pthread_join(addelright[i],&res);
    }
        for (i=0;i<delel_count;i++)
    {
                pthread_join(delel[i],&res);
    }
    for (i=0;i<showvalue_count;i++)
    {
            pthread_join(showvalue[i],&res);
    }
    for (i=0;i<delall_count;i++)
    {
            pthread_join(delall[i],&res);
    }
    delete[]   addtohead;
    delete[]   addtotail;
    delete[]   addelleft;
    delete[]   addelright;
    delete[]   delel;
    delete[]   showvalue;
    delete[]   delall;
    pthread_mutex_destroy(&sync);
    fclose(output);
    printf ("Exit");
        return 0;
}
