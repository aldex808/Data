/*
 *	FITOLAMP with OFF TIMER 
 * ATTINY2313 8 MHz  OUT PB2
 *		WS2812B
 * Created: 22.03.2020 12:41:39
 *  Author: aldex80
 */ 
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "light_ws2812.h"
/*------------------------------------------------*/
// Количество светодиодов в ленте 1 - 36
#define MAX_PIXEL 36

struct cRGB led[MAX_PIXEL];

volatile int cnt_timer;
volatile int cnt_minutes;
unsigned char mode = 0;

/******************************************************************/
#define ACT_BUTT_LEV    0       // Активный уровень нажатых кнопок
//---------------------
#define PORT_BUTT_1     PINB    // Порт кнопки MODE
#define PIN_BUTT_1      0       // Номер бита порта кнопки MODE
//---------------------
#define PORT_LED_1      PORTB   // Светодиоды
#define DDR_LED_1       DDRB    //
#define PIN_LED_1       1       //
#define PORT_LED_2      PORTB   //
#define DDR_LED_2       DDRB    //
#define PIN_LED_2       3      //
#define PORT_LED_4      PORTB   // Светодиоды
#define DDR_LED_4       DDRB    //
#define PIN_LED_4       4       //
/******************************************************************/
//				Переменные для кнопки				
/******************************************************************/
typedef struct{
	unsigned cur:   1;      // Текущее состояние
	unsigned prev:  1;      // Предыдущее состояние
	unsigned down:  1;      // Была нажата
	unsigned up:    1;      // Была отжата
}tButt;
volatile tButt  mButt_1 = {0,!ACT_BUTT_LEV,0,0};    // Объявляем структуру mButt_1 для нашей кнопки и иним её.
/******************************************************************/
/* Timer 1 Overflov Interrupt             */
/******************************************************************/
ISR(TIMER1_OVF_vect){
	
	//PORT_LED_4 ^=(1<<PIN_LED_4);
	
	cnt_timer++;
	if(cnt_timer==10000) //  Счетчик переполнений 
	{
		cnt_minutes++; // Счетчик примерно минут
		//PORT_LED_4 ^=(1<<PIN_LED_4);
		cnt_timer = 0;
	}
	//===================== Опрос кнопок ===========================================
	static unsigned char cnt_ms_butt=20;    // Счётчик для формирования периода в 20 мс.
	TCNT1 -= 4000;          // Перезапускаем таймер на 1 мс.
	//----------------------//
	if(!--cnt_ms_butt){     // Формируем 20-ти миллисек. периоды
		cnt_ms_butt=20;
		mButt_1.cur = (PORT_BUTT_1&(1<<PIN_BUTT_1))!=0;                 // Считываем текущее значение вывода.
		// Дальше работаем с этим битом, т.к. порт может измениться в любой момент.
		if((mButt_1.cur==ACT_BUTT_LEV) && (mButt_1.prev!=ACT_BUTT_LEV)) // Если текущее значение - "нажата" и предыдущее - "не нажата"
		mButt_1.down = 1;                                                   // Устанавливаем флаг down
		if((mButt_1.cur!=ACT_BUTT_LEV) && (mButt_1.prev==ACT_BUTT_LEV)) // Если текущее значение - "не нажата" и предыдущее - "нажата"
		mButt_1.up = 1;                                                     // Устанавливаем флаг up
		mButt_1.prev=mButt_1.cur;                                       // Сохраняем текущее значение. Оно для следующего входа будет предыдущим
	}
	//----------------------//
}
/************************ Конец опроса и прерывания ******************************************/
void TimerInit()
{
	 TCCR1B = _BV(CS10);	// Таймер работает на полной частоте системы
	 TIMSK |= _BV(TOIE1) ;	// Включить прерывания
	 TCNT1 = 0;
}
void PortInit()
{
	 PORT_LED_1  &=~(1<<PIN_LED_1);
	 DDR_LED_1   |=(1<<PIN_LED_1);
	 PORT_LED_2  &=~(1<<PIN_LED_2);
	 DDR_LED_2 |=(1<<PIN_LED_2);
	 PORT_LED_4  &=~(1<<PIN_LED_4);
	 DDR_LED_4 |=(1<<PIN_LED_4);
	 DDRB &=~(1<<PIN_BUTT_1);
	 PORTB |= (1<<PIN_BUTT_1); // Кнопка "MODE"
	 PORT_LED_4 &=~(1<<PIN_LED_4);
}

//*************************** ФУНКЦИИ ПОДГОТОВКИ ВЫВОДА НА ЛЕНТУ *************************
void Indication4_WS2812()// Вывод розового оттенка
{
		for(int i=0; i<MAX_PIXEL ; i++ )
	{
		led[i].r=255;led[i].g=0;led[i].b=255;    // Write pink to array
		// ws2812_setleds(led,NUM_PIXEL);
		// _delay_ms(500);
	}
}

void Indication3_WS2812() // Вывод синего оттенка
{
	for(int i=0; i<MAX_PIXEL ; i++ )
	{
		led[i].r=0;led[i].g=0;led[i].b=255;    // Write pink to array
		// ws2812_setleds(led,NUM_PIXEL);
		// _delay_ms(500);
	}
}

void Indication2_WS2812() // Вывод белого оттенка 
{
	for(int i=0; i<MAX_PIXEL ; i++ )
	{
		led[i].r=255;led[i].g=255;led[i].b=255;    // Write pink to array
		// ws2812_setleds(led,NUM_PIXEL);
		// _delay_ms(500);
	}
}


void Indication_WS2812() // Вывод розово-синего оттенка
{
	 for(int i=0; i<MAX_PIXEL ; i++ )
	 {
		 led[i].r=255;led[i].g=00;led[i].b=255;    // Write pink to array
		 // ws2812_setleds(led,NUM_PIXEL);
		 // _delay_ms(500);
	 }
	 for(int i=0; i<MAX_PIXEL; i+=2 )
	 {
		 led[i].r=0;led[i].g=00;led[i].b=255;    // Write blue to array
		 // ws2812_setleds(led,NUM_PIXEL);
	 }
}
void Reset_Leds() // Очистить ленту
{
	for(int p=0; p<MAX_PIXEL ; p++ )
	{
		led[p].r=0;led[p].g=00;led[p].b=0;    // Write empty to array
		// ws2812_setleds(led,NUM_PIXEL);
		// _delay_ms(500);
	}
	 ws2812_setleds(led,MAX_PIXEL); // Загружаем на ленту массив
}
//================================ Конец Функций =============================
int main(void)
{
	 TimerInit();
	 PortInit();
	 mode = 1; // Включаемся
	 sei();
 //============================= Главный Цикл ===============================  
    while(1)
    {
       
	  if(mButt_1.down){   // Если кнопка была  нажата
		  mButt_1.down=0;  // Сбрасываем флаг
		 // PORT_LED_2 ^=(1<<PIN_LED_2);    // Инвертируем светодиод №2
		  mode ++; // Задаем режимы по кругу
	  }
	//========================== РЕЖИМЫ РАБОТЫ =======================  
	 if(mode == 1)
	 {
		  Reset_Leds();
		// PORT_LED_1 &=~(1<<PIN_LED_1);
		 PORT_LED_1 |=(1<<PIN_LED_1);
		 PORT_LED_2 &=~(1<<PIN_LED_2);	// Переключаем светодиоды
	 }
	 if(mode == 2)
	 {
		  Indication_WS2812();
		//  mode = 0;
		//  PORT_LED_1 |=(1<<PIN_LED_1);
		  PORT_LED_1 &=~(1<<PIN_LED_1);	// Переключаем светодиоды
		  PORT_LED_2 |=(1<<PIN_LED_2);
		  //PORT_LED_4 &=~(1<<PIN_LED_4);
			cnt_timer = 0;	// Перезапускаем таймер    
	 }
	  if(mode == 3)
	  {
		 // mode = 0;
		  Indication2_WS2812();
		   cnt_timer = 0;	// Перезапускаем таймер
	  }
	  
	   if(mode == 4)
	   {
		  // mode = 0;
		   Indication3_WS2812();
		    cnt_timer = 0;	// Перезапускаем таймер
	   }
		 if(mode == 5)
		 {
			 mode = 0;
			 Indication4_WS2812();
			  cnt_timer = 0;	// Перезапускаем таймер
		 }
		//  PORT_LED_4 |=(1<<PIN_LED_4);
		//  _delay_ms(200);
		//  PORT_LED_4 &=~(1<<PIN_LED_4);
		//  cnt_minutes++;
		//  cnt_timer = 0;
		 if(cnt_minutes == 1500) // Время вышло гасим ленту
		 {
			 cnt_minutes = 0;
			 cnt_timer = 0;	// Перезапускаем таймер
			  Reset_Leds();
			 PORT_LED_1 |=(1<<PIN_LED_1);
			 mode = 0;
		 }
		// cli();
		 ws2812_setleds(led,MAX_PIXEL); // Загружаем на ленту массив
		// sei();
	  
        } // END LOOP
  }// END MAIN
        
        
     