#include "osKernel.h"


#define NUM_OF_THREADS				3
#define STACK_SIZE						100

#define BUS_FREQ							16000000

#define CTRL_ENABLE	  			  (1U<<0)
#define CTRL_TICKINT			   	(1U<<1)
#define CTRL_CLCKSRC	   			(1U<<2)
#define CTRL_COUNTFLAG				(1U<<16)

#define SYSTICK_RST						0

uint32_t		MILLIS_PRESCALER;

typedef struct tcb tcbType;

struct tcb{
	uint32_t *stackPt;
	tcbType *nextPt;
};


tcbType tcbs[NUM_OF_THREADS];
tcbType *currentPt;

int32_t TCB_STACK[NUM_OF_THREADS][STACK_SIZE];


void osKernelStackInit(int i) {
	
	tcbs[i].stackPt = (uint32_t*) &TCB_STACK[i][STACK_SIZE - 16]; /* Stack pointer */
	
	TCB_STACK[i][STACK_SIZE - 1] = (1U<<21); /* set bit 21 in PSR to 1 -> operate in Thumb mode */
	
	/* Dummy stack pointer */
	TCB_STACK[i][STACK_SIZE - 3]  = 0xAAAAAAAA;	/* R14 i.e LR */
	TCB_STACK[i][STACK_SIZE - 4]  = 0xAAAAAAAA;	/* R12 */
	TCB_STACK[i][STACK_SIZE - 5]  = 0xAAAAAAAA;	/* R2 */
	TCB_STACK[i][STACK_SIZE - 6]  = 0xAAAAAAAA;	/* R2 */
	TCB_STACK[i][STACK_SIZE - 7]  = 0xAAAAAAAA;	/* R1 */
	TCB_STACK[i][STACK_SIZE - 8]  = 0xAAAAAAAA;	/* R0 */

	TCB_STACK[i][STACK_SIZE - 9]  = 0xAAAAAAAA;	/* R11 */
	TCB_STACK[i][STACK_SIZE - 10] = 0xAAAAAAAA;	/* R10 */
	TCB_STACK[i][STACK_SIZE - 11] = 0xAAAAAAAA;	/* R9 */
	TCB_STACK[i][STACK_SIZE - 12] = 0xAAAAAAAA;	/* R8 */
	TCB_STACK[i][STACK_SIZE - 13] = 0xAAAAAAAA;	/* R7 */
	TCB_STACK[i][STACK_SIZE - 14] = 0xAAAAAAAA;	/* R6 */
	TCB_STACK[i][STACK_SIZE - 15] = 0xAAAAAAAA;	/* R5 */
	TCB_STACK[i][STACK_SIZE - 16] = 0xAAAAAAAA;	/* R4 */
}

uint8_t osKernelAddThreads(void(*task0)(void), void(*task1)(void), void(*task2)(void))
{
	/* Disable global interrupt */
	__disable_irq();
	
	tcbs[0].nextPt = &tcbs[1];
	tcbs[1].nextPt = &tcbs[2];
	tcbs[2].nextPt = &tcbs[0];
	
	/* Initialize stack for thread 1 */
	osKernelStackInit(0);
	/* Initialize PC */
	TCB_STACK[0][STACK_SIZE - 2] = (uint32_t)(task0);
	
	/* Initialize stack for thread 2 */
	osKernelStackInit(1);
	/* Initialize PC */
	TCB_STACK[1][STACK_SIZE - 2] = (uint32_t)(task1);
	
	/* Initialize stack for thread 3*/
	osKernelStackInit(2);
	/* Initialize PC */
	TCB_STACK[2][STACK_SIZE-2] = (uint32_t)(task2);

	/* Start of thread 0 */
	currentPt = &tcbs[0];
	
	/* Enable global interrupts */
	__enable_irq();

	return 1;
}

void osKernelInit(void) {
	MILLIS_PRESCALER = (BUS_FREQ/1000);
	
}

void osKernelLaunch(uint32_t quanta) {
	/* Reset Systick */
	SysTick->CTRL = SYSTICK_RST;
	/* Clear Systick current value register */
	SysTick->VAL = 0;
	/* Load quanta */
	SysTick->LOAD = (quanta * MILLIS_PRESCALER) - 1;
	/* Setup systick to low priority */
	NVIC_SetPriority(SysTick_IRQn, 15);
	/* Enable Systick, select internal clock */
	SysTick->CTRL |= CTRL_CLCKSRC | CTRL_ENABLE;
	/* Enable systick interrupt */
	SysTick->CTRL |= CTRL_TICKINT;
	/* Launch scheduler */
	osSchedulerLaunch();
}


void osKernelStart(void) {
	
}

void osSchedulerLaunch(void)
{

}
