#include<future.h>
void enQ(tid_typ que[QSIZE], tid_typ tid, int* head, int* tail)
{
	if(*head == -1 && *tail == -1)	{
		que[0] = tid;
		*head = 0;
		*tail = 0;
		return;
	}	
	
	if (*tail >-1 && *tail<(QSIZE-1))	{
		(*tail)++;
		que[*tail] = tid;
		return;
	}
}
void deQ(tid_typ que[QSIZE],int * head, int * tail)
{
	if(*head == -1 && *tail == -1)	{
		return;
	}
	if(*head >-1 && *tail > -1 && *head == *tail && *head < QSIZE)	{
		que[*tail] = -1;
		*head = -1;
		*tail = -1;
		return;
	}	
	
	if (*head >-1 && *head<QSIZE)	{
		que[*head] = -1;
		(*head)++;
		return;
	}
}
int isQempty(int* head, int* tail)
{
	if(*head == -1 && *tail == -1)
		return 1;
	return 0;
}
