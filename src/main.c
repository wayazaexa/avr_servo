#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>
#include "servo.h"

#define BUTTON_PIN PB1

#define BIT_SET(PORT, PIN) (PORT |= (1 << PIN))
#define BIT_CLEAR(PORT, PIN) (PORT &= ~(1 << PIN))
#define BIT_FLIP(PORT, PIN) (PORT ^= (1 << PIN))
#define BIT_CHECK(PORT, PIN) (PORT & (1 << PIN))
#define BUTTON_IS_CLICKED(PORT, PIN) !BIT_CHECK(PORT, PIN)

volatile unsigned char buttonTickVector_PB1 = 0;

typedef enum {
    Click_status_Pressed = 0,
    Click_status_Released = 1,
    Click_status_Not_sure = 2
} Click_status;

Click_status clicked(unsigned char tick_vector) {
    // Create a mask with the 5 lower bits set to 1 (0b00011111 = 31)
    unsigned char mask = 31;

    if ((tick_vector & mask) == mask) {
        return Click_status_Pressed; // 5 senaste värden är höga
    }
    else if ((tick_vector & mask) == 0) {
        return Click_status_Released; // 5 senaste värden är låga
    }
    else {
        return Click_status_Not_sure;
    }
}

ISR(TIMER2_OVF_vect)
{
    TCNT2 = 128; // Timer Preloading
    bool sample = BUTTON_IS_CLICKED(PINB, BUTTON_PIN);

    buttonTickVector_PB1 = (buttonTickVector_PB1 << 1) | sample;
}

void timer2_init()
{
    TCCR2A = 0;   // Init Timer2A
    TCCR2B = 0;   // Init Timer2B
    TCCR2B |= 7;  // Prescaler = 1024
    TCNT2 = 128;  // Timer Preloading
    TIMSK2 |= 1;  // Enable Timer Overflow Interrupt
}

int main(void) {
    init_servo();
    timer2_init();
    sei();

    // Sätt till input med pull-up resistor
    BIT_CLEAR(DDRB, BUTTON_PIN);
    BIT_SET(PORTB, BUTTON_PIN);

    bool isClicked = false;
    bool canRelease = false;

    servo1_set_percentage(100);
    
    while (1) {
        Click_status status = clicked(buttonTickVector_PB1);

        if (status == Click_status_Pressed) {
            canRelease = true;
        }

        if (canRelease && status == Click_status_Released) {
            isClicked = true;
            canRelease = false;
        }

        if (isClicked) {
            servo1_set_percentage(0);
            _delay_ms(1000);
            servo1_set_percentage(100);
            isClicked = false;
        }
    }
    return 0;
}
