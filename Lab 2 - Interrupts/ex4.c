#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*
 * number stores the current 6-bit binary value.
 * pending_bit tells the main loop which button was pressed.
 *
 * pending_bit:
 * -1 = no button event
 *  0 = Button A pressed
 *  1 = Button B pressed
 */
volatile unsigned char number = 0;
volatile signed char pending_bit = -1;

/*
 * INT0 interrupt.
 * PD2 = Button A.
 * Button A inputs binary 0.
 */
ISR(INT0_vect)
{
    pending_bit = 0;
}

/*
 * INT1 interrupt.
 * PD3 = Button B.
 * Button B inputs binary 1.
 */
ISR(INT1_vect)
{
    pending_bit = 1;
}

/*
 * Add one bit to the current 6-bit number.
 */
void add_bit(unsigned char bit)
{
    number = ((number << 1) | bit) & 0x3F;
    PORTB = number & 0x3F;
}

int main(void)
{
    /*
     * PB0 to PB5 are LED outputs.
     */
    DDRB |= 0x3F;

    /*
     * Initially display 000000.
     */
    PORTB = 0x00;

    /*
     * PD2 and PD3 are button inputs.
     */
    DDRD &= ~((1 << PD2) | (1 << PD3));

    /*
     * Enable internal pull-ups on PD2 and PD3.
     *
     * Not pressed = HIGH
     * Pressed     = LOW
     */
    PORTD |= (1 << PD2) | (1 << PD3);

    /*
     * Configure INT0 and INT1 for falling edge.
     *
     * INT0 falling edge:
     * ISC01 = 1, ISC00 = 0
     *
     * INT1 falling edge:
     * ISC11 = 1, ISC10 = 0
     */
    EICRA = (1 << ISC01) | (1 << ISC11);

    /*
     * Clear any pending interrupt flags before enabling interrupts.
     */
    EIFR |= (1 << INTF0) | (1 << INTF1);

    /*
     * Enable INT0 and INT1.
     */
    EIMSK |= (1 << INT0) | (1 << INT1);

    /*
     * Enable global interrupts.
     */
    sei();

    while (1)
    {
        if (pending_bit != -1)
        {
            signed char bit = pending_bit;
            pending_bit = -1;

            /*
             * Debounce delay.
             */
            _delay_ms(30);

            /*
             * Confirm the corresponding button is still pressed.
             */
            if (bit == 0)
            {
                if (!(PIND & (1 << PD2)))
                {
                    add_bit(0);

                    /*
                     * Wait until Button A is released.
                     * This prevents one long press from entering many zeros.
                     */
                    while (!(PIND & (1 << PD2)))
                    {
                    }

                    _delay_ms(30);
                }
            }
            else if (bit == 1)
            {
                if (!(PIND & (1 << PD3)))
                {
                    add_bit(1);

                    /*
                     * Wait until Button B is released.
                     * This prevents one long press from entering many ones.
                     */
                    while (!(PIND & (1 << PD3)))
                    {
                    }

                    _delay_ms(30);
                }
            }
        }
    }

    return 0;
}