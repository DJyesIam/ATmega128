#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <math.h>

#define IN1 PB7
#define IN2 PB4
#define TC0_PRESCALE_32 (1<<CS00 | 1<<CS01)
#define TC0_FAST_PWM (1<<WGM00| 1<<WGM01)
#define TC0_NONINVERT_PWM (1<<COM01)

void initialize();

int main(void)
{
	int count=0;
	double speed, t;
	unsigned char vel;
	initialize();
	
    while (1){
		t = (double)count / 180. * 3.141592;
		speed = 255 * sin(t);
		vel = (unsigned int)fabs(speed);
		
		if (speed>=0){
			PORTB|=(1<<IN1);
			OCR0=255-vel;
		}
		else{
			PORTB &=~(1<<IN1);
			OCR0=vel;
		}
		count++;
		_delay_ms(20);
	}
}

void initialize(){
	DDRB |= (1 << IN1) | (1 << IN2);
	TCCR0 |= TC0_FAST_PWM | TC0_NONINVERT_PWM | TC0_PRESCALE_32;
}
