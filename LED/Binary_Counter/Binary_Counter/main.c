// 스위치를 누를 때마다 1씩 증가되는 2진 카운트

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
	
	uint8_t count = 0x00;	// 스위치를 누를 때마다 증가할 카운트 변수
	
	while (1)
	{	
		if (!(PINB & 0x01)){	// 스위치가 눌리면
			count++;
			delay(200);
		}
		PORTA = (PORTA & 0xF0) | (count & 0x0F);	// PORTA의 상위 네 비트는 원래 값으로 유지하면서 count 변수의 하위 4비트만 취함
	}
}

