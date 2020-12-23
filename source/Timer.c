#include "Timer.h"

int counter = 0;

void ISR_increment(){
	counter++;
}

void Attendre(int secondes){

	TIMER_DATA(0) = TIMER_FREQ_1024(1);
	TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
	irqSet(IRQ_TIMER0, &ISR_increment);

	irqEnable(IRQ_TIMER0);
	while(counter < secondes){
		swiWaitForVBlank();
	}
	counter = 0;
	irqDisable(IRQ_TIMER0);
}

void Timer_init(){

}
