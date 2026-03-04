#include <util/delay.h>
#include "servo.h"

int main(void) {
    init_servo();
    sei();

    while(1) {
        servo1_set_percentage(100);
        _delay_ms(1000);

        servo1_set_percentage(50);
        _delay_ms(1000);

        servo1_set_percentage(0);
        _delay_ms(1000);

        servo1_set_percentage(-50);
        _delay_ms(1000);
 
        servo1_set_percentage(-100);
        _delay_ms(1000);
    }
    return 0;
}
