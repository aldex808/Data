/*
 * ADC.c
 *
 * Created: 10.01.2018 11:29:01
 *  Author: Администратор2
 */ 

// Обработчик прерывания от АЦП
#include "ADC.h"
//#include "display.h"

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
//#include "adc_rms.h"

//-----------------------------------------------
ISR (TIMER1_OVF_vect)
{
	//PORTB^=(1<<PB7);
	TCNT1+=TMR1_CALCULATE;
	if(sMeasure.cReady)
	{
		sMeasure.iArr[sMeasure.cCounter]= read_adc(0);
		sMeasure.cCounter++;
		if(sMeasure.cCounter >= N_IZM){sMeasure.cReady=0;}
	}
}


/*
Во время прихода положительной полуволны запускается АЦП, Который успевает за время прохождения этой полуволны сделать порядка 100 выборок, которые обрабатываются по формуле:
Urms= sqrt(1/T* S[u(t)*u(t)]dt,
где: sqrt - корень квадратный, S - интеграл за период времени от 0 до Т.
Для дискретного способа взятия интеграла формула упроститься до вида
Urms=K*sqrt(1/N*sum(Uadc*Uadc)),
Где: К - поправочный коэффициент, Uadc - значение на выходе АЦП, N - число выборок за время прохождения полуволны синусоиды.
*/


void fn_SetMeas(void)
{
	sMeasure.cCounter=0;
	sMeasure.cReady=1;
}
//------------------------------------------------
// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
	ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
	// Delay needed for the stabilization of the ADC input voltage
	_delay_us(10);
	// Start the AD conversion
	ADCSRA|=0x40;
	// Wait for the AD conversion to complete
	while ((ADCSRA & 0x10)==0);
	ADCSRA|=0x10;
	return ADCW;
}
//---------------------------------------d---------
unsigned int fn_Calculate(void)
{
	unsigned long lRez=0;
	unsigned long lKv;
	unsigned char i;
	for(i=0;i<N_IZM;i++)
	{
		lKv=(unsigned long)sMeasure.iArr[i]*(unsigned long)sMeasure.iArr[i];
		lRez+=lKv;
	}
	lRez/=N_IZM;
	//return sqrt_ll(lRez);
	return lsqrt(lRez);
}
//------------------------------------------------
/*============================================================================*/
// корень квадратный
unsigned int sqrt_ll(unsigned long long Val)
{
	unsigned long long Root;
	unsigned long long tmp;

	tmp  = 0x80000000;              // MaxRoot/2
	Root = 0;                 // start value

	while(tmp)
	{
		if( ((Root+tmp) * (Root+tmp)) <= Val )
		Root+=tmp;
		tmp=tmp>>1;            // /2
	}
	return (Root);           // result
}


void Adc_rms_init()
{
	// ADC initialization
	// ADC Clock frequency: 1000,000 kHz
	// ADC Voltage Reference: AVCC pin
	ADMUX=ADC_VREF_TYPE & 0xff;
	ADCSRA=0x83;
}
unsigned int lsqrt(unsigned  long arg){
	char count=16;
	unsigned long  res=0,tmp=0;
	if(arg!=0){
		if(!(arg&0xFF000000)){arg<<=8;count-=4;}


		res=1;
		while((tmp<1)&&(count)){
			count--;
			if(arg&0x80000000UL)tmp|=2;
			if(arg&0x40000000UL)tmp|=1;

			arg<<=2;


		};//поиск первой 1-ы
		tmp--;
		//for(count;count;count--)
		while(count--)
		{
			tmp<<=2;
			res<<=1;

			if(arg&0x80000000UL)tmp|=2;
			if(arg&0x40000000UL)tmp|=1;
			arg<<=2;

			if( tmp>=((res<<1)|1)){
				tmp-=((res<<1)|1);
				res|=1;
			}
			
		}
	}
	return (unsigned int)res;
}
	
//ISR(ADC_vect)
//{
	//PORTB ^=(1<<PB6);
	//Adc_off; // Выключаем АЦП
	//if((ADMUX & 0x0F)==1) // Если был выбран канал ADC1
	//{
	//	voltage_value = voltage_value + ADC; // Суммируем измеренные значения напряжения и помещаем в буфер
	//	ADMUX = (ADMUX & 0xF0) | 0; // Выбираем канал ADC0
	//}
	//else
	//{
		//current_value = current_value + ADC; // Суммируем измеренные значения тока и помещаем в буфер
	//	ADMUX = (ADMUX & 0xF0) | 1; // Выбираем канал ADC1
	//	adc_counter++; // Увеличиваем счетчик выборок АЦП на 1
	//}
	// Включаем АЦП
	//Adc_on;
//}
	//void Adc_init()
	//{
	
	
		
	//}
	//int Adc_measure()
	//{
	

		

			//adc_counter = 0; // Обнуляем счетчик выборок АЦП
			//voltage_value = 0; // Обнуляем буфер значений напряжения
	
			// Выводим данные на LCD
	
			// Включаем АЦП
			
		//}

