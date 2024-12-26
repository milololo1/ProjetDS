#pragma once

#include <nds.h>
#include <stdio.h>

#include "jeu.h"

//Variables invariantes du jeu du ballon
#define MAX_OBSTACLES 100
#define OBJECTIF_TILEX 28
#define OBJECTIF_TILEY 2
#define	BALLON_HITBOX_WIDTH	16
#define	BALLON_HITBOX_HEIGHT 16
#define	BALLON_STARTX 16
#define	BALLON_STARTY 8*20
#define	TEMPS_MIN_BALLON 200
#define	TEMPS_MAX_BALLON 2000


//pointeur sur le sprite ballon
u16* gfx_ballon;

//Struct des obstacles
typedef struct {
	int tileX;
	int tileY;
	struct obstacle* next;
} obstacle;


/*
 * mini_jeu_ballon:
 * lance le mini_jeu_ballon selon le statut (status) d'un jeu global
 */
void mini_jeu_ballon(game_status* status);
