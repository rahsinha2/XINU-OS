/**
 * @file etherOpen.c
 *
 * Code for opening a SMSC LAN9512 USB Ethernet Adapter device.
 */
/* Embedded Xinu, Copyright (C) 2013.  All rights reserved. */

#include "smc91111.h"
#include <bufpool.h>
#include <ether.h>
#include <stdlib.h>
#include <string.h>
#include <interrupt.h>


/* Implementation of etherOpen() for the smsc9512; see the documentation for
 * this function in ether.h.  */


devcall etherOpen(device *devptr)
{
    struct ether *ethptr;
    irqmask im;
    int retval = SYSERR;

    im = disable();

    /* Fail if device is not down.  */
    ethptr = &ethertab[devptr->minor];
    if (ethptr->state != ETH_STATE_DOWN)
    {
        goto out_restore;
    }

    /* Create buffer pool for Tx transfers.  */
    ethptr->outPool = bfpalloc(ETH_MAX_PKT_LEN,
                               SMC_MAX_TX_REQUESTS);
    if (ethptr->outPool == SYSERR)
    {
        goto out_restore;
    }

    /* Create buffer pool for Rx packets.  */
    ethptr->inPool = bfpalloc(sizeof(struct ethPktBuffer) + ETH_MAX_PKT_LEN,
                              SMC_MAX_RX_REQUESTS);
    if (ethptr->inPool == SYSERR)
    {
        goto out_free_out_pool;
    }

    /* Open and Initialize the SMC91111*/
    smc_init(ethptr);
    
    /* Success!  Set the device to ETH_STATE_UP. */
    ethptr->state = ETH_STATE_UP;
    retval = OK;
    goto out_restore;

out_free_out_pool:
    bfpfree(ethptr->outPool);
out_restore:
    restore(im);
    return retval;
}
