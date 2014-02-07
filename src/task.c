#include <stdio.h>
#include <common.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>

uint16_t stack_booked;
uint16_t main_sp;
uint16_t *ptr_sp;

uint16_t backup_sp;

task_ctrl_block *tcb_pivot, *tcb_run, *tcb_prev;
task_ctrl_block *tcb_temp;
task_ctrl_block *tcb_new, *tcb_local;

void TIMER1_COMPA_vect ( void ) __attribute__ ( ( signal, naked ) );

void timer1_init ( void ) 
{
    OCR1A = 0xFFFE;

    TCCR1B |= ( 1 << WGM12 );
    // Mode 4, CTC on OCR1A

    TIMSK1 |= ( 1 << OCIE1A );
    //Set interrupt on compare match

//    TCCR1B |= ( 1 << CS12 ) | ( 1 << CS10 ); //1024
    TCCR1B |= ( 1 << CS12 ); //256
    
}

void TIMER1_COMPA_vect ( void )
{
  SAVE_CONTEXT();
  
  ptr_sp = & ( main_sp );
  LOAD_PTR_TO_SP ();
  
  tcb_temp = tcb_pivot;
  tcb_run = NULL;
  
  while ( tcb_temp != NULL )
  {
	  switch ( tcb_temp->status )
	  {
		  case RUN:
		    if ( tcb_run == NULL )
		    {
				tcb_run = tcb_temp;
			} else
			{
			  if ( tcb_temp->priority < tcb_run->priority )
			  {
				tcb_run = tcb_temp;
			  }
		    }
			tcb_prev = tcb_temp;
			tcb_temp = tcb_temp->tcb_ptr;
		  break;
		  
		  case TERMINATE:
			if ( tcb_temp == tcb_pivot )
			{
				tcb_pivot = tcb_temp->tcb_ptr;
				free ( tcb_temp );
				tcb_temp = tcb_pivot;
			} else {
				tcb_prev->tcb_ptr = tcb_temp->tcb_ptr;
				free ( tcb_temp );
				tcb_temp = tcb_prev->tcb_ptr;
			}
		  break;
		  
		  case WAIT:
		    tcb_prev = tcb_temp;
		    tcb_temp = tcb_temp->tcb_ptr;
		  break;
		  
		  default:
		  break;
	  }	  
  }
  //if tcb_run == NULL go to idle (load main sp and back to forloop main )
  if ( tcb_run == NULL )
  {
    ptr_sp = & ( main_sp );
    LOAD_PTR_TO_SP ();
    RESTORE_CONTEXT ();
	asm volatile ( "reti" );	
  }
  
  ptr_sp = & ( tcb_run->stackpointer );
  LOAD_PTR_TO_SP ();
		  
  if ( tcb_run->is_firsttime == TRUE )
  {
	sei ();
	tcb_run->is_firsttime = FALSE;
	tcb_run->fun_ptr ();
  } 
  else 
  {	  
	RESTORE_CONTEXT ();
	asm volatile ( "reti" );			  
  }
  
  ptr_sp = &main_sp;
  LOAD_PTR_TO_SP();
  RESTORE_CONTEXT();
  asm volatile ( "reti" );
}

void initializeScheduler ( void )
{
  cli ();
  
  timer1_init ();
	  
  tcb_pivot = NULL;
  
  stack_booked = 0;
}

void startScheduler ( void )
{
  ptr_sp = & main_sp;
  
  sei ();  

  for (;;)
  {
	  printf ("in the main function\n");
  }
}

void changePriority ( char *name, uint8_t priority )
{
	  cli ();
	  
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

void changeStatus ( char *name, uint8_t status )
{
	  cli ();
	  
	  tcb_local = tcb_pivot;
	  
	  while ( tcb_local != NULL )
	  {
		  
		  if ( strcmp ( tcb_local->name , name ) == 0 ) 
		  {
			  tcb_local->status = status;
			  break;
		  }
			
		  tcb_local = tcb_local->tcb_ptr;
	  }
	  
	  sei ();
}

void deleteTask ( char *name )
{
	cli ();
	
	tcb_temp = tcb_pivot;
	tcb_prev = tcb_pivot;
	
	while ( tcb_temp != NULL )
	{
		if (  strcmp ( tcb_temp->name , name ) == 0 )
		{
			if ( tcb_temp == tcb_pivot )
			{
				tcb_pivot = tcb_temp->tcb_ptr;
			} else {
				tcb_prev->tcb_ptr = tcb_temp->tcb_ptr;
			}
			free ( tcb_temp );
			break;
		}
		tcb_prev = tcb_temp;
		tcb_temp = tcb_temp->tcb_ptr;
	}
	//changeStatus ( name, TERMINATE );
	sei ();
}

void createTask ( void ( * function_ptr )( void ), char *taskname, uint8_t priority, uint16_t stack_size )
{ 
  tcb_new = ( task_ctrl_block * ) malloc ( sizeof ( task_ctrl_block ) );
  
  tcb_new->fun_ptr = function_ptr;
  
  tcb_new->priority = priority;
  
  tcb_new->is_firsttime = TRUE;
  
  tcb_new->status = RUN;
  
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
