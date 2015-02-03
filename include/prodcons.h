/*
 * @file prodcons.h
 *	Rahul Sinha
 *	Vimalendu Shekhar	
 */	
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#ifndef _PRODCONS_H_
#define _PRODCONS_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/* Global variable declaration for producer consumer */

extern int n;

/* Function Prototype */

void consumer(int count);
void producer(int count);
#endif
