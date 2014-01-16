#include <avr/io.h>

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
