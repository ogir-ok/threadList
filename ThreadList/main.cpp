#include "thread_List.h"

extern "C"
{

thread_List* thread_List_init()
{
	return new thread_List();
}

thread_List_Element* thread_List_Element_init()
{
	return new thread_List_Element();
}

void thread_List_delete(thread_List* obj)
{
	delete obj;
}

void thread_List_Element_delete(thread_List_Element* obj)
{
	delete obj;
}

}
