/**
 * @file etherWrite.c
 */
/* Embedded Xinu, Copyright (C) 2013.  All rights reserved. */

#include "smc91111.h"
#include <bufpool.h>
#include <ether.h>
#include <interrupt.h>
#include <string.h>


/* Implementation of etherWrite() for the SMSC LAN9512; see the documentation
 * for this function in ether.h.  */
devcall etherWrite(device *devptr, const void *buf, uint len)
{
    struct ether *ethptr;
    int retLen;
    
    ethptr = &ethertab[devptr->minor];
    if (ethptr->state != ETH_STATE_UP ||
        len < ETH_HEADER_LEN || len > ETH_HDR_LEN + ETH_MTU)
    {
        return SYSERR;
    }

    /* Get a buffer for the packet.  (This may block.)  */
    //req = bufget(ethptr->outPool);
    
    retLen = smc_send(ethptr, buf, len);
    return retLen;
}
