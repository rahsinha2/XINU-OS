/**
 * @file etherRead.c
 */
/* Embedded Xinu, Copyright (C) 2013.  All rights reserved. */

#include "smc91111.h"
#include <bufpool.h>
#include <ether.h>
#include <interrupt.h>
#include <string.h>

/* Implementation of etherRead() for the smc91111; see the documentation for
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
    
    /* Wait for received packet to be available in the ethptr->in circular
     * queue.  */
    wait(ethptr->isema);

    /* Remove the received packet from the circular queue.  */
    pkt = ethptr->in[ethptr->istart];
    ethptr->istart = (ethptr->istart + 1) % ETH_IBLEN;
    ethptr->icount--;
    
    /* Copy the data from the packet buffer, being careful to copy at most the
     * number of bytes requested. */
    
    if (pkt->length < len)
    {
        len = pkt->length;
    }
    memcpy(buf, pkt->buf, len);

    /* Return the packet buffer to the pool, then return the length of the
     * packet received.  */
    buffree(pkt);
    restore(im);
    
    
    return len;
}
