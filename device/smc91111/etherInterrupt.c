/**
 * @file     etherInterrupt.c
 *
 */
/* Embedded Xinu, Copyright (C) 2013.  All rights reserved. */

#include "smc91111.h"
#include <bufpool.h>
#include <ether.h>
#include <string.h>
#include <stdio.h>

/**
 * @ingroup etherspecific
 *
 * Decode and handle hardware interrupt request from ethernet device.
 */
interrupt etherInterrupt(void)
{

    struct ether *ethptr;
    byte saved_pnr;
  	word saved_ptr;
  	byte status;
  
    ethptr = &ethertab[0];   /*Default ethernet driver*/
    
    printf("\n SMC91111: Interrrupt Generated ");  
    if(!ethptr)
    {
      return 0;
    }   
    
    SMC_SELECT_BANK(ethptr, 2);
	  /* save PTR and PTR registers */
	  saved_pnr = SMC_inb( ethptr, PN_REG );
	  saved_ptr = SMC_inw( ethptr, PTR_REG );
	  
	  status = SMC_inb (ethptr, SMC91111_INT_REG);
  
again:	  

	  if(status & IM_RCV_INT)
	  {
	    printf("\n SMC91111: Receive Interrupt");
	    SMC_outb (ethptr, IM_RCV_INT, SMC91111_INT_REG);
	    goto again;
	  }
	  else if(status & IM_TX_INT)
	  {
	    printf("\n SMC91111: Transmit Interrupt");
	    SMC_outb (ethptr, IM_TX_INT, SMC91111_INT_REG);
	    goto again;
	  }
	  else if(status & IM_ALLOC_INT)
	  {
	    printf("\n SMC91111: Allocation interrupt");
	    SMC_outb (ethptr, IM_ALLOC_INT, SMC91111_INT_REG);
	    goto again;
	  }
	  else if(status & IM_EPH_INT)
	  {
	    printf("\n SMC91111: Ethernet Protocol Handler Interrupt");
	    SMC_outb (ethptr, IM_EPH_INT, SMC91111_INT_REG);
	    goto again;	    
	  }
	  else if(status & IM_RX_OVRN_INT)
	  {
	    printf("\n SMC91111: Receiver Overrun interrupt ");
	    SMC_outb (ethptr, IM_RX_OVRN_INT, SMC91111_INT_REG);
	    goto again;	    
	  }
	  
	  /* restore previously saved registers */
	  SMC_outb( ethptr, saved_pnr, PN_REG );
    SMC_outw( ethptr, saved_ptr, PTR_REG );
}
