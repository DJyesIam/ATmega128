#include <avr/io.h>
#include <stdint.h>
#define F_CPU 16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#define HALF_CYC 500000UL

#include <avr/interrupt.h>
#define Use_IE0_Interrupt (EIMSK |= (0x01 << INT0))		// PORTD0를 인터럽트 핀으로 사용
#define IE0_Detect_Falling EICRA &= ~(0x01 << ISC00)	// 하강엣지
#define IE0_Detect_Rising (EICRA |= ((0x01 << ISC01) | (0x01 << ISC00)))	// 상승엣지

unsigned int i = -1;	// 멜로디 배열의 인덱스
int nextMelody = 1;	// 스피커를 끌지를 결정하는 변수

// 음 주파수 정의
#define DO 277 // SHARP
#define RE 294
#define MI 330
#define FA 370 // SHARP
#define SOL 415 // SHARP
#define LA 440
#define SI 494
#define DDO 554 // SHARP
#define RRE 587
#define MMI 659
#define FFA 740 // SHARP
#define SSOL 831 // SHARP
#define LLA 880
#define SSI 988

// 멜로디와 길이를 저장하는 배열 
unsigned int melody[] = {
	DDO, DDO, DDO, SI, DDO, SI,
	LA,
	LLA, LLA, SSOL, DDO, FFA,
	FFA, FFA, MMI, RRE,
	DDO, SI, LA,
	LA, SI, SI, MMI,
	DDO, DDO, DDO, SI, DDO, SI,
	LA,
	LLA, LLA, SSOL, DDO, FFA,
	FFA, FFA, MMI, RRE,
	DDO, SI, LA,
	FA, LA, SOL, LA, SI, LA,
	DDO, RRE, DDO, FFA,
	SSOL, LLA, SSOL, DDO,
	FFA, FFA, MMI, RRE, DDO,
	SI, DDO, RRE, MMI,
	LLA, SSOL, FFA,
	LLA, LLA, LLA, SSOL, FFA, MMI,
	MMI, LLA, SSOL, LLA, SSOL, FFA,
	FFA, FFA, FFA, MMI, FFA, MMI, DDO,
	LA, LA, SI, SI, MMI, DDO, LA,
	FA, LA, SOL, LA, SI, LA,
	FA, LA, SOL, LA, SI, LA
};

void initialize(void);
void beep(int pin, uint16_t on_time, uint16_t off_time);
void tone3(uint16_t fr, uint16_t dur);

ISR(INT0_vect){
	if ((EICRA & (0x01 << ISC00)) == 0){	// 만약 스위치가 눌리면
		i++;	// 인덱스를 +1
		IE0_Detect_Rising;	
		nextMelody = 0;	// nextMelody를 0으로
	}
	else {	// 만약 스위치가 떼지면
		IE0_Detect_Falling;
		nextMelody = 1;	// nextMelody를 1로
	}
}

int main(void)
{
	initialize();
	while (1) if (nextMelody == 1) tone3(melody[i], 1000);
}

void initialize(void){
	DDRD &= ~(1 << PORTD0);	PORTD |= 1 << PORTD0;	// 스위치
	DDRB |= 1 << PORTB0;	// 스피커
	
	Use_IE0_Interrupt;
	IE0_Detect_Falling;
	sei();
}

void beep(int pin, uint16_t on_time, uint16_t off_time){
	PORTB |= 1 << PORTB0;
	_delay_us(on_time);
	PORTB &= ~(1 << PORTB0);
	_delay_us(off_time);
}

void tone3(uint16_t fr, uint16_t dur){
	uint16_t on_time = 1./fr*500000.;
	uint16_t n = dur*1000./(2.*on_time);
	for (int i = 0; i < n; i++){
		if (nextMelody == 1) return; // nextMelody가 1이면(스위치가 떼지면) tone함수를 탈출한다.(스피커를 중간에 끈다.)
		beep(0, on_time, on_time);
	}
}

