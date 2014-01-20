#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include <macros.h>

#define F_CPU 16000000UL

extern void init_print ( void );
extern void timer1_init ( void );

extern void function_1 ( void );// __attribute__ ( ( naked ) );
extern void function_2 ( void );// __attribute__ ( ( naked ) );
extern void function_3 ( void );// __attribute__ ( ( naked ) );

void blink_led ( void );

void TIMER1_COMPA_vect ( void ) __attribute__ ( ( signal, naked ) );

uint16_t main_sp;

uint16_t *ptr_sp;

task_ctrl_block *tcb_pivot;

uint16_t stack_booked;

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
		  
		  if ( tcb_temp->status == NOT_RUNNING )
		  {
			  sei ();
			  tcb_temp->status = RUNNING;
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

void changePriority ( char *name, uint8_t priority )
{
	  cli ();
	  
	  task_ctrl_block *tcb_local;
	  
	  tcb_local = tcb_pivot;
	  
	  while ( tcb_local != NULL )
	  {
		  
		  if ( strcmp ( tcb_local->name , name ) == 0 ) 
		  {
			  tcb_local->priority = priority;
		  }
			
		  tcb_local = tcb_local->tcb_ptr;
	  }
	  
	  sei ();
}

void createTask ( void ( * function_ptr )( void ), char *taskname, uint8_t priority, uint16_t stack_size )
{
  task_ctrl_block *tcb_new, *tcb_local;
  
  tcb_new = ( task_ctrl_block * ) malloc ( sizeof ( task_ctrl_block ) );
  
  tcb_new->fun_ptr = function_ptr;
  
  tcb_new->priority = priority;
  
  tcb_new->status = NOT_RUNNING;
  
  tcb_new->stackpointer = USER_STACK_BASE - stack_booked;
  
  tcb_new->name = ( char * )malloc ( 20 );
  
  memset ( tcb_new->name, '\0', 20 );
  
  strcpy ( tcb_new->name, taskname );
  
  stack_booked += stack_size;
  
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
  
  stack_booked = 0;
  
  createTask ( &function_1, "fun_one", 1, 200 );
  
  createTask ( &function_2, "fun_two", 2, 200 );
  
  createTask ( &function_3, "fun_three", 3, 200 );
  
  ptr_sp = & main_sp;
  
  sei ();  

  for (;;);

  return 0;
}
