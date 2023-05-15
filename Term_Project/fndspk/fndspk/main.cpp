
// =============== 회로 정보 ================
// FND : C0 ~ C7, G0 ~ G3
// Led : A0
// Button : D0
// Ultrasonic Sensor : Trig - E6 , Echo - E7
// ==========================================

#define F_CPU 16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__
#define HALF_CYC 500000UL
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#define LED PORTA0
#define BUTTON PORTD0
#define TRIG PORTE6
#define ECHO PORTE7
#define SPK PORTB

enum CSTATE {RUN, PAUSE} curState = RUN;	// 현재 상태가 RUN이면 거리를 측정하고, PAUSE면 거리를 측정하지 않는다.

ISR(INT0_vect);
ISR(TIMER0_OVF_vect);
void disp_digit(unsigned char num, unsigned char d);
void disp_FND(unsigned int val);
void initialize();

unsigned char digit[10] = {0x88, 0xBE, 0xC4, 0xA4, 0xB2, 0xA1, 0x83, 0xBC, 0x80, 0xB0};		// FND 숫자 배열
volatile unsigned int dt = 0;	// ECHO핀이 HIGH인 시간 (단위 : us)
double distance;				// 센서로 측정한 거리	(단위 : cm)
double filtered_distance;		// Low Pass Filter로 필터링한 거리 (단위 : cm)

//추가
void tone(int,int);
void beep();
int getdist();
int dist;
//


int main(void)
{
	DDRB |= 1;
	initialize();
	while (1)
	{
		switch (curState){
			case RUN:	// RUN 상태이면 LED를 켜고 초음파 센서로 거리를 측정하여 FND에 표시한다.
			PORTA |= (1 << LED);
			
			beep();
						
			break;
			
			case PAUSE:		// PAUSE 상태이면 FND와 LED를 끄고 측정을 멈춘다.
			PORTA &= ~(1 << LED);
			PORTC = 0;
			PORTG = 0;
			break;
		}
	}
}


int getdist(){
			PORTE &= ~(1 << TRIG);	_delay_us(2);	// TRIG pin LOW
			PORTE |=  (1 << TRIG);  _delay_us(10);	// TRIG pin HIGH
			PORTE &= ~(1 << TRIG);					// TRIG pin LOW

			while(!(PINE & (1 << ECHO)));			// ECHO핀이 HIGH가 될 때까지 기다린다.

			dt = 0;
			TCCR0 |= 1 << CS01;						// ECHO핀이 HIGH인 시간을 재기 시작한다.
			while(PINE & (1 << ECHO));				// ECHO핀이 HIGH인 동안 기다린다.
			TCCR0 = 0;								// 시간 측정이 끝나면 Timer를 끈다.
			
			distance = dt / 58.2 * 3;				// 거리 구하는 공식 (원래 뒤에 *3 은 없는 게 맞는데 거리가 이상하게 나와서 임의로 붙임)
			
			filtered_distance = 0.99 * filtered_distance + 0.01 * distance;	// Low Pass Filter로 거리를 필터링한다. (민감도 : 0.01)
			int dd = (int)filtered_distance;
			
			if (filtered_distance > 31)	{SPK = 0; PORTC = 0; PORTG = 0; dd = 0;}	// 센서값이 30cm을 초과하면 0을 출력한다.
			else disp_FND(dd);		// 아니라면 FND에 거리를 출력한다.
			
			return dd;
}
void tone(){
	int fr = 500;
	uint32_t t_tot=HALF_CYC/fr;
	int t_ms=t_tot/1000, t_us=t_tot % 1000, i;
	for (i=0; i<20; i++){
		SPK |= 1;
		_delay_ms(t_ms); _delay_us(t_us);
		SPK &= ~1;
		_delay_ms(t_ms); _delay_us(t_us);
	}
}

void beep(){
	dist = getdist();
	if(dist>0) tone();
	for(int i=0; i<100;i++){
		SPK |= 0;
		_delay_us(dist*dist -6*dist + 9);
		dist = getdist();
		
	}
}

void disp_digit(unsigned char num, unsigned char d){
	PORTC = digit[num];
	PORTG = 0x01 << d;
}

void disp_FND(unsigned int num){
	unsigned char fnd[4];
	
	fnd[0] = (num / 1000) % 10;		// 천의 자리 수
	fnd[1] = (num / 100)  % 10;		// 백의 자리 수
	fnd[2] = (num / 10)   % 10;		// 십의 자리 수
	fnd[3] = (num / 1)    % 10;		// 일의 자리 수
	
	for (int i = 0; i < 4; i++){	// 유효한 값이 있는 자릿수만 출력한다.
		if (fnd[i] == 0){
			if (i == 0 || fnd[i-1] == 0) continue;
			else{
				disp_digit(fnd[i], i);
				_delay_ms(2);
			}
		}
		else{
			disp_digit(fnd[i], i);
			_delay_ms(2);
		}
	}
}

void initialize(){
	DDRC = 0xff; DDRG = 0x0f;	// FND
	
	DDRA |= (1 << LED);			// LED
	
	DDRD  |= (1 << BUTTON);		// BUTTON
	PORTD |= (1 << PORTD0);		// 내부 Pull-Up
	EICRA |= (1 << ISC01);		// 하강 에지
	EIMSK |= (1 << INT0);		// EI 인터럽트 활성
	
	DDRE |= (1 << TRIG);		// TRIG핀 출력
	DDRE &= ~(1 << ECHO);		// ECHO핀 입력
	
	TCCR0 |= (1 << CS01);		//	Prescale 8 -> 0.5us
	TIMSK |= (1 << TOIE0);		//  Interrupt Enable
	TCNT0 = 254; 				//	0.5us x (256 - 254) = 1us
	sei();
}

ISR(INT0_vect) {				// 버튼이 눌릴 때마다 상태를 바꾼다.
	switch (curState){
		case RUN:
		curState = PAUSE;
		break;
		case PAUSE:
		curState = RUN;
		break;
	}
}

ISR(TIMER0_OVF_vect) {
	dt++;
	TCNT0 = 254;				// 0.5us x (256 - 254) = 1us
}