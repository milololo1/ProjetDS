#pragma once

#include <nds.h>
#include "jeu.h"


void Timer_init();

void ISR_increment1();

void Attendre(int secondes);

void AnnexeCounter(game_status* status);
