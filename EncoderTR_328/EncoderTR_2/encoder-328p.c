/*
 * encoder_tr.c
 * Chip Atmega328
 * Created: 05.03.2020 15:21:03
 *  Author: aldex
 */ 


//  Управляем темброблоком с помощью энкодера
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/eeprom.h>

#include "button_lib.h"
#include "1637.h"
#include "ADC.h"
#include "Timer_init.h"
//============== Назначение выводов ======================
#define LED_STATUS	PD5 //D10

#define RELAY_K9		PD7//D9
#define MAX_VOLT		255
#define MAX_VOLT10		250
#define MIN_VOLT	     0
#define TRUE			1
#define FALCE			!TRUE
//=============================================================== Настройка экодера ========================================
//Константы
#define EncoderDDR DDRD                        /*порт настройки*/
#define EncoderPORT PORTD                    /*порт управления*/
#define EncoderPIN PIND                        /*порт чтения*/
#define EncoderF1 (1<<0)                    /*пины чтения енкодера*/
#define EncoderF2 (1<<2)                    /*пины чтения енкодера*/
//=================================== переменные для энкодера ============================================
//Глобалльные переменные
unsigned int EncoderValue = 0;

enum { State0, StateA, StateB, StateAB };     //состояния энкодера
	void To_Do_Step_Up();
	void To_Do_Step_Dn();

//Переменные
static char EncPrev;                          //предыдущее состояние энкодера
static char EncPrevPrev;                      //пред-предыдущее состояние энкодера


volatile unsigned char voltage_v,flag, button = 0;
unsigned int  Urms=0;
unsigned long lRez_long=0;

//=========================================== инициализация экодера =======================
//Инициализация энкодера
void EncoderInit(void)
{
	EncoderDDR &= ~(EncoderF1 | EncoderF2); //настройка портов на ввод
	EncoderPORT |= EncoderF1 | EncoderF2;   //включение подтягивающих резисторов
	EncPrev = State0;                       //инициализация предыдущего состояния
	EncPrevPrev = State0;                   //инициализация пред-предыдущего состояния
	
}
//=============================================================================================
//================================== Функция обработки энкодера ========================
//Обработка энкодера
void EncoderExe(void)
{
	char EncCur = 0;

	if(! (EncoderPIN & EncoderF1))
	EncCur  = StateA; //опрос фазы 1 энкодера

	if(! (EncoderPIN & EncoderF2))
	EncCur |= StateB; //опрос фазы 2 энкодера

	if(EncCur != EncPrev) {                    //если состояние изменилось,
		if(EncPrev == StateAB && EncCur != EncPrevPrev ){//если предыдущее состояние StateAB и текущее и пред-предыдущее не равны,
			if(EncCur == StateB) 
			{ 
				
				 To_Do_Step_Up();
			}             //если текущее состояние StateB,
			
			
			else
			{
				            //шаг вниз
				
				To_Do_Step_Dn();
			}
			
			
		}
		EncPrevPrev = EncPrev;              //сохранение пред-предыдущего состояния
		EncPrev = EncCur;                   //сохранение предыдущего состояния
	}
}
//===========================================================================
//=======================================    =================================
void To_Do_Step_Up()
{
	
	if(EncoderValue<MAX_VOLT10 && flag == FALCE) EncoderValue+=10;
	 if(EncoderValue<MAX_VOLT && flag == TRUE) EncoderValue++;
	
}
void To_Do_Step_Dn()
{
	
	if(EncoderValue > MIN_VOLT && flag == FALCE) EncoderValue-=10 ;
	 if(EncoderValue > MIN_VOLT && flag == TRUE) EncoderValue--;
}
//======================================================================================

//===================================================================================================================
int main (void)
{
	// Порты ввода/вывода
	DDRB = 0xFF; // Выходы
	PORTB = 0x00;
	DDRD |=  (1 << LED_STATUS) | (1 << RELAY_K9); // Входы //PD6 PD7 LED_STATUS Выходы
	//PORTD |= (1 << ENC_A)|(1 << ENC_B); //| (1 << KEY_ED); // Подключаем подтягивающие резисторы
	
	// Таймер 0
	//TCCR0B |= (1 << CS00); // Тактирование Т0 без предделителя
	//TIMSK0 |= (1 << TOIE0); // Разрешаем прерывание по переполнению Т0
	
	// ================================Внешние прерывания=========================
//	MCUCR |= (1 << ISC01); // Прерывание по заднему фронту INT0(по спаду импульса)
//	EIFR |= (1 << INTF0); // Очищаем флаг внешнего прерывания
//	EIMSK |= (1 << INT0);  // Разрешаем внешние прерывания INT0
	
	//===========================настройка таймера на прерывание с частотой 100Гц=====================
	OCR2A = F_CPU / 1024 / 100;					//формула для получения частоты 100Гц
	TIMSK2 |= (1<<OCIE2A);						//прерывание при совпадении
	TCCR2B = (1<<CS22)| (1<<CS21)| (1<<CS20);	//предделитель на 1024
	TCCR2A = (1<<WGM21)| (0<<WGM20);				//режим CTC (сброс при совпадении)
	//=========================================
	Timer_init();
	EncoderInit();
	Adc_rms_init();
	fn_SetMeas();
	BtnInit();
	Urms = 0;
	sei(); // Глобально разрешаем прерывания
	
	while(1)
	{
		
		 
		if(!sMeasure.cReady)

		{
			//Rashet_rms();
			sMeasure.iRez= fn_Calculate();
			lRez_long=(unsigned long)sMeasure.iRez * U_OPORN;
			lRez_long*=KF;
			lRez_long+=512; // снижаем ошибку целочисленного деления
			lRez_long/=1024; // делим в последнюю очередь
			lRez_long+=500;// снижаем ошибку целочисленного деления
			Urms=(unsigned int)(lRez_long/1000); // переводим в вольты
			
			//set_number(Urms);//   вывод значения в вольтах АЦП
			//set_char_at('U', 0);//вывод символа в оределенное знакоместо
			indication(Urms, 0x62);
			//
			_delay_ms(10);
			
			fn_SetMeas();
		}
		
		uint8_t button1 = BtnGet();
		//if((PIND & (1 << BUTTON_ENC)) == 0) // Если нажата кнопка энкодера
		if (button1 == BTN_SHRT2)
		{
			//PORTD |= (1 << PD7);
			//button++;
			if(button++>= 2) {
			 button = 1;
			 
			}
		}
			//while((PIND & (1 << BUTTON_ENC)) == 0){} // Ждем отпускания кнопки
			//_delay_ms(50);
			switch(button) {
				
		case 1:	 PORTD |= (1 << LED_STATUS); // 
			 PORTD |= (1 << RELAY_K9);
			// clear_screen();
			 TIMSK1 &=~ (1<<TOIE1);//Выключение АЦП
			 flag = FALCE;
			
			EncoderExe();
			
			 voltage_v =  EncoderValue;
			 if(voltage_v > 255) EncoderValue = 255;
				indication(voltage_v, 0xf3);
				_delay_ms(10);
			//button = 0;
			break;		
			
			//else PORTD &= ~(1 << PD7);
		case 2:
			 PORTD &=~ (1 << LED_STATUS); //
			 flag = TRUE;
			 
			EncoderExe();
			 voltage_v =  EncoderValue;
			 indication(voltage_v, 0xf3);
			_delay_ms(10);
			
			break;
			default:
			break;
			}
		
			if (button1 == BTN_LONG2)
			{
				
				PORTD &=~(1 << LED_STATUS);
				flag =FALCE;
				
				indication(voltage_v,0x60);
				//voltage_v = 255;
				PORTB = voltage_v;
				_delay_ms(3000);
				PORTD &=~(1 << RELAY_K9);
				TIMSK1 |= (1<<TOIE1);//запуск АЦП
				indication(Urms, 0x62);
			button = 0;
			_delay_ms(100);
			
			}
			
		
		
	}//  while
}// main

ISR (TIMER2_COMPA_vect)							//interrupt 100hz
{
	BtnExe();
	//PORTD ^=( 1 << LED_STATUS );
}
 