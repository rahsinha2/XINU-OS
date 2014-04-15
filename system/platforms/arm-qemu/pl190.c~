/**
 * @file pl190.c
 *
 * Driver for the PrimeCell Vectored Interrupt Controller (VIC) (PL190).
 *
 * This was written using the official manual from ARM Ltd.:
 *
 *  "PrimeCell Vectored Interrupt Controller (PL190) Technical Reference
 *  Manual", revision r1p2.
 *
 * Note: this simple driver currently does not actually use vectored interrupts
 * (where the controller provides the address of the interrupt service routine
 * in VICVECTADDR so that software doesn't have to check VICIRQSTATUS).  Nor
 * does it allow setting anything as a FIQ (Fast Interrupt Request) rather than
 * a regular IRQ.
 */
/* Embedded Xinu, Copyright (C) 2014.  All rights reserved. */

#include <interrupt.h>
#include <stdint.h>


/* Primary Interrupt controller of ARM*/
static volatile struct {
    uint32_t VICIRQSTATUS;            /* +0x000 */
    uint32_t VICFIQSTATUS;            /* +0x004 */
    uint32_t VICRAWINTR;              /* +0x008 */
    uint32_t VICINTSELECT;            /* +0x00C */
    uint32_t VICINTENABLE;            /* +0x010 */
    uint32_t VICINTENCLEAR;           /* +0x014 */
    uint32_t VICSOFTINT;              /* +0x018 */
    uint32_t VICSOFTINTCLEAR;         /* +0x01C */
    uint32_t VICPROTECTION;           /* +0x020 */
    uint32_t VIC_0x024_reserved[3];   /* +0x024 */
    uint32_t VICVECTADDR;             /* +0x030 */
    uint32_t VICDEFVECTADDR;          /* +0x034 */
    uint32_t VIC_0x038_reserved[50];  /* +0x038 */
    uint32_t VICVECTADDR_REGS[16];    /* +0x100 */
    uint32_t VIC_0x140_reserved[48];  /* +0x140 */
    uint32_t VICVECTCNTL_REGS[16];    /* +0x200 */
    uint32_t VIC_0x240_reserved[872]; /* +0x240 */
    uint32_t VICPERIPHID_REGS[4];     /* +0xFE0 */
    uint32_t VICPCELLID_REGS[4];      /* +0xFF0 */
} * const regs = (void*)0x10140000;

/* Secondary Interrupt Controller of ARM. Peripherals external to the development
chip have their interrupts routed to the PIC through the SIC_PICEnable register*/

static volatile struct {
    uint32_t SIC_STATUS;                        /* 0x10003000 Read Status of interrupt (after mask)*/
    uint32_t SIC_RAWSTAT;                       /* 0x10003004 Read Status of interrupt (before mask)*/
    uint32_t SIC_ENABLE;                        /* 0x10003008 Read Interrupt mask*/
    //SIC_ENSET;                                /* 0x10003008 Write Set bits in interrupt mask*/
    uint32_t SIC_ENCLR;                         /* 0x1000300C Write Clear bits in interrupt mask*/
    uint32_t SIC_SOFTINTSET;                    /* 0x10003010 Read/write Set software interrupt*/
    uint32_t SIC_SOFTINTCLR[3];                 /* 0x10003014 Write Clear software interrupt*/
    //SIC_PICENABLE;                            /* 0x10003020 Read Read status of pass-through mask
    //                                                (allows interrupt to pass directly to the
    //                                                primary interrupt controller) */
    uint32_t SIC_PICENSET;                      /* 0x10003020 Write Set interrupt pass through bits*/
    uint32_t SIC_PICENCLR;                      /* 0x10003024 Write*/
} * const sic = (void*)0x10003000;

interrupt_handler_t interruptVector[32];

/**
 * Enable an interrupt request line.
 *
 * @param irq
 *      Number of the IRQ to enable, which on this platform must be in the
 *      range [0, 31].
 */
void enable_irq(irqmask irq)
{
    regs->VICINTENABLE = 1U << irq;
    
    /* Enable SIC_PICENABLE bit for external peripheral irq so that i can get interrupt on PIC*/
    /*TODO: Is it right place to implement this ?? */
    
    if((irq >=25 && irq <= 28) || (irq == 32))
    {
      sic->SIC_PICENSET = 1U << irq;
    }
}

/**
 * Disable an interrupt request line.
 *
 * @param irq
 *      Number of the IRQ to disable, which on this platform must be in the
 *      range [0, 31].
 */
void disable_irq(irqmask irq)
{
    regs->VICINTENCLEAR = 1U << irq;
}

/**
 * Call the service routine for each pending IRQ.
 */
void dispatch(void)
{
    uint32_t status = regs->VICIRQSTATUS;

    do
    {
        uint irq = 31 - __builtin_clz(status);
        interruptVector[irq]();
        status ^= 1U << irq;
    }
    while (status);
}
