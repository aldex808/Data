/*
 * ADC.h
 *
 * Created: 10.01.2018 11:29:24
 *  Author: Администратор2
 */ 


#ifndef ADC_H_
#define ADC_H_
#include <avr/io.h>
#include <avr/interrupt.h>
//#define F_CPU 8000000UL
//#include <util/delay.h>

//unsigned int  adc_counter;
//unsigned int voltage;
//volatile unsigned long voltage_value;
// Включаем АЦП
//#define Adc_on ADCSRA |= (1 << ADEN)|(1 << ADSC)|(1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0)|(1 << ADIE);

//#define Adc_off ADCSRA = 0; // Выключаем АЦП
 
//void Adc_init();
//int Adc_measure();
#define F_LINE 50 // 50 Герц измеряем
#define N_IZM 200 // 200 выборок за период
#define F_MEASURE  F_LINE * N_IZM
// устанавливаем частоту на 1 таймер без пределителя 50*200 Hrz
#define TMR1_CALCULATE 65536 - (F_CPU/F_MEASURE)
#define ADC_VREF_TYPE 0x40
#define U_OPORN 5000 // напряжение опоры в милливольтах
#define  KF 131

//
volatile struct {
	unsigned char cCounter;
	unsigned int iArr[N_IZM];
	unsigned int iRez;
	char cReady;
}sMeasure;

//unsigned long lRez;
//unsigned char lcd_buffer[16];
//unsigned int  Urms=0;
void fn_SetMeas(void);
unsigned int read_adc(unsigned char adc_input);
unsigned int fn_Calculate(void);
unsigned int sqrt_ll(unsigned long long Val);
unsigned int lsqrt(unsigned  long arg);
void Adc_rms_init();


#endif /* ADC_H_ */