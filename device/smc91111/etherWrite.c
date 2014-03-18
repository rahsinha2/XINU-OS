/**
 * @file etherWrite.c
 */
/* Embedded Xinu, Copyright (C) 2013.  All rights reserved. */

#include "smc91111.h"
#include <bufpool.h>
#include <ether.h>
#include <interrupt.h>
#include <string.h>
#include <usb_core_driver.h>

/* Implementation of etherWrite() for the SMSC LAN9512; see the documentation
 * for this function in ether.h.  */
devcall etherWrite(device *devptr, const void *buf, uint len)
{
    struct ether *ethptr;
   
    return len;
}
