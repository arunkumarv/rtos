#include <stdio.h>
#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef BAUD
#define BAUD 38400
#endif

int uart_putch ( char ch, FILE *stream );
int uart_getch ( FILE *stream );

FILE uart_str = FDEV_SETUP_STREAM ( uart_putch, uart_getch, _FDEV_SETUP_RW );

void uart_init ( void )
{
        UBRR0H = (((F_CPU/BAUD)/16)-1)>>8;
        UBRR0L = (((F_CPU/BAUD)/16)-1);
        UCSR0B = (1<<RXEN0)|(1<<TXEN0);
        UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

void ansi_clear_screen ( void )
{
        putchar(27);
        putchar('[');
        putchar('H');
        putchar(27);
        putchar('[');
        putchar('J');
}

void ansi_me(void)
{
        putchar(27);
        putchar('[');
        putchar('0');
        putchar('m');
}

void uart_flush ( void )
{
        unsigned char dummy;
        while ( UCSR0A & (1<<RXC0) )
               dummy =  UDR0;
}

int uart_putch ( char ch, FILE *stream )
{
        if ( ch == '\n' )
                uart_putch ( '\r', stream );
        while (!(UCSR0A & (1<<UDRE0)));
        UDR0=ch;
        return 0;
}

int uart_getch ( FILE *stream )
{
        unsigned char ch = '\0';
        while ( !(UCSR0A & (1<<RXC0)));
        ch = UDR0;

        return ch;
}

void init_print ( void )
{
  stdout = stdin = &uart_str;
  uart_init ();
  ansi_me ();
  ansi_clear_screen ();
  ansi_me ();
  ansi_clear_screen ();
  uart_flush ();
}
