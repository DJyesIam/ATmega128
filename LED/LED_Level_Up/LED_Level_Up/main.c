// 스위치를 누를 때마다 켜지는 LED의 개수를 하나씩 늘리는 프로그램

#include <avr/io.h>

void delay(unsigned int millisec){	// ms 단위로 딜레이를 주는 함수
	volatile int i,j;
	for (i = 0; i < millisec; i++)
	for (j = 0; j < 1000; j++);
}

int main(void)
{
	// DDRA |= 0x0F;	// PORTA의 하위 네 비트를 출력 상태(1)로 만든다.
	DDRA |= ((1<<PORTA0) | (1<<PORTA1) | (1<<PORTA2)| (1<<PORTA3));
	// 위와 같은 코드. 핀이 나중에 바뀌었을 때 16진수를 16진수를 계산할 필요가 없다. 다른 부분에도 적용 가능
	PORTA &= !0x0F;	// PORTA의 하위 네 비트를 0으로 초기화한다.
	DDRB &= !0x01;	// PORTB의 0번 핀을 입력 상태(0)로 만든다.
	PORTB |= 0x01;	// PORTB의 0번째 핀을 1로 만든다.(내부 풀업 저항 사용 목적)
	
	uint8_t level = 0x00;	// 스위치를 누를 때마다 바뀔 레벨 변수
	
	while (1)
	{
		if (!(PINB & 0x01)){	// 스위치가 눌리면
			if (level != 0x0F){	// LED가 다 켜져 있지 않으면
				level <<= 1;	// 켜지는 LED를 한 칸씩 이동시킨다.
				level++;		// 가장 말단의 LED를 킨다.
			}
			else level &= 0xF0;	// LED가 다 켜져 있으면 LED를 다 끈다.
			delay(200);
		}
		PORTA = (PORTA & 0xF0) | (level & 0x0F);	// PORTA의 상위 네 비트는 원래 값으로 유지하면서 level 변수의 하위 4비트만 취함
	}
}

