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

	printf("timer setup, let's go on attend\n");

	while(counter0 < secondes){
		swiWaitForVBlank();
		//printf("alerte");
	}

	irqDisable(IRQ_TIMER0);

	counter0 = 0;
}

//permet de mettre un counter pendant un minijeu
void AnnexeCounter(int dsecondes, game_status status){ //dixième de secondes

	int counter1 = 0;

	TIMER_DATA(1) = TIMER_FREQ_1024(10);
	TIMER1_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
	irqSet(IRQ_TIMER1, &ISR_increment1);
}
