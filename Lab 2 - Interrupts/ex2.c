#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*
 * ISR for External Interrupt 0.
 *
 * This function automatically runs when INT0 is triggered.
 * INT0 is on PD2.
 *
 * In this program, INT0 triggers on the rising edge.
 * Rising edge means button release when using internal pull-up.
 */
ISR(INT0_vect)
{
    /*
     * Small debounce delay.
     * Mechanical buttons can bounce, so one release may produce
     * multiple quick signals.
     */
    _delay_ms(30);

    /*
     * Confirm that button is actually released.
     *
     * With internal pull-up:
     * released = HIGH
     */
    if (PIND & (1 << PD2))
    {
        /*
         * Toggle PB0.
         *
         * XOR with 1 changes the bit:
         * 0 becomes 1
         * 1 becomes 0
         */
        PORTB ^= (1 << PB0);
    }
}

int main(void)
{
    /*
     * PB0 is connected to LED.
     * So PB0 must be output.
     */
    DDRB |= (1 << PB0);

    /*
     * Initially turn LED OFF.
     */
    PORTB &= ~(1 << PB0);

    /*
     * PD2 is connected to push button.
     * So PD2 must be input.
     */
    DDRD &= ~(1 << PD2);

    /*
     * Enable internal pull-up resistor on PD2.
     *
     * Button not pressed/released = HIGH
     * Button pressed              = LOW
     */
    PORTD |= (1 << PD2);

    /*
     * Configure INT0 to trigger on rising edge.
     *
     * EICRA controls interrupt triggering style.
     *
     * ISC01 = 1 and ISC00 = 1 means:
     * rising edge of INT0 generates interrupt.
     */
    EICRA |= (1 << ISC01) | (1 << ISC00);

    /*
     * Enable INT0 external interrupt.
     *
     * EIMSK is External Interrupt Mask Register.
     * Setting INT0 bit allows INT0 interrupt to happen.
     */
    EIMSK |= (1 << INT0);

    /*
     * Enable global interrupts.
     *
     * Without sei(), interrupts will not run.
     */
    sei();

    /*
     * Main loop does nothing.
     *
     * The LED toggling happens inside the ISR.
     */
    while (1)
    {
        // Waiting for interrupt
    }

    return 0;
}