/**
 * @file etherControl.c
 */
/* Embedded Xinu, Copyright (C) 2008, 2013.  All rights reserved. */

#include <ether.h>
#include <network.h>
#include <string.h>
#include "smc91111.h"

/* Implementation of etherControl() for the smsc9512; see the documentation for
 * this function in ether.h.  */
devcall etherControl(device *devptr, int req, long arg1, long arg2)
{
    
    struct netaddr *addr;
    struct ether *ethptr;
    uint8_t *macaddr;
    int i;
    
    ethptr = &ethertab[devptr->minor];

    switch (req)
    {
    /* Program MAC address into device. */
    case ETH_CTRL_SET_MAC:
        
        break;

    /* Get MAC address from device. */
    case ETH_CTRL_GET_MAC:
        macaddr = &arg1;
        SMC_SELECT_BANK(ethptr, 1);
        for (i = 0; i < 6; ++i)
		          macaddr[i] = SMC_inb(ethptr, (ADDR0_REG + i));
        break;

    /* Enable or disable loopback mode.  */
    case ETH_CTRL_SET_LOOPBK:
        printf("\n Seeting device to loop back mode");
       
        SMC_SELECT_BANK (ethptr, 0);
        SMC_outw (ethptr, ((bool)arg1 == TRUE) ? TCR_EPH_LOOP : 0 , TCR_REG);
        break;

    /* Get link header length. */
    case NET_GET_LINKHDRLEN:
        return ETH_HDR_LEN;

    /* Get MTU. */
    case NET_GET_MTU:
        return ETH_MTU;

    /* Get hardware address.  */
    case NET_GET_HWADDR:
        addr = (struct netaddr *)arg1;
        addr->type = NETADDR_ETHERNET;
        addr->len = ETH_ADDR_LEN;
        return etherControl(devptr, ETH_CTRL_GET_MAC, (long)addr->addr, 0);

    /* Get broadcast hardware address. */
    case NET_GET_HWBRC:
        addr = (struct netaddr *)arg1;
        addr->type = NETADDR_ETHERNET;
        addr->len = ETH_ADDR_LEN;
        memset(addr->addr, 0xFF, ETH_ADDR_LEN);
        break;

    default:
        return SYSERR;
    }

    return OK;
}
