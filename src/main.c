#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include <macros.h>

#define F_CPU 16000000UL

extern void init_print ( void );
extern void timer1_init ( void );

extern void function_1 ( void ) __attribute__ ( ( naked ) );
extern void function_2 ( void ) __attribute__ ( ( naked ) );
extern void function_3 ( void ) __attribute__ ( ( naked ) );

void blink_led ( void );

void TIMER1_COMPA_vect ( void ) __attribute__ ( ( signal, naked ) );

uint16_t main_sp;

uint16_t *ptr_sp;

task_ctrl_block *tcb_pivot;
static task_ctrl_block *tcb_new, *tcb_temp;

void TIMER1_COMPA_vect ( void )
{
  SAVE_CONTEXT();
  
  tcb_temp = tcb_pivot;
  
  while (tcb_temp != NULL )
  {
	  if ( tcb_temp->priority == 1 )
	  {
		  ptr_sp = & ( tcb_temp->stackpointer );
		  LOAD_PTR_TO_SP ();
		  
		  if ( tcb_temp->status == INACTIVE )
		  {
			  sei ();
			  tcb_temp->status = ACTIVE;
			  tcb_temp->fun_ptr ();
			  
		  } else {
			  
			RESTORE_CONTEXT ();
			asm volatile ( "reti" );			  
		  }
	  }
	  tcb_temp = tcb_temp->tcb_ptr;
  }
 
  ptr_sp = &main_sp;
  LOAD_PTR_TO_SP();
  RESTORE_CONTEXT();
  asm volatile ( "reti" );
}

void blink_led ( void )
{
   PORTB ^= _BV ( PB5 );
}

task_ctrl_block * createTask ( uint8_t priority, uint16_t sp_base )
{
  task_ctrl_block *tcb_local;
  
  tcb_local = ( task_ctrl_block * ) malloc ( sizeof ( task_ctrl_block ) );
  tcb_local->fun_ptr = & function_1;
  tcb_local->priority = priority;
  tcb_local->status = INACTIVE;
  tcb_local->stackpointer = sp_base;
  tcb_local->tcb_ptr = NULL;
  
  return tcb_local;
}

int main ( void )
{
  cli ();
  init_print ();
  timer1_init ();
  
  tcb_pivot = createTask ( 1, FUN1_SP_BASE );
  
  tcb_new = ( task_ctrl_block * ) malloc ( sizeof ( task_ctrl_block ) );
  tcb_new->fun_ptr = & function_2;
  tcb_new->priority = 2;
  tcb_new->status = INACTIVE;
  tcb_new->stackpointer = FUN2_SP_BASE;
  tcb_new->tcb_ptr = NULL;
  
  tcb_temp = tcb_pivot;
  while (tcb_temp->tcb_ptr != NULL )
  {
	  tcb_temp = tcb_temp->tcb_ptr;
  }
  tcb_temp->tcb_ptr = tcb_new;
  
  tcb_new = ( task_ctrl_block * ) malloc ( sizeof ( task_ctrl_block ) );
  tcb_new->fun_ptr = & function_3;
  tcb_new->priority = 3;
  tcb_new->status = INACTIVE;
  tcb_new->stackpointer = FUN3_SP_BASE;
  tcb_new->tcb_ptr = NULL;
  
  tcb_temp = tcb_pivot;
  while (tcb_temp->tcb_ptr != NULL )
  {
	  tcb_temp = tcb_temp->tcb_ptr;
  }
  tcb_temp->tcb_ptr = tcb_new;
  
  
  ptr_sp = & main_sp;

  DDRB |= _BV ( DDB5 ); /* for led in arduino pin 13 */
  
  sei ();  

  while (1)
  {
  }

  return 0;
}
