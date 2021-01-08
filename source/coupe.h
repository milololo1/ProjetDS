#pragma once

#include <nds.h>
#include <stdio.h>

#include "Graphics.h"
#include "jeu.h"

typedef struct {
	int tileX;
	int tileY;
	int touchedPosition[16*16];
	int compteur;
} objectif_coupe;

void mini_jeu_coupe(game_status* status);
