#pragma once

#include <nds.h>
#include <stdio.h>

#include "Graphics.h"
#include "jeu.h"

#define	TEMPS_MIN 150
#define	TEMPS_MAX 1500

typedef struct {
	int tileX;
	int tileY;
	int touchedPosition[16*16];
	int compteur;
} objectif_coupe;

void mini_jeu_coupe(game_status* status);
