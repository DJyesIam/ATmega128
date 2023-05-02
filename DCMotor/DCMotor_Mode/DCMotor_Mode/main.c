#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define IN1 PB7
#define IN2 PB4

#define Use_IE0_Interrupt (EIMSK |= (1<<INT0))
#define IE0_Detect_Falling (EICRA = (1<<ISC01))

#define TC0_PRESCALE_32 (1<<CS00 | 1<<CS01)
#define TC0_FAST_PWM (1<<WGM00| 1<<WGM01)
#define TC0_NONINVERT_PWM (1<<COM01)

void initialize();
unsigned char vel;
enum {STOP, CW, STOP2, CCW} mode ;
	
ISR(INT0_vect) {
	if (mode==STOP)		mode = CW;
	if (mode==CW)		mode = STOP2;
	if (mode==STOP2)	mode = CCW;
	if (mode==CCW)		mode = STOP;
}	

int main(void)
{
	mode = STOP;
	vel = 100;
	initialize();
	
	while (1){
		switch (mode){
			case STOP:
			case STOP2:
				PORTB &=~(1<<IN1);
				OCR0=0;
				break;
			case CW:
				PORTB|=(1<<IN1);
				vel=0.3*255; // 70%
				OCR0=255-vel;
				break;
			case CCW:
				PORTB &=~(1<<IN1);
				vel=0.4*255; // 40%
				OCR0=vel;
				break;
		}
	}
}

void initialize(){
	DDRB=0xff; DDRD=0x00; PORTD|=0x01;
	OCR0 = 0;
	TCCR0 |= TC0_FAST_PWM | TC0_NONINVERT_PWM | TC0_PRESCALE_32; 
	EICRA |= IE0_Detect_Falling; // Falling Edge
	EIMSK |= (1<<INT0); // Use Interrupt
	sei();	
}
