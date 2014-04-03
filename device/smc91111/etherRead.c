/**
 * @file etherRead.c
 */
/* Embedded Xinu, Copyright (C) 2013.  All rights reserved. */

#include "smc91111.h"
#include <bufpool.h>
#include <ether.h>
#include <interrupt.h>
#include <string.h>

/* Implementation of etherRead() for the smsc9512; see the documentation for
 * this function in ether.h.  */
devcall etherRead(device *devptr, void *buf, uint len)
{
    irqmask im;
    struct ether *ethptr;
    struct ethPktBuffer *pkt;

    im = disable();

    /* Make sure device is actually up.  */
    ethptr = &ethertab[devptr->minor];
    if (ethptr->state != ETH_STATE_UP)
    {
        restore(im);
        return SYSERR;
    }
    
    
   return len;
}
