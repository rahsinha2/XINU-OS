/**
 * @file etherWrite.c
 */
/* Embedded Xinu, Copyright (C) 2013.  All rights reserved. */

#include "smc91111.h"
#include <bufpool.h>
#include <ether.h>
#include <interrupt.h>
#include <string.h>
#include <stdio.h>

/* Implementation of etherWrite() for the SMSC LAN9512; see the documentation
 * for this function in ether.h.  */
devcall etherWrite(device *devptr, const void *buf, uint len)
{
    struct ether *ethptr;
    char *buffer;
    int retLen;
    
    ethptr = &ethertab[devptr->minor];
    if (ethptr->state != ETH_STATE_UP ||
        len < ETH_HEADER_LEN || len > ETH_HDR_LEN + ETH_MTU)
    {
        return SYSERR;
    }

    /* Get a buffer for the packet.  (This may block.)  */
    buffer = bufget(ethptr->outPool);
    
    memcpy(buffer, buf, len);
    
    /* Actual hardware send smc91111*/
    retLen = smc_send(ethptr, (void *)buffer, (int)len);
    
    /* Free the buffer*/
    buffree(buffer);
    
    /* If return length is 0 then something went wrong before packet can be put in trasmit queue, 
    I need to record error */
    if(retLen == 0)
    {
        ethptr->errors++;
    } 
    
    return retLen;
}
