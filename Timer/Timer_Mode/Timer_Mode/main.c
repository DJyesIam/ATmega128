#include <avr/io.h>
#include <avr/interrupt.h>

typedef enum {READY, RUN, PAUSE, RERUN} CSTATE;

void disp_digit(unsigned char, unsigned char);
void initialize(void);

unsigned char digit[10] = {0x88, 0xBE, 0xC4, 0xA4, 0xB2, 0xA1, 0x83, 0xBC, 0x80, 0xB0};
volatile unsigned int count = 0, tick = 0;
CSTATE curState = READY;

ISR(TIMER0_OVF_vect){
	tick++;
	if ((curState == RUN) || (curState == RERUN)) count++;
	TCNT0 = 6;
}

ISR(INT4_vect){
	switch(curState){
		case READY:
			curState = RUN; break;
		case RUN:
			curState = PAUSE; break;
		case PAUSE:
			curState = RERUN; break;
		case RERUN:
			curState = READY; break;
	}
}

int main(void)
{
	unsigned char num;
	initialize();	sei();
	
	while (1)
	{
		if ((tick % 10)==0){ //1자리
			num = (count/10)%10;
			disp_digit(num, 3);
		}
		if ((tick % 10)==2){ //10자리
			num = (count/100)%10;
			disp_digit(num, 2);
		}
		if ((tick % 10)==4){ //100자리
			num = (count/1000)%10;
			disp_digit(num, 1);
		}
		if ((tick % 10)==8){ //1000자리
			num = (count/10000)%10;
			disp_digit(num, 0);
		}
	}
}

void disp_digit(unsigned char num, unsigned char d){
	PORTC = digit[num];
	PORTG = 0x01 << d;
}

void initialize(void){
	DDRC = 0xff;
	DDRG = 0x0f;
	DDRE &= ~(0x01 << PORTE4); PORTE |= 0x01 << PORTE4;
	EICRB |= (1 << ISC41);	// 하강엣지
	EIMSK |= (1 << INT4);	// EI 인터럽트 생성
	TCCR0 |= (1 << CS02);	// Prescale 64 -> 4us
	TIMSK |= (1 << TOIE0);	// 타이머 인터럽트
	TCNT0 = 6;				// 4us x (255-6) = 1ms
}