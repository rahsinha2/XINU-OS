/**
 * @file producer.c
 *	rahsinha, vshekhar
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */
extern int n;
#include <prodcons.h>

void producer(int count)
{
	int i = 0;
	
	for ( i = 0; i <= count; i++ )	{
		n = i;
		printf("produced : %d\n", i);
	}	

}
