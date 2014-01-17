#include <stdio.h>
#include <macros.h>

void function_1 ( void ) __attribute__ ( ( naked ) );
void function_2 ( void ) __attribute__ ( ( naked ) );
void function_3 ( void ) __attribute__ ( ( naked ) );

extern task_ctrl_block *tcb_pivot; 

static task_ctrl_block *tcb_temp; 

void function_1 ( void )
{
  while (1)
  {
	  tcb_temp = tcb_pivot;
	  tcb_temp->priority = 2;
	  tcb_temp = tcb_temp->tcb_ptr;
	  tcb_temp->priority = 1;
	  tcb_temp = tcb_temp->tcb_ptr;
	  tcb_temp->priority = 2;
	  tcb_temp = tcb_temp->tcb_ptr;
	  printf ( "I am in function 1\n" );
  }
}

void function_2 ( void )
{
  while (1)
  {
	tcb_temp = tcb_pivot;
	  tcb_temp->priority = 3;
	  tcb_temp = tcb_temp->tcb_ptr;
	  tcb_temp->priority = 2;
	  tcb_temp = tcb_temp->tcb_ptr;
	  tcb_temp->priority = 1;
	  tcb_temp = tcb_temp->tcb_ptr;
    printf ( "function 2\n" );
  }
}

void function_3 ( void )
{
  while (1)
  {
	tcb_temp = tcb_pivot;
	  tcb_temp->priority = 1;
	  tcb_temp = tcb_temp->tcb_ptr;
	  tcb_temp->priority = 2;
	  tcb_temp = tcb_temp->tcb_ptr;
	  tcb_temp->priority = 3;
	  tcb_temp = tcb_temp->tcb_ptr;
    printf ( "fun 3\n" );
  }
}
