// FND로 타이머를 구현하는 프로그램

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
uint8_t digit[10] = {0x88, 0xBE, 0xC4, 0xA4, 0xB2, 0xA1, 0x83, 0xBC, 0x80, 0xB0};
uint8_t digi_sel[4] = {0x01, 0x02, 0x04, 0x08}, fnd[4];


int main() {
	uint16_t count=0;
	DDRC = 0xff; DDRG= 0x0f;
	while (1) {
		count++;
		if (count == 10000) count = 0;
		fnd[0] = (count/1000)%10;
		fnd[1] = (count/100)%10;
		fnd[2] = (count/10)%10;
		fnd[3] = count%10;
		for (int i=0; i<4; i++){
			PORTC = digit[fnd[i]];
			PORTG = digi_sel[i];
			_delay_ms(2);
			if (i%2) _delay_ms(1);
		}
	}
}

