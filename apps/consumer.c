/**
 * @file consumer.c
 *	rahsinha, vshekhar
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */
extern int n;
#include <prodcons.h>

void consumer(int count)
{
	int i = 0;
	
	while (n<count)	{
		printf("consumed : %d\n", n);
	}
		printf("consumed : %d\n", n);
}
