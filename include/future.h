#ifndef _FUTURE_H_
#define _FUTURE_H_
 
/* define states */
#define FUTURE_EMPTY	  0
#define FUTURE_WAITING 	  1         
#define FUTURE_VALID 	  2         

/* modes of operation for future*/
#define FUTURE_EXCLUSIVE  1	
#define FUTURE_SHARED  	  2	
#define FUTURE_QUEUE      3	

#define QSIZE 10

#include<stddef.h>
#include<interrupt.h>
#include<queue.h>
#include<myqueue.h>
typedef struct futent
{
   int value;		
   int flag;		
   int state;         	
   tid_typ tid;
   int head_getq;
   int tail_getq;
   int head_setq;
   int tail_setq;
   tid_typ set_queue[QSIZE];
   tid_typ get_queue[QSIZE];
} future;

/* Interface for system call */
future* future_alloc(int future_flags);
syscall future_free(future*);
syscall future_get(future*, int*);
syscall future_set(future*, int*);
uint future_prod(future *fut); 
uint future_cons(future *fut);
#endif /* _FUTURE_H_ */
