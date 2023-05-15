// =============== ȸ�� ���� ================
// FND : C0 ~ C7, G0 ~ G3
// Led : A0
// Button : D0
// Ultrasonic Sensor : Trig - E6 , Echo - E7
// ==========================================

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED PORTA0
#define BUTTON PORTD0
#define TRIG PORTE6
#define ECHO PORTE7

enum CSTATE {RUN, PAUSE} curState = RUN;	// ���� ���°� RUN�̸� �Ÿ��� �����ϰ�, PAUSE�� �Ÿ��� �������� �ʴ´�.

ISR(INT0_vect);
ISR(TIMER0_OVF_vect);
void disp_digit(unsigned char num, unsigned char d);
void disp_FND(unsigned int val);
void initialize();

unsigned char digit[10] = {0x88, 0xBE, 0xC4, 0xA4, 0xB2, 0xA1, 0x83, 0xBC, 0x80, 0xB0};		// FND ���� �迭
volatile unsigned int dt = 0;	// ECHO���� HIGH�� �ð� (���� : us)
double distance;				// ������ ������ �Ÿ�	(���� : cm)
double filtered_distance;		// Low Pass Filter�� ���͸��� �Ÿ� (���� : cm)

int main(void)
{
	initialize();		
    while (1) 
    {
		switch (curState){
			case RUN:	// RUN �����̸� LED�� �Ѱ� ������ ������ �Ÿ��� �����Ͽ� FND�� ǥ���Ѵ�.
				PORTA |= (1 << LED);			
			
				PORTE &= ~(1 << TRIG);	_delay_us(2);	// TRIG pin LOW
				PORTE |=  (1 << TRIG);  _delay_us(10);	// TRIG pin HIGH
				PORTE &= ~(1 << TRIG);					// TRIG pin LOW

				while(!(PINE & (1 << ECHO)));			// ECHO���� HIGH�� �� ������ ��ٸ���.

				dt = 0;
				TCCR0 |= 1 << CS01;						// ECHO���� HIGH�� �ð��� ��� �����Ѵ�.
				while(PINE & (1 << ECHO));				// ECHO���� HIGH�� ���� ��ٸ���.
				TCCR0 = 0;								// �ð� ������ ������ Timer�� ����.
				
				distance = dt / 58.2 * 3;				// �Ÿ� ���ϴ� ���� (���� �ڿ� *3 �� ���� �� �´µ� �Ÿ��� �̻��ϰ� ���ͼ� ���Ƿ� ����)
				
				filtered_distance = 0.99 * filtered_distance + 0.01 * distance;	// Low Pass Filter�� �Ÿ��� ���͸��Ѵ�. (�ΰ��� : 0.01)
				
				if (filtered_distance > 31)	{PORTC = 0; PORTG = 0;}	// �������� 30cm�� �ʰ��ϸ� 0�� ����Ѵ�.
				else disp_FND((int)filtered_distance);		// �ƴ϶�� FND�� �Ÿ��� ����Ѵ�.
				
				break;
				
			case PAUSE:		// PAUSE �����̸� FND�� LED�� ���� ������ �����.
				PORTA &= ~(1 << LED);	
				PORTC = 0;
				PORTG = 0;
				break;
		}
    }
}

void disp_digit(unsigned char num, unsigned char d){
	PORTC = digit[num];
	PORTG = 0x01 << d;
}

void disp_FND(unsigned int num){
	unsigned char fnd[4];
	
	fnd[0] = (num / 1000) % 10;		// õ�� �ڸ� ��
	fnd[1] = (num / 100)  % 10;		// ���� �ڸ� ��
	fnd[2] = (num / 10)   % 10;		// ���� �ڸ� ��
	fnd[3] = (num / 1)    % 10;		// ���� �ڸ� ��
	
	for (int i = 0; i < 4; i++){	// ��ȿ�� ���� �ִ� �ڸ����� ����Ѵ�.
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
	PORTD |= (1 << PORTD0);		// ���� Pull-Up
	EICRA |= (1 << ISC01);		// �ϰ� ����
	EIMSK |= (1 << INT0);		// EI ���ͷ�Ʈ Ȱ��
	
	DDRE |= (1 << TRIG);		// TRIG�� ���
	DDRE &= ~(1 << ECHO);		// ECHO�� �Է�
	
	TCCR0 |= (1 << CS01);		//	Prescale 8 -> 0.5us
	TIMSK |= (1 << TOIE0);		//  Interrupt Enable
	TCNT0 = 254; 				//	0.5us x (256 - 254) = 1us
	sei();
}

ISR(INT0_vect) {				// ��ư�� ���� ������ ���¸� �ٲ۴�.
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