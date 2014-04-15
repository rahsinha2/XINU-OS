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
#include <kernel.h>
#include <clock.h>

/**
 * @ingroup etherspecific
 *
 *
 *
 * @param ethptr
 *      
 */
static void smc_91111_tx_complete(struct ether *ethptr)
{   
    byte status;
    byte saved_packet;
	  byte packet_no;
	  word tx_status;
    
    SMC_SELECT_BANK(ethptr, 2);
    
    /* Acknowledge the IM_TX_EMPTY_INT*/
    SMC_SELECT_BANK(ethptr, 2);
  	SMC_outb (ethptr, IM_TX_EMPTY_INT, SMC91111_INT_REG);
  	
  	/* Read interrupt status from SMC91111 interrupt status register*/ 
  	SMC_SELECT_BANK(ethptr, 2);
    status = SMC_inb (ethptr, SMC91111_INT_REG);

    
    if(status & IM_TX_INT)
	  {
	      /* Error while xmit packet*/
	      
	      /* Save the packet*/
	      saved_packet = SMC_inb (ethptr, PN_REG);
	      
	      /* Retrieve the packet number*/
	      packet_no = SMC_inb (ethptr, RXFIFO_REG );
	      packet_no &= 0x7F;

	      /* If the TX FIFO is empty then nothing to do */
	      if ( packet_no & TXFIFO_TEMPTY )
		      return;

	      /* select this as the packet to read from */
	      SMC_outb (ethptr, packet_no,  PN_REG );

	      /* read the first word (status word) from this packet */

	      tx_status = SMC_inw (ethptr, SMC91111_DATA_REG );
#ifdef TRACE_NET
	      kprintf(" TX DONE STATUS: %4x \n", tx_status);
#endif
	      /* Increment the error count*/
	      ethptr->errors++;

	      if ( tx_status & TS_SUCCESS ) 
	      {
#ifdef TRACE_NET	      
		      kprintf("Successful packet caused interrupt \n");
#endif		      
	      }
	      
	      /* Acknowledge the IM_TX_INT*/
        SMC_SELECT_BANK(ethptr, 2);
	      SMC_outb (ethptr, IM_TX_INT, SMC91111_INT_REG);
	      
	      /* re-enable transmit */
	      SMC_SELECT_BANK(ethptr, 0);
	      SMC_outw ( ethptr, TCR_DEFAULT, TCR_REG );

	      /* kill the packet */
	      SMC_SELECT_BANK(ethptr, 2 );
	      SMC_outw ( ethptr, MC_FREEPKT, MMU_CMD_REG );

	      /* wait for MMU getting ready (low) */
	      while (SMC_inw (ethptr, MMU_CMD_REG) & MC_BUSY)
	      {
		      udelay (10);
	      }
        
        /* Restore Packet Number*/
	      SMC_outb (ethptr, saved_packet, PN_REG );
	  }
	  
	  if(status & IM_TX_EMPTY_INT)
	  {
	      /* Increment the txirq count for sucessfull xmit*/
	      ethptr->txirq++;
    }
    
    /* Disable the Interrupt MASK for IM_TX_INT and IM_*/
        SMC_DISABLE_INT(ethptr,(IM_TX_INT | IM_TX_EMPTY_INT));
	
	  return;
}
/**
 * @ingroup etherspecific
 *
 *
 *
 * @param ethptr
 *      
 */
static void smc_91111_rx_complete(struct ether *ethptr)
{
    struct ethPktBuffer *pkt;
    int packet_length;
    char incoming_packet[ETH_MTU];     /* I need this to get incoming packet from hardware*/
    
    /* Increment the rxirq count*/
    ethptr->rxirq++;
    
    /* Receive actual packet from the SMC91111 hardware*/
    packet_length = smc_rcv(ethptr, incoming_packet);                
    
    if (ethptr->icount == ETH_IBLEN)
    {
        /* No space to buffer another received packet. So drop the packet on the floor */
#ifdef TRACE_NET
        kprintf("SMSC91111: Tallying overrun\n");
#endif        
        ethptr->ovrrun++;
        return;
    }
    
    if((packet_length != 0) && packet_length <= ETH_MAX_PKT_LEN)
    {
        /* Packet recieved sucessfully from hardware*/
        
        /* Buffer the received packet.  */
        pkt = bufget(ethptr->inPool);
        pkt->buf = pkt->data = (uint8_t*)(pkt + 1);
        pkt->length = packet_length;
        
        /* copy packet into packet buffer*/
        memcpy(pkt->buf, incoming_packet, packet_length);
        
        /* Stash the packet in input buffer pool*/
        ethptr->in[(ethptr->istart + ethptr->icount) % ETH_IBLEN] = pkt;
        ethptr->icount++;

#ifdef TRACE_NET
        kprintf("SMC91111: Rx packet (length=%u, icount=%u)\n",
                              pkt->length, ethptr->icount);
#endif
        /* This may wake up a thread in etherRead().  */
        signal(ethptr->isema);
    }
    else
    {
        /* Unable to recieve pacekt from hardware  */
#ifdef TRACE_NET
        kprintf("SMC91111: Rx transfer failed\n");
#endif        
        ethptr->errors++;
    }
}

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
    int flag; 
     
    ethptr = &ethertab[0];   /*Default ethernet driver*/
#ifdef TRACE_NET    
    kprintf("\n SMC91111: Interrrupt Generated ");  
#endif
    if(!ethptr)
    {
      return;
    }   
    
    /* Read the IM_TX_EMPTY_INT from MASK. If is enable then only execute the routine for interrupt else skip*/
    flag = smc_get_regbit(ethptr, 2, CONFIG_SMC91111_BASE, IM_REG, IM_TX_EMPTY_INT);
    
	  /* save PTR and PTR registers */    
    SMC_SELECT_BANK(ethptr, 2);
	  saved_pnr = SMC_inb( ethptr, PN_REG );
	  saved_ptr = SMC_inw( ethptr, PTR_REG );

again:	  
    
    /* Read interrupt status from SMC91111 interrupt status register*/
    SMC_SELECT_BANK(ethptr, 2);
    status = SMC_inb (ethptr, SMC91111_INT_REG);
    	  
	  if(status & IM_RCV_INT)
	  {
	    /* Handle the IM_RCV_INT*/
	    smc_91111_rx_complete(ethptr);

#ifdef TRACE_NET	    
	    kprintf("\n SMC91111: Receive Interrupt handled");
#endif	    
	    goto again;
	  }
	  else if ((status & IM_TX_EMPTY_INT) && flag )
	  {
      /* Handle IM_TX_EMPTY_INT */  
      smc_91111_tx_complete(ethptr);
      
      flag = 0;
#ifdef TRACE_NET       
      kprintf("\n SMC91111: Transmit Empty Interrupt handled");
#endif
      goto again;      
    }
	   else if(status & IM_EPH_INT)
	  {
#ifdef TRACE_NET	  
	    kprintf("\n SMC91111: Ethernet Protocol Handler Interrupt");
#endif	    
	    SMC_outb (ethptr, IM_EPH_INT, SMC91111_INT_REG);
	    goto again;	    
	  }
	  else if(status & IM_RX_OVRN_INT)
	  {
#ifdef TRACE_NET	  
	    kprintf("\n SMC91111: Receiver Overrun interrupt ");
#endif	    
	    SMC_outb (ethptr, IM_RX_OVRN_INT, SMC91111_INT_REG);
	    ethptr->ovrrun++;
	    goto again;	    
	  }
	  
	  /* restore previously saved registers */
	  SMC_outb( ethptr, saved_pnr, PN_REG );
    SMC_outw( ethptr, saved_ptr, PTR_REG );
}
