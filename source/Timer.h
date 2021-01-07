#pragma once

#include <nds.h>
#include "jeu.h"

void Timer_init();

void Attendre(int secondes);

void AnnexeCounter(int csecondes, game_status* status);
