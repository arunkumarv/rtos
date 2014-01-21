#include <stdio.h>
#include <macros.h>
#include <avr/interrupt.h>

void function_1 ( void );// __attribute__ ( ( naked ) );
void function_2 ( void );// __attribute__ ( ( naked ) );
void function_3 ( void );// __attribute__ ( ( naked ) );
void function_4 ( void );// __attribute__ ( ( naked ) );

extern task_ctrl_block *tcb_pivot; 

extern void changePriority ( char *name, uint8_t priority );

void function_1 ( void )
{
  while (1)
  {
	  changePriority ( "fun_one", 2 );
	  changePriority ( "fun_two", 1 );
	  changePriority ( "fun_three", 3 );
	  changePriority ( "fun_four", 4 );
	  
	  printf ( "I am in function 1\n" );
  }
}

void function_2 ( void )
{
  while (1)
  {
	  changePriority ( "fun_one", 2 );
	  changePriority ( "fun_two", 3 );
	  changePriority ( "fun_three", 1 );
	  changePriority ( "fun_four", 4 );
	  
      printf ( "function_2\n" );
  }
}

void function_3 ( void )
{
  while (1)
  {  

	  
	  changePriority ( "fun_one", 2 );
	  changePriority ( "fun_two", 3 );
	  changePriority ( "fun_three", 4 );
	  changePriority ( "fun_four", 1 );
	  
      printf ( "fun_3\n" );
      
  }
}

void function_4 ( void )
{
  while (1)
  {  

	  
	  changePriority ( "fun_one", 1 );
	  changePriority ( "fun_two", 3 );
	  changePriority ( "fun_three", 4 );
	  changePriority ( "fun_four", 2 );
	  
      printf ( "fun_ffffffoooouuurrrr\n" );
      
  }
}
