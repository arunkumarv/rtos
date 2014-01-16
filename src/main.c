#include <stdio.h>
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

uint16_t fn1_sp;
uint16_t fn2_sp;
uint16_t fn3_sp;

uint16_t *ptr_sp;

uint8_t number = 1;
uint8_t one_firsttime = 1, two_firsttime = 1, three_firsttime = 1;

task_ctrl_block tcb[3];

uint8_t i;

void TIMER1_COMPA_vect ( void )
{
  SAVE_CONTEXT();
  
  if ( tcb[0].priority == 1 )
  {
	  tcb[0].priority = 2;
	  tcb[1].priority = 1;
	  
	  ptr_sp = & ( tcb[0].stackpointer );
	  LOAD_PTR_TO_SP ();
	  
	  if ( tcb[0].status == INACTIVE )
	  {
		  sei ();
		  tcb[0].fun_ptr ();
		  
	  } else {  
		  
		RESTORE_CONTEXT ();
		asm volatile ( "reti" );
	  }
	  
  } else if ( tcb[1].priority == 1 ) {
	  
	  tcb[2].priority = 1;
	  tcb[1].priority = 3;
	  
	  ptr_sp = & ( tcb[1].stackpointer );
	  LOAD_PTR_TO_SP ();
	  
	  if ( tcb[1].status == INACTIVE )
	  {
		  sei ();
		  tcb[1].fun_ptr ();
		  
	  } else {  
		  
		RESTORE_CONTEXT ();
		asm volatile ( "reti" );
	  }
  } else if ( tcb[2].priority == 1 ) {
	  
	  tcb[0].priority = 1;
	  tcb[2].priority = 2;
	  
	  ptr_sp = & ( tcb[1].stackpointer );
	  LOAD_PTR_TO_SP ();
	  
	  if ( tcb[2].status == INACTIVE )
	  {
		  sei ();
		  tcb[2].fun_ptr ();
		  
	  } else {  
		  
		RESTORE_CONTEXT ();
		asm volatile ( "reti" );
	  }
  }
  /*
  for ( i = 0; i < 3; i++ )
  {
	  if ( tcb[i].priority == 1 )
	  {
		  ptr_sp = &tcb[i].stackpointer;
		  LOAD_PTR_TO_SP();
		  
		  if ( tcb[i].status == INACTIVE )
		  {
			  printf ("----------------------------");
			  tcb[i].status = ACTIVE;
			  sei ();
			  tcb[i].fun_ptr ();
			  
		  } else {
			printf ("-------------fsdfsdf---------------");
			RESTORE_CONTEXT ();
			asm volatile ( "reti" );
		  }
	  }
  }*/
  /*
  if ( number == 1 )
  {
	  number = 2;
	  ptr_sp = &fn1_sp;
	  LOAD_PTR_TO_SP ();
	  
	  if ( one_firsttime == 1 )
	  {
		one_firsttime = 0;
		sei ();		
		function_1 ();
		
	  } else {
		  		  
		RESTORE_CONTEXT ();
		asm volatile ( "reti" );
	  }
	  
  } else if ( number == 2 ){
	  
	  number = 3;
	  
	  ptr_sp = &fn2_sp;
	  LOAD_PTR_TO_SP ();
	  
	  if ( two_firsttime == 1 )
	  {
		two_firsttime = 0;
		sei ();		
		function_2 ();
		
	  } else {
		  		  
		RESTORE_CONTEXT ();
		asm volatile ( "reti" );
	  }
  }  else {
	  
	  number = 1;
	  
	  ptr_sp = &fn3_sp;
	  LOAD_PTR_TO_SP();
	  
	  if ( three_firsttime == 1 )
	  {
		three_firsttime = 0;
		sei ();		
		function_3 ();
		
	  } else {
		  		  
		RESTORE_CONTEXT ();
		asm volatile ( "reti" );
	  }
  }
  */
  ptr_sp = &main_sp;
  LOAD_PTR_TO_SP();
  RESTORE_CONTEXT();
  asm volatile ( "reti" );
}

void blink_led ( void )
{
   PORTB ^= _BV ( PB5 );
}

int main ( void )
{
  cli ();
  init_print ();
  timer1_init ();
  
  fn1_sp = FUN1_SP_BASE;
  fn2_sp = FUN2_SP_BASE;
  fn3_sp = FUN3_SP_BASE;
  
  tcb[0].fun_ptr = & function_1;
  tcb[0].priority = 1;
  tcb[0].status = INACTIVE;
  
  tcb[1].fun_ptr = & function_2;
  tcb[1].priority = 2;
  tcb[1].status = INACTIVE;
  
  tcb[2].fun_ptr = & function_3;
  tcb[2].priority = 3;
  tcb[2].status = INACTIVE;
  
  ptr_sp = & main_sp;

  DDRB |= _BV ( DDB5 ); /* for led in arduino pin 13 */
  
  sei ();  

  while (1)
  {
  }

  return 0;
}
