#ifndef _MYQUEUE_H_
#define _MYQUEUE_H_

#include <kernel.h>
/* Queue function prototypes */

void enQ(tid_typ queue[QSIZE], tid_typ tid, int * head, int * tail);
void deQ(tid_typ queue[QSIZE], int * head, int * tail);
int isQempty(int * head, int * tail);
#endif                          /* _MYQUEUE_H_ */
