/**
 * @file etherInit.c
 *
 * Initialization for the SMSC LAN9512 USB Ethernet Adapter.
 */
/* Embedded Xinu, Copyright (C) 2013.  All rights reserved. */

#include "smc91111.h"
#include <clock.h>
#include <ether.h>
#include <memory.h>
#include <platform.h>
#include <semaphore.h>
#include <stdlib.h>


/* Global table of Ethernet devices.  */
struct ether ethertab[NETHER];

devcall etherInit(device *devptr)
{
    struct ether *ethptr;
    
    /* Initialize the static `struct ether' for this device.  */
    ethptr = &ethertab[devptr->minor];
    bzero(ethptr, sizeof(struct ether));
    ethptr->dev = devptr;
    ethptr->state = ETH_STATE_DOWN;
    ethptr->mtu = ETH_MTU;
    ethptr->addressLength = ETH_ADDR_LEN;
    ethptr->isema = semcreate(0);
    if (isbadsem(ethptr->isema))
    {
        goto err;
    }
   
    return OK;


err_free_isema:
    semfree(ethptr->isema);
err:
    return SYSERR;
}
