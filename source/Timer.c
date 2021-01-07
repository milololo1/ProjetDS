#include "Timer.h"
#include "jeu.h"

int counter0 = 0;
int counter1 = 0;

void ISR_increment0(){
	counter0++;
}

void ISR_increment1(){
	++counter1;
}

//à utiliser quand on veut attendre (et rien faire d'autre)
void Attendre(int secondes){

	//irqInit();
	counter0 = 0;

	TIMER_DATA(0) = TIMER_FREQ_1024(1);
	TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
	irqSet(IRQ_TIMER0, &ISR_increment0);

	irqEnable(IRQ_TIMER0);

	while(counter0 < secondes){
		swiWaitForVBlank();
	}

	irqDisable(IRQ_TIMER0);

	counter0 = 0;
}

//permet de mettre un counter pendant un minijeu
void AnnexeCounter(int csecondes, game_status* status){ //centième de secondes

	int counter1 = 0;

	TIMER_DATA(1) = TIMER_FREQ_1024(100);
	TIMER1_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
	irqSet(IRQ_TIMER1, &ISR_increment1);

	while(counter1 < csecondes){
		swiWaitForVBlank();
		status->minigame_current_time = status->minigame_current_total_time - counter1;
	}

	irqDisable(IRQ_TIMER1);

	counter1 = 0;

}
