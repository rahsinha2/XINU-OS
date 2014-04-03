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
#include <interrupt.h>


/* Global table of Ethernet devices.  */
struct ether ethertab[NETHER];

devcall etherInit(device *devptr)
{
    struct ether *ethptr;
    int i;
    
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
    
    /* Setting base address for IO operation on ethernet device*/
    ethptr->iobase = CONFIG_SMC91111_BASE;
    
    SMC_SELECT_BANK(ethptr, 1);
    
    /* Getting MAC address from EPROM on Ethernet Device*/
	  for (i = 0; i < 6; ++i)
		  ethptr->devAddress[i] = SMC_inb(ethptr, (ADDR0_REG + i));
		
		/* Enable the interrupt for Ethernet device*/  
		printf("\n Enabling the interrupt at %d",devptr->irq);
		interruptVector[devptr->irq] = devptr->intr;
    enable_irq(devptr->irq);
    return OK;


err_free_isema:
    semfree(ethptr->isema);
err:
    return SYSERR;
}
