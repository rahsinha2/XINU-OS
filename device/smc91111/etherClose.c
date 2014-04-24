/**
 * @file etherClose.c
 */
/* Embedded Xinu, Copyright (C) 2013.  All rights reserved. */

#include <ether.h>
#include <interrupt.h>
#include "smc91111.h"

/* Implementation of etherClose() for the smc91111; see the documentation for
 * this function in ether.h.  */

devcall etherClose(device *devptr)
{
    struct ether *ethptr;
    irqmask im;
    im = disable();
    
    ethptr = &ethertab[devptr->minor];
    if (ethptr->state == ETH_STATE_DOWN)
    {
       goto out_restore;
    } 
    
    /* Halt the Hardware*/
    /* It will clear the TCR register, RCR register and disable the inerrupts through IM register */
    smc_halt(ethptr);
    
    /*change ethernet state*/
    ethptr->state = ETH_STATE_DOWN;
    
    /* Disable the interrupt*/
    enable_irq(devptr->irq);

out_restore:    
    restore(im);
    return OK;
}