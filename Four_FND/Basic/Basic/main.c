// 네 자리 FND에 0,1,2,3을 출력하는 프로그램

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

uint8_t digit[10] = {0x88, 0xBE, 0xC4, 0xA4, 0xB2, 0xA1, 0x83, 0xBC, 0x80, 0xB0};
uint8_t digi_sel[4] = {0x01, 0x02, 0x04, 0x08};

int main() {
	int i=0;
	DDRC = 0xff; DDRF = 0x0f; 
	
	while (1) {
		for (i = 0; i < 4; i++){
			PORTC = digit[i];
			PORTF = digi_sel[i];
			_delay_ms(2);
		}
	}
}

