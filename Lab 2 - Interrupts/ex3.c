#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*
 * count stores the number of times the push button was released.
 *
 * volatile is important because this variable is modified inside an ISR.
 * It tells the compiler:
 * "Do not optimize this variable wrongly, because it can change unexpectedly."
 */
volatile unsigned char count = 0;

/*
 * ISR for External Interrupt 0.
 *
 * INT0 is on PD2.
 * This ISR runs automatically when the button is released.
 */
ISR(INT0_vect)
{
    /*
     * Small debounce delay.
     * Mechanical buttons can produce multiple quick transitions.
     */
    _delay_ms(30);

    /*
     * Confirm that the button is actually released.
     *
     * With internal pull-up:
     * released = HIGH
     */
    if (PIND & (1 << PD2))
    {
        /*
         * Increase release count by 1.
         *
         * & 0x3F keeps only the lower 6 bits.
         * So the value stays from 0 to 63.
         */
        count = (count + 1) & 0x3F;

        /*
         * Display the count on PB0 to PB5.
         */
        PORTB = count & 0x3F;
    }
}

int main(void)
{
    /*
     * PB0 to PB5 are connected to LEDs.
     * Therefore, PB0-PB5 must be output pins.
     *
     * 0x3F = 0011 1111
     */
    DDRB |= 0x3F;

    /*
     * Initially display 0.
     */
    PORTB = 0x00;

    /*
     * PD2 is connected to the push button.
     * Therefore, PD2 must be input.
     */
    DDRD &= ~(1 << PD2);

    /*
     * Enable internal pull-up resistor on PD2.
     *
     * Button released = HIGH
     * Button pressed  = LOW
     */
    PORTD |= (1 << PD2);

    /*
     * Configure INT0 to trigger on rising edge.
     *
     * Rising edge means LOW to HIGH.
     * In our button circuit, that happens when the button is released.
     *
     * ISC01 = 1
     * ISC00 = 1
     */
    EICRA |= (1 << ISC01) | (1 << ISC00);

    /*
     * Enable INT0 external interrupt.
     */
    EIMSK |= (1 << INT0);

    /*
     * Enable global interrupts.
     *
     * Without this, the ISR will not run.
     */
    sei();

    /*
     * Main loop does nothing.
     * Counting and LED updating happen inside the ISR.
     */
    while (1)
    {
        // Waiting for button release interrupt
    }

    return 0;
}