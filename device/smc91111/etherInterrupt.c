/**
 * @file     etherInterrupt.c
 *
 *
 * This file provides USB transfer completion callbacks for the SMSC LAN9512 USB
 * Ethernet Adapter.  These are roughly the equivalent of etherInterrupt() as
 * implemented in other Xinu Ethernet drivers, hence the filename, but there is
 * no actual etherInterrupt() function here because of how USB works.  The SMSC
 * LAN9512 cannot actually issue an interrupt by itself--- what actually happens
 * is that a USB transfer to or from it completes, thereby causing the USB Host
 * Controller to interrupt the CPU.  This interrupt is handled by the USB Host
 * Controller Driver, which will then call the callback function registered for
 * the USB transfer.
 */
/* Embedded Xinu, Copyright (C) 2013.  All rights reserved. */

#include "smc91111.h"
#include <bufpool.h>
#include <ether.h>
#include <string.h>



