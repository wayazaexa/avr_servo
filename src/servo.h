#ifndef _SERVO_H_
#define _SERVO_H_

#define PORT_1	PORTD
#define PIN_1	PD5
#define DDR_1	DDRD

#define PORT_2	PORTD
#define PIN_2	PD6
#define DDR_2	DDRD

void init_servo(void);
void servo1_set_percentage(signed char percentage);
void servo2_set_percentage(signed char percentage);
 
#endif /* _SERVO_H_ */
