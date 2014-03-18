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


/* Implementation of etherOpen() for the smsc9512; see the documentation for
 * this function in ether.h.  */
/**
 * @details
 *
 * SMSC LAN9512-specific notes:  as a work-around to use USB's dynamic device
 * model at the same time as Xinu's static device model, this function will
 * block until the corresponding USB device has actually been attached to the
 * USB.  Strictly speaking, there is no guarantee as to when this will actually
 * occur, even if the device is non-removable.
 */
devcall etherOpen(device *devptr)
{
    int retval = SYSERR;

   
    return retval;
}
