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
#include <semaphore.h>
/* Global variable declaration for producer consumer */

extern int n;
extern semaphore produced;
extern semaphore consumed;
/* Function Prototype */

void consumer(int count);
void producer(int count);
#endif
