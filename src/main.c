#include <stdio.h>

#include <task.h>
#include <util.h>
#include <common.h>


void function_1 ( void );
void function_2 ( void );
void function_3 ( void );
void function_4 ( void );


int main ( void )
{
  init_print (); 
  
  initializeScheduler ();
  
  createTask ( &function_1, "fun_one", 1, 200 );
  
  createTask ( &function_2, "fun_two", 2, 200 );
  
  createTask ( &function_3, "fun_three", 3, 200 );
  
  createTask ( &function_4, "fun_four", 4, 50 );

  startScheduler ();
  
  return 0;
}

void function_1 ( void )
{
  while (1)
  {
	  changePriority ( "fun_one", 5 );
	  changePriority ( "fun_two", 2 );
	  changePriority ( "fun_three", 3 );
	  changePriority ( "fun_four", 4 );
	  
	  printf ( "I am in function 1\n" );
  }
}

void function_2 ( void )
{
  while (1)
  {
	  changePriority ( "fun_one", 3 );
	  changePriority ( "fun_two", 5 );
	  changePriority ( "fun_three", 2 );
	  changePriority ( "fun_four", 4 );
	  
      printf ( "function_2\n" );
  }
}

void function_3 ( void )
{
  while (1)
  {  	  
	  changePriority ( "fun_one", 4 );
	  changePriority ( "fun_two", 3 );
	  changePriority ( "fun_three",5  );
	  changePriority ( "fun_four", 2 );
	  
      printf ( "fun_3\n" );
      
  }
}

void function_4 ( void )
{
  
  while (1)
  {   
	  changePriority ( "fun_one", 2 );
	  changePriority ( "fun_two", 3 );
	  changePriority ( "fun_three", 4 );
	  changePriority ( "fun_four", 5 );
	  
      printf ( "fun_ffffffoooouuurrrr\n" );
      
  }
}
