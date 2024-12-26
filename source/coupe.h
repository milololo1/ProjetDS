#pragma once

#include <nds.h>
#include <stdio.h>

#include "Graphics.h"
#include "jeu.h"

#define	TEMPS_MIN_COUPE 150
#define	TEMPS_MAX_COUPE 1500

typedef struct {
	int tileX;
	int tileY;
	int touchedPosition[16*16];
	int compteur;
} objectif_coupe;

/*
 * mini_jeu_coupe:
 * lance le mini_jeu_coupe selon le statut (status) d'un jeu global
 */
void mini_jeu_coupe(game_status* status);
