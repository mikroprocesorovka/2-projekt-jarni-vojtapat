#include "stm8s.h"
#include "milis.h"
#include "stm8_hd44780.h"
#include "stdio.h"
#include "keypad.h" 
#include "swspi.h"

void process_keypad(void);
void max_init(void);
void max_posli(void);


uint16_t lasttime=0;

void main(void){
CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // 16MHz z interního RC oscilátoru
GPIO_Init(GPIOB,GPIO_PIN_7,GPIO_MODE_OUT_PP_LOW_SLOW);
GPIO_Init(GPIOG,GPIO_PIN_3,GPIO_MODE_IN_PU_NO_IT);
init_milis();
keypad_init();
max_init();
max_posli();

  while (1){
		process_keypad(); // obsluhujeme klávesnici
		if(GPIO_ReadInputPin(GPIOG,GPIO_PIN_3)==RESET){
			GPIO_WriteHigh(GPIOB,GPIO_PIN_7);
		}
	}
}

// pravidelně sleduje stav klávesnice a reaguje na stisknutí kláves
void process_keypad(void){
static uint8_t minule_stisknuto=0;	// ukládáme poslední stav klávesnice
static uint16_t last_time=0; 
uint16_t stisknuto;

	if(milis()-last_time > 20){ // každých 20ms ...
		last_time = milis();
		stisknuto=keypad_scan(); // ... skenujeme klávesnici
		if(minule_stisknuto == 0 && stisknuto != 0){ // pokud byla minule uvolněná a teď je něco stisknuto
			minule_stisknuto = stisknuto;
			// zachytili jsme stisk klávesy - můžeme na to zareagovat
			
			if(stisknuto < 10){     
					max7219_posli(DIGIT0, stisknuto); 
				}
			}
			else if(stisknuto == 15){ 
				GPIO_WriteHigh(GPIOB,GPIO_PIN_7);
			}
			else if(stisknuto == 16){ 
				GPIO_WriteLow(GPIOB,GPIO_PIN_7);
			}
		}			
		if(stisknuto == 0){minule_stisknuto=0;}
	}
void max_init(void){
GPIO_Init(CS_GPIO,CS_PIN,GPIO_MODE_OUT_PP_LOW_SLOW);
GPIO_Init(CLK_GPIO,CLK_PIN,GPIO_MODE_OUT_PP_LOW_SLOW);
GPIO_Init(DATA_GPIO,DATA_PIN,GPIO_MODE_OUT_PP_LOW_SLOW);
// nastavíme základní parametry budiče
max7219_posli(DECODE_MODE, DECODE_ALL); // zapnout znakovou sadu na všech cifrách
max7219_posli(SCAN_LIMIT, 7); // velikost displeje 8 cifer (počítáno od nuly, proto je argument číslo 7)
max7219_posli(INTENSITY, 5); // volíme ze začátku nízký jas (vysoký jas může mít velkou spotřebu - až 0.25A !)
max7219_posli(DISPLAY_TEST, DISPLAY_TEST_OFF); // 
max7219_posli(SHUTDOWN, DISPLAY_ON); // zapneme displej
}

void max_posli(void){
max7219_posli(DIGIT0,8);
max7219_posli(DIGIT1,7);
max7219_posli(DIGIT2,6);
max7219_posli(DIGIT3,5);
max7219_posli(DIGIT4,4);
max7219_posli(DIGIT5,3);
max7219_posli(DIGIT6,2);
max7219_posli(DIGIT7,1);
}

// pod tímto komentářem nic neměňte 
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */                                                
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */                               
  while (1)
  {
  }
}
#endif
