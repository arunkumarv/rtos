#include <stdio.h>
#include <macros.h>

void function_1 ( void ) __attribute__ ( ( naked ) );
void function_2 ( void ) __attribute__ ( ( naked ) );
void function_3 ( void ) __attribute__ ( ( naked ) );

extern task_ctrl_block *tcb[3]; 

void function_1 ( void )
{
	
  while (1)
  {
	tcb[0]->priority = 2;
	tcb[1]->priority = 1;
	tcb[2]->priority = 3;
    printf ( "I am in function 1\n" );
  }
}

void function_2 ( void )
{
  while (1)
  {
	tcb[1]->priority = 3;
	tcb[0]->priority = 2;
	tcb[2]->priority = 1;
    printf ( "function 2\n" );
  }
}

void function_3 ( void )
{
  while (1)
  {
	tcb[0]->priority = 1;
	tcb[1]->priority = 2;
	tcb[2]->priority = 3;
    printf ( "fun 3\n" );
  }
}
