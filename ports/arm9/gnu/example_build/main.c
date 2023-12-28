/* This is a small demo of the high-performance ThreadX kernel.  It includes examples of eight
   threads of different priorities, using a message queue, semaphore, mutex, event flags group, 
   byte pool, and block pool.  */

#include "tx_api.h"

#define DEMO_STACK_SIZE         1024
#define DEMO_BYTE_POOL_SIZE     2048
#define DEMO_BLOCK_POOL_SIZE    100
#define DEMO_QUEUE_SIZE         100

/* Define the ThreadX object control blocks...  */

TX_THREAD               thread_0;
TX_BYTE_POOL            byte_pool_0;

#define PIO_BASE	0x01c20800
#define PA_CFG0		0x00

#define PA_DATA		0x10

void    thread_0_entry(ULONG thread_input);

void delay(volatile unsigned int n)
{
	while(n--);
}

/* Define main entry point.  */

int main()
{
    /* Enter the ThreadX kernel.  */
    tx_kernel_enter();
}

void    tx_application_define(void *first_unused_memory)
{
    CHAR    *pointer = TX_NULL;

    /* Create a byte memory pool from which to allocate the thread stacks.  */
    tx_byte_pool_create(&byte_pool_0, "byte pool 0", first_unused_memory, DEMO_BYTE_POOL_SIZE);

    /* Put system definition stuff in here, e.g. thread creates and other assorted
       create information.  */

    /* Allocate the stack for thread 0.  */
    tx_byte_allocate(&byte_pool_0, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

    /* Create the main thread.  */
    tx_thread_create(&thread_0, "thread 0", thread_0_entry, 0,  
            pointer, DEMO_STACK_SIZE, 
            1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}

/* Define the test threads.  */

void    thread_0_entry(ULONG thread_input)
{
    volatile unsigned int *rp;

	/* configure PA3 to GPIO Output mode */
	rp = (volatile unsigned int *)(PIO_BASE + PA_CFG0);
	*rp &= ~(7 << 12);
	*rp |= (1 << 12);

	/* load PA Data register's content */
	rp = (volatile unsigned int *)(PIO_BASE + PA_DATA);

	while(1){
		/* set PA3 to low */
		*rp &= ~(1 << 3);

		/* make a delay */
		delay(0x0000f000);

		/* set PA3 to high */
		*rp |= (1 << 3);

		/* make a delay */
		delay(0x0000f000);
	}
}