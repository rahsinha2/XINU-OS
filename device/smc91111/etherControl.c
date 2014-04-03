/**
 * @file etherControl.c
 */
/* Embedded Xinu, Copyright (C) 2008, 2013.  All rights reserved. */

#include <ether.h>
#include <network.h>
#include <string.h>
#include "smc91111.h"
#include <stdio.h>
/* Implementation of etherControl() for the smsc9512; see the documentation for
 * this function in ether.h.  */
devcall etherControl(device *devptr, int req, long arg1, long arg2)
{
    
    struct netaddr *addr;
    struct ether *ethptr;
    word regContent;
    
    ethptr = &ethertab[devptr->minor];

    switch (req)
    {
    /* Program MAC address into device. */
    case ETH_CTRL_SET_MAC:
        
        break;

    /* Get MAC address from device. */
    case ETH_CTRL_GET_MAC:
        smc_read_hwaddr(ethptr, (uchar*)arg1);   
        break;

    /* Enable or disable loopback mode.  */
    case ETH_CTRL_SET_LOOPBK:
        printf("\n Loop back mode: %d", ((bool)arg1 == TRUE) ? 1 : 0);
        regContent = smc_read_phy_register(ethptr, PHY_CNTL_REG);
        
        if((bool)arg1 == TRUE) 
           regContent|=PHY_CNTL_LPBK; 
        else
           regContent&= ~(PHY_CNTL_LPBK);
           
        smc_write_phy_register(ethptr, PHY_CNTL_REG, regContent); 
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
