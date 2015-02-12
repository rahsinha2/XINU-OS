/**
 * @file consumer.c
 *	rahsinha, vshekhar
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */
//extern int n;
//extern semaphore produced;
//extern semaphore consumed;
#include <prodcons.h>

void consumer(int count)
{
	int i = 0;
	
	
	for ( i = 0; i <= count; i++ )  {
                wait(produced);
		printf("consumed : %d\n", n);
                signal(consumed);
        }


}
