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

static task_ctrl_block *tcb_temp;

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

void createTask ( void ( * function_ptr )( void ), uint8_t priority, uint16_t sp_base )
{
  task_ctrl_block *tcb_new, *tcb_local;
  
  tcb_new = ( task_ctrl_block * ) malloc ( sizeof ( task_ctrl_block ) );
  tcb_new->fun_ptr = function_ptr;
  tcb_new->priority = priority;
  tcb_new->status = INACTIVE;
  tcb_new->stackpointer = sp_base;
  tcb_new->tcb_ptr = NULL;
  
  if ( tcb_pivot == NULL )
  {
	  tcb_pivot = tcb_new;
  }
  else 
  {
	  tcb_local = tcb_pivot;
	  
	  while ( tcb_local->tcb_ptr != NULL )
	  {
		  tcb_local = tcb_local->tcb_ptr;
	  }
	  tcb_local->tcb_ptr = tcb_new;
  }
}

int main ( void )
{
  cli ();
  init_print ();
  timer1_init ();
  
  tcb_pivot = NULL;
  
  createTask ( &function_1, 1, FUN1_SP_BASE );
  
  createTask ( &function_2, 2, FUN2_SP_BASE );
  
  createTask ( &function_3, 3, FUN3_SP_BASE );
  
  ptr_sp = & main_sp;

  DDRB |= _BV ( DDB5 ); /* for led in arduino pin 13 */
  
  sei ();  

  while (1)
  {
  }

  return 0;
}
