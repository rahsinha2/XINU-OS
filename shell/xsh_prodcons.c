/*
 * @file     xsh_prodcons.c
 *	rahsinha , vshekhar
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#include <prodcons.h>
#include <future.h>
int n = 0;		/* Global Variable */
semaphore produced;	/* Semaphore */
semaphore consumed;
/**
 *
 * Shell command (prodcons).
 * @param nargs  number of arguments in args array
 * @param args   array of arguments
 * @return OK for success, SYSERR for syntax error
 */
shellcmd xsh_prodcons(int nargs, char *args[])
{
    int i = 0;
    int ch = 0;
    int count = 0;
    int future_flag = 0;
    char count_string[20];
    memset(count_string, 0, 20);
    consumed = semcreate(1);
    produced = semcreate(0);
    /* Output help, if '--help' argument was supplied */
    if (nargs == 2 && strcmp(args[1], "--help") == 0)
    {
        printf("Usage: %s\n\n", args[0]);
        printf("Description:\n");
        printf("\tProduce Consumer\n");
        printf("Options:\n");
        printf("\t--help\tdisplay this help and exit\n");
        return OK;
    }
    else if (nargs == 2 && strcmp(args[1], "-f") == 0)
 	future_flag = 1;
    /* Check for correct number of arguments */
    if (nargs > 2)
    {
        fprintf(stderr, "%s: too many arguments\n", args[0]);
        return SYSERR;
    }
    if (future_flag == 0)	{
	if (nargs == 2)
	{
		/* Check if its an integer */
		for ( i = 0; i < strlen(args[1]); i++ )	{
			if ( !isdigit(args[1][i]) ) 	{
        			fprintf(stderr, "%s: argument is not a valid positive integer\n", args[0]);
        			return SYSERR;
			}
		}	

		count = (int)atoi(args[1]);
    	}
    	else 
    	{
		count = 2000;
    	}	
		resume( create(producer, 1024, 20, "producer", 1, count) );
    		resume( create(consumer, 1024, 20, "consumer", 1, count) );
    }

    if (future_flag == 1)	{

	future *f1, *f2, *f3, *f4;
  	f1 = future_alloc(FUTURE_EXCLUSIVE);
  	f2 = future_alloc(FUTURE_EXCLUSIVE);
  	f3 = future_alloc(FUTURE_EXCLUSIVE);
  	f4 = future_alloc(FUTURE_EXCLUSIVE);
	 
	resume( create(future_cons, 1024, 20, "fcons1", 1, f1) );
  	resume( create(future_prod, 1024, 20, "fprod1", 1, f1) );
  	resume( create(future_cons, 1024, 20, "fcons2", 1, f2) );
  	resume( create(future_prod, 1024, 20, "fprod2", 1, f2) );
  	resume( create(future_cons, 1024, 20, "fcons3", 1, f3) );
  	resume( create(future_prod, 1024, 20, "fprod3", 1, f3) );        
  	resume( create(future_cons, 1024, 20, "fcons4", 1, f4) );
  	resume( create(future_prod, 1024, 20, "fprod4", 1, f4) );        
	
    }

    return OK;
}
