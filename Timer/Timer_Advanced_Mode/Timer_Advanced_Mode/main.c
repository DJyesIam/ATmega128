#include <avr/io.h>
#include <avr/interrupt.h>

enum CSTATE{READY, RUN, PAUSE, RERUN} curState=READY;
	
void disp_digit(unsigned char, unsigned char);
void initialize(void);

unsigned char digit[10] = {0x88, 0xBE, 0xC4, 0xA4, 0xB2, 0xA1, 0x83, 0xBC, 0x80, 0xB0};
volatile unsigned int count=0, tick=0, cur_count=0, prev_count=0;

ISR(TIMER0_COMP_vect) {
	cur_count++;
	switch (curState){
		case READY: break;
		case RUN: case RERUN:
		{ tick++; count++; break; }
		case PAUSE:
		{ tick++; break; }
	}
}

ISR(INT4_vect) {
	if (cur_count - prev_count > 100){
		prev_count = cur_count;
		switch(curState){
			case READY: // 대기 모드
			{ curState = RUN; break; }
			case RUN:	// 런 모드
			{ curState = PAUSE; break; }
			case PAUSE: // 일시정지 모드
			{ curState = RERUN; count = tick; break; }
			case RERUN: // 재가동 모드
			{ curState = READY; count=0; tick=0; break; }
		}
	}
}

int main(void) {
	unsigned char num;
	initialize();
	sei();
	
	while (1){
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
	PORTG = 0x01<<d;
}

void initialize(void){
	DDRC = 0xff;
	DDRG = 0x0f;
	DDRE = 0x00; PORTE|=1<<PORTE4; // 내부풀업
	EICRA |= (1<<ISC41); // 하강에지
	EIMSK |= (1<<INT4); // EI 인터럽트 활성
	TCCR0 |= 1<<CS02; // Prescale 64 -> 4us
	TIMSK |= 1<<OCIE0; // 타이머 인터럽트
	OCR0 = 250; // 4us x 250 = 1ms
}