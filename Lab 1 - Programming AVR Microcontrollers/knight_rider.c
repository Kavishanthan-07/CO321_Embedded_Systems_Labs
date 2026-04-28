#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

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

#define STEP_DELAY_MS 250

int main(void)
{
    /*
     * Set PB0, PB1, PB2, PB3 as output pins.
     */
    DDRB |= LED_MASK;

    /*
     * Sequence needed:
     * A B C D C B
     *
     * We do not add A at the end because when the loop repeats,
     * it automatically starts again from A.
     *
     * Repeating this array gives:
     * A B C D C B A B C D C B ...
     */
    uint8_t sequence[] = {
        LED_A,
        LED_B,
        LED_C,
        LED_D,
        LED_C,
        LED_B
    };

    while (1)
    {
        for (uint8_t i = 0; i < sizeof(sequence) / sizeof(sequence[0]); i++)
        {
            /*
             * First clear only the LED pins.
             * Then turn ON only the selected LED.
             */
            PORTB = (PORTB & ~LED_MASK) | (1 << sequence[i]);

            _delay_ms(STEP_DELAY_MS);
        }
    }

    return 0;
}