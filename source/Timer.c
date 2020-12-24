#include "Timer.h"
#include "jeu.h"

//à utiliser quand on veut attendre (et rien faire d'autre)
void Attendre(int secondes){

	int counter = 0;

	void ISR_increment(){
		counter++;
	}

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

//permet de mettre un counter pendant un minijeu
void AnnexeCounter(int dsecondes, game_status status){ //dixième de secondes

	int counter = 0;

	void ISR_increment(){
		++counter;
	}

	TIMER_DATA(1) = TIMER_FREQ_1024(10);
	TIMER1_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
	irqSet(IRQ_TIMER1, &ISR_increment);
}
