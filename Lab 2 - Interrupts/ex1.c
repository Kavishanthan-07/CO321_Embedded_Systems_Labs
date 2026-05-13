#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    /*
     * PORTB setup:
     * PB0 to PB5 are connected to 6 LEDs.
     * So we set PB0-PB5 as output pins.
     *
     * 0x3F = 0011 1111 in binary
     * This sets only the lower 6 bits of PORTB as outputs.
     */
    DDRB |= 0x3F;

    /*
     * PD7 setup:
     * PD7 is connected to the push button.
     * So we set PD7 as an input pin.
     */
    DDRD &= ~(1 << PD7);

    /*
     * Enable internal pull-up resistor on PD7.
     *
     * With pull-up:
     * Button not pressed = HIGH
     * Button pressed     = LOW
     */
    PORTD |= (1 << PD7);

    /*
     * This variable stores the number of button presses.
     * Since we use only 6 LEDs, we only display values from 0 to 63.
     */
    unsigned char count = 0;

    /*
     * Initially display 0 on LEDs.
     */
    PORTB = count & 0x3F;

    while (1)
    {
        /*
         * Check whether button is pressed.
         *
         * PIND reads the actual state of PORTD input pins.
         * (1 << PD7) creates a mask to check only PD7.
         *
         * If PD7 is LOW, the button is pressed.
         */
        if (!(PIND & (1 << PD7)))
        {
            /*
             * Small delay to avoid button bouncing.
             * Mechanical buttons do not give one clean signal.
             * They may rapidly change ON/OFF for a few milliseconds.
             */
            _delay_ms(30);

            /*
             * Check again after delay.
             * This confirms that the button is really pressed.
             */
            if (!(PIND & (1 << PD7)))
            {
                /*
                 * Increase button press count by 1.
                 * & 0x3F keeps only the lower 6 bits.
                 *
                 * So after 63, it becomes 0 again.
                 */
                count = (count + 1) & 0x3F;

                /*
                 * Display count on PB0-PB5.
                 *
                 * Example:
                 * count = 5 = 000101
                 * PB2 and PB0 will turn ON.
                 */
                PORTB = count & 0x3F;

                /*
                 * Wait until the button is released.
                 *
                 * This is very important.
                 * Without this, one long press would be counted many times.
                 */
                while (!(PIND & (1 << PD7)))
                {
                    // Do nothing until button is released
                }

                /*
                 * Small release debounce delay.
                 */
                _delay_ms(30);
            }
        }
    }

    return 0;
}