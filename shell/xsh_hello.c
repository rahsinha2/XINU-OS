/*
 * @file     xsh_hello.c
 *	rahsinha , vshekhar
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#include <stddef.h>
#include <stdio.h>
#include <string.h>

/**
 * @ingroup shell
 *
 * Shell command (date).
 * @param nargs  number of arguments in args array
 * @param args   array of arguments
 * @return OK for success, SYSERR for syntax error
 */
shellcmd xsh_hello(int nargs, char *args[])
{
    int i = 0;
    int j = 0;
    /* Output help, if '--help' argument was supplied */
    if (nargs == 2 && strcmp(args[1], "--help") == 0)
    {
        printf("Usage: %s\n\n", args[0]);
        printf("Description:\n");
        printf("\tDisplays input string\n");
        printf("Options:\n");
        printf("\t--help\tdisplay this help and exit\n");
        return OK;
    }

    char * buffer = (char*) malloc (strlen (args[1]) * 2);
    memset(buffer, 0, strlen (args[1]) * 2); 
    for (i = j = 0; i<strlen (args[1]); i++) {
   	if ((args[1][i] == '"' && i==0) || (args[1][i] == '"' && i == (strlen (args[1])-1))) 
	    continue;
	buffer[j++] = args[1][i];
    }

    if (nargs == 2)
    {
	printf("Hello %s, Welcome to the world of Xinu!!\n",buffer);
	return OK;
    }
	
    free(buffer);
    /* Check for correct number of arguments */
    if (nargs > 2)
    {
        fprintf(stderr, "%s: too many arguments\n", args[0]);
        return SYSERR;
    }
    if (nargs < 2)
    {
        fprintf(stderr, "%s: too few arguments\n", args[0]);
        return SYSERR;
    }

   
    return OK;
}
