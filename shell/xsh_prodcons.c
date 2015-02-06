/*
 * @file     xsh_prodcons.c
 *	rahsinha , vshekhar
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#include <prodcons.h>

int n = 0;		/* Global Variable */
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
    int count = 0;
    char count_string[20];
    memset(count_string, 0, 20);
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
    /* Check for correct number of arguments */
    if (nargs > 2)
    {
        fprintf(stderr, "%s: too many arguments\n", args[0]);
        return SYSERR;
    }
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
    

    return OK;
}
