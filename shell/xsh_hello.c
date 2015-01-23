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

    if (nargs == 2)
    {
	printf("Hello %s, Welcome to the world of Xinu!!\n",args[1]);
	return OK;
    }

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
