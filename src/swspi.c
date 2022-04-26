#include "stm8s_conf.h"
#include "swspi.h"

#define CLK_HIGH 			GPIO_WriteHigh(CLK_GPIO, CLK_PIN)
#define CLK_LOW 			GPIO_WriteLow(CLK_GPIO, CLK_PIN)
#define DATA_HIGH 		GPIO_WriteHigh(DATA_GPIO, DATA_PIN)
#define DATA_LOW 			GPIO_WriteLow(DATA_GPIO, DATA_PIN)
#define CS_HIGH 			GPIO_WriteHigh(CS_GPIO, CS_PIN)
#define CS_LOW 				GPIO_WriteLow(CS_GPIO, CS_PIN)



// Odesílá 16bit dat MSB first, SPI mode, sama provádí manipulaci s CS
void max7219_posli(uint8_t adresa, uint8_t data){
uint8_t maska; // pomocná proměnná, která bude sloužit k procházení dat bit po bitu
CS_LOW; // nastavíme linku LOAD/CS do úrovně Low (abychom po zapsání všech 16ti bytů mohli vygenerovat na CS vzestupnou hranu)

// nejprve odešleme prvních 8bitů zprávy (adresa/příkaz)
maska = 0b10000000; // lepší zápis je: maska = 1<<7
CLK_LOW; // připravíme si na CLK vstup budiče úroveň Low
while(maska){ // dokud jsme neposlali všech 8 bitů
	if(maska & adresa){ // pokud má právě vysílaný bit hodnotu 1
		DATA_HIGH; // nastavíme budiči vstup DIN do úrovně High
	}
	else{ // jinak má právě vysílaný bit hodnotu 0 a...
		DATA_LOW;	// ... nastavíme budiči vstup DIN do úrovně Low
	}
	CLK_HIGH; // přejdeme na CLK z úrovně Low do úrovně High, a budič si zapíše hodnotu bitu, kterou jsme nastavili na DIN
	maska = maska>>1; // rotujeme masku abychom v příštím kroku vysílali nižší bit
	CLK_LOW; // vrátíme CLK zpět do Low abychom mohli celý proces vysílání bitu opakovat
}

// poté pošleme dolních 8 bitů zprávy (data/argument)
maska = 0b10000000;
while(maska){ // dokud jsme neposlali všech 8 bitů
	if(maska & data){ // pokud má právě vysílaný bit hodnotu 1
		DATA_HIGH; // nastavíme budiči vstup DIN do úrovně High
	}
	else{ // jinak má právě vysílaný bit hodnotu 0 a...
		DATA_LOW;	// ... nastavíme budiči vstup DIN do úrovně Low
	}
	CLK_HIGH; // přejdeme na CLK z úrovně Low do úrovně High, a v budič si zapíše hodnotu bitu, kterou jsme nastavili na DIN
	maska = maska>>1; // rotujeme masku abychom v příštím kroku vysílali nižší bit
	CLK_LOW; // vrátíme CLK zpět do Low abychom mohli celý proces vysílání bitu opakovat
}

CS_HIGH; // nastavíme LOAD/CS z úrovně Low do úrovně High a vygenerujeme tím vzestupnou hranu (pokyn pro MAX7219 aby zpracoval náš příkaz)
}
