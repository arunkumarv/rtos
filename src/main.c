#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include <macros.h>

#define F_CPU 16000000UL

extern void init_print ( void );
void timer1_init ( void );

void blink_led ( void );

void TIMER1_COMPA_vect ( void ) __attribute__ ( ( signal, naked ) );

void function_1 ( void ) __attribute__ ( ( naked ) );
void function_2 ( void ) __attribute__ ( ( naked ) );
void function_3 ( void ) __attribute__ ( ( naked ) );

uint16_t main_sp;

uint16_t fn1_sp;
uint16_t fn2_sp;
uint16_t fn3_sp;

uint16_t *ptr_sp;

uint8_t number = 1;
uint8_t one_firsttime = 1, two_firsttime = 1, three_firsttime = 1;


void TIMER1_COMPA_vect ( void )
{
  SAVE_CONTEXT();
  
  if ( number == 1 )
  {
	  number = 2;
	  ptr_sp = &fn1_sp;
	  LOAD_PTR_TO_SP();
	  
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
	  LOAD_PTR_TO_SP();
	  
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
  
  ptr_sp = &main_sp;
  LOAD_PTR_TO_SP();
  RESTORE_CONTEXT();
  asm volatile ( "reti" );
}

void function_1 ( void )
{
  while (1)
  {
    printf ( "I am in function 1\n" );
  }
}

void function_2 ( void )
{
  while (1)
  {
    printf ( "function 2\n" );
  }
}

void function_3 ( void )
{
  while (1)
  {
    printf ( "fun 3\n" );
  }
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
  
  ptr_sp = & main_sp;

  DDRB |= _BV ( DDB5 ); /* for led in arduino pin 13 */
  
  sei ();  

  while (1)
  {
  }

  return 0;
}

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
