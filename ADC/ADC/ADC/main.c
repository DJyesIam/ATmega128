#include <avr/io.h>
#include <avr/interrupt.h>

#define TC0_PRESCALE_128 (1<<CS00 | 1<<CS02)
#define TC0_INTERRUPT_ENABLE 1<<TOIE0

#define ADC_AREF_CH0 64
#define ADC_PRESCALE_128 (1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2)
#define ADC_ENABLE (1<<ADEN)
#define ADC_INTERRUPT_ENABLE (1<<ADIE)
#define ADC_FREE_RUN (1<<ADFR)
#define ADC_START (1<<ADSC)

void disp_digit(unsigned char, unsigned char);
void disp_FND(unsigned int);

unsigned char digit[10] = {0x88, 0xBE, 0xC4, 0xA4, 0xB2, 0xA1, 0x83,0xBC, 0x80, 0xB0};
volatile unsigned int count=0, ad_val=0;

ISR(TIMER0_OVF_vect) {
	count++;
	TCNT0=130;
}
ISR(ADC_vect) {
	ad_val=ADC;
}

int main(void)
{
	DDRC = 0xff; DDRG = 0x0f;
	
	TCCR0 |= TC0_PRESCALE_128; // Prescale 128 --> 8us
	TIMSK |= TC0_INTERRUPT_ENABLE; // Interrupt Enable
	TCNT0=130; // 8us x (255-130)= 1ms
	
	ADMUX= ADC_AREF_CH0;
	ADCSRA |= ADC_PRESCALE_128 | ADC_ENABLE; // prescale 128
	ADCSRA |= ADC_INTERRUPT_ENABLE |ADC_FREE_RUN;
	ADCSRA |= ADC_START; // Start !!
	
	sei();
	
    while (1) 
    {
		disp_FND(ad_val);
    }
}

void disp_digit(unsigned char num, unsigned char d){
	PORTC = digit[num];
	PORTG = 0x01<<d; 
}

void disp_FND(unsigned int val){
	unsigned char num;
	if ((count % 10)==0){
	num = (val) % 10; disp_digit(num, 3); }
	if ((count % 10)==2){
	num = (val/10) % 10; disp_digit(num, 2); }
	if ((count % 10)==4){
	num = (val/100) % 10; disp_digit(num, 1); }
	if ((count % 10)==6){
	num = (val/1000) % 10; disp_digit(num, 0); }
}

