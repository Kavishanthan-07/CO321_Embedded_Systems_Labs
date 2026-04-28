#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

/*
 * LED connections:
 * LED A -> PB0 -> Arduino D8
 * LED B -> PB1 -> Arduino D9
 * LED C -> PB2 -> Arduino D10
 * LED D -> PB3 -> Arduino D11
 */

#define LED_A PB0
#define LED_B PB1
#define LED_C PB2
#define LED_D PB3

#define LED_MASK ((1 << LED_A) | (1 << LED_B) | (1 << LED_C) | (1 << LED_D))

#define BLINK_DELAY_MS 500

int main(void)
{
    /*
     * DDRB = Data Direction Register for Port B.
     * If a bit in DDRB is 1, that pin becomes OUTPUT.
     * If a bit in DDRB is 0, that pin becomes INPUT.
     *
     * Here we make PB0, PB1, PB2, PB3 outputs.
     */
    DDRB |= LED_MASK;

    while (1)
    {
        /*
         * Turn ON all 4 LEDs.
         * PORTB controls the output value of Port B pins.
         * Writing 1 to a bit makes that output pin HIGH.
         */
        PORTB |= LED_MASK;

        _delay_ms(BLINK_DELAY_MS);

        /*
         * Turn OFF all 4 LEDs.
         * ~LED_MASK creates 0s only at the LED bit positions.
         * AND operation clears those LED bits without affecting other pins.
         */
        PORTB &= ~LED_MASK;

        _delay_ms(BLINK_DELAY_MS);
    }

    return 0;
}