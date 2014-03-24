
#include "smc91111.h"
#include <clock.h>
#include <ether.h>
#include <stdio.h>

/*CREDIT*/
/*
 * Below functions are copies from U-boot smc91111 drivers and altered for xinu on QEMU
*/


/* Only one release command at a time, please */
static inline void smc_wait_mmu_release_complete (struct ether *dev)
{
	int count = 0;

	/* assume bank 2 selected */
	while (SMC_inw (dev, MMU_CMD_REG) & MC_BUSY) {
		udelay (1);	/* Wait until not busy */
		if (++count > 200)
			break;
	}
}

/*
 . Function: smc_reset( void )
 . Purpose:
 .	This sets the SMC91111 chip to its normal state, hopefully from whatever
 .	mess that any other DOS driver has put it in.
 .
 . Maybe I should reset more registers to defaults in here?  SOFTRST  should
 . do that for me.
 .
 . Method:
 .	1.  send a SOFT RESET
 .	2.  wait for it to finish
 .	3.  enable autorelease mode
 .	4.  reset the memory management unit
 .	5.  clear all interrupts
 .
*/

void smc_reset (struct ether *dev)
{

	/* This resets the registers mostly to defaults, but doesn't
	   affect EEPROM.  That seems unnecessary */
	SMC_SELECT_BANK (dev, 0);
	SMC_outw (dev, RCR_SOFTRST, RCR_REG);

	/* Setup the Configuration Register */
	/* This is necessary because the CONFIG_REG is not affected */
	/* by a soft reset */

	SMC_SELECT_BANK (dev, 1);
#if defined(CONFIG_SMC91111_EXT_PHY)
	SMC_outw (dev, CONFIG_DEFAULT | CONFIG_EXT_PHY, CONFIG_REG);
#else
	SMC_outw (dev, CONFIG_DEFAULT, CONFIG_REG);
#endif


	/* Release from possible power-down state */
	/* Configuration register is not affected by Soft Reset */
	SMC_outw (dev, SMC_inw (dev, CONFIG_REG) | CONFIG_EPH_POWER_EN,
		CONFIG_REG);

	SMC_SELECT_BANK (dev, 0);

	/* this should pause enough for the chip to be happy */
	udelay (10);

	/* Disable transmit and receive functionality */
	SMC_outw (dev, RCR_CLEAR, RCR_REG);
	SMC_outw (dev, TCR_CLEAR, TCR_REG);

	/* set the control register */
	SMC_SELECT_BANK (dev, 1);
	SMC_outw (dev, CTL_DEFAULT, CTL_REG);

	/* Reset the MMU */
	SMC_SELECT_BANK (dev, 2);
	smc_wait_mmu_release_complete (dev);
	SMC_outw (dev, MC_RESET, MMU_CMD_REG);
	while (SMC_inw (dev, MMU_CMD_REG) & MC_BUSY)
		udelay (1);	/* Wait until not busy */

	/* Note:  It doesn't seem that waiting for the MMU busy is needed here,
	   but this is a place where future chipsets _COULD_ break.  Be wary
	   of issuing another MMU command right after this */

	/* Disable all interrupts */
	SMC_outb (dev, 0, IM_REG);
}

/*
 . Function: smc_enable
 . Purpose: let the chip talk to the outside work
 . Method:
 .	1.  Enable the transmitter
 .	2.  Enable the receiver
 .	3.  Enable interrupts
*/
void smc_enable(struct ether *dev)
{
	SMC_SELECT_BANK( dev, 0 );
	/* see the header file for options in TCR/RCR DEFAULT*/
	SMC_outw( dev, TCR_DEFAULT, TCR_REG );
	SMC_outw( dev, RCR_DEFAULT, RCR_REG );

	/* clear MII_DIS */
  /*	smc_write_phy_register(PHY_CNTL_REG, 0x0000); */
}

